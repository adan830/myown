//---------------------------------------------------------
//�ļ���:       MapModule.h
//��  ��:       ��ͼ��ز���ģ��
//˵  ��:       
//				
//��������:      2015��4��21��
//������:         
//�޸���:
//   :         
//---------------------------------------------------------

#include "MapModule.h"
#include "FsGame/Define/MapDefine.h"
#include "utils/extend_func.h"
#include "FsGame/SystemFunctionModule/CapitalModule.h"
#include "FsGame/CommonModule/AsynCtrlModule.h"
#include "FsGame/SystemFunctionModule/ToolBoxModule.h"
#include "FsGame/Define/ClientCustomDefine.h"
#include "FsGame/Define/ServerCustomDefine.h"
#include "FsGame/Define/LogDefine.h"
#include "utils/XmlFile.h"
#include "utils/string_util.h"
#include "utils/custom_func.h"
#include <algorithm>
#include "FsGame/CommonModule/LuaExtModule.h"
#include "CommonModule/EnvirValueModule.h"
#include "CommonModule/ReLoadConfigModule.h"

#ifdef _DEBUG
// ����
int MapModule::nx_map_switch_scene(void *state)
{
	IKernel *pKernel = LuaExtModule::GetKernel(state);
	// ����������
	CHECK_ARG_NUM(state, nx_map_switch_scene, 2);

	// ���ÿ����������
	CHECK_ARG_OBJECT(state, nx_map_switch_scene, 1);
	CHECK_ARG_INT(state, nx_map_switch_scene, 2);
	PERSISTID player = pKernel->LuaToObject(state, 1);
	int sceneId = pKernel->LuaToInt(state, 2);

	CVarList var;
	var << CLIENT_CUSTOMMSG_MAP
		<< CS_REQUEST_MAP_SWITCH_SCENE
		<< sceneId;
	m_pMapModule->OnCustomCallBack(pKernel, player, player, var);

	pKernel->LuaPushBool(state, true);
	return 1;
}
#endif

// ��ʼ����ָ̬��
MapModule* MapModule::m_pMapModule = NULL;
AsynCtrlModule* MapModule::m_pAsynCtrlModule = NULL;

// ��ʼ��
bool MapModule::Init(IKernel* pKernel)
{
	// ��������
	m_pMapModule = this;
	m_pAsynCtrlModule = dynamic_cast<AsynCtrlModule*>(pKernel->GetLogicModule("AsynCtrlModule"));

	assert(m_pMapModule != NULL 
		&& m_pAsynCtrlModule != NULL);

	// ע����볡���ص�
	pKernel->AddEventCallback("player", "OnAfterEntryScene", MapModule::OnPlayerEntry);
	pKernel->AddIntCustomHook("player", CLIENT_CUSTOMMSG_MAP, MapModule::OnCustomCallBack);

	// ������Դ
	LoadResource(pKernel);
#ifdef _DEBUG
	// ������
	DECL_LUA_EXT(nx_map_switch_scene);
#endif

	RELOAD_CONFIG_REG("MapConfig", MapModule::ReLoadConfig);
	return true;
}

// ����
bool MapModule::Shut(IKernel* pKernel)
{
	return true;
}

bool MapModule::LoadResource(IKernel* pKernel)
{
	return true;
}

// ���볡��
int MapModule::OnPlayerEntry(IKernel * pKernel, const PERSISTID & self,
									 const PERSISTID & sender, const IVarList & args)
{
	return 0;
}

// ���տͻ�����Ϣ
int MapModule::OnCustomCallBack(IKernel * pKernel, const PERSISTID & self,
										const PERSISTID & sender, const IVarList & args)
{
	// ��֤�����Ƿ����
	if (!pKernel->Exists(self))
	{
		return 0;
	}

	// ������Ϣ����
	if (args.GetCount() < 2)
	{
		return 0;
	}

	// ȡ��Ϣid
	int subMsgId = args.IntVal(1);
	switch (subMsgId)
	{
	case CS_REQUEST_MAP_SWITCH_SCENE:
		m_pMapModule->OnCustomSwitchScene(pKernel, self, sender, args);
		break;
	}

	return 0;
}

// �����л�����
int MapModule::OnCustomSwitchScene(IKernel * pKernel, const PERSISTID & self,
										const PERSISTID & sender, const IVarList & args)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return 0;
	}

	// ��֤��������
	if (args.GetCount() != 3)
	{
		return 0;
	}


	// ȡ����Id
	int sceneId = args.IntVal(2);

	// ��֤�����Ƿ����
	if (!pKernel->GetSceneExists(sceneId))
	{
		extend_warning(LOG_ERROR, "[%s][%d] scene not exist %d", __FUNCTION__, __LINE__, sceneId);
		return 0;
	}

	// ս��״̬�Ƿ�����л�����
	int fightState = pSelfObj->QueryInt("FightState");
	if (fightState > 0)
	{
		return 0;
	}

// 	int curSceneID = pKernel->GetSceneId();
// 	if (curSceneID > OUTDOOR_SCENE_MAX || sceneId > OUTDOOR_SCENE_MAX)
// 	{
// 		return 1;
// 	} 



	// �г���
	if (!m_pAsynCtrlModule->SwitchBorn(pKernel, self, sceneId))
	{
		// ��ʾ�ͻ��� �г���ʧ��
	//	::CustomSysInfo(pKernel, self, SYS_INFO_MAP_STRING8, args);
		return 0;
	}
	
	return 1;
}

void MapModule::ReLoadConfig(IKernel * pKernel)
{
	m_pMapModule->LoadResource(pKernel);
}