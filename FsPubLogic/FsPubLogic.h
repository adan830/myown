//--------------------------------------------------------------------
// �ļ���:		FsPubLogic.h
// ��  ��:		�������ݷ������߼�ģ��
// ˵  ��:		
// ��������:	2014��06��10��
// ��������:	2014��06��10��
// ������:		  ( )
// �޸���:     
//    :	    
//--------------------------------------------------------------------

#ifndef __FsPubLogic_h__
#define __FsPubLogic_h__

#include "../server/IPubCallee.h"
#include "../server/PubLogicDll.h"
#include <vector>
#include "Public/IPubLogic.h"

// �������ݻص��ӿ�

extern std::vector<IPubLogic*> g_vecLogic;

class pub_callee: public IPubCallee
{
public:
	//����
	pub_callee();
	//����
	~pub_callee();

	/// \brief �������ݷ���������
	/// \param pPubKernel ����ָ�룬��ͬ
	virtual int OnPublicStart(IPubKernel* pPubKernel);

	/// \brief �����������
	/// \param space_name �������ݿռ���
	/// \param data_name ������������,�߼������ٴ˻ص���ͳһ������ԣ���
	virtual int OnPubDataCreate(IPubKernel* pPubKernel, 
		const char* space_name, const wchar_t* data_name);

	/// \brief ����������������
	/// \param space_name �������ݿռ���
	/// \param data_name ������������
	virtual int OnPubDataLoaded(IPubKernel* pPubKernel, 
		const char* space_name,
		const wchar_t* data_name);

	/// \brief �������ݿռ��е�������������
	/// \param space_name �������ݿռ���
	virtual int OnPubSpaceLoaded(IPubKernel* pPubKernel, 
		const char* space_name);

	/// \brief ���յ���������������Ϣ
	/// \param source_id ����Ϣ�ķ�������ʶ
	/// \param msg ��Ϣ����
	virtual int OnPublicMessage(IPubKernel* pPubKernel,
		int source_id, 
		int scene_id, 
		const IVarList& msg);

	// ������ÿ֡����
	virtual int OnFrameExecute(IPubKernel* pPubKernel,
		int frame_ticks); 

	// ���յ���������״̬�����������Ϣ
	virtual int OnServerState(IPubKernel* pPubKernel,
		const char* world_state,
		int max_players,
		int player_count, 
		int online_count,
		int offline_count,
		int queue_count); 

	// ���յ����Լ��ص���Ϸ�����߽�ɫ��Ϣ
	virtual int OnRoleOffline(IPubKernel* pPubKernel,
		int count,
		const IVarList& info); 

	/// \brief �������ر�֪ͨ
	virtual int OnServerClose(IPubKernel* pPubKernel);

	//����Ϸ�������Ƿ����
	bool IsServerReady(){
		return m_bServerReady;
	}
public:
	// for command
	bool SendPublicCommand(IPubKernel* pPubKernel, int cmd_id, const IVarList& msg);
public:

	static pub_callee * pInstance;

private:
	// ����ͨ����Ϣ
	bool ProcessCommonMessage(IPubKernel* pPubKernel,
		int source_id, 
		int scene_id, 
		const IVarList& msg);

	//�޸����ԣ���ʽ��string pubspace, wstring pubdata, int msgid, string prop, var value, ...;
	bool SetPubDataProp(IPubKernel* pPubKernel, 
		IPubData* pPubData, 
		int source_id,
		int scene_id, 
		const IVarList& msg);
	//�������ԣ���ʽ��string pubspace, wstring pubdata, int msgid, string prop, int value, ...;
	bool IncPubDataProp(IPubKernel* pPubKernel,
		IPubData* pPubData, 
		int source_id, 
		int scene_id, 
		const IVarList& msg);

	//����ĳ���������ʽ��string pubspace, wstring pubdata, int msgid, string rec, int maxrow, int cols, ...;
	bool AddPubDataRec(IPubKernel* pPubKernel,
		IPubData* pPubData,
		int source_id,
		int scene_id,
		const IVarList& msg);

	//���ĳ��������ݣ������ʽ��string pubspace, wstring pubdata, int msgid, string rec, ...;
	bool ClearPubDataRec(IPubKernel* pPubKernel,
		IPubData* pPubData, 
		int source_id,
		int scene_id, 
		const IVarList& msg);
	//ɾ��ĳ�����ĳ�����ݣ������ʽ��string pubspace, wstring pubdata, int msgid, string rec, int row, ...;
	bool RemovePubDataRecRow(IPubKernel* pPubKernel, 
		IPubData* pPubData, 
		int source_id, 
		int scene_id, 
		const IVarList& msg);
	//���ĳ�����һ�����ݣ������ʽ��string pubspace, wstring pubdata, int msgid, string rec, ...;
	bool AddPubDataRecRowValue(IPubKernel* pPubKernel,
		IPubData* pPubData, 
		int source_id, 
		int scene_id, 
		const IVarList& msg);

	//����ĳ�����ĳһ��ĳһ�е����ݣ������ʽ��string pubspace, wstring pubdata, int msgid, string rec, int row, int col, var value, ...;
	bool SetPubDataRecValue(IPubKernel* pPubKernel,
		IPubData* pPubData, 
		int source_id,
		int scene_id, 
		const IVarList& msg);
	//����ĳ�����ĳһ��ĳһ�е����ݣ������ʽ��string pubspace, wstring pubdata, int msgid, string rec, int row, int col, var value, ...;
	bool IncPubDataRecValue(IPubKernel* pPubKernel, 
		IPubData* pPubData, 
		int source_id, 
		int scene_id, 
		const IVarList& msg);

	//���ݹؼ�ֵɾ��ĳ�����ĳ�����ݣ������ʽ��string pubspace, wstring pubdata, int msgid, string rec, int keycol, var keyvalue;
	bool RemovePubDataRecRowByKey(IPubKernel* pPubKernel, 
		IPubData* pPubData, 
		int source_id,
		int scene_id, 
		const IVarList& msg);
	//���ݹؽ�ֵ���ù����ĳ�����ĳһ��ĳһ�е����ݣ������ʽ��string pubspace, wstring pubdata, int msgid, string rec, int keycol, var keyvalue, int col, var value, ...;
	bool SetPubDataRecValueByKey(IPubKernel* pPubKernel,
		IPubData* pPubData, 
		int source_id, 
		int scene_id, 
		const IVarList& msg);
	//���ݹؽ����ӹ����ĳ�����ĳһ��ĳһ�е����ݣ������ʽ��string pubspace, wstring pubdata, int msgid, string rec, int keycol, var keyvalue, int col, var value, ...;
	bool IncPubDataRecValueByKey(IPubKernel* pPubKernel,
		IPubData* pPubData, 
		int source_id, 
		int scene_id, 
		const IVarList& msg);
	

private:
	//�������ã������ʽ��string pubspace, wstring pubdata, int msgid
	bool UpdateConfig(IPubKernel* pPubKernel, 			 
		IPubData* pPubData,		 
		int source_id,	 
		int scene_id, 		 
		const IVarList& msg);
private:
	bool m_bServerReady;		// ����Ϸ�������Ƿ����
	int m_nMaxPlayers;			// ������ҷ�������������
	int m_nPlayerCount;			// ���������
	int m_nOnlineCount;			// �����������
	int m_nOfflineCount;		// �����������
	int m_nQueueCount;			// �����Ŷӵ��������
	float m_fGetOfflineTimer;	// ��ʱ��ȡ���߽�ɫ�ļ�����

};

//��������Ѱ�ҷ��๫�������߼�ģ�飨Ŀǰ���У�"domain", "guild"��
inline IPubLogic * FindPubLogic(const char * pub_name)
{

    LoopBeginCheck(a);
	for (size_t i = 0; i < g_vecLogic.size(); i++)
	{
        LoopDoCheck(a);

		if (strcmp(pub_name, g_vecLogic[i]->GetName()) == 0)
		{
			return g_vecLogic[i];
		}
	}

	return NULL;
}

#endif //__FsPubLogic_h__
