/********************************************************************************

  Copyright © 2020 MaxLinear, Inc. 
  Copyright (C) 2017-2018 Intel Corporation
  Lantiq Beteiligungs-GmbH & Co. KG
  Lilienthalstrasse 15, 85579 Neubiberg, Germany 

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

********************************************************************************/

/*  ***************************************************************************** 
 *         File Name    : ugw_defs.h                                          	*
 *         Description  : Common Library contains functions, defines,		*
 *			  structs, enums used across modules like CAL, CAPI,	*
 *			  CSD, Servd, and Management Entities			*
 *  *****************************************************************************/


/*! \file ugw_defs.h
\brief File contains the common definitions, macros and data
    structures common across all the modules in the software
*/
/** \addtogroup LIBHELP */
/* @{ */

#ifndef _UGW_DEFS_H
#define _UGW_DEFS_H

#include <stdint.h>
#include <syslog.h>

#include <help_defs.h>



#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define FACTORY_RESET_REBOOT 1
#define FACTORY_RESET_NO_REBOOT 0

typedef struct {
    int64_t nStart;
    int64_t  nEnd;
} __xTime;

#define HELP_TIME_INIT(__ix) __xTime __ix
#define HELP_TIME_START(__ix) __ix.nStart=help_getTimeMsec()
#define HELP_TIME_STOP(__ix) __ix.nEnd=help_getTimeMsec()
#define HELP_TIME_DIF(__ix) (double) (((double)__ix.nEnd - (double)__ix.nStart) / (double)1000)

#define HELP_PRINTF_TIME(__ix) LOGF_LOG_DEBUG("Time is: %f seconds\n",HELP_TIME_DIF(__ix));

#define MAX_COMPLETION_TIME (5)  /* Max number of seconds of time diff for HELP_PRINTF_DIFFTIME macro */

#define HELP_CRITICAL(fmt, args...) \
do {\
	if (!(SYS_LOG_TYPE_CONSOLE & LOGTYPE))\
		LOGF_LOG_PRINT(COLOR_RED, "CRITICAL", PACKAGE_ID " {%s, %d}: " fmt, __func__, __LINE__, ##args); \
	LOGF_LOG_CRITICAL(fmt, ##args); \
} while (0);


#define HELP_PRINTF_DIFFTIME(__ix, LOGFLG, OPT, fmt, args...) \
do { \
	double df = HELP_TIME_DIF(__ix); \
	if (df < MAX_COMPLETION_TIME) {\
		LOGF_LOG_DEBUG(fmt"completed in %f seconds\n", ##args, df);\
	} else {\
		HELP_CRITICAL(fmt"completed in %f seconds\n", ##args, df);\
	}\
} while (0);

#define BACKUP_RESTORE_DATA_FILE "/tmp/upgrade/run-data.xml"     /*!< backup or restore data xml file */
#define BACKUP_RESTORE_CONTROL_FILE "/tmp/upgrade/run-control.xml"    /*!< backup or restore control xml file */

#define ALIAS_NAME "cpe-"    /*!< Alias name identifier ex: cpe-xxxx-component_NAME*/
#define ALIAS_NAME_C "CPE-"    /*!< Alias name identifier ex: CPE-xxxx-component_NAME*/

#define INSTANCE_NO "*"    /*!< Magic delimiter to replace instance no */

#define DBG_INFO fprintf(stderr, "##########%s:%d##########\n",__func__,__LINE__);   /*!< DBG_INFO*/

/*!
        \brief
*/
#define CSD_CURRENT_STATUS_CHK "/tmp/csd/.__csd_db_status_chk"

# define NO_PARAM_VALUE "__st__"  /*!< String identifier */

/*!
        \brief CSD Owner Names
*/
#define OWNER_NAME_DEVM		"devmd"
#define OWNER_NAME_SERVD	"servd"
#define OWNER_NAME_WEB		"web"
#define OWNER_NAME_CLI		"cli"
#define OWNER_NAME_POLLD	"polld"
#define OWNER_NAME_OTHER	"other"
#define OWNER_NAME_VOIP		"voip"

/*!
        \brief Temporary file used to set/get objects in blob format
*/
#define HELP_DB_ACCESS_REQ_FILE	"/tmp/csd/.csd_tmp_req"
#define HELP_DB_ACCESS_RES_FILE	"/tmp/csd/.csd_tmp_res"

/*!
    \brief Path of image version file.
*/
#define HELP_IMG_VER_FILE            "/etc/version"
/*!
    \brief Path of image timestamp file.
*/
#define HELP_IMG_TS_FILE             "/etc/timestamp"

/*!
	\brief
*/
#define CAL_UBUS_SERVD_TIMEOUT 302000 //302 sec

/*! 
	\brief 
*/
#define UBUS_MSG_LENGTH 20000 /*!< Ubus message length used to define Tx/Rx of msg */

/*!
	\brief 
*/
#define MAX_UBUS_MSG_LENGTH 32768 /*!< Maximum length of the msg Tx/Rx over ubus with padding */

/*!
        \brief 
*/
#define  OID "Id"  /*!< Object Identifier */


#define MULTIINST "multiInst"	/*!< Defines given object is multi-instance */

/*!
        \brief 
*/
#define  SID "Sid"  /*!< Service Identifier */

/*!
        \brief 
*/
#define  WEBNAME "WebName"  /*!< Webname */

/*!
        \brief 
*/
#define  ACCESS "access"  /*!< Access */

/*!
        \brief 
*/
#define  PROFILE "profile"  /*!< PROFILE */

/*!
        \brief 
*/
#define  MAXENTRY  "maxEntries"  /*!< Maximum entries */

/*!
        \brief 
*/
#define  MINENTRY  "minEntries"  /*!< Minimum entries */

/*!
        \brief 
*/
#define  ENUMS  "enum"  /*!< A reference to enumerations */

/*!
        \brief 
*/
#define  MINVAL "minInclusive"  /*!< Minimum value */

/*!
        \brief 
*/
#define  MAXVAL "maxInclusive"  /*!< Maximum value */

/*!
        \brief 
*/
#define  MINLEN "minLength"  /*!< Minimum length */

/*!
        \brief 
*/
#define  MAXLEN "maxLength"  /*!< Maximum length */

/*!
        \brief 
*/
#define  NUMENTRIES "numEntriesParameter"  /*!< To get total numbers instance information */

/*!
        \brief 
*/
#define  FLAG "Flag"  /*!< Param (dynamic, type, modified) Identifier */

/*!
        \brief 
*/
#define  ACCESSLIST "ACL"  /*!< Macro for access control attribute */

/*!
        \brief 
*/
#define IS_THRED_ENV_SETL(bit) (bit & OPT_THREAD_ENVL) 	/*!< thread handling in ubus communication is set or not */

/*!
        \brief 
*/
#define IS_OBJLIST(x) OBJLIST(x)  /*!< Macro to check objlist structure used */

/*!
        \brief 
*/
#define IS_CHANGE_FLAG_SET(bit) (bit & ATTR_CHANGEFLAG)		/*!< Macro to check changeflag set attribute */

/*!
        \brief 
*/
#define IS_INFORM_TO_ACS_NOT_SET(bit) (bit & ATTR_INFORMTOACS) /*!< Macro to check inform to acs set or not attribute */


/*!
        \brief
*/
#define IS_RESET_CHANGE_FLAG_SET(bit) (bit & RESET_CHANGEFLAG) /*!< Macro to check reset flag set or not attribute */

/*!
        \brief 
*/
#define IS_GET_NOTIFY_SET(bit) (bit & GET_NOTIFY_SET)  /*!< Macro to check active notify  bit set or not */ 

/*!
        \brief 
*/
#define IS_NXT_LVL_SET(bit) (bit & NXT_LVL_TRUE) 	/*!< Macro to check next level bit set or not */ 

/*! 
	\brief 
*/
#define IS_SAVETOFLASH_NOT_SET(bit) (bit & ATTR_NOTSAVETOFLASH) 	/*!< Macro to check save to flash bit set */

/*! 
	\brief 
*/
#define IS_MOPT_GET(bit) (bit & MOPT_GET) 	/*!< Macro to check mainoper get bit set */

/*! 
	\brief 
*/
#define IS_MOPT_COMTREE(bit) (bit & MOPT_COMTREE) 	/*!< Macro to check mainoper complete tree req bit set */

/*!
	\brief
*/
#define IS_MOPT_SET(bit) (bit & MOPT_SET) 	/*!< Macro to check mainoper set bit set */

/*! 
	\brief 
*/
#define IS_MOPT_MODIFY(bit) (bit & MOPT_SET) 	/*!< Macro to check mainoper modify bit set */

/*! 
	\brief 
*/
#define IS_MOPT_ACTIVATE(bit) (bit & MOPT_ACTIVATE) 	/*!< Macro to check mainoper Activate bit set */

/*!
	\brief
*/
#define IS_MOPT_INIT(bit) (bit & MOPT_INIT) 	/*!< Macro to check mainoper init bit set */

/*!
	\brief
*/
#define IS_MOPT_UNINIT(bit) (bit & MOPT_UNINIT)	/*!< Macro to check mainoper uninit bit set */

/*!
	\brief
*/
#define IS_MOPT_NOTIFY(bit) (bit & MOPT_NOTIFY)	/*!< Macro to check mainoper notify bit set */
/*!
	\brief
*/
#define IS_MOPT_TIMERACTION(bit) (bit & MOPT_TIMERACTION) /*!< Macro to check mainoper timeraction bit set */

/*!
	\brief
*/
#define IS_MOPT_FACTORYRESET(bit)  (bit & MOPT_FACTORYRESET)	/*!< Macro to check mainoper factory reset bit set */
 
/*! 
	\brief 
*/
#define IS_MOPT_BACKUP(bit)  (bit & MOPT_BACKUP) 	/*!< Macro to check mainoper backup bit set */

/*! 
	\brief 
*/
#define IS_MOPT_REBOOT(bit)  (bit & MOPT_REBOOT) 	/*!< Macro to check mainoper reboot bit set */

/*! 
	\brief 
*/
#define IS_MOPT_FWUPGRADE(bit) (bit & MOPT_FWUPGRADE) /*!< Macro to check mainoper fwupgrade bit set */

/*! 
	\brief 
*/
#define IS_MOPT_ROLLBACK(bit) (bit & MOPT_ROLLBACK) /*!< Macro to check mainoper rollback bit set */

/*! 
	\brief 
*/
#define IS_MOPT_NOOPER(bit) (bit & MOPT_NOOPER) 	/*!< Macro to check mainoper noper bit set */

/*! 
	\brief 
*/
#define IS_MOPT_LOGSET(bit) (bit & MOPT_LOGSET) 	/*!< Macro to check mainoper logset bit set */

/*! 
	\brief 
*/
#define IS_MOPT_MESSAGE(bit) (bit & MOPT_MESSAGE) 	/*!< Macro to check mainoper message bit set */

/*!
	\brief 
*/
#define IS_MOPT_PREWATCH(bit) (bit & MOPT_PREWATCH) 	/*!< Macro to check mainoper Pre-Watch bit set */

/*!
	\brief 
*/
#define IS_MOPT_POSTWATCH(bit) (bit & MOPT_POSTWATCH) 	/*!< Macro to check mainoper Post-Watch bit set */



/*!
	\brief 
*/
#define IS_SOPT_VALIDATION(bit) (bit & SOPT_VALIDATION) 	/*!< Macro to check suboper validval bit set */

/*! 
	\brief 
*/
#define IS_SOPT_NOSUBOPER(bit) (bit & SOPT_NOSUBOPER)    /*!< Macro to check suboper suboper bit set */
 
/*! 
	\brief 
*/
#define IS_OBJOPT_ADD(bit) (bit & OBJOPT_ADD) 	/*!< Macro to check objoper add bit set */

/*! 
	\brief 
*/
#define IS_OBJOPT_DEL(bit) (bit & OBJOPT_DEL) /*!< Macro to check objoper delete bit set */

/*! 
	\brief 
*/
#define IS_OBJOPT_MODIFY(bit) (bit & OBJOPT_MODIFY) /*!< Macro to check objoper modify bit set */

/*! 
	\brief 
*/
#define IS_OBJOPT_NOSUBOPER(bit) (bit & OBJOPT_NOOBJOPER) /*!< Macro to check objoper suboper bit set */

/*! 
	\brief 
*/
#define IS_OBJOPT_ACTION(bit) (bit & OBJOPT_NO_ACTION) /*!< Macro to check objoper suboper bit set */

/*! 
	\brief 
*/
#define IS_OBJOPT_SKIP_COMMIT(bit) (bit & OBJOPT_SKIP_COMMIT) /*!< Macro to check objoper skip commit bit set */

/*! 
	\brief 
*/
#define IS_OBJOPT_DO_COMMIT(bit) (bit & OBJOPT_DO_COMMIT) /*!< Macro to check objoper do commit bit set */

/*!
	\brief
*/
#define IS_GETOPT_LOCAL(bit) (bit & GETOPT_LOCAL) /*!< Macro to check Object optional operation Local cache get bit set */

/*! 
	\brief 
*/
#define IS_GETOPT_LOCALDB(bit) (bit & GETOPT_LOCALDB) /*!< Macro to check Object optional operation Local cache get or failsafe DB get bit set */

/*! 
	\brief 
*/
#define IS_GETOPT_DBLOCAL(bit) (bit & GETOPT_DBLOCAL) /*!< Macro to check Object optional operation DB get or failsafe Local cache get bit set */


/*! 
	\brief 
*/
#define IS_DTYP_INT(bit) (bit & DTYP_INT) /*!< Macro to check int bit set */

/*! 
	\brief 
*/
#define IS_DTYP_UNSIGNEDINT(bit) (bit & DTYP_UNSIGNEDINT) /*!< Macro to check unsigned bit set */

/*! 
	\brief 
*/
#define IS_DTYP_STRING(bit) (bit & DTYP_STRING)  /*!< Macro to check string bit set */

/*! 
	\brief 
*/
#define IS_DTYP_DATETIME(bit) (bit & DTYP_DATETIME)  /*!< Macro to check datetime bit set */

/*! 
	\brief 
*/
#define IS_DTYP_BOOLEAN(bit) (bit & DTYP_BOOLEAN)  /*!< Macro to check boolean bit set */

/*! 
	\brief 
*/
#define IS_DTYP_HEXBINARY(bit) (bit & DTYP_HEXBINARY)  /*!< Macro to check HEXBINARY bit set */

/*! 
	\brief 
*/
#define IS_DTYP_LONG(bit) (bit & DTYP_LONG)  /*!< Macro to check LONG bit set */

/*! 
	\brief 
*/
#define IS_DTYP_UNLONG(bit) (bit & DTYP_UNLONG)  /*!< Macro to check UNLONG bit set */

/*! 
	\brief 
*/
#define IS_DTYP_BASE64(bit) (bit & DTYP_BASE64)  /*!< Macro to check BASE64 bit set */

/*! 
	\brief 
*/
#define IS_DTYP_NONE(bit) (bit & DTYP_NONE) /*!< Macro to check none bit set */

/*! 
        \brief 
*/
#define IS_DYNAMIC_SET(bit) (bit & MISC_DYNAMIC) /*!< Macro to check dynamic bit set or not */

/*! 
        \brief 
*/
#define IS_ACTIVATE_SET(bit) (bit & OBJOPT_ACTIVATE) /*!< Macro to check activate bit set or not */

/*! 
        \brief 
*/
#define IS_WRITE(bit) (bit & MISC_WRITE) /*!< Macro to check write access bit set or not */

/*! 
        \brief 
*/
#define IS_READ(bit) (bit & MISC_READ) /*!< Macro to check read access set or not */

/*! 
        \brief 
*/
#define IS_MODIFIED(bit) (bit & MISC_MODIFIED) /*!< Macro to check modified bit set or not */

/*! 
        \brief 
*/
#define IS_PASSWD_SET(bit) (bit & MISC_PASSWD) /*!< Macro to check password bit set or not. */

/*!
	\brief
*/
#define IS_MISC_DBSTATUS(bit) (bit & MISC_DBSTATUS) 	/*!< Macro to mainoper dbstatus bit set */

/*! 
        \brief 
*/
#define IS_PASSWD_GET_VAL(bit) (bit & GET_PWD_VAL) /*!< Macro to check password value get bit set or not. */

/*! 
        \brief 
*/
#define TOP 1 /*!< Indentation macro for top level */

/*! 
        \brief 
*/
#define DEV_LVL 2 /*!< Indentation macro for Device level */

/*! 
        \brief 
*/
#define LEVEL_OBJ 4 /*!< Indentation macro for Object level */

/*! 
        \brief 
*/
#define LEVEL_OBJ_PARAM 6 /*!< Indentation macro for object param  level */

/*! 
        \brief 
*/
#define LEVEL_OBJ_PARAM_VAL 8 /*!< Indentation macro for object param value level */
/*! 
        \brief 
*/
#define LEVEL_PARAM_TOP 10 /*!< Indentation macro for param list level */
/*! 
        \brief 
*/
#define LEVEL_PARAM 12  /*!< Indentation macro for param  level */
/*! 
        \brief 
*/
#define LEVEL_PARAM_VAL 14 /*!< Indentation macro for param value level */
/*!
	\brief
*/
#define SESSION_ID_SIZE 128 /*!< Indentation macro for web session */
/*!
        \brief
*/
#define MAX_UBUS_METHOD_SIZE 64 /*!< Macro to define UBUS method wrt daemon */

/*!
        \brief
*/
#define MAX_OWN_CNT 64 /*!< Indentation macro for own varaible size */

#endif  //#ifndef _UGW_DEFS_H

/* @} */
