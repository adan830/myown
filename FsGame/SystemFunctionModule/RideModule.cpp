//--------------------------------------------------------------------
// �ļ���:		RideModule.cpp
// ��  ��:		����
// ˵  ��:		
// ��������:	2014��12��32��
// ������:		
// �޸���:		 tongzt
//--------------------------------------------------------------------

#include "RideModule.h"
#include "FsGame/Define/ClientCustomDefine.h"
#include "FsGame/Define/ServerCustomDefine.h"
#include "FsGame/Define/CommandDefine.h"
#include "FsGame/CommonModule/LuaExtModule.h"
#include "FsGame/CommonModule/PropRefreshModule.h"
#include "FsGame/CommonModule/LogModule.h"
#include "FsGame/CommonModule/ContainerModule.h"
#include "FsGame/CommonModule/SwitchManagerModule.h"
#include "FsGame/Define/GameDefine.h"
#include "FsGame/Interface/FightInterface.h"
#include "FsGame/Define/ToolBoxSysInfoDefine.h"
#include "FsGame/SkillModule/Impl/SkillFlow.h"
#ifndef FSROOMLOGIC_EXPORTS
#include "FsGame/CommonModule/ReLoadConfigModule.h"
#endif
#include "FsGame/Define/ItemTypeDefine.h"

#include "utils/XmlFile.h"
#include "utils/string_util.h"
#include "utils/extend_func.h"
#include "utils/util_func.h"
#include "utils/custom_func.h"
#include "algorithm"
#include "FsGame/CommonModule/FunctionEventModule.h"
#include "CommonModule/EnvirValueModule.h"
#include "CapitalModule.h"
#include "ResetTimerModule.h"
#include "CommonModule/CommRuleModule.h"
#include "ActivateFunctionModule.h"

#define  PATH_RIDE_XML "ini/Ride/Ride.xml"
#define  PATH_RIDE_UPGRADE_STEP_XML "/ini/Ride/RideStepConfig.xml"
#define  PATH_RIDE_SKIN_XML "ini/Ride/RideSkin.xml"
#define  PATH_RIDE_SKIN_UNLOCK_XML "ini/Ride/RideSkinUnlock.xml"

RideModule* RideModule::m_pRideModule = NULL;

// �ؼ������������ļ�
int nx_reload_ride_config(void* state)
{
	// ��ú���ָ��
	IKernel* pKernel = LuaExtModule::GetKernel(state);
	// ģ���Ƿ��Ѿ�����
	if (NULL == pKernel)
	{
		return 0;
	}

	RideModule::m_pRideModule->LoadResource(pKernel);

	return 1;
}

// ����GM����
int nx_ride(void* state)
{
    IKernel* pKernel = LuaExtModule::GetKernel(state);
    if (pKernel == NULL)
    {
        return 1;
    }
	// �궨��  ���LUA�ű���������
    CHECK_ARG_NUM(state, nx_ride, 3);
	// �궨��  ����һ����������
    CHECK_ARG_OBJECT(state, nx_ride, 1)
	// �궨��  ���ڶ�����������
    CHECK_ARG_STRING(state, nx_ride, 2);
	// �궨��  ����������������
    CHECK_ARG_STRING(state, nx_ride, 3);

    // read params
    PERSISTID self = pKernel->LuaToObject(state, 1);
    const char *opt = pKernel->LuaToString(state, 2);
    const char *var = pKernel->LuaToString(state, 3);

    if (strcmp(opt, "add") == 0)
    {
        pKernel->LuaPushInt(state, RideModule::m_pRideModule->AddRide(pKernel, self, var));
    }
    else if (strcmp(opt, "chg") == 0)
    {
        pKernel->LuaPushBool(state, RideModule::m_pRideModule->ChangeRideSkin(pKernel, self, StringUtil::StringAsInt(var)));
    }
    else if (strcmp(opt, "addskin") == 0) 
    {
		pKernel->LuaPushInt(state, RideModule::m_pRideModule->AddRideSkin(pKernel, self, StringUtil::StringAsInt(var)));
    }
    else if (strcmp(opt, "show") == 0)
    {
        pKernel->LuaPushInt(state, RideModule::m_pRideModule->OnRide(pKernel, self));
    }
    else if (strcmp(opt, "hide") == 0)
    {
        pKernel->LuaPushInt(state, RideModule::m_pRideModule->DownRide(pKernel, self));
	}

    return 1;
}

// ��ʼ��
bool RideModule::Init(IKernel* pKernel)
{
	// ����ָ��
    m_pRideModule = this;

	// ���������߻ص�
    pKernel->AddEventCallback("player", "OnRecover", RideModule::OnPlayerRecover, -1);

	// �뿪����
	pKernel->AddEventCallback("player", "OnLeaveScene", RideModule::OnPlayerLeave);

	// ������׼�������ص�
    pKernel->AddEventCallback("player", "OnReady", RideModule::OnPlayerReady);

	// ��Ҷ�������
	pKernel->AddIntCustomHook("player", CLIENT_CUSTOMMSG_CONTINUE_ON_READY, RideModule::OnPlayerReady);

	// ���������λ�����ص�
	pKernel->AddEventCallback(CLASS_NAME_RIDE_BOX, "OnCreate", RideModule::OnRideBoxCreate);

	// ���������λ�ഴ���ص�
	pKernel->AddClassCallback(CLASS_NAME_RIDE_BOX, "OnCreateClass", RideModule::OnRideBoxClassCreate);

	// ע���ڲ���Ϣ�ص�����ʼʹ��ĳ���
    pKernel->AddIntCommandHook("player", COMMAND_SKILL_BEGIN, RideModule::OnCommandPlayerBeginSkill);

	// ע���ڲ���Ϣ�ص�������ʹ�õļ������̵Ľ���
    pKernel->AddIntCommandHook("player", COMMAND_SKILL_FINISH, RideModule::OnCommandPlayerFinishSkill);

	// ע���ڲ���Ϣ�ص�����buffer
	//pKernel->AddIntCommandHook("player", COMMAND_ADD_BUFFER_TO_OBJECT, RideModule::OnCommandAddBuffer);

	//��ɱ��Ϣ
	pKernel->AddIntCommandHook("player", COMMAND_BEKILL, RideModule::OnCommandBeKill, 100);

	//���˺���Ϣ
	pKernel->AddIntCommandHook("player", COMMAND_BEDAMAGE, RideModule::OnCommandBeDamage, 100);

	// ע��ͻ��˷�������ϵͳ��Ϣ�ص�
	pKernel->AddIntCustomHook("player", CLIENT_CUSTOMMSG_RIDE, RideModule::OnCustomMessage);

	// ���ܿ���
	pKernel->AddIntCommandHook("player", COMMAND_MSG_ACTIVATE_FUNCTION, RideModule::OnCommandActiveFunc);

    if (!LoadResource(pKernel)) 
	{
        return false;
    }

	DECL_RESET_TIMER(RESET_RIDE, RideModule::MountResetTimer);

#ifndef FSROOMLOGIC_EXPORTS
	RELOAD_CONFIG_REG("RideModule", RideModule::ReloadRideConfig);
#endif

    DECL_LUA_EXT(nx_ride);
	DECL_LUA_EXT(nx_reload_ride_config);

    return true;
}

bool RideModule::Shut(IKernel* pKernel)
{
	// ����Զ��建��
	m_RideStepCfgList.clear();
	m_RideSkinMap.clear();
	m_SkinUnlockCfgVec.clear();

    return true;
}

// �������������ļ�
bool RideModule::LoadResource(IKernel *pKernel)
{
    if (!pKernel->PreloadConfig(PATH_RIDE_XML))
    {
        return false;
    }
	if (!LoadUpgradeStepRes(pKernel))
	{
		return false;
	}
	if (!LoadRideSkinRes(pKernel))
	{
		return false;
	}

	if (!LoadSkinUnlocRes(pKernel))
	{
		return false;
	}

    return true;
}

// ���¼�����������
void RideModule::ReloadRideConfig(IKernel* pKernel)
{
	RideModule::m_pRideModule->LoadResource(pKernel);
}

/*!
* @brief	���������׼�����
*/
bool RideModule::LoadUpgradeStepRes(IKernel* pKernel)
{
	m_RideStepCfgList.clear();
	m_MaxStep = 0;

	std::string file_path = pKernel->GetResourcePath();
	file_path += PATH_RIDE_UPGRADE_STEP_XML;

	// xml��ȡ
	CXmlFile xml(file_path.c_str());
	if (!xml.LoadFromFile())
	{
		std::string err_msg = file_path;
		err_msg.append(" does not exists.");
		::extend_warning(LOG_ERROR, err_msg.c_str());
		return false;
	}

	// ����
	LoopBeginCheck(aa);
	for (int i = 0; i < (int)xml.GetSectionCount(); ++i)
	{
		LoopDoCheck(aa);
		const char *sec = xml.GetSectionByIndex(i);
		if (StringUtil::CharIsNull(sec))
		{
			continue;
		}

		// �׼�
		const int step = xml.ReadInteger(sec, "Step", 0);
		if (m_MaxStep < step)
		{
			m_MaxStep = step;
		}

		RideStepCfg step_cfg(step);
		step_cfg.need_star = xml.ReadInteger(sec, "NeedStar", 0);
		step_cfg.skin_id = xml.ReadString(sec, "SkinID", "");

		String_Vec upgrade_item_vec;
		xml.ReadList<std::string>(sec, "UpgradeItems", ";", &upgrade_item_vec);

		String_Vec::const_iterator start_it = upgrade_item_vec.begin();
		String_Vec::const_iterator end_it = upgrade_item_vec.end();

		LoopBeginCheck(b);
		for (; start_it != end_it; ++start_it)
		{
			LoopDoCheck(b);
			Consume_Vec consume_vec;
			CommRuleModule::ParseConsumeVec(start_it->c_str(), consume_vec);
			step_cfg.upgrade_items_list.push_back(consume_vec);
		}

		m_RideStepCfgList.push_back(step_cfg);
	}

	return true;
}

/*!
* @brief	����Ƥ�������ļ�����
*/
bool RideModule::LoadRideSkinRes(IKernel *pKernel)
{
	m_RideSkinMap.clear();

	// �ļ�·��
	std::string xmlPath = pKernel->GetResourcePath();
	xmlPath+=PATH_RIDE_SKIN_XML;

	CXmlFile xml(xmlPath.c_str());
	if (!xml.LoadFromFile())
	{
		std::string err_msg = xmlPath;
		err_msg.append(" does not exists.");
		::extend_warning(LOG_ERROR, err_msg.c_str());
		return false;
	}

	// ����XML
	LoopBeginCheck(d);
	for (int i = 0; i < (int)xml.GetSectionCount(); ++i)
	{
		LoopDoCheck(d);
		const char* section = xml.GetSectionByIndex(i);
		if(StringUtil::CharIsNull(section))
		{
			continue;
		}

		RideSkin ride_skin;
		ride_skin.skin_id = StringUtil::StringAsInt(section);
		ride_skin.att_add_pck_id = xml.ReadInteger(section, "ActAddPackage", 0);
		ride_skin.special_item = xml.ReadInteger(section, "SpecialItem", 0);
		ride_skin.skin_type = xml.ReadInteger(section, "SkinType", 0);

		m_RideSkinMap.insert(RideSkinMap::value_type(ride_skin.skin_id, ride_skin));
	}

	return true;
}

// ����Ƥ���������ü���
bool RideModule::LoadSkinUnlocRes(IKernel *pKernel)
{
	m_SkinUnlockCfgVec.clear();

	std::string config_file_path = pKernel->GetResourcePath();
	config_file_path.append(PATH_RIDE_SKIN_UNLOCK_XML);

	CXmlFile xml(config_file_path.c_str());
	if (!xml.LoadFromFile())
	{
		std::string err_msg = config_file_path;
		err_msg.append(" does not exists.");
		::extend_warning(LOG_ERROR, err_msg.c_str());
		return false;
	}

	CVarList sec_list;
	xml.GetSectionList(sec_list);

	int sec_count = (int)sec_list.GetCount();

	LoopBeginCheck(a)
	for (int i = 0; i < sec_count; i++)
	{
		LoopDoCheck(a)
			const char *sec = sec_list.StringVal(i);
		if (StringUtil::CharIsNull(sec))
		{
			continue;
		}

		const int id = xml.ReadInteger(sec, "SkinID", 0);
		if (id == 0)
		{
			continue;
		}

		SkinUnlockCfg cfg(id);
		cfg.pre_skin = xml.ReadInteger(sec, "PreSkinID", 0);
		const char *unlock_fee = xml.ReadString(sec, "UnlockFee", "");
		CommRuleModule::ParseConsumeVec(unlock_fee, cfg.unlock_fee);	
		const char *unlock_items = xml.ReadString(sec, "UnlockItems", "");
		CommRuleModule::ParseConsumeVec(unlock_fee, cfg.unlock_items);
		
		m_SkinUnlockCfgVec.push_back(cfg);
	}

	return true;
}

/*!
* @brief	�������Ƥ������
*/
const RideSkin* RideModule::GetRideSkin(const int skin_id)
{
	// ����
	RideSkinMap::const_iterator find_it = m_RideSkinMap.find(skin_id);
	if (find_it != m_RideSkinMap.end())
	{
		return &(find_it->second);
	}

	return NULL;
}

// ��������ȡ��Ƥ��ID
const int RideModule::GetSkinByType(const int skin_type)
{
	// ����
	RideSkinMap::const_iterator start_it = m_RideSkinMap.begin();
	RideSkinMap::const_iterator end_it = m_RideSkinMap.end();
	
	LoopBeginCheck(a);
	for (; start_it != end_it; ++start_it)
	{
		LoopDoCheck(a);
		if (start_it->second.skin_type == skin_type)
		{
			return start_it->second.skin_id;
		}
	}

	return 0;
}

// ��������ȡ��Ƥ��ID
const int RideModule::GetSkinByType(IKernel *pKernel, const PERSISTID &self, 
	const int skin_type)
{
	// �����ж�
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return 0;
	}

	// ȡ��Ƥ�����õ��������
	PERSISTID apply_ride = GetActivedRide(pKernel, self);
	IGameObj *pApplyRide = pKernel->GetGameObj(apply_ride);
	if (NULL == pApplyRide)
	{
		return 0;
	}

	// Ƥ����
	IRecord *pSkinRec = pApplyRide->GetRecord(FIELD_RECORD_RIDE_SKIN_REC);
	if (NULL == pSkinRec)
	{
		return 0;
	}

	// ����
	LoopBeginCheck(a);
	for (int row = 0; row < pSkinRec->GetRows(); ++row)
	{
		LoopDoCheck(a);
		const int skin_id = pSkinRec->QueryInt(row, COLUMN_RIDE_SKIN_REC_ID);
		const RideSkin *ride_skin = GetRideSkin(skin_id);
		if (NULL == ride_skin)
		{
			continue;
		}

		// ��ͬ����
		if (ride_skin->skin_type == skin_type)
		{
			return skin_id;
		}
	}

	return 0;
}

// ��ѯ��ǰ����׼�
const int RideModule::QueryRideStep(IKernel *pKernel, const PERSISTID &self)
{
	// �ҵ����������
	PERSISTID ride = GetActivedRide(pKernel, self);
	IGameObj *pRideObj = pKernel->GetGameObj(ride);
	if (NULL == pRideObj)
	{
		return 0;
	}

	return pRideObj->QueryInt(FIELD_PROP_RIDE_STEP);
}

/*!
* @brief	�ɷ�������
*/
bool RideModule::CanRide(IKernel* pKernel, 
							const PERSISTID& self)
{
	// �����ж�
	IGameObj* pSelf = pKernel->GetGameObj(self);
	if (NULL == pSelf)
	{
		return false;
	}

	IGameObj *pScene = pKernel->GetSceneObj();
	if (NULL == pScene)
	{
		return false;
	}

	if (pScene->QueryInt(FIELD_PROP_CAN_SHOW_RIDE) == 0)
	{
		return false;
	}

	//// linknpc����
	//if (pKernel->Exists(pSelf->QueryObject(FIELD_PROP_LINK_NPC)))
	//{
	//	return false;
	//}

	// �����Ƿ������ͷ�
	if (SkillFlowSingleton::Instance()->IsSkillUseBusy(pKernel, self) != USESKILL_RESULT_SUCCEED)
	{
		return false;
	}

	// �����ս��״̬��ֱ���˳�
	if (pSelf->QueryInt(FIELD_PROP_FIGHT_STATE) > 0)
	{
		return false;
	}

	//// �Ṧ�в���������
	//if (FightInterfaceInstance->IsInFlySkill(pKernel, self))
	//{
	//	return false;
	//}

	// ��������
	/*if (!StringUtil::CharIsNull(pSelf->QueryString(FIELD_PROP_TASK_RESOURCE)))
	{
	return false;
	}*/


    return true;
}

/*!
* @brief	const int

*/
const int RideModule::GetRidingState(IKernel* pKernel, 
							const PERSISTID& self)
{
	// �����ж�
	IGameObj* pSelf = pKernel->GetGameObj(self);
	if (NULL == pSelf)
	{
		return RIDE_UP_DOWN_OPERATE_DOWN;
	}

	if (pSelf->QueryInt(FIELD_PROP_ACTIVATED_RIDE) == 0)
	{
		return RIDE_UP_DOWN_OPERATE_DOWN;
	}
	else
	{
		return RIDE_UP_DOWN_OPERATE_UP;
	}
}

/*!
* @brief	ʹ��Ĭ��Ƥ��
*/
bool RideModule::UseDefaultSkin(IKernel *pKernel, const PERSISTID &ride )
{
	IGameObj *pRideObj = pKernel->GetGameObj(ride);
	if (NULL == pRideObj)
	{
		return false;
	}

	// Ĭ��Ƥ��id
	const int skin_id = pRideObj->QueryInt(FIELD_PROP_DEFAULT_SKIN);

	// ȡ�ø�Ƥ������
	const RideSkin* ride_skin = GetRideSkin(skin_id);
	if (NULL == ride_skin)
	{
		return false;
	}

	// Ƥ����
	IRecord *pSkinRec = pRideObj->GetRecord(FIELD_RECORD_RIDE_SKIN_REC);
	if (NULL == pSkinRec)
	{
		return false;
	}

	// ����
	int exist_row = pSkinRec->FindInt(COLUMN_RIDE_SKIN_REC_ID, skin_id);
	if (exist_row < 0)
	{
		// �޼�¼������
		CVarList new_row;
		new_row << skin_id << SKIN_USE_STATE_UNUSE << 1;

		exist_row = pSkinRec->AddRowValue(-1, new_row);
	}

	// �޼�¼������
	if (exist_row < 0)
	{
		return false;
	}

	// ����Ϊʹ��״̬
	return pSkinRec->SetInt(exist_row, COLUMN_RIDE_SKIN_REC_STATE, SKIN_USE_STATE_INUSE);
}

// ǰ��Ƥ���Ƿ��ѽ���
bool RideModule::IsUnlockPreSkin(IKernel* pKernel, const PERSISTID& self, 
	const int skin_id)
{
	// �����ж�
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return false;
	}

	// ��ǰ��
	if (skin_id == 0)
	{
		return true;
	}

	// ȡ��Ƥ�����õ��������
	PERSISTID apply_ride = GetActivedRide(pKernel, self);
	IGameObj *pApplyRide = pKernel->GetGameObj(apply_ride);
	if (NULL == pApplyRide)
	{
		return false;
	}

	// Ƥ����
	IRecord *pSkinRec = pApplyRide->GetRecord(FIELD_RECORD_RIDE_SKIN_REC);
	if (NULL == pSkinRec)
	{
		return false;
	}

	const RideSkin *ride_skin = GetRideSkin(skin_id);
	if (NULL == ride_skin)
	{
		// ��Ч��ǰ��
		return true;
	}

	// �ѽ���
	const int exist_row = pSkinRec->FindInt(COLUMN_RIDE_SKIN_REC_ID, skin_id);
	if (exist_row >= 0)
	{
		return true;
	}

	return false;
}

// Ƥ���ɷ����
bool RideModule::IsUnlock(IKernel* pKernel, const PERSISTID& self, 
	const int skin_id)
{
	// �����ж�
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return false;
	}

	// ȡ��Ƥ�����õ��������
	PERSISTID apply_ride = GetActivedRide(pKernel, self);
	IGameObj *pApplyRide = pKernel->GetGameObj(apply_ride);
	if (NULL == pApplyRide)
	{
		return false;
	}

	// Ƥ����
	IRecord *pSkinRec = pApplyRide->GetRecord(FIELD_RECORD_RIDE_SKIN_REC);
	if (NULL == pSkinRec)
	{
		return false;
	}

	const RideSkin *ride_skin = GetRideSkin(skin_id);
	if (NULL == ride_skin)
	{
		return false;
	}

	// �ѽ���
	const int exist_row = pSkinRec->FindInt(COLUMN_RIDE_SKIN_REC_ID, skin_id);
	if (exist_row >= 0)
	{
		return true;
	}

	return false;
}

// �ɷ��л�Ϊ����Ƥ��
bool RideModule::CanChangeStepSkin(IKernel* pKernel, const PERSISTID& self)
{
	// �����ж�
	IGameObj* pSelf = pKernel->GetGameObj(self);
	if (NULL == pSelf)
	{
		return false;
	}

	PERSISTID ride = GetActivedRide(pKernel, self);
	IGameObj *pRideObj = pKernel->GetGameObj(ride);
	if (NULL == pRideObj)
	{
		return false;
	}

	// Ƥ����
	IRecord *pSkinRec = pRideObj->GetRecord(FIELD_RECORD_RIDE_SKIN_REC);
	if (NULL == pSkinRec)
	{
		return false;
	}

	// ��ʹ���е�Ƥ��
	const int inuse_row = pSkinRec->FindInt(COLUMN_RIDE_SKIN_REC_STATE, SKIN_USE_STATE_INUSE);
	if (inuse_row < 0)
	{
		return true;
	}

	// ʹ���е�Ƥ��id
	const int inuse_skin = pSkinRec->QueryInt(inuse_row, COLUMN_RIDE_SKIN_REC_ID);
	const RideSkin *skin_cfg = GetRideSkin(inuse_skin);
	if (NULL == skin_cfg)
	{
		return true;
	}

	return skin_cfg->skin_type == RIDE_SKIN_TYPE_STEP;
}

/*!
* @brief	��������(ʹ����Ʒ)
*/
bool RideModule::AddRide(IKernel *pKernel,
						const PERSISTID &self, 
						const char *rideConfig)
{
	// �����ж�
	IGameObj* pSelf = pKernel->GetGameObj(self);
	if (NULL == pSelf)
	{
		return false;
	}

	// ��ȡ���ﱳ��
    PERSISTID ridebox = pKernel->GetChild(self, L"RideBox");
    if (!pKernel->Exists(ridebox))
    {
		return false;
    }

    int rideMax = pKernel->GetCapacity(ridebox);
    int rideCount = pKernel->GetChildCount(ridebox);
    if (rideCount >= rideMax)
    {
		return false;
    }

	//����Ѿ������˲������
	PERSISTID have_ride = ContainerModule::m_pContainerModule->FindItem(pKernel, ridebox, rideConfig);
	if ( pKernel->Exists(have_ride) )
	{
		return false;
	}

    int pos;
	LoopBeginCheck(e);
    for (pos = 1; pos < rideMax; pos++)
    {
		LoopDoCheck(e);
        PERSISTID rideInContainer = pKernel->GetItem(ridebox, pos);
        if (rideInContainer.IsNull()) 
		{
            break;
        }
    }

	// ����������
	PERSISTID newRide = pKernel->CreateFromConfig(pKernel->GetScene(), "RideItem", rideConfig);
	IGameObj* pNewRide = pKernel->GetGameObj(newRide);
	if (NULL == pNewRide)
	{
		return false;
	}

	bool placeResult = pKernel->PlacePos(newRide, ridebox, pos);
	if (false == placeResult)
	{
		return false;
	}

	// ���ó�ʼ���԰�
	SetRideAddPckID(pKernel, self, newRide);

	// ����
	ActiveRide(pKernel, self, newRide);

	// ��������Ƥ����Ϣ
	PushRideSkinMsg(pKernel, self, pos);

    return true;
}

/*!
* @brief	��������Ƥ��
*/
bool RideModule::AddRideSkin(IKernel *pKernel, const PERSISTID &self, 
							 const int skin_id)
{
	// �����ж�
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return false;
	}

	// ȡ��Ƥ�����õ��������
	PERSISTID apply_ride = GetActivedRide(pKernel, self);
	IGameObj *pApplyRide = pKernel->GetGameObj(apply_ride);
	if (NULL == pApplyRide)
	{
		return false;
	}

	// �ѽ���
	if (IsUnlock(pKernel, self, skin_id))
	{
		CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_19401, CVarList());
		return false;
	}

	// Ƥ����
	IRecord *pSkinRec = pApplyRide->GetRecord(FIELD_RECORD_RIDE_SKIN_REC);
	if (NULL == pSkinRec)
	{
		return false;
	}

	const RideSkin *ride_skin = GetRideSkin(skin_id);
	if (NULL == ride_skin)
	{
		return false;
	}

	// ����
	const int exist_row = pSkinRec->FindInt(COLUMN_RIDE_SKIN_REC_ID, skin_id);
	if (exist_row >= 0)
	{
		return true;
	}
	
	// �޼�¼������
	CVarList new_row;
	new_row << skin_id << SKIN_USE_STATE_UNUSE << 0;

	if (pSkinRec->AddRowValue(-1, new_row) < 0)
	{
		return false;
	}

	// ����
	if (ride_skin->special_item > 0)
	{
		/*char str_nation[128] = { 0 };
		SPRINTF_S(str_nation, "Nation%d", pSelfObj->QueryInt(FIELD_PROP_NATION));
		const wchar_t* name = pSelfObj->GetName();

		CVarList args;
		args << str_nation
		<< name
		<< pSelfObj->QueryInt(FIELD_PROP_VIP_LEVEL)
		<< skin_id;
		::CustomSysInfoBroadcast(pKernel, TIPSTYPE_SPECIAL_ITEM_MESSAGE, SYS_INFO_UNLOCK_RIDESKIN, args);*/
	}

	// ��¼����Ƥ����־��
	/*RideSkinLog log;
	log.config_id = pApplyRide->GetConfig();
	log.opt_type = LOG_RIDE_ACTIVE_SKIN;
	log.cur_step = pApplyRide->QueryInt(FIELD_PROP_RIDE_STEP);
	log.cur_star = pApplyRide->QueryInt(FIELD_PROP_RIDE_STAR);
	log.skin_id = skin_id;
	LogModule::m_pLogModule->OnRideChangeSkin(pKernel, self, log);*/

	// ����Ƥ��
	if (CanChangeStepSkin(pKernel, self))
	{
		ChangeRideSkin(pKernel, self, skin_id);
	}

	// ��������Ƥ����Ϣ
	PushRideSkinMsg(pKernel, self, pKernel->GetIndex(apply_ride));

	return true;
}

// ɾ������Ƥ��
bool RideModule::RemoveRideSkin(IKernel *pKernel, const PERSISTID &self, 
	const int skin_type)
{
	// �����ж�
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return false;
	}

	// ȡ�ø�����Ƥ��
	const int skin_id = GetSkinByType(pKernel, self, skin_type);
	const RideSkin *ride_skin = GetRideSkin(skin_id);
	if (NULL == ride_skin)
	{
		return false;
	}

	// ȡ��Ƥ�����õ��������
	PERSISTID apply_ride = GetActivedRide(pKernel, self);
	IGameObj *pApplyRide = pKernel->GetGameObj(apply_ride);
	if (NULL == pApplyRide)
	{
		return false;
	}

	// Ƥ����
	IRecord *pSkinRec = pApplyRide->GetRecord(FIELD_RECORD_RIDE_SKIN_REC);
	if (NULL == pSkinRec)
	{
		return false;
	}

	// ����
	const int exist_row = pSkinRec->FindInt(COLUMN_RIDE_SKIN_REC_ID, skin_id);
	if (exist_row < 0)
	{
		return true;
	}

	const int state = pSkinRec->QueryInt(exist_row, COLUMN_RIDE_SKIN_REC_STATE);
	const UpdatePkgTypes update_type = GetRideUpPckType(pKernel, self, apply_ride);

	// �Ƴ�Ƥ��
	pSkinRec->RemoveRow(exist_row);

	// ʹ���е�Ƥ��
	if (SKIN_USE_STATE_INUSE == state)
	{
		CVarList rm_inuse_msg;
		rm_inuse_msg << update_type
					 << EREFRESH_DATA_TYPE_REMOVE;

		// �Ƴ�Ƥ��ʹ��ʱ��������
		std::string inuse_pck_id = StringUtil::IntAsString(ride_skin->att_add_pck_id);
		UpdateRideAddPkg(pKernel, self, apply_ride, inuse_pck_id.c_str(), rm_inuse_msg);

		// ʹ����������Ĭ��Ƥ��
		if (pSkinRec->GetRows() > 0)
		{
			ChangeRideSkin(pKernel, self, pSkinRec->QueryInt(0, COLUMN_RIDE_SKIN_REC_ID));
		}
	}

	// ��¼ɾ����Ƥ����־��
	/*RideSkinLog log;
	log.config_id = pApplyRide->GetConfig();
	log.opt_type = LOG_RIDE_REMOVE_SKIN;
	log.cur_step = pApplyRide->QueryInt(FIELD_PROP_RIDE_STEP);
	log.cur_star = pApplyRide->QueryInt(FIELD_PROP_RIDE_STAR);
	log.skin_id = skin_id;
	LogModule::m_pLogModule->OnRideChangeSkin(pKernel, self, log);*/

	// ��������Ƥ����Ϣ
	PushRideSkinMsg(pKernel, self, pKernel->GetIndex(apply_ride));

	return true;
}

/*!
* @brief	������
*/
bool RideModule::OnRide(IKernel *pKernel, 
						 const PERSISTID &self)
{
	// �����ж�
	IGameObj* pSelf = pKernel->GetGameObj(self);
	if (NULL == pSelf)
	{
		return false;
	}

	// ��������
	if (GetRidingState(pKernel, self) == RIDE_UP_DOWN_OPERATE_UP)
	{
		return false;
	}

    // �ҵ����������
    PERSISTID activedRide = GetActivedRide(pKernel, self);
	IGameObj* pActivedRide = pKernel->GetGameObj(activedRide);
	if (NULL == pActivedRide)
	{
		return false;
	}

	// Ƥ����
	IRecord *pSkinRec = pActivedRide->GetRecord(FIELD_RECORD_RIDE_SKIN_REC);
	if (NULL == pSkinRec)
	{
		return false;
	}

	// ʹ���е�Ƥ��
	const int inuse_row = pSkinRec->FindInt(COLUMN_RIDE_SKIN_REC_STATE, SKIN_USE_STATE_INUSE);
	if (inuse_row < 0)
	{
		return false;
	}

	// �滻Ϊʹ���е�Ƥ��id
   const int inuse_skin = pSkinRec->QueryInt(inuse_row, COLUMN_RIDE_SKIN_REC_ID);

	CVarList up_msg;
	up_msg << UPDATE_PKG_TYPE_SPEED
		   << EREFRESH_DATA_TYPE_ADD;

	// ��������Ƥ��������
	m_pRideModule->UpdateUseSkinPkg(pKernel, self, activedRide, up_msg);

	//pKernel->CustomByKen(self, CVarList() << SERVER_CUSTOMMSG_RIDE << SC_RIDE_SHOW << self << inuse_skin);

	pSelf->SetInt(FIELD_PROP_ACTIVATED_RIDE, inuse_skin);
    return true;
}

/*!
* @brief	������
*/
bool RideModule::DownRide(IKernel *pKernel, 
						 const PERSISTID &self)
{
	// �����ж�
	IGameObj* pSelf = pKernel->GetGameObj(self);
	if (NULL == pSelf)
	{
		return false;
	}

	// ��������
	if (GetRidingState(pKernel, self) == RIDE_UP_DOWN_OPERATE_DOWN)
    {
		return false;
    }

    // �ҵ����������
    PERSISTID activedRide = GetActivedRide(pKernel, self);
	IGameObj *pActiveRide = pKernel->GetGameObj(activedRide);
    if (NULL == pActiveRide)
    {
		return false;
    }

	// ��������value
    pSelf->SetInt(FIELD_PROP_ACTIVATED_RIDE, 0);

	CVarList up_msg;
	up_msg << UPDATE_PKG_TYPE_SPEED
		   << EREFRESH_DATA_TYPE_REMOVE;

	// ��������Ƥ��������
	m_pRideModule->UpdateUseSkinPkg(pKernel, self, activedRide, up_msg);

    return true;
}


/*!
* @brief	��������
*/
bool RideModule::ActiveRide(IKernel *pKernel,  const PERSISTID &self, 
						   const PERSISTID &ride)
{
	// �ж�
	IGameObj* pSelf = pKernel->GetGameObj(self);
	if (NULL == pSelf)
	{
		return false;
	}

	IGameObj* pRide = pKernel->GetGameObj(ride);
	if (NULL == pRide)
	{
		return false;
	}

	// �������Ｄ��״̬
	pRide->SetInt(FIELD_PROP_RIDE_ACTIVED, RideState_Actived);

	// ʹ��Ĭ��Ƥ��
	if (!UseDefaultSkin(pKernel, ride))
	{
		return false;
	}

	// ����������
	CVarList up_msg;
	up_msg << UPDATE_PKG_TYPE_EXCEPT_SPEED
		   << EREFRESH_DATA_TYPE_ADD;

	// ��������������
	std::string pkg_id = StringUtil::IntAsString(pRide->QueryInt(FIELD_PROP_RIDE_ADD_PKG));
	m_pRideModule->UpdateRideAddPkg(pKernel, self, ride, pkg_id.c_str(), up_msg);

	// ��һ�μ��ֱ����ʾ
	if (m_pRideModule->CanRide(pKernel,self))
	{		
		OnRide(pKernel, self);
	}

#ifndef FSROOMLOGIC_EXPORTS
	// ��¼���Ｄ����־
	/*RideBaseLog log;
	log.config_id = pRide->GetConfig();
	log.opt_type = LOG_RIDE_ACTIVATE;
	log.cur_star = pRide->QueryInt(FIELD_PROP_RIDE_STAR);
	log.cur_step = pRide->QueryInt(FIELD_PROP_RIDE_STEP);
	LogModule::m_pLogModule->OnRideActive(pKernel, self, log);*/
#endif // FSROOMLOGIC_EXPORTS

    return true;
}

/*!
* @brief	��ȡ���������
*/
PERSISTID RideModule::GetActivedRide(IKernel *pKernel, 
									 const PERSISTID &self)
{
	// ��ȡ���ﱳ��
    PERSISTID RideBox = pKernel->GetChild(self, L"RideBox");
    if (RideBox.IsNull())
    {
        return PERSISTID();
    }

    // �ҵ��Ѿ����������
    unsigned int iPos = 0;
    PERSISTID Ride = pKernel->GetFirst(RideBox, iPos);
    IGameObj* pRideObj = pKernel->GetGameObj(Ride);
	if (pRideObj == NULL)
	{
		return PERSISTID();
	}

    int iStatus = pRideObj->QueryInt(FIELD_PROP_RIDE_ACTIVED);

	LoopBeginCheck(f);
    while (iStatus != RideState_Actived)
    {
		LoopDoCheck(f);
        Ride = pKernel->GetNext(RideBox, iPos);
		pRideObj = pKernel->GetGameObj(Ride);
		if (pRideObj == NULL)
		{
			break;
		}

        iStatus = pRideObj->QueryInt(FIELD_PROP_RIDE_ACTIVED);
    }

    return Ride;
}

/*!
* @brief	���������ж�
*/
bool RideModule::CanActiveRide(IKernel* pKernel, const PERSISTID& self, 
							   const char *ride_config)
{
	if (!pKernel->Exists(self))
	{
		return false;
	}

	//// �����ж�
	//IGameObj* pRide = pKernel->GetGameObj(Ride);
	//if (NULL == pRide)
	//{
	//	return false;
	//}

	//IGameObj* pPlayer = pKernel->GetGameObj(Player);
	//if (NULL == pPlayer)
	//{
	//	return false;
	//}

	//// ��������
	//int iFilterType = pRide->QueryInt("RideCondition");
	//if (0 == iFilterType)
	//{
	//	return true;
	//}

	//bool result = true;

	//switch(iFilterType)
	//{
	//case RIDE_FILTER_TYPE_KING: // �����ж�
	//	{

	//		break;
	//	}		
	//default:
	//    break;
	//}

	//return result;
	return true;
}

/*!
* @brief	���������Ϣ
* @param	step �׼�
* @return	RideStepCfg*
*/
const RideStepCfg* RideModule::GetRideStepCfg(const int step)
{
	RideStepCfg find_step(step);
	RideStepCfgList::const_iterator find_it = find(
		m_RideStepCfgList.begin(), 
		m_RideStepCfgList.end(), 
		find_step);

	if (find_it != m_RideStepCfgList.end())
	{
		return &(*find_it);
	}

	return NULL;
}

// ȡ��Ƥ����������
const SkinUnlockCfg* RideModule::GetSkinUnlockCfg(const int id)
{
	if (id == 0)
	{
		return NULL;
	}

	SkinUnlockCfg temp(id);
	SkinUnlockCfgVec::const_iterator find_it = find(
		m_SkinUnlockCfgVec.begin(),
		m_SkinUnlockCfgVec.end(),
		temp);

	if (find_it != m_SkinUnlockCfgVec.end())
	{
		return &(*find_it);
	}

	return NULL;
}

// �ɷ�����
bool RideModule::CanUpgradeStep(const int step, const int star)
{
	const RideStepCfg *step_cfg = GetRideStepCfg(step);
	if (NULL == step_cfg)
	{
		return false;
	}

	return star == step_cfg->need_star;
}

/*!
* @brief	��������Ƥ��
*/
bool RideModule::ChangeRideSkin(IKernel *pKernel, const PERSISTID &self, 
								const int new_skin_id, const int ride_index /*= 1*/)
{
	IGameObj *pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return false;
	}	

	PERSISTID ride = GetActivedRide(pKernel, self);
	IGameObj *pRideObj = pKernel->GetGameObj(ride);
	if (NULL == pRideObj)
	{
		return false;
	}

	// ����Ƥ����
	IRecord *pSkinRec = pRideObj->GetRecord(FIELD_RECORD_RIDE_SKIN_REC);
	if (NULL == pSkinRec)
	{
		return false;
	}

	// ��Ƥ���Ƿ����ڸ�����
	const int exist_row = pSkinRec->FindInt(COLUMN_RIDE_SKIN_REC_ID, new_skin_id);
	if (exist_row < 0)
	{
		// �л�ʧ��
		return false;
	}

	// ���ҵ�ǰ��ʹ���е�Ƥ��
	const int inuse_row = pSkinRec->FindInt(COLUMN_RIDE_SKIN_REC_STATE, SKIN_USE_STATE_INUSE);
	if (inuse_row >= 0)
	{
		// ���þ�Ƥ��δʹ��
		pSkinRec->SetInt(inuse_row, COLUMN_RIDE_SKIN_REC_STATE, SKIN_USE_STATE_UNUSE);		
	}

	// ������Ƥ��Ϊʹ��״̬
	pSkinRec->SetInt(exist_row, COLUMN_RIDE_SKIN_REC_STATE, SKIN_USE_STATE_INUSE);
	pSkinRec->SetInt(exist_row, COLUMN_RIDE_SKIN_REC_FLAG, 1);

	// ��������Ƥ����Ϣ
	PushRideSkinMsg(pKernel, self, ride_index);

	// ����û�Ƥ��
	/*RideSkinLog log;
	log.config_id = pRideObj->GetConfig();
	log.opt_type = LOG_RIDE_CHANGE_SKIN;
	log.cur_step = pRideObj->QueryInt(FIELD_PROP_RIDE_STEP);
	log.cur_star = pRideObj->QueryInt(FIELD_PROP_RIDE_STAR);
	log.skin_id = new_skin_id;
	LogModule::m_pLogModule->OnRideChangeSkin(pKernel, self, log);*/


	// ��������
	if (GetRidingState(pKernel, self) == RIDE_UP_DOWN_OPERATE_UP)
	{
		DownRide(pKernel, self);
		OnRide(pKernel, self);
	}

	//// �Ƴ�ԭƤ��������
	//const RideSkin *old_ride_skin = GetRideSkin(old_skin_id);
	//if (NULL != old_ride_skin)
	//{
	//	CVarList rm_msg;
	//	rm_msg << GetRideUpPckType(pKernel, self, ride)
	//			<< EREFRESH_DATA_TYPE_REMOVE;

	//	// �Ƴ�Ƥ��ʹ��ʱ��������
	//	std::string old_inuse_pck_id = StringUtil::IntAsString(old_ride_skin->att_add_pck_id);
	//	UpdateRideAddPkg(pKernel, self, ride, old_inuse_pck_id.c_str(), rm_msg);
	//}

	// ������Ƥ��������
	//const RideSkin *new_ride_skin = GetRideSkin(new_skin_id);
	//if (NULL != new_ride_skin)
	//{
	//	CVarList add_msg;
	//	add_msg << GetRideUpPckType(pKernel, self, ride)
	//			<< EREFRESH_DATA_TYPE_ADD;

	//	// ����Ƥ��ʹ��ʱ��������
	//	std::string new_inuse_pck_id = StringUtil::IntAsString(new_ride_skin->att_add_pck_id);
	//	UpdateRideAddPkg(pKernel, self, ride, new_inuse_pck_id.c_str(), add_msg);
	//}

	
	
	return true;
}

// ��������Ƥ����Ϣ
bool RideModule::PushRideSkinMsg(IKernel *pKernel, const PERSISTID &self, 
								 const int ride_index /*= 1*/)
{

	IGameObj *pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return false;
	}

	// ��������ȡ�������
	PERSISTID ride = GetActivedRide(pKernel, self);
	IGameObj *pRideObj = pKernel->GetGameObj(ride);
	if (NULL == pRideObj)
	{
		return false;
	}

	// ����Ƥ����
	IRecord *pSkinRec = pRideObj->GetRecord(FIELD_RECORD_RIDE_SKIN_REC);
	if (NULL == pSkinRec)
	{
		return false;
	}

	// ����Ƥ����Ϣ
	CVarList skin_msg;
	skin_msg << SERVER_CUSTOMMSG_RIDE
			 << SC_RIDE_SKIN_MSG_RES
			 << ride_index
			 << pSkinRec->GetRows();

	// ����Ƥ����
	LoopBeginCheck(k);
	for (int i = 0; i < pSkinRec->GetRows(); ++i)
	{
		LoopDoCheck(k);
		CVarList row_value;
		pSkinRec->QueryRowValue(i, row_value);
		skin_msg.Concat(row_value);
	}

	return pKernel->Custom(self, skin_msg);
}

// �����������
void RideModule::RideUpDownOperate(IKernel* pKernel, const PERSISTID& self, 
	const int operate_type)
{
	IGameObj *pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return;
	}

	if (RIDE_UP_DOWN_OPERATE_UP == operate_type)
	{
		// �����Ƿ������ͷ�
		if (SkillFlowSingleton::Instance()->IsSkillUseBusy(pKernel, self) != USESKILL_RESULT_SUCCEED)
		{
			return;
		}

		// �ɷ�����
		if (RideModule::m_pRideModule->CanRide(pKernel, self))
		{
			OnRide(pKernel, self);
		}		
	}

	// ������
	if (RIDE_UP_DOWN_OPERATE_DOWN == operate_type)
	{
		DownRide(pKernel, self);
	}
}

// ȥ����Ƥ����ʶ
void RideModule::ClearSkinFlag(IKernel* pKernel, const PERSISTID& self, 
	const int skin_id)
{
	IGameObj *pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return;
	}

	PERSISTID ride = GetActivedRide(pKernel, self);
	IGameObj *pRideObj = pKernel->GetGameObj(ride);
	if (NULL == pRideObj)
	{
		return;
	}

	// ����Ƥ����
	IRecord *pSkinRec = pRideObj->GetRecord(FIELD_RECORD_RIDE_SKIN_REC);
	if (NULL == pSkinRec)
	{
		return;
	}

	// �����Ƥ����ʶ
	const int exist_row = pSkinRec->FindInt(COLUMN_RIDE_SKIN_REC_ID, skin_id);
	if (exist_row < 0)
	{
		return;
	}

	if (pSkinRec->QueryInt(exist_row, COLUMN_RIDE_SKIN_REC_FLAG) == 0)
	{
		pSkinRec->SetInt(exist_row, COLUMN_RIDE_SKIN_REC_FLAG, 1);
		PushRideSkinMsg(pKernel, self, pKernel->GetIndex(ride));
	}
}

// ��������Ƥ��
bool RideModule::UnlockSkin(IKernel *pKernel, const PERSISTID &self, 
	const int skin_id, const int ride_index /*= 1*/)
{
	if (!pKernel->Exists(self))
	{
		return false;
	}

	// �ҵ����������
	PERSISTID ride = GetActivedRide(pKernel, self);
	IGameObj *pRideObj = pKernel->GetGameObj(ride);
	if (NULL == pRideObj)
	{
		return false;
	}

	// �ѽ���
	if (IsUnlock(pKernel, self, skin_id))
	{
		CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_19401, CVarList());
		return false;
	}

	// �����Ƿ�Ϊ��
	PERSISTID item_box = pKernel->GetChild(self, ITEM_BOX_NAME_WSTR);
	if (!pKernel->Exists(item_box))
	{
		return false;
	}

	// Ƥ����������
	const SkinUnlockCfg *cfg = GetSkinUnlockCfg(skin_id);
	if (NULL == cfg)
	{
		return false;
	}

	// ǰ���Ƿ����
	if (!IsUnlockPreSkin(pKernel, self, cfg->pre_skin))
	{
		CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_19402, CVarList());
		return false;
	}

	// ��Ʒ�Ƿ��㹻
	if (!CommRuleModule::CanDecItems(pKernel, self, cfg->unlock_items))
	{
		return false;
	}

	// �����Ƿ��㹻
	if (!CommRuleModule::CanDecCapitals(pKernel, self, cfg->unlock_fee))
	{
		return false;
	}

	// �ۻ���
	if (!CommRuleModule::ConsumeCapitals(pKernel, self, cfg->unlock_fee, FUNCTION_EVENT_ID_RIDE_SKIN_UNLOCK))
	{
		return false;
	}

	if (!CommRuleModule::ConsumeItems(pKernel, self, cfg->unlock_items, FUNCTION_EVENT_ID_RIDE_SKIN_UNLOCK))
	{
		return false;
	}

	// ���Ƥ��
	return AddRideSkin(pKernel, self, skin_id);
}

// ��������
void RideModule::UpgradeLevel(IKernel *pKernel, const PERSISTID &self)
{
	// �ж�
	IGameObj* pSelf = pKernel->GetGameObj(self);
	if (NULL == pSelf)
	{
		return;
	}

	// �ҵ����������
	PERSISTID ride = GetActivedRide(pKernel, self);
	IGameObj *pRide = pKernel->GetGameObj(ride);
	if (NULL == pRide)
	{
		return;
	}

	// ��ǰ����Ľ׼����Ǽ�
	const int cur_step = pRide->QueryInt(FIELD_PROP_RIDE_STEP);
	const int cur_star = pRide->QueryInt(FIELD_PROP_RIDE_STAR);

	// ȡ�õ�ǰ�׼�����
	const RideStepCfg *pStepCfg = GetRideStepCfg(cur_step);
	if (NULL == pStepCfg)
	{
		return;
	}

	// �Ǽ����������
	if (cur_star >= pStepCfg->need_star)
	{
		return;
	}

	// ȡ������������Ʒ
	int idx = cur_star - 1;
	if (idx < 0 || idx >= (int)pStepCfg->upgrade_items_list.size())
	{
		return;
	}

	const Consume_Vec& consume_vec = pStepCfg->upgrade_items_list[idx];

	// ��Ʒ�Ƿ��㹻
	if (!CommRuleModule::CanDecItems(pKernel, self, consume_vec))
	{
		return;
	}

	if (!CommRuleModule::ConsumeItems(pKernel, self, consume_vec, FUNCTION_EVENT_ID_RIDE_TRAIN))
	{
		return;
	}

	// �����ɹ�
	pRide->SetInt(FIELD_PROP_RIDE_STAR, cur_star + 1);

	// ����������id
	SetRideAddPckID(pKernel, self, ride);

	// ����������
	Upgrade2UpdateAddPkg(pKernel, self, ride);
}

// ��������
void RideModule::UpgradeStep(IKernel *pKernel, const PERSISTID &self)
{
	// �ж�
	IGameObj* pSelf = pKernel->GetGameObj(self);
	if (NULL == pSelf)
	{
		return;
	}

	// �ҵ����������
	PERSISTID ride = GetActivedRide(pKernel, self);
	IGameObj *pRide = pKernel->GetGameObj(ride);
	if (NULL == pRide)
	{
		return;
	}

	// ��ǰ����Ľ׼����Ǽ�
	const int cur_step = pRide->QueryInt(FIELD_PROP_RIDE_STEP);
	const int cur_star = pRide->QueryInt(FIELD_PROP_RIDE_STAR);

	// ȡ�õ�ǰ�׼�����
	const RideStepCfg *pStepCfg = GetRideStepCfg(cur_step);
	if (NULL == pStepCfg)
	{
		return;
	}

	// �Ǽ����������
	if (cur_star < pStepCfg->need_star)
	{
		return;
	}

	// ȡ������������Ʒ
	int idx = cur_star - 1;
	if (idx < 0 || idx >= (int)pStepCfg->upgrade_items_list.size())
	{
		return;
	}

	const Consume_Vec& consume_vec = pStepCfg->upgrade_items_list[idx];

	// ��Ʒ�Ƿ��㹻
	if (!CommRuleModule::CanDecItems(pKernel, self, consume_vec))
	{
		return;
	}

	if (!CommRuleModule::m_pThis->ConsumeItems(pKernel, self, consume_vec, FUNCTION_EVENT_ID_RIDE_TRAIN))
	{
		return;
	}

	// ���׳ɹ�
	pRide->SetInt(FIELD_PROP_RIDE_STEP, cur_step+ 1);
	pRide->SetInt(FIELD_PROP_RIDE_STAR, 1);

	// ����������id
	SetRideAddPckID(pKernel, self, ride);

	// ����������
	Upgrade2UpdateAddPkg(pKernel, self, ride);
}

// �������
int RideModule::PlayerOnline(IKernel* pKernel, const PERSISTID& self)
{
	// �ж�
	IGameObj* pSelf = pKernel->GetGameObj(self);
	if (NULL == pSelf)
	{
		return 0;
	}

	REGIST_RESET_TIMER(pKernel, self, RESET_RIDE);

	// �ָ����ﱳ��
	PERSISTID box = pKernel->GetChild(self, L"RideBox");
	if (!pKernel->Exists(box))
	{
		box = pKernel->CreateContainer(self, CLASS_NAME_RIDE_BOX, MAX_RIDE_NUMBER);
		if (!pKernel->Exists(box))
		{
			extend_warning(LOG_ERROR, "[Error]create RideBox error!");
			return 0;
		}
	}

	// �Ѽ�������
	PERSISTID active_ride = GetActivedRide(pKernel, self);
	IGameObj *pActiveRide = pKernel->GetGameObj(active_ride);
	if (pActiveRide == NULL)
	{
		return 0;
	}

	// ����ˢ������������ID��������ڸ�������������
	// ��ǰ�ȼ��ͽ׼�
	const int ride_star = pActiveRide->QueryInt(FIELD_PROP_RIDE_STAR);
	const int ride_step = pActiveRide->QueryInt(FIELD_PROP_RIDE_STEP);

	const char* base_pkg_str = pKernel->GetConfigProperty(pActiveRide->GetConfig(), FIELD_PROP_BASE_ADD_PKG);
	if (StringUtil::CharIsNull(base_pkg_str))
	{
		return 0;
	}

	// ���»���������
	const int bask_pkg = StringUtil::StringAsInt(base_pkg_str);
	pActiveRide->SetInt(FIELD_PROP_BASE_ADD_PKG, bask_pkg);

	// ����������
	const int ride_add_pkg = bask_pkg + ride_step*RIDE_BASE_PACKAGE_COEFFICIENT + ride_star;
	pActiveRide->SetInt(FIELD_PROP_RIDE_ADD_PKG, ride_add_pkg);

	// ��������������
	CVarList up_msg;
	up_msg << GetRideUpPckType(pKernel, self, active_ride)
		   << EREFRESH_DATA_TYPE_ADD;
	std::string pkg_id = StringUtil::IntAsString(pActiveRide->QueryInt(FIELD_PROP_RIDE_ADD_PKG));
	UpdateRideAddPkg(pKernel, self, active_ride, pkg_id.c_str(), up_msg);

	// ��������Ƥ�������
	UpdateUseSkinPkg(pKernel, self, active_ride, up_msg);

	return 0;
}

// ����뿪��Ӫս
int RideModule::OnPlayerLeave(IKernel *pKernel, const PERSISTID &self,
	const PERSISTID &sender, const IVarList &args)
{
	if (!pKernel->Exists(self))
	{
		return 0;
	}

	//m_pRideModule->DownRide(pKernel, self);

	return 0;
}

// ����������ö�ʱ��
int RideModule::MountResetTimer(IKernel *pKernel, const PERSISTID &self, 
	int slice)
{
	IGameObj *pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return 0;
	}

	return 0;
}

// ���ܿ���
int RideModule::OnCommandActiveFunc(IKernel *pKernel, const PERSISTID &self, 
	const PERSISTID &sender, const IVarList &args)
{
	if (!pKernel->Exists(self))
	{
		return 0;
	}

	const int func_type = args.IntVal(1);
	if (func_type != AFM_RIDE)
	{
		return 0;
	}

	// ������
	const char *ride_id = EnvirValueModule::EnvirQueryString(ENV_VALUE_FIRST_RIDE);
	if (StringUtil::CharIsNull(ride_id))
	{
		return 0;
	}

	m_pRideModule->AddRide(pKernel, self, ride_id);

	return 0;
}
