//----------------------------------------------------------
// �ļ���:      LookModule.cpp
// ��  ��:      �鿴��Ϣ
// ˵  ��:
// ��������:
// ������:             
//----------------------------------------------------------
#include "LookModule.h"
#include "utils/util_func.h"
#include "utils/extend_func.h"
#include "utils/custom_func.h"
#include "FsGame/Define/ClientCustomDefine.h"
#include "FsGame/Define/ServerCustomDefine.h"
#include "FsGame/Define/CommandDefine.h"
#include "FsGame/Define/ContainerDefine.h"
#include "FsGame/Define/PubDefine.h"
#include "FsGame/CommonModule/ContainerModule.h"
#include "LookDefine.h"



LookModule * LookModule::m_pLookModule = NULL;
ContainerModule* LookModule::m_pContainerModule = NULL;


//��ʼ��
bool LookModule::Init(IKernel* pKernel)
{
	m_pLookModule = this;

	m_pContainerModule = (ContainerModule*)pKernel->GetLogicModule("ContainerModule");

	Assert(m_pContainerModule !=NULL);

	pKernel->AddIntCustomHook("player", CLIENT_CUSTOMMSG_LOOK_CHAT_ITEM, OnCustomLookChatItem);

	//pKernel->AddIntCustomHook("player", CLIENT_CUSTOMMSG_LOOK_EQUIPS, OnCustomLookEquips);
    //pKernel->AddIntCustomHook("player", CLIENT_CUSTOMMSG_LOOK_ITEM, OnCustomLookItem);   
	//pKernel->AddIntCommandHook("player", COMMAND_BELOOK_EQUIPS, OnCommandBeLookEquips);
	//pKernel->AddIntCommandHook("player", COMMAND_LOOK_EQUIPS_ECHO, OnCommandLookEquipsEcho);
	//pKernel->AddIntCommandHook("player", COMMAND_BELOOK_ITEM, OnCommandBeLookItem);
	//pKernel->AddIntCommandHook("player", COMMAND_LOOK_ITEM_ECHO, OnCommandLookItemEcho);


	return true;
}

bool LookModule::Shut(IKernel* pKernel)
{
	return true;
}

//�鿴������ɫ�ı����е�ĳ����Ʒ
int LookModule::OnCustomLookItem(IKernel * pKernel, const PERSISTID & self, 
								 const PERSISTID & sender, const IVarList & args)
{
	const wchar_t * destname = args.WideStrVal(1);
	const char * uniqueid = args.StringVal(2);
	
	m_pLookModule->LookItem(pKernel, self, destname, uniqueid);

	return 0;
}

//�鿴������ɫչʾ��ĳ����Ʒ
int LookModule::OnCustomLookChatItem(IKernel * pKernel, const PERSISTID & self, 
                                        const PERSISTID & sender, const IVarList & args)
{
    // args �ĸ�ʽ: 
    //     [msg_id][target_user][item_unique_id]
    
    const wchar_t *target_user = args.WideStrVal(1);
    const char *item_unique_id = args.StringVal(2);
    
    bool look_ret = LookRoleShowoffItem(pKernel, self, target_user, item_unique_id);
    
    if (!look_ret)
    {
        //�鿴[{@0:����}]����Ʒʧ�ܣ�������Ʒ��Ϣʧ��
		CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_19202, CVarList() << target_user);
        
        return 0;
    }
    
    return 0;
}

//��������ɫ�鿴ĳ����Ʒ
int LookModule::OnCommandBeLookItem(IKernel * pKernel, const PERSISTID & self,
									const PERSISTID & sender, const IVarList & args)
{
	const wchar_t * srcname = args.WideStrVal(1);
	const char * identifyid = args.StringVal(2);
	m_pLookModule->BeLookItem(pKernel, self, srcname, identifyid);

	return 0;
}

//�鿴������ɫ��ĳ����Ʒ�Ļظ�
int LookModule::OnCommandLookItemEcho(IKernel * pKernel, const PERSISTID & self,
									  const PERSISTID & sender, const IVarList & args)
{
	const wchar_t * destname = args.WideStrVal(1);
	const char * info = args.StringVal(2);
	int type = args.IntVal(3);
	const char* uniqueid = args.StringVal(4);
	m_pLookModule->LookItemEcho(pKernel, self, destname, info, type,uniqueid);

	return 0;
}



//�鿴������ɫ��װ��
void LookModule::LookEquips(IKernel * pKernel, const PERSISTID & self, const wchar_t * destname)
{
	if (destname == NULL || wcslen(destname) == 0)
	{
		return;
	}

	const wchar_t * self_name = pKernel->QueryWideStr(self, "Name");
	if (wcscmp(self_name,destname) == 0)
	{
		//��Ҫ�鿴�Լ���װ��
		return;
	}

	//��������
	//CVarList msg;
	//msg << COMMAND_BELOOK_EQUIPS << self_name;
	//if (!pKernel->CommandByName(destname, msg))
	//{
	//	//[{@0:����}]���������ߣ��鿴װ��ʧ��
	//	::CustomSysInfo(pKernel, self, TIPSTYPE_SYSFUNCTION_PROMPT_MESSAGE, LOOK_ROLE_OFFLINE, CVarList() << destname);
	//}
}

//��������ɫ�鿴װ��
void LookModule::BeLookEquips(IKernel * pKernel, const PERSISTID & self, const wchar_t * srcname)
{
	//CVarList msg;
	//msg << COMMAND_LOOK_EQUIPS_ECHO << pKernel->QueryWideStr(self, "Name");

	//fast_string szInfo = "";

	////����ɫ����������׼�����ַ�����
	//if (::player_info_to_string(pKernel, self, szInfo))
	//{
	//	msg << szInfo.c_str();
	//}

	////����ɫ����������Էŵ��ַ�����
	//if (::player_prop_to_string(pKernel, self, szInfo))
	//{
	//	msg << szInfo.c_str();
	//}

	////��װ������Ϣ׼�����ַ�����
	//PERSISTID equip_box = pKernel->GetChild(self, EQUIP_BOX_NAME_WSTR);
	//if (!pKernel->Exists(equip_box))
	//{
	//	return;
	//}

	//IRecord * pStrengthenRec = pKernel->GetRecord(self, EQUIP_STRHENTHEN_REC);
	//if (NULL == pStrengthenRec)
	//{
	//	return;
	//}

	//int nStrLevel = 0;
	//int nStrLevelRow = 0;
	//fast_string strEquipAdd = "";

 //   LoopBeginCheck(a)
	//for (int i = EQUIP_POS_WEAPON; i < EQUIP_MAX_PLACEHOLDER; ++i)
	//{
	//    LoopDoCheck(a)
	//    
	//	nStrLevel = 0;
	//	strEquipAdd = "";

	//	nStrLevelRow = pStrengthenRec->FindInt(EQUIP_STRHENTHEN_INDEX, i);
	//	if (nStrLevelRow >= 0)
	//	{
	//		//ǿ���ȼ�
	//		nStrLevel = pStrengthenRec->QueryInt(nStrLevelRow, EQUIP_STRHENTHEN_LEVEL);

	//		strEquipAdd += "StrLevel,";
	//		strEquipAdd += util_int64_as_string(nStrLevel);
	//		strEquipAdd += ";";

	//		//��ʯ
	//		LoopBeginCheck(c)
	//		for (int m = EQUIP_STRHENTHEN_JEWEL1; m <= EQUIP_STRHENTHEN_JEWEL4; ++m)
	//		{
	//		    LoopDoCheck(c)
	//		    
	//			strEquipAdd += "Jewel";
	//			strEquipAdd += util_int64_as_string(m-EQUIP_STRHENTHEN_JEWEL1+1).c_str();
	//			strEquipAdd += ",";
	//			strEquipAdd += pStrengthenRec->QueryString(nStrLevelRow, m);
	//			strEquipAdd += ";";
	//		}

	//		//���
	//		strEquipAdd += "Soul1,";
	//		strEquipAdd += pStrengthenRec->QueryString(nStrLevelRow, EQUIP_STRHENTHEN_SOUL);
	//		strEquipAdd += ";";
	//	}

	//	PERSISTID obj = pKernel->GetItem(equip_box, i);
	//	if (pKernel->Exists(obj) && ::player_equip_to_string(pKernel, obj, szInfo))
	//	{
	//		szInfo += strEquipAdd;
	//		msg << szInfo.c_str();
	//	}
	//	else
	//	{
	//		msg << "";
	//	}
	//}

	////��������
	//pKernel->CommandByName(srcname, msg);
}

//�鿴������ɫ��װ���ظ�
void LookModule::LookEquipsEcho(IKernel * pKernel, const PERSISTID & self,
								const wchar_t * destname, const IVarList & infos)
{
	/*CVarList msg;
	msg << SERVER_CUSTOMMSG_LOOK_EQUIPS_ECHO << destname << infos;
	pKernel->Custom(self, msg);*/
} 


//�鿴������ɫ�ı����е�ĳ����Ʒ
void LookModule::LookItem(IKernel * pKernel, const PERSISTID & self,
						  const wchar_t * destname, const char * uniqueid)
{
	//if (destname == NULL || wcslen(destname) == 0)
	//{
	//	return;
	//}

	//std::wstring self_name = pKernel->QueryWideStr(self, "Name");

	////��������
	//CVarList msg;
	//msg << COMMAND_BELOOK_ITEM << self_name.c_str() << uniqueid;
	//if (!pKernel->CommandByName(destname, msg))
	//{
	//	//[{@0:����}]���������ߣ��鿴��Ʒʧ��
	//	::CustomSysInfo(pKernel, self, TIPSTYPE_SYSFUNCTION_PROMPT_MESSAGE, LOOK_ROLE_OFFLINE, CVarList() << destname);
	//}
}

//��������ɫ�鿴�����е�ĳ����Ʒ
void LookModule::BeLookItem(IKernel * pKernel, const PERSISTID & self,
							const wchar_t * srcname, const char * uniqueid)
{
	//const wchar_t * selfname = pKernel->QueryWideStr(self, "Name");

	////Ѱ��ָ��UniqueID�ŵ���Ʒ

	//// 	���ڱ�����Ѱ��ָ��װ��

	//PERSISTID toolbox = pKernel->GetChild(self, ITEM_BOX_NAME_WSTR);
	//PERSISTID item = m_pContainerModule->FindItemByUid(pKernel, toolbox, uniqueid);

 //	if (!pKernel->Exists(item))
 //	{
 //		//δ�ҵ�����װ�����в�ѯ
 //		PERSISTID equip_box = pKernel->GetChild(self, EQUIP_BOX_NAME_WSTR);
	//	item = ((ContainerModule*)m_pContainerModule)->FindItemByUid(pKernel, equip_box, uniqueid);
 //	}

	//if (!pKernel->Exists(item))
	//{
	//	//�鿴[{@0:����}]����Ʒʧ�ܣ�ָ������Ʒ�Ѳ��ڶԷ��ı�����
	//	CustomSysInfoByName(pKernel, srcname, TIPSTYPE_SYSFUNCTION_PROMPT_MESSAGE, LOOK_ROLE_NOT_IN_TOOLBOX, CVarList() << selfname);
	//	return;
	//}

	//fast_string szInfo;
	//if (!(::player_item_to_string(pKernel, item, szInfo)))
	//{
	//	//�鿴[{@0:����}]����Ʒʧ�ܣ�������Ʒ��Ϣʧ��
	//	CustomSysInfoByName(pKernel, srcname, TIPSTYPE_SYSFUNCTION_PROMPT_MESSAGE, LOOK_ROLE_ITEM_INFO_FAILED, CVarList() << selfname);
	//	return;
	//}

	////��������
	//CVarList msg;
	//msg << COMMAND_LOOK_ITEM_ECHO << selfname;
	//msg << szInfo.c_str();
	//msg << 0;
	//msg << uniqueid;


	//pKernel->CommandByName(srcname, msg);
}

//�鿴������ɫ�ı����е�ĳ����Ʒ�Ļظ�
void LookModule::LookItemEcho(IKernel * pKernel, const PERSISTID & self,
							  const wchar_t * destname, const char * info, int type, const char* uniqueid)
{/*
	CVarList msg;
	if (type == 0)
	{
		msg << SERVER_CUSTOMMSG_LOOK_ITEM_ECHO << destname << info;
    }

	pKernel->Custom(self, msg);*/
}

//�鿴������ɫ��װ��
int LookModule::OnCustomLookEquips(IKernel * pKernel, const PERSISTID & self, 
								   const PERSISTID & sender, const IVarList & args)
{
	/*const wchar_t * destname = args.WideStrVal(1);
	m_pLookModule->LookEquips(pKernel, self, destname);*/

	return 0;
}

//����ص�
//��������ɫ�鿴װ��
int LookModule::OnCommandBeLookEquips(IKernel * pKernel, const PERSISTID & self,
									  const PERSISTID & sender, const IVarList & args)
{
	/*const wchar_t * srcname = args.WideStrVal(1);
	m_pLookModule->BeLookEquips(pKernel, self, srcname);*/

	return 0;
}
//�鿴������ɫ��װ���ظ�
int LookModule::OnCommandLookEquipsEcho(IKernel * pKernel, const PERSISTID & self,
										const PERSISTID & sender, const IVarList & args)
{
	/*const wchar_t * destname = args.WideStrVal(1);

	CVarList infos;
	infos.Append(args, 2, args.GetCount() - 2);

	m_pLookModule->LookEquipsEcho(pKernel, self, destname, infos);*/

	return 0;
}


// �鿴�������չʾ����Ʒ
bool LookModule::LookRoleShowoffItem(IKernel * pKernel, const PERSISTID & self, const wchar_t * destname,
    const char * item_unique_id)
{
    IRecord *showoff_rec = GetShowoffItemRecord(pKernel, self);   
    if (showoff_rec == NULL)
    {
        return false;
    }

    // ������ƷUID���Ҽ�¼
    int row_no = showoff_rec->FindString(SHOWOFF_ITEM_PUB_RECORD_UID_COL, item_unique_id);
    if (row_no == -1)
    {
        return false;
    }
    
    const char *item_prop = showoff_rec->QueryString(row_no, SHOWOFF_ITEM_PUB_RECORD_PROP_COL);

    CVarList msg;
    msg << SERVER_CUSTOMMSG_SHOWOFF_ITEM << destname << item_prop;

    pKernel->Custom(self, msg);

    return true;
}

// ��ȡ��Ʒչʾ��¼��
IRecord* LookModule::GetShowoffItemRecord(IKernel * pKernel, const PERSISTID & self)
{
    // ��ȡ�������������
    if (!pKernel->FindPubSpace(PUBSPACE_DOMAIN))
    {
        return NULL;
    }

    IPubSpace * pub_space = pKernel->GetPubSpace(PUBSPACE_DOMAIN);
    if (pub_space == NULL)
    {
        return NULL;
    }

    wchar_t domain_name[256] = {0};
    SWPRINTF_S(domain_name, DOMAIN_LOOK, pKernel->GetServerId());

    IPubData * pub_data = pub_space->GetPubData(domain_name);
    if (pub_data == NULL)
    {
        return NULL;
    }

    // չʾ��¼��
    IRecord * item_rec = pub_data->GetRecord(SHOWOFF_ITEM_PUB_RECORD_NAME);

    return item_rec;
}