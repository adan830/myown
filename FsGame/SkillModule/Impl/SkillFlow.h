
//------------------------------------------------------------------------------
// �ļ���:      SkillFlow.h
// ��  ��:      ����������ش���
// ˵  ��:      ����ֻ�ڼ���ϵͳ�ڲ�ʹ�ã��Ҳ�Ҫֱ�Ӷ���SkillFlow��ʵ������ʹ��
//              �䵥��ģʽ�ӿ�SkillFlowSingleton::Instance()
// ��������:    2014��10��27��
// ������:       
// ��ע:
//    :       
//------------------------------------------------------------------------------

#ifndef __SKILLFLOW_H__
#define __SKILLFLOW_H__

#include "Fsgame/Define/header.h"

#include "utils/singleton.h"
#include "FsGame/skillmodule/impl/skilldefinedatastruct.h"

class SkillFlow
{
public:
    bool Init(IKernel* pKernel);

public:
    
    /**
    *@brief  ʹ�ü��ܵ����սӿڣ����жԼ���ʹ�õĵ��ö���������
    *@param  [in] pKernel �����ں�ָ��
    *@param  [in] self ʩ����
    *@param  [in] x,y,z �����ͷ�ʱʩ����λ��
    *@param  [in] skill ���ܶ���
    *@param  [in] target �������ö���
    *@param  [in] dx,dy,dz �������õ�λ��
    *@param  [in] dorient �������÷���
	*@param  [in] nBeginEffect ��ʼ�ƶ�Ч������ 0Ϊû���ƶ�Ч��
    *@return int ����1��ʾ�����ͷţ�����ֵ�����Ӧ�Ŵ���ŷ���
    */
    int UseSkill(IKernel* pKernel, const PERSISTID& self,
                 float x, float y, float z, const PERSISTID& skill,
                 const PERSISTID& target, float dx, float dy, float dz, float dorient);

	/*!
	 * @brief	�ͻ������������߼�����
	 * @param	self ʩ����
	 * @param	x,z,dorient �����ͷ�ʱʩ����λ��
	 * @param	skill ���ܶ���
	 * @param	nSkilluid ����uid
	 * @param	nCurHitIndex ��ǰ���д���
	 * @param	targetList �����ƶ���Ŀ���б�
	 * @param	moveTargetList Ҫ�ƶ���Ŀ���б�(���л��ˡ����ɵ�Ч��)
	 * @return	int
	 */
	int DoSkillHitByClient(IKernel* pKernel, const PERSISTID& self, const PERSISTID& skill, const int64_t nSkilluid,
						 const int nCurHitIndex, const IVarList& targetList, const IVarList& moveTargetList);
public:
    // �Ƿ��Ǽ�������
    bool IsSkillFlow(IKernel* pKernel, const PERSISTID& self);

    // �Ƿ���������
    bool IsLeading(IKernel* pKernel, const PERSISTID& self);

    // �Ƿ���׼���׶���
    bool IsPreparing(IKernel* pKernel, const PERSISTID& self);

    // �Ƿ�����������
    bool IsHiting(IKernel* pKernel, const PERSISTID& self);

    // �ж��Ƿ�������ʹ�ü���
    int IsSkillUseBusy(IKernel* pKernel, const PERSISTID& self);

    /**
    *@brief  ��ϼ�������
    *@param  [in] pKernel �����ں�ָ��
    *@param  [in] self ʩ����
    *@param  [in] breaker �������̴����
    *@param  [in] bForce �Ƿ�ǿ�ƴ��
    *@return bool 
    */
    bool BreakSkill(IKernel* pKernel, const PERSISTID& self, const PERSISTID& breaker, bool bForce, int iBreakReason);

    // ��ȡ���÷�Χ�ڵ�Ŀ��
    bool GetHitRangeTargets(IKernel* pKernel, const PERSISTID& self, const PERSISTID& skill,
        float x, float y, float z, int nHitIndex, IVarList& result);
	// �ж��ͷſ���ǿ���ͷż���
	int CheckForceUseSkill(IKernel* pKernel, const PERSISTID& self, const PERSISTID& skill);

private:
    // -------------------------------���̻ص�----------------------------------
    //����׼���ڿ�ʼ�Ļص�����
    static bool OnFlowBeforePrepare(IKernel* pKernel, const PERSISTID& self);
    // ����׼���ڽ����Ļص�����
    static bool OnFlowPrepare(IKernel* pKernel, const PERSISTID& self);
    //����������ʼ�Ļص�����
    static bool OnFlowBeforeLead(IKernel* pKernel, const PERSISTID& self);
    // ���������ڽ����Ļص�����
    static bool OnFlowHit(IKernel* pKernel, const PERSISTID& self);
    // ���̱���ϵĻص�����
    static bool OnFlowBreak(IKernel* pKernel, const PERSISTID& self);
    // ���̵ȴ��ڽ����Ļص�����
    static bool OnFlowFinish(IKernel* pKernel, const PERSISTID& self);
private:
    // -----------------------------���̴�����--------------------------------
    // ׼��ʱ���ѵ�
    bool DoPrepareSkill(IKernel* pKernel, const PERSISTID& self, const PERSISTID& skill,
                        const PERSISTID& target, float x, float y, float z);

    // ����ʱ���ѵ�����ʼִ�м��ܵ�Ч��
    bool DoHitSkill(IKernel* pKernel, const PERSISTID& self,
                    const PERSISTID& skill, const int64_t iSkillUUID,
                    const PERSISTID& target, float x, float y, float z);

    // ��Ч���д���
    bool DoEffectHitTargetProcess(IKernel* pKernel, const PERSISTID& self, 
                                  const PERSISTID& skill, int64_t iSkillUUID,
                                  const PERSISTID& target, const IVarList& targetList);

    // ����ʹ�ý���
    bool DoFinishSkill(IKernel* pKernel, const PERSISTID& self, const PERSISTID& skill);

    // ����ʹ�ñ����
    bool DoBreakSkill(IKernel* pKernel, const PERSISTID& self, const PERSISTID& skill);

private:
    // ��ʼʹ�ü���
    void BeginUseSkill(IKernel* pKernel, const PERSISTID& self,
                       float x, float y, float z, float orient, const PERSISTID& skill,
                       const PERSISTID& target, float dx, float dy, float dz);
	
	// �����ͷ�ǰ����Ϣ����
    void InitBeginUseSkillInfo(IKernel* pKernel, const PERSISTID& self,
                               const PERSISTID& skill, int64_t iSkillUUID, 
                               const PERSISTID& target, float dx, float dy, float dz);

    void PrepareUseSkill(IKernel* pKernel, const PERSISTID& self,
                                const PERSISTID& skill, int64_t iSkillUUID,
                                const PERSISTID& target, 
                                float dx, float dy, float dz, float orient);

    bool RandomHitTarget(IKernel* pKernel, const PERSISTID& self, const PERSISTID& skill, float dx, float dy, float dz, int count, IVarList& result);
    bool RandomHitTargetPos(IKernel* pKernel, const PERSISTID& self, const PERSISTID& skill, float dx, float dy, float dz, int count, IVarList& result);
    bool RandomHitTargetRange(IKernel* pKernel, const PERSISTID& self, const PERSISTID& skill, float dx, float dy, float dz, float orient, int count, IVarList& result);
    bool RandomCirclePosition(IKernel* pKernel, const PERSISTID& self, const PERSISTID& skill, float dx, float dy, float dz, float orient, int count, IVarList& result);
    bool RandomRectPosition(IKernel* pKernel, const PERSISTID& self, const PERSISTID& skill, float dx, float dy, float dz, float orient, int count, IVarList& result);
    bool GetSkillHitRangeByRandom(IKernel* pKernel, const PERSISTID& self, const PERSISTID& skill, std::vector<SkillHitRangeParams>& vecRandomRanges);

    // ��ʼʹ�ü�����
    void BeginSkillFlow(IKernel* pKernel, const PERSISTID& self);

    // ��ȡ����������׼��,����ʱ��
    bool GetSkillCollocateTime(IKernel* pKernel, const PERSISTID& self, const PERSISTID& skill,const SkillMainData *pSkillMain,
							int& iPrepareTime, int& iLeadTime, int& iLeadSepTime);
    
    // ��ö�������������ʱ���б�
    bool GetMultipleFlowRealHitTime(IKernel* pKernel, const PERSISTID& self, IGameObj* pSkill, const SkillMainData *pSkillMain, IVarList& hittimelist);

    //////////////////////////////////////////////////////////////////////////
    // ���ʹ�ü��ܹ��������õ��������
    bool ClearUseSkillData(IKernel* pKernel, const PERSISTID& self, const PERSISTID& skill);

    // �����ͷ���λ��
    void UpdateObjectLocate(IKernel* pKernel, const PERSISTID& self, const PERSISTID& target, const PERSISTID& skill, float x, float y, float z, float dorient);

	// ����Ӳֱ״̬
	void AddSkillStiffState(IKernel* pKernel, const PERSISTID& self, const PERSISTID& skill);

    // ��ȴ���� ����CDʱ��
    int ComputeSkillCoolTime(IKernel* pKernel, const PERSISTID& self, const PERSISTID& skill);

	// Ⱥ���˺���Ҫһ���͵ļ��ܴ����߼�
	//void DoRandomMultiAttack(IKernel* pKernel, const PERSISTID& self, const PERSISTID& skill, const int64_t nSkillUid, const int nHitCount, const IVarList& targetList);

	// �ͻ���������֤��ʼ��
	void InitClientValidData(IKernel* pKernel, const PERSISTID& self, const PERSISTID& skill);

	// ����Ӳֱ״̬����,����������Ķ���ֻ�ܲ��ż���,����������AI��Ϊ
	static int H_SkillStiffStateEnd(IKernel* pKernel, const PERSISTID& creator, int slice);

	// ��������һ��Ŀ��
	void OnHitOneTarget(IKernel* pKernel, PERSISTID target, const PERSISTID& skill, IGameObj* pSelfObj, const SkillMainData* pSkillMain, const int64_t nSkilluid);
}; // end of class SkillFlow

typedef HPSingleton<SkillFlow> SkillFlowSingleton;

#endif // __SKILLFLOW_H__
