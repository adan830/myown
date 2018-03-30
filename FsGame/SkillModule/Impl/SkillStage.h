//------------------------------------------------------------------------------
// �ļ���:      SkillStage.h
// ��  ��:      ���ܽ׶���Ϣ����
// ˵  ��:      ����ֻ�ڼ���ϵͳ�ڲ�ʹ�ã��Ҳ�Ҫֱ�Ӷ���SkillStage��ʵ������ʹ��
//              �䵥��ģʽ�ӿ�SkillStageSingleton::Instance()
// ��������:    2014��10��23��
// ������:       
// ��ע:
//    :       
//------------------------------------------------------------------------------

#ifndef __SKILLSTAGE_H__
#define __SKILLSTAGE_H__

#include "Fsgame/Define/header.h"

#include "utils/singleton.h"
#include "FsGame/Define/SkillDefine.h"
#include "FsGame/Define/SkillEventManagerDefine.h"

enum EMessageSendType
{
    EMSG_SEND_TYPE_ALL,         // ��Ϣ�㲥�����ӷ�Χ�ڵ�������
    EMSG_SEND_TYPE_SELF,        // ��Ϣֻ�����Լ�
    EMSG_SEND_TYPE_SELF_TARGET, // ��Ϣֻ�����Լ��ͶԷ�
};

class SkillStage
{
public:
    // ��ʼ	
    bool CustomSkillBeginStage(IKernel* pKernel,
                               const PERSISTID& self,
                               const char* szSkillConfigID,
                               const int64_t uuid, 
                               const ESkillStageTargetType eSkillStageTargetType, 
                               const IVarList& params);

    // ����
    bool CustomSkillPrepareStage(IKernel* pKernel,
                                 const PERSISTID& self,
                                 const int64_t uuid,
                                 const int iPrepareTime);

    // ����
    bool CustomSkillLeadStage(IKernel* pKernel,
                              const PERSISTID& self,
                              const int64_t uuid,
                              const int iLeadTime);

    // ��������Ч�������ϲ���Ϣ����
    bool CustomSkillEffectStageRealTime(IKernel* pKernel,
                                        const PERSISTID& self,                      // ������
                                        const int64_t uuid,                         // ����uuid
                                        const ESkillEventType eEventType,           // �����¼�����
                                        const PERSISTID& target,                    // Ч�����ö��� 
                                        const ESkillStageEffectType eEffectType,    // Ч��Ч������ID
                                        const IVarList& params,                     // Ч�����跢�͵�����
                                        const EMessageSendType eMsgSendType);       // ��Ϣ���͸���Щ����

    // ��Ϣֻ�����Լ�
    bool CustomSkillEffectStageBySelf(IKernel* pKernel,
                                      const PERSISTID& self,              // ������
                                      const int64_t uuid,                 // ����uuid
                                      const int iCompressValue,			 // �����¼�����Ч���ͺϲ�ֵ
                                      const IVarList& params);            // eEventType���͵�����Ч���Ĳ�����ÿ��Ч���Ĳ�����Ҫ���������ĸ�ʽ�����Ϣ

    // Ч��
    bool CustomSkillEffectStage(IKernel* pKernel,
                                const PERSISTID& self,              // ������
                                const int64_t uuid,                 // ����uuid
                                const int iCompressValue,			 // �����¼�����Ч���ͺϲ�ֵ
                                const IVarList& params);            // eEventType���͵�����Ч���Ĳ�����ÿ��Ч���Ĳ�����Ҫ���������ĸ�ʽ�����Ϣ

    // ���
    bool CustomSkillBreakStage(IKernel* pKernel,
                               const PERSISTID& self,
                               const int64_t uuid,
                               const int reason);

    // ����
    bool CustomSkillFinish(IKernel* pKernel,
                           const PERSISTID& self,
                           const int64_t uuid,
                           const int reason);

	// �����ܿշ�
	int CustomSkillWithoutTarget(IKernel* pKernel, const PERSISTID& self, const PERSISTID& skill, float fOrient);

	// ���Ͳ��Ż���Ч��
//	int CustomSkillHitFly(IKernel* pKernel, const PERSISTID& self, const PERSISTID& target,const PERSISTID& skill);

	// �ͻ�������ʧ�ܷ��� ��������
	int CustomSkillClientHitFailed(IKernel* pKernel, const PERSISTID& self, const char* strSkillId, int nFailedType, const IVarList& targetList);

}; // end of class SkillStage

typedef HPSingleton<SkillStage> SkillStageSingleton;


#endif // __SKILLSTAGE_H__


