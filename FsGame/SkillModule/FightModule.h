
//------------------------------------------------------------------------------
// �ļ���:      fight_module.h
// ��  ��:      ս��ϵͳ�ӿ�
// ˵  ��:
// ��������:    2013��2��17��
// ������:       
// ��ע:
//    :       
//------------------------------------------------------------------------------

#ifndef _FightModule_H_
#define _FightModule_H_

#include "Fsgame/Define/header.h"
#include "FsGame/Define/FightDefine.h"
#include <cmath>

/////////////////////////////////////////////////////////////

class FightModule : public ILogicModule
{
public:
    // ��ʼ��
    virtual bool Init(IKernel* pKernel);

    // �ͷ�
    virtual bool Shut(IKernel* pKernel);

public:

    /**
    *@brief  ����ս������ʽ���Է�����˺�
    *@param  [in] pKernel �����ں�ָ��
    *@param  [in] self ����˺����˺���Դ����
    *@param  [in] target ���˺���Ŀ��
    *@param  [in] skill ���ܶ���
    *@param  [in] iSkillUUID ����uuid
    *@return bool 
    */
    bool SkillDamageTarget(IKernel* pKernel,
                           const PERSISTID& self,
                           const PERSISTID& target,
                           const PERSISTID& skill,
                           const int64_t iSkillUUID);

    /**
    *@brief  �¼��˺�����ר�ýӿڣ���ֹ���˺���Ϣ�����˺��¼�������������ѭ��
    *@param  [in] pKernel �����ں�ָ��
    *@param  [in] self ������
    *@param  [in] target ����Ŀ��
    *@param  [in] object ���ܶ����buffer���󣬶����޷�ȷ���ģ������ǿ�
    *@param  [in] uuid ���ܶ����buffer������Ӧ��uuid�������޷�ȷ����object�������ǿ�
    *@param  [in] bCanTriggerEvent �Ƿ��ܴ����¼�
    *@param  [in] iDamageValue ��ɵ��˺�ֵ
    *@return bool 
    */
    bool EventDamageTarget(IKernel* pKernel, 
                           const PERSISTID& self,
                           const PERSISTID& target,
                           const PERSISTID& object,
                           const int64_t uuid,
                           bool bCanTriggerEvent, 
                           int iDamageValue);

	/**
    *@brief  �����¼��˺�
    *@param  [in] pKernel �����ں�ָ��
    *@param  [in] self ������
    *@param  [in] target ����Ŀ��
    *@param  [in] object ���ܶ���
    *@param  [in] uuid ���ܶ����buffer������Ӧ��uuid�������޷�ȷ����object�������ǿ�
    *@param  [in] fRate �˺�����
    *@return bool 
    */
    bool SkillEventDamageTarget(IKernel* pKernel, 
                           const PERSISTID& self,
                           const PERSISTID& target,
                           const PERSISTID& skill,
                           const int64_t iSkillUUID,
						   const float fRate);

public:

    /**
    *@brief  ֱ��ɱ��Ŀ��
    *@param  [in] pKernel �����ں�ָ��
    *@param  [in] self ������
    *@param  [in] target ��ɱ������
    *@object [in] skill or buffer
    */
    void KillTarget(IKernel* pKernel, const PERSISTID& self, const PERSISTID& target, const PERSISTID& object, const int64_t uuid, int nDamageCate);

	//�ܹ�����Ŀ��
	static bool CanDamageTarget(IKernel* pKernel, const PERSISTID& self, const PERSISTID& target);
protected:
    // TODO
    // ���������¼���Ϣ����
    bool CommandMsgProcess(IKernel* pKernel, const PERSISTID& self, const PERSISTID& target,
                           const PERSISTID& skill, const int64_t iSkillUUID,
                           EHIT_TYPE ePhyHitType);


    // ��ͻ��˷��ͼ�Ѫ��Ϣ
    void CustomSkillDamageMsg(IKernel* pKernel, const PERSISTID& self, const PERSISTID& target,
                              int iHPDamage, int iHitType, const PERSISTID& skill,
                              const int64_t iSkillUUID);


    // ��Ŀ������˺��Ĵ���
    int Damage(IKernel* pKernel, const PERSISTID& self, const PERSISTID& target,
                const PERSISTID& skill, const int64_t iSkillUUID, int nTempDamageValue,
                EHIT_TYPE ePhyHitType);

	// Miss״̬�µĴ���
	void MissSkillDamageTargetHandle(IKernel* pKernel, const PERSISTID& self, const PERSISTID& target, const PERSISTID& skill, const int64_t iSkillUUID);

    // �����˺�������
    int RealDamage(IKernel* pKernel, const PERSISTID& self, const PERSISTID& target,
                   int iHPDamage, int iHitType, bool bIsDamageEvent,
                   int iDamageCate, const IVarList& params);

protected:

	bool BufferModifyDamage(IKernel* pKernel, const PERSISTID& buffer, float& hp_damage);

    /// ----------------------------------------�˺�ֵ����------------------------------------
    // �����˺����㹫ʽ�����˺�ֵ
    void ModifyDamageByFormula(IKernel* pKernel, const PERSISTID& self,
                               const PERSISTID& skill, const PERSISTID& target,
                               EHIT_TYPE ePhyHitType,
                               float& fDamageValue);

	// ����NPC�˺�����
	bool ModifyDamageBySpecialState(IKernel* pKernel, const PERSISTID& self,
		const PERSISTID& target, int& nOutDamageValue);

	// �˺�Ŀ��ǰ
	bool BeforeDamgeTarget( IKernel* pKernel, const PERSISTID& self, const PERSISTID& skill, const PERSISTID& target,
				const int64_t iSkillUUID, int& nOutHitType, int& nOutDamageVal);
public:
    //��̬����������̬��������
    static FightModule* m_pInstance;
};

#endif
