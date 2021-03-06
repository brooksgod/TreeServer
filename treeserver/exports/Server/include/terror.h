/******************************
*	Module Name:
*		TERROR.H
*
*	Abstract:
*		Contains defines for error code which are commonly 
*	used in Tree Server.
*
*	Author:
*		NiuJingyu.
*
*	Copyright (c) NiuJingyu 1997  China Railway Software Corporation
*
*	Revision History:
*		Write by NiuJingyu, 1997.10
*		Update by NiuJingyu, 1998.03
********************************************************************************/

#ifndef _INC_TERROR_
#define _INC_TERROR_

#ifndef _WINERROR_
#include <winerror.h>
#endif

#pragma pack(push,1)

#ifdef  __cplusplus
extern "C" {
#endif

#define TERR_SUCCESS		ERROR_SUCCESS
#define TERR_BASE			7000

#define TERR_UNKNOWN_ERROR				TERR_BASE
#define TERR_WARN_QUEUE_TOO_SMALL		(TERR_BASE+1)
#define TERR_QUEUE_ERROR				(TERR_BASE+2)
#define TERR_QUEUE_FULL					(TERR_BASE+3)
#define TERR_QUEUE_EMPTY				(TERR_BASE+4)
#define TERR_QUEUE_NO_RESPONSE			(TERR_BASE+5)
#define TERR_INVALID_USER				(TERR_BASE+6)
#define TERR_INVALID_PASSWORD			(TERR_BASE+7)
#define TERR_INVALID_PARAMETER			(TERR_BASE+8)
#define TERR_INVALID_COMMAND			(TERR_BASE+9)
#define TERR_SYNC_OBJECT_EXIST			(TERR_BASE+10)
#define TERR_INVALID_MESSAGE			(TERR_BASE+11)
#define TERR_INVALID_COMPUTER			(TERR_BASE+12)
#define TERR_NO_INVALID_TED				(TERR_BASE+13)
#define TERR_INVALID_TED_LIBRARY		(TERR_BASE+14)
#define TERR_INVALID_TED_COMMAND		(TERR_BASE+15)
#define TERR_INVALID_TED_ENTRY			(TERR_BASE+16)
#define TERR_INVALID_TED_CAPABILITY		(TERR_BASE+17)
#define TERR_INVALID_TED_TIMER			(TERR_BASE+18)
#define TERR_INVALID_TED_IDLE			(TERR_BASE+19)
#define TERR_INVALID_TED_TIMEOUT		(TERR_BASE+20)
#define TERR_INVALID_TED_ENABLE			(TERR_BASE+21)
#define TERR_TED_DISABLE				(TERR_BASE+22)
#define TERR_TED_LAOD_FAILURE			(TERR_BASE+23)
#define TERR_TED_ERROR_ENTRY			(TERR_BASE+24)
#define TERR_INVALID_REQUEST			(TERR_BASE+25)
#define TERR_NO_TEINFO_ENTRY			(TERR_BASE+26)
#define TERR_INVALID_FLAG				(TERR_BASE+27)
#define TERR_INVALID_TASKTABLE			(TERR_BASE+28)
#define TERR_ADJUST_TASKTABLE			(TERR_BASE+29)
#define TERR_INVALID_TASK				(TERR_BASE+30)
#define TERR_NAMEDPIPE_TIMEOUT			(TERR_BASE+31)
#define TERR_INVALID_TASK_LOCATION		(TERR_BASE+32)
#define TERR_ACCESS_VIOLATION			(TERR_BASE+33)
#define TERR_INVALID_ADDRESS			(TERR_BASE+34)
#define TERR_INVALID_AUTHSERVER			(TERR_BASE+35)
#define TERR_ALLOC_EXCHBUF_FAILURE		(TERR_BASE+36)
#define TERR_NOT_LOGON					(TERR_BASE+37)
#define TERR_CLIENT_CLOSE				(TERR_BASE+38)
#define TERR_SERVICE_CLOSE				(TERR_BASE+39)
#define TERR_NOT_ALL_COPYED				(TERR_BASE+40)

#ifdef  __cplusplus
}
#endif

#pragma pack(pop)

#endif // _INC_TERROR_