//--------------------------------------------------------------------
// �ļ���:		header.h
// ��  ��:		��Ϸ��Ҫͷ�ļ�
// ˵  ��:		
// ��������:	2015��11��24��
// ������:		     
// ��Ȩ����:	 
//--------------------------------------------------------------------

#ifndef _GAME_HEADER_H
#define _GAME_HEADER_H

#ifdef FSROOMLOGIC_EXPORTS
#include "server/IPubData.h"
#include "server/IRoomKernel.h"
#include "server/IRoomLogicModule.h"
#define ILogicModule IRoomLogicModule
#else
#include "server/IKernel.h"
#include "server/ILogicModule.h"
#endif
#include "server/ILogicModule_t.h"




#endif // _GAME_HEADER_H

