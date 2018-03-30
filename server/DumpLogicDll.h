//--------------------------------------------------------------------
// �ļ���:		DumpLogicDll.h
// ��  ��:		
// ˵  ��:		
// ��������:	2010��4��7��
// ������:		½����
// ��Ȩ����:	������ţ�������޹�˾
//--------------------------------------------------------------------

#ifndef _SERVER_DUMPLOGICDLL_H
#define _SERVER_DUMPLOGICDLL_H

#include "../public/Macros.h"
#include "../public/ICore.h"
#include "IDumpKernel.h"

// ����һ������ת���߼�DLL
#define DECLARE_DUMP_LOGIC_DLL(a) \
ICore* g_pCore = NULL; \
extern "C" FX_DLL_EXPORT \
const char* __cdecl FxModule_GetType() { return "FxGameLogic"; } \
extern "C" FX_DLL_EXPORT \
void __cdecl FxModule_Init(ICore* pCore) { g_pCore = pCore; } \
extern "C" FX_DLL_EXPORT \
int __cdecl FxDumpLogic_GetVersion() { return DUMP_LOGIC_MODULE_VERSION; } \
extern "C" FX_DLL_EXPORT \
IDumpCallee* __cdecl FxDumpLogic_GetCallee() { static a s_##a; return &s_##a; } \
ASSERTION_FAIL_DEFINED;

#endif // _SERVER_DUMPLOGICDLL_H

