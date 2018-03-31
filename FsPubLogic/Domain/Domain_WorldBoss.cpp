//--------------------------------------------------------------------
// �ļ���:      Domain_WorldBoss.cpp
// ��  ��:      ����BOSS
// ˵  ��:		
// ��������:    2015��7��6��
// ������:      ������
// �޸���:        
// ��Ȩ����:   
//--------------------------------------------------------------------
#include <algorithm>
#include "Domain_WorldBoss.h"
#include "../../FsGame/Define/PubDefine.h"
#include "../../FsGame/Define/WorldBossNpcDefine.h"
#include "../../utils/extend_func.h"
#include "../../utils/util_func.h"
#include "../../utils/string_util.h"
#include "utils/record.h"

Domain_WorldBoss* Domain_WorldBoss::m_pDomainWorldBoss = NULL;

Domain_WorldBoss::Domain_WorldBoss() : IDomainData(L"", 0)
{
	 m_pDomainWorldBoss = this;
}

// ����
const std::wstring& Domain_WorldBoss::GetDomainName(IPubKernel* pPubKernel)
{
	if (m_domainName.empty())
	{
		wchar_t wstr[256];
		const int server_id = pPubKernel->GetServerId();
		SWPRINTF_S(wstr, L"Domain_WorldBoss_%d", server_id);

		m_domainName = wstr;
	}

	return m_domainName;
}


bool Domain_WorldBoss::GetSave()
{
	return true;
}

// ����
int Domain_WorldBoss::OnCreate(IPubKernel* pPubKernel, IPubSpace* pPubSpace)
{
	return OnLoad(pPubKernel, pPubSpace);
}

// ����
int Domain_WorldBoss::OnLoad(IPubKernel* pPubKernel, IPubSpace* pPubSpace)
{
	const std::wstring& name = GetDomainName(pPubKernel);

	IPubData* pPubData = pPubSpace->GetPubData(name.c_str());
	if (NULL == pPubData)
	{
		return 0;
	}

	// ��ɱboss���������
	CVarList col_types;
	col_types << VTYPE_INT << VTYPE_WIDESTR << VTYPE_INT;

	_create_record(pPubKernel, pPubData, PUB_KILL_WORLD_BOSS_PLAYER_NAME_REC, MAX_KWBP_PUB_ROW_NUM, col_types, true);

	// ����boss�ɳ����ݱ�
	col_types.Clear();
	col_types << VTYPE_INT << VTYPE_INT;

	_create_record(pPubKernel, pPubData, PUB_WORLD_BOSS_GROW_UP_REC, MAX_WORLD_BOSS_GROW_ROWS, col_types, true);

	/////*****************������--�״̬************************////// 
	col_types.Clear();
	col_types << VTYPE_INT << VTYPE_INT << VTYPE_INT;

	if (pPubData->FindRecord(WORLD_BOSS_ACTIVE_STATUS_REC))
	{
		// �����ݱ��������ԣ�ɾ���ɱ�
		if (pPubData->GetRecordCols(WORLD_BOSS_ACTIVE_STATUS_REC) != (int)col_types.GetCount())
		{
			pPubData->RemoveRecord(WORLD_BOSS_ACTIVE_STATUS_REC);
			pPubData->AddRecord(WORLD_BOSS_ACTIVE_STATUS_REC, 50, (int)col_types.GetCount(), col_types);
		}
	}
	else
	{
		pPubData->AddRecord(WORLD_BOSS_ACTIVE_STATUS_REC, 50, (int)col_types.GetCount(), col_types);
	}
	pPubData->SetRecordSave(WORLD_BOSS_ACTIVE_STATUS_REC, false);

	return 0;
}

// ��Ϣ����
int Domain_WorldBoss::OnMessage(IPubKernel* pPubKernel, IPubSpace* pPubSpace, int source_id, int scene_id, const IVarList& msg)
{
	const std::wstring & name = GetDomainName(pPubKernel);
	IPubData* pWorldBossData = pPubSpace->GetPubData(name.c_str());
	if (NULL == pWorldBossData)
	{
		return 0;
	}
		
	const int iCount = (int)msg.GetCount();
	if (iCount < 4)
	{
		return 0;
	}

	const int msg_type = msg.IntVal(2);
	if (SP_WORLD_BOSS_MSG_STATUS != msg_type)
	{
		return 0;
	}
	
	const int sub_msg_type = msg.IntVal(3);
	
	switch (sub_msg_type)
	{
		// ���û״̬
	case SP_WORLD_BOSS_ACTIVE_STATUS_SET:
		{
			int iSceneID = msg.IntVal(4);
			int iInStatus = msg.IntVal(5);
			int iID = msg.IntVal(6);

			IRecord* pRecord = pWorldBossData->GetRecord(WORLD_BOSS_ACTIVE_STATUS_REC);
			if (NULL == pRecord)
			{
				return false;
			} 
			const int iRow = pRecord->FindInt(WORLD_BOSS_STATUS_REC_COL_SCENEID, iSceneID);
			if (iRow > -1)
			{
				pRecord->SetInt(iRow, WORLD_BOSS_STATUS_REC_COL_STATUS, iInStatus);
				pRecord->SetInt(iRow, WORLD_BOSS_STATUS_REC_COL_ID, iID);
			}
			else
			{
				CVarList rowData;
				rowData << iSceneID << iInStatus << iID;
				pRecord->AddRowValue(-1, rowData);
			}
		}
		break;
		// ����BOSS����ɱ����
	case SP_WORLD_BOSS_BE_KILL_TIME_SET_GM:
		{
			int iSceneID = msg.IntVal(4);
			int iBeKillTime = msg.IntVal(5);

			IRecord* pRec = pWorldBossData->GetRecord(PUB_WORLD_BOSS_GROW_UP_REC);
			if (NULL == pRec)
			{
				return 0;
			}

			int iUpdateRow = pRec->FindInt(WORLD_BOSS_GROW_UP_REC_COL_SCENEID, iSceneID);

			if(iUpdateRow != -1)
			{
				pRec->SetInt(iUpdateRow, WORLD_BOSS_GROW_UP_REC_COL_LEVEL, iBeKillTime);
			}
			else
			{
				CVarList RowData;
				RowData << iSceneID << iBeKillTime;
				pWorldBossData->AddRecordRowValue(PUB_WORLD_BOSS_GROW_UP_REC, -1, RowData);
			}
		}
		break;

		// ����BOSS����ɱ����
	case SP_WORLD_BOSS_BE_KILL_TIME_SET:
		{
			int iSceneID = msg.IntVal(4);
			int iBeKillTime = msg.IntVal(5);
			int iMaxLevel = msg.IntVal(6);

			IRecord* pRecord = pWorldBossData->GetRecord(PUB_WORLD_BOSS_GROW_UP_REC);
			if (NULL == pRecord)
			{
				return 0;
			}
			int iUpdateRow = pRecord->FindInt(WORLD_BOSS_GROW_UP_REC_COL_SCENEID, iSceneID);
			if(iUpdateRow != -1)
			{
				int iOldVal = pRecord->QueryInt(iUpdateRow, WORLD_BOSS_GROW_UP_REC_COL_LEVEL);

				int save_val = iBeKillTime + iOldVal;
				if (save_val < 0)
				{
					save_val = 0;
				}

				save_val = __min(save_val, iMaxLevel);
				pRecord->SetInt(iUpdateRow, WORLD_BOSS_GROW_UP_REC_COL_LEVEL, save_val);
			}
			else
			{
				if (iBeKillTime < 0)
				{
					iBeKillTime = 0;
				}
				CVarList RowData;
				RowData << iSceneID << iBeKillTime;
				pRecord->AddRowValue(-1, RowData);
			}
		}
		break;
		// ��¼����boss�˺����а�
	case SP_UPDATE_BOSS_REC_INFO:
		{
			UpdateBossRecInfo(pWorldBossData, msg);
		}
		break;
	default:
		break;
	}

	return 0;
}

// ��¼����boss�˺����а�
void Domain_WorldBoss::UpdateBossRecInfo(IPubData* pWorldBossData, const IVarList & args)
{
	if (NULL == pWorldBossData)
	{
		return;
	}

	IRecord* pDamageRec = pWorldBossData->GetRecord(PUB_KILL_WORLD_BOSS_PLAYER_NAME_REC);
	if(NULL == pDamageRec)
	{
		return;
	}

	// ����������Ѿ����������
	int nSceneId = args.IntVal(4);
	const wchar_t* wsKillerName = args.WideStrVal(5); // bossδ��ɱ�����Ϊ��
	int nBossState = args.IntVal(6);
	int nRow = pDamageRec->FindInt(KWBP_PUB_COL_SCENE_ID, nSceneId);
	if (-1 == nRow)
	{
		pDamageRec->AddRowValue(-1, CVarList() << nSceneId << wsKillerName << nBossState);
	}
	else
	{
		pDamageRec->SetWideStr(nRow, KWBP_PUB_COL_KILLER_NAME, wsKillerName);
		pDamageRec->SetInt(nRow, KWBP_PUB_COL_BOSS_STATE, nBossState);
	}
}