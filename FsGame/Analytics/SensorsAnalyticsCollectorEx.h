//--------------------------------------------------------------------
// �ļ���:	SensorsAnalyticsCollectorEx.h
// ��  ��:	�����Ϸ���ݷ����ռ���
// ˵  ��:	
// ��������:	2016��08��24��
// ������:	���
// ������:            
//--------------------------------------------------------------------
#ifndef __SENSORS_ANALYTICS_COLLECTOR_IMP_H__
#define __SENSORS_ANALYTICS_COLLECTOR_IMP_H__

#include "IGameAnalyticsCollector.h"
#include "SensorsAnalyticsDef.h"
#include "utils/json.h"
#include "utils/string_util.h"
#include <vector>

class SensorsAnalyticsCollectorEx //: public IGameAnalyticsCollector
{
private:
	int64_t m_nDeployId;					// ��Ϸ����id
	int64_t m_nGameId;						// ��Ϸid
	int64_t m_nSrvId;						// ������id
	int64_t m_nDistrictId;					// ����id
	int64_t m_nChannelId;					// ��Ϸ��������id
	int64_t m_nProductionId;				// ����id: ��ʽ�������ǿ������� ...
			
	std::string m_strGameVer;			// ��Ϸ�������汾
	std::string m_strGameName;			// ��Ϸ��
	std::string m_strChannelName;		// ����

	IAnalytisLogger m_anaLogger;

	// ������ʱ��

	
	// ʹ�ù�����ʱ������, ����ʱ���δ�������ʱ��
	std::map<std::string, int64_t> m_strRoleCache;
public:

	SensorsAnalyticsCollectorEx( IAnalytisLogger& logger, 
							int64_t nDeployId,			// ����id
							int64_t nGameid,
							int64_t nSrvId,
							int64_t nDistrictId,
							int64_t nChannelId,				// ��Ϸ���������� ���˻�ר������
							int64_t nProductionId,			// ����id: ��ʽ�������ǿ������� ...
							const char* gameVer,
							const char* gameName, 
							const char* m_strDistrictName);
	//��ʼ��
	bool InitAction();

	IAnalytisLogger& GetLogger()
	{
		return m_anaLogger;
	}

	void SetDeployId(int64_t nDeployId)
	{
		m_nDeployId = nDeployId;
	}
	int64_t GetDeployId() const
	{
		return m_nDeployId;
	}

	void SetGameId(int64_t nGameId)
	{
		m_nGameId = nGameId;
	}
	int64_t GetGameId() const
	{
		return m_nGameId;
	}

	void SetServerId(int64_t nSrvId)
	{
		m_nSrvId = nSrvId;
	}
	int64_t GetServerId() const
	{
		return m_nSrvId;
	}

	void SetDistrictId(int64_t nDistrictId)
	{
		m_nDistrictId = nDistrictId;
	}
	int64_t GetDistrictId() const
	{
		return m_nDistrictId;
	}

	void SetChannelId(int64_t nChannelId)
	{
		m_nChannelId = nChannelId;
	}
	int64_t GetChannelId() const
	{
		return m_nChannelId;
	}

	void SetProductionId(int64_t nProductionId)
	{
		m_nProductionId = nProductionId;
	}
	int64_t GetProductionId() const
	{
		return m_nProductionId;
	}


	//-------------------------------------------------------------------------
	// ������
	//-------------------------------------------------------------------------
	// ����������
	virtual void OnServerStart();
	// �������ر� 
	virtual void OnServerClose();
	// ���ÿͻ����ն���Ϣ
	virtual void OnClientInfo(	const char* roleid,			// ��ɫid(require)
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
								const KVArgs * args);

	//-------------------------------------------------------------------------
	// �˺�
	//-------------------------------------------------------------------------
	// ע���û�
	virtual void OnNewUser(
							const char* uid,		// �û�Ψһid (require)
							const char* anid,		// �û�ԭ����id(option)
							const char* nick,		// �ǳ�/�û���(require)
							int sex,				// �Ա�(option)
							__int64 bron			// ��������(option)
							);
	// ɾ���˺�
	virtual void OnDelUser(const char* uid);
	// ��½�ɹ�
	virtual void OnUserLogin(const char* uid,			// �û�Ψһid (require)
								const char* nick,		// �ǳ�/�û���(require)
								const char* ip,			// �ն�ip(option)
								const char* platform,	// ƽ̨(option)
								const char* manufacturer,	// ������(option)
								const char* devid,		// �ն��豸Ψһ��ʶ(option)
								const char* model,		// �ն�����(option)
								const char* os,			// �ն�ϵͳ(option)
								const char* os_ver,		// �ն�ϵͳ�汾(option)
								bool bReset = false );
	// �˳���½
	virtual void OnUserLogout(const char* uid, int64_t nTimestamp = 0, bool bReset = false);


	//-------------------------------------------------------------------------
	// ��ɫ
	//-------------------------------------------------------------------------
	// ������ɫ
	virtual void OnNewRole(const char* roleid,			// ��ɫid(require)
								const char* uid,		// ��ɫ����uid(require)
								const char* roleName,	// ��ɫ��(option)
								int job,				// ְҵ(option)
								int sex,				// �Ա�(option)
								int flag,				// ������־(option) ����/��Ӫ
								const char* ip,			// ip
								const KVArgs* initArgs	// ��ʼ������
								);
	// ɾ����ɫ roleid(require)
	virtual void OnDelRole(const char* roleid, int level, const char* uid);
	// �����ɫ roleid(require)
	virtual void OnRoleEntry(const char* roleid,			// ��ɫid
								const char* uid,			// ��ɫ����uid(require)
								int level,					// ��ɫ�ȼ�
								int vip,					// ��ɫvip
								int64_t gold,				// ��ǰ��ӵ�еĽ��
								const char* ip,				// ip
								const KVArgs* args,			// �����б�(��������ֵ��)
								bool bReset = false);
	// �˳���ɫ roleid(require)
	virtual void OnRoleExit(const char* roleid, 		// ��ɫid
								const char* uid,		// ��ɫ����uid(require)
								int level,					// ��ɫ�ȼ�
								int vip,					// ��ɫvip
								int64_t gold,				// ��ǰ��ӵ�еĽ��
								int64_t onlinetime,			// ����ʱ��
								int64_t totaltime,			// ������ʱ��
								const char* ip,				// ip
								const KVArgs* args,			// �����б�(��������ֵ��)
								bool bReset = false);


	//-------------------------------------------------------------------------
	// ����
	//-------------------------------------------------------------------------
	// ���ͳ�ֵ roleid(require)
	virtual void OnPaying(
							const char* roleid,			// ��ɫid(require)
							const char* uid,			// ��ɫ����uid(require)
							int level,					// ��ɫ�ȼ�
							int vip,					// ��ɫvip�ȼ�
							const char* orderid,		// ����id(require)
							const char* itemid,			// ��Ʒid(require) ���ж���
							int count,					// ���������(require)
							float price,				// ��Ʒ�۸�(require)
							bool first,					// �Ƿ��׳�
							const char* platform		// �ͻ���ƽ̨
							);
	// ֧������
	virtual void OnPayCompleted(
									const char* roleid,			// ��ɫid(require)
									const char* uid,			// ��ɫ����uid(require)
									int level,					// ��ɫ�ȼ�
									int vip,					// ��ɫvip�ȼ�
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
									);

	//-------------------------------------------------------------------------
	// �ɳ�/���� (��Ϸ�����Ϊģ����¼�)
	//-------------------------------------------------------------------------
	// ���Ӿ���
	virtual void OnIncExp(
		const char* roleid,		// ��ɫid(require)
		int level,				// ��ɫ�ȼ�
		int value,					// ����ֵ(require)
		int newValue,				// ���Ӻ��ֵ
		int origValue,				// ԭʼֵ
		int source	,				// ��Դ
		const KVArgs * args);
	// �ȼ�����
	virtual void OnLevelup( 
									const char* roleid,		// ��ɫid(require)
									int level,				// ��ǰֵ(require)
									int original,			// ԭʼֵ(require)
									const KVArgs * args
									);

	// �����ȼ�����
	void OnMeridianLevelup(
									const char* roleid,		// ��ɫid(require)
									int level,				// ��ɫ��ǰ�ȼ�(require)
									int vip,				// vip�ȼ�(require)
									int value,				// ��ǰֵ(require)
									int original,			// ԭʼֵ(require)
									const KVArgs * args
									);

	// �����Ϸ��
	virtual void OnGainCoin(
									const char* roleid,			// ��ɫid(require)
									int level,				// ��ɫ��ǰ�ȼ�(require)
									int vip,				// vip�ȼ�(require)
									int coid,				// ��Ϸ��id(��/��/ͭ/����....)(require), ��Ϸ�ж���
									int64_t value,			// ����ֵ(require)
									int64_t newvalue,		// ��ǰ��ֵ(require)
									int64_t original,		// ԭʼֵ(require)
									int module,				// ����ģ��(option), ��Ϸ�ж���(��ʲôģ��)
									int func,				// �����¼�(option), ��Ϸ�ж���(��ʲô�»��)
									const char* extra,		// ����˵��(option), ��event��һ��˵��
									const KVArgs * args
									);
	// ������Ϸ��
	virtual void OnCostCoin(
									const char* roleid,			// ��ɫid(require)
									int level,				// ��ɫ��ǰ�ȼ�(require)
									int vip,				// vip�ȼ�(require)
									int coid,				// ��Ϸ��id(��/��/ͭ/����....)(require), ��Ϸ�ж���
									int64_t value,			// ����ֵ(require)
									int64_t newvalue,		// ��ǰ��ֵ
									int64_t original,		// ԭʼֵ(require)
									int module,				// ����ģ��(option), ��Ϸ�ж���(��ʲôģ��)
									int func,				// �����¼�(option), ��Ϸ�ж���(��ʲô�»��)
									const char* extra,		// ����˵��(option), ��event��һ��˵��
									const KVArgs * args
									);

	// �����Ʒ
	virtual void OnGainItem(
									const char* roleid,			// ��ɫid(require)
									int level,				// ��ɫ��ǰ�ȼ�(require)
									int vip,				// vip�ȼ�(require)
									const char* itemid,		// ��Ʒid(require)
									int num,				// �����ֵ(require)
									int count,				// ��ǰӵ����
									int orig,				// ԭ��ӵ����
									int module,				// ����ģ��(option), ��Ϸ�ж���(��ʲôģ��)
									int func,				// �����¼�(option), ��Ϸ�ж���(��ʲô�»��)
									const char* extra,		// ����˵��(option), ��event��һ��˵��
									const KVArgs * args
									);
	// ʹ����Ʒ
	virtual void OnCostItem(
									const char* roleid,			// ��ɫid(require)
									int level,				// ��ɫ��ǰ�ȼ�(require)
									int vip,				// vip�ȼ�(require)
									const char* itemid,		// ��Ʒid(require)
									int num,				// �����ֵ(require)
									int count,				// ��ǰӵ����
									int orig,				// ԭ��ӵ����
									int module,				// ����ģ��(option), ��Ϸ�ж���(��ʲôģ��)
									int func,				// �����¼�(option), ��Ϸ�ж���(��ʲô�»��)
									const char* extra,		// ����˵��(option), ��event��һ��˵��
									const KVArgs * args
									);

	// vip����
	virtual void OnVipLevelup(
										const char* roleid,			// ��ɫid(require)
										int level,				// ��ɫ��ǰ�ȼ�(require)
										int value,				// ��ֵ(require)
										int original,			// ԭʼֵ(require)
										int module,				// ����ģ��(option), ��Ϸ�ж���(��ʲôģ��)
										int func,				// �����¼�(option), ��Ϸ�ж���(��ʲô�»��)
										const char* extra,		// ����˵��(option), ��event��һ��˵��
										const KVArgs * args
										);
	// �ؼ����Ա仯
	virtual void OnPlayerPropertyChanged(										
										const char* roleid,			// ��ɫid(require)
										int level,				// ��ɫ��ǰ�ȼ�(require)
										int vip,				// vip�ȼ�(require)
										const char* propery,					// ������
										float value,								// ����ֵ(require)
										float original,							// ԭʼֵ(require)
										int module,				// ����ģ��(option), ��Ϸ�ж���(��ʲôģ��)
										int func,				// �����¼�(option), ��Ϸ�ж���(��ʲô�»��)
										const char* extra,		// ����˵��(option), ��event��һ��˵��
										const KVArgs * args
										);
	// ��ҽ��׼�¼
	virtual void OnPlayerTransaction(const char* transid,	// ����id(require)
										int transtype,			// ��������(require), ��Ϸ�ڶ���, ��0���ɽ��ף� 1����֮���ͻ�......
										const char* sender,		// ��ɫid1(require)
										const char* reciver,	// ��ɫid2(require)
										const char* senderItem,	// ��Ʒ1id(require)
										int senderCount,		// ��Ʒ1����(require)
										const char* reciverItem,// ��Ʒ2id(option) Ϊ�ձ�ʾ��������
										int reciverCount,		// ��Ʒ2����(option)
										const KVArgs * args
										);
	// ��������
	virtual void OnGuideEvent(
								const char* roleid,			// ��ɫid(require)
								int level,				// ��ɫ��ǰ�ȼ�(require)
								int vip,				// vip�ȼ�(require)
								int guideid,				// ����id
								int step,					// ��������
								const KVArgs * args
								);

	//-------------------------------------------------------------------------
	// ����ͳ��
	//-------------------------------------------------------------------------
	// ������
	virtual void OnTaskStart(
									const char* roleid,		// ��ɫid(require)
									int level,				// ��ɫ��ǰ�ȼ�(require)
									int vip,				// vip�ȼ�(require)
									int taskid,				// ��ȡ������id(require)
									int type,				// ��������(option) ��Ϸ�ж���
									int opt_type,			// ���������ʽ(option) ��Ϸ�ж���
									const int state,		// ����״̬	
									const KVArgs * args
									);
	// �������
	virtual void OnTaskCompleted(
									const char* roleid,		// ��ɫid(require)
									int level,				// ��ɫ��ǰ�ȼ�(require)
									int vip,				// vip�ȼ�(require)
									int taskid,				// ��ȡ������id(require)
									int type,				// ��������(option) ��Ϸ�ж���
									int opt_type,			// ���������ʽ(option) ��Ϸ�ж���
									const int state,		// ����״̬	
									const KVArgs * args
									);
	// ������
	virtual void OnTaskSubmit(
		const char* roleid,		// ��ɫid(require)
		int level,				// ��ɫ��ǰ�ȼ�(require)
		int vip,				// vip�ȼ�(require)
		int taskid,				// ��ȡ������id(require)
		int type,				// ��������(option) ��Ϸ�ж���
		int opt_type,			// ���������ʽ(option) ��Ϸ�ж���
		const int state,		// ����״̬	
		const KVArgs * args
		);

	//-------------------------------------------------------------------------
	// ����ͳ��
	//-------------------------------------------------------------------------
	// ��������
	virtual void OnSceneOpen(
											const char* roleid,		// ��ɫid(require)
											int level,				// ��ɫ��ǰ�ȼ�(require)
											int vip,				// vip�ȼ�(require)
											int sid,					// ����id(require)
											int chapter,				// �����½�(option)
											int type ,					// ��������(option) ��Ϸ�ж���
											const KVArgs * args
											) ;
	// ���븱��
	virtual void OnSceneEntry(
											const char* roleid,		// ��ɫid(require)
											int level,				// ��ɫ��ǰ�ȼ�(require)
											int vip,				// vip�ȼ�(require)
											int sid,				// ����id(require)
											int chapter,			// �����½�(option)
											int type,				// ��������(option) ��Ϸ�ж���
											const KVArgs * args
											);
	// �˳�����
	virtual void OnSceneExit(
											const char* roleid,		// ��ɫid(require)
											int level,				// ��ɫ��ǰ�ȼ�(require)
											int vip,				// vip�ȼ�(require)
											int sid,				// ����id(require)
											int chapter,			// �����½�(option)
											int type,				// ��������(option) ��Ϸ�ж���
											int64_t staytime,		// פ��ʱ��
											int diedNum,			// ��������
											int result,				// ս�����(require)
											const KVArgs * args
											) ;


	//-------------------------------------------------------------------------
	// ��������ģ��ͳ��
	//-------------------------------------------------------------------------
	// ����ĳ������(����˵���ܿ���)
	virtual void OnVisitFunction(
								const char* roleid,		// ��ɫid(require)
								int level,				// ��ɫ�ȼ�(require)
								int vip,				// vip�ȼ�(require)
								int module,				// ����ģ��(require), ��Ϸ�ж���(��ʲôģ��)
								int func,				// ������(require) ��Ϸ�ж���(����������funcһ��)
								bool result,			// ���(require) true:�ɹ� false: ʧ��
								int value,				// ����˵��(option), ��func��һ��˵��
								const char* extra,		// ����˵��(option), ��func��һ��˵��
								const KVArgs * args
								);
	// ��չ�Զ���
	virtual void OnCustomEvent( 
								const char* roleid,		// ��ɫid(option)
								int level,				// ��ɫ�ȼ�(require)
								int vip,				// vip�ȼ�(require)
								const char* evtName,		// �¼���(require)
								int module,				// ����ģ��(require), ��Ϸ�ж���(��ʲôģ��)
								int func,				// ������(require) ��Ϸ�ж���(����������funcһ��)
								const KVArgs* args		// �����б�(��������ֵ��)
								);


	// �����������
	virtual void SetProfile(const char* roleid, const KVArgs& args);

	
private:
	void FillSensorsTrackHeader(Json::Value& root, 
								const char* type,
								const char* evt, 
								const char* roleid,
								int level = 0);
	void FillModuleInfo(Json::Value& root, int module, int func, const char* extra, const KVArgs* args = NULL) const;
	void FillSensorsProfileHeader(Json::Value& root, const char* roleid, const char* type);
	static void FillSensorsProfileArgs( Json::Value& root, int job, const KVArgs* args)
	{
		if (NULL != args)
		{
			for (auto itr = args->begin(); itr != args->end(); ++itr)
			{		
				root[ itr.memberName() ] = *itr;
				/*
				switch (itr->second.GetType())
				{
				case VTYPE_BOOL:
				{
					bool value = itr->second.BoolVal();
					root[itr->first.c_str()] = value;
				}
				break;
				case VTYPE_INT:
				{
					int value = itr->second.IntVal();
					root[itr->first.c_str()] = value;
				}
				break;
				case VTYPE_INT64:
				{
					int64_t value = itr->second.Int64Val();
					root[itr->first.c_str()] = value;
				}
				break;
				case VTYPE_FLOAT:
				{
					float value = itr->second.FloatVal();
					root[itr->first.c_str()] = value;
				}
				break;
				case VTYPE_DOUBLE:
				{
					double value = itr->second.DoubleVal();
					root[itr->first.c_str()] = value;
				}
				break;
				case VTYPE_STRING:
				{
					const char* value = itr->second.StringVal();
					InsertString(root, itr->first.c_str(), value);
				}
				break;
				case VTYPE_WIDESTR:
				{
					const char* value = StringUtil::WideStrAsUTF8String(itr->second.WideStrVal()).c_str();
					InsertString(root, itr->first.c_str(), value);
				}
				break;
				default:
					break;
				}
				*/
			}
		}
	}
	static void FillCustomArgs(Json::Value& root, const KVArgs* args)
	{
		if (NULL != args)
		{
			for (auto itr = args->begin(); itr != args->end(); ++itr)
			{
				root[itr.memberName()] = *itr;
			}
		}
	}
	//const char* GetModuleName(int nModule) const;
	//const char* GetFuncName(int nFuncName) const;
	static void InsertString(Json::Value& root, const char* key, const char* value)
	{
		if (NULL != value)
		{
			root[key] = value;
		}
	}

	void WriteAnalyticsLog(const Json::Value& root) const
	{
		Json::FastWriter writter;
		ANALYTICS_LOG(m_anaLogger, writter.write(root));
	}

	struct LogActionType
	{
		std::string m_actionName; //����
		int m_actionMin;		  //��С��Χ
		int m_actionMax;		  //���Χ
	};

	std::vector<LogActionType> m_action;

	inline const char* GetActionName(int actionType)const
	{
		std::vector<LogActionType>::const_iterator it = m_action.begin();
		for (; it != m_action.end(); ++it)
		{
			if (it->m_actionMin <= actionType && actionType <= it->m_actionMax){
				return (it->m_actionName).c_str();
			}
		}
		return "log_event";
	}

};

#endif