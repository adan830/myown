//--------------------------------------------------------------------
// �ļ���:		Domain_Task.cpp
// ��  ��:		������Ϣ��
// ˵  ��:		
// ��������:	2016��07��01��
// ������:		  tongzt
// �޸���:        
//--------------------------------------------------------------------

#include "Domain_Task.h"
#include "../../FsGame/Define/PubDefine.h"

#include "../../utils/extend_func.h"
#include "../../utils/record.h"
#include "../../utils/util_func.h"
#include "../../utils/string_util.h"
#include <time.h>
#include "../../FsGame/TaskModule/TaskDefine.h"


// ��ʼ�����
Domain_Task * Domain_Task::m_pDomain_Task = NULL;

// ����
Domain_Task::Domain_Task() : IDomainData(L"", 0)
{
	m_pDomain_Task = this;
}

// ȡ����
const std::wstring & Domain_Task::GetDomainName(IPubKernel *pPubKernel)
{
	// ������
	if (m_domainName.empty())
	{
		wchar_t wstr[256];
		const int server_id = pPubKernel->GetServerId();
		SWPRINTF_S(wstr, L"Domain_Task_%d", server_id);

		m_domainName = wstr;
	}

	return m_domainName;
}

// ����
int Domain_Task::OnCreate(IPubKernel *pPubKernel, IPubSpace *pPubSpace)
{
	return OnLoad(pPubKernel, pPubSpace);
}

// ����
int Domain_Task::OnLoad(IPubKernel*pPubKernel, IPubSpace *pPubSpace)
{
	// ������
	const std::wstring & domain_name = GetDomainName(pPubKernel);
	IPubData * pPubData = pPubSpace->GetPubData(domain_name.c_str());

	if (NULL == pPubData)
	{
		return 1;
	}

	//// ��̽Npcͳ�Ʊ�
	//CVarList col_types;
	//col_types  << VTYPE_STRING
	//				 << VTYPE_INT;
	//CreateRecord(pPubKernel, pPubData, SpyNpcDamageRec, col_types,
	//	SPY_NPC_DAMAGE_REC_COL_MAX, SPY_NPC_DAMAGE_REC_MAX_ROWS, CVarList(), SPY_NPC_DAMAGE_REC_COL_ID);

	// ����һ����������
	if (!pPubData->FindAttr("ResetTaskDate"))
	{
		pPubData->AddAttr("ResetTaskDate", VTYPE_DOUBLE);
		pPubData->SetAttrSave("ResetTaskDate", true);
		pPubData->SetAttrDouble("ResetTaskDate", util_get_date());
	}

	// ��Ӷ�ʱ��
	pPubKernel->AddTimer("ResetTaskTimer", Domain_Task::ResetTaskTimer, 
		pPubSpace->GetSpaceName(), GetDomainName(pPubKernel).c_str(), 5 * 1000, 0);

	return 1;
}

// ��Ϣ
int Domain_Task::OnMessage(IPubKernel *pPubKernel, IPubSpace *pPubSpace,
								 int source_id,  int scene_id, const IVarList & msg)
{
	// ������
	const std::wstring & name = GetDomainName(pPubKernel);
	IPubData * pPubData = pPubSpace->GetPubData(name.c_str());
	if (NULL == pPubData)
	{
		return 1;
	}

	// �������
	if (msg.GetCount() < 4)
	{
		return 1;
	}

	//������
	const int msg_type = msg.IntVal(2);
	const int sub_msg = msg.IntVal(3);
// 	if (msg_type != SP_DOMAIN_MSG_TASK)
// 	{
// 		return 1;
// 	}

	//CVarList args;
	//args.Append(msg, 4, msg.GetCount() - 4);

	//switch (sub_msg)
	//{
	//case S2P_SUBMSG_SPY_NPC_DAMAGE_INC:	// ��̽Npc�˺�ֵ����
	//	IncSpyNpcDamage(pPubKernel, pPubSpace, args);
	//	break;
	//default:
	//	break;
	//}

	return 1;
}

// ����
void Domain_Task::CreateRecord(IPubKernel *pPubKernel, IPubData *pPubData,
									 const char *rec_name, IVarList& cols_type, const int cols, 
									 const int max_row, IVarList& new_col_val, const int rec_key, bool is_save /*= true*/)
{
	if (NULL == pPubKernel 
		|| NULL == pPubData)
	{
		return;
	}

	if (StringUtil::CharIsNull(rec_name))
	{
		return;
	}

	// ȡ��
	IRecord* pRecord = pPubData->GetRecord(rec_name);
	if (pRecord == NULL)		
	{												
		pPubData->AddRecord(rec_name, max_row, cols, cols_type);
		pPubData->SetRecordSave(rec_name, is_save);
	}
	else if (check_record_changed(pRecord, cols_type, max_row))
	{
		// ���¹����
		change_pub_record(pPubData, rec_name, cols_type, new_col_val, max_row, rec_key);
	}
}

// ����ʱ��
int Domain_Task::ResetTaskTimer(IPubKernel *pPubKernel, const char *space_name, 
	const wchar_t *data_name, int time)
{
	IPubSpace *pPubSpace = pPubKernel->GetPubSpace(space_name);
	if (NULL == pPubSpace)
	{
		return 0;
	}

	// ��������
	IPubData * pPubData = pPubSpace->GetPubData(m_pDomain_Task->GetDomainName(pPubKernel).c_str());
	if (NULL == pPubData)
	{
		return 0;
	}

	// ����ʱ�����
	const double last_reset_date = pPubData->QueryAttrDouble("ResetTaskDate");
	const double cur_date = util_get_date();
	const double diff_day = util_get_time_diff(cur_date, last_reset_date);
	if (diff_day < 1 && last_reset_date != 0.0f)	// δ����
	{
		return 0;
	}

	// ��ǰʱ��
	time_t date = ::time(NULL);
	tm* cur_time = ::localtime(&date);
	if (NULL == cur_time)
	{
		return 0;
	}

	// �賿5������
	if (cur_time->tm_hour < DOMAIN_TASK_RESET_TIME)
	{
		return 0;
	}

	// ��������ʱ��
	pPubData->SetAttrDouble("ResetTaskDate", cur_date);

	return 0;
}
