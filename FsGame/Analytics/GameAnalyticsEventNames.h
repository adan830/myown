//--------------------------------------------------------------------
// �ļ���:	GameAnalyticsEventNames.h
// ��  ��:	��Ϸ�����¼���
// ˵  ��:	
// ��������:	2016��08��24��
// ������:	���
// ������:            
//--------------------------------------------------------------------
#ifndef __GAME_ANALYTICS_EVENT_NAMES_H__
#define __GAME_ANALYTICS_EVENT_NAMES_H__

#define  GAME_ANALYTICS_EVENT_GAME_START		"game_start"	// ��ʼ��Ϸ
#define  GAME_ANALYTICS_EVENT_GAME_END			"game_end"		// �ر���Ϸ
#define  GAME_ANALYTICS_EVENT_SET_CLIENT		"set_client"	// ���ÿͻ�����Ϣ

#define  GAME_ANALYTICS_EVENT_NEW_USER			"new_user"		// ע���˺�
#define  GAME_ANALYTICS_EVENT_DEL_USER			"del_user"		// ɾ���˺�

#define  GAME_ANALYTICS_EVENT_SIGNIN			"signin"		// ��½
#define  GAME_ANALYTICS_EVENT_SIGNOUT			"signout"		// �˳�

#define  GAME_ANALYTICS_EVENT_NEW_ROLE			"new_role"		// ������ɫ
#define  GAME_ANALYTICS_EVENT_DEL_ROLE			"del_role"		// ɾ����ɫ
#define  GAME_ANALYTICS_EVENT_ROLE_ENTRY		"role_entry"	// ��ɫ����
#define  GAME_ANALYTICS_EVENT_ROLE_EXIT			"role_exit"		// ��ɫ�˳�

#define  GAME_ANALYTICS_EVENT_U_NEW_ROLE		"u_new_role"	// �û�������ɫ
#define  GAME_ANALYTICS_EVENT_U_ROLE_ENTRY		"u_role_entry"	// �û���ɫ����

#define  GAME_ANALYTICS_EVENT_PAY_START		"pay_start"			// ֧����ʼ
#define  GAME_ANALYTICS_EVENT_PAY_END		"pay_end"			// ֧������

#define  GAME_ANALYTICS_EVENT_INC_EXP		"inc_exp"			// ����ֵ����
#define  GAME_ANALYTICS_EVENT_LEVELUP		"levelup"			// �������
#define  GAME_ANALYTICS_EVENT_VIPUP			"vip_up"			// vip����
#define  GAME_ANALYTICS_EVENT_INC_COIN		"inc_coin"			// ���ӻ���
#define  GAME_ANALYTICS_EVENT_DEC_COIN		"dec_coin"			// ���Ļ���
#define  GAME_ANALYTICS_EVENT_GAIN_ITEM		"gain_item"			// �����Ʒ
#define  GAME_ANALYTICS_EVENT_COST_ITEM		"cost_item"			// ������Ʒ
#define  GAME_ANALYTICS_EVENT_PROPERTY_CHANGED "property_change"// ����ֵ�仯
#define  GAME_ANALYTICS_EVENT_TRANSZCTION	"transaction"		// �����¼�
#define  GAME_ANALYTICS_EVENT_GUIDE			"guide"				// �����¼�
#define  GAME_ANALYTICS_EVENT_MERIDIAN_UP	"meridian_up"		// ������������

#define  GAME_ANALYTICS_EVENT_TASK_ACCEPT	"task_accept"		// ����ʼ
#define  GAME_ANALYTICS_EVENT_TASK_COMPLETE	"task_complete"		// �������
#define  GAME_ANALYTICS_EVENT_TASK_SUBMIT	"task_give"		// �����ύ

#define  GAME_ANALYTICS_EVENT_COPY_SCENE_OPEN	"cp_scene_open"		// ��������
#define  GAME_ANALYTICS_EVENT_COPY_SCENE_ENTRY	"cp_scene_entry"	// ���븱��
#define  GAME_ANALYTICS_EVENT_COPY_SCENE_EXIT	"cp_scene_exit"		// �˳�����

#define  GAME_ANALYTICS_EVENT_VISIT_FUNCTION	"visit_func"		// ���ʹ���

#define  GAME_ANALYTICS_EVENT_ONLINE_COUNT		"online_count"	// ��������


// ��չ�¼�
#define GAME_EXTRA_EVENT_SKILL_LEVELUP	"skill_levelup"		// ��������

// ����
#define GAME_EXTRA_EVENT_TASK_OP	"task_op"				// �������
#define GAME_EXTRA_EVENT_TASK_OP_ONE_KEY	"task_op_onekey"// ����һ�I���
#define GAME_EXTRA_EVENT_TASK_OP_REFRESH_STAR	"task_op_refresh_star"// ˢ���Ǽ��΄�
#define GAME_EXTRA_EVENT_TASK_OP_REFRESH	"task_op_refresh"// ˢ���΄�
#define GAME_EXTRA_EVENT_ACHIEVE	"achieve_completed"		// �ɾ����
#define GAME_EXTRA_EVENT_ACHIEVE_REWARD	"achieve_reward"	// �ɾͽ�����ȡ

// ����
#define GAME_EXTRA_EVENT_FRIEND_OP	"friend_op"				// ���Ѳ���
#define GAME_EXTRA_EVENT_SND_GIFT	"friend_snd_gift"		// �����ͻ�
#define GAME_EXTRA_EVENT_RCV_GIFT	"friend_rcv_gift"		// �����ջ�
#define GAME_EXTRA_EVENT_TEAM_FIGHT "friend_team_fight"		// ������Ӵ���������ܶ�
#define GAME_EXTRA_EVENT_CAHRMS_CHANGED "charms_changed"	// ����ֵ�����仯
#define GAME_EXTRA_EVENT_SWORN		"friend_sworn"			// ���ѽ����Ե
//װ��
#define GAME_EXTRA_EVENT_EQUIP_STRENGTHEN "equip_strengthen" //װ��ǿ��
#define GAME_EXTRA_EVENT_EQUIP_FORGE "equip_forge"				  //װ������
#define GAME_EXTRA_EVENT_EQUIP_BAPTISE "equip_baptise"			//װ��ϴ��
#define GAME_EXTRA_EVENT_EQUIP_SMELT "equip_smelt"				//����

//��ʯ����
#define GAME_EXTRA_EVENT_JEWEL_INLAY	"jewel_inlay"				//��ʯ��Ƕ
#define GAME_EXTRA_EVENT_JEWEL_UP		"jewel_up"					//��ʯ����
#define GAME_EXTRA_EVENT_JEWEL_DISMANTLE "jewel_dismantle"		//��ʯж��
// ����
#define GAME_EXTRA_EVENT_GUILD_EVENT		"guild_event"		// ��������¼�
#define GAME_EXTRA_EVENT_GUILD_PROMOTION	"guild_promotion"	// �����Ա����
#define GAME_EXTRA_EVENT_GUILD_DEMOTION		"guild_demotion"	// �����Ա��ְ
#define GAME_EXTRA_EVENT_GUILD_DONATE		"guild_donate"		// �������
#define GAME_EXTRA_EVENT_GUILD_DONATE_COST	"guild_donate_cost"	// �ﹱ����

// ����
#define GAME_EXTRA_EVENT_PET_ACTIVE			"pet_active"		// ��������
#define GAME_EXTRA_EVENT_PET_UPGRADE		"pet_upgrade"		// ��������
#define GAME_EXTRA_EVENT_PET_SKILLUP		"pet_skill_up"		// ������������

// ����
#define GAME_EXTRA_EVENT_RIDE_ACTIVE		"ride_active"		// ���Ｄ��
#define GAME_EXTRA_EVENT_RIDE_FEED			"ride_feed"			// ��������
#define GAME_EXTRA_EVENT_RIDE_UPGRADE		"ride_upgrade"		// ��������
#define GAME_EXTRA_EVENT_RIDE_SKILL_UPGRADE	"ride_skill_upgrade"// ���＼������
#define GAME_EXTRA_EVENT_RIDE_SKIN			"ride_skin"			// ����Ƥ��

// ��λ
#define GAME_EXTRA_EVENT_PEERAGE_UPGRADE	"peerage_upgrade"	// ��λ����

// Ѻ��
#define GAME_EXTRA_EVENT_ESCORT_START		"escort_start"		// Ѻ����ʼ
#define GAME_EXTRA_EVENT_ESCORT_END			"escort_end"		// Ѻ������

// �������
#define GAME_EXTRA_EVENT_TEAMOFFER_END			"teamoffer_end"		// ������ͽ���

// ӡ��
#define GAME_EXTRA_EVENT_MARKER_EXCH		"marker_exch"		// ӡ�Ƕһ�
#define GAME_EXTRA_EVENT_MARKER_ACTIVE		"marker_active"		// ӡ�Ǽ���
#define GAME_EXTRA_EVENT_MARKER_BAPTISE		"marker_baptise"	// ӡ��ϴ��
#define GAME_EXTRA_EVENT_MARKER_LEVELUP		"marker_levelup"	// ӡ������
//�����輧
#define GAME_EXTRA_EVENT_AWARD_DANCING_GIRL   "award_girl"      // �����輧
#define GAME_EXTRA_EVENT_SUMMER_DANCING_GIRL  "summer_girl"		 //�ٻ��輧
//�ؾ������
#define GAME_EXTRA_EVENT_BUY_CLONE_COIN     "buy_clone_coin"    //�������
// ������
#define GAME_EXTRA_EVENT_ARENA_GAME_COST	"arena_game_cost"	// ����������   LOG_ARENA_COST_TYPE
#define GAME_EXTRA_EVENT_ARENA_GAME_RANK	"arena_game_rank"	// �����������仯

// ����
#define GAME_EXTRA_EVENT_OFFER_REWARD_ISSUE		"offer_reward_issue"	// ���ͷ�������¼�
#define GAME_EXTRA_EVENT_OFFER_REWARD_ACCEPT	"offer_reward_accpet"	// ������ȡ����¼�

// �̵�
#define GAME_EXTRA_EVENT_SHOP_REFRESH			"shop_refresh"				// �̵�ˢ��
#define GAME_EXTRA_EVENT_SHOP_EXCHANGE			"shop_exchange"				// ��Ʒ�һ� 
#define GAME_EXTRA_EVENT_SHOP_BUY_WELFARE_CARD	"shop_buy_welfare_card"		// ��������

// ������
#define GAME_EXTRA_EVENT_RA_CHALLENGE		"ra_challenge"		// ��ս������
#define GAME_EXTRA_EVENT_RA_SAY_GOOD		"ra_say_good"		// ��ս���к�

// ��Ӫ
#define GAME_EXTRA_EVENT_SELECT_CAMP		"select_camp"		// ѡ����Ӫ
// ����
#define  GAME_EXTRA_EVENT_AUCTION_TRANS		"auct_trans"		// �����н���

//ʦͽ
#define GAME_EXTRA_MASTER_APRTC_ADD		"masteraprtc_add"   // ���ʦ��ͽ��
#define GAME_EXTRA_MASTER_APRTC_DEL		"masteraprtc_del"   // ɾ��ʦ��ͽ��
#define GAME_EXTRA_MASTER_ATRTC_FINISH	"masteraptrtc_finish" //��ʦ

//��������
#define GAME_EXTRA_EVENT_SKIP_UP_START_SCENE		"skip_up_start_scene"			// ������������

// ����/���
#define GAME_EXTRA_EVENT_FORBID_SPEAK		"forbid_speak"		// ����
#define GAME_EXTRA_EVENT_ALLOW_SPEAK		"allow_speak"		// �������
#define GAME_EXTRA_EVENT_BLOCK_PLAYER		"block_player"		// ���
#define GAME_EXTRA_EVENT_RESUME_PLAYER		"resume_player"		// ������

//limit scene

#define GAME_EXTRA_EVENT_ENTRY_FRO_LIMIT_SCENE			  "fro_entry_limit_scene"//������ʱ����
#define GAME_EXTRA_EVENT_FRO_GET_LIMIT_SCENE_AWARD        "fro_get_limit_scene_award"//��ó������佱��

//��ҹ��
#define GAME_EXTRA_DARD_CITY_KILL_BOSS		"dard_city_kill_boss"	// ��ҹ�ǻ�ɱBOSS

// ҹϮ����
#define GAME_EXTRA_FRONTIER_TREASURY_AUCTION "frontier_treas_auction"	// �������ۻ��

// ��ʱ���
#define GAME_EXTRA_LIMIT_GIFT_BOUGHT "limit_gift_bought"	// ������ʱ���
// �ɳ�����
#define GAME_EXTRA_FUND_BOUGHT		"fund_bought"	// ����ɳ�����
// �������
#define GAME_EXTRA_SEVEN_GIFT_BOUGHT "seven_gift_bought"	// �����������

#endif