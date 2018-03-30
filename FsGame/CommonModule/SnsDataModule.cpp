//---------------------------------------------------------
//�ļ���:       SnsDataModule.cpp
//��  ��:       ���ߵ��������
//˵  ��:       
//          
//��������:      2014��12��24��
//������:         
//�޸���:
//   :         
//---------------------------------------------------------

#include "SnsDataModule.h"

#include "utils/extend_func.h"
#include "utils/util_func.h"
#include "utils/util_ini.h"
#include "utils/util_func.h"
#include "utils/XmlFile.h"
#include "utils/custom_func.h"
#include "utils/string_util.h"
#include "public/VarList.h"

//#include "FsGame/Define/NationOfficialDefine.h"
#include "FsGame/Define/SnsDataDefine.h"
#include "FsGame/Define/ServerCustomDefine.h"
#include "FsGame/Define/ViewDefine.h"
#include "FsGame/Define/ClientCustomDefine.h"
#include "FsGame/Define/CommandDefine.h"
//#include "FsGame/Define/EquipDefine.h"
//#include "FsGame/Define/RideDefine.h"
#include "FsGame/Define/Fields.h"

//#include "FsGame/SocialSystemModule/PetModule.h"

#include "FsGame/CommonModule/LuaExtModule.h"
#include "FsGame/CommonModule/ReLoadConfigModule.h"

#define CONFIG_FILE_VAR_DEF        "ini/SystemFunction/SNS/var_def.xml"
#define CONFIG_FILE_ELEMENT_DEF    "ini/SystemFunction/SNS/element_def.xml"

#define SECTION_PLAYER_ATTRIBUTE           "PlayerAttribute"
#define SECTION_PLAYER_EQUIPMENT           "PlayerEquipment"
#define SECTION_PLAYER_SKILL               "PlayerSkill"
#define SECTION_PLAYER_RIDE                "PlayerRide"
#define SECTION_PLAYER_PET                 "PlayerPet"
#define SECTION_PLAYER_PASSIVE_SKILL       "PlayerPassiveSkill"
#define SECTION_PLAYER_EQUIPMENT_BAPTISE   "PlayerEquipmentBaptise"
#define SECTION_WING_EQUIPMENT             "WingEquipment"

SnsDataModule::VarDefMap SnsDataModule::m_VarDef;
SnsDataModule::SectionElementsMap SnsDataModule::m_SectionElements;
SnsDataModule* SnsDataModule::m_pSnsDataModule = NULL;

//����ʹ�õ�GM����
// 
int nx_player_data(void* state)
{
    IKernel* pKernel = LuaExtModule::GetKernel(state);

    // ����������
    CHECK_ARG_NUM(state, nx_player_data, 2);

    // ����������
    CHECK_ARG_OBJECT(state, nx_player_data, 1);
    CHECK_ARG_STRING(state, nx_player_data, 2);

    // ��ȡ����
    PERSISTID player = pKernel->LuaToObject(state, 1);
    const char *target_player_name = pKernel->LuaToString(state, 2);
    const wchar_t *wtarget_player_name = pKernel->LuaToWideStr(state, 2);

    std::wstring target_name = util_string_as_widestr(target_player_name);

    CVarList args;
    args << 0 << 0;
    args << target_name.c_str();

    bool ret = SnsDataModule::OnQueryPlayerData(pKernel, player, PERSISTID(), args);
    
    int code = (ret ? 1 : 0);

    return code;
}

int nx_reload_sns_data_config(void* state)
{
	// ��ȡ����ָ��
	IKernel* pKernel = LuaExtModule::GetKernel(state);
	// �ж�
	bool result = false;
	if (NULL != SnsDataModule::m_pSnsDataModule)
	{
		result = SnsDataModule::m_pSnsDataModule->LoadResource(pKernel);
	}	
	return result == true ? 1 :0;
}

bool SnsDataModule::Init(IKernel *pKernel)
{
    // �¼�
	pKernel->AddEventCallback("player", "OnStore", OnPlayerStoreEvent, 10000);

	// �ڲ���Ϣ
	pKernel->AddIntCommandHook("player", COMMAND_SNS_DATA, OnPlayerCommandMsg);

	// �ͻ�����Ϣ
    pKernel->AddIntCustomHook("player", CLIENT_CUSTOMMSG_SNS_DATA, OnPlayerCustomMsg);
    
    // ���Թ���������Ϣ
    pKernel->AddEventCallback("scene", "OnPublicMessage", OnPublicMessage);

    pKernel->AddEventCallback("player", "OnEntry", OnPlayerEntry);

	m_pSnsDataModule = this;

    LoadResource(pKernel);

    DECL_LUA_EXT(nx_player_data);
	DECL_LUA_EXT(nx_reload_sns_data_config);

	RELOAD_CONFIG_REG("SnsDataConfig", SnsDataModule::ReloadConfig);
	return true;
}

bool SnsDataModule::Shut(IKernel *pKernel)
{
	return true;
}

// ��һ����ҵ���������д��SNS
bool SnsDataModule::SaveSnsData(IKernel* pKernel, const PERSISTID &self)
{
    CVarList offlien_data;
    offlien_data << COMMAND_SNS_DATA;
    offlien_data << SNS_DATA_SAVE_PLAYER_DATA;
    offlien_data << pKernel->SeekRoleUid(pKernel->QueryWideStr(self, "Name"));
    offlien_data << 8; // 8������: ����, װ��, ����, ����, ����, ��������, װ��ϴ��ֵ,�������,

    ReadPlayerAttr(pKernel, self, offlien_data);
//    ReadPlayerEquipment(pKernel, self, offlien_data);
    ReadPlayerSkill(pKernel, self, offlien_data);
//     ReadPlayerRide(pKernel, self, offlien_data);
//     ReadPlayerPet(pKernel, self, offlien_data);
//     ReadPlayerPassiveSkill(pKernel, self, offlien_data);
//	ReadPlayerEquipmentBaptise(pKernel, self, offlien_data);

    SendMsgToSnsServer(pKernel, self, offlien_data);
    
    return true;
}

//������ݼ������
int SnsDataModule::OnPlayerEntry(IKernel *pKernel, const PERSISTID &player,
                           const PERSISTID &sender, const IVarList &args)
{
    // �˲�����Ϊ����SNS���洴�������
    
    CVarList offlien_data;
    offlien_data << COMMAND_SNS_DATA;
    offlien_data << SNS_DATA_CREATE_PLAYER_ENTRY;
    offlien_data << pKernel->SeekRoleUid(pKernel->QueryWideStr(player, "Name"));
    offlien_data << 0; // 0������
    
    SendMsgToSnsServer(pKernel, player, offlien_data);
    
    return 0;
}

// player �� OnStore �¼�
int SnsDataModule::OnPlayerStoreEvent(IKernel* pKernel, const PERSISTID& self,
                                      const PERSISTID& sender, const IVarList& args)
{
    // ��������ҵ����ݱ��浽SNS������
    
    int reason = args.IntVal(0);

    // ��������뿪��Ϸ��ʱ����д洢
    if (reason == STORE_EXIT || reason == STORE_EDIT /*|| STORE_SWITCH == reason*/)
    {
        SaveSnsData(pKernel, self);
    }

    return 0;
}

// Player ��Command��Ϣ������
int SnsDataModule::OnPlayerCommandMsg(IKernel* pKernel, const PERSISTID& self, 
                                      const PERSISTID& sender, const IVarList& args)
{
    // ��Ϣ��ʽ
    //
    //    [command_id][sub_msg_id][logic_id]....
    //
    // ��Ӧ��Ϣ 
    //
    //    N/A

    int sub_msg_id = args.IntVal(1);

    switch (sub_msg_id)
    {
    case SNS_DATA_QUERY_PLAYER_DATA:
        {            
            // ����SNS�������Ĳ�ѯ���

            // ��Ϣ��ʽ: 
            //    [msg_id][sub_msg_id][logic_id][requester_uid][target_uid]
            //    [section_count]
            //    ["PlayerAttribute"][row][col][...]
            //    ["PlayerEquipment"][row][col][...]

            int args_size = (int)args.GetCount();
            int logic_id = args.IntVal(2);
            const char *request_uid = args.StringVal(3);
            
            // ������Ϣ
            int core_msg_size = args_size - 4; //core_msg �� target_uid ��ʼ
            int core_msg_index = 4; // core_msg �� target_uid ��ʼ
            
            const wchar_t *request_name = pKernel->SeekRoleName(request_uid);
            if (request_uid == NULL || request_name == NULL)
            {
                return 0;
            }
            
            #if defined(_DEBUG)
                const char *target_uid = args.StringVal(3);
                const wchar_t *target_name = pKernel->SeekRoleName(target_uid);
            #endif

            CVarList core_msg;
            core_msg << COMMAND_SNS_DATA;
            core_msg << SNS_DATA_HERE_IS_DATA_YOU_WANT;
            core_msg << logic_id;
            core_msg.Append(args, core_msg_index, core_msg_size);

            pKernel->CommandByName(request_name, core_msg);
        }
        break;
    case SNS_DATA_SHOW_ME_YOUR_DATA:
        {
            OnCommandRequestData(pKernel, self, args);
        }
        break;
    case SNS_DATA_HERE_IS_DATA_YOU_WANT:
        {
            // ��Ϣ��ʽ: [command_id][sub_msg_id][logic_id][targer_uid]
            //          [section_count][section1_name][row][col]...[sectionX_name][row][col]...

            int logic_id = args.IntVal(2);
            
            // ֻ�����������а������
            if (logic_id == SNS_DATA_REQUEST_FORM_RANK_LIST_PLAYER ||
				logic_id == SNS_DATA_REQUEST_FORM_RANK_LIST_PET ||
				logic_id == SNS_DATA_REQUEST_FORM_RANK_LIST_RIDE ||
				logic_id == SNS_DATA_REQUEST_FORM_RANK_LIST_WING ||
				logic_id == SNS_DATA_REQUEST_FORM_TEAM_PLAYER_LIST)
            {
                int core_msg_index = 4; 
                int core_msg_size = ((int)args.GetCount()) - 4;

                const char *target_uid = args.StringVal(3);
                const wchar_t *target_name = pKernel->SeekRoleName(target_uid);

                #if defined(_DEBUG)
                    const wchar_t *request_name = pKernel->QueryWideStr(self, "Name");
                #endif    

                CVarList player_data;
				player_data << logic_id;
                player_data << target_name;
                player_data.Append(args, core_msg_index, core_msg_size);

                OnCommandGotData(pKernel, self, player_data);
            }
			// �����������������
			if (logic_id == SNS_DATA_REQUEST_FORM_CHAT)
			{
				// �����ѯ���
				OnCommandGotChatData(pKernel, self, args);
			}
			
        }
        break;
    default:
        break;
    }

    return 0;
}

// Player �Ŀͻ�����Ϣ������
int SnsDataModule::OnPlayerCustomMsg(IKernel* pKernel, const PERSISTID& self,
                                     const PERSISTID& sender, const IVarList& args)
{
    // ��Ϣ��ʽ
    //
    //     [msg_id][sub_msg_id]...
    //
    // ��Ӧ��Ϣ 
    // 
    //     ͨ�� SERVER_CUSTOMMSG_SNS_DATA �Զ�����Ϣ����


    ClientRequestRankData(pKernel, self, sender, args);
    return 0;
}

// �ͻ����������а�����ҵ�����
int SnsDataModule::ClientRequestRankData(IKernel* pKernel, const PERSISTID& self,
									     const PERSISTID& sender, const IVarList& args)
{
	int sub_msg = args.IntVal(1);
	CVarList query_args;
	query_args << sub_msg;
	query_args << args.WideStrVal(2);

	switch (sub_msg)
	{
	case SNS_DATA_REQUEST_FORM_RANK_LIST_PLAYER:
		{
			query_args << 3;
			query_args << SECTION_PLAYER_ATTRIBUTE;
			query_args << SECTION_PLAYER_EQUIPMENT;
			query_args << SECTION_PLAYER_EQUIPMENT_BAPTISE;
		}
		break;
	case SNS_DATA_REQUEST_FORM_RANK_LIST_PET:
		{
			query_args << 1;
			query_args << SECTION_PLAYER_PET;
		}
		break;
	case SNS_DATA_REQUEST_FORM_RANK_LIST_RIDE:
		{
			query_args << 1;
			query_args << SECTION_PLAYER_RIDE;
		}
		break;
// 	case SNS_DATA_REQUEST_FORM_RANK_LIST_WING:
// 		{
// 			query_args << 2;
// 			query_args << SECTION_PLAYER_ATTRIBUTE;
// 			query_args << SECTION_WING_EQUIPMENT;
// 		}
// 		break;
	case SNS_DATA_REQUEST_FORM_TEAM_PLAYER_LIST:
	{
			query_args << 2
			<< SECTION_PLAYER_ATTRIBUTE
			<< SECTION_PLAYER_EQUIPMENT;
	}break;
	default:
		{
			return 0;
		}
		break;
	}

	OnQueryPlayerData(pKernel, self, sender, query_args);

	return 0;
}

// ����public����������Ϣ
int SnsDataModule::OnPublicMessage(IKernel * pKernel, const PERSISTID & self,
                                   const PERSISTID & sender, const IVarList & args)
{
    // ֻ��1�ų�������������
    int scene_id = pKernel->GetSceneId();
    if (scene_id != 1)
    {
        return 0;
    }
    
    int msg_id = args.IntVal(0);
    
//     switch (msg_id)
//     {
//     case COMMAND_NATION_OFFICIAL_MODULE:
//         OnNationPostChanged(pKernel, args);
//         break;
//     default:
//         break;
//    }

    return 0;
}

//////////////////////////////////////////////////////////////////////////
// ���ܺ���
//////////////////////////////////////////////////////////////////////////

// ����Ϣ��SNS������
bool SnsDataModule::SendMsgToSnsServer(IKernel* pKernel, const PERSISTID& self,
                         const IVarList &msg)
{
    const char *uid = pKernel->SeekRoleUid(pKernel->QueryWideStr(self, "Name"));
    return SendMsgToSnsServer(pKernel, uid, msg);
}

bool SnsDataModule::SendMsgToSnsServer(IKernel* pKernel, const char* uid,
                               const IVarList &msg)
{
    CVarList wrapper_msg;
    wrapper_msg << "sns" << L"SnsDataSync";
    wrapper_msg << msg;

    bool ret = pKernel->SendSnsMessage(pKernel->GetDistrictId(),
        pKernel->GetServerId(),
        pKernel->GetMemberId(),
        pKernel->GetSceneId(),
        uid,
        wrapper_msg);

    return ret;
}

// ��ȡ��ҵ���������
bool SnsDataModule::ReadPlayerAttr(IKernel* pKernel, const PERSISTID& self,
                           IVarList &result)
{
    // ������Թ�����: ������,����ֵ
    int col_count = 2; // name,value ����
    std::string section_name = SECTION_PLAYER_ATTRIBUTE;

    CVarList temp_result;

    const std::vector<ElementDef> * attr_list = GetSectionElements(section_name);
    if (attr_list != NULL)
    {
        std::vector<ElementDef>::const_iterator it = attr_list->begin();
        std::vector<ElementDef>::const_iterator it_end = attr_list->end();
        
        LoopBeginCheck(a);
        for (; it!=it_end; ++it)
        {
            LoopDoCheck(a);
            
            const ElementDef &element = *it;

            if (!pKernel->Find(self, element.name.c_str()))
            {
                continue;
            }

            switch (element.type)
            {
            case VTYPE_INT:
                {
                    int attr_val = pKernel->QueryInt(self, element.name.c_str());
                    temp_result << element.name.c_str();
                    temp_result << attr_val;
                }
                break;
            case VTYPE_INT64:
                {
                    int64_t attr_val = pKernel->QueryInt64(self, element.name.c_str());
                    temp_result << element.name.c_str();
                    temp_result << attr_val;
                }
                break;
            case VTYPE_FLOAT:
                {
                    float attr_val = pKernel->QueryFloat(self, element.name.c_str());
                    temp_result << element.name.c_str();
                    temp_result << attr_val;
                }
                break;
            case VTYPE_DOUBLE:
                {
                    double attr_val = pKernel->QueryDouble(self, element.name.c_str());
                    temp_result << element.name.c_str();
                    temp_result << attr_val;
                }
                break;
            case VTYPE_STRING:
                {
                    const char *attr_val = pKernel->QueryString(self, element.name.c_str());
                    temp_result << element.name.c_str();
                    temp_result << attr_val;
                }
                break;
            case VTYPE_WIDESTR:
                {
                    const wchar_t *attr_val = pKernel->QueryWideStr(self, element.name.c_str());
                    temp_result << element.name.c_str();
                    temp_result << attr_val;
                }
                break;
            default:
                break; 
            }
        }
    }

    result << section_name.c_str();
    result << ((int)temp_result.GetCount())/col_count;
    result << col_count; // ����: ������, ����ֵ
    result << temp_result;

    return true;
}

// ��ȡ���װ����Ϣ
// bool SnsDataModule::ReadPlayerEquipment(IKernel* pKernel, const PERSISTID& self,
//                                 IVarList &result)
// {
//     // װ����Ϣ����7��: װ��ConfigID, Ʒ��, ��ħ��, ������, ǿ���ȼ�, ��ʯ, ���
//     int col_count = 7;
//     std::string section_name = SECTION_PLAYER_EQUIPMENT;
//     CVarList temp_result;
// 
//     // ȡ��װ����
//     PERSISTID equipment_box = pKernel->GetChild(self, L"EquipBox");
//     if (!pKernel->Exists(equipment_box))
//     {
//         result << section_name.c_str();
//         result << 0; 
//         result << col_count;
//         result << temp_result;
// 
//         return false;
//     }
// 
//     // ȡ��ǿ����
//     IRecord * pStrengthenRec = pKernel->GetRecord(self, EQUIP_STRHENTHEN_REC);
//     if (NULL == pStrengthenRec)
//     {
//         result << section_name.c_str();
//         result << 0; 
//         result << col_count;
//         result << temp_result;
// 
//         return false;
//     }
// 
//     // ����װ����, ��ȡÿ����λ��װ��, ��ʯ, ���
//     LoopBeginCheck(a);
//     for (int i = EQUIP_POS_WEAPON; i < EQUIP_MAX_PLACEHOLDER; ++i)
//     {
//         LoopDoCheck(a);
//         
//         PERSISTID equip = pKernel->GetItem(equipment_box, i);
// 
//         // ��ȡװ����Ϣ
//         if (pKernel->Exists(equip))
//         {
// #if defined(_DEBUG)
//             const wchar_t *equipment_name = pKernel->QueryWideStr(equip, "Name");
// #endif
//             temp_result << pKernel->QueryString(equip, "Config");
//             temp_result << pKernel->QueryInt(equip, "ColorLevel");
//             temp_result << pKernel->QueryInt(equip, "AddPackage");
//             temp_result << pKernel->QueryString(equip, "ModifyAddPackage");
//         }
//         else
//         {
//             temp_result << "";
//             temp_result << 0;
//             temp_result << 0;
//             temp_result << "";
//         }
// 
//         // ��ǿ�����л�ȡ��Ӧ��λ��װ����ǿ���ȼ�
// 
//         int row_id = pStrengthenRec->FindInt(EQUIP_STRHENTHEN_INDEX, i);
//         if (row_id == -1)
//         {
//             temp_result << 0; // ǿ���ȼ�
//             temp_result << ""; // ��λ��Ƕ�ı�ʯ
//             temp_result << ""; // ��λ��Ƕ�����
// 
//             continue;
//         }
// 
//         // ǿ���ȼ�
//         int iEquipStrLv = pStrengthenRec->QueryInt(row_id, EQUIP_STRHENTHEN_LEVEL);
// 
//         // ��ǿ�����л�ȡ��Ӧ��λ�ı�ʯ
//         fast_string jewel = "";
//         
//         LoopBeginCheck(b);
//         for (int m = EQUIP_STRHENTHEN_JEWEL1; m <= EQUIP_STRHENTHEN_JEWEL4; ++m)
//         {
//             LoopDoCheck(b);
//             
//             jewel += pStrengthenRec->QueryString(row_id, m);
//             jewel += ",";
//         }
// 
//         // ��ǿ�����л�ȡ��Ӧ��λ�����
//         fast_string soul = "";
//         soul += pStrengthenRec->QueryString(row_id, EQUIP_STRHENTHEN_SOUL);
// 
//         temp_result << iEquipStrLv;
//         temp_result << jewel;
//         temp_result << soul;
//     }
// 
//     result << section_name.c_str();
//     result << ((int)temp_result.GetCount())/col_count; 
//     result << col_count;
//     result << temp_result;
// 
//     return true;
// }

// ��ȡ��Ҽ�����Ϣ
bool SnsDataModule::ReadPlayerSkill(IKernel* pKernel, const PERSISTID& self,
    IVarList &result)
{
    // ���ܰ���2��: ����ID, ���ܵȼ�
    int col_count = 2;
    std::string section_name = SECTION_PLAYER_SKILL;
    CVarList temp_result;

    // ȡ�ü�����
    PERSISTID skill_box = pKernel->GetChild(self, L"SkillContainer");
    if (!pKernel->Exists(skill_box))
    {
        result << section_name.c_str();
        result << 0; 
        result << col_count;
        result << temp_result;

        return false;
    }
    
    IGameObj *skill_box_obj = pKernel->GetGameObj(skill_box);
    if (skill_box_obj == NULL)
    {
        result << section_name.c_str();
        result << 0; 
        result << col_count;
        result << temp_result;

        return false;
    }
    
    CVarList skill_list;
    
    int box_capacity = skill_box_obj->GetCapacity();
    
    LoopBeginCheck(a);
    for (int i=1; i<=box_capacity; ++i)
    {
        LoopDoCheck(a);
        
        IGameObj *temp_skill = skill_box_obj->GetChildByIndex(i);
        if (temp_skill != NULL)
        {
            skill_list << temp_skill->GetConfig();
            skill_list << temp_skill->QueryInt("Level");
        }
    }
    
    result << section_name.c_str();
    result << (int)skill_list.GetCount()/col_count; 
    result << col_count;
    result << skill_list;

    return true;
}

// ��ȡ���������Ϣ
// bool SnsDataModule::ReadPlayerRide(IKernel* pKernel, const PERSISTID& self,
//                                    IVarList &result)
// {
//     // �������7��: 
//     // ����id��ʹ����Ƥ��id������׼����Ǽ�
//     int col_count = 4;
//     std::string section_name = SECTION_PLAYER_RIDE;
//     CVarList the_ride_info;
// 
//     // ȡ��Ŀ������
//     PERSISTID the_box = pKernel->GetChild(self, L"RideBox");
//     if (!pKernel->Exists(the_box))
//     {
//         result << section_name.c_str();
//         result << 0; 
//         result << col_count;
//         result << the_ride_info;
// 
//         return false;
//     }
// 
//     IGameObj *the_box_obj = pKernel->GetGameObj(the_box);
//     if (the_box_obj == NULL)
//     {
//         result << section_name.c_str();
//         result << 0; 
//         result << col_count;
//         result << the_ride_info;
// 
//         return false;
//     }
// 
//     // �ҵ��Ѽ��������һλ��������
// 	IGameObj *pRide = the_box_obj->GetChildByIndex(1);
// 	if (NULL == pRide)
// 	{
// 		result << section_name.c_str();
// 		result << 0;
// 		result << col_count;
// 		result << the_ride_info;
// 		return false;
// 	}
// 
// 	// ȡ��ʹ���е�Ƥ��
// 	IRecord *pSkinRec = pRide->GetRecord(FIELD_RECORD_RIDE_SKIN_REC);
// 	if (NULL == pSkinRec)
// 	{
// 		result << section_name.c_str();
// 		result << 0;
// 		result << col_count;
// 		result << the_ride_info;
// 
// 		return false;
// 	}
// 
// 	the_ride_info << pRide->GetConfig();
// 
// 	// ʹ���е�Ƥ��
// 	const int inuse_row = pSkinRec->FindInt(COLUMN_RIDE_SKIN_REC_STATE, SKIN_USE_STATE_INUSE);
// 	if (inuse_row < 0)
// 	{
// 		the_ride_info << pRide->QueryString(FIELD_PROP_DEFAULT_SKIN);
// 	}
// 	else
// 	{
// 		the_ride_info << pSkinRec->QueryString(inuse_row, COLUMN_RIDE_SKIN_REC_ID);
// 	}
// 
// 	// ����
// 	the_ride_info << pRide->QueryInt(FIELD_PROP_RIDE_STEP);
// 	the_ride_info << pRide->QueryInt(FIELD_PROP_RIDE_STAR);
// 
//     result << section_name.c_str();
//     result << (int)the_ride_info.GetCount()/col_count; 
//     result << col_count;
//     result << the_ride_info;
//     
//     return true;
// }
// 
// // ��ȡ��ҳ�����Ϣ
// bool SnsDataModule::ReadPlayerPet(IKernel* pKernel, const PERSISTID& self,
//                           IVarList &result)
// {
// 
//     // ���ܰ���6��: 
//     // ����ID, �ȼ�, �Ǽ�, �׼�, ��վ״̬, ����
// 
//     int col_count = 8;
//     std::string section_name = SECTION_PLAYER_PET;
//     CVarList the_pet_info;
// 
//     // ��ȡ���г�ս��NPC
//     CVarList pet_list;
//     PetModule::GetPetAll(pKernel, self, pet_list);
//     
//     int pet_count = (int) pet_list.GetCount();
// 
//     LoopBeginCheck(a);
//     for (int i=0; i<pet_count; ++i)
//     {
//         LoopDoCheck(a);
//         
//         PERSISTID temp_pet = pet_list.ObjectVal(i);
//         
//         if (!pKernel->Exists(temp_pet))
//         {
//             continue;
//         }
//         
//         IGameObj *temp_pet_obj = pKernel->GetGameObj(temp_pet);
//         if (temp_pet_obj == NULL)
//         {
//             continue;
//         }
//         
//         // ��������
//         the_pet_info << temp_pet_obj->GetConfig();
//         the_pet_info << temp_pet_obj->QueryInt("Level");
//         the_pet_info << temp_pet_obj->QueryInt("StarLevel");
//         the_pet_info << temp_pet_obj->QueryInt("StepLevel");
//         the_pet_info << temp_pet_obj->QueryInt("Work");
// 		the_pet_info << temp_pet_obj->QueryInt(FIELD_PROP_PET_SCORE);
//         
//         // �����б�
//         // ���ܵĸ�ʽΪ�� config_id:level,config_id:level,...config_id:level
//         std::string pet_skill_list;
//         
//         IGameObj *pet_skill_box_obj = temp_pet_obj->GetChild(L"SkillContainer");
//         if (pet_skill_box_obj != NULL)
//         { 
//             int the_box_capacity = pet_skill_box_obj->GetCapacity();
// 
//             LoopBeginCheck(b);
//             for (int i=1; i<=the_box_capacity; ++i)
//             {
//                 LoopDoCheck(b);
//                 
//                 IGameObj *temp_skill_obj = pet_skill_box_obj->GetChildByIndex(i); //pKernel->GetItem(container, i);
//                 if (temp_skill_obj == NULL)
//                 {
//                     continue;
//                 }
// 
//                 pet_skill_list.append(temp_skill_obj->GetConfig());
//                 pet_skill_list.append(":");
//                 pet_skill_list.append(util_int_as_string(temp_skill_obj->QueryInt("Level")).c_str());
//                 pet_skill_list.append(",");
//             }
//         }
// 
//         the_pet_info << pet_skill_list.c_str(); 
// 
// 		// �츳�б�
// 		std::string talent_str = "";
// 		IRecord *pRec = temp_pet_obj->GetRecord(FIELD_RECORD_PET_TALENT_REC);
// 		if (NULL != pRec)
// 		{
// 			LoopBeginCheck(b);
// 			for (int row = 0; row < pRec->GetRows(); ++row)
// 			{
// 				LoopDoCheck(b);
// 				const char *talent_id = pRec->QueryString(row, COLUMN_PET_TALENT_REC_TALENT_ID);
// 				if (StringUtil::CharIsNull(talent_id))
// 				{
// 					continue;
// 				}
// 
// 				if (!talent_str.empty())
// 				{
// 					talent_str.append(",");
// 
// 				}
// 
// 				talent_str.append(talent_id);
// 			}
// 		}
// 		the_pet_info << talent_str.c_str();
//         
//     }// End of ������������
// 
//     result << section_name.c_str();
//     result << (int)the_pet_info.GetCount()/col_count; 
//     result << col_count;
//     result << the_pet_info;
// 
//     return true;
// }
// 
// // ��ȡ��ұ���������Ϣ
// bool SnsDataModule::ReadPlayerPassiveSkill(IKernel* pKernel, const PERSISTID& self,
//                                            IVarList &result)
// {
//     std::string section_name = SECTION_PLAYER_PASSIVE_SKILL;
// 
//     // ȡ�ü�����
//     IRecord *skill_rec = pKernel->GetRecord(self, "passive_skill_rec");
// 	if (skill_rec == NULL)
// 	{
// 		return false;
// 	}
// 	// �������ܰ���3��: ����ID, �ȼ�, ��Դ
// 	int col_count = skill_rec->GetCols();
// 	if (col_count == 0)
// 	{
// 		return false;
// 	}
//  
//     CVarList skill_list;
//     CVarList temp_row;
// 
//     int skill_count = skill_rec->GetRows();
//     
//     LoopBeginCheck(a);
//     for (int i=0; i<skill_count; ++i)
//     {
//         LoopDoCheck(a);
//         
//         temp_row.Clear();
//         
//         skill_rec->QueryRowValue(i, temp_row);
//         
//         skill_list << temp_row;
//     }
// 
//     result << section_name.c_str();
//     result << (int)skill_list.GetCount()/col_count; 
//     result << col_count;
//     result << skill_list;
// 
//     return true;
// }

// ��ȡ���ϴ������
// bool SnsDataModule::ReadPlayerEquipmentBaptise(IKernel* pKernel, const PERSISTID& self,
// 											   IVarList &result)
// {
// 	// װ��ϴ����Ϣ����9��: 
// 	// װ����λ����, ����1ϴ��ֵ, ����2ϴ��ֵ, ����3ϴ��ֵ, ����4ϴ��ֵ, ����1�Ƿ�����, ����2�Ƿ�����, ����3�Ƿ�����, ����4�Ƿ�����
// 	int col_count = 9;
// 	std::string section_name = SECTION_PLAYER_EQUIPMENT_BAPTISE;
// 	CVarList temp_result;
// 
// 	// ȡ��װ����
// 	PERSISTID equipment_box = pKernel->GetChild(self, L"EquipBox");
// 	if (!pKernel->Exists(equipment_box))
// 	{
// 		result << section_name.c_str();
// 		result << 0; 
// 		result << col_count;
// 		result << temp_result;
// 
// 		return false;
// 	}
// 
// 	// ȡ��ϴ����
// 	IRecord * pBaptiseRec = pKernel->GetRecord(self, EQUIP_BAPTISE_REC);
// 	if (NULL == pBaptiseRec)
// 	{
// 		result << section_name.c_str();
// 		result << 0; 
// 		result << col_count;
// 		result << temp_result;
// 
// 		return false;
// 	}
// 
// 	// ����װ����, ��ϴ������ȡ����Ӧ������
// 	LoopBeginCheck(a);
// 	for (int i = EQUIP_POS_WEAPON; i < EQUIP_MAX_PLACEHOLDER; ++i)
// 	{
// 		LoopDoCheck(a);
// 
// 		PERSISTID equip = pKernel->GetItem(equipment_box, i);
// 
// 		// ��ȡװ����Ϣ
// 		if (pKernel->Exists(equip))
// 		{
// #if defined(_DEBUG)
// 			const wchar_t *equipment_name = pKernel->QueryWideStr(equip, "Name");
// #endif
// 			// ��ϴ������ȡ����Ӧ��ϴ������
// 
// 			int row_id = pBaptiseRec->FindInt(0, i);
// 			if (row_id == -1)
// 			{
// 				temp_result << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0;
// 				continue;
// 			}
// 
// 			temp_result << pBaptiseRec->QueryInt(row_id, EQUIP_BAPTISE_INDEX);
// 			temp_result << pBaptiseRec->QueryInt(row_id, EQUIP_BAPTISE_PROP_ONE);
// 			temp_result << pBaptiseRec->QueryInt(row_id, EQUIP_BAPTISE_PROP_TWO);
// 			temp_result << pBaptiseRec->QueryInt(row_id, EQUIP_BAPTISE_PROP_THREE);
// 			temp_result << pBaptiseRec->QueryInt(row_id, EQUIP_BAPTISE_PROP_FOUR);
// 			temp_result << pBaptiseRec->QueryInt(row_id, EQUIP_BAPTISE_PROP_ONE_LOCK_STATE);
// 			temp_result << pBaptiseRec->QueryInt(row_id, EQUIP_BAPTISE_PROP_TWO_LOCK_STATE);
// 			temp_result << pBaptiseRec->QueryInt(row_id, EQUIP_BAPTISE_RPOP_THREE_LOCK_STATE);
// 			temp_result << pBaptiseRec->QueryInt(row_id, EQUIP_BAPTISE_RPOP_FOUR_LOCK_STATE);
// 		}
// 		else
// 		{
// 			temp_result << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0;
// 		}
// 	}
// 
// 	result << section_name.c_str();
// 	result << ((int)temp_result.GetCount())/col_count; 
// 	result << col_count;
// 	result << temp_result;
// 
// 	return true;
// }

// ��Ҳ�ѯ��������
bool SnsDataModule::OnQueryPlayerData(IKernel* pKernel, const PERSISTID& self, 
    const PERSISTID& sender, const IVarList& args)
{
    // ��Ϣ��ʽ
    //
    //      [logic_id][target_user_name]
    //      [section_count][section1_name][section2_name]...
    //
    // ��Ӧ��Ϣ 
    //
    //    ͨ�� COMMAND_SNS_DATA ֪ͨ��ѯ���
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if( pSelfObj == NULL )
	{
		return false;
	}

    int logic_id = args.IntVal(0);
    const wchar_t *target_player_name = args.WideStrVal(1);
    int section_count = args.IntVal(2);
    int section_index = 3;
    
    if (section_count <= 0)
    {
        return false;
    }
    
    const char *targer_uid = pKernel->SeekRoleUid(target_player_name);
       
    if (target_player_name == NULL || targer_uid == NULL)
    {
        return false;
    }

    if (strlen(targer_uid) == 0)
    {
        return false;
    }

    // ������
    const char *requester_uid = pKernel->SeekRoleUid(pSelfObj->QueryWideStr("Name"));

    if(pKernel->GetPlayerScene(target_player_name) > 0)
    {
        
        // Ŀ���������
        CVarList request_msg;
        request_msg << COMMAND_SNS_DATA;
        request_msg << SNS_DATA_SHOW_ME_YOUR_DATA;
        request_msg << logic_id;
        request_msg << requester_uid;
        request_msg << section_count;
        
        LoopBeginCheck(a);
        for (int i=0; i<section_count; ++i)
        {
            LoopDoCheck(a);
            
            request_msg << args.StringVal(section_index + i);
        }

        pKernel->CommandByName(target_player_name, request_msg);

        return true;
    }
    else
    {

        // Ŀ����Ҳ�����

        CVarList query_msg;
        query_msg << COMMAND_SNS_DATA;
        query_msg << SNS_DATA_QUERY_PLAYER_DATA;
        query_msg << logic_id;
        query_msg << requester_uid;
        query_msg << targer_uid;
        
        query_msg << section_count;

        LoopBeginCheck(b);
        for (int i=0; i<section_count; ++i)
        {
            LoopDoCheck(b);
            
            query_msg << args.StringVal(section_index + i);
        }
        
        bool sns_success = SendMsgToSnsServer(pKernel, self, query_msg);
        
        return sns_success;
    }
}


// ��������Դ洢��SNS
// args �ĸ�ʽ: [name][value]...[name][value]
// name ���ַ�������, ���õ�name������SnsData.xml �� element_def ҳ��
bool SnsDataModule::SavePlayerAttr(IKernel* pKernel, const PERSISTID& player, const IVarList& args)
{
    if (!pKernel->Exists(player))
    {
        return false;
    }

    int attr_count = (int)(args.GetCount()/2);
    
    CVarList sns_data;
    sns_data << COMMAND_SNS_DATA;
    sns_data << SNS_DATA_SAVE_PLAYER_DATA;
    sns_data << pKernel->SeekRoleUid(pKernel->QueryWideStr(player, "Name"));
    
    sns_data << 1; // ����ֻ��һ�����ݣ��������
    sns_data << SECTION_PLAYER_ATTRIBUTE; // ����
    sns_data << attr_count; // ���Ը���
    sns_data << 2;  // ÿ������ռ������(name + value)
    sns_data << args;

    SendMsgToSnsServer(pKernel, player, sns_data);
    
    return true;
}

// ��ѯ��������������
bool SnsDataModule::OnCommandGotChatData(IKernel* pKernel, const PERSISTID& self,const IVarList& args)
{
	// args��ʽ: [command_id][sub_msg_id][logic_id][targer_uid]
	//          [section_count][section1_name][row][col]...[sectionX_name][row][col]...

	if (!pKernel->Exists(self))
	{
		return false;
	}

	const char *target_uid = args.StringVal(3);
	const wchar_t *target_name = pKernel->SeekRoleName(target_uid);

	#if defined(_DEBUG)
	const wchar_t *request_name = pKernel->QueryWideStr(self, "Name");
	#endif    

	int index = 6;
	int row = args.IntVal(index++); 
	int col = args.IntVal(index++);

	CVarList player_data;
	CVarList result_data;
	// ȡ�������������
	player_data.Append(args, index, row*col);
	
	const int propLen = 5;
	//��������ע��˳��
	std::string strPropName[propLen] = 
	{
		"Level", "Job", "Sex","GuildName"
	};
	// ��˳��ȡ
	
	LoopBeginCheck(a);
	for (unsigned int i = 0; i < strPropName->size(); ++i)
	{
	    LoopDoCheck(a);
	    
	    LoopBeginCheck(b);
		for (unsigned int counter = 0; counter < args.GetCount();++counter)
		{
		    LoopDoCheck(b);
		    
			if (strcmp(strPropName[i].c_str(),args.StringVal(counter)) == 0)
			{
				int data_type = pKernel->GetType(self,args.StringVal(counter));
				switch (data_type)
				{
				case VTYPE_INT:
					{
						result_data << args.IntVal(++counter);
					}
					break;
				case VTYPE_INT64:
					{
						result_data << args.Int64Val(++counter);
					}
					break;
				case VTYPE_FLOAT:
					{
						result_data << args.FloatVal(++counter);
					}
					break;
				case VTYPE_DOUBLE:
					{
						result_data << args.DoubleVal(++counter);
					}
					break;
				case VTYPE_STRING:
					{
						result_data << args.StringVal(++counter);
					}
					break;
				case VTYPE_WIDESTR:
					{
						result_data << args.WideStrVal(++counter);
					}
					break;
				default:
					break; 
				}
			}

		}
	}

	CVarList client_msg;
	client_msg << SERVER_CUSTOMMSG_CHAT_QUERY_RST;
	client_msg << SNS_DATA_REQUEST_FORM_CHAT;
	client_msg << target_uid <<target_name;
	client_msg << result_data;

	pKernel->Custom(self, client_msg);

	return true;

	
}

// ��������
bool SnsDataModule::OnCommandRequestData(IKernel* pKernel, const PERSISTID& self, 
                                        const IVarList& args)
{
    // args ��ʽ��
    //      [msg_id][sub_msg_id][logic_id]
    //      [requester_uid][section_count][section1_name]...
    IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if( pSelfObj == NULL )
	{
		return false;
	}

    int logic_id = args.IntVal(2);
    const char* requester_uid = args.StringVal(3);
    int section_count = args.IntVal(4);
    int section_index = 5;
    
    const wchar_t *requester_name = pKernel->SeekRoleName(requester_uid);
    if (requester_name == NULL)
    {
        return false;
    }

    CVarList player_data;
    player_data << COMMAND_SNS_DATA;
    player_data << SNS_DATA_HERE_IS_DATA_YOU_WANT;
    player_data << logic_id;
    player_data << pKernel->SeekRoleUid(pSelfObj->QueryWideStr("Name"));
    player_data << section_count;
    
    LoopBeginCheck(a);
    for (int i=0; i<section_count; ++i)
    {
        LoopDoCheck(a);
        
        const char* section_name = args.StringVal(section_index +i);
        
        if (strcmp(section_name, SNS_DATA_SECTION_ATTR) == 0)
        {
            ReadPlayerAttr(pKernel, self, player_data);
        }
        else if (strcmp(section_name, SNS_DATA_SECTION_EQUIPMENT) == 0)
        {
//            ReadPlayerEquipment(pKernel, self, player_data);
        }
        else if (strcmp(section_name, SNS_DATA_SECTION_SKILL) == 0)
        {
            ReadPlayerSkill(pKernel, self, player_data);
        }
// 		else if (strcmp(section_name, SNS_DATA_SECTION_PLAYER_PET) == 0)
// 		{
// 			ReadPlayerPet(pKernel, self, player_data);
// 		}
// 		else if (strcmp(section_name, SNS_DATA_SECTION_PLAYER_RIDE) == 0)
// 		{
// 			ReadPlayerRide(pKernel, self, player_data);
// 		}
// 		else if (strcmp(section_name, SNS_DATA_SECTION_EQUIPMENT_BAPTISE) == 0)
// 		{
// 			ReadPlayerEquipmentBaptise(pKernel, self, player_data);
// 		}
        else
        {
            // û�д˶ζ��壬�Ǿͱ��Ϊ���У���
            player_data << section_name << 0 << 0; 
        }
    }

    pKernel->CommandByName(requester_name, player_data);

    return true;
}

// �õ�����
bool SnsDataModule::OnCommandGotData(IKernel* pKernel, const PERSISTID& self,
                                     const IVarList& args)
{
	// args ��ʽ��
	//      [logic_id]
	//      [target_name]
	//      [section_count]
	//      ["PlayerAttribute"][row][col][...]
	//      ["PlayerEquipment"][row][col][...]
	//      ["PlayerPet"][row][col][...]
	//      ["PlayerRide"][row][col][...]

	int args_count = (int) args.GetCount();
	if (args_count == 0)
	{
		return false;
	}

	int logic_id = args.IntVal(0);

	// ȥ����һλ��ǩ
	CVarList tempArgs;
	tempArgs.Append(args, 1, args_count - 1);
	int tempArgs_count = (int)tempArgs.GetCount();

	// ������������ʱ, ��һ����Ϣ��ֳɶ��, ÿ����Ϣ���100���ֶ�
	int fragment_size = 100;

	int fragment_count = (tempArgs_count + fragment_size - 1)/fragment_size;
	int temp_count = fragment_size;

	CVarList client_msg;
	LoopBeginCheck(a);
	for (int i=0; i<fragment_count; ++i)
	{
	    LoopDoCheck(a);

		client_msg.Clear();

		client_msg << SERVER_CUSTOMMSG_SNS_DATA;
		client_msg << logic_id;
		client_msg << fragment_count;
		client_msg << i;

		temp_count = fragment_size;
		if ((i*fragment_size + fragment_size) >= tempArgs_count)
		{
			temp_count = fragment_size - ((i*fragment_size + fragment_size) - tempArgs_count);
		}

		client_msg.Append(tempArgs, i*fragment_size, temp_count);

#if defined(_DEBUG)
		int len = (int)client_msg.GetCount();
#endif    
		pKernel->Custom(self, client_msg);
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
// ������صĺ���
//////////////////////////////////////////////////////////////////////////

// ��ȡsection������
const std::vector<SnsDataModule::ElementDef> * SnsDataModule::GetSectionElements(
    const std::string &sec_name)
{
    SectionElementsMap::const_iterator it = m_SectionElements.find(sec_name);

    if (it == m_SectionElements.end())
    {
        return NULL;
    }
    else
    {
        return &(it->second);
    }
}

// ��ȡ������id, ���󷵻�-1 
int SnsDataModule::GetVarID(const std::string &var_name)
{
    VarDefMap::const_iterator it = m_VarDef.find(var_name);

    if (it == m_VarDef.end())
    {
        return -1;
    }
    else
    {
        return it->second;
    }
}

// ���������ļ�
bool SnsDataModule::LoadResource(IKernel *pKernel)
{
    if (!LoadVarDef(pKernel))
    {
        return false;
    }

    if (!LoadElementsDef(pKernel))
    {
        return false;
    }

    return true;
}

// ���������ļ�-��������
bool SnsDataModule::LoadVarDef(IKernel *pKernel)
{
    //
    //    <Property ID="2" TypeName="int"/>
    //

    std::string res_path = pKernel->GetResourcePath();

    std::string config_file = res_path;
    config_file.append(CONFIG_FILE_VAR_DEF);

    CXmlFile xml(config_file.c_str());
    if (!xml.LoadFromFile())
    {
        ::extend_warning(LOG_ERROR, "%s does not exists.", config_file.c_str());
        return false;
    }

    CVarList sec_list;
    xml.GetSectionList(sec_list);

    size_t sec_count = sec_list.GetCount();
    
    LoopBeginCheck(a);
    for (size_t i=0; i<sec_count; i++)
    {
        LoopDoCheck(a);
        
        const char *xml_section = sec_list.StringVal(i);
        std::string type_id = xml_section;
        std::string type_name = xml.ReadString(xml_section, "TypeName", "");

        if (type_id.length() == 0 || type_name.length() == 0)
        {                
            ::extend_warning(LOG_ERROR, "%s: invalid var define.", config_file.c_str());

            return false;
        }

        int int_id = atoi(type_id.c_str());

        m_VarDef.insert(VarDefMap::value_type(type_name, int_id));
    }

    return true;
}

// ���������ļ�-section �����
bool SnsDataModule::LoadElementsDef(IKernel *pKernel)
{
    //
    //    <Property ID="Name" Type="wstr" Section="PlayerAttribute"/>
    //

    std::string res_path = pKernel->GetResourcePath();

    std::string config_file = res_path;
    config_file.append(CONFIG_FILE_ELEMENT_DEF);

    CXmlFile xml(config_file.c_str());
    if (!xml.LoadFromFile())
    {
        ::extend_warning(LOG_ERROR, "%s does not exists.", config_file.c_str());
        return false;
    }

    CVarList sec_list;
    xml.GetSectionList(sec_list);

    size_t sec_count = sec_list.GetCount();
    
    LoopBeginCheck(a);
    for (size_t i=0; i<sec_count; i++)
    {
        LoopDoCheck(a);
        
        const char *xml_section = sec_list.StringVal(i);
        std::string elem_name = xml_section;
        std::string elem_type = xml.ReadString(xml_section, "Type", "");
        std::string elem_section = xml.ReadString(xml_section, "Section", "");

        if (elem_type.length() == 0 || elem_section.length() == 0)
        {                
            ::extend_warning(LOG_ERROR, "%s: invalid element define.", config_file.c_str());

            return false;
        }

        int type_id = GetVarID(elem_type);
        if (type_id == -1)
        {
            ::extend_warning(LOG_ERROR, "%s: invalid element define.", config_file.c_str());
            return false;
        }

        ElementDef element;
        element.name = elem_name;
        element.type = type_id;
        element.section = elem_section;

        SectionElementsMap::iterator it = m_SectionElements.find(element.section);
        if (it == m_SectionElements.end())
        {
            std::vector<ElementDef> element_list;
            element_list.push_back(element);

            m_SectionElements.insert(SectionElementsMap::value_type(element.section, element_list));
        }
        else
        {
            std::vector<ElementDef> &element_list = it->second;
            element_list.push_back(element);
        }
    }

    return true;
}

void SnsDataModule::ReloadConfig(IKernel* pKernel)
{
	m_pSnsDataModule->LoadResource(pKernel);
}