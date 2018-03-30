// -------------------------------------------
// �ļ����ƣ�	 CommRuleDefine.h
// �ļ�˵���� ���ù����壨����һЩͨ�ò��Ҽ򵥵Ĺ���
// �������ڣ� 2018/03/09
// �� �� �ˣ�  tzt
// -------------------------------------------

#ifndef __CommRuleDefine_H__
#define __CommRuleDefine_H__

#include "Fsgame/Define/header.h"
#include <vector>

#define TEAM_MEMBER_VERIFY_DISTANCE 50.0f	// �����������Ա��������
#define VALID_DAMAGE_REC_TIME 60000		// �˺���Чʱ��
#define LIMIT_KILL_REC_TIME 900000		// ��Ч��ɱ����ʱ��

// ����/�г� ����
enum GroupChangeType
{
	GROUP_CHANGE_TYPE_ENTRY,
	GROUP_CHANGE_TYPE_QUIT
};

// �������ݽṹ
struct Consume
{
	std::string id;
	int num;
	Consume() : num(0){}
};
typedef std::vector<Consume> Consume_Vec;

// ���ͣ��ַ��б�
typedef std::vector<int> Integer_Vec;
typedef std::vector<std::string> String_Vec;

#endif

