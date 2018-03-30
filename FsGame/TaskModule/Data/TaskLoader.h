// -------------------------------------------
// �ļ����ƣ� TaskLoader.h
// �ļ�˵���� �������ݶ�ȡ/��ѯ
// �������ڣ� 2018/02/23
// �� �� �ˣ�  tzt
// -------------------------------------------

#ifndef _TaskLoader_H_
#define _TaskLoader_H_

#include "TaskBaseData.h"
#include "Fsgame/Define/header.h"

class CXmlFile;

typedef std::map<int, TaskBaseData *> TaskBaseDataS;	// key=����ID value=���������Ϣ
typedef std::map<int, TaskBaseDataS> TaskBaseDataMap;	// key=�������� value=������������Ϣ�б�

/*
** ������Ϣ��ȡ��
*/

class TaskLoader
{
public:
	static TaskLoader &instance()
    {
		static TaskLoader task_loader;
		return task_loader;
    }

public:
    // ����ȫ����������
    bool LoadRes(IKernel *pKernel);

    // �ͷ���������
	bool ReleaseRes(const int task_type = 0);

	// ���¼�����������
	bool ReloadRes(IKernel *pKernel, const int task_type = 0);

private:
	// ����������������
	bool LoadTaskConfig(IKernel *pKernel, const int task_type = 0);

	// ���������������
	bool LoadTaskBaseData(IKernel *pKernel, const int task_type, 
		TaskBaseDataMap& data_map);

	// ������Ӧ������������
	TaskBaseData *CreateTaskDataClass(const CXmlFile &xml, const char *sec_name,
		const int task_rule);

public:
	// �ָ�����б����͵ļ�ֵ���ַ���
	void ParseCoupleStr(const char *soure_str, CoupleElementList& ele_list, 
		const char *del1 = ",", const char *del2 = ":");

	// �ָ������ֵ���ַ���
	void ParseCoupleStr(const char *soure_str, CoupleElement& element,
		const char *del1 = ":");

	// �ָ��������Ϊ���͵�ֵ�ַ���
	void ParseSinglelStr(const char *soure_str, NumList& ele_list,
		const char *del1 = ",");

	// �ָ��������Ϊ�ַ��ĵ�ֵ�ַ���
	void ParseSinglelStr(const char *soure_str, StrList& ele_list,
		const char *del1 = ",");

	// ����������Ϣ
	void ParsePosition(const char *soure_str, PositionList& position_list,
		const char *del1 = ";", const char *del2 = ",");

    // ��ȡһ�������¼
	const TaskBaseData* GetTaskBase(const int task_id) const;

	// ��ȡĳ����������
	const TaskConfig* GetTaskConfig(const int task_type) const;

	// ��ȡĳ���������͵������
	const TaskBaseDataS* GetTaskTableByType(const int task_type) const;

private:
    // ����Ĭ�Ϲ��캯��
	TaskLoader() {}
    // ����Ĭ����������
	~TaskLoader() {}
    // ���ÿ������캯��
	TaskLoader(const TaskLoader &) {}
    // ���ø�ֵ���캯��
	TaskLoader &operator= (const TaskLoader &) {}

private:
	TaskBaseDataMap m_TaskData;
	TaskConfigList m_TaskConfig;
};

#endif
