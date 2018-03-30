//--------------------------------------------------------------------
// �ļ���:      GroupSceneBase.cpp
// ��  ��:      ���鳡��������
// ˵  ��:
// ��������:    2015��8��4��
// ������:       
// �޸���:        
//    :       
//--------------------------------------------------------------------
#include "FsGame/SceneBaseModule/GroupScene/GroupSceneBase.h"
#include "FsGame/CommonModule/LuaExtModule.h"
#include "FsGame/Define/GroupSceneDefine.h"
#include "FsGame/Define/LogDefine.h"
#include "Fsgame/Define/GameDefine.h"

#include "utils/string_util.h"
#include "utils/XmlFile.h"
#include "utils/extend_func.h"
#include "utils/util_func.h"
#include "utils/util_config.h"
#include <list>

GroupSceneBase* GroupSceneBase::m_pGroupSceneBase = NULL;

#define SECRET_SCENE_LEVEL "ini/Scene/secretscene/npc_scene.xml"

//���¶�ȡ��Դ
int nx_reload_group_scene_resource(void *state)
{
    IKernel *pKernel = LuaExtModule::GetKernel(state);

    GroupSceneBase::m_pGroupSceneBase->LoadResource(pKernel);
    return 1;
}

//��ʼ��
bool GroupSceneBase::Init(IKernel* pKernel)
{
    m_pGroupSceneBase = (GroupSceneBase*)pKernel->GetLogicModule("GroupSceneBase");
    Assert(m_pGroupSceneBase != NULL);
    LoadResource(pKernel);

    DECL_LUA_EXT(nx_reload_group_scene_resource);
    return true;
}
//�ر�
bool GroupSceneBase::Shut(IKernel* pKernel)
{
	Configure<CfgSecretInfo>::Release();
    return true;
}

//��������
bool GroupSceneBase::LoadResource(IKernel* pKernel)
{
	Configure<CfgSecretInfo>::Release();
	std::string xmlPath = pKernel->GetResourcePath();
    xmlPath += SECRET_SCENE_LEVEL;

	return Configure<CfgSecretInfo>::LoadXml(xmlPath.c_str(), OnConfigError );
}

//��ó�������
int GroupSceneBase::GetSceneType(IKernel* pKernel, int sceneID)
{
    const char* name = pKernel->GetSceneScript(sceneID);
	if ( NULL == name )
	{
		return GROUP_SCENE_NOT_SECRET;
	}

    if (strcmp(name, "SecretScene") == 0)
    {
        return GROUP_SCENE_SECRET_COMMON;
    }
    else if (strcmp(name, "NationSecretScene") == 0)
    {
        return GROUP_SCENE_SECRET_TREASURE;
    }
    else if (strcmp(name, "TeamScene") == 0 || strcmp(name, "SingleScene") == 0)
    {
        return GROUP_SCENE_SECRET_TEAM;
    }
    else if (strcmp(name, "MoneySecretScene") == 0)
    {
        return GROUP_SCENE_SECRET_MONEY;
    }
    else if (strcmp(name, "ItemSecretScene") == 0)
    {
        return GROUP_SCENE_SECRET_ITEM;
    }
    else if (strcmp(name, "EliteSecretScene") == 0)
    {
        return GROUP_SCENE_SECRET_ELITE;
    }else if(strcmp(name,"GuardSecretScene") == 0)
	{
		return GROUP_SCENE_SECRET_GUARD;
	}else if(strcmp(name,"SneakOnSecretScene") == 0)
	{		
		return GROUP_SCENE_SECRET_SNEAK_ON;
	}
	else if (strcmp(name,"SwordsmanSecretScene") == 0){
		return GROUP_SCENE_SECRET_SWARDSMAN;
	}
	else if (strcmp(name, "MeridianSecretScene") == 0){
		return GROUP_SCENE_SECRET_MERIDIAN;
	}

	return GROUP_SCENE_NOT_SECRET;
}
//����GroupID
bool GroupSceneBase::SetGroupID(IKernel* pKernel,const PERSISTID& self,const int groupID)
{
    IGameObj* pSelfObj = pKernel->GetGameObj(self);
    if (pSelfObj == NULL)
    {
        return false;
    }
    if (pSelfObj->FindAttr("LastObject"))
    {
        pSelfObj->SetObject("LastObject", PERSISTID());
    }
    pSelfObj->SetInt("GroupID", groupID);

    return true;
}

//���������Ϣ
// CfgSecretInfo* GroupSceneBase::GetResInfo(int dwID)
// {
// 	return Configure<CfgSecretInfo>::Find((size_t)dwID);
// }

//�������NPC����
void GroupSceneBase::GetTrapConfig(int sceneID, std::string& trapNpcConfig, int& trapAmount)
{
   /* CfgSecretInfo* info = GetResInfo(sceneID);
    if (NULL == info)
    {
        return;
    }
	const std::vector<std::string>& trapList = info->GetTrapNpcConfig();
    if (trapList.size() > 0)
    {
		int random = util_random_int((int)trapList.size());
		trapNpcConfig = trapList[random];
		trapAmount = info->GetTrapAmount();
	}*/
}