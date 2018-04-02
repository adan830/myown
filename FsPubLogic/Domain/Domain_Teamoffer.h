//----------------------------------------------------------
// �ļ���:      Domain_Teamoffer.h
// ��  ��:      ��Ӽ���ϵͳ
// ˵  ��:
// ��������:    2018��04��02��
// ������:      tzt 
// �޸���:    
//    :       
//----------------------------------------------------------

#ifndef __Domain_Teamoffer_H__
#define __Domain_Teamoffer_H__

#include "IDomainData.h"

class Domain_Teamoffer : public IDomainData
{
public:
	Domain_Teamoffer() : IDomainData(L"", 0)
	{} 

	virtual const std::wstring & GetDomainName(IPubKernel * pPubKernel);

	//����
	virtual std::wstring GetName(IPubKernel* pPubKernel){return GetDomainName(pPubKernel);}

	//�Ƿ񱣴浽���ݿ�
	virtual bool GetSave(){return true;}

	virtual int OnCreate(IPubKernel* pPubKernel, IPubSpace * pPubSpace);

	virtual int OnLoad(IPubKernel* pPubKernel, IPubSpace * pPubSpace);
private:
	// ���յ���������������Ϣ
	virtual int OnMessage(IPubKernel* pPubKernel, IPubSpace * pPubSpace, 
		int source_id, int scene_id, const IVarList& msg);

	// ������ͽ���
	void CreateTeamOfferRec(IPubKernel* pKernel, IPubData* pPubData);

	// ���������Ϣ����
	void OnTeamOfferMessage(IPubKernel* pPubKernel, IPubSpace * pPubSpace, 
		int source_id, int scene_id, const IVarList& msg);

	// ������ͷ���
	void OnLaunch(IPubKernel* pPubKernel, IPubData* pPubData, 
		const IVarList& msg, int source_id, int scene_id);

	// ȡ�������������
	void OnStop(IPubKernel* pPubKernel, IPubData* pPubData,
		const IVarList& msg, int source_id, int scene_id);

	// ������Ϳ�ʼ
	void OnStart(IPubKernel* pPubKernel, IPubSpace * pPubSpace,
		const IVarList& msg, int source_id, int scene_id);

	// ���������Ч��֤
	void OnCheck(IPubKernel* pPubKernel, IPubSpace * pPubSpace,
		const IVarList& msg, int source_id, int scene_id);

	// ������ͽ��
	void OnResult(IPubKernel* pPubKernel, IPubSpace * pPubSpace,
		const IVarList& msg, int source_id, int scene_id);

	// ����һ���������
	void OnReset(IPubKernel* pPubKernel, IPubSpace * pPubSpace,
		const IVarList& msg, int source_id, int scene_id);

	// ���������͵�������
	void SendP2SMsg(IPubKernel* pPubKernel, int source_id, 
		int scene_id, const IVarList& sub_p2s_msg);

private:
	static Domain_Teamoffer* m_pDomain_Teamoffer;
};
#endif