//--------------------------------------------------------------------
// �ļ���:		IAuctionCallee.h
// ��  ��:		
// ˵  ��:		
// ��������:	2008��11��12��
// ������:		½����
// ��Ȩ����:	������ţ�������޹�˾
//--------------------------------------------------------------------

#ifndef _SERVER_IAUCTIONCALLEE_H
#define _SERVER_IAUCTIONCALLEE_H

#include "../public/Macros.h"
#include "../public/IVarList.h"

// ���������ݻص��ӿ�

class IAuctionKernel;

class IAuctionCallee
{
public:
	virtual ~IAuctionCallee() = 0;

	/// \brief ���������ݷ���������
	/// \param pAuctionKernel ����ָ�룬��ͬ
	virtual int OnAuctionStart(IAuctionKernel* pAuctionKernel) = 0;

	/// \brief ������������������
	virtual int OnAuctionAttrLoaded(IAuctionKernel* pAuctionKernel, const wchar_t * auction_data) = 0;

	/// \brief ������������������
	virtual int OnAuctionDataLoaded(IAuctionKernel* pAuctionKernel) = 0;

	/// \brief ���յ���������������Ϣ
	/// \param source_id ����Ϣ�ķ�������ʶ
	/// \param scene_id ����Ϣ�ĳ�����ʶ
	/// \param msg ��Ϣ����
	virtual int OnAuctionMessage(IAuctionKernel* pAuctionKernel, int source_id, 
		int scene_id, const IVarList& msg) = 0;

	/// \brief ������ÿ֡����
	/// \param frame_ticks ��֡�ĺ�����
	virtual int OnFrameExecute(IAuctionKernel* pAuctionKernel, int frame_ticks) = 0;

	/// \brief ���յ���������״̬�����������Ϣ
	/// \param world_state ��������״̬
	/// \param max_players �����������
	/// \param player_count �������
	/// \param online_count �����������
	/// \param offline_count �����������
	/// \param queue_count �����Ŷӵ��������
	virtual int OnServerState(IAuctionKernel* pAuctionKernel,
		const char* world_state, int max_players, int player_count, 
		int online_count, int offline_count, int queue_count) = 0;

	/// \brief ���յ����Լ��ص���Ϸ�����߽�ɫ��Ϣ
	/// \param count ��ɫ����
	/// \param info ��ɫ��Ϣ���У�string account, wstring role_name, double resume_time, string resume_info��
	virtual int OnRoleOffline(IAuctionKernel* pAuctionKernel, int count,
		const IVarList& info) = 0;

	/// \brief �������ر�֪ͨ
	virtual int OnServerClose(IAuctionKernel* pAuctionKernel) = 0;
};

inline IAuctionCallee::~IAuctionCallee() {}

#endif // _SERVER_IAUCTIONCALLEE_H

