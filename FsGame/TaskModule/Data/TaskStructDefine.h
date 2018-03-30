// -------------------------------------------
// �ļ����ƣ� TaskStructDefine.h
// �ļ�˵���� �������ݽṹ����
// �������ڣ� 2018/02/23
// �� �� �ˣ�  tzt   
// -------------------------------------------

#ifndef _TaskStructDefine_H_
#define _TaskStructDefine_H_

#include <map>
#include <vector>

typedef std::vector<int> NumList;	// �����б�
typedef std::vector<std::string> StrList;	// �ַ����б�

// ������Ϣ
struct Position
{
	float x;	// x����
	float y;	// y����
	float z;	// z����
	float o;	// ����

	Position(float x, float y, float z, float o) : x(x), y(y), z(z), o(o) {}
	Position() : x(0.0f), y(0.0f), z(0.0f), o(0.0f){}
};
typedef std::vector<Position> PositionList;	// �����б�

// ��ֵ��Ԫ��
struct CoupleElement
{
	std::string id;
	int num;

	CoupleElement(const char *_id) : num(0), id(_id){}

	bool operator == (const CoupleElement& element) const
	{
		return strcmp(id.c_str(), element.id.c_str()) == 0;
	}
};
typedef std::vector<CoupleElement> CoupleElementList;

// ����������Ϣ
struct TaskConfig
{
	int type;
	std::string file_name;
	int limit_num;
	bool in_use;
	int first_task;

	TaskConfig(int _type) :
		type(_type),
		limit_num(0), 
		in_use(true), 
		first_task(0)
	{}

	bool operator == (const TaskConfig& config) const
	{
		return config.type == type;
	}

};
typedef std::vector<TaskConfig> TaskConfigList;

#endif