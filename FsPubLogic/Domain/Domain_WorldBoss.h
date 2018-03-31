//--------------------------------------------------------------------
// �ļ���:      Domain_WorldBoss.h
// ��  ��:      ����BOSS
// ˵  ��:		
// ��������:    2015��7��6��
// ������:      ������
// �޸���:        
// ��Ȩ����:    
//--------------------------------------------------------------------

#ifndef __Domain_WorldBoss_h__
#define __Domain_WorldBoss_h__

#include "IDomainData.h"

#include <map>

class Domain_WorldBoss : public IDomainData
{
public:
	Domain_WorldBoss();
public:
	virtual const std::wstring& GetDomainName(IPubKernel* pPubKernel);
	virtual std::wstring GetName(IPubKernel* pPubKernel){return GetDomainName(pPubKernel);}

	virtual bool GetSave();
	virtual int OnCreate(IPubKernel * pPubKernel, IPubSpace * pPubSpace); 
	virtual int OnLoad(IPubKernel * pPubKernel, IPubSpace * pPubSpace);
private:
	///  \brief ���յ���������������Ϣ
	///  \param source_id ����Ϣ�ķ�������ʶ
	///  \param msg ��Ϣ����
	virtual int OnMessage(IPubKernel * pPubKernel,
		IPubSpace * pPubSpace,
		int source_id,
		int scene_id,
		const IVarList & msg);


	// ��¼����boss�˺����а�
	void UpdateBossRecInfo(IPubData* pWorldBossData, const IVarList & args);
private:
	static Domain_WorldBoss* m_pDomainWorldBoss;
};

#endif
