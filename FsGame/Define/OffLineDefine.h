//----------------------------------------------------------
// �ļ���:      OffLineDefine.h
// ��  ��:      ���߹һ���غ궨��
// ˵  ��:
// ��������:    2014��10��17��
// ������:       
// �޸���:		 
//    :       
//----------------------------------------------------------

#ifndef __OffLineDefine_H__ 
#define __OffLineDefine_H__

//�һ�����
#define   OFFLINE_BOX              L"OffLineBox"

// #define PLAYER_OFFLINE_STATE_REC	"offline_state_rec"
// enum
// {
// 	OFFLINE_PLAYER_NAME	= 0,	//�����������������
// 	OFFLINE_PLAYER_STATE,		//�������״̬ OffLineState
// 	OFFLINE_PLAYER_ENTER_SCENE, //OffLineStateΪ2ʱ,��ҽ��볡����id,0�����߼����볡��
// 	OFFLINE_PLAYER_ACCOUNT,		//��ҵ��˺�
// 
// 	OFFLINE_STATE_REC_MAX,
// 
// 	MAX_OFFLINE_PLAYER_ROWS = 500
// };
// 
// // ���߹һ����ܱ�
// #define PLAYER_OFFLINE_SKILL_REC	"OffLine_skill_rec"
// enum
// {
// 	OFFLINE_SKILL_ID = 0,			//����id
// 
// 	OFFLINE_SKILL_REC_MAX,
// };


// ��ҵ�½״̬
enum LoginState
{
	ONLINE_LOGIN,					//���ߵ�½
	OFFLINE_LOGIN,					//���ߵ�½
	NOT_LOGIN,						//δ��½
};

// ����������ҷֲ���
#define SCENE_OFFLINE_REC "OffLineDistrbuteRec"
enum SceneOfflineRec
{
	SCENE_COL_OFFLINE_POS_INDEX = 0,		// �ֵ�����
	SCENE_COL_OFFLINE_POS_NORMAL_NUM,		// �ֵ���ͨ�������
	SCENE_COL_OFFLINE_POS_VIP_NUM,			// �ֵ�VIP����
};

// ����������Ҿ������ض���
// ��������
enum MirDataType
{	
	DATA_PROPERTY_TYPE,				// ��������
	DATA_CONTAINER_TYPE,			// ��������
	DATA_RECORD_TYPE				// ������
};

// ����������ұ�
enum OfflineCopyRec
{
	OCR_COL_PLAYER_UID,				// �������uid
	OCR_COL_NPC_OBJECT_ID,			// npc�����

	MAX_OCR_COLS
};

// Sns��Ҽ��ܱ�
enum SnsSkillRec
{
	SSR_COL_SKILL_ID,				// ����id
	SSR_COL_SKILL_LEVEL,			// ���ܵȼ�

	MAX_SSR_SKILL_COLS
};

#define NOT_VALID_VALUE -1 			// ��Чֵ

#endif