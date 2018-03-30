//--------------------------------------------------------------------
// �ļ���:      PassiveSkillModifyPack.h
// ��  ��:      �����������԰��߼�
// ˵  ��:
// ��������:    2016��3��3��
// ������:		 ������
//    :       
//--------------------------------------------------------------------

#include "..\CommonModule\PropRefreshModule.h"
#include "PassiveSkillModifyPack.h"
#include "..\Define\CommandDefine.h"
#include "utils\string_util.h"

/**
@brief  ��ʼ��
@param  pKernel [IN] �������ָ��
@return ���س�ʼ�����
*/
bool PassiveSkillModifyPack::Init(IKernel* pKernel)
{
	// ��������������Ϣ����
	pKernel->AddIntCommandHook("player", COMMAND_PASSIVE_SKILL_ADD_PROP, PassiveSkillModifyPack::AddPassiveSkillModifyPack);

	// ��������������Ϣ����
	pKernel->AddIntCommandHook("PetNpc", COMMAND_PASSIVE_SKILL_ADD_PROP, PassiveSkillModifyPack::AddPassiveSkillModifyPack);

	// ���������Ƴ���Ϣ����
	pKernel->AddIntCommandHook("player", COMMAND_PASSIVE_SKILL_REMOVE_PROP, PassiveSkillModifyPack::RemovePassiveSkillModifyPack);

	// ���������Ƴ���Ϣ����
	pKernel->AddIntCommandHook("PetNpc", COMMAND_PASSIVE_SKILL_REMOVE_PROP, PassiveSkillModifyPack::RemovePassiveSkillModifyPack);

	return true;
}

// ���±�������Ӱ�����������
int PassiveSkillModifyPack::AddPassiveSkillModifyPack(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return 0;
	}

	IRecord* pPropModifyRec = pSelfObj->GetRecord(CONST_PROP_MODIFY_BYEQUIP_REC);
	if (NULL == pPropModifyRec)
	{
		return 0;
	}

	int nIndex = 1;
	int nSkillId = args.IntVal(nIndex++);
	int nPropCount = args.IntVal(nIndex++);

	CVarList propNameList;
	CVarList propValueList;

	// ͳ�ƶ�������ֵ�ı�
	MapPropValue mapPropValue;
	LoopBeginCheck(b);
	for (int i = 0;i < nPropCount;++i)
	{
		LoopDoCheck(b);
		const char* strPorp = args.StringVal(nIndex++);
		float fValue = args.FloatVal(nIndex++);

		propNameList << strPorp;
		propValueList << fValue;

		PropRefreshModule::m_pInstance->CountRefreshData(pKernel, self, strPorp, fValue, EModify_ADD_VALUE, mapPropValue);

		// ����������������������
		CVarList data;
		data << strPorp << fValue << StringUtil::IntAsString(nSkillId);
		pPropModifyRec->AddRowValue(-1, data);
	}

	// ������������
	PropRefreshModule::m_pInstance->RefreshData(pKernel, self, mapPropValue, EREFRESH_DATA_TYPE_ADD);
	return 0;
}

// �Ƴ���������Ӱ�����������
int PassiveSkillModifyPack::RemovePassiveSkillModifyPack(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return 0;
	}

	int nPassiveSkillId = args.IntVal(1);
	std::string strPassiveSkillId = StringUtil::IntAsString(nPassiveSkillId);
	IRecord* pPropModifyRec = pSelfObj->GetRecord(CONST_PROP_MODIFY_BYEQUIP_REC);
	if (NULL == pPropModifyRec)
	{
		return 0;
	}

	CVarList propNameList;
	CVarList propValueList;

	// ͳ�ƶ�������ֵ�ı�
	MapPropValue mapPropValue;
	LoopBeginCheck(c);
	while (true)
	{
		LoopDoCheck(c);
		int nRow = pPropModifyRec->FindString(PMPS_PROP_SOURCE, strPassiveSkillId.c_str());
		if (nRow < 0)
		{
			break;
		}

		std::string strPropName = pPropModifyRec->QueryString(nRow, PMPS_PROP_NAME);
		float fValue = pPropModifyRec->QueryFloat(nRow, PMPS_PROP_VALUE);

		propNameList << strPropName.c_str();
		propValueList << fValue;

		// fValueֵ�ڼ���recordʱ�Ѿ��Ǽ��������������ռ�ֵ���˴�ֻ�贫��EModify_ADD_VALUEͳ�Ƽ���
		PropRefreshModule::m_pInstance->CountRefreshData(pKernel, self, strPropName.c_str(), fValue, EModify_ADD_VALUE, mapPropValue); 
		pPropModifyRec->RemoveRow(nRow);
	}

	// ����ֵ���
	PropRefreshModule::m_pInstance->RefreshData(pKernel, self, mapPropValue, EREFRESH_DATA_TYPE_REMOVE);
	return 0;
}