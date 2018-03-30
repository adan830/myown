
//--------------------------------------------------------------------
// �ļ���:      FormulaManagerModule.h
// ��  ��:      
// ˵  ��:      
// ��������:    2015��7��29��
// ������:       
//    :       
//--------------------------------------------------------------------

#include "FsGame/SkillModule/Impl/FormulaManagerModule.h"
#include "utils/Xmlfile.h"
#include "utils/string_util.h"
#include "utils/extend_func.h"
#include "../../Define/GameDefine.h"
#include "Formula.h"
#include "FormulaPropIndexDefine.h"
#include "../../CommonModule/LuaExtModule.h"

//const char* const FormulaManagerModule::CONST_PROP_RELATION_FORMULA_XML = "share/modifypack/propdefined.xml";
const char* const FormulaManagerModule::CONST_PROP_DAMAGER_FORMULA_XML = "ini/ModifyPackage/FightDataFormula.xml";

FormulaManagerModule* FormulaManagerModule::m_instance = NULL;

// ���¼���ս����ʽ����
int nx_reload_fight_formula_config(void* state)
{
	IKernel* pKernel = LuaExtModule::GetKernel(state);
	
	if (FormulaManagerModule::m_instance != NULL)
	{
		FormulaManagerModule::m_instance->LoadResource(pKernel);
	}
	return 1;
}

bool FormulaManagerModule::Init(IKernel* pKernel)
{
    m_instance = this;
    
    FormulaPropIndex::RegisterPropIndex();
    LoadResource(pKernel);

	DECL_LUA_EXT(nx_reload_fight_formula_config);
    return true;
}

bool FormulaManagerModule::Shut(IKernel* pKernel)
{
    Release();
    return true;
}

bool FormulaManagerModule::Release()
{
	LoopBeginCheck(a)
	int nSize = (int)m_vecFormulas.size();
    for (int i = 0; i < nSize; ++i)
    {
		LoopDoCheck(a)
        Formula* formula = m_vecFormulas[i];
        if (NULL != formula)
        {
            SAFE_DELETE(formula);
            m_vecFormulas[i] = NULL;
        }
    }

	m_vecFormulas.clear();
    return true;
}

bool FormulaManagerModule::GetValue(IGameObj* pSelfObj, IGameObj* pTargetObj, IGameObj* pSelfSkill, int formula_index, float& value)
{
    if (formula_index < 0 || formula_index >= FORMULA_PROP_MAX)
    {
        return false;
    }
	// ������м����Ļ���ֵ
	for (int i = FORMULA_PROP_BasePhysAttackRate; i < FORMULA_PROP_MAX; ++i)
	{
		Formula* pTmpFormula = GetFormula(i);
		if (NULL != pTmpFormula)
		{
			pTmpFormula->ClearCacheVal();
		}
	}

    Formula* formula = GetFormula(formula_index);
    if (NULL == formula)
    {
        return false;
    }

    return formula->GetValue(pSelfObj, pTargetObj, pSelfSkill, value, FormulaManagerModule::GetRecursionFormula);
}

Formula* FormulaManagerModule::GetRecursionFormula(int formula_index)
{
    // �ж��Ƿ�����Ҫ�ݹ����Ĺ�ʽ����
	if (formula_index < FORMULA_PROP_BasePhysAttackRate || formula_index >= FORMULA_PROP_MAX)
    {
        return NULL;
    }
    return m_instance->GetFormula(formula_index);
}

Formula* FormulaManagerModule::GetFormula(int formula_index)
{
    if (formula_index < 0 || formula_index >= FORMULA_PROP_MAX)
    {
        return NULL;
    }

    return m_vecFormulas[formula_index];
}


bool FormulaManagerModule::LoadResource(IKernel* pKernel)
{
	Release();

    std::string path = pKernel->GetResourcePath();
    std::string strFileName = path + CONST_PROP_DAMAGER_FORMULA_XML;
    CXmlFile xml(strFileName.c_str());
    if (!xml.LoadFromFile())
    {
        ::extend_warning(LOG_ERROR,
            "[FormulaManagerModule::LoadResource] not found file:%s.",
            strFileName.c_str());
        return false;
    }

    ParseFormula(xml);
	// ��ʱȥ�����Թ�ʽ��
//     xml.Release();
//     strFileName = path + CONST_PROP_RELATION_FORMULA_XML;
//     xml.SetFileName(strFileName.c_str());
//     if (!xml.LoadFromFile())
//     {
//         ::extend_warning(LOG_WARNING,
//             "[FormulaManagerModule::LoadResource] not found file:%s.",
//             strFileName.c_str());
//         return false;
//     }
// 
//     ParseFormula(xml);
    return true;
}

bool FormulaManagerModule::ParseFormula(const CXmlFile& xml)
{
    CVarList vSectionList;
    xml.GetSectionList(vSectionList);
    size_t nSectionCount = vSectionList.GetCount();

	m_vecFormulas.resize(FORMULA_PROP_MAX);
	LoopBeginCheck(b)
    for (size_t t = 0; t < nSectionCount; t++)
    {
		LoopDoCheck(b)
        std::string strSection = vSectionList.StringVal(t);
        if (strSection.empty() || strSection.compare("-1") == 0 || strSection.compare("0") == 0)
        {
            continue;
        }

        const char* szRelationFormula = xml.ReadString(strSection.c_str(), "RelationFormula", "");
        if (StringUtil::CharIsNull(szRelationFormula))
        {
            continue;
        }

        int index = FormulaPropIndex::ToPropIndex(strSection.c_str());
        if (index < 0 || index >= FORMULA_PROP_MAX)
        {
            continue;
        }
        if (m_vecFormulas[index] != NULL)
        { // �������ظ��Ķ���
            continue;
        }

        Formula* formula = NEW Formula;
        if (NULL == formula)
        {
            continue;
        }

        const char* szMinValue = xml.ReadString(strSection.c_str(), "Min", "");
        const char* szMaxValue = xml.ReadString(strSection.c_str(), "Max", "");
        if (!formula->Parse(szRelationFormula, szMinValue, szMaxValue))
        {
            SAFE_DELETE(formula);
            continue;
        }

        m_vecFormulas[index] = formula;
    }

    return true;
}

// ��ȡ��ʱ�����Ĺ�ʽֵ
// bool FormulaManagerModule::GetImmediateFormualValue(IGameObj* pSelfObj, IGameObj* pTargetObj, const char* str, float& value)
// {
//     // 1.�ж��Ƿ��Ѿ���������
//     // 2.δ�����������Ƚ���Ȼ�󱣴�����Ĺ�ʽformula��
//     // 3.�ӹ�ʽ�����б����ҵ���Ӧ��formula�����ع�ʽֵ��
//     if (StringUtil::CharIsNull(str))
//     {
//         return false;
//     }
// 
//     Formula* formula = NULL;
//     TStringPod<char, Formula*>::iterator it = m_ImmediateFormulaMap.Find(str);
//     if (it == m_ImmediateFormulaMap.End())
//     {
//         formula = NEW Formula;
//         if (NULL == formula)
//         {
//             return false;
//         }
// 
//         if (!formula->Parse(str, "", ""))
//         {
//             SAFE_DELETE(formula);
//             return false;
//         }
// 
//         if (!m_ImmediateFormulaMap.Add(str, formula))
//         {
//             SAFE_DELETE(formula);
//             return false;
//         }
//     }
//     else
//     {
//         formula = it.GetData();
//     }
//     
//     if (NULL == formula)
//     {
//         return false;
//     }
// 
//     return formula->GetValue(pSelfObj, pTargetObj, value, FormulaManagerModule::GetRecursionFormula);
// }
