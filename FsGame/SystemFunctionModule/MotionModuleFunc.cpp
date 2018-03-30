//--------------------------------------------------------------------
// �ļ�    MotionModuleFunc.cpp
// ˵��	   ���ƶ�ģ�飬�ṩ���ʹ�õĺ���
//			��Ҫ��������
//--------------------------------------------------------------------
#include "MotionModule.h"
#include "FsGame/Define/GameDefine.h"
#include "utils/util_func.h"
#include "FsGame/NpcBaseModule/AI/AIFunction.h"


const int LITTLE_STEP_GRID = 1;

// Ѱ��Ŀ��㸽������ɢ��Χ
const float NEAR_DISTANCE_MAX = 30.0f; 

//************************************
// Method:    GetGridCanStandPoint
// Access:    public static 
// Returns:   int ��������
// Qualifier: ����Ŀ��㸽���Ŀ�ʹ������
// Parameter: IKernel * pKernel ������
// Parameter: const PERSISTID & self ����
// Parameter: float destX	Ŀ���x����
// Parameter: float destZ	Ŀ���z����
// Parameter: float & nearX Ŀ��㸽����ʹ��x����
// Parameter: float & nearZ Ŀ��㸽����ʹ��z����
//************************************
int MotionModule::GetEnableGridNearTarget(IKernel* pKernel, const PERSISTID& self, float destX, float destZ, float& nearX, float& nearZ)
{
	
	if (!pKernel->Exists(self))
	{
		return 0;
	}

	int distanceMax = static_cast<int>(NEAR_DISTANCE_MAX);
	int gridIndex = 0;
	// ѭ������
	LoopBeginCheck(eh);
	for (int i = 0; i < distanceMax; ++i)
	{
		LoopDoCheck(eh);
		// Χ��Ŀ��һȦһȦ����
		gridIndex = m_pMotionModule->GetNearestEnableGridInAllQuadrant(
			pKernel, self, destX, destZ, 
			static_cast<float>(i), 
			static_cast<float>(i + 1));

		// �ҵ����ʵ� ����
		if (gridIndex > 0)
		{
			break;
		}
	}

	if (gridIndex <= 0)
	{
		return 0;
	}

	// ת��Ϊ����
	pKernel->GetPosByIndex(gridIndex, nearX, nearZ);
	
	return gridIndex;
}

//�����Ƿ����
bool MotionModule::GridEnabled(IKernel * pKernel, const int iIndex, int groupId)
{
	if (iIndex <= 0)
	{
		return false;
	}

	//����
	bool canwalk, logicbalk, objectoccupy;
	pKernel->QueryPathGridStatus( iIndex, canwalk, logicbalk, objectoccupy, groupId);
	//��������
	if (!canwalk) 
	{
		return false;
	}

	//�����߼�ռ��
	if (logicbalk)
	{
		return false;
	}

	//������ռ��
	if (objectoccupy)
	{
		return false;
	}

	return true;
}

//  �����Ƿ���� 
//  �����self �Ƿ����
//  self��ռ��������ͬ�����ӵ�ռ�������ͬ
bool MotionModule::GridEnabled(IKernel * pKernel, const PERSISTID& self, const int iIndex, bool bSpace/* = true*/)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return false;
	}

	if (iIndex <= 0)
	{
		return false;
	}

	// ����ת������
	float indexx = 0.0f;
	float indexz = 0.0f;
	pKernel->GetPosByIndex(iIndex, indexx, indexz);

	// �Ƿ���� ������ռ�ø���
	CVarList gridList;
	bool ret = pKernel->CanStandPointByPathGrid(self, indexx, indexz, gridList) == MRESULT_CONTINUE;

	// �����߼�ռ��
	if (bSpace && ret)
	{
		ret = IsGridSpace(pKernel, self, gridList);
	}

	return ret;
}

// �໥��ļ��������ռ��
bool MotionModule::IsGridSpace(IKernel* pKernel, const PERSISTID& self, const IVarList& gridList)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return false;
	}

	// ��ռ����Ϊ��
	if (gridList.IsEmpty())
	{
		return true;
	}

	// ���������
	int spaceGrid = pSelfObj->QueryInt("SpaceGrid");
	if (spaceGrid <= 0)
	{
		return true;
	}

	// ��ռ��������-1Ϊ�˷���������㣩
	int pathGrid = pSelfObj->QueryInt("PathGrid") - 1;
	if (pathGrid < 0)
	{
		pathGrid = 0;
	}

	// ��ռ�������ϽǸ�������
	int gridLeftTop = gridList.IntVal(0);
	if (gridLeftTop <= 0)
	{
		return true;
	}

	// ��ͼ������
	size_t iTotalRow = pKernel->GetPathGridRows();
	if (iTotalRow == 0)
	{
		return false;
	}

	// ����ת��Ϊ���꣨���ǵ�ͼ���꣩
	size_t x = (gridLeftTop - 1) % iTotalRow + 1;
	size_t y = (gridLeftTop - 1) / iTotalRow;

	size_t tempX = x - spaceGrid;
	size_t tempY = y - spaceGrid;
	int tempIndex = 0;

	int groupId = pSelfObj->QueryInt("GroupID");

	// �ܱ�������
	// �ܸ����� - ����ռ�ø�����
	int totalCount = ((pathGrid + 1) + (spaceGrid * 2)) * ((pathGrid + 1) + (spaceGrid * 2)) - ((pathGrid + 1) * (pathGrid + 1));
	// ѭ������
	LoopBeginCheck(ej);
	do 
	{
		LoopDoCheck(ej);
		// ���ø����Ƿ����
		tempIndex = int(tempX + tempY * iTotalRow);
		if (!GridEnabled(pKernel, tempIndex, groupId))
		{
			return false;
		}

		// ����ռ�ø�����Χ�����и���
		// y��ĩβ
		if (tempY >= y + pathGrid + spaceGrid)
		{
			// x ��ĩβ �������
			if (tempX >= x + pathGrid + spaceGrid)
			{
				return true;
			}
			else
			{
				++tempX;
				tempY = y - spaceGrid;
			}
		}
		else
		{
			// ѭ������
			LoopBeginCheck(ei);
			do 
			{
				LoopDoCheck(ei);
				++tempY;
			}
			while (tempX >= x  && tempX <= x + pathGrid 
				&& tempY >= y && tempY <= y + pathGrid);
		}

		--totalCount;

	} while (totalCount >= 0);

	return false;
}