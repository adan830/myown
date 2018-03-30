//--------------------------------------------------------------------
// �ļ���:		FriendDefine.h
// ��  ��:		����ģ��
// ˵  ��:		һ����غ궨���
// ��������:	2014��06��10��
// ��������:	2014��06��10��
// ������:		  ( )
//    :	    
//--------------------------------------------------------------------

#ifndef __FriendDefine_H__ 
#define __FriendDefine_H__ 
#include "utils/util_config.h"


#define DOMAIN_FRIEND_RECOMMEND_REC		L"Domain_Friend_Recommend_%d"
#define SNS_FRIEND_SPACE				L"SnsFriend"


enum
{
	TEAM_ADD_INTIMACY_OFFER,//����
	TEAM_ADD_INTIMACY_SECRET,//��ӽ���
	TEAM_ADD_INTIMACY_INBREAK,//��֯����
	TEAM_ADD_INTIMACY_ESCORT,//���Ѻ��
	TEAM_ADD_INTIMACY_KILL_NPC,//Ұ��һ�
};






// ������������������������Ϣ����
enum 
{
	FRIEND_SEVER_PUB_ADD_ONLINE_PLAYER_INFO = 0,     // ������������Ϣ
	FRIEND_SEVER_PUB_DEL_ONLINE_PLAYER_INFO,		 // ɾ�����������Ϣ
	FRIEND_SEVER_PUB_SET_LEVEL_INFO,				 // ���õȼ�
	FRIEND_SEVER_PUB_SET_BATTLEABILITY_INFO,		 // ����ս����
	FRIEND_SEVER_PUB_SET_GUILDNAME_INFO,		 // ���ð����
	FRIEND_SEVER_PUB_SET_TEAM_ID,			//���ö���id
	FRIEND_SEVER_PUB_SET_VIP,						//����vip
};

//�������·���Ϣ
enum
{
	PUB_FRIEND_MARRY_UPDATE = 0,				// �ҵ�UID
};

// ����������������ұ�
#define PUB_ONLINE_PLAYER_REC "pub_online_player_rec"
// ������ұ��ж���
enum OnlinePlayerRec
{
	PUB_COL_PLAYER_UID = 0,			// ���UID
	PUB_COL_PLAYER_NAME,	        // �������
	PUB_COL_PLAYER_LEVEL,		    // ��ҵȼ�
	PUB_COL_PLAYER_JOB,				// ���ְҵ
	PUB_COL_PLAYER_SEX,				// ����Ա�
	PUB_COL_PLAYER_BATTLE_ABLITITY,	// ���ս����
	PUB_COL_PLAYER_GUILD_NAME,	//	�����
	PUB_COL_PLAYER_TEAM_ID,		//	teamID
	PUB_COL_PLAYER_VIP,			//vip
	PUB_COL_PLAYER_MAX,
};
//���ѹ�ϵ
enum FriendRelation
{
	FRIEND_RELATION_NOMAL,	//��ͨ���ѹ�ϵ
	FRIEND_RELATION_SWORN,	//���
};


//��������
enum ApplyType
{
    APPLY_TYPE_FRIEND = 0,      //�������
   
};

enum
{
	FRIEND_SWORN_ADD = 1, //��ӽ���
	FRIEND_SOWRN_DEL = -1, //ɾ������

};

struct NearbyRecommend 
{
	NearbyRecommend(): distance(0.0f){}
	PERSISTID pid;
	float distance;
};


// ���������
DECLARE_CONFIG_OBJECT_BEGIN(ConfigFriendGiftItem)

#define CONFIG_CLASS_NAME ConfigFriendGiftItem
#define PROPLIST      \
		DECLARE_CONFIG_PROP_FIELD_EX(ID, "������ID", 1, size_t )	\
		DECLARE_CONFIG_PROP_FIELD(ItemID, "����ֵ", string)		\
		DECLARE_CONFIG_PROP_FIELD_EX(Value, "������",  0, size_t)	

#include "utils/util_config_template.h"

DECLARE_CONFIG_OBJECT_END()

// �������ܶ�������
DECLARE_CONFIG_OBJECT_BEGIN(ConfigFriendIntimacyUpgradeItem)

#define CONFIG_CLASS_NAME ConfigFriendIntimacyUpgradeItem
#define PROPLIST      \
		DECLARE_CONFIG_PROP_FIELD_EX(ID, "������ID", 1, size_t )	\
		DECLARE_CONFIG_PROP_FIELD_EX(Exp, "���辭��ֵ", 0xFFFFFFFF, size_t) 

#include "utils/util_config_template.h"

DECLARE_CONFIG_OBJECT_END()


// �������ܶ�������
DECLARE_CONFIG_OBJECT_BEGIN(ConfigSwornBuff)

#define CONFIG_CLASS_NAME ConfigSwornBuff
#define PROPLIST      \
		DECLARE_CONFIG_PROP_FIELD_EX(ID, "������ID", 1, size_t )	\
		DECLARE_CONFIG_PROP_FIELD(BuffID, "buffid", string) 

#include "utils/util_config_template.h"

DECLARE_CONFIG_OBJECT_END()

// ��Ӻ�������
DECLARE_CONFIG_OBJECT_BEGIN(ConfigTeamAddIntimacy)

#define CONFIG_CLASS_NAME ConfigTeamAddIntimacy
#define PROPLIST      \
		DECLARE_CONFIG_PROP_FIELD_EX(ID, "������ID", 1, size_t )	\
		DECLARE_CONFIG_PROP_FIELD_EX(DailyLimit, "ÿ�ջ�ȡ����",0,size_t )  \
		DECLARE_CONFIG_PROP_FIELD_EX(TeamOffer, "����",0,size_t )  \
		DECLARE_CONFIG_PROP_FIELD_EX(TeamSecret, "��ӽ���",0,size_t )  \
		DECLARE_CONFIG_PROP_FIELD_EX(TeamInbreak, "��֯����",0,size_t )  \
		DECLARE_CONFIG_PROP_FIELD_EX(TeamEscort, "���Ѻ��",0,size_t )  \
		DECLARE_CONFIG_PROP_FIELD_EX(TeamKillNpc, "Ұ��һ�",0 ,size_t)  


#include "utils/util_config_template.h"

DECLARE_CONFIG_OBJECT_END()


#endif