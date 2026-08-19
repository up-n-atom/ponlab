#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include <unistd.h>

#include "icc.h"
#include "drv_icc_user.h"
#include <errno.h>

#define IA_ICC_DEV    "/dev/ltq_icc"
#define IA_ICC_CLIENT IA // suppose to be 1

/*static structure to hold all related unix sock */
struct icc_s{
    void           *mmap_data;
    uint32_t       mmap_size;
    int32_t        datafd;
    fd_set         readfd;
    fd_set         writefd;
};

static struct icc_s my_icc = {
    .mmap_data  = NULL,
    .mmap_size  = 0,
    .datafd     = -1,
    .readfd     = {{0}},
    .writefd    = {{0}}
};

fd_set  readfd;
fd_set  writefd;
int     maxfd;

#ifdef DBG_DUMP
static void bindump(const void * buf, size_t size)
{
    FILE* pOut = NULL;
    char * fname = "/tmp/icc-dump.bin";

    pOut = fopen(fname, "ab");

    if(pOut)
    {
        printf("dump to %s\n", fname);

        fflush(stdout);
        if( 0 == fwrite(buf, size, 1, pOut))
            printf("Dump %s failed\n", fname);
        else
            printf("Dump %s success\n", fname);
        fclose(pOut);
        pOut = NULL;
    }

    return;
}

void icc_msg_dump( icc_msg_t *pMsg )
{
    int i = 0;

    printf("\t---------------\n");
    printf("\tICC_MSG:\n");
    printf("\tsrc_client_id : 0x%x\n", pMsg->src_client_id);
    printf("\tdst_client_id  : 0x%x\n", pMsg->dst_client_id);
    printf("\tmsg_id         : 0x%x\n", pMsg->msg_id);
    printf("\tparam_attr     : 0x%x\n", pMsg->param_attr);

    for (i=0; i<MAX_UPSTRM_DATAWORDS; i++ )
         printf("\t  param[%d]: 0x%x\n", i, pMsg->param[i]);
    printf("\t---------------\n");

    return;
}

#endif /* DBG_DUMP */

uint32_t icc_init_server()
{
    if (my_icc.datafd >= 0)
        return 1;

    my_icc.datafd = open(IA_ICC_DEV, (O_RDWR&(~O_NONBLOCK)));

    if( my_icc.datafd < 0)
    {
        fprintf( stderr, "Data FD open on %s Failed : %d\n", IA_ICC_DEV, errno);
        return 2;
    }

    /*registering the client with client id 1*/
    ioctl(my_icc.datafd, ICC_IOC_REG_CLIENT, IA_ICC_CLIENT);

    #ifdef DBG_DUMP
    system("rm -vf /tmp/icc-dump.bin");
    #endif /* DBG_DUMP */

    return 0;
}

uint32_t icc_init_cli()
{
    if (my_icc.datafd >= 0)
        return 1;

    my_icc.datafd = open(IA_ICC_DEV, (O_RDWR&(~O_NONBLOCK)));

    if( my_icc.datafd < 0)
    {
        fprintf( stderr, "Data FD open on %s Failed : %d\n", IA_ICC_DEV, errno);
        return 2;
    }

    /*registering the client with client id 1*/
    ioctl(my_icc.datafd, ICC_IOC_REG_CLIENT, IA_ICC_CLIENT);

    return 0;
}

uint32_t icc_loadimage(uint8_t **pp_buf, unsigned int *buf_size, uint8_t *p_chunk_mode)
{
    uint32_t      ret    = 0;
    icc_msg_t     msg;
    icc_commit_t  cmt;
    void          *p_phys_addr;

    /* icc select */
    int     selval;
    ssize_t readval = -1;

    if( 0 != icc_init_server() || 0 >= my_icc.datafd)
    {
        fprintf( stderr, "icc_init failed\n");
        return 1;
    }

    FD_ZERO(&readfd);
    FD_ZERO(&writefd);
    FD_SET(my_icc.datafd, &readfd);
    FD_SET(my_icc.datafd,&writefd);
    maxfd = my_icc.datafd;
    selval  = -1;

    memset(&msg,0,sizeof(icc_msg_t));

    selval = select( maxfd+1,
                     &readfd,
                     &writefd,
                     (fd_set*)NULL,
                     (struct timeval *)NULL);

    if (selval < 0) {
        fprintf( stderr, "Error:select unblock without any proper reason\n");
    }
    else {
        printf("Num of ready descriptor %d\n", selval);

        if (FD_ISSET(my_icc.datafd, &readfd)) {
            printf("READ FD is ready!\n");

            /* read from icc */
            readval = read(my_icc.datafd, &msg, sizeof(icc_msg_t));

            #ifdef DBG_DUMP
            printf("READ GOT %d ( Excpect %d )\n", (int)readval, (int)sizeof(icc_msg_t));
            #endif /* DBG_DUMP */

            if( readval < sizeof(icc_msg_t))
                fprintf( stderr, "Error receiving icc message\n");

            #ifdef DBG_DUMP
            icc_msg_dump(&msg);
            #endif /* DBG_DUMP */

            /*Do a hit invalidate before mmap, to fetch fresh contents from RAM*/
            memset(&cmt, 0, sizeof(icc_commit_t));
            cmt.address[0] = msg.param[0];
            cmt.length[0]  = msg.param[1] & 0x7FFFFFFF;
            cmt.count      = 1;

            /*ioctl for invalidate*/
            ioctl(my_icc.datafd, ICC_IOC_MEM_INVALIDATE, &cmt);

            /* mmap to userspace */
            p_phys_addr      = (void *)CONVERT_KERNEL_TO_PHYADDR(msg.param[0]);
            my_icc.mmap_data = mmap(0,
                                    (size_t)(msg.param[1] & 0x7FFFFFFF),
                                    PROT_READ|PROT_WRITE,MAP_SHARED|MAP_LOCKED,
                                    my_icc.datafd,
                                    (off_t)p_phys_addr);
            my_icc.mmap_size = msg.param[1] & 0x7FFFFFFF;

            /* do something with the data */
            #ifdef DBG_DUMP
            bindump((const void *)my_icc.mmap_data, my_icc.mmap_size);
            #endif /* DBG_DUMP */

            if( (msg.param[1] & 0x80000000) != 0x80000000 ) {
                *pp_buf   = (uint8_t*)my_icc.mmap_data;
                *buf_size = my_icc.mmap_size;
                *p_chunk_mode = 0;
            } else {
                printf("[%s] chunk mode\n", __func__);

                *pp_buf       = (uint8_t*)my_icc.mmap_data;
                *buf_size     = my_icc.mmap_size;
                *p_chunk_mode = 1;
            }
        }
    }

    return ret;
}

uint32_t icc_freeimage(uint8_t *p_buf, unsigned int buf_size)
{
    if(!p_buf || !buf_size)
        return 1;

    if( p_buf != my_icc.mmap_data || buf_size != my_icc.mmap_size )
        fprintf(stderr, "strange, not matched, 0x%08X<=>0x%08X, size %d<=>%d\n", (uint32_t)p_buf, (uint32_t)my_icc.mmap_data, buf_size, my_icc.mmap_size);

    if( my_icc.mmap_data && my_icc.mmap_size)
    {
        munmap(my_icc.mmap_data,(size_t)my_icc.mmap_size);/*free the mapping*/
        my_icc.mmap_data = NULL;
        my_icc.mmap_size = 0;
    }

    return 0;
}

uint32_t icc_reply(uint8_t val) // 0 = fail, 1 = success
{
    icc_msg_t     msg;

        if (FD_ISSET(my_icc.datafd, &writefd)) {
            printf("WRITE FD is ready!\n");

             /* give him empty reply that this is finished , well the test app give some data back through previous write mmap but i dont think i need it */
            memset(&msg,0,sizeof(icc_msg_t));
            msg.src_client_id = IA_ICC_CLIENT;/*Client as 1*/
            msg.dst_client_id = IA_ICC_CLIENT;
            msg.msg_id        = val;

            write(my_icc.datafd, &msg, sizeof(icc_msg_t));
        }
    return 0;
}

uint32_t icc_send_chunk(uint8_t *p_buf, uint32_t buf_size, uint8_t lastChunk) //lastChunk 0:false, 1:true
{
    icc_msg_t     msg;
    icc_commit_t  cmt;

    int     selval;

    if ( !p_buf || 0 == buf_size)
        return 1;

    if ( !my_icc.mmap_data ||
         my_icc.mmap_size == 0 ||
         my_icc.mmap_size < sizeof(buf_size)){
        fprintf(stderr, "[%s]no shared mmap data avail!\n", __func__);
        return 2;
    }


        if (FD_ISSET(my_icc.datafd, &writefd)) {
            printf("WRITE FD is ready!\n");

             /* give him empty reply that this is finished , well the test app give some data back through previous write mmap but i dont think i need it */
            memset(&msg,0,sizeof(icc_msg_t));
            msg.src_client_id = IA_ICC_CLIENT;/*Client as 1*/
            msg.dst_client_id = IA_ICC_CLIENT;
            msg.msg_id        = IA;
            msg.param_attr    = SET_PTR(msg.param_attr,0);/*set the 0th param as ptr, since 1 is length no need to set attr for 1*/
            msg.param[0]      = (uint32_t)my_icc.mmap_data; // mmap address alocated before
            msg.param[1]      = buf_size;
            msg.param[2]      = 0x4; // 4 is image_root_fs, wadever

            if(!lastChunk)
                msg.param[1] |= 0x80000000; //set bit 31 to indicate more

            /*memcommit operation(cache wback of the address)*/
            memset(&cmt, 0, sizeof(icc_commit_t));
            /*Address for commit should always be mmap'ed from driver as
              driver manages the user space virtual address to kernel space virtual address conversion*/
            cmt.address[0] = (uint32_t)my_icc.mmap_data;/*copy user space virtual address*/
            cmt.length[0]  = buf_size;/*copy the length*/
            cmt.count      = 1;/*count of addresses to commit*/
            /*ioctl for commiting to RAM memory, should be done before write to driver*/
            ioctl(my_icc.datafd, ICC_IOC_MEM_COMMIT, &cmt);

            write(my_icc.datafd, &msg, sizeof(icc_msg_t));
        }
    return 0;
}

uint32_t icc_recv_chunk(uint8_t **pp_buf, uint32_t *p_buf_size, uint8_t *p_lastChunk) //lastChunk 0:false, 1:true
{
    icc_msg_t     msg;
    icc_commit_t  cmt;
    void          *p_phys_addr;

    /* icc select */
    int     selval;
    ssize_t readval = -1;

    if( !pp_buf || !p_buf_size || !p_lastChunk) {
        fprintf( stderr, "Error:Invalid Param\n");
        return 1;
    }

    FD_ZERO(&readfd);
    FD_ZERO(&writefd);
    FD_SET(my_icc.datafd,&readfd);
    FD_SET(my_icc.datafd,&writefd);
    selval  = -1;

    selval = select( maxfd+1,
                     &readfd,
                     &writefd,
                     (fd_set*)NULL,
                     (struct timeval *)NULL);

    if (selval < 0) {
        fprintf( stderr, "Error:select unblock without any proper reason\n");
        return 2;
    }
    else {
        printf("Num of ready descriptor %d\n", selval);

        if (FD_ISSET(my_icc.datafd, &readfd)) {
            printf("READ FD is ready!\n");

            /* read from icc */
            readval = read(my_icc.datafd, &msg, sizeof(icc_msg_t));

            #ifdef DBG_DUMP
            printf("READ GOT %d ( Excpect %d )\n", (int)readval, (int)sizeof(icc_msg_t));
            #endif /* DBG_DUMP */

            if( readval < sizeof(icc_msg_t))
                fprintf( stderr, "Error receiving icc packet\n");

            #ifdef DBG_DUMP
            icc_msg_dump(&msg);
            #endif /* DBG_DUMP */

            /*Do a hit invalidate before mmap, to fetch fresh contents from RAM*/
            memset(&cmt, 0, sizeof(icc_commit_t));
            cmt.address[0] = msg.param[0];
            cmt.length[0]  = msg.param[1] & 0x7FFFFFFF;
            cmt.count      = 1;

            /*ioctl for invalidate*/
            ioctl(my_icc.datafd, ICC_IOC_MEM_INVALIDATE, &cmt);

            /* mmap to userspace */
            p_phys_addr      = (void *)CONVERT_KERNEL_TO_PHYADDR(msg.param[0]);
            my_icc.mmap_data = mmap(0,
                                    (size_t)(msg.param[1] & 0x7FFFFFFF),
                                    PROT_READ|PROT_WRITE,MAP_SHARED|MAP_LOCKED,
                                    my_icc.datafd,
                                    (off_t)p_phys_addr);
            my_icc.mmap_size = msg.param[1] & 0x7FFFFFFF;

            /* do something with the data */
            #ifdef DBG_DUMP
            bindump((const void *)my_icc.mmap_data, my_icc.mmap_size);
            #endif /* DBG_DUMP */

            *pp_buf     = my_icc.mmap_data;
            *p_buf_size = my_icc.mmap_size;

             if ( (msg.param[1] & 0x80000000) != 0x80000000 )
                *p_lastChunk = 1;
            else
                *p_lastChunk = 0;
        }
    }

    return 0;
}

uint32_t icc_wait_reply(uint8_t *p_val)
{
    icc_msg_t     msg;

    /* icc select */
    int     maxfd;
    fd_set  readfd;
    //fd_set  writefd;
    int     selval;
    ssize_t readval = -1;

    if( !p_val) {
        fprintf( stderr, "Error:Invalid Param\n");
        return 1;
    }

    FD_ZERO(&readfd);
    //FD_ZERO(&writefd);
    FD_SET(my_icc.datafd,&readfd);
    //FD_SET(my_icc.datafd,&writefd);
    maxfd = my_icc.datafd;
    selval  = -1;

    selval = select( maxfd+1,
                     &readfd,
                     (fd_set*)NULL,//&writefd,
                     (fd_set*)NULL,
                     (struct timeval *)NULL);

    if (selval < 0) {
        fprintf( stderr, "Error:select unblock without any proper reason\n");
        return 2;
    }
    else {
        printf("Num of ready descriptor %d\n", selval);

        if (FD_ISSET(my_icc.datafd, &readfd)) {
            printf("READ FD is ready!\n");

            /* read from icc */
            readval = read(my_icc.datafd, &msg, sizeof(icc_msg_t));

            #ifdef DBG_DUMP
            printf("READ GOT %d ( Excpect %d )\n", (int)readval, (int)sizeof(icc_msg_t));
            #endif /* DBG_DUMP */

            if( readval < sizeof(icc_msg_t))
                fprintf( stderr, "Error receiving icc packet\n");

            #ifdef DBG_DUMP
            icc_msg_dump(&msg);
            #endif /* DBG_DUMP */

            /* now in real icc we need to mmap from address in icc_msg_t but in this
             * simulator i guess just i just use the buf in my static struct
             * TODO : still need to receive the real data
            */

            *p_val     = msg.msg_id;
        }
    }

    return 0;
}

