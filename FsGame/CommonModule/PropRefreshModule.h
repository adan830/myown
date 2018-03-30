//------------------------------------------------------------------------------
// �ļ���:      PropRefreshModule.h
// ��  ��:
// ˵  ��:
// ��������:    2012��12��28��
// ������:       
// �޸���:		  
// ��ע:
//    :       
//------------------------------------------------------------------------------


#ifndef __PROPREFRESHMODULE_H__
#define __PROPREFRESHMODULE_H__

#include <vector>
#include <string>
#include <map>

#include "Fsgame/Define/header.h"
#include "FsGame/Define/ModifyPackDefine.h"
#include "Define/PlayerBaseDefine.h"
enum {MAX_PARAM_NUM = 8};
// ��ʽ����
enum FormulaType
{
	FORMULA_TYPE_PARAM = 1, // ������ʽ�Ĺ�ʽ
	FORMULA_TYPE_FLAG = 2,  // ����ǰ׺�Ĺ�ʽ
};
// ���Թ�ʽ�ṹ
struct PropFormula
{
	int iFormulaType;								 // ��ʽ����
	std::string strRelationFormula[PLAYER_JOB_MAX];  // ������ֵ��ʽ

	int srcs[MAX_PARAM_NUM + 1];
	std::string props[MAX_PARAM_NUM + 1];
    PropFormula():iFormulaType(0)
    {
        memset(srcs, 0, sizeof(int) * (MAX_PARAM_NUM + 1));
    }
};
class PropRefreshModule : public ILogicModule
{
public:

    virtual bool Init(IKernel* pKernel);
    virtual bool Shut(IKernel* pKernel);

	
public:
	// �ص����� ��Ҽ����������
	static int OnPlayerRecover(IKernel* pKernel, const PERSISTID& self,
		const PERSISTID& sender, const IVarList& args);

	bool LoadResource(IKernel* pKernel);

	// �����������  nClassType ���԰�����  strFormId ���԰��ڱ��е�id nNewPackageId ���µ����԰�id 
	static bool UpdateModifyProp(IKernel* pKernel, const PERSISTID& self, int nClassType, const char* strFormId, int nNewPackageId);

    // ʹ��ˢ������ˢ�¶���
    bool RefreshData(IKernel* pKernel, const PERSISTID& obj, const MapPropValue& mapPropValue, ERefreshDataType eRefreshType);

	// ���ˢ�����ݣ�������ְҵ���������
	bool PlayerRefreshData(IKernel* pKernel, const PERSISTID& obj, const MapPropValue& mapPropValue, ERefreshDataType eRefreshType);

    // ͳ��ˢ������
    bool CountRefreshData(IKernel* pKernel, const PERSISTID& obj,
        const char* szPropName, double fValue, int iComputeType, MapPropValue& mapPropValue);

	// ��ò��ظ�������
	bool GetFinalProp(IKernel* pKernel, const PERSISTID& self, const PERSISTID& equip,
		const IVarList& vPropNameList, const IVarList& vPropValueList, MapPropValue& mapFinalPropValue);

    // ��ȡ������װ��Ӱ���������Ϣ
    bool GetFinalEquipModifyObjProp(IKernel* pKernel, const IVarList& vPropNameList,
        const IVarList& vPropValueList, MapPropValue& mapFinalPropValue) const;

    // ���/ɾ����������
	bool CalFormulaForProp(IKernel* pKernel,const PERSISTID& self);
	void PropRefreshModule::BindPlayerRefreshProp(IKernel* pKernel, const PERSISTID& self);
private:
	// ��ʼ����ҵȼ��ɳ�����
    bool InitPlayerLevelProp(IKernel* pKernel, const PERSISTID& self);
    // ��ʼ����һ���ս����
  //  bool InitPlayerBaseBA(IKernel* pKernel, const PERSISTID& self);
	// ������ʽ��������ֵ
	void AnalyzeFormula(IKernel* pKernel,const PERSISTID& self,const char* szPropName,const PropFormula& propFormula);
	// ��ö���ǰ���õ����Թ�ϵ��ʽ
	const char* GetRelationFormula(IKernel* pKernel, const PERSISTID& self, const PropFormula& propFormula) const;

    // �ӹ�ʽ�н�����������������ȡӰ�칫ʽ���������
    bool GetRelationPropList(IKernel* pKernel, const char * propFormula, IVarList& res) const;

    // �������Թ�ϵ����
    bool LoadPropRelationData(IKernel* pKernel);

    std::string ModifyFormulaFormat(IKernel* pKernel, const PERSISTID& self,
        const MapPropValue& mapSelfData, const std::string& strFormula) const;

    // ������������
    bool CheckControlProp(IKernel* pKernel, const char* szPropName);
	static int PropRefreshModule::C_OnBindPropChanged(IKernel* pKernel, const PERSISTID& self,const char* property, const IVar& old);

	// �ȼ��仯�ص�
	static int OnDCLevelChange(IKernel* pKernel, const PERSISTID &self, const PERSISTID & sender, const IVarList & args);

	// ��ȡ��ҵȼ����԰�id nLevelΪ0 ȡ��ҵ�ǰ�ȼ�
	int GetLevelPropId(IKernel* pKernel, IGameObj* pSelfObj, int nLevel = 0);

	// ��Ӧ������������Ӵ���
	bool OnAddPlayerProperty(IKernel* pKernel, const PERSISTID& self, const char* strFormId, const IVarList& proNameList, const IVarList& proValueList);

	// ��Ӧ���������Ƴ�
	bool OnRemovePlayerProperty(IKernel* pKernel, const PERSISTID& self, const char* strFormId);

	// ��ȡ���԰�
	void GetPropPackage(int nClassType, int nPackageid, IVarList& outFinalNameList, IVarList& outFinalValueList);
public:
	static void ReloadConfig(IKernel* pKernel);
public:
    // ��ģ��ָ��
    static PropRefreshModule*   m_pInstance;

private:
	
	
    // ���������ṹ
    struct PropDescStruct
    {
        short iDataType;                                // ������������
        PropFormula sPropFormual;
        std::vector<std::string> vecEffectProp;     // ��Ӱ����Щ����
        PropDescStruct():iDataType(0)
        {
			vecEffectProp.clear();
        }
    };

    std::map<std::string, PropDescStruct> m_PropRelationMap;  // ���Թ�ϵ��

	std::vector<std::string> m_PropRelationIndex;

	//std::map<std::string, float>	m_mapBattleAbility;	// ս��������
}; // end of class PropRefreshModule


#endif // __PROPREFRESHMODULE_H__

