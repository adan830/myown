//--------------------------------------------------------------------
// �ļ���:      EquipmentModule.cpp
// ��  ��:      װ��ģ��
// ˵  ��:		
// ��������:    2014��10��24��
// ������:        
// �޸���:        
//    :       
//--------------------------------------------------------------------

#include "EquipmentModule.h"
#include "utils/util_func.h"
#include "utils/extend_func.h"
#include "utils/custom_func.h"
#include "utils/string_util.h"
#include "utils/XmlFile.h"
#include "FsGame/Define/ClientCustomDefine.h"
#include "FsGame/Define/ViewDefine.h"
#include "FsGame/Define/ItemTypeDefine.h"
#include "FsGame/Define/ContainerDefine.h"
#include "FsGame/Define/CommandDefine.h"
#include "FsGame/Define/StaticDataDefine.h"
#include "FsGame/CommonModule/LuaExtModule.h"
#include "FsGame/SystemFunctionModule/DropModule.h"
#include "FsGame/SkillModule/SkillToSprite.h"

#include "FsGame/Define/ModifyPackDefine.h"
#ifndef FSROOMLOGIC_EXPORTS
#include "FsGame/CommonModule/ReLoadConfigModule.h"
#endif
#include "FsGame/Define/GameDefine.h"
#include "ItemModule/ToolItem/ToolItemModule.h"
#include "SystemFunctionModule/StaticDataQueryModule.h"


EquipmentModule *	EquipmentModule::m_pEquipmentModule = NULL;
DropModule *		EquipmentModule::m_pDropModule = NULL;

inline int nx_reload_equipment_config(void* state)
{
	// ��ú���ָ��
	IKernel* pKernel = LuaExtModule::GetKernel(state);

	// ���ָ����ڣ�����ģ���Ѵ���
	if (NULL != EquipmentModule::m_pEquipmentModule)
	{
		EquipmentModule::m_pEquipmentModule->LoadResource(pKernel);
	}

	return 0;
}
//��ʼ��
bool EquipmentModule::Init(IKernel* pKernel)
{
    m_pEquipmentModule = this;
    m_pDropModule = (DropModule*)pKernel->GetLogicModule("DropModule");

	assert(m_pDropModule != NULL);

    pKernel->AddEventCallback("player", "OnRecover", EquipmentModule::OnPlayerRecover, -1);
    pKernel->AddEventCallback("player", "OnReady", EquipmentModule::OnPlayerReady);
	pKernel->AddEventCallback("player", "OnContinue", EquipmentModule::OnPlayerReady);

	pKernel->AddEventCallback("EquipBox", "OnCreate", EquipmentModule::OnEquipBoxCreate);
    pKernel->AddClassCallback("EquipBox", "OnCreateClass", EquipmentModule::OnEquipBoxClassCreate);

	//���ﴩ��װ���ص�
    pKernel->AddEventCallback("EquipBox", "OnAfterAdd", EquipmentModule::OnEquipBoxAfterAdd, -1);
    pKernel->AddEventCallback("EquipBox", "OnRemove", EquipmentModule::OnEquipBoxRemove, -1);
	//�����Ƿ��ܴ���װ���ص�(��0��ʾ���ܼ���)
    pKernel->AddEventCallback("EquipBox", "OnNoAdd", EquipmentModule::OnEquipBoxNoAdd);
    pKernel->AddEventCallback("EquipBox", "OnNoRemove", EquipmentModule::OnEquipBoxNoRemove);

	LoadResource(pKernel);

#ifndef FSROOMLOGIC_EXPORTS
	RELOAD_CONFIG_REG("EquipmentModule", EquipmentModule::ReloadEquipConfig);
#endif
	
	DECL_LUA_EXT(nx_reload_equipment_config);

    return true;
}

//�ͷ�
bool EquipmentModule::Shut(IKernel* pKernel)
{
	ReleaseResource(pKernel);

    return true;
}

//////////////////////////////////////////////////////////////////////////

/**
@ brief    ���4�����Իص���ˢ����װ�� ˢ���������ϵ�װ��,�����½�ص�< ��½ʱ���� >
@ param [IN]�����б�
*/
int EquipmentModule::OnPlayerRecover(IKernel* pKernel,const PERSISTID& self,
                                     const PERSISTID& sender,const IVarList& args)
{
    PERSISTID box = pKernel->GetChild(self, L"EquipBox");
    if (!pKernel->Exists(box))
    {
		box = pKernel->CreateContainer(self, "EquipBox", EQUIP_POS_STR_MAX);
		if (!pKernel->Exists(box))
		{
			extend_warning(LOG_ERROR, "[Error]create EquipBox error!");
			return 0;
		}
    }

    return 0;
}

int EquipmentModule::OnPlayerReady(IKernel* pKernel,const PERSISTID& self,
                                   const PERSISTID& sender,const IVarList& args)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return 0;
	}

    int online_flag = pSelfObj->QueryInt("Online");
    
    // online_flag == 1 ��ʾ���������
    if (!pKernel->FindViewport(self, VIEWPORT_EQUIP) && online_flag == 1)
    {
        PERSISTID box = pKernel->GetChild(self, L"EquipBox");
        if (pKernel->Exists(box))
        {
            pKernel->AddViewport(self, VIEWPORT_EQUIP, box);
        }
        else
        {
            ::extend_warning(pKernel, /*LOG_WARNING, */"[EquipmentModule::OnPlayerReady]: EquipBox not Exists");
        }
    }

    return 0;
}

int EquipmentModule::OnEquipBoxCreate(IKernel* pKernel, const PERSISTID& equipbox,
                                      const PERSISTID& sender, const IVarList& args)
{
	IGameObj* pEquipBoxObj = pKernel->GetGameObj(equipbox);
	if (pEquipBoxObj == NULL)
	{
		return 0;
	}

    pEquipBoxObj->SetWideStr("Name", L"EquipBox");

    return 0;
}

int EquipmentModule::OnEquipBoxClassCreate(IKernel* pKernel, int index)
{
    pKernel->SetSaving(index, "Name", true);

    return 0;
}



/**
@ brief    װ��װ����ˢ�����
@ param sender���Ƿ���ȥ��equipment
*/
int EquipmentModule::OnEquipBoxAfterAdd(IKernel* pKernel,const PERSISTID& equipbox,
                                        const PERSISTID& sender,const IVarList& args)
{
    PERSISTID owner = pKernel->Parent(equipbox);
    if (pKernel->Exists(owner) && pKernel->Type(owner) == TYPE_PLAYER)
    {
        m_pEquipmentModule->OnPutonEquipment(pKernel, owner, sender);
	}

    return 0;
}

int EquipmentModule::OnEquipBoxRemove(IKernel* pKernel,const PERSISTID& equipbox,
                                      const PERSISTID& sender,const IVarList& args)
{
    PERSISTID owner = pKernel->Parent(equipbox);
    if (pKernel->Exists(owner) && pKernel->Type(owner) == TYPE_PLAYER)
    {
        m_pEquipmentModule->OnTakeoffEquipment(pKernel, owner, sender);
    }
    
    return 0;
}

// �ж��Ƿ�ɷ�װ����
int EquipmentModule::OnEquipBoxNoAdd(IKernel* pKernel,const PERSISTID& equipbox,
                                     const PERSISTID& item,const IVarList& args)
{
	PERSISTID owner = pKernel->Parent(equipbox);//װ����ӵ����<ͨ�������>
	if (!pKernel->Exists(owner))
	{
		return 1;
	}

	int pos = args.IntVal(0);//װ������
	//�ж��ܷ�װ�����жϽ��
	EQUIPCHECK_RESULT result = m_pEquipmentModule->CanEquip(pKernel, owner, item, pos);
	if (result != EQUIPCHECK_RESULT_SUCCEED)
	{
		//�������
		//fast_string fast_string_id = ::util_int64_as_string(result + 1200);
		//::CustomSysInfo(pKernel, owner, TIPSTYPE_SYSFUNCTION_PROMPT_MESSAGE, fast_string_id.c_str(), CVarList());
		return 1;
	}

	//�ж���ҵ�ǰ�ܷ������ǰװ��
	result = m_pEquipmentModule->CanChangeEquip(pKernel, owner, item);
	if (result != EQUIPCHECK_RESULT_SUCCEED)
	{
		//�������
		//fast_string fast_string_id = ::util_int64_as_string(result + 1200);
		//::CustomSysInfo(pKernel, owner, TIPSTYPE_SYSFUNCTION_PROMPT_MESSAGE, fast_string_id.c_str(), CVarList());
		return 1;
	}

    return 0;
}

int EquipmentModule::OnEquipBoxNoRemove(IKernel* pKernel,const PERSISTID& equipbox,
                                        const PERSISTID& equip,const IVarList& args)
{
	PERSISTID owner = pKernel->Parent(equipbox);
	if (!pKernel->Exists(owner))
	{
		return 0;
	}

	//�ж���ҵ�ǰ�ܷ������ǰװ��
	EQUIPCHECK_RESULT result = m_pEquipmentModule->CanChangeEquip(pKernel, owner, equip);
	if (result != EQUIPCHECK_RESULT_SUCCEED)
	{
		//�������
		m_pEquipmentModule->ReportEquipError(pKernel, equip, result);
		return 0;
	}

	return 0;
}

//ˢ������
bool EquipmentModule::RefreshVisible(IKernel* pKernel, const PERSISTID& self)
{
	IGameObj* pPlayerObj = pKernel->GetGameObj(self);
	if (pPlayerObj == NULL)
	{
		return false;
	}

    // �������
    //m_pFashionModule->OnEquipmentChanged(pKernel, self);

	return true;
}

void EquipmentModule::LoadResource(IKernel* pKernel)
{
	m_EquipNameIndex.clear();
	m_EquipIndexName.clear();

	std::string path = std::string(pKernel->GetResourcePath()) + "ini/";
	std::string pathname = path + "Item/Equip/EquipmentConfig.xml";

	CXmlFile inifile(pathname.c_str());
	inifile.SetFileName(pathname.c_str());
	if (!inifile.LoadFromFile())
	{
		::extend_warning(pKernel/*LOG_INFO*/, "[Error]EquipmentModule::LoadResource: No found file:");
		::extend_warning(pKernel/*LOG_INFO*/, pathname.c_str());
		return;
	}

	//��ȡ��װ�������Ӧ��װ������λ��
	CVarList sec_list;
	inifile.GetSectionList(sec_list);
	
	LoopBeginCheck(d)
	for (size_t i = 0; i < sec_list.GetCount(); i++)
	{
	    LoopDoCheck(d)
	    
		std::string strName = sec_list.StringVal(i);
		if (strName.empty())
		{
			continue;
		}

		std::string strPos = inifile.ReadString(strName.c_str(), "Position", "");
		int pos = ::atoi(strPos.c_str());
		
        m_EquipIndexName.insert(std::make_pair(pos, strName)); //[pos] = strName;
        m_EquipNameIndex.insert(std::make_pair(strName, pos)); //[strName] = pos;
	}
}

// �ͷ������ļ�
void EquipmentModule::ReleaseResource(IKernel* pKernel)
{
	m_EquipNameIndex.clear();
	m_EquipIndexName.clear();
}

//�ж�һ��װ���Ƿ��װ������ҵ�װ������ָ��λ��
EQUIPCHECK_RESULT EquipmentModule::CanEquip(IKernel* pKernel,const PERSISTID& self,const PERSISTID& item,int pos)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return EQUIPCHECK_RESULT_ERR_NOTEQUIPITEM;
	}

	IGameObj* pItemObj = pKernel->GetGameObj(item);
	if (pItemObj == NULL)
	{
		return EQUIPCHECK_RESULT_ERR_NOTEQUIPITEM;
	}

	//�ж��Ƿ���һ��װ��
	std::string strScript = pItemObj->QueryString(FIELD_PROP_SCRIPT);
	if (!StringUtil::CharIsEqual(strScript.c_str(), CLASS_NAME_EQUIPMENT))
	{
		return EQUIPCHECK_RESULT_ERR_NOTEQUIPITEM;
	}

	//�ȼ��Ƿ�
	if (pSelfObj->QueryInt(FIELD_PROP_LEVEL) < pItemObj->QueryInt(FIELD_PROP_LIMIT_LEVEL))
	{
		return EQUIPCHECK_RESULT_ERR_LEVEL;
	}

	//ְҵ�����Ƿ���ȷ
	int job = pSelfObj->QueryInt(FIELD_PROP_JOB);
	int joblimit = pItemObj->QueryInt(FIELD_PROP_CLASS_LIMIT);
	if (!IsMatch(joblimit, job))
	{
		return EQUIPCHECK_RESULT_ERR_JOB;
	}

	//����װ�����͵Ĳ�ͬ���ж��Ƿ��ǿ���װ����λ��
	std::string equiptype = pItemObj->QueryString(FIELD_PROP_EQUIP_TYPE);
	if (equiptype.empty())
	{
		return EQUIPCHECK_RESULT_ERR_NOEQUIPTYPE;
	}

	//�ж�ָ��λ���ܷ�װ��װ��
	if (pos > 0)
	{
        std::map<std::string, int>::iterator it = m_EquipNameIndex.find(equiptype);
        
		if (it == m_EquipNameIndex.end())
		{
			return EQUIPCHECK_RESULT_ERR_NOEQUIPTYPE;
		}

		bool bmatchpos = (it->second == pos);
		if (!bmatchpos)
		{
			//����ֻ�ж�װ����λ���Ƿ�ƥ��
			return EQUIPCHECK_RESULT_ERR_POSITION;
		}
	}

	return EQUIPCHECK_RESULT_SUCCEED;
}

//���װ���Ƿ���Ա�����
EQUIPCHECK_RESULT EquipmentModule::CanChangeEquip(IKernel* pKernel, const PERSISTID& owner, const PERSISTID& equip)
{
	IGameObj* pEquipObj = pKernel->GetGameObj(equip);
	if (pEquipObj == NULL)
	{
		return EQUIPCHECK_RESULT_ERR_NOTEQUIPITEM;
	}


	const char* str_equ_type = pEquipObj->QueryString(FIELD_PROP_EQUIP_TYPE);
	if (NULL == str_equ_type)
	{
		return EQUIPCHECK_RESULT_ERR_NOTEQUIPITEM;
	}

	return EQUIPCHECK_RESULT_SUCCEED;
}

//����װ������
void EquipmentModule::ReportEquipError(IKernel* pKernel, const PERSISTID& equip, EQUIPCHECK_RESULT result)
{
	PERSISTID owner = ::find_firsttype_parent(pKernel, equip, TYPE_PLAYER);
	if (!pKernel->Exists(owner))
	{
		return;
	}

	//::CustomSysInfo(pKernel, owner, TIPSTYPE_SYSFUNCTION_PROMPT_MESSAGE, "EquipmentModule::ReportEquipError", CVarList());
}

int EquipmentModule::GetEquipCount( IKernel* pKernel, const PERSISTID& self )
{
	PERSISTID equipbox = pKernel->GetChild(self, L"EquipBox");
	int count = 0;

	unsigned int it;
	PERSISTID equip = pKernel->GetFirst(equipbox, it);
	
	while (pKernel->Exists(equip))
	{
		IGameObj* pEquipObj = pKernel->GetGameObj(equip);
		if (pEquipObj != NULL)
		{
			count++;
		}
		equip = pKernel->GetNext(equipbox, it);
	}

	return count;
}

int EquipmentModule::GetEquipCountByColor( IKernel* pKernel, const PERSISTID& self, int nColor )
{
	PERSISTID equipbox = pKernel->GetChild(self, L"EquipBox");
	int count = 0;

	unsigned int it;
	PERSISTID equip = pKernel->GetFirst(equipbox, it);

	while (pKernel->Exists(equip))
	{
		IGameObj* pEquipObj = pKernel->GetGameObj(equip);
		if (pEquipObj != NULL)
		{
			int colorlevel = pEquipObj->QueryInt("ColorLevel");
			if(colorlevel == nColor)
			{
				count++;
			}
		}
		equip = pKernel->GetNext(equipbox, it);
	}

	return count;
}

int EquipmentModule::GetEquipCountByGreaterColor( IKernel* pKernel, const PERSISTID& self, int nMinColor )
{
	PERSISTID equipbox = pKernel->GetChild(self, L"EquipBox");
	int count = 0;

	unsigned int it;
	PERSISTID equip = pKernel->GetFirst(equipbox, it);

	while (pKernel->Exists(equip))
	{
		IGameObj* pEquipObj = pKernel->GetGameObj(equip);
		if (pEquipObj != NULL)
		{
			int colorlevel = pEquipObj->QueryInt("ColorLevel");
			if(colorlevel >= nMinColor)
			{
				count++;
			}
		}
		equip = pKernel->GetNext(equipbox, it);
	}

	return count;
}


// ����ɫ���ϵ�����Ƕ���״̬
bool EquipmentModule::CheckSoul(IKernel* pKernel, const PERSISTID& self,
                                SoulState *soul_state)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return false;
	}

    PERSISTID equipbox = pKernel->GetChild(self, EQUIP_BOX_NAME_WSTR);
    if (!pKernel->Exists(equipbox))
    { 
        return false;
    }

    //ǿ����
    IRecord * pStrengthenRec = pSelfObj->GetRecord(EQUIP_STRHENTHEN_REC);
    if (NULL == pStrengthenRec)
    {
        return false;
    }
    
    soul_state->equip_all_set = true;
    soul_state->soul_all_set = true;
    soul_state->soul_level = 0;
    soul_state->soul_class = 0;
    
    //����װ����
    LoopBeginCheck(c)
    for (int pos = EQUIP_POS_STR_MIN; pos <= EQUIP_POS_STR_MAX; ++pos)
    {
        LoopDoCheck(c)
        
        // ���˵����
       /* if (pos == EQUIP_POS_WING)
        {
            continue;
        }*/
        
        // �Ȳ�װ����λ�Ƿ���װ��
        PERSISTID equip = pKernel->GetItem(equipbox, pos);
        if (!pKernel->Exists(equip))
        {
            soul_state->equip_all_set = false;
        }
        
        // ����@pos�ҵ���װ����ǿ�����е�λ��
        int row = pStrengthenRec->FindInt(EQUIP_STRHENTHEN_INDEX, pos);
        if (row < 0)
        {
            continue;
        }

        // ��ǿ�����л�����
        const char *soul_config_id = pStrengthenRec->QueryString(row, EQUIP_STRHENTHEN_SOUL);

        if (strlen(soul_config_id) == 0)
        {
            // ����λû�����
            soul_state->soul_all_set = false;
            continue;;
        }

        // ���ĵȼ��ͷ���
        int temp_soul_level = atoi(pKernel->GetConfigProperty(soul_config_id, "Level"));
        int temp_soul_class = atoi(pKernel->GetConfigProperty(soul_config_id, "SoulClass"));

        // ȡ�������͵ĵȼ�
        if (soul_state->soul_level == 0)
        {
            soul_state->soul_level = temp_soul_level;
        }
        else if (soul_state->soul_level > temp_soul_level)
        {
            soul_state->soul_level = temp_soul_level;
        }
        
        // ȡ�������͵ķ���
        if (soul_state->soul_class == 0)
        {
            soul_state->soul_class = temp_soul_class;
        }
        else if (soul_state->soul_class > temp_soul_class)
        {
            soul_state->soul_class = temp_soul_class;
        }
    }

    return true;
}

// ��ȡ��ɫ���ϵ�������Ϣ
const char* EquipmentModule::GetEquipConfig(IKernel* pKernel, const PERSISTID& self, int equip_index)
{
    static const char *empty = "";
    
    if (equip_index < EQUIP_POS_STR_MIN || equip_index > EQUIP_POS_STR_MAX)
    {
        return empty;
    }
    
    if (!pKernel->Exists(self))
    {
        return empty;
    }

    PERSISTID equipbox = pKernel->GetChild(self, EQUIP_BOX_NAME_WSTR);
    if (!pKernel->Exists(equipbox))
    { 
        return empty;
    }
    
    PERSISTID equip = pKernel->GetItem(equipbox, equip_index);
    if (pKernel->Exists(equip))
    {
        return pKernel->GetConfig(equip);
    }
    
    return empty;
}

// ��ȡ��ɫ���ϵ�������Ϣ
bool EquipmentModule::GetEquipInfo(IKernel* pKernel, const PERSISTID& self, int equip_index, EquipInfo *info)
{
    if (equip_index < EQUIP_POS_STR_MIN || equip_index > EQUIP_POS_STR_MAX)
    {
        return false;
    }

    if (!pKernel->Exists(self))
    {
        return false;
    }

    PERSISTID equipbox = pKernel->GetChild(self, EQUIP_BOX_NAME_WSTR);
    if (!pKernel->Exists(equipbox))
    { 
        return false;
    }

    PERSISTID equip = pKernel->GetItem(equipbox, equip_index);
	IGameObj* pEquipObj = pKernel->GetGameObj(equip);
	if (pEquipObj != NULL)
	{
        info->config_id = pKernel->GetConfig(equip);
        info->color = pEquipObj->QueryInt("ColorLevel");
    }
    else
    {
        info->config_id = "";
        info->color = 0;
    }

    return true;
}

//���ĳ�����ӵ�װ������
std::string EquipmentModule::GetEquipType(int index)
{
	std::map<int, std::string>::iterator itr = m_EquipIndexName.find(index);
	if (itr != m_EquipIndexName.end())
	{
		return (*itr).second;
		
	}
	
	return "";
}

//���ĳ�����Ͷ�Ӧ�ĸ���
int EquipmentModule::GetEquipIndex(const char * szType)
{
	std::map<std::string, int>::iterator itr = m_EquipNameIndex.find(szType);
	if (itr != m_EquipNameIndex.end())
	{
		return (*itr).second;

	}

	return 0;
}

// �Ƿ�ƥ��
bool EquipmentModule::IsMatch(int classlimit, int condition)
{
	if (classlimit == 0)
	{
		return false;
	}

	int temp = 0;
	while(classlimit != 0)
	{
		temp = classlimit % 10;
		if (temp == condition)
		{
			return true;
		}

		classlimit /= 10;
	}

	return false;
}

//��װ��������һ��װ��
bool EquipmentModule::CreateEquip(IKernel * pKernel, const PERSISTID & container, const char * config, int count, int equip_prop_percent)
{
	IGameObj* pContainerObj = pKernel->GetGameObj(container);
	if (pContainerObj == NULL)
	{
		return false;
	}

	if (StringUtil::CharIsNull(config))
	{
		//��Ʒ��
		return false;
	}

	const wchar_t* wsName = pContainerObj->QueryWideStr("Name");
	if (StringUtil::CharIsNull(wsName) || ::wcscmp(wsName, EQUIP_BOX_NAME_WSTR) != 0)
	{
		//����װ����
		return false;
	}

	const char * szScript = pKernel->GetConfigProperty(config, "Script");
	const char * szEquipType = pKernel->GetConfigProperty(config, "EquipType");
	if (StringUtil::CharIsNull(szScript) ||
		StringUtil::CharIsNull(szEquipType) ||
		::strcmp(szScript, "Equipment") != 0)
	{
		//��װ��
		return false;
	}

	int iEquipIndex = GetEquipIndex(szEquipType);
	if (iEquipIndex == 0)
	{
		//û���ҵ���Ӧ����
		return false;
	}

	PERSISTID oldequip = pKernel->GetItem(container, iEquipIndex);
	if (pKernel->Exists(oldequip))
	{
		//��װ����
		return false;
	}

	PERSISTID equip = pKernel->CreateFromConfig(pKernel->GetScene(), "", config);
	IGameObj* pEquipObj = pKernel->GetGameObj(equip);
	if (pEquipObj == NULL)
	{
		return false;
	}

	if (::strcmp(szEquipType, "Fashion") != 0)
	{
		//��ʱװ
		SetRandEquipProp(pKernel, equip, equip, count, equip_prop_percent);
	}

	pEquipObj->SetInt("Amount", 1);

	if (!pKernel->PlacePos(equip, container, iEquipIndex))
	{
		//װ����������,����ȼ�,ְҵ��,�ο�CanEquip
		return false;
	}

	return true;
}

// ���ϳ��
void EquipmentModule::OnPutonEquipment(IKernel * pKernel, const PERSISTID &owner,
    const PERSISTID & equipment)
{  
	IGameObj* pEquipObj = pKernel->GetGameObj(equipment);
	if (pEquipObj == NULL)
	{
		return;
	}
#ifndef FSROOMLOGIC_EXPORTS
	if (ToolItemModule::IsBindAble(pKernel, equipment))
	{
		// ��
		pEquipObj->SetInt(FIELD_PROP_BIND_STATUS, ITEM_BOUND);
	}
#endif

    // ˢ�����
    m_pEquipmentModule->RefreshVisible(pKernel, owner);
}

// ���³��
void EquipmentModule::OnTakeoffEquipment(IKernel * pKernel, const PERSISTID &owner,
    const PERSISTID & equipment)
{  
	IGameObj* pEquipObj = pKernel->GetGameObj(equipment);
	if (pEquipObj == NULL)
	{
		return;
	}

    // ˢ�����
    m_pEquipmentModule->RefreshVisible(pKernel, owner);
}

// ���װ���������
int EquipmentModule::SetRandEquipProp(IKernel * pKernel, const PERSISTID & self, 
                                 const PERSISTID & equip, int color_level, int prop_percent)
{
	IGameObj* pEquipObj = pKernel->GetGameObj(equip);
	if (pEquipObj == NULL)
	{
		return 0;
	}

    // ColorLevel=0û��Ҫ��������
	if (color_level > COLOR_ZERO 
		&& color_level < COLOR_MAX)
    {
        // ����BasePackage
        int add_package = pEquipObj->QueryInt("AddPackage");
        if (add_package <= 0)
        {
            return 1;
        }

        add_package += color_level * ITEM_BASE_PACKAGE_COEFFICIENT;

        // ����ColorLevel
        pEquipObj->SetInt("ColorLevel", color_level);
        pEquipObj->SetInt("AddPackage", add_package);

        // ����ModifyAddPackage
        int modify_level = pEquipObj->QueryInt("ModifyLevel");
        if (modify_level <= 0)
        {
            // ����û�и�������
            return 1;
        }

        modify_level += color_level * ITEM_BASE_PACKAGE_COEFFICIENT;

        pEquipObj->SetInt("ModifyLevel", modify_level);

        // �������Ժ͸��������������
        CVarList vPropNameList;
        CVarList vPropValueList;
        CVarList vModifyNameList;
        CVarList vModifyValeList;

        char buff[64] = {0};
        itoa(add_package, buff, 10);
        std::string strAddPackage = buff;
        itoa(modify_level, buff, 10);
        std::string strModifyLevel = buff;

        StaticDataQueryModule::m_pInstance->GetOneRowData(STATIC_DATA_EQUIP_ADD_PACK, 
            strAddPackage, vPropNameList, vPropValueList);

        StaticDataQueryModule::m_pInstance->GetOneRowDataStr(STATIC_DATA_EQUIP_RANDINTERVAL_PACK, 
            strModifyLevel, vModifyNameList, vModifyValeList);

        if (vPropNameList.GetCount() != vPropValueList.GetCount() || 
            vModifyNameList.GetCount() != vModifyValeList.GetCount())
        {
            assert(0);
            return 0;
        }

        fast_string strComProp = "";
        CVarList tempList;
        int modifyValue = 0;

        // ����Ѱ�Ҹ������Բ������������
        LoopBeginCheck(a)
        for (size_t i = 0; i < vPropNameList.GetCount(); ++i)
        {
            LoopDoCheck(a)

                const char * szPropName = vPropNameList.StringVal(i);
            if (StringUtil::CharIsNull(szPropName))
            {
                continue;
            }

            // ��ֵ�޸������ֵ
            if (vPropValueList.IntVal(i) == 0)
            {
                continue;
            }

            LoopBeginCheck(b)
                for (size_t m = 0; m < vModifyNameList.GetCount(); ++m)
                {
                    LoopDoCheck(b)

                    const char * szModifyName = vModifyNameList.StringVal(m);
                    if (StringUtil::CharIsNull(szModifyName))
                    {
                        continue;
                    }

                    if (strcmp(szPropName, szModifyName) == 0)
                    {
                        tempList.Clear();

                        util_split_string(tempList, vModifyValeList.StringVal(m), ":");

                        if (tempList.GetCount() >= 2)
                        {
                            modifyValue = GetRandAddProp(tempList.IntVal(0), tempList.IntVal(1), prop_percent);

                            if (strComProp.empty())
                            {
                                SPRINTF_S(buff, "%d", modifyValue);
                            }
                            else
                            {
                                SPRINTF_S(buff, ",%d", modifyValue);
                            }

                            strComProp += buff;
                        }
                    }
                }
            }

            if (!strComProp.empty())
            {
                pEquipObj->SetString("ModifyAddPackage", strComProp.c_str());
            }
    }

    pKernel->Command(self, equip, CVarList()<<COMMAND_EQUIP_ABILITY);

    return 0;
}

int EquipmentModule::GetRandAddProp(int minValue, int maxValue, int prop_percent)
{
    int result = minValue;

    if (minValue == maxValue)
    {
        return result;
    }
    else if (minValue > maxValue)
    {
        // �߻����
        assert(0);
        result = maxValue;
    }
    else
    {   
        int offset = 0;
        
        if (prop_percent < 0)
        {
            offset = rand() % (maxValue - minValue);
        }
        else
        {
            float prop_rate = (float(prop_percent))/100;
            offset = static_cast<int>((maxValue - minValue) * prop_rate);
        }
        
        result = minValue + offset;
    }

    return result;
}

// ���¼���װ������
void EquipmentModule::ReloadEquipConfig(IKernel* pKernel)
{
	EquipmentModule::m_pEquipmentModule->LoadResource(pKernel);
}
