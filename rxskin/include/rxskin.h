#ifndef _RXSKIN_EXPORT_INCLUDE_
#define _RXSKIN_EXPORT_INCLUDE_

#include "windows.h"
#include "rxtype.h"
#include "rxlogger.h"
using namespace RX;
using namespace RX::Fundation;
#ifdef RXSKIN_EXPORTS
#define RXSKIN_API __declspec(dllexport)
#else
#define RXSKIN_API
#endif

#define RXAPI __stdcall

#include <richedit.h>

enum EResourceType
{
	RESOURCE_FROM_FILE = 0,
	RESOURCE_FROM_EXE,
	RESOURCE_FROM_ZIP,
	RESOURCE_FROM_DLL,
};

void RXSKIN_API RXSkinSetLogger(CRXLogger* poLogger);

extern CRXLogger* m_poRXSkinLogger;

#define RXSKIN_LOG_DEBUG(format, ...)  	\
{								\
	char buf[4*1024] = { 0 };		\
	memset(buf,0, 4*1024); \
    sprintf(buf, "[%s:%s:%d] [RXSKIN] [DEBUG] " format "", \
    __FILE__,__FUNCTION__ , __LINE__, ##__VA_ARGS__);     \
	if(m_poRXSkinLogger)                           \
		m_poRXSkinLogger->Log(buf);\
}

#define RXSKIN_LOG_CRIT(format, ...)  	\
{								\
	char buf[4*1024] = { 0 };		\
	memset(buf,0, 4*1024); \
    sprintf(buf, "[%s:%s:%d] [RXSKIN] [CRIT] " format "", \
    __FILE__,__FUNCTION__ , __LINE__, ##__VA_ARGS__);     \
	if(m_poRXSkinLogger)                           \
		m_poRXSkinLogger->Log(buf);\
}

#define RXSKIN_LOG_NOTI(format, ...)  	\
{								\
	char buf[4*1024] = { 0 };		\
	memset(buf,0, 4*1024); \
    sprintf(buf, "[%s:%s:%d] [RXSKIN] [NOTI] " format "", \
    __FILE__,__FUNCTION__ , __LINE__, ##__VA_ARGS__);     \
	if(m_poRXSkinLogger)                           \
		m_poRXSkinLogger->Log(buf);\
}

#define RXSKIN_LOG_ERR(format, ...)  	\
{								\
	char buf[4*1024] = { 0 };		\
	memset(buf,0, 4*1024); \
    sprintf(buf, "[%s:%s:%d] [RXSKIN] [ERROR] " format "", \
    __FILE__,__FUNCTION__ , __LINE__, ##__VA_ARGS__);     \
	if(m_poRXSkinLogger)                           \
		m_poRXSkinLogger->Log(buf);\
}

#define RXSKIN_LOG_WARN(format, ...)  	\
{								\
	char buf[4*1024] = { 0 };		\
	memset(buf,0, 4*1024); \
    sprintf(buf, "[%s:%s:%d] [RXSKIN] [WARN] " format "", \
    __FILE__,__FUNCTION__ , __LINE__, ##__VA_ARGS__);     \
	if(m_poRXSkinLogger)                           \
		m_poRXSkinLogger->Log(buf);\
}

#define RXSKIN_LOG_FATAL(format, ...)  	\
{								\
	char buf[4*1024] = { 0 };		\
	memset(buf,0, 4*1024); \
    sprintf(buf, "[%s:%s:%d] [RXSKIN] [FATAL] " format "", \
    __FILE__,__FUNCTION__ , __LINE__, ##__VA_ARGS__);     \
	if(m_poRXSkinLogger)                           \
		m_poRXSkinLogger->Log(buf);\
}

#define RXSKIN_LOG_INFO(format, ...)  	\
{								\
	char buf[4*1024] = { 0 };		\
	memset(buf,0, 4*1024); \
    sprintf(buf, "[%s:%s:%d] [RXSKIN] [INFO] " format "", \
    __FILE__,__FUNCTION__ , __LINE__, ##__VA_ARGS__);     \
	if(m_poRXSkinLogger)                           \
		m_poRXSkinLogger->Log(buf);\
}
#endif


