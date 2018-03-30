//--------------------------------------------------------------------
// �ļ���:		CrossLogicDll.h
// ��  ��:		����߼����ܿⶨ��
// ˵  ��:		
// ��������:	2012��11��01��
// ������:		����
// ��Ȩ����:	������ţ�������޹�˾
//--------------------------------------------------------------------

#ifndef _SERVER_CROSSLOGICDLL_H
#define _SERVER_CROSSLOGICDLL_H

#include "../public/Macros.h"
#include "../public/ICore.h"
#include "ICrossKernel.h"

// ����һ�����������߼�DLL
#define DECLARE_CROSS_LOGIC_DLL(a) \
ICore* g_pCore = NULL; \
extern "C" FX_DLL_EXPORT \
const char* __cdecl FxModule_GetType() { return "FxGameLogic"; } \
extern "C" FX_DLL_EXPORT \
void __cdecl FxModule_Init(ICore* pCore) { g_pCore = pCore; } \
extern "C" FX_DLL_EXPORT \
int __cdecl FxCrossLogic_GetVersion() { return CROSS_LOGIC_MODULE_VERSION; } \
extern "C" FX_DLL_EXPORT \
ICrossCallee* __cdecl FxCrossLogic_CreateCallee() { return NEW a; } \
extern "C" FX_DLL_EXPORT \
void __cdecl FxCrossLogic_DeleteCallee(ICrossCallee* p) { delete p; } \
ASSERTION_FAIL_DEFINED;

#endif // _SERVER_CROSSLOGICDLL_H

