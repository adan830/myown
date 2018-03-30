// -------------------------------------------
// �ļ����ƣ� TaskBaseData.h
// �ļ�˵���� �����������
// �������ڣ� 2018/02/24
// �� �� �ˣ�  tzt
// -------------------------------------------

#ifndef _TaskBaseData_H_
#define _TaskBaseData_H_

#include "TaskStructDefine.h"

/*
** ���������Ϣ��
*/
class TaskBaseData
{
public:
	int task_id;			// ������
	int type;				// ����
	int rule;				// ����
	int accept_min_level;	// ������С�ȼ�
	int accept_max_level;	// �������ȼ�
	int count;				// �����ܽ���
	int calculate_mode;		// ͳ�Ʒ�ʽ
	NumList prev_task_list;	// ǰ�������
	NumList post_task_list;	// ���������
	int task_star;			// �����Ǽ�

	int is_group;			// �Ƿ��з��鸱����0�޷��鸱�� 1�з��鸱����
	int auto_pathfinding;	// ��ȡ������Ƿ��Զ�Ѱ·
	int is_auto_submit;		// ���������Ƿ��Զ��ύ
	int is_switch_scene;	// ��ȡ������Ƿ��л�����
	int is_switch_position;	// ��ȡ������Ƿ��л�����
	int is_show_reward;		// �ύ������Ƿ�򿪽�������

	NumList	scene_list;		// �����б�
	PositionList posi_list;	// �����б�
	int reward_id;			// ����ID

public:
	TaskBaseData() :
		task_id(0),
		type(0),
		rule(0),
		accept_min_level(0),
		accept_max_level(0),
		count(0),
		calculate_mode(0), 
		task_star(0),
		is_group(0),
		auto_pathfinding(0),
		is_auto_submit(0),
		is_switch_scene(0),
		is_switch_position(0),
		is_show_reward(0),
		reward_id(0)
	{
		prev_task_list.clear();
		post_task_list.clear();
		scene_list.clear();
		posi_list.clear();
	}


public:
	// �Ƿ��з��鸱��
	bool IsGrouping() const
	{
		return is_group > 0 ? true : false;
	}

	// ��ȡ������Ƿ��Զ�Ѱ·
	bool IsAutoPathFinding() const
	{
		return auto_pathfinding > 0 ? true : false;
	}

	// ���������Ƿ��Զ��ύ
	bool IsAutoSubmit() const
	{
		return is_auto_submit > 0 ? true : false;
	}

	// ��ȡ������Ƿ��л�����
	bool IsSwitchScene() const
	{
		return is_switch_scene > 0 ? true : false;
	}

	// ��ȡ������Ƿ��л�����
	bool IsSwitchPosition() const
	{
		return is_switch_position > 0 ? true : false;
	}

	// �ύ������Ƿ�򿪽�������
	bool IsShowReward() const
	{
		return is_show_reward > 0 ? true : false;
	}

	// �Ƿ��н���
	bool IsHaveReward() const
	{
		return reward_id > 0;
	}
};

#endif
