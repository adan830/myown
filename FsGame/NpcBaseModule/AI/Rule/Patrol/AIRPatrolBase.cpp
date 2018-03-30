//--------------------------------------------------------------------
// �ļ���:      AIRPatrolBase.cpp
// ��  ��:      Ѳ�߹���
// ˵  ��:
// ��������:    2014��12��31��
// ������:       
// �޸���:        
//    :       
//--------------------------------------------------------------------
#include "AIRPatrolBase.h"
#include "FsGame/NpcBaseModule/AI/AISystem.h"
#include "FsGame/NpcBaseModule/AI/AIFunction.h"
#include "utils/util_func.h"
AIRPatrolBase::AIRPatrolBase()
{
	m_ruleType = AI_RULE_PATROL;
	m_subRuleCode = AIRS_PATROL_BASE;
}

int AIRPatrolBase::DoRule(IKernel * pKernel, const PERSISTID & self, const PERSISTID & sender, const IVarList & args, AITemplateBase &templateWrap)
{
	if (!pKernel->Exists(self))
		return AI_RT_SUCCESS;

    IGameObj *pSelf = pKernel->GetGameObj(self);
    if(pSelf == NULL)
    {
        return 0;
    }

    IRecord *pPatrolPointRec = pSelf->GetRecord("PatrolPointRec");
    if(pPatrolPointRec == NULL)
    {
        return AI_RT_NULL;
    }

    if(pSelf->QueryInt("Dead") > 0)
    {
        return AI_RT_SUCCESS;
    }

	//Ѳ��״̬��Ѳ��
 	if (pSelf->QueryInt("CurState") != AI_STATE_PATROL)
	{
		return AI_RT_IGNORE;
	}

    //��ӵ��жϷ�ֹ������
    //�����Χ��ҽ���һ�ִ���
	float SpringRange = AISystem::GetSpringRange(pSelf);
    CVarList arg_list;
    CVarList temp;
    pKernel->GetAroundList(self, SpringRange, TYPE_PLAYER, 10, arg_list);
    LoopBeginCheck(a);
    for ( size_t i = 0; i < arg_list.GetCount(); ++i )
    {
        LoopDoCheck(a);
        PERSISTID player = arg_list.ObjectVal(i);
        if ( pKernel->Exists(player) )
        {
            if(templateWrap.OnProcess(pKernel, AI_RULE_SPRING, self, player, temp) == AI_RT_SUCCESS)
            {
                return AI_RT_IGNORE;
            }
        }
    }

    //�����ƶ�����
    if ( pSelf->QueryInt("CantMove") > 0 )
    {
        return AI_RT_IGNORE;
    }

    //��ǰ����Ѳ���߶���
	int sub_state = pSelf->QueryInt("SubState");
	if ( sub_state == AI_PATROL_WALK)
	{
		return AI_RT_IGNORE;
	}
	else if (sub_state == AI_PATROL_WAIT)
	{
		//û��Ѳ�ߵ�
		int rows = pPatrolPointRec->GetRows();
		if (rows < 1)
		{
			return AI_RT_IGNORE;
		}

		int step = pSelf->QueryDataInt("PatrolStep");
		if (step < 0)
		{
			//��һ��Ѳ��
			pSelf->SetDataInt("PatrolStep", 0);
			pSelf->SetDataInt("PatrolRestTime", 0);

			//����Ѳ��,PatrolStep++
			return NextPatrol(pKernel, self, templateWrap);	
		}

		unsigned int restTime = (unsigned int)pSelf->QueryDataInt("PatrolRestTime");
		if (restTime >= 0)
		{
			unsigned int newRestTime = (unsigned int)restTime + (unsigned int)args.IntVal(0);
			if (step >= rows)
				step = 0;

			//��Ϣ���
			if (newRestTime >= (unsigned int)pPatrolPointRec->QueryInt(step, AI_PATROL_REC_POS_TIME))
			{
				//����Ѳ��,PatrolStep++
				NextPatrol(pKernel, self, templateWrap);	
				//�����Ϣ���
				pSelf->SetDataInt("PatrolRestTime", 0);
			}
			else
			{
				//������Ϣ
				pSelf->SetDataInt("PatrolRestTime", (int)newRestTime);
			}
		}
	}

	return AI_RT_SUCCESS;
}

//Ѳ����һ��,PatrolStep++
int AIRPatrolBase::NextPatrol(IKernel *pKernel, const PERSISTID &self, const AITemplateBase &templateWrap)
{
    IGameObj *pSelf = pKernel->GetGameObj(self);
    if(pSelf == NULL)
    {
        return 0;
    }

    IRecord *pPatrolPointRec = pSelf->GetRecord("PatrolPointRec");
    if(pPatrolPointRec == NULL)
    {
        return AI_RT_NULL;
    }

	int rows = pPatrolPointRec->GetRows();
	if (rows < 1)
	{
		return AI_RT_IGNORE;
	}

	//ȡ�õ�ǰѲ�ߵ�
	int step = pSelf->QueryDataInt("PatrolStep");
	if (step >= rows)
		step = 0;

	//ȡ��Ѳ��ģʽ
	int mode = pSelf->QueryInt("PatrolMode");
	if (mode == AI_PATROL_MODE_RANDOM)
	{
		//���ģʽ
		int n = util_random_int(rows);
		//��һ����ͬ��patrolStep��
		if(n == step)
		{
			if(step >= (rows - 1))
				n = 0;
			else
				n ++;
		}
		step = n;
	}
	else if (mode == AI_PATROL_MODE_BACK)
	{
		//����ģʽ
		//���÷��ر��
		if (pSelf->QueryDataInt("PatrolBack") == 0)
		{
			//��ǰ��
			step ++;
			if (step >= rows)
			{
				//�����ڵ�������
				step = __max(rows - 2, 0);
				//���÷��ر��
				pSelf->SetDataInt("PatrolBack", 1);	
			}
		}
		else
		{
			//�����
			step --;
			if (step < 0)
			{
				//��2�㿪ʼ��
				step = 1;
				//ɾ�����ر��
				pSelf->SetDataInt("PatrolBack", 0);	//
			}
		}
	}
	else if(mode == AI_PATROL_MODE_ROUND)
	{
		//������ģʽ
		step ++;
		//��0�㿪ʼ������
		if (step >= rows)
			step = 0;
	}

	if(step < 0 || step >= rows)
		step = 0;

	//����һ��Ѳ�ߵ���ȥ
	pSelf->SetDataInt("PatrolStep", step);
	float x = pPatrolPointRec->QueryFloat(step, AI_PATROL_REC_POS_X);
	float z = pPatrolPointRec->QueryFloat(step, AI_PATROL_REC_POS_Z);

	if (AIFunction::StartPatrolToPoint(pKernel, self, x, z, false))
	{
		pSelf->SetInt("SubState", AI_PATROL_WALK);
	}

	return AI_RT_SUCCESS;
}