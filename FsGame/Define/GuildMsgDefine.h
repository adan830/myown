//--------------------------------------------------------------------
// �ļ���:		GuildMsgDefine.h
// ��  ��:		����ϵͳ-��Ϣ����
// ˵  ��:		
// ��������:	2014��06��10��
// ��������:	2014��06��10��
// ������:		  ( )
// �޸���:     
//    :	    
//--------------------------------------------------------------------

#ifndef __GuildMsgDefine_H__
#define __GuildMsgDefine_H__

// �ͻ�������������͵���Ϣ
enum CSGuildMsg
{
	CS_GUILD_CREATE            	= 0,  				// �������� wstring������
	CS_GUILD_GUILD_LIST        	= 1,  				// ���󹫻��б� int �ڼ�ҳ0(0��ҳ)...
    CS_GUILD_SEARCH            	= 2,  				// �������� wstring������
	CS_GUILD_APPLY_JOIN        	= 3,  				// ������빫�� wstring������
    CS_GUILD_CANCEL_APPLY_JOIN 	= 4,  				// ����ȡ�����빫������ wstring������
    CS_GUILD_ACCEPT_APPLY		= 5,  				// ���ܼ��빫������� wstring�������
    CS_GUILD_REFUSE_APPLY		= 6,  				// �ܾ����빫������� wstring�������
    CS_GUILD_GUILD_INFO			= 7,  				// ���󹫻���Ϣ
    CS_GUILD_MEMBER_LIST		= 8,  				// �����Ա�б�
    CS_GUILD_APPLY_LIST			= 9,  				// �����������б�
    CS_GUILD_NOTICE            	= 10, 				// �����޸Ĺ��� wstring����
    CS_GUILD_QUIT              	= 11, 				// �����˳�����
    CS_GUILD_FIRE              	= 12, 				// �����߳����� wstring�����
    CS_GUILD_DISMISS           	= 13, 				// �����ɢ����
    CS_GUILD_DONATE            	= 14, 				// ������� int����id
    CS_GUILD_PROMOTION         	= 15, 				// ������ְ wstring�����
    CS_GUILD_DEMOTION          	= 16, 				// ����ְ wstring�����
    CS_GUILD_SHOP_LIST         	= 17,				// ���󹫻��̵��б�
    CS_GUILD_BUY_ITEM          	= 18, 				// �������̵���Ʒ wstring���ߺ�, int ����
    CS_GUILD_CAPTAIN_TRANSFER  	= 19,				// �����ƽ� wstring�����
    CS_GUILD_JOIN_CD           	= 20, 				// ������빫��CDʱ��
	CS_GUILD_IS_IN             	= 21, 				// ��������Ƿ��ڹ����У��鿴�����Ƿ��ڹ����У�
	CS_GUILD_LOG				= 22,				// ��������־	int ��� (��һҳ����Я����ֵ, ��һ�������-1)
	CS_GUILD_LOG_DONATE			= 23,				// ���������־ �޲���
	CS_GUILD_DECLARATION      	= 24, 				// �����޸����� wstring����
	//�����ļ� GUILD_IMPEACH_CLIENT_MSG
	CS_GUILD_ONE_KEY_APPLY_JOIN = 31,				// һ�����󹫻�
	CS_GUILD_AUTO_AGREE			= 32,				// �Զ����빫��
	CS_GUILD_UNLOCK_SKILL		= 33,				// �������Ἴ��
	CS_GUILD_GET_SKILL_LIST		= 34,				// ��ȡ���Ἴ���б�
	CS_GUILD_SKILL_LEVEL_UP		= 35,				// ��������
	CS_GUILD_BUILDING_OPERATE_LV  = 42,				// ������������ int �������� int ���� 0 ������1��ͣ
	CS_GUILD_NUM_BUY            = 43,				// ��֯��ֵ���� int   ��ֵ����
	CS_GUILD_CHANGE_SHORT_NAME	= 44,				// �޸���֯��� wstring ��֯��
	CS_GUILD_GET_JIAN_KANG_DU_GIF = 45,			    // ���������
	CS_GUILD_GET_GUILD_NUM		= 46,				// ������֯��ֵ wstring ��֯��
	CS_GUILD_GET_GUILD_SYSMBOL  = 47,				// ������֯��� wstring ��֯��
	CS_GUILD_GET_GUILD_BUILDING = 48,				// ��֯������Ϣ
	CS_GUILD_GET_GUILD_SET_INFO = 49,				// ��֯������Ϣ
	CS_GUILD_START_OPERATE_SKILL	= 50,			 //��֯�о����ܲ��� string ����id int ���� 0 ������1��ͣ 
	CS_GUILD_USE_RARE_TREASURE     = 51,			 // ʹ���ر� int �ر�����
	CS_GUILD_GET_RARE_TREASURE_LIST     = 52,		 // ʹ���ر��б�
	CS_GUILD_GET_NUM_BUY_RECORD = 53,                //�����֯��ֵ�����¼
	CS_GUILD_GET_BONUS_INFO		= 54,				// ��ȡ�ֺ���Ϣ
	CS_GUILD_GET_WELFARE_INFO       = 55,				// ��ȡ������Ϣ
	CS_GUILD_GIVE_OUT_BONUS		= 56,				 //��֯�ֺ� int �ֺ����ͣ�int ÿ���˷ֶ��٣�
	CS_GUILD_INVITE_JOIN_GUILD  = 57,                // ���������֯ wstring �����
	CS_GUILD_INVITE_JOIN_GUILD_RSP = 58,             // ͬ�����������֯
	CS_GUILD_ADD_PETITION = 59,						 // �����Ը int ����
	CS_GUILD_GET_PETITION_LIST = 60,				// ��ȡ��Ը int ���� (-1 ��ʾ����)
	CS_GUILD_GET_OTHER_GUILD_INFO		= 61,       // ���������֯��Ϣ

	CS_GUILD_TEACH_LIST = 62,			// ���󴫹��б� args: none
	CS_GUILD_TEACH_REQ = 63,			// ����Է����� args: nick[widestr] type[1:req 2:push]
	CS_GUILD_TEACH_ACK = 64,			// ��Ӧ���Է����� args: nick[widestr] type[1:req 2:push] agree[EmTeachAckResult]
	CS_GUILD_TEACH_READY = 65,			// �ͻ��˸��߷�����׼������ args: none

};

// �������ڲ�������Ϣ
enum SSGuildMsg
{
	SS_GUILD_CREATE_RESULT			= 0,        	// ���ᴴ�����
	SS_GUILD_APPLY_JOIN_RESULT		= 1,     		// ������빫��Ľ��
	SS_GUILD_CANCEL_APPLY_RESULT	= 2,     		// ȡ��������빫��Ľ��wstring��������int 0ʧ��1�ɹ�
	SS_GUILD_ACCEPT_RESULT		    = 3,     		// ֪ͨ�����߽�������Ľ�� int 0ʧ��1�ɹ���wstring �����ˣ�wstring������
	SS_GUILD_ACCEPT_JOIN_RESULT		= 4,     		// ֪ͨ�����߼��빫����
	SS_GUILD_REFUSE_JOIN           	= 5,    		// ֪ͨ�����߾ܾ����빫�������
	SS_GUILD_REFUSE_JOIN_ALL		= 6,     		// ֪ͨ�����߾ܾ�ȫ�����빫�������
	SS_GUILD_NOTICE_RESULT			= 7,    		// ֪ͨ�޸Ĺ�����
	SS_GUILD_QUIT_RESULT			= 8,    		// �˳�������
	SS_GUILD_FIRE_RESULT			= 9,    		// �߳�������
	SS_GUILD_DISMISS_RESULT			= 10,     		// ��ɢ������
	SS_GUILD_LEAVE					= 11,    		// ֪ͨ����뿪�˹���
    SS_GUILD_DONATE_RESULT			= 12,    		// ���׽��
	SS_GUILD_MODIFY_DEVOTE			= 13,    		// ֪ͨ����޸Ĺ��׶�
	SS_GUILD_BUY_ITEM_RESULT		= 14,    		// ֪ͨ����䱦������Ʒ�۳�����ֵ���
	SS_GUILD_BUY_GEM_ITEM_RESULT	= 15,    		// ֪ͨ����䱦������Ʒ�۳���������ֵֵ���
    SS_GUILD_PROMOTION_RESULT		= 16,    		// ������ְ
	SS_GUILD_BE_PROMOTION			= 17,			// ְλ����
    SS_GUILD_DEMOTION_RESULT		= 18,    		// ����ְ
	SS_GUILD_BE_DEMOTION			= 19,			// ְλ�½�
    SS_GUILD_CAPTAIN_TRANSFER		= 20,    		// �����ƽ�
	SS_GUILD_CAPTAIN_BE_TRANSFER	= 21,    		// ��Ϊ����
	SS_GUILD_DECLARATION_RESULT		= 22,    		// ֪ͨ�޸����Խ��
	SS_GUILD_NEW_APPLAY				= 23,    		// ֪ͨ���µİ������
	SS_GUILD_SKILL_UNLOCK			= 25,			// ���ܽ���
	SS_GUILD_REFRESH_GUILD_SYSMBOL   = 28,			// ˢ����֯����
	SS_GUILD_ADD_GUILD_BUILDING_EXP = 30,			// ������֯��������
	SS_GUILD_INVITE_JOIN_GUILD      = 31,          //  ���������֯

	SS_GUILD_TEACH_REQUEST = 32,				// ������������ type[��������] sender[widestr �������ǳ�]
	SS_GUILD_TEACH_ACK = 33,					// ������Ӧ���� result[int] type[��������] responser[widestr ��Ӧ���ǳ�]
	SS_GUILD_TEACH_FIN = 34,					// ����������� result[int] type[��������] sender[widestr �������ǳ�]
	SS_GUILD_TEACH_ENT_FAILED = 35,				// ��������ʧ�� type[��������] sender[widestr �������ǳ�]
};
									
// ��������ͻ��˷��͵���Ϣ
enum SCGuildMsg
{
	SC_GUILD_CREATE_RESULT		= 0,                // ���ᴴ����� int 0ʧ��1�ɹ���wstring������
    SC_GUILD_SEARCH_RESULT		= 1,                // ����������� int���� array�����Ϣ��
	SC_GUILD_APPLY_RESULT		= 2,                // ������빫���� int 0ʧ��1�ɹ���wstring������
	SC_GUILD_CANCEL_APPLY		= 3,                // ȡ��������빫��Ľ�� int 0ʧ��1�ɹ���wstring������
    SC_GUILD_ACCEPT_RESULT		= 4,                // ��������Ľ�� int 0ʧ��1�ɹ���intְλ, wstring����������, int�������Ա�, int�����ߵȼ�, 
													// int������ְҵ, int��ǰ���׶�, int����׶�, intս����, int�Ƿ�����, int�������ʱ�� 
    SC_GUILD_REFUSE_RESULT		= 5,                // �ܾ����빫������� int 0ʧ��1�ɹ� wstring����������
    SC_GUILD_GUILD_INFO			= 6,		        // ������Ϣ
	SC_GUILD_MEMBER_LIST		= 7,		        // ��Ա�б� int ���� ����Ϣ��(�ο������Ա�б��У�
    SC_GUILD_APPLY_LIST			= 8,		        // �������б� int ���� ����Ϣ��(�ο������Ա�б��У�
    SC_GUILD_NOTICE_RESULT		= 9,                // �޸Ĺ����� int 0ʧ��1�ɹ� wstring��������
    SC_GUILD_QUIT_RESULT		= 10,               // �˳������� int 0ʧ��1�ɹ�
    SC_GUILD_FIRE_RESULT		= 11,               // �߳������� int 0ʧ��1�ɹ���wstring�߳����������
    SC_GUILD_DISMISS_RESULT		= 12,               // ��ɢ������ int 0ʧ��1�ɹ�
    SC_GUILD_LEAVE_GUILD		= 13,               // �뿪���� wstring������
    SC_GUILD_DONATE_RESULT		= 14,               // ������׽�� int 0ʧ��1�ɹ� int����ȼ� int���ᾭ��ֵ int����ǰ���׶� int��������׶�
    SC_GUILD_PROMOTION			= 15,               // ������ְ��� int 0ʧ��1�ɹ� wstring����� intְλ
    SC_GUILD_DEMOTION			= 16,               // ���ήְ��� int 0ʧ��1�ɹ� wstring����� intְλ
    SC_GUILD_SHOP_LIST			= 17,               // �����̵��б�
    SC_GUILD_BUY_ITEM_RESULT	= 18,				// ������Ʒ��� int 0ʧ��1�ɹ���wstring��Ʒ��
	SC_GUILD_CAPTAIN_TRANSFER	= 19,				// �����ƽ���� int 0ʧ��1�ɹ�, wstring�°������֣�int�Լ��ĵ�ǰְλ
	SC_GUILD_JOIN_CD			= 20,               // ���빫��CDʱ�� intʣ��CD����
	SC_GUILD_IS_IN				= 21,               // ����Ƿ��ڹ����� int 0����1�� wstring ��������
	SC_GUILD_LOG				= 22,				// �·������־ int ���(��һҳ����ʱ����) int ѭ������ {int ��־���� int64 ʱ��� wstring ����,�Զ��ŷָ�}
	SC_GUILD_LOG_DONATE			= 23,				// �·���������־ int ѭ������ {int ��־���� int64 ʱ��� wstring ����,�Զ��ŷָ�}
	SC_GUILD_DECLARATION_RESULT	= 24,               // �޸����Խ�� int 0ʧ��1�ɹ� wstring����
	SC_GUILD_IS_DONATED      	= 25, 				// �Ƿ��Ѿ����׹�, int 0û��1���׹�
	SC_GUILD_NEW_APPLY      	= 26, 				// ���µ����� ,int 0 ���� 1����ʾ
	SC_GUILD_BOSS_DONATE		= 28,				// boss���� ,int 0 ���� 1����ʾ
	SC_GUILD_SKILL_LIST			= 29,				// ���Ἴ���б� string �б�
	SC_GUILD_NUM_VALUE			= 30,			    // ���ᵱǰ��ֵ	int ��ֵ����  int ��ǰֵ  
	SS_GUILD_ISSUE_MUTUAL_LIST	= 32,			   //  �����б�
	SS_GUILD_ISSUE_MUTUAL_RANK	= 33,			   //  �������� 
	SS_GUILD_TOMOROW_SHOP_LIST  = 34,			   //  �����̵����Ʒ�б� int ���� string ��Ʒid
	SS_GUILD_BUILDING_LEVEL_UP  = 35,			   //  �������� int ���
	SS_GUILD_NUM_BUY		    = 36,			   //  ��֯��ֵ����	int ��ֵ���� int ��� int��ǰֵ
	SS_GUILD_CHANGE_GUILD_SYSMBOL     = 37,			// ��֯��ʶ�޸� int 0 ʧ�ܣ�1�ɹ�
	SC_GUILD_GUILD_NUM_INFO     = 38,				//��֯��ֵ���� // [int ��ֵ���� int ��ʹ���ʽ� int �����ʽ�],[...]
	SC_GUILD_GUILD_SYSMBOL      = 39,               //��֯��ʶ��Ϣ int ��֯��ʶ wstring ��֯���
	SC_GUILD_BUILDING_INFO      = 40,				//��֯������Ϣ [int ��������, int �����ȼ� ,int ������ǰ����, int ����״̬],[...]
	SC_GUILD_SET_INFO			= 41,				//��֯������Ϣ ��[GUILD_SET_REC_COL]
	SC_GUILD_JIAN_KANG_DU_GIT   = 42,				//�����Ƚ���[itemid:num,]
	SC_GUILD_SKILL_OPERATE_RET  = 43,               // ���ܲ������ int[���] int[����id] int[���ܵȼ�]
	SC_GUILD_USE_GUILD_RARE_TREASURE     = 44,		// ʹ���ر� int ��� 
	SC_GUILD_RARE_TREASURE_LIST = 45,               // �ر��б� int �������� [int ���� int ����]
	SC_GUILD_NUM_BUY_RECORD    = 46,               // ��֯��ֵ�����¼ int�������� [int ���� int ����] 

	SC_GUILD_CREATED_BROADCAST = 47,				// ����������Ϣ�㲥 args: ������[widestr] ������vip[int]
	SC_GUILD_MEMBER_BEKILL_BROADCAST = 48,			// ��֯��Ա��ɱ�㲥 args: ��֯���[widestr] vip[int] ɱ�����[widestr] vip[int] ����[����id(int), x(float) y(float) z(float)]
	SC_GUILD_MEMBER_JOIN_BROADCAST = 49,			// ��֯��Ա����㲥 args: ��֯���[widestr] vip[int]

	SC_GUILD_BONUS_INFO = 50,						 //��֯�ֺ���Ϣ int �ֺ���ʽ� int�����ʽ� int ���� �������,���������
	SC_GUILD_WELFARE_INFO= 51,						 //��֯������Ϣ int �Ƿ�����ȡ��� ��0δ��ȡ��1����ȡ�� int �ѻ�ȡ��֯�ʽ�int �ѻ�ȡ�����ʽ�
	SC_GUILD_GIVE_OUT_BONUS = 52,					 //��֯�ֺ�ɹ�
	SC_GUILD_INVITE_JOIN_GUILD = 53,				 //���������֯ ����� ��֯��
	SC_GUILD_GET_PETTION_LIST = 54,					 //��Ը�б� int ���� �����
	SC_GUILD_OTHER_GUILD_INFO = 55,					 //������֯��Ϣint ���ٶ� int ��֯�ʽ� int ռ��ǳ�id��0Ϊδռ���κγǳأ� int ��֯��ҹ�ǲ������� [int flagType,int flagID] int ��������[������Ϣ]

	SC_GUILD_BUILDING_LVUP_BROADCAST = 56,			// ���������㲥 args: ����id[int] Level[int]
	SC_GUILD_DECLARATION_BROADCAST = 57,            // �޸����Թ㲥 args: wstring����

	SC_GUILD_TEACH_LIST = 58,           // �����б� args: �������б�[ �û���[int] [uid nick sex job level power].... ] �������б�[ �û���[int] [uid nick sex job level power].... ]
	SC_GUILD_TEACH_TARGET_REQ = 59,     // �Է��������� args: nick[widestr] type[1:req 2:push]
	SC_GUILD_TEACH_RSP_REQUEST = 60,	// ��Ӧ������(��������ͷ����������Ϊ) args: result[0 �ɹ��� ��0������] type[1:req 2:push] nick[widestr]
	SC_GUILD_TEACH_RSP_PASSIVE = 61,	// ��Ӧ������(��������ͷ���ı���������Ϊ) args: result[0 �ɹ��� ��0������] type[1:req 2:push] nick[widestr]
	SC_GUILD_TEACH_PREPARE_PUSH = 62,	// �������㴫����ʼ args:  target[nick sex job level teachtype]
	SC_GUILD_TEACH_PLAY_CG = 63,		// �������㲥����CG args: cgid[int] sender[widestr] reciver[widestr] 
	SC_GUILD_TEACH_COUNT = 64,			// �������� args: sendtime[int] gettime[int]
};





#endif //__GuildMsgDefine_H__