#include "CommonModule\CommRuleDefine.h"
//--------------------------------------------------------------------
// �ļ���:      EquipBaptiseDefine.h
// ��  ��:      װ��ϴ������
// ˵  ��:		
// ��������:    2018��03��12��
// ������:       tzt 
//--------------------------------------------------------------------

#ifndef _EquipBaptiseDefinee_h__
#define _EquipBaptiseDefinee_h__

const char* const BAPTISE_PROP_FILE = "ini/Item/Baptise/BaptiseProp.xml";
const char* const BP_RANGE_FILE = "ini/Item/Baptise/BaptisePropRange.xml";
const char* const BP_LIMIT_FILE = "ini/Item/Baptise/BaptisePropLimit.xml";
const char* const BAPTISE_CONSUME_FILE = "ini/Item/Baptise/BaptiseConsume.xml";

// ϴ������
enum BaptiseType
{
	BAPTISE_TYPE_JUNIOR = 1,	// ������������
	BAPTISE_TYPE_MIDDLE,		// �м������ϣ�
};

// ��������
enum LockPropState
{
	LOCK_PROP_STATE_UNLOCK = 0,		// ����״̬
	LOCK_PROP_STATE_LOCK,			// ����״̬	
};

// �ͻ���-������������Ϣ
enum C2S_Baptise_SubMsg
{
	/*!
	* @brief	����ִ��һ��ϴ��
	* @param int ϴ������BaptiseType
	* @param int ϴ����λEQUIP_POS
	*/
	C2S_BAPTISE_SUBMSG_EXCUTE = 1,

	/*!
	* @brief	���󱣴�ϴ�����
	* @param int ϴ����λEQUIP_POS
	*/
	C2S_BAPTISE_SUBMSG_RESULT_SAVE,

	/*!
	* @brief	�������ϴ�����
	* @param int ϴ����λEQUIP_POS
	*/
	C2S_BAPTISE_SUBMSG_RESULT_GIVEUP,

	/*!
	* @brief	������������
	* @param int ϴ����λEQUIP_POS
	* @param string ��������	
	*/
	C2S_BAPTISE_SUBMSG_LOCK_PROP,

	/*!
	* @brief	�����������
	* @param int ϴ����λEQUIP_POS
	* @param string ��������
	*/
	C2S_BAPTISE_SUBMSG_UNLOCK_PROP,
};


// ϴ������
struct BaptiseProp 
{
	int index;	// ��λ
	int job;	// ְҵ
	String_Vec prop_list;	// ϴ�������б�

	BaptiseProp() : 
		index(0), 
		job(0)
	{
		prop_list.clear();
	}
};
typedef std::vector<BaptiseProp> BaptisePropVec;

// ϴ�����Բ�ѯ
struct BPFinder
{
	int _idx;
	int _job;
	BPFinder(int idx, int job) :
		_idx(idx),
		_job(job)
	{}

	bool operator () (const BaptiseProp& cfg) const
	{
		return cfg.index == _idx && cfg.index == _job;
	}
};

// ϴ������ȡֵȨ��
struct BPRangeWeight
{
	int min_value;
	int max_value;
	int lower_weight;
	int upper_weight;

	BPRangeWeight() :
		min_value(0),
		max_value(0),
		lower_weight(0),
		upper_weight(0)
	{}
};
typedef std::vector<BPRangeWeight> BPRWeightVec;

// ϴ������ȡֵ��ѯ
struct BPRangeWeightFinder
{
	int weight_value;
	BPRangeWeightFinder(int value) : weight_value(value)
	{}
	
	bool operator () (const BPRangeWeight& cfg) const
	{
		return weight_value >= cfg.lower_weight 
			&& weight_value <= cfg.upper_weight;
	}
};

// ϴ������ȡֵ
struct BPRange
{
	int index;
	int job;
	std::string prop_name;
	int baptise_type;
	int sum_weight;
	BPRWeightVec bprw_list;

	BPRange() :
		index(0),
		job(0),
		baptise_type(0), 
		sum_weight(0)
	{
		bprw_list.clear();
	}
};
typedef std::vector<BPRange> BPRangeVec;

// ϴ������ȡֵ
struct BPRangeFinder
{
	int _index;
	int _job;
	int bp_type;
	std::string _name;
	BPRangeFinder(int idx, int job, int type, std::string name) : 
		_index(idx), 
		_job(job), 
		bp_type(type), 
		_name(name)
	{}

	bool operator () (const BPRange& cfg) const
	{
		return cfg.index == _index 
			&& cfg.job == _job 
			&& bp_type == cfg.baptise_type 
			&& strcmp(_name.c_str(), cfg.prop_name.c_str()) == 0;
	}
};

// ϴ������ֵ����
struct BPLimitValue
{
	int index;
	int job;
	std::string prop_name;
	int lower_str_lvl;
	int upper_str_lvl;
	int max_value;

	BPLimitValue() :
		index(0),
		job(0),
		lower_str_lvl(0),
		upper_str_lvl(0),
		max_value(0)
	{}
};
typedef std::vector<BPLimitValue> BPLimitValueVec;

// ϴ������ֵ���޲�ѯ
struct BPLimitValueFinder
{
	int _index;
	int _job;
	int _str_lvl;
	std::string _name;
	BPLimitValueFinder(int idx, int job, int str_lvl, std::string name) :
		_index(idx),
		_job(job),
		_str_lvl(str_lvl),
		_name(name)
	{}

	bool operator () (const BPLimitValue& cfg) const
	{
		return cfg.index == _index
			&& cfg.job == _job
			&& strcmp(_name.c_str(), cfg.prop_name.c_str()) == 0
			&& _str_lvl >= cfg.lower_str_lvl 
			&& _str_lvl <= cfg.upper_str_lvl;
	}
};

// ϴ������
struct BaptiseConsume
{
	int baptise_type;
	Consume_Vec capitals;
	Consume_Vec items;

	BaptiseConsume(int _bt) :
		baptise_type(_bt)
	{
		capitals.clear();
		items.clear();
	}

	bool operator == (const BaptiseConsume& cfg) const
	{
		return cfg.baptise_type == baptise_type;
	}
};
typedef std::vector<BaptiseConsume> BConsumeVec;

#endif
