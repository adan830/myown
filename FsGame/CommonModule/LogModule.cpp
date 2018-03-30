//--------------------------------------------------------------------
// �ļ���:      LogModule.cpp
// ��  ��:      ��־ģ��
// ˵  ��:
// ��������:    2014��10��20��
// ��������:    2015��09��10��
// ������:       
// ������:       
//    :       
//--------------------------------------------------------------------
#include "FsGame/CommonModule/LogModule.h"
#include "FsGame/CommonModule/LuaExtModule.h"
#include "FsGame/Define/CapitalDefine.h"
//#include "FsGame/Define/EquipDefine.h"
#include "FsGame/Define/ItemTypeDefine.h"
#include "FsGame/Define/GameDefine.h"
#include "utils/util_ini.h"
#include "utils/util_func.h"
#include "utils/string_util.h"
#include "utils/extend_func.h"
#include "utils/XmlFile.h"
#include "public/VarList.h"
#include <ctime>
//#include "../Define/OffLineDefine.h"
#include "SystemFunctionModule/CapitalModule.h"
#include "Analytics/GameAnalyticsDef.h"
#include "Analytics/GameAnalyticsEventNames.h"
#include "Analytics/GameAnalyticsProperties.h"
#ifdef ANALY_BY_USER
#include "Analytics/SensorsAnalyticsCollector.h"
#else
#include "Analytics/SensorsAnalyticsCollectorEx.h"
#endif
#include "utils/IniFile.h"
//#include "SocialSystemModule/ChatModule.h"
#include "Define/ContainerDefine.h"
#include "ContainerModule.h"
#ifndef FSROOMLOGIC_EXPORTS
#include "ItemModule/ToolItem/ToolItemModule.h"
#endif
//#include "Define/AuctionDefine.h"
#include "Define/SnsDefine.h"

//#include "FsGame/Middle/MiddleModule.h"

// ͳ�����ݳ���id
#define STATICS_SCENE_ID 2

const static int COMMENT_MAX_LEN = 255;

// ������־����
static const char* GA_CAPITAL_PROPERTIES[CAPITAL_MAX_TYPE] = 
{
	GAME_ANALYTICS_PROPERTIES_COPPER,
	GAME_ANALYTICS_PROPERTIES_SILVER,
// 	GAME_ANALYTICS_PROPERTIES_SMELT,
// 	GAME_ANALYTICS_PROPERTIES_EXPLOIT,
// 	GAME_ANALYTICS_PROPERTIES_SOUL,
// 	GAME_ANALYTICS_PROPERTIES_HONOR,
// 	GAME_ANALYTICS_PROPERTIES_CHIVALRY,
// 	GAME_ANALYTICS_PROPERTIES_TRADE_SILVER,
// 	GAME_ANALYTICS_PROPERTIES_CAMPVALUE,
};

LogModule* LogModule::m_pLogModule = NULL;
const char *LOG4CPLUS = "log4plush.properties";
const char* GAME_INI_PATH = "game.ini";

int64_t LogModule::ms_nNextReportCount = 0;
const int HB_REPORT_COUNT_INTERVAL = 10000;					// 10s
const int64_t REPORT_COUNT_INTERVAL = 60 * 5;				// ���5����

bool LogModule::Init(IKernel* pKernel)
{
	m_pLogModule = this;

	log4cplus::initialize();
	const char *path = pKernel->GetResourcePath();
	log4cplus::tstring pathname(path);
	pathname.append( LOG4CPLUS );
	log4cplus::PropertyConfigurator::doConfigure( pathname );
	// �����־
	log4cplus::Logger _anaLogger = log4cplus::Logger::getInstance("ANA");

	// ��ȡ��Ϸ����
	std::string strGameIni(path);
	strGameIni.append(GAME_INI_PATH);
	CIniFile iniFile(strGameIni.c_str());
	if (!iniFile.LoadFromFile())
	{
		Assert(false);
		::extend_warning(LOG_ERROR, "open game.ini(path:%s) failed", strGameIni.c_str());
		return false;
	}
	int nDeployId = iniFile.ReadInteger("game_info", "game_deploy_id", 0);
	int nGameId = iniFile.ReadInteger("game_info", "game_id", 0);
	const char* pszGameName = iniFile.ReadString("game_info", "game_name", "");
	const char* pszGameVer = iniFile.ReadString("game_info", "game_ver", "1.0");
	const char* pszDistrictName = iniFile.ReadString("game_info", "district_name", "");
	int pszChannelId = iniFile.ReadInteger("game_info", "channelid", 0);
	int production = iniFile.ReadInteger("game_info", "production", 0);
	int nLogLevel = iniFile.ReadInteger("game_info", "log_level", 0);
	if (nLogLevel > 0)
	{
		// ������־�ȼ�
		extend_log_level(pKernel, (LogLevelEnum)nLogLevel, "");
	}

#ifndef FSROOMLOGIC_EXPORTS
	m_pAnaCollector = new GameAnalytics(_anaLogger, 
					nDeployId,
					nGameId,
					pKernel->GetServerId(),
					pKernel->GetDistrictId(),
					pszChannelId,
					production,
					StringUtil::StringAsUtf8String(pszGameVer).c_str(),
					StringUtil::StringAsUtf8String(pszGameName).c_str(),
					StringUtil::StringAsUtf8String(pszDistrictName).c_str());
#else
	m_pAnaCollector = new GameAnalytics(_anaLogger,
		nDeployId,
		nGameId,
		pKernel->GetPlatformId(),
		pKernel->GetRoomId(),
		pszChannelId,
		production,
		StringUtil::StringAsUtf8String(pszGameVer).c_str(),
		StringUtil::StringAsUtf8String(pszGameName).c_str(),
		StringUtil::StringAsUtf8String(pszDistrictName).c_str());
#endif
	m_pAnaCollector->InitAction();
	Assert(m_pLogModule);

	m_pAnaCollector->OnServerStart();

	pKernel->AddEventCallback("scene", "OnCreate", LogModule::OnCreate);

	// ������־
	m_pChatLogger = log4cplus::Logger::getInstance("CHAT");

	
	DECL_HEARTBEAT(LogModule::H_RecOnlineCount);
	return true;
}

bool LogModule::Shut(IKernel* pKernel)
{
	if (NULL != m_pAnaCollector)
	{
		m_pAnaCollector->OnServerClose();
		m_pAnaCollector->GetLogger().shutdown();
		delete m_pAnaCollector;
		m_pAnaCollector = NULL;
	}
	return true;
}

void LogModule::SetSrvInfo(IKernel* pKernel, const SrvInfo& info)
{
	Assert(NULL != m_pAnaCollector);
	if (NULL != m_pLogModule)
	{
		m_pAnaCollector->SetChannelId(info.ChannelId);
		m_pAnaCollector->SetDistrictId(info.AreaId);
		m_pAnaCollector->SetGameId(info.GameId);
		m_pAnaCollector->SetProductionId(info.ProductionId);
		m_pAnaCollector->SetServerId(info.ServerId);
		m_pAnaCollector->SetDeployId(info.DeployId);
	}
	else
	{
		::extend_warning(LOG_ERROR, "init logmodule info failed. not init analysis instance");
	}
}

void LogModule::OnClientInfo(IKernel* pKernel, const PERSISTID& player, const char* ip,
							const char* platform,
							const char* clientver,
							const char* manufacturer,
							const char* deviceid,
							const char* model,
							const char* os,
							const char* osver,
							bool bwifi )
{
	if (NULL != m_pAnaCollector)
	{
		IGameObj* pPlayer = pKernel->GetGameObj(player);
		if (NULL == pPlayer)return;

		const char* rid = pPlayer->QueryString( FIELD_PROP_UID );
		//int nLevel = pPlayer->QueryInt(FIELD_PROP_LEVEL);
		//int64_t nExp = pPlayer->QueryInt64(FIELD_PROP_PLAYER_EXP);
		bool bFirst = pPlayer->QueryInt( FIELD_PROP_BORN_STATE) != EM_PLAYER_STATE_NORMAL;
		if (bFirst)
		{
			pPlayer->SetInt(FIELD_PROP_BORN_STATE, EM_PLAYER_STATE_NORMAL);
		}
		KVArgs args;
		FillCommonArgs(pKernel, player, args);
		m_pAnaCollector->OnClientInfo(rid, bFirst, clientver, ip, platform, bwifi, manufacturer, deviceid, model, os, osver, &args);
	}
}

#ifdef ANALY_BY_USER
// ע���û�
void LogModule::OnNewUser(	const char* uid,		// �û�Ψһid (require)
							const char* anid,		// �û�ԭ����id(option)
							const char* nick,		// �ǳ�/�û���(require)
							int sex,				// �Ա�(option)
							__int64 bron,			// ��������(option)
							const char* ip,			// �ն�id(option)
							const char* platform,	// ע��ƽ̨(option)
							bool wifi,				// �Ƿ�ʹ��wifi(option)
							const char* manufacturer,	// ������(option)
							const char* devid,		// �ն��豸Ψһ��ʶ(option)
							const char* model,		// �ն�����(option)
							const char* os,			// �ն�ϵͳ(option)
							const char* os_ver		// �ն�ϵͳ�汾(option)
							)
{
	if (NULL != m_pAnaCollector)
	{
		m_pAnaCollector->OnNewUser(uid, anid, nick, sex, bron, ip, platform, wifi, manufacturer, devid, model, os, os_ver);
	}
}

void LogModule::OnDelUser(const char* uid)
{
	if (NULL != m_pAnaCollector)
	{
		m_pAnaCollector->OnDelUser(uid);
	}
}
#endif
// �û���½
void LogModule::OnUserLogin(const char* uid,			// �û�Ψһid (require)
							const char* nick,		// �ǳ�/�û���(require)
							const char* ip,			// �ն�ip(option)
							const char* platform,	// ƽ̨(option)
							const char* manufacturer,	// ������(option)
							const char* devid,		// �ն��豸Ψһ��ʶ(option)
							const char* model,		// �ն�����(option)
							const char* os,			// �ն�ϵͳ(option)
							const char* os_ver,		// �ն�ϵͳ�汾(option)
							bool bReset /*= false*/	)
{
	if (NULL != m_pAnaCollector)
	{
		m_pAnaCollector->OnUserLogin(uid, nick, ip, platform, manufacturer, devid, model, os, os_ver, bReset);
	}
}

void LogModule::OnUserLogout(const char* uid, int64_t timestamp, bool bReset/* = false*/ )
{
	if (NULL != m_pAnaCollector)
	{
		m_pAnaCollector->OnUserLogout(uid, timestamp, bReset);
	}
}

// ������ɫ
void LogModule::OnCreateRole(IKernel* pKernel, const PERSISTID& player)
{
	if (NULL != m_pAnaCollector)
	{
		IGameObj* pPlayer = pKernel->GetGameObj(player);
		if (NULL == pPlayer)return;

		const char* uid = pPlayer->QueryString(FIELD_PROP_ACCOUNT);
		const char* rid = pPlayer->QueryString( FIELD_PROP_UID );
		const wchar_t* pwRoleName = pPlayer->QueryWideStr(FIELD_PROP_NAME);
		const std::string& roleName = StringUtil::WideStrAsUTF8String(pwRoleName);
		int job = pPlayer->QueryInt(FIELD_PROP_JOB);
		int sex = pPlayer->QueryInt(FIELD_PROP_SEX);
		int nation = 0/*pPlayer->QueryInt(FIELD_PROP_NATION)*/;
		const char* ip = pPlayer->QueryString(FIELD_PROP_ADDRESS);

		KVArgs args;
		FillCommonArgs(pKernel, player, args);
		m_pAnaCollector->OnNewRole(rid, uid, roleName.c_str(), job, sex, nation, ip, &args);

		//int nLevel = pPlayer->QueryInt(FIELD_PROP_LEVEL);
		//int nVip = pPlayer->QueryInt(FIELD_PROP_VIP_LEVEL);
		// �մ������ùؼ���ʼ�����Ա仯
		//for (int i = CAPITAL_MIN_TYPE; i < CAPITAL_MAX_TYPE; ++i)
		//{
		//	int64_t gold = CapitalModule::m_pCapitalModule->GetCapital(pKernel, player, i);
		//	if (0 != gold)
		//	{
		//		m_pAnaCollector->OnGainCoin(rid, nLevel, nVip, i, gold, gold, 0, 0, LOG_GAIN_INIT_ROLE, NULL);
		//	}
		//}
	}
}
// ��ɫ����
void LogModule::OnRoleEntry(IKernel* pKernel, const PERSISTID& player, bool bReset/* = false*/)
{
#ifdef TEST_LOG_PERFOMANCE
	int64_t tNow = util_get_utc_ms_time();
	for (int i = 0; i < 10000; ++i)
	{
#endif
		if (NULL != m_pAnaCollector)
		{
			IGameObj* pPlayer = pKernel->GetGameObj(player);
			if (NULL == pPlayer)return;

			// ����������
			if (pPlayer->QueryInt(FIELD_PROP_ONLINE) == OFFLINE)
			{
				return;
			}

			const char* uid = pPlayer->QueryString(FIELD_PROP_ACCOUNT);
			const char* rid = pPlayer->QueryString(FIELD_PROP_UID);
			int level = pPlayer->QueryInt(FIELD_PROP_LEVEL);
			int vip = pPlayer->QueryInt(FIELD_PROP_VIP_LEVEL);
			const char* ip = pPlayer->QueryString(FIELD_PROP_ADDRESS);
			int64_t gold = CapitalModule::m_pCapitalModule->GetCapital(pKernel, player, CAPITAL_GOLD);
			//int64_t silver = CapitalModule::m_pCapitalModule->GetCapital(pKernel, player, CAPITAL_TRADE_SILVER);
			int64_t copper = CapitalModule::m_pCapitalModule->GetCapital(pKernel, player, CAPITAL_COPPER);

			// �����������ͳ��
			int nCurOnlineCount = 0;
#ifndef FSROOMLOGIC_EXPORTS
			nCurOnlineCount = pKernel->GetOnlineCount();
#endif
			KVArgs args;
			args[GAME_ANALYTICS_PROPERTIES_ONLINE_COUNT] = nCurOnlineCount;
			//args[GAME_ANALYTICS_PROPERTIES_TRADE_SILVER] = silver;
			args[GAME_ANALYTICS_PROPERTIES_COPPER] = copper;

			m_pAnaCollector->OnRoleEntry(rid, uid, level, vip, gold, ip, &args, bReset);
		}
#ifdef TEST_LOG_PERFOMANCE
	}
	int64_t tEnd = util_get_utc_ms_time();
	int64_t tCost = tEnd - tNow;
	printf("log 1000 cost:%lldms\n", tCost);
#endif
}
// ��ɫ�˳�
void LogModule::OnRoleExit(IKernel* pKernel, const PERSISTID& player, bool bReset/* = false*/)
{
	if (NULL != m_pAnaCollector)
	{
		IGameObj* pPlayer = pKernel->GetGameObj(player);
		if (NULL == pPlayer)return;

		const char* uid = pPlayer->QueryString(FIELD_PROP_ACCOUNT);
		const char* rid = pPlayer->QueryString(FIELD_PROP_UID);
		int level = pPlayer->QueryInt(FIELD_PROP_LEVEL);
		int vip = pPlayer->QueryInt(FIELD_PROP_VIP_LEVEL);
		const char* ip = pPlayer->QueryString(FIELD_PROP_ADDRESS);
		int64_t gold = CapitalModule::m_pCapitalModule->GetCapital(pKernel, player, CAPITAL_GOLD);
//		int64_t silver = CapitalModule::m_pCapitalModule->GetCapital(pKernel, player, CAPITAL_TRADE_SILVER);
		int64_t copper = CapitalModule::m_pCapitalModule->GetCapital(pKernel, player, CAPITAL_COPPER);

		int64_t nowTime = util_get_utc_time();
		int64_t loginTime = pPlayer->QueryInt64( FIELD_PROP_ROLE_LOGIN_TIME );
		int64_t onlinetime = nowTime - loginTime;
		int64_t totalonline = pPlayer->QueryInt(FIELD_PROP_TOTALSEC);
		// ����ʱ�������ߴ洢�Ż��¼��Ҫ���ϵ�ǰ����ʱ��
		totalonline += onlinetime;
		//pPlayer->SetInt64(FIELD_PROP_ON_LINE_TIME, totalonline);

		// �����������ͳ��
		int nCurOnlineCount = 0;
#ifndef FSROOMLOGIC_EXPORTS
		nCurOnlineCount = pKernel->GetOnlineCount();
#endif
		
		KVArgs args;
		args[GAME_ANALYTICS_PROPERTIES_ONLINE_COUNT] = nCurOnlineCount;
		//args[GAME_ANALYTICS_PROPERTIES_TRADE_SILVER] = silver;
		args[GAME_ANALYTICS_PROPERTIES_COPPER] = copper;

		m_pAnaCollector->OnRoleExit(rid, uid, level, vip, gold, onlinetime, totalonline, ip, &args, bReset);
	}
}

void LogModule::OnPaying(IKernel* pKernel, const PERSISTID& player, 
						const char* orderid, const char* itemid, int count, float price, bool first)
{
	if (NULL != m_pAnaCollector)
	{
		IGameObj* pPlayer = pKernel->GetGameObj(player);
		if (NULL == pPlayer)return;

		const char* uid = pPlayer->QueryString(FIELD_PROP_ACCOUNT);
		const char* rid = pPlayer->QueryString(FIELD_PROP_UID);
		int level = pPlayer->QueryInt(FIELD_PROP_LEVEL);
		int vip = pPlayer->QueryInt(FIELD_PROP_VIP_LEVEL);

		const char* osType = pPlayer->QueryString(FIELD_PROP_CLIENT_TYPE);
		m_pAnaCollector->OnPaying(rid, uid, level, vip, orderid, itemid, count, price, first, osType);
	}
}

void LogModule::OnPayed(IKernel* pKernel, const PERSISTID& player, 
			const char* orderid, float amount, int result, bool first, bool bIsReal,
			const char* platform, const char* os)
{
	IGameObj* pPlayer = pKernel->GetGameObj(player);
	if (NULL == pPlayer)return;

	const char* uid = pPlayer->QueryString(FIELD_PROP_ACCOUNT);
	if (NULL != m_pAnaCollector)
	{
		const char* rid = pPlayer->QueryString(FIELD_PROP_UID);
		int level = pPlayer->QueryInt(FIELD_PROP_LEVEL);
		int vip = pPlayer->QueryInt(FIELD_PROP_VIP_LEVEL);
		float totalamount = pPlayer->QueryFloat(FIELD_PROP_TOTAL_PAY);
		float realamount = pPlayer->QueryFloat(FIELD_PROP_TOTAL_PAY_REAL);
		int paytimes = pPlayer->QueryInt(FIELD_PROP_PAY_TIMES);

		OnPayed(rid, uid, level, vip, orderid, amount, totalamount, realamount, paytimes, result, first, bIsReal, platform, os);
	}

	IGameObj* pDcObj = pKernel->GetGameObj(player);
	Assert(pDcObj != NULL);
	const wchar_t* name = pDcObj->QueryWideStr(FIELD_PROP_NAME);
	const std::string& strTime = m_pLogModule->GetLogTime();

	// �����ֵ��־
	CVarList logInfo;
	logInfo << uid << name << strTime << (first ? 1 : 0) << 0 << orderid << StringUtil::FloatAsString(amount) << "";
	pKernel->CustomLog("traceorder_log", logInfo);
}

void LogModule::OnPayed( const char* roleid, const char* uid, int level, int vip, 
	const char* orderid, float amount, float totalamount, float realamount, int paytimes, int result, bool first, bool bIsReal,
							const char* platform, const char* os)
{
	if (NULL != m_pAnaCollector)
	{
		m_pAnaCollector->OnPayCompleted(roleid, uid, level, vip, orderid, amount, totalamount, realamount, paytimes, result, first, bIsReal, platform, os);
	}
}

void LogModule::OnIncExp(IKernel* pKernel, const PERSISTID& player, 
							int value,					// ����ֵ(require)
							int newValue,				// ���Ӻ��ֵ
							int origValue,				// ԭʼֵ
							int source					// ��Դ
							)
{
	// ֻ��¼ָ�����ܻ�õľ���
	if (NULL != m_pAnaCollector)
	{
		IGameObj* pPlayer = pKernel->GetGameObj(player);
		if (NULL == pPlayer)return;
		const char* rid = pPlayer->QueryString(FIELD_PROP_UID);
		int level = pPlayer->QueryInt(FIELD_PROP_LEVEL);
		KVArgs args;
		FillCommonArgs(pKernel, player, args);
		m_pAnaCollector->OnIncExp(rid, level, value, newValue, origValue, source, &args);
	}
}

void LogModule::OnLevelup(IKernel* pKernel, const PERSISTID& player, int level, int original)
{
	if (NULL != m_pAnaCollector)
	{
		IGameObj* pPlayer = pKernel->GetGameObj(player);
		if (NULL == pPlayer)return;

		const char* rid = pPlayer->QueryString(FIELD_PROP_UID);
		KVArgs args;
		FillCommonArgs(pKernel, player, args);
		m_pAnaCollector->OnLevelup(rid, level, original, &args);
	}
}

void LogModule::OnGainCoin(IKernel* pKernel, const PERSISTID& player, 
						int coid, /* ��Ϸ��id(��/��/ͭ/����....)(require), ��Ϸ�ж��� */ 
						int64_t value, /* ����ֵ(require) */ 
						int64_t newvalue, /* ��ǰ��ֵ(require) */ 
						int64_t original, /* ԭʼֵ(require) */ 
						int module, /* ����ģ��(option), ��Ϸ�ж���(��ʲôģ��) */ 
						int func, /* �����¼�(option), ��Ϸ�ж���(��ʲô�»��) */ 
						const char* extra /*= NULL/* ����˵��(option), ��event��һ��˵�� */)
{
	if (NULL != m_pAnaCollector)
	{
		IGameObj* pPlayer = pKernel->GetGameObj(player);
		if (NULL == pPlayer)return;
		const char* rid = pPlayer->QueryString(FIELD_PROP_UID);
		int level = pPlayer->QueryInt(FIELD_PROP_LEVEL);
		int vip = pPlayer->QueryInt(FIELD_PROP_VIP_LEVEL);
		KVArgs args;
		FillCommonArgs(pKernel, player, args);
		m_pAnaCollector->OnGainCoin(rid, level, vip, coid, value, newvalue, original, module, func, extra, &args);
	}
}

void LogModule::OnCostCoin(IKernel* pKernel, const PERSISTID& player, 
				int coid, /* ��Ϸ��id(��/��/ͭ/����....)(require), ��Ϸ�ж��� */ 
				int64_t value, /* ����ֵ(require) */
				int64_t newvalue, /* ��ǰ��ֵ */
				int64_t original, /* ԭʼֵ(require) */ 
				int module, /* ����ģ��(option), ��Ϸ�ж���(��ʲôģ��) */ 
				int func, /* �����¼�(option), ��Ϸ�ж���(��ʲô�»��) */ 
				const char* extra /*= NULL/* ����˵��(option), ��event��һ��˵�� */)
{
	if (NULL != m_pAnaCollector)
	{
		IGameObj* pPlayer = pKernel->GetGameObj(player);
		if (NULL == pPlayer)return;
		const char* rid = pPlayer->QueryString(FIELD_PROP_UID);
		int level = pPlayer->QueryInt(FIELD_PROP_LEVEL);
		int vip = pPlayer->QueryInt(FIELD_PROP_VIP_LEVEL);
		KVArgs args;
		FillCommonArgs(pKernel, player, args);
		m_pAnaCollector->OnCostCoin(rid, level, vip, coid, value, newvalue, original, module, func, extra, &args);
	}
}

void LogModule::OnGainItem(IKernel* pKernel, const PERSISTID& player, 
							const char* itemid, /* ��Ʒid(require) */ 
							int color,				// ��ƷƷ��
							int num, /* �����ֵ(require) */ 
							int count, /* ��ǰӵ���� */ 
							int orig, /* ԭ��ӵ���� */ 
							int module, /* ����ģ��(option), ��Ϸ�ж���(��ʲôģ��) */ 
							int func, /* �����¼�(option), ��Ϸ�ж���(��ʲô�»��) */ 
							const char* extra /*= NULL/* ����˵��(option), ��event��һ��˵�� */)
{
	if (NULL != m_pAnaCollector)
	{
		IGameObj* pPlayer = pKernel->GetGameObj(player);
		if (NULL == pPlayer)return;
		const char* rid = pPlayer->QueryString(FIELD_PROP_UID);
		int level = pPlayer->QueryInt(FIELD_PROP_LEVEL);
		int vip = pPlayer->QueryInt(FIELD_PROP_VIP_LEVEL);
		KVArgs args;
		FillCommonArgs(pKernel, player, args);
		if (color >= 0)
		{
			args[GAME_ANALYTICS_PROPERTIES_ITEM_COLOR] = color;
		}
		m_pAnaCollector->OnGainItem(rid, level, vip, itemid, num, count, orig, module, func, extra, &args);
	}
}

void LogModule::OnCostItem(IKernel* pKernel, const PERSISTID& player, 
							const char* itemid, /* ��Ʒid(require) */ 
							int color,				// ��ƷƷ��
							int num, /* �����ֵ(require) */ 
							int count, /* ��ǰӵ���� */ 
							int orig, /* ԭ��ӵ���� */
							int module, /* ����ģ��(option), ��Ϸ�ж���(��ʲôģ��) */ 
							int func, /* �����¼�(option), ��Ϸ�ж���(��ʲô�»��) */ 
							const char* extra /*= NULL/* ����˵��(option), ��event��һ��˵�� */)
{
	if (NULL != m_pAnaCollector)
	{
		IGameObj* pPlayer = pKernel->GetGameObj(player);
		if (NULL == pPlayer)return;
		const char* rid = pPlayer->QueryString(FIELD_PROP_UID);
		int level = pPlayer->QueryInt(FIELD_PROP_LEVEL);
		int vip = pPlayer->QueryInt(FIELD_PROP_VIP_LEVEL);
		KVArgs args;
		FillCommonArgs(pKernel, player, args);
		if (color >= 0)
		{
			args[GAME_ANALYTICS_PROPERTIES_ITEM_COLOR] = color;
		}
		m_pAnaCollector->OnCostItem(rid, level, vip, itemid, num, count, orig, module, func, extra, &args);
	}
}

void LogModule::OnSkipUpStartScene(IKernel* pKernel, const PERSISTID& player)
{
	IGameObj* pPlayer = pKernel->GetGameObj(player);
	if (NULL == pPlayer)return;

	const char* rid = pPlayer->QueryString(FIELD_PROP_UID);
	int level = pPlayer->QueryInt(FIELD_PROP_LEVEL);
	int vip = pPlayer->QueryInt(FIELD_PROP_VIP_LEVEL);

	KVArgs args;
	FillCommonArgs(pKernel, player, args);
	m_pAnaCollector->OnCustomEvent(rid, level, vip, GAME_EXTRA_EVENT_SKIP_UP_START_SCENE, 0, 0, &args);
}

void LogModule::OnVisitFunction(IKernel* pKernel, const PERSISTID& player,
						int module, /* ����ģ��(require), ��Ϸ�ж���(��ʲôģ��) */ 
						int func, /* ������(require) ��Ϸ�ж���(����������funcһ��) */
						bool result, /* ���(require) true:�ɹ� false: ʧ�� */ 
						int value, /* ����˵��(option), ��func��һ��˵�� */ 
						const char* extra /* ����˵��(option), ��func��һ��˵�� */)
{
	if (NULL != m_pAnaCollector)
	{
		IGameObj* pPlayer = pKernel->GetGameObj(player);
		if (NULL == pPlayer)return;

		//const char* uid = pPlayer->QueryString(FIELD_PROP_ACCOUNT);
		const char* rid = pPlayer->QueryString(FIELD_PROP_UID);
		int level = pPlayer->QueryInt(FIELD_PROP_LEVEL);
		int vip = pPlayer->QueryInt(FIELD_PROP_VIP_LEVEL);
		KVArgs args;
		FillCommonArgs(pKernel, player, args);
		m_pAnaCollector->OnVisitFunction(rid, level, vip, module, func, result, value, extra, &args);
	}
}


void LogModule::OnFriendEvent(IKernel* pKernel, const PERSISTID& player, const wchar_t* pFriendName, int opType)
{
	if (NULL != m_pAnaCollector)
	{
		IGameObj* pPlayer = pKernel->GetGameObj(player);
		if (NULL == pPlayer)return;

		const char* rid = pPlayer->QueryString(FIELD_PROP_UID);
		int level = pPlayer->QueryInt(FIELD_PROP_LEVEL);
		int vip = pPlayer->QueryInt(FIELD_PROP_VIP_LEVEL);

		KVArgs args;
		//args[GAME_EXTRA_PROPERTIES_FRIEND_SELF_NAME] = StringUtil::WideStrAsUTF8String(pPlayer->QueryWideStr(FIELD_PROP_NAME));
		args[GAME_EXTRA_PROPERTIES_FRIEND_NAME] = StringUtil::WideStrAsUTF8String(pFriendName);
		//args[GAME_EXTRA_PROPERTIES_FRIEND_RID] = pFriendId;
		FillCommonArgs(pKernel, player, args);
		m_pAnaCollector->OnCustomEvent(rid, level, vip, GAME_EXTRA_EVENT_FRIEND_OP, 0, opType, &args);
	}
}

void LogModule::OnFriendSendGift(IKernel* pKernel, const PERSISTID& player, const wchar_t* pFriendName, int nGift, int nIntLevel, int nIncIntimacy)
{
	if (NULL != m_pAnaCollector)
	{
		IGameObj* pPlayer = pKernel->GetGameObj(player);
		if (NULL == pPlayer)return;

		const char* rid = pPlayer->QueryString(FIELD_PROP_UID);
		int level = pPlayer->QueryInt(FIELD_PROP_LEVEL);
		int vip = pPlayer->QueryInt(FIELD_PROP_VIP_LEVEL);

		KVArgs args;
		//args[GAME_EXTRA_PROPERTIES_FRIEND_SELF_NAME] = StringUtil::WideStrAsUTF8String(pPlayer->QueryWideStr(FIELD_PROP_NAME));
		args[GAME_EXTRA_PROPERTIES_FRIEND_NAME] = StringUtil::WideStrAsUTF8String(pFriendName);
		//args[GAME_EXTRA_PROPERTIES_FRIEND_RID] = pFriendId;

		args[GAME_EXTRA_PROPERTIES_FRIEND_GIFT_NUM] = nGift;
		args[GAME_EXTRA_PROPERTIES_FRIEND_INTLEVEL] = nIntLevel;
		args[GAME_EXTRA_PROPERTIES_FRIEND_INC_INTIMACY] = nIncIntimacy;
		FillCommonArgs(pKernel, player, args);
		m_pAnaCollector->OnCustomEvent(rid, level, vip, GAME_EXTRA_EVENT_SND_GIFT, 0, 0, &args);
	}
}

void LogModule::OnFriendRecvGift(IKernel* pKernel, const PERSISTID& player, const wchar_t* pFriendName, int nGift, int nIntLevel, int nIncIntimacy)
{
	if (NULL != m_pAnaCollector)
	{
		IGameObj* pPlayer = pKernel->GetGameObj(player);
		if (NULL == pPlayer)return;

		//const char* uid = pPlayer->QueryString(FIELD_PROP_ACCOUNT);
		const char* rid = pPlayer->QueryString(FIELD_PROP_UID);
		int level = pPlayer->QueryInt(FIELD_PROP_LEVEL);
		int vip = pPlayer->QueryInt(FIELD_PROP_VIP_LEVEL);


		KVArgs args;
		//args[GAME_EXTRA_PROPERTIES_FRIEND_SELF_NAME] = StringUtil::WideStrAsUTF8String(pPlayer->QueryWideStr(FIELD_PROP_NAME));
		args[GAME_EXTRA_PROPERTIES_FRIEND_NAME] = StringUtil::WideStrAsUTF8String(pFriendName);
		//args[GAME_EXTRA_PROPERTIES_FRIEND_RID] = pFriendId;
		args[GAME_EXTRA_PROPERTIES_FRIEND_GIFT_NUM] = nGift;
		args[GAME_EXTRA_PROPERTIES_FRIEND_INTLEVEL] = nIntLevel;
		args[GAME_EXTRA_PROPERTIES_FRIEND_INC_INTIMACY] = nIncIntimacy;
		FillCommonArgs(pKernel, player, args);
		m_pAnaCollector->OnCustomEvent(rid, level, vip, GAME_EXTRA_EVENT_RCV_GIFT, 0, 0, &args);
	}
}

void LogModule::OnFriendTeamFight(IKernel* pKernel, const PERSISTID& player, const wchar_t* pFriendName, int nIntLevel, int nIncIntimacy)
{
	if (NULL != m_pAnaCollector)
	{
		IGameObj* pPlayer = pKernel->GetGameObj(player);
		if (NULL == pPlayer)return;

		//const char* uid = pPlayer->QueryString(FIELD_PROP_ACCOUNT);
		const char* rid = pPlayer->QueryString(FIELD_PROP_UID);
		int level = pPlayer->QueryInt(FIELD_PROP_LEVEL);
		int vip = pPlayer->QueryInt(FIELD_PROP_VIP_LEVEL);

		KVArgs args;
		//args[GAME_EXTRA_PROPERTIES_FRIEND_SELF_NAME] = StringUtil::WideStrAsUTF8String(pPlayer->QueryWideStr(FIELD_PROP_NAME));
		args[GAME_EXTRA_PROPERTIES_FRIEND_NAME] = StringUtil::WideStrAsUTF8String(pFriendName);
		//args[GAME_EXTRA_PROPERTIES_FRIEND_RID] = pFriendId;
		args[GAME_EXTRA_PROPERTIES_FRIEND_INTLEVEL] = nIntLevel;
		args[GAME_EXTRA_PROPERTIES_FRIEND_INC_INTIMACY] = nIncIntimacy;
		FillCommonArgs(pKernel, player, args);
		m_pAnaCollector->OnCustomEvent(rid, level, vip, GAME_EXTRA_EVENT_TEAM_FIGHT, 0, 0, &args);
	}
}

void LogModule::OnFriendCharmChanged(IKernel* pKernel, const PERSISTID& player, int nCharm, int nTotalCharm, int nReason)
{
	if (NULL != m_pAnaCollector)
	{
		IGameObj* pPlayer = pKernel->GetGameObj(player);
		if (NULL == pPlayer)return;

		//const char* uid = pPlayer->QueryString(FIELD_PROP_ACCOUNT);
		const char* rid = pPlayer->QueryString(FIELD_PROP_UID);
		int level = pPlayer->QueryInt(FIELD_PROP_LEVEL);
		int vip = pPlayer->QueryInt(FIELD_PROP_VIP_LEVEL);

		KVArgs args;
		// ��������ֵ
		args[GAME_ANALYTICS_PROPERTIES_ROLE_CHARMS] = nTotalCharm;
		m_pAnaCollector->SetProfile(rid, args);

		// д���¼�
		args[GAME_ANALYTICS_PROPERTIES_COMM_C_VALUE] = nCharm;
		FillCommonArgs(pKernel, player, args);
		m_pAnaCollector->OnCustomEvent(rid, level, vip, GAME_EXTRA_EVENT_CAHRMS_CHANGED, 0, nReason, &args);

	}
}

void LogModule::OnSworn(IKernel*pKernel, const PERSISTID& player, const wchar_t* playerName, int isSameSex, int isAdd)
{
	IGameObj* pPlayer = pKernel->GetGameObj(player);
	if (NULL == pPlayer)return;

	const char* rid = pPlayer->QueryString(FIELD_PROP_UID);
	int level = pPlayer->QueryInt(FIELD_PROP_LEVEL);
	int vip = pPlayer->QueryInt(FIELD_PROP_VIP_LEVEL);

	KVArgs args;
	args[GAME_EXTRA_PROPERTIES_FRIEND_NAME] = StringUtil::WideStrAsUTF8String(playerName);
	args[GAME_EXTRA_PROPERTIES_FRIEND_IS_SAME_SEX] = isSameSex;
	args[GAME_EXTRA_PROPERTIES_FRIEND_IS_SWORN_ADD] = isAdd;
	FillCommonArgs(pKernel, player, args);
	m_pAnaCollector->OnCustomEvent(rid, level, vip, GAME_EXTRA_EVENT_SWORN, 0, 0, &args);

}

void LogModule::OnForbidSpeak(IKernel* pKernel, const PERSISTID& player, int64_t nInterval, int64_t nExpiredTime)
{
	if (NULL == m_pAnaCollector)
	{
		Assert(false);
		return;
	}

	LogBaseInfo baseInfo;
	if (!GetPlayerBaseInfo(pKernel, player, baseInfo))
	{
		return;
	}

	KVArgs args;
	args[GAME_EXTRA_FORBID_EXPIRED_TIME] = nExpiredTime;
	m_pAnaCollector->SetProfile(baseInfo.strUid.c_str(), args);

	args[GAME_EXTRA_DURATION_TIME] = nInterval;
	FillCommonArgs(pKernel, player, args);
	m_pAnaCollector->OnCustomEvent(baseInfo.strUid.c_str(), 0, 0, GAME_EXTRA_EVENT_FORBID_SPEAK, 0, FUNCTION_EVENT_ID_GMCC_WEB, &args);
}

void LogModule::OnAllowSpeak(IKernel* pKernel, const PERSISTID& player )
{
	if (NULL == m_pAnaCollector)
	{
		Assert(false);
		return;
	}

	LogBaseInfo baseInfo;
	if (!GetPlayerBaseInfo(pKernel, player, baseInfo))
	{
		return;
	}

	KVArgs args;
	args[GAME_EXTRA_FORBID_EXPIRED_TIME] = 0;
	m_pAnaCollector->SetProfile(baseInfo.strUid.c_str(), args);
	FillCommonArgs(pKernel, player, args);
	m_pAnaCollector->OnCustomEvent(baseInfo.strUid.c_str(), 0, 0, GAME_EXTRA_EVENT_ALLOW_SPEAK, 0, FUNCTION_EVENT_ID_GMCC_WEB, &args);
}

void LogModule::OnBlockPlayer(IKernel* pKernel, const PERSISTID& player, int64_t nInterval, int64_t nExpiredTime)
{
	if (NULL == m_pAnaCollector)
	{
		Assert(false);
		return;
	}

	LogBaseInfo baseInfo;
	if (!GetPlayerBaseInfo(pKernel, player, baseInfo))
	{
		return;
	}

	KVArgs args;
	args[GAME_EXTRA_BLOCK_EXPIRED_TIME] = nExpiredTime;
	m_pAnaCollector->SetProfile(baseInfo.strUid.c_str(), args);

	args[GAME_EXTRA_DURATION_TIME] = nInterval;
	FillCommonArgs(pKernel, player, args);

	m_pAnaCollector->OnCustomEvent(baseInfo.strUid.c_str(), 0, 0, GAME_EXTRA_EVENT_BLOCK_PLAYER, 0, FUNCTION_EVENT_ID_GMCC_WEB, &args);
}

void LogModule::OnBlockPlayer(IKernel* pKernel, const char* pszRoleId, int64_t nInterval, int64_t nExpiredTime)
{
	if (NULL == m_pAnaCollector)
	{
		Assert(false);
		return;
	}

	KVArgs args;
	args[GAME_EXTRA_BLOCK_EXPIRED_TIME] = nExpiredTime;
	m_pAnaCollector->SetProfile(pszRoleId, args);

	args[GAME_EXTRA_DURATION_TIME] = nInterval;

	m_pAnaCollector->OnCustomEvent(pszRoleId, 0, 0, GAME_EXTRA_EVENT_BLOCK_PLAYER, 0, FUNCTION_EVENT_ID_GMCC_WEB, &args);
}

void LogModule::OnResumePlayer(IKernel* pKernel, const PERSISTID& player)
{
	if (NULL == m_pAnaCollector)
	{
		Assert(false);
		return;
	}

	LogBaseInfo baseInfo;
	if (!GetPlayerBaseInfo(pKernel, player, baseInfo))
	{
		return;
	}

	KVArgs args;
	args[GAME_EXTRA_BLOCK_EXPIRED_TIME] = 0;
	m_pAnaCollector->SetProfile(baseInfo.strUid.c_str(), args);
	FillCommonArgs(pKernel, player, args);

	m_pAnaCollector->OnCustomEvent(baseInfo.strUid.c_str(), 0, 0, GAME_EXTRA_EVENT_RESUME_PLAYER, 0, FUNCTION_EVENT_ID_GMCC_WEB, &args);
}

void LogModule::OnResumePlayer(IKernel* pKernel, const char* pszRoleId)
{
	if (NULL == m_pAnaCollector)
	{
		Assert(false);
		return;
	}

	KVArgs args;
	args[GAME_EXTRA_BLOCK_EXPIRED_TIME] = 0;
	m_pAnaCollector->SetProfile(pszRoleId, args);

	m_pAnaCollector->OnCustomEvent(pszRoleId, 0, 0, GAME_EXTRA_EVENT_RESUME_PLAYER, 0, FUNCTION_EVENT_ID_GMCC_WEB, &args);
}

void LogModule::FillCommonArgs(IKernel*pKernel, const PERSISTID&player, KVArgs& args)
{
	IGameObj *pSelfObj = pKernel->GetGameObj(player);
	if (pSelfObj == NULL){
		return;
	}

	args[GAME_ANALYTICS_PROPERTIES_UID] = pSelfObj->QueryString(FIELD_PROP_ACCOUNT);
}

// ��ȡ��ҵĻ�������
bool LogModule::GetPlayerBaseInfo(IKernel *pKernel, const PERSISTID& player, LogBaseInfo& outBaseInfo)
{
	IGameObj* pPlayer = pKernel->GetGameObj(player);
	if (NULL == pPlayer)
	{
		return false;
	}
	
	outBaseInfo.strAccount	= pPlayer->QueryString(FIELD_PROP_ACCOUNT);
	outBaseInfo.strUid		= pPlayer->QueryString(FIELD_PROP_UID);
	outBaseInfo.nLevel		= pPlayer->QueryInt(FIELD_PROP_LEVEL);
	outBaseInfo.nVipLv		= pPlayer->QueryInt(FIELD_PROP_VIP_LEVEL);
	//outBaseInfo.nJob		= pPlayer->QueryInt(FIELD_PROP_JOB);

	return true;
}

/*!
* @brief	��ҵ�½��־
* @param    ����ָ��
* @param    ��Ҷ���
* @param    ��ɫ�ṹ
* @return	���bool
*/
bool LogModule::SaveRoleLog(IKernel* pKernel, const PERSISTID& player, const RoleLog &log)
{
	//1�������ݴ�
	if ( NULL == pKernel || !pKernel->Exists(player) || pKernel->Type(player) != TYPE_PLAYER )
	{
		return false;
	}
	IGameObj *playerObj = pKernel->GetGameObj(player);
	if (NULL == playerObj)
	{
		return false;
	}

	//3����ע�ַ����ȼ��
	std::wstring strComment = log.comment;
	if (log.comment.length() >= COMMENT_MAX_LEN)
	{
		strComment = StringUtil::SubstrW(log.comment.c_str(), 0, COMMENT_MAX_LEN - 1 );
	}

	//4������˺ź�����
	const char* account = playerObj->QueryString("Account");
	if (StringUtil::CharIsNull(account))
	{
		::extend_warning(LOG_ERROR, "[LogModule::SaveRoleLog] account is NULL or empty! ip -> %s ", log.ip.c_str());
		return false;
	}
	const wchar_t* name = playerObj->QueryWideStr("Name");
	if (StringUtil::CharIsNull(name))
	{
		::extend_warning(LOG_ERROR, "[LogModule::SaveRoleLog] name is NULL or empty! ip -> %s ", log.ip.c_str());
		return false;
	}

	//5����ȡ��¼ʱ�䣬����ʱ��
	std::string strTimeIn = m_pLogModule->GetLogTime(log.loginTime);
	std::string strTimeOut = m_pLogModule->GetLogTime(log.logoutTime);

	//6��������ҵ�¼��־
	CVarList logInfo;
	logInfo << account << name << log.ip << strTimeIn.c_str() << strTimeOut.c_str() << log.mac << log.osType << strComment.c_str();

	return pKernel->CustomLog("role_log", logInfo);
}

/*!
* @brief	��¼��Ϸ�淨��־
* @param    ����ָ��
* @param    ��Ҷ���
* @param    �淨�ṹ
* @return	���bool
*/
bool LogModule::SaveGameActionLog(IKernel* pKernel, const PERSISTID& player, const GamePlayerActionLog &log)
{
	//1�������ݴ��ж�
	if (NULL == pKernel)
	{
		return false;
	}

	//2�������ʱ���������ֻ���ǳƼ�¼
	const char* account = "";
	const wchar_t* name = L"";
	//int nation = 0;
	if (pKernel->Exists(player) && pKernel->Type(player) == TYPE_PLAYER)
	{
		IGameObj *playerObj = pKernel->GetGameObj(player);
		if (NULL == playerObj)
		{
			return false;
		}

		//3������˺ź�����
		account = playerObj->QueryString("Account");
		if (StringUtil::CharIsNull(account))
		{
			::extend_warning(LOG_ERROR, "[LogModule::SaveGameActionLog] account is NULL or empty! logType -> %d ", log.actionType);
			return false;
		}
		name = playerObj->QueryWideStr("Name");
		if (StringUtil::CharIsNull(name))
		{
			::extend_warning(LOG_ERROR, "[LogModule::SaveGameActionLog] name is NULL or empty! logType -> %d ", log.actionType);
			return false;
		}

		//nation = playerObj->QueryInt("Nation");

		// Analytics
		if (NULL != m_pAnaCollector)
		{
			const char* rid = playerObj->QueryString(FIELD_PROP_UID);
			int level = playerObj->QueryInt(FIELD_PROP_LEVEL);
			int vip = playerObj->QueryInt(FIELD_PROP_VIP_LEVEL);
			int job = playerObj->QueryInt(FIELD_PROP_JOB);
			KVArgs args;
			FillCommonArgs(pKernel, player, args);
			m_pAnaCollector->OnVisitFunction(rid, level, vip, 0, log.actionType, true, log.actionState, NULL, &args);
			//m_pAnaCollector->OnCustomEvent(account, rid, job, level, vip, "game_custom", log.actionType, log.actionState, NULL);
		}
	}
	else
	{
		name = log.name.c_str();
	}

	//4����ȡ��־ʱ�����Ϸ�淨ID
	std::string strTime = m_pLogModule->GetLogTime();

	//5����ע�ַ����ȼ��
	std::wstring strComment = log.comment;
	if (log.comment.length() >= COMMENT_MAX_LEN)
	{
		strComment = StringUtil::SubstrW(log.comment.c_str(), 0, COMMENT_MAX_LEN - 1);
	}

	//6����ȡ��ʵ����ID
	int sceneID = pKernel->GetSceneId();
	if (pKernel->GetSceneClass() == 2)
	{
		sceneID = pKernel->GetPrototypeSceneId(sceneID);
	}

	//7����װ����
	CVarList logInfo;
	logInfo << account << name << strTime.c_str() << log.actionType << sceneID << log.actionState << strComment.c_str();

	//8��������Ϸ�淨��־
	return pKernel->CustomLog("game_player_action_log", logInfo);
}


/*!
* @brief	������־
* @param    ����ָ��
* @param    ��Ҷ���
* @param    ����ṹ
* @return	���bool
*/
bool LogModule::SaveTaskLog(IKernel* pKernel, const PERSISTID& player, const TaskLog &log)
{
	//1�������ݴ�
	if ( NULL == pKernel || !pKernel->Exists(player) || pKernel->Type(player) != TYPE_PLAYER )
	{
		return false;
	}
	IGameObj *playerObj = pKernel->GetGameObj(player);
	if (NULL == playerObj)
	{
		return false;
	}

	//3����ע�ַ����ȼ��
	std::wstring strComment = log.comment;
	if (log.comment.length() >= COMMENT_MAX_LEN)
	{
		strComment = StringUtil::SubstrW(log.comment.c_str(), 0, COMMENT_MAX_LEN - 1 );
	}

	//4������˺ź�����
	const char* account = playerObj->QueryString("Account");
	if (StringUtil::CharIsNull(account))
	{
		::extend_warning(LOG_ERROR, "[LogModule::SaveTaskLog] account is NULL or empty! taskID -> %d ", log.taskID);
		return false;
	}
	const wchar_t* name = playerObj->QueryWideStr("Name");
	if (StringUtil::CharIsNull(name))
	{
		::extend_warning(LOG_ERROR, "[LogModule::SaveTaskLog] name is NULL or empty! taskID -> %d ", log.taskID);
		return false;
	}

	//5����ȡ��ǰ��־ʱ�������ID
	std::string strTime = m_pLogModule->GetLogTime();
	std::string strTaskID = StringUtil::IntAsString(log.taskID);
	

	//6����װ������־����
	CVarList logInfo;
	logInfo << account << name << strTime.c_str() << strTaskID.c_str() << log.type << log.state << strComment.c_str();

	//7��������־
	return pKernel->CustomLog("task_log", logInfo);
}


/*!
* @brief	��Ǯ��־
* @param    ����ָ��
* @param    ��Ҷ���
* @param    ��Ǯ�ṹ
* @return	���bool
*/
bool LogModule::SaveMoneyLog(IKernel* pKernel, const PERSISTID& player, const MoneyLog &log)
{
	//1�������ݴ�
	if ( NULL == pKernel || !pKernel->Exists(player) || pKernel->Type(player) != TYPE_PLAYER )
	{
		return false;
	}
	IGameObj *playerObj = pKernel->GetGameObj(player);
	if (NULL == playerObj)
	{
		return false;
	}

	//3������˺ź�����
	const char* account = playerObj->QueryString("Account");
	if (StringUtil::CharIsNull(account))
	{
		::extend_warning(LOG_ERROR, "[LogModule::SaveMoneyLog] account is NULL or empty! eventID -> %d ", log.eventID);
		return false;
	}
	const wchar_t* name = playerObj->QueryWideStr("Name");
	if (StringUtil::CharIsNull(name))
	{
		::extend_warning(LOG_ERROR, "[LogModule::SaveMoneyLog] name is NULL or empty! eventID -> %d ", log.eventID);
		return false;
	}

    //4����ȡ���Uid
    const char* uid = pKernel->SeekRoleUid(name);
	if( StringUtil::CharIsNull(uid) )
	{
		return false;
	}

    //5��ʱ��
	std::string strTime = m_pLogModule->GetLogTime();

	//6����ȡ��ʵ����ID
	int sceneID = pKernel->GetSceneId();
	if(pKernel->GetSceneClass() == 2)
	{
		sceneID = pKernel->GetPrototypeSceneId(sceneID);
	}
	
	//7���жϹ�������
	const wchar_t* guildName = playerObj->QueryWideStr("GuildName");
	if ( StringUtil::CharIsNull(guildName) )
	{
		guildName = L"";
	}

	//9����ע�ַ����ȼ��
	std::wstring strComment = log.comment;
	if (log.comment.length() >= COMMENT_MAX_LEN)
	{
		strComment = StringUtil::SubstrW(log.comment.c_str(), 0, COMMENT_MAX_LEN - 1 );
	}

	//10����װ��Ǯ�춯��־
	CVarList logInfo;
	logInfo << strTime.c_str() << -1 << -1 << account << name << uid 
		    << log.eventID << log.eventType  << log.eventSerial << log.capitalType
		    << log.before << log.money << log.after << log.senderName << log.senderUid 
			<< sceneID << "" << guildName << log.state << strComment.c_str();

	// ANALYTICES:
	if (NULL != m_pAnaCollector)
	{
		// �ʼ�ת���Ĳ���¼�����
		if (log.eventID != FUNCTION_EVENT_ID_ITEM_TO_MAIL && log.eventID != FUNCTION_EVENT_ID_SYSTEM_MAIL)
		{
			if (log.eventType == LOG_ACT_EVENT_GAIN)
			{
				OnGainCoin(pKernel, player, log.capitalType, log.money, log.after, log.before, 0, log.eventID, NULL);
			}
			else
			{
				OnCostCoin(pKernel, player, log.capitalType, log.money, log.after, log.before, 0, log.eventID, NULL);
			}
		}
	}

	//11�������Ǯ�춯��־
	return pKernel->CustomLog("money_log", logInfo);
}


/*!
* @brief	��Ʒ��־��
* @param    ����ָ��
* @param    ��Ҷ���
* @param    ��Ʒ�ṹ
* @return	���bool
*/
bool LogModule::SaveItemLog(IKernel* pKernel, const PERSISTID& player, const ItemLog &log)
{
	//1�������ݴ�
	if ( NULL == pKernel || !pKernel->Exists(player)  || pKernel->Type(player) != TYPE_PLAYER )
	{
		return false;
	}
	IGameObj *playerObj = pKernel->GetGameObj(player);
	if (NULL == playerObj)
	{
		return false;
	}

	//3���ж��˺ź��������
	const char* account = playerObj->QueryString("Account");
	if (StringUtil::CharIsNull(account))
	{
		::extend_warning(LOG_ERROR, "[LogModule::SaveItemLog] account is NULL or empty! eventID -> %d ", log.eventID);
		return false;
	}
	const wchar_t* name = playerObj->QueryWideStr("Name");
	if (StringUtil::CharIsNull(name))
	{
		::extend_warning(LOG_ERROR, "[LogModule::SaveItemLog] name is NULL or empty! eventID -> %d ", log.eventID);
		return false;
	}

	//4����ȡ���Uid
	const char* uid = pKernel->SeekRoleUid(name);
	if( StringUtil::CharIsNull(uid) )
	{
		return false;
	}

	//5��ʱ��
	std::string strTime = m_pLogModule->GetLogTime();

	//6����ȡ��ʵ����ID
	int sceneID = pKernel->GetSceneId();
	if(pKernel->GetSceneClass() == 2)
	{
		sceneID = pKernel->GetPrototypeSceneId(sceneID);
	}

	//7���жϹ�������
	const wchar_t* guildName = playerObj->QueryWideStr("GuildName");
	if ( StringUtil::CharIsNull(guildName) )
	{
		guildName = L"";
	}

	//8����ע�ַ����ȼ��
	std::wstring strComment = log.comment;
	if (log.comment.length() >= COMMENT_MAX_LEN)
	{
		strComment = StringUtil::SubstrW(log.comment.c_str(), 0, COMMENT_MAX_LEN - 1 );
	}

	//9�������װ��itemNumΪƷ��
	int num = log.itemNum;
	int colorLevel = log.colorLevel;
	if(colorLevel < 0)
	{
#ifndef FSROOMLOGIC_EXPORTS
		if (ToolItemModule::IsCanWearItem(pKernel, log.itemID.c_str()))
		{
			num = 1;
			colorLevel = log.itemNum;
		}
		else
#endif
		{
			num = log.itemNum;
			const char* colorProperty = pKernel->GetConfigProperty(log.itemID.c_str(), "ColorLevel");  //���õ���ɫƷ��
			colorLevel = StringUtil::StringAsInt(colorProperty);
		}
	}

	int nAfter = log.after;
	int nBefore = log.before;
	if (nAfter == 0 && nBefore == 0)
	{
		// ȡ������ǰ���ݼ�������
		PERSISTID toolBox = pKernel->GetChild(player, ITEM_BOX_NAME_WSTR);
		if ( pKernel->Exists(toolBox) )
		{
			nAfter = ContainerModule::GetItemCount(pKernel, toolBox, log.itemID.c_str());
			if (log.eventType == LOG_ACT_EVENT_GAIN)
			{
				nBefore = nAfter - num;
			}
			else
			{
				nBefore = nAfter + num;
			}
		}
	}
	
	//10����װ��Ʒ�춯����
	CVarList logInfo;
	logInfo << strTime.c_str() << -1 << -1 << account << name << uid 
	     	<< log.eventID << log.eventType << log.eventSerial
		    << log.itemID << log.itemUid << log.itemLifeTime 
			<< nBefore << num << nAfter << colorLevel 
			<< log.senderName << log.senderUid
			<< sceneID << "" << guildName << log.state << strComment.c_str();

	// ANALYTICES:
	if (NULL != m_pAnaCollector)
	{
		// �ʼ�ת���Ĳ���¼�����
		if (log.eventID != FUNCTION_EVENT_ID_ITEM_TO_MAIL && log.eventID != FUNCTION_EVENT_ID_SYSTEM_MAIL
			&& log.eventID != FUNCTION_EVENT_ID_GM_COMMAND 
			&& log.eventID != FUNCTION_EVENT_ID_SYS )
		{
			int level = playerObj->QueryInt(FIELD_PROP_LEVEL);
			int vip = playerObj->QueryInt(FIELD_PROP_VIP_LEVEL);
			if (log.eventType == LOG_ACT_EVENT_GAIN)
			{
				OnGainItem(pKernel, player, log.itemID.c_str(), colorLevel, num, nAfter, nBefore, 0, log.eventID, NULL);
			}
			else
			{
				OnCostItem(pKernel, player, log.itemID.c_str(), colorLevel, num, nAfter, nBefore, 0, log.eventID, NULL);
			}
		}
	}

	//11��������Ʒ�춯��־
	return pKernel->CustomLog("item_log", logInfo);
}


/*!
* @brief	������־(������ܡ�����)
* @param    ����ָ��
* @param    ��Ҷ���
* @param    ������־�ṹ
* @return	���bool
*/
bool LogModule::SaveRoleUpgradeLog(IKernel* pKernel, const PERSISTID& player, const RoleUpgradeLog &log)
{
	// 1�������ݴ�
	if ( NULL == pKernel || !pKernel->Exists(player) || pKernel->Type(player) != TYPE_PLAYER )
	{
		return false;
	}
	IGameObj *playerObj = pKernel->GetGameObj(player);
	if (NULL == playerObj)
	{
		return false;
	}

	//3���ж��˺ź��������
	const char* account = playerObj->QueryString("Account");
	if (StringUtil::CharIsNull(account))
	{
		::extend_warning(LOG_ERROR, "[LogModule::SaveRoleUpgradeLog] account is NULL or empty! type -> %d ", log.type);
		return false;
	}
	const wchar_t* name = playerObj->QueryWideStr("Name");
	if (StringUtil::CharIsNull(name))
	{
		::extend_warning(LOG_ERROR, "[LogModule::SaveRoleUpgradeLog] name is NULL or empty! type -> %d ", log.type);
		return false;
	}

	//4����־ʱ��
	std::string strTime = m_pLogModule->GetLogTime();

	//5����ע�ַ����ȼ��
	std::wstring strComment = log.comment;
	if (log.comment.length() >= COMMENT_MAX_LEN)
	{
		strComment = StringUtil::SubstrW(log.comment.c_str(), 0, COMMENT_MAX_LEN - 1 );
	}

	//6����־����
	CVarList logInfo;
	logInfo << account << name << strTime.c_str() << log.type << log.levelBefore << log.levelAfter << log.skillID << strComment.c_str();

	// ANALYTICS:
	if (NULL != m_pAnaCollector)
	{
		switch ( log.type )
		{
		case LOG_ROLE_UPGRADE_PLAYER:
			this->OnLevelup(pKernel, player, log.levelAfter, log.levelBefore);
			break;
		default:
			break;
		}
	}

	//7������������־
	return pKernel->CustomLog("role_upgrade_log", logInfo);
}

/*!
* @brief	BOSS��־��¼
* @param    ����ָ��
* @param    ��Ҷ���
* @param    BOSS��־�ṹ
* @return	���bool
*/
bool LogModule::SaveBossLog(IKernel* pKernel, const PERSISTID& player, const BossLog &log)
{
	//1�������ݴ�
	if ( NULL == pKernel || !pKernel->Exists(player) || pKernel->Type(player) != TYPE_PLAYER )
	{
		return false;
	}
	//�ƺ�
	//MiddleModule::Instance()->OnTitleConditionStr(pKernel, player, ETitleCond_KillBossID, log.bossID.c_str());

	IGameObj *playerObj = pKernel->GetGameObj(player);
	if (NULL == playerObj)
	{
		return false;
	}

	//3���˺ź����ֺϷ��Լ��
	const char* account = playerObj->QueryString("Account");
	if (StringUtil::CharIsNull(account))
	{
		::extend_warning(LOG_ERROR, "[LogModule::SaveBossLog] account is NULL or empty! bossID -> %d ", log.bossID);
		return false;
	}
	const wchar_t* name = playerObj->QueryWideStr("Name");
	if (StringUtil::CharIsNull(name))
	{
		::extend_warning(LOG_ERROR, "[LogModule::SaveBossLog] name is NULL or empty! bossID -> %d ", log.bossID);
		return false;
	}

	//4����־ʱ��
	std::string strTime = m_pLogModule->GetLogTime();

	//5����ע�ַ����ȼ��
	std::wstring strComment = log.comment;
	if (log.comment.length() >= COMMENT_MAX_LEN)
	{
		strComment = StringUtil::SubstrW(log.comment.c_str(), 0, COMMENT_MAX_LEN - 1 );
	}

	//6����ȡ��ʵ����ID
	int sceneID = pKernel->GetSceneId();
	if(pKernel->GetSceneClass() == 2)
	{
		sceneID = pKernel->GetPrototypeSceneId(sceneID);
	}

	//7������Boss��־
	CVarList logInfo;
	logInfo << account << name << strTime.c_str() << sceneID << log.bossID << strComment.c_str();

	return pKernel->CustomLog("boss_log", logInfo);
}

/*!
* @brief	������־
* @param    ����ָ��
* @param    ��Ҷ���
* @param    ������־�ṹ
* @return	���bool
*/
bool LogModule::SaveSceneLog(IKernel* pKernel, const PERSISTID& player, const SceneLog &log)
{
	//1�������ݴ�
	if ( NULL == pKernel || !pKernel->Exists(player)  || pKernel->Type(player) != TYPE_PLAYER )
	{
		return false;
	}
	IGameObj *playerObj = pKernel->GetGameObj(player);
	if (NULL == playerObj)
	{
		return false;
	}

	//3���˺ź����ֺϷ��Լ��
	const char* account = playerObj->QueryString("Account");
	if (StringUtil::CharIsNull(account))
	{
		::extend_warning(LOG_ERROR, "[LogModule::SaveSceneLog] account is NULL or empty! ");
		return false;
	}
	const wchar_t* name = playerObj->QueryWideStr("Name");
	if (StringUtil::CharIsNull(name))
	{
		::extend_warning(LOG_ERROR, "[LogModule::SaveSceneLog] name is NULL or empty! ");
		return false;
	}

	//4����ע�ַ����ȼ��
	std::wstring strComment = log.comment;
	if (log.comment.length() >= COMMENT_MAX_LEN)
	{
		strComment = StringUtil::SubstrW(log.comment.c_str(), 0, COMMENT_MAX_LEN - 1 );
	}

	//5����ȡ��ʵ����ID
	int sceneID = pKernel->GetSceneId();
	if(pKernel->GetSceneClass() == 2)
	{
		sceneID = pKernel->GetPrototypeSceneId(sceneID);
	}

	//6����ȡ��¼ʱ�䣬����ʱ��
	std::string strTimeIn = m_pLogModule->GetLogTime(log.loginTime);
	std::string strTimeOut = m_pLogModule->GetLogTime(log.logoutTime);
	int64_t totalTime = log.logoutTime - log.loginTime;

	//7������Boss��־
	CVarList logInfo;
	logInfo << account << name << sceneID << strTimeIn.c_str() << strTimeOut.c_str() << totalTime << strComment.c_str();

	return pKernel->CustomLog("scene_log", logInfo);
}



/*!
* @brief	��¼�����־
* @param    ����ָ��
* @param    ��Ҷ���
* @param    ��־����
* @param    ��չ����
* @return	���bool
*/
bool LogModule::SavePlayerLog(IKernel* pKernel, const PERSISTID& player,
							  const int logType, const wchar_t* comment)
{
	//1�������ݴ�Ͳ��������ж�
	if ( NULL == pKernel || !pKernel->Exists(player) || pKernel->Type(player) != TYPE_PLAYER )
	{
		return false;
	}
	IGameObj *playerObj = pKernel->GetGameObj(player);
	if (NULL == playerObj)
	{
		return false;
	}

	//3�������ж�
	std::wstring strComment = L"";
	if( !StringUtil::CharIsNull(comment) )
	{
		if( std::wstring(comment).length() >= COMMENT_MAX_LEN )
		{
			strComment = StringUtil::SubstrW(comment, 0, COMMENT_MAX_LEN - 1 );
		}
		else
		{
			strComment = comment;
		}
	}

	//4�� ���ֺϷ���
	const wchar_t* name = playerObj->QueryWideStr("Name");
	if (StringUtil::CharIsNull(name))
	{
		::extend_warning(LOG_ERROR, "[LogModule::SavePlayerLog without Param:name] name is NULL or empty! ");
		return false;
	}

	//5����ע
	const wchar_t* content = L"SavePlayerLog";

	//6�����������־
	return pKernel->SaveLog(name, logType, content, strComment.c_str());
}


/*!
* @brief	GM��־
* @param    ����ָ��
* @param    ����
* @param    ��չ����
* @return	���bool
*/
bool LogModule::SaveGMLog(IKernel* pKernel, const PERSISTID& player, const wchar_t* GMContent, const wchar_t* comment)
{
	//1�������ݴ�Ͳ����Ϸ��Լ��
	if ( NULL == pKernel || !pKernel->Exists(player) || StringUtil::CharIsNull(GMContent) || StringUtil::CharIsNull(comment) )
	{
		return false;
	}
	IGameObj *playerObj = pKernel->GetGameObj(player);
	if (NULL == playerObj)
	{
		return false;
	}

	//3���˺�
	const char* acc = playerObj->QueryString("Account");
	if (StringUtil::CharIsNull(acc))
	{
		::extend_warning(LOG_ERROR, "[LogModule::SaveGMLog] account is NULL or empty! ");
		return false;
	}
	std::wstring account = std::wstring(L"*") + ::util_string_as_widestr(acc);

	// ����GM������־
	return pKernel->GmLog(account.c_str(), 0, GMContent, comment);
}

/*!
* @brief	�ʼ�������־
* @param    ����ָ��
* @param    �ʼ�������־�ṹ
* @return	���bool
*/
bool LogModule::SaveMailAwardLog( IKernel* pKernel, const MailLog &log )
{
	const wchar_t* name = log.name.c_str();
	if (StringUtil::CharIsNull(name))
	{
		::extend_warning(LOG_ERROR, "[LogModule::SaveMailAwardLog] name is NULL or empty! logType -> %s ", log.templateType.c_str());
		return false;
	}

	// 2����־ʱ��
	std::string strTime = m_pLogModule->GetLogTime();

	// 3����ע�ַ����ȼ��
	std::wstring strComment = log.comment;
	std::string strItemParams = log.itemParams;
	
	if (log.comment.length() >= COMMENT_MAX_LEN)
	{
		strComment = StringUtil::SubstrW(log.comment.c_str(), 0, COMMENT_MAX_LEN - 1 );
	}

	if (log.itemParams.length() >= COMMENT_MAX_LEN)
	{
		strItemParams = StringUtil::Substr(log.itemParams.c_str(), 0, COMMENT_MAX_LEN - 1 );
	}

	// 4�������ʼ�������־
	CVarList logInfo;
	logInfo << "" << name << strTime.c_str() 
		    << log.templateType.c_str() << log.capitalParams.c_str() 
			<< strItemParams.c_str() << strComment.c_str();
	return pKernel->CustomLog("mail_log", logInfo);
}

//��������
int LogModule::OnCreate(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args)
{
	if (pKernel->GetSceneId() != STATICS_SCENE_ID)
	{
		return 0;
	}
	
	// 10sһ������ ��֤ƫ��ֵ��0-10֮��
	ADD_HEART_BEAT(pKernel, self, "LogModule::H_RecOnlineCount", 10000);
	return 0;
}

// ��¼���������
int LogModule::H_RecOnlineCount(IKernel* pKernel, const PERSISTID& creator, int slice)
{
	if (NULL == m_pLogModule->m_pAnaCollector)
	{
		return 0;
	}

	// Ĭ��ֵΪ0�� ������һ��ִ�б������
	time_t tNow = util_get_utc_time();
	if (tNow >= ms_nNextReportCount)
	{
		int nCurOnlineCount = 0;
#ifndef FSROOMLOGIC_EXPORTS
		nCurOnlineCount = pKernel->GetOnlineCount();
#endif
		char szid[32] = { 0 };
#ifndef FSROOMLOGIC_EXPORTS
		SPRINTF_S(szid, "%d", pKernel->GetServerId());
#else
		SPRINTF_S(szid, "%d", pKernel->GetRoomId());
#endif

		KVArgs args;
		args[GAME_ANALYTICS_PROPERTIES_ONLINE_COUNT] = nCurOnlineCount;
		m_pLogModule->m_pAnaCollector->OnCustomEvent(szid, 0, 0, GAME_ANALYTICS_EVENT_ONLINE_COUNT, 0, 0, &args);

		// �����һ��ʱ��
		//int hour = 0, minute = 0, seconds = 0;
		//// ��֤��������ֵ
		//int nWeek = util_get_day_of_week_ex(tNow + HB_REPORT_COUNT_INTERVAL, hour, minute, seconds);
		//seconds += minute * 60 + hour * 3600;
		//seconds %= REPORT_COUNT_INTERVAL;
		time_t seconds = tNow % REPORT_COUNT_INTERVAL;
		ms_nNextReportCount = tNow - seconds + REPORT_COUNT_INTERVAL;
	}
	return 0;
}

// ��ȡ��׼��ʽ����־ʱ��
std::string LogModule::GetLogTime(time_t slice)
{
	if (slice <= 0)
	{
		slice = time(NULL);
	}

	tm* t = ::localtime(&slice);
	if (NULL == t)
	{
		return std::string("");
	}
	char buf[32] = "\0";
	SPRINTF_S(buf, "%04d%02d%02d %02d:%02d:%02d", 
		t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);

	return buf;
}