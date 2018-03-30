#ifndef __COMMANDDEFINE_H__ 
#define __COMMANDDEFINE_H__

enum
{
#pragma region //CommonModule(1-100)
	//GMCC����
	COMMAND_GMCC,

	// ����ƶ�
	COMMAND_PLAYER_MOTION,

    // SNS ���������Ϣ
    COMMAND_SNS_DATA,

	/*!
	* @brief	�޸�ѭ�������� 
	* @param	int count ������
	*/
	COMMAND_SET_MAX_CIRCULATE_COUNT,

	//���û�����ʶ
	COMMAND_RESET_ACTION_MUTEX,

	// �ͻ�������ָ�������������
	COMMAND_CLIENT_QUERY_PROP_DATA,

	/*!
	* @brief	��ҷ���ı�
	* @param	int �ı䷽ʽ��GroupChangeType��
	*/
	COMMAND_GROUP_CHANGE,

	// ֪ͨ��һ�ɱNpc�����ض�����
	COMMAND_KILL_NPC_WITH_RULE,

	// ֪ͨ��һ�ɱ��ң����ض�����
	COMMAND_KILL_PLAYER_WITH_RULE,

	/*!
	* @brief	Cg���Ž���
	* @param	string CGID
	*/
	COMMAND_PLAY_CG_DONE,
#pragma endregion

#pragma region //SystemFunctionModule(101-200)
    // �����а���ص���Ϣ��ID, ����ϢID�� RankingDefine.h
    COMMAND_RANK_LIST = 101,

	//��ҵȼ��仯 �����ʽ��usgined cmdid, int oldlevel
	COMMAND_LEVELUP_CHANGE,

	// �
	COMMAND_DYNAMIC_ACT,

	// ��ֵ
	COMMAND_MSG_PAY,

    // ���������ļ�GM����
    COMMAND_MSG_REREAD_CFG_GM,

	// �ַ�GM����
	COMMAND_MSG_DISPATCH_GM,

	//�̵�����
	COMMAND_MSG_SHOP,

	// ����roleinfo
	COMMAND_SAVE_ROLE_INFO,

	// �������
	COMMAND_WING_LEVEL_UP,
	
	// ս�������� �����ʽ��usgined cmdid, int nType(BatttleAbilityType)
	COMMAND_MSG_UPDATE_BATTLE_ABILITY,

	// ���ܿ��� �����ʽ��usgined cmdid,int nFuncType(AFMFunction)
	COMMAND_MSG_ACTIVATE_FUNCTION,

	// ��һ�ν�����Ϸ
	COMMAND_MSG_FIRST_ENTER,
#pragma endregion

#pragma region //SocialSystemModule(201-300)
	//����
	COMMAND_FRIEND_MSG = 201,

	// �����ż�unsigned cmd_id
	COMMAND_QUERY_LETTER,

	// ����
	COMMAND_CHAT,

	// ���
	COMMAND_TEAM_MSG,
	//���֮�佻������RequestModule
	//�ٴ�ȷ�����������ʽ��usgined cmdid, int request_type, widestr targetname, int result  ...(����:��ʱֻ������õ�)
	COMMAND_GET_ANSWER_PARA,
	//�ٴ�ȷ�ϱ����������ʽ��usgined cmdid, int request_type, widestr targetname, int result(0:ʧ�ܣ�1:�ɹ�,-1:�ܾ�����)
	COMMAND_RET_ANSWER_PARA,
	// ��Ӧ������ҷ��͵����������ʽ��usgined cmdid, int request_type, widestr srcname, ...(����������request_type�Ĳ�ͬ����ͬ)
	COMMAND_BEREQUEST,
	// ��������һظ��������������ʽ��usgined cmdid, int request_type, widestr targetname, int result;
	COMMAND_REQUEST_ANSWER,
	// ��Ӧ������ҷ��͵�����Ļظ�(COMMAND_BEREQUST���ܻᱻ�첽����ͨ���������ж��Ƿ���ȷ����Է�����������߼�)
	// �����ʽ��usgined cmdid, int request_type, widestr targetname, int result(0:ʧ�ܣ�1:�ɹ�,-1:�ܾ�����)
	COMMAND_BEREQUEST_ECHO,
	//�ٴ�ȷ�����������ʽ��usgined cmdid, int request_type, widestr targetname, int result  ...(����:��ʱֻ������õ�)
	COMMAND_GET_REQUEST_PARA,
	//�ٴ�ȷ�ϱ����������ʽ��usgined cmdid, int request_type, widestr targetname, int result(0:ʧ�ܣ�1:�ɹ�,-1:�ܾ�����)		
	COMMAND_RET_REQUEST_PARA,
	// �����������
	// ��������ɹ�
	COMMAND_TEAM_CREATE,
	// �������
	COMMAND_TEAM_JOIN,
	// ��ӳ�Ա
	COMMAND_TEAM_ADD_MEMBER,
	// ɾ����Ա
	COMMAND_TEAM_REMOVE_MEMBER,
	// ���������Ϣ
	COMMAND_TEAM_CLEAR,
	// ���³�Ա��Ϣ
	COMMAND_TEAM_UPDATE_MEMBER_INFO,
	// ���¶�����Ϣ
	COMMAND_TEAM_UPDATE_TEAM_INFO,
	// �����Աλ�û�ְλ�ı�
	COMMAND_TEAM_MEMBER_CHANGE,
	// �����Ա����ʱ���״̬�ж�
	COMMAND_TEAM_MEMBER_RECOVER,
	// ��������������
	COMMAND_NEARBYTEAM_CLEAR,
	//�����Ա�Ƿ��ܹ�������淨�ļ��
	COMMAND_TEAM_CHECK_CONDITION,

	// ��������ڲ�����: unsigned cmd_id, unsigned sub_cmd, ...
	COMMAND_GUILD_BY_NAME,
	// �������
	COMMAND_GUILD_UP_LEVEL,
	// ��ή��
	COMMAND_GUILD_DOWN_LEVEL,
#pragma endregion

#pragma region //ItemModule(301-400)

	// ʹ����Ʒ
	COMMAND_USE_ITEM = 301,

#pragma endregion

#pragma region //NpcBaseModule(401-500)
	//�������ӳٴ���
	COMMAND_NPC_AUTO_TRANSFER = 401,

	//NPC����Ӳֱ״̬����
	COMMAND_NPC_STIFF_OVER = 402,

#pragma endregion

#pragma region //SceneBaseModule(601-700)

#pragma endregion

#pragma region //SkillModule(701-800) ս���������
	// ����ɱ�������ʽ��unsigned cmdid(sender:src)
	COMMAND_BEKILL = 701,
	// ��ɱĿ�꣬�����ʽ��unsigned cmdid, object target
	COMMAND_KILLTARGET,

	// playerʹ�ü���ʧ��
	COMMAND_PLAYER_USESKILL_FAILED,

	// ��ʼʹ��ĳ��ܣ������ʽ��unsigned cmdid, object skill
	COMMAND_SKILL_BEGIN,
	// ���ܱ���ϣ������ʽ��unsigned cmdid, object skill
	COMMAND_SKILL_BREAKED,
	// ����ʹ�õļ������̵Ľ�����unsigned cmdid, object skill, object self
	COMMAND_SKILL_FINISH,
	// ʹ�ü�������ǰ��unsigned cmdid, object skill
	COMMAND_SKILL_BEFORE_HIT,

	// ��ʹ�ü��ܳ��Թ�����unsigned cmdid, object target, object skill
	COMMAND_SKILL_TRY_BE_ATTACK,

	// ����Ŀ�꣬�����ʽ��unsigned cmdid, object target
	COMMAND_ATTACKTARGET,
	// �˺�Ŀ��֮ǰ�������ʽ��unsigned cmdid, object target, int damage, int damagetype
	COMMAND_BEFORE_DAMAGETARGET,
	// ���˺�֮ǰ�������ʽ��unsigned cmdid, int damage, int damagetype(sender:src)
	COMMAND_BEFORE_BEDAMAGE,
	// �˺�Ŀ�꣬�����ʽ��unsigned cmdid, object target, int damage, int damagetype
	COMMAND_DAMAGETARGET,

	// ��������ƶ��ڲ����� �����ʽ��unsigned cmdid, motion mode
	COMMAND_REQ_MOVE,

	// �������������ʽ��unsigned cmdid, int damage, int damagetype(sender:src)
	COMMAND_BEDAMAGE,
	//�޸���ǰ��������һ�����ܷ�����COMMAND_BEDAMAGE�����һ��û���˺�������֪ͨ��������
	COMMAND_BEATTACK,

	//������
	COMMAND_BEVA,

	//����δ����	unsigned cmd_id, object target
	COMMAND_HIT_MISS,
	//����			unsigned cmd_id, object target
	COMMAND_SELF_VA,
	//��Ӧ�ͻ��������ϼ��� unsigned cmd_id
	COMMAND_ON_CLIENT_BREAK_SKILL,
	// ��Ӧ���Ա仯��ϼ��� unsigned cmd_id
	COMMAND_CANT_USE_SKILL_BREAK_SKILL,
	/*!
	 * @brief	// ��Ӧ������������������������
	 * @param	unsigned cmd_id, ��Ϣ��
	 * @param	int		����id
	 * @param	int		Ӱ�켼�����Եĸ���n
	 ����ѭ��n��
	 * @param	string  ������
	 * @param	float	value
	 * @return	void
	 */
	COMMAND_PASSIVE_SKILL_ADD_PROP,

	/*!
	 * @brief	// ��Ӧ���������Ƴ�
	 * @param	unsigned cmd_id, ��Ϣ��
	 * @param	int		����id
	 */
	 COMMAND_PASSIVE_SKILL_REMOVE_PROP,

	 /*!
	 * @brief	// ��Ӧ������������,Ӱ��������������
	 * @param	unsigned cmd_id, ��Ϣ��
	 * @param	int 0ɾ��1��ӻ�����
	 * @param	string	Ӱ����������id
	 * @param	string	Ӱ������
	 */
	 COMMAND_PASSIVE_SKILL_AFFECT_SKILL_CONFIG,
#pragma endregion

#pragma region //TaskModule(801-850)

	/*!
	 * @brief	��������Ϣ
	 * @param int ״̬��TaskActions��
	 * @param int ����ID
	 */
	 COMMAND_TASK_ACTION_MSG = 801,

	// ���������Ϣ
	COMMAND_TASK_COMMAND,

#pragma endregion

#pragma region //EquipmentModule(851-900)
	// ˢ��װ��ս����
	COMMAND_EQUIP_ABILITY = 851,

	// װ��ǿ���ȼ��仯
	COMMAND_EQUIP_STRENGTHEN_CHANGE_LEVEL,

	// װ�����������������ı�
	COMMAND_EQUIPMENT_ADDMODIFY,

	// ���±�ʯ���԰�
	COMMAND_EQUIPMENT_JEWEL_PKG,

	// װ��ǿ����װ���԰�
	COMMAND_EQUIP_STR_SUIT_PKG,

	// װ��ǿ�����԰�
	COMMAND_EQUIP_STRENGTHEN_PKG,

	// װ��ϴ�����԰�
	COMMAND_EQUIP_BAPTISE_PKG,

	// ������԰�
	COMMAND_APPERANCE_PKG,

#pragma endregion

#pragma region //OPActivity��Ӫ�(901-951)
#pragma endregion


// ע�⣺��ϢID���ܳ���1024, ��������ID��1023

};

#endif //__COMMANDDEFINE_H__
