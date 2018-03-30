//--------------------------------------------------------------------
// �ļ���:		IPubCallee.h
// ��  ��:		
// ˵  ��:		
// ��������:	2008��11��12��
// ������:		½����
// ��Ȩ����:	������ţ�������޹�˾
//--------------------------------------------------------------------

#ifndef _SERVER_IPUBCALLEE_H
#define _SERVER_IPUBCALLEE_H

#include "../public/Macros.h"
#include "../public/IVarList.h"

// �������ݻص��ӿ�

class IPubKernel;

class IPubCallee
{
public:
	virtual ~IPubCallee() = 0;

	/// \brief �������ݷ���������
	/// \param pPubKernel ����ָ�룬��ͬ
	virtual int OnPublicStart(IPubKernel* pPubKernel) = 0;

#ifndef _DEBUG
	/// \brief �����������
	/// \param space_name �������ݿռ���
	/// \param data_name ������������,�߼������ٴ˻ص���ͳһ������ԣ���
	virtual int OnPubDataCreate(IPubKernel* pPubKernel, 
		const char* space_name, const wchar_t* data_name) = 0;
#endif

	/// \brief ����������������
	/// \param space_name �������ݿռ���
	/// \param data_name ������������
	virtual int OnPubDataLoaded(IPubKernel* pPubKernel, 
		const char* space_name, const wchar_t* data_name) = 0;

	/// \brief �������ݿռ��е�������������
	/// \param space_name �������ݿռ���
	virtual int OnPubSpaceLoaded(IPubKernel* pPubKernel, 
		const char* space_name) = 0;

	/// \brief ���յ���������������Ϣ
	/// \param source_id ����Ϣ�ķ�������ʶ
	/// \param scene_id ����Ϣ�ĳ�����ʶ
	/// \param msg ��Ϣ����
	virtual int OnPublicMessage(IPubKernel* pPubKernel, int source_id, 
		int scene_id, const IVarList& msg) = 0;

	/// \brief ������ÿ֡����
	/// \param frame_ticks ��֡�ĺ�����
	virtual int OnFrameExecute(IPubKernel* pPubKernel, int frame_ticks) = 0;

	/// \brief ���յ���������״̬�����������Ϣ
	/// \param world_state ��������״̬
	/// \param max_players �����������
	/// \param player_count �������
	/// \param online_count �����������
	/// \param offline_count �����������
	/// \param queue_count �����Ŷӵ��������
	virtual int OnServerState(IPubKernel* pPubKernel,
		const char* world_state, int max_players, int player_count, 
		int online_count, int offline_count, int queue_count) = 0;

	/// \brief ���յ����Լ��ص���Ϸ�����߽�ɫ��Ϣ
	/// \param count ��ɫ����
	/// \param info ��ɫ��Ϣ���У�string account, wstring role_name, double resume_time, string resume_info��
	virtual int OnRoleOffline(IPubKernel* pPubKernel, int count,
		const IVarList& info) = 0;

	/// \brief �������ر�֪ͨ
	virtual int OnServerClose(IPubKernel* pPubKernel) = 0;
};

inline IPubCallee::~IPubCallee() {}

#endif // _SERVER_IPUBCALLEE_H

