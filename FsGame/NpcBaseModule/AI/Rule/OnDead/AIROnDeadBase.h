//--------------------------------------------------------------------
// �ļ���:      AIROnDeadBase.h
// ��  ��:      ��������
// ˵  ��:
// ��������:    2014��12��31��
// ������:       
// �޸���:        
//    :       
//--------------------------------------------------------------------
#pragma once
#include "FsGame/NpcBaseModule/AI/Rule/AIRuleBase.h"

class AIROnDeadBase : public AIRuleBase
{
public:
	AIROnDeadBase(void);

	//ִ�иù���
	virtual int DoRule(IKernel * pKernel,const PERSISTID & self,const PERSISTID & sender,const IVarList & args, AITemplateBase &templateWrap);
protected:
	// �˺�����
	struct DamageData 
	{
		PERSISTID	player;			// ��Ҷ����
		int			nDamageVal;		// �˺�ֵ

		bool operator < (const DamageData& ch) const
		{
			return nDamageVal > ch.nDamageVal;
		}
	};

	typedef std::vector<DamageData> DamageDataVec;

    //ȡ��NPC��������,��û������ǰȡ�õ�ֵ���Ǵ��
    int GetNpcAscription(IKernel * pKernel, const PERSISTID & self, const PERSISTID & sender, IVarList &varKillers);

	//��ȡNPC�����˺�ǰnλ
	int GetNpcAscriptionFront(IKernel * pKernel, const PERSISTID & self, IVarList &varKillers);

    //֪ͨ����ģ������߼�
    void InformTask(IKernel *pKernel, const PERSISTID &self, const PERSISTID &killer);
};
