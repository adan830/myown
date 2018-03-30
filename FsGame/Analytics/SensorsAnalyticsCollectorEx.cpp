#include "SensorsAnalyticsCollectorEx.h"
#include "public/Debug.h"
#include "utils/util_func.h"
#include "GameAnalyticsEventNames.h"
#include "GameAnalyticsProperties.h"
#include "utils/string_util.h"
#include "utils/util_ini.h"
#include "Define/GameDefine.h"

SensorsAnalyticsCollectorEx::SensorsAnalyticsCollectorEx(IAnalytisLogger& logger, 
												int64_t nDeployId,			// ����id
												int64_t nGameId,
												int64_t nSrvId,
												int64_t nDistrictId,
												int64_t nChannelId,				// ��Ϸ���������� ���˻�ר������
												int64_t nProductionId,			// ����id: ��ʽ�������ǿ������� ...
												const char* gameVer, 
												const char* gameName,
												const char* strDistrictlName )
	: m_nDeployId(nDeployId)
	, m_nGameId(nGameId)
	, m_nSrvId(nSrvId)
	, m_nDistrictId( nDistrictId )
	, m_nChannelId( nChannelId )
	, m_nProductionId( nProductionId )
	, m_anaLogger(logger)
	
{
	if (NULL != gameVer)
	{
		m_strGameVer = gameVer;
	}

	if (NULL != gameName)
	{
		m_strGameName = gameName;
	}

	if (NULL != strDistrictlName)
	{
		m_strChannelName = strDistrictlName;
	}
}

void SensorsAnalyticsCollectorEx::FillSensorsTrackHeader(Json::Value& root,
										const char* type,
										const char* evt,
										const char* roleid,
										int level )
{
	Assert(NULL != roleid);
	Assert(NULL != type);
	Assert(NULL != evt);

	root[SENSORS_INNER_DISTINCT_ID] = roleid;
	root[SENSORS_INNER_TYPE] = type;
	root[SENSORS_INNER_EVENT] = evt;

	// ʱ�䴦��
	int64_t tNow = util_get_utc_ms_time();
	
		// �ж�����Ƿ��ڻ�����
	auto it = m_strRoleCache.find(roleid);
	if (it != m_strRoleCache.end())
	{
		if (tNow <= it->second)
		{
#ifdef FSGAME_EXPORTS
			if (it->second - tNow >= 5000)
			{
				::extend_warning(LOG_WARNING, "role event too much role=%s",roleid);
			}
#endif
			it->second += 1;
			// ����ʱ�䵽��һms
			tNow = it->second;
		}
		else
		{
			it->second = tNow;
		}
		
	}
	else
	{
		m_strRoleCache[roleid] = tNow;
	}
	// ʹ�ú����ʱ��
	root[SENSORS_INNER_TIME] = tNow;

	if (!m_strGameName.empty())
	{
		root[SENSORS_INNER_PROJECT] = m_strGameName;
	}

	// �������
	Json::Value& properties = root[SENSORS_INNER_PROPERTYIES];
	properties[GAME_ANALYTICS_PROPERTIES_DEPLOY_ID] = m_nDeployId;
	properties[GAME_ANALYTICS_PROPERTIES_GAME_ID] = m_nGameId;
	properties[GAME_ANALYTICS_PROPERTIES_SERVER_ID] = m_nSrvId;
	properties[GAME_ANALYTICS_PROPERTIES_DISTRICT_ID] = m_nDistrictId;
	properties[GAME_ANALYTICS_PROPERTIES_GAME_VER] = m_strGameVer;
	properties[GAME_ANALYTICS_PROPERTIES_CHANNEL] = m_nChannelId;
	properties[GAME_ANALYTICS_PROPERTIES_PRODUCTION_ID] = m_nProductionId;
	//properties[GAME_ANALYTICS_PROPERTIES_DISTRICT_NAME] = m_strChannelName;
	// ����
	// std::string date;
	// util_convert_date_to_string2(date);
	// properties[GAME_ANALYTICS_PROPERTIES_DATE] = date;
	
	// ��ɫid
	//properties[GAME_ANALYTICS_PROPERTIES_UID] = uid;
	properties[GAME_ANALYTICS_PROPERTIES_ROLE_ID] = roleid;
	if (level > 0)
	{
		properties[GAME_ANALYTICS_PROPERTIES_LEVEL] = level;
	}
}

void SensorsAnalyticsCollectorEx::FillModuleInfo(Json::Value& root, int module, int func, const char* extra, const KVArgs* args/* = NULL*/) const
{
	//const char* pModuleName = GetModuleName(module);
	//const char* pFuncName = GetFuncName(func);

	//InsertString(root, GAME_ANALYTICS_PROPERTIES_GAME_MODULE, pModuleName);
	//InsertString(root, GAME_ANALYTICS_PROPERTIES_GAME_ACTION, pFuncName);
	if (module == 0)
	{
		module = func;
	}
	const char* actoinName = GetActionName(module);
	if (actoinName != NULL){
		//root[GAME_ANALYTICS_PROPERTIES_GAME_MODULE] = module;
		//root[GAME_ANALYTICS_PROPERTIES_GAME_ACTION] = actoinName;
		root[actoinName] = func;
	}
	InsertString(root, GAME_ANALYTICS_PROPERTIES_EXTRA_PARAM, extra);
	FillCustomArgs(root, args);
}

void SensorsAnalyticsCollectorEx::FillSensorsProfileHeader(Json::Value& root, const char* roleid, const char* type)
{
	Assert(NULL != roleid);
	Assert(NULL != type);

	root[SENSORS_INNER_DISTINCT_ID] = roleid;
	root[SENSORS_INNER_TYPE] = type;
	root[SENSORS_INNER_TIME] = util_get_utc_ms_time();

	// ��������Ҫ����Ϸ�����Ϣ
	Json::Value& properties = root[SENSORS_INNER_PROPERTYIES];
	properties[GAME_ANALYTICS_PROPERTIES_DEPLOY_ID] = m_nDeployId;
	properties[GAME_ANALYTICS_PROPERTIES_GAME_ID] = m_nGameId;
	properties[GAME_ANALYTICS_PROPERTIES_SERVER_ID] = m_nSrvId;
	properties[GAME_ANALYTICS_PROPERTIES_DISTRICT_ID] = m_nDistrictId;
	properties[GAME_ANALYTICS_PROPERTIES_CHANNEL] = m_nChannelId;
	properties[GAME_ANALYTICS_PROPERTIES_ROLE_ID] = roleid;
}


bool SensorsAnalyticsCollectorEx::InitAction()
{
	// ������Դ
	LogActionType lgFunction;
	lgFunction.m_actionName = "function_id";
	lgFunction.m_actionMin = FUNCTION_EVENT_ID_SYS - 1;
	lgFunction.m_actionMax = FUNCTION_EVENT_ID_END;
	m_action.push_back(lgFunction);

	// ��������
// 	LogActionType lgTaskType;
// 	lgTaskType.m_actionName = "task_log_type";
// 	lgTaskType.m_actionMin = LOG_TASK_STATE_MIN;
// 	lgTaskType.m_actionMax = LOG_TASK_STATE_MAX;
// 	m_action.push_back(lgTaskType);
// 
// 	// ��Ϸ����
// 	LogActionType lgGameType;
// 	lgGameType.m_actionName = "game_func_log_type";
// 	lgGameType.m_actionMin = LOG_GAME_ACTION_MIN;
// 	lgGameType.m_actionMax = LOG_GAME_ACTION_MAX;
// 	m_action.push_back(lgGameType);

	// �������
	LogActionType lgUpgradeType;
	lgUpgradeType.m_actionName = "role_upgrade_type";
	lgUpgradeType.m_actionMin = LOG_ROLE_UPGRADE_MIN;
	lgUpgradeType.m_actionMax = LOG_ROLE_UPGRADE_MAX;
	m_action.push_back(lgUpgradeType);

// 	// �ͷ���־����
// 	LogActionType lgPunishType;
// 	lgPunishType.m_actionName = "punish_log_type";
// 	lgPunishType.m_actionMin = LOG_PUNISH_MIN;
// 	lgPunishType.m_actionMax = LOG_PUNISH_MAX;
// 	m_action.push_back(lgPunishType);
// 
// 	// �����־
// 	LogActionType lgPlayerType;
// 	lgPlayerType.m_actionName = "player_log_type";
// 	lgPlayerType.m_actionMin = LOG_PLAYER_MIN;
// 	lgPlayerType.m_actionMax = LOG_PLAYER_MAX;
// 	m_action.push_back(lgPlayerType);
// 
// 	// ������־
// 	LogActionType lgFridType;
// 	lgFridType.m_actionName = "friend_log_type";
// 	lgFridType.m_actionMin = LOG_FRIEND_MIN;
// 	lgFridType.m_actionMax = LOG_FRIEND_MAX;
// 	m_action.push_back(lgFridType);
// 
// 	// װ����¼
// 	LogActionType lgEquipType;
// 	lgEquipType.m_actionName = "equip_log_type";
// 	lgEquipType.m_actionMin = LOG_EQUIP_MIN;
// 	lgEquipType.m_actionMax = LOG_EQUIP_MAX;
// 	m_action.push_back(lgEquipType);
// 
// 	// ������־����
// 	LogActionType lgGuildType;
// 	lgGuildType.m_actionName = "guild_log_type";
// 	lgGuildType.m_actionMin = LOG_GUILD_MIN;
// 	lgGuildType.m_actionMax = LOG_GUILD_MAX;
// 	m_action.push_back(lgGuildType);
// 
// 	// ��Ϸ�淨
// 	LogActionType lgGameLogType;
// 	lgGameLogType.m_actionName = "game_log_state";
// 	lgGameLogType.m_actionMin = LOG_GAME_ACTION_STATE_MIN;
// 	lgGameLogType.m_actionMax = LOG_GAME_ACTION_STATE_MAX;
// 	m_action.push_back(lgGameLogType);
// 
// 	// ������־
// 	LogActionType lgRideType;
// 	lgRideType.m_actionName = "pet_ride_log_type";
// 	lgRideType.m_actionMin = LOG_PET_RIDE_MIN;
// 	lgRideType.m_actionMax = LOG_PET_RIDE_MAX;
// 	m_action.push_back(lgRideType);
// 
// 	// ����ϵͳ��־
// 	LogActionType lgNationType;
// 	lgNationType.m_actionName = "nation_log_type";
// 	lgNationType.m_actionMin = LOG_NATION_MIN;
// 	lgNationType.m_actionMax = LOG_NATION_MAX;
// 	m_action.push_back(lgNationType);
// 
// 	// �ƺ���־
// 	LogActionType lgTitleType;
// 	lgTitleType.m_actionName = "title_log_type";
// 	lgTitleType.m_actionMin = LOG_TITLE_MIN;
// 	lgTitleType.m_actionMax = LOG_TITLE_MAX;
// 	m_action.push_back(lgTitleType);
// 
// 	// ����ϵͳ��־
// 	LogActionType lgMeridianType;
// 	lgMeridianType.m_actionName = "meridian_log_type";
// 	lgMeridianType.m_actionMin = LOG_MERIDIAN_LEVEL_UP;
// 	lgMeridianType.m_actionMax = LOG_MERIDIAN_MAX;
// 	m_action.push_back(lgMeridianType);
	return true;
}

void SensorsAnalyticsCollectorEx::OnServerStart()
{
	// д��һ�����б�־��Ϸ��ʼ
	//ANALYTICS_LOG(m_anaLogger, "{}\r\n");
}

void SensorsAnalyticsCollectorEx::OnServerClose()
{

}

void SensorsAnalyticsCollectorEx::OnClientInfo(
									const char* roleid,			// ��ɫid(require)
									bool bfirst,				// �Ƿ����״�
									const char* clientver,
									const char* ip,				// �ն�id(option)
									const char* platform,		// ע��ƽ̨(option)
									bool wifi,					// �Ƿ�ʹ��wifi(option)
									const char* manufacturer,	// ������(option)
									const char* devid,			// �ն��豸Ψһ��ʶ(option)
									const char* model,			// �ն�����(option)
									const char* os,				// �ն�ϵͳ(option)
									const char* os_ver,			// �ն�ϵͳ�汾(option)
									const KVArgs * args)
								
{
	// profile
	Json::Value profiledata;
	FillSensorsProfileHeader(profiledata, roleid, SENSORS_EVENT_TYPE_PROFILE_SET);
	Json::Value& profileset = profiledata[SENSORS_INNER_PROPERTYIES];
	if (bfirst)
	{
		InsertString(profileset, GAME_ANALYTICS_PROPERTIES_PLATFORM, platform);
		InsertString(profileset, GAME_ANALYTICS_PROPERTIES_DEVICE_ID, devid);
		InsertString(profileset, GAME_ANALYTICS_PROPERTIES_MANUFACTURER, manufacturer);
		InsertString(profileset, GAME_ANALYTICS_PROPERTIES_MODEL, model);
		InsertString(profileset, GAME_ANALYTICS_PROPERTIES_OS, os);
		InsertString(profileset, GAME_ANALYTICS_PROPERTIES_OS_VER, os_ver);
		InsertString(profileset, GAME_ANALYTICS_PROPERTIES_CLIENT_VER, clientver);
		profileset[GAME_ANALYTICS_PROPERTIES_WIFI] = wifi;
		InsertString(profileset, GAME_ANALYTICS_PROPERTIES_REGISTER_IP, ip);
	}
	InsertString(profileset, GAME_ANALYTICS_PROPERTIES_LOGIN_IP, ip);
	WriteAnalyticsLog(profiledata);

	Json::Value trackdata;
	FillSensorsTrackHeader(trackdata, SENSORS_EVENT_TYPE_TRACK, GAME_ANALYTICS_EVENT_SET_CLIENT, roleid, 0);

	// �������
	Json::Value& properties = trackdata[SENSORS_INNER_PROPERTYIES];
	InsertString(properties, GAME_ANALYTICS_PROPERTIES_PLATFORM, platform);
	InsertString(properties, GAME_ANALYTICS_PROPERTIES_DEVICE_ID, devid);
	InsertString(properties, SENSORS_PROPERTY_IP, ip);
	properties[SENSORS_PROPERTY_WIFI] = wifi;
	properties[GAME_ANALYTICS_PROPERTIES_IS_REGISTER] = bfirst;
	InsertString(properties, SENSORS_PROPERTY_MANUFACTURER, manufacturer);
	InsertString(properties, SENSORS_PROPERTY_MODEL, model);
	InsertString(properties, SENSORS_PROPERTY_OS, os);
	InsertString(properties, SENSORS_PROPERTY_OS_VER, os_ver);
	InsertString(properties, GAME_ANALYTICS_PROPERTIES_CLIENT_VER, clientver);
	FillCustomArgs(properties, args);
	WriteAnalyticsLog( trackdata );

}


void SensorsAnalyticsCollectorEx::OnNewUser(const char* uid, 
											const char* anid,
											const char* nick, 
											int sex, 
											__int64 bron )
{
	Assert(false);
}

void SensorsAnalyticsCollectorEx::OnDelUser(const char* uid)
{
	Assert(false);
}

void SensorsAnalyticsCollectorEx::OnUserLogin(const char* uid,			// �û�Ψһid (require)
													const char* nick,		// �ǳ�/�û���(require)
													const char* ip,			// �ն�ip(option)
													const char* platform,	// ƽ̨(option)
													const char* manufacturer,	// ������(option)
													const char* devid,		// �ն��豸Ψһ��ʶ(option)
													const char* model,		// �ն�����(option)
													const char* os,			// �ն�ϵͳ(option)
													const char* os_ver,		// �ն�ϵͳ�汾(option)
													bool bReset/* = false*/)
{
	Json::Value trackdata;
	FillSensorsTrackHeader(trackdata, SENSORS_EVENT_TYPE_TRACK, GAME_ANALYTICS_EVENT_SIGNIN, uid, 0);
	trackdata.removeMember(GAME_ANALYTICS_PROPERTIES_ROLE_ID);
	trackdata[SENSORS_INNER_DISTINCT_ID] = uid;
	// �������
	Json::Value& properties = trackdata[SENSORS_INNER_PROPERTYIES];
	InsertString(properties, GAME_ANALYTICS_PROPERTIES_LOGIN_IP, ip);
	InsertString(properties, GAME_ANALYTICS_PROPERTIES_PLATFORM, platform);
	InsertString(properties, GAME_ANALYTICS_PROPERTIES_DEVICE_ID, devid);
	InsertString(properties, GAME_ANALYTICS_PROPERTIES_OS, os);
	InsertString(properties, GAME_ANALYTICS_PROPERTIES_OS_VER, os_ver);

	WriteAnalyticsLog(trackdata);
}

void SensorsAnalyticsCollectorEx::OnUserLogout(const char* uid, int64_t nTimestamp/* = 0*/, bool bReset/* = false*/)
{
	Json::Value trackdata;
	FillSensorsTrackHeader(trackdata, SENSORS_EVENT_TYPE_TRACK, GAME_ANALYTICS_EVENT_SIGNOUT, uid, 0);
	trackdata.removeMember( GAME_ANALYTICS_PROPERTIES_ROLE_ID );
	// �������
	trackdata[SENSORS_INNER_DISTINCT_ID] = uid;
	if (nTimestamp > 0)
	{
		trackdata[SENSORS_INNER_TIME] = nTimestamp;
	}

	
	WriteAnalyticsLog(trackdata);
}

void SensorsAnalyticsCollectorEx::OnNewRole(const char* roleid,
	const char* uid,
	const char* roleName,
	int job,
	int sex,
	int flag,
	const char* ip,
	const KVArgs* args
	)
{
	// profile
	{
		Json::Value profiledata;
		FillSensorsProfileHeader(profiledata, roleid, SENSORS_EVENT_TYPE_PROFILE_SET);
		Json::Value& profileset = profiledata[SENSORS_INNER_PROPERTYIES];
		profileset[GAME_ANALYTICS_PROPERTIES_SEX] = sex;
		profileset[GAME_ANALYTICS_PROPERTIES_ROLE_JOB] = job;
		profileset[GAME_ANALYTICS_PROPERTIES_ROLE_FLAG] = flag;
		InsertString(profileset, GAME_ANALYTICS_PROPERTIES_ROLE_NAME, roleName);
		InsertString(profileset, GAME_ANALYTICS_PROPERTIES_UID, uid);
		InsertString(profileset, GAME_ANALYTICS_PROPERTIES_GAME_VER, m_strGameVer.c_str());
		InsertString(profileset, GAME_ANALYTICS_PROPERTIES_GAME_NAME, m_strGameName.c_str());
		InsertString(profileset, GAME_ANALYTICS_PROPERTIES_REGISTER_IP, ip);
		profileset[GAME_ANALYTICS_PROPERTIES_GAME_ID] = m_nGameId;
		profileset[GAME_ANALYTICS_PROPERTIES_SERVER_ID] = m_nSrvId;
		profileset[GAME_ANALYTICS_PROPERTIES_DISTRICT_ID] = m_nDistrictId;
		profileset[GAME_ANALYTICS_PROPERTIES_CHANNEL] = m_nChannelId;
		profileset[GAME_ANALYTICS_PROPERTIES_PRODUCTION_ID] = m_nProductionId;
		// ������
		std::string date;
		util_convert_date_to_string2(date);
		profileset[GAME_ANALYTICS_PROPERTIES_CTIME] = date;
		FillSensorsProfileArgs(profileset, job, args);
		WriteAnalyticsLog(profiledata);
	}

	{
		Json::Value trackdata;
		FillSensorsTrackHeader(trackdata, SENSORS_EVENT_TYPE_TRACK, GAME_ANALYTICS_EVENT_NEW_ROLE, roleid, 1);
		// �������
		Json::Value& properties = trackdata[SENSORS_INNER_PROPERTYIES];
		InsertString(properties, GAME_ANALYTICS_PROPERTIES_UID, uid);
		InsertString(properties, GAME_ANALYTICS_PROPERTIES_ROLE_NAME, roleName);
		properties[GAME_ANALYTICS_PROPERTIES_ROLE_JOB] = job;
		properties[GAME_ANALYTICS_PROPERTIES_SEX] = sex;
		properties[GAME_ANALYTICS_PROPERTIES_ROLE_FLAG] = flag;
		InsertString(properties, GAME_ANALYTICS_PROPERTIES_REGISTER_IP, ip);
		FillCustomArgs(properties, args);
		WriteAnalyticsLog(trackdata);
	}

	// ��¼�û��½���ɫ
	{
		Json::Value trackdata;
		FillSensorsTrackHeader(trackdata, SENSORS_EVENT_TYPE_TRACK, GAME_ANALYTICS_EVENT_U_NEW_ROLE, roleid, 1);
		// �������
		trackdata[SENSORS_INNER_DISTINCT_ID] = uid;
		Json::Value& properties = trackdata[SENSORS_INNER_PROPERTYIES];
		InsertString(properties, GAME_ANALYTICS_PROPERTIES_UID, uid);
		InsertString(properties, GAME_ANALYTICS_PROPERTIES_ROLE_NAME, roleName);
		InsertString(properties, GAME_ANALYTICS_PROPERTIES_REGISTER_IP, ip);
		properties[GAME_ANALYTICS_PROPERTIES_ROLE_JOB] = job;
		properties[GAME_ANALYTICS_PROPERTIES_SEX] = sex;
		properties[GAME_ANALYTICS_PROPERTIES_ROLE_FLAG] = flag;
		WriteAnalyticsLog(trackdata);
	}
}

void SensorsAnalyticsCollectorEx::OnDelRole(const char* roleid, int level, const char* uid)
{
	Json::Value profiledata;
	FillSensorsProfileHeader(profiledata, roleid, SENSORS_EVENT_TYPE_PROFILE_DELETE);
	WriteAnalyticsLog(profiledata);	
	
	Json::Value trackdata;
	FillSensorsTrackHeader(trackdata, SENSORS_EVENT_TYPE_TRACK, GAME_ANALYTICS_EVENT_DEL_ROLE, roleid, level);
	Json::Value& properties = trackdata[SENSORS_INNER_PROPERTYIES];
	InsertString(properties, GAME_ANALYTICS_PROPERTIES_UID, uid);
	WriteAnalyticsLog( trackdata );

}

void SensorsAnalyticsCollectorEx::OnRoleEntry(const char* roleid,		// ��ɫid
												const char* uid,			// ��ɫ����uid(require)
												int level,					// ��ɫ�ȼ�
												int vip,					// ��ɫvip
												int64_t gold,				// ��ǰ��ӵ�еĽ��
												const char* ip,				// ip
												const KVArgs* args,			// �����б�(��������ֵ��)
												bool bReset/* = false*/)
{
	// profile
	{
		Json::Value profiledata;
		FillSensorsProfileHeader(profiledata, roleid, SENSORS_EVENT_TYPE_PROFILE_SET);
		Json::Value& profileset = profiledata[SENSORS_INNER_PROPERTYIES];
		InsertString(profileset, GAME_ANALYTICS_PROPERTIES_UID, uid);
		profileset[GAME_ANALYTICS_PROPERTIES_SILVER] = gold;
		profileset[GAME_ANALYTICS_PROPERTIES_VIP] = vip;
		profileset[GAME_ANALYTICS_PROPERTIES_LEVEL] = level;
		// ��½ʱ��
		std::string date;
		util_convert_date_to_string2(date);
		profileset[GAME_ANALYTICS_PROPERTIES_LTIME] = date;
		profileset[GAME_ANALYTICS_PROPERTIES_ONLINE] = true;
		if ( NULL != ip && ip[0] != '\0' )
		{
			profileset[GAME_ANALYTICS_PROPERTIES_LOGIN_IP] = ip;
		}
		WriteAnalyticsLog(profiledata);
	}

	{
		Json::Value trackdata;
		FillSensorsTrackHeader(trackdata, SENSORS_EVENT_TYPE_TRACK, GAME_ANALYTICS_EVENT_ROLE_ENTRY, roleid, level);
		// �������
		Json::Value& properties = trackdata[SENSORS_INNER_PROPERTYIES];
		InsertString(properties, GAME_ANALYTICS_PROPERTIES_UID, uid);
		InsertString(properties, GAME_ANALYTICS_PROPERTIES_LOGIN_IP, ip);
		properties[GAME_ANALYTICS_PROPERTIES_VIP] = vip;
		properties[GAME_ANALYTICS_PROPERTIES_SILVER] = gold;
		FillCustomArgs(properties, args);
		WriteAnalyticsLog(trackdata);
	}

	{
		Json::Value trackdata;
		FillSensorsTrackHeader(trackdata, SENSORS_EVENT_TYPE_TRACK, GAME_ANALYTICS_EVENT_U_ROLE_ENTRY, roleid, level);
		// �������
		trackdata[SENSORS_INNER_DISTINCT_ID] = uid;
		Json::Value& properties = trackdata[SENSORS_INNER_PROPERTYIES];
		InsertString(properties, GAME_ANALYTICS_PROPERTIES_LOGIN_IP, ip);
		InsertString(properties, GAME_ANALYTICS_PROPERTIES_UID, uid);
		properties[GAME_ANALYTICS_PROPERTIES_VIP] = vip;
		properties[GAME_ANALYTICS_PROPERTIES_SILVER] = gold;
		FillCustomArgs(properties, args);
		WriteAnalyticsLog(trackdata);
	}
}

void SensorsAnalyticsCollectorEx::OnRoleExit(const char* roleid, 		// ��ɫid
											const char* uid,			// ��ɫ����uid(require)
											int level,					// ��ɫ�ȼ�
											int vip,					// ��ɫvip
											int64_t gold,				// ��ǰ��ӵ�еĽ��
											int64_t onlinetime,			// ����ʱ��
											int64_t totaltime,			// ������ʱ��
											const char* ip,				// ip
											const KVArgs* args,			// �����б�(��������ֵ��)
											bool bReset/* = false*/)
{
	// ?? ����ʱ���ֶα�ʾ��˼��ͻ(profile��track)
	Json::Value profiledata;
	FillSensorsProfileHeader(profiledata, roleid, SENSORS_EVENT_TYPE_PROFILE_SET);
	Json::Value& profileset = profiledata[SENSORS_INNER_PROPERTYIES];
	InsertString(profileset, GAME_ANALYTICS_PROPERTIES_UID, uid);
	profileset[GAME_ANALYTICS_PROPERTIES_SILVER] = gold;
	profileset[GAME_ANALYTICS_PROPERTIES_VIP] = vip;
	profileset[GAME_ANALYTICS_PROPERTIES_LEVEL] = level;
	profileset[GAME_ANALYTICS_PROPERTIES_ONLINETIME] = totaltime;
	// �˳�ʱ��
	std::string date;
	util_convert_date_to_string2(date);
	profileset[GAME_ANALYTICS_PROPERTIES_QTIME] = date;
	profileset[GAME_ANALYTICS_PROPERTIES_ONLINE] = false;
	if (NULL != ip && ip[0] != '\0')
	{
		profileset[GAME_ANALYTICS_PROPERTIES_LOGIN_IP] = ip;
	}
	WriteAnalyticsLog(profiledata);

	Json::Value trackdata;
	FillSensorsTrackHeader(trackdata, SENSORS_EVENT_TYPE_TRACK, GAME_ANALYTICS_EVENT_ROLE_EXIT, roleid, level);
	// �������
	Json::Value& properties = trackdata[SENSORS_INNER_PROPERTYIES];
	InsertString(properties, GAME_ANALYTICS_PROPERTIES_UID, uid);
	InsertString(properties, GAME_ANALYTICS_PROPERTIES_LOGIN_IP, ip);
	properties[GAME_ANALYTICS_PROPERTIES_VIP] = vip;
	properties[GAME_ANALYTICS_PROPERTIES_SILVER] = gold;
	properties[GAME_ANALYTICS_PROPERTIES_LEVEL] = level;
	properties[GAME_ANALYTICS_PROPERTIES_ONLINETIME] = onlinetime;
	FillCustomArgs(properties, args);
	WriteAnalyticsLog( trackdata );

	if (!bReset)
	{
		auto it = m_strRoleCache.find(uid);
		if (it != m_strRoleCache.end())
		{
			m_strRoleCache.erase(it);
		}
	}
}


void SensorsAnalyticsCollectorEx::OnPaying(
									const char* roleid,			// ��ɫid(require)
									const char* uid,			// ��ɫ����uid(require)
									int level,					// ��ɫ�ȼ�
									int vip,					// ��ɫvip�ȼ�
									const char* orderid, 
									const char* itemid, 
									int count, 
									float price,				// ��Ʒ�۸�(require)
									bool first,					// �Ƿ��׳�
									const char* platform		// �ͻ���os(android, ios, pc...)
									)
{
	Json::Value trackdata;
	FillSensorsTrackHeader(trackdata, SENSORS_EVENT_TYPE_TRACK, GAME_ANALYTICS_EVENT_PAY_START, roleid, level);
	// �������
	Json::Value& properties = trackdata[SENSORS_INNER_PROPERTYIES];
	InsertString(properties, GAME_ANALYTICS_PROPERTIES_UID, uid);
	InsertString(properties, GAME_ANALYTICS_PROPERTIES_PAY_ORDER_ID, orderid);
	InsertString(properties, GAME_ANALYTICS_PROPERTIES_PAY_ITEM_ID, itemid);
	properties[GAME_ANALYTICS_PROPERTIES_PAY_ITEM_COUNT] = count;
	properties[GAME_ANALYTICS_PROPERTIES_PAY_ITEM_PRICE] = price;
	properties[GAME_ANALYTICS_PROPERTIES_VIP] = vip;
	properties[GAME_ANALYTICS_PROPERTIES_PAY_FIRST] = first;
	InsertString(properties, GAME_ANALYTICS_PROPERTIES_PLATFORM, platform);

	WriteAnalyticsLog( trackdata );
}

void SensorsAnalyticsCollectorEx::OnPayCompleted(
												const char* roleid,				// ��ɫid(require)
												const char* uid,				// ��ɫ����uid(require)
												int level,						// ��ɫ�ȼ�
												int vip,						// ��ɫvip�ȼ�
												const char* orderid,			// ����id(require)
												float amount,					// ʵ��֧�����(require)
												float totalamount,				// ��֧�����
												float realamount,				// ��ʵ֧�����
												int paytimes,					// �ܳ�ֵ����
												int result,						// ֧�����(require) 0:�ɹ� 1:ʧ�� 2:ȡ��
												bool first,						// �Ƿ��׳�
												bool bIsReal,					// �Ƿ�����ҳ�ֵ
												const char* platform,			// ֧��ƽ̨(require)(xy, xiaomi,baidu....)
												const char* os					// �ͻ���os
											)
{
	// �ۼ�֧�����
	Json::Value paydata;
	FillSensorsProfileHeader(paydata, roleid, SENSORS_EVENT_TYPE_PROFILE_SET);
	Json::Value& payset = paydata[SENSORS_INNER_PROPERTYIES];
	payset[GAME_ANALYTICS_PROPERTIES_PAY_TOTAL_AMOUNT] = totalamount;
	payset[GAME_ANALYTICS_PROPERTIES_PAY_REAL_AMOUNT] = realamount;
	payset[GAME_ANALYTICS_PROPERTIES_PAY_VIRTUAL_AMOUNT] = totalamount-realamount;
	payset[GAME_ANALYTICS_PROPERTIES_PAY_TIMES] = paytimes;
	payset[GAME_ANALYTICS_PROPERTIES_PAY_LAST_TIME] = util_get_utc_time();
	WriteAnalyticsLog(paydata);	
	
	Json::Value trackdata;
	FillSensorsTrackHeader(trackdata, SENSORS_EVENT_TYPE_TRACK, GAME_ANALYTICS_EVENT_PAY_END, roleid, level);
	// �������
	Json::Value& properties = trackdata[SENSORS_INNER_PROPERTYIES];
	InsertString(properties, GAME_ANALYTICS_PROPERTIES_UID, uid);
	InsertString(properties, GAME_ANALYTICS_PROPERTIES_PAY_ORDER_ID, orderid);
	properties[GAME_ANALYTICS_PROPERTIES_PAY_RESULT] = result;
	properties[GAME_ANALYTICS_PROPERTIES_VIP] = vip;
	properties[GAME_ANALYTICS_PROPERTIES_PAY_FIRST] = first;
	if (bIsReal)
	{
		properties[GAME_ANALYTICS_PROPERTIES_PAY_AMOUNT_1] = 0;
		properties[GAME_ANALYTICS_PROPERTIES_PAY_AMOUNT] = amount;
	}
	else
	{
		properties[GAME_ANALYTICS_PROPERTIES_PAY_AMOUNT_1] = amount;
		properties[GAME_ANALYTICS_PROPERTIES_PAY_AMOUNT] = 0;
	}
	properties[GAME_ANALYTICS_PROPERTIES_PAY_TIMES] = paytimes;
	InsertString(properties, GAME_ANALYTICS_PROPERTIES_PAY_PLATFORM, platform);
	InsertString(properties, GAME_ANALYTICS_PROPERTIES_OS, os);
	WriteAnalyticsLog( trackdata );
}

void SensorsAnalyticsCollectorEx::OnIncExp(
										const char* roleid,		// ��ɫid(require)
										int level,				// ��ɫ�ȼ�
										int value,					// ����ֵ(require)
										int newValue,				// ���Ӻ��ֵ
										int origValue,				// ԭʼֵ
										int source	,				// ��Դ
										const KVArgs * args
										)
{
	Json::Value trackdata;
	FillSensorsTrackHeader(trackdata, SENSORS_EVENT_TYPE_TRACK, GAME_ANALYTICS_EVENT_INC_EXP, roleid, level);
	// �������
	Json::Value& properties = trackdata[SENSORS_INNER_PROPERTYIES];
	properties[GAME_ANALYTICS_PROPERTIES_INC_EXP] = value;
	properties[GAME_ANALYTICS_PROPERTIES_ORIG_EXP] = origValue;
	properties[GAME_ANALYTICS_PROPERTIES_EXP] = newValue;
	FillCustomArgs(properties, args);
	FillModuleInfo(properties, 0, source, NULL);
	WriteAnalyticsLog( trackdata );
}

void SensorsAnalyticsCollectorEx::OnLevelup(const char* roleid, int level, int original, const KVArgs * args)
{
	Json::Value profiledata;
	FillSensorsProfileHeader(profiledata, roleid, SENSORS_EVENT_TYPE_PROFILE_SET);
	Json::Value& profileset = profiledata[SENSORS_INNER_PROPERTYIES];
	profileset[GAME_ANALYTICS_PROPERTIES_LEVEL] = level;
	WriteAnalyticsLog(profiledata);	
	
	Json::Value trackdata;
	FillSensorsTrackHeader(trackdata, SENSORS_EVENT_TYPE_TRACK, GAME_ANALYTICS_EVENT_LEVELUP, roleid, level);
	// �������
	Json::Value& properties = trackdata[SENSORS_INNER_PROPERTYIES];
	properties[GAME_ANALYTICS_PROPERTIES_PRE_LEVEL] = original;
	FillCustomArgs(properties, args);
	WriteAnalyticsLog( trackdata );
}

// �����ȼ�����
void SensorsAnalyticsCollectorEx::OnMeridianLevelup(const char* roleid, int level, int vip, int value, int original,const KVArgs * args)
{
	Json::Value profiledata;
	FillSensorsProfileHeader(profiledata, roleid, SENSORS_EVENT_TYPE_PROFILE_SET);
	Json::Value& profileset = profiledata[SENSORS_INNER_PROPERTYIES];
	profileset[GAME_ANALYTICS_PROPERTIES_MERIDIAN_LEVEL] = value;
	WriteAnalyticsLog(profiledata);

	Json::Value trackdata;
	FillSensorsTrackHeader(trackdata, SENSORS_EVENT_TYPE_TRACK, GAME_ANALYTICS_EVENT_MERIDIAN_UP, roleid, level);
	// �������
	Json::Value& properties = trackdata[SENSORS_INNER_PROPERTYIES];
	properties[GAME_ANALYTICS_PROPERTIES_MERIDIAN_LEVEL] = value;
	properties[GAME_ANALYTICS_PROPERTIES_MERIDIAN_PRE_LEVEL] = original;
	FillCustomArgs(properties, args);
	WriteAnalyticsLog(trackdata);
}

void SensorsAnalyticsCollectorEx::OnGainCoin(
												const char* roleid,			// ��ɫid(require)
												int level,					// ��ɫ��ǰ�ȼ�(require)
												int vip,					// vip�ȼ�(require)
												int coid,
												int64_t value,
												int64_t newvalue,
												int64_t original,
												int module, 
												int func, 
												const char* extra,
												const KVArgs * args)
{
	Json::Value profiledata;
	FillSensorsProfileHeader(profiledata, roleid, SENSORS_EVENT_TYPE_PROFILE_SET);
	Json::Value& profileset = profiledata[SENSORS_INNER_PROPERTYIES];
	// profile
	switch (coid)
	{
	case CAPITAL_GOLD:
		profileset[GAME_ANALYTICS_PROPERTIES_SILVER]=  newvalue;
		break;
	case  CAPITAL_COPPER:
		profileset[GAME_ANALYTICS_PROPERTIES_COPPER] = newvalue;
		break;
	case  CAPITAL_BIND_GOLD:
		// TODO
		break;
	case  CAPITAL_TSILVER:
		// TODO
		break;
	case  CAPITAL_SMELT:
		// TODO
		break;
	default:
		Assert(false);
	}
	WriteAnalyticsLog(profiledata);

	Json::Value trackdata;
	FillSensorsTrackHeader(trackdata, SENSORS_EVENT_TYPE_TRACK, GAME_ANALYTICS_EVENT_INC_COIN, roleid, level);
	// �������
	Json::Value& properties = trackdata[SENSORS_INNER_PROPERTYIES];
	properties[GAME_ANALYTICS_PROPERTIES_VIP] = vip;
	properties[GAME_ANALYTICS_PROPERTIES_COIN_ID] = coid;
	properties[GAME_ANALYTICS_PROPERTIES_COIN_VALUE] = value;
	properties[GAME_ANALYTICS_PROPERTIES_COIN_NEW_VALUE] = newvalue;
	properties[GAME_ANALYTICS_PROPERTIES_COIN_ORIGINAL] = original;
	FillModuleInfo(properties, module, func, extra);
	FillCustomArgs(properties, args);
	WriteAnalyticsLog(trackdata);
}

void SensorsAnalyticsCollectorEx::OnCostCoin(
											const char* roleid,			// ��ɫid(require)
											int level,				// ��ɫ��ǰ�ȼ�(require)
											int vip,				// vip�ȼ�(require)
											int coid, 
											int64_t value,
											int64_t newvalue,
											int64_t original,
											int module, 
											int func, 
											const char* extra,
											const KVArgs * args)
{
	Json::Value profiledata;
	FillSensorsProfileHeader(profiledata, roleid, SENSORS_EVENT_TYPE_PROFILE_SET);
	Json::Value& profileset = profiledata[SENSORS_INNER_PROPERTYIES];
	// profile
	switch (coid)
	{
	case CAPITAL_GOLD:
		profileset[GAME_ANALYTICS_PROPERTIES_SILVER] = newvalue;
		break;
	case  CAPITAL_COPPER:
		profileset[GAME_ANALYTICS_PROPERTIES_COPPER] = newvalue;
		break;
	case CAPITAL_TSILVER:
		// TODO
		break;
	case CAPITAL_BIND_GOLD:
		// TODO
		break;
	case CAPITAL_SMELT:
		// TODO
		break;
// 	case CAPITAL_BATTLE_SOUL:
// 		profileset[GAME_ANALYTICS_PROPERTIES_SOUL] = newvalue;
// 		break;
// 	case CAPITAL_SMELT:
// 		profileset[GAME_ANALYTICS_PROPERTIES_SMELT] = newvalue;
// 		break;
// 	case  CAPITAL_EXPLOIT:
// 		profileset[GAME_ANALYTICS_PROPERTIES_EXPLOIT] = newvalue;
// 		break;
// 	case  CAPITAL_HONOR:
// 		profileset[GAME_ANALYTICS_PROPERTIES_HONOR] = newvalue;
// 		break;
// 	case  CAPITAL_CHIVALRY:
// 		profileset[GAME_ANALYTICS_PROPERTIES_CHIVALRY] = newvalue;
// 		break;
// 	case  CAPITAL_TRADE_SILVER:
// 		profileset[GAME_ANALYTICS_PROPERTIES_TRADE_SILVER] = newvalue;
// 		break;
// 	case  CAPITAL_CAMPVALUE:
// 		profileset[GAME_ANALYTICS_PROPERTIES_CAMPVALUE] = newvalue;
// 		break;
// 	case  CAPITAL_WISH_SCORE:
// 		profileset[GAME_ANALYTICS_PROPERTIES_WISHSCORE] = newvalue;
// 		break;
// 	case CAPITAL_GUILD_CURRENCY:
// 		profileset[GAME_ANALYTICS_PROPERTIES_GUILD_CURRENCY] = newvalue;
// 		break;
	default:
		Assert(false);
	}
	WriteAnalyticsLog(profiledata);

	Json::Value trackdata;
	FillSensorsTrackHeader(trackdata, SENSORS_EVENT_TYPE_TRACK, GAME_ANALYTICS_EVENT_DEC_COIN, roleid, level);
	// �������
	Json::Value& properties = trackdata[SENSORS_INNER_PROPERTYIES];
	properties[GAME_ANALYTICS_PROPERTIES_VIP] = vip;
	properties[GAME_ANALYTICS_PROPERTIES_COIN_ID] = coid;
	properties[GAME_ANALYTICS_PROPERTIES_COIN_VALUE] = value;
	properties[GAME_ANALYTICS_PROPERTIES_COIN_NEW_VALUE] = newvalue;
	properties[GAME_ANALYTICS_PROPERTIES_COIN_ORIGINAL] = original;

	FillModuleInfo(properties, module, func, extra);
	FillCustomArgs(properties, args);
	WriteAnalyticsLog(trackdata);

}

void SensorsAnalyticsCollectorEx::OnGainItem(
												const char* roleid,			// ��ɫid(require)
												int level,				// ��ɫ��ǰ�ȼ�(require)
												int vip,				// vip�ȼ�(require)
												const char* itemid, 
												int num,				// �����ֵ(require)
												int count,				// ��ǰӵ����
												int orig,				// ԭ��ӵ����
												int module, 
												int func, 
												const char* extra,
												const KVArgs * args)
{
	Json::Value trackdata;
	FillSensorsTrackHeader(trackdata, SENSORS_EVENT_TYPE_TRACK, GAME_ANALYTICS_EVENT_GAIN_ITEM, roleid, level);
	// �������
	Json::Value& properties = trackdata[SENSORS_INNER_PROPERTYIES];
	properties[GAME_ANALYTICS_PROPERTIES_VIP] = vip;
	InsertString(properties, GAME_ANALYTICS_PROPERTIES_ITEM_ID, itemid);
	properties[GAME_ANALYTICS_PROPERTIES_ITEM_COUNT] = count;
	properties[GAME_ANALYTICS_PROPERTIES_ITEM_NUM] = num;
	properties[GAME_ANALYTICS_PROPERTIES_ITEM_ORIG_COUNT] = orig;
	FillModuleInfo(properties, module, func, extra);
	FillCustomArgs(properties, args);
	WriteAnalyticsLog( trackdata );
}

void SensorsAnalyticsCollectorEx::OnCostItem(
												const char* roleid,			// ��ɫid(require)
												int level,				// ��ɫ��ǰ�ȼ�(require)
												int vip,				// vip�ȼ�(require)
												const char* itemid, 
												int num,				// �����ֵ(require)
												int count,				// ��ǰӵ����
												int orig,				// ԭ��ӵ����
												int module,
												int func, 
												const char* extra,
												const KVArgs * args)
{
	Json::Value trackdata;
	FillSensorsTrackHeader(trackdata, SENSORS_EVENT_TYPE_TRACK, GAME_ANALYTICS_EVENT_COST_ITEM, roleid, level);
	// �������
	Json::Value& properties = trackdata[SENSORS_INNER_PROPERTYIES];
	properties[GAME_ANALYTICS_PROPERTIES_VIP] = vip;
	InsertString(properties, GAME_ANALYTICS_PROPERTIES_ITEM_ID, itemid);
	properties[GAME_ANALYTICS_PROPERTIES_ITEM_COUNT] = count;
	properties[GAME_ANALYTICS_PROPERTIES_ITEM_NUM] = num;
	properties[GAME_ANALYTICS_PROPERTIES_ITEM_ORIG_COUNT] = orig;
	FillCustomArgs(properties, args);
	FillModuleInfo(properties, module, func, extra);
	WriteAnalyticsLog( trackdata );
}

void SensorsAnalyticsCollectorEx::OnVipLevelup(
												const char* roleid,			// ��ɫid(require)
												int level,				// ��ɫ��ǰ�ȼ�(require)
												int value, 
												int original, 
												int module, 
												int func, 
												const char* extra,
												const KVArgs * args)
{
	// profile
	Json::Value profiledata;
	FillSensorsProfileHeader(profiledata, roleid, SENSORS_EVENT_TYPE_PROFILE_SET);
	Json::Value& profileset = profiledata[SENSORS_INNER_PROPERTYIES];
	profileset[GAME_ANALYTICS_PROPERTIES_VIP] = value;
	WriteAnalyticsLog(profiledata);

	Json::Value trackdata;
	FillSensorsTrackHeader(trackdata, SENSORS_EVENT_TYPE_TRACK, GAME_ANALYTICS_EVENT_VIPUP, roleid, level);
	// �������
	Json::Value& properties = trackdata[SENSORS_INNER_PROPERTYIES];
	properties[GAME_ANALYTICS_PROPERTIES_VIP] = value;
	properties[GAME_ANALYTICS_PROPERTIES_PRE_VIP] = original;

	FillModuleInfo(properties, module, func, extra);
	FillCustomArgs(properties, args);
	WriteAnalyticsLog(trackdata);
}

void SensorsAnalyticsCollectorEx::OnPlayerPropertyChanged(
											const char* roleid,			// ��ɫid(require)
											int level,				// ��ɫ��ǰ�ȼ�(require)
											int vip,				// vip�ȼ�(require)
											const char* propery, 
											float value, 
											float original, 
											int module, 
											int func, 
											const char* extra,
											const KVArgs * args)
{
	Json::Value trackdata;
	FillSensorsTrackHeader(trackdata, SENSORS_EVENT_TYPE_TRACK, GAME_ANALYTICS_EVENT_PROPERTY_CHANGED, roleid, level);
	// �������
	Json::Value& properties = trackdata[SENSORS_INNER_PROPERTYIES];
	properties[GAME_ANALYTICS_PROPERTIES_VIP] = vip;
	InsertString(properties, GAME_ANALYTICS_PROPERTIES_PLAYER_PROPERTY, propery);
	properties[GAME_ANALYTICS_PROPERTIES_PLAYER_PROPERTY_VALUE] = value;
	properties[GAME_ANALYTICS_PROPERTIES_PLAYER_PROPERTY_PRE_VALUE] = original;

	FillModuleInfo(properties, module, func, extra);
	FillCustomArgs(properties, args);
	WriteAnalyticsLog( trackdata );
}

void SensorsAnalyticsCollectorEx::OnPlayerTransaction(const char* transid, 
									int transtype, 
									const char* sender, 
									const char* reciver, 
									const char* senderItem, 
									int senderCount, 
									const char* reciverItem, 
									int reciverCount,
									const KVArgs * args)
{
	Json::Value trackdata;
	FillSensorsTrackHeader(trackdata, transid, SENSORS_EVENT_TYPE_TRACK, GAME_ANALYTICS_EVENT_TRANSZCTION);
	// �������
	Json::Value& properties = trackdata[SENSORS_INNER_PROPERTYIES];
	properties[GAME_ANALYTICS_PROPERTIES_TRANS_TYPE] = transtype;
	InsertString(properties, GAME_ANALYTICS_PROPERTIES_SENDER, sender);
	InsertString(properties, GAME_ANALYTICS_PROPERTIES_RECIVER, reciver);
	InsertString(properties, GAME_ANALYTICS_PROPERTIES_SENDER_ITEM, senderItem);
	InsertString(properties, GAME_ANALYTICS_PROPERTIES_RECIVER_ITEM, reciverItem);
	properties[GAME_ANALYTICS_PROPERTIES_SENDER_COUNT] = senderCount;
	properties[GAME_ANALYTICS_PROPERTIES_RECIVER_COUNT] = reciverCount;
	FillCustomArgs(properties, args);
	WriteAnalyticsLog( trackdata );

}

void SensorsAnalyticsCollectorEx::OnGuideEvent(
										const char* roleid,			// ��ɫid(require)
										int level,				// ��ɫ��ǰ�ȼ�(require)
										int vip,				// vip�ȼ�(require)
										int guideid, int step, const KVArgs * args)
{
	Json::Value trackdata;
	FillSensorsTrackHeader(trackdata, SENSORS_EVENT_TYPE_TRACK, GAME_ANALYTICS_EVENT_GUIDE, roleid, level);
	// �������
	Json::Value& properties = trackdata[SENSORS_INNER_PROPERTYIES];
	properties[GAME_ANALYTICS_PROPERTIES_VIP] = vip;
	properties[GAME_ANALYTICS_PROPERTIES_GUIDE_ID] = guideid;
	properties[GAME_ANALYTICS_PROPERTIES_GUIDE_STEP] = step;
	FillCustomArgs(properties, args);
	WriteAnalyticsLog( trackdata );
}

void SensorsAnalyticsCollectorEx::OnTaskStart(
												const char* roleid,		// ��ɫid(require)
												int level,				// ��ɫ��ǰ�ȼ�(require)
												int vip,				// vip�ȼ�(require)
												int taskid,				// ��ȡ������id(require)
												int type,				// ��������(option) ��Ϸ�ж���
												int opt_type,			// ���������ʽ(option) ��Ϸ�ж���
												const int state,		// ����״̬	
												const KVArgs * args)
{
	Json::Value trackdata;
	FillSensorsTrackHeader(trackdata, SENSORS_EVENT_TYPE_TRACK, GAME_ANALYTICS_EVENT_TASK_ACCEPT, roleid, level);
	// �������
	Json::Value& properties = trackdata[SENSORS_INNER_PROPERTYIES];
	properties[GAME_ANALYTICS_PROPERTIES_VIP] = vip;
	properties[GAME_ANALYTICS_PROPERTIES_TASK_ID] = taskid;
	properties[GAME_ANALYTICS_PROPERTIES_TASK_TYPE] = type;
	properties[GAME_ANALYTICS_PROPERTIES_TASK_OPT_WAY] = opt_type;
	properties[GAME_ANALYTICS_PROPERTIES_TASK_STATE] = state;
	FillCustomArgs(properties, args);
	WriteAnalyticsLog( trackdata );
}

void SensorsAnalyticsCollectorEx::OnTaskCompleted(
												const char* roleid,		// ��ɫid(require)
												int level,				// ��ɫ��ǰ�ȼ�(require)
												int vip,				// vip�ȼ�(require)
												int taskid,				// ��ȡ������id(require)
												int type,				// ��������(option) ��Ϸ�ж���
												int opt_type,			// ���������ʽ(option) ��Ϸ�ж���
												const int state,		// ����״̬	
												const KVArgs * args)
{
	Json::Value trackdata;
	FillSensorsTrackHeader(trackdata, SENSORS_EVENT_TYPE_TRACK, GAME_ANALYTICS_EVENT_TASK_COMPLETE, roleid, level);
	// �������
	Json::Value& properties = trackdata[SENSORS_INNER_PROPERTYIES];
	properties[GAME_ANALYTICS_PROPERTIES_VIP] = vip;
	properties[GAME_ANALYTICS_PROPERTIES_TASK_ID] = taskid;
	properties[GAME_ANALYTICS_PROPERTIES_TASK_TYPE] = type;
	properties[GAME_ANALYTICS_PROPERTIES_TASK_OPT_WAY] = opt_type;
	properties[GAME_ANALYTICS_PROPERTIES_TASK_STATE] = state;
	FillCustomArgs(properties, args);
	WriteAnalyticsLog( trackdata );
}

// ������
void SensorsAnalyticsCollectorEx::OnTaskSubmit(
	const char* roleid, /* ��ɫid(require) */
	int level, /* ��ɫ��ǰ�ȼ�(require) */
	int vip, /* vip�ȼ�(require) */
	int taskid, /* ��ȡ������id(require) */
	int type, /* ��������(option) ��Ϸ�ж��� */
	int opt_type, /* ���������ʽ(option) ��Ϸ�ж��� */
	const int state,		// ����״̬	
	const KVArgs * args
	)

{
	Json::Value trackdata;
	FillSensorsTrackHeader(trackdata, SENSORS_EVENT_TYPE_TRACK, GAME_ANALYTICS_EVENT_TASK_SUBMIT, roleid, level);
	// �������
	Json::Value& properties = trackdata[SENSORS_INNER_PROPERTYIES];
	properties[GAME_ANALYTICS_PROPERTIES_VIP] = vip;
	properties[GAME_ANALYTICS_PROPERTIES_TASK_ID] = taskid;
	properties[GAME_ANALYTICS_PROPERTIES_TASK_TYPE] = type;
	properties[GAME_ANALYTICS_PROPERTIES_TASK_OPT_WAY] = opt_type;
	properties[GAME_ANALYTICS_PROPERTIES_TASK_STATE] = state;
	FillCustomArgs(properties, args);
	WriteAnalyticsLog(trackdata);
}

void SensorsAnalyticsCollectorEx::OnSceneOpen(
												const char* roleid,		// ��ɫid(require)
												int level,				// ��ɫ��ǰ�ȼ�(require)
												int vip,				// vip�ȼ�(require)
												int sid, 
												int chapter, 
												int type ,
												const KVArgs * args)
{
	Json::Value trackdata;
	FillSensorsTrackHeader(trackdata, SENSORS_EVENT_TYPE_TRACK, GAME_ANALYTICS_EVENT_COPY_SCENE_OPEN, roleid, level);
	// �������
	Json::Value& properties = trackdata[SENSORS_INNER_PROPERTYIES];
	properties[GAME_ANALYTICS_PROPERTIES_VIP] = vip;
	properties[GAME_ANALYTICS_PROPERTIES_SCENE_ID] = sid;
	properties[GAME_ANALYTICS_PROPERTIES_SCENE_CHAPTER] = chapter;
	properties[GAME_ANALYTICS_PROPERTIES_SCENE_TYPE] = type;
	FillCustomArgs(properties, args);
	WriteAnalyticsLog( trackdata );
}

void SensorsAnalyticsCollectorEx::OnSceneEntry(
													const char* roleid,		// ��ɫid(require)
													int level,				// ��ɫ��ǰ�ȼ�(require)
													int vip,				// vip�ȼ�(require)
													int sid,				// ����id(require)
													int chapter, 
													int type,
													const KVArgs * args)
{
	Json::Value trackdata;
	FillSensorsTrackHeader(trackdata, SENSORS_EVENT_TYPE_TRACK, GAME_ANALYTICS_EVENT_COPY_SCENE_ENTRY, roleid, level);
	// �������
	Json::Value& properties = trackdata[SENSORS_INNER_PROPERTYIES];
	properties[GAME_ANALYTICS_PROPERTIES_VIP] = vip;
	properties[GAME_ANALYTICS_PROPERTIES_SCENE_ID] = sid;
	properties[GAME_ANALYTICS_PROPERTIES_SCENE_CHAPTER] = chapter;
	properties[GAME_ANALYTICS_PROPERTIES_SCENE_TYPE] = type;
	FillCustomArgs(properties, args);
	WriteAnalyticsLog( trackdata );
}

void SensorsAnalyticsCollectorEx::OnSceneExit(
												const char* roleid,		// ��ɫid(require)
												int level,				// ��ɫ��ǰ�ȼ�(require)
												int vip,				// vip�ȼ�(require)
												int sid,				// ����id(require)
												int chapter,			// �����½�(option)
												int type,				// ��������(option) ��Ϸ�ж���
												time_t staytime,		// פ��ʱ��
												int diedNum,			// ��������
												int result,				// ս�����(require)
												const KVArgs * args)
{
	Json::Value trackdata;
	FillSensorsTrackHeader(trackdata, SENSORS_EVENT_TYPE_TRACK, GAME_ANALYTICS_EVENT_COPY_SCENE_EXIT, roleid, level);
	// �������
	Json::Value& properties = trackdata[SENSORS_INNER_PROPERTYIES];
	properties[GAME_ANALYTICS_PROPERTIES_VIP] = vip;
	properties[GAME_ANALYTICS_PROPERTIES_SCENE_ID] = sid;
	properties[GAME_ANALYTICS_PROPERTIES_SCENE_CHAPTER] = chapter;
	properties[GAME_ANALYTICS_PROPERTIES_SCENE_TYPE] = type;
	properties[GAME_ANALYTICS_PROPERTIES_SCENE_RESULT] = result;
	properties[GAME_ANALYTICS_PROPERTIES_SCENE_STAY_TIME] = staytime;
	properties[GAME_ANALYTICS_PROPERTIES_SCENE_DIED_NUM] = diedNum;
	FillCustomArgs(properties, args);
	WriteAnalyticsLog( trackdata );
}

void SensorsAnalyticsCollectorEx::OnVisitFunction(
												const char* roleid,	// ��ɫid(require)
												int level,				// ��ɫ�ȼ�(require)
												int vip,				// vip�ȼ�(require)
												int module,				// ����ģ��(require), ��Ϸ�ж���(��ʲôģ��)
												int func,				// ������(require) ��Ϸ�ж���(����������funcһ��)
												bool result,			// ���(require) true:�ɹ� false: ʧ��
												int value,				// ����˵��(option), ��func��һ��˵��
												const char* extra,		// ����˵��(option), ��func��һ��˵��
												const KVArgs * args
												)
{
	Json::Value trackdata;
	FillSensorsTrackHeader(trackdata, SENSORS_EVENT_TYPE_TRACK, GAME_ANALYTICS_EVENT_VISIT_FUNCTION, roleid, level);
	// �������
	Json::Value& properties = trackdata[SENSORS_INNER_PROPERTYIES];
	properties[GAME_ANALYTICS_PROPERTIES_VIP] = vip;
	properties[GAME_ANALYTICS_PROPERTIES_VISIT_RESULT] = result;
	properties[GAME_ANALYTICS_PROPERTIES_VISIT_VALUE] = value;
	FillModuleInfo(properties, module, func, extra);
	FillCustomArgs(properties, args);
	WriteAnalyticsLog( trackdata );
}

void SensorsAnalyticsCollectorEx::OnCustomEvent(
												const char* roleid,		// ��ɫid(option)
												int level,				// ��ɫ�ȼ�(require)
												int vip,				// vip�ȼ�(require)
												const char* evtName,	// �¼���(require)
												int module,				// ����ģ��(require), ��Ϸ�ж���(��ʲôģ��)
												int func,				// ������(require) ��Ϸ�ж���(����������funcһ��)
												const KVArgs* args		// �����б�(��������ֵ��)
												)
{
	Json::Value trackdata;
	FillSensorsTrackHeader(trackdata, SENSORS_EVENT_TYPE_TRACK, evtName, roleid, level);
	// �������
	Json::Value& properties = trackdata[SENSORS_INNER_PROPERTYIES];
	if (vip > 0)
	{
		properties[GAME_ANALYTICS_PROPERTIES_VIP] = vip;
	}
	FillModuleInfo(properties, module, func, NULL, args);
	WriteAnalyticsLog( trackdata );
}

void SensorsAnalyticsCollectorEx::SetProfile(const char* roleid, const KVArgs& args)
{
	// profile
	Json::Value profiledata;
	FillSensorsProfileHeader(profiledata, roleid, SENSORS_EVENT_TYPE_PROFILE_SET);
	Json::Value& profileset = profiledata[SENSORS_INNER_PROPERTYIES];
	
	FillCustomArgs(profileset, &args);
	WriteAnalyticsLog(profiledata);
}

