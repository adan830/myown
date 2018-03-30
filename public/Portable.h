//--------------------------------------------------------------------
// �ļ���:		Portable.h
// ��  ��:		
// ˵  ��:		
// ��������:	2010��12��14��
// ������:		½����
// ��Ȩ����:	������ţ�������޹�˾
//--------------------------------------------------------------------

#ifndef _PUBLIC_PORTABLE_H
#define _PUBLIC_PORTABLE_H

#include "Macros.h"

#ifdef FX_SYSTEM_WINDOWS
	#include "../system/WinPortable.h"
	#include "../system/WinFileSys.h"
	#include "../system/WinString.h"
#endif // FX_SYSTEM_WINDOWS

#ifdef FX_SYSTEM_LINUX
	#include "../system/LinuxPortable.h"
	#include "../system/LinuxFileSys.h"
	#include "../system/LinuxString.h"
#endif // FX_SYSTEM_LINUX

#endif // _PUBLIC_PORTABLE_H
