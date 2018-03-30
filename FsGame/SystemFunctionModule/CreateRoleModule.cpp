//--------------------------------------------------------------------
// �ļ���:		CreateRoleModule.cpp
//--------------------------------------------------------------------

#include "CreateRoleModule.h"
#include "utils/util_func.h"
#include "utils/XmlFile.h"
#include "utils/string_util.h"
#include "FsGame/Define/GameDefine.h"
//#include "FsGame/Define/NationDefine.h"
#include "FsGame/Define/CommandDefine.h"
#include "FsGame/Define/FightPropertyDefine.h"
//#include "FsGame/Define/EquipDefine.h"
#include "FsGame/Define/LogDefine.h"
#include "FsGame/Define/StaticDataDefine.h"
#include "FsGame/Define/ViewDefine.h"
#include "FsGame/Define/ContainerDefine.h"
#include "FsGame/SkillModule/SkillModule.h"
#include "FsGame/CommonModule/ContainerModule.h"
#include "FsGame/SystemFunctionModule/ToolBoxModule.h"
#include "FsGame/CommonModule/LevelModule.h"
#include "FsGame/CommonModule/LogModule.h"
#include "FsGame/SystemFunctionModule/MotionModule.h"
#include "FsGame/SystemFunctionModule/StaticDataQueryModule.h"
//#include "FsGame/ItemModule/EquipmentModule.h"
#include "FsGame/ItemModule/ItemBaseModule.h"
//#include "FsGame/SocialSystemModule/RideModule.h"
//#include "FsGame/Define/RideDefine.h"
#include "FsGame/SystemFunctionModule/CapitalModule.h"
#include "FsGame/SystemFunctionModule/PlayerBaseModule.h"
#include "utils/string_util.h"
#include <time.h>
#include "FsGame/CommonModule/LuaExtModule.h"
#include "FsGame/Define/PlayerBaseDefine.h"
// #include "../SocialSystemModule/OffLineModule.h"
// #include "../Define/OffLineDefine.h"
//#include "FsGame/SceneBaseModule/SecretSceneModule.h"
#include "FsGame/Define/ServerCustomDefine.h"
#include "utils/custom_func.h"
#include "FsGame/Define/ClientCustomDefine.h"
#include "server/KnlConst.h"
//#include "FsGame/SocialSystemModule/PetConfigManage.h"

#include <sstream>
#include "../CommonModule/EnvirValueModule.h"

//#include "SceneBaseModule/GroupScene/GroupSceneBase.h"
#include "../CommonModule/LuaScriptModule.h"
#include "RewardModule.h"
#include "EquipmentModule/EquipDefine.h"


#define SKILL_CONTAINER_CAPACITY     512//������������
#define BUFF_CONTAINER_CAPACITY      128//BUFF��������
const time_t ESCAPE_GAP_TIME = 600;    //���뿨���ļ��ʱ��10����

#define PLAYER_RESOURCE    "ini/SystemFunction/Createrole/ResourceConfig.xml"
#define PLAYER_CREATE	   "ini/SystemFunction/Createrole/CreateRoleInfo.xml"

#define ARENA_ROBOT_EQUIP_FILE "ini/SystemFunction/Createrole/ArenaRobotEquip.xml"


MotionModule *		CreateRoleModule::m_pMotionModule = NULL;
CreateRoleModule *	CreateRoleModule::m_pCreateRoleModule = NULL;
ContainerModule *	CreateRoleModule::m_pContainerModule = NULL;
LevelModule *		CreateRoleModule::m_pLevelModule = NULL;
//NationModule *		CreateRoleModule::m_pNationModule = NULL;
//TreasureModule*		CreateRoleModule::m_pTreasureModule = NULL;
StaticDataQueryModule *	CreateRoleModule::m_pStaticDataQueryModule = NULL;
EquipmentModule *	CreateRoleModule::m_pEquipmentModule = NULL;
ItemBaseModule *	CreateRoleModule::m_pItemBaseModule = NULL;
//RideModule *		CreateRoleModule::m_pRideModule	= NULL;
CapitalModule *		CreateRoleModule::m_pCapitalModule = NULL;
//SecretSceneModule * CreateRoleModule::m_pSecretSceneModule = NULL;

// �ؼ��ش�����ɫ�����ļ�
int CreateRoleModule::nx_reload_create_role_config(void* state)
{
	// ���ߺ���ָ��
	IKernel* pKernel = LuaExtModule::GetKernel(state);


	if (!m_pCreateRoleModule->LoadSaveRoleInfo(pKernel))
	{
		return 0;
	}

	return 1;
}

// ��ʼ��
bool CreateRoleModule::Init(IKernel* pKernel)
{
	m_pCreateRoleModule = this;
    m_pMotionModule		= (MotionModule*)pKernel->GetLogicModule("MotionModule");
    //m_pEquipmentModule	= (EquipmentModule *)pKernel->GetLogicModule("EquipmentModule");
    m_pContainerModule	= (ContainerModule*)pKernel->GetLogicModule("ContainerModule");
	m_pLevelModule		= (LevelModule*)pKernel->GetLogicModule("LevelModule");
	m_pStaticDataQueryModule	= (StaticDataQueryModule*)pKernel->GetLogicModule("StaticDataQueryModule");
	m_pItemBaseModule   = (ItemBaseModule*)pKernel->GetLogicModule("ItemBaseModule");
	m_pCapitalModule	= (CapitalModule*)pKernel->GetLogicModule("CapitalModule");

	Assert(m_pCreateRoleModule != NULL && 
		m_pMotionModule != NULL && 
		m_pContainerModule != NULL && 
		m_pLevelModule != NULL && 
		m_pStaticDataQueryModule != NULL && 
/*		m_pEquipmentModule != NULL &&*/
		m_pItemBaseModule != NULL &&
		m_pCapitalModule != NULL );

	// ��ӻص�
	pKernel->AddLogicClass("role", TYPE_PLAYER, "player");

	pKernel->AddEventCallback("role", "OnCreateRole", OnCreateRole);
	// �༭����������ݻص�
	pKernel->AddEventCallback("player", "OnRecover", CreateRoleModule::OnFirstRecover, INT_MIN);
	pKernel->AddEventCallback("player", "OnStore", CreateRoleModule::OnStore);

	pKernel->AddIntCommandHook("player", COMMAND_SAVE_ROLE_INFO, OnSaveRoleInfo);
    pKernel->AddIntCustomHook("player", CLIENT_CUSTOMMSG_ESCAPE_LOCK, CreateRoleModule::EscapeFromLock);

//	pKernel->AddEventCallback(CLASS_NAME_TEMPORARY_BAG_BOX, "OnCreate", OnTemporayBoxCreate);

	LoadSaveRoleInfo(pKernel);

    #if defined(CREATE_ROBOT)
        LoadArenaRobotInfo(pKernel);
    #endif

	DECL_LUA_EXT(nx_reload_create_role_config);
	return true;
}

// �ͷ�
bool CreateRoleModule::Shut(IKernel* pKernel)
{
	m_vSaveRoleProps.clear();
	return true;
}

// �������Ļ���������
bool CreateRoleModule::LoadArenaRobotInfo(IKernel* pKernel)
{
    // �ļ���ʽ��
    //  <ID="", Job="", Level="", Equipment="", StrLevel="", Jewel="", Soul="" />
    //
    std::string file_path = pKernel->GetResourcePath();
    file_path.append(ARENA_ROBOT_EQUIP_FILE);
    
    CXmlFile xml(file_path.c_str());
    if (!xml.LoadFromFile())
    {
        std::string err_msg = file_path;
        err_msg.append(" does not exists.");
        ::extend_warning(LOG_ERROR, err_msg.c_str());
        return false;
    }
    
    CVarList sec_list;
    xml.GetSectionList(sec_list);

    int count = (int) sec_list.GetCount();
	LoopBeginCheck(h)
    for (int i = 0; i < count; i++)
    {
		LoopDoCheck(h)
        const char *section = sec_list.StringVal(i);
        
        ArenaRobotEquipment equip;
        equip.role_job = xml.ReadInteger(section, "Job", 0);
        equip.role_level = xml.ReadInteger(section, "Level", 0);
        equip.equip = xml.ReadString(section, "Equipment", "");
        equip.equip_strlevel = xml.ReadString(section, "StrLevel", "");
        equip.equip_jewel = xml.ReadString(section, "Jewel", "");
        equip.equip_soul = xml.ReadString(section, "Soul", "");
        
        if (equip.role_job ==0 || equip.role_level ==0)
        {
            continue;
        }
        
        std::stringstream ss;
        ss << equip.role_job << "_" << equip.role_level;
        
        ArenaEquipMap::iterator it_find = m_ArenaRobotEquipment.find(ss.str());
        if (it_find != m_ArenaRobotEquipment.end())
        {
            it_find->second.push_back(equip);
        }
        else
        {
            std::vector<ArenaRobotEquipment> temp_vec;
            temp_vec.push_back(equip);
            
            m_ArenaRobotEquipment.insert(std::make_pair(ss.str(), temp_vec));
        }
    }
    
    return true;
}

// ������ɫ
int CreateRoleModule::OnCreateRole(IKernel* pKernel, const PERSISTID& self,
								   const PERSISTID& sender, const IVarList& args)
{
	// ��Ҷ���
	IGameObj *pPlayer = pKernel->GetGameObj(self);
	if (NULL == pPlayer)
	{
		return 0;
	}

	// �Ա� 1�� 2Ů
	int job = args.IntVal(0);
	int sex = args.IntVal(1);
	int level = 1;
	
#if defined (CREATE_ROBOT)

    level = args.IntVal(2);
	int nNation = args.IntVal(3);

#endif

	// ����Ա��Ƿ�Ϸ�
	if (!IsValidSex(sex))
	{
		// ���Ϸ�����������
		sex = SEX_MAN;
	}

	// ���ְҵ�Ƿ�Ϸ�
	if (!IsValidJob(job))
	{
		// ���Ϸ��������һ��ְҵ
		job = PLAYER_JOB_MIN + 1;
	}

	//ְҵ
	pPlayer->SetInt("Job",job);
	//�Ա�
	pPlayer->SetInt("Sex",sex);

	pPlayer->SetInt("Level", level);
	
#if defined (CREATE_ROBOT)
    pPlayer->SetInt("PrepareRole", ROLE_FLAG_ROBOT);
	pPlayer->SetInt(FIELD_PROP_NATION, nNation);
#else
    pPlayer->SetInt("PrepareRole", ROLE_FLAG_NORMAL_PLAYER);

#endif
    
	//���ó鱦����
//     pPlayer->SetInt("CopperFirst", 1);
//     pPlayer->SetInt("SliverFirst", 1);
	pPlayer->SetInt64(FIELD_PROP_ROLE_CREATE_TIME, ::time(NULL));
	int64_t playerHP = pPlayer->QueryInt64("MaxHP");
	pPlayer->SetInt64("HP", playerHP);

	//����ռ�ø������Ƿ�͸ 
	pPlayer->SetInt("PathGrid", TYPE_PLAYER_PATH_GRID);

	//������
	const char * point = EnvirValueModule::EnvirQueryString(ENV_VALUE_GUIDE_SCENE_BRON_POS);
	pKernel->SetLandPoint(self,point);
	len_t self_x,self_y,self_z,self_orient;
	int sceneID;
	pKernel->GetSwitchLocation(point,sceneID,self_x,self_y,self_z,self_orient);
	pKernel->SetLandPosi(self, sceneID, self_x, self_y, self_z, self_orient);
	
	// ���õ�ǰ�����°汾��
	if (PLAYER_VERSION_MAX > 1)
	{
		pKernel->SetInt(self, "PlayerVersion", PLAYER_VERSION_MAX - 1);
	}

	// ����0��ʾ�����ɹ�
	return 0;
}

// һ��Ҫ��֤��һ�ε���
int CreateRoleModule::OnFirstRecover(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args)
{
	IGameObj* pSelf = pKernel->GetGameObj(self);
	if (NULL == pSelf)
	{
		return 0;
	}

	pSelf->SetInt(FIELD_PROP_GROUP_ID, -1);

	// �ж�״̬
	int nState = pSelf->QueryInt(FIELD_PROP_BORN_STATE);
	if (nState == EM_PLAYER_STATE_BRON)
	{
		LogModule::m_pLogModule->OnCreateRole(pKernel, self);

		// ��ʼ������
		//��ʼ����Ϣ ��Ҵ���ʱ�Ļ�����Ϣȫ���ڳ�ʼ���д���
		m_pCreateRoleModule->RoleInit(pKernel, self);
		//������Ϣ
		m_pCreateRoleModule->OnSaveRoleInfo(pKernel, self, sender, args);

		// ��ҳ���ʱ����Ʒ
		if (NULL != RewardModule::m_pRewardInstance)
		{
			int nAwardId = EnvirValueModule::EnvirQueryInt(ENV_VALUE_PLAYER_CREATE_REWARD);
			RewardModule::m_pRewardInstance->RewardPlayerById(pKernel, self, nAwardId);
		}

		// ���õ�ǰ�����°汾��
		if (PLAYER_VERSION_MAX > 1)
		{
			pSelf->SetInt("PlayerVersion", PLAYER_VERSION_MAX - 1);
		}

		// ��������
		pSelf->SetInt(FIELD_PROP_BORN_STATE, EM_PLAYER_STATE_INIT);

		pKernel->SavePlayerData(self);

		// �մ���������Ϸ
		pKernel->Command(self, self, CVarList() << COMMAND_MSG_FIRST_ENTER);
	}

	// ������Ұ汾���߼�
	PlayerBaseModule::HandlePlayerVersion(pKernel, self, CVarList());

	// patch
	if (LuaScriptModule::m_pLuaScriptModule)
	{
		CVarList results;

		CVarList luaargs;
		luaargs << self;
		LuaScriptModule::m_pLuaScriptModule->RunLuaScript(pKernel, "patch.lua", "main", luaargs, 1, &results);
		const char* pszResult_ = results.StringVal(0);
		if ( strcmp(pszResult_, "ok") != 0 )
		{
			extend_warning(LOG_ERROR, "[CreateRoleModule::OnFirstRecover] patch data error(role id:%s)", pKernel->QueryString(self, FIELD_PROP_UID));
		}
	}
	return 0;
}

// ��ɫ����
int CreateRoleModule::OnStore(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args)
{
	int reason = args.IntVal(0);
	if (reason == STORE_EXIT)
	{
		//������Ϣ
		pKernel->Command(self, self, CVarList() << COMMAND_SAVE_ROLE_INFO);
	}

	return 0;
}


//���������Ļ����˴���װ��
// int CreateRoleModule::SetRobotEquip(IKernel* pKernel,const PERSISTID& self)
// {
// 	// ��Ҷ���
// 	IGameObj *pPlayer = pKernel->GetGameObj(self);
// 	if (NULL == pPlayer)
// 	{
// 		return 0;
// 	}
// 
//     int role_job = pPlayer->QueryInt("Job");
//     int role_level = pPlayer->QueryInt("Level");
//     
//     // װ����
//     PERSISTID equip_box = pKernel->GetChild(self, L"EquipBox");
//     if (!pKernel->Exists(equip_box))
//     {
//         return 0;
//     }
//     
//     // ǿ����
//     IRecord * strengthen_record = pPlayer->GetRecord(EQUIP_STRHENTHEN_REC);
//     if (NULL == strengthen_record)
//     {
//         return 0;
//     }
//     
//     // ���ҵ����ʵ�װ��
//     const ArenaRobotEquipment *robot_equip = NULL;
// 	LoopBeginCheck(m)
//     for (int i=role_level; i>0; --i)
//     {
// 		LoopDoCheck(m)
//         std::stringstream ss;
//         ss << role_job << "_" << i;
//         std::string key = ss.str();
//         
//         ArenaEquipMap::iterator it_find = m_ArenaRobotEquipment.find(key);
//         if (it_find == m_ArenaRobotEquipment.end())
//         {
//             continue;
//         }
//         
//         // �ҵ������õ�װ����Ϣ
//         int config_count = (int)it_find->second.size();
//         
//         if (config_count == 0)
//         {
//             // ľ��װ��������Ϣ
//             return 0;
//         }
//         
//         if (config_count > 1)
//         {
//             // ȡ��һ�������, ����ѡ��һ������
//             int idx = util_random_int(config_count);
//             robot_equip = &((it_find->second)[idx]);
//         }
//         else
//         {
//             robot_equip = &(it_find->second.front());
//         }
//         
//         break;
//     }
//     
//     if (robot_equip == NULL)
//     {
//         // �˵ȼ���ְҵľ��װ��
//         return 0;
//     }
//     
//     // ����װ���б�, ����װ��ֱ���÷ֺŷָ�
//     CVarList equip_list;
//     util_split_string(equip_list, robot_equip->equip, ";");
//     int equip_config_count = (int)equip_list.GetCount();
// 	LoopBeginCheck(n)
//     for (int i=0; i<equip_config_count; ++i)
//     {
// 		LoopDoCheck(n)
//         std::string one_config = equip_list.StringVal(i);
//         
//         // @one_equip �ĸ�ʽ��װ��λ��,װ��ID,װ��Ʒ��
//     
//         CVarList one_config_detail;
//         util_split_string(one_config_detail, one_config, ",");
//         if (one_config_detail.GetCount() != 3)
//         {
//             continue;
//         }
//         
//         CreateEquip(pKernel, self, equip_box, one_config_detail);
//     }
//     
//     // ǿ���ȼ�
//     CVarList str_level_list;
//     util_split_string(str_level_list, robot_equip->equip_strlevel, ";");
//     int str_level_config_count = (int)str_level_list.GetCount();
// 	LoopBeginCheck(o)
//     for (int i=0; i<str_level_config_count; ++i)
//     {
// 		LoopDoCheck(o)
//         std::string one_config = str_level_list.StringVal(i);
// 
//         // @one_level �ĸ�ʽ��װ��λ��,�ȼ�
// 
//         CVarList one_config_detail;
//         util_split_string(one_config_detail, one_config, ",");
//         if (one_config_detail.GetCount() != 2)
//         {
//             continue;
//         }
// 
//         int pos = atoi(one_config_detail.StringVal(0));
//         int level = atoi(one_config_detail.StringVal(1));
//         
//         // �ҵ�װ��
//         PERSISTID the_equip = pKernel->GetItem(equip_box, pos);
//         if (!pKernel->Exists(the_equip))
//         {
//             continue;
//         }
//         
//         // �ҵ�ǿ�����е�λ��
//         int row = strengthen_record->FindInt(EQUIP_STRHENTHEN_INDEX, pos);
//         if (row == -1)
//         {
//             continue;
//         }
//         
//         strengthen_record->SetInt(row, EQUIP_STRHENTHEN_LEVEL, level);
//         
//         // ����ǿ��֪ͨ
//         pKernel->Command(the_equip, self, CVarList() << COMMAND_EQUIP_STRLEVELUP << 0 << level);
//     }    
//     
//     // ��ʯ
//     CVarList str_jewel_list;
//     util_split_string(str_jewel_list, robot_equip->equip_jewel, ";");
//     int jewel_config_count = (int)str_jewel_list.GetCount();
// 	LoopBeginCheck(p)
//     for (int i=0; i<jewel_config_count; ++i)
//     {
// 		LoopDoCheck(p)
//         std::string one_config = str_jewel_list.StringVal(i);
// 
//         // @one_level �ĸ�ʽ��װ��λ��,��ʯ1,��ʯ2,��ʯ3,��ʯ4
// 
//         CVarList one_config_detail;
//         util_split_string(one_config_detail, one_config, ",");
//         if (one_config_detail.GetCount() != 5)
//         {
//             continue;
//         }
// 
//         int pos = atoi(one_config_detail.StringVal(0));
//         const char *jewel1 = one_config_detail.StringVal(1);
//         const char *jewel2 = one_config_detail.StringVal(2);
//         const char *jewel3 = one_config_detail.StringVal(3);
//         const char *jewel4 = one_config_detail.StringVal(4);
// 
//         // �ҵ�ǿ�����е�λ��
//         int row = strengthen_record->FindInt(EQUIP_STRHENTHEN_INDEX, pos);
//         if (row == -1)
//         {
//             continue;
//         }
// 
//         strengthen_record->SetString(row, EQUIP_STRHENTHEN_JEWEL1, jewel1);
//         strengthen_record->SetString(row, EQUIP_STRHENTHEN_JEWEL2, jewel2);
//         strengthen_record->SetString(row, EQUIP_STRHENTHEN_JEWEL3, jewel3);
//         strengthen_record->SetString(row, EQUIP_STRHENTHEN_JEWEL4, jewel4);
//         
//         // ������ʯ��Ƕ֪ͨ
//         pKernel->Command(self, self, CVarList() << COMMAND_EQUIPMENT_JEWEL << pos << EQUIP_STRPROP_OPTION_ADD << jewel1);
//         pKernel->Command(self, self, CVarList() << COMMAND_EQUIPMENT_JEWEL << pos << EQUIP_STRPROP_OPTION_ADD << jewel2);
//         pKernel->Command(self, self, CVarList() << COMMAND_EQUIPMENT_JEWEL << pos << EQUIP_STRPROP_OPTION_ADD << jewel3);
//         pKernel->Command(self, self, CVarList() << COMMAND_EQUIPMENT_JEWEL << pos << EQUIP_STRPROP_OPTION_ADD << jewel4);
//     }
//     
//     // ���
//     CVarList str_soul_list;
//     util_split_string(str_soul_list, robot_equip->equip_soul, ";");
//     int soul_config_count = (int)str_soul_list.GetCount();
// 	LoopBeginCheck(r)
//     for (int i=0; i<soul_config_count; ++i)
//     {
// 		LoopBeginCheck(r)
//         std::string one_config = str_soul_list.StringVal(i);
// 
//         // @one_level �ĸ�ʽ��װ��λ��,���
// 
//         CVarList one_config_detail;
//         util_split_string(one_config_detail, one_config, ",");
//         if (one_config_detail.GetCount() != 2)
//         {
//             continue;
//         }
// 
//         int pos = atoi(one_config_detail.StringVal(0));
//         const char *soul = one_config_detail.StringVal(1);
// 
//         // �ҵ�ǿ�����е�λ��
//         int row = strengthen_record->FindInt(EQUIP_STRHENTHEN_INDEX, pos);
//         if (row == -1)
//         {
//             continue;
//         }
// 
//         strengthen_record->SetString(row, EQUIP_STRHENTHEN_SOUL, soul);
//         
//         // ��������֪ͨ
//         CVarList add_soul_args;
//         add_soul_args << COMMAND_EQUIPMENT_SOUL << pos << EQUIP_STRPROP_OPTION_ADD << soul;
//         pKernel->Command(self, self, add_soul_args );
//     }
// 
//     return 0;
// }


int CreateRoleModule::RoleInit(IKernel* pKernel,const PERSISTID& self)
{
	_initRoleInfoContainers(pKernel, self); //��ʼ������
	_initRoleStrRecord(pKernel, self);			//��ʼ����
	return 0;
}

void CreateRoleModule::_initRoleInfoContainers(IKernel* pKernel, const PERSISTID& self)
{
	// buff����
	_initRoleInfoContainer(pKernel, self, wszBufferContainerName, "BufferContainer", BUFF_CONTAINER_CAPACITY);

	// ��������
	_initRoleInfoContainer(pKernel, self, SKILL_CONTAINER_NAME, "SkillContainer", SKILL_CONTAINER_CAPACITY);

	// װ������
	_initRoleInfoContainer(pKernel, self, EQUIP_BOX_NAME_WSTR, EQUIP_BOX_NAME, EQUIP_POS_STR_MAX);

    //�����౳��
    int itemCapacity = ToolBoxModule::GetItemBoxCapacity();
    _initRoleInfoContainer(pKernel, self, ITEM_BOX_NAME_WSTR, ITEM_BOX_NAME, itemCapacity);

    //ʵ���౳��
//     int powerCapacity = ToolBoxModule::GetPowerBoxCapacity();
//     _initRoleInfoContainer(pKernel, self, POWER_BOX_NAME_WSTR, POWER_BOX_NAME, powerCapacity);
// 
//     //���ı���
//     int badgeCapacity = ToolBoxModule::GetBadgeBoxCapacity();
//     _initRoleInfoContainer(pKernel, self, BADGE_BOX_NAME_WSTR, BADGE_BOX_NAME, badgeCapacity);
// 
//     //��Ƭ����
//     int chipCapacity = ToolBoxModule::GetChipBoxCapacity();
//     _initRoleInfoContainer(pKernel, self, CHIP_BOX_NAME_WSTR, CHIP_BOX_NAME, chipCapacity);

	//�һ�����
	//_initRoleInfoContainer(pKernel, self, OFFLINE_BOX, "OffLineBox", 0);

	//������
	//_initRoleInfoContainer(pKernel, self, L"RideBox", "RideBox", MAX_RIDE_NUMBER);

	// ������
	//_initRoleInfoContainer(pKernel, self, L"PetBox", "PetBox", PetConfigManage::GetMaxPetNum());

	//��ʱ���� 
// 	int temporay_cap = ToolBoxModule::GetTemporaryBoxCapacity();
// 	_initRoleInfoContainer(pKernel, self, TEMPORARY_BAG_BOX_NAME_WSTR, CLASS_NAME_TEMPORARY_BAG_BOX, temporay_cap);

	// ӡ������
	//_initRoleInfoContainer(pKernel, self, MARKER_BOX_NAME_WSTR, MARKER_BOX_NAME, MARK_TYPE_END - 1);
}

bool CreateRoleModule::_initRoleInfoContainer(IKernel* pKernel, const PERSISTID& self, const wchar_t* name,
											  const char* script, int capacity, bool needReset)
{
	PERSISTID container = pKernel->GetChild(self, name);
	if (!pKernel->Exists(container))
	{
		container = pKernel->CreateContainer(self, script, capacity);
		if (!pKernel->Exists(container))
		{
			return false;
		}
		pKernel->SetWideStr(container, "Name", name);
	}
	else
	{
		if (needReset)
		{
			unsigned int it;
			PERSISTID element = pKernel->GetFirst(container, it);
			LoopBeginCheck(v)
			while (pKernel->Exists(element))
			{
				LoopDoCheck(v)
				pKernel->Destroy(container, element);
				element = pKernel->GetNext(container, it);
			}
		}
	}
	return true;
}

// ��ʼ����ɫ������Ϣ
void CreateRoleModule::_initRoleInfoTreasure(IKernel* pKernel, const PERSISTID& self)
{
	if (NULL == pKernel || !pKernel->Exists(self))
	{
		return;
	}
	//m_pTreasureModule->InitPlayerTreasureData(pKernel, self);
}

void CreateRoleModule::_initRoleStrRecord(IKernel* pKernel, const PERSISTID& self)
{
	PERSISTID equbox = pKernel->GetChild(self, EQUIP_BOX_NAME_WSTR);
	if (!pKernel->Exists(equbox))
	{
		return;
	}

 	IRecord * pStrengthenRec = pKernel->GetRecord(self, EQUIP_STRHENTHEN_REC);
 	if (NULL == pStrengthenRec)
 	{
 		return;
 	}
 
 	pStrengthenRec->ClearRow();
 
 	LoopBeginCheck(w)
 	for (int i = 0; i < (int)pStrengthenRec->GetRowMax(); ++i)
 	{
 		LoopDoCheck(w)
 		//װ����������,ǿ���ȼ�,���,���ɸ���,��ʯx4
 		pStrengthenRec->AddRowValue(i, CVarList()<<i+1<<0<<""<<0<<""<<""<<""<<"");
 	}
}

 int CreateRoleModule::OnSaveRoleInfo(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args)
{
	m_pCreateRoleModule->SaveRoleInfo(pKernel, self);
	return 0;
}

bool CreateRoleModule::SaveRoleInfo(IKernel* pKernel, const PERSISTID& self)
{
	std::string info = "";

	LoopBeginCheck(x)
	for (int i = 0; i < (int)m_vSaveRoleProps.size(); i++)
	{
		LoopDoCheck(x)
		if (!pKernel->Find(self, m_vSaveRoleProps[i].c_str()))
		{
			info += m_vSaveRoleProps[i].c_str();
			info += ",";
			info += "";
		}
		else
		{
			int nType = pKernel->GetType(self, m_vSaveRoleProps[i].c_str());
			if (nType == VTYPE_INT)
			{
				int nValue = pKernel->QueryInt(self, m_vSaveRoleProps[i].c_str());
				info += m_vSaveRoleProps[i].c_str();
				info += ",";
				info += util_int64_as_string(nValue).c_str();
			}
			else if (nType == VTYPE_STRING)
			{
				std::string strValue = pKernel->QueryString(self, m_vSaveRoleProps[i].c_str());
				info += m_vSaveRoleProps[i].c_str();
				info += ",";
				info += strValue;
			}
			else if (nType == VTYPE_INT64)
			{
				__time64_t nValue64 = pKernel->QueryInt64(self, m_vSaveRoleProps[i].c_str());
				info += m_vSaveRoleProps[i].c_str();
				info += ",";
				info += util_int64_as_string(nValue64).c_str();
			}
			else
			{
				info += m_vSaveRoleProps[i].c_str();
				info += ",";
				info += "";
			}
		}
		info += ";";
	}

	std::wstring roleinfo = util_string_as_widestr(info.c_str());

	if (info.empty())
	{
		::extend_warning(LOG_INFO, "CreateRoleModule role_info is null");
		return false;
	}

	pKernel->SetWideStr(self, "RoleInfo", roleinfo.c_str());

	return true;
}

bool CreateRoleModule::LoadSaveRoleInfo(IKernel* pKernel)
{
	std::string path = pKernel->GetResourcePath();
	path += "ini/SystemFunction/createrole/RoleSaveProps.xml";

	CXmlFile xml(path.c_str());
	if (!xml.LoadFromFile())
	{
		std::string msg = std::string("[Error]ini/SystemFunction/createrole/rolesaveprops.xml No found file : ");
		::extend_warning(LOG_INFO, msg.c_str());
		return false;
	}

	CVarList sec_list;
	xml.GetSectionList(sec_list);
	LoopBeginCheck(y)
	for (size_t i = 0; i < sec_list.GetCount(); i++)
	{
		LoopDoCheck(y)
		std::string section = sec_list.StringVal(i);
		std::string prop = xml.ReadString(section.c_str(), "Prop", "");
		if (!prop.empty())
		{
			m_vSaveRoleProps.push_back(prop);
		}
	}

	return true;
}
//���뿨��
int CreateRoleModule::EscapeFromLock(IKernel* pKernel,const PERSISTID& self,const PERSISTID& sender,const IVarList& args)
{
	// ��Ҷ���
	IGameObj *pPlayer = pKernel->GetGameObj(self);
	if (NULL == pPlayer)
	{
		return 0;
	}

	int sceneID = pKernel->GetSceneId();
// 	GROUP_SCENE_TYPE type = (GROUP_SCENE_TYPE)GroupSceneBase::GetSceneType(pKernel, sceneID);
// 	if (type != GROUP_SCENE_NOT_SECRET)
// 	{
// 		int subType = m_pSecretSceneModule->GetSubType(SecretSceneModule::GetSecretIndex(pPlayer));
// 		if (SCENE_SUB_TYPE_SCREEN == subType)
// 		{
// 			::CustomSysInfo(pKernel, self, TIPSTYPE_SYSFUNCTION_PROMPT_MESSAGE, STR_ESCAPE_FROM_LOCK_1, CVarList());
// 
// 			return 1;
// 		}
//	}
    PERSISTID scene = pKernel->GetScene();

    if (!pPlayer->FindData("EscapeTime"))
    {
        ADD_DATA(pKernel, self, "EscapeTime", VTYPE_INT64);
    }

    time_t lastTime = pPlayer->QueryDataInt64("EscapeTime");
    time_t now = ::time(NULL);
    time_t remainTime = now - lastTime;
    //ʮ������ȴʱ���ڲ�����ʹ��
    if (remainTime < ESCAPE_GAP_TIME)
    {
		::CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_19201, CVarList() << (ESCAPE_GAP_TIME - remainTime));

        return 1;
    }

    len_t x ,y, z, orient;
    pKernel->GetSceneBorn(x, y, z, orient);
    pKernel->MoveTo(self, x, y, z, orient);
    //BattleTeamModule::m_pBattleTeamModule->AddLittleStepGridRec(pKernel, self, x, z);
    pPlayer->SetDataInt64("EscapeTime", now);
    pKernel->Custom(self, CVarList() << SERVER_CUSTOMMSG_ESCAPE_LOCK << 1);

    return 0;
}