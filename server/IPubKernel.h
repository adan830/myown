//--------------------------------------------------------------------
// �ļ���:		IPubKernel.h
// ��  ��:		
// ˵  ��:		
// ��������:	2008��11��12��	
// ������:		½����
// ��Ȩ����:	������ţ�������޹�˾
//--------------------------------------------------------------------

#ifndef _SERVER_IPUBKERNEL_H
#define _SERVER_IPUBKERNEL_H

#include "../public/Macros.h"
#include "../public/IVarList.h"
#include "IPubSpace.h"

// ���������߼��ӿڰ汾��
#define PUBLIC_LOGIC_MODULE_VERSION 0x3010

class IPubKernel;

// ��ʱ������
typedef int (__cdecl* PUBLIC_TIMER_FUNC)(IPubKernel* pPubKernel, 
	const char* space_name, const wchar_t* data_name, int time) ;
	
// �������ݺ��Ľӿ�

class IPubKernel
{
public:
	virtual ~IPubKernel() = 0;
	
	/// \brief ���ҹ������ݿռ�
	/// \param space_name �ռ�����
	virtual bool FindPubSpace(const char* space_name) = 0;
	/// \brief ��ù������ݿռ�����
	virtual int GetPubSpaceCount() = 0;
	/// \brief ��ù������ݿռ��б�
	/// \param result �ռ������б�
	virtual int GetPubSpaceList(IVarList& result) = 0;
	/// \brief ���ָ�����ֵĹ������ݿռ�
	/// \param space_name �ռ�����
	virtual IPubSpace* GetPubSpace(const char* space_name) = 0;
	/// \brief ���湫�����ݿռ����иı��������
	/// \param space_name �ռ�����
	virtual bool SavePubSpace(const char* space_name) = 0;
	/// \brief ���湫�������� 
	/// \param space_name �ռ�����
	/// \param data_name ����������
	virtual bool SavePubData(const char* space_name, 
		const wchar_t* data_name) = 0;
	
	/// \brief ����Ϣ������������
	/// \param source_id ��������ʶ��Ϊ0��ʾ0�ų���������
	/// \param scene_id ������ţ�Ϊ0��ʾ��һ������
	/// \param msg ��Ϣ����
	virtual bool SendPublicMessage(int source_id, int scene_id, 
		const IVarList& msg) = 0;
	
	/// \brief ���Ҷ�ʱ��
	/// \param name ��ʱ������
	/// \param space_name �����������ݿռ�����
	/// \param data_name ��������������
	virtual bool FindTimer(const char* name, const char* space_name, 
		const wchar_t* data_name) = 0;
	/// \brief ��Ӷ�ʱ��
	/// \param name ��ʱ������
	/// \param func ��ʱ������ָ��
	/// \param space_name �����������ݿռ�����
	/// \param data_name ��������������
	/// \param time ��ʱʱ��
	/// \param count ��ʱ����
	virtual bool AddTimer(const char* name, PUBLIC_TIMER_FUNC func, 
		const char* space_name, const wchar_t* data_name, int time, 
		int count) = 0;
	/// \brief ɾ����ʱ��
	/// \param name ��ʱ������
	/// \param space_name �����������ݿռ�����
	/// \param data_name ��������������
	virtual bool RemoveTimer(const char* name, const char* space_name, 
		const wchar_t* data_name) = 0;
	
	/// \brief ��������������״̬�����������Ϣ
	virtual bool RequestServerState() = 0;
	/// \brief �����ÿ��Լ��ص������������߽�ɫ����
	/// \param amount �������
	/// \param year,month,day,hour,minute,second ��׼ʱ��
	/// \param update_resume_time �Ƿ�Ҫ���½�ɫ����������ʱ��Ϊ��׼ʱ��
	/// \param scene_id ֻ��ȡָ�����������߽�ɫ��Ϊ0��ʾȫ������
	virtual bool RequestRoleOffline(int amount, int year, int month, int day,
		int hour, int minute, int second, bool update_resume_time,
		int scene_id = 0) = 0;
	/// \brief �����������������������
	/// \param account ����ʺ�
	/// \param role_name ��ҽ�ɫ��
	/// \param live_seconds ����Ϸ�д��ڵ�ʱ�䣨������
	virtual bool RequestLoadPlayer(const char* account, 
		const wchar_t* role_name, int live_seconds) = 0;
		
	/// \brief ������Ϸ��־
	/// \param name ����
	/// \param type ����
	/// \param content ����
	/// \param comment ��ע
	virtual bool SaveLog(const wchar_t* name, int type, 
		const wchar_t* content, const wchar_t* comment) = 0;
	/// \brief �����Զ�����ʽ����־���Զ�������־��ˮ�ţ�
	/// \param table_name ��־���ݿ����
	/// \param fields �����ֶ�����
	/// �ֶ��������ͣ�����(int)����ֵ(double)���ַ��������ַ����������ַ�������ʽ"YYYYMMDD hh:mm:ss"��
	virtual bool CustomLog(const char* table_name, const IVarList& fields) = 0;
	
	/// \brief �����Դ�ļ�·��
	virtual const char* GetResourcePath() = 0;
	
	/// \brief ��õ�ǰ������
	virtual int GetDistrictId() = 0;
	/// \brief ��õ�ǰ���������
	virtual int GetServerId() = 0;
	
	/// \brief �����ϵͳLOG�ļ�
	/// \param info ��Ϣ����
	virtual void Trace(const char* info) = 0;
	/// \brief ���������̨��Ļ
	/// \param info ��Ϣ����
	virtual void Echo(const char* info) = 0;

	/// \brief ���ݽ�ɫ����ý�ɫΨһ��ʶ�����ؿ��ַ�����ʾ��ɫ������
	/// \param role_name ��ɫ��
	virtual const char* SeekRoleUid(const wchar_t* role_name) = 0;
	/// \brief ���ݽ�ɫΨһ��ʶ��ý�ɫ�������ؿ��ַ�����ʾ��ɫ������
	/// \param role_uid ��ɫΨһ��ʶ
	virtual const wchar_t* SeekRoleName(const char* role_uid) = 0;
	/// \brief ��ý�ɫ�Ƿ��ѱ�ɾ��
	/// \param role_name ��ɫ��
	virtual bool GetRoleDeleted(const wchar_t* role_name) = 0;
	/// \brief ��ý�ɫ�Ƿ���ɾ��������
	/// \param role_name ��ɫ��
	virtual bool GetRoleDeleteProtect(const wchar_t* role_name) = 0;

	/// \brief ���ҹ�ϵ
	/// \param uid ��ұ��
	/// \param target_uid Ŀ����ұ��
	virtual bool FindRelation(const char* uid, const char* target_uid) = 0;
	/// \brief ��ù�ϵ��Χ
	/// \param uid ��ұ��
	/// \param target_uid Ŀ����ұ��
	virtual int GetRelation(const char* uid, const char* target_uid) = 0;
	/// \brief ��ӹ�ϵ
	/// \param uid ��ұ��
	/// \param target_uid Ŀ����ұ��
	/// \param range ��ϵ��Χ
	virtual bool AddRelation(const char* uid, const char* target_uid, 
		int range) = 0;
	/// \brief ɾ����ϵ
	/// \param uid ��ұ��
	/// \param target_uid Ŀ����ұ��
	virtual bool RemoveRelation(const char* uid, const char* target_uid) = 0;
	/// \brief ���ָ�����͵Ĺ�ϵ����
	/// \param uid ��ұ��
	/// \param range ��ϵ��Χ
	virtual int GetRelationCount(const char* uid, int range) = 0;
	/// \brief ���ָ����Χ��ϵ����б�
	/// \param uid ��ұ��
	/// \param range ��ϵ��Χ
	/// \param result ������ұ���б�
	/// \param max_num ����������
	virtual int GetRelationList(const char* uid, int range, IVarList& result, 
		int max_num) = 0;
	/// \brief ���ָ�����͵ı�����ϵ����
	/// \param uid ��ұ��
	/// \param range ��ϵ��Χ
	virtual int GetPassiveRelationCount(const char* uid, int range) = 0;

	/// \brief ������Ϊ��������
	/// \param value ����
	virtual bool SetFeedCacheMax(int value) = 0;
	/// \brief ������Ϊ�Զ�����Ϣ���
	/// \param value �Զ�����Ϣ���
	virtual bool SetFeedCustomId(int value) = 0;
	/// \brief ���ò�ͬ������Ϊ������ʱ��
	/// \param level ��Ϊ����
	/// \param live_days ����ʱ�䣬��λ��
	virtual bool SetFeedLiveTime(int level, double live_days) = 0;
	/// \brief ������Ϊ
	/// \param uid ��ұ��
	/// \param target_uid Ŀ����ұ��
	/// \param range ��ϵ��Χ
	/// \param level ��Ϊ����
	/// \param category ��Ϊ����
	/// \param desc ��Ϊ����
	/// \param comment_at ������Ϊ���
	/// \param need_save �Ƿ���Ҫ���浽���ݿ�
	/// \return feed_id �µ���Ϊ���
	virtual const char* NewFeed(const char* uid, const char* target_uid, 
		int range, int level, int category, const wchar_t* desc, 
		const char* comment_at, bool need_save = true) = 0;
	/// \brief ��������Ϊ�б�
	/// \param uid ��ұ��
	/// \param range ��ϵ��Χ
	/// \param max_num �������
	/// \param max_time_span ���ʱ���(Ϊ0��ʾ��ʱ������)
	/// \param filter_uid ָ���ض�����Ϊ������
	virtual bool SyncRecentFeeds(const char* uid, int range, int max_num,
		double max_time_span, const char* filter_uid) = 0;
	/// \brief �����Ϊ������
	/// \param uid ��ұ��
	/// \param feed_id ����Ϊ���
	/// \param max_num �������
	/// \param max_time_span ���ʱ���(Ϊ0��ʾ��ʱ������)
	virtual bool SyncFeedComments(const char* uid, const char* feed_id, 
		int max_num, double max_time_span) = 0;
	/// \brief �����Ϊ�ķ�����Ϣ����
	/// \param uid ��ұ��
	/// \param feed_id ����Ϊ���
	virtual bool SyncFeedBack(const char* uid, const char* feed_id) = 0;
	/// \brief �ı���Ϊ����
	/// \param feed_id ��Ϊ���
	/// \param desc ��Ϊ����
	/// \param source_uid ��Դ��ұ�ţ����뷢����Ϊ����ң�
	virtual bool SetFeedDesc(const char* feed_id, const wchar_t* desc,
		const char* source_uid) = 0;
	/// \brief ���Ӻ�������	
	/// \param feed_id ��Ϊ���
	/// \param source_uid ��Դ��ұ��
	virtual bool IncFeedGoodCount(const char* feed_id, 
		const char* source_uid) = 0;
	/// \brief ���Ӳ�������	
	/// \param feed_id ��Ϊ���
	/// \param source_uid ��Դ��ұ��
	virtual bool IncFeedBadCount(const char* feed_id, 
		const char* source_uid) = 0;
	/// \brief ��ú�������	
	/// \param feed_id ��Ϊ���
	virtual int GetFeedGoodCount(const char* feed_id) = 0;
	/// \brief ��ò�������	
	/// \param feed_id ��Ϊ���
	virtual int GetFeedBadCount(const char* feed_id) = 0;
	/// \brief ������ۼ���	
	/// \param feed_id ��Ϊ���
	virtual int GetFeedCommentCount(const char* feed_id) = 0;
	
	/// \brief ���Զ�����Ϣ��ָ��������ҵĿͻ���
	/// \param name �������
	/// \param msg ��Ϣ�����б�
	virtual bool CustomByName(const wchar_t* name, const IVarList& msg) = 0;
	/// \brief ���Զ�����Ϣ����Ϸ��������ҵĿͻ���
	/// \param msg ��Ϣ�����б�
	virtual bool CustomByWorld(const IVarList& msg) = 0;
	/// \brief ���Զ�����Ϣ��ָ��Ƶ���ڵĿͻ���
	/// \param channel Ƶ�����
	/// \param msg ��Ϣ�����б�
	virtual bool CustomByChannel(int channel, const IVarList& msg) = 0;

	/// \brief ���ý����Զ�����Ϣ���
	/// \param value �Զ�����Ϣ���
	virtual bool SetTradeCustomId(int value) = 0;
	/// \brief ������Ʒ�����ط�����������Ʒ��������
	/// \param role_name ��ɫ����
	/// \param item_name ָ����Ʒ����
	/// \param item_type ָ����Ʒ����
	/// \param max_num һ�η��͵��ͻ��˵��������
	/// \param start ������ʼ���
	virtual int TradeSearch(const wchar_t* role_name, 
		const wchar_t* item_name, int item_type, int max_num, int start) = 0;
	/// \brief ������Ʒ�����س�����Ʒ�ı�ʶ����
	/// \param seller_name ����������
	/// \param seller_uid �����߱�ʶ��
	/// \param item_type ��Ʒ����
	/// \param item_name ��Ʒ����
	/// \param item_info ��Ʒ��Ϣ
	/// \param price ���ۼ۸�
	/// \param time_len ����ʱ������λ�죩
	virtual const char* TradeSell(const wchar_t* seller_name, 
		const char* seller_uid,	int item_type, const wchar_t* item_name, 
		const wchar_t* item_info, int price, double time_len) = 0;
	/// \brief ȡ�����ۣ��ѳɽ���Ʒ����ȡ����
	/// \param serial_no ������Ʒ��ʶ��
	/// \param seller_name ����������
	/// \param seller_uid �����߱�ʶ��
	virtual bool TradeUnsell(const char* serial_no, 
		const wchar_t* seller_name, const char* seller_uid) = 0;
	/// \brief ����������Ʒ
	/// \param serial_no ������Ʒ��ʶ��
	/// \param buyer_name ����������
	/// \param buyer_uid �����߱�ʶ��
	/// \param price ����۸�
	virtual bool TradeBuy(const char* serial_no, const wchar_t* buyer_name, 
		const char* buyer_uid, int price) = 0;
	/// \brief ��ó�����Ʒ״̬
	/// \param serial_no ������Ʒ��ʶ��
	/// \return 1-����״̬��2-�ɽ�״̬��3-�ѳ�ʱ
	virtual int TradeGetStatus(const char* serial_no) = 0;
	/// \brief ��ó�����Ʒ����
	/// \param serial_no ������Ʒ��ʶ��
	virtual const wchar_t* TradeGetItemName(const char* serial_no) = 0;
	/// \brief ��ó�����Ʒ��Ϣ
	/// \param serial_no ������Ʒ��ʶ��
	virtual const wchar_t* TradeGetItemInfo(const char* serial_no) = 0;
	/// \brief ����Ѿ��ɽ���Ʒ������
	/// \param serial_no ������Ʒ��ʶ��
	/// \param seller_name ����������
	/// \param seller_uid �����߱�ʶ��
	virtual int TradeGainProfit(const char* serial_no, 
		const wchar_t* seller_name, const char* seller_uid) = 0;
	/// \brief ��鳬���������޵���Ʒ
	/// \return ��һ����ʱ����Ʒ��ţ�Ϊ���ַ�����ʾ��û�г�ʱ
	virtual const char* TradeCheckTimeout() = 0;
	
	/// \brief ����Ϣ��ָ������
	/// \param scene_id �������
	/// \param msg ��Ϣ����
	virtual bool SendToScene(int scene_id, const IVarList& msg) = 0;
	
	/// \brief �����ָ�����ֵĳ�����������Ҷ���
	/// \param name �������
	/// \param msg ��Ϣ����
	virtual bool CommandByName(const wchar_t* name, const IVarList& msg) = 0;
};

inline IPubKernel::~IPubKernel() {}

#endif // _SERVER_IPUBKERNEL_H

