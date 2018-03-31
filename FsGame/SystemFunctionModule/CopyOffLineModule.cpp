//----------------------------------------------------------
// �ļ���:      CreateMirrorModule.h
// ��  ��:      ����������Ҿ���
// ˵  ��:		
// ��������:    2015��3��16��
// ������:       
// �޸���:    
//    :       
//----------------------------------------------------------
#include "CopyOffLineModule.h"
#include "utils\XmlFile.h"
#include "..\Define\PubDefine.h"
#include "utils\extend_func.h"
#include "utils\string_util.h"
#include "..\Define\SnsDefine.h"
#include "..\Interface\FightInterface.h"
#include "..\CommonModule\LuaExtModule.h"
#include "..\SystemFunctionModule\StaticDataQueryModule.h"
#include "..\Define\StaticDataDefine.h"
#include "utils\util_func.h"
#include "..\SkillModule\SkillToSprite.h"
#include "../Define/GameDefine.h"
#include "../Define/FightNpcDefine.h"
#include "../Define/CommandDefine.h"
#include "..\CommonModule\EnvirValueModule.h"
#include <algorithm>
#include "utils\util.h"

std::wstring			CopyOffLineModule::m_wsDomainName = L"";	// ��������
CopyOffLineModule*		CopyOffLineModule::m_pCopyOffLineModule		= NULL;

#define LEVEL_SECTION_NUM 21		// ÿ��ְҵ�ȼ������ø���
#define MAX_JOB_NUM 2				// ��ǰ��ְҵ

// ��ʼ��
bool CopyOffLineModule::Init(IKernel* pKernel)
{
	m_pCopyOffLineModule = this;

	Assert(NULL != m_pCopyOffLineModule);

	// ����PUB��������Ϣ
	pKernel->AddEventCallback("scene", "OnPublicMessage", CopyOffLineModule::OnPublicMessage);

	// ����SNS��������Ϣ
	pKernel->AddEventCallback("scene", "OnSnsMessage", CopyOffLineModule::OnSNSMessage);

	// ��ȡ����
	if (!LoadResource(pKernel))
	{
		return false;
	}
	return true;
}

// �ر�
bool CopyOffLineModule::Shut(IKernel* pKernel)
{
	return true;
}

/*!
 * @brief	���������������
 * @param	self	NPC����
 * @param   nLevel	�ȼ�
 * @return	int
 */
bool CopyOffLineModule::CopyOffLineData(IKernel* pKernel, const PERSISTID& self)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return false;
	}

	// ��ѯ��������������ҵ�uid ��ʽ��string "domain", wstring L"Domain_Offline_serverid", int msgid, int nLevel, int nSceneId, PERSISTID npc, int nGroupId
	int nSceneId = pKernel->GetSceneId();

	int nGroupId = -1;
	if (pSelfObj->GetClassType() == TYPE_PLAYER)
	{
		nGroupId = pSelfObj->QueryInt("GroupID");
	}
	int nNationId = pSelfObj->QueryInt("Nation");
	// �˵ȼ�Ϊ��ǰ��ҵĵȼ�
	int nLevel = (std::max)(pSelfObj->QueryInt("Level"), 1); // ��СΪ1��
	
	// ȥ�򹫹����������Ͳ�ѯ���uid����Ϣ
	CVarList pub_msg;
	pub_msg << PUBSPACE_DOMAIN;
	pub_msg << CopyOffLineModule::GetDomainName(pKernel).c_str();
	pub_msg << SP_OFFLINE_MSG_QUERY_PLAYER_UID;
	pub_msg << nLevel;
	pub_msg << nSceneId;
	pub_msg << self;
	pub_msg << nGroupId;
	pub_msg << nNationId;

	pKernel->SendPublicMessage(pub_msg);
	return true;
}

/*!
 * @brief	���������¼��
 * @param	int nGroupId
 * @return	void
 */
void CopyOffLineModule::ClearOfflinePubRec(IKernel* pKernel, int nGroupId)
{
	int nSceneId = pKernel->GetSceneId();
	// ȥ�򹫹����������Ͳ�ѯ���uid����Ϣ
	CVarList pub_msg;
	pub_msg << PUBSPACE_DOMAIN;
	pub_msg << CopyOffLineModule::GetDomainName(pKernel).c_str();
	pub_msg << SP_OFFLINE_MSG_CLEAR_GROUP_REC;
	pub_msg << nSceneId;
	pub_msg << nGroupId;

	pKernel->SendPublicMessage(pub_msg);
}

/*!
 * @brief	���ĳһ����¼�������
 * @param	int nGroupId
 * @param	PERSISTID self
 * @return	void
 */
void CopyOffLineModule::ClearOfflinePubRecOneRow(IKernel* pKernel, int nGroupId, const PERSISTID& self)
{
	int nSceneId = pKernel->GetSceneId();
	// ȥ�򹫹����������Ͳ�ѯ���uid����Ϣ
	CVarList pub_msg;
	pub_msg << PUBSPACE_DOMAIN;
	pub_msg << CopyOffLineModule::GetDomainName(pKernel).c_str();
	pub_msg << SP_OFFLINE_MSG_CLEAR_GROUP_REC_ONE_ROW;
	pub_msg << nSceneId;
	pub_msg << nGroupId;
	pub_msg << self;

	pKernel->SendPublicMessage(pub_msg);
}

/*!
 * @brief	���������������ͨ�����uid
 * @param	self	NPC����
 * @param	strUid	���uid
 * @return	bool
 */
bool CopyOffLineModule::CopyPlayerDataByUid(IKernel* pKernel, const PERSISTID& self, const char* strUid)
{
	if (!pKernel->Exists(self) || pKernel->Type(self) != TYPE_NPC)
	{
		return false;
	}
	if (!StringUtil::CharIsNull(strUid))
	{
		CVarList arg_list;
		int iDistribute_id = pKernel->GetDistrictId();
		int iServer_id = pKernel->GetServerId();
		int iMember_id = pKernel->GetMemberId();
		int iscene_id = pKernel->GetSceneId();
		arg_list << SPACE_SNS << L"OffLineDelay" << SNS_MSG_QUERY_PLAYER_FIGHT_DATA << self;
		pKernel->SendSnsMessage(iDistribute_id, iServer_id, iMember_id, iscene_id, strUid, arg_list);
	}
	return true;
}

// ��������
bool CopyOffLineModule::LoadResource(IKernel* pKernel)
{
	// ��ȡ������Ϣ����
	if (!LoadDataInfoConfig(pKernel))
	{
		return false;
	}
	// ��ȡĬ������
	if (!LoadDefaultName(pKernel))
	{
		return false;
	}

	return true;
}

// ��ȡ������Ϣ����
bool CopyOffLineModule::LoadDataInfoConfig(IKernel* pKernel)
{
	// �����ļ�·��
	std::string pathName = pKernel->GetResourcePath();
	pathName += "ini/OffLine/MirrorPorperty.xml";

	CXmlFile xmlfile(pathName.c_str());
	if (!xmlfile.LoadFromFile())
	{
		std::string info = "Not Exist " + pathName;
		::extend_warning(LOG_ERROR, info.c_str());
		return false;
	}
	
	const int iSectionCount = (int)xmlfile.GetSectionCount();
	m_vDataInfo.reserve(iSectionCount);
	LoopBeginCheck(a)
	for (int i = 0; i < iSectionCount; i++)
	{
		LoopDoCheck(a)
		const char* strSectionName = xmlfile.GetSectionByIndex(i); 
		// ��һ������Ϊϵ����������
		if (0 == i)
		{
			m_kRatioInfo.nLevelDiff		= xmlfile.ReadInteger(strSectionName, "LevelDiff", 0);
			m_kRatioInfo.fArenaRatio	= xmlfile.ReadFloat(strSectionName, "ArenaModifyRatio", 1.0f);
			const char* strRatio		= xmlfile.ReadString(strSectionName, "ModifyRatio", "");
			const char* strDCRatio		= xmlfile.ReadString(strSectionName, "DCChangeRatio", "");

			// ����npc�ȼ���ϵ����ֵ��ȡ
			CVarList args;
			StringUtil::SplitString(args, strRatio, ",");
			int nCount = (int)args.GetCount();
			if (MAX_RATIO_NUM != nCount)
			{
				::extend_warning(LOG_ERROR, "Config %s Section 0 ModifyRatio is error", pathName.c_str());
				return false;
			}
			LoopBeginCheck(b)
			for (int j = 0;j < MAX_RATIO_NUM;++j)
			{
				LoopDoCheck(b)
				m_kRatioInfo.fRatio[j] = args.FloatVal(j);
			}
			// Ĭ�����Ա仯��
			args.Clear();
			StringUtil::SplitString(args, strDCRatio, ",");
			nCount = (int)args.GetCount();
			if (2 != nCount)
			{
				::extend_warning(LOG_ERROR, "Config %s Section 0 DCChangeRatio is error", pathName.c_str());
				return false;
			}
			m_kRatioInfo.fMinDCRatio = args.FloatVal(0);
			m_kRatioInfo.fMaxDCRatio = args.FloatVal(1);
		}
		else
		{
			MirDataInfo info;
			info.nType		= xmlfile.ReadInteger(strSectionName, "Type", 0);
			info.strName	= xmlfile.ReadString(strSectionName, "DataName", "");
			info.bIsModify	= xmlfile.ReadInteger(strSectionName, "IsModify", 0) == 1;
			info.bIsLoadConfig = xmlfile.ReadInteger(strSectionName, "IsLoadConfig", 0) == 1;
			if (DATA_CONTAINER_TYPE == info.nType)
			{
				info.strSnsTable = xmlfile.ReadString(strSectionName, "SnsTable", "");
			}

			m_vDataInfo.push_back(info);
		}
	}

	return true;
}

// ��ȡĬ�ϵ���������
bool CopyOffLineModule::LoadDefaultName(IKernel* pKernel)
{
	// �����ļ�·��
	std::string pathName = pKernel->GetResourcePath();
	pathName += "ini/SystemFunction/OffLine/MirrorNameLib.xml";

	CXmlFile xmlfile(pathName.c_str());
	if (!xmlfile.LoadFromFile())
	{
		std::string info = "Not Exist " + pathName;
		::extend_warning(LOG_ERROR, info.c_str());
		return false;
	}

	const int iSectionCount = (int)xmlfile.GetSectionCount();
	m_vDefaultName.reserve(iSectionCount);
	LoopBeginCheck(c);
	for (int i = 0; i < iSectionCount; i++)
	{
		LoopDoCheck(c);
		const char* strSectionName = xmlfile.GetSectionByIndex(i); 
		std::wstring wsName	= StringUtil::StringAsWideStr(xmlfile.ReadString(strSectionName, "Name", ""));
		m_vDefaultName.push_back(wsName);
	}

	return true;
}

// ���ȡ����ͬһ�������
bool CopyOffLineModule::FindNameList(int nNameNum, std::vector<std::wstring>& vecOutNameList)
{
	if (nNameNum <= 0)
	{
		return false;
	}
	vecOutNameList.clear();
	int nSize = (int)m_vDefaultName.size();
	nNameNum = __min(nSize, nNameNum);
	std::vector<int> vIndexList;
	util_different_Random_int(0, nSize, nNameNum, vIndexList);

	int nIndexSize = vIndexList.size();
	vecOutNameList.reserve(nIndexSize);
	LoopBeginCheck(y);
	for (int i = 0; i < nIndexSize; ++i)
	{
		LoopDoCheck(y);
		int nIndex = vIndexList[i];
		if (nIndex < 0 || nIndex >= nSize)
		{
			continue;
		}
		vecOutNameList.push_back(m_vDefaultName[nIndex]);
	}

	return true;
}

// ���������ݷ������´�����Ϣ
int CopyOffLineModule::OnPublicMessage(IKernel* pKernel, const PERSISTID& self,
	const PERSISTID& sender, const IVarList& args)
{
	/*
        ��Ϣ��ʽ
            [sub_msg_id][...]
    */
    int main_msg_id = args.IntVal(2);
    if (main_msg_id != PS_OFFLINE_MSG_FINDED_PLAYER_UID)
    {
        return 0;
    }
	// ����ѯ�����uid
	const char* strUid = args.StringVal(3);

	// ����ֵ���ݵ�npc����
	PERSISTID npc = args.ObjectVal(4);
	if (!StringUtil::CharIsNull(strUid))
	{
		CVarList arg_list;
		int iDistribute_id = pKernel->GetDistrictId();
		int iServer_id = pKernel->GetServerId();
		int iMember_id = pKernel->GetMemberId();
		int iscene_id = pKernel->GetSceneId();
		arg_list << SPACE_SNS << L"OffLineDelay" << SNS_MSG_QUERY_PLAYER_FIGHT_DATA << npc;
		pKernel->SendSnsMessage(iDistribute_id, iServer_id, iMember_id, iscene_id, strUid, arg_list);
	}
	return 0;
}

// ����SNS�������´�����Ϣ
int CopyOffLineModule::OnSNSMessage(IKernel* pKernel, const PERSISTID& self,
	const PERSISTID& sender, const IVarList& args)
{
	int msgid = args.IntVal(0);
	if (SNS_MSG_QUERY_PLAYER_FIGHT_DATA != msgid)
	{
		return 0;
	}
	
	PERSISTID npc = args.ObjectVal(1);
	IGameObj* pNpcObj = pKernel->GetGameObj(npc);
	if (NULL == pNpcObj)
	{
		return 0;
	}
	// ��������˳��ȡ������,������npc��
	int nIndex = 2;

	// �����һ������ֵΪ��Ч ���϶���ȡʧ�� ����Ĭ������
	int nDataType = args.GetType(nIndex);
	// �Ƿ�Ϊ��Чֵ
	if (args.GetCount() <= 3 || VTYPE_INT == nDataType && NOT_VALID_VALUE == args.IntVal(nIndex))
	{
		return 0;
	}
	
	// ��������ǰ�ȼ�Ϊ��ҵĵȼ�
	int nSelfLevel = pNpcObj->QueryInt("Level");

	int nSize = (int)m_pCopyOffLineModule->m_vDataInfo.size();
	LoopBeginCheck(e)
	for (int i = 0;i < nSize;++i)
	{
		LoopDoCheck(e)
		MirDataInfo& info = m_pCopyOffLineModule->m_vDataInfo[i];
		// �����������
		if (DATA_PROPERTY_TYPE == info.nType)
		{
			m_pCopyOffLineModule->CopyPropertyData(pKernel, pNpcObj, info.strName.c_str(), args, nIndex);
		}
		else if(DATA_CONTAINER_TYPE == info.nType) // �����������
		{
			m_pCopyOffLineModule->CopyContainerData(pKernel, pNpcObj, info.strName.c_str(), args, nIndex);
		}
		else // ��������
		{
			m_pCopyOffLineModule->CopyRecordData(pKernel, pNpcObj, info.strName.c_str(), args, nIndex);
		}
	}

	CopyOffLineModule::m_pCopyOffLineModule->OnAferCopyData(pKernel, pNpcObj, nSelfLevel);
	return 0;
}

// ������������
bool CopyOffLineModule::CopyPropertyData(IKernel* pKernel, IGameObj* pSelfObj, const char* strName, const IVarList& data, int& nIndex)
{
	if (NULL == pSelfObj)
	{
		++nIndex;
		return false;
	}
	int nDataType = data.GetType(nIndex);
	// �Ƿ�Ϊ��Чֵ ���Դ���
	if (VTYPE_INT == nDataType && NOT_VALID_VALUE == data.IntVal(nIndex))
	{
		++nIndex;
		return false;
	}

	// ����û�ҵ� ���Դ��� ��ֹ�߻����ô���
	if (!pSelfObj->FindAttr(strName))
	{
		++nIndex;
		return false;
	}

	// �����͸�ֵ����
	int nAttrType = pSelfObj->GetAttrType(strName);
	if (nAttrType != nDataType)
	{
		++nIndex;
		return false;
	}
	switch(nAttrType)
	{
	case VTYPE_INT:
		{
			int nValue = data.IntVal(nIndex);
			pSelfObj->SetInt(strName, nValue);
		}
		break;
	case VTYPE_INT64:
		{
			int64_t nValue = data.Int64Val(nIndex);
			pSelfObj->SetInt64(strName, nValue);
		}
		break;
	case VTYPE_FLOAT:
		{
			float fValue = data.FloatVal(nIndex);
			pSelfObj->SetFloat(strName, fValue);
		}
		break;
	case VTYPE_STRING:
		{
			std::string strValue = data.StringVal(nIndex);
			pSelfObj->SetString(strName, strValue.c_str());
		}
		break;
	case VTYPE_WIDESTR:
		{
			std::wstring wsValue = data.WideStrVal(nIndex);
			pSelfObj->SetWideStr(strName, wsValue.c_str());
		}
		break;
	}

	++nIndex;
	return true;
}

// ������������
bool CopyOffLineModule::CopyContainerData(IKernel* pKernel, IGameObj* pSelfObj, const char* strName, const IVarList& data, int& nIndex)
{
	bool bResult = false;
	// ������������
	if (0 == strcmp(strName, "SkillContainer"))
	{
		bResult = CopySkillContainerData(pKernel, pSelfObj, strName, data, nIndex);
	}
	else if (0 == strcmp(strName, "PetBox"))
	{
		//bResult = CopyPetContainerData(pKernel, pSelfObj, strName, data, nIndex);
	}

	return bResult;
}

// ����������������
bool CopyOffLineModule::CopySkillContainerData(IKernel* pKernel, IGameObj* pSelfObj, const char* strName, const IVarList& data, int& nIndex)
{
	int nRows = data.IntVal(nIndex++);
	int nCols = data.IntVal(nIndex++);
	// ���ݲ�����
	if (NULL == pSelfObj || 0 == nRows || MAX_SSR_SKILL_COLS != nCols)
	{
		return false;
	}
	
	// ����ռ���
	SKillToSpriteSingleton::Instance()->ClearAllSkill(pKernel, pSelfObj->GetObjectId());
	
	LoopBeginCheck(g)
	// ���������������
	for (int i = 0;i < nRows;++i)
	{
		LoopDoCheck(g)
		const char* strSkill = data.StringVal(nIndex++);
		int nLevel = data.IntVal(nIndex++);

		PERSISTID skill = FightInterfaceInstance->AddSkill(pKernel, pSelfObj->GetObjectId(), strSkill);
		if (skill.IsNull())
		{
			continue;
		}
		pKernel->SetInt(skill, "Level", nLevel);
	}

	return true;
}

// ����������������
// bool CopyOffLineModule::CopyPetContainerData(IKernel* pKernel, IGameObj* pSelfObj, const char* strName, const IVarList& data, int& nIndex)
// {
// 	int nRows = data.IntVal(nIndex++);
// 	int nCols = data.IntVal(nIndex++);
// 	// ���ݲ�����
// 	if (NULL == pSelfObj || 0 == nRows)
// 	{
// 		return false;
// 	}
// 	// ֻ�о�����npc ��Ҫ������������
// 	if (ARENA_BATTLE_NPC != pSelfObj->QueryInt("PurposeType"))
// 	{
// 		return false;
// 	}
// 
// 	//������������
// 	PERSISTID petBox = pKernel->CreateContainer(pSelfObj->GetObjectId(), PET_BOX_NAME, PetConfigManage::GetMaxPetNum());
// 	if (petBox.IsNull())
// 	{
// 		return false;
// 	}
// 	pKernel->SetWideStr(petBox, "Name", PET_BOX_NAME_W);
// 
// 	LoopBeginCheck(h)
// 	// �����������
// 	for (int i = 0;i < nRows;++i)
// 	{
// 		LoopDoCheck(h)
// 		const char* strPetId = data.StringVal(nIndex++);
// 		PERSISTID pet = PetModule::AddPet(pKernel, pSelfObj->GetObjectId(), strPetId);
// 		IGameObj* pPetObj = pKernel->GetGameObj(pet);
// 		if (NULL == pPetObj)
// 		{
// 			continue;
// 		}
// 		// ���ó��������ֵ
// 		// �ȼ�
// 		int nTmpVal = data.IntVal(nIndex++);
// 		pPetObj->SetInt("Level", nTmpVal);
// 		// �Ǽ�
// 		nTmpVal = data.IntVal(nIndex++);
// 		pPetObj->SetInt("StarLevel", nTmpVal);
// 		// �׼�
// 		nTmpVal = data.IntVal(nIndex++);
// 		pPetObj->SetInt("StepLevel", nTmpVal);
// 		// ��ս״̬
// 		nTmpVal = data.IntVal(nIndex++);
// 		pPetObj->SetInt("Work", nTmpVal);
// 		// ����
// 		nTmpVal = data.IntVal(nIndex++);
// 		pPetObj->SetInt(FIELD_PROP_PET_SCORE, nTmpVal);
// 		// �������Ը���
// 		PetModule::UpdateMasterWorkAP(pKernel, pSelfObj->GetObjectId(), pet);
// 		PetModule::UpdatePetNpcAttribute(pKernel, pet);
// 		PetModule::UpdateSkillActivate(pKernel, pSelfObj->GetObjectId(), pet);
// 
// 		// �������Ը���
// 		PetModule::PetHelpOnOrOff(pKernel, pSelfObj->GetObjectId(), strPetId, true);
// 
//  		const char* strPetSkillData = data.StringVal(nIndex++);
//  		SetPetSkillData(pKernel, pSelfObj, pet, strPetSkillData);
// 
// 		const char* strPetTalent = data.StringVal(nIndex++);
// 		SetPetTalentData(pKernel, pSelfObj, pet, strPetTalent);
// 	}
// 
// 	const int pet_num = PetModule::GetPetAll(pKernel, pSelfObj->GetObjectId(), CVarList());
// 	PetModule::UpdateRecruitPck(pKernel, pSelfObj->GetObjectId(), 0, pet_num);
// 	
// 	return true;
// }

// ����������
bool CopyOffLineModule::CopyRecordData(IKernel* pKernel, IGameObj* pSelfObj, const char* strName, const IVarList& data, int& nIndex)
{
	int nRows = data.IntVal(nIndex++);
	int nCols = data.IntVal(nIndex++);
	// ���ݲ�����
	if (NULL == pSelfObj || 0 == nRows)
	{
		return false;
	}
	IRecord* pRecord = pSelfObj->GetRecord(strName);
	// ������
	if (NULL == pRecord || nCols > pRecord->GetCols())
	{
		return false;
	}
	// ��sns�������������
	CVarList recdata;
	LoopBeginCheck(m)
	for (int i = 0;i < nRows;++i)
	{
		LoopDoCheck(m)
		recdata.Clear();
		LoopBeginCheck(n)
		for (int j = 0;j < nCols; ++j)
		{
			LoopDoCheck(n)
			int nColType = pRecord->GetColType(j);
			if(data.GetType(nIndex) != nColType)
			{
				continue;
			}
			switch(nColType)
			{
			case VTYPE_INT:
				recdata << data.IntVal(nIndex++);
				break;
			case VTYPE_INT64:
				recdata << data.Int64Val(nIndex++);
				break;
			case VTYPE_FLOAT:
				recdata << data.FloatVal(nIndex++);
				break;
			case VTYPE_STRING:
				recdata << data.StringVal(nIndex++);
				break;
			case VTYPE_WIDESTR:
				recdata << data.WideStrVal(nIndex++);
				break;
			}
		}

		pRecord->AddRowValue(-1, recdata);
	}
	return true;
}

// ���ó��＼������
// void CopyOffLineModule::SetPetSkillData(IKernel* pKernel, IGameObj* pSelfObj, const PERSISTID& petitem, const char* strSkillData)
// {
// 	if (NULL == pSelfObj || !pKernel->Exists(petitem) || StringUtil::CharIsNull(strSkillData))
// 	{
// 		return;
// 	}
// 	CVarList skillArray;
// 	StringUtil::SplitString(skillArray, strSkillData, ",");
// 	// �������г��＼�ܵȼ�
// 	LoopBeginCheck(i)
// 	int nSkillNum = (int)skillArray.GetCount();
// 	for (int i = 0;i < nSkillNum;++i)
// 	{
// 		LoopDoCheck(i)
// 		const char* strData = skillArray.StringVal(i);
// 		CVarList data;
// 		StringUtil::SplitString(data, strData, ":");
// 		// ���ݸ�ʽ: ����id:���ܵȼ�
// 		if (data.GetCount() != 2)
// 		{
// 			continue;
// 		}
// 		const char* strSkillId = data.StringVal(0);
// 		int nSkillLevel = data.IntVal(1);
// 		// ��ȡ���＼��,�����ü��ܵȼ�
// 		PERSISTID skill = PetModule::GetPetSkill(pKernel, petitem, strSkillId);
// 		if(skill.IsNull())
// 		{
// 			return;
// 		}
// 		PetModule::SetPetSkillLevel(pKernel, pSelfObj->GetObjectId(), petitem, skill, nSkillLevel);
// 	}	
// }
// 
// // ���ó����츳����
// void CopyOffLineModule::SetPetTalentData(IKernel* pKernel, IGameObj* pSelfObj, const PERSISTID& petitem, const char* strTalentData)
// {
// 	if (NULL == pSelfObj || !pKernel->Exists(petitem) || StringUtil::CharIsNull(strTalentData))
// 	{
// 		return;
// 	}
// 
// 	CVarList skillArray;
// 	StringUtil::SplitString(skillArray, strTalentData, ",");
// 	// �������г����츳����
// 	LoopBeginCheck(i);
// 	int nSkillNum = (int)skillArray.GetCount();
// 	for (int i = 0; i < nSkillNum; ++i)
// 	{
// 		LoopDoCheck(i);
// 		const char* strData = skillArray.StringVal(i);
// 	
// 		PetModule::AddTalentOnIdleSlot(pKernel, pSelfObj->GetObjectId(), petitem, strData);
// 	}
// }

// ��ȡdomain������
const std::wstring& CopyOffLineModule::GetDomainName(IKernel * pKernel)
{
	if (m_wsDomainName.empty())
	{
		wchar_t wstr[256];
		const int server_id = pKernel->GetServerId();
		SWPRINTF_S(wstr, L"Domain_Offline_%d", server_id);

		m_wsDomainName = wstr;
	}

	return m_wsDomainName;
}

// �������ݺ�Ĳ���
void CopyOffLineModule::OnAferCopyData(IKernel* pKernel, IGameObj* pSelfObj, int nPlayerLevel)
{
	if (NULL == pSelfObj)
	{
		return;
	}
	
	float fModifyRatio = GetPropertyRatio(pKernel, pSelfObj, nPlayerLevel);

	int nSize = (int)m_pCopyOffLineModule->m_vDataInfo.size();
	LoopBeginCheck(l)
	for (int i = 0;i < nSize;++i)
	{
		LoopDoCheck(l)
		MirDataInfo& info = m_pCopyOffLineModule->m_vDataInfo[i];
		// ����ϵ���޸���������
		if (DATA_PROPERTY_TYPE == info.nType && info.bIsModify)
		{
			int nDataType = pSelfObj->GetAttrType(info.strName.c_str()); 
			if (VTYPE_INT == nDataType)
			{
				int nSrcVal = pSelfObj->QueryInt(info.strName.c_str());
				int nCurVal = (int)(fModifyRatio * (float)nSrcVal);
				pSelfObj->SetInt(info.strName.c_str(), nCurVal);
			}
			else if (VTYPE_FLOAT == nDataType)
			{
				float  fSrcVal = pSelfObj->QueryFloat(info.strName.c_str());
				float fCurVal = fModifyRatio * fSrcVal;
				pSelfObj->SetFloat(info.strName.c_str(), fCurVal);
			}	
		}	
	}

	// ��ȡ�������ʵ������,������npc����buff
// 	const char* buffID = m_pCopyOffLineModule->GetLevelBuffer(nPlayerLevel);
// 	if( !StringUtil::CharIsNull(buffID) && ARENA_BATTLE_NPC != pSelfObj->QueryInt("PurposeType"))
// 	{
// 		FightInterfaceInstance->AddBuffer(pKernel, pSelfObj->GetObjectId(),  pSelfObj->GetObjectId(), buffID);
// 	}

	// ����ʱ��Ѫ
	int64_t nMaxHP = pSelfObj->QueryInt64("MaxHP");
	pSelfObj->SetInt64("HP", nMaxHP);
	// ����Ϊ��ҵ����߸߶Ȳ�
	float fWalkStep = EnvirValueModule::EnvirQueryFloat(ENV_VALUE_PLAYER_WALK_STEP_DEFAULT);
	pSelfObj->SetFloat("WalkStep", fWalkStep);

	// ���ù�������
	float fMinDis = 0.0f;
	float fMaxDis = 0.0f;
	FightInterfaceInstance->ReadyUseRandomSkill(pKernel, pSelfObj->GetObjectId(), fMinDis, fMaxDis);

	// ��ֹ��������Ϊ0,������
	fMaxDis = __max(2, fMaxDis);

	pSelfObj->SetFloat("MinAttackDist", fMinDis);
	pSelfObj->SetFloat("MaxAttackDist", fMaxDis);
	
	// ���ó���������
	pSelfObj->SetFloat("BornX", pSelfObj->GetPosiX());
	pSelfObj->SetFloat("BornZ", pSelfObj->GetPosiZ());
	pSelfObj->SetInt("Invisible", 0);
	pSelfObj->SetInt("CantAttack", 0);
	pSelfObj->SetInt("CantBeAttack", 0);

	// �����ƶ��ٶ�
	float fRunSpeed = EnvirValueModule::EnvirQueryFloat(ENV_VALUE_DEFAULT_RUN_SPEED);
	pSelfObj->SetFloat("RunSpeed", fRunSpeed);

	// �����������ݵ�������ܻ����MinAttack > MaxAttack�����, ��������
	int nMaxAttack = pSelfObj->QueryInt("MaxAttack");
	int nMinAttack = pSelfObj->QueryInt("MinAttack");
	if (nMinAttack > nMaxAttack)
	{
		pSelfObj->SetInt("MaxAttack", nMinAttack);
	}

// 	if (ARENA_BATTLE_NPC == pSelfObj->QueryInt("PurposeType"))
// 	{
// 		PetModule::RestoreWorkPetNpcAll(pKernel, pSelfObj->GetObjectId(), true);
// 	}

	// �����ڲ���Ϣ
	pKernel->Command(pSelfObj->GetObjectId(), pSelfObj->GetObjectId(), CVarList() << COMMAND_OFFLINE_DATA_COPY_FINISH);
}

// ��ȡ���Ա仯ϵ��
const float CopyOffLineModule::GetPropertyRatio(IKernel* pKernel, IGameObj* pSelfObj, int nPlayerLevel)
{
	if (NULL == pSelfObj)
	{
		return 0.0f;
	}
	int nPurpose = NORMAL_BATTLE_NPC;
	if (pSelfObj->FindAttr("PurposeType"))
	{
		nPurpose = pSelfObj->QueryInt("PurposeType");
	}
	// ������npc��ʵ��ϵ��
	if (ARENA_BATTLE_NPC == nPurpose)
	{
		return m_kRatioInfo.fArenaRatio;
	}

	int nSelfLevel = pSelfObj->QueryInt("Level");
	int nLevelDiff = nSelfLevel - nPlayerLevel;
	int nRatioType = GREATER_THAN_RATIO;
	if (nLevelDiff > m_kRatioInfo.nLevelDiff)
	{
		nRatioType = GREATER_THAN_RATIO;
	}
	else if (nLevelDiff <= m_kRatioInfo.nLevelDiff && nLevelDiff > 0)
	{
		nRatioType = LESS_THAN_RATIO;
	}
	else if(nLevelDiff <= 0)
	{
		nRatioType = LESS_THAN_ZERO_RATIO;
	}

	return m_kRatioInfo.fRatio[nRatioType];
}

// ����Ĭ�����Ա仯��
const float CopyOffLineModule::ComputeDCRatio()
{
	// ��ȡ����ı仯��
	float fRatio = m_kRatioInfo.fMinDCRatio + ::util_random_float(m_kRatioInfo.fMaxDCRatio - m_kRatioInfo.fMinDCRatio);
	return fRatio;
}