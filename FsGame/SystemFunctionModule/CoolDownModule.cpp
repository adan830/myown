//--------------------------------------------------------------------
// �ļ���:      Server\FsGame\SkillModule\CoolDownModule.cpp
// ��  ��:      ��ȴϵͳ����
// ˵  ��:
// ��������:    2008/01/28
// ������:      �����
// �޸���:		  lixiongwen
//    :       
//--------------------------------------------------------------------
#include "CoolDownModule.h"

#include "utils/util_func.h"
#include "FsGame/Define/CoolDownDefine.h"
#include "../Define/ServerCustomDefine.h"
#include "utils/custom_func.h"
#include "utils/XmlFile.h"
#include "../CommonModule/LuaExtModule.h"
#include <algorithm>

#ifndef FSROOMLOGIC_EXPORTS
#include "CommonModule/ReLoadConfigModule.h"
#endif

const char* COOLDOWN_REC = "cooldown_rec";                  //��ȴ��

CoolDownModule* CoolDownModule::m_pInstance = NULL;

// ���ܣ����CD��
int nx_clear_cd_rec(void* state)
{
	IKernel* pKernel = LuaExtModule::GetKernel(state);

	// ����������
	CHECK_ARG_NUM(state, nx_clear_cd_rec, 1);
	// ����������
	CHECK_ARG_OBJECT(state, nx_clear_cd_rec, 1);

	// ��ȡ����
	PERSISTID self = pKernel->LuaToObject(state, 1);

	CoolDownModule::m_pInstance->ClearAllCD(pKernel, self);
	return 1;
}

// ��ȡ����
int nx_reload_cd_config(void* state)
{
	IKernel* pKernel = LuaExtModule::GetKernel(state);

	CoolDownModule::m_pInstance->LoadConfig(pKernel);

	return 1;
}

//��ʼ��
bool CoolDownModule::Init(IKernel* pKernel)
{
	m_pInstance = this;
	if (!LoadConfig(pKernel))
	{
		return false;
	}

	DECL_LUA_EXT(nx_clear_cd_rec);
	DECL_LUA_EXT(nx_reload_cd_config);

	pKernel->AddEventCallback("player", "OnStore", CoolDownModule::OnPlayerStore);

#ifndef FSROOMLOGIC_EXPORTS
	RELOAD_CONFIG_REG("CooldownConfig", CoolDownModule::ReloadConfig);
#endif // FSROOMLOGIC_EXPORTS
	return true;
}

//�ͷ�
bool CoolDownModule::Shut(IKernel* pKernel)
{
	return true;
}

CoolDownModule* CoolDownModule::Instance()
{
	return m_pInstance;
}
//////////////////////////////////////////////////////////////////////////
//��ʼһ����ȴ�������ȴ
bool CoolDownModule::BeginCoolDown(IKernel* pKernel, const PERSISTID& self,
								   int cooldowncategory, int cooldowntime )
{
    IGameObj* pSelfObj = pKernel->GetGameObj(self);
    if (NULL == pSelfObj)
    {
        return false;
    }

	if (cooldowntime <= 0)
	{
		return true;
	}

	if (pSelfObj->FindData("NoCoolDown") &&
		pSelfObj->QueryDataInt("NoCoolDown") > 0)
	{//GM���� �������߹رռ���CD
		return true;
	}

	IRecord* pRecordCool = pSelfObj->GetRecord(COOLDOWN_REC);
	if (NULL == pRecordCool)
	{
		return false;
	}

	//��ʼʱ��
	int64_t now = ::util_get_time_64();

	//����ʱ��
	int64_t endtime = now + cooldowntime;

	if (!InnerBeginCoolDown(pRecordCool, cooldowncategory, now, endtime))
	{
		return false;
	}

	return true;
}

/**
@brief        ����һ����ȴ�������ȴ
@param        ����
[IN]cooldowncategory:��ȴ������
@remarks
@return       ����ɹ���ʼ��ȴ������true�����򣬷���false
*/
bool CoolDownModule::EndCoolDown(IKernel* pKernel, const PERSISTID& self, int cooldowncategory)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return false;
	}

	IRecord* pRecordCool = pSelfObj->GetRecord(COOLDOWN_REC);
	if (NULL == pRecordCool)
	{
		return false;
	}

	//�Ƿ���ڸ�����ȴ����
	int row = pRecordCool->FindInt(COOLDOWN_REC_COL_ID, cooldowncategory);
	if (row < 0)
	{
		return false;
	}
	pRecordCool->RemoveRow(row);
	return true;
}

//ָ�������Ƿ�������ȴ��
bool CoolDownModule::IsCoolDown(IKernel* pKernel, const PERSISTID& self, int cooldowncategory)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return false;
	}

	IRecord* pRecord = pSelfObj->GetRecord(COOLDOWN_REC);

	if (pRecord == NULL)
	{
		return false;
	}

	// ���ڵ�ʱ��
	int64_t now = ::util_get_time_64();

	if (InnerIsCoolDown(pRecord, cooldowncategory, now) >= 0)
	{
		return true;
	}

	return false;
}

// ȡ����ȴ��ʣ������
int64_t CoolDownModule::GetMillisecRemain(IKernel* pKernel, const PERSISTID& self, int cooldowncategory)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return 0;
	}

    IRecord* pRecord = pSelfObj->GetRecord(COOLDOWN_REC);
    if (pRecord == NULL)
    {
        return 0;
    }

    // ���ڵ�ʱ��
    int64_t now = ::util_get_time_64();
    
    int row = pRecord->FindInt(COOLDOWN_REC_COL_ID, cooldowncategory);

    if (row < 0)
    {
        return 0;
    }

    //����ʱ��
    int64_t endtime = pRecord->QueryInt64(row, COOLDOWN_REC_COL_ENDTIME);
    
    return (endtime - now);
}

//���ȫ��CD
void CoolDownModule::ClearAllCD(IKernel* pKernel, const PERSISTID& self)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return;
	}

	IRecord* pRecordCool = pSelfObj->GetRecord(COOLDOWN_REC);
	if (NULL != pRecordCool)
	{
		pRecordCool->ClearRow();
	}
}

//�������CD
void CoolDownModule::ClearPlayerNormalSkillCD(IKernel* pKernel, const PERSISTID& self)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return;
	}

	IRecord* pRecordCool = pSelfObj->GetRecord(COOLDOWN_REC);
	if (NULL == pRecordCool)
	{
		return;
	}
	int nRows = pRecordCool->GetRows();
	LoopBeginCheck(a);
	for (int i = nRows - 1; i >= 0; i--)
	{
		LoopDoCheck(a);

		int nCategory = pRecordCool->QueryInt(i, COOLDOWN_REC_COL_ID);
		if (nCategory >= COOLDOWN_SKILL_BEGIN && nCategory <= COOLDOWN_PlAYER_NORMAL_SKILL)
		{
			pRecordCool->RemoveRow(i);
		}
	}
}

// �������
int CoolDownModule::OnPlayerStore(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return 0;
	}

	IRecord* pRecordCool = pSelfObj->GetRecord(COOLDOWN_REC);
	if (NULL == pRecordCool)
	{
		return 0;
	}
	int nRows = pRecordCool->GetRows();
	LoopBeginCheck(c)
	for (int i = nRows - 1; i >= 0; i --)
	{
		LoopDoCheck(c)
		int nCategory = pRecordCool->QueryInt(i, COOLDOWN_REC_COL_ID);
		// ����Ҫ�����CD,���ߺ����
		if (!m_pInstance->IsNeedSaveCategory(nCategory))
		{
			pRecordCool->RemoveRow(i);
		}
	}
	return 0;
}

//�����ȴ��Ϣ����ȴ��
bool CoolDownModule::InnerBeginCoolDown(IRecord* pRecordCool, const int& cooldowncategory,
										const int64_t& begin_time, const int64_t& end_time)
{
	//�����ȴ������ ����һ��
	if (pRecordCool->GetRows() >= pRecordCool->GetRowMax())
	{
		ClearCoolDownRec(pRecordCool);
	}

	//�Ƿ���ڸ�����ȴ����
	int row = pRecordCool->FindInt(COOLDOWN_REC_COL_ID, cooldowncategory);
	if (row < 0)
	{
		pRecordCool->AddRowValue(-1, CVarList() << cooldowncategory << begin_time << end_time);
	}
	else
	{
		pRecordCool->SetInt64(row, COOLDOWN_REC_COL_BEGINTIME, begin_time);
		pRecordCool->SetInt64(row, COOLDOWN_REC_COL_ENDTIME, end_time);
	}
	return true;
}


// ����Ƿ�������ȴ���������ȴ�����кţ����������ȴ����-1
int CoolDownModule::InnerIsCoolDown(IRecord* pRecordCool, const int cooldowncategory, const int64_t now)
{
	int row = pRecordCool->FindInt(COOLDOWN_REC_COL_ID, cooldowncategory);

	if (row < 0)
	{
		return -1;
	}

	//int n = pRecordCool->GetRows();
	//����ʱ��
	int64_t endtime = pRecordCool->QueryInt64(row, COOLDOWN_REC_COL_ENDTIME);

	if (now < endtime)
	{
		//��ȴ��
		return row;
	}
	else
	{
		//������ȴ�У�����ɾ����������(��ֹ��ɾ�������͵���ȴʱ����)
		if (cooldowncategory > COOLDOWN_CAREGOTYID_CHATTYPE_END)
		{
			pRecordCool->RemoveRow(row);
		}
		return -1;
	}
}


bool CoolDownModule::ClearCoolDownRec(IRecord* pRecord)
{
	if (pRecord == NULL)
	{
		return false;
	}
	int nRows = pRecord->GetRows();

	int64_t now = ::util_get_time_64();

	LoopBeginCheck(b);
	for (int i = nRows - 1; i >= 0; i --)
	{
		LoopDoCheck(b);

		int64_t endtime = pRecord->QueryInt64(i, COOLDOWN_REC_COL_ENDTIME);

		if (now >= endtime)
		{
			pRecord->RemoveRow(i);
		}
	}

	return true;
}

// ��ȡ����
bool CoolDownModule::LoadConfig(IKernel* pKernel)
{
	// �����ļ�·��
	std::string pathName = pKernel->GetResourcePath();
	pathName += "ini/Skill/Char/SaveCoolDownCategory.xml";
	CXmlFile xml(pathName.c_str());
	std::string errMsg;
	if (!xml.LoadFromFile(errMsg))
	{
		// ·�������ļ��޷���
		extend_warning(LOG_WARNING, errMsg.c_str());
		return false;
	}

	m_vSavedCategoryIds.clear();

	CVarList vSectionList;
	xml.GetSectionList(vSectionList);
	int nSize = (int)vSectionList.GetCount();
	m_vSavedCategoryIds.resize(nSize);
	LoopBeginCheck(c)
	for (int index = 0; index < nSize; ++index)
	{
		LoopDoCheck(c)
		const char* section = vSectionList.StringVal(index);
		if (NULL == section)
		{
			continue;
		}
		
		m_vSavedCategoryIds[index] = xml.ReadInteger(section, "Category", 0);
	}

	return true;
}

// �Ƿ���Ҫ�����CD����
bool CoolDownModule::IsNeedSaveCategory(int nCategory)
{
	bool bSave = false;
	std::vector<int>::iterator iter = find(m_vSavedCategoryIds.begin(), m_vSavedCategoryIds.end(), nCategory);
	if (iter != m_vSavedCategoryIds.end())
	{
		bSave = true;
	}

	return bSave;
}

void CoolDownModule::ReloadConfig(IKernel* pKernel)
{
	m_pInstance->LoadConfig(pKernel);
}