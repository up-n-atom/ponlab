/****************************************************************************

         Copyright (c) 2021 MaxLinear, Inc.
         Copyright (c) 2009 Lantiq Deutschland GmbH

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

*****************************************************************************/
#ifndef _lib_ini_access_h
#define _lib_ini_access_h

/** \file
   ini file access
*/

#ifdef __cplusplus
   extern "C" {
#endif

/* ===========================
   Includes
   =========================== */
#include "ifx_types.h"

/* ===========================
   Defines
   =========================== */
#define WRITE_SECTION(name)         fprintf(fileout, "[" #name "]\r\n")
#define WRITE_SECTION_NO(name, no)  fprintf(fileout, "[" #name "_%d]\r\n", (IFX_int32_t)(no))
#define WRITE_KEY(name, val)        fprintf(fileout,   #name "=%d\r\n", (IFX_int32_t)(val))
#define WRITE_KEY_STRING(name, val) fprintf(fileout,   #name "=%s\r\n", (IFX_char_t*)(val))


/* ===========================
   Function Declarations
   =========================== */
IFX_int32_t GetNextLine(const IFX_char_t* pData, IFX_char_t* pRetLine, IFX_int32_t nLine);
IFX_int32_t GetKeyString(const IFX_char_t* pSectionName, const IFX_char_t* pKeyName, const IFX_char_t* pDefault, IFX_char_t* pRetString, IFX_int32_t nSize, const IFX_char_t* pFile);
IFX_int32_t GetKeyInt(const IFX_char_t* pSectionName, const IFX_char_t* pKeyName, IFX_int32_t nDefault, const IFX_char_t* pFile);
IFX_int32_t GetSection(const IFX_char_t *pSection, IFX_char_t *pBuffer, IFX_int32_t nBufferSize, const IFX_char_t* filein);

#ifdef __cplusplus
}
#endif

#endif /*_lib_ini_access_h*/




