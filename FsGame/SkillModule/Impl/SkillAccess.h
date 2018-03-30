//------------------------------------------------------------------------------
// �ļ���:      skill_access.h
// ��  ��:      ���ܶ������Է��ʡ�����״̬��ز�ѯ
// ˵  ��:      ����ֻ�ڼ���ϵͳ�ڲ�ʹ�ã��Ҳ�Ҫֱ�Ӷ���SkillAccess��ʵ������ʹ��
//              �䵥��ģʽ�ӿ�SkillAccessSingleton::Instance()
// ��������:    2014��10��27��
// ������:       
// ��ע:
//    :       
//------------------------------------------------------------------------------

#ifndef __SKILLACCESS_H__
#define __SKILLACCESS_H__

#include "Fsgame/Define/header.h"

#include "utils/Singleton.h"
#include "FsGame/Define/SkillDefine.h"
#include "FsGame/Define/FightDefine.h"
#include <vector>

struct SkillMainData;

class SkillAccess
{
public:
    //
    bool Init(IKernel* pKernel);

public:
    // ָ�������Ƿ�������ȴ
    bool IsCoolDown(IKernel* pKernel, const PERSISTID& self, const PERSISTID& skill);
public:
    // ��ȡ��������Ŀ��,λ��,����
    bool GetSkillTargetAndPos(IKernel* pKernel, const PERSISTID& self, const PERSISTID& skill, PERSISTID& useskilltarget, float& dx, float& dy, float& dz, float& doirent);

    // ��ü��ܵ���ȴʱ��
    bool GetCoolDownTime(IKernel* pKernel, const PERSISTID& self, const PERSISTID& skill, int& iPubCooldowntime, int& iCateCooldowntime, int& iCoolCategory);
	
	// ��ȡ������Ϣ�ж������ʱ������˺�ϵ��
	void GetExtraHitDamageRatio(IKernel* pKernel, const PERSISTID& self, const PERSISTID& skill, float& fExtraHitDamageRatio);

    // ��ȡ���ܵ��ͷ�Ŀ�����ͣ���Ŀ�����Ŀ�����֣�����USESKILL_TYPE_TARGET����USESKILL_TYPE_POSITION��
    int GetUseSkillType(IKernel* pKernel, const PERSISTID& skill);

	// ��֤λ�Ƶ���Ч�� nObjType��ӦSkillDisplaceObject
	bool IsValidDisplace(IKernel* pKernel, const PERSISTID& self, const char* strSkillId, int nObjType, float dx, float dz);

	// ��ѯ���ܵ�λ�����ͺ��ٶ� nObjType��ӦSkillDisplaceObject
	bool QuerySkillDisplaceTypeAndSpeed(IKernel* pKernel, const char* strSkillId, const int nObjType, int& nOutDisplaceType, float& fDisplaceSpeed);

    /// -------------------------------------------------------------------------
    // ��ȡ�����ͷ�ѡ������
    ESkillSelectType GetSkillSelectType(IKernel* pKernel, const PERSISTID& skill);

    // ��ȡ������̲���(��������self��ԭ���ǣ������̲������ܻ��ܵ�self���Ե�Ӱ�죩
    bool GetSkillCastRange(IKernel* pKernel, const PERSISTID& self, const PERSISTID& skill, SkillCastRangeParams& cast_range_params,const SkillMainData* pSkillMain);

    // ��ȡ�������÷�Χ����(��������self��ԭ���ǣ�������÷�Χ�������ܻ��ܵ�self���Ե�Ӱ�죬Ŀǰ���������ȵ�Ӱ�죩
    bool GetSkillHitRange(IKernel* pKernel, const PERSISTID& self, const PERSISTID& skill, int nHitIndex, SkillHitRangeParams& hit_range_params);

	// ͨ������id�ҵ���������
	const float GetSkillHitDistance(IKernel* pKernel, const char* szSkillId);

    // ��ȡ���ܿ����ж����ϵ
    bool GetSkillHitTargetRelation(IKernel* pKernel, const PERSISTID& skill, int& iTargetRelationType, int& iTargetClassType);

	// �Ƿ�ͻ���ѡ��Ŀ������
	bool IsClientHitTarget(IKernel* pKernel, IGameObj* pSelfObj, IGameObj* pSkillObj);

	// ��ȡ��ǰ����ʣ���CD (��λ:����)
	int GetSkillLeftCD(IKernel* pKernel, const PERSISTID& self, const char* strSkill);

	// ��ȡ��ǰ���д���
	int GetHitIndex(IKernel* pKernel, IGameObj* pSelfObj, IGameObj* pSkillObj);

	// ������ֵ
	int ComputeHateValue(IKernel* pKernel, const PERSISTID& skill, int nDamValue);
private:
	// ͨ����Χid�ҵ���Ӧ�Ĳ���
	bool GetSkillHitRangeByHitRangeID(IKernel* pKernel, const char* szHitRangeID, SkillHitRangeParams& hitRangeParams);
}; // end of class SkillAccess

typedef HPSingleton<SkillAccess>    SkillAccessSingleton;

#endif // __SKILLACCESS_H__



