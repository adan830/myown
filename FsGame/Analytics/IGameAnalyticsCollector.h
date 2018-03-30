//--------------------------------------------------------------------
// �ļ���:	IGameAnalyticsCollector.h
// ��  ��:	��Ϸ���ݷ����ռ���
// ˵  ��:	
// ��������:	2016��08��24��
// ������:	���
// ������:            
//--------------------------------------------------------------------
#ifndef __IGAME_ANALYTICS_COLLECTOR_H__
#define __IGAME_ANALYTICS_COLLECTOR_H__

#include <string>
#include <vector>
#include "public/Macros.h"
#include "utils/json.h"

typedef Json::Value	KVArgs;
class  IGameAnalyticsCollector	
{
public:
	virtual ~IGameAnalyticsCollector() = 0;

	//-------------------------------------------------------------------------
	// ������
	//-------------------------------------------------------------------------
	// ����������
	virtual void OnServerStart() = 0;
	// �������ر� 
	virtual void OnServerClose() = 0;
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
								const char* os_ver			// �ն�ϵͳ�汾(option)
								) = 0;

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
			) = 0;
	// ɾ���˺�
	virtual void OnDelUser(const char* uid) = 0;
	// ��½�ɹ�
	virtual void OnUserLogin( const char* uid ) = 0;
	// �˳���½
	virtual void OnUserLogout( const char* uid, int64_t onlinetime  ) = 0;

	//-------------------------------------------------------------------------
	// ��ɫ
	//-------------------------------------------------------------------------
	// ������ɫ
	virtual void OnNewRole(	const char* roleid,			// ��ɫid(require)
								const char* uid,		// ��ɫ����uid(require)
								const char* roleName,	// ��ɫ��(option)
								int job,				// ְҵ(option)
								int sex,				// �Ա�(option)
								int flag,				// ������־(option) ����/��Ӫ
								const KVArgs* initArgs	// ��ʼ������
								) = 0;	
	// ɾ����ɫ roleid(require)
	virtual void OnDelRole(const char* roleid, int level, const char* uid) = 0;
	// �����ɫ roleid(require)
	virtual void OnRoleEntry(const char* roleid,		// ��ɫid
							int level,					// ��ɫ�ȼ�
							int vip,					// ��ɫvip
							int64_t gold,				// ��ǰ��ӵ�еĽ��
							const KVArgs* args			// �����б�(��������ֵ��)
							) = 0;
	// �˳���ɫ roleid(require)
	virtual void OnRoleExit(const char* roleid, 		// ��ɫid
							int level,					// ��ɫ�ȼ�
							int vip,					// ��ɫvip
							int64_t gold,				// ��ǰ��ӵ�еĽ��
							int64_t onlinetime,			// ����ʱ��
							const KVArgs* args			// �����б�(��������ֵ��)
							) = 0;


	//-------------------------------------------------------------------------
	// ����
	//-------------------------------------------------------------------------
	// ���ͳ�ֵ roleid(require)
	virtual void OnPaying(
						const char* roleid,			// ��ɫid(require)
						int level,					// ��ɫ�ȼ�
						int vip,					// ��ɫvip�ȼ�
						const char* orderid,		// ����id(require)
						const char* itemid,			// ��Ʒid(require) ���ж���
						int count,					// ���������(require)
						float price,				// ��Ʒ�۸�(require)
						bool first,					// �Ƿ��׳�
						const char* platform		// �ͻ���ƽ̨
						) = 0;
	// ֧������
	virtual void OnPayCompleted(
						const char* roleid,				// ��ɫid(require)
						int level,						// ��ɫ�ȼ�
						int vip,						// ��ɫvip�ȼ�
						const char* orderid,			// ����id(require)
						float amount,					// ʵ��֧�����(require)
						int result,						// ֧�����(require) 0:�ɹ� 1:ʧ�� 2:ȡ��
						bool first,						// �Ƿ��׳�
						const char* platform,			// ֧��ƽ̨(require)(xy, xiaomi,baidu....)
						const char* os					// �ͻ���os
						) = 0;

	//-------------------------------------------------------------------------
	// �ɳ�/���� (��Ϸ�����Ϊģ����¼�)
	//-------------------------------------------------------------------------
	// ���Ӿ���
	virtual void OnIncExp(	
							const char* roleid,		// ��ɫid(require)
							int level,				// ��ɫ�ȼ�
							int value,				// ����ֵ(require)
							int module,				// ����ģ��(option), ��Ϸ�ж���(��ʲôģ��)
							int func,				// �����¼�(option), ��Ϸ�ж���(��ʲô�»��)
							const char* extra		// ����˵��(option), ��event��һ��˵��
							) = 0;
	// �ȼ�����
	virtual void OnLevelup(	
							const char* roleid,			// ��ɫid(require)
							int level,				// ��ǰֵ(require)
							int original			// ԭʼֵ(require)
							) = 0;
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
								const char* extra		// ����˵��(option), ��event��һ��˵��
								) = 0;
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
								const char* extra		// ����˵��(option), ��event��һ��˵��
								) = 0;

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
							const char* extra		// ����˵��(option), ��event��һ��˵��
							) = 0;
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
							const char* extra		// ����˵��(option), ��event��һ��˵��
							) = 0;

	// vip����
	virtual void OnVipLevelup(
							const char* roleid,			// ��ɫid(require)
							int level,				// ��ɫ��ǰ�ȼ�(require)							
							int value,				// ��ֵ(require)
							int original,			// ԭʼֵ(require)
							int module,				// ����ģ��(option), ��Ϸ�ж���(��ʲôģ��)
							int func,				// �����¼�(option), ��Ϸ�ж���(��ʲô�»��)
							const char* extra		// ����˵��(option), ��event��һ��˵��
							) = 0;
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
							const char* extra		// ����˵��(option), ��event��һ��˵��
							) = 0;
	// ��ҽ��׼�¼
	virtual void OnPlayerTransaction( const char* transid,	// ����id(require)
									int transtype,			// ��������(require), ��Ϸ�ڶ���, ��0���ɽ��ף� 1����֮���ͻ�......
									const char* sender,		// ��ɫid1(require)
									const char* reciver,	// ��ɫid2(require)
									const char* senderItem,	// ��Ʒ1id(require)
									int senderCount,		// ��Ʒ1����(require)
									const char* reciverItem,// ��Ʒ2id(option) Ϊ�ձ�ʾ��������
									int reciverCount		// ��Ʒ2����(option)
									) = 0;
	// ��������
	virtual void OnGuideEvent(
								const char* roleid,			// ��ɫid(require)
								int level,				// ��ɫ��ǰ�ȼ�(require)
								int vip,				// vip�ȼ�(require)
								int guideid,				// ����id
								int step					// ��������
								) = 0;

	//-------------------------------------------------------------------------
	// ����ͳ��
	//-------------------------------------------------------------------------
	// ������
	virtual void OnTaskStart(
								const char* roleid,		// ��ɫid(require)
								int level,				// ��ɫ��ǰ�ȼ�(require)
								int vip,				// vip�ȼ�(require)
								int taskid,				// ��ȡ������id(require)
								int type				// ��������(option) ��Ϸ�ж���
								) = 0;
	// �������
	virtual void OnTaskCompleted(
									const char* roleid,		// ��ɫid(require)
									int level,				// ��ɫ��ǰ�ȼ�(require)
									int vip,				// vip�ȼ�(require)
									int taskid,				// ��ȡ������id(require)
									int type				// ��������(option) ��Ϸ�ж���
									) = 0;


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
								int type 					// ��������(option) ��Ϸ�ж���
								) = 0;
	// ���븱��
	virtual void OnSceneEntry(
									const char* roleid,		// ��ɫid(require)
									int level,				// ��ɫ��ǰ�ȼ�(require)
									int vip,				// vip�ȼ�(require)
									int sid,				// ����id(require)
									int chapter,			// �����½�(option)
									int type				// ��������(option) ��Ϸ�ж���
									) = 0;
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
									int result				// ս�����(require)
									) = 0;


	//-------------------------------------------------------------------------
	// ��������ģ��ͳ��
	//-------------------------------------------------------------------------
	// ����ĳ������(����˵���ܿ���)
	virtual void OnVisitFunction(
								const char* roleid,	// ��ɫid(require)
								int level,				// ��ɫ�ȼ�(require)
								int vip,				// vip�ȼ�(require)
								int module,				// ����ģ��(require), ��Ϸ�ж���(��ʲôģ��)
								int func,				// ������(require) ��Ϸ�ж���(����������funcһ��)
								bool result,			// ���(require) true:�ɹ� false: ʧ��
								int value,				// ����˵��(option), ��func��һ��˵��
								const char* extra		// ����˵��(option), ��func��һ��˵��
								) = 0;
	// ��չ�Զ���
	virtual void OnCustomEvent(
								const char* roleid,		// ��ɫid(option)
								int level,				// ��ɫ�ȼ�(require)
								int vip,				// vip�ȼ�(require)
								const char* evtName,		// �¼���(require)
								int module,				// ����ģ��(require), ��Ϸ�ж���(��ʲôģ��)
								int func,				// ������(require) ��Ϸ�ж���(����������funcһ��)
								const KVArgs* args		// �����б�(��������ֵ��)
								) = 0;

	// �����������
	virtual void SetProfile(const char* roleid, const KVArgs& args) = 0;
};
inline IGameAnalyticsCollector::~IGameAnalyticsCollector(){};

#endif