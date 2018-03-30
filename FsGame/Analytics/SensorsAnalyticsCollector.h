//--------------------------------------------------------------------
// �ļ���:	SensorsAnalyticsCollector.h
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

class SensorsAnalyticsCollector : public IGameAnalyticsCollector
{
private:
	int m_nGameId;						// ��Ϸid
	int m_nSrvId;						// ������id
	int m_nChannelId;					// ����id
			
	std::string m_strGameVer;			// ��Ϸ�������汾
	std::string m_strGameName;			// ��Ϸ��
	std::string m_strChannelName;		// ����

	IAnalytisLogger m_anaLogger;
public:

	SensorsAnalyticsCollector( IAnalytisLogger& logger, 
							int nGameid, int nSrvId, int nChannelId,
							const char* gameVer, const char* gameName, const char* m_strChannelName);
	IAnalytisLogger& GetLogger()
	{
		return m_anaLogger;
	}

	//-------------------------------------------------------------------------
	// ������
	//-------------------------------------------------------------------------
	// ����������
	virtual void OnServerStart();
	// �������ر� 
	virtual void OnServerClose();
	// ���ÿͻ����ն���Ϣ
	virtual void OnClientInfo(const char* uid,				// �û�Ψһid (require)
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
							const char* channel			// ����(option)
							);

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
	virtual void OnUserLogin(const char* uid);
	// �˳���½
	virtual void OnUserLogout(const char* uid, int64_t onlinetime);


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
								const KVArgs* initArgs	// ��ʼ������
								);
	// ɾ����ɫ roleid(require)
	virtual void OnDelRole(const char* roleid, int job, int level, const char* uid);
	// �����ɫ roleid(require)
	virtual void OnRoleEntry(const char* roleid,			// ��ɫid
								const char* roleName,		// ��ɫ��
								int job,
								int level,					// ��ɫ�ȼ�
								int vip,					// ��ɫvip
								int64_t gold,				// ��ǰ��ӵ�еĽ��
								const char* uid				// ��ɫ�����˺�
								);
	// �˳���ɫ roleid(require)
	virtual void OnRoleExit(const char* roleid, 		// ��ɫid
								const char* roleName,		// ��ɫ��
								int job,
								int level,					// ��ɫ�ȼ�
								int vip,					// ��ɫvip
								int64_t gold,				// ��ǰ��ӵ�еĽ��
								int64_t onlinetime,			// ����ʱ��
								const char* uid				// ��ɫ�����˺�
								);


	//-------------------------------------------------------------------------
	// ����
	//-------------------------------------------------------------------------
	// ���ͳ�ֵ roleid(require)
	virtual void OnPaying(const char* uid,			// �û�id
							const char* roleid,			// ��ɫid(require)
							int job,
							int level,					// ��ɫ�ȼ�
							int vip,					// ��ɫvip�ȼ�
							const char* orderid,		// ����id(require)
							const char* itemid,			// ��Ʒid(require) ���ж���
							int count,					// ���������(require)
							float price,				// ��Ʒ�۸�(require)
							bool first,					// �Ƿ��׳�
							const char* os				// �ͻ���os(android, ios, pc...)
							);
	// ֧������
	virtual void OnPayCompleted(const char* uid,				// �û�id
									const char* roleid,			// ��ɫid(require)
									int job,
									int level,					// ��ɫ�ȼ�
									int vip,					// ��ɫvip�ȼ�
									const char* orderid,			// ����id(require)
									float amount,					// ʵ��֧�����(require)
									int result,						// ֧�����(require) 0:�ɹ� 1:ʧ�� 2:ȡ��
									bool first,						// �Ƿ��׳�
									const char* platform,			// ֧��ƽ̨(require)(xy, xiaomi,baidu....)
									const char* os,					// �ͻ���os
									const char* reason				// �ǳɹ�ԭ��(option)
									);

	//-------------------------------------------------------------------------
	// �ɳ�/���� (��Ϸ�����Ϊģ����¼�)
	//-------------------------------------------------------------------------
	// ���Ӿ���
	virtual void OnIncExp(const char* uid,			// �û�id
							const char* roleid,		// ��ɫid(require)
							int job,
							int level,				// ��ɫ�ȼ�
							int value,				// ����ֵ(require)
							int module,				// ����ģ��(option), ��Ϸ�ж���(��ʲôģ��)
							int func,				// �����¼�(option), ��Ϸ�ж���(��ʲô�»��)
							const char* extra		// ����˵��(option), ��event��һ��˵��
							);
	// �ȼ�����
	virtual void OnLevelup( const char* uid,				// �û�id
									const char* roleid,		// ��ɫid(require)
									int job,
									int level,				// ��ǰֵ(require)
									int original			// ԭʼֵ(require)
									);
	// �����Ϸ��
	virtual void OnGainCoin(const char* uid, 
									const char* roleid,			// ��ɫid(require)
									int job,
									int level,				// ��ɫ��ǰ�ȼ�(require)
									int vip,				// vip�ȼ�(require)
									int coid,				// ��Ϸ��id(��/��/ͭ/����....)(require), ��Ϸ�ж���
									int64_t value,			// ����ֵ(require)
									int64_t newvalue,		// ��ǰ��ֵ(require)
									int64_t original,		// ԭʼֵ(require)
									int module,				// ����ģ��(option), ��Ϸ�ж���(��ʲôģ��)
									int func,				// �����¼�(option), ��Ϸ�ж���(��ʲô�»��)
									const char* extra		// ����˵��(option), ��event��һ��˵��
									);
	// ������Ϸ��
	virtual void OnCostCoin(const char* uid,
									const char* roleid,			// ��ɫid(require)
									int job,
									int level,				// ��ɫ��ǰ�ȼ�(require)
									int vip,				// vip�ȼ�(require)
									int coid,				// ��Ϸ��id(��/��/ͭ/����....)(require), ��Ϸ�ж���
									int64_t value,			// ����ֵ(require)
									int64_t newvalue,		// ��ǰ��ֵ
									int64_t original,		// ԭʼֵ(require)
									int module,				// ����ģ��(option), ��Ϸ�ж���(��ʲôģ��)
									int func,				// �����¼�(option), ��Ϸ�ж���(��ʲô�»��)
									const char* extra		// ����˵��(option), ��event��һ��˵��
									);

	// �����Ʒ
	virtual void OnGainItem(const char* uid,
									const char* roleid,			// ��ɫid(require)
									int job,
									int level,				// ��ɫ��ǰ�ȼ�(require)
									int vip,				// vip�ȼ�(require)
									const char* itemid,		// ��Ʒid(require)
									const char* itemName,	// ��Ʒ��ʾ����(option)
									int num,				// �����ֵ(require)
									int count,				// ��ǰӵ����
									int orig,				// ԭ��ӵ����
									int module,				// ����ģ��(option), ��Ϸ�ж���(��ʲôģ��)
									int func,				// �����¼�(option), ��Ϸ�ж���(��ʲô�»��)
									const char* extra		// ����˵��(option), ��event��һ��˵��
									);
	// ʹ����Ʒ
	virtual void OnCostItem(const char* uid,
									const char* roleid,			// ��ɫid(require)
									int job,
									int level,				// ��ɫ��ǰ�ȼ�(require)
									int vip,				// vip�ȼ�(require)
									const char* itemid,		// ��Ʒid(require)
									const char* itemName,	// ��Ʒ��ʾ����(option)
									int num,				// �����ֵ(require)
									int count,				// ��ǰӵ����
									int orig,				// ԭ��ӵ����
									int module,				// ����ģ��(option), ��Ϸ�ж���(��ʲôģ��)
									int func,				// �����¼�(option), ��Ϸ�ж���(��ʲô�»��)
									const char* extra		// ����˵��(option), ��event��һ��˵��
									);

	// vip����
	virtual void OnVipLevelup(const char* uid,
										const char* roleid,			// ��ɫid(require)
										int job,
										int level,				// ��ɫ��ǰ�ȼ�(require)
										int value,				// ��ֵ(require)
										int original,			// ԭʼֵ(require)
										int module,				// ����ģ��(option), ��Ϸ�ж���(��ʲôģ��)
										int func,				// �����¼�(option), ��Ϸ�ж���(��ʲô�»��)
										const char* extra		// ����˵��(option), ��event��һ��˵��
										);
	// �ؼ����Ա仯
	virtual void OnPlayerPropertyChanged(
										const char* uid,
										const char* roleid,			// ��ɫid(require)
										int job,
										int level,				// ��ɫ��ǰ�ȼ�(require)
										int vip,				// vip�ȼ�(require)
										const char* propery,					// ������
										float value,								// ����ֵ(require)
										float original,							// ԭʼֵ(require)
										int module,				// ����ģ��(option), ��Ϸ�ж���(��ʲôģ��)
										int func,				// �����¼�(option), ��Ϸ�ж���(��ʲô�»��)
										const char* extra		// ����˵��(option), ��event��һ��˵��
										);
	// ��ҽ��׼�¼
	virtual void OnPlayerTransaction(const char* transid,	// ����id(require)
										int transtype,			// ��������(require), ��Ϸ�ڶ���, ��0���ɽ��ף� 1����֮���ͻ�......
										const char* sender,		// ��ɫid1(require)
										const char* reciver,	// ��ɫid2(require)
										const char* senderItem,	// ��Ʒ1id(require)
										int senderCount,		// ��Ʒ1����(require)
										const char* reciverItem,// ��Ʒ2id(option) Ϊ�ձ�ʾ��������
										int reciverCount		// ��Ʒ2����(option)
										);
	// ��������
	virtual void OnGuideEvent(const char* uid,
								const char* roleid,			// ��ɫid(require)
								int job,
								int level,				// ��ɫ��ǰ�ȼ�(require)
								int vip,				// vip�ȼ�(require)
								int guideid,				// ����id
								int step					// ��������
								);

	//-------------------------------------------------------------------------
	// ����ͳ��
	//-------------------------------------------------------------------------
	// ������
	virtual void OnTaskStart(const char* uid,
									const char* roleid,		// ��ɫid(require)
									int job,
									int level,				// ��ɫ��ǰ�ȼ�(require)
									int vip,				// vip�ȼ�(require)
									int taskid,				// ��ȡ������id(require)
									const char* taskName,	// ������
									int type				// ��������(option) ��Ϸ�ж���
									);
	// ������
	virtual void OnTaskCompleted(const char* uid,
									const char* roleid,		// ��ɫid(require)
									int job,
									int level,				// ��ɫ��ǰ�ȼ�(require)
									int vip,				// vip�ȼ�(require)
									int taskid,				// ��ȡ������id(require)
									const char* taskName,	// ������
									int type				// ��������(option) ��Ϸ�ж���
									);


	//-------------------------------------------------------------------------
	// ����ͳ��
	//-------------------------------------------------------------------------
	// ��������
	virtual void OnSceneOpen(const char* uid,
											const char* roleid,		// ��ɫid(require)
											int job,
											int level,				// ��ɫ��ǰ�ȼ�(require)
											int vip,				// vip�ȼ�(require)
											int sid,					// ����id(require)
											const char* sname,			// ������
											int chapter,				// �����½�(option)
											int type 					// ��������(option) ��Ϸ�ж���
											) ;
	// ���븱��
	virtual void OnSceneEntry(const char* uid,
											const char* roleid,		// ��ɫid(require)
											int job,
											int level,				// ��ɫ��ǰ�ȼ�(require)
											int vip,				// vip�ȼ�(require)
											int sid,				// ����id(require)
											const char* sname,			// ������
											int chapter,			// �����½�(option)
											int type,				// ��������(option) ��Ϸ�ж���
											int result,				// ������(require)
											const char* reason		// �������(option)
											);
	// �˳�����
	virtual void OnSceneExit(const char* uid,
											const char* roleid,		// ��ɫid(require)
											int job,
											int level,				// ��ɫ��ǰ�ȼ�(require)
											int vip,				// vip�ȼ�(require)
											int sid,				// ����id(require)
											const char* sname,		// ������
											int chapter,			// �����½�(option)
											int type,				// ��������(option) ��Ϸ�ж���
											int64_t staytime,		// פ��ʱ��
											int result,				// ս�����(require)
											const char* reason		// �������(option)
											) ;


	//-------------------------------------------------------------------------
	// ��������ģ��ͳ��
	//-------------------------------------------------------------------------
	// ����ĳ������(����˵���ܿ���)
	virtual void OnVisitFunction(const char* uid,		// �û�id(require)
								const char* roleid,		// ��ɫid(require)
								int job,
								int level,				// ��ɫ�ȼ�(require)
								int vip,				// vip�ȼ�(require)
								int module,				// ����ģ��(require), ��Ϸ�ж���(��ʲôģ��)
								int func,				// ������(require) ��Ϸ�ж���(����������funcһ��)
								bool result,			// ���(require) true:�ɹ� false: ʧ��
								int value,				// ����˵��(option), ��func��һ��˵��
								const char* extra		// ����˵��(option), ��func��һ��˵��
								);
	// ��չ�Զ���
	virtual void OnCustomEvent( const char* uid,		// �û�id(require)
								const char* roleid,		// ��ɫid(option)
								int job,
								int level,				// ��ɫ�ȼ�(require)
								int vip,				// vip�ȼ�(require)
								const char* evtName,		// �¼���(require)
								int module,				// ����ģ��(require), ��Ϸ�ж���(��ʲôģ��)
								int func,				// ������(require) ��Ϸ�ж���(����������funcһ��)
								const KVArgs* args		// �����б�(��������ֵ��)
								);

private:
	void FillSensorsTrackHeader(Json::Value& root, 
								const char* distinctid, 
								const char* type,
								const char* evt, 
								const char* roleid = NULL,
								int level = 0,
								const char* original_id = NULL) const ;
	void FillModuleInfo(Json::Value& root, int module, int func, const char* extra, const KVArgs* args = NULL) const;
	void FillSensorsProfileHeader(Json::Value& root, const char* distinctid, const char* type);

	template<typename T>
	void FillSensorsProfileProperty(Json::Value& root, int job, const char* key, T& value)
	{
		char szKey[256] = { 0 };
		sprintf_s(szKey, "%s_%d", key, job);
		root[szKey] = value;
	};
	template<>
	void FillSensorsProfileProperty<const char*>(Json::Value& root, int job, const char* key, const char*& value)
	{
		if (NULL != value)
		{
			char szKey[256] = { 0 };
			sprintf_s(szKey, "%s_%d", key, job);
			root[szKey] = value;
		}
	};
	template<>
	void FillSensorsProfileProperty<Json::Value>(Json::Value& root, int job, const char* key, Json::Value& value)
	{
		char szKey[256] = { 0 };
		sprintf_s(szKey, "%s_%d", key, job);
		root[szKey] = value;
	};
	void FillSensorsProfileArgs( Json::Value& root, int job, const KVArgs* args)
	{
		if (NULL != args)
		{
			Json::Value::iterator itr = args->begin();
			for (; itr != args->end(); ++itr)
			{
				FillSensorsProfileProperty( root, job, itr.memberName(), *itr);
			}
			/*
			for (auto itr = args->begin(); itr != args->end(); ++itr)
			{
				switch (itr->second.GetType())
				{
				case VTYPE_BOOL:
				{
					bool value = itr->second.BoolVal();
					FillSensorsProfileProperty(root, job, itr->first.c_str(), value);
				}
				break;
				case VTYPE_INT:
				{
					int value = itr->second.IntVal();
					FillSensorsProfileProperty(root, job, itr->first.c_str(), value);
				}
				break;
				case VTYPE_INT64:
				{
					int64_t value = itr->second.Int64Val();
					FillSensorsProfileProperty(root, job, itr->first.c_str(), value);
				}
				break;
				case VTYPE_FLOAT:
				{
					float value = itr->second.FloatVal();
					FillSensorsProfileProperty(root, job, itr->first.c_str(), value);
				}
				break;
				case VTYPE_DOUBLE:
				{
					double value = itr->second.DoubleVal();
					FillSensorsProfileProperty(root, job, itr->first.c_str(), value);
				}
				break;
				case VTYPE_STRING:
				{
					const char* value = itr->second.StringVal();
					FillSensorsProfileProperty(root, job, itr->first.c_str(), value);
				}
				break;
				case VTYPE_WIDESTR:
				{
					const char* value = StringUtil::WideStrAsUTF8String(itr->second.WideStrVal()).c_str();
					FillSensorsProfileProperty(root, job, itr->first.c_str(), value);
				}
				break;
				default:
					break;
				}
			}
			*/
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
};

#endif