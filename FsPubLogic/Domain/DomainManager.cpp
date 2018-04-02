//--------------------------------------------------------------------
// �ļ���:		DomainManager.cpp
// ��  ��:		�������ݷ������߼�ģ��:ȫ�ֹ������ݵĹ���
// ˵  ��:		
// ��������:	2014��06��10��
// ��������:	2014��06��10��
// ������:		  ( )
// �޸���:     
//    :	    
//--------------------------------------------------------------------

#include "DomainManager.h"
#include "DomainRankList.h"
#include "EnvirValue.h"
#include "Domain_Task.h"
#include "Domain_SceneList.h"
#include "Domain_Look.h"
#include "Domain_Friend.h"
#include "Domain_Team.h"
#include "Domain_Boss.h"
#include "Domain_WorldBoss.h"
#include "Domain_Teamoffer.h"

DomainManager* DomainManager::m_pInstance = NULL;

DomainManager::DomainManager()
{
}

DomainManager::~DomainManager()
{
	MapData::iterator it = m_mapDataList.begin();

    LoopBeginCheck(a);
	for (; it != m_mapDataList.end(); ++it)
	{
        LoopDoCheck(a);

		delete it->second;
		it->second = 0;
	}

	m_mapDataList.clear();
}

IPubLogic* DomainManager::Instance()
{
	if (m_pInstance == NULL)
	{
		m_pInstance = NEW DomainManager;
	}

	return m_pInstance;
}

/// \brief �������ݷ���������
/// \param pPubKernel ����ָ�룬��ͬ
int DomainManager::OnPublicStart(IPubKernel* pPubKernel)
{
	IDomainData* pDomainData = NULL;
	// ���һ�����ݿռ�
	pDomainData = NEW Domain_SceneList();
	m_mapDataList[pDomainData->GetDomainName(pPubKernel)] = pDomainData;

    pDomainData = NEW DomainRankList();
    m_mapDataList[pDomainData->GetDomainName(pPubKernel)] = pDomainData;

	pDomainData = NEW Domain_Task();
	m_mapDataList[pDomainData->GetDomainName(pPubKernel)] = pDomainData;

	pDomainData = NEW Domain_Look();
	m_mapDataList[pDomainData->GetDomainName(pPubKernel)] = pDomainData;

	pDomainData = NEW Domain_Friend();
	m_mapDataList[pDomainData->GetDomainName(pPubKernel)] = pDomainData;

	pDomainData = NEW Domain_Team();
	m_mapDataList[pDomainData->GetDomainName(pPubKernel)] = pDomainData;

	pDomainData = NEW Domain_Boss();
	m_mapDataList[pDomainData->GetDomainName(pPubKernel)] = pDomainData;

	pDomainData = NEW Domain_WorldBoss();
	m_mapDataList[pDomainData->GetDomainName(pPubKernel)] = pDomainData;

	pDomainData = NEW Domain_Teamoffer();
	m_mapDataList[pDomainData->GetDomainName(pPubKernel)] = pDomainData;

	EnvirValue::Init(pPubKernel);
	return 1;
}

/// \brief �������ݷ������ر�
/// \param pPubKernel ����ָ�룬��ͬ
int DomainManager::OnServerClose(IPubKernel* pPubKernel)
{
	// ���������������ݵ�space
	pPubKernel->SavePubSpace(GetName());

	return 1;
}

/// \brief ����������������
/// \param data_name ������������
int DomainManager::OnPubDataLoaded(IPubKernel* pPubKernel, const wchar_t * data_name)
{
	IPubSpace * pPubSpace = pPubKernel->GetPubSpace(GetName());
	if (pPubSpace == NULL)
	{
		Assert(false);

		return 1;
	}

	MapData::iterator it = m_mapDataList.find(data_name);
	if (it != m_mapDataList.end())
	{
		it->second->OnLoad(pPubKernel, pPubSpace);
	}

	return 1;
}

/// \brief �������ݿռ��е�������������
/// \param pPubKernel ����ָ�룬��ͬ
int DomainManager::OnPubSpaceLoaded(IPubKernel* pPubKernel)
{
	IPubSpace * pPubSpace = pPubKernel->GetPubSpace(GetName());
	if (pPubSpace == NULL)
	{
		Assert(false);

		return 1;
	}

	MapData::iterator it = m_mapDataList.begin();

    LoopBeginCheck(b);
	for (; it != m_mapDataList.end(); ++it)
	{
        LoopDoCheck(b);

		IDomainData * pDomainData = it->second;
		if (!pPubSpace->FindPubData(pDomainData->GetDomainName(pPubKernel).c_str()))
		{
			bool res = pPubSpace->AddPubData(pDomainData->GetDomainName(pPubKernel).c_str(), pDomainData->GetSave());
			if (res)
			{
				pDomainData->OnCreate(pPubKernel, pPubSpace);
			}
		}
	}

	return 1;
}

/// \brief ���յ���������������Ϣ
/// \param source_id ����Ϣ�ķ�������ʶ
/// \param msg ��Ϣ����
int DomainManager::OnPublicMessage(IPubKernel* pPubKernel, int source_id, 
								   int scene_id, const IVarList& msg)
{
	IPubSpace * pPubSpace = pPubKernel->GetPubSpace(GetName());
	if (pPubSpace == NULL)
	{
		Assert(false);

		return 1;
	}

	const wchar_t * data_name = msg.WideStrVal(1);
	MapData::iterator it = m_mapDataList.find(data_name);
	if (it != m_mapDataList.end())
	{
		it->second->OnMessage(pPubKernel, pPubSpace, source_id, scene_id, msg);
	}

	return 1;
}

// ��������
bool DomainManager::OnUpdateConfig(IPubKernel* pPubKernel, int source_id, 
								 int scene_id, const IVarList& msg)
{
	IPubSpace * pPubSpace = pPubKernel->GetPubSpace(GetName());
	if (NULL == pPubSpace)
	{
		Assert(false);

		return false;
	}

	const wchar_t * data_name = msg.WideStrVal(1);
	MapData::iterator it = m_mapDataList.find(data_name);
	if (it != m_mapDataList.end())
	{
		return it->second->OnUpdateConfig(pPubKernel, pPubSpace, source_id, scene_id, msg);
	}

	return false;
}

/// \brief ���ָ�����Ƶ�DomainData
/// \param data_name ����
/// \param msg ��Ϣ����
IDomainData * DomainManager::FindDomainData(const wchar_t * data_name)
{
	MapData::iterator it = m_mapDataList.find(data_name);
	if (it != m_mapDataList.end())
	{
		return it->second;
	}
	else
	{
		return NULL;
	}
}
bool DomainManager::OnPublicCommand(IPubKernel* pPubKernel, int cmd_id, const IVarList& args)
{
	IPubSpace * pPubSpace = pPubKernel->GetPubSpace(GetName());
	if (pPubSpace == NULL)
	{
		Assert(false);

		return 1;
	}

	MapData::iterator it = m_mapDataList.begin();

	LoopBeginCheck(b);
	for (; it != m_mapDataList.end(); ++it)
	{
		LoopDoCheck(b);

		it->second->OnPublicCommand(pPubKernel, cmd_id, args);
	}
	return true;
}
