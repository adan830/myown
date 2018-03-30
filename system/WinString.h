//--------------------------------------------------------------------
// �ļ���:		WinString.h
// ��  ��:		
// ˵  ��:		
// ��������:	2010��12��14��
// ������:		½����
// ��Ȩ����:	������ţ�������޹�˾
//--------------------------------------------------------------------

#ifndef _SYSTEM_WINSTRING_H
#define _SYSTEM_WINSTRING_H

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

// �ַ���ת��Ϊ64λ����
inline __int64 Port_StringToInt64(const char* str)
{
	if (str == NULL)
	{
		return 0;
	}
	return _atoi64(str);
}

// 64λ����ת��Ϊ�ַ���
inline void Port_Int64ToString(__int64 val, char* buf, size_t byte_size)
{
	_snprintf(buf, byte_size - 1, "%I64d", val);
	buf[byte_size - 1] = 0;
}

// ���ַ���ת��Ϊ64λ����
inline __int64 Port_WideStrToInt64(const wchar_t* str)
{
	return _wtoi64(str);
}

// 64λ����ת��Ϊ���ַ���
inline void Port_Int64ToWideStr(__int64 val, wchar_t* buf, size_t byte_size)
{
	size_t len = byte_size / sizeof(wchar_t);
	_snwprintf(buf, len - 1, L"%I64d", val);
	buf[len - 1] = 0;
}

// �ַ���ת��Ϊ��д
inline char* Port_StringUpper(char* str)
{
	return _strupr(str);
}

// �ַ���ת��ΪСд
inline char* Port_StringLower(char* str)
{
	return _strlwr(str);
}

// ���ַ���ת��Ϊ��д
inline wchar_t* Port_WideStrUpper(wchar_t* str)
{
	return _wcsupr(str);
}

// ���ַ���ת��ΪСд
inline wchar_t* Port_WideStrLower(wchar_t* str)
{
	return _wcslwr(str);
}

// ��֯�ַ���
inline void Port_SafeSprintf(char* buf, size_t byte_size, const char* info, ...)
{
	va_list args;
	
	va_start(args, info);
	
	_vsnprintf(buf, byte_size - 1, info, args);

	buf[byte_size - 1] = 0;

	va_end(args);
}

// ��֯�ַ���
inline void Port_SafeSprintList(char* buf, size_t byte_size, const char* info, 
	va_list args)
{
	_vsnprintf(buf, byte_size - 1, info, args);
	
	buf[byte_size - 1] = 0;
}

// ��֯���ַ���
inline void Port_SafeSwprintf(wchar_t* buf, size_t byte_size, 
	const wchar_t* info, ...)
{
	size_t size = byte_size / sizeof(wchar_t);
	
	va_list args;
	
	va_start(args, info);
	
	_vsnwprintf(buf, size - 1, info, args);

	buf[size - 1] = 0;

	va_end(args);
}

// ��֯���ַ���
inline void Port_SafeSwprintList(wchar_t* buf, size_t byte_size, 
	const wchar_t* info, va_list args)
{
	size_t size = byte_size / sizeof(wchar_t);
	
	_vsnwprintf(buf, size - 1, info, args);
	
	buf[size - 1] = 0;
}

#endif // _SYSTEM_WINSTRING_H