#ifndef EQUIP_FORGING_DEFINE_H_
#define EQUIP_FORGING_DEFINE_H_

#include <string>
#include <vector>
#include "CommonModule\CommRuleDefine.h"

#define EQUIP_FORGING_CONFIG "ini/Item/Forging/EquipForging.xml"

// �ͻ���-������
enum C2S_Forging_SubMsg
{
	/*!
	* @brief	����װ��
	* @param string ͼֽ
	*/
	C2S_FORGING_SUBMSG_EXECUTE = 1,
};

// ������-�ͻ���
enum S2C_FORGING_SUBMSG
{
	/*!
	* @brief	����ɹ�
	*/
    S2C_FORGING_SUBMSG_SUCCESS = 1,
};

// Ʒ��Ȩ��
struct OutputColorWeight
{
	int output_color;	// ���Ʒ��
	int lower_weight;
	int upper_weight;

	OutputColorWeight() :
		output_color(0),
		lower_weight(0),
		upper_weight(0)
	{}
};
typedef std::vector<OutputColorWeight> OutputColorWeightVec;

// ���Ʒ�ʲ���
struct OutputColorFinder
{
	int weight;
	OutputColorFinder(int _w) : weight(_w)
	{}

	bool operator () (const OutputColorWeight& cfg) const
	{
		return weight >= cfg.lower_weight && weight <= cfg.upper_weight;
	}
};

// �������
typedef std::vector<std::string> OutputList;
struct ForgingRule
{
	std::string drawing;		// ͼֽ
	std::string output_equip;			// ����ɹ�������װ��
	OutputColorWeightVec color_weight_list;
	int sum_weight;

	Consume_Vec items;	// ���Ĳ���
	Consume_Vec capitals;	// ���Ļ���

	ForgingRule(const char *id) :
		drawing(id), 
		sum_weight(0)
	{
		items.clear();
		capitals.clear();
		output_equip.clear();
		color_weight_list.clear();
	}

	bool operator == (const ForgingRule &cfg) const
	{
		return strcmp(drawing.c_str(), cfg.drawing.c_str()) == 0;
	}
};
typedef std::vector<ForgingRule> ForgingRuleVec;

#endif
