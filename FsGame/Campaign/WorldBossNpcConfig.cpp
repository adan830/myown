//--------------------------------------------------------------------
// �ļ���:      WorldBossNpcConfig.cpp
// ��  ��:      
// ˵  ��:		
// ��������:    2015��12��11��
// ������:       
// �޸���:        
//    :       
//--------------------------------------------------------------------

#include "WorldBossNpc.h"

#include "FsGame/CommonModule/PubModule.h"

#include "FsGame/Define/PubDefine.h"
#include "FsGame/Define/WorldBossNpcDefine.h"
#include "FsGame/Define/GameDefine.h"
#include "FsGame/Define/LogDefine.h"
#include "FsGame/Define/CommandDefine.h"
#include "FsGame/Define/ClientCustomDefine.h"
#include "FsGame/Define/ServerCustomDefine.h"
#include "FsGame/NpcBaseModule/ai/AIDefine.h"
#include "FsGame/SystemFunctionModule/RewardModule.h"
#include "FsGame/Define/ToolBoxSysInfoDefine.h"
#include "FsGame/CommonModule/LevelModule.h"
#include "FsGame/NpcBaseModule/AI/AISystem.h"
#include "FsGame/CommonModule/LandPosModule.h"
#include "FsGame/CommonModule/LuaExtModule.h"
#include "FsGame/Define/ModifyPackDefine.h"
#include "FsGame/SystemFunctionModule/StaticDataQueryModule.h"
#include "FsGame/Define/StaticDataDefine.h"
#include "FsGame/CommonModule/PropRefreshModule.h"
#include "FsGame/SystemFunctionModule/RankListModule.h"
#include "FsGame/SystemFunctionModule/CoolDownModule.h"
#include "FsGame/Define/CoolDownDefine.h"
#include "FsGame/CommonModule/ActionMutex.h"

#include "FsGame/Interface/FightInterface.h"

#include <algorithm>
#include "utils/custom_func.h"
#include "utils/XmlFile.h"
#include "utils/extend_func.h"
#include "utils/string_util.h"
#include "utils/util_func.h"
#include "utils/util.h"
#include <time.h>
#include "CommonModule/EnvirValueModule.h"
#include "CommonModule/CommRuleModule.h"

// ����õ�ַ
static const std::string XML_CONFIG_ACTIVE_INFO_PATH  = "ini/Campaign/WorldBoss/NpcWorldBoss.xml";
// �������õ�ַ
static const std::string XML_CONFIG_AWARG_PATH        = "ini/Campaign/WorldBoss/WorldBossAward.xml";
// ��������õ�ַ
static const std::string XML_CONFIG_WORLD_BOSS_CONST_PATH  = "ini/Campaign/WorldBoss/WorldBossConst.xml";
// ����bossС������
static const std::string XML_CONFIG_OTHER_NPC_PATH = "ini/Campaign/WorldBoss/WorldBossOtherNpc.xml";
// ����boss�ɳ�����
static const std::string XML_CONFIG_GROW_UP_PATH = "ini/Campaign/WorldBoss/WorldBossGrowUp.xml";

int WorldBossNpc::m_nCustomSceneId = 0;

/*!
* @brief	���ػ��Ϣ����  
* @param	����ָ��
* @return	bool
*/
bool WorldBossNpc::LoadActiveInfoResource(IKernel* pKernel)
{
	m_vecActiveInfo.clear();

	//�ж��ļ��Ƿ����
	std::string xmlPath = pKernel->GetResourcePath() + XML_CONFIG_ACTIVE_INFO_PATH;
	TiXmlDocument doc(xmlPath.c_str());
	if (!doc.LoadFile())
	{
		return false;
	}

	// ���ڵ�
	TiXmlElement* pRoot = doc.RootElement();
	if (NULL == pRoot)
	{
		return false;
	}

	std::vector<int> vecScopeScene;
	std::vector<WorldBossActive_t>& vecActiveInfo = m_vecActiveInfo;

	// Property�ڵ�
	TiXmlElement* pGroup = pRoot->FirstChildElement("Property");
	if (NULL == pGroup)
	{
		return false;
	}
	LoopBeginCheck(ak);
	while (pGroup)
	{
		LoopDoCheck(ak);
		WorldBossActive_t tActiveInfo;
		tActiveInfo.m_ID = convert_int(pGroup->Attribute("ID"), 0);
		tActiveInfo.m_SceneID = convert_int(pGroup->Attribute("SceneID"), 0);

		if (0 == m_nCustomSceneId)
		{
			m_nCustomSceneId = tActiveInfo.m_SceneID;
		}

		const char* cWeeks = pGroup->Attribute("Week");
		const char* strBosses = pGroup->Attribute("BossId");
		const char* bossPos = pGroup->Attribute("BossPos");
		const char* playerPos = pGroup->Attribute("PlayerPos");
		if (StringUtil::CharIsNull(cWeeks)
			|| StringUtil::CharIsNull(strBosses) 
			|| StringUtil::CharIsNull(bossPos)
			|| StringUtil::CharIsNull(playerPos))
		{
			return false;
		}

		std::vector<int> vecWeeks;
		m_pWorldBossNpc->ParseWeek(cWeeks, vecWeeks);
		tActiveInfo.m_Weeks = vecWeeks;

		// ֪ͨʱ��
		tActiveInfo.m_NoticeBeginTime = ParseTime(pGroup->Attribute("NoticeBeginTime"));
		tActiveInfo.m_NoticeEndTime = ParseTime(pGroup->Attribute("NoticeEndTime"));

		// �ʱ��
		tActiveInfo.m_ActiveBeginTime = ParseTime(pGroup->Attribute("ActiveBeginTime"));
		tActiveInfo.m_ActiveEndTime = ParseTime(pGroup->Attribute("ActiveEndTime"));

		if (StringUtil::CharIsNull(strBosses))
		{
			extend_warning(LOG_ERROR, " BossId is empty! ");
			return false;
		}

		tActiveInfo.m_strBossID = strBosses;

		CommRuleModule::ParsePosInfo(tActiveInfo.m_PlayerPos, playerPos);
		CommRuleModule::ParsePosInfo(tActiveInfo.m_BossPos, bossPos);

		tActiveInfo.m_PlayerLevel = convert_int(pGroup->Attribute("PlayerLevel"), 0);

		vecActiveInfo.push_back(tActiveInfo);
		vecScopeScene.push_back(tActiveInfo.m_SceneID);
		m_SceneScopeVec.push_back(tActiveInfo.m_SceneID);

		pGroup = pGroup->NextSiblingElement("Property");
	}

	doc.Clear();
	return true;
}


/*!
* @brief	���ؽ������� 
* @param	����ָ��
* @return	bool
*/
bool WorldBossNpc::LoadAwardResource(IKernel* pKernel)
{
	// �������
	m_vecBossAward.clear();

	//�ж��ļ��Ƿ����
	std::string xmlPath = pKernel->GetResourcePath() + XML_CONFIG_AWARG_PATH;
	TiXmlDocument doc(xmlPath.c_str());
	if (!doc.LoadFile())
	{
		return false;
	}

	TiXmlElement* pRoot = doc.RootElement();
	if (NULL == pRoot)
	{
		return false;
	}

	std::vector<WorldBossAward_t>& tAwardVector = m_vecBossAward;
	TiXmlElement* _pElement = pRoot->FirstChildElement("Property");
	if (NULL == _pElement)
	{
		return false;
	}

	LoopBeginCheck(b);
	while (_pElement)
	{
		LoopDoCheck(b);

		WorldBossAward_t tAward;
		tAward.m_BossCfgID = _pElement->Attribute("BossID");
		if (StringUtil::CharIsNull(tAward.m_BossCfgID.c_str()))
		{
			Assert(false);
			return false;
		}

		tAward.m_nLastHurtAward = convert_int(_pElement->Attribute("LastHurtAward"), 0);
		//��������
		TiXmlElement* pElement = _pElement->FirstChildElement("RankAward");
		LoopBeginCheck(f);
		while (pElement)
		{
			LoopDoCheck(f);

			RankAward_t tRankAward;
			tRankAward.m_MinRank = convert_int(pElement->Attribute("MinRank"),0);
			tRankAward.m_MaxRank = convert_int(pElement->Attribute("MaxRank"),0);
			tRankAward.m_nWinRewardId = convert_int(pElement->Attribute("WinReward"), 0);
			tRankAward.m_nWinRewardId = convert_int(pElement->Attribute("FailedReward"), 0);

			tAward.m_vecRankAward.push_back(tRankAward);

			pElement = pElement->NextSiblingElement("RankAward");
		}
	}

	doc.Clear();
	return true;
}

/*!
* @brief	���ػ������������ 
* @param	����ָ��
* @return	bool
*/
bool WorldBossNpc::LoadActiveRuleResource(IKernel* pKernel)
{
	if (NULL == pKernel)
	{
		return false;
	}

	std::string xmlPath = pKernel->GetResourcePath() + XML_CONFIG_WORLD_BOSS_CONST_PATH;
	TiXmlDocument doc(xmlPath.c_str());
	if (!doc.LoadFile())
	{
		return false;
	}

	TiXmlElement* pRoot = doc.RootElement();
	if (NULL == pRoot)
	{
		return false;
	}

	TiXmlElement* pGroup = pRoot->FirstChildElement("Property");
	if (NULL == pGroup)
	{
		return false;
	}

	LoopBeginCheck(k);
	while (pGroup)
	{
		LoopDoCheck(k);

		m_kConstConfig.nEndRankNum		= convert_int(pGroup->Attribute("EndRankLNum"), 0);
		m_kConstConfig.nReadyTime		= convert_int(pGroup->Attribute("ReadyTime"), 0) * 1000;
		m_kConstConfig.nBornCGTime		= convert_int(pGroup->Attribute("BornCGTime"), 0) * 1000;
		m_kConstConfig.nLeaveSceneDelay = convert_int(pGroup->Attribute("LeaveSceneDelay"), 0) * 1000;
		m_kConstConfig.nActiveRankNum	= convert_int(pGroup->Attribute("ActiveRankNum"), 0);

		pGroup = pGroup->NextSiblingElement("Property");
	}

	doc.Clear();

	return true;
}

// ��������boss�ɳ�����
bool WorldBossNpc::LoadWorldBossGrowUpConfig(IKernel* pKernel)
{
	// �����ļ�·��
	std::string pathName = pKernel->GetResourcePath();
	pathName += XML_CONFIG_GROW_UP_PATH;

	CXmlFile xmlfile(pathName.c_str());
	if (!xmlfile.LoadFromFile())
	{
		std::string info = "Not Exist " + pathName;
		::extend_warning(LOG_ERROR, info.c_str());
		return false;
	}

	m_vecWorldBossGrowUp.clear();
	const int iSectionCount = (int)xmlfile.GetSectionCount();
	m_vecWorldBossGrowUp.reserve(iSectionCount);

	LoopBeginCheck(f);
	for (int i = 0; i < iSectionCount; i++)
	{
		LoopDoCheck(f);
		const char* section = xmlfile.GetSectionByIndex(i);

		WorldBossGrowUp data;
		data.nSceneId		= StringUtil::StringAsInt(section);
		data.nKillDurationTime	= xmlfile.ReadInteger(section, "KillDurationTime", 0);
		data.nBasePackageId		= xmlfile.ReadInteger(section, "BasePackageID", 0);
		data.nIncrement			= xmlfile.ReadInteger(section, "Increment", 1);
		data.nMaxLevel			= xmlfile.ReadInteger(section, "MaxLevel", 1);

		m_vecWorldBossGrowUp.push_back(data);
	}

	return true;
}

/*!
* @brief	ʵʱ��ʾ�˺��б�(5��һ��)
* @param	pKernel
* @param	self
* @param	slice
* @return	int
*/
int WorldBossNpc::HB_ShowAttackRank(IKernel* pKernel, const PERSISTID& self, int slice)
{
	IGameObj* pScene = pKernel->GetGameObj(self);
	if (NULL == pScene)
	{
		return 0;
	}

	const PERSISTID& BossObj = pScene->QueryDataObject("world_boss_actived");
	if (BossObj.IsNull())
	{
		return 0;
	}

	IGameObj* pBoss = pKernel->GetGameObj(BossObj);
	if (NULL == pBoss)
	{
		return 0;
	}

	IRecord* pRec = pBoss->GetRecord("worldboss_attack_list_rec");
	if (NULL == pRec)
	{
		return 0;
	}

	int iCount = (int)pRec->GetRows();
	if (iCount < 1)
	{
		return 0;
	}

	// ����
	CopyAIHurtVector tempDatas;
	tempDatas.reserve(iCount);

	LoopBeginCheck(uu);
	for (int i = 0; i < iCount; ++i)
	{
		LoopDoCheck(uu);
		int iDamage = pRec->QueryInt(i, WORLDBOSS_ATTACK_LIST_REC_DAMAGE);
		if (iDamage > 0)
		{
			CopyAIHurt_t tmp;
			tmp.m_player_name = pRec->QueryWideStr(i, WORLDBOSS_ATTACK_LIST_REC_PLAYER_NAME);
			tmp.m_damage = iDamage;
			tempDatas.push_back(tmp);
		}		

	}
	sort(tempDatas.begin(), tempDatas.end(), std::less<CopyAIHurt_t>());

	// ͳ��ǰ����
	int nRankNum = m_kConstConfig.nActiveRankNum;
	CVarList cRankList;
	int iRowCount = (int)tempDatas.size();
	int nDataCount = 0;
	LoopBeginCheck(dd);
	for (int i = 0; i < iRowCount; ++i)
	{
		LoopDoCheck(dd);
		if (i >= nRankNum)
		{
			break;
		}
		CopyAIHurt_t info = tempDatas[i];
		int iHurt = info.m_damage;
		const wchar_t* wName = info.m_player_name.c_str();
		if (StringUtil::CharIsNull(wName))
		{
			continue;
		}
		cRankList << i + 1
				  << wName 
			      << iHurt;
		++nDataCount;
	}

	// ��������ͽ����������
	CVarList ShowMsg;

	//���������ҷ����Լ����˺�����
	LoopBeginCheck(ee);
	for (int i = 0; i < iRowCount; ++i)
	{
		LoopDoCheck(ee);
		CopyAIHurt_t info = tempDatas[i];
		const wchar_t* player_name = info.m_player_name.c_str();
		if (StringUtil::CharIsNull(player_name))
		{
			continue;
		}

		// ��Ҳ��ڵ�ǰ���������ͽ������
		PERSISTID& Attacker = pKernel->FindPlayer(player_name);
		IGameObj* pAttacker = pKernel->GetGameObj(Attacker);
		if (NULL == pAttacker)
		{
			continue;
		}
		int iHurt = info.m_damage;

		ShowMsg.Clear();

		ShowMsg << SERVER_CUSTOMMSG_WORLD_BOSS_ACTIVE
			<< SC_WORLD_BOSS_SELF_HURT_INFO
			<< i + 1
			<< iHurt;
		::SafeCustom(pKernel, Attacker, ShowMsg);
	}

	ShowMsg.Clear();
	ShowMsg << SERVER_CUSTOMMSG_WORLD_BOSS_ACTIVE
		<< SC_WORLD_BOSS_ACTIVE_HURT_RANK
		<< nDataCount
		<< cRankList;
	// ���򳡾�������ҷ���ʵʱ�˺���������
	pKernel->CustomByScene(pKernel->GetSceneId(), ShowMsg);

	return 0;
}