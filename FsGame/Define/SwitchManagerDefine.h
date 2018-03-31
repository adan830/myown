//----------------------------------------------------------
// �ļ���:      switchmanagerdefine.h
// ��  ��:      ���ܿ��ض���
// ˵  ��:
// ������:        
// ��������:    2015��6��30��
//    :        
//----------------------------------------------------------
#ifndef __SWITCHMANAGER_DEFINE_H_
#define __SWITCHMANAGER_DEFINE_H_

// ���ܿ��ر�
const char* const DOMAIN_SWITCH_REC = "switch_rec";

// ���ܿ��������ݼ��ص����ݿⶨʱ��ʱ��,1����
#define SWITCH_REC_DELAY_TIME       (60*1000)           

// ����
enum SwitchRecCol
{
    SWITCH_REC_COL_FUNCTION_ID = 0,		// ����Id
	SWITCH_REC_COL_FUNCTION_NAME = 1,	// ��������
    SWITCH_REC_COL_CUR_STATE = 2,       // ��ǰ״̬(1:���� 0:�ر�)
	SWITCH_REC_COL_TOTAL,
};

enum SwitchRecType
{
	SWITCH_REC_TYPE_INIT = 0,		// ��Դ��ʼ������
	SWITCH_REC_TYPE_CHANGE = 1,		// �ⲿ�Ķ�
};

// ����id(��ini/SystemFunction/function_switch.xml��ͬ)
//$@ Switch
enum SwitchFunctionType
{
    SWITCH_FUNCTION_MIN = 0,

	SWITCH_FUNCTION_CAPITAL_MONITOR,		//  = �����޶���
	SWITCH_FUNCTION_PLUG_IN,				//  = ��Ҽ��
	SWITCH_FUNCTION_PASSIVE_SKILL,			//	= ��������
	SWITCH_FUNCTION_SKILL_UP,				//	= ��������	
	SWITCH_FUNCTION_WING,					//	= �����	
	SWITCH_FUNCTION_JADE,					//	= ���幦��	
	SWITCH_FUNCTION_GUILD_CREATE,			//  = ���ᴴ��
	SWITCH_FUNCTION_GUILD_JOIN,				//  = ���빫��
	SWITCH_FUNCTION_GUILD_LEAVE,			//  = �뿪����
	SWITCH_FUNCTION_GUILD_DONATE,			//  = �������
	SWITCH_FUNCTION_GUILD_SHOP,				//  = �����̵�
	SWITCH_FUNCTION_ARENA,					//  = ���䳡
	SWITCH_FUNCTION_WORLD_BOSS,				//  = ����boss
	SWITCH_FUNCTION_CAMPAIGN_ASURA_BATTLE,	//  = ����ս��

	SWITCH_FUNCTION_MAX,
};

#endif