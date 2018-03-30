//--------------------------------------------------------------------
// �ļ���:		AIBattleNpc.cpp
// ��  ��:		��սNPC����
// ˵  ��:		
// ��������:	2015��03��17��
// ������:		  
//    :	   
//--------------------------------------------------------------------
#include "AISystem.h"

#define BATTLE_TEAM_ROW 2
#define BATTLE_TEAM_COL 2

#define BATTLE_TEAM_NUM 4


// ����С��
bool AISystem::CreateBattleTeam(IKernel* pKernel, const PERSISTID& self, int iNation, int nTeamId, int iLevel)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return false;
	}

	// ��ȡ��������
	const BattleTeam* pTeamData = m_pAISystem->QueryTeamConfig(nTeamId);
	//��ȡ��ӦNPC����
	int iSceneID = pKernel->GetSceneId();
	//��սNPC����
	std::map<int, BattleNpcStruct>::iterator it = m_pAISystem->m_mapBattleNpcInfo.find(iSceneID);
	if (it == m_pAISystem->m_mapBattleNpcInfo.end() || NULL == pTeamData)
	{
		return false;
	}

	//��������ϻ�ȡ��Ӧ�����к�
	const char * TeamName = ("BattleTeamCampRec" + util_int_as_string(pTeamData->nCamp)).c_str();
	IRecord* TeamRec = pSelfObj->GetRecord(TeamName);
	if (TeamRec == NULL)
	{
		return false;
	}

	//���������
	int iAddRow = TeamRec->AddRow(-1);
	if (iAddRow < 0)
	{
		return false;
	}

	//��ȡ��Ӧ����Ӫ����
	std::map<int, std::map<int, BattlePos>>::iterator it2 = it->second.m_MapBattleNpc.find(pTeamData->nCamp);
	if (it2 == it->second.m_MapBattleNpc.end())
	{
		TeamRec->RemoveRow(iAddRow);
		return false;
	}

	//��ȡ��Ӧ���е���������
	std::map<int, BattlePos>::iterator it3 = it2->second.find(pTeamData->nBornPosIndex);
	if (it3 == it2->second.end())
	{
		TeamRec->RemoveRow(iAddRow);
		return false;
	}

	//����
	int iGroupId = -1; 
	if (pSelfObj->GetClassType() == TYPE_PLAYER)
	{
		iGroupId = pSelfObj->QueryInt("GroupID");
	}

	//�����ӳ� //���ң����飬�ȼ�
	CVarList args;
	args << CREATE_TYPE_BATTLELEAD_NPC << pTeamData->nMoveType << self << iAddRow << pTeamData->nCamp << pTeamData->nLifeTime;
	//�µ����ĵ�����
	float PosX = it3->second.fPosX;
	float PosZ = it3->second.fPosZ;
	int iGridIndex = pKernel->GetGridIndexByPos(PosX, PosZ);
	pKernel->GetPosByIndex(iGridIndex, PosX, PosZ);
	
	float fPosY = pKernel->GetMapHeight(PosX, PosZ);
	//�ȹ������ӳ�����
	const PERSISTID LeadNpc = pKernel->CreateObjectArgs("", it->second.BattleLeadNpc.c_str(), 0, PosX, fPosY, PosZ, it3->second.fOrient, args);
	if (!pKernel->Exists(LeadNpc))
	{
		TeamRec->RemoveRow(iAddRow);
		return false;
	}
	IGameObj* pNpc = pKernel->GetGameObj(LeadNpc);
	if (NULL == pNpc)
	{
		return false;
	}

	//������Ӫ
 	pNpc->SetInt("Camp", pTeamData->nCamp);

	//����
	pNpc->SetInt("Nation", iNation);
	//����
	pNpc->SetInt("GroupID", iGroupId);
	//�ȼ�
	pNpc->SetInt("Level", iLevel);

	//����������·����
	AISystem::m_pAISystem->SetBattleNpcPath(pKernel, LeadNpc, pTeamData->nCamp);
	//��������
    AISystem::m_pAISystem->GetAITemlate(pKernel, LeadNpc).BeginPatrol(pKernel, LeadNpc);

	int team_num = BATTLE_TEAM_NUM;

	// ��ȡ�������
	const BattleTeamFormation *formation = m_pAISystem->QueryTeamFormation(pKernel->GetSceneId());
	if (NULL != formation)
	{
		const int team_row = formation->row;
		const int team_col = formation->column;
		team_num = team_row * team_col;
	}

	//���������Ӷ���NPC
    LoopBeginCheck(a);
	for (int i = 1; i <= team_num; ++i)
	{
        LoopDoCheck(a);
		args.Clear();
		args << CREATE_TYPE_BATTLE_NPC << pTeamData->nMoveType << LeadNpc << i << iGroupId /*<< self*/;

		//�������������ƫ��λ��
		float OtherPosX = PosX;
		float OtherPosZ = PosZ;
		m_pAISystem->GetOtherNpcPos(pKernel, PosX, PosZ, i, OtherPosX, OtherPosZ);

		//�����������NPC
		PERSISTID npc = pKernel->CreateObjectArgs("", it->second.BattleNpc.c_str(), 0, OtherPosX, 
			pKernel->GetMapHeight(OtherPosX, OtherPosZ), OtherPosZ, it3->second.fOrient, args);

		pNpc = pKernel->GetGameObj(npc);
		if (NULL == pNpc)
		{
			continue;
		}
		//������Ӫ
 		pNpc->SetInt("Camp", pTeamData->nCamp);

		//����
		pNpc->SetInt("Nation", iNation);

		//�ȼ�
		pNpc->SetInt("Level", iLevel);

#ifndef FSROOMLOGIC_EXPORTS
		//AISystem::m_pCopyOffLineModule->CopyOffLineData(pKernel, npc);
#endif // FSROOMLOGIC_EXPORTS
	}
	return true;
}

//����������Ա��ƫ��λ��
bool AISystem::GetOtherNpcPos(IKernel* pKernel, float PosX, float PosZ, int iIndex, float& OtherPosX, float& OtherPosZ)
{
	int team_num = BATTLE_TEAM_NUM;
	int team_row = BATTLE_TEAM_ROW;
	int team_col = BATTLE_TEAM_COL;

	// ��ȡ�������
	const BattleTeamFormation *formation = m_pAISystem->QueryTeamFormation(pKernel->GetSceneId());
	if (NULL != formation)
	{
		team_row = formation->row;
		team_col = formation->column;
		team_num = team_row * team_col;
	}

	if (iIndex < 1 || iIndex > team_num)
	{
		return false;
	}
	//��ȡ��Ӧ�����ĵ�����
	int iGridIndex = pKernel->GetGridIndexByPos(PosX, PosZ);
	//�鿴��Ӧ����ƫ����
	int iDiffRow = (iIndex - 1) % team_row - 1;
	//��ƫ��
	int iDiffCol = (iIndex - 1) / team_col - 1;

	//�鿴���Ӷ�Ӧ������
	int iTotalRows = (int)pKernel->GetPathGridRows();
	int iTotalCols = (int)pKernel->GetPathGridCols();
	int iOtherGridIndex = iGridIndex + iDiffRow*team_row + iDiffCol*iTotalRows*team_col;
	if (iOtherGridIndex < 0 || iOtherGridIndex > iTotalRows*iTotalCols)
	{
		return false;
	}

	pKernel->GetPosByIndex(iOtherGridIndex, OtherPosX, OtherPosZ);
	return true;
}

//���ù�ս����·��
bool AISystem::SetBattleNpcPath(IKernel* pKernel, const PERSISTID& self, int nCamp)
{
	//��ȡ����
	IGameObj* selfObj = pKernel->GetGameObj(self);
	if (selfObj == NULL)
	{
		return false;
	}
	
	//�鿴���߲���
	int iMoveType = selfObj->QueryDataInt("MoveType");
	//��ȡ��Ӧ����·��Ϣ
	int iSceneID = pKernel->GetSceneId();
	std::map<int, MoveTypeDef>::iterator it = m_pAISystem->m_mapMoveTypeInfo.find(iSceneID);
	if (it == m_pAISystem->m_mapMoveTypeInfo.end())
	{
		return false;
	}

	//��ȡ��Ӫ����
	MoveTypeDef::iterator it2 = it->second.find(nCamp);
	if (it2 == it->second.end())
	{
		return false;
	}

	//��ȡ���߲���
	std::map<int, std::vector<MoveTypePos>>::iterator it3 = it2->second.find(iMoveType);
	if (it3 == it2->second.end())
	{
		return false;
	}

	CVarList arg_list;
	//����·������������
	int iCount = (int)it3->second.size();
	if (iCount > 0)
	{
		//��/�� ������->��Ŀ���
		len_t dx = it3->second[iCount - 1].fPosX;
		len_t dz = it3->second[iCount - 1].fPosZ;

		len_t fX = pKernel->GetPosiX(self);
		len_t fZ = pKernel->GetPosiZ(self);
		len_t fDis = util_dot_distance(fX, fZ, dx, dz);
		len_t fDis1;
        LoopBeginCheck(b);
		for( int i = 0; i < iCount; ++i )
		{
            LoopDoCheck(b);
			fDis1 = util_dot_distance( dx, dz, it3->second[i].fPosX, it3->second[i].fPosZ );
			if( fDis > fDis1 )
			{
				arg_list << it3->second[i].fPosX
					<< pKernel->GetMapHeight(it3->second[i].fPosX, it3->second[i].fPosZ) //GetFirstWalkHeight(pKernel, it3->second[i].fPosX, it3->second[i].fPosZ)
					<< it3->second[i].fPosZ;
			}
			else
			{
				continue;
			}
		}
	}
	AIFunction::CreateNpcPath(pKernel, self, arg_list);
	return true;
}