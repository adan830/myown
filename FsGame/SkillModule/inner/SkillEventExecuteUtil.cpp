//--------------------------------------------------------------------
// �ļ���:      skill_event_execute_util.h
// ��  ��:      ����Ч���Ĺ��ܺ���
// ˵  ��:
// ��������:    2014��10��30��
// ������:       
//    :       
//--------------------------------------------------------------------

#include "FsGame/Define/SkillDefine.h"
#include "utils/geometry_func.h"
#include "utils/math/vector.h"
#include "FsGame/SkillModule/inner/SkillEventExecuteUtil.h"
#include "../../Interface/PropertyInterface.h"
#include "../Impl/Skillaccess.h"
#include "../../Define/FightPropertyDefine.h"
#include "../Impl/TargetFilter.h"
#include "../FightModule.h"
#include "utils/custom_func.h"
#include "utils/util_func.h"
#include "utils/math/fm_fixedpoint.h"
#include "../Impl/SkillDefineDataStruct.h"
#include "../Impl/SkillDataQueryModule.h"
#include "../../NpcBaseModule/AI/AIDefine.h"
#include "../Impl/SkillFlow.h"
#include "../../Interface/FightInterface.h"
#include "Define/Classes.h"
#include "Define/Fields.h"

#define TRACE_LINE_WALK_STEP 4.0f

namespace SkillEventExecuteUtil
{
	bool LocateObject(IKernel* pKernel, IGameObj* pGameTarget, float fPosX, float fPosY, float fPosZ, float fOrient)
	{
		if (pGameTarget)
		{
			if (fabs(pGameTarget->GetPosiX() - fPosX) < 0.1f
				&& fabs(pGameTarget->GetPosiZ() - fPosZ) < 0.1f)
			{
				return false;
			}

			bool res = pKernel->MoveTo(pGameTarget->GetObjectId(), fPosX, fPosY, fPosZ, fOrient);
			return res;
		}
		return false;
	}


	bool MotionObject(IKernel* pKernel, IGameObj* pGameTarget, float fMoveSpeed, float fDestX, float fDestZ, int nMode)
	{
		if (pGameTarget)
		{
			if (fabs(pGameTarget->GetPosiX() - fDestX) < 0.1f
				&& fabs(pGameTarget->GetPosiZ() - fDestZ) < 0.1f)
			{
				return false;
			}

			bool res = false;
			if (MOTION_MODE_HITBACK == nMode)
			{
				res = pKernel->MotionNoRotate(pGameTarget->GetObjectId(), fMoveSpeed, fDestX, fDestZ, nMode);
			}
			else
			{
				res = pKernel->Motion(pGameTarget->GetObjectId(), fMoveSpeed, PI2, fDestX, fDestZ, nMode);
			}

			// motion���ʱ,������겻��,locate��
			if (pGameTarget->GetClassType() == TYPE_PLAYER)
			{
				float fDestY = pKernel->GetMapHeight(fDestX, fDestZ);
				pKernel->Locate(pGameTarget->GetObjectId(), fDestX, fDestY, fDestZ, pGameTarget->GetOrient());
			}
			return res;
		}
		return false;
	}

	// Ѱ�Ҷ��󸽽���һ������λ�� 
	bool SearchObjectAroundPos(IKernel* pKernel, IGameObj* pGameSelf, float& fOutX, float& fOutY, float& fOutZ)
	{
		// ���Ŀ�������
		float fTargetX = pGameSelf->GetPosiX();
		float fTargetZ = pGameSelf->GetPosiZ();

		// ���Ŀ���������
		int nTargetGridIndex = pKernel->GetGridIndexByPos(fTargetX, fTargetZ);

		if (nTargetGridIndex <= 0)
		{
			return false;
		}

		// �����Χ���õĸ�����Ϣ
		CVarList GridResult;
		pKernel->GetAroundGridIndex(nTargetGridIndex, GridResult);

		if (GridResult.GetCount() == 0)
		{
			return false;
		}

		// ���һ��Ŀ�긽���Ŀո���
		int nMoveGridIndex = GridResult.IntVal(util_random_int((int)GridResult.GetCount()));

		// ��ȡ�ø��ӵ�����
		pKernel->GetPosByIndex(nMoveGridIndex, fOutX, fOutZ);

		fOutY = pKernel->GetMapHeight(fOutX, fOutZ);
		return true;
	}

	// һ����Χ�ڵĵ����ܵ������˺� 
	void DamgeRangeMultiEnemy(IKernel* pKernel, const PERSISTID& skill, IGameObj* pGameSelf, IGameObj* pTarget,
		float fRadio, float fValue, bool bIncludeSelf, EventDamageType type, int nMaxHurtNum, IVarList& outResultObjs)
	{
		if (NULL == pGameSelf)
		{
			return;
		}
		// ��ѯĿ����Χ�����ж���
		PERSISTID self = pGameSelf->GetObjectId();
		PERSISTID center = NULL == pTarget ? pGameSelf->GetObjectId() : pTarget->GetObjectId();

		pKernel->GetAroundList(center, fRadio, 0, 0, outResultObjs);
		if (bIncludeSelf)
		{
			outResultObjs.AddObject(center);
		}

		// ��ȡ���ܵ�����
		int nHitTargetRelationType, nTargetClassType;
		SkillAccessSingleton::Instance()->GetSkillHitTargetRelation(pKernel, skill, nHitTargetRelationType, nTargetClassType);

		int64_t nSkillUID = pGameSelf->QueryInt64(FIGHTER_CUR_SKILL_UUID);

		int nCurHitNum = 0;
		LoopBeginCheck(a)
		for (size_t i = 0; i < outResultObjs.GetCount(); ++i)
		{
			LoopDoCheck(a)
				// nMaxHurtNumΪ0��ʾ����������
			if (nMaxHurtNum != 0 && nCurHitNum >= nMaxHurtNum)
			{
				break;
			}
			PERSISTID target = outResultObjs.ObjectVal(i);
			IGameObj* pSearchTargetObj = pKernel->GetGameObj(target);
			if (NULL == pSearchTargetObj)
			{
				continue;
			}
			// Ŀ���Ƿ�����
			int nDead = 0;
			if (FPropertyInstance->GetDead(pSearchTargetObj, nDead) && nDead > 0)
			{
				continue;
			}
			// �ҵ��з�����Ѫ 
			if (USESKILL_RESULT_SUCCEED == TargetFilter::SatisfyRelation(pKernel, self, target, nHitTargetRelationType, nTargetClassType))
			{
				if (SKILL_EVENT_DAMAGE == type)
				{
					FightModule::m_pInstance->SkillEventDamageTarget(pKernel, self, target, skill, nSkillUID, fValue);
				}
				else if (VALUE_EVENT_DAMAGE == type)
				{
					FightModule::m_pInstance->EventDamageTarget(pKernel, self, target, skill, nSkillUID, true, (int)fValue);
				}
				else if (USE_SKILL_DAMAGE == type)
				{
					FightModule::m_pInstance->SkillDamageTarget(pKernel, self, target, skill, nSkillUID);
				}
				else if (PERCENT_DAMAGE == type)
				{
#ifdef FSROOMLOGIC_EXPORTS
					// ���ս�Ľ�����boss�����ܵ��ٷֱ��˺���Ӱ��
					if (1 == pSearchTargetObj->QueryInt("GVG_IsTower") || 0 == strcmp(pSearchTargetObj->GetScript(), "GVGBoss"))
					{
						continue;
					}
#endif
					int64_t nTarMaxHP = pSearchTargetObj->QueryInt64("MaxHP");
					float fDamVal = (float)nTarMaxHP * fValue;
					FightModule::m_pInstance->EventDamageTarget(pKernel, self, target, skill, nSkillUID, true, (int)fDamVal);
				}
				++nCurHitNum;
			}
		}
	}

	// ͳ�ƹ������˸���
	int	StatDamageEnemyNum(IKernel* pKernel, IGameObj* pGameSelf, const PERSISTID& skill, float fRadio, int nMaxHurtNum)
	{
		if (NULL == pGameSelf || !pKernel->Exists(skill))
		{
			return 0;
		}
		PERSISTID self = pGameSelf->GetObjectId();
		CVarList outResultObjs;
		pKernel->GetAroundList(self, fRadio, 0, 0, outResultObjs);

		// ��ȡ���ܵ�����
		int nHitTargetRelationType, nTargetClassType;
		SkillAccessSingleton::Instance()->GetSkillHitTargetRelation(pKernel, skill, nHitTargetRelationType, nTargetClassType);
		// �������˸���
		int nCurHitNum = 0;
		LoopBeginCheck(b)
		for (size_t i = 0; i < outResultObjs.GetCount(); ++i)
		{
			LoopDoCheck(b)
				// nMaxHurtNumΪ0��ʾ����������
			if (nMaxHurtNum != 0 && nCurHitNum >= nMaxHurtNum)
			{
				break;
			}
			PERSISTID target = outResultObjs.ObjectVal(i);
			IGameObj* pTarget = pKernel->GetGameObj(target);
			if (NULL == pTarget)
			{
				continue;
			}
			// Ŀ���Ƿ�����
			int nDead = 0;
			if (FPropertyInstance->GetDead(pTarget, nDead) && nDead > 0)
			{
				continue;
			}
			// �ҵ��з�����Ѫ 
			if (USESKILL_RESULT_SUCCEED == TargetFilter::SatisfyRelation(pKernel, self, target, nHitTargetRelationType, nTargetClassType))
			{
				++nCurHitNum;
			}
		}

		return nCurHitNum;
	}
	// Ϊһ������ӻظ�HP
	int RestoreHPForObject(IKernel* pKernel, IGameObj* pSelfObj, float fVal, EventCureCal type, float fAddRate)
	{
		// ��ʵ��Ѫֵ
		int64_t nRealAdd = 0;
		if (NULL == pSelfObj)
		{
			return 0;
		}
		// �õ���ҵ�ǰ��hp��maxhp
		int64_t nMaxValue = pSelfObj->QueryInt64("MaxHP");
		int64_t nCurValue = pSelfObj->QueryInt64("HP");
		// �õ�׼����Ѫ����ֵ
		int64_t nValue = 0;
		if (EVENT_VALUE_RESTORE == type)
		{
			nValue = (int64_t)fVal;
		}
		else
		{
			nValue = (int64_t)((float)nMaxValue * fVal);
		}

		if (fAddRate > 0.0f)
		{
			nValue = (int64_t)((float)nValue * (1 + fAddRate));
		}

		// HP�ظ��������Դ���
		// 	int nWeakRate = pSelfObj->QueryInt("WeakRestoreHPRate");
		// 	float fWeakRate = 0.0f;
		// 	if (nWeakRate > 0)
		// 	{
		// 		fWeakRate = (float)nWeakRate / 100.0f;
		// 	}
		// 	nValue = (int64_t)((float)nValue * (1 - fWeakRate)); 

		// ������ʵ��Ѫֵ�ͼ�Ѫ�����ֵ
		if (nCurValue + nValue > nMaxValue)
		{
			nValue = nMaxValue;
			nRealAdd = nMaxValue - nCurValue;
		}
		else if (nCurValue + nValue < 0)
		{
			nValue = 0;
			nRealAdd = 0;
		}
		else
		{
			nRealAdd = nValue;
			nValue = nCurValue + nValue;
		}
		// ���õ�ǰHP
		pSelfObj->SetInt64("HP", nValue);
		return (int)nRealAdd;
	}

	// ͨ������Ϊ����ظ�HP
	// int RestoreHPForObjectBySkill(IKernel* pKernel, IGameObj* pSelfObj, const PERSISTID& skiller, const PERSISTID& object, float fVal, EventCureCal type)
	// {
	// 	IGameObj* pObj = pKernel->GetGameObj(object);
	// 	IGameObj* pSkillerObj = pKernel->GetGameObj(skiller);
	// 	if (NULL == pSelfObj || NULL == pObj || NULL == pSkillerObj)
	// 	{
	// 		return 0;
	// 	}
	// 	// HP�ָ���������Ч���ж�
	// 	bool bAddReHP = false;
	// 	// ����ҩƷ����������Ƽ���
	// 	if (0 == strcmp(pObj->GetScript(), AMBER_SKILL_NORMAL) && 0 != strcmp(pObj->GetConfig(), RESTORE_HP_SKILL_ID))
	// 	{
	// 		bAddReHP = true;
	// 	}
	// 	// �����ɼ��ܲ�����buff
	// 	else if (0 == strcmp(pObj->GetScript(), AMBER_BUFFER))
	// 	{
	// 		PERSISTID skill = pObj->QueryObject("Skill");
	// 
	// 		if (pKernel->Exists(skill) && 0 == strcmp(pKernel->GetScript(skill), AMBER_SKILL_NORMAL))
	// 		{
	// 			bAddReHP = true;
	// 		}
	// 	}
	//  
	// 	float fAddRate = 0.0f;
	// 	if (bAddReHP)
	// 	{
	// 		int nReHPAddRate = 0;
	// 		if (pKernel->Exists(skiller))
	// 		{
	// 			nReHPAddRate = pSkillerObj->QueryInt("ReHPAddRate");
	// 		}
	// 		fAddRate = (float)nReHPAddRate / 10000.0f;
	// 	}
	// 	
	// 	return RestoreHPForObject(pKernel, pSelfObj, fVal, type, fAddRate);
	// }

	/*!
	* @brief	�������Χ���η�Χ����һ����վ����
	* @param	int �������n(������n�λ�û�ҵ���,��Ѱ��ʧ��)
	* @param	float fMinRadios, float fMaxRadios, ����Բ�뾶���Ƕ�
	* @param	float& fOutX, float& fOutY ���ص�����ֵ
	* @return	bool �Ƿ���ҳɹ�
	*/
	bool RandomFindAroundPos(IKernel* pKernel, IGameObj* pSelfObj, int nRandomNum, float fMinRadios, float fMaxRadios, float& fOutX, float& fOutY, float& fOutZ)
	{
		bool bFind = false;
		if (NULL == pSelfObj || nRandomNum <= 0)
		{
			return false;
		}
		// ���һ������
		float fRadios = fMinRadios + util_random_float(fMaxRadios - fMinRadios);
		LoopBeginCheck(c)
		for (int i = 0; i < nRandomNum; ++i)
		{
			LoopDoCheck(c)
				// ����Ƕ�
				float fTmpAnagle = util_random_float(PI);
			if (FindPosByDisAndOrient(pKernel, pSelfObj, fRadios, fTmpAnagle, fOutX, fOutY, fOutZ))
			{
				bFind = true;
				break;
			}
		}

		return bFind;
	}

	/*!
	* @brief	���Ź̶���������ҵ�λ��
	* @param	fDistance ����
	* @param	fOrient ����
	* @param	float& fOutX, float& fOutY, float& fOutZ ���ص�λ������
	* @return	void
	*/
	bool FindPosByDisAndOrient(IKernel* pKernel, IGameObj* pSelfObj, float fDistance, float fOrient, float& fOutX, float& fOutY, float& fOutZ)
	{
		bool bFind = false;
		if (NULL == pSelfObj)
		{
			return false;
		}

		fOutX = 0.0f;
		fOutY = 0.0f;
		fOutZ = 0.0f;

		float fDeltaX = fDistance * FMSIN(fOrient);
		float fDeltaZ = fDistance * FMCOS(fOrient);
		// �������յ��������
		float fTmpX = pSelfObj->GetPosiX() + fDeltaX;
		float fTmpZ = pSelfObj->GetPosiZ() + fDeltaZ;
		// ��ȡ�˵�ĸ�������
		int nGridIndex = pKernel->GetGridIndexByPos(fTmpX, fTmpZ);
		if (-1 == nGridIndex)
		{
			return false;
		}

		// �ж������Ƿ����
		bool bCanWalk = false;
		bool bLogicbalk = false;
		bool bObjectoccupy = false;
		int groupId = pSelfObj->QueryInt("GroupID");
		pKernel->QueryPathGridStatus(nGridIndex, bCanWalk, bLogicbalk, bObjectoccupy, groupId);
		if (bCanWalk)
		{
			fOutX = fTmpX;
			fOutZ = fTmpZ;
			fOutY = pKernel->GetMapHeight(fOutX, fOutZ);
			bFind = true;
		}

		return bFind;
	}

	// ���ǵ����ϰ��������һ��λ��
	void GetFinalPosByOrient(IKernel* pKernel, float fSrcX, float fSrcY, float fSrcZ, float fOrient, float fDistance,
		float& fFinalX, float& fFinalY, float& fFinalZ)
	{
		//�ƶ�����ĵ�λ����
		FmVec2 vNormal = FmVec2(::sin(fOrient), ::cos(fOrient));

		//����Ŀ���
		float fDestX = fSrcX + vNormal.x * fDistance;
		float fDestZ = fSrcZ + vNormal.y * fDistance;

		//��ȡ���յ�
		float fNewX = 0.0f, fNewY = 0.0f, fNewZ = 0.0f;
		if (!pKernel->TraceLineWalk(TRACE_LINE_WALK_STEP, fSrcX, fSrcY, fSrcZ, fDestX, fDestZ, fNewX, fNewY, fNewZ))
		{
			fFinalX = fNewX;
			fFinalY = fNewY;
			fFinalZ = fNewZ;
		}
		else
		{
			fFinalX = fDestX;
			fFinalZ = fDestZ;
			fFinalY = pKernel->GetMapHeight(fFinalX, fFinalZ);
		}
	}

	// ͨ��buff������Ұ�id
	const char* QueryBuffPackageId(IKernel* pKernel, const PERSISTID& buff)
	{
		const char* strBasePackageID = "";
		do
		{
			if (!pKernel->Exists(buff))
			{
				break;
			}
			const char* szFromID = pKernel->GetConfig(buff);
			const BuffBaseData* pBuffBaseData = SkillDataQueryModule::m_pInstance->GetBuffBaseData(szFromID);
			if (pBuffBaseData == NULL)
			{
				break;
			}

			//��ȡbuff����id
			strBasePackageID = pBuffBaseData->GetBasePackage();
		} while (0);

		return strBasePackageID;
	}

	// ��������npc����
	PERSISTID CreateTrapNpcObj(IKernel* pKernel, IGameObj* pSelfObj, const char* strNpcId, float fPosX, float fPosY, float fPosZ)
	{
		if (NULL == pSelfObj || StringUtil::CharIsNull(strNpcId))
		{
			return PERSISTID();
		}

		CVarList args;
		args << CREATE_TYPE_PROPERTY_VALUE << "GroupID" << pSelfObj->QueryInt("GroupID") << "Master" << pSelfObj->GetObjectId();

		PERSISTID npc = pKernel->CreateObjectArgs("", strNpcId, 0, fPosX, fPosY, fPosZ, pSelfObj->GetOrient(), args);
		return npc;
	}

	// ���ݷ�Χͳ�����幥��Ŀ��
	bool FindTargetList(IKernel* pKernel, IVarList& outTargetList, IGameObj* pSelfObj,
		const PERSISTID& master, const PERSISTID& skill, int nHitIndex, bool bUseSkillRange, float fSkillRadius)
	{
		if (NULL == pSelfObj || !pKernel->Exists(master) || !pKernel->Exists(skill) || nHitIndex < 0)
		{
			return false;
		}

		outTargetList.Clear();

		// ���ü��ܷ�Χ
		if (!bUseSkillRange)
		{
			// ��ѯĿ����Χ�����ж���
			CVarList resultObj;
			pKernel->GetAroundList(pSelfObj->GetObjectId(), fSkillRadius, TYPE_PLAYER | TYPE_NPC, 0, outTargetList);
		}
		else
		{

			float fPosX = pSelfObj->GetPosiX();
			float fPosY = pSelfObj->GetPosiY();
			float fPosZ = pSelfObj->GetPosiZ();

			// ���д�����1����
			++nHitIndex;

			//ͳ�Ƽ���Ӱ��Ķ���
			SkillFlowSingleton::Instance()->GetHitRangeTargets(pKernel, master, skill, fPosX, fPosY, fPosZ, nHitIndex, outTargetList);
		}
		return true;
	}

	// �Ƿ���ͬ�����д���
	bool IsSameHitIndex(IKernel* pKernel, IGameObj* pSelfObj, IGameObj* pSkillObj, int nHitIndex)
	{
		if (NULL == pSkillObj || NULL == pSelfObj)
		{
			return false;
		}

		// ����Ϊ0ʱ,����֤����ֵ
		if (0 == nHitIndex)
		{
			return true;
		}

		bool bSame = false;
		int nMaxHitNum = SkillDataQueryModule::m_pInstance->GetMaxHitNum(pKernel, pSkillObj);
		if (nHitIndex > nMaxHitNum)
		{
			nHitIndex = nMaxHitNum;
		}
		// תΪ��0����
		--nHitIndex;
		// ֻ����ָ������ʱ,�Ż���ס�з�
		int nCurHitIndex = SkillAccessSingleton::Instance()->GetHitIndex(pKernel, pSelfObj, pSkillObj);

		if (nCurHitIndex == nHitIndex)
		{
			bSame = true;;
		}

		return bSame;
	}

	// �Ƿ�Ϊ�������
	bool IsPlayerType(IGameObj* pSelfObj)
	{
		if (pSelfObj == NULL)
		{
			return false;
		}

		return pSelfObj->GetClassType() == TYPE_PLAYER || pSelfObj->GetClassType() == TYPE_NPC && strcmp(pSelfObj->GetScript(), "BattleNpc") == 0;
	}

	// ���Ŀ�������Ƿ����
	bool CheckTargetType(IGameObj* pTargetObj, int nCheckType)
	{
		if (NULL == pTargetObj)
		{
			return false;
		}

		// Ŀ�����ͼ��,nAddTargetTypeΪALL_TARGET_TYPE���ü��
		if (ALL_TARGET_TYPE == nCheckType)
		{
			return true;
		}

		bool bCheckResult = false;
		int nTargetType = pTargetObj->GetClassType();
		if (TYPE_NPC == nCheckType)
		{
			bCheckResult = nTargetType == nCheckType;
		}
		else if (TYPE_PLAYER == nCheckType)
		{
			bCheckResult = SkillEventExecuteUtil::IsPlayerType(pTargetObj);
		}

		return bCheckResult;
	}
};