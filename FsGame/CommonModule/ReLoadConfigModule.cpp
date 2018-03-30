//--------------------------------------------------------------------
// �ļ���:		ReLoadConfigModule.h
// ��  ��:		���¼�������
// ˵  ��:		
// ��������:	2016��09��13��
// ��������:	2016��09��13��
// ������:		������  
//--------------------------------------------------------------------
#include "ReLoadConfigModule.h"
#include "FsGame\Define\CommandDefine.h"
#include "FsGame/CommonModule/LuaExtModule.h"
#include "server\KnlConst.h"
#include "FsGame\Define\PubDefine.h"
#include "FsGame\Define\SceneListDefine.h"
#include "utils\string_util.h"
#include "utils\extend_func.h"

ReLoadConfigModule* ReLoadConfigModule::m_pReLoadConfigModule = NULL;

//gm���������ļ�
int nx_reload_config(void* state)
{
	// ��ú���ָ��
	IKernel* pKernel = LuaExtModule::GetKernel(state);

	// ����������
	CHECK_ARG_NUM(state, nx_reload_config, 1);
	// ����������
	CHECK_ARG_STRING(state, nx_reload_config, 1);


	const char* strKey = pKernel->LuaToString(state, 1);

	pKernel->LuaPushBool(state, ReLoadConfigModule::m_pReLoadConfigModule->SendReloadConfigCommand(pKernel, strKey));
	return 1;
}

// ��ʼ��
bool ReLoadConfigModule::Init(IKernel* pKernel)
{
	m_pReLoadConfigModule = this;
	//gm���������ļ�
	pKernel->AddIntCommandHook("scene", COMMAND_MSG_REREAD_CFG_GM, ReLoadConfigModule::OnCommandRereadLoadResource);

	DECL_LUA_EXT(nx_reload_config);

	m_mapReloadConfig.clear();
	return true;
}

// �ͷ�
bool ReLoadConfigModule::Shut(IKernel* pKernel)
{
	return true;
}

// ע���������ûص�
void ReLoadConfigModule::RegisterReloadConfig(const char* strKey, RELOAD_CONFIG_CALLBACK pReloadCall)
{
	if (NULL == strKey || StringUtil::CharIsNull(strKey) || NULL == pReloadCall)
	{
		return;
	}
	ReloadConfigCallMap::iterator iter = m_mapReloadConfig.find(strKey);
	if (iter == m_mapReloadConfig.end())
	{
		m_mapReloadConfig.insert(std::make_pair(strKey, pReloadCall));
	}
}

// ����������������
bool ReLoadConfigModule::SendReloadConfigCommand(IKernel* pKernel, const char* strKey)
{
	if (StringUtil::CharIsNull(strKey))
	{
		return false;
	}
	// �����ռ�
	IPubSpace* pPubSpace = pKernel->GetPubSpace(PUBSPACE_DOMAIN);
	if (pPubSpace == NULL)
	{
		return false;
	}

	// ��������
	wchar_t pubDataName[256];
	SWPRINTF_S(pubDataName, L"Domain_SceneList%d", pKernel->GetServerId());
	IPubData* pPubData = pPubSpace->GetPubData(pubDataName);
	if (pPubData == NULL)
	{
		return false;
	}

	// �ϼܱ�
	IRecord* pRecord = pPubData->GetRecord(SERVER_MEMBER_FIRST_LOAD_SCENE_REC);
	if (pRecord == NULL)
	{
		return false;
	}

	CVarList msg;
	msg << COMMAND_MSG_REREAD_CFG_GM
		<< strKey;

	int rows = pRecord->GetRows();
	LoopBeginCheck(a);
	for (int i = 0; i < rows; ++i)
	{
		LoopDoCheck(a);
		pKernel->CommandToScene(pRecord->QueryInt(i, SERVER_MEMBER_FIRST_LOAD_SCENE_COL_scene_id), msg);
	}

	return true;
}

// ���������ļ�
int ReLoadConfigModule::OnCommandRereadLoadResource(IKernel *pKernel, const PERSISTID &self, const PERSISTID &sender, const IVarList &args)
{
	const char* strKey = args.StringVal(1);
	ReloadConfigCallMap::iterator iter = m_pReLoadConfigModule->m_mapReloadConfig.find(strKey);
	if (iter == m_pReLoadConfigModule->m_mapReloadConfig.end())
	{
		return 0;
	}

	RELOAD_CONFIG_CALLBACK pReloadCall = iter->second;
	if (NULL == pReloadCall)
	{
		return 0;
	}

	pReloadCall(pKernel);

	extend_warning(LOG_WARNING, "RereadLoadResource %s success", strKey);
	return 0;
}