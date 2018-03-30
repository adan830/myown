#ifndef __RideDefine_h__
#define __RideDefine_h__

#include <vector>
#include <map>
#include "FsGame/CommonModule/CommRuleDefine.h"

#define MAX_RIDE_NUMBER 1

// ����׼�����
struct RideStepCfg
{
	RideStepCfg(int _step) : 
		step(_step), 
		need_star(0)
	{
		upgrade_items_list.clear();
	}

	int step;	// �׼�
	int need_star;	// ���������Ǽ�
	std::vector<Consume_Vec> upgrade_items_list;
	std::string skin_id;	// Ƥ��ID

	bool operator == (const RideStepCfg& step_cfg) const
	{
		return step_cfg.step == step;
	}
};
typedef std::vector<RideStepCfg> RideStepCfgList;

// ����Ƥ��
struct RideSkin
{
	int skin_id;	// Ƥ��id
	int att_add_pck_id;	// ����������
	int special_item;
	int skin_type;	// Ƥ������

	RideSkin() : 
		skin_id(0),
		att_add_pck_id(0), 
		special_item(0), 
		skin_type(0)
		{

		}
};
typedef std::map<int, RideSkin> RideSkinMap;

// Ƥ����������
struct SkinUnlockCfg
{
	SkinUnlockCfg(int _id) :
	id(_id), 
	pre_skin(0)
	{
		unlock_fee.clear();
		unlock_items.clear();
	}

	int id;	// ConfigID
	Consume_Vec unlock_fee;
	Consume_Vec unlock_items;
	int pre_skin;

	bool operator == (const SkinUnlockCfg& cfg) const
	{
		return cfg.id == id;
	}
};
typedef std::vector<SkinUnlockCfg> SkinUnlockCfgVec;

// ���Ｄ��״̬
enum RideState 
{
    RideState_None,
    RideState_Actived,
};

// �����������
enum RideUpDownOperate
{
	RIDE_UP_DOWN_OPERATE_UP = 0,	// ��
	RIDE_UP_DOWN_OPERATE_DOWN,		// ��
};

// Ƥ��ʹ��״̬
enum SkinUseState
{
	SKIN_USE_STATE_UNUSE = 0,	// δʹ��
	SKIN_USE_STATE_INUSE = 1,	// ʹ����
};

// ������������ʽ
enum UpdatePkgTypes
{
	UPDATE_PKG_TYPE_NONE,
	UPDATE_PKG_TYPE_ALL = 1,	// ������������������
	UPDATE_PKG_TYPE_SPEED,	// ������������speed����
	UPDATE_PKG_TYPE_EXCEPT_SPEED,	// ������������speed�������
};

// Ƥ������
enum RideSkinTypes
{
	RIDE_SKIN_TYPE_STEP = 0,	// ����
	RIDE_SKIN_TYPE_FASION = 1,	// ʱװ
};


// ����2����ϢID  �ͻ��˷������
enum CSRideMsg
{
	/*!
	* @brief	����Ƥ����Ϣ����
	* @param int ��������
	*/
	CS_RIDE_SKIN_MSG_REQ = 1,

	/*!
	* @brief	�������Ƥ��
	* @param int ����Ƥ��id
	* @param int ��������
	*/
	CS_RIDE_CHANGE_SKIN,

	/*!
	* @brief	�ֶ���������
	* @param int �������ͣ�0�������� 1�������
	*/
	CS_RIDE_UP_DOWN,

	/*!
	* @brief	ȥ����Ƥ����ʶ
	* @param int ����Ƥ��id
	*/
	CS_RIDE_CLEAR_SKIN_FLAG,

	/*!
	* @brief	��������Ƥ��
	* @param int Ƥ��ID
	*/
	CS_RIDE_SKIN_UNLOCK,

	/*!
	* @brief	��������
	*/
	CS_RIDE_UPGRADE_LEVEL,

	/*!
	* @brief	��������
	*/
	CS_RIDE_UPGRADE_STEP,
};

// ����2����ϢID  ����˷��ͻ���
enum SCRideMsg
{
	/*!
	 * @brief	����Ƥ����Ϣ
	 * @param int ��������
	 * @param int ������n
	 * @param	����ѭ��n��
		int Ƥ��id
		int ״̬
		int ��Ƥ����ʶ��0���� 1���ɣ�
	 */
	SC_RIDE_SKIN_MSG_RES = 0, 
};


// ���������� = ������ + �׼�*RIDE_BASE_PACKAGE_COEFFICIENT + �ȼ�
const int RIDE_BASE_PACKAGE_COEFFICIENT = 1000; 

#endif // __RideDefine_h__