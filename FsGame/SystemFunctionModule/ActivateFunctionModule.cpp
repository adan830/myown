//--------------------------------------------------------------------
// �ļ���:      ActivateFunctionModule.h
// ��  ��:      ���ܿ���
// ˵  ��:
// ��������:    2018��3��21��
// ������:      ������ 
//    :       
//--------------------------------------------------------------------
#include "ActivateFunctionModule.h"
#include "CommonModule\ReLoadConfigModule.h"
#include "Define\CommandDefine.h"
#include "utils\XmlFile.h"
#include "utils\extend_func.h"
#include "TaskModule\TaskDefine.h"

ActivateFunctionModule* ActivateFunctionModule::m_pInstance = NULL;

bool ActivateFunctionModule::Init(IKernel *pKernel)
{
	m_pInstance = this;

	RELOAD_CONFIG_REG("ActivateFunction", ReloadRes);
	if (!LoadRes(pKernel))
	{
		return false;
	}
	pKernel->AddIntCommandHook("player", COMMAND_TASK_ACTION_MSG, ActivateFunctionModule::ActivateFunctionByTask, 0);
	pKernel->AddIntCommandHook("player", COMMAND_LEVELUP_CHANGE, ActivateFunctionModule::ActivateFunctionByLevel);

	// �����ȼ�Ϊ1�Ĺ���
	pKernel->AddIntCommandHook("player", COMMAND_MSG_FIRST_ENTER, ActivateFunctionModule::ActivateFunctionByLevel);
	return true;
}

bool ActivateFunctionModule::Shut(IKernel* pKernel)
{
	return true;
}


//�鿴�����Ƿ񼤻�
bool ActivateFunctionModule::CheckActivateFunction(IKernel *pKernel, const PERSISTID &self, const int& type)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return false;
	}

	IRecord* pAFRec = pSelfObj->GetRecord(FIELD_RECORD_ACTIVATE_FUNCTION_REC);
	if (NULL == pAFRec)
	{
		return false;
	}

	int nRowIndex = pAFRec->FindInt(COLUMN_ACTIVATE_FUNCTION_REC_TYPE, type);
	return nRowIndex != -1;
}

bool ActivateFunctionModule::LoadRes(IKernel *pKernel)
{
	// �����ļ�·��
	std::string pathName = pKernel->GetResourcePath();
	pathName += "ini/SystemFunction/ActivateFunction.xml";

	CXmlFile xmlfile(pathName.c_str());
	if (!xmlfile.LoadFromFile())
	{
		std::string info = "Not Exist " + pathName;
		::extend_warning(LOG_ERROR, info.c_str());
		return false;
	}

	int nSectionCount = xmlfile.GetSectionCount();

	m_vecActivateData.clear();
	m_vecActivateData.reserve(nSectionCount);

	LoopBeginCheck(e);
	for (int i = 0; i < nSectionCount; ++i)
	{
		LoopDoCheck(e);
		const char* section = xmlfile.GetSectionByIndex(i);

		ActivateData data;
		data.nActivateType = xmlfile.ReadInteger(section, "Type", 0);
		data.nValue = xmlfile.ReadInteger(section, "Value", 0);
		data.nFunctionType = xmlfile.ReadInteger(section, "ActivateFunction", 0);

		m_vecActivateData.push_back(data);
	}

	return true;
}

void ActivateFunctionModule::ReloadRes(IKernel*pKernel)
{
	m_pInstance->LoadRes(pKernel);
}

//���񼤻��
int ActivateFunctionModule::ActivateFunctionByTask(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return 0;
	}
	int nActionType = args.IntVal(1);
	int nTaskId = args.IntVal(2);
	// ֻ�����������
	if (nActionType != TASK_ACTION_COMPLETE)
	{
		return 0;
	}

	// �����Ƿ��й��ܿ���
	FuncVec vecFunc;
	if(!m_pInstance->QueryFunctionType(vecFunc, ACTIVATE_FUNCTION_BY_TASK, nTaskId))
	{
		return 0;
	}

	// ���ܼ���
	for (FuncVecIter iter = vecFunc.begin(); iter != vecFunc.end(); ++iter)
	{
		m_pInstance->ActiveFunction(pKernel, self, *iter);
	}
	
	return 0;
}

//�ȼ������
int ActivateFunctionModule::ActivateFunctionByLevel(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return 0;
	}

	int nCurLv = pSelfObj->QueryInt(FIELD_PROP_LEVEL);

	// �����Ƿ��й��ܿ���
	FuncVec vecFunc;
	if (!m_pInstance->QueryFunctionType(vecFunc, ACTIVATE_FUNCTION_BY_LEVEL, nCurLv))
	{
		return 0;
	}

	// ���ܼ���
	for (FuncVecIter iter = vecFunc.begin(); iter != vecFunc.end(); ++iter)
	{
		m_pInstance->ActiveFunction(pKernel, self, *iter);
	}
	return 0;
}


// �����
void ActivateFunctionModule::ActiveFunction(IKernel*pKernel, const PERSISTID&self, int nFuncType)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return;
	}

	IRecord* pAFRec = pSelfObj->GetRecord(FIELD_RECORD_ACTIVATE_FUNCTION_REC);
	if (NULL == pAFRec)
	{
		return;
	}

	int nRowIndex = pAFRec->FindInt(COLUMN_ACTIVATE_FUNCTION_REC_TYPE, nFuncType);
	if (-1 == nRowIndex)
	{
		pAFRec->AddRowValue(-1, CVarList() << nFuncType);
		pKernel->Command(self, self, CVarList() << COMMAND_MSG_ACTIVATE_FUNCTION << nFuncType);
	}
}

//�������й���
bool ActivateFunctionModule::ActivateAllFunction(IKernel* pKernel, const PERSISTID& self)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return false;
	}

	int nSize = (int)m_vecActivateData.size();
	LoopBeginCheck(a);
	for (int i = 0; i < nSize; ++i)
	{
		LoopDoCheck(a);
		const ActivateData& data = m_vecActivateData[i];
		ActiveFunction(pKernel, self, data.nFunctionType);
	}
	return true;
}

//������й���
bool ActivateFunctionModule::CloseAllFunction(IKernel* pKernel, const PERSISTID& self)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return false;
	}

	IRecord* pAFRec = pSelfObj->GetRecord(FIELD_RECORD_ACTIVATE_FUNCTION_REC);
	if (NULL == pAFRec)
	{
		return false;
	}

	pAFRec->ClearRow();
	return true;
}

// ��ѯ��������
bool ActivateFunctionModule::QueryFunctionType(std::vector<int>& outAcFunc, int nType, int nValue)
{
	int nSize = (int)m_vecActivateData.size();
	outAcFunc.clear();
	int nFuncType = NONE_FUNCTION_TYPE;
	LoopBeginCheck(a);
	for (int i = 0; i < nSize;++i)
	{
		LoopDoCheck(a);
		const ActivateData& data = m_vecActivateData[i];
		
		if (data.nActivateType != nType)
		{
			continue;
		}

		// ����id����һ��
		if (ACTIVATE_FUNCTION_BY_TASK == nType)
		{
			if (data.nValue == nValue)
			{
				outAcFunc.push_back(data.nFunctionType);
			}
		}
		else  // �ȼ���С�ھ��ܿ�������
		{		
			 if (data.nValue <= nValue)
			 {
				 outAcFunc.push_back(data.nFunctionType);
			 }
		}
	}
	
	return !outAcFunc.empty();
}