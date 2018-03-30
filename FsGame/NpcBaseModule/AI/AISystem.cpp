//--------------------------------------------------------------------
// �ļ���:      AISystem.cpp
// ��  ��:      AIϵͳ
// ˵  ��:
// ��������:    2014��12��31��
// ������:       
// �޸���:        
//    :       
//--------------------------------------------------------------------
#include <algorithm>
#include "AISystem.h"
#include "utils/util_ini.h"
#include "utils/util_func.h"
#include "FsGame/Define/CommandDefine.h"
#include "utils/XmlFile.h"
#include "utils/string_util.h"
#include "FsGame/CommonModule/LuaExtModule.h"
#include "Rule/AIRuleManage.h"

//template
#include "FsGame/NpcBaseModule/AI/Template/AITNormalFight.h"
#include "FsGame/NpcBaseModule/AI/Template/AITBoss.h"
#include "FsGame/Define/GameDefine.h"
#include "FsGame/CommonModule/EnvirValueModule.h"
#include "Define/PlayerBaseDefine.h"
#include "Interface/FightInterface.h"

AISystem* AISystem::m_pAISystem = NULL;
FightModule * AISystem::m_pFightModule = NULL;
SkillModule * AISystem::m_pSkillModule = NULL;
MotionModule * AISystem::m_pMotionModule = NULL;
DropModule * AISystem::m_pDropModule = NULL;
ContainerModule * AISystem::m_pContainerModule = NULL;
LevelModule * AISystem::m_pLevelModule = NULL;
CapitalModule * AISystem::m_pCapitalModule = NULL;

#ifndef FSROOMLOGIC_EXPORTS
//CopyOffLineModule* AISystem::m_pCopyOffLineModule =NULL;
#endif // FSROOMLOGIC_EXPORTS

static const float MAX_SPRING_RANGE = 40.0f; // ��󴥷���Χ

// ����NPC���
int nx_init_battle_npc(void *state)
{
	// ��ȡ����ָ��
	IKernel* pKernel = LuaExtModule::GetKernel(state);
	// ����������
	CHECK_ARG_NUM(state, nx_init_battle_npc, 1);
	// ����������
	CHECK_ARG_INT(state, nx_init_battle_npc, 1);
	// ��ȡ����
	PERSISTID scene = pKernel->GetScene();
	int nTeamId = pKernel->LuaToInt(state, 1);

//	AISystem::CreateBattleTeam(pKernel, scene, 1, nTeamId, 10);
	return 1;
}

bool AISystem::Init(IKernel* pKernel)
{
	m_pAISystem = this;
	m_pFightModule = (FightModule*)pKernel->GetLogicModule("FightModule");
	m_pSkillModule = (SkillModule*)pKernel->GetLogicModule("SkillModule");
	m_pMotionModule = (MotionModule*)pKernel->GetLogicModule("MotionModule");
	m_pDropModule = (DropModule*)pKernel->GetLogicModule("DropModule");
	m_pContainerModule = (ContainerModule*)pKernel->GetLogicModule("ContainerModule");
	m_pLevelModule = (LevelModule*)pKernel->GetLogicModule("LevelModule");
	m_pCapitalModule = (CapitalModule*)pKernel->GetLogicModule("CapitalModule");

	Assert(m_pAISystem != NULL && m_pFightModule !=NULL && m_pSkillModule != NULL && m_pMotionModule != NULL 
		&& m_pDropModule != NULL && m_pContainerModule != NULL && m_pLevelModule != NULL && m_pCapitalModule != NULL);

#ifndef FSROOMLOGIC_EXPORTS
// 	m_pCopyOffLineModule = (CopyOffLineModule*)pKernel->GetLogicModule("CopyOffLineModule");
// 	Assert(m_pCopyOffLineModule != NULL);
#endif // FSROOMLOGIC_EXPORTS

    pKernel->AddEventCallback("NormalNpc", "OnCreateArgs", AISystem::OnCreateArgs);
	pKernel->AddEventCallback("FunctionNpc", "OnCreateArgs", AISystem::OnCreateArgs);
	pKernel->AddEventCallback("NormalNpc", "OnEntry", OnEntry);
	pKernel->AddEventCallback("NormalNpc", "OnSpring", OnSpring);
	pKernel->AddEventCallback("NormalNpc", "OnDestroy", OnDestroy);
	
	pKernel->AddIntCommandHook("NormalNpc", COMMAND_BEDAMAGE, AISystem::OnBeDamaged);
	pKernel->AddIntCommandHook("NormalNpc", COMMAND_BEKILL, AISystem::OnBeKilled);
// 	pKernel->AddIntCommandHook("player", COMMAND_RELIVE, AISystem::OnPlayerRelive);
// 	pKernel->AddIntCommandHook("NormalNpc", COMMAND_RELIVE, AISystem::OnRelive);
// 
// 	pKernel->AddIntCommandHook("NormalNpc", COMMAND_NPC_STIFF_OVER, AISystem::OnFightOverMove);
// 
// 	//��������������
// 	pKernel->AddIntCommandHook("BattleNpc", COMMAND_OFFLINE_DATA_COPY_FINISH, AISystem::OnCopyDataComplete);

	//��ʼ��ģ��
	InitTemplateVector(pKernel);
	//����NPC����Ȩֵ
	LoadNPCSkill(pKernel);
    //����NPC���ܰ�
    LoadNpcSkillPackage(pKernel);
	//���ػ���NPC·��
	LoadConvoyPath(pKernel);
	//���ع�������
	LoadInterceptInfo(pKernel);
	//����Ѳ��·��
	LoadPatrolPath(pKernel);
	//���ع�սNPC
	LoadBattleNpcBornIndex(pKernel);
	//�����ƶ�·��
	LoadBattleNpcMoveType(pKernel);
	// ����С������
	LoadBattleNpcTeam(pKernel);
	// ����С�Ӷ���
	LoadBattleTeamFormation(pKernel);
	//�������ر���
	LoadBoxConfig(pKernel);
	// ���صȼ����԰�
	LoadLevelPackage(pKernel);

	//ս������
	DECL_HEARTBEAT(AIHeartBeat::HB_AIFighting);

	//Ѳ������
	DECL_HEARTBEAT(AIHeartBeat::HB_AIPatrol);

	//���ս������
	DECL_HEARTBEAT(AIHeartBeat::HB_AIEndFight);
	// ����Npc��λ
// 	DECL_HEARTBEAT(AIHeartBeat::HB_LocationConvyNpc);
// 	// �ӳٻ���
// 	DECL_HEARTBEAT(AIHeartBeat::HB_DelayConvoy);

	// Ѻ��Npc��������
	/*DECL_HEARTBEAT(AIHeartBeat::HB_EscortNpcFollow);*/

	//��������
	DECL_HEARTBEAT(AIHeartBeat::HB_AIBornEnd);

	//ת������
	DECL_HEARTBEAT(AIHeartBeat::HB_AIRatate);

	//����
	DECL_HEARTBEAT(AIHeartBeat::HB_AIFind);

	//�������
	DECL_HEARTBEAT(AIHeartBeat::HB_AIClonePlayer);

    //��ͨ�ֻس����㳬ʱ����
    DECL_HEARTBEAT(AIHeartBeat::HB_BACK_BORN_TIMEOUT);

    //�����սʿ����Ѱ����
//     DECL_HEARTBEAT(AIHeartBeat::HB_Nation_Fight_Find);
// 
//     //�����ս��ʬ�岻�ɼ�
//     DECL_HEARTBEAT(AIHeartBeat::HB_Nation_Dead_Invisible);
	//����npc״̬�ı�ص�
	//DECL_CRITICAL(AISectorSpringRangeNpc::c_OnAICurStateChange);

	//
	//DECL_HEARTBEAT(AISectorSpringRangeNpc::HB_ScanAround);

	DECL_HEARTBEAT(AIHeartBeat::HB_EndEscape);

	DECL_HEARTBEAT(AIHeartBeat::HB_CheckEscape);

	DECL_LUA_EXT(nx_init_battle_npc);

	//��ʼ�����������
	AIRuleManage::Init(pKernel);
	AIFunction::Init(pKernel);
	return true;
}

// �ر�
bool AISystem::Shut(IKernel* pKernel)
{
	m_pAISystem->m_mapSkillConfig.clear();
	m_pAISystem->m_mapConvoyStep.clear();
	m_pAISystem->m_mapIntercept.clear();
	m_pAISystem->m_mapPatrolInfo.clear();
	m_pAISystem->m_mapBattleNpcInfo.clear();
	m_pAISystem->m_mapMoveTypeInfo.clear();
	AIFunction::Shut(pKernel);
	return true;
}

//��ʼ��ģ��
void AISystem::InitTemplateVector(IKernel *pKernel)
{
    LoopBeginCheck(a);
	for (int i = 0; i < AI_TEMP_MAX; ++i)
	{
        LoopDoCheck(a);
		m_vctAITemplate.push_back(NULL);
	}
	//ע����ͨNPCģ��
	RegistTemplate(pKernel, new AITemplateBase);
	//ע����ͨս��NPCģ��
	RegistTemplate(pKernel, new AITNormalFight);
	//BossAI
// 	RegistTemplate(pKernel, new AITBoss);
// 	//���ر���
// 	RegistTemplate(pKernel, new AITBox);
// 	//����NPCģ��
// 	RegistTemplate(pKernel, new AITPet);

#ifndef FSROOMLOGIC_EXPORTS
	//ע�Ụ��NPC
// 	RegistTemplate(pKernel, new AITConvoy);
// 	//ע������NPC
// 	RegistTemplate(pKernel, new AITIntercept);
// 	//ע�Ụ��NPC
// 	RegistTemplate(pKernel, new AITEscort);
// 	//ע��Ѳ��NPC
// 	RegistTemplate(pKernel, new AITPatrolOnlyNpc);
// 	//ע���ս�ӳ�
// 	RegistTemplate(pKernel, new AITBattleLeadNpc);
// 	//ע��С�ӳ�Ա
// 	RegistTemplate(pKernel, new AITBattleNpc);
// 	//�����������
// 	RegistTemplate(pKernel, new AITInterceptPlayer);
// 	//���߹һ�
// 	//RegistTemplate(pKernel, new AITOutLinePlayer);
//     //������������
//     RegistTemplate(pKernel, new AITNationGuard);
//     // ������AI
//     RegistTemplate(pKernel, new AITArenaNpc);
//     //ע��Ѳ��Ұ��NPC
//     RegistTemplate(pKernel, new AITPatrolMonsterNpc);
// 	//������Ӧ��npc
// 	RegistTemplate(pKernel,	new AISectorSpringRangeNpc );
// 	//�ؾ�����npc
// 	RegistTemplate(pKernel, new	AISecretSceneGuardNpc);
// 	//������Ŀ��
// 	RegistTemplate(pKernel, new AISecretProgectObjNpc);
// 	//����npc
// 	RegistTemplate(pKernel, new AIEscapeNpc);
// 	//ս��Ѳ��ncp
// 	RegistTemplate(pKernel ,new AIFightPatrolNpc);
// 	//��Ӫսbuffer Npc
// 	RegistTemplate(pKernel, new AITBattleBuffNpc);
// 	//������������ Npc
// 	RegistTemplate(pKernel, new AIDeadGenTrapNpc);
// 	//���鳡��boss
// 	RegistTemplate(pKernel, new AIStartSceneBoss);
// 	//���鳡��bossѪ������
// 	RegistTemplate(pKernel, new AIStartSceneControlHpBoss);
// 	//�߾�����
// 	RegistTemplate(pKernel, new AITowerNpc);
// 	//����boss
// 	RegistTemplate(pKernel, new AIInvadeBossNpc);
    
#endif // FSROOMLOGIC_EXPORTS
}

// ע��ģ��
bool AISystem::RegistTemplate(IKernel *pKernel, AITemplateBase *pTemplateBase)
{
	if (pTemplateBase->GetTemplateType() < 0 || pTemplateBase->GetTemplateType() >= (int)m_vctAITemplate.size())
	{
		delete pTemplateBase;
		extend_warning(pKernel,"����AIģ��ʧ��");
        assert(0);
		return false;
	}
	if (m_vctAITemplate[pTemplateBase->GetTemplateType()] == NULL)
	{
        pTemplateBase->Init(pKernel);
		m_vctAITemplate[pTemplateBase->GetTemplateType()] = pTemplateBase;
	}
    else
    {
        extend_warning(pKernel, "�ظ�ע������Ͷ���");
        assert(0);
    }

	return true;
}

//�Ҳ�����Ӧģ��ʱ�û���ģ��
AITemplateBase AISystem::s_AITemplateBase;

//���AIģ�����,�Ҳ�����Ӧģ��ʱ�÷��ػ���ģ��
AITemplateBase &AISystem::GetAITemlate(IKernel* pKernel, const PERSISTID& self)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		assert(0);
		return s_AITemplateBase;
	}

	if (!pSelfObj->FindAttr("AITemplate"))
	{
		return s_AITemplateBase;
	}

	//ȡ��NPC�����AIģ������
	AI_TEMPLATE_TYPE AITemplateType = (AI_TEMPLATE_TYPE)pSelfObj->QueryInt("AITemplate");

	if (AITemplateType < 0 || AITemplateType >= AI_TEMP_MAX || AITemplateType >= (int)m_vctAITemplate.size())
    {
        assert(0);
        return s_AITemplateBase;
    }

	return *m_vctAITemplate[AITemplateType];
}

// ���볡��
int AISystem::OnEntry(IKernel * pKernel, const PERSISTID & self, const PERSISTID & sender, const IVarList & args)
{
	if (pKernel->Type(sender) == TYPE_SCENE)
	{
		IGameObj* pSelfObj = pKernel->GetGameObj(self);
		if (pSelfObj == NULL)
		{
			return 0;
		}

		//CampaignGuildChaosFight::SetNpcLevelPackage(pKernel, self);
//#ifndef FSROOMLOGIC_EXPORTS
//		if (pKernel->GetServerId() == 7440001 || pKernel->GetServerId() == 7440005)
//		{
//			//////////////////////////////////////////////////////////////////////////
//			// �������Debug�ã��ǵ�ע��		//
//			//////////////////////////////////////////////////////////////////////////
//			AIHeartBeat::HB_EscortNpcFollow(pKernel, self, 0);
//			ADD_HEART_BEAT(pKernel, self, "AIHeartBeat::HB_EscortNpcFollow", 10000);
//			//////////////////////////////////////////////////////////////////////////
//			// �������Debug�ã��ǵ�ע��		//
//			//////////////////////////////////////////////////////////////////////////
//		}
//		else
//		{
//#if defined _DEBUG
//			//////////////////////////////////////////////////////////////////////////
//			// �������Debug�ã��ǵ�ע��		//
//			//////////////////////////////////////////////////////////////////////////
//			AIHeartBeat::HB_EscortNpcFollow(pKernel, self, 0);
//			ADD_HEART_BEAT(pKernel, self, "AIHeartBeat::HB_EscortNpcFollow", 10000);
//			//////////////////////////////////////////////////////////////////////////
//			// �������Debug�ã��ǵ�ע��		//
//			//////////////////////////////////////////////////////////////////////////
//#endif
//		}
//#endif

		// �������߸߶�
		float fWalkStep = EnvirValueModule::EnvirQueryFloat(ENV_VALUE_NPC_WALK_STEP_DEFAULT);
		pSelfObj->SetFloat("WalkStep", fWalkStep);

		// ��¼����ʱ�ĸ߶�
		//pSelfObj->SetFloat(FIELD_PROP_BORN_HEIGHT, pSelfObj->GetPosiY());

        AISystem::Instance()->GetAITemlate(pKernel, self).OnBorn(pKernel, self, sender, args);
	}
	return 0;
}

//������
int AISystem::OnSpring(IKernel * pKernel,const PERSISTID & self,const PERSISTID & sender,const IVarList & args)
{
	if (pKernel->Exists(self) && pKernel->Exists(sender))
	{
        AISystem::Instance()->GetAITemlate(pKernel, self).OnProcess(pKernel, AI_RULE_SPRING, self, sender, args);
	}
	return 0;
}

//��������
int AISystem::OnDestroy(IKernel *pKernel, const PERSISTID &self, const PERSISTID &sender, const IVarList &args)
{
	if (pKernel->Exists(self) && pKernel->Exists(sender))
	{
		AISystem::Instance()->GetAITemlate(pKernel, self).OnProcess(pKernel, AI_RULE_DESTORY, self, sender, args);
	}
	return 0;
}


//���˺�
int AISystem::OnBeDamaged(IKernel * pKernel,const PERSISTID & self,const PERSISTID & sender,const IVarList & args)
{
	if (!pKernel->Exists(self))
	{
		return 0;
	}
    AISystem::Instance()->GetAITemlate(pKernel, self).OnProcess(pKernel, AI_RULE_BE_DAMAGED, self, sender, args);
	return 0;
}

//����ɱ
int AISystem::OnBeKilled(IKernel * pKernel,const PERSISTID & self,const PERSISTID & sender,const IVarList & args)
{
	if (!pKernel->Exists(self))
	{
		return 0;
	}
    AISystem::Instance()->GetAITemlate(pKernel, self).OnProcess(pKernel, AI_RULE_ONDEAD, self, sender, args);

	return 0;
}

//��ʼѲ��
int AISystem::OnPatrol(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args)
{
	if (!pKernel->Exists(self))
	{
		return 0;
	}
    AISystem::Instance()->GetAITemlate(pKernel, self).OnProcess(pKernel, AI_RULE_PATROL, self, sender, args);
	return 0;
}

//�����¼�
int AISystem::OnRelive(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args)
{
	if (!pKernel->Exists(self))
	{
		return 0;
	}
    AISystem::Instance()->GetAITemlate(pKernel, self).OnProcess(pKernel, AI_RULE_RELIVE, self, sender, args);
	return 0;
}

// ʵ�ü��ܺ��ƶ�
int AISystem::OnFightOverMove(IKernel *pKernel, const PERSISTID &self, const PERSISTID &sender, const IVarList &args)
{
	// ����ƶ���Ϊ
	IGameObj* pAttacker = pKernel->GetGameObj(sender);
	if (NULL == pAttacker)
	{
		return 0;
	}

	// �����Ƿ��ƶ�
	int nFightMoveRate = pAttacker->QueryInt(FIELD_PROP_FIGHT_MOVE_RATE);
	int nRandomRate = util_random_int(100);
	if (nRandomRate >= nFightMoveRate)
	{
		return 0; 
	}

	// �ƶ��ĵľ���
	float nDis = pAttacker->QueryFloat(FIELD_PROP_FIGHT_MOVE_DIS);
	PERSISTID target = pAttacker->QueryObject(FIELD_PROP_AITARGET_OBEJCT);
	IGameObj* pTargetObj = pKernel->GetGameObj(target);
	if (NULL == pTargetObj)
	{
		return 0;
	}
//	float fDesX, fDesY, fDesZ;
	// Ѱ���ƶ�������
// 	if (SkillEventExecuteUtil::RandomFindAroundPos(pKernel, pTargetObj, 10, nDis, nDis, fDesX, fDesY, fDesZ))
// 	{
// 		AIFunction::StartPatrolToPoint(pKernel, sender, fDesX, fDesZ, false);
// 	}

	// ������Ϊ
	AISystem::Instance()->GetAITemlate(pKernel, self).OnProcess(pKernel, AI_RULE_ESCAPE, self, sender, args);
	return 0;
}

//��Ҹ���
int AISystem::OnPlayerRelive(IKernel * pKernel, const PERSISTID & self, const PERSISTID & sender, const IVarList & args)
{
	CVarList temp;
	CVarList result;
	pKernel->GetAroundList(self, 100.0f, TYPE_NPC, 20, result);
    LoopBeginCheck(b);
	for(int i = 0; i < static_cast<int>(result.GetCount()); ++i)
	{
        LoopDoCheck(b);
		PERSISTID npc = result.ObjectVal(i);
        IGameObj *pNpc = pKernel->GetGameObj(npc);
        if(pNpc == NULL)
        {
            continue;
        }
		if (pNpc->QueryInt("NpcType") >= NPC_TYPE_FUNCTION)
		{
			continue;
		}
		if ( pKernel->Exists(npc) )
		{
			if(pNpc->QueryInt("Dead") < 1)
				OnSpring(pKernel, npc, self, temp);
		}
	}

	return 0;
}

// ��������һ��
int AISystem::OnAutoSpring(IKernel * pKernel, const PERSISTID & self)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return 0;
	}

	//�����Χ��ҽ���һ�ִ���
	float SpringRange = AISystem::GetSpringRange(pSelfObj);;
	//�鿴��Χ�����
	CVarList arg_list;
	CVarList temp;
	pKernel->GetAroundList(self, SpringRange, TYPE_PLAYER|TYPE_NPC, 40, arg_list);
    LoopBeginCheck(c);
	for ( size_t i = 0; i < arg_list.GetCount(); ++i )
	{
        LoopDoCheck(c);
		PERSISTID player = arg_list.ObjectVal(i);
		if ( pKernel->Exists(player) )
		{
			OnSpring(pKernel, self, player, temp);
		}
	}
	return 0;
}

//AI������Ϊ��������
int	AISystem::OnActionEnd(IKernel* pKernel, const PERSISTID& self, int msg)
{	
    AISystem::Instance()->GetAITemlate(pKernel,self).OnBasicActionMsg(pKernel, self, msg);
	return 0;
}

bool AISystem::ClearFight(IKernel* pKernel, const PERSISTID& self)
{
    IGameObj *pSelf = pKernel->GetGameObj(self);
    if(pSelf == NULL)
    {
        return false;
    }

    //�����������
    pSelf->SetObject("AITargetObejct", PERSISTID());
    pSelf->SetInt("LastAttackTime", 0);

    //ɾ��ս��������
    if (pKernel->FindHeartBeat(self,"AIHeartBeat::HB_AIFighting"))
    {
        pKernel->RemoveHeartBeat(self,"AIHeartBeat::HB_AIFighting");
    }

    //���������Ϣ
    IRecord* record = pSelf->GetRecord(NPC_SKILL_REC);
    if (record == NULL)
    {
        return false;
    }
    int rows = record->GetRows();
    LoopBeginCheck(d);
    for (int i = 0; i < rows; i++)
    {
        LoopDoCheck(d);
        record->SetInt(i, BOSS_SKILL_VALUE_CUR, 0);//��ǰȨֵ����
    }

    return true;
}

// ���ܰ��Ƿ����
bool AISystem::IsSkillPkgExist(const char* skill_pkg)
{
	std::map<std::string, NpcSkillPackage>::iterator pakIter = AISystem::m_pAISystem->m_mapSkillPackage.find(skill_pkg);
	if (pakIter == m_pAISystem->m_mapSkillPackage.end())
	{
		return false;
	}

	return true;
}
 const char*  AISystem::GetNpcNomalSkill(const char* skillCfg)
{
	std::map<std::string, NpcSkillPackage>::iterator pakIter = AISystem::m_pAISystem->m_mapSkillPackage.find(skillCfg);
	if (pakIter != m_pAISystem->m_mapSkillPackage.end())
	{
		return pakIter->second.normalSkill.c_str();
	}

	return NULL;
}

// ��ȡ������Χ
 float AISystem::GetSpringRange(IGameObj* pSelfObj)
 {
	 if (NULL == pSelfObj || pSelfObj->GetClassType() != TYPE_NPC)
	 {
		 return 0.0f;
	 }

	 float fSpringRange = pSelfObj->QueryFloat("SpringRange");
	 if (fSpringRange >= MAX_SPRING_RANGE)
	 {
		 fSpringRange = MAX_SPRING_RANGE;
	 }
	 return fSpringRange;
 }

 // ȡ�õȼ����԰�
 const NpcLevelPackage* AISystem::GetNpcLevelPackage(const char *p_config_id, const int n_level)
 {
	 if (StringUtil::CharIsNull(p_config_id)
		 || n_level <= 0)
	 {
		 return NULL;
	 }

	 struct LevelPackageFinder 
	 {
		 LevelPackageFinder(const char *_s_key, const int _n_value) : 
		 s_key(_s_key), n_value(_n_value)
		 {}

		 std::string s_key;
		 int n_value;

		 bool operator () (const NpcLevelPackage& cfg) const
		 {
			 return (strcmp(cfg.s_config_id.c_str(), s_key.c_str()) == 0 && 
				 (n_value >= cfg.n_lower_level && n_value <= cfg.n_upper_level));
		 }
	 };

	 NpcLevelPackageVec::const_iterator find_it = find_if(
		 m_pAISystem->m_NpcLevelPackageVec.begin(),
		 m_pAISystem->m_NpcLevelPackageVec.end(),
		 LevelPackageFinder(p_config_id, n_level));
	 if (find_it == m_pAISystem->m_NpcLevelPackageVec.end())
	 {
		 return NULL;
	 }

	 return &(*find_it);
 }

 // ȡ��Npc���԰�
 const int AISystem::GetNpcPackage(IGameObj* pSelfObj)
 {
	 if (NULL == pSelfObj)
	 {
		 return 0;
	 }

	 // ���԰��ȼ�
	 const int n_npc_package_lvl = pSelfObj->QueryInt(FIELD_PROP_NPC_PACKAGE_LEVEL);
	 if (n_npc_package_lvl > 0)
	 {
		 // ȡ��
		 const NpcLevelPackage *cfg = GetNpcLevelPackage(pSelfObj->GetConfig(), n_npc_package_lvl);
		 if (NULL == cfg)
		 {
			 return pSelfObj->QueryInt(FIELD_PROP_NPC_BASE_PACKAGE);
		 }
		 else
		 {
			 return cfg->n_package_id;
		 }
	 }
	 else
	 {
		 return pSelfObj->QueryInt(FIELD_PROP_NPC_BASE_PACKAGE);
	 } 
 }

 //��������������
int AISystem::OnCopyDataComplete(IKernel *pKernel, const PERSISTID &self, const PERSISTID &sender, const IVarList &args)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return 0;
	}

	if (!pSelfObj->FindAttr(FIELD_PROP_AITEMPLATE))
	{
		return 0;
	}

	int nAITemplate = pSelfObj->QueryInt(FIELD_PROP_AITEMPLATE);
	if (AI_TEMP_BATTLENPC != nAITemplate && AI_TEMP_ARENA_NPC != nAITemplate && AI_TEMP_INTERCEPTPLAYER != nAITemplate)
	{
		return 0;
	}

// 	IRecord* pSkillRec = pSelfObj->GetRecord(FIELD_RECORD_BATTLE_SKILL_REC);
// 	if (NULL == pSkillRec)
// 	{
// 		return 0;
// 	}
// 
// 	pSkillRec->ClearRow();
// #ifndef FSROOMLOGIC_EXPORTS
// // 	if (NULL != OffLineModule::m_pOffLineModule)
// // 	{
// // 		OffLineModule::m_pOffLineModule->AddSkillToRec(pKernel, pSelfObj, pSkillRec);
// // 	}
// #endif
// 
// 	// Ԥ������ҵľ�λ�ȼ�,�϶���0��
// 	int nPeerageLevel = pSelfObj->QueryInt(FIELD_PROP_PEERAGE_LEVEL);
// 	// ����Ԥ����������ݵ���Ӫսnpc����һ��ʵ��buff
// 	if (AI_TEMP_BATTLENPC == nAITemplate && 0 == nPeerageLevel)
// 	{
// 		const char* strStrenBuff = EnvirValueModule::EnvirQueryString(ENV_VALUE_BATTLE_NPC_STREN_BUFF);
// 		FightInterfaceInstance->AddBuffer(pKernel, self, self, strStrenBuff);
// 	}
	return 0;
}