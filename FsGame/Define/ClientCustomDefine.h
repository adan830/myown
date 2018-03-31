// �ͻ�����Ϣ����
#ifndef _CLIENT_CUSTOM_DEFINE_H_
#define _CLIENT_CUSTOM_DEFINE_H_

enum
{
#pragma region // CommonModule(1-100)
	//����GM�����ʽ��int msgid,
	CLIENT_CUSTOMMSG_GM = 1,

	//Gmcc ָ��
	CLIENT_CUSTOMMSG_GMCC_MSG,

	// ������������ص���ϢID, ����ϢID�� ContainerDefine.h
	CLIENT_CUSTOMMSG_CONTAINER,

	/*!
	* @brief	��ȡ�豸��¼
	* @param	string		clientType
	* @param	string		token
	* @param	wstring		serverName
	* @param	string		AppPackage

	* @param	string		platform ע��ƽ̨
	* @param	int			wifi �Ƿ�ʹ��wifi 1�� 0����
	* @param	string		manufacturer ������
	* @param	string		devid		�ն��豸Ψһ��ʶ
	* @param	string		model		�ն�����
	* @param	string		os			�ն�ϵͳ
	* @param	string		os_ver		�ն�ϵͳ�汾
	*/
	CLIENT_CUSTOMMSG_TOKEN,

	//���뿨��
	CLIENT_CUSTOMMSG_ESCAPE_LOCK,

	// ��SNS ������ص���Ϣ
	CLIENT_CUSTOMMSG_SNS_DATA,

	// ��SNS ����ģ��������ص���Ϣ
	CLIENT_CUSTOMMSG_SNS_PUSH_MODULE,

	// �ͻ������������������
	CLIENT_CUSTOMMSG_HEART_BEAT,

	//�ͻ���ת����̨
	CLIENT_CUSTOMMSG_BACKGROUND,

	// GM���� ����Ϊ���ֽ�
	CLIENT_CUSTOMMSG_GM_MULTIBYTE,

	/*!
	* @brief	CG���Ž���
	* @param	string CGID
	*/
	CLIENT_CUSTOMMSG_PLAY_CG_DONE,
#pragma endregion

#pragma region //SystemFunctionModule(101-200)

	// �����а���ص���Ϣ��ID, ����ϢID�� RankingDefine.h
	CLIENT_CUSTOMMSG_RANKING = 101,

	//��ʵ���䣬��Ʒʰȡ��Ϣ
	CLIENT_CUSTOMMSG_PICK_UP_DROP_ITEM,

	// �̵����ͻ�����Ϣ
	CLIENT_CUSTOMMSG_SHOP_MANAGER,

	// ��Ӫ�
	CLIENT_CUSTOMMSG_DYNAMIC_ACTIVITY,

	// ȫ�����а�
	CLIENT_CUSTOMMSG_GLOBAL_RANK_LIST,

	/*!
	* @brief	���Ҷһ�
	* @param	int	�һ�����
	* @param	int	���ĸ߼���������
	*/
	CLIENT_CUSTOMMSG_EXCHANGE_CAPITAL,

	// ��̨�ɹ���
	CLIENT_CUSTOMMSG_DYNAMIC_ACTIVITY_EX,

	// ����ѡ������
	CLIENT_CUSTOMMSG_CHECKED_SELECTROLE,

	//���������ʱ��
	CLIENT_CUSTOMMSG_GET_SERVERTIME,

	//�ͻ�������������Ļ�����������
	CLIENT_CUSTOMMSG_SET_VISUAL_PLAYERS,

	// ���ݿͻ��˴����ֶλ�ȡ����������� ��ʽ: �ͻ����Զ����ʶ Ҫ��ѯ�����(�����б�[������ ������ ������]) ...
	// ����: ���(0�ɹ� 1 ��Ҳ�����) �ͻ��Զ����ʶ �������������+����+ֵ�� ....
	CLIENT_CUSTOMMSG_GET_PLAYER_PROP_DATA,

	// ����ϵͳ
	CLIENT_CUSTOMMSG_RIDE,

	// �ճ�������Ϣ
	CLIENT_CUSTOMMSG_DAILY_ACTIVITY,

	// ����������ͻ���׼��������֪ͨ�ͻ���
	// (�൱��OnReady�ص������Ƕ���������ʱ��û�д˻ص���������ͻ����Զ������Ϣ)
	CLIENT_CUSTOMMSG_CONTINUE_ON_READY,

	//������Ϣ����������ڶ���
	CLIENT_CUSTOMMSG_PET,

	// ����
	CLIENT_CUSTOMMSG_AUCTION,

	//��ɫ����ϵͳ
	CLIENT_CUSTOMMSG_DEAD_SYSTEM,

	// ������ʾ��Ʒ
	CLIENT_CUSTOMMSG_LOOK_CHAT_ITEM,

	// ��������
	CLIENT_CUSTOMMSG_REVIVE,

	// ���
	CLIENT_CUSTOMMSG_WING,
#pragma endregion


#pragma region //SocialSystemModule(201-700)
	/*!
	* @brief	�ͻ�����������
	* @param	int			channeltype(0˽�ģ�1���ӷ�Χ��2������3����)
	* @param	int			contenttype (0���� 1����)
	����-> @param	wstring		content  (˽��:@param	wstring		targetname)
	����-> @param	int64		voicemsgid  @param	int		voicemsgtime (˽��:@param	wstring		targetname)
	*/
	CLIENT_CUSTOMMSG_CHAT = 201,

	//����
	CLIENT_CUSTOMMSG_FRIEND,

	// �ͻ������󴴽����� int msg_id,int msg_id(������Ϣ),��Ӧ������Ϣ����
	CLIENT_CUSTOMMSG_GUILD,

	// ϵͳ�ʼ���Ϣ
	CLIENT_CUSTOMMSG_SYSTEM_MAIL,

	//�鿴������ɫ��ȫ��װ����Ϣ����Ϣ��ʽ��unsigned msgid, wstring name(�������߽�ɫ����)
	CLIENT_CUSTOMMSG_LOOK_EQUIPS,

	//ReviveManagerModule
	//��ɫ���󸴻� ��ʽ��string msgid int 0ԭ�ظ��� 1����㸴��,int ���Ÿ���� 1~6[ԭ�ظ��0��0��ָ������㸴��1��1~6������1��0ʱΪ�ͽ����Ե㸴��]
	CLIENT_CUSTOMMSG_REQUEST_RELIVE,	

	// ��ӿͻ�����Ϣ
	//�û�����������Ϣ��ʽ��unsigned msgid, int request_type, wstring name(�������߽�ɫ����)
	CLIENT_CUSTOMMSG_REQUEST,

	//�û����������Ӧ����Ϣ��ʽ��unsigned msgid, int request_type, wstring name(�������߽�ɫ����), int result(0:�ܾ�,1:ͬ��,2:��ʱ)
	CLIENT_CUSTOMMSG_REQUEST_ANSWER,

	// ��ӿͻ�����Ϣ����Ϣ��ʽ��unsigned msgid
	CLIENT_CUSTOMMSG_TEAM,

	// ������ʷ��������
	CLIENT_CUSTOMMSG_REQUEST_CHAT_HISTORY,

	// ��ѯ���������Ϣ��Ϣ
	CLIENT_CUSTOMMSG_CHAT_QUERY,
	
#pragma endregion

#pragma region //ItemModule(701-800)
	// ������ص���Ϣ, ����ʹ�õ���֮��
	CLIENT_CUSTOMMSG_TOOL_BOX = 701,

	// ������Ϣ 
	CLIENT_CUSTOMMSG_JADE,

#pragma endregion

#pragma region //NpcBaseModule(801-900)
	//ʹ�õ��߽��븱��
	CLIENT_CUSTOMMSG_ITEM_SCENE = 801,

	// ��ѯboss����
	CLIENT_CUSTOMMSG_QUERY_BOSS_INFO,
#pragma endregion

#pragma region //SceneBaseModule(1001-1100)
#pragma endregion

#pragma region //SkillModule(1101-1200)
	/*!
	 * @brief	ʹ�ü���
	 * @param	string		���ܵ�id
	 * @param	float sx, sz, dx, dz
	 * @param	PERSISTID	λ�Ƶ�Ŀ��
	 * @param	int			��ʼ�����ƶ�Ч��
	 */
	CLIENT_CUSTOMMSG_USE_SKILL  = 1101,

    // ��������
    CLIENT_CUSTOMMSG_SKILL_UPGRADE,

	/*!
	 * @brief	ʹ�ü���ʱ,���������λ��
	 * @param	int64		���ܵ�uid
	 * @param	string		���ܵ�id
	 * @param	PERSISTID	λ�Ƶ�Ŀ��
	 * @param	float x,z,orient
	 */
	CLIENT_CUSTOMMSG_SKILL_LOCATE_OBJECT,

	/*!
	 * @brief	ʹ�ü���(�ͻ��˴�������)
	 * @param	string		���ܵ�id
	 * @param	int64		���ܵ�uid
	 * @param	int			��ǰ���д�������
	 * @param	int			�����λ��Ŀ����n
	 ����ѭ��n��
	 * @param	PERSISTID	Ŀ������
	 * @param	int			�����λ��Ŀ����m
	 ����ѭ��m��
	 * @param	PERSISTID	Ŀ������
	 * @param	float x,z,speed
	 * @param   int action �ܻ�����
	 */
	CLIENT_CUSTOMMSG_USE_SKILL_BY_CLIENT,

	/*!
	 * @brief	��ϼ���
	 */
	CLIENT_CUSTOMMSG_BREAK_SKILL_FINISH,

	/*!
	* @brief	������������
	* @param	int			��������id
	*/
	CLIENT_CUSTOMMSG_PASSIVE_SKILL_UPGRADE,

	/*!
	* @brief	���ñ�������
	*/
	CLIENT_CUSTOMMSG_RESET_PASSIVE_SKILL,

	/*!
	* @brief	pkս��һ����Ϣ
	*/
	CLIENT_CUSTOMMSG_PK_MODEL,
#pragma endregion

#pragma region // �����Լ�����ϵͳ���(1201-1300)
	// ���������Ϣ
	CLIENT_CUSTOMMSG_TASK_MSG = 1201,

#pragma endregion


#pragma region // ֧��ϵͳ���(1301-1400)
	// ��ֵ
	CLIENT_CUSTOMMSG_PAY = 1301,
	// �̳�
	CLIENT_CUSTOMMSG_SHOP,
    //����������ܴ���ͳһ����
    CLIENT_CUSTOMMSG_BUY_INFO,
#pragma endregion

#pragma region //EquipmentModule(1401-1500)
	// �ͻ��˱�ʯ�����Ϣ
	CLIENT_CUSTOMMSG_JEWEL = 1401,

	//�ͻ���ǿ�������Ϣ
	CLIENT_CUSTOMMSG_STRENGTHEN,

	// �ͻ���װ�����������Ϣ
	CLIENT_CUSTOMMSG_SMELT,

	// װ��������ص���Ϣ
	CLIENT_CUSTOMMSG_FORGING,

	// װ��ϴ����ص���Ϣ
	CLIENT_CUSTOMMSG_BAPTISE,

	// ʱװ���
	CLIENT_CUSTOMMSG_FASHION,

#pragma endregion

#pragma region //OPActivity��Ӫ�(1501-1600)
#pragma endregion

#pragma region //CampaignModule�淨(1601-1800)
	// ���䳡��Ϣ
	CLIENT_CUSTOMMSG_ARENA_MSG = 1601,

	// ����BOSS������Ϣ
	CLIENT_CUSTOMMSG_WORLD_BOSS_ACTIVE,

	// ����ս���
	CLIENT_CUSTOMMSG_ASURA_BATTLE,
#pragma endregion
// ��Ϣ���ܳ���2048
};

#endif
