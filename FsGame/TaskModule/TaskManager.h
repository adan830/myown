// -------------------------------------------
// �ļ����ƣ� TaskManager.h
// �ļ�˵���� �������ģ��
// �������ڣ� 2018/02/24
// �� �� �ˣ�  tzt  
// -------------------------------------------
	
#ifndef _TaskManager_H_
#define _TaskManager_H_

#include "FsGame/Define/header.h"
#include "Template/TaskTemplate.h"
#include "Rule/TaskBaseRule.h"

class TaskManager : public ILogicModule
{
public:
	// ��ʼ��
    virtual bool Init(IKernel *pKernel);

	// �ر�
    virtual bool Shut(IKernel *pKernel);

    // ������Դ�ļ�
    bool LoadRes(IKernel *pKernel);

private:
	// ע������ģ��
	bool RegistTemplate(IKernel *pKernel, TaskTemplate *pTemplate);

	// ��ʼ������ģ��
	bool InitTemplates(IKernel *pKernel);

	// ����������
	TaskBaseRule *CreateRule(const TaskRules rule);

	// ��ʼ���������
	bool InitRules(IKernel *pKernel);

public:
	// ȡ������ģ��
	TaskTemplate* GetTemplate(const TaskTypes task_type);

	// ȡ���������
	TaskBaseRule* GetRule(const TaskRules task_rule);

	// Ϊ��������ע�ᶨʱ��
	void RegistResetTimer(IKernel *pKernel, const PERSISTID &self);

	// ��ȡ����domain
	static const std::wstring& GetDomainName(IKernel * pKernel);

	// �������ú���
#define DefineReloadFunc(funcname) \
	static void Reload##funcname(IKernel *pKernel)

	DefineReloadFunc(Task_All);		// ������������
	DefineReloadFunc(Task_Main);	// ������������
	DefineReloadFunc(Task_Branch);	// ֧����������
	DefineReloadFunc(Task_Daily);	// �ճ���������

private:
	// ������
	static int OnCommandTaskAction(IKernel *pKernel, const PERSISTID &self,
		const PERSISTID &sender, const IVarList &args);

	// �ͻ���׼������
	static int OnReady(IKernel* pKernel, const PERSISTID& self,
		const PERSISTID& sender, const IVarList& args);

	// ����command��Ϣ
	static int OnCommandTask(IKernel *pKernel, const PERSISTID &self,
		const PERSISTID &sender, const IVarList &args);

	// �ͻ���������Ϣ
	static int OnCustomTask(IKernel *pKernel, const PERSISTID &self,
		const PERSISTID &sender, const IVarList &args);

	// ��Ҵ����ݿ�ָ�������ɣ�������ߣ�
	static int OnPlayerRecover(IKernel *pKernel, const PERSISTID &self, 
		const PERSISTID &sender, const IVarList &args);

	// ����ƶ�
	static int OnCommandMotion(IKernel *pKernel, const PERSISTID &self, 
		const PERSISTID &sender, const IVarList &args);

	// �ȼ����� 
	static int OnCommandLevelUp(IKernel *pKernel, const PERSISTID &self,
		const PERSISTID &sender, const IVarList &args);

	// ����뿪����
	static int OnLeaveScence(IKernel *pKernel, const PERSISTID &self,
		const PERSISTID &sender, const IVarList &args);

	// ��ɱNpc
	static int OnKillNpc(IKernel *pKernel, const PERSISTID &self,
		const PERSISTID &sender, const IVarList &args);

	// ��ɱ���
	static int OnKillPlayer(IKernel *pKernel, const PERSISTID &self,
		const PERSISTID &sender, const IVarList &args);

	// CG���Ž�����Ϣ
	static int OnCommandCGDone(IKernel *pKernel, const PERSISTID &self,
		const PERSISTID &sender, const IVarList &args);

private:
	// ����ģ�庯��
	TaskTemplate* m_pTemplates[TASK_TYPE_TOTAL];

	// ���������
	TaskBaseRule* m_pRules[TASK_RULE_TOTAL];

public:
	// ������������
	static std::wstring m_domainName;
	static TaskManager *m_pThis;
};

//typedef HPSingleton<TaskManager> TaskManagerSingleton;
#endif
