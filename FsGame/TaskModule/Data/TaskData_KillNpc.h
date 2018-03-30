// -------------------------------------------
// �ļ����ƣ� TaskData_KillNpc.h
// �ļ�˵���� ��ɱNpc��������
// �������ڣ� 2018/02/24
// �� �� �ˣ�  tzt
// -------------------------------------------

#ifndef _TaskData_KillNpc_H_
#define _TaskData_KillNpc_H_

#include "TaskBaseData.h"

/*
** ��ɱNpc������Ϣ��
*/
class TaskData_KillNpc : public TaskBaseData
{
public:
	CoupleElementList npc_list;	// ��ɱ�����б�
	int kill_way;				// ��ɱ������㷽ʽ��0��ɱ���� 1��ɱ��һ�� 2��ɱָ��������
	
public:
	TaskData_KillNpc() : TaskBaseData(), kill_way(0)
	{
		npc_list.clear();
	}
};

#endif
