//--------------------------------------------------------------------
// �ļ���:		AuctionLogicDll.h
// ��  ��:		
// ˵  ��:		
// ��������:	2008��11��12��
// ������:		½����
// ��Ȩ����:	������ţ�������޹�˾
//--------------------------------------------------------------------

#ifndef _SERVER_AUCTIONLOGICDLL_H
#define _SERVER_AUCTIONLOGICDLL_H

#include "../public/Macros.h"
#include "../public/ICore.h"
#include "IAuctionKernel.h"

// ����һ�������������߼�DLL
#define DECLARE_AUCTION_LOGIC_DLL(a) \
ICore* g_pCore = NULL; \
extern "C" FX_DLL_EXPORT \
const char* __cdecl FxModule_GetType() { return "FxGameLogic"; } \
extern "C" FX_DLL_EXPORT \
void __cdecl FxModule_Init(ICore* pCore) { g_pCore = pCore; } \
extern "C" FX_DLL_EXPORT \
int __cdecl FxAuctionLogic_GetVersion() { return AUCTION_LOGIC_MODULE_VERSION; } \
extern "C" FX_DLL_EXPORT \
IAuctionCallee* __cdecl FxAuctionLogic_CreateCallee() { return NEW a; } \
extern "C" FX_DLL_EXPORT \
void __cdecl FxAuctionLogic_DeleteCallee(IAuctionCallee* p) { delete p; } \
ASSERTION_FAIL_DEFINED;

#endif // _SERVER_AUCTIONLOGICDLL_H

