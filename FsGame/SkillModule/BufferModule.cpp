//--------------------------------------------------------------------
// �ļ���:      Server\FsRoomLogic\skill_module\BufferModule.cpp
// ��  ��:      Bufferϵͳ
// ˵  ��:
// ��������:    2014��11��4��
// ������:		 
//    :       
//--------------------------------------------------------------------

#include "BufferModule.h"
#include "FsGame/Define/CommandDefine.h"
#include "FsGame/Define/FightPropertyDefine.h"
#include "FsGame/SkillModule/impl/SkillDataQueryModule.h"
#include "FsGame/Interface/PropertyInterface.h"
#include "FsGame/CommonModule/LuaExtModule.h"
#include "FsGame/Define/ViewDefine.h"
//#include "BufferDecayLogic.h"
#include "../Define/GameDefine.h"
#include "BufferModifyPack.h"

BufferModule* BufferModule::m_pInstance = NULL;

// ԭ�ͣ�int nx_add_buffer(object self, object sender, object buffer)
// ���ܣ����Buffer
int nx_add_buffer(void* state)
{
	IKernel* pKernel = LuaExtModule::GetKernel(state);

	// ����������
	CHECK_ARG_NUM(state, nx_add_buffer, 3);
	// ����������
	CHECK_ARG_OBJECT(state, nx_add_buffer, 1);
	CHECK_ARG_OBJECT(state, nx_add_buffer, 2);
	CHECK_ARG_STRING(state, nx_add_buffer, 3);

	// ��ȡ����
	PERSISTID self = pKernel->LuaToObject(state, 1);
	PERSISTID sender = pKernel->LuaToObject(state, 2);
	const char* buffid = pKernel->LuaToString(state, 3);

	if (BufferModule::m_pInstance != NULL)
	{
		pKernel->LuaPushBool(state, BufferModule::m_pInstance->AddBuffer(pKernel, self, sender, buffid));
	}
	else
	{
		pKernel->LuaPushBool(state, false);
	}

	return 1;
}

// ԭ�ͣ�int nx_remove_buffer(object self, object buffer)/
//     ��int nx_remove_buffer(object self, object sender, string szbufferid)
// ���ܣ�ɾ��Buffer
int nx_remove_buffer(void* state)
{
	IKernel* pKernel = LuaExtModule::GetKernel(state);

	// ����������
	int arg_num = pKernel->LuaGetArgCount(state);
	if (arg_num != 2 && arg_num != 3)
	{
		pKernel->LuaErrorHandler(state, "nx_remove_buffer argument number error");
		return 0;
	}

	if (BufferModule::m_pInstance == NULL)
	{
		pKernel->LuaPushBool(state, false);
		return 1;
	}

	// ����������
	CHECK_ARG_OBJECT(state, nx_remove_buffer, 1);
	CHECK_ARG_OBJECT(state, nx_remove_buffer, 2);
	CHECK_ARG_STRING(state, nx_remove_buffer, 3);
	// ��ȡ����
	PERSISTID self = pKernel->LuaToObject(state, 1);
	PERSISTID sender = pKernel->LuaToObject(state, 2);
	const char* buffer_id = pKernel->LuaToString(state, 3);
	if (BufferModule::m_pInstance != NULL)
	{
		pKernel->LuaPushBool(state, BufferModule::m_pInstance->RemoveBuffer(pKernel, self, buffer_id));
	}
	else
	{
		pKernel->LuaPushBool(state, false);
	}

	return 1;
}

//��ʼ��
bool BufferModule::Init(IKernel* pKernel)
{
    m_pInstance = this;

    // ע�᱾ģ����Ҫ����Ļص�����
	pKernel->AddEventCallback("NormalNPC", "OnLoad", BufferModule::OnLoad);
	pKernel->AddEventCallback("player", "OnRecover", BufferModule::OnLoad);
	pKernel->AddEventCallback("Buffer", "OnLoad", BufferModule::OnBufferLoad);
	pKernel->AddEventCallback("player", "OnStore", BufferModule::OnStore);
	pKernel->AddEventCallback("player", "OnReady", BufferModule::OnReady);
	pKernel->AddEventCallback("player", "OnContinue", BufferModule::OnReady);
	// �������buff�������
	pKernel->AddEventCallback("player", "OnLeaveScene", BufferModule::OnLeaveScene, -1);

	pKernel->AddEventCallback("BufferContainer", "OnCreate", BufferModule::OnBufferContainerCreate);
	pKernel->AddEventCallback("Buffer", "OnRecover", BufferModule::OnBufferOnRecover);

	pKernel->AddIntCommandHook("player", COMMAND_BEKILL, BufferModule::OnCommandBeKill);
//	pKernel->AddIntCommandHook("BattleNpc", COMMAND_BEKILL, BufferModule::OnCommandBeKill);

	pKernel->AddIntCommandHook("player", COMMAND_BEDAMAGE, BufferModule::OnCommandEffectBuffer);
	pKernel->AddIntCommandHook("player", COMMAND_SKILL_BEFORE_HIT, BufferModule::OnCommandEffectBuffer);
	pKernel->AddIntCommandHook("player", COMMAND_BEVA, BufferModule::OnCommandEffectBuffer);
	pKernel->AddIntCommandHook("player", COMMAND_DAMAGETARGET, BufferModule::OnCommandEffectBuffer);

	pKernel->AddIntCommandHook("NormalNPC", COMMAND_BEDAMAGE, BufferModule::OnCommandEffectBuffer);
	pKernel->AddIntCommandHook("NormalNPC", COMMAND_SKILL_BEFORE_HIT, BufferModule::OnCommandEffectBuffer);
	pKernel->AddIntCommandHook("NormalNPC", COMMAND_BEVA, BufferModule::OnCommandEffectBuffer);
	pKernel->AddIntCommandHook("NormalNPC", COMMAND_DAMAGETARGET, BufferModule::OnCommandEffectBuffer);

    DECL_HEARTBEAT(BufferModule::H_BufferUpdate);

	DECL_LUA_EXT(nx_add_buffer);
	DECL_LUA_EXT(nx_remove_buffer);

    // ����Ԥ����BUff����
    PreloadBuffConfig(pKernel);

	// buff˥��ģ���ʼ��
// 	if(!BufferDecayLogicSingleton::Instance()->Init(pKernel))
// 	{
// 		return false;
// 	}

	// buff���԰�
	if(!BufferModifyPackSingleton::Instance()->Init(pKernel))
	{
		return false;
	}
    return true;
}

//�ͷ�
bool BufferModule::Shut(IKernel* pKernel)
{
    return true;
}

//����ĳ��ID��BUFFER
bool BufferModule::FindBuffer(IKernel* pKernel,
                              const PERSISTID& self,
                              const char* szBufferID,
                              IVarList& result)
{
	if (!pKernel->Exists(self) || NULL == szBufferID || StringUtil::CharIsNull(szBufferID))
    {
        return false;
    }

    PERSISTID container = GetBufferContainer(pKernel, self);
    if (!pKernel->Exists(container))
    {
        ::extend_warning(LOG_WARNING,
                         "[BufferModule::FindBuffer]: the role %d-%d BufferContainer not exists.",
                         self.nIdent, self.nSerial);
        return false;
    }

    unsigned int it;
    PERSISTID buffer = pKernel->GetFirst(container, it);
	LoopBeginCheck(d)
    while (pKernel->Exists(buffer))
    {
		LoopDoCheck(d)
        if (strcmp(pKernel->GetConfig(buffer), szBufferID) == 0)
        {
            result << buffer;
        }

        buffer = pKernel->GetNext(container, it);
    }

    return result.GetCount() > 0;
}

//���BUFFER����
PERSISTID BufferModule::GetBufferContainer(IKernel* pKernel, const PERSISTID& self)
{
	if (!pKernel->Exists(self))
	{
		return PERSISTID();
	}
	return pKernel->GetChild(self, wszBufferContainerName);
}

//����ID���BUFFER
bool BufferModule::AddBuffer(IKernel* pKernel,
                             const PERSISTID& self,
                             const PERSISTID& sender,
                             const char* szBufferConfigID)
{
	if (!pKernel->Exists(self) || NULL == szBufferConfigID || StringUtil::CharIsNull(szBufferConfigID))
    {
        return false;
    }

    // ����BUFF
    PERSISTID buffer = pKernel->CreateFromConfig(pKernel->GetScene(), "", szBufferConfigID);
    if (!pKernel->Exists(buffer))
    {
        return false; 
    }

	bool res = InterAddBuffer(pKernel, self, sender, buffer);
    if (!res)
    {
        pKernel->Destroy(pKernel->GetScene(), buffer);
    }

    return res;
}

//ɾ��BUFFER
bool BufferModule::RemoveBuffer(IKernel* pKernel, const PERSISTID& self, const char* szBufferID)
{
    if (!pKernel->Exists(self))
    {
        return false;
    }

    if (StringUtil::CharIsNull(szBufferID))
    {
        return false;
    }

    int iRemoveCount = 0;
    CVarList bufferList;
    if (!m_pInstance->FindBuffer(pKernel, self, szBufferID, bufferList))
    {
        return false;
    }

    size_t count = bufferList.GetCount();
	LoopBeginCheck(a)
    for (size_t t=0; t<count; ++t)
    {
		LoopDoCheck(a)
        PERSISTID buffer = bufferList.ObjectVal(t);
        if (pKernel->Exists(buffer))
        {
            InterRemoveBuffer(pKernel, self, buffer, BUFFER_REMOVE_CLEAR);

            iRemoveCount++;
        }
    }

    return iRemoveCount > 0;
}

// ���BUFFͬʱ�޸�ʱ��
PERSISTID BufferModule::AddBuffWithLifeTime(IKernel* pKernel,
											const PERSISTID& self,
											const PERSISTID& sender,
											const char* szBufferID,
											const int iLifeTime)
{
	if (!pKernel->Exists(self) || NULL == szBufferID || StringUtil::CharIsNull(szBufferID))
	{
		return PERSISTID();
	}

	// ����BUFF ����У��
	PERSISTID buffer = pKernel->CreateFromConfig(pKernel->GetScene(), "", szBufferID);
	IGameObj* pBufferObj = pKernel->GetGameObj(buffer);
	if (NULL == pBufferObj)
	{
		return PERSISTID();
	}

	// ����Buffer������ʱ��
	BPropertyInstance->SetLifeTime(pBufferObj,iLifeTime);

	// ���buffer
	if (!m_pInstance->InterAddBuffer(pKernel, self, sender, buffer))
	{
		pKernel->Destroy(pKernel->GetScene(), buffer);
		buffer = PERSISTID();
	}

	return buffer; 
}

// ���������ʱЧ���Ƶ�BUFFER
void BufferModule::ClearAllBuffer(IKernel* pKernel, const PERSISTID& self)
{
	PERSISTID container = BufferModule::GetBufferContainer(pKernel, self);
	if (!pKernel->Exists(container))
	{
		return;
	}

	unsigned int it;
	PERSISTID buffer = pKernel->GetFirst(container, it);
	LoopBeginCheck(e)
	while (pKernel->Exists(buffer))
	{
		LoopDoCheck(e)
		BufferModule::InterRemoveBuffer(pKernel, self, buffer, BUFFER_REMOVE_CLEAR);
		buffer = pKernel->GetNext(container, it);
	}
}

//��ȡbuffer��������
PERSISTID BufferModule::GetBufferOwner(IKernel* pKernel, const PERSISTID& buffer)
{
    if (!pKernel->Exists(buffer))
    {
        return PERSISTID();
    }

    // buffer����������������BufferContainer���߳���)
    PERSISTID parent = pKernel->Parent(buffer);
    if (!pKernel->Exists(parent))
    {
        return parent;
    }
    // buffer��������Ӧ�Ķ���
    parent = pKernel->Parent(parent);
    if (!pKernel->Exists(parent))
    {
        return parent;
    }
    if (pKernel->Type(parent) == TYPE_PLAYER || pKernel->Type(parent) == TYPE_NPC)
    {
        return parent;
    }
    return PERSISTID();
}

//BufferContainer����
int BufferModule::OnBufferContainerCreate(IKernel* pKernel, const PERSISTID& container,
										  const PERSISTID& buffer, const IVarList& args)
{
	IGameObj* pContainerObj = pKernel->GetGameObj(container);
	if (NULL == pContainerObj)
	{
		return 0;
	}
	pContainerObj->SetWideStr("Name", wszBufferContainerName);
	return 0;
}

int BufferModule::OnBufferOnRecover(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args)
{
	IGameObj* pBuffObj = pKernel->GetGameObj(self);
	if (NULL == pBuffObj)
	{
		return 0;
	}

	const char* szBufferConfigID = pKernel->GetConfig(self);
	BPropertyInstance->SetConfig(pBuffObj, szBufferConfigID);

	ADD_HEART_BEAT(pKernel, self, "BufferModule::H_BufferUpdate", BUFFER_HEART_INTERVAL);
	return 0;
}

// Buffer�������
int BufferModule::OnBufferLoad(IKernel* pKernel, const PERSISTID& self,
						 const PERSISTID& sender, const IVarList& args)
{
	// ��ȫ���
	IGameObj* pBufferObj = pKernel->GetGameObj(self);
	if (pBufferObj == NULL)
	{
		return 0;
	}

	// ���config����
	const char* szBufferConfigID = pKernel->GetConfig(self);
	pBufferObj->SetString("Config", szBufferConfigID);

	return 0;
}

//����������-NPC�����
int BufferModule::OnLoad(IKernel* pKernel, const PERSISTID& self,
						 const PERSISTID& sender, const IVarList& args)
{
	if (!pKernel->Exists(self))
	{
		return 0;
	}

	//Buffer����
	PERSISTID container = m_pInstance->GetBufferContainer(pKernel, self);
	if (!pKernel->Exists(container))
	{
		container = pKernel->CreateContainer(self, "BufferContainer", 32);
	}

	if (!pKernel->Exists(container))
	{
		::extend_warning(LOG_TRACE,
			"[BufferModule::OnLoad] the role '%d-%d' cant create BufferContainer.",
			self.nIdent, self.nSerial);
		return 0;
	}

	// ������Ϊ��������������������û������������һ��
	BufferModule::m_pInstance->RemoveBuffByClearType(pKernel, self, BUFFER_OFFLINE_CLEAR);
	return 0;
}

//���浽���ݿ�ǰ
int BufferModule::OnStore(IKernel* pKernel, const PERSISTID& self,
						  const PERSISTID& sender, const IVarList& args)
{
	int type = args.IntVal(0);
	if (type == STORE_EXIT)
	{
		//����
		//������п��Ա������BUFFER
		//������OffLineClear == 1
		BufferModule::m_pInstance->RemoveBuffByClearType(pKernel, self, BUFFER_OFFLINE_CLEAR);
	}
	return 0;
}

int BufferModule::OnReady(IKernel* pKernel, const PERSISTID& self,
						  const PERSISTID& sender, const IVarList& args)
{
	if (!pKernel->Exists(self))
	{
		return 0;
	}

	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return 0;
	}

    int online_flag = pSelfObj->QueryInt("Online");
    
    // online_flag == 1 ��ʾ���������
	if (!pKernel->FindViewport(self, VIEWPORT_BUFFER) && online_flag == 1)
	{
		PERSISTID buffercontainer = m_pInstance->GetBufferContainer(pKernel, self);
		//����Skill����
		if (pKernel->Exists(buffercontainer))
		{
			pKernel->AddViewport(self, VIEWPORT_BUFFER, buffercontainer);
		}
	}
	return 0;
}

// �л�����
int BufferModule::OnLeaveScene(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args)
{
	PERSISTID container = m_pInstance->GetBufferContainer(pKernel, self);
	if (!pKernel->Exists(container))
	{
		return 0;
	}

	BufferModule::m_pInstance->RemoveBuffByClearType(pKernel, self, BUFFER_SWITCH_CLEAR);
	return 0;
}

//����ɱ
int BufferModule::OnCommandBeKill(IKernel* pKernel, const PERSISTID& self,
								  const PERSISTID& sender, const IVarList& args)
{
	//������п��Ա������BUFFER
	//������DeadClear == 1
	BufferModule::m_pInstance->RemoveBuffByClearType(pKernel, self, BUFFER_DEAD_CLEAR);
	return 0;
}

//�¼���Ӧ
//˵����
//1:BUFFER����ʱ���ܻ�ı��������Ի���
//  ���ı�������¼�����ʱ����Щ�¼�ֻ��������BeAttack��BeDamage��BeKill���¼����¼���������Щ�¼���sender
//2:ֻ����Ӧս��������ص�Command
int BufferModule::OnCommandEffectBuffer(IKernel* pKernel, const PERSISTID& self,
								 const PERSISTID& sender, const IVarList& args)
{
	int cmdid = args.IntVal(0);

	// ����Ϣ�¼���ת��BUFF�¼�
	EBufferEventType event_type = BUFFEVENT_UNKNOW;

	// �ڶ�Ŀ��
	CVarList params;
	switch (cmdid)
	{
	case COMMAND_BEDAMAGE:
		{
			event_type = BUFFEVENT_BEDAMAGE;
			params << args.IntVal(1) << args.IntVal(2);
		}
		break;
	case COMMAND_SKILL_BEFORE_HIT:
		{
			event_type = BUFFEVENT_BEFORE_HIT;
			params << args.ObjectVal(1);
		}
		break;
	case COMMAND_BEVA:
		{
			event_type = BUFFEVENT_BE_VA;
		}
		break;
	case COMMAND_DAMAGETARGET:
		{
			event_type = BUFFEVENT_DAMAGE;
			params << args.ObjectVal(1);
		}
		break;
	default:
		::extend_warning(LOG_TRACE, 
			"[BufferModule::OnCommandEffectBuffer] reckon without COMMAND type:%d.",
			cmdid);
		return 0;
	}

	PERSISTID buffercontainer = m_pInstance->GetBufferContainer(pKernel, self);
	if (!pKernel->Exists(buffercontainer))
	{
		::extend_warning(LOG_TRACE,
			"[BufferModule::OnCommandEffectBuffer] the role '%d-%d' not have BufferContainer.", 
			self.nIdent, self.nSerial);
		return 0;
	}

	unsigned int uit;
	PERSISTID buffer = pKernel->GetFirst(buffercontainer, uit);
	LoopBeginCheck(f)
	while (pKernel->Exists(buffer))
	{
		LoopDoCheck(f)
		m_pInstance->BufferEventEffect(pKernel, event_type, self, sender, buffer, params);
		buffer = pKernel->GetNext(buffercontainer, uit);
	}

	return 0;
}

//buffer�����ʼ����ʱ����
int BufferModule::H_BufferUpdate(IKernel* pKernel, const PERSISTID& buffer, int slice)
{
	IGameObj* pBufferObj = pKernel->GetGameObj(buffer);
	if (NULL == pBufferObj)
	{
		return 0;
	}

	PERSISTID owner = pKernel->Parent(pKernel->Parent(buffer));
	IGameObj* pOwnerObj = pKernel->GetGameObj(owner);
	if (NULL == pOwnerObj)
	{
		// buffer�����Ķ��󲻴���
		return 0;
	}

	const char* szBufferConfigID = pKernel->GetConfig(buffer);
	if (StringUtil::CharIsNull(szBufferConfigID))
	{
		return 0;
	}

	const BuffBaseData* pBuffBaseData = SkillDataQueryModule::m_pInstance->GetBuffBaseData(szBufferConfigID);
	if (NULL == pBuffBaseData)
	{
		return false;
	}

	//BUFFER�Ѵ��ڵ�ʱ��
	int exist_time  = 0;
	BPropertyInstance->GetTimer(pBufferObj,exist_time);
	int life_Time   = 0;
	BPropertyInstance->GetLifeTime(pBufferObj,life_Time);

	int iEffectType = pBuffBaseData->GetEffectType();
	int iLifeTimeType = pBuffBaseData->GetLifeTimeType();

	if ( BUFFER_EFFECTTYPE_3 == iEffectType )
	{
		//3:��X������Ч,ÿY�뷢��һ��
		int y = 0;
		if (NULL != pBuffBaseData)
		{
			y = pBuffBaseData->GetEffectTimer();
		}
		int count   = 0;
		if (y > 0)
		{
			count = (exist_time % y + slice) / y;
		}

		//������Чcount��
		LoopBeginCheck(b)
		for (int i = 0; i < count; i++)
		{
			LoopDoCheck(b)
			PERSISTID sender;
			BPropertyInstance->GetSender(pBufferObj,sender);
			m_pInstance->TimerBuffer(pKernel, owner, sender, buffer);
			if (!pKernel->Exists(buffer))
			{
				return 0;
			}
		}
	}

	//����Buffer����ʱ��
	BPropertyInstance->SetTimer(pBufferObj,exist_time+slice);

	//ʱ�䵽ɾ��
	BPropertyInstance->GetTimer(pBufferObj, exist_time);
	int64_t addTime = 0;
	BPropertyInstance->GetAddTime(pBufferObj, addTime);
	// ������buff����Ҫ��ʱ��ɾ��
	if ( BUFFER_EFFECTTYPE_5 == iEffectType )
	{
		return 0;
	}
	if (m_pInstance->GetBufferLeaveTime(iLifeTimeType, life_Time, exist_time, addTime) <= 0)
	{
		//��Чʱ���ѵ�����״̬��ʧ
		m_pInstance->InterRemoveBuffer(pKernel, owner, buffer, BUFFER_REMOVE_TIMEEND);
	}

	return 0;
}

bool BufferModule::PreloadBuffConfig(IKernel* pKernel)
{
	CVarList pathList;
	if (!SkillDataQueryModule::m_pInstance->GetBuffConfigPath("BuffReg", pathList))
	{
		return false;
	}
	
	// ������ҪԤ���ص������ļ�
    fast_string strFile = "";
	LoopBeginCheck(c)
	for (size_t i = 0; i < pathList.GetCount(); ++i)
	{
		LoopDoCheck(c)
		strFile = "ini/";
        strFile += pathList.StringVal(i);
		pKernel->PreloadConfig(strFile.c_str());
	}
	return true;
}