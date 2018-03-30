//--------------------------------------------------------------------
// �ļ���:      FunctionEventModule.h
// ��  ��:      ��Ϸ��������ģ��
// ˵  ��:
// ��������:    2017��02��10��
// ������:      ���
//--------------------------------------------------------------------
#include "FunctionEventModule.h"
#include "FsGame/Define/ContainerDefine.h"
#include "utils/XmlFile.h"
#include "utils/extend_func.h"

#ifndef FSROOMLOGIC_EXPORTS
#include "ReLoadConfigModule.h"
#endif
#include "utils/string_util.h"

// ��������·��
#define PATH_FUNCTION_EVENT_CONFIG "ini/SystemFunction/FunctionEvent.xml"		

FunctionEventModule::FuncInfo FunctionEventModule::ms_funcInfo[FUNCTION_EVENT_ID_END] = { { ITEM_NOT_BOUND, 0 } };

bool FunctionEventModule::Init(IKernel *pKernel)
{
	ReloadConfig(pKernel);

#ifndef FSROOMLOGIC_EXPORTS
	RELOAD_CONFIG_REG("FunctionEventConfig", FunctionEventModule::ReloadConfig);
#endif
	return true;
}

bool FunctionEventModule::Shut(IKernel *pKernel)
{
	return true;
}

// ��ѯint����
int FunctionEventModule::GetItemBindState( int nFunctionId )
{
	if (nFunctionId < FUNCTION_EVENT_ID_SYS || nFunctionId >= FUNCTION_EVENT_ID_END )
	{
		return ITEM_NOT_BOUND;
	}

	return ms_funcInfo[nFunctionId].bind_state ? ITEM_BOUND : ITEM_NOT_BOUND;
}



bool FunctionEventModule::GetItemBindPriorUse(int nFunctionId)
{
	if (nFunctionId < FUNCTION_EVENT_ID_SYS || nFunctionId >= FUNCTION_EVENT_ID_END)
	{
		return false;
	}

	return ms_funcInfo[nFunctionId].bind_prior_use;
}

// ���¼�������
void FunctionEventModule::ReloadConfig(IKernel *pKernel)
{
	// �����ļ�·��
	std::string pathName = pKernel->GetResourcePath();
	pathName += PATH_FUNCTION_EVENT_CONFIG;

	CXmlFile xmlfile(pathName.c_str());
	if (!xmlfile.LoadFromFile())
	{
		::extend_warning(LOG_ERROR, "not exist %s", pathName.c_str());
		return;
	}

	// ��������״̬
	memset(ms_funcInfo, 0, sizeof(ms_funcInfo));

	// ����������ö�ٸ���Ҫһ��
	const int iSectionCount = (int)xmlfile.GetSectionCount();
	LoopBeginCheck(a);
	for (int i = 0; i < iSectionCount; i++)
	{
		LoopDoCheck(a);
		const char* section = xmlfile.GetSectionByIndex(i);
		int nIndex = EnumIndex<EmFunctionEventId>(section);
		if (nIndex < FUNCTION_EVENT_ID_SYS || nIndex >= FUNCTION_EVENT_ID_END)
		{
			::extend_warning(LOG_WARNING, "[FunctionEventModule::ReloadConfig] function id overflow[id:%s]", section);
			continue;
		}

		int nBindState = ITEM_NOT_BOUND;
		int nBindPriorUse = 0;
		const char* strValue = xmlfile.ReadString(section, "BindState", "");
		if (!StringUtil::CharIsNull(strValue))
		{
			nBindState = StringUtil::StringAsInt(strValue);
		}
		strValue = xmlfile.ReadString(section, "BindPriorUse", "");
		if (!StringUtil::CharIsNull(strValue))
		{
			nBindPriorUse = StringUtil::StringAsInt(strValue);
		}

		ms_funcInfo[nIndex].bind_state = nBindState == ITEM_BOUND;
		ms_funcInfo[nIndex].bind_prior_use = nBindPriorUse != 0;
	}
}