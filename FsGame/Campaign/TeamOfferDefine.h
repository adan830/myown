//--------------------------------------------------------------------
// �ļ���:		TeamOfferDefine.h
// ��  ��:		������Ͷ���
// ˵  ��:		
// ��������:		2017��01��12��
// ������:		tongzt
// �޸���:		 
//--------------------------------------------------------------------
#ifndef _TEAM_OFFER_DEFINE_H_
#define _TEAM_OFFER_DEFINE_H_

#include <vector>
#include <map>

// ��������
const char* const TEAM_OFFER_CAPTAIN_BOX_FILE = "ini/Campaign/TeamOffer/CaptainBox.xml";	// �ӳ�����
const char* const TEAM_OFFER_AWARD_RULE_FILE = "ini/Campaign/TeamOffer/AwardRule.xml";	// ���˽�������

// �ṹ����
// �ӳ�����
struct CaptainBox
{
	int offer_num;
	int reward_id;

	CaptainBox(const int num) :
		offer_num(num),
		reward_id(0)
	{
	}

	bool operator == (const CaptainBox& cfg) const
	{
		return cfg.offer_num == offer_num;
	}
};
typedef std::vector<CaptainBox> CaptainBoxVec;

// ���˽�������
struct OfferAwardRule 
{
	int lower_num;
	int upper_num;
	int lower_level;
	int upper_level;

	int reward_id;

	OfferAwardRule() :
		lower_num(0),
		upper_num(0), 
		lower_level(0),
		upper_level(0), 
		reward_id(0)
	{
	}
};
typedef std::vector<OfferAwardRule> OfferAwardRuleVec;


// ö�ٶ���
// ������ͼ�¼����������
#define TEAMOFFER_REC "teamoffer_rec"
enum TeamOfferRec
{
	TEAMOFFER_REC_COL_TEAM_ID = 0,	// ����ID
	TEAMOFFER_REC_COL_TASK_ID,		// ��������ID
	TEAMOFFER_REC_COL_GROUP_ID,		// ����ID
	TEAMOFFER_REC_COL_STATE,		// ״̬
	TEAMOFFER_REC_COL_CREATOR,		// ˢ����
	TEAMOFFER_REC_COL_STATE_TIME,	// ״̬��ʼʱ��
	
	TEAMOFFER_REC_COL_MAX,	
};

// �������״̬
enum TeamOfferStates
{
	TEAMOFFER_STATE_NONE = 1,	// δ��ʼ
	TEAMOFFER_STATE_DOING = 2,	// ����
	TEAMOFFER_STATE_END = 3,	// ����
};

// ���ö��
enum Results
{
	RESULTS_FAILED = 0,	// ʧ��
	RESULTS_SUCCESS,		// �ɹ�
};

// ������->����������Ϣ
enum S2P_TeamOffer_Sub_Msg
{
	S2P_TEAMOFFER_SUB_MSG_NONE = 0,

	/*!
	* @brief	����һ���������
	* @param wstring �ӳ���
	* @param int ����ID
	* @param int ��������ID
	* @param int �����
	* @param int ״̬
	* @param object ˢ����
	*/
	S2P_TEAMOFFER_SUB_MSG_LAUNCH,

	/*!
	* @brief	��ֹһ���������
	* @param int ����ID
	* @param int ��������ID
	* @param int �����
	*/
	S2P_TEAMOFFER_SUB_MSG_STOP,

	/*!
	* @brief	������Ϳ�ʼ֪ͨ
	* @param int ����ID
	* @param int ��������ID
	* @param int �����
	* @param object ˢ����
	*/
	S2P_TEAMOFFER_SUB_MSG_START,

	/*!
	* @brief	���������Ч��֤
	* @param int ����ID
	* @param int ����ID
	*/
	S2P_TEAMOFFER_SUB_MSG_CHECK,

	/*!
	* @brief	������ͽ��
	* @param int �����0��ʧ�� 1���ɹ���
	* @param int ����ID
	*/
	S2P_TEAMOFFER_SUB_MSG_RESULT,

	/*!
	* @brief	����һ���������
	* @param int ����ID
	*/
	S2P_TEAMOFFER_SUB_MSG_RESET,
};

// ������->����������Ϣ
enum P2S_TeamOffer_Sub_Msg
{
	P2S_TEAMOFFER_SUB_MSG_NONE = 0,

	/*!
	* @brief	֪ͨ�����Աͬ����������
	* @param wstring �ӳ���
	* @param int ����ID
	* @param int ��������ID
	*/
	P2S_TEAMOFFER_SUB_MSG_SYNC_TASK,

	/*!
	* @brief	֪ͨ���������鸱��
	* @param int ����ID
	* @param ��Ա��������ѭ��n��
	* @param wstring ��Ա��

	*/
	P2S_TEAMOFFER_SUB_MSG_ENTRY_GROUP,

	/*!
	* @brief	������ͽ��
	* @param int �����0��ʧ�� 1���ɹ���
	* @param int ����ID
	* @param object ˢ����
	* @param int ����ʱ�䣨s��
	* @param ��Ա��������ѭ��n��
	* @param wstring ��Ա��
	*/
	P2S_TEAMOFFER_SUB_MSG_RESULT,

};

// ������->����������Ϣ
enum S2S_TeamOffer_Sub_Msg
{
	S2S_TEAMOFFER_SUB_MSG_NONE = 0,

	/*!
	* @brief	֪ͨ�ӳ�����ͬ����������
	* @param int ����id
	* @param int ��������id
	*/
	S2S_TEAMOFFER_SUB_MSG_SYNC_TASK,

	/*!
	* @brief	����������ͷ��鸱��
	* @param int ����ID
	*/
	S2S_TEAMOFFER_SUB_MSG_ENTRY,

	/*!
	* @brief	������ͽ��
	* @param int �����0��ʧ�� 1���ɹ���
	* @param int ����ID
	* @param object ˢ����
	* @param int ����ʱ�䣨s��
	* @param ��Ա��������ѭ��n��
	* @param wstring ��Ա��
	*/
	S2S_TEAMOFFER_SUB_MSG_RESULT,

	/*!
	* @brief	֪ͨ��Աͬ����������
	* @param int ����id
	* @param int ��������id
	*/
	S2S_TEAMOFFER_SUB_MSG_MEMBER_SYNC_TASK,

	/*!
	* @brief	֪ͨ�Ӷӳ�������һ��
	*/
	S2S_TEAMOFFER_SUB_MSG_CONTINUE,

	/*!
	* @brief	֪ͨ��Աɾ���ɵ���������
	*/
	S2S_TEAMOFFER_SUB_MSG_DELETE_TASK,

};

// ������->�ͻ�������Ϣ
enum S2C_TeamOffer_Sub_Msg
{
	S2C_TEAMOFFER_SUB_MSG_NONE = 0, 

	/*!
	* @brief	������ͽ��
	* @param int �����0��ʧ�� 1���ɹ���
	* �ɹ�������£�
	* @param int ��ǰ����
	* @param int �ܴ���
	* @param int ͨ��ʱ�䣨ms��
	* @param int ����ID
	*/
	S2C_TEAMOFFER_SUB_MSG_RESULT,

	/*!
	* @brief	������͵���ʱ
	* @param int ���͵���ʱ(ms)
	*/
	S2C_TEAMOFFER_SUB_MSG_COUNTDOWN,

	/*!
	* @brief	��������˺���
	* @param int ������n
	* ����ѭ��n��
	* @param wstring �����
	* @param int ְҵ
	* @param int �˺�ֵ
	*/
	S2C_TEAMOFFER_SUB_MSG_DAMAGE_INFO,

	/*!
	* @brief	���������ѽ����������½�ȡ��һ������
	* @param int ����ɴ���
	*/
	S2C_TEAMOFFER_SUB_MSG_CONTINUE_NEXT,
};

// �ͻ���->����������Ϣ
enum C2S_TeamOffer_Sub_Msg
{
	C2S_TEAMOFFER_SUB_MSG_NONE = 0,

	/*!
	 * @brief	����һ���������
	 */
	C2S_TEAMOFFER_SUB_MSG_LAUNCH,

	 /*!
	 * @brief	�������ͷ���
	 */
	C2S_TEAMOFFER_SUB_MSG_ENTRY_GROUP,

	 /*!
	 * @brief	�˳��������
	 */
	C2S_TEAMOFFER_SUB_MSG_LEAVE,	
};

#endif	// _TEAM_OFFER_DEFINE_H_