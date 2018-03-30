//--------------------------------------------------------------------
// �ļ���:      BufferModifyPack.h
// ��  ��:      Buffer���԰��߼�
// ˵  ��:
// ��������:    2016��3��3��
// ������:		 ������
//    :       
//--------------------------------------------------------------------

#include "BufferModifyPack.h"
#include "..\CommonModule\PropRefreshModule.h"
#include "utils\exptree.h"
#include "utils\extend_func.h"
#include "..\SystemFunctionModule\StaticDataQueryModule.h"
#include "Impl\SkillDefineDataStruct.h"
#include "Impl\SkillDataQueryModule.h"
#include "..\Define\StaticDataDefine.h"

/**
@brief  ��ʼ��
@param  pKernel [IN] �������ָ��
@return ���س�ʼ�����
*/
bool BufferModifyPack::Init(IKernel* pKernel)
{
	//�Ƴ�buff
	pKernel->AddEventCallback("BufferContainer", "OnRemove", BufferModifyPack::OnBuffRemove);

	//���buff
	pKernel->AddEventCallback("BufferContainer", "OnAfterAdd", BufferModifyPack::OnBuffAfterAdd);

	// �������
	pKernel->AddEventCallback("player", "OnRecover", BufferModifyPack::OnPlayerRecover);

	return true;
}

//�ص����� ��Ҽ����������
int BufferModifyPack::OnPlayerRecover(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args)
{
	MapPropValue mapPropValue;

	PERSISTID buffbox = pKernel->GetChild(self, L"BufferContainer");
	if (pKernel->Exists(buffbox))
	{
		unsigned int i;
		PERSISTID buff = pKernel->GetFirst(buffbox, i);
		LoopBeginCheck(m);
		while (pKernel->Exists(buff))
		{
			LoopDoCheck(m);
			BufferModifyPackSingleton::Instance()->UpdateBufferModifyPack(pKernel, self, buff, mapPropValue);
			buff = pKernel->GetNext(buffbox, i);
		}
	}

	PropRefreshModule::m_pInstance->RefreshData(pKernel, self, mapPropValue, EREFRESH_DATA_TYPE_ADD);

	return 0;
}

// ����buff���԰�
bool BufferModifyPack::UpdateBufferModifyPack(IKernel* pKernel, const PERSISTID& self, const PERSISTID& buffer, MapPropValue& mapPropValue)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	IGameObj* pBufferObj = pKernel->GetGameObj(buffer);
	if (NULL == pSelfObj || NULL == pBufferObj)
	{
		return false;
	}
	IRecord* pPropModifyRec = pSelfObj->GetRecord(CONST_PROP_MODIFY_REC);
	if (NULL == pPropModifyRec)
	{
		return false;
	}

	const char* szFromID = pKernel->GetConfig(buffer);

	bool isMilitaryRank = false;

	// ����buff�ļ��ܶ���
	PERSISTID skill = pBufferObj->QueryObject("Skill");

	// ����buff�Ķ���
	PERSISTID sender = pBufferObj->QueryObject("Sender");

	// ���߱����buff����ʱ,sender�ǿյ�
	if (!pKernel->Exists(sender))
	{
		sender = self;
	}

	const BuffBaseData* pBuffBaseData = SkillDataQueryModule::m_pInstance->GetBuffBaseData(szFromID);
	if ( pBuffBaseData == NULL  || !pBuffBaseData->GetIsModifyProp()) 
	{
		return false;
	}

	// ֵ��������
	int nCalType = pBuffBaseData->GetCalculateType();
	//��ȡ�µİ�
	std::string strBasePackageID = pBuffBaseData->GetBasePackage();
	if ( strBasePackageID.empty() )
	{
		return false;
	}

	CVarList vPropNameList;
	CVarList vPropValueList;

	//�鿴��Ӧ��������
	StaticDataQueryModule::m_pInstance->GetOneRowDataStr(STATIC_DATA_BUFF_BASE_PACK, 
		strBasePackageID, vPropNameList, vPropValueList);

	LoopBeginCheck(v);
	for ( size_t i = 0; i < vPropNameList.GetCount(); ++i )
	{
		LoopDoCheck(v);
		const char* strPropName = vPropNameList.StringVal(i);

		// ��ȡ���㹫ʽ����
		fast_string strPropFormula = vPropValueList.StringVal(i);
		if (strPropFormula.empty())
		{
			continue;
		}

		// ���㼼���˺�ϵ��
		ExpTree exp;

		float fVal = exp.CalculateEvent(pKernel, sender, skill, PERSISTID(), PERSISTID(), strPropFormula.c_str());

		PropRefreshModule::m_pInstance->CountRefreshData(pKernel, self, 
			vPropNameList.StringVal(i), fVal, nCalType, mapPropValue);

		MapPropValue::iterator iter = mapPropValue.find(strPropName);
		if (iter != mapPropValue.end())
		{
			fVal = (float)iter->second;
		}

		// ����BUFFӰ�����������
		CVarList data;
		data << strPropName << fVal << szFromID;
		pPropModifyRec->AddRowValue(-1, data);
	}

	return true;
}

// �Ƴ�buff���԰�
bool BufferModifyPack::RemoveBufferModifyPack(IKernel* pKernel, const PERSISTID& self, const PERSISTID& buffer, MapPropValue& mapPropValue)
{
	if (!pKernel->Exists(buffer))
	{
		return false;
	}
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return false;
	}
	IRecord* pPropModifyRec = pSelfObj->GetRecord(CONST_PROP_MODIFY_REC);
	if (NULL == pPropModifyRec)
	{
		return false;
	}

	const char* szFromID = pKernel->GetConfig(buffer);
	const BuffBaseData* pBuffBaseData = SkillDataQueryModule::m_pInstance->GetBuffBaseData(szFromID);
	if ( pBuffBaseData == NULL) 
	{
		return false;
	}

	bool isMilitaryRank = false;
	CVarList propNameList;
	CVarList propValueList;

	LoopBeginCheck(w);
	while (true)
	{
		LoopDoCheck(w);
		int nRow = pPropModifyRec->FindString(PMPS_PROP_SOURCE, szFromID);
		if (nRow < 0)
		{
			break;
		}

		std::string strPropName = pPropModifyRec->QueryString(nRow, PMPS_PROP_NAME);
		float fValue = pPropModifyRec->QueryFloat(nRow, PMPS_PROP_VALUE);

		// fValueֵ�ڼ���recordʱ�Ѿ��Ǽ��������������ռ�ֵ���˴�ֻ�贫��EModify_ADD_VALUEͳ�Ƽ���
		PropRefreshModule::m_pInstance->CountRefreshData(pKernel, self, strPropName.c_str(), fValue, EModify_ADD_VALUE, mapPropValue); 

		pPropModifyRec->RemoveRow(nRow);
	}

	return true;
}

// ����buff
int BufferModifyPack::OnBuffAfterAdd(IKernel* pKernel, const PERSISTID& buffcontainer, const PERSISTID& buffer, const IVarList& args)
{
	if (!pKernel->Exists(buffcontainer) || !pKernel->Exists(buffer))
	{
		return 0;
	}

	PERSISTID player = pKernel->Parent(buffcontainer);
	if (!pKernel->Exists(player))
	{
		return 0;
	}

	MapPropValue mapPropValue;

	//�Ƚ����buff�ĸ������԰�ˢ����������ȥ
	BufferModifyPackSingleton::Instance()->UpdateBufferModifyPack(pKernel, player, buffer, mapPropValue);

	// ����ֵ�ı�
	PropRefreshModule::m_pInstance->RefreshData(pKernel, player, mapPropValue, EREFRESH_DATA_TYPE_ADD);

	return 0;
}

// �Ƴ�buff
int BufferModifyPack::OnBuffRemove(IKernel* pKernel, const PERSISTID& buffcontainer, const PERSISTID& buffer, const IVarList& args)
{
	if (!pKernel->Exists(buffcontainer) || !pKernel->Exists(buffer))
	{
		return 0;
	}

	PERSISTID player = pKernel->Parent(buffcontainer);
	if (!pKernel->Exists(player))
	{
		return 0;
	}

	MapPropValue mapPropValue;

	//�Ƚ����buff�ĸ������԰�����������ȥ��
	BufferModifyPackSingleton::Instance()->RemoveBufferModifyPack(pKernel, player, buffer, mapPropValue);

	// �ı������ֵ�Ƴ�
	PropRefreshModule::m_pInstance->RefreshData(pKernel, player, mapPropValue, EREFRESH_DATA_TYPE_REMOVE);

	return 0;
}