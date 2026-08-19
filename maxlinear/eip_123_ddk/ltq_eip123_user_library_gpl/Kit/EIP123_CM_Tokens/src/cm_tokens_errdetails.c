/* cm_tokens_errdetails.c
 *
 * Crypto Module Tokens Parser/Generator.
 */

/*****************************************************************************
* Copyright (c) 2010-2015 INSIDE Secure B.V. All Rights Reserved.
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

#include "c_cm_tokens.h"        // configuration options

#include "basic_defs.h"         // uint8_t, IDENTIFIER_NOT_USED, etc.
#include "clib.h"               // memset, memcpy

#include "cm_tokens_common.h"   // the API to implement

#ifndef CMTOKENS_REMOVE_ERROR_DESCRIPTIONS

typedef struct
{
    unsigned int ErrorNumber;
    const char * ErrorText_p;
} CMTokens_ErrorMessage_t;

static const CMTokens_ErrorMessage_t CMTokens_ErrorMessages[] =
{
    // error code 0 must be last (indicates end of list)
    { 0x001, "Sequencer 1: Invalid operation requested in OpCode/SubCode" },
    { 0x002, "Sequencer 2: At least one invalid parameter provided" },
    { 0x003, "Sequencer 3: Invalid key size" },
    { 0x004, "Sequencer 4: Invalid length" },
    { 0x005, "Sequencer 5: Invalid location" },
    { 0x006, "Sequencer 6: Clock disabled" },
    { 0x007, "Sequencer 7: Asset incompatible or no access" },
    { 0x00A, "Sequencer 10: Unwrap error" },
    { 0x00B, "Sequencer 11: Data too large" },
    { 0x00C, "Sequencer 12: Asset checksum error" },
    { 0x00D, "Sequencer 13: Invalid asset" },
    { 0x00E, "Sequencer 14: Asset store full" },
    { 0x00F, "Sequencer 15: Invalid Address" },
    { 0x010, "Sequencer 16: Engine not available" },
    { 0x011, "Sequencer 17: Invalid Modulus" },
    { 0x012, "Sequencer 18: Verify Failed" },
    { 0x01F, "Sequencer 31: Panic, please reset me" },
    { 0x041, "TRNG 1: Too many FRO shutdown" },
    { 0x042, "TRNG 2: Output error" },
    { 0x044, "TRNG 3: Noise source failure" },
    { 70,    "Sequencer: Reserved" },
    { 80,    "DMA: Bus Error" },
    { 90,    "Panic, please reset me" },
    { 100,   "Reserved number, should not occur" },
    { 0x000, "Sequencer 0: No error" },
};

#ifndef CMTOKENS_REMOVE_PARSERESPONSE_NVMERRORDETAILS
static const CMTokens_ErrorMessage_t CMTokens_NvmErrorMessages[] =
{
    // error code 0 must be last (indicates end of list)
    { 1, "NVM 1: Protected Asset has been Replaced" },
    { 2, "NVM 2: Protected Asset has been Removed" },
    { 3, "NVM 0: Premature end" },
    { 4, "NVM 0: Programmed-State mismatch" },
    { 0, "NVM 0: No error" }
};
#endif /* CMTOKENS_REMOVE_PARSERESPONSE_NVMERRORDETAILS */
#endif /* CMTOKENS_REMOVE_ERROR_DESCRIPTIONS */


/*----------------------------------------------------------------------------
 * CMTokens_ParseResponse_ErrorDetails
 */
#ifndef CMTOKENS_REMOVE_PARSERESPONSE_ERRORDETAILS
int
CMTokens_ParseResponse_ErrorDetails(
        CMTokens_Response_t * const ResponseToken_p,
        const char ** const ErrorText_pp)
{
    unsigned int ErrorNumber = 0;
    uint32_t Word0;

    if (ResponseToken_p == NULL)
    {
        return -1;
    }

    Word0 = ResponseToken_p->W[0];
    if (Word0 & BIT_31)
    {
        // error bit is set
        int ResultSrc = (Word0 >> 29) & MASK_2_BITS;
        int Result = (Word0 >> 24) & MASK_5_BITS;

        switch (ResultSrc)
        {
            case 0:
                // Source = Sequencer
                // map reserved numbers to 70
                if ((Result >= 0x08 && Result <= 0x09) ||
                    (Result >= 0x13 && Result <= 0x1E))
                {
                    ErrorNumber = 70;
                }
                else
                {
                    ErrorNumber = Result;
                }
                break;

            case 1:
                // Source = DMA
                // Result field holds the result from the DMA
                // we simply report the error without showing the details
                ErrorNumber = 80;
                break;

            case 2:
                // Source = TRNG
                ErrorNumber = 0x40 + Result;
                break;

            case 3:
                // Source = Panic
                ErrorNumber = 90;
                break;

            default:
                // reserved
                ErrorNumber = 100;
                break;
        } // switch
    }

    if (ErrorText_pp)
    {
        static const char CMTokens_ErrorMsg_NotAvailable[] = "n/a";

        // start with the default result "n/a"
        *ErrorText_pp = CMTokens_ErrorMsg_NotAvailable;

        // find the proper error description
        // if the table has been compiled in
#ifndef CMTOKENS_REMOVE_ERROR_DESCRIPTIONS
        {
            const CMTokens_ErrorMessage_t * p = CMTokens_ErrorMessages;
            do
            {
                if (p->ErrorNumber == ErrorNumber)
                {
                    *ErrorText_pp = p->ErrorText_p;
                    break;      // from the while
                }
                p++;
            }
            while (p->ErrorNumber != 0);
        }
#endif /* CMTOKENS_REMOVE_ERROR_DESCRIPTIONS */
    } // if

    return ErrorNumber;
}
#endif /* !CMTOKENS_REMOVE_PARSERESPONSE_ERRORDETAILS */


/*----------------------------------------------------------------------------
 * CMTokens_ParseResponse_NvmErrorDetails
 *
 * This function translates the specific NVM error code (returned in the
 * Version Info token) into an error message.
 */
#ifndef CMTOKENS_REMOVE_PARSERESPONSE_NVMERRORDETAILS

static const char CMTokens_NvmErrorMsg_NotAvailable[] = "n/a";

const char *
CMTokens_ParseResponse_NvmErrorDetails(
        unsigned int NvmErrorCode)
{
    // find the proper error description
    // if the table has been compiled in
#ifndef CMTOKENS_REMOVE_ERROR_DESCRIPTIONS
    {
        const CMTokens_ErrorMessage_t * p = CMTokens_NvmErrorMessages;
        do
        {
            if (p->ErrorNumber == NvmErrorCode)
                return p->ErrorText_p;      // ## RETURN ##

            p++;
        }
        while (p->ErrorNumber != 0);
    }
#endif /* CMTOKENS_REMOVE_ERROR_DESCRIPTIONS */

    return CMTokens_NvmErrorMsg_NotAvailable;
}

#endif /* !CMTOKENS_REMOVE_PARSERESPONSE_NVMERRORDETAILS */

/* end of file cm_tokens_errdetails.c */
