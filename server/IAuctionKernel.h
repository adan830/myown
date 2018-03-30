//--------------------------------------------------------------------
// �ļ���:		IAuctionKernel.h
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
#include "../server/IAuctionData.h"

// �����������߼��ӿڰ汾��
#define AUCTION_LOGIC_MODULE_VERSION 0x3002

class IAuctionKernel;

// ��ʱ������
typedef int (__cdecl* AUCTION_TIMER_FUNC)(IAuctionKernel* pPubKernel, const wchar_t* data_name, int time) ;
	
// ���������ݺ��Ľӿ�

class IAuctionKernel
{
public:
	virtual ~IAuctionKernel() = 0;
	
	/// \brief ����������
	/// \param name ��������������
	virtual bool AddAuctionData(const wchar_t* name, bool save) = 0;
	/// \brief ɾ������������
	/// \param name ������������
	virtual bool RemoveAuctionData(const wchar_t* name) = 0;
	/// \brief ��������������
	/// \param name ��������������
	virtual bool FindAuctionData(const wchar_t* name) = 0;
	/// \brief �����������������
	virtual int GetAuctionDataCount() = 0;
	/// \brief ������������������б�
	/// \param result ���������������б�
	virtual int GetAuctionDataList(IVarList& result) = 0;
	/// \brief ���ָ�����Ƶ�����������
	/// \param name ������������
	virtual IAuctionData* GetAuctionData(const wchar_t* name) = 0;
	/// \brief ��������������
	/// \param name ����������
	virtual bool SaveAuctionData(const wchar_t* name) = 0;
		
	/// \brief ����Ϣ������������
	/// \param source_id ��������ʶ��Ϊ0��ʾ0�ų���������
	/// \param scene_id ������ţ�Ϊ0��ʾ��һ������
	/// \param msg ��Ϣ����
	virtual bool SendAuctionMessage(int source_id, int scene_id, 
		const IVarList& msg) = 0;
	
	/// \brief ���Ҷ�ʱ��
	/// \param name ��ʱ������
	/// \param data_name ��������������
	virtual bool FindTimer(const char* name, const wchar_t* data_name) = 0;
	/// \brief ��Ӷ�ʱ��
	/// \param name ��ʱ������
	/// \param func ��ʱ������ָ��
	/// \param data_name ��������������
	/// \param time ��ʱʱ��
	/// \param count ��ʱ����
	virtual bool AddTimer(const char* name, AUCTION_TIMER_FUNC func, 
		const wchar_t* data_name, int time, int count) = 0;
	/// \brief ɾ����ʱ��
	/// \param name ��ʱ������
	/// \param data_name ��������������
	virtual bool RemoveTimer(const char* name, const wchar_t* data_name) = 0;
	
	/// \brief ��������������״̬�����������Ϣ
	virtual bool RequestServerState() = 0;
		
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

	/// \brief ����Ϣ��ָ������
	/// \param scene_id �������
	/// \param msg ��Ϣ����
	virtual bool SendToScene(int scene_id, const IVarList& msg) = 0;
	
	/// \brief �����ָ�����ֵĳ�����������Ҷ���
	/// \param name �������
	/// \param msg ��Ϣ����
	virtual bool CommandByName(const wchar_t* name, const IVarList& msg) = 0;
	
	/// \brief ��ȡWorld��ͳһʱ��
	virtual __time64_t GetWorldTime_64( void ) = 0;
	virtual time_t GetWorldTime( void ) = 0;
};

inline IAuctionKernel::~IAuctionKernel() {}

#endif // _SERVER_IPUBKERNEL_H

