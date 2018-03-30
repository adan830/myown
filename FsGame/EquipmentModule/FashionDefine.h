//---------------------------------------------------------
//�ļ���:       FashionDefine.h
//��  ��:       ʱװģ��Ķ���
//˵  ��:               
//��������:      2017��02��21��
//������:         tongzt
//�޸���:    
//---------------------------------------------------------

#ifndef FSGAME_FASHION_DEFINE_H_
#define FSGAME_FASHION_DEFINE_H_

#include <string>
#include <vector>
#include <map>
#include "CommonModule\CommRuleDefine.h"

#define FASHION_CONFIG_FILE	"ini/Item/Apperance/Fashion.xml"

// ʱװ����
struct FashionCfg 
{
	FashionCfg(int _id) :
		fashion_id(_id),
		type(0),
		sex_limit(0),
		job_limit(0), 
		special_item(0)
		{
			unlock_fee.clear();
			unlock_items.clear();
		}

	int fashion_id;
	int type;
	int sex_limit;
	int job_limit;
	
	Consume_Vec unlock_fee;
	Consume_Vec unlock_items;
	int special_item;
	int pre_fashion;

	bool operator == (const FashionCfg& cfg) const
	{
		return cfg.fashion_id == fashion_id;
	}
};
typedef std::vector<FashionCfg> FashionCfgVec;

// �ͻ���->������������Ϣ
enum C2S_Fashion_SubMsg
{
	/*!
	* @brief	�������
	* @param int ʱװid
	*/
	C2S_FASHION_SUBMSG_UNLOCK = 1,

	/*!
	* @brief	����ʱװ
	* @param int ʱװid
	*/
	C2S_FASHION_SUBMSG_PUTON,

	/*!
	* @brief	������ʱװ
	* @param int ʱװid
	*/
	C2S_FASHION_SUBMSG_TAKEOFF,
};

// ������->�ͻ��˶�����Ϣ
enum S2C_Fashion_SubMsg
{
	/*!
	* @brief	ʱװʱЧ����
	* @param int ʱװid
	*/
	S2C_FASHION_SUBMSG_VALID_TIME_END= 1,

	
};

#endif
