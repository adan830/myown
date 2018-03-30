//--------------------------------------------------------------------
// �ļ���:		GmccDefine.h
// ��  ��:		GmccDefine����ض���
// ˵  ��:		
// ��������:    2016��9��19��
//--------------------------------------------------------------------
#ifndef __GMCC_XX_DEFINE_H__
#define __GMCC_XX_DEFINE_H__

enum GmccCmd
{
	GMCC_CMD_EXEC_GM_COMMAND,			// ִ��gm����
// 	GMCC_CMD_EXEC_FORBID_SPEAK,			// �������
// 	GMCC_CMD_EXEC_ALLOW_SPEAK,			// �����ҽ���
// 	GMCC_CMD_EXEC_KICK_OUT_IN_TIMER,	// ��ʱ����
	GMCC_CMD_EXEC_PAYED,				// ֧���ɹ� ����: order id
	GMCC_CMD_EXEC_PROP_QUERY,			// ��ѯ���� ����: queryid, keys
	GMCC_CMD_EXEC_RECORD_QUERY,			// ��ѯ�� ����: queryid, recordName, offset, count
	GMCC_CMD_SYNC_SEV_INFO,				// gmccͬ����������Ϣ
	GMCC_CMD_RECV_GLOBAL_MAIL,			// ��ȡȫ���ʼ�
	GMCC_CMD_REBATE_INFO,				// ��ֵ������Ϣ args: rebate[int]
	GMCC_CMD_REBATE_RESULT,				// ��ֵ������� args: result[int]
};

// --- �ͻ��˷��͹���GMCC���͵���Ϣ ----
enum GMCC2SMsg
{
	GMCC_C2S_NULL = 0,			// ĳ����Ϣ���� 
	GMCC_C2S_ACCUSE,			// �ٱ�
	GMCC_C2S_BUG, 				// BUG�ύ
	GMCC_C2S_EXP, 				// �����ύ
	GMCC_C2S_EXCH_GIFT,			// �һ���Ʒ
	GMCC_C2S_PICK_PAYED_REBATE,	// ��ȡ���Գ�ֵ���� args: none
};

enum GmccS2CMsg
{
	/*!
	* @brief	����gmcc����
	* @param	int			ͨ�����n
	����ѭ��n��
	* @param	int64		ͨ��id
	* @param	string		ͨ������
	* @param	int			�Ƿ�ѭ������
	* @param	int			���ż��ʱ��
	*/
	GMCC_S2C_ADD_NOTICE,				// ���ӹ���

	/*!
	* @brief	����gmcc����
	* @param	int64		ͨ��id
	*/
	GMCC_S2C_DELETE_NOTICE,				// ɾ������

	GMCC_S2C_PICK_PAYED_REBATE,			// ��ȡ���Գ�ֵ���� args: �Ƿ�ɹ�(int: EmRebateResult)
	GMCC_S2C_PAYED_REBATE_NOTIFY,		// ��ֵ����֪ͨ	args: ����Ԫ����[int]
};

#define  PUB_GMCC_NOTICE_REC "gmcc_notice_rec" // gmccͨ���
// gmccͨ���
enum GmccNoticeRec
{
	PUB_COL_GNR_NOTICE_ID = 0,		// ͨ��id
	PUB_COL_GNR_NOTICE_NAME,		// ͨ������
	PUB_COL_GNR_NOTICE_CONTENT,		// ͨ������
	PUB_COL_GNR_NOTICE_CREATE_TIME,	// ͨ�洴��ʱ��
	PUB_COL_GNR_NOTICE_END_TIME,	// ͨ�����ʱ��
	PUB_COL_GNR_NOTICE_INTERVAL,	// ͨ�沥�ż��ʱ��
	PUB_COL_GNR_NOTICE_LOOP,		// �Ƿ�ѭ��
	PUB_COL_GNR_NOTICE_TYPE,		// ֪ͨ����

	PUB_GNR_NOTICE_MAX_COL,
	PUB_GNR_NOTICE_MAX_ROW = 100
};

#define PUB_GLOBAMAIL_REC	"pub_global_mail_rec"	// ȫ���ʼ���
#define PUB_GLOBAMAIL_MAX_ROWS 10240			// ����ż�¼
enum EmGlobalMail
{
	EM_PUB_COL_GLOBAL_MAIL_ID = 0,		// �ʼ�Ψһid
	EM_PUB_COL_GLOBAL_MAIL_ORI_ID,		// �����ʼ�ID, ������
	EM_PUB_COL_GLOBAL_MAIL_FROM,		// �ʼ���Դ
	EM_PUB_COL_GLOBAL_MAIL_SENDER,		// ������
	EM_PUB_COL_GLOBAL_MAIL_TITLE,		// ����
	EM_PUB_COL_GLOBAL_MAIL_CONTENT,		// ����
	EM_PUB_COL_GLOBAL_MAIL_ATTACHEMENT,	// ����
	EM_PUB_COL_GLOBAL_MAIL_CTIME,		// ����ʱ��
	EM_PUB_COL_GLOBAL_MAIL_ETIME,		// ��Ч��ֹʱ��
};

enum EmAddGlobalMailResult
{
	EM_GLOBAL_MAIL_ADD_SUCCESS = 0,		// �ɹ�
	EM_GLOBAL_MAIL_ADD_FAILED_PARAM,	// ������
	EM_GLOBAL_MAIL_ADD_FAILED_UNKOWN,	// ��������
};

#define PUB_GMCC_PAYED_REC	"gmcc_payed_rec"	// ֧���ɹ���Ϣ��
#define PUB_GMCC_PAYED_MAX_ROWS	10240			// ����ż�¼
enum EmGmccPayRecCols
{
	PUB_COL_GNR_PAYED_ROLE_ID = 0,		// ��ɫuid string
	PUB_COL_GNR_PAYED_ORDER,			// ������ string 
	PUB_COL_GNR_PAYED_PRODUCT_ID,		// ��Ʒid string
	PUB_COL_GNR_PAYED_AMOUNT,			// ֧���Ľ�� float
	PUB_COL_GNR_PAYED_ITEMS,			// ��Ʒ��־����Ʒid�б�(��ʽ:item1:num,item2:num...) string
	PUB_COL_GNR_PAYED_REWARDS,			// ���͵���Ʒid�б�(��ʽ:item1:num,item2:num...) string
	PUB_COL_GNR_PAYED_GIFTS,			// �׳����͵���Ʒid�б�(��ʽ:item1:num,item2:num...) string
	PUB_COL_GNR_PAYED_EXTRAS,			// ����������������Ʒid�б�(��ʽ:item1:num,item2:num...) string
	PUB_COL_GNR_PAYED_BILL_TYPE,		// �˵����� int
	PUB_COL_GNR_PAYED_PRODUCT_RULE,		// ����������� int
	PUB_COL_GNR_PAYED_PRODUCT_NUMBER,	// ��������Ӧ������ int
	PUB_COL_GNR_PAYED_ORDER_TIME,		// ��������ʱ�� int64
	PUB_COL_GNR_PAYED_PAYED_TIME,		// ����֧��ʱ�� int64

	PUB_COL_GNR_PAYEED_MAX_COL,
};

#define PUB_REC_GMCC_PRODUCTS		"pub_rec_products"	// ֧��������Ʒ�б�
enum EmGmccProductsRecCols
{
	PUB_COL_GNR_PRODUCTS_PRODUCT_ID = 0,	// ��Ʒid string
	PUB_COL_GNR_PRODUCTS_PRICE,				// ��Ʒ�۸� float
	PUB_COL_GNR_PRODUCTS_ITEMS,				// ��Ʒ��־����Ʒid�б�(��ʽ:item1:num,item2:num...) string
	PUB_COL_GNR_PRODUCTS_REWARDS,			// ���͵���Ʒid�б�(��ʽ:item1:num,item2:num...) string
	PUB_COL_GNR_PRODUCTS_GIFTS,				// �׳����͵���Ʒid�б�(��ʽ:item1:num,item2:num...) string
	PUB_COL_GNR_PRODUCTS_EXTRAS,			// ����������������Ʒid�б�(��ʽ:item1:num,item2:num...) string
	PUB_COL_GNR_PRODUCTS_RULE,				// ������� int
	PUB_COL_GNR_PRODUCTS_STATE,				// ��Ʒ״̬ int
	PUB_COL_GNR_PRODUCTS_NUMBER,			// ������ƵĿɹ������ int(С�ڵ���0 ������)

	PUB_COL_GNR_PRODUCTS_MAX_COL,
};

// ��EmPayedResult(gmcc->ProtocolsID)��Ӧ
enum EmGmccPayedResult
{
	EM_PAYED_RESULT_SUCCESS,				// �ɹ�����
	EM_PAYED_RESULT_REPEAT,					// �ظ�����
	EM_PAYED_RESULT_DATA_ERR,				// ���ݴ�
};

enum EmGmccMondifyMode
{
	EM_GMCC_MONDIFY_ADD,				// ����
	EM_GMCC_MONDIFY_PUSH,				// ����
	EM_GMCC_MONDIFY_REMOVE,				// �Ƴ�
};

// ��EmProductState(gmcc->ProtocolsID)��Ӧ
enum EmGmccProductState
{
	EM_GMCC_PRODUCT_STATE_NONE,			// δ����
	EM_GMCC_PRODUCT_STATE_NORMAL,		// ����������
	EM_GMCC_PRODUCT_STATE_OBSOLETE,		// ����״̬
};

const char* const GMCC_GM_COMMAND_PRI = "gm_";
// ���� string uid int nSilenceTime(��λ:minute)
const char* const GMCC_EXEC_FORBID_SPEAK = "forbid_speak";
// ������� string uid
const char* const GMCC_EXEC_ALLOW_SPEAK = "allow_speak";
// ���� string uid int time ��
const char* const GMCC_EXEC_KICK_OUT_IN_TIMER = "kick_out_int_timer";
// ��� string uid int time ��
const char* const GMCC_EXEC_LOCK_PLAYER = "lock_player";
// ������� string uid delay(s�ӳ�ʱ�� 0������Ч)
const char* const GMCC_EXEC_UNLOCK_PLAYER = "unlock_player";
// gm��Ȩָ�� level
const char* const GMCC_EXEC_GRANT_AUTH = "grant_auth";
// ���¼���lua
const char* const GMCC_EXEC_RELOAD_LUA = "reload_lua";


// ��ʾ��Ϣ
#define TIP_EXCH_GIFT_SUCCEED			"sys_tip_exch_gift_succeed"		// �ɹ�
#define TIP_EXCH_GIFT_UNKOWN			"sys_tip_exch_gift_unkown"		// δ�����
#define TIP_EXCH_GIFT_INVALID			"sys_tip_exch_gift_invalid"		// ��Чkey
#define TIP_EXCH_GIFT_TAKED				"sys_tip_exch_gift_taked"		// ����ȡ
#define TIP_EXCH_GIFT_EXPIRED			"sys_tip_exch_gift_expired"		// �ѹ���
#define TIP_EXCH_GIFT_LIMIT				"sys_tip_exch_gift_limit"		// �����ȡ����������

// �ʼ�����
#define MAIL_NAME_PAYED							"mail_name_payed"				// ֧���ɹ��ʼ�
#define MAIL_NAME_PAYED_ITEM					"mail_name_payed_item"			// ֧���ɹ�������Ʒ�ʼ�
#define MAIL_NAME_PAYED_REWORD					"mail_name_payed_reword"		// ֧���ɹ����ͽ���
#define MAIL_NAME_PAYED_FIRST_PRODUCT			"mail_name_payed_first_product"	// ��Ʒ�׳�֧���ɹ����ͽ���


#endif // __GMCC_XX_DEFINE_H__