//--------------------------------------------------------------------
// �ļ���:      AITemplateBase.cpp
// ��  ��:      NPC����ģ��
// ˵  ��:
// ��������:    2014��12��31��
// ������:       
// �޸���:        
//    :       
//--------------------------------------------------------------------
#include "../AISystem.h"
#include "../AIDefine.h"
#include "../../AI/Rule/AIRuleManage.h"
#include "AITemplateBase.h"
#include "../Rule/AIRuleBase.h"
#include "utils/util_func.h"
#include "../AIFunction.h"
#include "FsGame/Define/ModifyPackDefine.h"
#include "FsGame/SystemFunctionModule/StaticDataQueryModule.h"
#include "FsGame/Define/StaticDataDefine.h"
#include "FsGame/CommonModule/PropRefreshModule.h"
#include "FsGame/Interface/FightInterface.h"
#include <time.h>
#include "FsGame/Define/GameDefine.h"
//#include "FsGame/SocialSystemModule/BattleTeamModule.h"
#include "utils/string_util.h"
#include "utils/custom_func.h"
//#include "FsGame/SceneBaseModule/SecretSceneModule.h"
#include "FsGame/SystemFunctionModule/MotionModule.h"
//#include "FsGame/Define/DeadManagerDefine.h"
#include "FsGame/CommonModule/EnvirValueModule.h"
//#include "../../BossRefreshModule.h"
#include "Define/ServerCustomDefine.h"

// ��ʱ����
static const char* DEFINE_STRING_SCENE_NAME = "scene_";

//���캯��
AITemplateBase::AITemplateBase()
{
	m_templateType = AI_TEMP_BASE;

	RegistEventRule(AI_RULE_SPRING, AIRS_SPRING_BASE);
	//ѡ�񹥻�����AI����
	RegistEventRule(AI_RULE_SEL_TARTGET, AIRS_SEL_TARGET_BASE); 
	RegistEventRule(AI_RULE_BE_DAMAGED, AIRS_BE_DAMAGED_BASE);
	RegistEventRule(AI_RULE_PATROL, AIRS_PATROL_BASE);
}

//��ʼ��
bool AITemplateBase::Init(IKernel* pKernel)
{
    return true;
}

//��������
AITemplateBase::~AITemplateBase()
{
	m_EventRuleVct.clear();
}

// ��ñ�ģ������
const AI_TEMPLATE_TYPE AITemplateBase::GetTemplateType()
{
    return m_templateType;
}

//ע�����ģ�崦�����
bool AITemplateBase::RegistEventRule(AI_RULE_TYPE nEvent, int nRule)
{
    if(m_EventRuleVct.empty())
    {
        m_EventRuleVct.resize(AI_RULE_MAX, -1);
    }

    if (nEvent < 0 || nEvent >= AI_RULE_MAX)
    {
        return false;
    }

    m_EventRuleVct[nEvent] = nRule;

    return true;
}


//ģ�洦�����,ִ��AI
int AITemplateBase::OnProcess(IKernel *pKernel, AI_RULE_TYPE ruleType, const PERSISTID &self, const PERSISTID &sender, const IVarList &args)
{
	if (!pKernel->Exists(self))
	{
		return AI_RT_IGNORE;
	}
    
    //�����û������������
    if(ruleType < 0 || ruleType >= AI_RULE_MAX)
    {
        return AI_RT_IGNORE;
    }

    //�����û��ע��ִ���߼�AI
    if(m_EventRuleVct[ruleType] == -1)
    {
        return AI_RT_IGNORE;
    }

    //ִ��AI����
    return AIRuleManage::GetRuleItem(ruleType, m_EventRuleVct[ruleType]).DoRule(pKernel, self, sender, args, *this);

}

//ÿ���ƶ���ɺ�ص�
int AITemplateBase::OnBasicActionMsg(IKernel * pKernel , const PERSISTID & self , int msg , const IVarList & args)
{
	if (!pKernel->Exists(self))
		return AI_RT_IGNORE;

    IGameObj *pSelf = pKernel->GetGameObj(self);
    if(pSelf == NULL)
    {
        return AI_RT_IGNORE;
    }

	//�Ѿ�����
	if (pSelf->QueryInt("Dead") > 0)
		return AI_RT_IGNORE;

	int cur_state = pSelf->QueryInt("CurState");
	if (cur_state == AI_STATE_PATROL)
	{
		pSelf->SetDataInt("PatrolRestTime", 0);
		pSelf->SetInt("SubState",AI_PATROL_WAIT);
	}
	else if (cur_state == AI_STATE_BACK)
	{
        //�س�������ɣ����������
		IRecord* pAttackerRecord = pSelf->GetRecord("AttackerList");
		if (pAttackerRecord != NULL)
		{
			pAttackerRecord->ClearRow();
		}
        
        //�Ƿ�ҪѲ��
		int iBornStand = pSelf->QueryInt("BornStand");
		if (iBornStand != 1)
		{
			BeginPatrol(pKernel,self);
		}
		else
		{
            //����Ϊ����״̬
		    ChangeState(pKernel, self, AI_STATE_BORN);
	    }

        //���»ָ�Ѫ��
		int nCantRestore = pSelf->QueryInt(FIELD_PROP_CANT_RESTORE_HP);
		if (0 == nCantRestore)
		{
			int64_t max_hp = pSelf->QueryInt64("MaxHP");
			pSelf->SetInt64("HP", max_hp);
		}
		//�����������
    	ADD_COUNT_BEAT(pKernel, self, "AIHeartBeat::HB_AIRatate", 1000, 1);
	}

	return AI_RT_SUCCESS;
}
//�ı�AI״̬
int AITemplateBase::ChangeState(IKernel * pKernel, const PERSISTID & self, int state) const
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return 0;
	}

	pSelfObj->SetInt("CurState", state);
	return AI_RT_SUCCESS;
}

//���ó���
int AITemplateBase::OnBorn(IKernel *pKernel, const PERSISTID &self, const PERSISTID &sender, const IVarList &args)
{
	if ( !pKernel->Exists(self) )
	{
		return 0;
	}
	
	IGameObj * pSelfObj = pKernel->GetGameObj(self);
	if ( pSelfObj == NULL )
	{
		return 0;
	}

    //��������
    BornNotice(pKernel, self);

	//��̬NPC�����ƶ�
	if ( pSelfObj->QueryInt("NpcType") >= NPC_TYPE_FUNCTION )
	{
		return 1;
	}
    
    //����ʱĬ��CurState = 0;
	if (pSelfObj->QueryInt("CurState") == AI_STATE_BORN)
	{
        //���ó�����ʼ�����԰�
		AITemplateBase::InitPropAfterBorn( pKernel, self );
	
		//û���ƶ��ٶȣ��Ͳ�������Ѱ·
		len_t fWalkSpeed = pSelfObj->QueryFloat("RunSpeed");
		int iBornStand = pSelfObj->QueryInt("BornStand");
		if (!FloatEqual( fWalkSpeed, 0.0f) && iBornStand == 0)
		{
            //����Ѳ��·��
			AITemplateBase::InitRandomPatrolPath(pKernel, self);
		}
		
        //��ʼ�������ʼ����
		AITemplateBase::InitSkillAfterBorn(pKernel, self);

        //������ײ���
		pSelfObj->SetInt("PathThrough", 0);
		pSelfObj->SetInt("PathGrid", 1);

        //���׷����Χ���ڴ�����Χ��׷����Χ�͵��ڴ�����Χ.
        if(pSelfObj->FindAttr("ChaseRange"))
        {
            float chaseRange = pSelfObj->QueryFloat("ChaseRange");
			float springRange = AISystem::GetSpringRange(pSelfObj);
            if(chaseRange <= springRange)
            {
                pSelfObj->SetFloat("ChaseRange", springRange);
            }
        }

		//����Ϊ��Ѫ
		int64_t max_hp = pSelfObj->QueryInt64("MaxHP");
		pSelfObj->SetInt64("HP", max_hp);

        // ����ʱ��(��Ҫ���ڳ���ʱ����һ������ſ�ʼִ��AI)
        int nBornTime = pSelfObj->QueryInt("BornTime");
        if (nBornTime > 0)
        {
            ADD_COUNT_BEAT(pKernel, self, "AIHeartBeat::HB_AIBornEnd", nBornTime, 1);
        }
        else
        {
            BeginPatrol(pKernel,self);
        }
	}
	
	return AI_RT_SUCCESS;
}

//��ʼѲ��
int AITemplateBase::BeginPatrol(IKernel * pKernel, const PERSISTID & self) const
{
    IGameObj * pSelfObj = pKernel->GetGameObj(self);
    if ( pSelfObj == NULL )
    {
        return 0;
    }
    
	// û��Ѳ�߱�Ĳ���Ѳ��
    IRecord *pPatrolPointRec = pSelfObj->GetRecord("PatrolPointRec");
    if(pPatrolPointRec == NULL)
    {
        return 0;
    }
	if(pPatrolPointRec->GetRows() == 0)
	{
		return 0;
	}
	//����Ѳ��״̬
	ChangeState(pKernel, self, AI_STATE_PATROL);

	int nCantRestore = pSelfObj->QueryInt(FIELD_PROP_CANT_RESTORE_HP);
	if (0 == nCantRestore)
	{
		//���»ָ�Ѫ��
		int64_t max_hp = pSelfObj->QueryInt64("MaxHP");
		pSelfObj->SetInt64("HP", max_hp);
	}
	
	//��ʼ����ǰѲ�߲���
	if (!pSelfObj->FindData("PatrolStep"))
    {
        pSelfObj->AddDataInt("PatrolStep", -1);
    }
    else
    {
        pSelfObj->SetDataInt("PatrolStep", -1);
    }
    

	//��ʼ��Ѳ����Ϣʱ��
	if (!pSelfObj->FindData("PatrolRestTime"))
    {
        pSelfObj->AddDataInt("PatrolRestTime", 0);
    }
    else
    {
        pSelfObj->SetDataInt("PatrolRestTime", 0);
    }

	//���û��߱�־
	if (!pSelfObj->FindData("PatrolBack"))
    {
        pSelfObj->AddDataInt("PatrolBack", 0);
    }
    else
    {
        pSelfObj->SetDataInt("PatrolBack", 0);
    }
	    

	int PatrolFreq = 2 * 1000 + util_random_int(2000);
	
	//����Ѳ������
	if (!pKernel->FindHeartBeat(self, "AIHeartBeat::HB_AIPatrol"))
	{
		pKernel->AddHeartBeat(self, "AIHeartBeat::HB_AIPatrol", PatrolFreq);
	}
	return AI_RT_SUCCESS;
}

//����Ѳ��
int AITemplateBase::EndPatrol(IKernel * pKernel , const PERSISTID & self) const
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return 0;
	}

	pSelfObj->SetInt("SubState", AI_PATROL_WAIT);

	//ɾ��Ѳ������
	if (pKernel->FindHeartBeat(self,"AIHeartBeat::HB_AIPatrol"))
	{
		MotionModule::MotionStop(pKernel, self);
		pKernel->RemoveHeartBeat(self,"AIHeartBeat::HB_AIPatrol");
	}
	return AI_RT_SUCCESS;
}

//���������Ѳ�ߵ�
void AITemplateBase::InitRandomPatrolPath(IKernel * pKernel, const PERSISTID & self, bool doStop ) const
{
	IGameObj * pSelfObj = pKernel->GetGameObj(self);
	if ( pSelfObj == NULL )
	{
		return;
	}

	//�鿴׷����Χ���ɳ�ʼ·��
	float radius = pSelfObj->QueryFloat("PatrolRange");
	if ( FloatEqual(radius, 0.0f) )
	{
		return;
	}

	//�Ѿ�����Ѳ��
    IRecord *pPatrolPointRec = pSelfObj->GetRecord("PatrolPointRec");
    if(pPatrolPointRec == NULL)
    {
        return;
    }
	int rows = pPatrolPointRec->GetRows();
	if (rows > 0)
		return;

	// 4������,�Ƕ�Ԥ����
	const float fAngle[4] = {PI_4, -PI_4 + PI, PI_4 + PI, -PI_4};

	// ���ڱ�֤��ȡ�����޲�ͬ
	int bUsed[4] = {0, 0, 0, 0};

	float random_orient = 0.0f;
	float random_radius = 0.0f;

	// �ڰ뾶��Χ���������point_num��·����
	float x = pSelfObj->GetPosiX();
	float z = pSelfObj->GetPosiZ();
	float y = pSelfObj->GetPosiY();

	float lastX = x;
	float lastZ = z;
	float lastY = y;

	float newX = 0.0f;
	float newZ = 0.0f;
	float newY = 0.0f;

	int orient_index = 0;

    LoopBeginCheck(a);
	for (int i = 0; i < AI_PARTOL_COUNT; ++i)
	{
        LoopDoCheck(a);
		// �õ�����ĽǶȺͰ뾶(��������ʽ��֤ÿ��������ڲ�ͬ��4������֮һ)
		orient_index = util_random_int(4);

        LoopBeginCheck(b);
		while (bUsed[orient_index] > 0)
		{
            LoopDoCheck(b);
			orient_index = util_random_int(4);
		}

		bUsed[orient_index] = 1;

		random_orient = fAngle[orient_index] + util_random_float(PI_4);
		random_radius = 0.75f * radius + util_random_float(0.25f * radius);

		// ��������
		newX = x + random_radius * cos(random_orient);
		newZ = z + random_radius * sin(random_orient);
		newY = GetCurFloorHeight(pKernel, newX, lastY, newZ);

		// �жϱ�������ĵ���ϴεĵ�֮���Ƿ��������
		float bx, by, bz;
		if (!pKernel->TraceLineWalk(2.0f, lastX, 
			lastY, lastZ, newX, newZ, bx, by, bz))
		{
			newX = bx;
			newZ = bz;
			newY = by;
		}

		int newIndex = pKernel->GetGridIndexByPos(newX,newZ);
		int lastIndex = pKernel->GetGridIndexByPos(lastX,lastZ);
		if (newIndex == lastIndex)
		{
			continue;
		}

		// ���Ѳ�߱������ϸ�������
		lastX = newX;
		lastZ = newZ;
		lastY = newY;

		// ���һ��Ѳ�ߵ��ͣ��ʱ��(ms) 5-10��
        int stop_time = 1000 * (5 + util_random_int(5));
        if (!doStop)
        {
            //ǿ��ʯ�ؾ���Ҫ��Ѳ��ͣ��ʱ�����
            stop_time = 1000;
        }
		
		CVarList var;
		var << lastX << lastZ << stop_time;
		pPatrolPointRec->AddRowValue(-1, var);
	}

    //����Ѳ��·��ʧ��
    if(pPatrolPointRec->GetRows() < 1)
    {
        pSelfObj->SetInt("BornStand", 1);
    }
}

//���ù����ʼ����
void AITemplateBase::InitPropAfterBorn(IKernel* pKernel, const PERSISTID& self) const
{
	IGameObj * pSelfObj = pKernel->GetGameObj(self);
	if ( pSelfObj == NULL )
	{
		return;
	}

	//�Ȼ�ȡ���ù���Ļ�������
	int strBasePackage = AISystem::GetNpcPackage(pSelfObj);
	if ( strBasePackage > 0 )
	{
		MapPropValue mapPropValue;
		//��ӻ���ʵ��
		CVarList vPropNameList;
		CVarList vPropValueList;

		//�鿴��Ӧ��������
		StaticDataQueryModule::m_pInstance->GetOneRowData(STATIC_DATA_NPC_BASE_PACK, 
			util_int_as_string(strBasePackage).c_str(), vPropNameList, vPropValueList);

        LoopBeginCheck(c);
		for ( size_t i = 0; i < vPropNameList.GetCount(); ++i )
		{
            LoopDoCheck(c);
			// ��ȡ���㹫ʽ����
			float fVal = (float)vPropValueList.IntVal(i);
			if ( FloatEqual(fVal, 0.0f) )
			{
				continue;
			}

			PropRefreshModule::m_pInstance->CountRefreshData(pKernel, self, 
				vPropNameList.StringVal(i), fVal, 0, mapPropValue);
		}

		PropRefreshModule::m_pInstance->RefreshData(pKernel, self, mapPropValue, EREFRESH_DATA_TYPE_ADD);
	}

	float fX = pSelfObj->GetPosiX();
	float fZ = pSelfObj->GetPosiZ();
	float fOrient = pSelfObj->GetOrient();

	//���ó�����
	if ( pSelfObj->FindAttr("BornX") )
	{
		pSelfObj->SetFloat("BornX", fX);
	}

	if ( pSelfObj->FindAttr("BornZ") )
	{
		pSelfObj->SetFloat("BornZ", fZ);
	}

	if ( pSelfObj->FindAttr("BornOrient") )
	{
		pSelfObj->SetFloat("BornOrient", fOrient);
	}

	//����NPC HPΪMAXHP
	pSelfObj->SetInt64("HP", pSelfObj->QueryInt64("MaxHP"));
	
	//�����������߱���
	if ( pSelfObj->QueryInt("Initiative") == 1 )
	{
		pSelfObj->SetInt("NoSpring", 0);
	}
	else
	{
		pSelfObj->SetInt("NoSpring", 1);
	}
}

//���ù����ʼ����
void AITemplateBase::InitSkillAfterBorn(IKernel* pKernel, const PERSISTID& self) const
{
    IGameObj * pSelfObj = pKernel->GetGameObj(self);
    if ( pSelfObj == NULL )
    {
        return;
    }
    
	//���ݼ��ܰ����ö�Ӧ��������
	const char* skillStr = pSelfObj->QueryString("SkillStr");
	if (!StringUtil::CharIsNull(skillStr))
	{
		AIFunction::InitSkill(pKernel, self, skillStr);
	}
	
	len_t minAttackDist = 0.0f;
	len_t maxAttackDist = 0.0f;
	FightInterfaceInstance->ReadyUseRandomSkill(pKernel, self, minAttackDist, maxAttackDist);

	maxAttackDist = __max(2.0f, maxAttackDist);
	pSelfObj->SetFloat("MinAttackDist", minAttackDist);
	pSelfObj->SetFloat("MaxAttackDist", maxAttackDist);
}

//NPC�������棨���������й��ҵ�BOSS��
void AITemplateBase::BornNotice(IKernel *pKernel, const PERSISTID &self) const
{
    IGameObj * pSelfObj = pKernel->GetGameObj(self);
    if ( pSelfObj == NULL )
    {
        return;
    }

#ifndef FSROOMLOGIC_EXPORTS
	// �Ƿ�Ϊ��Ҫ����ͨ��ĳ���
// 	if (!BossRefreshModule::m_pBossRefreshModule->IsRefreshBossScene(pKernel))
// 	{
// 		return;
// 	}
#endif 

// 	int sceneID = pKernel->GetSceneId();
//     int npcType = pSelfObj->QueryInt("NpcType");
// 	const char* strBossId = pSelfObj->GetConfig();
//     if(npcType == NPC_TYPE_BOSS)
//     {
//         std::string sceneName = DEFINE_STRING_SCENE_NAME;
//         sceneName.append(StringUtil::IntAsString(sceneID));
// 
// 		CVarList msg;
// 		msg << SERVER_CUSTOMMSG_BOSS_BORN << strBossId << sceneID;
// 		pKernel->CustomByWorld(msg);
//     }
}