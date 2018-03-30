//--------------------------------------------------------------------
// �ļ���:		RoomLogicDll.h
// ��  ��:		
// ˵  ��:		
// ��������:	2014��11��5��
// ������:		���н�
// ��Ȩ����:	������ţ�������޹�˾
//--------------------------------------------------------------------

#ifndef _SERVER_ROOM_LOGICDLL_H
#define _SERVER_ROOM_LOGICDLL_H

#include "../public/Macros.h"
#include "../public/ICore.h"
#include "IRoomKernel.h"
#include "IRoomCreator.h"
#include "IRoomModuleCreator.h"

// ����һ����Ϸ�߼�DLL
#define DECLARE_ROOM_LOGIC_DLL \
ICore* g_pCore = NULL; \
IRoomCreator* g_pRoomCreator = NULL; \
IRoomModuleCreator* g_pRoomModuleCreator = NULL; \
extern "C" FX_DLL_EXPORT \
const char* __cdecl FxModule_GetType() { return "FxGameLogic"; } \
extern "C" FX_DLL_EXPORT \
void __cdecl FxModule_Init(ICore* pCore) { g_pCore = pCore; } \
extern "C" FX_DLL_EXPORT \
int __cdecl FxGameLogic_GetVersion() { return GAME_LOGIC_MODULE_VERSION; } \
extern "C" FX_DLL_EXPORT \
IRoomCreator* __cdecl FxGameLogic_GetCreator() { return g_pRoomCreator; } \
extern "C" FX_DLL_EXPORT \
IRoomModuleCreator* __cdecl FxGameLogic_GetModuleCreator() { return g_pRoomModuleCreator; } \
ASSERTION_FAIL_DEFINED;

#endif // _SERVER_LOGICDLL_H

