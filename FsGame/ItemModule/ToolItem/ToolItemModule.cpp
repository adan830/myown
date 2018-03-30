//---------------------------------------------------------
//�ļ���:       ToolItemModule.h
//��  ��:       �ɱ�ʹ�õĵ�������Ʒ
//˵  ��:       
//          
//��������:      2014��11��24��
//������:         
//�޸���:
//   :         
//---------------------------------------------------------

#include "ToolItemModule.h"

#include "utils/extend_func.h"
#include "utils/util_func.h"
#include "utils/util_ini.h"
#include "utils/util_func.h"
#include "utils/XmlFile.h"
#include "utils/custom_func.h"
#include "utils/string_util.h"

#include "FsGame/Define/CommandDefine.h"
#include "FsGame/Define/ClientCustomDefine.h"
#include "FsGame/Define/CoolDownDefine.h"
#include "FsGame/Define/FlowCategoryDefine.h"
#include "FsGame/Define/ServerCustomDefine.h"
#include "FsGame/Define/ToolBoxDefine.h"
#include "FsGame/Define/ItemTypeDefine.h"
//#include "FsGame/Define/OffLineDefine.h"
#include "FsGame/Define/ToolBoxSysInfoDefine.h"
//#include "FsGame/Define/NewNotifyDefine.h"

#include "FsGame/SystemFunctionModule/DropModule.h"
#include "FsGame/SystemFunctionModule/CoolDownModule.h"

#include "FsGame/CommonModule/LogModule.h"
#include "FsGame/CommonModule/LevelModule.h"
#include "FsGame/CommonModule/ContainerModule.h"
#include "FsGame/CommonModule/LuaExtModule.h"

#include "FsGame/SystemFunctionModule/ToolBoxModule.h"
//#include "FsGame/ItemModule/EquipmentModule.h"
 //#include "FsGame/Define/PayDefine.h"
#include "ItemEffectBase.h"
#include "../../Define/Fields.h"
#ifndef FSROOMLOGIC_EXPORTS
#include "FsGame/CommonModule/ReLoadConfigModule.h"
#endif
#include "FsGame/Define/GameDefine.h"
#include "utils/itemhelper.h"

#define USE_ITEM_CONFIG_MERGE           "ini/Item/ItemMerge.xml"
#define USE_ITEM_CONFIG_EFFECT_TYPE     "ini/Item/ItemUseEffect.xml"
#define USE_ITEM_CONFIG_EFFECT_PACKAGE  "ini/Item/ItemUsePackage.xml"
#define PATH_ITEM_BIND_LIMIT			"ini/Item/bind_limit.xml"
#define PATH_ITEM_PATCH_DATA			"ini/Item/Patch.xml"

ToolItemModule * ToolItemModule::m_pToolItemModule = NULL;
FlowModule * ToolItemModule::m_pFlowModule = NULL;
DropModule * ToolItemModule::m_pDropModule = NULL;
ContainerModule *ToolItemModule::m_pContainerModule = NULL;
CoolDownModule *ToolItemModule::m_pCoolDownModule = NULL;
//EquipmentModule *ToolItemModule::m_pEquipmentModule = NULL;
BufferModule *ToolItemModule::m_pBufferModule = NULL;
LogModule *ToolItemModule::m_pLogModule = NULL;

//�����������������
int nx_reload_item_use_config(void* state)
{
	IKernel* pKernel = LuaExtModule::GetKernel(state);

	if (NULL != ToolItemModule::m_pToolItemModule)
	{
		ToolItemModule::m_pToolItemModule->LoadResource(pKernel);
	}

	return 1;
}

//������Ʒʹ�õ�GM����
// /use_item config_id
int nx_use_item(void* state)
{
	IKernel* pKernel = LuaExtModule::GetKernel(state);

	if (NULL == ToolItemModule::m_pToolItemModule)
	{
		return 0;
	}

	// ����������
	CHECK_ARG_NUM(state, nx_use_item, 3);

	// ����������
	CHECK_ARG_OBJECT(state, nx_use_item, 1);
	CHECK_ARG_STRING(state, nx_use_item, 2);
	CHECK_ARG_INT(state, nx_use_item, 3);

	// ��ȡ����
	PERSISTID player = pKernel->LuaToObject(state, 1);
	const char *config_id = pKernel->LuaToString(state, 2);
	int amount = pKernel->LuaToInt(state, 3);

	if (!pKernel->Exists(player))
	{
		return 0;
	}

	if (config_id == NULL)
	{
		return 0;
	}

	int max_amount = atoi(pKernel->GetConfigProperty(config_id, "MaxAmount"));

	if (amount > max_amount)
	{
		amount = max_amount;
	}

	// ������Ʒ���뱳��
	PERSISTID toolbox = pKernel->GetChild(player, ITEM_BOX_NAME_WSTR);
    if(!pKernel->Exists(toolbox))
    {
        return 0;
    }

	PERSISTID item = pKernel->CreateFromConfig(pKernel->GetScene(), "", config_id);
	if (!pKernel->Exists(item))
	{
		return 0;
	}

	IGameObj* pItemObj = pKernel->GetGameObj(item);
	if(NULL == pItemObj)
	{
		return 0;
	}

	//�ŵ������ϵĶ����������"Invisible"=false
	pItemObj->SetInt("Invisible", 1);
	//�ŵ������ϵĶ����������"LifeTime"=1s/1000ms�����߸����������
	pItemObj->SetInt("LifeTime", 1000);

	ContainerModule::PlaceItem(pKernel, toolbox, item, FUNCTION_EVENT_ID_GM_COMMAND, true);

	pItemObj->SetInt("Amount", amount);

	bool ret = ToolItemModule::m_pToolItemModule->UseItem(pKernel, player, pKernel->GetScene(), item, amount);

	if (ret)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

bool ToolItemModule::Init(IKernel* pKernel)
{
	m_pToolItemModule = this;

	m_pFlowModule = (FlowModule*)pKernel->GetLogicModule("FlowModule");
	m_pDropModule = (DropModule*)pKernel->GetLogicModule("DropModule");
	m_pContainerModule = (ContainerModule*)pKernel->GetLogicModule("ContainerModule");
	m_pCoolDownModule = (CoolDownModule*)pKernel->GetLogicModule("CoolDownModule");
	//m_pEquipmentModule = (EquipmentModule*)pKernel->GetLogicModule("EquipmentModule");
	m_pBufferModule = (BufferModule*)pKernel->GetLogicModule("BufferModule");

	Assert(m_pFlowModule && m_pDropModule && m_pContainerModule
		&& m_pCoolDownModule && m_pBufferModule);

	DECL_LUA_EXT(nx_use_item);
	DECL_LUA_EXT(nx_reload_item_use_config);

	LoadResource(pKernel);

#ifndef FSROOMLOGIC_EXPORTS
	RELOAD_CONFIG_REG("ToolItemModule", ToolItemModule::ReloadToolItemConfig);
#endif

	// ��ƷЧ�����ʼ��
// 	for (int i = 0;i < MAX_ITEM_EFFECT_NUM;++i)
// 	{
// 		m_pArrItemEffect[i]->OnInit(pKernel);
// 	}

	return true;
}

bool ToolItemModule::Shut(IKernel* pKernel)
{
	return true;
}

// �ж�ʹ����Ʒ�Ƿ���cd��
bool ToolItemModule::IsItemCoolDown(IKernel* pKernel, const PERSISTID& self, const PERSISTID& item)
{
	IGameObj* pItemObj = pKernel->GetGameObj(item);
	if (pItemObj == NULL)
	{
		return false;
	}

	if (!pKernel->Exists(self))
	{
		return false;
	}
	bool bInCD = false;
	// �����ȴʱ��
	int cooldown_id = pItemObj->QueryInt("CoolDownID");
	if (cooldown_id != 0)
	{
		if (m_pCoolDownModule->IsCoolDown(pKernel, self, cooldown_id))
		{
			bInCD = true;
		}
	}

	return bInCD;
}

bool ToolItemModule::UseItem(IKernel* pKernel, const PERSISTID& user, const PERSISTID& sender, const PERSISTID& item, int nCount)
{
	if (nCount <= 0)
	{
		return false;
	}

	// ʹ����������
	if (nCount > USE_ITEM_MAX_COUNT)
	{
		nCount = USE_ITEM_MAX_COUNT;
	}

	if (!pKernel->Exists(user))
	{
		return false;
	}

	IGameObj *user_obj = pKernel->GetGameObj(user);
	if (user_obj == NULL)
	{
		return false;
	}

	if (user_obj->QueryInt("Dead") == 1)
	{
		::CustomSysInfo(pKernel, user, SYSTEM_INFO_ID_7001, CVarList());
		// �����������ʹ��
		return false;
	}

	IGameObj *item_obj = pKernel->GetGameObj(item);
	if (item_obj == NULL)
	{
		return false;
	}

	// ������Ʒ�Ƿ��������
	PERSISTID item_owner = ::find_firsttype_parent(pKernel, item, TYPE_PLAYER);
	if (user != item_owner)
	{
		return false;
	}

	// ��������
	int item_amout = item_obj->QueryInt("Amount");
	if (nCount > item_amout)
	{
		return false;
	}

	// �����Ʒʹ�õĵȼ�����
	int limit_level = item_obj->QueryInt("LimitLevel");
	int player_level = user_obj->QueryInt("Level");
	if (limit_level > player_level)
	{
		return false;
	}

	// �����ȴʱ��
	int cooldown_id = item_obj->QueryInt("CoolDownID");
	int cooldown_time = item_obj->QueryInt("CoolDownTime");
	cooldown_time *= 1000; //ת�ɺ���
	if (cooldown_id != 0)
	{
		if (m_pCoolDownModule->IsCoolDown(pKernel, user, cooldown_id))
		{
			// ��Ʒ������ȴ��
			::CustomSysInfo(pKernel, user, SYSTEM_INFO_ID_7002, CVarList());
			return false;
		}
	}

	bool use_success = false;
	const char *script = pKernel->GetScript(item);
	if (strcmp(script, "Equipment") == 0)
	{
		// װ����ʹ��
//		use_success = m_pToolItemModule->PutOnEquipment(pKernel, user, item);
	}
	else 
	{
		// ��ͨ��Ʒ
		if (!item_obj->FindAttr("Use"))
		{
			// ����Ʒû��ʹ�ð���
			return false;
		}

		if (InnerUseItem(pKernel, user, item, nCount) > 0)
		{
			use_success = true;
		}
	}

	// ʹ�ú󣬲��ܳɹ���񶼽�����ȴ
	if (cooldown_id != 0)
	{
		m_pCoolDownModule->BeginCoolDown(pKernel, user, cooldown_id, cooldown_time);
	}

	return use_success;
}

//////////////////////////////////////////////////////////////////////////
// ��Ʒʹ��Ч��
//////////////////////////////////////////////////////////////////////////

// ʹ����Ʒ, �������ĵ���Ʒ����
int ToolItemModule::InnerUseItem(IKernel *pKernel, const PERSISTID &user, const PERSISTID &item, int request_amount)
{
	IGameObj* pItemObj = pKernel->GetGameObj(item);
	if (pItemObj == NULL)
	{
		return 0;
	}

	// �Ƿ�ﵽ��ʹ����Ʒ�ȼ�[lihl 2016/08/10]
	int nLevel = pItemObj->QueryInt( FIELD_PROP_LIMIT_LEVEL );
	if (nLevel > 0)
	{
		int nCurLevel = pKernel->QueryInt(user, FIELD_PROP_LEVEL);
		if (nCurLevel < nLevel)
		{
			// ��ʾ��ҵȼ�����
			CVarList tip_args;
			tip_args << nLevel;
			::CustomSysInfo(pKernel, user, SYSTEM_INFO_ID_7003, tip_args);
			return false;
		}
	}

	std::string pck_name = pItemObj->QueryString("Use");

	const std::vector<ItemEffectParam> *entry_list = GetEffectEntry(pck_name);

	if (entry_list == NULL)
	{
		return 0;
	}

	if (entry_list->empty())
	{
		return 0;
	}

	// ��ʹ������
	int used_amount = 0;
	const char *item_config = pKernel->GetConfig(item);

	// ��Ҫչʾ����Ʒ�б�
	CVarList showItems;
	// ��Ҫ��ʾ��ȡ��VIP��������
	int VipExp = 0;

	LoopBeginCheck(a);
	for (int i = 0; i < request_amount; ++i)
	{
		LoopDoCheck(a);

		// ������Ʒ��,�Ѽ�����Ч�����еĵ�����Ʒ
		// ���ظ����䣬һ�η��� 
		// ��֤���ĵ�����Ʒ��ʵ�ʷ��ŵ�һ�£�������ͨ�� ����ʧ��
		CVarList dropItemList;

		if (!pKernel->Exists(item))
		{
			// ��Ʒ������
			return used_amount;
		}

		// ��Ʒ�����˶��Ч����, �����һ����ʹ��ʧ�ܾ���Ϊʧ��
		// ����ʹ��
		std::vector<ItemEffectParam>::const_iterator try_it = entry_list->begin();
		std::vector<ItemEffectParam>::const_iterator try_it_end = entry_list->end();

		// lihl[2016/08/08]: �������öԿռ������⴦��
		int nCap = 0;

		LoopBeginCheck(b);
		for (; try_it != try_it_end; ++try_it)
		{
			LoopDoCheck(b);
			const ItemEffectParam& effect = *try_it;
			if (effect.effect_type <= ITEM_EFFECT_NONE || effect.effect_type >= MAX_ITEM_EFFECT_NUM)
			{
				continue;
			}
			
			if (!m_pToolItemModule->m_pArrItemEffect[effect.effect_type]->TryApplyItemEffect(pKernel, user, item, effect))
			{
				return used_amount;
			}

// 			if (effect.effect_type == ITEM_EFFECT_GIFT)
// 			{
// 				nCap += StringUtil::StringAsInt(effect.ext_param02);
// 			}
		}

		if (nCap > 0)
		{
			const PERSISTID toolbox = pKernel->GetChild(user, ITEM_BOX_NAME_WSTR);
			// ��������Ƿ����
			if (!pKernel->Exists(toolbox))
			{
				return used_amount;
			}

			// �ж������ռ��Ƿ��㹻
			int nLeftCap = ContainerModule::GetFreePosCount(pKernel, toolbox);
			if (nCap > nLeftCap)
			{
				// ��ʾ�ռ䲻��
				::CustomSysInfo(pKernel, user, SYSTEM_INFO_ID_7004, CVarList());
				return used_amount;
			}
		}

		// Ӧ����Ʒ��Ч����
		std::vector<ItemEffectParam>::const_iterator entry_it = entry_list->begin();
		std::vector<ItemEffectParam>::const_iterator entry_it_end = entry_list->end();

		LoopBeginCheck(c);
		for (; entry_it != entry_it_end; ++entry_it)
		{
			LoopDoCheck(c);
			const ItemEffectParam& effect = *entry_it;
			if (effect.effect_type <= ITEM_EFFECT_NONE || effect.effect_type >= MAX_ITEM_EFFECT_NUM)
			{
				continue;
			}
			m_pToolItemModule->m_pArrItemEffect[effect.effect_type]->ApplyItemEffect(pKernel, user, item, effect, showItems);
		}

		// �۳�һ����Ʒ
		int nColor = pKernel->QueryInt(item, FIELD_PROP_COLOR_LEVEL);
		if (ConsumItem(pKernel, item, 1) > 0)
		{
			++used_amount;

#ifndef FSROOMLOGIC_EXPORTS
			if (ContainerModule::IsNeedSaveItemLog(pKernel, item_config, nColor))
			{
				ItemLog log;
				log.eventID = FUNCTION_EVENT_ID_USE_ITEM;
				log.eventType = LOG_ACT_EVENT_LOSE;
				log.itemID = item_config;
				log.itemNum = 1;
				log.state = LOG_ACT_STATE_SUCCESS;
				log.colorLevel = nColor;
				LogModule::m_pLogModule->SaveItemLog(pKernel, user, log);
			}
#endif // FSROOMLOGIC_EXPORTS
		}
		else
		{
			return used_amount;
		}
	}

	// չʾ��õ���Ʒ
	if (!showItems.IsEmpty())
	{
		ShowGetItem(pKernel, user, showItems);
	}

	// ��ʾ���VIP����
// 	if (VipExp > 0)
// 	{
// 		// ��ʾ
// 		::CustomSysInfo(pKernel, user, TIPSTYPE_SYSFUNCTION_PROMPT_MESSAGE,
// 			SYS_INFO_TOOL_BOX_TOTAL_PAY, CVarList() << (VipExp * PAY_TOTAL_TO_VIP_EXP_RATIO));
// 	}

	return used_amount;
}

// չʾ��ȡ����Ʒ
void ToolItemModule::ShowGetItem(IKernel *pKernel, const PERSISTID &user, const IVarList& showItems)
{
	// չʾ�б�Ϊ�գ�����Ҫչʾ��Ʒ
	if (showItems.IsEmpty())
	{
		return;
	}

	// ���ͻ��˷��͵Ĳ���
	CVarList argsItems;

	// ��ȡ��Ҫչʾ����Ʒ����
	int showItemsCount = static_cast<int>(showItems.GetCount()) / TOOL_BOX_SUBMSG_SHOW_ITEM_ATTR_NUM;

	// ��ͬ��Ʒ�ϲ�
	LoopBeginCheck(showa);
	for (int i = 0; i < showItemsCount; ++i)
	{
		LoopDoCheck(showa);

		// �Ƿ���Ҫ����
		bool bAdd = true;

		const char* showConfigID = showItems.StringVal(i * TOOL_BOX_SUBMSG_SHOW_ITEM_ATTR_NUM + SHOW_ITEM_CONFIG_ID);
		if (StringUtil::CharIsNull(showConfigID))
		{
			continue;
		}

		// װ�����ϲ�
		if (!ToolItemModule::IsCanWearItem(pKernel, showConfigID))
		{
			int showAmount = showItems.IntVal(i * TOOL_BOX_SUBMSG_SHOW_ITEM_ATTR_NUM + SHOW_ITEM_AMOUNT);

			// ��������Ʒ����
			int argsCount = static_cast<int>(argsItems.GetCount()) / TOOL_BOX_SUBMSG_SHOW_ITEM_ATTR_NUM;
			LoopBeginCheck(showb);
			for (int j = 0; j < argsCount; ++j)
			{
				LoopDoCheck(showb);
				const char* argsConfigID = argsItems.StringVal(j * TOOL_BOX_SUBMSG_SHOW_ITEM_ATTR_NUM + SHOW_ITEM_CONFIG_ID);
				if (StringUtil::CharIsNull(argsConfigID))
				{
					continue;
				}

				// ConfigID��ͬ�������Ժϲ�
				if (strcmp(showConfigID, argsConfigID) == 0)
				{
					int argsAmount = argsItems.IntVal(j * TOOL_BOX_SUBMSG_SHOW_ITEM_ATTR_NUM + SHOW_ITEM_AMOUNT);
					argsItems.SetInt(j * TOOL_BOX_SUBMSG_SHOW_ITEM_ATTR_NUM + SHOW_ITEM_AMOUNT, argsAmount + showAmount);
					bAdd = false;
					break;
				}
			}
		}

		if (bAdd)
		{
			argsItems << showItems.StringVal(i * TOOL_BOX_SUBMSG_SHOW_ITEM_ATTR_NUM + SHOW_ITEM_CONFIG_ID)
				<< showItems.StringVal(i * TOOL_BOX_SUBMSG_SHOW_ITEM_ATTR_NUM + SHOW_ITEM_UNIQUE_ID)
				<< showItems.IntVal(i * TOOL_BOX_SUBMSG_SHOW_ITEM_ATTR_NUM + SHOW_ITEM_AMOUNT);
		}
	}

	// ����Ϣ���ͻ��ˣ�չʾ��õ���Ʒ
	CVarList args;
	args << SERVER_CUSTOMMSG_ITEM_REMIND
		<< TOOL_BOX_SUBMSG_SHOW
		<< static_cast<int>(argsItems.GetCount()) / TOOL_BOX_SUBMSG_SHOW_ITEM_ATTR_NUM
		<< argsItems;
	pKernel->Custom(user, args);
}

// ���߿ɷ�ʹ��
bool ToolItemModule::IsItemUsable(IKernel* pKernel, const PERSISTID& user,
								  const PERSISTID& item, int amount)
{
	return true;
}

// ������Ʒ, ����ʵ�����ĵ�����
int ToolItemModule::ConsumItem(IKernel* pKernel, const PERSISTID& item, int request_amount)
{
	// item �� amount ���ɵ����߼��

	IGameObj* pItemObj = pKernel->GetGameObj(item);
	if (pItemObj == NULL)
	{
		return 0;
	}

	int item_amount = pItemObj->QueryInt("Amount");

	if (request_amount >= item_amount)
	{
		pKernel->DestroySelf(item);

		return item_amount;
	}
	else
	{
		pItemObj->SetInt("Amount", item_amount-request_amount);

		pKernel->RunEventCallback("OnRemove", pKernel->Parent(item), item, CVarList());

		return request_amount;
	}
}

//////////////////////////////////////////////////////////////////////////
// ��Դ����
//////////////////////////////////////////////////////////////////////////

// ���ʹ�ð�����Ŀ
const std::vector<ItemEffectParam> *ToolItemModule::GetEffectEntry(const std::string &package_name)
{
	EffectParamMap::iterator it = m_EffectParam.find(package_name);
	EffectParamMap::iterator it_end = m_EffectParam.end();

	if (it == it_end)
	{
		return NULL;
	}
	else
	{
		return &(it->second);
	}
}

bool ToolItemModule::GetEffectID(std::string &the_name, int *the_id)
{
	EffectTypeMap::iterator it = m_EffectType.find(the_name);
	if (it == m_EffectType.end())
	{
		return false;
	}
	else
	{
		*the_id = it->second;

		return true;
	}
}

// ������Դ
bool ToolItemModule::LoadResource(IKernel *pKernel)
{
	if (!LoadEffectTypeConfig(pKernel))
	{
		return false;
	}

	if (!LoadEffectPackage(pKernel))
	{
		return false;
	}

	std::string bindLimitPath = pKernel->GetResourcePath();
	bindLimitPath += PATH_ITEM_BIND_LIMIT;

	Configure<CfgItemBindLimit>::Release();
	if (!Configure<CfgItemBindLimit>::LoadXml(bindLimitPath.c_str(), OnConfigError))
	{
		return false;
	}
	return true;
}

// ��Ʒʹ��Ч���ķ���
bool ToolItemModule::LoadEffectTypeConfig(IKernel *pKernel)
{
	m_EffectType.clear();

	std::string res_path = pKernel->GetResourcePath();
	std::string the_config_file = res_path;
	the_config_file.append(USE_ITEM_CONFIG_EFFECT_TYPE);

	// <Property ID="1" EffectName="hp"/>

	CXmlFile xml(the_config_file.c_str());
	if (!xml.LoadFromFile())
	{
		std::string err_msg = the_config_file;
		err_msg.append(" does not exists.");
		::extend_warning(LOG_ERROR, err_msg.c_str());

		return false;
	}

	CVarList sec_list;
	xml.GetSectionList(sec_list);

	size_t sec_count = sec_list.GetCount();
	if(sec_count != MAX_ITEM_EFFECT_NUM)
	{
		// ���ñ�����ItemEffectType ����һ��
		std::string err_msg = the_config_file;
		err_msg.append("item effect type num error");
		::extend_warning(LOG_ERROR, err_msg.c_str());
	}

	// ��ƷЧ����ָ���ʼ��
	for (int i = 0;i < MAX_ITEM_EFFECT_NUM;++i)
	{
		m_pArrItemEffect[i] = NULL;
	}

	LoopBeginCheck(i);
	for (size_t i = 0; i < MAX_ITEM_EFFECT_NUM;++i)
	{
		LoopDoCheck(i);

		const char *section = sec_list.StringVal(i);
		int nEffectType = atoi(section);
		const char* strEffectName = xml.ReadString(section, "EffectName", "");
		if (nEffectType < 0 || nEffectType >= MAX_ITEM_EFFECT_NUM || StringUtil::CharIsNull(strEffectName))
		{
			continue;
		}
		
		ItemEffectBase* pItemEffect = ItemEffectBase::CreateItemEffect((ItemEffectType)nEffectType);
		if (NULL != pItemEffect)
		{
			m_pArrItemEffect[nEffectType] = pItemEffect;
			m_EffectType.insert(EffectTypeMap::value_type(strEffectName, nEffectType));
		}
	}

	return true;
}

// ��Ʒʹ��Ч����
bool ToolItemModule::LoadEffectPackage(IKernel *pKernel)
{
	m_EffectParam.clear();

	std::string res_path = pKernel->GetResourcePath();
	std::string the_config_file = res_path;
	the_config_file.append(USE_ITEM_CONFIG_EFFECT_PACKAGE);

	// <Property ID="1" Package="pck01" EffectType="hp" EffectValue="100" ExtParam01="1000" ExtParam02="N/A"/>

	CXmlFile xml(the_config_file.c_str());
	if (!xml.LoadFromFile())
	{
		std::string err_msg = the_config_file;
		err_msg.append(" does not exists.");
		::extend_warning(LOG_ERROR, err_msg.c_str());

		return false;
	}

	CVarList sec_list;
	xml.GetSectionList(sec_list);

	size_t sec_count = sec_list.GetCount();

	LoopBeginCheck(j);
	for (size_t i=0; i<sec_count; i++)
	{
		LoopDoCheck(j);

		ItemEffectParam entry;

		const char *section = sec_list.StringVal(i);
		std::string package_name = xml.ReadString(section, "Package", "");
		std::string effect_type = xml.ReadString(section, "EffectType", "");

		if (!GetEffectID(effect_type, &(entry.effect_type)))
		{
			continue;
		}

		entry.effect_value = xml.ReadString(section, "EffectValue", "");
		entry.ext_param01 = xml.ReadString(section, "ExtParam01", "");
		entry.ext_param02 = xml.ReadString(section, "ExtParam02", "");

		if (package_name.length() == 0 || effect_type.length() == 0)
		{
			continue;
		}

		EffectParamMap::iterator it = m_EffectParam.find(package_name);
		if (it != m_EffectParam.end())
		{
			std::vector<ItemEffectParam> &entry_list = it->second;
			entry_list.push_back(entry);
		}
		else
		{
			std::vector<ItemEffectParam> entry_list;
			entry_list.push_back(entry);

			m_EffectParam.insert(EffectParamMap::value_type(package_name, entry_list));
		}
	}

	return true;
}

// ���¼��ص�������
void ToolItemModule::ReloadToolItemConfig(IKernel* pKernel)
{
	ToolItemModule::m_pToolItemModule->LoadResource(pKernel);
}


//��ȡ����Ч����Ϣ
bool ToolItemModule::GetItemEffectInfo(IKernel* pKernel, const char* itemConfig, int effectType, CVarList& args)
{
	const char* packName = pKernel->GetConfigProperty(itemConfig, "Use");
	if (strcmp(packName, "") == 0)
	{
		return false;
	}
	const std::vector<ItemEffectParam>* v_effectList = GetEffectEntry(packName);
	if (v_effectList == NULL || v_effectList->empty())
	{
		return false;
	}
	std::vector<ItemEffectParam>::const_iterator it = v_effectList->begin();

	LoopBeginCheck(k);
	for ( ; it != v_effectList->end(); ++it)
	{
		LoopDoCheck(k);

		ItemEffectParam effectInfo = *it;
		int type = effectInfo.effect_type;
		std::string value = effectInfo.effect_value;
		std::string param1 = effectInfo.ext_param01;
		std::string param2 = effectInfo.ext_param02;
		if (type == effectType)
		{
			args << value << param1 << param2;
			return true;
		}

	}
	return false;
}

// ϡ����Ʒ��ʾ
int ToolItemModule::TipsSpecialItem(IKernel* pKernel, const PERSISTID& self, 
	const char* itemConfigID, const int color, const EmFunctionEventId src)
{
	// ��ȡ��Ҷ���
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return 0;
	}

	// ��ȫ���
	if (StringUtil::CharIsNull(itemConfigID))
	{
		return 0;
	}

	// ��ȡϡ�м���
	int special = StringUtil::StringAsInt(pKernel->GetConfigProperty(itemConfigID, "SpecialItem"));
	if (special <= SPECIAL_TYPE_NORMAL)
	{
		// ����ϡ����Ʒ������ʾ
		return 0;
	}
	else
	{	
		char strNation[128] = {0};
		SPRINTF_S(strNation, "Nation%d", pSelfObj->QueryInt("Nation"));
		const wchar_t* name = pSelfObj->QueryWideStr("Name");

		// ������װ
		CVarList args;
		args << strNation 
			 << name 
			 << pSelfObj->QueryInt(FIELD_PROP_VIP_LEVEL)
			 << src
			 << itemConfigID 
			 << color;

		::CustomSysInfoBroadcast(pKernel, SYSTEM_INFO_ID_7005, args);
	}

	return 0;
}

bool ToolItemModule::IsMatchState(IKernel* pKernel, const PERSISTID& item, int nMatch)
{
	if (nMatch == ITEM_MATCH_ALL)
	{
		return true;
	}

	// ƥ��󶨲������
	if ( (nMatch & ITEM_MATCH_BOUND_AND_FROZEN) == ITEM_MATCH_BOUND_AND_FROZEN )
	{
		int nBound = pKernel->QueryInt(item, FIELD_PROP_BIND_STATUS);
		if (nBound == ITEM_BOUND)
		{
			time_t tNow = util_get_utc_time();
			int64_t nFrozeTime = pKernel->QueryInt64(item, FIELD_PROP_ITEM_FROZEN_TIME);
			if (nFrozeTime > tNow)
			{
				return true;
			}
		}
	}

	// ƥ��󶨲��������
	if ((nMatch & ITEM_MATCH_BOUND_NOT_FROZEN) == ITEM_MATCH_BOUND_NOT_FROZEN)
	{
		int nBound = pKernel->QueryInt(item, FIELD_PROP_BIND_STATUS);
		if (nBound == ITEM_BOUND)
		{
			time_t tNow = util_get_utc_time();
			int64_t nFrozeTime = pKernel->QueryInt64(item, FIELD_PROP_ITEM_FROZEN_TIME);
			if (nFrozeTime < tNow)
			{
				return true;
			}
		}
	}


	// ƥ��ǰ󶨲������
	if ((nMatch & ITEM_MATCH_NOT_BOUND_AND_FROZEN) == ITEM_MATCH_NOT_BOUND_AND_FROZEN)
	{
		int nBound = pKernel->QueryInt(item, FIELD_PROP_BIND_STATUS);
		if (nBound == ITEM_NOT_BOUND)
		{
			time_t tNow = util_get_utc_time();
			int64_t nFrozeTime = pKernel->QueryInt64(item, FIELD_PROP_ITEM_FROZEN_TIME);
			if (nFrozeTime > tNow)
			{
				return true;
			}
		}
	}

	// ƥ��ǰ󶨲��������
	if ((nMatch & ITEM_MATCH_NOT_BOUND_NOT_FROZEN) == ITEM_MATCH_NOT_BOUND_NOT_FROZEN)
	{
		int nBound = pKernel->QueryInt(item, FIELD_PROP_BIND_STATUS);
		if (nBound == ITEM_NOT_BOUND)
		{
			time_t tNow = util_get_utc_time();
			int64_t nFrozeTime = pKernel->QueryInt64(item, FIELD_PROP_ITEM_FROZEN_TIME);
			if (nFrozeTime < tNow)
			{
				return true;
			}
		}
	}

	// ����һ��ƥ�䶼����true
	if ((nMatch & ITEM_MATCH_BOUND) == ITEM_MATCH_BOUND || (nMatch & ITEM_MATCH_NOT_BOUND) == ITEM_MATCH_NOT_BOUND)
	{
		int nBound = pKernel->QueryInt(item, FIELD_PROP_BIND_STATUS);
		if (nBound == ITEM_BOUND)
		{
			if ((nMatch & ITEM_MATCH_BOUND) == ITEM_MATCH_BOUND)
			{
				return true;
			}
		}
		else
		{
			if ((nMatch & ITEM_MATCH_NOT_BOUND) == ITEM_MATCH_NOT_BOUND)
			{
				return true;
			}
		}
	}

	if ((nMatch & ITEM_MATCH_FROZEN) == ITEM_MATCH_FROZEN || (nMatch & ITEM_MATCH_NOT_FROZEN) == ITEM_MATCH_NOT_FROZEN)
	{
		time_t tNow = util_get_utc_time();
		int64_t nFrozeTime = pKernel->QueryInt64(item, FIELD_PROP_ITEM_FROZEN_TIME);
		if (nFrozeTime > tNow)
		{
			if ((nMatch & ITEM_MATCH_FROZEN) == ITEM_MATCH_FROZEN)
			{
				return true;
			}
		}
		else
		{
			if ((nMatch & ITEM_MATCH_NOT_FROZEN) == ITEM_MATCH_NOT_FROZEN)
			{
				return true;
			}
		}
	}

	return false;
}

bool ToolItemModule::IsBindAble( IKernel* pKernel, const char* pszItem )
{
	bool bBindAble = false;
	const char* pszBindAble = pKernel->GetConfigProperty(pszItem, FIELD_PROP_BIND_ABLE);
	if ( StringUtil::StringAsInt( pszBindAble ) != 0 )
	{
		bBindAble = true;
		const CfgItemBindLimit* pLimit = Configure<CfgItemBindLimit>::Find(pszItem);
		if (NULL != pLimit)
		{
			const char* pszColor = pKernel->GetConfigProperty(pszItem, FIELD_PROP_COLOR_LEVEL);
			int nColor = StringUtil::StringAsInt(pszColor);
			if (nColor < pLimit->GetColor())
			{
				bBindAble = false;
			}
		}
	}

	return bBindAble;
}

bool ToolItemModule::IsBindAble( IKernel* pKernel, const PERSISTID& item )
{
	bool bBindAble = false;
	int nBindAble = pKernel->QueryInt(item, FIELD_PROP_BIND_ABLE);
	if (nBindAble != 0)
	{
		bBindAble = true;
		const CfgItemBindLimit* pLimit = Configure<CfgItemBindLimit>::Find( pKernel->GetConfig(item) );
		if (NULL != pLimit)
		{
			int nColor = pKernel->QueryInt(item, FIELD_PROP_COLOR_LEVEL);
			if (nColor < pLimit->GetColor())
			{
				bBindAble = false;
			}
		}
	}

	return bBindAble;
}

bool ToolItemModule::IsBindAble(IKernel* pKernel, const char* pszItem, int nColor )
{
	bool bBindAble = false;
	const char* pszBindAble = pKernel->GetConfigProperty(pszItem, FIELD_PROP_BIND_ABLE);
	if (StringUtil::StringAsInt(pszBindAble) != 0)
	{
		bBindAble = true;
		const CfgItemBindLimit* pLimit = Configure<CfgItemBindLimit>::Find(pszItem);
		if (NULL != pLimit)
		{
			if (nColor < pLimit->GetColor())
			{
				bBindAble = false;
			}
		}
	}

	return bBindAble;
}

// �Ƿ�Ϊ��װ������Ʒ
bool ToolItemModule::IsCanWearItem(IKernel* pKernel, const char* strItemConfig)
{
	const char *script = pKernel->GetConfigProperty(strItemConfig, "Script");

	return strcmp(script, "Equipment") == 0 || strcmp(script, "Bianstone") == 0;
}