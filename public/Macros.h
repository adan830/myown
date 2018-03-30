//--------------------------------------------------------------------
// �ļ���:		Macros.h
// ��  ��:		
// ˵  ��:		
// ��������:	2007��1��30��
// ������:		½����
// ��Ȩ����:	������ţ�������޹�˾
//--------------------------------------------------------------------

#ifndef _PUBLIC_MACROS_H
#define _PUBLIC_MACROS_H

#ifdef _WIN32
	#define FX_SYSTEM_WINDOWS
	#ifdef _WIN64
		#define FX_SYSTEM_64BIT
	#else
		#define FX_SYSTEM_32BIT
	#endif // _WIN64
#endif // _WIN32

#ifdef __linux__
	#define FX_SYSTEM_LINUX
	#define FX_SYSTEM_64BIT
#endif // __linux__

#ifdef FX_SYSTEM_WINDOWS
	#include <stddef.h>
	#pragma warning(disable: 4786)
	#pragma warning(disable: 4996)
	#pragma warning(disable: 4819)
	#define FX_DLL_EXPORT __declspec(dllexport)
	typedef signed __int64 int64_t;
	typedef unsigned __int64 uint64_t;
#endif // FX_SYSTEM_WINDOWS

#ifdef FX_SYSTEM_LINUX
	#include <stddef.h>
	#include <stdint.h>
	#define stricmp strcasecmp
	#define wcsicmp wcscasecmp
	#define FX_DLL_EXPORT __attribute__((visibility("default")))
	#define __cdecl	
#endif // FX_SYSTEM_LINUX

#ifndef SAFE_RELEASE
	#define SAFE_RELEASE(p)	{ if (p) { (p)->Release(); (p) = NULL; } }
#endif // !SAFE_RELEASE

#include "Debug.h"

#endif // _PUBLIC_MACROS_H

