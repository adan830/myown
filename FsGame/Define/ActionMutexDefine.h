//------------------------------------------------------------------
// �� �� ��:    ActionMutexDefine.h
// ��    ��:    ��Ϊ����
// ˵    ��:    
// �� �� ��:      
// ��������:
//    :        
//-------------------------------------------------------------------
#ifndef _ACTION_DEFINE_H__
#define _ACTION_DEFINE_H__
#include <vector>

//�淨ID
enum
{
	ACTION_NONE = 0,

	ACTION_IN_TASK_GROUP,		// ���������
	ACTION_SECRET,				// �ؾ�
	ACTION_ARENA,				// ������	
	ACTION_GUILD_STATION,		// ���פ��
	ACTION_REVENGE_ARENA,		// ������
	ACTION_ESCORT,				// Ѻ��
	ACTION_TRANSFORM,			// ����
	ACTION_CONVOY,				// ����
	ACTION_TEAM_SECRET,			// ����ؾ�
	ACTION_PLUNDER_BATTLE,		// �Ӷ�ս
	ACTION_TEAMOFFER,			// �������
	ACTION_TOWER,				// ������
	ACTION_INFORMAL_PVP,		// �д�
	ACTION_KIDNAP,				// ��Ʊ
	ACTION_TREASURY_FRONTIER,	// ҹϮ����
	ACTION_IN_JAIL,				// ����
	ACTION_POWER_BATTLE,		// ����ս
	ACTION_FRO_TOWER,			// �߾�����
	ACTION_TEACHING,			// ���ڴ���
	ACTION_MAX,
};


#endif // _ACTION_DEFINE_H__