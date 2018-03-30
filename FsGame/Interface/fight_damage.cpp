//------------------------------------------------------------------------------
// �ļ���:      fight_damage.cpp
// ��  ��:      ս���˺��������ʵ��
// ˵  ��:
// ��������:    2014��10��31��
// ������:       
// ��ע:
//    :       
//------------------------------------------------------------------------------


#include "FsGame/Interface/FightInterface.h"

#include "FsGame/Define/FightDefine.h"
#include "FsGame/SkillModule/FightModule.h"


// ֱ���˺��Է�
int FightInterface::Damage(IKernel* pKernel,
                           const PERSISTID& self,
                           const PERSISTID& target,
                           const PERSISTID& object,
                           const int64_t uuid,
                           bool bCanTriggerEvent,
                           int iDamageValue)
{
    if (!pKernel->Exists(self) || !pKernel->Exists(target))
    {
        return 0;
    }

    return FightModule::m_pInstance->EventDamageTarget(pKernel,
                                                          self,
                                                          target,
                                                          object, 
                                                          uuid,
                                                          bCanTriggerEvent,
                                                          iDamageValue);
}