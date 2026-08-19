/* umdevxs_peekpoke.c
 *
 * Program to interactively use UMDevXS Kernel Driver services.
 */

/*****************************************************************************
* Copyright (c) 2009-2013 INSIDE Secure B.V. All Rights Reserved.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 2 of the License, or
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

// get definitions for MAP1/2/3_BAR/OFS/LEN
#include "cs_umdevxspeekpoke.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>                  // isprint
#include <pthread.h>                // pthread_create
#include <sys/time.h>               // gettimeofday
#include <signal.h>                 // sigaction
#include <stdlib.h>                 // exit
#include <unistd.h>                 // usleep

#include "umdevxsproxy.h"           // init, shutdown
#include "umdevxsproxy_device.h"    // find
#include "umdevxsproxy_shmem.h"

#ifdef UMDEVXSPEEKPOKE_INTERRUPT
#include "umdevxsproxy_interrupt.h"
#endif

static int AttachedTo;      // 0 = not, 1 = device, 2 = shmem
static unsigned int Attached_Size;
static int Attached_DeviceID;
static uint8_t * Attached_Ptr;
static unsigned int LastDumpAddr;
static unsigned int LastDumpCount = 10;
static unsigned int LastScanAddr;
static unsigned int LastScanCount = 10;

#ifdef UMDEVXSPEEKPOKE_SHMEM_PROVIDER
static UMDevXSProxy_SHMem_Handle_t Attached_Handle;
#endif
#ifdef UMDEVXSPEEKPOKE_SHMEM_OBTAINER
static UMDevXSProxy_SHMem_Handle_t Attached_Handle;
#endif

// byte-to-char for hexdump
static char b2c[256];
#ifdef UMDEVXSPEEKPOKE_INTERRUPT
static int ReportInterrupts = 1;
#endif

static int ReadPoll_IsRunning;


/*----------------------------------------------------------------------------
 * PeekPoke_Detach
 */
static void
PeekPoke_Detach(void)
{
    int res = 0;

    if (AttachedTo == 0)
        return;

    if (AttachedTo == 1)
    {
        res = UMDevXSProxy_Device_Unmap(
                  Attached_DeviceID,
                  Attached_Ptr,
                  Attached_Size);
    }

#ifdef UMDEVXSPEEKPOKE_SHMEM_PROVIDER
    if (AttachedTo == 2)
    {
        res = UMDevXSProxy_SHMem_Free(Attached_Handle);
    }
#endif

#ifdef UMDEVXSPEEKPOKE_SHMEM_OBTAINER
    if (AttachedTo == 3)
    {
        res = UMDevXSProxy_SHMem_Detach(Attached_Handle);
    }
#endif

    AttachedTo = 0;
    Attached_Ptr = NULL;
    Attached_Size = 0;
    Attached_DeviceID = 0;

    if (res < 0)
        printf("Failed to unmap resource\n");
}


/*----------------------------------------------------------------------------
 * PeekPoke_Attach
 */
static void
PeekPoke_Attach(
        const char * Name_p)
{
    int res;
    int DeviceID;
    unsigned int Size;

    PeekPoke_Detach();

    //printf("Name='%s'\n", Name_p);

    res = UMDevXSProxy_Device_Find(
              Name_p,
              &DeviceID,
              &Size);

    //printf("UMDevXSProxy_Device_Find returned %d, DeviceID=%d, Size=%u\n", res, DeviceID, Size);

    if (res < 0)
    {
        printf("Failed to attach to resource '%s'\n", Name_p);
        return;
    }

    // add the device memory to our memory map
    {
        void * p;

        p = UMDevXSProxy_Device_Map(DeviceID, Size);

        if (p == NULL)
        {
            printf("Failed to map resource '%s'\n", Name_p);
            return;
        }

        Attached_Ptr = (uint8_t *)p;
        Attached_Size = Size;
        Attached_DeviceID = DeviceID;
        AttachedTo = 1;

        printf("Attached to resource '%s' of size=%u\n", Name_p, Attached_Size);
    }
}


/*----------------------------------------------------------------------------
 * PeekPoke_AttachShMemAlloc
 */
#ifdef UMDEVXSPEEKPOKE_SHMEM_PROVIDER
static void
PeekPoke_AttachShMemAlloc(
        const unsigned int Size)
{
    UMDevXSProxy_SHMem_Handle_t Handle;
    UMDevXSProxy_SHMem_BufPtr_t BufPtr;
    UMDevXSProxy_SHMem_DevAddr_t DevAddr;
    unsigned int BufSize;
    int res;

    PeekPoke_Detach();

    res = UMDevXSProxy_SHMem_Alloc(
                 Size,
                 0,          // dummy Bank
                 4 * 1024,   // Alignment
                 &Handle,
                 &BufPtr,
                 &DevAddr,
                 &BufSize);

    if (res != 0)
    {
        printf("Failed to attach to shared memory block (res %d)\n", res);
        return;
    }

    printf("Attached to shared memory block of size %u\n", BufSize);

    // the GBI format is known here
    // it is Size followed Address both 32bit integers stored LSB-first
    {
        unsigned int W;
        uint8_t GBI[8];

        W = BufSize;
        GBI[0] = 255 & W;
        GBI[1] = 255 & (W >> 8);
        GBI[2] = 255 & (W >> 16);
        GBI[3] = 255 & (W >> 24);

        W = (uintptr_t)DevAddr.p;
        GBI[4] = 255 & W;
        GBI[5] = 255 & (W >> 8);
        GBI[6] = 255 & (W >> 16);
        GBI[7] = 255 & (W >> 24);

        printf("GBI: %02X %02X %02X %02X %02X %02X %02X %02X\n",
            GBI[0], GBI[1], GBI[2], GBI[3],
            GBI[4], GBI[5], GBI[6], GBI[7]);
    }

    Attached_Ptr = (uint8_t *)BufPtr.p;
    Attached_Handle = Handle;
    Attached_Size = BufSize;
    AttachedTo = 2;
}
#endif /* UMDEVXSPEEKPOKE_SHMEM_PROVIDER */


/*----------------------------------------------------------------------------
 * PeekPoke_MapShMem
 */
#ifdef UMDEVXSPEEKPOKE_SHMEM_OBTAINER
static void
PeekPoke_MapShMem(
        const unsigned int * EightArgs_p)
{
    int res;
    UMDevXSProxy_SHMem_Handle_t Handle;
    UMDevXSProxy_SHMem_BufPtr_t BufPtr;
    UMDevXSProxy_SHMem_DevAddr_t DevAddr;
    unsigned int Size;
    unsigned int BufSize;

    // sanity-check the input
    {
        int i;
        for (i = 0; i < 8; i++)
            if (EightArgs_p[i] > 255)
            {
                printf("m: Invalid argument: 0x%x\n", EightArgs_p[i]);
                return;
            }
    }

    PeekPoke_Detach();

    printf(
        "GBI: %02X %02X %02X %02X %02X %02X %02X %02X\n",
        EightArgs_p[0], EightArgs_p[1], EightArgs_p[2], EightArgs_p[3],
        EightArgs_p[4], EightArgs_p[5], EightArgs_p[6], EightArgs_p[7]);

    // the GBI format is known here
    // it is Size followed by Address, both 32bit integers stored LSB-first
    {
        unsigned int W;

        W = EightArgs_p[0];
        W |= (EightArgs_p[1] << 8);
        W |= (EightArgs_p[2] << 16);
        W |= (EightArgs_p[3] << 24);
        Size = W;

        W = EightArgs_p[4];
        W |= (EightArgs_p[5] << 8);
        W |= (EightArgs_p[6] << 16);
        W |= (EightArgs_p[7] << 24);

#ifdef UMDEVXSPEEKPOKE_GBI_ADDR_ADD
        W += UMDEVXSPEEKPOKE_GBI_ADDR_ADD;
#endif

        DevAddr.p = (void *)(uintptr_t)W;
    }

    res = UMDevXSProxy_SHMem_Attach(
                      DevAddr,
                      Size,
                      0, // dummy Bank
                      &Handle,
                      &BufPtr,
                      &BufSize);

    if (res != 0)
    {
        printf("Failed to attach to shared memory block (res %d)\n", res);
        return;
    }

    printf("Attached to shared memory block of size %u\n", BufSize);

    Attached_Ptr = (uint8_t *)BufPtr.p;
    Attached_Handle = Handle;
    Attached_Size = BufSize;
    AttachedTo = 3;
}
#endif /* UMDEVXSPEEKPOKE_SHMEM_OBTAINER */


/*----------------------------------------------------------------------------
 * PeekPoke_ShMem_CommitRefresh
 */
#ifdef UMDEVXSPEEKPOKE_SHMEM_COMMON
static void
PeekPoke_ShMem_CommitRefresh(
        const unsigned int SubsetStart,
        const unsigned int SubsetLen,
        const int fCommit)
{
    if (AttachedTo == 2 ||      // allocated
        AttachedTo == 3)        // attached
    {
        if (SubsetStart >= Attached_Size)
        {
            printf("Invalid subset start: 0x%x (max is 0x%x)\n", SubsetStart, Attached_Size);
            return;
        }

        if (SubsetLen >= Attached_Size ||
            SubsetStart + SubsetLen >= Attached_Size)
        {
            printf("Invalid subset end: 0x%x (max is 0x%x)\n", SubsetStart, Attached_Size);
            return;
        }

        if (fCommit)
        {
            if (SubsetStart == 0 && SubsetLen == 0)
                printf("Committing entire buffer\n");
            else
                printf("Committing range 0x%x..0x%x\n", SubsetStart, SubsetStart + SubsetLen);

            UMDevXSProxy_SHMem_Commit(Attached_Handle, SubsetStart, SubsetLen);
        }
        else
        {
            if (SubsetStart == 0 && SubsetLen == 0)
                printf("Refreshing entire buffer\n");
            else
                printf("Refresshing range 0x%x..0x%x\n", SubsetStart, SubsetStart + SubsetLen);

            UMDevXSProxy_SHMem_Refresh(Attached_Handle, SubsetStart, SubsetLen);
        }

        return;
    }

    printf("Not attached to a shared memory block (%d)\n", AttachedTo);
}
#endif /* UMDEVXSPEEKPOKE_SHMEM_COMMON */


/*----------------------------------------------------------------------------
 * PeekPoke_PrintConfig
 */
static void
PeekPoke_PrintConfig(void)
{
    int i;

    printf("Attachable devices:\n");

    for (i = 0; i <= 255; i++)
    {
        char Name[50];
        int res;

        res = UMDevXSProxy_Device_Enum(i, 50, Name);
        if (res != 0)
            break;      // from the for

        // skip empty entries
        if (Name[0])
            printf("   %s\n", Name);
    } // for
}


/*----------------------------------------------------------------------------
 * PeekPoke_Read
 */
static void
PeekPoke_Read(
        unsigned int addr,
        unsigned int access_size)
{
    if (AttachedTo == 0)
    {
        printf("r: Not attached\n");
        return;
    }

    if (addr >= Attached_Size)
    {
        printf("r: Invalid address! (max = 0x%06x)\n", Attached_Size);
        return;
    }

    switch(access_size)
    {
        case 32:
            {
                uint32_t * p32;
                uint32_t v32;

                if (addr & 3)
                {
                    printf("r: Address not 32bit aligned! (0x%06x)\n", addr);
                    break;
                }

                p32 = (uint32_t *)(Attached_Ptr + addr);
                v32 = *p32;

                printf("r 0x%06x: 0x%08x [%c%c%c%c]\n",
                    addr, v32,
                    b2c[(v32 >> 24) & 255],
                    b2c[(v32 >> 16) & 255],
                    b2c[(v32 >> 8) & 255],
                    b2c[v32 & 255]);
            }
            break;

        case 16:
            {
                uint16_t * p16;
                uint16_t v16;

                if (addr & 1)
                {
                    printf("r: Address not 16bit aligned! (0x%06x)\n", addr);
                    break;
                }

                p16 = (uint16_t *)(Attached_Ptr + addr);
                v16 = *p16;

                printf("r 0x%06x: 0x%04x [%c%c]\n",
                    addr, v16,
                    b2c[(v16 >> 8) & 255],
                    b2c[v16 & 255]);
            }
            break;

        case 8:
            {
                uint8_t * p8;
                uint8_t v8;

                p8 = (uint8_t *)(Attached_Ptr + addr);
                v8 = *p8;

                printf("r 0x%06x: 0x%02x [%c]\n", addr, v8, b2c[v8]);
            }
            break;

        default:
            printf("r: invalid size %d, use 8 or 16 or 32\n", access_size);
            break;
    } // switch
}


/*----------------------------------------------------------------------------
 * PeekPoke_Write
 */
static void
PeekPoke_Write(
        unsigned int addr,
        unsigned int value,
        int access_size)
{
    if (AttachedTo == 0)
    {
        printf("w: Not attached\n");
        return;
    }

    if (addr >= Attached_Size)
    {
        printf("w: Invalid address! (max = 0x%06x)\n", Attached_Size);
        return;
    }

    switch(access_size)
    {
        case 32:
            {
                uint32_t * p32;
                uint32_t v32;

                if (addr & 3)
                {
                    printf("w: Address not 32bit aligned! (0x%06x)\n", addr);
                    break;
                }

                p32 = (uint32_t *)(Attached_Ptr + addr);
                v32 = (uint32_t)value;

                printf("w 0x%06x: 0x%08x\n", addr, v32);

                *p32 = v32;
            }
            break;

        case 16:
            {
                uint16_t * p16;
                uint16_t v16;

                if (addr & 1)
                {
                    printf("r: Address not 16bit aligned! (0x%06x)\n", addr);
                    break;
                }

                p16 = (uint16_t *)(Attached_Ptr + addr);
                v16 = (uint16_t)value;

                printf("r 0x%06x: 0x%04x\n", addr, v16);

                *p16 = v16;
            }
            break;

        case 8:
            {
                uint8_t * p8;
                uint8_t v8;

                p8 = (uint8_t *)(Attached_Ptr + addr);
                v8 = (uint8_t)value;

                printf("w 0x%06x: 0x%02x\n", addr, v8);

                *p8 = v8;
            }
            break;

        default:
            printf("w: invalid size %d, use 8 or 16 or 32\n", access_size);
            break;
    } // switch
}


/*----------------------------------------------------------------------------
 * PeekPoke_Fill
 */
static void
PeekPoke_Fill(
        unsigned int addr,
        unsigned int count,
        unsigned int value,
        unsigned int vstep,
        unsigned int astep)
{
    if (AttachedTo == 0)
    {
        printf("f: Not attached\n");
        return;
    }

    if (addr >= Attached_Size)
    {
        printf("f: Invalid address! (max = 0x%06x)\n", Attached_Size);
        return;
    }

    if (addr & 3)
    {
        printf("f: Address not 32bit aligned! (0x%06x)\n", addr);
        return;
    }

    if (astep & 3)
    {
        printf("f: astep (%d) must be multiple of 4\n", astep);
        return;
    }

    if (addr + count * astep > Attached_Size)
    {
        count = (Attached_Size - addr) / astep;
        printf("f: Limiting to %d words\n", count);
    }

    while(count--)
    {
        uint32_t * p32;
        uint32_t v32;

        p32 = (uint32_t *)(Attached_Ptr + addr);
        v32 = (uint32_t)value;

        printf("w 0x%06x: 0x%08x\n", addr, v32);

        *p32 = v32;

        addr += astep;
        value += vstep;
    } // while
}


/*----------------------------------------------------------------------------
 * PeekPoke_Dump
 */
static unsigned int
PeekPoke_Dump(
        unsigned int addr,
        int count,
        int astep)
{
    uint32_t * p32;
    uint32_t v32;

    if (AttachedTo == 0)
    {
        printf("d: Not attached\n");
        return 0;
    }

    if (addr >= Attached_Size)
    {
        printf("d: Invalid address! (max = 0x%06x)\n", Attached_Size);
        return 0;
    }

    if (addr & 3)
    {
        printf("d: Unaligned address! (0x%06x)\n", addr);
        return 0;
    }

    if (astep & 3)
    {
        printf("d: astep (%d) must be multiple of 4\n", astep);
        return 0;
    }

    if (4 * count + addr > Attached_Size)
    {
        count = (Attached_Size - addr) / 4;
        printf("d: Limiting to %d words\n", count);
    }

    while(count--)
    {
        p32 = (uint32_t *)(Attached_Ptr + addr);
        v32 = *p32;

        printf("r 0x%06x: 0x%08x [%c%c%c%c]\n",
            addr, v32,
            b2c[(v32 >> 24) & 255],
            b2c[(v32 >> 16) & 255],
            b2c[(v32 >> 8) & 255],
            b2c[v32 & 255]);

        addr += astep;
    }

    return addr;
}


/*----------------------------------------------------------------------------
 * PeekPoke_ReadPoll
 */
static void
PeekPoke_ReadPoll(
        unsigned int addr,
        unsigned int timeout,
        unsigned int Delay_ms)
{
    if (AttachedTo == 0)
    {
        printf("p: Not attached\n");
        return;
    }

    if (addr >= Attached_Size)
    {
        printf("p: Invalid address! (max = 0x%06x)\n", Attached_Size);
        return;
    }

    if (addr & 3)
    {
        printf("p: Address not 32bit aligned! (0x%06x)\n", addr);
        return;
    }

    if (Delay_ms > 5000)
    {
        Delay_ms = 5000;
        printf("p: Limiting interval to %u ms\n", Delay_ms);
    }

    printf("Polling address 0x%06x every %u milliseconds, for %u seconds\n", addr, Delay_ms, timeout);
    ReadPoll_IsRunning = 1;

    {
        struct timeval timeStart;
        struct timeval timeNow;
        uint32_t * p32;
        uint32_t v32, v32_prev;

        gettimeofday(&timeStart, NULL);

        p32 = (uint32_t *)(Attached_Ptr + addr);
        v32 = *p32;
        v32_prev = v32 ^ 3;

        do
        {
            if (v32 != v32_prev)
            {
                v32_prev = v32;

                printf("p 0x%06x: 0x%08x [%c%c%c%c]\n",
                    addr, v32,
                    b2c[(v32 >> 24) & 255],
                    b2c[(v32 >> 16) & 255],
                    b2c[(v32 >> 8) & 255],
                    b2c[v32 & 255]);
            }

            if (Delay_ms)
            {
                usleep(Delay_ms * 1000);
            }

            gettimeofday(&timeNow, NULL);

            v32 = *p32;

            // Ctrl+C triggers sig-handler, which clears this global
            if (ReadPoll_IsRunning == 0)
            {
                // stop the poll
                timeout = 0;
                printf("\n");
            }
        }
        while(difftime(timeNow.tv_sec, timeStart.tv_sec) < timeout);
    }

    ReadPoll_IsRunning = 0;
}


/*----------------------------------------------------------------------------
 * PeekPoke_Scan
 */
static unsigned int
PeekPoke_Scan(
        unsigned int addr,
        int count)
{
    unsigned int addr_start = addr;
    unsigned int addr_last = addr;

    if (AttachedTo == 0)
    {
        printf("s: Not attached\n");
        return 0;
    }

    if (addr >= Attached_Size)
    {
        printf("s: Invalid address! (max = 0x%06x)\n", Attached_Size);
        return 0;
    }

    if (addr & 3)
    {
        printf("s: Unaligned address! (0x%06x)\n", addr);
        return 0;
    }

    while(addr < Attached_Size)
    {
        uint32_t * p32 = (uint32_t *)(Attached_Ptr + addr);
        uint32_t v32 = *p32;

        addr_last = addr;
        addr += 4;

        if (v32 != 0xffffffff && v32 != 0)
        {
            printf("Found life at 0x%06x (0x%08x)\n", addr_last, v32);

            count--;
            if (count == 0)
            {
                // next time, start at the next address
                break;
            }
        }
    }

    printf("Scanned addresses 0x%06x .. 0x%06x\n", addr_start, addr_last);

    return addr;
}


/*----------------------------------------------------------------------------
 * PeekPoke_PollThread
 */
#ifdef UMDEVXSPEEKPOKE_INTERRUPT
static void *
PeekPoke_PollThread(
        void * args)
{
    int res;

    // call the UMDevXS Proxy to check for interrupts
    // then print something
    do
    {
        res = UMDevXSProxy_Interrupt_WaitWithTimeout(1000);

        if (res == 0)
        {
            if (ReportInterrupts)
            {
                printf("<interrupt>\n");

                // avoid tons of printouts
                // 100ms = 10x / sec
                //  50ms = 20x / sec
                usleep(50 * 1000);
            }
        }
    }
    while(res >= 0);

    return args;
}
#endif /* UMDEVXSPEEKPOKE_INTERRUPT */


/*----------------------------------------------------------------------------
 * SigHandler_Exit
 *
 * This function is called when Ctrl+C is pressed to terminate the
 * application.
 */
static void
SigHandler_Exit(int signum)
{
    if (ReadPoll_IsRunning)
    {
        ReadPoll_IsRunning = 0;
        return;
    }

    PeekPoke_Detach();
    UMDevXSProxy_Shutdown();

    printf("\nGoodbye! (2)\n");
    exit(0);
}


/*----------------------------------------------------------------------------
 * TransferVerilogFile
 *
 * This function transfers the contents of the Verilog hex file associated
 * with 'istrm_p' to the currently attached device. Note that the (start)
 * address (typically 0) is also read from the Verilog file.
 * Return value: 0 on success, -1 otherwise.
 *
 * Verilog file syntax:
 * // BlaBla                  -- comment line
 * @00000                     -- address line: address = 0x0 here
 * 200010                     -- value for address 0: 0x200010
 * 60ff00                     -- value for address 4: 0x60ff00
 * ...                        -- etc
 */
static int
TransferVerilogFile(
        FILE * istrm_p)
{
    unsigned int addr = Attached_Size;  // ensures 'invalid until provided'
    char * p;
    char buf[128];

    for (p = fgets(buf, sizeof(buf), istrm_p);
         p != NULL;
         p = fgets(buf, sizeof(buf), istrm_p))
    {
        unsigned int value;
        int count;

        // printf("<%s>\n", buf);

        switch (buf[0])
        {
            case '/':
                break;

            case '@':
                count = sscanf(buf + 1, "%x", &addr);
                if ((count != 1) || (addr & 3))
                {
                    printf("Bad address: %x\n", addr);
                    return -2;
                }
                break;

            default:
                // silently ignore empty lines
                if (buf[0] == '\n')
                    break;

                count = sscanf(buf, "%x", &value);
                if (count != 1)
                {
                    printf("Bad hex value: %s\n", buf);
                    return -3;
                }

                if (addr >= Attached_Size)
                {
                    printf("Error: addr=0x%x, Attached_Size=0x%x\n",
                           addr, Attached_Size);
                    return -4;
                }

                PeekPoke_Write(addr, value, 32);
                addr += 4;
                break;
        } // switch
    } // for

    if (!feof(istrm_p))
        return -1;

    return 0;
}


/*----------------------------------------------------------------------------
 * main
 */
int
main(void)
{
    int res;
    int fQuit = 0;

    AttachedTo = 0;

    /* Avoid buffering of printf to simplify controlling 'peekpoke'
       through another program. */
    setbuf(stdout, NULL);

    res = UMDevXSProxy_Init();
    if (res < 0)
    {
        printf(
            "UMDevXSProxy_Init returned %d. "
            "Has the driver been loaded?\n",
            res);
        return 1;
    }

    {
        struct sigaction SA = { 0 };
        SA.sa_handler = SigHandler_Exit;
        res = sigaction(SIGINT, &SA, NULL);
        //printf("sigaction result: %d\n", res);
    }

    for (res = 0; res < 256; res++)
    {
        if (isprint(res))
            b2c[res] = (char)res;
        else
            b2c[res] = '.';
    }

    printf("Welcome to PeekPoke\n\n");


#ifdef UMDEVXSPEEKPOKE_INTERRUPT
    // create the worker thread and provide the arguments (by reference)
    {
        pthread_t ThreadId;
        pthread_create(
                    &ThreadId,
                    /*attr:*/ NULL,
                    PeekPoke_PollThread,
                    /* args */NULL);
    }
#endif

    while(!fQuit)
    {
        char buf[256];
        char prevbuf[256];
        char * CmdStr_p;

        printf("> ");
        CmdStr_p = fgets(buf, 256, stdin);

        //printf("Command received: %s\n", CmdStr_p);

        if (CmdStr_p[0] == '.')
            memcpy(buf, prevbuf, 256);
        else
            if (CmdStr_p[0] != 0)
                memcpy(prevbuf, buf, 256);

        switch(tolower(CmdStr_p[0]))
        {
            case 0:
                // silently ignore empty commands
                break;

            case 'h':
                printf(
                    "Help:\n"
                    " h     Display this message\n"
                    " q     Quit program\n"
                    " .     Repeat last command\n"
                    " c     Print configuration\n"
                    " a name\n"
                    "       Attach to resource with given name\n"
                    " b n   Attach to PCI device with BAR=n\n"
#ifdef UMDEVXSPEEKPOKE_SHMEM_PROVIDER
                    " k n   Allocate and attach to shared memory buffer of size n\n"
#endif
#ifdef UMDEVXSPEEKPOKE_SHMEM_OBTAINER
                    " m gbi Map shared memory buffer based on 8-byte GBI (hex)\n"
#endif
#ifdef UMDEVXSPEEKPOKE_SHMEM_COMMON
                    " o [[subset_start] subset_len]\n"
                    " e [[subset_start] subset_len]\n"
                    "       Commit/Refresh shared memory, or subset (hex)\n"
#endif
                    " s     Scan entire window for 32bit value with 0/1 bitmix\n"
                    " r addr [size]\n"
                    "       Read and print value from addr using size bit access\n"
                    "       addr: address (hex)\n"
                    "       size: access size in bits (dec): 8, 16 or 32. Default=32\n"
                    " w addr value [size]\n"
                    "       Write value (hex) to addr using size bit access\n"
                    " d [addr [cnt [astep]]]\n"
                    "       Read and print from addr using size=32\n"
                    "       cnt: number of reads (dec). Default=10\n"
                    "       astep: (dec) address step, must be multiple of 4\n"
                    " f addr cnt value [vstep [astep]]\n"
                    "       Fill from addr up in step (dec) words\n"
                    "       value: (hex) start value\n"
                    "       vstep: (dec) increment for value\n"
                    "       astep: (dec) address step, must be multiple of 4\n"
#ifdef UMDEVXSPEEKPOKE_INTERRUPT
                    " i     Toggle interrupt reporting on/off\n"
#endif
                    " p addr [sec] [interval]\n"
                    "       Poll addr and print differences\n"
                    "       sec: how long to poll, in seconds. Ctrl+C aborts\n"
                    "       interval: time between reads, in milliseconds\n"
                    " x file\n"
                    "       Transfer contents of Verilog hex file to attached device\n"
                    );
                break;

            case 'q':
                fQuit = 1;
                break;

            case 'a':
                {
                    char Name[50 + 1];
                    int i;
                    char * p = CmdStr_p + 1;

                    while(*p == ' ')
                        p++;

                    for(i = 0; i < 50; i++)
                    {
                        Name[i] = *p++;
                        Name[i + 1] = 0;

                        // abort on eol and cr
                        if (Name[i] < 32)
                        {
                            Name[i] = 0;
                            break;
                        }
                    }

                    if (Name[0] == 0)
                    {
                        printf("a: Too few arguments\n");
                        break;
                    }

                    PeekPoke_Attach(Name);
                }
                break;

            case 'b':
                {
                    int newbar;
                    int argcount;
                    char Name[5+1] = "PCI.0";

                    argcount = sscanf(CmdStr_p+1, "%d", &newbar);

                    if (argcount == 0)
                    {
                        printf("b: Too few arguments\n");
                        break;
                    }

                    if (newbar < 0 || newbar > 9)
                    {
                        printf("Invalid bar: %d\n", newbar);
                        newbar = 1;
                    }

                    Name[4] = (char)('0' + newbar);
                    PeekPoke_Attach(Name);
                }
                break;

            case 'c':
                PeekPoke_PrintConfig();
                break;

            case 'r':
                {
                    int argcount;
                    unsigned int addr = 0;
                    unsigned int size = 0;

                    argcount = sscanf(CmdStr_p+1, "%x %u", &addr, &size);
                    if (argcount == 0)
                    {
                        printf("r: Too few arguments\n");
                        break;
                    }

                    if (argcount < 2)
                        size = 32;

                    PeekPoke_Read(addr, size);
                }
                break;

            case 'p':
                {
                    int argcount;
                    unsigned int addr = 0;
                    unsigned int timeout = 0;
                    unsigned int delay_ms = 0;

                    argcount = sscanf(CmdStr_p+1, "%x %u %u", &addr, &timeout, &delay_ms);
                    if (argcount == 0)
                    {
                        printf("p: Too few arguments\n");
                        break;
                    }

                    if (argcount < 3)
                        delay_ms = 10;

                    if (argcount < 2)
                        timeout = 10;

                    PeekPoke_ReadPoll(addr, timeout, delay_ms);
                }
                break;

            case 's':
                {
                    unsigned int addr = LastScanAddr;
                    unsigned int count = 0;
                    int argcount;

                    argcount = sscanf(CmdStr_p+1, "%x %u", &addr, &count);

                    if (argcount < 2)
                        count = LastScanCount;

                    LastScanAddr = PeekPoke_Scan(addr, count);
                    LastScanCount = count;
                }
                break;

            case 'd':
                {
                    int argcount;
                    unsigned int addr = LastDumpAddr;
                    int count = 0;
                    int astep = 0;

                    argcount = sscanf(CmdStr_p+1, "%x %d %d", &addr, &count, &astep);
                    if (argcount == 0)
                    {
                        printf("d: Too few arguments\n");
                        break;
                    }
                    if (argcount <= 1)
                        count = LastDumpCount;
                    if (argcount <= 2)
                        astep = 4;

                    LastDumpAddr = PeekPoke_Dump(addr, count, astep);
                    LastDumpCount = count;
                }
                break;

            case 'w':
                {
                    int argcount;
                    unsigned int addr = 0;
                    int value = 0;
                    int size = 0;

                    argcount = sscanf(CmdStr_p+1, "%x %x %d", &addr, &value, &size);
                    if (argcount < 2)
                    {
                        printf("w: Too few arguments\n");
                        break;
                    }

                    if (argcount < 3)
                        size = 32;

                    PeekPoke_Write(addr, value, size);
                }
                break;

            case 'f':
                {
                    int argcount;
                    int addr;
                    int count;
                    int value;
                    int vstep;
                    int astep;

                    // addr cnt [value [vstep [astep]]]
                    argcount = sscanf(CmdStr_p+1, "%x %d %x %d %d", &addr, &count, &value, &vstep, &astep);
                    if (argcount < 3)
                    {
                        printf("f: Too few arguments\n");
                        break;
                    }
                    if (argcount <= 3)
                        vstep = 1;
                    if (argcount <= 4)
                        astep = 4;

                    PeekPoke_Fill(addr, count, value, vstep, astep);
                }
                break;

#ifdef UMDEVXSPEEKPOKE_SHMEM_PROVIDER
            case 'k':
                // allocate and attach to a shared memory buffer
                {
                    int argcount;
                    unsigned int size = 0;

                    argcount = sscanf(CmdStr_p+1, "%u", &size);
                    if (argcount == 0)
                    {
                        printf("k: Too few arguments\n");
                        break;
                    }

                    PeekPoke_AttachShMemAlloc(size);
                }
                break;
#endif /* UMDEVXSPEEKPOKE_SHMEM_PROVIDER */

#ifdef UMDEVXSPEEKPOKE_SHMEM_OBTAINER
            case 'm':
                // attach to a shared memory buffer based on GBI
                {
                    int argcount;
                    unsigned int gbi[8];

                    argcount = sscanf(CmdStr_p+1, "%x %x %x %x %x %x %x %x",
                                    gbi+0, gbi+1, gbi+2, gbi+3, gbi+4, gbi+5, gbi+6, gbi+7);
                    if (argcount != 8)
                    {
                        printf("m: Too few arguments (expected 8 bytes, in hex)\n");
                        break;
                    }

                    PeekPoke_MapShMem(gbi);
                }
                break;
#endif /* UMDEVXSPEEKPOKE_SHMEM_OBTAINER */

#ifdef UMDEVXSPEEKPOKE_SHMEM_COMMON
            case 'o':
            case 'e':
                {
                    int fCommit = (tolower(CmdStr_p[0]) == 'o');
                    int argcount;
                    unsigned int SubsetStart = 0;
                    unsigned int SubsetLen = 0;

                    argcount = sscanf(CmdStr_p+1, "%x %x", &SubsetStart, &SubsetLen);

                    PeekPoke_ShMem_CommitRefresh(SubsetStart, SubsetLen, fCommit);
                }
                break;
#endif /* UMDEVXSPEEKPOKE_SHMEM_COMMON */

#ifdef UMDEVXSPEEKPOKE_INTERRUPT
            case 'i':
                if (ReportInterrupts)
                {
                    ReportInterrupts = 0;
                    printf("Interrupt reporting turned OFF\n");
                }
                else
                {
                    ReportInterrupts = 1;
                    printf("Interrupt reporting turned ON\n");
                }
                break;
#endif /* UMDEVXSPEEKPOKE_INTERRUPT */

            case 'x':
                {
                    char * p = CmdStr_p + 1;
                    FILE * istrm_p;
                    size_t len;
                    int rv;

                    if (AttachedTo == 0)
                    {
                        printf("Not attached\n");
                        break;
                    }

                    while(*p == ' ')
                        p++;

                    len = strlen(p);
                    if (p[len-1] == '\n')
                        p[len-1] = '\0';

                    istrm_p = fopen(p, "rt");
                    if (NULL == istrm_p)
                    {
                        printf("Cannot open file: \"%s\"\n", p);
                        break;
                    }
                    rv = TransferVerilogFile(istrm_p);
                    if (rv < 0)
                    {
                        printf("Transfer failed (%d)\n", rv);
                    }
                    fclose(istrm_p);
                }
                break;

            default:
                printf("Unknown command: %s\n", CmdStr_p);
                break;
        }
    } // while

    PeekPoke_Detach();
    UMDevXSProxy_Shutdown();

    printf("\nGoodbye!\n");

    return 0;
}

/* end of file umdevxs_peekpoke.c */
