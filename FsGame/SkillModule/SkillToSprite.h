//--------------------------------------------------------------------
// �ļ���:      SkillToSprite.h
// ��  ��:      NPC����ҵļ��ܵ���ӣ�ɾ���Ͳ�ѯ
// ˵  ��:
// ������:       
// ��������:    2014��10��24��
//    :       
//--------------------------------------------------------------------
#ifndef FSGAME_SKILLMODULE_SKILL_TO_SPRITE_H_
#define FSGAME_SKILLMODULE_SKILL_TO_SPRITE_H_

#include "Fsgame/Define/header.h"
#include "utils/Singleton.h"


class SkillToSprite
{
public:
    // ��ȡ����ļ�����������
    PERSISTID GetSkillContainer(IKernel* pKernel, const PERSISTID& self);

    /// �Ƿ����м���
    bool Contains(IKernel* pKernel, const PERSISTID& self, const char* szSkillConfigID);

    /// ��ռ��������еļ���
    bool ClearAllSkill(IKernel* pKernel, const PERSISTID& self);
   
	/// \brief ����ĳ��ɫ�����Ƿ���ָ���ļ���
    /// \param pKernel
    /// \param self ��Ӽ��ܵ�npc�����
    /// \param szSkillConfigID Ҫ���ҵļ�������id
    PERSISTID FindSkill(IKernel* pKernel, const PERSISTID& self, const char* szSkillConfigID);

    // ���/ɾ������
    bool AddSkill(IKernel* pKernel, const PERSISTID& self, const PERSISTID& skill);
    bool AddSkill(IKernel* pKernel, const PERSISTID& self, const char* szSkillConfigID);
    bool RemoveSkill(IKernel* pKernel, const PERSISTID& self, const PERSISTID& skill);

	// ����ҵ�һ��δ��ȴ�ļ���
	const PERSISTID FindRandomSkillNotInCD(IKernel* pKernel, const PERSISTID& self, float& fSkillDis);

	// �ж������Ƿ�Ϸ�,�Ϸ��Ļ�ȡ�����ܵľ���
	bool IsSkillValid(IKernel* pKernel, const PERSISTID& self, const PERSISTID& skill, float& fSkillDis);

	// ������ļ��ܵȼ�������Ϊ1��
//	void SetSkillLevelMinLevel(IKernel* pKernel, const PERSISTID& self);
private:

    /// ����������ָ��λ����Ӽ���
    bool AddSkillToContainer(IKernel* pKernel, const PERSISTID& self, const PERSISTID& skill, int pos = 0);

    /// ɾ��ָ�����������е�ָ������
    bool RemoveSkillFromContainer(IKernel* pKernel, const PERSISTID& self, const PERSISTID& skill);

	// ����ҵ�һ������id
	const PERSISTID RandomFindSkillId(IKernel* pKernel, const PERSISTID& self);
};

typedef HPSingleton<SkillToSprite> SKillToSpriteSingleton;

#endif // FSGAME_SKILLMODULE_SKILL_TO_SPRITE_H_
