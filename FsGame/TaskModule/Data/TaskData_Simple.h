// -------------------------------------------
// �ļ����ƣ� TaskData_Simple.h
// �ļ�˵���� ��һ����������������
// �������ڣ� 2018/02/28
// �� �� �ˣ�  tzt
// -------------------------------------------

#ifndef _TaskData_Simple_H_
#define _TaskData_Simple_H_

#include "TaskBaseData.h"

/*
** ��һ��������������Ϣ��
*/
class TaskData_Simple : public TaskBaseData
{
public:
	std::string add_info;	// ��һ����
	
public:
	TaskData_Simple() : TaskBaseData()
	{
		
	}
};

#endif
