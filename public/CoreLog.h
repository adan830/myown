//--------------------------------------------------------------------
// �ļ���:		CoreLog.h
// ��  ��:		
// ˵  ��:		
// ��������:	2008��5��8��
// ������:		½����
// ��Ȩ����:	������ţ�������޹�˾
//--------------------------------------------------------------------

#ifndef _PUBLIC_CORELOG_H
#define _PUBLIC_CORELOG_H

#include "ICore.h"
#include "Converts.h"
#include <tchar.h>

extern ICore* g_pCore;

// ��¼��־
inline void CORE_TRACE(const char* info)
{
	g_pCore->TraceLog(info);
}

inline void CORE_TRACE(const wchar_t* info)
{
	g_pCore->TraceLogW(info);
}

// ��չ��־
inline void _logic_trace(const char* level, const char* func_name, const char* file_name, 
						 int line_no, const char* fmt, ...)
{
	Assert((level != NULL) && (func_name != NULL) && (file_name != NULL) && (fmt != NULL));

	if ((NULL == level) || (NULL == func_name) || (NULL == file_name) || (NULL == fmt))
	{
		return;
	}

	const char * file = _tcsrchr(file_name, '\\');

	if (NULL == file)
	{
		file = file_name;
	}
	else
	{
		file++;
	}

	char buf[512];

	Port_SafeSprintf(buf, sizeof(buf), "%s, (%s) ", level, func_name);

	size_t pos = strlen(buf);

	if (pos < sizeof(buf))
	{
		va_list	args;

		va_start(args, fmt);

		Port_SafeSprintList(buf+pos, sizeof(buf)-pos, fmt, args);

		va_end(args);
	}

	pos = strlen(buf);

	if (pos < sizeof(buf))
	{
		Port_SafeSprintf(buf+pos, sizeof(buf)-pos, " %s:%d", file, line_no);
	}

	g_pCore->TraceLog(buf);
}

inline void _logic_trace(const char* level, const char* func_name, const char* file_name, 
						 int line_no, const wchar_t* fmt, ...)
{
	Assert((level != NULL) && (func_name != NULL) && (file_name != NULL) && (fmt != NULL));

	if ((NULL == level) || (NULL == func_name) || (NULL == file_name) || (NULL == fmt))
	{
		return;
	}

	const char * file = _tcsrchr(file_name, '\\');

	if (NULL == file)
	{
		file = file_name;
	}
	else
	{
		file++;
	}

	char buf[512];

	Port_SafeSprintf(buf, sizeof(buf), "%s, (%s) ", level, func_name);

	size_t pos = strlen(buf);

	if (pos < sizeof(buf))
	{
		wchar_t wbuf[128];

		va_list	args;

		va_start(args, fmt);

		Port_SafeSwprintList(wbuf, sizeof(wbuf), fmt, args);

		va_end(args);

		Port_WideStrToString(wbuf, buf+pos, sizeof(buf)-pos);
	}

	pos = strlen(buf);

	if (pos < sizeof(buf))
	{
		Port_SafeSprintf(buf+pos, sizeof(buf)-pos, " %s:%d", file, line_no);
	}

	g_pCore->TraceLog(buf);
}

#define LOG_DEBUG(format, ...) \
	_logic_trace("DEBUG", __FUNCTION__, __FILE__, __LINE__, format, ##__VA_ARGS__)

#define LOG_MESSAGE(format, ...) \
	_logic_trace("INFO ", __FUNCTION__, __FILE__, __LINE__, format, ##__VA_ARGS__)

#define LOG_WARING(format, ...) \
	_logic_trace("WARN ", __FUNCTION__, __FILE__, __LINE__, format, ##__VA_ARGS__)

#define LOG_ERROR(format, ...) \
 	_logic_trace("ERROR", __FUNCTION__, __FILE__, __LINE__, format, ##__VA_ARGS__)

// ��������־ϵͳ

// ��־����
enum FXLOG_FILTER_TYPE
{
	FXLOG_FILTER_INVALID	= 0x00000000,
	FXLOG_FILTER_ERROR		= 0x00000001,
	FXLOG_FILTER_WARNING	= 0x00000002,
	FXLOG_FILTER_USER		= 0x00000004,
	FXLOG_FILTER_DEVELOPER	= 0x00000008,
	FXLOG_FILTER_TESTOR		= 0x00000010,
	FXLOG_FILTER_ALL		= 0xFFFFFFFF,
};

// �߳�����
enum FXCORE_ROUTINE_TYPE
{
	FXCORE_ROUTINE_EMPTY	= 0x00000000,
	FXCORE_ROUTINE_RENDER	= 0x00000001,
	FXCORE_ROUTINE_LOGIC	= 0x00000002,
	FXCORE_ROUTINE_RESOURCE	= 0x00000004,
	FXCORE_ROUTINE_NET		= 0x00000010,
	FXCORE_ROUTINE_ALL		= 0xFFFFFFFF,
};

// ע���߳���Ϣ
inline void CORE_LOG_RegistRoutine(FXCORE_ROUTINE_TYPE type, size_t start_addr, 
	size_t stack_size)
{
	g_pCore->LogRegisterThread(type, start_addr, stack_size);
}

// ������־����ģʽ
inline void CORE_LOG_EnableDropMode(bool bEnable)
{
	g_pCore->LogSetDropMode(bEnable);
}

// ��¼��־
#ifdef _DEBUG

inline void __cdecl CORE_LOG_ERROR(const char* format, ...)
{
	va_list	vlist;

	va_start(vlist, format);
	
	g_pCore->LogWrite(FXLOG_FILTER_ERROR, format, vlist);

	va_end(vlist);
}

inline void __cdecl CORE_LOG_WARNING(const char* format, ...)
{
	va_list	vlist;
	
	va_start(vlist, format);
	
	g_pCore->LogWrite(FXLOG_FILTER_WARNING, format, vlist);
	
	va_end(vlist);
}

inline void __cdecl CORE_LOG_USER(const char* format, ...)
{
	va_list	vlist;
	
	va_start(vlist, format);
	
	g_pCore->LogWrite(FXLOG_FILTER_USER, format, vlist);
	
	va_end(vlist);
}

inline void __cdecl CORE_LOG_DEVELOPER(const char* format, ...)
{
	va_list	vlist;
	
	va_start(vlist, format);
	
	g_pCore->LogWrite(FXLOG_FILTER_DEVELOPER, format, vlist);
	
	va_end(vlist);
}

inline void __cdecl CORE_LOG_TESTOR(const char* format, ...)
{
	va_list	vlist;
	
	va_start(vlist, format);
	
	g_pCore->LogWrite(FXLOG_FILTER_TESTOR, format, vlist);
	
	va_end(vlist);
}

inline void __cdecl CORE_LOGEX_ERROR(bool bAssert, const char* format, ...)
{
	if (!bAssert)
	{
		va_list	vlist;
		
		va_start(vlist, format);
		
		g_pCore->LogWrite(FXLOG_FILTER_ERROR, format, vlist);
		
		va_end(vlist);
	}
}

inline void __cdecl CORE_LOGEX_WARNING(bool bAssert, const char* format, ...)
{
	if (!bAssert)
	{
		va_list	vlist;
		
		va_start(vlist, format);
		
		g_pCore->LogWrite(FXLOG_FILTER_WARNING, format, vlist);
		
		va_end(vlist);
	}
}

inline void __cdecl CORE_LOGEX_USER(bool bAssert, const char* format, ...)
{
	if (!bAssert)
	{
		va_list	vlist;
		
		va_start(vlist, format);
		
		g_pCore->LogWrite(FXLOG_FILTER_USER, format, vlist);
		
		va_end(vlist);
	}
}

inline void __cdecl CORE_LOGEX_DEVELOPER(bool bAssert, const char* format, ...)
{
	if (!bAssert)
	{
		va_list	vlist;
		
		va_start(vlist, format);
		
		g_pCore->LogWrite(FXLOG_FILTER_DEVELOPER, format, vlist);
		
		va_end(vlist);
	}
}

inline void __cdecl CORE_LOGEX_TESTOR(bool bAssert, const char* format, ...)
{
	if (!bAssert)
	{
		va_list	vlist;
		
		va_start(vlist, format);
		
		g_pCore->LogWrite(FXLOG_FILTER_TESTOR, format, vlist);
		
		va_end(vlist);
	}
}

#else

inline void __cdecl CORE_LOG_ERROR(const char* format, ...)
{
	va_list	vlist;

	va_start(vlist, format);
	
	g_pCore->LogWrite(FXLOG_FILTER_ERROR, format, vlist);

	va_end(vlist);
}

inline void __cdecl CORE_LOG_WARNING(const char* format, ...)
{
	va_list	vlist;
	
	va_start(vlist, format);
	
	g_pCore->LogWrite(FXLOG_FILTER_WARNING, format, vlist);
	
	va_end(vlist);
}

inline void __cdecl CORE_LOG_USER(const char* format, ...)
{
}

inline void __cdecl CORE_LOG_DEVELOPER(const char* format, ...)
{
}

inline void __cdecl CORE_LOG_TESTOR(const char* format, ...)
{
}

#define	CORE_LOGEX_ERROR(a, p)
#define	CORE_LOGEX_WARNING(a, p)
#define CORE_LOGEX_USER(a, p)
#define	CORE_LOGEX_DEVELOPER(a, p)
#define	CORE_LOGEX_TESTOR(a, p)

#endif // _DEBUG

#endif // _PUBLIC_CORELOG_H

