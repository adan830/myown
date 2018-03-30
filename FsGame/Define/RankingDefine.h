//---------------------------------------------------------
//�ļ���:       RankingDefine.h
//��  ��:       ���а�ģ��Ķ���
//˵  ��:       
//          
//��������:      2014��11��06��
//������:         
//�޸���:
//   :         
//---------------------------------------------------------
#ifndef FSGAME_DEFINE_RANKING_DEFINE_H
#define FSGAME_DEFINE_RANKING_DEFINE_H

// ���а��������50��
#define MAX_RANKING_COL_COUNT	50
#define RESERVED_NUMBER			0 // ���������, ������䱣���ֶ�

#define DO_NOT_INSERT			0 // ��Ҫ����
#define INSERT_IF_NOT_EXIST		1 // ��������ھͲ���

// ���а�Ķ�����Ϣ
enum ERankingSubMsg
{
    RANKING_SUB_MSG_QUERY = 1,          // ����������ѯ
    RANKING_SUB_MSG_QUERY_TOP_N,        // ��ѯǰN��, �ѷ���
    RANKING_SUB_MSG_UPDATE_ENTRY,       // ���ݷ����仯, ˢ�����а�
    RANKING_SUB_MSG_DELETE_ENTRY,       // ɾ��һ����¼
    RANKING_SUB_MSG_CLEAR,              // ������а�
    RANKING_SUB_MSG_UPDATE_ENTRY_CAL,   // ��������ID
    RANKING_SUB_MSG_CLEAR_PLANNED,      // ���а��������ݼƻ����
    RANKING_SUB_MSG_DATE,               // ���а����������
    RANKING_SUB_MSG_SWAP,               // �������а��е�����
	RANKING_SUB_MSG_CLEAR_AWARD,        // ���а��������ݼƻ���շ��ͽ���
};

// ���а���ղ���
enum ERankingClear
{
    RANKING_CLEAR_ON_REQUEST = 0, // �����������
    RANKING_CLEAR_ON_DAY = 1,     // ÿ�����
    RANKING_CLEAR_ON_WEEK = 2,    // ÿ��һ��������
    RANKING_CLEAR_ON_MONTH = 3,   // ÿ�µ�һ���������
};

// ����Ķ������ʹ������Ҫ�������

// ���а�
#define PLAYER_LEVEL_RANK_LIST                  "player_level_rank_list"				// �ȼ����а�
#define PLAYER_FIGHT_POWER_RANK_LIST            "player_fight_power_rank_list"			// ս�������а�
#define PLAYER_RED_PACKET_RANK_LIST             "player_red_packet_rank_list"           // ��ҷ��ź������Ԫ�����а�
#define PLAYER_LEVEL_RANK_LIST_30_35            "player_level_rank_list_30_35"			// �ֶεȼ����а�
#define REVENGE_ARENA_RANK_LIST					"revenge_arena_rank_list"				// �����޸���ʤ�����а�																				   
#define OFFER_REWARD_RANK_LIST					"offer_reward_rank_list"				// �������а�
#define CHASE_SUCCESS_RANK_LIST					"chase_success_rank_list"				// �������а�
#define	ARENA_RANK_LIST							"arena_rank_list"						// ������
#define PUB_PLAYER_CHARM_RANK_NAME				"player_charm_rank"						// ����ֵ���а�����
#define PEERAGE_RANK_LIST						"peerage_rank_list"						// ��λ���а�
#define WORLD_TOWER_RANK_LIST					"world_tower_rank_list"					// ���������а�
#define PET_RANK_LIST							"pet_rank_list"							// �������а�
#define GUILD_LEVEL_RANK_LIST					"guild_level_rank_list"					// ����ȼ����а�
#define GUILD_FIGHT_POWER_RANK_LIST				"guild_fight_power_rank_list"			// ����ս�������а�
#define TRADE_SILVER_RANK_LIST					"trade_silver_rank_list"				// �������а�
#define ACHIEVEMENT_RANK_LIST					"achievement_rank_list"					// �ɾ����а�
#define WEEKLYSCORE_RANK_LIST					"weekly_score_rank_list"				// �ܳ����ָ������а�
#define GUILD_WEEKLYSCORE_RANK_LIST				"guild_weekly_score_rank_list"			// �����ܳ��������а�

#define PVP_EXP_RANK_LIST						"player_pvp_exp_list"					// pvp�������а�
#define GLOBAL_PLAYER_GVG_RANK_LIST				"global_player_gvg_rank"				// gvgȫ�����˰�
#define GLOBAL_GUILD_GVG_RANK_LIST				"global_guild_gvg_rank"					// gvgȫ������
#endif
