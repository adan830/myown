//--------------------------------------------------------------------
// �ļ���:		PubLogicDll.h
// ��  ��:		
// ˵  ��:		
// ��������:	2008��11��12��
// ������:		½����
// ��Ȩ����:	������ţ�������޹�˾
//--------------------------------------------------------------------

#ifndef _SERVER_PUBLOGICDLL_H
#define _SERVER_PUBLOGICDLL_H

#include "../public/Macros.h"
#include "../public/ICore.h"
#include "IPubKernel.h"

// ����һ�����������߼�DLL
#define DECLARE_PUBLIC_LOGIC_DLL(a) \
ICore* g_pCore = NULL; \
extern "C" FX_DLL_EXPORT \
const char* __cdecl FxModule_GetType() { return "FxGameLogic"; } \
extern "C" FX_DLL_EXPORT \
void __cdecl FxModule_Init(ICore* pCore) { g_pCore = pCore; } \
extern "C" FX_DLL_EXPORT \
int __cdecl FxPublicLogic_GetVersion() { return PUBLIC_LOGIC_MODULE_VERSION; } \
extern "C" FX_DLL_EXPORT \
IPubCallee* __cdecl FxPublicLogic_CreateCallee() { return NEW a; } \
extern "C" FX_DLL_EXPORT \
void __cdecl FxPublicLogic_DeleteCallee(IPubCallee* p) { delete p; } \
ASSERTION_FAIL_DEFINED;

#endif // _SERVER_PUBLOGICDLL_H

