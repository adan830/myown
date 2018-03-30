//----------------------------------------------------------
// �ļ���:      SwitchManagerModule.cpp
// ��  ��:      ���ܿ��ض���
// ˵  ��:
// ������:        
// ��������:    2015��6��30��
//    :        
//----------------------------------------------------------
#include "SwitchManagerModule.h"
#include "FsGame/CommonModule/LuaExtModule.h"
#include "FsGame/Define/PubDefine.h"
//#include "FsGame/Define/BackStageDefine.h"
#include "FsGame/Define/GameDefine.h"

#include "utils/extend_func.h"
#include "utils/custom_func.h"
#include "utils/string_util.h"
#include "utils/util_func.h"

SwitchManagerModule* SwitchManagerModule::m_SwitchManagerModule = NULL;

// ԭ�ͣ�bool nx_set_function(function_id, new_state)
// ���ܣ����ù���״̬
int nx_set_function(void* state)
{
    IKernel* pKernel = LuaExtModule::GetKernel(state);

    // ����������
    CHECK_ARG_NUM(state, nx_set_function, 2);
    // ����������
    CHECK_ARG_INT(state, nx_set_function, 1);
    CHECK_ARG_INT(state, nx_set_function, 2);

    const int function_id = pKernel->LuaToInt(state, 1);
    int new_state = pKernel->LuaToInt(state, 2);
    new_state = new_state > 0 ? 1 : 0;

    pKernel->LuaPushBool(state, SwitchManagerModule::SetFunctionEnable(pKernel, function_id, new_state));
    return 1;
}

bool SwitchManagerModule::Init(IKernel* pKernel)
{
    m_SwitchManagerModule = this;

	// ��������
	pKernel->AddEventCallback("scene", "OnCreate", OnSceneCreate);

	// ע��GM����
    DECL_LUA_EXT(nx_set_function);

	//ע����ع������ݵ����ݿ��ӳ�����
	DECL_HEARTBEAT(SwitchManagerModule::CB_DomainDB);

    return true;
}

bool SwitchManagerModule::Shut(IKernel* pKernel)
{
    return true;
}

//��ȡ����
const std::wstring SwitchManagerModule::GetDomainName(IKernel* pKernel)
{
    if (m_DomainName.empty())
    {
        wchar_t wstr[256];
        int server_id = 0;
#ifndef FSROOMLOGIC_EXPORTS
		server_id = pKernel->GetServerId();
#endif
        SWPRINTF_S(wstr, L"Domain_Switch_%d", server_id);
        m_DomainName = wstr;
    }

    return m_DomainName;
}

//��鹦���Ƿ���
bool SwitchManagerModule::CheckFunctionEnable(IKernel* pKernel, const int function_id, const PERSISTID& self)
{
	CVarList msg = m_SwitchManagerModule->QueryPubFunctionState(pKernel, function_id);
    const int curState = msg.IntVal(0);
    bool ret = curState > 0 ? true : false;

	if (!ret && pKernel->Exists(self))
	{
		SendCustomSysInfo(pKernel, self, function_id);
	}

    return ret;
}

//��Ϣ����
bool SwitchManagerModule::GetSwitchFunctionInfo(IKernel* pKernel, const char* content)
{
	if (pKernel == NULL || StringUtil::CharIsNull(content))
	{
		return false;
	}

	// ��������
	TiXmlDocument xml;
	xml.Parse(content);
	TiXmlHandle xmlHandle(&xml);
	TiXmlElement* pNode = xmlHandle.FirstChild("Property").ToElement();
	if (pNode == NULL)
	{
		return false;
	}

	// ��ȡid
	const TiXmlElement* pElement = (TiXmlElement*)pNode->FirstChild("functionid");
	if (pElement == NULL)
	{
		return false;
	}

	const char* ID = pElement->GetText();
	if (StringUtil::CharIsNull(ID))
	{
		return false;
	}

	// ��ȡ����
	pElement = (TiXmlElement*)pNode->FirstChild("state");
	if (pElement == NULL)
	{
		return false;
	}

	const char* state = pElement->GetText();
	if (StringUtil::CharIsNull(state))
	{
		return false;
	}

	//��������״̬
	const int NewState = StringUtil::StringAsInt(state);

	//��Ϊ���ܿ���ȫ��ʱID = all�������й��ܿ���״̬����ΪNewState
	if (0 == ::strcmp("all", ID))
	{
		LoopBeginCheck(a);	//��ֹ��ѭ��
		for (int i = 1; i < SWITCH_FUNCTION_MAX; ++i)
		{
			LoopDoCheck(a); //��ֹ��ѭ��
			// ��΢�ŷ���������һ��ȫ��֮�⣬��Ϊ΢�ŷ�����ʱ����ƽ̨����
// 			if (SWITCH_FUNCTION_SHARE_WEIXIN == i)
// 			{
// 				continue;
// 			}

			if (!SetFunctionEnable(pKernel, i, NewState))
			{
				return false;
			}
		}
	}
	else
	{
		//��ȡǰ׺ID = switch_1 ��Ϊ function_id = 1
		//��Ϊһ����صĻ� ID = switch_1,switch_2,switch_3��Ϊfunction_id = 1,2,3
		CVarList section_list;
		StringUtil::SplitString(section_list, ID, ",");
		const int section_count = static_cast<int>(section_list.GetCount());

		LoopBeginCheck(b);	//��ֹ��ѭ��
		for (int i = 0; i < section_count; ++i)
		{
			LoopDoCheck(b);	//��ֹ��ѭ��
			CVarList function_list;
			StringUtil::SplitString(function_list, section_list.StringVal(i), "_");
			// ����Id
			const int FunctionID = StringUtil::StringAsInt(function_list.StringVal(1));

			if (!SetFunctionEnable(pKernel, FunctionID, NewState))
			{
				return false;
			}
		}
	}
	
	return true;
}

//���ÿ�������
bool SwitchManagerModule::SetFunctionEnable(IKernel* pKernel, const int function_id, const int newState)
{
    if (function_id <= SWITCH_FUNCTION_MIN || function_id >= SWITCH_FUNCTION_MAX)
    {
        return false;
    }

    const int tempNewState = newState > 0 ? 1 : 0;
	//�õ����ع�������״̬
	CVarList func_list = m_SwitchManagerModule->QueryPubFunctionState(pKernel, function_id);
    const int curState = func_list.IntVal(0);
    if (tempNewState == curState)
    {
        return true; // û��
    }

	//�õ����ع�����������
	const std::string function_name = func_list.StringVal(1);
    // ����Ϣ������������Ϣ
#ifndef FSROOMLOGIC_EXPORTS
    CVarList msg;
    msg << PUBSPACE_DOMAIN << m_SwitchManagerModule->GetDomainName(pKernel).c_str() << SP_DOMAIN_MSG_FUNCTION_STATE 
		<< function_id << tempNewState << function_name << SWITCH_REC_TYPE_CHANGE;
    pKernel->SendPublicMessage(msg);
#endif

	// �޸����ݿ��еĹ��ܿ���״̬
	m_SwitchManagerModule->SaveSwitchConfigToDB(pKernel, function_id, tempNewState, function_name);

    return true;
}

//�鿴���ܿ��ع�������״̬
const CVarList& SwitchManagerModule::QueryPubFunctionState(IKernel* pKernel, const int function_id)
{
	static CVarList switch_list;
	//����ʱ����
	switch_list << 1 << "";

    // �õ�����������
#ifndef FSROOMLOGIC_EXPORTS
    IPubSpace* pPubSpace = pKernel->GetPubSpace(PUBSPACE_DOMAIN);
    if (!pPubSpace)
    {
        return switch_list;
    }

    // �õ�����������
    IPubData* pSwitchPubData = pPubSpace->GetPubData(GetDomainName(pKernel).c_str());
    if (NULL == pSwitchPubData)
    {
        return switch_list;
    }

    if (!pSwitchPubData->FindRecord(DOMAIN_SWITCH_REC))
    {
        return switch_list;
    }

    IRecord* pSwitchRec = pSwitchPubData->GetRecord(DOMAIN_SWITCH_REC);
    if (NULL == pSwitchRec)
    {
        return switch_list;
    }

    const int row = pSwitchRec->FindInt(SWITCH_REC_COL_FUNCTION_ID, function_id);
    if (row < 0)
    {
        // ��������û�ҵ��ù���
        return switch_list;
    }
	
	//���ܿ���״̬������
	const int func_state = pSwitchRec->QueryInt(row, SWITCH_REC_COL_CUR_STATE);
	const std::string func_name = pSwitchRec->QueryString(row, SWITCH_REC_COL_FUNCTION_NAME);

	switch_list.Clear();
	switch_list << func_state << func_name;
#endif
    return switch_list;
}

//���湦�ܿ������õ����ݿ���
bool SwitchManagerModule::SaveSwitchConfigToDB(IKernel* pKernel, const int function_id, const int newState, const std::string& function_name)
{
	//��ǰ׺��function_id = 1 ��Ϊ functionID = switch_1;
	std::string functionID = "switch_";
	functionID += StringUtil::IntAsString(function_id);
	
	// ���»���������Ϣ��ֵ��nx_config��
// 	if (!pKernel->UpdateManageBase(functionID.c_str(), util_string_as_widestr(function_name.c_str()).c_str(), NX_CONFIG_TYPE_SWITCH, L"", newState))
// 	{
// 		return false;
// 	}

	// ������Ӫ������Ϣ��ֵ��nx_config��
	//if (!pKernel->UpdateManageInfo(functionID.c_str(), L"", NX_CONFIG_TYPE_SWITCH, L"", newState, 0))
	//{
	//	return false;
	//}
	return true;
}

// �ӳټ��ع������ݵ����ݿ�
int SwitchManagerModule::CB_DomainDB(IKernel* pKernel, const PERSISTID& self, int slice)
{
	m_SwitchManagerModule->LoadResource(pKernel);

	return 0;
}

// ��������, �ڳ����������ӳ�����
int SwitchManagerModule::OnSceneCreate(IKernel* pKernel, const PERSISTID& scene, 
									const PERSISTID& sender, const IVarList& args)
{
	// �������ɢ�ڶ��member�ϣ�����ȷ��ÿ��member����һ������
	// ÿ��memberֻ��Ӧ�Լ����صĳ����ص������Դ˴�����Ӧ�ĳ����������member�ϵ�
	static bool alreadyAddBeat = false;
	if (alreadyAddBeat)
	{
		// �����������Ѿ������������
		return 0;
	}

	// ��ʱ�����ӳټ��������ļ���ȷ�������ݿ����ӳɹ������
	ADD_COUNT_BEAT(pKernel, scene, "SwitchManagerModule::CB_DomainDB", SWITCH_REC_DELAY_TIME, 1);

	alreadyAddBeat = true;

	return 0;
}

// ���ع������ݵ����ݿ�
bool SwitchManagerModule::LoadResource(IKernel* pKernel)
 {
#ifndef FSROOMLOGIC_EXPORTS
	// �õ�����������
	IPubSpace* pPubSpace = pKernel->GetPubSpace(PUBSPACE_DOMAIN);
	if (!pPubSpace)
	{
		// ����ʧ��
		return false;
	}

	// �õ�����������
	IPubData* pSwitchPubData = pPubSpace->GetPubData(GetDomainName(pKernel).c_str());
	if (NULL == pSwitchPubData)
	{
		// ����ʧ��
		return false;
	}

	if (!pSwitchPubData->FindRecord(DOMAIN_SWITCH_REC))
	{
		// ����ʧ��
		return false;
	}

	IRecord* pSwitchRec = pSwitchPubData->GetRecord(DOMAIN_SWITCH_REC);
	if (NULL == pSwitchRec)
	{
		// ����ʧ��
		return false;
	}

	const int row = pSwitchRec->GetRows();
	const int DefineRow = static_cast<int>(SWITCH_FUNCTION_MAX) - 1;
	if (row != DefineRow)
	{
		// ����ʧ��
		return false;
	}

	LoopBeginCheck(c);	//��ֹ��ѭ��
	for ( int i = 1; i < SWITCH_FUNCTION_MAX; ++i )
	{
		LoopDoCheck(c);	//��ֹ��ѭ��
		int irow = pSwitchRec->FindInt(SWITCH_REC_COL_FUNCTION_ID, i);
		if (irow < 0)
		{
			// ����ʧ��
			return false;
		}

		std::string function_name = pSwitchRec->QueryString(irow, SWITCH_REC_COL_FUNCTION_NAME);
		int function_state = pSwitchRec->QueryInt(irow, SWITCH_REC_COL_CUR_STATE);

		m_SwitchManagerModule->SaveSwitchConfigToDB(pKernel, i, function_state, function_name);
	}
#endif
	return true;
};

//֪ͨ�ͻ���
void SwitchManagerModule::SendCustomSysInfo(IKernel* pKernel, const PERSISTID& self, const int functin_id)
{
	if (!pKernel->Exists(self))
	{
		return;
	}
	//��ʱ����ͳһ��ʾ
	//     char szBuff[32];
	//     SPRINTF_S(szBuff, "sys_switch_%d", functin_id);
	//CustomSysInfo(pKernel, self, TIPSTYPE_SYSFUNCTION_PROMPT_MESSAGE, "sys_switch_functoin", CVarList());
}