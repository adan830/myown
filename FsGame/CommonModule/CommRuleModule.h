// -------------------------------------------
// �ļ����ƣ�	 CommRuleModule.h
// �ļ�˵���� ���ù���������һЩͨ�ò��Ҽ򵥵Ĺ���
// �������ڣ� 2018/03/02
// �� �� �ˣ�  tzt
// -------------------------------------------

#ifndef __CommRuleModule_H__
#define __CommRuleModule_H__

#include "CommRuleDefine.h"

class CommRuleModule : public ILogicModule
{
public:
	// ��ʼ��
	virtual bool Init(IKernel *pKernel);

	// �ر�
	virtual bool Shut(IKernel *pKernel);
public:
	// Ӧ��ɱ�ֹ���
	static void ApplyKillNpcRule(IKernel *pKernel, const PERSISTID &npc);

	// Ӧ��ɱ�˹���
	static void ApplyKillPlayerRule(IKernel *pKernel, const PERSISTID &deader);

	// ��¼�������
	static void RecordAttackPlayer(IKernel *pKernel, const PERSISTID &sufferer,
		const PERSISTID &damager);

	// ��Ҹı����
	static bool ChangeGroup(IKernel* pKernel, const PERSISTID& self,
		const int change_type, const int new_group = 0);

	// ����CG
	static void PlayCG(IKernel* pKernel, const PERSISTID& self,
		const char *cg_id, const IVarList &args = CVarList());

	// ����Ϊ�����б�
	static void ParseConsumeVec(const char *src_str, Consume_Vec& consume_vec, 
		const char *del_first = ",", const char *del_second = ":");

	// �ɷ�������Ʒ
	static bool CanDecItems(IKernel *pKernel, const PERSISTID &self,
		const Consume_Vec &items);

	// �۳���Ʒ
	static bool ConsumeItems(IKernel *pKernel, const PERSISTID &self,
		const Consume_Vec &items, const int function_id);

	// �ɷ������ʽ�
	static bool CanDecCapitals(IKernel *pKernel, const PERSISTID &self,
		const Consume_Vec &capitals);

	// �۳��ʽ�
	static bool ConsumeCapitals(IKernel *pKernel, const PERSISTID &self,
		const Consume_Vec &capitals, const int function_id);

	// �Ƿ�Ϸ�װ����λ
	static bool IsValidEquipPos(const int equip_pos);

	// �Ƿ�Ϸ�ְҵ
	static bool IsValidJob(const int job);

	// �Ƿ�Ϸ�Ʒ��
	static bool IsValidColor(const int color);

private:
	// NPC��ɱʱ�ص�
	static int OnNpcBeKilled(IKernel *pKernel, const PERSISTID &self,
		const PERSISTID &sender, const IVarList &args);

	// ��ұ���ɱ�ص�
	static int OnPlayerBeKilled(IKernel *pKernel, const PERSISTID &self,
		const PERSISTID &sender, const IVarList &args);

	// ��ұ��˺��ص�
	static int OnPlayerBeDamaged(IKernel* pKernel, const PERSISTID& self,		  
		const PERSISTID& sender, const IVarList& args);

	// CG���Ž���
	static int OnCustomCGDone(IKernel *pKernel, const PERSISTID &self,
		const PERSISTID &sender, const IVarList &args);

	// ���鹲���ɱ
	void KillNpc_TeamShare(IKernel *pKernel, const PERSISTID &npc);

	// ��Ч��ɱ�����֤
	void KillPlayer_Validate(IKernel *pKernel, const PERSISTID &deader);

	// ��¼��Ч��ɱ
	bool RecordValidKill(IKernel* pKernel, const PERSISTID& self,
		const char *deader_uid);

public:
	static CommRuleModule *m_pThis;
    
};
//typedef HPSingleton<CommRuleModule> CommRuleS;

#endif

