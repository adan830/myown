
#ifndef JewelDefine_h__
#define JewelDefine_h__
#include <string>
#include <map>
#include <vector>

// ��ʯʹ��λ��
struct JewelUsePos
{
	int jewel_type;
	std::vector<int> equip_pos_list;

	JewelUsePos(int _type) : jewel_type(_type)
	{
		equip_pos_list.clear();
	}

	bool operator == (const JewelUsePos& temp) const
	{
		return temp.jewel_type == jewel_type;
	}
};
typedef std::vector<JewelUsePos> JewelUsePosVec;

// ��ʯ����������
typedef std::map<int, std::string> JewelIndexMap;

// ��ʯ����������
typedef std::map<std::string, int> JewelMap;

// ��ʯ�ϳ�����
struct JewelComposeConsume
{
	int point_values;
	int compose_fee;
	int buy_cost;
	int buy_capital;

	JewelMap jewel_map;

	JewelComposeConsume() :
		point_values(0),
		compose_fee(0),
		buy_cost(0),
		buy_capital(0)
	{
		jewel_map.clear();
	}
};

// �ͻ���->������
enum C2S_Jewel_Submsg
{
	/*!
	* @brief	��Ƕ
	* @param int ��ʯ��������
	* @param int �������е�λ��
	* @param int Ҫ��Ƕ��װ����������
	* @param int Ҫ��Ƕ��װ���Ŀ�λ����
	*/
	C2S_JEWEL_SUBMSG_INLAY = 1,

	/*!
	* @brief	����
	* @param int Ҫ���µ�װ����������
	* @param int Ҫ���µ�װ���Ŀ�λ����
	*/
	C2S_JEWEL_SUBMSG_DISMANTLE,

	/*!
	* @brief	�ϳ�
	* @param int Ҫ�ϳɵı�ʯ������
	* @param int �ϳɸ���
	* @param int ��ѡ�ϳɲ�����n * 2��һ�ԣ���š�������
	* @param ����ѭ��n��
	* @param int ��ѡ�������
	* @param int ����
	* @param int �蹺��ϳɲ�����n * 2��һ�ԣ���š�������
	* @param int �蹺��������
	* @param int ����
	*/
	C2S_JEWEL_SUBMSG_COMPOSE,
};

// ������->�ͻ���
enum S2C_Jewel_SubMsg
{
	/*!
	* @brief	�ϳɳɹ�
	* @param string ��ʯid
	* @param int ����
	*/
	S2C_JEWEL_SUBMSG_COMPOSE_SUCCESS = 1,
};

// �Ƿ��Զ�����
enum 
{
	AUTO_BUY_YES = 1,	// �Զ�����
	AUTO_BUY_NO = 2,	// ���Զ�����
};


#endif // JewelDefine_h__
