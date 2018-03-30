//--------------------------------------------------------------------
// �ļ���:      AITemplateBase.h
// ��  ��:      NPC����ģ��
// ˵  ��:
// ��������:    2014��12��31��
// ������:       
// �޸���:        
//    :       
//--------------------------------------------------------------------
#ifndef _AI_TEMPLATE_BASE_H_
#define _AI_TEMPLATE_BASE_H_
#include "Fsgame/Define/header.h"
#include "FsGame/NpcBaseModule/AI/AIDefine.h"
#include <vector>

//NPC��״̬����
enum 
{
	AI_STATE_BORN = 0, //����
	AI_STATE_PATROL,   //Ѳ��
	AI_STATE_FIGHT,    //ս��
	AI_STATE_BACK,     //������
	AI_STATE_DIED,     //����
    AI_STATE_FOLLOW,   //����
    AI_RECUPERATE,     //����
	AI_ESCAPE,		   //����
};

//NPC Ѳ�ߵ���״̬
enum
{
	AI_PATROL_WAIT = 0,		// ��Ϣ״̬
	AI_PATROL_WALK,			// �߶�״̬
	AI_PATROL_STOP,			// ��ͣ״̬
};

class AITemplateBase
{
public:
	AITemplateBase(void);
	virtual ~AITemplateBase();
public:

	//���ģ������
	const AI_TEMPLATE_TYPE GetTemplateType();

	//ע���¼�
	bool RegistEventRule(AI_RULE_TYPE nEvent,int nRule);

public:

    //��ʼ��
    virtual bool Init(IKernel* pKernel);

	//������Ϊ��������Ϣ����
	virtual int	OnBasicActionMsg(IKernel* pKernel, const PERSISTID& self, int msg, const IVarList& args = CVarList());

	//����
	virtual int OnBorn(IKernel * pKernel,const PERSISTID & self, const PERSISTID & sender, const IVarList & args);

	//ģ�洦�����
	virtual int OnProcess(IKernel* pKernel, AI_RULE_TYPE processType, const PERSISTID& self, const PERSISTID& sender = PERSISTID(), const IVarList& args = CVarList());

    //Ѳ��
    virtual int BeginPatrol(IKernel * pKernel, const PERSISTID & sel) const;

public:

	//����Ѳ��
	int EndPatrol(IKernel * pKernel , const PERSISTID & self) const;

	//�ı�AI״̬
	int ChangeState(IKernel * pKernel, const PERSISTID & self, int state) const;

	//��ȡ�ӹ�������
	int GetSubAIRule(int nRuleType,const PERSISTID & self) const;

	//���������Ѳ�ߵ�
	void InitRandomPatrolPath(IKernel * pKernel, const PERSISTID & self, bool doStop = true) const;

	// ���ﴴ����ʼ������
	void InitPropAfterBorn(IKernel* pKernel, const PERSISTID& self) const;

	// ��Ӽ���
	void InitSkillAfterBorn(IKernel * pKernel, const PERSISTID& self) const;

    //NPC�������棨���������й��ҵ�BOSS��
    void BornNotice(IKernel *pKernel, const PERSISTID &self) const;


protected:

    //��ǰģ�����ͱ��
	AI_TEMPLATE_TYPE m_templateType;

	//�¼������Ӧ��
	std::vector< int > m_EventRuleVct;
};

#endif // _AI__TEMPLATE_BASE_H_
