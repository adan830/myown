
//--------------------------------------------------------------------
// �ļ���:      FormulaManagerModule.h
// ��  ��:      
// ˵  ��:      
// ��������:    2015��7��29��
// ������:       
//    :       
//--------------------------------------------------------------------


#ifndef __FORMULA_MANAGER_H__
#define __FORMULA_MANAGER_H__

#include "Fsgame/Define/header.h"
#include "utils/xmlfile.h"
#include "utils/StringPod.h"
#include "FormulaPropIndex.h"
#include "FormulaPropIndexDefine.h"
#include <vector>

class Formula;

class FormulaManagerModule : public ILogicModule
{
public:

    bool Init(IKernel* pKernel);
    bool Shut(IKernel* pKernel);

    // ��ȡ���Թ�ϵ�������õľ�̬��ʽ��ϵ��ֵ
    bool GetValue(IGameObj* pSelfObj, IGameObj* pTargetObj, IGameObj* pSelfSkill, int formula_index, float& value);

    // ��ȡ��ʱ�����Ĺ�ʽֵ
    bool GetImmediateFormualValue(IGameObj* pSelfObj, IGameObj* pTargetObj, const char* str, float& value);

	// ��ȡ����
	bool LoadResource(IKernel* pKernel);
private:
	// ����Formula���ݹ������
    static Formula* GetRecursionFormula(int formula_index); 

	// ��ȡ��ʽ
    Formula* GetFormula(int formula_index);

	// ��ʽ���ý���
    bool ParseFormula(const CXmlFile& xml);

	// ���ü�������е��м�ֵ����
	void ResetCacheVal();

	// ��ʽ����ɾ��
	bool Release();
private:
    static const char* const CONST_PROP_RELATION_FORMULA_XML;
    static const char* const CONST_PROP_DAMAGER_FORMULA_XML;

	typedef std::vector<Formula*> FormulaVec;

	FormulaVec m_vecFormulas;

    // ���漴ʱ�����Ĺ�ʽ��û������һ����ʽʱ���Ȳ��Ҹ������Ƿ��Ѿ�������
   // TStringPod<char, Formula*>  m_ImmediateFormulaMap;

public:
    static FormulaManagerModule* m_instance;
};

#endif // __FORMULA_MANAGER_H__
