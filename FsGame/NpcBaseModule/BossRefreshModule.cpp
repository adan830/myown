//--------------------------------------------------------------------
// �ļ���:      BossRefreshModule.h
// ��  ��:      Bossˢ��ģ��
// ˵  ��:		
// ��������:    2016��6��28��
// ������:      ������
// �޸���:        
//    :       
//--------------------------------------------------------------------

#include "BossRefreshModule.h"
#include "utils/util_func.h"
#include "utils/XmlFile.h"
#include "time.h"
#include "FsGame/Define/CommandDefine.h"
#include "FsGame/Define/GameDefine.h"
#include "FsGame/Define/PubDefine.h"
#include "FsGame/Define/ClientCustomDefine.h"
#include "FsGame/Define/ServerCustomDefine.h"
//#include "FsGame/Define/MapDefine.h"
#include "FsGame/Define/StaticDataDefine.h"
#include "utils/string_util.h"
#include "utils/util.h"
#include "FsGame/NpcBaseModule/AI/AIDefine.h"
#include "FsGame/NpcBaseModule/NpcCreatorModule.h"
//#include "FsGame/SceneBaseModule/SceneInfoModule.h"
#include "FsGame/CommonModule/AsynCtrlModule.h"
#include "FsGame/SystemFunctionModule/StaticDataQueryModule.h"
#include "CommonModule/EnvirValueModule.h"
#include "Define/ModifyPackDefine.h"
#include "CommonModule/PropRefreshModule.h"
#include "CommonModule/ReLoadConfigModule.h"
#include "Define/FightNpcDefine.h"

std::wstring	BossRefreshModule::m_domainName = L"";
BossRefreshModule * BossRefreshModule::m_pBossRefreshModule = NULL;
SceneInfoModule * BossRefreshModule::m_pSceneInfoModule = NULL;
NpcCreatorModule * BossRefreshModule::m_pNpcCreatorModule = NULL;
AsynCtrlModule * BossRefreshModule::m_pAsynCtrlModule = NULL;
const char* const BOSS_REFRESH_INFO = "ini/npc/BossRefresh/SceneBoss.xml";

// ��ʼ��
bool BossRefreshModule::Init(IKernel* pKernel)
{
    m_pBossRefreshModule = this;
    m_pSceneInfoModule = (SceneInfoModule*)pKernel->GetLogicModule("SceneInfoModule");
    m_pNpcCreatorModule = (NpcCreatorModule*)pKernel->GetLogicModule("NpcCreatorModule");
    m_pAsynCtrlModule = (AsynCtrlModule*)pKernel->GetLogicModule("AsynCtrlModule");

    Assert(m_pBossRefreshModule != NULL && m_pSceneInfoModule != NULL 
        && m_pNpcCreatorModule != NULL && m_pAsynCtrlModule != NULL);

    pKernel->AddEventCallback("scene", "OnCreate", BossRefreshModule::OnCreate);
    pKernel->AddEventCallback("NormalNpc", "OnEntry", OnBossEntry);

    pKernel->AddIntCommandHook("BossNpc", COMMAND_BEKILL, OnCommandBossBeKilled);
    pKernel->AddIntCustomHook("player", CLIENT_CUSTOMMSG_QUERY_BOSS_INFO, OnCustomQuerySceneBoss);

    LoadResource(pKernel);
    DECL_HEARTBEAT(BossRefreshModule::HB_BossCreate);

	RELOAD_CONFIG_REG("BossRefreshConfig", BossRefreshModule::ReloadConfig);
    return true;
}

// �ͷ�
bool BossRefreshModule::Shut(IKernel* pKernel)
{
    return true;
}

// �Ƿ�Ϊˢboss����
bool BossRefreshModule::IsRefreshBossScene(IKernel* pKernel)
{
	int nSceneId = pKernel->GetSceneId();
	map<int, SceneRefreshInfo>::iterator it = m_pBossRefreshModule->m_MapSceneBossInfo.find(nSceneId);

	return it != m_pBossRefreshModule->m_MapSceneBossInfo.end();
}

//��������
int BossRefreshModule::OnCreate(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args)
{
    int iSceneID = pKernel->GetSceneId();
    map<int, SceneRefreshInfo>::iterator it = m_pBossRefreshModule->m_MapSceneBossInfo.find(iSceneID);
    if (it == m_pBossRefreshModule->m_MapSceneBossInfo.end())
    {
        return 0;
    }

	IGameObj* pSceneObj = pKernel->GetGameObj(self);
	if (NULL == pSceneObj)
	{
		return 0;
	}

	IRecord* pBossRecord = pSceneObj->GetRecord(SCENE_BOSS_REC);
	if (pBossRecord == NULL)
	{
		return 0;
	}

	if (pBossRecord->GetRows() > 0)
	{
		return 0;
	}

	vector<BossInfo>& vecBoss = it->second.VecSceneBossInfo;
	int nSize = (int)vecBoss.size();
	LoopBeginCheck(e);
	for (int i = 0;i < nSize;++i)
	{
		LoopDoCheck(e);
		pBossRecord->AddRowValue(-1, CVarList() << vecBoss[i].sBossConfig.c_str() << PERSISTID() << 0.0f << 0.0f << 0.0f);
	}

    //û�����������Ӽ������
    ADD_HEART_BEAT(pKernel, pKernel->GetScene(), "BossRefreshModule::HB_BossCreate", 5*1000);
    return 0;
}

//����BOSS����
int BossRefreshModule::HB_BossCreate(IKernel* pKernel, const PERSISTID& self, int silce)
{
    IGameObj* pSceneObj = pKernel->GetGameObj(self);
    if (NULL == pSceneObj)
    {
        return 0;
    }
    //�������򲻴���
    int iSceneID = pKernel->GetSceneId();
    std::map<int, SceneRefreshInfo>::iterator iter = m_pBossRefreshModule->m_MapSceneBossInfo.find(iSceneID);
    if (iter == m_pBossRefreshModule->m_MapSceneBossInfo.end())
    {
        return 0;
    }
    SceneRefreshInfo& sRefreshInfo = iter->second;
	// �Ƿ���Ҫˢboss
	if (sRefreshInfo.nOpen == 1 && m_pBossRefreshModule->NotRefresh(sRefreshInfo.beginTime, sRefreshInfo.endTime))
	{
		return 0;
	}
	//�Ƿ����BOSS��
    IRecord* pSceneBossRec = pSceneObj->GetRecord(SCENE_BOSS_REC);
    if (pSceneBossRec == NULL)
    {
        return 0;
    } 
	// ��ȡ������boss��¼��
	IRecord* pBossPubRec = m_pBossRefreshModule->GetPubBossInSceneRec(pKernel, SCENE_BOSS_TOTAL_REC);
	if (NULL == pBossPubRec)
	{
		return 0;
	}
	int nRecRows = pSceneBossRec->GetRows();
	LoopBeginCheck(q);
	for (int i = 0;i < nRecRows;++i)
	{
		LoopDoCheck(q);
		const char* strBossConfig = pSceneBossRec->QueryString(i, SCENE_BOSS_CONFIG);
		int nPubRowIndex = pBossPubRec->FindString(SCENE_BOSS_INFO_CONFIG, strBossConfig);
		// û�ҵ�˵���տ���,����ˢ��boss
		if (-1 == nPubRowIndex)
		{
			m_pBossRefreshModule->CreateBoss(pKernel, strBossConfig, pSceneBossRec, i);
			continue;
		}
		int64_t nDeadTime = pBossPubRec->QueryInt64(nPubRowIndex, SCENE_BOSS_DEAD_TIME);
		if (nDeadTime == BS_BOSS_BORN)
		{
			continue;
		}
		int64_t nCurTime = ::time(NULL);
		// ��ʱ��ˢboss
		int nInterval = m_pBossRefreshModule->QueryBossInterval(sRefreshInfo, strBossConfig);
		if (0 == nInterval)
		{
			continue;
		}
		if (nCurTime - nDeadTime > nInterval)
		{
			m_pBossRefreshModule->CreateBoss(pKernel, strBossConfig, pSceneBossRec, i);
		}
	}
    return 0;
}

//��Դ����
bool BossRefreshModule::LoadResource(IKernel* pKernel)
{
	if (!LoadSceneBoss(pKernel))
	{
		return false;
	}

	return true;
}

//����������
int BossRefreshModule::GetRandomPos(IKernel* pKernel, const vector<PosInfo>& VecBossInfo, float& fBornX, float& fBornZ, float& fOrient)
{
    IGameObj* pSceneObj = pKernel->GetSceneObj();
    if (NULL == pSceneObj)
    {
        return 0;
    }
    IRecord* record = pSceneObj->GetRecord(SCENE_BOSS_REC);
    if (NULL == record)
    {
        return 0;
    }
    int size = (int)VecBossInfo.size();
	if (VecBossInfo.empty())
    {
        return 0;
    }

	int random = util_random_int(size);

	fBornX = VecBossInfo[random].PosX;
	fBornZ = VecBossInfo[random].PosZ;
	fOrient = VecBossInfo[random].Orient;
    return 0;
}

//�Ƿ�ͳ�ƴ˳�����BOSS����
bool BossRefreshModule::ContainSceneID(int sceneID)
{
    map<int, SceneRefreshInfo>::iterator it = m_MapSceneBossInfo.find(sceneID);
    if (it == m_MapSceneBossInfo.end())
    {
        return false;
    }
    return true;
}

//NPC���볡��
int BossRefreshModule::OnBossEntry(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args)
{
    if (!pKernel->Exists(self))
    {
        return 0;
    }
    IGameObj* pNpcObj = pKernel->GetGameObj(self);
    if (NULL == pNpcObj)
    {
        return 0;
    }
    if (pNpcObj->QueryInt("NpcType") != NPC_TYPE_BOSS)
    {
        //����BOSS
        return 0;
    }
    int sceneID = pKernel->GetSceneId();
	const BossInfo* pBossInfo = m_pBossRefreshModule->FindBossInfo(pKernel, pNpcObj->GetConfig());
	if (!m_pBossRefreshModule->ContainSceneID(sceneID) || NULL == pBossInfo)
    {
        //�˳�����ͳ�ƻ�������û�е�boss����¼
        return 0;
    }

    //֪ͨС���ˢ��
    //pKernel->CommandByWorld(CVarList() << COMMAND_ZONES_ENTRY);

    CVarList msg;
	msg << PUBSPACE_DOMAIN
		<< GetDomainName(pKernel)
		<< SP_DOMAIN_MSG_SCENE_BOSS_STATE
		<< pNpcObj->GetConfig()
		<< BS_BOSS_BORN
		<< sceneID;

    pKernel->SendPublicMessage(msg);

	m_pBossRefreshModule->SendBossStateChangeMsg(pKernel, pNpcObj->GetConfig(), BS_BOSS_BORN);
    return 0;
}

//NPC����
int BossRefreshModule::OnCommandBossBeKilled(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args)
{
    if (!pKernel->Exists(self))
    {
        return 0;
    }
    IGameObj* pNpcObj = pKernel->GetGameObj(self);
    if (NULL == pNpcObj)
    {
        return 0;
    }
    if (pNpcObj->QueryInt("NpcType") != NPC_TYPE_BOSS)
    {
        //����BOSS
        return 0;
    }
    int sceneID = pKernel->GetSceneId();
    if (!m_pBossRefreshModule->ContainSceneID(sceneID))
    {
        //�˳�����ͳ��
        return 0;
    }

	IGameObj* pSceneObj = pKernel->GetSceneObj();
	if (NULL == pSceneObj)
	{
		return 0;
	}
	//�Ƿ����BOSS��
	IRecord* pSceneBossRec = pSceneObj->GetRecord(SCENE_BOSS_REC);
	if (pSceneBossRec == NULL)
	{
		return 0;
	}
	const char* strBossConfig = pNpcObj->GetConfig();
	int nRowsIndex = pSceneBossRec->FindString(SCENE_BOSS_CONFIG, strBossConfig);
	if (-1 == nRowsIndex)
	{
		return 0;
	}

	// �������սNPC����boss����ʱ�ڳ����㴴��
	const char*	strRestNpc = pNpcObj->QueryString(FIELD_PROP_REST_BOSS_NPC);
	if (StringUtil::CharIsNull(strRestNpc))
	{
		pSceneBossRec->SetObject(nRowsIndex, SCENE_BOSS_OBJ, PERSISTID());
	}
	else
	{
		float fPosX = pSceneBossRec->QueryFloat(nRowsIndex, SCENE_BOSS_POS_X);
		float fPosZ = pSceneBossRec->QueryFloat(nRowsIndex, SCENE_BOSS_POS_Z);
		float fOrient = pSceneBossRec->QueryFloat(nRowsIndex, SCENE_BOSS_POS_ORIENT);
		float fPosY = pKernel->GetMapHeight(fPosX, fPosZ);
		PERSISTID restnpc = pKernel->CreateObjectArgs("", strRestNpc, 0, fPosX, fPosY, fPosZ, fOrient, CVarList() << CREATE_TYPE_PROPERTY_VALUE << FIELD_PROP_GROUP_ID << -1);
		if (pKernel->Exists(restnpc))
		{
			pSceneBossRec->SetObject(nRowsIndex, SCENE_BOSS_OBJ, restnpc);
		}
	}

    CVarList msg;
	msg << PUBSPACE_DOMAIN
		<< GetDomainName(pKernel)
		<< SP_DOMAIN_MSG_SCENE_BOSS_STATE
		<< strBossConfig
		<< BS_BOSS_DEAD
		<< sceneID;

	pKernel->SendPublicMessage(msg);

	m_pBossRefreshModule->SendBossStateChangeMsg(pKernel, pNpcObj->GetConfig(), BS_BOSS_DEAD);			
    return 0;
}

//����ͻ�����Ϣ
int BossRefreshModule::OnCustomQuerySceneBoss(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args)
{
    if (!pKernel->Exists(self))
    {
        return 0;
    }
    if (args.GetCount() < 1)
    {
        return 0;
    }

	//��ѯBOSS��Ϣ
	m_pBossRefreshModule->QueryBossInfoInScene(pKernel, self);
    return 0;
}

//��ѯ������BOSS��Ϣ
int BossRefreshModule::QueryBossInfoInScene(IKernel* pKernel, const PERSISTID& self)
{
    IGameObj* pSelfObj = pKernel->GetGameObj(self);
    if (NULL == pSelfObj)
    {
        return 0;
    }
	
	IRecord* pPubBossRec = m_pBossRefreshModule->GetPubBossInSceneRec(pKernel, SCENE_BOSS_TOTAL_REC);
	if (pPubBossRec == NULL)
	{
		return 0;
	}

	int nBossRows = pPubBossRec->GetRows();
	CVarList data;
	data << nBossRows;
	LoopBeginCheck(w);
	for (int i = 0;i < nBossRows;++i)
	{
		LoopDoCheck(w);
		const char* strBossConfig = pPubBossRec->QueryString(i, SCENE_BOSS_INFO_CONFIG);
		data << strBossConfig;
		int nSceneId = pPubBossRec->QueryInt(i, SCENE_BOSS_INFO_SCENEID);
		std::map<int, SceneRefreshInfo>::iterator iter = m_pBossRefreshModule->m_MapSceneBossInfo.find(nSceneId);
		if (iter == m_pBossRefreshModule->m_MapSceneBossInfo.end())
		{
			return 0;
		}
		SceneRefreshInfo& sRefreshInfo = iter->second;

		// �Ƿ���Ҫˢboss
		if (sRefreshInfo.nOpen == 1 && m_pBossRefreshModule->NotRefresh(sRefreshInfo.beginTime, sRefreshInfo.endTime))
		{
			data << BS_BOSS_SLEEP;
		}
		else
		{
			int64_t nDeadTime = pPubBossRec->QueryInt64(i, SCENE_BOSS_DEAD_TIME);
			if (BS_BOSS_BORN == nDeadTime)
			{
				data << nDeadTime;
			}
			else
			{
				int nInterval = QueryBossInterval(sRefreshInfo, strBossConfig);
				if (0 == nInterval)
				{
					continue;
				}
				int nLeftTime = (int)(nInterval - (::time(NULL) - nDeadTime));
				data << (int64_t)nLeftTime;
			}
		}

		data << nSceneId;
	}
   
    CVarList list;
    list << SERVER_CUSTOMMSG_BOSS_INFO
		<< SC_SUBMSG_QUERY_BOSS_IN_SCENE
		<< data;

    pKernel->Custom(self, list);

    return 0;
}

// ��ѯboss����
const BossRefreshModule::BossInfo* BossRefreshModule::FindBossInfo(IKernel* pKernel, const string& strBossConfig)
{
	int nSceneId = pKernel->GetSceneId();
	SceneInfoMap::iterator iter = m_MapSceneBossInfo.find(nSceneId);
	if (iter == m_MapSceneBossInfo.end())
	{
		return NULL;
	}

	vector<BossInfo>& bossinfo = iter->second.VecSceneBossInfo;
	BossInfo* pFindInfo = NULL;
	int nSize = (int)bossinfo.size();
	LoopBeginCheck(w);
	for (int i = 0;i < nSize;++i)
	{
		LoopDoCheck(w);
		BossInfo& info = bossinfo[i];
		if (info.sBossConfig == strBossConfig)
		{
			pFindInfo = &bossinfo[i];
			break;
		}
	}
	return pFindInfo;
}

//����boss
void BossRefreshModule::CreateBoss(IKernel* pKernel, const char* strBossConfig, IRecord* pBossRecord, int nRowIndex)
{
	if (NULL == pBossRecord || nRowIndex < 0 || nRowIndex > pBossRecord->GetRows())
	{
		return;
	}
	const BossInfo* pBossInfo = FindBossInfo(pKernel, strBossConfig);
	if (NULL == pBossInfo)
	{
		return;
	}
	float fBornX = 0.0f;
	float fBornZ = 0.0f;
	float fOrient = 0.0f;
	GetRandomPos(pKernel, pBossInfo->VecBossInfo, fBornX, fBornZ, fOrient);
	float fBornY = pKernel->GetMapHeight(fBornX, fBornZ);
	PERSISTID boss = pKernel->CreateObjectArgs("", pBossInfo->sBossConfig.c_str(), 0, fBornX, fBornY, fBornZ, fOrient, CVarList() << CREATE_TYPE_PROPERTY_VALUE << FIELD_PROP_GROUP_ID << -1);
	if (!pKernel->Exists(boss))
	{
		return;
	}

	// ����սnpc,��ɾ��
	PERSISTID restnpc = pBossRecord->QueryObject(nRowIndex, SCENE_BOSS_OBJ);
	if (pKernel->Exists(restnpc))
	{
		pKernel->DestroySelf(restnpc);
	}

	pBossRecord->SetObject(nRowIndex, SCENE_BOSS_OBJ, boss);
	pBossRecord->SetFloat(nRowIndex, SCENE_BOSS_POS_X, fBornX);
	pBossRecord->SetFloat(nRowIndex, SCENE_BOSS_POS_Z, fBornZ);
	pBossRecord->SetFloat(nRowIndex, SCENE_BOSS_POS_ORIENT, fOrient);
}

// ��ȡ����boss����
bool BossRefreshModule::LoadSceneBoss(IKernel* pKernel)
{
	m_MapSceneBossInfo.clear();
	std::string xmlPath = pKernel->GetResourcePath();
	xmlPath += BOSS_REFRESH_INFO;

	CXmlFile xml(xmlPath.c_str());
	if (!xml.LoadFromFile())
	{
		extend_warning(LOG_WARNING, "[BossRefreshModule::LoadSceneBoss] failed");
		return false;
	}

	try
	{
		char* pfilebuff = GetFileBuff(xmlPath.c_str());

		if (NULL == pfilebuff)
		{
			return false;
		}

		xml_document<> doc;

		doc.parse<rapidxml::parse_default>(pfilebuff);

		xml_node<>* pNodeRoot = doc.first_node("Object");
		if (NULL == pNodeRoot)
		{
			return false;
		}
		xml_node<>* pNode = pNodeRoot->first_node("Property");
		if (NULL == pNode)
		{
			return false;
		}

		LoopBeginCheck(d);
		while (pNode != NULL)
		{
			LoopDoCheck(d);
			SceneRefreshInfo tempSceneRefreshInfo;
			int sceneID = convert_int(QueryXmlAttr(pNode, "ID"), 0);
			tempSceneRefreshInfo.nOpen = convert_int(QueryXmlAttr(pNode, "Open"), 0);
			tempSceneRefreshInfo.beginTime = util_convert_string_to_sec(QueryXmlAttr(pNode, "BeginTime"));
			tempSceneRefreshInfo.endTime = util_convert_string_to_sec(QueryXmlAttr(pNode, "EndTime"));

			xml_node<>* pItem = pNode->first_node("Item");
			LoopBeginCheck(e);
			while (pItem != NULL)
			{
				LoopDoCheck(e);
				BossInfo tempBossInfo;
				const char* str = QueryXmlAttr(pItem, "Boss");
				tempBossInfo.sBossConfig = str;
				tempBossInfo.nInterval = convert_int(QueryXmlAttr(pItem, "Interval"), 0);
				xml_node<>* pPosition = pItem->first_node("Position");
				LoopBeginCheck(f);
				while (pPosition != NULL)
				{
					LoopDoCheck(f);
					PosInfo tempPosInfo;
					tempPosInfo.PosX = convert_float(QueryXmlAttr(pPosition, "x"), 0.0f);
					tempPosInfo.PosZ = convert_float(QueryXmlAttr(pPosition, "z"), 0.0f);
					tempPosInfo.Orient = convert_float(QueryXmlAttr(pPosition, "ay"), 0.0f);
					tempBossInfo.VecBossInfo.push_back(tempPosInfo);
					pPosition = pPosition->next_sibling("Position");
				}


				tempSceneRefreshInfo.VecSceneBossInfo.push_back(tempBossInfo);
				pItem = pItem->next_sibling("Item");
			}

			m_MapSceneBossInfo.insert(make_pair(sceneID, tempSceneRefreshInfo));
			pNode = pNode->next_sibling("Property");
		}
	}
	catch (parse_error& e)
	{
		::extend_warning(pKernel, e.what());

		return false;
	}
	return true;
}

//��ȡ����������BOSS��Ϣ��
IRecord * BossRefreshModule::GetPubBossInSceneRec(IKernel * pKernel, const char* strRecName)
{
    IPubSpace * pDomainSpace = pKernel->GetPubSpace(PUBSPACE_DOMAIN);
    if (NULL == pDomainSpace)
    {
        return NULL;
    }

    // ��ȡ�����ռ�
    IPubData * pPubData = pDomainSpace->GetPubData(GetDomainName(pKernel).c_str());
    if (NULL == pPubData)
    {
        return NULL;
    }

	IRecord * sceneBossRec = pPubData->GetRecord(strRecName);
    if (NULL == sceneBossRec)
    {
        return NULL;
    }

    return sceneBossRec;
}

// ��ѯĳ��������Ұ��boss���� outData ��ʽ:outData << bossid << lefttime << bossid << lefttime..
bool BossRefreshModule::QueryBossInfo(IKernel * pKernel, int nSceneId, IVarList& outData)
{
	outData.Clear();
	IRecord* pPubBossRec = m_pBossRefreshModule->GetPubBossInSceneRec(pKernel, SCENE_BOSS_TOTAL_REC);
	if (pPubBossRec == NULL)
	{
		return false;
	}

	int nBossRows = pPubBossRec->GetRows();
	LoopBeginCheck(w);
	for (int i = 0; i < nBossRows; ++i)
	{
		LoopDoCheck(w);
		const char* strBossConfig = pPubBossRec->QueryString(i, SCENE_BOSS_INFO_CONFIG);
		int nTmpSceneId = pPubBossRec->QueryInt(i, SCENE_BOSS_INFO_SCENEID);
		if (nTmpSceneId != nSceneId)
		{
			continue;
		}
		std::map<int, SceneRefreshInfo>::iterator iter = m_pBossRefreshModule->m_MapSceneBossInfo.find(nSceneId);
		if (iter == m_pBossRefreshModule->m_MapSceneBossInfo.end())
		{
			continue;
		}
		SceneRefreshInfo& sRefreshInfo = iter->second;
		outData << strBossConfig;
		// �Ƿ���Ҫˢboss
		if (sRefreshInfo.nOpen == 1 && m_pBossRefreshModule->NotRefresh(sRefreshInfo.beginTime, sRefreshInfo.endTime))
		{
			outData << BS_BOSS_SLEEP;
		}
		else
		{
			int64_t nDeadTime = pPubBossRec->QueryInt64(i, SCENE_BOSS_DEAD_TIME);
			if (BS_BOSS_BORN == nDeadTime)
			{
				outData << nDeadTime;
			}							 
			else
			{
				int nInterval = QueryBossInterval(sRefreshInfo, strBossConfig);
				if (nInterval > 0)
				{
					int64_t nLeftTime = (int64_t)(nInterval - (::time(NULL) - nDeadTime));
					outData << nLeftTime;
				}
			}
		}
	}

	return true;
}

// ����boss״̬�ı�
void BossRefreshModule::SendBossStateChangeMsg(IKernel * pKernel, const char* strBossId, int nState)
{
	if (StringUtil::CharIsNull(strBossId))
	{
		return;
	}

	int nSceneId = pKernel->GetSceneId();
	std::map<int, SceneRefreshInfo>::iterator iter = m_pBossRefreshModule->m_MapSceneBossInfo.find(nSceneId);
	if (iter == m_pBossRefreshModule->m_MapSceneBossInfo.end())
	{
		return;
	}
	SceneRefreshInfo& sRefreshInfo = iter->second;

	CVarList msg;
	msg << SERVER_CUSTOMMSG_BOSS_INFO << SC_SUBMSG_BOSS_STATE_CHANGE << strBossId;
	
	// �Ƿ���Ҫˢboss
	if (sRefreshInfo.nOpen == 1 && m_pBossRefreshModule->NotRefresh(sRefreshInfo.beginTime, sRefreshInfo.endTime))
	{
		msg << BS_BOSS_SLEEP;
	}
	else
	{
		if (nState == BS_BOSS_DEAD)
		{
			int nInterval = QueryBossInterval(sRefreshInfo, strBossId);
			if (0 == nInterval)
			{
				return;
			}
			msg << nInterval;
		}
		else
		{
			msg << BS_BOSS_BORN;
		}
	}

	pKernel->CustomByScene(nSceneId, msg);
}

// ��ѯboss��ˢ�¼��
int BossRefreshModule::QueryBossInterval(const SceneRefreshInfo& sceneInfo, const char* strBoss)
{
	int nInterval = 0;
	int nSize = sceneInfo.VecSceneBossInfo.size();
	LoopBeginCheck(s);
	for (int i = 0; i < nSize;++i)
	{
		const BossInfo& info = sceneInfo.VecSceneBossInfo[i];
		if (strcmp(info.sBossConfig.c_str(), strBoss) == 0)
		{
			nInterval = info.nInterval;
			break;
		}
	}

	return nInterval;
}

//�Ƿ��ڲ�ˢ��ʱ�����
bool BossRefreshModule::NotRefresh(int beginTime, int endTime)
{
    int curTime = util_convert_zerotime_to_sec();
    if (beginTime > endTime)
    {
        //����
        if (curTime >= beginTime || curTime <= endTime)
        {
            return true;
        }
    }
    else if (beginTime < endTime)
    {
        //����
        if (curTime >= beginTime && curTime <= endTime)
        {
            return true;
        }
    }

    return false;
}

// ���¼��ؼ�������
void BossRefreshModule::ReloadConfig(IKernel* pKernel)
{
	m_pBossRefreshModule->LoadResource(pKernel);
}

// ��ȡ������������
const std::wstring & BossRefreshModule::GetDomainName(IKernel * pKernel)
{
	//�������
	if (m_domainName.empty())
	{
		wchar_t wstr[256];
		const int server_id = pKernel->GetServerId();
		SWPRINTF_S(wstr, L"Domain_Boss_%d", server_id);

		m_domainName = wstr;
	}

	return m_domainName;
}