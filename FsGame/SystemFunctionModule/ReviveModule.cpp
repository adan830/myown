//--------------------------------------------------------------------
// �ļ���:		ReviveModule.h
// ��  ��:		��������
// ˵  ��:		
//				
// ��������:	2018��03��13��
// ��������:	
// ������:		liumf   
//--------------------------------------------------------------------
#include "Define/ReviveDefine.h"
#include "ReviveModule.h"
#include "CommonModule/ReLoadConfigModule.h"
#include "Define/ClientCustomDefine.h"
#include "Define/CommandDefine.h"
#include "utils/XmlFile.h"
#include "utils/extend_func.h"
#include "utils/string_util.h"
#include "Define/ServerCustomDefine.h"
#include "EnergyModule.h"

#define REVIVE_PANEL_CONFIG_PATH "ini/SystemFunction/Revive/RevivePanel.xml"

ReviveModule* ReviveModule::m_pReviveModule = NULL;

// ��ʼ��
bool ReviveModule::Init(IKernel* pKernel)
{
	m_pReviveModule = this;

	Assert(m_pReviveModule != NULL);


	pKernel->AddEventCallback("player", "OnReady", ReviveModule::OnPlayerReady);          //������߾ͽ���������
	pKernel->AddEventCallback("player", "OnRecover", ReviveModule::OnPlayerRecover);      //��ҵ�¼���ݻָ�
	pKernel->AddEventCallback("player", "OnContinue", ReviveModule::OnPlayerReady);    

	pKernel->AddIntCommandHook("player", COMMAND_BEKILL, ReviveModule::OnCommandBeKill);

	pKernel->AddIntCustomHook("player", CLIENT_CUSTOMMSG_REVIVE, ReviveModule::OnCustomRevive);

	if (!LoadConfig(pKernel))
	{
		return false;
	}

	RELOAD_CONFIG_REG("ReviveManagerConfig", ReviveModule::ReLoadConfig);
	return true;
}

/// \brief ����
int ReviveModule::OnPlayerRecover(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args)
{
	return 0;
}

int ReviveModule::OnPlayerReady(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return 0;
	}

	// ��ҽ��볡��ʱ,��Ϊ����״̬,�ͽ�����
	int nDead = pSelfObj->QueryInt(FIELD_PROP_DEAD);
	if (nDead > 0)
	{
		m_pReviveModule->NearbyRevive(pKernel, self);
	}
	return 0;
}

//���󸴻���Ϣ���� 
int ReviveModule::OnCustomRevive(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& msg)
{
	if (!pKernel->Exists(self) || msg.GetCount() < 2 || msg.GetType(0) != VTYPE_INT || msg.GetType(1) != VTYPE_INT)
	{
		return 0;
	}

	int nSubMsg = msg.IntVal(1);
	switch (nSubMsg)
	{
	case CS_REQUEST_REVIVE:
		m_pReviveModule->OnCustomRequestRevive(pKernel, self, msg);
		break;
	}
	return 0;
}

// ��Ӧ���󸴻�
void ReviveModule::OnCustomRequestRevive(IKernel* pKernel, const PERSISTID& self, const IVarList& msg)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj || msg.GetCount() < 3 || msg.GetType(2) != VTYPE_INT)
	{
		return;
	}

	int nReviveType = msg.IntVal(2);

	// �ж������Ƿ��Ѹ���״̬
	if (pSelfObj->QueryInt("Dead") <= 0)
	{
		return;
	}

	// У�鸴������
	if (!CheckReviveType(pKernel, nReviveType))
	{
		return;
	}

	switch (nReviveType)
	{
	case NEARBY_REVIVE:
		NearbyRevive(pKernel, self);
		break;
	case CITY_REVIVE:
		CityRevive(pKernel, self);
		break;
	case LOCAL_REVIVE:
		LocalRevive(pKernel, self);
		break;
	}
}

// �������
int ReviveModule::OnCommandBeKill(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return 0;
	}

	RevivePanelConfig_s* pReviveData = m_pReviveModule->GetRevivePanelConfig(pKernel);
	if (NULL == pReviveData)
	{
		return 0;
	}

	CVarList msg;
	msg << SERVER_CUSTOMMSG_REVIVE << SC_DEAD_PANEL << pReviveData->m_revivePanel.c_str();
	pKernel->Custom(self, msg);
	return 0;
}

// ��������
bool ReviveModule::LoadConfig(IKernel* pKernel)
{
	if (!LoadRevivePanelConfig(pKernel))
	{
		return false;
	}
	return true;
}

// ���¼���
void ReviveModule::ReLoadConfig(IKernel* pKernel)
{
	m_pReviveModule->LoadConfig(pKernel);
}

// ���ظ����������
bool ReviveModule::LoadRevivePanelConfig(IKernel* pKernel)
{
	// �����ļ�·��
	std::string pathName = pKernel->GetResourcePath();
	pathName += REVIVE_PANEL_CONFIG_PATH;

	CXmlFile xml(pathName.c_str());

	if (!xml.LoadFromFile())
	{
		extend_warning(LOG_ERROR, "[ReviveModule::LoadRevivePanelConfig %s] failed", pathName.c_str());
		return false;
	}

	m_mapRevivePanelConfig.clear();
	// xml��ȡ
	LoopBeginCheck(b);
	for (int i = 0; i < (int)xml.GetSectionCount(); ++i)
	{
		LoopDoCheck(b);
		const char *section = xml.GetSectionByIndex(i);
		if (StringUtil::CharIsNull(section))
		{
			continue;
		}

		RevivePanelConfig_s data;
		data.m_sceneID = StringUtil::StringAsInt(section);
		data.m_revivePanel = xml.ReadString(section, "RevivePanel", "");

		RevivePanelConfigMap::iterator iter = m_mapRevivePanelConfig.find(data.m_sceneID);
		if (iter == m_mapRevivePanelConfig.end())
		{
			m_mapRevivePanelConfig.insert(std::make_pair(data.m_sceneID, data));
		}
	}
	return true;
}

//Ӧ������������
ReviveModule::RevivePanelConfig_s* ReviveModule::GetRevivePanelConfig(IKernel* pKernel)
{
	int nSceneId = pKernel->GetSceneId();
// 	if (pKernel->GetSceneClass() == 2)	 ��������
// 	{
// 		nSceneId = pKernel->GetPrototypeSceneId(nSceneId);
// 	}
	
	RevivePanelConfigMap::iterator  it = m_mapRevivePanelConfig.find(nSceneId);
	if (it == m_mapRevivePanelConfig.end())
	{
		return NULL;
	}

	return &it->second;
}

// ԭ�ظ���
void ReviveModule::LocalRevive(IKernel* pKernel, const PERSISTID& self)
{
	 // �ж�Ԫ���Ƿ��㹻

	PlayerRevive(pKernel, self);
}

// �ͽ�����
void ReviveModule::NearbyRevive(IKernel* pKernel, const PERSISTID& self)
{
	PlayerRevive(pKernel, self);

	//1����ʼ������
	len_t dx = 0.0f;
	len_t dy = 0.0f;
	len_t dz = 0.0f;
	len_t oo = 0.0f;

	pKernel->GetSceneBorn(dx, dy, dz, oo);

	pKernel->MoveTo(self, dx, dy, dz, oo);
}

// ���и���
void ReviveModule::CityRevive(IKernel* pKernel, const PERSISTID& self)
{
	NearbyRevive(pKernel, self);
}

// �������
void ReviveModule::PlayerRevive(IKernel* pKernel, const PERSISTID& self)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return;
	}

	// ���Ϊ����״̬
	if (pSelfObj->QueryInt("Dead") > 0)
	{
		// �������HP
		int64_t nMaxHP = pSelfObj->QueryInt64(FIELD_PROP_MAX_HP);
		pSelfObj->SetInt("Dead", 0);

		EnergyModule::UpdateEnergy(pKernel, self, ENERGY_HP, nMaxHP);
	}
}

// У�鸴������
bool ReviveModule::CheckReviveType(IKernel* pKernel, const int reviveType)
{
	// ��������
	if (reviveType < LOCAL_REVIVE || reviveType >= MAX_REVIVE_NUM)
	{
		return false;
	}

	// ���ø������֤(����������ֱ�ӷ���)
	RevivePanelConfig_s* config = GetRevivePanelConfig(pKernel);
	if (config == NULL)
	{
		return false;
	}

	//3��ѭ��ƥ�临������
	CVarList vList;
	StringUtil::SplitString(vList, config->m_revivePanel, ",");
	int len = static_cast<int>(vList.GetCount());
	bool bCheck = false;
	LoopBeginCheck(b);
	for (int i = 0; i < len; ++i)
	{
		LoopDoCheck(b);
		if (reviveType == vList.IntVal(i))
		{
			bCheck = true;
			break;
		}
	}

	return bCheck;
}