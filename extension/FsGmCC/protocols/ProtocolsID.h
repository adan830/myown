//--------------------------------------------------------------------
// �ļ���:		ProtocolKey.h
// ��  ��:		Gmcc WorldЭ��key����
// ˵  ��:		
// ��������:		2016��9��17��
// ������:		���
// ��Ȩ����:		���ݾ����������޹�˾
//--------------------------------------------------------------------
#ifndef __NET_PROTOCOL_KEY_H__
#define __NET_PROTOCOL_KEY_H__
#include "utils/enum.h"

enum
{
	PROTOCOL_ID_CONFIRM_ACK = 127,	 // ��Ϸȷ���յ���Ϣ	GAME->EXT 
	PROTOCOL_ID_GAME_READY	= 128,	// ��Ϸ������OK		GAME->EXT
	PROTOCOL_ID_GAME_CHECK_READY,	// gmcc������Ϸok״̬	EXT->GAME
	PROTOCOL_ID_GAME_REQ_SRV_INFO,	// ��Ϸ�����������ʼ��������Ϣ		GAME->EXT
	RPOTOCOL_ID_GAME_INIT_INFO,		// gmcc��ʼ�����		EXT->GAME

	PROTOCOL_ID_GM_EXEC,			// ִ��gm		EXT->GAME
	PROTOCOL_ID_GM_REPLY,			// ִ��gm��Ӧ		GAME->EXT

	PROTOCOL_ID_MAIL_POST,			// ִ�з����ʼ�	EXT->GAME
	PROTOCOL_ID_MAIL_RESULT,		// ִ�з����ʼ����GAME->EXT

	PROTOCOL_ID_NOTICE_SYNC,		// ͬ�����й�������(ͨ��PROTOCOL_ID_NOTICE_POST���أ� GAME->EXT
	PROTOCOL_ID_NOTICE_DELETE,		// ɾ������		EXT->GAME
	PROTOCOL_ID_NOTICE_POST,		// ִ�з��͹���	EXT->GAME
	PROTOCOL_ID_NOTICE_RESULT,		// ִ�з��͹�����GAME->EXT 

	PROTOCOL_ID_GIFT_EXCH,			// �һ���Ʒ		GAME->EXT
	PROTOCOL_ID_GIFT_RESULT,		// ��Ʒ���		EXT->GAME

	PROTOCOL_ID_ROLE_PROP_QUERY,	// ��ɫ���Բ�ѯ	EXT->GAME
	PROTOCOL_ID_ROLE_PROP_DATA,		// ��ɫ���Բ�ѯ����	GAME->EXT

	PROTOCOL_ID_ROLE_RECORD_QUERY,	// ��ɫ�����ݲ�ѯ		EXT->GAME
	PROTOCOL_ID_ROLE_RECORD_DATA,	// ��ɫ�����ݲ�ѯ����	GAME->EXT

	PROTOCOL_ID_PUSH_NOTIFICATION,	// ����֪ͨ����		GAME->EXT

	PROTOCOL_ID_PUSH_SHOP_ITEM,		// ��������̳���Ʒ		EXT->GAME
	PROTOCOL_ID_DEL_SHOP_ITEM,		// ɾ���̳���Ʒ		EXT->GAME
	PROTOCOL_ID_QUERY_SHOP_ITEM,	// ��ѯ����̳���Ʒ		EXT->GAME
	PROTOCOL_ID_SHOP_ITEM_LIST,		// ��ѯ���������Ʒ		GAME->EXT

	PROTOCOL_ID_SET_ACTIVITY_CONFIG,// ���û���ñ�		EXT->GAME �������ͼ�EmActivityConfigType

	PROTOCOL_ID_REPORT_MEMBER_INFO,	// �ϱ�������״̬		GAME->EXT

	PROTOCOL_ID_LOAD_CONFIG,		// ���ػ���ñ�		EXT->GAME �������ͼ�EmActivityConfigType

	PROTOCOL_ID_PRODUCTS_PSH,				// ��Ʒ�б�����	EXT->GAME
	PROTOCOL_ID_PAYMENT_PSH,				// ֧��֪ͨ		EXT->GAME
	PROTOCOL_ID_ORDER_REQ,					// ֧���µ�		GAME->EXT
	PROTOCOL_ID_ORDER_INFO,					// ֧���µ���Ӧ	EXT->GAME
	PROTOCOL_ID_ORDER_NOTIFY,				// ����ȷ��֪ͨ	GAME->EXT

	PROTOCOL_ID_REBATE_QUERY,				// ��ѯ��ֵ����	GAME->EXT
	PROTOCOL_ID_REBATE_INFO,				// ���س�ֵ������Ϣ	EXT->GAME
	PROTOCOL_ID_REBATE_PICK,				// ��ȡ����ֵ����֪ͨ	GAME->EXT
	PROTOCOL_ID_REBATE_RESULT,				// ��ȡ����ֵ�������	EXT->GAME
};

enum EmHttpRequest
{
	PROTOCOL_HTTP_PROFILE = 1,			// ����profile
	PROTOCOL_HTTP_NOTICE,				// ����notice
	PROTOCOL_HTTP_GIFT,					// ����gift
	PROTOCOL_HTTP_PUSH_NOTIFICATION,	// ����֪ͨ
	PROTOCOL_HTTP_CONFIG_ACTIVITY,		// ���������
	PROTOCOL_HTTP_MALL_ITEMS,			// ��Ӫ�̳��б�
	PROTOCOL_HTTP_MAIL_RESULT,			// �ʼ����
	PROTOCOL_HTTP_GMCMD_RESULT,			// gm���
	PROTOCOL_HTTP_PRODUCTS,				// ��Ʒ�б�
	PROTOCOL_HTTP_ORDER_STATE,			// ����֪ͨ
	PROTOCOL_HTTP_ORDER_PREPARE,		// ֧���µ�
	PROTOCOL_HTTP_REBATE_INFO,			// ��ֵ������Ϣ
	PROTOCOL_HTTP_REBATE_PICK,			// ���ŷ�������
};

// ����������
enum EmRebateResult
{
	EM_REBATE_RESULT_SUCCEED,			// �ɹ�
	EM_REBATE_RESULT_PICKED,			// ����ȡ
	EM_REBATE_RESULT_ERROR,				// ��������
};

enum EmPayedResult
{
	PAYED_RESULT_SUCCED,				// ��Ϸ���������ճɹ�
	PAYED_RESULT_RECVED,				// �����ظ�
	PAYED_RESULT_DATA_ERR,				// ���ݴ�
};

enum EmDataQueryResult
{
	DATA_QUERY_SUCCESS,				// �ɹ�
	DATA_QUERY_ROLEID_ERR,			// ��ɫid����ȷ
	DATA_QUERY_ROLEIN_OFFLINE,		// ��ɫ������
	DATA_QUERY_KEY_ERROR,			// ���Ի��������
	DATA_QUERY_SERVER_DISCONNECT,	// �������Ͽ�
};

// �����
enum EmActivityConfigType
{
	EM_ACT_TYPE_NONE = 0,			// ������
	EM_ACT_TYPE_LOGIN_DAY_TOTAL = 1,			// �ۻ���¼��
	EM_ACT_TYPE_ON_LINE_TIME = 2,			// ���߻����
	EM_ACT_TYPE_PLAYER_LEVEL = 3,			// ��ҵȼ��	
	EM_ACT_TYPE_KILL_PLAYER = 4,			// ��һ�ɱ�
	EM_ACT_TYPE_BATTLE_RANK = 5,			// ս�������а�
	EM_ACT_TYPE_BATTLE = 6,			// ս�������
	EM_ACT_TYPE_PAY_DAILY = 7,			// ÿ�ճ�ֵ
	EM_ACT_TYPE_PAY_TOTAL = 8,			// �ۻ���ֵ
	EM_ACT_TYPE_PLAYER_LEVEL_MATCH = 9,            // �弶����	
	EM_ACT_TYPE_RIDE_STEP_LV_UP = 10,           // �������׻
	EM_ACT_TYPE_MERIDIAN_LV_UP = 11,           // ���������
	EM_ACT_TYPE_JEWEL_INLAY = 12,           // ��ʯ��Ƕ�
	EM_ACT_TYPE_RIDE_STEP_SHOP = 13,           // ���������̵�
	EM_ACT_TYPE_MERIDIAN_SHOP = 14,           // ���������̵�
	EM_ACT_TYPE_JEWEL_SHOP = 15,           // ��ʯ��Ƕ�̵�
	EM_ACT_TYPE_WISH = 16,			// ������

	EM_ACT_TYPE_PRINCESS_BLESS = 30,			// �Ź�����ף��

	EM_ACT_TYPE_TOTAL,
};

// ���÷���
enum EmConfigCatalog
{
	EM_CONFIG_CATALOG_INVALID = 0,		// ��Ч����id
	
	EM_CONFIG_CATALOG_ACTIVITY = 1,		// ͨ�û����
	EM_CONFIG_CATALOG_MALL = 2,			// ��Ӫ�̳�����
	EM_CONFIG_CATALOG_FIRST_PAY = 3,	// �׳�����
	EM_CONFIG_CATALOG_LEVELING = 4,		// �弶����

	EM_CONFIG_CATALOG_END = 1024,
};

#define ERROR_CODE_SUCCESS	"10000"
#define ERROR_CODE_CONFIRM_PAY_TIMEOUT	"102022"	// ֧��ȷ�ϳ�ʱ�ط�


// ��Ʒ�һ�������
#define ERROR_CODE_GIFT_INVALID		"11001"		// ��Ʒ�벻��ȷ
#define ERROR_CODE_GIFT_TAKED		"11002"		// ��Ʒ������ȡ
#define ERROR_CODE_GIFT_EXPIRED		"11003"		// ��Ʒ���ѹ���
#define ERROR_CODE_GIFT_LIMIT		"11004"		// ��Ʒ����ȡ��������

enum EmGiftCode
{
	EM_GIFT_CODE_SUCCESS,	
	EM_GIFT_CODE_INVLID,			// ��Ʒ�벻��ȷ
	EM_GIFT_CODE_TAKED,				// ��Ʒ������ȡ
	EM_GIFT_CODE_EXPIRED,			// ��Ʒ���ѹ���
	EM_GIFT_CODE_LIMIT,				// ��Ʒ����ȡ��������

	EM_GIFT_CODE_UNKOWN,			// ����δ�������
};

inline int GetGiftErrorCode(const char* pszCode)
{
	static const char* szArrCode[] = {
		ERROR_CODE_SUCCESS,
		ERROR_CODE_GIFT_INVALID,				// ��Ʒ�벻��ȷ
		ERROR_CODE_GIFT_TAKED,					// ��Ʒ������ȡ
		ERROR_CODE_GIFT_EXPIRED,				// ��Ʒ���ѹ���
		ERROR_CODE_GIFT_LIMIT,					// ��Ʒ����ȡ��������
	};

	static const int nCount = sizeof(szArrCode) / sizeof(szArrCode[0]);
	for (int i = 0; i < nCount; ++i)
	{
		if (strcmp(pszCode, szArrCode[i]) == 0)
		{
			return EM_GIFT_CODE_SUCCESS + i;
		}
	}

	return EM_GIFT_CODE_UNKOWN;
}

// GMCC��������
enum EmNoticeType
{
	EM_NOTICE_TYPE_TOP = 1,			// ��������ƹ���
	EM_NOTICE_INDEX,				// ��ҳ����
	EM_NOTICE_TYPE_CENTER,			// ��Ļ�м乫��
	EM_NOTICE_TYPE_HORN,			// С���ȹ���
};

// ��Ϸ����������ȷ�Ͻ��
enum EmGmccResult
{
	EM_GMCC_RESULT_SUCCEED = 1,		// �ɹ�
	EM_GMCC_RESULT_NO_PLAYER,		// û�ҵ����
	EM_GMCC_RESULT_PLAYER_OFFLINE,	// ��Ҳ�����
	EM_GMCC_RESULT_COMMAND_ERROR,	// �������

	EM_GMCC_RESULT_ERROR = 10000,	// ��������
};

enum EmBillType
{
	EM_BILL_TYPE_VIRTUAL = 0,	// ����ҳ�ֵ����  VIRTUAL_CURRENCY("V"),
	EM_BILL_TYPE_REAL,			// ��ʵ��ֵ����	 THIRD_CURRENCY("T")

	EM_BILL_TYPE_MAX_END,		// 
};

ENUM_TO_STRING_EX(EmBillType, EM_BILL_TYPE_MAX_END,
	ENUM_ITEM_STR(V)
	ENUM_ITEM_STR(T)
);

enum EmOrderState
{
	EM_ORDER_STATE_UNKOWN = 0,				// ����״̬
	EM_ORDER_STATE_INIT,					// �Ѵ��� INIT("0"),
	EM_ORDER_STATE_PAYED,					// ��֧�� PAY("P"),
	EM_ORDER_STATE_ARRIVED,					// ��Ϸ�������Ѿ��յ� ARRIVED("A")
	EM_ORDER_STATE_TAKE,					// ����ȡTAKE("T")
	EM_ORDER_STATE_FINISH,					// �����_FINISH("F")

	EM_ORDER_STATE_MAX_END,					// 
};

ENUM_TO_STRING_EX(EmOrderState, EM_ORDER_STATE_MAX_END,
	ENUM_ITEM_STR(U)
	ENUM_ITEM_STR(0)
	ENUM_ITEM_STR(P)
	ENUM_ITEM_STR(A)
	ENUM_ITEM_STR(T)
	ENUM_ITEM_STR(F)
	);

enum EmProductRule
{
	EM_PRODUCT_RULE_NORMAL,				// ��������
	EM_PRODUCT_RULE_DAILY,				// ÿ��DAILY("D")
	EM_PRODUCT_RULE_WEEK,				// ÿ��WEEK("W")
	EM_PRODUCT_RULE_MONTH,				// ÿ��MONTH("M")
	EM_PRODUCT_RULE_YEAR,				// ÿ��MONTH("Y")
	EM_PRODUCT_RULE_ONCE,				// ֻ����һ��ONCE("F")

	EM_PRODUCT_RULE_MAX_END,			// 
};
ENUM_TO_STRING_EX(EmProductRule, EM_PRODUCT_RULE_MAX_END,
	ENUM_ITEM_STR(N)
	ENUM_ITEM_STR(D)
	ENUM_ITEM_STR(W)
	ENUM_ITEM_STR(M)
	ENUM_ITEM_STR(Y)
	ENUM_ITEM_STR(F)
	);
// ��gmccdefine��EmGmccProductState����һ��
enum EmProductState
{
	EM_PRODUCT_STATE_NONE,			// δ����
	EM_PRODUCT_STATE_ACTIVE,		// ����������
	EM_PRODUCT_STATE_INACTIVE,		// ����״̬

	EM_PRODUCT_STATE_END,			// ����״̬
};
ENUM_TO_STRING_EX(EmProductState, EM_PRODUCT_STATE_END,
	ENUM_ITEM_STR(N)
	ENUM_ITEM_STR(A)
	ENUM_ITEM_STR(I)
	);

#endif 