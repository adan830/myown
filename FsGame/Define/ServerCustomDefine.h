// ��������Ϣ����
#ifndef _SERVER_CUSTOM_DEFINE_H_
#define _SERVER_CUSTOM_DEFINE_H_

enum
{
#pragma region //CommonModule (1-100)
	// ϵͳ��Ϣ����ʽ��int msgid,  ...(������)
	SERVER_CUSTOMMSG_SYSINFO = 1,

	/*!
	* @brief	����������Ϣ
	* @param	int			channeltype(0˽�ģ�1���ӷ�Χ��2������3����)
	* @param	int			contenttype (0���� 1����)
	����-> @param	wstring		content @param	wstring		playername
	����-> @param	int64		voicemsgid  @param	int		voicemsgtime  @param	wstring		playername 
	*/
	SERVER_CUSTOMMSG_SPEECH,

	//��ͻ��˷����Զ��������������ݣ���Ϣ��ʽ��
	//unsigned msgid, string recname, ...(��һ������), ..., ...(��n������)
	SERVER_CUSTOMMSG_VIRTUALREC_ADD,

	//֪ͨ�ͻ�������Զ��������������ݣ���Ϣ��ʽ��unsigned msgid, string recname
	SERVER_CUSTOMMSG_VIRTUALREC_CLEAR,

	//֪ͨ�ͻ��˸����Զ�����������ĳ��ĳ�е����ݣ���Ϣ��ʽ��
	//unsigned msgid, string recname, int row(�к�), int col(�к�),(����)
	SERVER_CUSTOMMSG_VIRTUALREC_UPDATE,

	//֪ͨ�ͻ��˸����Զ�����������ĳ�е����ݣ���Ϣ��ʽ��
	//unsigned msgid, string recname, int row(�к�), ...(������)
	SERVER_CUSTOMMSG_VIRTUALREC_UPDATE_ROW,

	//֪ͨ�ͻ���ɾ���Զ�����������ĳ�У���Ϣ��ʽ��
	//unsigned msgid, string recname, int row(�к�)
	SERVER_CUSTOMMSG_VIRTUALREC_REMOVE_ROW,

	//�Զ�����Ч����Ϣ��ʽ��unsigned msgid, object target, ...(�ɿͻ��˳���ȷ��),���ֻ��һ���������Ϊ�����ı䣬ֻ����object target ���ID
	//���ڼ��ܻ���BUFF������ͳһ�ɿͻ��˶�ȡ�����ļ���������ʽ
	SERVER_CUSTOMMSG_EFFECT,// "effect";

	/*!
	 * @brief	��ʾЧ����Ϣ
	 * @param	int	��Ϣ��������(CustomDisplayType)
	 * @param	PERSISTID	Ŀ�����
	 ����Ϊ�Զ������
	 * @param	CVarlist
	 */
	SERVER_CUSTOMMSG_DISPLAY,
    
    //����������ص���Ϣ, ����ϢID�� ContainerDefine.h
    SERVER_CUSTOMMSG_CONTAINER,

	//������ʱ��Ϣ
	SERVER_CUSTOMMSG_DELAY_TIME,
	
	//���뿨��
	SERVER_CUSTOMMSG_ESCAPE_LOCK,
	//��������
	SERVER_CUSTOMMSG_QUERY_PUSH_CONFIG,

    // ��SNS ������ص���Ϣ
    SERVER_CUSTOMMSG_SNS_DATA,
    
	// ��SNS ����ģ����ص���Ϣ
	SERVER_CUSTOMMSG_SNS_PUSH_MODULE,

	// ������������ʷ��Ϣ ��ʽ: int channel, wstring content, wstring name
	SERVER_CUSTOMMSG_HISTORY_SPEECH,

	// ս���������
	SERVER_CUSTOMMSG_BATTLE_APPLY_MODULE,

	// �رն�������
	SERVER_CUSTOMMSG_CONNECT_CLOSE,

    //�ͻ���ת����̨
    SERVER_CUSTOMMSG_BACKGROUND,

	// gmcc��Ϣ
	SERVER_CUSTOMMSG_GMCC,

	// ������ɫ������Ϣ
	SERVER_CUSTOMMSG_SWITCH_ROLE,

	// ��ȡ���������������Ϣ args: ���(0�ɹ� 1 ��Ҳ�����) �ͻ��Զ����ʶ �������������+����+ֵ�� ....
	SERVER_CUSTOMMSG_GET_PLAYER_PROP_RSP,

	/*!
	* @brief	����CG
	* @param	string CGID
	* @param	args �ɱ��������Ҫʱ���䣩
	*/
	SERVER_CUSTOMMSG_PLYA_CG,

	// �����ѯ���
	SERVER_CUSTOMMSG_CHAT_QUERY_RST,
#pragma endregion

#pragma region //SystemFunctionModule(101-200)

    // �����а���ص���Ϣ��ID, ����ϢID�� RankingDefine.h
    SERVER_CUSTOMMSG_RANKING = 101,

    // ����Ʒ������ص���Ϣ
	// @param	int64	��������uid
	// @param	string	������䣬��ͨ����Ϊ""	["box_drop"�������]
	// @param	string	��Ʒid
	// @param	int		��Ʒ����
    SERVER_CUSTOMMSG_ITEM_DROP,

	// ���Ҷһ��ɹ�	 int nType int nNum
	SERVER_CUSTOMMSG_CAPITAL_EXCHANGE_SUC,

	//��Ʒ���
	SERVER_CUSTOMMSG_GET_ITEM,

	// ��ȡ��Ʒ����
	SERVER_CUSTOMMSG_ITEM_REMIND,

	// ����ϵͳʱ���
	SERVER_CUSTOMMSG_SERVER_TIME,

	// ��������ص���Ϣ, ������Ʒ��ʹ��, ���۵ȵ�
	SERVER_CUSTOMMSG_TOOL_BOX,

	// �̵������Ϣ
	SERVER_CUSTOMMSG_SHOP_INFO,

	// ��Ʒչʾ
	SERVER_CUSTOMMSG_SHOWOFF_ITEM,

	// ����
	SERVER_CUSTOMMSG_REVIVE,

	// ���
	SERVER_CUSTOMMSG_WING,

	// ����
	SERVER_CUSTOMMSG_RIDE,
#pragma endregion

#pragma region //SocialSystemModule(301-600)
 	// ��������·���Ϣ����Ϣ string msg_id, int sub_id, ...
 	SERVER_CUSTOMMSG_GUILD = 301,
    
    // �û��ʼ�
    SERVER_CUSTOMMSG_SYSTEM_MAIL,

	// �л�������Ч��Ϣ args << 0���н���/1���г���
	SERVER_CUSTOMMSG_SWITCH_GROUP_EFFECT,

	// ���ѹ���
	SERVER_CUSTOMMSG_FRIEND,

	//��ӹ���
	SERVER_CUSTOMMSG_TEAM,
#pragma endregion

#pragma region //ItemModule(601-700)
	// ����ϵͳ
	SERVER_CUSTOMMSG_JADE = 601,
#pragma endregion

#pragma region //NpcBaseModule(701-800)
    //������BOSS��Ϣ
    SERVER_CUSTOMMSG_BOSS_INFO = 701,
#pragma endregion

#pragma region //SceneBaseModule(1001-1100)

#pragma endregion

#pragma region //SkillModule(1101-1200)
	//����������Ϣ����Ϣ��ʽ��unsigned msgid, unsigned submsgid, object target, ...
	SERVER_CUSTOMMSG_SKILL  = 1101,// "skill";

	/*!
	 * @brief	������ʹ����ĳ���������λ�Ƶ�λ��
	 * @param	int64 	���ܵ�uid
	 * @param	string	Ч��id
	 * @param	PERSISTID ���ܵ�Ŀ��
	 */
	SERVER_CUSTOMMSG_SKILL_REQUEST_TARGET_POS,

	/*!
	 * @brief	ʹ�ü���ʱ,���������λ��
	 * @param	int64 	���ܵ�uid
	 * @param	PERSISTID λ�Ƶ�Ŀ��
	 */
	SERVER_CUSTOMMSG_SKILL_LOCATE_OBJECT,

	/*!
	 * @brief	����������Ч������
	 * @param	int64 	���ܵ�uid
	 * @param	int 	�����ܴ��� 
	 * @param	int 	��������n
	 ����ѭ��n��
	 * @param	PERSISTID ����Ŀ��
	 * @param	int	��������
	 * @param	int	�˺�ֵ
	 */
	SERVER_CUSTOMMSG_SKILL_RANDOM_MULTI_ATTACK,

	/*!
	 * @brief	NPC����,���Ż���Ч��
	 * @param	PERSISTID ���ɵ�Ŀ��
	 */
	SERVER_CUSTOMMSG_SKILL_DEAD_EFFECT,

	/*!
	 * @brief	�ͻ�������ʧ����Ϣ,��Ϊ����ʹ��
	 * @param	wstring ʩ��������
	 * @param	string ����id
	 * @param	int ʧ��ԭ��(0 ʱ����֤ʧ�� 1����������֤ʧ�� 2������֤ʧ��)
	 * @param	float x,z
	 * @param	int ����Ŀ�����n
	 ����ѭ��n��
	 *  @param	PERSISTID Ŀ��
	 *  @param	float x,z
	 */
	SERVER_CUSTOMMSG_SKILL_HIT_FAILED,

	/// ��������, , ����ϢID�� SkillDefine.h
	SERVER_CUSTOMMSG_SKILL_UPGRADE,

	// PKģʽ
	SERVER_CUSTOMMSG_PK_MODEL,
#pragma endregion

#pragma region //TaskModule(1301-1400)
	// ����ϵͳ����Ϣ string msg�� int subType.....
	SERVER_CUSTOMMSG_TASK_MSG = 1301,
#pragma endregion

#pragma region //CampaignModule(1401-1500)

    // ����������Ϣ
    SERVER_CUSTOMMSG_ARENA_MSG = 1401,

	// ����BOSS����
	SERVER_CUSTOMMSG_WORLD_BOSS_ACTIVE,

	// ����ս���߼���Ϣ
	SERVER_CUSTOMMSG_ASURA_BATTLE,

	// ���������Ϣ
	SERVER_CUSTOMMSG_TEAMOFFER,

#pragma endregion

#pragma region //PayModule(1501-1600)
	// ��ֵ
	SERVER_CUSTOMMSG_PAY = 1501,
	// �̳�
	SERVER_CUSTOMMSG_SHOP,
    //VIP����������ܴ���ͳһ���� [��Ϣͷ][������Ϣ][���][�ѹ������][�ɹ����ܴ���]
    SERVER_CUSTOMMSG_BUY_INFO,
#pragma endregion

#pragma region //EquipmentModule(1601-1650)
	// ��ʯ�����Ϣ
	SERVER_CUSTOMMSG_JEWEL = 1601,

	// װ��ǿ���ɹ���Ϣ
	SERVER_CUSTOMMSG_STRENTHEN,

	// װ��������Ϣ
	SERVER_CUSTOMMSG_SMELT,

	// װ������
	SERVER_CUSTOMMSG_FORGING,

	// ʱװ��Ϣ
	SERVER_CUSTOMMSG_FASHION,
#pragma endregion

#pragma region //OPActivity��Ӫ�(1651-1700)
#pragma endregion
};

#endif