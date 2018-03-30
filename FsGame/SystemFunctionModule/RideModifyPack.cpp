//--------------------------------------------------------------------
// �ļ���:		RideModifyPack.cpp
// ��  ��:		����������������
// ˵  ��:		
// ��������:	2014��12��32��
// ������:		
// �޸���:		 tongzt
//--------------------------------------------------------------------

#include "RideModule.h"
#include "FsGame/CommonModule/PropRefreshModule.h"
#include "FsGame/Define/GameDefine.h"

#include "utils/string_util.h"
#include "Define/CommandDefine.h"
#include "Define/BattleAbilityDefine.h"
#include "StaticDataQueryModule.h"
#include "Define/StaticDataDefine.h"

/*!
* @brief	��������������
*/
void RideModule::UpdateRideAddPkg(IKernel *pKernel, const PERSISTID &self, 
	const PERSISTID& ride, const char *pkg_id, const IVarList& up_msg)
{
	if (!pKernel->Exists(self) 
		|| !pKernel->Exists(ride))
	{
		return;
	}

	if (up_msg.GetCount() < 2)
	{
		return;
	}

	// ������������Ϣ
	const UpdatePkgTypes up_pkg_type = UpdatePkgTypes(up_msg.IntVal(0));	// ������������ʽ
	const ERefreshDataType refresh_type = ERefreshDataType(up_msg.IntVal(1));	// ����ˢ������

	CVarList prop_names;
	CVarList prop_values;
	bool ret = StaticDataQueryModule::m_pInstance->GetOneRowData(STATIC_DATA_RIDE_ATTRIBUTE_PACK, 
		pkg_id, prop_names, prop_values, VTYPE_FLOAT);
	if (!ret) 
	{
		return;
	}

	// ����-ֵ
	MapPropValue map_prop_value;

	// ȡ������������-ֵ
	if (!GetPckMapPropValue(pKernel, self, up_pkg_type, prop_names, prop_values, map_prop_value))
	{
		return;
	}

	// ���Ը���
	PropRefreshModule::m_pInstance->RefreshData(pKernel, self, map_prop_value, refresh_type);

	// ս��������
	pKernel->Command(self, self, CVarList() << COMMAND_MSG_UPDATE_BATTLE_ABILITY << BA_PROPERTY_TYPE);
}

/*!
* @brief	Ϊ������������������
*/
void RideModule::Upgrade2UpdateAddPkg(IKernel *pKernel, const PERSISTID &self, 
									  const PERSISTID& ride)
{
	if (!pKernel->Exists(self))
	{
		return;
	}

	IGameObj *pRideObj = pKernel->GetGameObj(ride);
	if (NULL == pRideObj)
	{
		return;
	}

	// ����״̬��ʱ��������
	if (pRideObj->QueryInt(FIELD_PROP_RIDE_ACTIVED) == RideState_Actived)
	{
		CVarList up_rm_msg;
		CVarList up_add_msg;

		// �Ƴ�
		up_rm_msg << GetRideUpPckType(pKernel, self, ride)
					<< EREFRESH_DATA_TYPE_REMOVE;

		// ����
		up_add_msg << GetRideUpPckType(pKernel, self, ride)
					<< EREFRESH_DATA_TYPE_ADD;

		// �Ƴ��ɵ�������
		std::string pre_pck_id = StringUtil::IntAsString(pRideObj->QueryInt(FIELD_PROP_PRE_RIDE_ADD_PKG));
		UpdateRideAddPkg(pKernel, self, ride, pre_pck_id.c_str(), up_rm_msg);

		// �����µ�������
		std::string pck_id = StringUtil::IntAsString(pRideObj->QueryInt(FIELD_PROP_RIDE_ADD_PKG));
		UpdateRideAddPkg(pKernel, self, ride, pck_id.c_str(), up_add_msg);
	}
}

/*!
* @brief	����ʹ��Ƥ��������
*/
void RideModule::UpdateUseSkinPkg(IKernel* pKernel, const PERSISTID& self, 
	const PERSISTID& ride, const IVarList& up_msg)
{
	if (!pKernel->Exists(self))
	{
		return;
	}

	IGameObj *pRideObj = pKernel->GetGameObj(ride);
	if (NULL == pRideObj)
	{
		return;
	}

	// Ƥ����
	IRecord *pSkinRec = pRideObj->GetRecord(FIELD_RECORD_RIDE_SKIN_REC);
	if (NULL == pSkinRec)
	{
		return;
	}

	const int inuse_row = pSkinRec->FindInt(COLUMN_RIDE_SKIN_REC_STATE, SKIN_USE_STATE_INUSE);
	if (inuse_row < 0)
	{
		return;
	}

	const int skin_id = pSkinRec->QueryInt(inuse_row, COLUMN_RIDE_SKIN_REC_ID);
	const RideSkin *ride_skin = GetRideSkin(skin_id);
	if (NULL == ride_skin)
	{
		return;
	}

	std::string att_pck_id = StringUtil::IntAsString(ride_skin->att_add_pck_id);
	UpdateRideAddPkg(pKernel, self, ride, att_pck_id.c_str(), up_msg);
}

/*!
* @brief	�������԰�
* @param	pKernel
* @param	Player 
* @param	Ride  
* @return	bool
*/
bool RideModule::SetRideAddPckID(IKernel* pKernel, 
							   const PERSISTID& Player, 
							   const PERSISTID& Ride)
{
	// �����ж�
	IGameObj* pRide = pKernel->GetGameObj(Ride);
	if (NULL == pRide)
	{
		return false;
	}

	// ����������
	const int base_pkg = pRide->QueryInt(FIELD_PROP_BASE_ADD_PKG);
	if (0 == base_pkg)
	{
		return false;
	}

	// ��ǰ�Ǽ��ͽ׼�
	const int ride_star = pRide->QueryInt(FIELD_PROP_RIDE_STAR);
	const int ride_step = pRide->QueryInt(FIELD_PROP_RIDE_STEP);

	// ��һ��������
	const int pre_add_pkg = pRide->QueryInt(FIELD_PROP_RIDE_ADD_PKG);

	// ����������
	const int ride_add_pkg = base_pkg + ride_step*RIDE_BASE_PACKAGE_COEFFICIENT + ride_star;
	
	if (pRide->SetInt(FIELD_PROP_RIDE_ADD_PKG, ride_add_pkg))
	{
		return pRide->SetInt(FIELD_PROP_PRE_RIDE_ADD_PKG, pre_add_pkg);
	}
	
	return false;
}

/*!
* @brief	ȡ��Ҫ��������Ҫ����������
* @param	up_pkg_type ���������·�ʽ
* @param	prop_names �������б� 
* @param	prop_values ����ֵ�б�
* @param	map_prop_value ��������������-ֵ
* @return	bool
*/
bool RideModule::GetPckMapPropValue(IKernel *pKernel, const PERSISTID &self, 
									const UpdatePkgTypes up_pkg_type, const IVarList& prop_names, 
									const IVarList& prop_values, std::map<std::string, double>& map_prop_value)
{
	// ������������������
	if (UPDATE_PKG_TYPE_ALL == up_pkg_type)
	{
		LoopBeginCheck(b);
		for (int i = 0; i < (int)prop_names.GetCount(); ++i)
		{
			LoopDoCheck(b);

			PropRefreshModule::m_pInstance->CountRefreshData(pKernel, self, 
				prop_names.StringVal(i), prop_values.DoubleVal(i), EModify_ADD_VALUE, map_prop_value);
		}
	}

	// ������������speed����
	if (UPDATE_PKG_TYPE_SPEED == up_pkg_type)
	{
		LoopBeginCheck(c);
		for (int i = 0; i < (int)prop_names.GetCount(); ++i)
		{
			LoopDoCheck(c);
			const char *prop_name = prop_names.StringVal(i);
			float prop_value = prop_values.FloatVal(i);

			// ��0
			if (DoubleEqual(prop_value, 0.0))
			{
				continue;
			}

			// ��speed���ԣ�����
			if (strcmp(prop_name, "RunSpeedAdd") != 0)
			{
				continue;
			}

			MapPropValue::iterator find_it = map_prop_value.find(prop_name);
			if (find_it != map_prop_value.end())
			{
				//�о��ۼ�
				(*find_it).second = (*find_it).second + prop_value;
			}
			else
			{
				//û�оͲ���
				map_prop_value.insert(std::make_pair(prop_name, prop_value));
			}
		}
	}

	// ������������speed�������
	if (UPDATE_PKG_TYPE_EXCEPT_SPEED == up_pkg_type)
	{
		LoopBeginCheck(d);
		for (int i = 0; i < (int)prop_names.GetCount(); ++i)
		{
			LoopDoCheck(d);
			const char *prop_name = prop_names.StringVal(i);
			double prop_value = prop_values.DoubleVal(i);

			// ��0
			if (DoubleEqual(prop_value, 0.0))
			{
				continue;
			}

			// ������speed����
			if (strcmp(prop_name, "RunSpeedAdd") == 0)
			{
				continue;
			}

			MapPropValue::iterator find_it = map_prop_value.find(prop_name);
			if (find_it != map_prop_value.end())
			{
				//�о��ۼ�
				(*find_it).second = (*find_it).second + prop_value;
			}
			else
			{
				//û�оͲ���
				map_prop_value.insert(std::make_pair(prop_name, prop_value));
			}
		}
	}

	// ����������
	if (map_prop_value.empty())
	{
		return false;
	}

	return true;
}

/*!
* @brief	ȡ��������ʾ������ʱ�ĸ�����������Ϣ
* @param	self   �Լ�
* @param	ride   ����
* @param	up_msg	������Ϣ
* @return	bool
*/
const UpdatePkgTypes RideModule::GetRideUpPckType(IKernel* pKernel, const PERSISTID& self, 
							  const PERSISTID& ride)
{
	if (!pKernel->Exists(self) 
		|| !pKernel->Exists(ride))
	{
		return UPDATE_PKG_TYPE_NONE;
	}

	// ��
	if (GetRidingState(pKernel, self) == RIDE_UP_DOWN_OPERATE_UP)
	{
		return UPDATE_PKG_TYPE_ALL;
	}
	else	// ��
	{
		return UPDATE_PKG_TYPE_EXCEPT_SPEED;
	}
}