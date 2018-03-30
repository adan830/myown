//------------------------------------------------------------------
// �� �� ��:    ActionMutex.cpp
// ��    ��:    �����Ϊ����ģ��
// ˵    ��:
// �� �� ��:      
// ��������:    
//    :        
//-------------------------------------------------------------------

#include "ActionMutex.h"

#include "FsGame/CommonModule/LuaExtModule.h"
#include "FsGame/CommonModule/SwitchManagerModule.h"
#include "FsGame/Define/CommandDefine.h"
#include "FsGame/Define/GameDefine.h"
#include "utils/custom_func.h"

// �����Ϊ
inline int reset_action_mutex(void* state)
{
	// ��ú���ָ��
	IKernel* pKernel = LuaExtModule::GetKernel(state);

	// ����������
	CHECK_ARG_NUM(state, reset_action_mutex, 1);

	// ����������
	CHECK_ARG_OBJECT(state, reset_action_mutex, 1);

	// ��ȡ����
	PERSISTID player = pKernel->LuaToObject(state, 1);   
	IGameObj* pSelfObj = pKernel->GetGameObj(player);
	if (pSelfObj == NULL)
	{
		pKernel->LuaPushBool(state, false);
		return 0;
	}

	pKernel->LuaPushBool(state, pSelfObj->SetInt(FIELD_PROP_CUR_ACTION, ACTION_NONE));

	return 0;
}

// ��ʼ��
bool ActionMutexModule::Init( IKernel* pKernel )
{
	// �����Ϊ��Ϣ
	pKernel->AddIntCommandHook("player", COMMAND_RESET_ACTION_MUTEX, OnCommandCleanAction);

	DECL_LUA_EXT(reset_action_mutex);
	
	return true;
}

// �ر�
bool ActionMutexModule::Shut( IKernel* pKernel )
{
	return true;
}

/*!
* @brief	�Ƿ�������Ϊ
* @param int ��Ϊ����
* @return  true������Ϊ��false����Ϊ
*/
bool ActionMutexModule::IsHaveAction(IKernel * pKernel, const PERSISTID & self, bool show_sysinfo/* = true*/)
{
	IGameObj *pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return false;
	}
	// ��ǰ��Ϊ
	const int cur_action = pSelfObj->QueryInt(FIELD_PROP_CUR_ACTION);

	// ����Ϊ
	if (cur_action == ACTION_NONE)
	{
		return false;
	}

	// ϵͳ��ʾ��#{cur_action}�����У����ɽ�����������
	if (show_sysinfo)
	{
		// ƴ��key
		//char key[128] = { 0 };
		//SPRINTF_S(key, "%s_%d", STR_ACTION_MUTEX_SYSINFO_PROMPT, cur_action);

		int system_id = SYSTEM_INFO_ID_3001 - 1;
		system_id += cur_action;
		::CustomSysInfo(pKernel, self, system_id, CVarList());
	}

	return true;
}

/*!
* @brief	��¼��Ϊ
* @param int ��Ϊ����
*/
void ActionMutexModule::NoteAction(IKernel * pKernel, const PERSISTID & self, 
	const int action)
{
	IGameObj *pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return;
	}

	// ���Ϸ�
	if (!IsValidAction(action))
	{
		return;
	}

	pSelfObj->SetInt(FIELD_PROP_CUR_ACTION, action);
}

/*!
* @brief	�����Ϊ
*/
void ActionMutexModule::CleanAction(IKernel * pKernel, const PERSISTID & self)
{
	IGameObj *pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return;
	}

	if (pSelfObj->QueryInt(FIELD_PROP_CUR_ACTION) != ACTION_NONE)
	{
		pSelfObj->SetInt(FIELD_PROP_CUR_ACTION, ACTION_NONE);
	}
}

/*!
* @brief	��ǰ�Ƿ�Ϊָ���
* @param int ��Ϊ����
*/
bool ActionMutexModule::IsThisAction(IKernel * pKernel, const PERSISTID & self, 
	const int action)
{
	IGameObj *pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return false;
	}

	if (pSelfObj->GetClassType() != TYPE_PLAYER)
	{
		return false;
	}

	// ���Ϸ�
	if (!IsValidAction(action))
	{
		return false;
	}

	if (action == pSelfObj->QueryInt(FIELD_PROP_CUR_ACTION))
	{
		return true;
	}

	return false;
}

/*!
* @brief	��Ϊ�Ƿ�Ϸ�
* @param int ��Ϊ����
* @return  true�Ϸ���false���Ϸ�
*/
bool ActionMutexModule::IsValidAction(const int action)
{
	return (action > ACTION_NONE && action < ACTION_MAX);
}

// �����Ϊ��Ϣ
int ActionMutexModule::OnCommandCleanAction( IKernel * pKernel, const PERSISTID & self, 
	const PERSISTID & sender, const IVarList & var )
{
	// �����Ϊ
	CleanAction(pKernel, self);

	return 0;
}
