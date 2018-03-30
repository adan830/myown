//--------------------------------------------------------------------
// �ļ���:      skill_event_execute_util.h
// ��  ��:      ����Ч���Ĺ��ܺ���
// ˵  ��:
// ��������:    2014��10��30��
// ������:       
//    :       
//--------------------------------------------------------------------

#ifndef __SKILLEVENTEXECUTEUTIL_H__
#define __SKILLEVENTEXECUTEUTIL_H__

#include "Fsgame/Define/header.h"
#include "../../Define/FightDefine.h"

namespace SkillEventExecuteUtil
{
	enum
	{
		ALL_TARGET_TYPE = 0,		// ��������
	};

	// �¼��˺�����
	enum EventDamageType
	{
		SKILL_EVENT_DAMAGE,			// �뼼�ܹ�ʽ��ص��˺�
		VALUE_EVENT_DAMAGE,			// �̶�ֵ���˺�
		USE_SKILL_DAMAGE,			// ʹ�ü����˺�
		PERCENT_DAMAGE				// ���ٷֱ��˺�
	};

	// ��������
	enum EventCureCal
	{
		EVENT_VALUE_RESTORE,		// �̶�ֵ�ظ�Ѫ��
		EVENT_PERCENT_RESTORE,		// ���ٷֱȻظ�Ѫ��
	};

	// λ������
	struct PosInfo
	{
		PosInfo() : fPosX(0.0f), fPosZ(0.0f){}
		float	fPosX;		// x����
		float	fPosZ;		// z����
	};

	//����λ���ڲ�������һ�NPC
	bool LocateObject(IKernel* pKernel, IGameObj* pGameTarget, float fPosX, float fPosY, float fPosZ, float fOrient);

	//�����ƶ����ڲ�������һ�NPC
	bool MotionObject(IKernel* pKernel, IGameObj* pGameTarget, float fMoveSpeed, float fDestX, float fDestZ, int nMode = 0);

	// Ѱ�Ҷ��󸽽���һ������λ�� 
	bool SearchObjectAroundPos(IKernel* pKernel, IGameObj* pGameSelf, float& fOutX, float& fOutY, float& fOutZ);

	// һ����Χ�ڵĵ����ܵ������˺� nMaxHurtNum 0 ��ʾ�������� (typeΪVALUE_EVENT_DAMAGEʱ �������˺��ص��е��ô˺���)
	void DamgeRangeMultiEnemy(IKernel* pKernel, const PERSISTID& skill, IGameObj* pGameSelf, IGameObj* pTarget,
		float fRadio, float fValue, bool bIncludeSelf, EventDamageType type, int nMaxHurtNum, IVarList& outResultObjs);

	// ͳ�ƹ������˸���
	int	StatDamageEnemyNum(IKernel* pKernel, IGameObj* pGameSelf, const PERSISTID& skill, float fRadio, int nMaxHurtNum);

	// Ϊһ������ӻظ�HP
	int RestoreHPForObject(IKernel* pKernel, IGameObj* pSelfObj, float fVal, EventCureCal type, float fAddRate = 0.0f);

	// ͨ������Ϊ����ظ�HP object�����Ǽ��ܻ�buff
	//int RestoreHPForObjectBySkill(IKernel* pKernel, IGameObj* pSelfObj, const PERSISTID& skiller, const PERSISTID& object, float fVal, EventCureCal type);

	/*!
	* @brief	�������Χ���η�Χ����һ����վ����
	* @param	int �������n(������n�λ�û�ҵ���,��Ѱ��ʧ��)
	* @param	float fMinRadios, float fMaxRadios ����Բ�뾶
	* @param	float& fOutX, float& fOutY ���ص�����ֵ
	* @return	bool �Ƿ���ҳɹ�
	*/
	bool RandomFindAroundPos(IKernel* pKernel, IGameObj* pSelfObj, int nRandomNum, float fMinRadios, float fMaxRadios, float& fOutX, float& fOutY, float& fOutZ);

	/*!
	* @brief	���Ź̶���������ҵ�λ��
	* @param	fDistance ����
	* @param	fOrient ����
	* @param	float& fOutX, float& fOutY, float& fOutZ ���ص�λ������
	* @return	void
	*/
	bool FindPosByDisAndOrient(IKernel* pKernel, IGameObj* pSelfObj, float fDistance, float fOrient, float& fOutX, float& fOutY, float& fOutZ);

	// ���ǵ����ϰ��������һ��λ��
	void GetFinalPosByOrient(IKernel* pKernel, float fSrcX, float fSrcY, float fSrcZ, float fOrient, float fDistance,
		float& fFinalX, float& fFinalY, float& fFinalZ);

	// ͨ��buff������Ұ�id
	const char* QueryBuffPackageId(IKernel* pKernel, const PERSISTID& buff);

	// ��������npc����
	PERSISTID CreateTrapNpcObj(IKernel* pKernel, IGameObj* pSelfObj, const char* strNpcId, float fPosX, float fPosY, float fPosZ);

	// ���ݷ�Χͳ�����幥��Ŀ��
	bool FindTargetList(IKernel* pKernel, IVarList& outTargetList, IGameObj* pSelfObj,
		const PERSISTID& master, const PERSISTID& skill, int nHitIndex, bool bUseSkillRange, float fSkillRadius = 0.0f);

	// �Ƿ���ͬ�����д���
//	bool IsSameHitIndex(IKernel* pKernel, IGameObj* pSelfObj, IGameObj* pSkillObj, int nHitIndex);

	// �Ƿ�Ϊ�������
	bool IsPlayerType(IGameObj* pSelfObj);

	// ���Ŀ�������Ƿ����
	bool CheckTargetType(IGameObj* pTargetObj, int nCheckType);
};

#endif // __SKILLEVENTEXECUTEUTIL_H__