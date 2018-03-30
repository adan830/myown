//--------------------------------------------------------------------
// �ļ���:      AIRFightBase.cpp
// ��  ��:      ����ս������
// ˵  ��:
// ��������:    2014��12��31��
// ������:       
// �޸���:        
//    :       
//--------------------------------------------------------------------
#include "AIRFightBase.h"
#include "FsGame/NpcBaseModule/AI/AISystem.h"
#include "utils/util_func.h"
#include "FsGame/NpcBaseModule/AI/AIDefine.h"
#include "FsGame/Interface/FightInterface.h"
#include "utils/string_util.h"
#include <time.h>

AIRFightBase::AIRFightBase()
{
	m_ruleType = AI_RULE_FIGHT;
	m_subRuleCode = AIRS_FIGHT_BASE;
}

int AIRFightBase::DoRule(IKernel * pKernel,const PERSISTID & self,const PERSISTID & sender,const IVarList & args, AITemplateBase &templateWrap)
{
    IGameObj *pSelf = pKernel->GetGameObj(self);
    if(pSelf == NULL)
    {
        return 0;
    }

	//�Ѿ�����
	if (pSelf->QueryInt("Dead") > 0)
	{
		return 0;
	}

    //����״̬
	if (pSelf->QueryInt("BornState") > 0)
	{
		return 0;
	}

	//��鹥����
	int ai_ret = templateWrap.OnProcess(pKernel, AI_RULE_CHECK_ATTACKERS, self, sender, args);
// 	if(ai_ret == AI_RT_END_FIGHT)
// 	{
// 		return templateWrap.OnProcess(pKernel, AI_RULE_END_FIGHT, self, sender, args);
// 	}

	//ѡ��Ŀ��
	ai_ret = templateWrap.OnProcess(pKernel, AI_RULE_SEL_TARTGET, self, sender, args);
	if(ai_ret == AI_RT_END_FIGHT)
	{
		return templateWrap.OnProcess(pKernel, AI_RULE_END_FIGHT, self, sender, args);
	}

	//��ʼ������Ϊ
	return DoFight(pKernel, self, sender, args, templateWrap); 
}

int AIRFightBase::DoFight(IKernel * pKernel, const PERSISTID & self, const PERSISTID & sender,const IVarList & args, AITemplateBase &templateWrap)
{
    IGameObj *pSelf = pKernel->GetGameObj(self);
    if(pSelf == NULL)
    {
        return 0;
    }

	//�õ�Ŀ�����
	PERSISTID target = pSelf->QueryObject("AITargetObejct");
	if(!pKernel->Exists(target))
	{
		return 0;
	}

    //����ʱ�似��Ȩֵ�ۼ�
    IRecord *npcSkillRec = pSelf->GetRecord(NPC_SKILL_REC);
    if(npcSkillRec == NULL)
    {
        return AI_RT_NULL;
    }
    NpcSkill skillInfoTemp;
    int skills = npcSkillRec->GetRows();
    LoopBeginCheck(a);
    for (int i = 0; i < skills; i++)
    {
        LoopDoCheck(a);
        if (!AISystem::GetSkillConfig(pKernel, self, npcSkillRec->QueryString(i, BOSS_SKILL_ID), skillInfoTemp))
        {
            continue;
        }
        npcSkillRec->SetInt(i, BOSS_SKILL_VALUE_CUR, (npcSkillRec->QueryInt(i, BOSS_SKILL_VALUE_CUR) + skillInfoTemp.time));
    }

    //ȡ�ù�������
    const char* resultSkill = AIRFightBase::GetUseSkill(pKernel, self);
    if (StringUtil::CharIsNull(resultSkill))
    {
        return AI_RT_NULL;
    }

    //ȡ�õ�ǰ�������ܿ��þ���
    len_t fMinDis = 0.0f;
    len_t fMaxDis = 0.0f;
    FightInterfaceInstance->ReadyUseRandomSkill(pKernel, self, fMinDis, fMaxDis, resultSkill);
    if(fMaxDis <= 0) //��֤��С�ڵ���0
    {
        fMaxDis = 2;
    }
    pSelf->SetFloat("MinAttackDist", fMinDis);
    pSelf->SetFloat("MaxAttackDist", fMaxDis);

    //�鿴�Ƿ񵽴�׷������
    if (templateWrap.OnProcess(pKernel, AI_RULE_CHASE_TARGET, self, target, args) != AI_RT_SUCCESS)
    {
        return AI_RT_IGNORE;
    }

    //����ϴι�����ȥ��ʱ���Ƿ����㹥��Ƶ��
    int newlastAttackTime = pSelf->QueryInt("LastAttackTime") + args.IntVal(0);
    if(newlastAttackTime < pSelf->QueryInt("FightAttackFreq"))
    {
        //�����ϴι�����ȥ��ʱ��
        pSelf->SetInt("LastAttackTime", newlastAttackTime);
        return AI_RT_IGNORE;
    }
    pSelf->SetInt("LastAttackTime", 0);

	if (strcmp(resultSkill, "") != 0)
	{
        //ʹ�ù�������
		FightInterfaceInstance->UseSkill(pKernel, self, resultSkill, CVarList() << target);        

        //���õ�ǰʹ�ù������ܵ�
        int row = npcSkillRec->FindString(BOSS_SKILL_ID, resultSkill);
        if (row >= 0)
        {
            npcSkillRec->SetInt(row, BOSS_SKILL_VALUE_CUR, 0);
        }

        //ÿ�ι���ʱ�ۼƼ���Ȩ�أ�����������õ�
        NpcSkill skillInfoTempConfig;
        int skillCount = npcSkillRec->GetRows();
        LoopBeginCheck(b);
        for (int i = 0; i < skillCount; i++)
        {
            LoopDoCheck(b);
            //����õ�
            if(i == row)
            {
                continue;
            }

            if (!AISystem::GetSkillConfig(pKernel, self, npcSkillRec->QueryString(i, BOSS_SKILL_ID), skillInfoTempConfig))
            {
                continue;
            }
                
            npcSkillRec->SetInt(i, BOSS_SKILL_VALUE_CUR, (npcSkillRec->QueryInt(i, BOSS_SKILL_VALUE_CUR) + skillInfoTempConfig.addWeighting));
        }

        //��¼��������ʱ��
        IRecord *pAttackerList = pSelf->GetRecord("AttackerList");
        if(pAttackerList == NULL)
        {
            return AI_RT_NULL;
        }
        row = pAttackerList->FindObject(AI_ATTACKER_REC_OBJECT, target);
        if(row >= 0)
        {
            pAttackerList->SetInt64(row, AI_ATTACKER_REC_LASTTIME, ::time(NULL));
        }
	}

	return AI_RT_SUCCESS;
}

//��ȡ���ü���
const char* AIRFightBase::GetUseSkill(IKernel* pKernel, const PERSISTID& self)
{
    IGameObj *pSelf = pKernel->GetGameObj(self);
    if(pSelf == NULL)
    {
        return "";
    }

    IRecord *npcSkillRec = pSelf->GetRecord(NPC_SKILL_REC);
    if(npcSkillRec == NULL)
    {
        return "";
    }

    int npcSkillRecLength = npcSkillRec->GetRows();
    if (npcSkillRecLength <= 0)
    {
        return "";
    }

    int64_t curHP = pSelf->QueryInt64("HP");
    int64_t maxHP = pSelf->QueryInt64("MaxHP");
    if(maxHP <= 0 || curHP <= 0)
    {
        return "";
    }
    int curHPPercent = (int) (100 * curHP / maxHP);

    const char *resultSkill = "";
    int priorTemp = -1;
    NpcSkill itemSkillTemp;
    LoopBeginCheck(c);
    for(int i = 0; i < npcSkillRecLength; ++i)
    {
        LoopDoCheck(c);
        if(!AISystem::GetSkillConfig(pKernel, self, npcSkillRec->QueryString(i, BOSS_SKILL_ID), itemSkillTemp))
        {
            continue;
        }

        //Ѫ����
        if(curHPPercent > itemSkillTemp.condition)
        {
            continue;
        }

        //��ǰȨֵ����
        if(npcSkillRec->QueryInt(i, BOSS_SKILL_VALUE_CUR) < itemSkillTemp.maxWeighting)
        {
            continue;
        }

        //��ȴ��
        if (FightInterfaceInstance->IsCoolDown(pKernel, self, itemSkillTemp.skillID.c_str()))
        {
            continue;
        }

        //ѡ�����ȼ���ߵ�
        if(itemSkillTemp.prior > priorTemp)
        {
            priorTemp = itemSkillTemp.prior;
            resultSkill = npcSkillRec->QueryString(i, BOSS_SKILL_ID);
        }
    }

    return resultSkill;
}
