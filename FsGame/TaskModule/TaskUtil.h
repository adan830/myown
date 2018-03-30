// -------------------------------------------
// �ļ����ƣ� TaskUtil.h
// �ļ�˵���� ���񹤾���
// �������ڣ� 2018/02/26
// �� �� �ˣ�  tzt  
// -------------------------------------------
	
#ifndef _TaskUtil_H_
#define _TaskUtil_H_

#include "FsGame/Define/header.h"
#include "TaskDefine.h"
#include "Data/TaskStructDefine.h"


class TaskUtil
{
public:
	// ��ʼ��
	bool Init(IKernel *pKernel);

	// ��ѯ����״̬
	const TaskStatus QueryTaskStatus(IKernel *pKernel, const PERSISTID &self,
		const int task_id);

	// ǰ�������Ƿ����ύ
	bool IsPrevTaskSubmit(IKernel *pKernel, const PERSISTID &self,
		const int task_id);

	// �����Ƿ��ѿ���
	bool IsStart(IKernel *pKernel, const PERSISTID &self,
		const int task_type);

	// �����Ƿ��д���
	bool IsHaveNumOfType(IKernel *pKernel, const PERSISTID &self,
		const int task_type);

	// ��ǰ�Ƿ���ָ�����͵�����
	bool IsHaveOfType(IKernel *pKernel, const PERSISTID &self,
		const int task_type);

	// ��ǰ�Ƿ���ָ�����������
	bool IsHaveOfRule(IKernel *pKernel, const PERSISTID &self,
		const int task_rule);

	// ���ָ���������͵�����
	const int RandomTask(IKernel *pKernel, const PERSISTID &self,
		const int task_type);

	// ��¼������ɴ���
	void RecordTaskNum(IKernel *pKernel, const PERSISTID &self,
		const int task_type);

	// ��ȡ��������
	void AcceptPostTask(IKernel *pKernel, const PERSISTID &self,
		const int task_id);

	// ��ǰ��������ȫ���
	void MainTaskSafeCheck(IKernel *pKernel, const PERSISTID &self);

	// ��������Լ��
	void SelfCheck(IKernel *pKernel, const PERSISTID &self,
		const int task_id);

	// �����Զ��г���
	void AutoSwitchScene(IKernel *pKernel, const PERSISTID &self,
		const int task_id);

	// �Զ�������������cg
	void AutoPlayTaskCG(IKernel *pKernel, const PERSISTID &self);

	// �������񸱱�
	bool CreateGroupClone(IKernel *pKernel, const PERSISTID &self,
		const int task_id);

	// �������񸱱�
	void CleanGroupClone(IKernel *pKernel, const PERSISTID &self, 
		const int task_id);

	// ��������
	void UpdateTask(IKernel *pKernel, const PERSISTID &self,
		const IVarList &args);

	// �ύ����
	void SubmitTask(IKernel *pKernel, const PERSISTID &self,
		const IVarList &args);

	// �����Զ�Ѱ·��ʱ״̬
	void SetAutoPathFind(IKernel *pKernel, const PERSISTID &self, 
		const IVarList &args);

	// ����ɱ������
	void UpdateKillNpc(IKernel *pKernel, const PERSISTID &self, 
		const PERSISTID &npc);

	// ת��CoupleElementListΪstring
	bool ConvertCEL2Str(const CoupleElementList& src_list, std::string& target_str, 
		bool zero_value = false, const char *del1 = ":", const char *del2 = ",");

	// ת��stringΪCoupleElementList
	bool ConvertStr2CEL(const std::string& src_str, CoupleElementList& target_list,
		const char *del1 = ":", const char *del2 = ",");

	// ���񳡾���֤
	bool IsTargetScene(IKernel *pKernel, const NumList& scene_list,
		const int cur_scene = 0);

	// ������֤
	bool IsTargetPosition(IKernel *pKernel, const PositionList& position_list,
		const Position& position, const float distance = COMMON_VERIFY_DISTANCE);

private:
	// �ӳٸ��»�ɱNpc��������
	static int HB_DelayUpdateKillNpc(IKernel* pKernel, const PERSISTID& self, int slice);
};

typedef HPSingleton<TaskUtil> TaskUtilS;
#endif
