//--------------------------------------------------------------------
// �ļ���:      AIROnDeadBase.cpp
// ��  ��:      ��������
// ˵  ��:
// ��������:    2014��12��31��
// ������:       
// �޸���:        
//    :       
//--------------------------------------------------------------------
#include <algorithm>
#include "utils/string_util.h"
#include "AIROnDeadBase.h"
#include "FsGame/NpcBaseModule/AI/AISystem.h"
#include "FsGame/NpcBaseModule/AI/AIDefine.h"
#include "FsGame/NpcBaseModule/AI/AIFunction.h"
//#include "FsGame/NpcBaseModule/BossRefreshModule.h"
#include "FsGame/SystemFunctionModule/StaticDataQueryModule.h"
#include "FsGame/Define/CommandDefine.h"
#include "utils/util_func.h"
#include "utils/custom_func.h"
//#include "FsGame/Define/OffLineDefine.h"
#include "FsGame/Define/StaticDataDefine.h"
#include "FsGame/Define/CommandDefine.h"
//#include "FsGame/Define/SceretSceneDefine.h"
#include "FsGame/Define/ServerCustomDefine.h"
#include "FsGame/Define/GameDefine.h"
//#include "FsGame/SocialSystemModule/OffLineHangUp.h"
#include "FsGame/Define/ToolBoxSysInfoDefine.h"
//#include "FsGame/Define/NationDefine.h"
//#include "FsGame\TradeModule\VipModule.h"
#include "FsGame\Define\SnsDefine.h"
//#include "FsGame/Define/DeadManagerDefine.h"
#ifndef FSROOMLOGIC_EXPORTS
#include "ItemModule/ToolItem/ToolItemModule.h"
#endif

AIROnDeadBase::AIROnDeadBase(void)
{
	m_ruleType = AI_RULE_ONDEAD;
	m_subRuleCode = AIRS_ONDEAD_BASE;
}

//ִ�иù���
int AIROnDeadBase::DoRule(IKernel * pKernel,const PERSISTID & self,const PERSISTID & sender,const IVarList & args, AITemplateBase &templateWrap)
{
    IGameObj *pSelf = pKernel->GetGameObj(self);
    if(pSelf == NULL)
    {
        return AI_RT_NULL;
    }

	MotionModule::MotionStop(pKernel, self);
	pKernel->ClearHeartBeat(self);
	templateWrap.ChangeState(pKernel, self, AI_STATE_DIED);

    //������ܱ�
	IRecord* pRecord = pSelf->GetRecord(NPC_SKILL_REC);
	if (pRecord != NULL)
	{
		pRecord->ClearRow();
	}
   

	//��ȡAI���� ������ʧʱ��
	pSelf->SetInt("LifeTime", 4000);

    //������ײ���
    pSelf->SetInt("PathThrough", 0);
    pSelf->SetInt("PathGrid", 0);

    //����ǳ���
    IGameObj *pSender = pKernel->GetGameObj(sender);
    if(pSender == NULL)
    {
        return AI_RT_NULL;
    }
    PERSISTID target = get_pet_master(pKernel, sender);
	if (!pKernel->Exists(target) || pKernel->Type(target) == TYPE_NPC)
	{
		return AI_RT_IGNORE;
	}

    //ȡ��������������
	CVarList varKillers;
    int dropType = GetNpcAscription(pKernel, self, target, varKillers);
	int nRows = (int)varKillers.GetCount();
    if(nRows <= 0)
    {
        varKillers << target;
    }
    LoopBeginCheck(a);
	for (int i = 0; i < nRows; ++i)
	{
        LoopDoCheck(a);
		PERSISTID killer = varKillers.ObjectVal(i);
        IGameObj *pKiller = pKernel->GetGameObj(killer);
        if(pKiller == NULL || pKiller->GetClassType() == TYPE_NPC)
        {
            continue;
        }

		//֪ͨ����ģ��
		//InformTask(pKernel, self, killer);

#ifndef FSROOMLOGIC_EXPORTS
		AISystem::m_pDropModule->AllocateDropAward(pKernel, killer, self, varKillers);
#endif // FSROOMLOGIC_EXPORTS
	}


	return AI_RT_SUCCESS;
}

//ȡ��NPC��������,��û������ǰȡ�õ�ֵ���Ǵ��
int AIROnDeadBase::GetNpcAscription(IKernel * pKernel, const PERSISTID & self, const PERSISTID & sender, IVarList &varKillers)
{
    assert((pKernel != NULL) && (!self.IsNull()));
    IGameObj *pSelf = pKernel->GetGameObj(self);
    if(pSelf == NULL)
    {
        return AI_RT_NULL;
    }

    //ȡ�õ������
    int dropType = pSelf->QueryInt("DropType");  
    switch (dropType)
    {
    //��һ��
    case AI_DROP_FIRST_ATTACKER:
        {
            IRecord *pAttackerList = pSelf->GetRecord("AttackerList");
            if(pAttackerList == NULL)
            {
                return AI_RT_NULL;
            }

			//ȡ�õ�һ�ι���ʱ��
			int64_t firstTime = 0;
			int64_t rowTime = 0;
			int row = -1;
			//ȡ�ù��������
			int rowLength = pAttackerList->GetRows();
			PERSISTID attacker;
            LoopBeginCheck(b);
			for(int i = 0; i < rowLength; ++i)
			{
                LoopDoCheck(b);
				rowTime = pAttackerList->QueryInt64(i, AI_ATTACKER_REC_FIRSTTIME);
				PERSISTID temp = pAttackerList->QueryObject(i, AI_ATTACKER_REC_OBJECT);
				PERSISTID master = get_pet_master(pKernel, temp);
				if (!pKernel->Exists(master))
				{
					continue;
				}

				if (rowTime > 0)
				{
					if (firstTime == 0)
					{
						firstTime = rowTime;
						row = i;
						attacker = master;
					}
					else 
					{
						if (rowTime < firstTime)
						{
							firstTime = rowTime;
							row = i;
							attacker = master;
						}
					}
				}
			}

			if(row > -1)
			{
				if (pKernel->Exists(attacker))
				{
					varKillers << attacker;
				}
			}
        }
        break;
    //���˺����ǰ��λ
    case AI_DROP_DAMAGE_MAX:
        {
			GetNpcAscriptionFront(pKernel, self, varKillers);
        }
        break;
    //���һ��
    case AI_DROP_LAST_ATTACKER:
        {
            IRecord *pAttackerList = pSelf->GetRecord("AttackerList");
            if(pAttackerList == NULL)
            {
                return AI_RT_NULL;
            }

            //ȡ�����һ�ι���ʱ��
            int64_t lastTime = 0;

            int64_t rowTime = 0;

            int row = -1;

            //ȡ�ù��������
            int rowLength = pAttackerList->GetRows();
            LoopBeginCheck(h);
            for(int i = 0; i < rowLength; ++i)
            {
                LoopDoCheck(h);
                rowTime = pAttackerList->QueryInt64(i, AI_ATTACKER_REC_LASTTIME);
                if(rowTime > lastTime)
                {
                    lastTime = rowTime;
                    row = i;
                }
            }

            if(row > -1)
			{
				varKillers << pAttackerList->QueryObject(row, AI_ATTACKER_REC_OBJECT);
			}
        }
        break;
    //Ĭ��
    default:
        {
            varKillers << pSelf->QueryObject("AITargetObejct");
        }
        break;
    }  

	return dropType;
}

//��ȡNPC�����˺�ǰnλ
int AIROnDeadBase::GetNpcAscriptionFront(IKernel * pKernel, const PERSISTID & self, IVarList &varKillers)
{
	IGameObj *pSelf = pKernel->GetGameObj(self);
	if(pSelf == NULL)
	{
		return AI_RT_NULL;
	}

	// ��ȡ���˺�ɱ�ָ�������Ϊ1
	int killerCnt = pSelf->QueryInt("DropToKillerCnt");
	killerCnt = killerCnt < 1 ? 1 : killerCnt;

	//ȡ�ù��������
	IRecord *pRec = pSelf->GetRecord("AttackerList");
	if (NULL == pRec)
	{
		return AI_RT_NULL;
	}

	DamageDataVec vecDamageList;
	int nRows = pRec->GetRows();

	vecDamageList.reserve(nRows);
	LoopBeginCheck(e);
	for (int i = 0; i < nRows;++i)
	{
		LoopDoCheck(e);
		DamageData data;
		data.player = pRec->QueryObject(i, AI_ATTACKER_REC_OBJECT);
		IGameObj* pPlayer = pKernel->GetGameObj(data.player);
		if (NULL == pPlayer || pPlayer->GetClassType() != TYPE_PLAYER)
		{
			continue;
		}
		data.nDamageVal = pRec->QueryInt(i, AI_ATTACKER_REC_DAMAGE);
		if (data.nDamageVal > 0)
		{
			vecDamageList.push_back(data);
		}
	}

	sort(vecDamageList.begin(), vecDamageList.end(), std::less<DamageData>());

	int nSize = (int)vecDamageList.size();
	int nCount = nSize <= killerCnt ? nSize : killerCnt;
	LoopBeginCheck(f);
	for (int i = 0; i < nCount;++i)
	{
		LoopDoCheck(f); 
		varKillers << vecDamageList[i].player;
	}
	
	return (int)varKillers.GetCount();
}

//֪ͨ����ģ������߼�
void AIROnDeadBase::InformTask(IKernel *pKernel, const PERSISTID &self, const PERSISTID &killer)
{
	//IGameObj* pSelfObj = pKernel->GetGameObj(self);
	//if (pSelfObj == NULL)
	//{
	//	return;
	//}

 //   struct teamDistance
 //   {
 //       float distance;
 //       PERSISTID obj;
 //   };
 //   std::map<int, teamDistance> informList;
 //   std::map<int, teamDistance>::iterator it;

 //   PERSISTID item;
 //   int itemTeamID = 0;
 //   IRecord *attackerRce = pSelfObj->GetRecord("AttackerList");
 //   if(attackerRce == NULL)
 //   {
 //       return;
 //   }
 //   int rowLength = attackerRce->GetRows();
 //   LoopBeginCheck(k);
 //   for(int i = 0; i < rowLength; ++i)
 //   {
 //       LoopDoCheck(k);
 //       if(attackerRce->QueryInt(i, AI_ATTACKER_REC_DAMAGE) == 0)
 //       {
 //           continue;
 //       }

 //       item = attackerRce->QueryObject(i, AI_ATTACKER_REC_OBJECT);
	//	// ���ﲻ��֪ͨ
	//	IGameObj* pItemObj = pKernel->GetGameObj(item);
	//	if (pItemObj == NULL || pItemObj->GetClassType() == TYPE_NPC)
	//	{
	//		continue;
	//	}

 //       if(pItemObj->FindAttr("TeamID") && pItemObj->QueryInt("TeamID") > 0)
 //       {
 //           itemTeamID = pItemObj->QueryInt("TeamID");
 //           it = informList.find(itemTeamID);
 //           float distance = pKernel->Distance2D(item, self);
 //           if(it != informList.end())
 //           {
 //               if(distance < it->second.distance)
 //               {
 //                   it->second.distance = distance;
 //                   it->second.obj = item;
 //               }
 //           }
 //           else
 //           {
 //               teamDistance td;
 //               td.distance = distance;
 //               td.obj = item;
 //               informList.insert(std::pair<int, teamDistance>(itemTeamID, td));
 //           }
 //       }
 //       else
 //       {
 //           pKernel->Command(item, self, CVarList() << COMMAND_QUEST_NPC_BEKILL);
 //       }
 //   }

 //   //�����ж����
 //   LoopBeginCheck(l);
 //   for(it = informList.begin(); it != informList.end(); ++it)
 //   {
 //       LoopDoCheck(l);
 //       pKernel->Command(it->second.obj, self, CVarList() << COMMAND_QUEST_NPC_BEKILL);
 //   }
}