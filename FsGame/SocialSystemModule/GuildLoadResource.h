#ifndef __GuildLoadResource_H__
#define __GuildLoadResource_H__

#include "../Define/GuildDefine.h"

class IKernel;

// ���ع�������������Ϣ
bool LoadGuildUpLevelConfig(const char *resourcePath, GuildUpLevelConfigVector &configVec);

// ���ع������������Ϣ
bool LoadGuildDonateConfig(const char *resourcePath, GuildDonateConfigVector &configVec);

// ���ع���ְ��������Ϣ
bool LoadGuildPositionConfig(const char *resourcePath, GuildPositionConfigVector &configVec);

// ���ع����̵�������Ϣ
bool LoadGuildShopConfig(const char *resourcePath, GuildShopConfigVector &configVec);

// ���ع����̵����ö�ʱ��
bool LoadGuildTimer(const char *resourcePath, GuildTimerCallbackVector &configVec);

//���ع��Ἴ��
bool LoadGuildSkill(const char*resourcePath, GuildSkillConfigMap&configVec);
// ���ؼ�����������
bool LoadGuildSkillLevelUpConst(const char*resourcePath, GuildSkillLevelUpSpendMap&configVec);
//������������
bool LoadGuildSkillLvUpExp(const char*resourcePath, GuildSkillLvExp&configVec);

#endif // __GuildLoadResource_H__