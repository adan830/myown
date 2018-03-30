//--------------------------------------------------------------------
// �ļ���:      skill_event_manager.h
// ��  ��:      �����¼�������
// ˵  ��:
// ��������:    2014��11��25��
// ������:       
//    :       
//--------------------------------------------------------------------
#ifndef _SkillEventManager_H_
#define _SkillEventManager_H_

#include "Fsgame/Define/header.h"
#include "FsGame/Define/SkillEventFuncIdDefine.h"
#include "FsGame/Define/SkillEventManagerDefine.h"

//����������
#define CHECK_PARAM_COUNT(real_count, expect_count)\
	if (real_count < expect_count)\
	{\
	return false;\
	}

struct SkillEffectData;
class SkillEventBase;

class SkillEventManager : public ILogicModule
{
public:
    SkillEventManager() {
		memset(m_pEventContainer, 0, sizeof(m_pEventContainer));
	}
    ~SkillEventManager() {}

public:
    // ��ʼ��
    virtual bool Init(IKernel* pKernel);

    // ����
    virtual bool Shut(IKernel* pKernel);

public:

    // �����¼�������
    bool InitEventExecutes(IKernel* pKernel);

	// ������ִ�в���, �¼�������iEventType���ڼ�����˵��ESkillEventType������buffer��˵��EBufferEventType
	bool ExecuteOperate(IKernel* pKernel,
						const EEventExecType funcid,
						const IVarList& OpParams,
						const PERSISTID& self,
						const PERSISTID& object,
						const int64_t uuid,
						const int iEventType,
						const IVarList& params,
						IVarList& outMsgParams);
public:

    // ���ܼ������ͷ�
    static bool SkillBegin(IKernel* pKernel,
                           const PERSISTID& self,
                           const PERSISTID& skill,
                           int64_t iSkillUUID,
                           const PERSISTID& target,
                           float dx, float dy, float dz);

    // ���ܿ�ʼ׼��
    static bool SkillPrepare(IKernel* pKernel,
                             const PERSISTID& self, 
                             const PERSISTID& skill,
                             const int64_t uuid,
                             const PERSISTID& target,
                             float x, float y, float z);

    // ����׼��֮��
    static bool SkillAfterPrepare(IKernel* pKernel, 
                                  const PERSISTID& self,
                                  const PERSISTID& skill,
                                  const int64_t uuid,
                                  const PERSISTID& target,
                                  float x, float y, float z);

    // ��������֮ǰ
    static bool SkillBeforeHit(IKernel* pKernel,
                               const PERSISTID& self,
                               const PERSISTID& skill,
                               const int64_t uuid);

    // �������ж���
    static bool SkillAfterHit(IKernel* pKernel,
                              const PERSISTID& self,
                              const PERSISTID& skill, 
                              const int64_t uuid,
                              const PERSISTID& target);

    // �������в����˺��˶���
    static bool SkillHitDamage(IKernel* pKernel,
                               const PERSISTID& self,
                               const PERSISTID& skill, 
                               const int64_t uuid,
                               const PERSISTID& target,
							   const int nDamageVal);

    // ����ʹ�ý���
    static bool SkillFinish(IKernel* pKernel,
                            const PERSISTID& self,
                            const PERSISTID& skill,
                            const int64_t uuid,
                            const PERSISTID& target);

    // ���ܱ����
    static bool SkillBreak(IKernel* pKernel,
                           const PERSISTID& self, 
                           const PERSISTID& skill,
                           const int64_t uuid);

	// ����ʹ�ý���֮��
	static bool SkillAfterFinish(IKernel* pKernel,
		const PERSISTID& self,
		const PERSISTID& skill,
		const int64_t uuid);

private:
	// �¼�ע��
	bool RegistSkillEvent(const EEventExecType func_id);

    //�����¼�����
    bool SkillEventProcess(IKernel* pKernel,
                                  const PERSISTID& self,
                                  const PERSISTID& skill,
                                  const int64_t uuid,
                                  const PERSISTID& target, 
                                  const ESkillEventType eSkillEventType,
                                  const EParamType eParamType,
                                  const IVarList& exParams);

    // ��ȡ���ܵ�ǰ���д���ĸ߼�Ч����
    bool GetEffectPackList(IKernel* pKernel,
								  const PERSISTID& self,
                                  const PERSISTID& skill,
                                  CVarList& vEffectPackList);

    // �ж�ָ���߼�Ч����ִ������
    bool CanExecute(IKernel* pKernel,
                           const PERSISTID& self,
                           const PERSISTID& target,
                           const SkillEffectData* pSkillEffect,
                           const ESkillEventType eSkillEventType);

    // ��ȡ�¼�ִ�в���
    bool GetParams(IKernel* pKernel,
                          const PERSISTID& self,
                          const PERSISTID& skill,
                          const PERSISTID& target,
                          const SkillEffectData* pSkillEffect,
                          const EParamType eParamType,
                          const IVarList& exParams,
                          CVarList& params);
private:
	SkillEventBase*		m_pEventContainer[MAX_EXECUTE];			// �����¼�����

public:
    static SkillEventManager*   m_pInstance;
};

#endif // _SkillEventManager_H_
