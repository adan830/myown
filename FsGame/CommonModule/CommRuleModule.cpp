// -------------------------------------------
// �ļ����ƣ�	 CommRuleModule.cpp
// �ļ�˵���� ���ù���������һЩͨ�ò��Ҽ򵥵Ĺ���
// �������ڣ� 2018/03/02
// �� �� �ˣ�  tzt
// -------------------------------------------

#include "CommRuleModule.h"
#include "FsGame/Define/GameDefine.h"
#include "FsGame/Define/CommandDefine.h"
#include "FsGame/Define/ClientCustomDefine.h"
#include "FsGame/Define/ServerCustomDefine.h"
#include <vector>
#include "utils/util_func.h"
#include "utils/string_util.h"
#include "ContainerModule.h"
#include "FunctionEventModule.h"
#include "SystemFunctionModule/CapitalModule.h"
#include "EquipmentModule/EquipDefine.h"
#include "Define/PlayerBaseDefine.h"
#include "Define/ItemTypeDefine.h"

CommRuleModule *CommRuleModule::m_pThis = NULL;


// ��ʼ��
bool CommRuleModule::Init(IKernel *pKernel)
{
	m_pThis = this;
	// npc����ɱ
	pKernel->AddIntCommandHook(CLASS_NAME_NPC, COMMAND_BEKILL, CommRuleModule::OnNpcBeKilled);
	// ��ұ���ɱ
	pKernel->AddIntCommandHook(CLASS_NAME_PLAYER, COMMAND_BEKILL, CommRuleModule::OnPlayerBeKilled);
	// ��ұ��˺�
	pKernel->AddIntCommandHook(CLASS_NAME_PLAYER, COMMAND_BEDAMAGE, CommRuleModule::OnPlayerBeDamaged);

	// CG���Ž�����Ϣ
	pKernel->AddIntCustomHook(CLASS_NAME_PLAYER, CLIENT_CUSTOMMSG_PLAY_CG_DONE, CommRuleModule::OnCustomCGDone);

	return true;
}

// �ر�
bool CommRuleModule::Shut(IKernel *pKernel)
{
	return true;
}

// Ӧ��ɱ�ֹ���
void CommRuleModule::ApplyKillNpcRule(IKernel *pKernel, const PERSISTID &npc)
{
	if (NULL == m_pThis)
	{
		return;
	}
	// ɱ�ֶ��鹲�����
	m_pThis->KillNpc_TeamShare(pKernel, npc);
}

// Ӧ��ɱ�˹���
void CommRuleModule::ApplyKillPlayerRule(IKernel *pKernel, const PERSISTID &deader)
{
	if (NULL == m_pThis)
	{
		return;
	}

	// ��Ч��ɱ����
	m_pThis->KillPlayer_Validate(pKernel, deader);
}

// ��¼�������
void CommRuleModule::RecordAttackPlayer(IKernel *pKernel, const PERSISTID &sufferer, 
	const PERSISTID &damager)
{
	// �ܺ���
	IGameObj *pSufferer = pKernel->GetGameObj(sufferer);
	if (NULL == pSufferer)
	{
		return;
	}

	// �����ߣ�����Ļ����ҵ���������
	PERSISTID true_damager = get_pet_master(pKernel, damager);
	IGameObj* pDamager = pKernel->GetGameObj(true_damager);
	if (NULL == pDamager)
	{
		return;
	}

	// ����¼�����
	if (pDamager->GetClassType() != TYPE_PLAYER)
	{
		return;
	}

	// ��Ч�˺���
	IRecord *pDamageRec = pSufferer->GetRecord(FIELD_RECORD_VALID_DAMAGE_REC);
	if (NULL == pDamageRec)
	{
		return;
	}

	// ����
	const int64_t now_time = util_get_time_64();
	LoopBeginCheck(a);
	for (int row = pDamageRec->GetRows() - 1; row >= 0; --row)
	{
		LoopDoCheck(a);
		const int64_t time = pDamageRec->QueryInt64(row, COLUMN_VALID_DAMAGE_REC_TIME);
		const int64_t diff_time = now_time - time;
		// ������Чʱ��
		if (diff_time > VALID_DAMAGE_REC_TIME 
			|| diff_time < 0)
		{
			pDamageRec->RemoveRow(row);
		}
	}

	// ��¼�˺�
	const char *uid = pDamager->QueryString(FIELD_PROP_UID);
	const int exist_row = pDamageRec->FindString(COLUMN_VALID_DAMAGE_REC_UID, uid);
	if (exist_row < 0)
	{
		CVarList row_value;
		row_value << uid
				  << now_time;
		pDamageRec->AddRowValue(-1, row_value);
	}
	else
	{
		pDamageRec->SetInt64(exist_row, COLUMN_VALID_DAMAGE_REC_TIME, now_time);
	}
}

// ��Ҹı����
bool CommRuleModule::ChangeGroup(IKernel* pKernel, const PERSISTID& self, 
	const int change_type, const int new_group/* = 0*/)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj 
		|| TYPE_PLAYER != pSelfObj->GetClassType())
	{
		return false;
	}

	// �������
	if (change_type == GROUP_CHANGE_TYPE_ENTRY)
	{
		// �Ƿ��ķ����
		if (new_group <= 0)
		{
			return false;
		}

		pSelfObj->SetInt(FIELD_PROP_BEFORE_GROUP, pSelfObj->QueryInt(FIELD_PROP_GROUP_ID));
		pSelfObj->SetInt(FIELD_PROP_GROUP_ID, new_group);

		// ����������
		// TODO
	}
	else// �뿪����
	{		
		// ����ǰ����
		int before_group = pSelfObj->QueryInt(FIELD_PROP_BEFORE_GROUP);

		// �ص�ָ������
		if (new_group != 0)
		{
			before_group = new_group;
		}
		
		before_group = before_group == 0 ? -1 : before_group;
		pSelfObj->SetInt(FIELD_PROP_GROUP_ID, before_group);
		pSelfObj->SetInt(FIELD_PROP_BEFORE_GROUP, 0);

		// �����뿪����
		// TODO
	}

	// ֪ͨ�ͻ����з������
	pKernel->Custom(self, CVarList() << SERVER_CUSTOMMSG_SWITCH_GROUP_EFFECT << change_type);
	pKernel->Command(self, self, CVarList() << COMMAND_GROUP_CHANGE << change_type);

	return true;
}

// ����CG
void CommRuleModule::PlayCG(IKernel* pKernel, const PERSISTID& self, 
	const char *cg_id, const IVarList &args/* = CVarList()*/)
{
	if (!pKernel->Exists(self))
	{
		return;
	}

	CVarList s2c_msg;
	s2c_msg << SERVER_CUSTOMMSG_PLYA_CG
			<< cg_id;
	s2c_msg.Concat(args);
	pKernel->Custom(self, s2c_msg);
}

// ����Ϊ�����б�
void CommRuleModule::ParseConsumeVec(const char *src_str, Consume_Vec& consume_vec, 
	const char *del_first /*= ","*/, const char *del_second /*= ":"*/)
{
	if (StringUtil::CharIsNull(src_str))
	{
		return;
	}

	consume_vec.clear();
	CVarList result_first;
	util_split_string(result_first, src_str, del_first);

	LoopBeginCheck(a);
	for (int i = 0; i < (int)result_first.GetCount(); ++i)
	{
		LoopDoCheck(a);
		CVarList result_second;
		util_split_string(result_second, result_first.StringVal(i), del_second);
		if (result_second.GetCount() < 2)
		{
			continue;;
		}

		Consume cfg;
		cfg.id = result_second.StringVal(0);
		cfg.num = result_second.IntVal(1);

		consume_vec.push_back(cfg);
	}
}

// �ɷ�������Ʒ
bool CommRuleModule::CanDecItems(IKernel *pKernel, const PERSISTID &self, 
	const Consume_Vec &items)
{
	IGameObj *pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return false;
	}

	// ȡ�ñ���
	PERSISTID item_box = pKernel->GetChild(self, ITEM_BOX_NAME_WSTR);
	if (!pKernel->Exists(item_box))
	{
		return false;
	}

	// ������Ʒ�Ƿ�����
	bool can_dec = true;
	LoopBeginCheck(a);
	for (int i = 0; i < (int)items.size(); ++i)
	{
		LoopDoCheck(a);
		const char *item_id = items[i].id.c_str();
		const int count = items[i].num;
		if (!StringUtil::CharIsNull(item_id)
			&& count > 0
			&& !ContainerModule::m_pContainerModule->TryRemoveItems(pKernel, item_box, item_id, count))
		{
			can_dec = false;
			break;
		}
	}

	return can_dec;
}

// �۳���Ʒ
bool CommRuleModule::ConsumeItems(IKernel *pKernel, const PERSISTID &self, 
	const Consume_Vec &items, const int function_id)
{
	IGameObj *pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return false;
	}

	// ȡ�ñ���
	PERSISTID item_box = pKernel->GetChild(self, ITEM_BOX_NAME_WSTR);
	if (!pKernel->Exists(item_box))
	{
		return false;
	}

	// �۳���Ʒ
	bool remove_done = true;
	LoopBeginCheck(a);
	for (int i = 0; i < (int)items.size(); ++i)
	{
		LoopDoCheck(a);
		const char *item_id = items[i].id.c_str();
		const int count = items[i].num;
		if (StringUtil::CharIsNull(item_id) || count == 0)
		{
			continue;
		}

		// �۳�
		int remove_count = 0;
		if (FunctionEventModule::GetItemBindPriorUse(function_id))
		{
			remove_count = ContainerModule::RemoveItemsBindPrior(pKernel, item_box,
				item_id, count, EmFunctionEventId(function_id), true);
		}
		else
		{
			remove_count = ContainerModule::m_pContainerModule->RemoveItems(pKernel, item_box,
				item_id, count, EmFunctionEventId(function_id), true);
		}

		if (remove_count != count)
		{
			remove_done = false;
			break;
		}
	}

	return remove_done;
}

// �ɷ������ʽ�
bool CommRuleModule::CanDecCapitals(IKernel *pKernel, const PERSISTID &self,
	const Consume_Vec &capitals)
{
	IGameObj *pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return false;
	}

	// �����ʽ��Ƿ�����
	bool can_dec = true;
	LoopBeginCheck(a);
	for (int i = 0; i < (int)capitals.size(); ++i)
	{
		LoopDoCheck(a);
		const int capital_type = StringUtil::StringAsInt(capitals[i].id);
		const int count = capitals[i].num;

		if (!CapitalModule::m_pCapitalModule->CanDecCapital(pKernel, self, capital_type, count))
		{
			can_dec = false;
			break;
		}
	}

	return can_dec;
}

// �۳��ʽ�
bool CommRuleModule::ConsumeCapitals(IKernel *pKernel, const PERSISTID &self, 
	const Consume_Vec &capitals, const int function_id)
{
	IGameObj *pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return false;
	}

	// �����ʽ��Ƿ�����
	bool dec_done = true;
	LoopBeginCheck(a);
	for (int i = 0; i < (int)capitals.size(); ++i)
	{
		LoopDoCheck(a);
		const int capital_type = StringUtil::StringAsInt(capitals[i].id);
		const int count = capitals[i].num;

		if (CapitalModule::m_pCapitalModule->DecCapital(pKernel, self, capital_type, count, EmFunctionEventId(function_id)) != DC_SUCCESS)
		{
			dec_done = false;
			break;
		}
	}

	return dec_done;
}

// ������������
bool CommRuleModule::ParsePosInfo(PosInfo& outPos, const char* strPos, const char *delims)
{
	CVarList pos;
	StringUtil::SplitString(pos, strPos, delims);
	if (pos.GetCount() != 3)
	{
		return false;
	}
	outPos.fPosX = pos.FloatVal(0);
	outPos.fPosZ = pos.FloatVal(1);
	outPos.fOrient = pos.FloatVal(2);

	return true;
}

// �Ƿ�Ϸ�װ����λ
bool CommRuleModule::IsValidEquipPos(const int equip_pos)
{
	return equip_pos >= EQUIP_POS_STR_MIN 
		&& equip_pos <= EQUIP_POS_STR_MAX;
}

// �Ƿ�Ϸ�ְҵ
bool CommRuleModule::IsValidJob(const int job)
{
	return job > PLAYER_JOB_MIN 
		&& job < PLAYER_JOB_MAX;
}

// �Ƿ�Ϸ�Ʒ��
bool CommRuleModule::IsValidColor(const int color)
{
	return color > COLOR_MIN 
		&& color < COLOR_MAX;
}

// NPC��ɱʱ�ص�
int CommRuleModule::OnNpcBeKilled(IKernel *pKernel, const PERSISTID &self, 
	const PERSISTID &sender, const IVarList &args)
{
	CommRuleModule::m_pThis->ApplyKillNpcRule(pKernel, self);

	return 0;
}

// ��ұ���ɱ�ص�
int CommRuleModule::OnPlayerBeKilled(IKernel *pKernel, const PERSISTID &self, 
	const PERSISTID &sender, const IVarList &args)
{
	CommRuleModule::m_pThis->ApplyKillPlayerRule(pKernel, self);

	return 0;
}

// ��ұ��˺��ص�
int CommRuleModule::OnPlayerBeDamaged(IKernel* pKernel, const PERSISTID& self, 
	const PERSISTID& sender, const IVarList& args)
{
	CommRuleModule::m_pThis->RecordAttackPlayer(pKernel, self, sender);

	return 0;
}

// CG���Ž���
int CommRuleModule::OnCustomCGDone(IKernel *pKernel, const PERSISTID &self, 
	const PERSISTID &sender, const IVarList &args)
{
	if (!pKernel->Exists(self))
	{
		return 0;
	}

	// ֪ͨcg���Ž���
	pKernel->Command(self, self, CVarList() << COMMAND_PLAY_CG_DONE << args.StringVal(1));

	return 0;
}


// ���鹲���ɱ
void CommRuleModule::KillNpc_TeamShare(IKernel *pKernel, const PERSISTID &npc)
{
	IGameObj *pNpc = pKernel->GetGameObj(npc);
	if (NULL == pNpc)
	{
		return;
	}

	// �����б�
	IRecord *pAttackRec = pNpc->GetRecord(FIELD_RECORD_ATTACKER_LIST);
	if (pAttackRec == NULL)
	{
		return;
	}

	// �����б�
	std::vector<int> team_list;

	// ���ܻ�ɱ����б�
	CVarList player_list;

	// npc�ķ����
	const int npc_group_id = pNpc->QueryInt(FIELD_PROP_GROUP_ID);

	// �����б��е���ҿɹ���
	LoopBeginCheck(a);
	for (int row = 0; row < pAttackRec->GetRows(); ++row)
	{
		LoopDoCheck(a);
		if (pAttackRec->QueryInt(row, COLUMN_ATTACKER_LIST_0001) == 0)
		{
			continue;
		}

		PERSISTID obj = pAttackRec->QueryObject(row, COLUMN_ATTACKER_LIST_0000);
		IGameObj* pObj = pKernel->GetGameObj(obj);
		if (pObj == NULL)
		{
			continue;
		}

		if (pObj->GetClassType() != TYPE_PLAYER)
		{
			continue;
		}

		if (pObj->QueryInt(FIELD_PROP_GROUP_ID) != npc_group_id)
		{
			continue;
		}

		// �ж����ֻ��¼�ö����һ����Ա
		const int team_id = 0/*pObj->QueryInt(FIELD_PROP_TEAM_ID)*/; // TODO
		if (team_id > 0)
		{
			std::vector<int>::const_iterator find_it = find(
				team_list.begin(),
				team_list.end(),
				team_id);

			// ��һ�μ�¼
			if (find_it == team_list.end())
			{
				team_list.push_back(team_id);
				player_list << obj;
			}
		}
		else // �޶���ֱ�Ӽ�¼
		{
			player_list << obj;
		}
	}

	// ���鹲���ɱ
	CVarList s2s_msg;
	s2s_msg << COMMAND_KILL_NPC_WITH_RULE;
	LoopBeginCheck(b);
	for (int i = 0; i < (int)player_list.GetCount(); ++i)
	{
		LoopDoCheck(b);
		PERSISTID player = player_list.ObjectVal(i);
		IGameObj* pPlayer = pKernel->GetGameObj(player);
		if (pPlayer == NULL)
		{
			continue;
		}

		// �����Ա
		CVarList member_list;
		bool have_team = false/*TeamModule::m_pTeamModule->GetTeamMemberList(pKernel, player, member_list)*/; // TODO
		if (have_team)
		{
			LoopBeginCheck(c);
			for (int j = 0; j < (int)member_list.GetCount(); ++j)
			{
				LoopDoCheck(c);
				// �����Ա��
				const wchar_t *member_name = member_list.WideStrVal(j);

				// ���������Ҷ����Ա
				const PERSISTID member = pKernel->FindPlayer(member_name);
				IGameObj *pMember = pKernel->GetGameObj(member);
				if (NULL == pMember)
				{
					continue;
				}

				// �����һ��
				if (pMember->QueryInt(FIELD_PROP_GROUP_ID) != npc_group_id)
				{
					continue;
				}

				// �����Ա�뵱ǰ��ҵľ����������
				if (pKernel->Distance2D(npc, member) > TEAM_MEMBER_VERIFY_DISTANCE)
				{
					continue;
				}

				// ���͸�����������
				pKernel->Command(npc, member, s2s_msg);
			}
		}
		else
		{
			// ����Ų�һ��
			if (pPlayer->QueryInt(FIELD_PROP_GROUP_ID) != npc_group_id)
			{
				continue;
			}

			// ������npc�ľ����������
			if (pKernel->Distance2D(npc, player) > TEAM_MEMBER_VERIFY_DISTANCE)
			{
				continue;
			}

			// ���͸�����������
			pKernel->Command(npc, player, s2s_msg);
		}
	}

	team_list.clear();
	player_list.Clear();
}

// ��Ч��ɱ�����֤
void CommRuleModule::KillPlayer_Validate(IKernel *pKernel, const PERSISTID &deader)
{
	// ������
	IGameObj *pDeader = pKernel->GetGameObj(deader);
	if (NULL == pDeader)
	{
		return;
	}

	// ��Ч�˺���¼��
	IRecord *pDamageRec = pDeader->GetRecord(FIELD_RECORD_VALID_DAMAGE_REC);
	if (NULL == pDamageRec)
	{
		return;
	}

	// ����uid
	const char *deader_uid = pDeader->QueryString(FIELD_PROP_UID);

	// ��ɱ��Ϣ
	CVarList s2s_msg;
	s2s_msg << COMMAND_KILL_PLAYER_WITH_RULE;

	// �����ɱ
	LoopBeginCheck(a);
	for (int row = 0; row < pDamageRec->GetRows(); ++row)
	{
		LoopDoCheck(a);
		const char *uid = pDamageRec->QueryString(row, COLUMN_VALID_DAMAGE_REC_UID);
		const int64_t time = pDamageRec->QueryInt64(row, COLUMN_VALID_DAMAGE_REC_TIME);
		const int64_t diff_time = util_get_time_64() - time;
		// ������Чʱ��
		if (diff_time > VALID_DAMAGE_REC_TIME
			|| diff_time < 0)
		{
			continue;
		}

		// ֻ֪ͨ��ǰ�����������
		const wchar_t *name = pKernel->SeekRoleName(uid);
		PERSISTID player = pKernel->FindPlayer(name);
		IGameObj *pPlayer = pKernel->GetGameObj(player);
		if (NULL == pPlayer)
		{
			continue;
		}

		// ��Ҫ����ɱ�Ƿ���Ч
		if (RecordValidKill(pKernel, player, deader_uid))
		{
			pKernel->Command(player, player, s2s_msg);
		}
	}

	pDamageRec->ClearRow();
}

// ��¼��Ч��ɱ
bool CommRuleModule::RecordValidKill(IKernel* pKernel, const PERSISTID& self,
	const char *deader_uid)
{
	IGameObj *pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return false;
	}

	// ��uid����ʾ����ҵ�Npc���������������֤
	if (StringUtil::CharIsNull(deader_uid))
	{
		return true;
	}

	// ��Ч��ɱ��֤��
	IRecord *pKillRec = pSelfObj->GetRecord(FIELD_RECORD_VALID_KILL_REC);
	if (NULL == pKillRec)
	{
		return false;
	}

	// �����ѳ�����Ч����ʱ��ļ�¼
	const int64_t now_time = util_get_time_64();
	LoopBeginCheck(a);
	for (int row = pKillRec->GetRows() - 1; row >= 0; --row)
	{
		LoopDoCheck(a);
		const int64_t diff_time = now_time - pKillRec->QueryInt64(row, COLUMN_VALID_KILL_REC_TIME);
		if (diff_time < 0 || diff_time > LIMIT_KILL_REC_TIME)
		{
			pKillRec->RemoveRow(row);
		}
	}

	// ����
	const int exist_row = pKillRec->FindString(COLUMN_VALID_KILL_REC_UID, deader_uid);
	// �м�¼��ʾ�϶�������
	if (exist_row >= 0)
	{
		return false;
	}

	// �޼�¼����Ч��ɱ�����Ӽ�¼
	CVarList row_value;
	row_value << deader_uid
		<< now_time;
	pKillRec->AddRowValue(-1, row_value);

	return true;
}
