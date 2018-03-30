//--------------------------------------------------------------------
// �ļ���:      EnvirValueModule.h
// ��  ��:      �����еĳ���
// ˵  ��:
// ��������:    2016��03��09��
// ������:       
// �޸���:        
//    :       
//--------------------------------------------------------------------
#include "EnvirValueModule.h"
#include "public\VarType.h"
#include "utils\string_util.h"
#include "utils\XmlFile.h"
#include "utils\extend_func.h"

#ifndef FSROOMLOGIC_EXPORTS
#include "ReLoadConfigModule.h"
#endif

#define ENVIR_VALUE_CONFIG "ini/GameConst.xml"		// ��������·��

CVar EnvirValueModule::m_arrEnvirData[ENVIR_VALUE_MAX];

bool EnvirValueModule::Init(IKernel *pKernel)
{
	ReloadConfig(pKernel);

#ifndef FSROOMLOGIC_EXPORTS
	RELOAD_CONFIG_REG("EnvirValueConfig", EnvirValueModule::ReloadConfig);
#endif
	return true;
}

bool EnvirValueModule::Shut(IKernel *pKernel)
{
	return true;
}

// ��ѯint����
int	EnvirValueModule::EnvirQueryInt(int type)
{
	if (type < ENV_VALUE_NONE || type >= ENVIR_VALUE_MAX)
	{
		return -1;
	}

	return m_arrEnvirData[type].IntVal();
}

// ��ѯfloat����
float EnvirValueModule::EnvirQueryFloat(int type)
{
	if (type < ENV_VALUE_NONE || type >= ENVIR_VALUE_MAX)
	{
		return 0.0f;
	}

	return m_arrEnvirData[type].FloatVal();
}

// ��ѯstring����
const char*	EnvirValueModule::EnvirQueryString(int type)
{
	if (type < ENV_VALUE_NONE || type >= ENVIR_VALUE_MAX)
	{
		return "";
	}

	return m_arrEnvirData[type].StringVal();
}

// ���¼�������
void EnvirValueModule::ReloadConfig(IKernel *pKernel)
{
	// �����ļ�·��
	std::string pathName = pKernel->GetResourcePath();
	pathName += ENVIR_VALUE_CONFIG;

	CXmlFile xmlfile(pathName.c_str());
	if (!xmlfile.LoadFromFile())
	{
		std::string info = "Not Exist " + pathName;
		::extend_warning(LOG_ERROR, info.c_str());
		return;
	}

	// ����������ö�ٸ���Ҫһ��
	const int iSectionCount = (int)xmlfile.GetSectionCount();
	if (iSectionCount != ENVIR_VALUE_MAX)
	{
		::extend_warning(LOG_ERROR, "EnvirValue Count error[%s]", pathName.c_str());
	}

	LoopBeginCheck(a);
	for (int i = 0; i < iSectionCount; i++)
	{
		LoopDoCheck(a);
		const char* section = xmlfile.GetSectionByIndex(i);
		int nIndex = EnumIndex<EnvirValueTypes>(section);
		if (nIndex < ENV_VALUE_NONE || nIndex >= ENVIR_VALUE_MAX)
		{
			continue;
		}
		const char* strValue = xmlfile.ReadString(section, "Value", "");
		int			nValType = xmlfile.ReadInteger(section, "ValueTye", 0);
		if (StringUtil::CharIsNull(strValue))
		{
			continue;
		}

		switch (nValType)
		{
		case VTYPE_INT:
			m_arrEnvirData[nIndex].SetInt(StringUtil::StringAsInt(strValue));
			break;
		case VTYPE_FLOAT:
			m_arrEnvirData[nIndex].SetFloat(StringUtil::StringAsFloat(strValue));
			break;
		case VTYPE_STRING:
			m_arrEnvirData[nIndex].SetString(strValue);
			break;
		default:
			{
				std::string info = pathName + std::string(section) + "Value Type error";
				::extend_warning(LOG_ERROR, info.c_str());
			}
		}
	}
}