#include "GuildBaseManager.h"
#include "FsGame\Define\GuildDefine.h"
#include "utils\record.h"
#include "GuildManager.h"
#include "utils\string_util.h"
#include "FsGame\SocialSystemModule\GuildLoadResource.h"
#include "FsGame\Define\GameDefine.h"
#include <time.h>
#include "utils\custom_func.h"
#include "GuildNumManage.h"
#include "utils\util_func.h"
#include <list>
#include "FsGame\Define\CommandDefine.h"
#include "..\Domain\EnvirValue.h"
#include "..\FsBindLogic.h"
#include "FsGame\Define\RankingDefine.h"
#include "..\Domain\DomainRankList.h"
#include "FsGame\Define\SnsDefine.h"
#include "FsGame\SocialSystemModule\GuildHelper.h"
//#include "FsGame\Helper\GuildHelper.h"

GuildBaseManager* GuildBaseManager::m_pInstance = NULL;
int GuildBaseManager::m_autoImpeachMaxTime = 0;
int GuildBaseManager::m_memOffLineTimes = 0;
int GuildBaseManager::m_stationGroup = 0;

bool GuildBaseManager::Init(IPubKernel* pPubKernel)
{
	if (!InitPubGuildData(pPubKernel))
	{
		return false;
	}

	// ���������
	m_GuildSortHelperSet.clear();
	CheckAndRefreshSortRecord();


	IRecord* pGuildListRec = m_pGuildPubData->GetRecord(GUILD_LIST_REC);
	if (pGuildListRec == NULL)
	{
		return 0;
	}
	int rows = pGuildListRec->GetRows();
	int64_t offlineTime = ::time(NULL);
	// ѭ������
	LoopBeginCheck(ao);
	for (int i = 0; i < rows; i++)
	{
		LoopDoCheck(ao);
		// �������״̬
		const wchar_t *guildName = pGuildListRec->QueryWideStr(i, GUILD_LIST_REC_COL_NAME);
		IRecord* memberList = GetGuildDataRecOpt(guildName, GUILD_MEMBER_REC);
		if (memberList == NULL)
		{
			continue;
		}
		int memberCount = memberList->GetRows();
		// ѭ������
		LoopBeginCheck(ap);
		for (int idx = 0; idx < memberCount; idx++)
		{
			LoopDoCheck(ap);
			int online = memberList->QueryInt(idx, GUILD_MEMBER_REC_COL_ONLINE);
			if (online == ONLINE)
			{
				memberList->SetInt(idx, GUILD_MEMBER_REC_COL_ONLINE, OFFLINE);
				memberList->SetInt64(idx, GUILD_MEMBER_REC_COL_OUTLINE_DATE, offlineTime);
			}
		}
	}




	if (!LoadResource(pPubKernel))
	{
		return false;
	}

	return true;
}

bool GuildBaseManager::InitPubGuildData(IPubKernel* pPubKernel)
{
	bool ret = false;
	{
		// ����б��ж���
		CVarList cols;
		cols << VTYPE_WIDESTR
			<< VTYPE_STRING
			<< VTYPE_WIDESTR
			<< VTYPE_INT
			<< VTYPE_WIDESTR
			<< VTYPE_INT64
			<< VTYPE_INT
			<< VTYPE_INT64
			<< VTYPE_WIDESTR
			<< VTYPE_INT;

		ret = _create_pub_record_safe(m_pGuildPubData, GUILD_LIST_REC, cols, CVarList(), GUILD_LIST_REC_ROW_MAX, GUILD_LIST_REC_COL_NAME);
		if (!ret){
			return false;
		}
	}
	
	{
		// �����û���������б����򴴽�
		CVarList cols;
		cols << VTYPE_STRING
			<< VTYPE_WIDESTR
			<< VTYPE_WIDESTR;

		ret = _create_pub_record_safe(m_pGuildPubData, GUILD_APPLY_REC, cols, CVarList(), GUILD_APPLY_LIST_ROW_MAX);
		if (!ret){
			return false;
		}
	}
	
	{
		// �����û��������Ҽ��빫�����򴴽�
		CVarList cols;
		cols << VTYPE_STRING
			<< VTYPE_WIDESTR
			<< VTYPE_WIDESTR
			<< VTYPE_INT64;
		
		ret = _create_pub_record_safe(m_pGuildPubData, GUILD_OUTLINE_JOIN_REC, cols, CVarList(), GUILD_OUTLINE_JOIN_ROW_MAX);
		if (!ret){
			return false;
		}
	}
	
	
	{
		// ���������������ڣ��򴴽�
		CVarList cols;
		cols << VTYPE_WIDESTR;
		ret = _create_pub_record_safe(m_pGuildPubData, GUILD_SORT_REC, cols, CVarList(), GUILD_SORT_REC_ROW_MAX, GUILD_SORT_REC_COL_NAME,false);
		if (!ret){
			return false;
		}
	}

	
	
	{
		// �����̵���߹��������
		CVarList cols;
		cols << VTYPE_WIDESTR << VTYPE_INT;
		ret = _create_pub_record_safe(m_pGuildPubData, GUILD_SHOP_BUY_REC, cols, CVarList(), GUILD_SHOP_BUY_REC_ROW_MAX);
		if (!ret){
			return false;
		}
	}
	
	
	{
		// �����Ա�������ͳ�Ʊ�
		CVarList cols;
		cols << VTYPE_STRING << VTYPE_INT;
		ret = _create_pub_record_safe(m_pGuildPubData, GUILD_MEMBER_BUY_REC, cols, CVarList(), GUILD_MEMBER_BUY_REC_ROW_MAX);
		if (!ret){
			return false;
		}
	}

	{//��֯��ʶ
		CVarList cols;
		cols << VTYPE_WIDESTR << VTYPE_INT << VTYPE_WIDESTR;
		ret = _create_pub_record_safe(m_pGuildPubData, GUILD_SYSMBOL_REC, cols, CVarList(), GUILD_LIST_REC_ROW_MAX);
		if (!ret){
			return false;
		}
	}

	{//��֯������Ϣ
		CVarList cols;
		cols << VTYPE_WIDESTR
			<< VTYPE_INT
			<< VTYPE_INT
			<< VTYPE_INT
			<< VTYPE_INT
			<< VTYPE_INT
			<< VTYPE_INT
			<< VTYPE_INT;
		ret = _create_pub_record_safe(m_pGuildPubData, GUILD_SET_REC, cols, CVarList(), GUILD_LIST_REC_ROW_MAX);
		if (!ret){
			return false;
		}

	}

	// ��ʼ����᳡��������Դ��
	InitGuildStationGroupIDRec();

	// �رշ�����ʱ��
	if (!m_pGuildPubData->FindAttr("CloseYear"))
	{
		m_pGuildPubData->AddAttr("CloseYear", VTYPE_INT);
	}
	if (!m_pGuildPubData->FindAttr("CloseMonth"))
	{
		m_pGuildPubData->AddAttr("CloseMonth", VTYPE_INT);
	}
	if (!m_pGuildPubData->FindAttr("CloseDay"))
	{
		m_pGuildPubData->AddAttr("CloseDay", VTYPE_INT);
	}
	if (!m_pGuildPubData->FindAttr("CloseHour"))
	{
		m_pGuildPubData->AddAttr("CloseHour", VTYPE_INT);
	}
	if (!m_pGuildPubData->FindAttr("CloseMinute"))
	{
		m_pGuildPubData->AddAttr("CloseMinute", VTYPE_INT);
	}
	if (!m_pGuildPubData->FindAttr("CloseSecond"))
	{
		m_pGuildPubData->AddAttr("CloseSecond", VTYPE_INT);
	}
	pPubKernel->SavePubSpace(GetName());
	return true;
}

// ��ʼ����᳡��������Դ��
int GuildBaseManager::InitGuildStationGroupIDRec()
{
	//1��������᳡��������Դ��
	if (!m_pGuildPubData->FindRecord(GUILD_STATION_GROUP_REC))
	{
		CVarList cols;
		cols << VTYPE_WIDESTR
			<< VTYPE_INT;

		if (!m_pGuildPubData->AddRecord(GUILD_STATION_GROUP_REC,
			GUILD_LIST_REC_ROW_MAX, GUILD_STATION_GROUP_REC_COL_MAX, cols))
		{
			return false;
		}
		m_pGuildPubData->SetRecordSave(GUILD_STATION_GROUP_REC, false);
	}

	//2����ʼ����᳡��������Դ����
	InitGuildStationGroupIDData();

	return 0;
}

// ��ʼ����᳡��������Դ����
int GuildBaseManager::InitGuildStationGroupIDData()
{
	//1����ȡ�����б�
	IRecord* guildList = m_pGuildPubData->GetRecord(GUILD_LIST_REC);
	if (guildList == NULL)
	{
		return 0;
	}

	//2����ȡ���᳡������ż�¼��
	IRecord* guildStationList = m_pGuildPubData->GetRecord(GUILD_STATION_GROUP_REC);
	if (guildStationList == NULL)
	{
		return 0;
	}
	
	//3��������ð���������
	guildStationList->ClearRow();

	//4�����·��������ID
	int guildCount = guildList->GetRows();
	LoopBeginCheck(a);
	for (int i = 0; i < guildCount; ++i)
	{
		LoopDoCheck(a);
		const wchar_t* guildName = guildList->QueryWideStr(i, GUILD_LIST_REC_COL_NAME);
		if (StringUtil::CharIsNull(guildName))
		{
			continue;
		}
		//��������������֯��ɢ��ʱ��
		CVarList var;
		var << guildName << (10 + ++m_stationGroup);  // ����� << �����(��10��ʼ)
		guildStationList->AddRowValue(-1, var);
	}

	return 0;
}

bool GuildBaseManager::LoadResource(IPubKernel* pPubKernel)
{
	if (!LoadGuildUpLevelConfig(pPubKernel->GetResourcePath(), m_GuildUpLevelConfig))
	{
		return false;
	}

// 	if (!LoadGuldImpeach(pPubKernel))
// 	{
// 		return false;
// 	}

	return true;
}


// ���������������
void GuildBaseManager::CheckAndRefreshSortRecord()
{
	// ��������
	IRecord* pGuildSortRecord = m_pGuildPubData->GetRecord(GUILD_SORT_REC);
	if (pGuildSortRecord == NULL)
	{
		return;
	}

	// ����б�
	IRecord* pGuildList = m_pGuildPubData->GetRecord(GUILD_LIST_REC);
	if (pGuildList == NULL)
	{
		return;
	}



	// �����������
	m_GuildSortHelperSet.clear();

	// �������в������ݣ�����������
	GuildSortHelper_t tempHelper;
	int memberCount = pGuildList->GetRows();
	// ѭ������
	LoopBeginCheck(ar);
	for (int i = 0; i < memberCount; ++i)
	{
		LoopDoCheck(ar);
		if (!DumpGuildSortHelper(pGuildList, i, &tempHelper))
		{
			const wchar_t* wstrGuildName = pGuildList->QueryWideStr(i, GUILD_LIST_REC_COL_NAME);
			extend_warning_pub(LOG_ERROR, "CheckAndRefreshSortRecord error DumpGuildSortHelper failed GuildName:%s",
				(wstrGuildName != NULL ? StringUtil::WideStrAsString(wstrGuildName).c_str() : ""));
			continue;
		}

		m_GuildSortHelperSet.insert(tempHelper);
	}

	// ��������
	pGuildSortRecord->ClearRow();

	// ������������
	// ѭ������
	LoopBeginCheck(as);
	CVarList varList;
	for (GuildSortHelperSetIter iter = m_GuildSortHelperSet.begin();
		iter != m_GuildSortHelperSet.end(); ++iter)
	{
		LoopDoCheck(as);
		varList.Clear();
		varList << iter->guildName;
		pGuildSortRecord->AddRowValue(-1, varList);
	}
}




void GuildBaseManager::FireNumResetTimerCb(IPubKernel* pPubKernel, const char* space_name, const wchar_t* data_name)
{
	IPubSpace* pPubSpace = pPubKernel->GetPubSpace(space_name);
	if (pPubSpace == NULL)
	{
		return;
	}
	IPubData* guildPubData = PubGuildManager::m_pInstance->m_pGuildPubData;
	if (guildPubData == NULL)
	{
		return;
	}
	IRecord* pGuildList = guildPubData->GetRecord(GUILD_LIST_REC);
	if (pGuildList == NULL)
	{
		return;
	}

	// �����й�������˼�������
	int nRows = pGuildList->GetRows();
	LoopBeginCheck(a);
	for (int i = 0; i < nRows; ++i)
	{
		LoopDoCheck(a);
		std::wstring wstrGuildName = pGuildList->QueryWideStr(i, GUILD_LIST_REC_COL_NAME);
		//����Ƶ��
		std::wstring guildDataName = wstrGuildName + util_int_as_widestr(pPubKernel->GetServerId());
		IPubData *pPubData = pPubSpace->GetPubData(guildDataName.c_str());
		if (NULL == pPubData)
		{
			continue;;
		}
		// �����˴�������
		if (pPubData->FindAttr("FireNum"))
		{
			pPubData->SetAttrInt("FireNum", 0);
		}
	}
}

// �輧����
void GuildBaseManager::DancingGirlReset(IPubKernel* pPubKernel, const char* space_name, const wchar_t* data_name)
{

	IPubSpace* pPubSpace = pPubKernel->GetPubSpace(space_name);
	if (pPubSpace == NULL)
	{
		return;
	}

	// ����б�
	IRecord* pGuildSetList = PubGuildManager::m_pInstance->m_pGuildPubData->GetRecord(GUILD_SET_REC);
	if (pGuildSetList == NULL)
	{
		return;
	}

	// �������а��
	int nRows = pGuildSetList->GetRows();
	for (int i = 0; i < nRows; ++i)
	{
		std::wstring guildName = pGuildSetList->QueryWideStr(i, GUILD_LIST_REC_COL_NAME);
		int girlSate = pGuildSetList->QueryInt(i, GUILD_SET_REC_COL_DANCING_GIRL);
		if (girlSate == 0)
		{
			GuildNumManage::m_pInstance->DecJianKangDu(guildName.c_str(), GUILD_NUM_CHANGE_NOT_OPEN_GUILD_ACTIVITY);
		}

		pGuildSetList->SetInt(i, GUILD_SET_REC_COL_DANCING_GIRL, 0);
	}


}





int GuildBaseManager::OnPublicMessage(IPubKernel* pPubKernel, int source_id, int scene_id, const IVarList& msg)
{

	if (m_pPubSpace == NULL || m_pGuildPubData == NULL)
	{
		return 0;
	}


	const wchar_t* guildName = msg.WideStrVal(1);
	int msgId = msg.IntVal(2);


	switch (msgId)
	{
		case SP_GUILD_MSG_CREATE:// ���󴴽�����
		{
			OnCreateGuild(pPubKernel, guildName, source_id, scene_id, msg);
		}
		break;
		case SP_GUILD_MSG_APPLY_JOIN:// ������빫��
		{
			OnApplyJoinGuild(pPubKernel, guildName, source_id, scene_id, msg);
		}
		break;
		case SP_GUILD_START_ONE_KEY_APPLY_JOIN://һ���������
		{
			OnOneKeyApplyJoinGuild(pPubKernel, source_id, scene_id, msg);
		}break;
		case SP_GUILD_MSG_CANCEL_APPLY_JOIN:// ȡ�����빫������
		{
			OnCancelApplyJoinGuild(pPubKernel, guildName, source_id, scene_id, msg);
		}
		break;
		case SP_GUILD_MSG_ACCEPT_JOIN:// ���ܼ��빫�����������
		{
			OnAcceptJoinGuild(pPubKernel, guildName, source_id, scene_id, msg);
		}
		break;
		case SP_GUILD_MSG_REFUSE_JOIN:// �ܾ����빫������
		{
			OnRefuseJoinGuild(pPubKernel, guildName, source_id, scene_id, msg);
		}
		break;
		case SP_GUILD_MSG_MEMBER_ONLINE_STATE:  // �����Ա������
		{
			OnMemberOnlineStateChange(pPubKernel, guildName, source_id, scene_id, msg);
		}
		break;
		case SP_GUILD_MSG_DELETE_JOIN_RECORD:   // ��Ա������ʱ�����빫��
		{
			OnDeleteOutlineJoinRecord(pPubKernel, guildName, source_id, scene_id, msg);
		}
		break;

		case SP_GUILD_MSG_UPDATE_PROPS: //���³�Ա����
		{
			OnUpdateProps(pPubKernel, guildName, source_id, scene_id, msg);
		}
		break;
		case SP_GUILD_MSG_UPDATE_APPLY_PROPS: //��������������
		{
			OnUpdateApplyProps(pPubKernel, guildName, source_id, scene_id, msg);
		}
		break;
		case  SP_GUILD_MSG_UPDATE_APPLY_ONLINE_STATE:
		{
			OnUpdateApplyOnline(pPubKernel, guildName, source_id, scene_id, msg);
		}break;
		case SP_GUILD_MSG_CHANGE_NOTICE:// �޸Ĺ���
		{
			OnChangeNotice(pPubKernel, guildName, source_id, scene_id, msg);
		}
		break;
		case SP_GUILD_CHANGE_DECLARATION:// �޸�����
		{
			OnChangeDeclaration(pPubKernel, guildName, source_id, scene_id, msg);
		}
		break;
		case SP_GUILD_MSG_QUIT:// �˳�����
		{
			OnQuit(pPubKernel, guildName, source_id, scene_id, msg);
		}
		break;
		case SP_GUILD_MSG_FIRE:// �߳�����
		{
			OnFire(pPubKernel, guildName, source_id, scene_id, msg);
		}
		break;
		case SP_GUILD_MSG_DISMISS:// ��ɢ����
		{
			OnDismiss(pPubKernel, guildName, source_id, scene_id, msg);
		}
		break;
		case SP_GUILD_MSG_DONATE: //����
		{
			OnDonate(pPubKernel, guildName, source_id, scene_id, msg);
		}
		break;
		case SP_GUILD_MSG_PROMOTION:
		case SP_GUILD_MSG_DEMOTION:  //ְλ�䶯
		{
			OnPromotion(pPubKernel, guildName, source_id, scene_id, msg);
		}
		break;
		
		case SP_GUILD_MSG_CAPTAIN_TRANSFER: //�ƽ�����
		{
			OnCaptainTransfer(pPubKernel, guildName, source_id, scene_id, msg);
		}
		break;
		case SP_GUILD_MSG_CHAT_INFO:
		{
			OnSaveGuidChannelInfo(pPubKernel, guildName, source_id, scene_id, msg);
		}
		break;
		
		case SP_GUILD_RECORD_GUILD_LOG: // ��¼�����־
		{
			int type = msg.IntVal(3);
			std::wstring wstrLogArgs = msg.WideStrVal(4);
			RecordGuildLog(guildName, type, wstrLogArgs.c_str());
		}
		break;
// 		case SP_GUILD_START_IMPEACH:    //��ʼ����
// 		{
// 			OnStartImpeach(pPubKernel, guildName, source_id, scene_id, msg);
// 		}
// 		break;
// 		case SP_GUILD_START_IMPEACH_UNDO:    //��������
// 		{
// 			OnUndoImpeach(pPubKernel, guildName, msg.StringVal(3));
// 		}
// 		break;
// 		case SP_GUILD_START_IMPEACH_RESPOND:    //��Ӧ����
// 		{
// 			OnRespondImpeach(pPubKernel, guildName, source_id, scene_id, msg);
// 		}
// 		break;
		case SP_GUILD_SET_AUTO_AGREE_STATE: //���ð���Զ�����
		{
			int autoAgree = msg.IntVal(3);
			int minLevel = msg.IntVal(4);
			int maxLevel = msg.IntVal(5);
			int ability = msg.IntVal(6);
			IRecord * pGuildSetRc = m_pGuildPubData->GetRecord(GUILD_SET_REC);
			if (pGuildSetRc == NULL){
				return false;
			}
			int guildRow = pGuildSetRc->FindWideStr(GUILD_SET_REC_COL_GUILD_NAME, guildName);
			if (guildRow >= 0)
			{
				pGuildSetRc->SetInt(guildRow, GUILD_SET_REC_COL_AUTO_AGREE, autoAgree ? 1 : 0);
				pGuildSetRc->SetInt(guildRow, GUILD_SET_REC_COL_AUTO_AGREE_MIN_LEVEL, minLevel);
				pGuildSetRc->SetInt(guildRow, GUILD_SET_REC_COL_AUTO_AGREE_MAX_LEVEL, maxLevel);
				pGuildSetRc->SetInt(guildRow, GUILD_SET_REC_COL_AUTO_AGREE_ABILITY, ability);
			}

		}break;
// 		case SP_GUILD_SET_DINE_DANCING_STATE://���ð���輧״̬
// 		{
// 			IRecord* pGuildSetList = m_pGuildPubData->GetRecord(GUILD_SET_REC);
// 			if (pGuildSetList == NULL)
// 			{
// 				return 0;
// 			}
// 			int guildRow = pGuildSetList->FindWideStr(GUILD_LIST_REC_COL_NAME, guildName);
// 			if (guildRow < 0)
// 			{
// 				return 0;
// 			}
// 
// 			pGuildSetList->SetInt(guildRow, GUILD_SET_REC_COL_DANCING_GIRL, 1);
// 
// 		}break;
		

		case SP_GUILD_TIGGER_TIMER:
		{
			PubGuildManager::m_pInstance->UpdateGuildTimer(pPubKernel);
		}break;
// 		case SP_GUILD_SET_DANCING_GIRL_AUTO_OPEN_TIME://������֯�輧��ʱ����ʱ��
// 		{
// 			OnSetDancingGirlAutoOpenTime(pPubKernel, guildName, msg.IntVal(3));
// 
// 		}break;
// 		case SP_GUILD_CHANGE_SHORT_NAME://�޸ļ��
// 		{
// 			OnSetGuildShortName(pPubKernel, guildName,source_id,scene_id,msg);
// 		}break;
// 
// 		case SP_GUILD_ADD_GUID_DEVOTE_MAX:	    //������֯���˰ﹱ���ֵ
// 		{
// 			AddMemberGuildDevoteMax(guildName, msg.WideStrVal(3), msg.IntVal(4));
// 		}break;
		case SP_GUILD_AGREE_INVITE_JOIN_GUILD:
		{
			OnInviteAgreeJoinGuild(pPubKernel, guildName, source_id, scene_id, msg);
		}break;
// 		case SP_GUILD_ADD_PETITION:
// 		{
// 			OnPetition(pPubKernel, guildName, source_id, scene_id, msg);
// 		}break;
// 		case SP_GUILD_UPDATE_TEACH_NUM:
// 		{
// 			OnUpdateTeachNum(pPubKernel, guildName, source_id, scene_id, msg, 3);
// 		}break;
		default:
			break;
	}
	return true;
}

void GuildBaseManager::OnPetition(IPubKernel* pPubKernel, const wchar_t* guildName, int sourceId, int sceneId, const IVarList& args)
{

	int petitionType = args.IntVal(3);
	const  wchar_t* playerName = args.WideStrVal(4);
	if (StringUtil::CharIsNull(playerName) || StringUtil::CharIsNull(guildName)){
		return;
	}

	std::wstring guildDataName = guildName + m_ServerId;
	IPubData *guildData = m_pPubSpace->GetPubData(guildDataName.c_str());
	if (guildData == NULL){
		return;
	}

	IRecord* pGuildPettionRec = guildData->GetRecord(GUILD_PETITION_REC);
	if (pGuildPettionRec == NULL){
		return;
	}
	
	for (int row = 0; row < pGuildPettionRec->GetRows(); ++row)
	{
		int typeRow = pGuildPettionRec->QueryInt(row, GUILD_PETITION_REC_COL_TYPE);
		if (typeRow != petitionType){
			continue;
		}

		const wchar_t* playerNameRow = pGuildPettionRec->QueryWideStr(row, GUILD_PETITION_REC_COL_NAME);
		if (wcscmp(playerName, playerNameRow) == 0){
			return;
		}
	}

	pGuildPettionRec->AddRowValue(-1, CVarList() << playerName << petitionType);
}

int GuildBaseManager::OnCreateGuild(IPubKernel* pPubKernel, const wchar_t* guildName,
	int sourceId, int sceneId, const IVarList& args)
{
	const char* pPlayerUid = args.StringVal(3);
	const wchar_t* playerName = args.WideStrVal(4);
	int sex = args.IntVal(5);
	int level = args.IntVal(6);
	int career = args.IntVal(7);
	int ability = args.IntVal(8);
	int nation = args.IntVal(9);
	int guildIndentifying = args.IntVal(10);
	const wchar_t *guildShortName = args.WideStrVal(11);
	int vip = args.IntVal(12);



	if (!CreateGuild(pPubKernel, guildName, nation, guildIndentifying, guildShortName)){
		SendCreateGuildResult(pPubKernel, sourceId, sceneId, guildName, playerName, GUILD_MSG_REQ_FAIL);
	};


	// ��ӹ���˽�������������ԭ�����ڷ��ش���
	std::wstring guildDataName = guildName + m_ServerId;
	IPubData *guildData = m_pPubSpace->GetPubData(guildDataName.c_str());
	if (guildData == NULL)
	{
		SendCreateGuildResult(pPubKernel, sourceId, sceneId, guildName, playerName, GUILD_MSG_REQ_FAIL);
		return 0;
	}
	IRecord* guildList = m_pGuildPubData->GetRecord(GUILD_LIST_REC);
	if (guildList == NULL){
		return false;
	}
	int row = guildList->FindWideStr(GUILD_LIST_REC_COL_NAME, guildName);
	if (row < 0){
		return false;
	}

	guildList->SetString(row,GUILD_LIST_REC_COL_CAPTAIN_UID, pPlayerUid);
	guildList->SetWideStr(row,GUILD_LIST_REC_COL_CAPTAIN, playerName);
		// �����Ա���������
	CVarList rowValue;
	int64_t curDate = ::time(NULL);
	rowValue << pPlayerUid << playerName << sex << GUILD_POSITION_CAPTAIN << level
		<< career << ability << 0 << curDate << ONLINE << 0<<vip << 0 << curDate << 0 << 0;
	if (guildData->AddRecordRowValue(GUILD_MEMBER_REC, -1, rowValue) < 0)
	{
		m_pPubSpace->RemovePubData(guildDataName.c_str());
		SendCreateGuildResult(pPubKernel, sourceId, sceneId, guildName, playerName, GUILD_MSG_REQ_FAIL);
		return 0;
	}

	// ɾ��֮ǰ�������¼
	DelAllApplyRecord(playerName);

	SendCreateGuildResult(pPubKernel, sourceId, sceneId, guildName, playerName, GUILD_MSG_REQ_SUCC);
	
	// ����µİ�᳡��������Դ����
	
	pPubKernel->SavePubSpace(GetName());

	RefreshGuildFightAbility(pPubKernel, guildName);
	return 0;
}


bool GuildBaseManager::CreateGuild(IPubKernel*pPubKernel, const wchar_t*guildName, int nation, int guildIndentifying, const wchar_t*guildShortName)
{
	IRecord* guildList = m_pGuildPubData->GetRecord(GUILD_LIST_REC);
	if (guildList == NULL){
		return false;
	}
	IRecord *pGuildSymbol = m_pGuildPubData->GetRecord(GUILD_SYSMBOL_REC);
	if (pGuildSymbol == NULL){
		return false;
	}

	int row = pGuildSymbol->FindWideStr(GUILD_SYSMBOL_REC_COL_SHORT_NAME, guildShortName);
	if (row >= 0){
		return false;
	}



	 row = guildList->FindWideStr(GUILD_LIST_REC_COL_NAME, guildName);
	if (row >= 0){
		return false;
	}

	std::wstring guildDataName = guildName + m_ServerId;
	IPubData *guildData = m_pPubSpace->GetPubData(guildDataName.c_str());
	if (guildData != NULL){
		return false;
	}


	if (!m_pPubSpace->AddPubData(guildDataName.c_str(), true)){
		return false;
	}

	guildData = m_pPubSpace->GetPubData(guildDataName.c_str());
	if (guildData == NULL)
	{
		return false;
	}
	

	if (!AddGuildTable( guildData )){
		return false;
	}

	bool ret = PubGuildManager::m_pInstance->CallBackAllSub([guildData](GuildBase*pBase)->bool{
		if (pBase != NULL)
		{
			return pBase->CreateGuildInit(guildData);
		}
		return false;
	});

	if (!ret)
	{
		m_pPubSpace->RemovePubData(guildDataName.c_str());
		return false;
	}
	// �ѹ���д�빫���б���
	if (!GuildBaseManager::m_pInstance->AddGuildToGuildList(guildName, "", L"", 0, nation))
	{
		m_pPubSpace->RemovePubData(guildDataName.c_str());
		return false;
	}


	IRecord *pGuildSetRec = m_pGuildPubData->GetRecord(GUILD_SET_REC);
	if (pGuildSetRec != NULL)
	{
		int row = pGuildSetRec->FindWideStr(GUILD_SET_REC_COL_GUILD_NAME, guildName);
		if (row >= 0)
		{
			pGuildSetRec->RemoveRow(row);
		}
		pGuildSetRec->AddRowValue(-1, CVarList() << guildName << 0 << 0 << 0 << 0 << 0 << 75600 << 0);
	}
	//�����֯��ʶ
	pGuildSymbol->AddRowValue(-1, CVarList() << guildName << guildIndentifying << guildShortName);

	GuildSortHelper_t guildSortHelperNew;
	if (DumpGuildSortHelper(guildList, row, &guildSortHelperNew))
	{
		// ��Ϊ���½����ᣬ��Ȼ�ŵ�����һ
		SortGuild(NULL, &guildSortHelperNew);
	}

	AddGuildStationGroupIDData(guildName);
	return  true;
		
}

bool GuildBaseManager::Patch(IPubData* guildData)
{
	// ����������־����
	PatchRecord( guildData );
	return true;
}

void GuildBaseManager::PatchRecord(IPubData* guildData)
{
	if (NULL == guildData)
	{
		return;
	}

	// ��־��
	IRecord* pLogRecord = guildData->GetRecord(GUILD_LOG_REC);
	if (pLogRecord == NULL)
	{
		return;
	}

	// �鿴���Ƿ���
	int nRows_ = pLogRecord->GetRows();
	int nMax_ = GUILD_LOG_REC_MAX_ROW;

	if (nRows_ > nMax_)
	{
		int nRowCount_ = nRows_ - nMax_;
		for (int i = 0; i < nRowCount_; ++i)
		{
			pLogRecord->RemoveRow(0);
		}
	}

	CVarList cols;
	int nCols = pLogRecord->GetCols();
	for (int i = 0; i < nCols; ++i )
	{
		cols << pLogRecord->GetColType(i);
	}
	change_pub_record(guildData, GUILD_LOG_REC, cols, CVarList(), GUILD_LOG_REC_MAX_ROW);

	AddGuildTable(guildData);
}

// ����������빫�����Ϣ
int GuildBaseManager::OnApplyJoinGuild(IPubKernel* pPubKernel, const wchar_t* guildName, int sourceId, int sceneId, const IVarList& args)
{
	const char* pPlayerNameUid = args.StringVal(3);
	const wchar_t* playerName = args.WideStrVal(4);
	int playerSex = args.IntVal(5);
	int playerLevel = args.IntVal(6);
	int ability = args.IntVal(7);
	int career = args.IntVal(8);
	int vipLevel = args.IntVal(9);
	std::wstring guildDataName = guildName + m_ServerId;
	IPubData* pGuildData = m_pPubSpace->GetPubData(guildDataName.c_str());
	if (pGuildData == NULL)
	{
		SendRequestResult(pPubKernel, sourceId, sceneId, guildName, playerName, PS_GUILD_MSG_APPLY_JOIN, GUILD_MSG_REQ_FAIL);
		return 0;
	}

	// ���빫��������߱���
	if (!AddPlayerToApplyTable(pGuildData, pPlayerNameUid, playerName, playerSex, playerLevel, ability, career,vipLevel))
	{
		SendRequestResult(pPubKernel, sourceId, sceneId, guildName, playerName, PS_GUILD_MSG_APPLY_JOIN, GUILD_MSG_REQ_FAIL);
		return 0;
	}
	//֪ͨ
	SendToGuildMemApplyNew(pPubKernel, pGuildData, sourceId, sceneId);


	// ��¼��ҵ������¼
	if (!AddRecordToApplyListTable(pPlayerNameUid, playerName, guildName))
	{
		SendRequestResult(pPubKernel, sourceId, sceneId, guildName, playerName, PS_GUILD_MSG_APPLY_JOIN, GUILD_MSG_REQ_FAIL);
		return 0;
	}

	// ֪ͨ����ɹ���Ϣ
	SendRequestResult(pPubKernel, sourceId, sceneId, guildName, playerName, PS_GUILD_MSG_APPLY_JOIN, GUILD_MSG_REQ_SUCC);

	AutoAgreeJoinGuild(pPubKernel, guildName, playerName, sourceId, sceneId);
	return 0;
}


int GuildBaseManager::OnOneKeyApplyJoinGuild(IPubKernel* pPubKernel, int sourceId, int sceneId, const IVarList& args)
{
	const char* pPlayerNameUid = args.StringVal(3);
	const wchar_t* playerName = args.WideStrVal(4);
	int playerSex = args.IntVal(5);
	int playerLevel = args.IntVal(6);
	int ability = args.IntVal(7);
	int career = args.IntVal(8);
	int vipLv = args.IntVal(9);
	int count = args.IntVal(10);
	int maxCount = args.GetCount();
	std::wstring guildList;

	LoopBeginCheck(a);
	for (int i = 0; i < count; i++)
	{
		LoopDoCheck(a);
		int index = 10 + i;
		if (index >= maxCount)
		{
			break;
		}
		const wchar_t *guildName = args.WideStrVal(index);
		if (StringUtil::CharIsNull(guildName))
		{
			continue;
		}
		std::wstring guildDataName = guildName + m_ServerId;
		IPubData* pGuildData = m_pPubSpace->GetPubData(guildDataName.c_str());
		if (pGuildData == NULL)
		{
			continue;
		}

		// ���빫��������߱���
		if (!AddPlayerToApplyTable(pGuildData, pPlayerNameUid, playerName, playerSex, playerLevel, ability, career,vipLv))
		{
			continue;
		}

		// ��¼��ҵ������¼
		if (!AddRecordToApplyListTable(pPlayerNameUid, playerName, guildName))
		{
			continue;
		}

		if (AutoAgreeJoinGuild(pPubKernel, guildName, playerName, sourceId, sceneId))
		{
			break;
		}


		guildList = guildList + guildName + L";";
	}
	// ֪ͨ����ɹ���Ϣ
	SendRequestResult(pPubKernel, sourceId, sceneId, guildList.c_str(), playerName, PS_GUILD_MSG_APPLY_JOIN, GUILD_MSG_REQ_SUCC);
	return 0;
}

// ����ȡ��������빫�����Ϣ
int GuildBaseManager::OnCancelApplyJoinGuild(IPubKernel* pPubKernel, const wchar_t* guildName,
	int sourceId, int sceneId, const IVarList& args)
{
	//const char* pPlayerNameUid = args.StringVal(3);
	const wchar_t* playerName = args.WideStrVal(4);

	bool rc = DelGuildApplyRecord(guildName, playerName);
	SendRequestResult(pPubKernel, sourceId, sceneId, guildName, playerName,
		PS_GUILD_MSG_CANCEL_APPLY, (rc ? GUILD_MSG_REQ_SUCC : GUILD_MSG_REQ_FAIL));

	return 0;
}

// ������ܼ��빫���������Ϣ
int GuildBaseManager::OnAcceptJoinGuild(IPubKernel* pPubKernel, const wchar_t* guildName, int sourceId, int sceneId, const IVarList& args)
{
	const char* pApplyNameUid = args.StringVal(3);
	const wchar_t* applyName = args.WideStrVal(4);
	const wchar_t* playerName = args.WideStrVal(5);
	int curLevel = args.IntVal(6);
	int maxMember = args.IntVal(7);
	int applyOnline = args.IntVal(8);

	if (!CanJoinGuild(guildName, maxMember))
	{
		SendAcceptJoinGuildResult(pPubKernel, sourceId, sceneId, guildName,
			playerName, applyName, OFFLINE, GUILD_MSG_REQ_FAIL);
		//CustomSysInfoByName(pPubKernel, playerName, TIPSTYPE_SYSFUNCTION_PROMPT_MESSAGE, GUILD_CUSTOM_STRING_15, CVarList());
		return 0;
	}

	int sex = 0;
	int level = 0;
	int fightAbility = 0;
	int career = 0;
	int vipLv = 0;
	if (!GetApplyInfo(guildName, applyName, sex, career, level, fightAbility,vipLv))
	{
		SendAcceptJoinGuildResult(pPubKernel, sourceId, sceneId, guildName,
			playerName, applyName, applyOnline, GUILD_MSG_REQ_FAIL);
		return 0;
	}

	// ɾ�������������¼
	if (!DelAllApplyRecord(applyName))
	{
		SendAcceptJoinGuildResult(pPubKernel, sourceId, sceneId, guildName,
			playerName, applyName, applyOnline, GUILD_MSG_REQ_FAIL);
		return 0;
	}

	IRecord* pGuildList = m_pGuildPubData->GetRecord(GUILD_LIST_REC);
	if (pGuildList == NULL)
	{
		SendAcceptJoinGuildResult(pPubKernel, sourceId, sceneId, guildName,
			playerName, applyName, applyOnline, GUILD_MSG_REQ_FAIL);
		return 0;
	}
	int guildRow = pGuildList->FindWideStr(GUILD_LIST_REC_COL_NAME, guildName);
	if (guildRow < 0)
	{
		SendAcceptJoinGuildResult(pPubKernel, sourceId, sceneId, guildName,
			playerName, applyName, applyOnline, GUILD_MSG_REQ_FAIL);
		return 0;
	}

	GuildSortHelper_t guildSortHelperOld;
	if (!DumpGuildSortHelper(pGuildList, guildRow, &guildSortHelperOld))
	{
		SendAcceptJoinGuildResult(pPubKernel, sourceId, sceneId, guildName,
			playerName, applyName, applyOnline, GUILD_MSG_REQ_FAIL);
		return 0;
	}

	// �������߼��빫����
	if (!AddApplyJoinGuild(guildName, pApplyNameUid, applyName, sex, career, level, fightAbility, applyOnline,vipLv))
	{
		SendAcceptJoinGuildResult(pPubKernel, sourceId, sceneId, guildName,
			playerName, applyName, applyOnline, GUILD_MSG_REQ_FAIL);
		return 0;
	}
	else
	{
		// ����ɹ�����¼��־
		RecordGuildLog(guildName, GUILD_LOG_TYPE_JOIN, applyName);
	}
	// ��������߲����ߣ���ӵ���񱣴棬�����������������빫��
	if (applyOnline == OFFLINE)
	{
		AddOutlineJoinGuild(guildName, pApplyNameUid, applyName);
	}

	CVarList msg;
	msg << PUBSPACE_GUILD << GUILD_LOGIC << PS_GUILD_MSG_ACCEPT_JOIN
		<< guildName << playerName << applyName << applyOnline << GUILD_MSG_REQ_SUCC;
	// ����»�Ա����
	msg << GUILD_POSITION_MEMBER << sex << level << career << 0 << 0 << fightAbility << 0 << vipLv;
	pPubKernel->SendPublicMessage(sourceId, sceneId, msg);
	RefreshGuildFightAbility(pPubKernel, guildName);
	// ��������
	GuildSortHelper_t guildSortHelperNew;
	if (DumpGuildSortHelper(pGuildList, guildRow, &guildSortHelperNew))
	{
		SortGuild(&guildSortHelperOld, &guildSortHelperNew);
	}

	return 0;
}

int GuildBaseManager::OnInviteAgreeJoinGuild(IPubKernel* pPubKernel, const wchar_t* guildName, int sourceId, int sceneId, const IVarList& args)
{
	int index = 2;
	const char* pApplyNameUid = args.StringVal(++index);
	const wchar_t* applyName = args.WideStrVal(++index);
	int sex = args.IntVal(++index);
	int level = args.IntVal(++index);
	int fightAbility = args.IntVal(++index);
	int career = args.IntVal(++index);
	int vipLv = args.IntVal(++index);
	int maxMember = args.IntVal(++index);
	int applyOnline = true;

	if (!CanJoinGuild(guildName, maxMember))
	{
		CustomSysInfoByName(pPubKernel, applyName, SYSTEM_INFO_ID_17601, CVarList());
		return 0;
	}

	// ɾ�������������¼
	if (!DelAllApplyRecord(applyName))
	{
		return 0;
	}

	IRecord* pGuildList = m_pGuildPubData->GetRecord(GUILD_LIST_REC);
	if (pGuildList == NULL)
	{
		return 0;
	}
	int guildRow = pGuildList->FindWideStr(GUILD_LIST_REC_COL_NAME, guildName);
	if (guildRow < 0)
	{
		return 0;
	}

	GuildSortHelper_t guildSortHelperOld;
	if (!DumpGuildSortHelper(pGuildList, guildRow, &guildSortHelperOld))
	{
		return 0;
	}

	// �������߼��빫����
	if (!AddApplyJoinGuild(guildName, pApplyNameUid, applyName, sex, career, level, fightAbility, applyOnline, vipLv))
	{
		SendAcceptJoinGuildResult(pPubKernel, sourceId, sceneId, guildName,
			applyName, applyName, applyOnline, GUILD_MSG_REQ_FAIL);
		return 0;
	}
	else
	{
		// ����ɹ�����¼��־
		RecordGuildLog(guildName, GUILD_LOG_TYPE_JOIN, applyName);
	}

	// ��������߲����ߣ���ӵ���񱣴棬�����������������빫��
	if (applyOnline == OFFLINE)
	{
		AddOutlineJoinGuild(guildName, pApplyNameUid, applyName);
	}

	CVarList msg;
	msg << PUBSPACE_GUILD << GUILD_LOGIC << PS_GUILD_MSG_ACCEPT_JOIN
		<< guildName << applyName << applyName << applyOnline << GUILD_MSG_REQ_SUCC;
	// ����»�Ա����
	msg << GUILD_POSITION_MEMBER << sex << level << career << 0 << 0 << fightAbility << 0 << vipLv;
	pPubKernel->SendPublicMessage(sourceId, sceneId, msg);
	RefreshGuildFightAbility(pPubKernel, guildName);
	// ��������
	GuildSortHelper_t guildSortHelperNew;
	if (DumpGuildSortHelper(pGuildList, guildRow, &guildSortHelperNew))
	{
		SortGuild(&guildSortHelperOld, &guildSortHelperNew);
	}

	return 0;
}

// ����ܾ����빫���������Ϣ
int GuildBaseManager::OnRefuseJoinGuild(IPubKernel* pPubKernel, const wchar_t* guildName,
	int sourceId, int sceneId, const IVarList& args)
{
	const wchar_t* applyName = args.WideStrVal(3);
	const wchar_t* playerName = args.WideStrVal(4);

	DelGuildApplyRecord(guildName, applyName);
	CVarList msg;
	msg << PUBSPACE_GUILD << GUILD_LOGIC << PS_GUILD_MSG_REFUSE_JOIN
		<< playerName << applyName << guildName << GUILD_MSG_REQ_SUCC;
	pPubKernel->SendPublicMessage(sourceId, sceneId, msg);

	return 0;
}
// �������Ա����״̬�仯����Ϣ
int GuildBaseManager::OnMemberOnlineStateChange(IPubKernel* pPubKernel, const wchar_t* guildName,
	int sourceId, int sceneId, const IVarList& args)
{
	int onlineState = args.IntVal(3);
	const wchar_t* playerName = args.WideStrVal(4);

	int64_t curDate = ::time(NULL);
	if (onlineState == ONLINE)
	{
		SetGuildPubData(guildName, GUILD_LIST_REC, GUILD_LIST_REC_COL_NAME,
			GUILD_LIST_REC_COL_MEMBER_LOGIN_TIME, CVarList() << curDate);
	}
	else
	{
		SetGuildData(guildName, playerName, GUILD_MEMBER_REC,
			GUILD_MEMBER_REC_COL_NAME, GUILD_MEMBER_REC_COL_OUTLINE_DATE, CVarList() << curDate);
	}

	SetGuildData(guildName, playerName, GUILD_MEMBER_REC,
		GUILD_MEMBER_REC_COL_NAME, GUILD_MEMBER_REC_COL_ONLINE, CVarList() << onlineState);

	return 0;
}

// ����ɾ��������Ҽ��빫���¼����һ��
int GuildBaseManager::OnDeleteOutlineJoinRecord(IPubKernel* pPubKernel, const wchar_t* guildName,
	int sourceId, int sceneId, const IVarList& args)
{
	const wchar_t* playerName = args.WideStrVal(3);
	IRecord* pOutlineJoin = m_pGuildPubData->GetRecord(GUILD_OUTLINE_JOIN_REC);
	if (pOutlineJoin == NULL)
	{
		return 0;
	}

	int row = pOutlineJoin->FindWideStr(GUILD_OUTLINE_JOIN_REC_COL_PLAYER_NAME, playerName);
	if (row >= 0)
	{
		pOutlineJoin->RemoveRow(row);
	}

	return 0;
}


// �����޸Ĺ���
int GuildBaseManager::OnChangeNotice(IPubKernel* pPubKernel, const wchar_t* guildName,
	int sourceId, int sceneId, const IVarList& args)
{
	//const char* pPlayerNameUid = args.StringVal(3);
	const wchar_t* playerName = args.WideStrVal(4);
	const wchar_t* text = args.WideStrVal(5);

	int spend = EnvirValue::EnvirQueryInt(ENV_VALUE_CHANGE_ANNOUNCEMENT);
	if (!GuildNumManage::m_pInstance->CanDecGuildNumValue(guildName, GUILD_NUM_TYPE::GUILD_NUM_CAPITAL, spend))
	{
		SendRequestResult(pPubKernel, sourceId, sceneId, guildName, playerName, PS_GUILD_MSG_CHANGE_NOTICE, GUILD_MSG_REQ_FAIL);
		return 0;
	}


	CVarList notice;
	notice << text;
	if (!SetGuildPubData(guildName, GUILD_LIST_REC,
		GUILD_LIST_REC_COL_NAME, GUILD_LIST_REC_COL_ANNOUNCEMENT, notice))
	{
		SendRequestResult(pPubKernel, sourceId, sceneId, guildName, playerName, PS_GUILD_MSG_CHANGE_NOTICE, GUILD_MSG_REQ_FAIL);
		return 0;
	}

	GuildNumManage::m_pInstance->DecGuildNumValue(guildName, GUILD_NUM_TYPE::GUILD_NUM_CAPITAL, spend, GUILD_NUM_CHANGE::GUILD_NUM_CHANGE_CHANGE_GUILD_NOTIFY, playerName);

	CVarList msg;
	msg << PUBSPACE_GUILD << GUILD_LOGIC << PS_GUILD_MSG_CHANGE_NOTICE
		<< guildName << playerName << GUILD_MSG_REQ_SUCC << text;
	pPubKernel->SendPublicMessage(sourceId, sceneId, msg);

	return 0;
}



// �����޸�����
int GuildBaseManager::OnChangeDeclaration(IPubKernel* pPubKernel, const wchar_t* guildName,
	int sourceId, int sceneId, const IVarList& args)
{
	const wchar_t* playerName = args.WideStrVal(4);
	const wchar_t* text = args.WideStrVal(5);
	CVarList decal;
	decal << text;
	if (!SetGuildPubData(guildName, GUILD_LIST_REC,
		GUILD_LIST_REC_COL_NAME, GUILD_LIST_REC_COL_DECLARATION, decal))
	{
		SendRequestResult(pPubKernel, sourceId, sceneId, guildName, playerName, PS_GUILD_CHANGE_DECLARATION, GUILD_MSG_REQ_FAIL);
		return 0;
	}

	CVarList msg;
	msg << PUBSPACE_GUILD << GUILD_LOGIC << PS_GUILD_CHANGE_DECLARATION
		<< guildName << playerName << GUILD_MSG_REQ_SUCC << text;
	pPubKernel->SendPublicMessage(sourceId, sceneId, msg);

	return 0;
}
// �����˳���������
int GuildBaseManager::OnQuit(IPubKernel* pPubKernel, const wchar_t* guildName,
	int sourceId, int sceneId, const IVarList& args)
{
	//const char* pPlayerNameUid = args.StringVal(3);
	const wchar_t* playerName = args.WideStrVal(4);

	IRecord* pGuildList = m_pGuildPubData->GetRecord(GUILD_LIST_REC);
	if (pGuildList == NULL)
	{
		SendQuitResult(pPubKernel, sourceId, sceneId, guildName, playerName, GUILD_MSG_REQ_FAIL, L"");
		return 0;
	}
	int guildRow = pGuildList->FindWideStr(GUILD_LIST_REC_COL_NAME, guildName);
	if (guildRow < 0)
	{
		SendQuitResult(pPubKernel, sourceId, sceneId, guildName, playerName, GUILD_MSG_REQ_FAIL, L"");
		return 0;
	}
	const wchar_t *captainName = pGuildList->QueryWideStr(guildRow, GUILD_LIST_REC_COL_CAPTAIN);
	if (StringUtil::CharIsNull(captainName))
	{
		SendQuitResult(pPubKernel, sourceId, sceneId, guildName, playerName, GUILD_MSG_REQ_FAIL, L"");
		return 0;
	}

	// �������Ƿ��ڹ����Ա�����
	int row = 0;
	IRecord* guildMemberRecord = GetGuildDataOpt(guildName, playerName, GUILD_MEMBER_REC, GUILD_MEMBER_REC_COL_NAME, row);
	if (guildMemberRecord == NULL)
	{
		SendQuitResult(pPubKernel, sourceId, sceneId, guildName, playerName, GUILD_MSG_REQ_FAIL, L"");
		return 0;
	}
	int memberCount = guildMemberRecord->GetRows();

	// ��������
	GuildSortHelper_t guildSortHelperOld;
	if (!DumpGuildSortHelper(pGuildList, guildRow, &guildSortHelperOld))
	{
		SendQuitResult(pPubKernel, sourceId, sceneId, guildName, playerName, GUILD_MSG_REQ_FAIL, L"");
		return 0;
	}

	// ����ǹ���᳤�˳����ᣬ������ֻ��һ����ʱ��ɢ���ᣬ����᳤�ƽ�
	if (wcscmp(playerName, captainName) == 0)
	{
		if (memberCount <= 1)
		{
			int rc = DismissGuild(pPubKernel, guildName, sourceId, sceneId, playerName);
			SendQuitResult(pPubKernel, sourceId, sceneId, guildName, playerName, rc, captainName);
			return 0;
		}
		else
		{
			// �Ұﹱ��ߵĻ�Ա��Ϊ�»᳤
			GuildMemberSortHelperSet sortHelper;
			// ѭ������
			LoopBeginCheck(ba);
			for (int i = 0; i < memberCount; i++)
			{
				LoopDoCheck(ba);
				int position = guildMemberRecord->QueryInt(i, GUILD_MEMBER_REC_COL_POSITION);
				if (position == GUILD_POSITION_CAPTAIN)
				{
					continue;
				}
				const wchar_t *memberName = guildMemberRecord->QueryWideStr(i, GUILD_MEMBER_REC_COL_NAME);
				int contrib = guildMemberRecord->QueryInt(i, GUILD_MEMBER_REC_COL_DEVOTE);
				int rowlevel = guildMemberRecord->QueryInt(i, GUILD_MEMBER_REC_COL_LEVEL);
				int ability = guildMemberRecord->QueryInt(i, GUILD_MEMBER_REC_COL_FIGHT_ABILITY);
				sortHelper.insert(GuildMemberSortHelper_t(memberName, position, contrib, rowlevel, ability));
			}
			GuildMemberSortHelperSet::iterator iter(sortHelper.begin());
			const wchar_t *newCaptain = iter->memberName.c_str();
			if (TransferCaptain(guildName, playerName, newCaptain) == GUILD_MSG_REQ_FAIL)
			{
				SendQuitResult(pPubKernel, sourceId, sceneId, guildName, playerName, GUILD_MSG_REQ_FAIL, L"");
				return 0;
			}
		}
	}

	if (!guildMemberRecord->RemoveRow(row))
	{
		SendQuitResult(pPubKernel, sourceId, sceneId, guildName, playerName, GUILD_MSG_REQ_FAIL, L"");
		return 0;
	}
	else
	{
		// �뿪����ɹ�����¼��־
		RecordGuildLog(guildName, GUILD_LOG_TYPE_LEAVE, playerName);

		// ���ս������Ӧ����˰��
		//GVGBattleGameSingleton::Instance()->OnPlayerExitGuild(pPubKernel, guildName, playerName);
	}

	// �Ƴ���Ը��¼
	RemovePetition(pPubKernel, guildName, playerName);

	captainName = pGuildList->QueryWideStr(guildRow, GUILD_LIST_REC_COL_CAPTAIN);
	SendQuitResult(pPubKernel, sourceId, sceneId, guildName, playerName, GUILD_MSG_REQ_SUCC, captainName);
	RefreshGuildFightAbility(pPubKernel, guildName);
	// ��������
	GuildSortHelper_t guildSortHelperNew;
	if (DumpGuildSortHelper(pGuildList, guildRow, &guildSortHelperNew))
	{
		SortGuild(&guildSortHelperOld, &guildSortHelperNew);
	}
	//OnUndoImpeach(pPubKernel, guildName, pPubKernel->SeekRoleUid(playerName));
	return 0;
}
// �����߳���������
int GuildBaseManager::OnFire(IPubKernel* pPubKernel, const wchar_t* guildName,
	int sourceId, int sceneId, const IVarList& args)
{
	//const char* pPlayerNameUid = args.StringVal(3);
	const wchar_t* playerName = args.WideStrVal(4);
	const char* pMemberNameUid = args.StringVal(5);
	const wchar_t* memberName = args.WideStrVal(6);

	int onLine = OFFLINE;

	CVarList captainName;
	captainName << L"";
	if (!GetGuildPubData(guildName, GUILD_LIST_REC,
		GUILD_LIST_REC_COL_NAME, GUILD_LIST_REC_COL_CAPTAIN, captainName))
	{
		SendFireResult(pPubKernel, sourceId, sceneId, guildName, playerName, GUILD_MSG_REQ_FAIL, onLine, L"");
		return 0;
	}

	// �᳤���ܱ��߳�����
	if (wcscmp(memberName, captainName.WideStrVal(0)) == 0)
	{
		SendFireResult(pPubKernel, sourceId, sceneId, guildName, playerName, GUILD_MSG_REQ_FAIL, onLine, L"");
		return 0;
	}

	// �������Ƿ��ڹ����Ա�����
	int row = 0;
	IRecord* pRecord = GetGuildDataOpt(guildName, memberName, GUILD_MEMBER_REC, GUILD_MEMBER_REC_COL_NAME, row);
	if (pRecord == NULL)
	{
		SendFireResult(pPubKernel, sourceId, sceneId, guildName, playerName, GUILD_MSG_REQ_FAIL, onLine, L"");
		return 0;
	}

	// ��ɾ����Աǰ�Ȼ�ȡ��Ա�Ĳ�����Ϣ
	CVarList onLineState;
	onLineState << int(OFFLINE);
	if (GetGuildData(guildName, memberName, GUILD_MEMBER_REC,
		GUILD_MEMBER_REC_COL_NAME, GUILD_MEMBER_REC_COL_ONLINE, onLineState))
	{
		onLine = onLineState.IntVal(0);
	}

	IRecord* pGuildList = m_pGuildPubData->GetRecord(GUILD_LIST_REC);
	if (pGuildList == NULL)
	{
		SendFireResult(pPubKernel, sourceId, sceneId, guildName, playerName, GUILD_MSG_REQ_FAIL, onLine, L"");
		return 0;
	}
	int guildRow = pGuildList->FindWideStr(GUILD_LIST_REC_COL_NAME, guildName);
	if (guildRow < 0)
	{
		SendFireResult(pPubKernel, sourceId, sceneId, guildName, playerName, GUILD_MSG_REQ_FAIL, onLine, L"");
		return 0;
	}

	GuildSortHelper_t guildSortHelperOld;
	if (!DumpGuildSortHelper(pGuildList, guildRow, &guildSortHelperOld))
	{
		SendFireResult(pPubKernel, sourceId, sceneId, guildName, playerName, GUILD_MSG_REQ_FAIL, onLine, L"");
		return 0;
	}

	// ɾ����Ա
	if (!pRecord->RemoveRow(row))
	{
		SendFireResult(pPubKernel, sourceId, sceneId, guildName, playerName, GUILD_MSG_REQ_FAIL, onLine, L"");
		return 0;
	}
	else
	{
		std::wstring wstrlogArgs = std::wstring(playerName) + L"," + std::wstring(memberName);
		// ���˳ɹ� ��¼��־
		RecordGuildLog(guildName, GUILD_LOG_TYPE_BE_LEAVE, wstrlogArgs.c_str());
		// ���ս������Ӧ����˰��
		//GVGBattleGameSingleton::Instance()->OnPlayerExitGuild(pPubKernel, guildName, playerName);
	}
	// �Ƴ���Ը��¼
	RemovePetition(pPubKernel, guildName, memberName);


	SendFireResult(pPubKernel, sourceId, sceneId, guildName, playerName, GUILD_MSG_REQ_SUCC, onLine, memberName);
	// ���¹�������˴���
	std::wstring guildDataName = guildName + m_ServerId;
	IPubData* pGuildData = m_pPubSpace->GetPubData(guildDataName.c_str());
	if (pGuildData == NULL)
	{
		return 0;
	}

	// �������˴�������
	if (!pGuildData->FindAttr("FireNum"))
	{
		pGuildData->AddAttr("FireNum", VTYPE_INT);
		pGuildData->SetAttrSave("FireNum", true);
	}

	// �������˴���
	if (pGuildData->FindAttr("FireNum"))
	{
		int nFireNum = pGuildData->QueryAttrInt("FireNum");
		pGuildData->SetAttrInt("FireNum", nFireNum + 1);
	}
	RefreshGuildFightAbility(pPubKernel, guildName);
	CheckAndRefreshSortRecord();
	return 0;
}


// �����ɢ��������
int GuildBaseManager::OnDismiss(IPubKernel* pPubKernel, const wchar_t* guildName,
	int sourceId, int sceneId, const IVarList& args)
{
	//const char* pPlayerNameUid = args.StringVal(3);
	const wchar_t* playerName = args.WideStrVal(4);

	CVarList captainName;
	captainName << L"";
	if (!GetGuildPubData(guildName, GUILD_LIST_REC,
		GUILD_LIST_REC_COL_NAME, GUILD_LIST_REC_COL_CAPTAIN, captainName))
	{
		return 0;
	}

	// ֻ�л᳤�ſ��Խ�ɢ����
	if (wcscmp(playerName, captainName.WideStrVal(0)) != 0)
	{
		return 0;
	}

	DismissGuild(pPubKernel, guildName, sourceId, sceneId, playerName);

	return 0;
}
//����
int GuildBaseManager::OnDonate(IPubKernel* pPubKernel, const wchar_t* guildName,
	int sourceId, int sceneId, const IVarList& args)

{
	const wchar_t *playerName = args.WideStrVal(3);
	const char *playerUid = args.StringVal(4);
	int guildCaptial = args.IntVal(5);
	int donateValue = args.IntVal(6);
	int addSelfGuildCurrency = args.IntVal(7);
	// ��������Լ��İﹱ
	std::wstring guildDataName = guildName + m_ServerId;
	IPubData* pGuildData = m_pPubSpace->GetPubData(guildDataName.c_str());
	if (pGuildData == NULL)
	{
		SendGuildDonate(pPubKernel, sourceId, sceneId, guildName, playerName, GUILD_MSG_REQ_FAIL, CVarList() << donateValue << addSelfGuildCurrency);
		return 0;
	}

	// �߻�Ҫ��ȥ���������ֵ�жϣ� ������AddGuildNumValue���Զ�ȡ���ֵ [12/1/2017 lihailuo]
	if (!GuildNumManage::m_pInstance->CanAddGuildNumValue(guildName, GUILD_NUM_TYPE::GUILD_NUM_CAPITAL))
	{

		SendGuildDonate(pPubKernel, sourceId, sceneId, guildName, playerName, GUILD_MSG_REQ_FAIL, CVarList() << donateValue << addSelfGuildCurrency);
		return 0;
	}




	IRecord* guildMemberRecord = pGuildData->GetRecord(GUILD_MEMBER_REC);
	if (guildMemberRecord == NULL)
	{
		SendGuildDonate(pPubKernel, sourceId, sceneId, guildName, playerName, GUILD_MSG_REQ_FAIL, CVarList() << donateValue << addSelfGuildCurrency);
		return 0;
	}
	int memberRow = guildMemberRecord->FindWideStr(GUILD_MEMBER_REC_COL_NAME, playerName);
	if (memberRow < 0)
	{
		SendGuildDonate(pPubKernel, sourceId, sceneId, guildName, playerName, GUILD_MSG_REQ_FAIL, CVarList() << donateValue << addSelfGuildCurrency);
		return 0;
	}

	IRecord* guildList = m_pGuildPubData->GetRecord(GUILD_LIST_REC);
	if (guildList == NULL)
	{
		SendGuildDonate(pPubKernel, sourceId, sceneId, guildName, playerName, GUILD_MSG_REQ_FAIL, CVarList() << donateValue << addSelfGuildCurrency);
		return 0;
	}
	int guildRow = guildList->FindWideStr(GUILD_LIST_REC_COL_NAME, guildName);
	if (guildRow < 0)
	{
		SendGuildDonate(pPubKernel, sourceId, sceneId, guildName, playerName, GUILD_MSG_REQ_FAIL, CVarList() << donateValue << addSelfGuildCurrency);
		return 0;
	}

	GuildSortHelper_t guildSortHelperOld;
	if (!DumpGuildSortHelper(guildList, guildRow, &guildSortHelperOld))
	{
		SendGuildDonate(pPubKernel, sourceId, sceneId, guildName, playerName, GUILD_MSG_REQ_FAIL, CVarList() << donateValue << addSelfGuildCurrency);
		return 0;
	}
	
	GuildNumManage::m_pInstance->AddGuildNumValue(guildName, GUILD_NUM_TYPE::GUILD_NUM_CAPITAL, guildCaptial, GUILD_NUM_CHANGE_DONATE, playerName);

	
	
	int maxDevoteSilver =  guildMemberRecord->QueryInt(memberRow, GUILD_MEMBER_REC_COL_DEVOTE_SILVER);
	maxDevoteSilver += donateValue;
	guildMemberRecord->SetInt(memberRow, GUILD_MEMBER_REC_COL_DEVOTE_SILVER, maxDevoteSilver);

	CVarList msg;
	msg << PUBSPACE_GUILD << GUILD_LOGIC << PS_GUILD_NUM_CHANGE
		<< playerName << GUILD_NUM_TYPE::GUILD_NUM_CAPITAL << GuildNumManage::m_pInstance->GetGuildNumValue(guildName, GUILD_NUM_TYPE::GUILD_NUM_CAPITAL);
	pPubKernel->SendPublicMessage(sourceId, sceneId, msg);

	SendGuildDonate(pPubKernel, sourceId, sceneId, guildName, playerName, GUILD_MSG_REQ_SUCC, CVarList() << donateValue << addSelfGuildCurrency);


	return 0;
}



// ����������
int GuildBaseManager::OnPromotion(IPubKernel* pPubKernel, const wchar_t* guildName, int sourceId, int sceneId, const IVarList& args)
{
	int msgId = args.IntVal(2);
	const wchar_t* selfName = args.WideStrVal(3);
	const wchar_t* memberName = args.WideStrVal(4);
	int selfPosition = args.IntVal(5);
	int oldPosition = args.IntVal(6);
	int newPosition = args.IntVal(7);

	// ���صĽ��
	CVarList rmsg;
	rmsg << PUBSPACE_GUILD << GUILD_LOGIC;
	switch (msgId)
	{
		case SP_GUILD_MSG_PROMOTION:
			rmsg << PS_GUILD_MSG_PROMOTION;
			break;
		case SP_GUILD_MSG_DEMOTION:
			rmsg << PS_GUILD_MSG_DEMOTION;
			break;
		default:
			return 0;
	}
	rmsg << guildName << selfName;

	// ��ȡ�������������
	std::wstring guildPubDataName = std::wstring(GUILD_PUB_DATA) + m_ServerId;
	IPubData* pubData = m_pPubSpace->GetPubData(guildPubDataName.c_str());
	if (NULL == pubData)
	{
		return 0;
	}

	// ��֤�Ƿ���ͬһ���
	std::wstring guildDataName = guildName + m_ServerId;
	IPubData* pGuildData = m_pPubSpace->GetPubData(guildDataName.c_str());
	if (pGuildData == NULL)
	{
		rmsg << GUILD_MSG_REQ_FAIL << memberName << 0;
		pPubKernel->SendPublicMessage(sourceId, sceneId, rmsg);
		return 0;
	}
	IRecord* guildMemberRecord = pGuildData->GetRecord(GUILD_MEMBER_REC);
	if (guildMemberRecord == NULL)
	{
		rmsg << GUILD_MSG_REQ_FAIL << memberName << 0;
		pPubKernel->SendPublicMessage(sourceId, sceneId, rmsg);
		return 0;
	}
	int selfRow = guildMemberRecord->FindWideStr(GUILD_MEMBER_REC_COL_NAME, selfName);
	int memberRow = guildMemberRecord->FindWideStr(GUILD_MEMBER_REC_COL_NAME, memberName);
	if (selfRow < 0 || memberRow < 0)
	{
		rmsg << GUILD_MSG_REQ_FAIL << memberName << 0;
		pPubKernel->SendPublicMessage(sourceId, sceneId, rmsg);
		return 0;
	}

	// �ų�����������֮��ͬ��ʱ����������
	int pubSelfPosition = guildMemberRecord->QueryInt(selfRow, GUILD_MEMBER_REC_COL_POSITION);
	int pubOldPosition = guildMemberRecord->QueryInt(memberRow, GUILD_MEMBER_REC_COL_POSITION);
	if (selfPosition != pubSelfPosition || oldPosition != pubOldPosition)
	{
		rmsg << GUILD_MSG_REQ_FAIL << memberName << 0;
		pPubKernel->SendPublicMessage(sourceId, sceneId, rmsg);
		return 0;
	}

	// �����Ƿ����
	IRecord *guildListRecord = pubData->GetRecord(GUILD_LIST_REC);
	if (guildListRecord == NULL)
	{
		return 0;
	}
	int guildRow = guildListRecord->FindWideStr(GUILD_LIST_REC_COL_NAME, guildName);
	if (guildRow < 0)
	{
		return 0;
	}

	
	
	int xiangFangLevel = 1; //GuildBuildingManage::m_pInstance->GetBuildingLevel(guildName, GUILD_BUILDING_TYPE::BUILD_BUILDING_TYPE_XIANG_FANG);

	if (xiangFangLevel >=(int) m_GuildUpLevelConfig.size())
	{
		return 0;
	}

	// ���ȼ�����
	const GuildUpLevelConfig_t& upLevelConfig = m_GuildUpLevelConfig[xiangFangLevel];

	// �����Ƿ���
	switch (newPosition)
	{
		case GUILD_POSITION_ASSISTANT_CAPTAIN:
		{
			//m_GuildUpLevelConfig
			if (IsPositionFull(guildMemberRecord, newPosition, upLevelConfig.m_DeputyCaptainNum))
			{
				rmsg << GUILD_MSG_REQ_FAIL << memberName << 0;
				pPubKernel->SendPublicMessage(sourceId, sceneId, rmsg);

				// ��ʾ����ʧ��
				CustomSysInfoByName(pPubKernel, selfName, SYSTEM_INFO_ID_17602, CVarList());
				return 0;
			}
		}
		break;
		case GUILD_POSITION_GENERAL:
		{
			if (IsPositionFull(guildMemberRecord, newPosition, upLevelConfig.m_GeneralNum))
			{
				rmsg << GUILD_MSG_REQ_FAIL << memberName << 0;
				pPubKernel->SendPublicMessage(sourceId, sceneId, rmsg);

				// ��ʾ����ʧ��
				CustomSysInfoByName(pPubKernel, selfName, SYSTEM_INFO_ID_17602, CVarList());
				return 0;
			}
		}
		break;
		case GUILD_POSITION_ELDER:
		{
			if (IsPositionFull(guildMemberRecord, newPosition, upLevelConfig.m_Elder))
			{
				rmsg << GUILD_MSG_REQ_FAIL << memberName << 0;
				pPubKernel->SendPublicMessage(sourceId, sceneId, rmsg);

				// ��ʾ����ʧ��
				CustomSysInfoByName(pPubKernel, selfName, SYSTEM_INFO_ID_17602, CVarList());
				return 0;
			}
		}
		break;
		case GUILD_POSITION_MEMBER:
			break;
		default:
			rmsg << GUILD_MSG_REQ_FAIL << memberName << 0;
			pPubKernel->SendPublicMessage(sourceId, sceneId, rmsg);
			return 0;
	}

	guildMemberRecord->SetInt(memberRow, GUILD_MEMBER_REC_COL_POSITION, newPosition);

	// ְλ�䶯 ��¼��־
	std::wstring wstrlogArgs = std::wstring(selfName) + L"," + std::wstring(memberName) + L"," + std::wstring(GUILD_LEVEL) + StringUtil::IntAsWideStr(newPosition);
	if (msgId == SP_GUILD_MSG_PROMOTION)
	{
		RecordGuildLog(guildName, GUILD_LOG_TYPE_PROMOTION_NEW, wstrlogArgs.c_str());
	}
	else if (msgId == SP_GUILD_MSG_DEMOTION)
	{
		RecordGuildLog(guildName, GUILD_LOG_TYPE_DEMOTION_NEW, wstrlogArgs.c_str());
	}

	rmsg << GUILD_MSG_REQ_SUCC << memberName << newPosition;
	pPubKernel->SendPublicMessage(sourceId, sceneId, rmsg);

	return 0;
}

// ���³�Ա����
int GuildBaseManager::OnUpdateProps(IPubKernel* pPubKernel, const wchar_t* guildName,
	int sourceId, int sceneId, const IVarList& args)
{
	const wchar_t* playerName = args.WideStrVal(3);
	int level = args.IntVal(4);
	int ability = args.IntVal(5);
	int vipLevel = args.IntVal(6);

	// ����ǻ᳤����Ҫ���¹����б��л᳤�ȼ�
	CVarList captainName;
	captainName << L"";
	if (!GetGuildPubData(guildName, GUILD_LIST_REC,
		GUILD_LIST_REC_COL_NAME, GUILD_LIST_REC_COL_CAPTAIN, captainName))
	{
		return 0;
	}
	if (wcscmp(playerName, captainName.WideStrVal(0)) == 0)
	{
		SetGuildPubData(guildName, GUILD_LIST_REC, GUILD_LIST_REC_COL_NAME,
			GUILD_LIST_REC_COL_CAPTAIN_LEVEL, CVarList() << level);
	}

	std::wstring guildDataName = guildName + m_ServerId;
	IPubData* pGuildData = m_pPubSpace->GetPubData(guildDataName.c_str());
	if (pGuildData == NULL)
	{
		return 0;
	}

	// ���¹����Ա�б�
	IRecord* pGuildMemberList = pGuildData->GetRecord(GUILD_MEMBER_REC);
	if (pGuildMemberList != NULL)
	{
		int row = pGuildMemberList->FindWideStr(GUILD_MEMBER_REC_COL_NAME, playerName);
		if (row >= 0)
		{

			int levelRow = pGuildMemberList->QueryInt(row, GUILD_MEMBER_REC_COL_LEVEL);
			if (levelRow < level)
			{
				//�ȼ��б䶯���鿴��ǰ�ȼ��ǲ���10��������־
				if (level % 10 == 0)
				{
					SendGuildLog(guildName, GUILD_LOG_MEMBER_EVENT,
						CVarList() << GUILD_LOG_MEMBER_EVENT_PLAYER_LV_UP
						<<playerName<<level);
				}
			}

			pGuildMemberList->SetInt(row, GUILD_MEMBER_REC_COL_LEVEL, level);
			pGuildMemberList->SetInt(row, GUILD_MEMBER_REC_COL_FIGHT_ABILITY, ability);
			pGuildMemberList->SetInt(row, GUILD_MEMBER_REC_COL_VIP_LEVEL, vipLevel);
		}
	}
	RefreshGuildFightAbility(pPubKernel, guildName);
	CheckAndRefreshSortRecord();
	return 0;
}

// ��������������
int GuildBaseManager::OnUpdateApplyProps(IPubKernel* pPubKernel, const wchar_t* guildName,
	int sourceId, int sceneId, const IVarList& args)
{
	const wchar_t* playerName = args.WideStrVal(3);
	const wchar_t* guildNames = args.WideStrVal(4);
	int level = args.IntVal(5);
	int ability = args.IntVal(6);
	int vipLv = args.IntVal(7);

	CVarList guildList;
	::util_split_wstring(guildList, guildNames, L";");

	int guildCount = (int)guildList.GetCount();
	// ѭ������
	LoopBeginCheck(bd);
	for (int i = 0; i < guildCount; ++i)
	{
		LoopDoCheck(bd);
		const wchar_t* changeGuildName = guildList.WideStrVal(i);
		std::wstring guildDataName = changeGuildName + m_ServerId;
		IPubData* pGuildData = m_pPubSpace->GetPubData(guildDataName.c_str());
		if (pGuildData == NULL)
		{
			return 0;
		}

		IRecord* pApplyList = pGuildData->GetRecord(GUILD_JOIN_REC);
		if (pApplyList != NULL)
		{
			int row = pApplyList->FindWideStr(GUILD_JOIN_REC_COL_PLAYER_NAME, playerName);
			if (row >= 0)
			{
				pApplyList->SetInt(row, GUILD_JOIN_REC_COL_LEVEL, level);
				pApplyList->SetInt(row, GUILD_JOIN_REC_COL_FIGHT_ABILITY, ability);
				pApplyList->SetInt(row, GUILD_JOIN_REC_COL_VIP, vipLv);
			}
		}
	}

	return 0;
}

void GuildBaseManager::OnUpdateApplyOnline(IPubKernel* pPubKernel, const wchar_t* guildName, int sourceId, int sceneId, const IVarList& args)
{
	const wchar_t* playerName = args.WideStrVal(3);
	const wchar_t* guildNames = args.WideStrVal(4);
	int nOnline_ = args.IntVal(5);

	CVarList guildList;
	::util_split_wstring(guildList, guildNames, L";");

	int guildCount = (int)guildList.GetCount();
	// ѭ������
	LoopBeginCheck(bd);
	for (int i = 0; i < guildCount; ++i)
	{
		LoopDoCheck(bd);
		const wchar_t* changeGuildName = guildList.WideStrVal(i);
		std::wstring guildDataName = changeGuildName + m_ServerId;
		IPubData* pGuildData = m_pPubSpace->GetPubData(guildDataName.c_str());
		if (pGuildData == NULL)
		{
			continue;
		}

		IRecord* pApplyList = pGuildData->GetRecord(GUILD_JOIN_REC);
		if (pApplyList != NULL)
		{
			int row = pApplyList->FindWideStr(GUILD_JOIN_REC_COL_PLAYER_NAME, playerName);
			if (row >= 0)
			{
				pApplyList->SetInt(row, GUILD_JOIN_REC_COL_ONLINE, nOnline_);
			}
		}
	}
}

//��ɢ����
int GuildBaseManager::DismissGuild(IPubKernel* pPubKernel, const wchar_t* guildName,
	int sourceId, int sceneId, const wchar_t *playerName)
{
	// ɾ�������б��еļ�¼��
	IRecord* pGuildList = m_pGuildPubData->GetRecord(GUILD_LIST_REC);
	if (pGuildList == NULL)
	{
		return GUILD_MSG_REQ_FAIL;
	}
	int guildRow = pGuildList->FindWideStr(GUILD_LIST_REC_COL_NAME, guildName);
	if (guildRow < 0)
	{
		return GUILD_MSG_REQ_FAIL;
	}
	// ����dump��������Ϣ������ɾ�������
	GuildSortHelper_t guildSortHelperOld;
	if (!DumpGuildSortHelper(pGuildList, guildRow, &guildSortHelperOld))
	{
		extend_warning_pub(LOG_ERROR, "[%s][%d] DumpGuildSortHelper failed", __FILE__, __LINE__);
		return 0;
	}

	// ɾ�������߼�¼��������������б�
	if (!DelGuildAllApplyRecord(guildName, true))
	{
		return GUILD_MSG_REQ_FAIL;
	}

	// ɾ����Ա�б�
	std::wstring guildDataName = guildName + m_ServerId;
	IPubData* pGuildData = m_pPubSpace->GetPubData(guildDataName.c_str());
	if (pGuildData == NULL)
	{
		return GUILD_MSG_REQ_FAIL;
	}
	IRecord* pGuildMemberList = pGuildData->GetRecord(GUILD_MEMBER_REC);
	if (pGuildMemberList == NULL)
	{
		return GUILD_MSG_REQ_FAIL;
	}
	int rowCount = pGuildMemberList->GetRows();
	if (rowCount <= 0)
	{
		return GUILD_MSG_REQ_FAIL;
	}

	// ѭ������
	LoopBeginCheck(be);
	for (int row = 0; row < rowCount; ++row)
	{
		LoopDoCheck(be);
		const char* pMemberNameUid = pGuildMemberList->QueryString(row, GUILD_MEMBER_REC_COL_NAME_UID);
		const wchar_t* memberName = pGuildMemberList->QueryWideStr(row, GUILD_MEMBER_REC_COL_NAME);
		int onLine = pGuildMemberList->QueryInt(row, GUILD_MEMBER_REC_COL_ONLINE);
		// ֪ͨ��Ա�����ɢ
		if (sourceId >= 0 && sceneId >= 0)
		{
			SendGuildGone(pPubKernel, sourceId, sceneId, guildName, playerName, onLine, memberName);
		}
	}

	pGuildMemberList->ClearRow();
	pGuildData->RemoveRecord(GUILD_MEMBER_REC);



	IRecord *pGuildSymbol = m_pGuildPubData->GetRecord(GUILD_SYSMBOL_REC);
	if (pGuildSymbol != NULL){
		int row = pGuildSymbol->FindWideStr(GUILD_SYSMBOL_REC_COL_GUILD_NAME, guildName);
		if (row >= 0)
		{
			pGuildSymbol->RemoveRow(row);
		}
	}

	//ɾ��������Ը
	IRecord * target_petition = pGuildData->GetRecord(C_REC_GUILD_PETITION_AUCTION);
	if (target_petition != NULL)
	{
		pGuildData->RemoveRecord(C_REC_GUILD_PETITION_AUCTION);
	}

	// ɾ������˽�����ݿռ�
	m_pPubSpace->RemovePubData(guildDataName.c_str());

	// ɾ�������б��еļ�¼��
	pGuildList->RemoveRow(guildRow);

	// ɾ�������������
	SortGuild(&guildSortHelperOld, NULL);

	if (sourceId >= 0 && sceneId >= 0)
	{
		CVarList msg;
		msg << PUBSPACE_GUILD << GUILD_LOGIC << PS_GUILD_MSG_DISMISS << guildName;
		pPubKernel->SendPublicMessage(sourceId, sceneId, msg);
	}

	IRecord* pGuildSetList = m_pGuildPubData->GetRecord(GUILD_SET_REC);
	if (pGuildSetList != NULL)
	{
		int row = pGuildSetList->FindWideStr(GUILD_SET_REC_COL_GUILD_NAME, guildName);
		if (row >= 0)
		{
			pGuildSetList->RemoveRow(row);
		}
	}

	// ���¹���ս�������а�
	IPubSpace* pPubSpace = pPubKernel->GetPubSpace(PUBSPACE_DOMAIN);
	if (NULL == pPubSpace)
	{
		return GUILD_MSG_REQ_FAIL;
	}
	// �������ս���͹���ȼ����а��е�����
	CVarList update_msg;
	update_msg																												 
		<< GUILD_FIGHT_POWER_RANK_LIST
		<< "guild_name" << guildName;

	DomainRankList::m_pDomainRankList->DeleteEntry(pPubKernel, pPubSpace, update_msg);

	update_msg.Clear();
	update_msg
		<< GUILD_LEVEL_RANK_LIST
		<< "guild_name" << guildName;

	DomainRankList::m_pDomainRankList->DeleteEntry(pPubKernel, pPubSpace, update_msg);


	//��ɢ���ᣬ����������,��Ҫ֪ͨ���ҵ�������
	FsBindLogic::Instance()->SendPublicCommand(pPubKernel, E_PUB_CMD_GUILD_DISMISS, CVarList() << guildName);

	pPubKernel->SendPublicMessage(pPubKernel->GetServerId(), 1,
		CVarList() << PUBSPACE_GUILD << GUILD_LOGIC << PS_GUILD_MSG_DISMISS_PUBLIC << guildName);
	return GUILD_MSG_REQ_SUCC;
}



//����ת��
int GuildBaseManager::OnCaptainTransfer(IPubKernel* pPubKernel, const wchar_t* guildName,
	int sourceId, int sceneId, const IVarList& args)
{
	//const char *playerUid = args.StringVal(3);
	const wchar_t *oldCaptain = args.WideStrVal(4);
	const wchar_t *newCaptain = args.WideStrVal(5);

	if (TransferCaptain(guildName, oldCaptain, newCaptain) <= 0)
	{
		SendCaptainResult(pPubKernel, sourceId, sceneId, oldCaptain, GUILD_MSG_REQ_FAIL, newCaptain, GUILD_POSITION_CAPTAIN, guildName);
	}
	else
	{
		SendCaptainResult(pPubKernel, sourceId, sceneId, oldCaptain, GUILD_MSG_REQ_SUCC, newCaptain, GUILD_POSITION_MEMBER, guildName);
		FsBindLogic::Instance()->SendPublicCommand(pPubKernel, E_PUB_CMD_GUILD_CAPTAIN_CHANGE, CVarList() << guildName << newCaptain);
	}

	return 0;
}



//�ƽ�����
int GuildBaseManager::TransferCaptain(const wchar_t* guildName, const wchar_t *oldCaptain, const wchar_t *newCaptain)
{
	// ������б�
	IRecord* pGuildList = m_pGuildPubData->GetRecord(GUILD_LIST_REC);
	if (pGuildList == NULL)
	{
		return GUILD_MSG_REQ_FAIL;
	}
	int guildRow = pGuildList->FindWideStr(GUILD_LIST_REC_COL_CAPTAIN, oldCaptain);
	if (guildRow < 0)
	{
		return GUILD_MSG_REQ_FAIL;
	}

	// ������Ա�б�
	std::wstring guildDataName = guildName + m_ServerId;
	IPubData* pGuildData = m_pPubSpace->GetPubData(guildDataName.c_str());
	if (pGuildData == NULL)
	{
		return GUILD_MSG_REQ_FAIL;
	}
	IRecord* pGuildMemberList = pGuildData->GetRecord(GUILD_MEMBER_REC);
	if (pGuildMemberList == NULL)
	{
		return GUILD_MSG_REQ_FAIL;
	}
	int oldCaptainRow = pGuildMemberList->FindWideStr(GUILD_MEMBER_REC_COL_NAME, oldCaptain);
	int newCaptainRow = pGuildMemberList->FindWideStr(GUILD_MEMBER_REC_COL_NAME, newCaptain);
	if (oldCaptainRow < 0 || newCaptainRow < 0)
	{
		return GUILD_MSG_REQ_FAIL;
	}

	// ���ȸ��°���б�����
	const char *newCaptainUid = pGuildMemberList->QueryString(newCaptainRow, GUILD_MEMBER_REC_COL_NAME_UID);
	int newCaptainLevel = pGuildMemberList->QueryInt(newCaptainRow, GUILD_MEMBER_REC_COL_LEVEL);
	pGuildList->SetString(guildRow, GUILD_LIST_REC_COL_CAPTAIN_UID, newCaptainUid);
	pGuildList->SetWideStr(guildRow, GUILD_LIST_REC_COL_CAPTAIN, newCaptain);
	pGuildList->SetInt(guildRow, GUILD_LIST_REC_COL_CAPTAIN_LEVEL, newCaptainLevel);

	// ���ð���Աְ��
	pGuildMemberList->SetInt(oldCaptainRow, GUILD_MEMBER_REC_COL_POSITION, GUILD_POSITION_MEMBER);
	pGuildMemberList->SetInt(newCaptainRow, GUILD_MEMBER_REC_COL_POSITION, GUILD_POSITION_CAPTAIN);

	// ְλ�䶯��¼��־
	std::wstring wstrlogArgs = std::wstring(L"@system") + L"," + std::wstring(oldCaptain) + L"," + std::wstring(GUILD_LEVEL) + StringUtil::IntAsWideStr(GUILD_POSITION_MEMBER);
	RecordGuildLog(guildName, GUILD_LOG_TYPE_DEMOTION_NEW, wstrlogArgs.c_str());
	wstrlogArgs.clear();
	wstrlogArgs = std::wstring(L"@system") + L"," + std::wstring(newCaptain) + L"," + std::wstring(GUILD_LEVEL) + StringUtil::IntAsWideStr(GUILD_POSITION_CAPTAIN);
	RecordGuildLog(guildName, GUILD_LOG_TYPE_PROMOTION_NEW, wstrlogArgs.c_str());

	return GUILD_MSG_REQ_SUCC;
}





// ����������Ϣ
int GuildBaseManager::OnSaveGuidChannelInfo(IPubKernel* pPubKernel, const wchar_t* guildName,
	int sourceId, int sceneId, const IVarList& args)
{
	std::wstring guildDataName = guildName + m_ServerId;
	IPubData* pGuildData = m_pPubSpace->GetPubData(guildDataName.c_str());
	if (pGuildData == NULL)
	{
		return 0;
	}

	IRecord *pRec = pGuildData->GetRecord(GUILD_CHAT_HISTORY_REC);
	if (NULL == pRec)
	{
		return 0;
	}
	//�������ֵ
	int nRows = pRec->GetRows();
	if (nRows >= GUILD_CHAT_HISTORY_ROW_MAX)
	{
		pRec->RemoveRow(0);
	}

	const wchar_t *pwName = args.WideStrVal(4);
	const wchar_t *pContent = args.WideStrVal(5);
	const char *resource = args.StringVal(6);
	const int nNationpost = args.IntVal(7);
	const int nVipLevel = args.IntVal(8);
	const int post = args.IntVal(9);
	const int64_t time = args.Int64Val(10);
	const int level = args.IntVal(11);

	CVarList var;
	var << pwName << pContent << guildName << resource << nNationpost << nVipLevel << post << time << level;
	pRec->AddRowValue(-1, var);
	return 1;
}
void GuildBaseManager::OnSetDancingGirlAutoOpenTime(IPubKernel* pPubKernel, const wchar_t*guildName, int time)
{
	// ����б�
	IRecord* pGuildSetList = m_pGuildPubData->GetRecord(GUILD_SET_REC);
	if (pGuildSetList == NULL)
	{
		return;
	}

	int row = pGuildSetList->FindWideStr(GUILD_LIST_REC_COL_NAME, guildName);
	if (row < 0){
		return;
	}

	pGuildSetList->SetInt(row, GUILD_SET_REC_COL_AUTO_OPEN_DANCING, time);


}

void GuildBaseManager::OnSetGuildShortName(IPubKernel* pPubKernel, const wchar_t* guildName, int sourceId, int sceneId, const IVarList& args)
{

	const wchar_t* guildShortName = args.WideStrVal(3);
	const wchar_t* playerName = args.WideStrVal(4);
	int spend = 0;// EnvirValue::EnvirQueryInt(ENV_VALUE_CHANGE_SHORT_NAME);
	bool ret = GuildNumManage::m_pInstance->CanDecGuildNumValue(guildName, GUILD_NUM_TYPE::GUILD_NUM_CAPITAL, spend);

	if (!ret)
	{
		SendChangeGuildShortNameResult(pPubKernel, sourceId, sceneId, guildName, playerName, GUILD_MSG_REQ_FAIL);
		return;
	}

	IRecord *pGuildSymbol = m_pGuildPubData->GetRecord(GUILD_SYSMBOL_REC);
	if (pGuildSymbol == NULL)
	{
		SendChangeGuildShortNameResult(pPubKernel, sourceId, sceneId, guildName, playerName, GUILD_MSG_REQ_FAIL);
		return ;
	}

	int shortNameRow = pGuildSymbol->FindWideStr(GUILD_SYSMBOL_REC_COL_SHORT_NAME, guildShortName);
	if (shortNameRow >= 0)
	{
		SendChangeGuildShortNameResult(pPubKernel, sourceId, sceneId, guildName, playerName, GUILD_MSG_REQ_FAIL);
		return ;
	}
	int row = pGuildSymbol->FindWideStr(GUILD_SYSMBOL_REC_COL_GUILD_NAME, guildName);
	if (row < 0)
	{
		return;
	}
	
	pGuildSymbol->SetWideStr(row, GUILD_SYSMBOL_REC_COL_SHORT_NAME, guildShortName);
	GuildNumManage::m_pInstance->DecGuildNumValue(guildName, GUILD_NUM_TYPE::GUILD_NUM_CAPITAL, spend, GUILD_NUM_CHANGE::GUILD_NUM_CHANGE_CHANGE_SHORT_NAME, playerName);
	SendChangeGuildShortNameResult(pPubKernel, sourceId, sceneId, guildName,playerName, GUILD_MSG_REQ_SUCC);

}



void GuildBaseManager::OnUpdateTeachNum(IPubKernel* pPubKernel, const wchar_t*guildName, int sourceId, int sceneId, const IVarList& args, int nOffset)
{
	// ��ʽ: domain[string] guildName[widestr] msgid[int] uid[string] type[EmTeachType] num[int](�ı��ֵ)
	const char* pszUid = args.StringVal(nOffset++);
	EmTeachType eType = (EmTeachType)args.IntVal(nOffset++);
	int nChanged = args.IntVal(nOffset);
	if ( StringUtil::CharIsNull(pszUid) )
	{
		::extend_warning_pub(LOG_ERROR, "[GuildBaseManager::OnUpdateTeachNum] player uid is null");
		return;
	}

	if (nChanged == 0)
	{
		::extend_warning_pub(LOG_ERROR, "[GuildBaseManager::OnUpdateTeachNum] not changed value( type:%d value:%d)", eType, nChanged);
		return;
	}

	IPubData* pGuildData = FindGuildData(guildName);
	if (pGuildData == NULL)
	{
		::extend_warning_pub(LOG_ERROR, "[GuildBaseManager::OnUpdateTeachNum] not found guild pub data(guildName:%s)",
			NULL == guildName ? "" : StringUtil::WideStrAsString(guildName));
		return;
	}

	IRecord *pRec = pGuildData->GetRecord(GUILD_MEMBER_REC);
	if (NULL == pRec)
	{
		::extend_warning_pub(LOG_ERROR, "[GuildBaseManager::OnUpdateTeachNum] not found guild member record");
		return;
	}

	// ��������
	int nRow = pRec->FindString(GUILD_MEMBER_REC_COL_NAME_UID, pszUid);
	if (nRow == -1)
	{
		::extend_warning_pub(LOG_ERROR, "[GuildBaseManager::OnUpdateTeachNum] not found member(uid:%s guild:%s)");
	}
	else
	{
		int nColIndex = GUILD_MEMBER_REC_COL_TEACH_NUM;
		if (eType == EM_TEACH_RECIVER)
		{
			nColIndex = GUILD_MEMBER_REC_COL_BE_TEACH_NUM;
		}
		int nNum = pRec->QueryInt(nRow, nColIndex);
		nNum += nChanged;
		pRec->SetInt(nRow, nColIndex, nNum);
	}
}

///////////////////////////////////////////////////////////////////////////
// �ظ�������������Ϣ����
///////////////////////////////////////////////////////////////////////////
// ���ʹ�������Ľ��������������
void GuildBaseManager::SendCreateGuildResult(IPubKernel* pPubKernel, int sourceId, int sceneId,
	const wchar_t* guildName, const wchar_t* playerName,
	int result)
{
	CVarList msg;
	msg << PUBSPACE_GUILD << GUILD_LOGIC << PS_GUILD_MSG_CREATE <<
		guildName << playerName << result;
	pPubKernel->SendPublicMessage(sourceId, sceneId, msg);
}
// ���ͽ���������빫��Ľ��������������
void GuildBaseManager::SendAcceptJoinGuildResult(IPubKernel* pPubKernel, int sourceId, int sceneId,
	const wchar_t* guildName, const wchar_t* playerName,
	const wchar_t* applyName, int onLine, int result)
{
	CVarList msg;
	msg << PUBSPACE_GUILD << GUILD_LOGIC << PS_GUILD_MSG_ACCEPT_JOIN
		<< guildName << playerName << applyName << onLine << result;
	pPubKernel->SendPublicMessage(sourceId, sceneId, msg);
}



// ���ʹ�������Ľ��
void GuildBaseManager::SendRequestResult(IPubKernel* pPubKernel, int sourceId, int sceneId,
	const wchar_t* guildName, const wchar_t* playerName,
	int msgId, int result)
{
	CVarList msg;
	msg << PUBSPACE_GUILD << GUILD_LOGIC << msgId << guildName << playerName << result;
	pPubKernel->SendPublicMessage(sourceId, sceneId, msg);
}
// �����˳�����ظ�
void GuildBaseManager::SendQuitResult(IPubKernel* pPubKernel, int sourceId, int sceneId,
	const wchar_t* guildName, const wchar_t* playerName,
	int result, const wchar_t* captainName)
{
	CVarList msg;
	msg << PUBSPACE_GUILD << GUILD_LOGIC << PS_GUILD_MSG_QUIT
		<< guildName << playerName << result << captainName;
	pPubKernel->SendPublicMessage(sourceId, sceneId, msg);
}


// �����߳�����ظ�
void GuildBaseManager::SendFireResult(IPubKernel* pPubKernel, int sourceId, int sceneId,
	const wchar_t* guildName, const wchar_t* playerName,
	int result, int onLine, const wchar_t* memberName)
{
	CVarList msg;
	msg << PUBSPACE_GUILD << GUILD_LOGIC << PS_GUILD_MSG_FIRE
		<< guildName << playerName << result << onLine << memberName;
	pPubKernel->SendPublicMessage(sourceId, sceneId, msg);
}


// ֪ͨ��Ա�����ɢ
void GuildBaseManager::SendGuildGone(IPubKernel* pPubKernel, int sourceId, int sceneId,
	const wchar_t* guildName, const wchar_t* playerName,
	int onLine, const wchar_t* memberName)
{
	CVarList msg;
	msg << PUBSPACE_GUILD << GUILD_LOGIC << PS_GUILD_MSG_GUILD_GONE
		<< guildName << playerName << onLine << memberName;
	pPubKernel->SendPublicMessage(sourceId, sceneId, msg);
}


// ֪ͨ����
void GuildBaseManager::SendGuildDonate(IPubKernel* pPubKernel, int sourceId, int sceneId,
	const wchar_t* guildName, const wchar_t* playerName,
	int result, const IVarList & args)
{
	CVarList msg;
	msg << PUBSPACE_GUILD << GUILD_LOGIC << PS_GUILD_MSG_DONATE
		<< guildName << playerName << result << args;
	pPubKernel->SendPublicMessage(sourceId, sceneId, msg);
}

// ����ת��֪ͨ
void GuildBaseManager::SendCaptainResult(IPubKernel* pPubKernel, int sourceId, int sceneId, const wchar_t* oldCaptain,
	int result, const wchar_t* newCaptain, int oldCaptainPosition, const wchar_t* guildName)

{
	CVarList msg;
	msg << PUBSPACE_GUILD << GUILD_LOGIC << PS_GUILD_MSG_CAPTAIN_TRANSFER
		<< result << oldCaptain << newCaptain << oldCaptainPosition << guildName;
	pPubKernel->SendPublicMessage(sourceId, sceneId, msg);
}




void GuildBaseManager::SendChangeGuildShortNameResult(IPubKernel* pPubKernel, int sourceId, int sceneId, const wchar_t* guildName, const wchar_t* playerName, int result)
{
	CVarList msg;
	msg << PUBSPACE_GUILD << GUILD_LOGIC << PS_GUILD_CHANGE_GUILD_SHORT_NAME
		<< result<<guildName<<playerName;
	pPubKernel->SendPublicMessage(sourceId, sceneId, msg);
}

///////////////////////////////////////////////////////////////////////////
// ���ܺ���
///////////////////////////////////////////////////////////////////////////






// ��ӹ��ᵽ�����б���
bool GuildBaseManager::AddGuildToGuildList(const wchar_t* guildName, const char* playerNameUid, const wchar_t* playerName,
	int playerLevel, int nation)
{
	IRecord* pGuildList = m_pGuildPubData->GetRecord(GUILD_LIST_REC);
	if (pGuildList == NULL)
	{
		return false;
	}

	if (pGuildList->GetRows() >= GUILD_LIST_REC_ROW_MAX)
	{
		return false;
	}

	// �б��в�Ӧ���ڹ����¼
	int row = pGuildList->FindWideStr(GUILD_LIST_REC_COL_NAME, guildName);
	if (row >= 0)
	{
		return false;
	}


	// ���һ�м�¼-������Ϣ
	CVarList rows_value;
	int64_t curDate = ::time(NULL);
	rows_value << guildName << playerNameUid << playerName << playerLevel
		<< L"" << curDate << nation << curDate << L"" << (int)0 ;
	row = pGuildList->AddRowValue(-1, rows_value);
	return (row >= 0);
}




// ��ӹ���˽�����ݱ��
bool GuildBaseManager::AddGuildTable( IPubData* pGuildData)
{
	if (NULL == pGuildData)
	{
		return false;
	}

	// ��ṫ�����ݲ�����Ӱ�������
	std::wstring guildPubDataName = std::wstring(GUILD_PUB_DATA) + m_ServerId;
	if (wcscmp(pGuildData->GetName(), guildPubDataName.c_str()) == 0)
	{
		return false;
	}



	IRecord* pMemRec = pGuildData->GetRecord(GUILD_MEMBER_REC);
	CVarList cols;
	cols << VTYPE_STRING << VTYPE_WIDESTR << VTYPE_INT << VTYPE_INT
		<< VTYPE_INT << VTYPE_INT << VTYPE_INT << VTYPE_INT
		<< VTYPE_INT64 << VTYPE_INT << VTYPE_INT << VTYPE_INT << VTYPE_INT << VTYPE_INT64 << VTYPE_INT << VTYPE_INT;
	// �����Ա��
	if (NULL == pMemRec)
	{

		if (!pGuildData->AddRecord(GUILD_MEMBER_REC, GUILD_MEMBER_REC_ROW_MAX, GUILD_MEMBER_REC_COL_COUNT, cols))
		{
			return false;
		}
		pGuildData->SetRecordSave(GUILD_MEMBER_REC, true);
	}
	else if (check_record_changed(pMemRec, cols, GUILD_MEMBER_REC_ROW_MAX))
	{
		// ���¹����
		change_pub_record(pGuildData, GUILD_MEMBER_REC, cols, CVarList(), GUILD_MEMBER_REC_ROW_MAX, GUILD_MEMBER_REC_COL_NAME_UID);
	}

	// ������뱾���������б�
	cols.Clear();
	cols << VTYPE_STRING << VTYPE_WIDESTR << VTYPE_INT << VTYPE_INT << VTYPE_INT << VTYPE_INT << VTYPE_INT << VTYPE_INT;
	if (!_create_pub_record_safe(pGuildData, GUILD_JOIN_REC, cols, CVarList(), GUILD_JOIN_REC_ROW_MAX) )
	{
		return false;
	}

	// ���������¼��
	cols.Clear();
	cols << VTYPE_WIDESTR << VTYPE_WIDESTR << VTYPE_WIDESTR << VTYPE_STRING
		<< VTYPE_INT << VTYPE_INT << VTYPE_INT << VTYPE_INT64 << VTYPE_INT;

	// У������Ƿ����仯
	int nCols = (int)cols.GetCount();
	if (pGuildData->FindRecord(GUILD_CHAT_HISTORY_REC))
	{
		if (pGuildData->GetRecordCols(GUILD_CHAT_HISTORY_REC) != nCols)
		{
			pGuildData->RemoveRecord(GUILD_CHAT_HISTORY_REC);
			pGuildData->AddRecord(GUILD_CHAT_HISTORY_REC, GUILD_CHAT_HISTORY_ROW_MAX, nCols, cols);
		}
	}
	else
	{
		if (!pGuildData->AddRecord(GUILD_CHAT_HISTORY_REC, GUILD_CHAT_HISTORY_ROW_MAX, nCols, cols))
		{
			return false;
		}
	}

	// ������־��
	if (!pGuildData->FindRecord(GUILD_LOG_REC))
	{
		CVarList cols;
		cols << VTYPE_INT << VTYPE_INT64 << VTYPE_WIDESTR;
		if (!pGuildData->AddRecord(GUILD_LOG_REC, GUILD_LOG_REC_MAX_ROW, GUILD_LOG_COL_TOTAL, cols))
		{
			return false;
		}
		pGuildData->SetRecordSave(GUILD_LOG_REC, true);
	}

	//������
// 	if (!pGuildData->FindRecord(GUILD_IMPEACH_MEMBER_REC))
// 	{
// 		CVarList cols;
// 		cols << VTYPE_STRING << VTYPE_WIDESTR << VTYPE_INT << VTYPE_INT << VTYPE_INT64;
// 		if (!pGuildData->AddRecord(GUILD_IMPEACH_MEMBER_REC, 0, int(cols.GetCount()), cols))
// 		{
// 			return false;
// 		}
// 		pGuildData->SetRecordSave(GUILD_IMPEACH_MEMBER_REC, true);
// 	}

	//������Ը�����ģ���ʱ�� [�������ڣ�����ʱ�����]
	if (!pGuildData->FindRecord(C_REC_GUILD_PETITION_AUCTION))
	{
		CVarList cols;
		cols << VTYPE_WIDESTR ;
		if (!pGuildData->AddRecord(C_REC_GUILD_PETITION_AUCTION, 0, int(cols.GetCount()), cols))
		{
			return false;
		}
		pGuildData->SetRecordSave(C_REC_GUILD_PETITION_AUCTION, false);
	}

	//ͶƱ�ռ��ʱ�乫������������
// 	if (!pGuildData->FindAttr(GUILD_IMPEACH_VOTE_TIME))
// 	{
// 		pGuildData->AddAttr(GUILD_IMPEACH_VOTE_TIME, VTYPE_INT64);
// 		pGuildData->SetAttrSave(GUILD_IMPEACH_VOTE_TIME, true);
// 	}
// 	//�����ɹ�������Ӧ�����ٷ�С��
// 	if (!pGuildData->FindAttr(GUILD_IMPEACH_ANSWER_NUM))
// 	{
// 		pGuildData->AddAttr(GUILD_IMPEACH_ANSWER_NUM, VTYPE_INT);
// 		pGuildData->SetAttrSave(GUILD_IMPEACH_ANSWER_NUM, true);
// 	}
// 	//�ܵ����İ�������ʱ��
// 	if (!pGuildData->FindAttr(GUILD_IMPEACH_OFFLINE_TIME))
// 	{
// 		pGuildData->AddAttr(GUILD_IMPEACH_OFFLINE_TIME, VTYPE_INT64);
// 		pGuildData->SetAttrSave(GUILD_IMPEACH_OFFLINE_TIME, true);
// 	}

	// �������ݰ汾������ 
	if (!pGuildData->FindAttr(GUILD_PUB_DATA_VERSION_ID))
	{
		// ����Ա��ְλֵ����
		UpdateMemberPositionVal(pGuildData);

		pGuildData->AddAttr(GUILD_PUB_DATA_VERSION_ID, VTYPE_INT);
		pGuildData->SetAttrSave(GUILD_PUB_DATA_VERSION_ID, true);
		pGuildData->SetAttrInt(GUILD_PUB_DATA_VERSION_ID, GUILD_DATA_VERSION_ID);
	}
	// �������˴�������
	if (!pGuildData->FindAttr("FireNum"))
	{
		pGuildData->AddAttr("FireNum", VTYPE_INT);
		pGuildData->SetAttrSave("FireNum", true);
	}
	//��Ը	
	if (!pGuildData->FindRecord(GUILD_PETITION_REC))
	{
		CVarList cols;
		cols << VTYPE_WIDESTR << VTYPE_INT;
		if (!pGuildData->AddRecord(GUILD_PETITION_REC, 0, int(cols.GetCount()), cols))
		{
			return false;
		}
		pGuildData->SetRecordSave(GUILD_PETITION_REC, true);
	}

	


	// ���Ӱ�ս������
	//GVGBattleGameSingleton::Instance()->AddGuildGVGData(pGuildData);
	return true;
}






// ��������˵���������б�����
bool GuildBaseManager::AddPlayerToApplyTable(IPubData* pGuildData, const char* pPlayerNameUid, const wchar_t* playerName,
	int playerSex, int playerLevel, int ability, int career, int vipLv)
{
	IRecord* pApplyList = pGuildData->GetRecord(GUILD_JOIN_REC);
	if (pApplyList == NULL)
	{
		return false;
	}

	int row = pApplyList->FindWideStr(GUILD_JOIN_REC_COL_PLAYER_NAME, playerName);
	if (row >= 0)
	{
		// �Ѽ�¼����ҵ����룬�����ظ���¼
		return false;
	}

	// ���������
	if (pApplyList->GetRows() >= GUILD_JOIN_REC_ROW_MAX)
	{
		pApplyList->RemoveRow(0);
	}

	// ���һ�м�¼-������Ϣ
	CVarList rowValue;
	rowValue << pPlayerNameUid << playerName << playerSex << playerLevel << career << ability<<vipLv << ONLINE;
	row = pApplyList->AddRowValue(-1, rowValue);

	return (row >= 0);
}








void GuildBaseManager::SendToGuildMemApplyNew(IPubKernel* pPubKernel, IPubData* pGuildData, int sourceId, int sceneId)
{

	if (pGuildData == NULL || pPubKernel){
		return;
	}

	IRecord* pGuildMemberList = pGuildData->GetRecord(GUILD_MEMBER_REC);
	if (pGuildMemberList == NULL)
	{
		return;
	}
	int rowCount = pGuildMemberList->GetRows();
	if (rowCount <= 0)
	{
		return;
	}

	// ѭ������
	LoopBeginCheck(be);
	for (int row = 0; row < rowCount; ++row)
	{
		LoopDoCheck(be);
		const char* pMemberNameUid = pGuildMemberList->QueryString(row, GUILD_MEMBER_REC_COL_NAME_UID);
		const wchar_t* memberName = pGuildMemberList->QueryWideStr(row, GUILD_MEMBER_REC_COL_NAME);
		int onLine = pGuildMemberList->QueryInt(row, GUILD_MEMBER_REC_COL_ONLINE);
		CVarList msg;
		msg << PUBSPACE_GUILD << GUILD_LOGIC << PS_GUILD_MSG_NEW_APPLY << memberName;
		pPubKernel->SendPublicMessage(sourceId, sceneId, msg);
	}

}







// ���һ����¼���������б���
bool GuildBaseManager::AddRecordToApplyListTable(const char *playerUid, const wchar_t* playerName, const wchar_t* guildName)
{
	IRecord* pApplyList = m_pGuildPubData->GetRecord(GUILD_APPLY_REC);
	if (pApplyList == NULL)
	{
		return false;
	}

	int row = pApplyList->FindWideStr(GUILD_APPLY_REC_COL_NAME, playerName);
	if (row >= 0)
	{
		// �Ѵ��ڸ���ҵ����빫���¼�����º󷵻�
		std::wstring guildNames;
		guildNames = pApplyList->QueryWideStr(row, GUILD_APPLY_REC_COL_GUILD);
		guildNames += L";";
		guildNames += guildName;
		pApplyList->SetWideStr(row, GUILD_APPLY_REC_COL_GUILD, guildNames.c_str());
		return true;
	}

	// ��������ڣ���Ҫ����ӣ����ж������Ƿ�ﵽ����
	if (pApplyList->GetRows() >= GUILD_APPLY_LIST_ROW_MAX)
	{
		// ����ﵽ���ޣ���ɾ�������һ����¼
		const wchar_t* applyName = pApplyList->QueryWideStr(0, GUILD_APPLY_REC_COL_NAME);
		if (applyName == NULL)
		{
			return false;
		}
		if (!DelAllApplyRecord(applyName))
		{
			return false;
		}
	}

	CVarList rowValue;
	rowValue << playerUid << playerName << guildName;
	row = pApplyList->AddRowValue(-1, rowValue);

	return (row >= 0);
}
// ɾ����������б��еļ�¼
bool GuildBaseManager::DelPlayerFromApplyTable(IPubData* pGuildData, const wchar_t* playerName)
{
	IRecord* pApplyList = pGuildData->GetRecord(GUILD_JOIN_REC);
	if (pApplyList == NULL)
	{
		return false;
	}

	int row = pApplyList->FindWideStr(GUILD_JOIN_REC_COL_PLAYER_NAME, playerName);
	if (row >= 0)
	{
		pApplyList->RemoveRow(row);
	}

	return true;
}


// ɾ���������б������빫�����¼
bool GuildBaseManager::DelRecordFromApplyListTable(const wchar_t* playerName, const wchar_t* guildName)
{
	IRecord* pApplyList = m_pGuildPubData->GetRecord(GUILD_APPLY_REC);
	if (pApplyList == NULL)
	{
		return false;
	}

	int row = pApplyList->FindWideStr(GUILD_APPLY_REC_COL_NAME, playerName);
	if (row >= 0)
	{
		std::wstring guildNames;
		CVarList newGuildList;

		guildNames = pApplyList->QueryWideStr(row, GUILD_APPLY_REC_COL_GUILD);
		CVarList guildList;
		::util_split_wstring(guildList, guildNames, L";");
		int guildCount = (int)guildList.GetCount();
		// ѭ������
		LoopBeginCheck(bg);
		for (int i = 0; i < guildCount; ++i)
		{
			LoopDoCheck(bg);
			if (wcscmp(guildName, guildList.WideStrVal(i)) == 0)
			{
				continue;
			}
			newGuildList.AddWideStr(guildList.WideStrVal(i));
		}

		int newGuildCount = (int)newGuildList.GetCount();
		if (newGuildCount <= 0)
		{
			pApplyList->RemoveRow(row);
		}
		else
		{
			std::wstring newGuildNames;
			// ѭ������
			LoopBeginCheck(bh);
			for (int i = 0; i < newGuildCount; ++i)
			{
				LoopDoCheck(bh);
				newGuildNames += newGuildList.WideStrVal(i);
				if (i != newGuildCount - 1)
				{
					newGuildNames += L";";
				}
			}
			pApplyList->SetWideStr(row, GUILD_APPLY_REC_COL_GUILD, newGuildNames.c_str());
		}
	}

	return true;
}
// ����Ƿ񻹿��Լ��빫��
bool GuildBaseManager::CanJoinGuild(const wchar_t* guildName, int maxMember)
{
	IRecord* pGuildList = m_pGuildPubData->GetRecord(GUILD_LIST_REC);
	if (pGuildList == NULL)
	{
		return false;
	}

	int row = pGuildList->FindWideStr(GUILD_LIST_REC_COL_NAME, guildName);
	if (row < 0)
	{
		return false;
	}
	

	std::wstring guildDataName = guildName + m_ServerId;
	IPubData* pGuildData = m_pPubSpace->GetPubData(guildDataName.c_str());
	if (pGuildData == NULL)
	{
		return false;
	}
	IRecord* pGuildMemberList = pGuildData->GetRecord(GUILD_MEMBER_REC);
	if (pGuildMemberList == NULL)
	{
		return false;
	}

	int curMember = pGuildMemberList->GetRows();
	return (curMember < maxMember);
}
	



// ��ȡ�����˵���Ϣ
bool GuildBaseManager::GetApplyInfo(const wchar_t* guildName,
	const wchar_t* applyName, int &sex, int& career, int& level, int& fight, int& vipLv)
{
	std::wstring guildDataName = guildName + m_ServerId;
	IPubData* pGuildData = m_pPubSpace->GetPubData(guildDataName.c_str());
	if (pGuildData == NULL)
	{
		return false;
	}
	IRecord* joinRecord = pGuildData->GetRecord(GUILD_JOIN_REC);
	if (joinRecord == NULL)
	{
		return false;
	}
	int row = joinRecord->FindWideStr(GUILD_JOIN_REC_COL_PLAYER_NAME, applyName);
	if (row < 0)
	{
		return false;
	}

	career = joinRecord->QueryInt(row, GUILD_JOIN_REC_COL_CAREER);
	sex = joinRecord->QueryInt(row, GUILD_MEMBER_REC_COL_SEX);
	level = joinRecord->QueryInt(row, GUILD_JOIN_REC_COL_LEVEL);
	fight = joinRecord->QueryInt(row, GUILD_JOIN_REC_COL_FIGHT_ABILITY);
	vipLv = joinRecord->QueryInt(row, GUILD_JOIN_REC_COL_VIP);

	return true;
}




// ɾ�������ߵ�ȫ�������¼
bool GuildBaseManager::DelAllApplyRecord(const wchar_t* applyName)
{
	IRecord* pApplyList = m_pGuildPubData->GetRecord(GUILD_APPLY_REC);
	if (pApplyList == NULL)
	{
		return false;
	}

	int row = pApplyList->FindWideStr(GUILD_APPLY_REC_COL_NAME, applyName);
	if (row < 0)
	{
		return true;
	}

	std::wstring guildNames = pApplyList->QueryWideStr(row, GUILD_APPLY_REC_COL_GUILD);
	CVarList guildList;
	::util_split_wstring(guildList, guildNames, L";");

	int guildCount = (int)guildList.GetCount();
	// ѭ������
	LoopBeginCheck(bi);
	for (int i = 0; i < guildCount; ++i)
	{
		LoopDoCheck(bi);
		// ɾ��������������б��е������߼�¼
		std::wstring guildDataName = guildList.WideStrVal(i) + m_ServerId;
		IPubData* pGuildData = m_pPubSpace->GetPubData(guildDataName.c_str());
		if (pGuildData == NULL)
		{
			continue;
		}
		IRecord* pGuildApplyList = pGuildData->GetRecord(GUILD_JOIN_REC);
		if (pGuildApplyList == NULL)
		{
			continue;
		}
		int guildRow = pGuildApplyList->FindWideStr(GUILD_JOIN_REC_COL_PLAYER_NAME, applyName);
		if (guildRow < 0)
		{
			continue;
		}

		if (!pGuildApplyList->RemoveRow(guildRow))
		{
			continue;
		}
	}

	// ɾ���������������б��е������߼�¼
	return pApplyList->RemoveRow(row);
}



// ɾ����������һ�������е������¼
bool GuildBaseManager::DelGuildApplyRecord(const wchar_t* guildName, const wchar_t* applyName)
{
	// ɾ�����������߱��ļ�¼
	DelRecordFromApplyListTable(applyName, guildName);

	std::wstring guildDataName = guildName + m_ServerId;
	IPubData* pGuildData = m_pPubSpace->GetPubData(guildDataName.c_str());
	if (pGuildData == NULL)
	{
		// �����ڹ��� ֱ��ȡ���ɹ�
		return true;
	}

	// ɾ�������߱��������ù���ļ�¼
	DelPlayerFromApplyTable(pGuildData, applyName);

	return true;
}


// ɾ��һ������ȫ���������¼
bool GuildBaseManager::DelGuildAllApplyRecord(const wchar_t* guildName, bool needDeleteRecord)
{
	std::wstring guildDataName = guildName + m_ServerId;
	IPubData* pGuildData = m_pPubSpace->GetPubData(guildDataName.c_str());
	if (pGuildData == NULL)
	{
		return false;
	}

	IRecord* pGuildApplyList = pGuildData->GetRecord(GUILD_JOIN_REC);
	if (pGuildApplyList == NULL)
	{
		return false;
	}

	int row = pGuildApplyList->GetRows();
	if (row <= 0)
	{
		// ���û�������ߣ�����Ҫ����
		return true;
	}

	// ѭ������
	LoopBeginCheck(bj);
	for (int i = 0; i < row; ++i)
	{
		LoopDoCheck(bj);
		const wchar_t* applyName = pGuildApplyList->QueryWideStr(i, GUILD_JOIN_REC_COL_PLAYER_NAME);
		if (applyName != NULL)
		{
			DelRecordFromApplyListTable(applyName, guildName);
		}
	}

	pGuildApplyList->ClearRow();

	if (needDeleteRecord)
	{
		pGuildData->RemoveRecord(GUILD_JOIN_REC);
	}

	return true;
}



// ��������߼��빫����
bool GuildBaseManager::AddApplyJoinGuild(const wchar_t* guildName, const char* pApplyNameUid, const wchar_t* applyName,
	int sex, int career, int level, int fight, int onLine,int  vipLv)
{
	std::wstring guildDataName = guildName + m_ServerId;
	IPubData* pGuildData = m_pPubSpace->GetPubData(guildDataName.c_str());
	if (pGuildData == NULL)
	{
		return false;
	}
	IRecord* pGuildMemberList = pGuildData->GetRecord(GUILD_MEMBER_REC);
	if (pGuildMemberList == NULL)
	{
		return false;
	}

	int row = pGuildMemberList->FindWideStr(GUILD_MEMBER_REC_COL_NAME, applyName);
	if (row >= 0)
	{
		return true;
	}

	CVarList rowValue;
	int64_t curDate = ::time(NULL);
	rowValue << pApplyNameUid << applyName << sex << GUILD_POSITION_MEMBER << level << career << fight << 0
		<< curDate << onLine <<0<<vipLv << 0 << curDate << 0 << 0;
	row = pGuildMemberList->AddRowValue(-1, rowValue);

	return (row >= 0);
}
// ������������߼��빫���¼
bool GuildBaseManager::AddOutlineJoinGuild(const wchar_t* guildName, const char* applyUid, const wchar_t* applyName)
{
	IRecord* pOutlineJoin = m_pGuildPubData->GetRecord(GUILD_OUTLINE_JOIN_REC);
	if (pOutlineJoin == NULL)
	{
		return false;
	}

	int row = pOutlineJoin->FindWideStr(GUILD_OUTLINE_JOIN_REC_COL_PLAYER_NAME, applyName);
	if (row >= 0)
	{
		return false;
	}

	if (pOutlineJoin->GetRows() >= GUILD_OUTLINE_JOIN_ROW_MAX)
	{
		return false;
	}

	CVarList rowValue;
	rowValue << applyUid << applyName << guildName << ::time(NULL);
	row = pOutlineJoin->AddRowValue(-1, rowValue);

	return (row >= 0);
}
// ��ȡ�������ӿڼ���Ҽ�¼������
IRecord* GuildBaseManager::GetGuildDataOpt(const wchar_t* guildName, const wchar_t* playerName,
	const char* tableName, int playNameCol, int& row)
{
	std::wstring guildDataName = guildName + m_ServerId;
	IPubData* pGuildData = m_pPubSpace->GetPubData(guildDataName.c_str());
	if (pGuildData == NULL)
	{
		return NULL;
	}
	IRecord* pRecord = pGuildData->GetRecord(tableName);
	if (pRecord == NULL)
	{
		return NULL;
	}

	row = pRecord->FindWideStr(playNameCol, playerName);
	if (row < 0)
	{
		return NULL;
	}

	return pRecord;
}



// ��ȡ�������ӿ�
IRecord* GuildBaseManager::GetGuildDataRecOpt(const wchar_t* guildName, const char* tableName)
{
	std::wstring guildDataName = guildName + m_ServerId;
	IPubData* pGuildData = m_pPubSpace->GetPubData(guildDataName.c_str());
	if (pGuildData == NULL)
	{
		return NULL;
	}
	IRecord* pRecord = pGuildData->GetRecord(tableName);
	if (pRecord == NULL)
	{
		return NULL;
	}

	return pRecord;
}


// �򹫻��˽�����ݱ����д������
bool GuildBaseManager::SetGuildData(const wchar_t* guildName, const wchar_t* playerName,
	const char* tableName, int playNameCol, int col, const IVarList& value)
{
	if (guildName == NULL || tableName == NULL || value.GetCount() != 1)
	{
		return false;
	}

	int row = 0;
	IRecord* pRecord = GetGuildDataOpt(guildName, playerName, tableName, playNameCol, row);
	if (pRecord == NULL)
	{
		return false;
	}

	switch (value.GetType(0))
	{
		case VTYPE_INT:
		{
			pRecord->SetInt(row, col, value.IntVal(0));
		}
		break;
		case VTYPE_INT64:
		{
			pRecord->SetInt64(row, col, value.Int64Val(0));
		}
		break;
		default:
			return false;
	}

	return true;
}


// �ӹ����˽�����ݱ���л�ȡ����
bool GuildBaseManager::GetGuildData(const wchar_t* guildName, const wchar_t* playerName,
	const char* tableName, int playNameCol, int col, CVarList& value)
{
	if (guildName == NULL || tableName == NULL || value.GetCount() != 1)
	{
		return false;
	}

	int row = 0;
	IRecord* pRecord = GetGuildDataOpt(guildName, playerName, tableName, playNameCol, row);
	if (pRecord == NULL)
	{
		return false;
	}

	switch (value.GetType(0))
	{
		case VTYPE_INT:
		{
			value.SetInt(0, pRecord->QueryInt(row, col));
		}
		break;
		case VTYPE_INT64:
		{
			value.SetInt64(0, pRecord->QueryInt64(row, col));
		}
		break;
		default:
			return false;
	}

	return true;
}




// �򹫻�Ĺ������ݱ����д������
bool GuildBaseManager::SetGuildPubData(const wchar_t* guildName, const char* tableName,
	int guildNameCol, int col, const IVarList& value)
{
	if (guildName == NULL || tableName == NULL || value.GetCount() != 1)
	{
		return false;
	}

	IRecord* pRecord = m_pGuildPubData->GetRecord(tableName);
	if (pRecord == NULL)
	{
		return false;
	}

	int row = pRecord->FindWideStr(guildNameCol, guildName);
	if (row < 0)
	{
		return false;
	}

	switch (value.GetType(0))
	{
		case VTYPE_INT:
		{
			pRecord->SetInt(row, col, value.IntVal(0));
		}
		break;
		case VTYPE_INT64:
		{
			pRecord->SetInt64(row, col, value.Int64Val(0));
		}
		break;
		case VTYPE_WIDESTR:
		{
			pRecord->SetWideStr(row, col, value.WideStrVal(0));
		}
		break;
		default:
			return false;
	}

	return true;
}


// �ӹ���Ĺ������ݱ���л�ȡ����
bool GuildBaseManager::GetGuildPubData(const wchar_t* guildName, const char* tableName,
	int guildNameCol, int col, CVarList& value)
{
	if (guildName == NULL || tableName == NULL || value.GetCount() != 1)
	{
		return false;
	}

	IRecord* pRecord = m_pGuildPubData->GetRecord(tableName);
	if (pRecord == NULL)
	{
		return false;
	}

	int row = pRecord->FindWideStr(guildNameCol, guildName);
	if (row < 0)
	{
		return false;
	}

	switch (value.GetType(0))
	{
		case VTYPE_INT:
		{
			value.SetInt(0, pRecord->QueryInt(row, col));
		}
		break;
		case VTYPE_INT64:
		{
			value.SetInt64(0, pRecord->QueryInt64(row, col));
		}
		break;
		case VTYPE_WIDESTR:
		{
			value.SetWideStr(0, pRecord->QueryWideStr(row, col));
		}
		break;
		default:
			return false;
	}

	return true;
}

// ����������������Ӱ
bool GuildBaseManager::DumpGuildSortHelper(IRecord *guildRec, int row, GuildSortHelper_t *helper)
{
	if (guildRec == NULL || helper == NULL || 0 > row)
	{
		return false;
	}

	int rows = guildRec->GetRows();
	if (row > rows)
	{
		return false;
	}
	helper->guildName = guildRec->QueryWideStr(row, GUILD_LIST_REC_COL_NAME);
	if (helper->guildName.empty())
	{
		return false;
	}
	helper->guildLevel = 1;// GuildBuildingManage::m_pInstance->GetBuildingLevel(helper->guildName.c_str(), GUILD_BUILDING_TYPE::BUILD_BUILDING_TYPE_JU_YI_TING);;
	helper->fightAbiliey = guildRec->QueryInt(row, GUILD_LIST_REC_COL_FIGHT_ABILITY);
	std::wstring guildDataName = helper->guildName + m_ServerId;
	IPubData* pGuildData = m_pPubSpace->GetPubData(guildDataName.c_str());
	if (pGuildData == NULL)
	{
		return false;
	}
	IRecord* pGuildMemberList = pGuildData->GetRecord(GUILD_MEMBER_REC);
	if (pGuildMemberList == NULL)
	{
		return false;
	}
	helper->curMember = pGuildMemberList->GetRows();
	helper->randDate = guildRec->QueryInt64(row, GUILD_LIST_REC_COL_RANK_DATE);

	return true;
}


// ��������
void GuildBaseManager::SortGuild(const GuildSortHelper_t *helperOld, const GuildSortHelper_t *helperNew)
{
	IRecord* guildSortRecord = m_pGuildPubData->GetRecord(GUILD_SORT_REC);
	if (guildSortRecord == NULL)
	{
		return;
	}

	if (helperOld != NULL && helperNew != NULL && *helperOld == *helperNew)
	{
		return;
	}
	// ����ɾ��sort record�е��ϼ�¼
	if (helperOld != NULL)
	{
		m_GuildSortHelperSet.erase(*helperOld);
		int row = guildSortRecord->FindWideStr(GUILD_SORT_REC_COL_NAME, helperOld->guildName.c_str());
		if (row >= 0)
		{
			guildSortRecord->RemoveRow(row);
		}
	}

	// Ȼ������¼�¼
	if (helperNew != NULL)
	{
		// ��������
		std::pair<GuildSortHelperSetIter, bool> pairRet = m_GuildSortHelperSet.insert(*helperNew);

		// ����ɹ�
		if (pairRet.second)
		{
			// �ҵ�����λ�õĺ�̽��
			GuildSortHelperSetIter iter = ++pairRet.first;

			int insertRow = -1;
			if (iter != m_GuildSortHelperSet.end())
			{
				// ������Ҫ�����λ��
				const wchar_t *succeedGuildName = iter->guildName.c_str();
				int succeedRow = guildSortRecord->FindWideStr(GUILD_SORT_REC_COL_NAME, succeedGuildName);
				if (succeedRow >= 0)
				{
					insertRow = succeedRow;
				}
			}

			CVarList varList;
			varList << helperNew->guildName;
			guildSortRecord->AddRowValue(insertRow, varList);
		}
	}

	// У�������
	CheckAndRefreshSortRecord();
}


void GuildBaseManager::AddMemberGuildDevoteMax(const wchar_t *guildName, const wchar_t*playerName, int value)
{
	if (StringUtil::CharIsNull(guildName) || StringUtil::CharIsNull(playerName))
	{
		return;
	}
	std::wstring guildDataName = guildName + m_ServerId;
	IPubData* pGuildData = m_pPubSpace->GetPubData(guildDataName.c_str());
	if (pGuildData == NULL){
		return ;
	}

	IRecord* guildMemberRecord = pGuildData->GetRecord(GUILD_MEMBER_REC);
	if (guildMemberRecord == NULL)
	{
		return;
	}

	int row = guildMemberRecord->FindWideStr(GUILD_MEMBER_REC_COL_NAME,playerName);
	if (row < 0){
		return  ;
	}

	int maxDonate = guildMemberRecord->QueryInt(row, GUILD_MEMBER_REC_COL_DEVOTE);
	maxDonate += value;
	guildMemberRecord->SetInt(row, GUILD_MEMBER_REC_COL_DEVOTE,maxDonate);

}


// ְλ�Ƿ�����
bool GuildBaseManager::IsPositionFull(IRecord* memberList, int position, int maxNumber)
{
	if (memberList == NULL)
	{
		return true;
	}
	if (position >= GUILD_POSITION_COUNT)
	{
		return true;
	}
	int posCount = 0;
	int rowCount = memberList->GetRows();
	// ѭ������
	LoopBeginCheck(bk);
	for (int i = 0; i < rowCount; i++)
	{
		LoopDoCheck(bk);
		int memberPosition = memberList->QueryInt(i, GUILD_MEMBER_REC_COL_POSITION);
		if (position == memberPosition)
		{
			posCount++;
			if (posCount >= maxNumber)
			{
				return true;
			}
		}
	}
	return posCount >= maxNumber;
}






//��ӹ��ᾭ��
int GuildBaseManager::OnAddGuildDefendExp(IPubKernel* pPubKernel, const wchar_t* guildName,
	int sourceId, int sceneId, const IVarList& args)
{
	////1���������˹���
	//int rewardGuildExp = args.IntVal(3);

	////2����ȡ�����б�
	//IRecord* guildList = m_pGuildPubData->GetRecord(GUILD_LIST_REC);
	//if (guildList == NULL)
	//{
	//	return 0;
	//}
	//int guildRow = guildList->FindWideStr(GUILD_LIST_REC_COL_NAME, guildName);
	//if (guildRow < 0)
	//{
	//	return 0;
	//}

	//GuildSortHelper_t guildSortHelperOld;
	//if (!DumpGuildSortHelper(guildList, guildRow, &guildSortHelperOld))
	//{
	//	return 0;
	//}

	////4�����¹��ᾭ��ֵ���������Զ���������
	//int oldGuildLevel = guildList->QueryInt(guildRow, GUILD_LIST_REC_COL_LEVEL);
	//int guildLevel = oldGuildLevel;
	//int guildExp = guildList->QueryInt(guildRow, GUILD_LIST_REC_COL_CUR_EXP);
	//int guildCurExp = guildExp + rewardGuildExp;
	////int guildMaxExp = 0;
	//// ѭ������
	//LoopBeginCheck(bc);
	//while (true)
	//{
	//	LoopDoCheck(bc);
	//	if (guildLevel >= (int)m_GuildUpLevelConfig.size() || guildLevel < 0)
	//	{
	//		break;
	//	}
	//	GuildUpLevelConfig_t *upLevelConfig = &m_GuildUpLevelConfig[guildLevel];
	//	if (guildCurExp >= upLevelConfig->m_LevelUpExp)
	//	{
	//		if (guildLevel < (int)m_GuildUpLevelConfig.size() - 1)
	//		{
	//			guildLevel++;
	//			guildCurExp -= upLevelConfig->m_LevelUpExp;
	//		}
	//		else
	//		{
	//			break;
	//		}
	//	}
	//	else
	//	{
	//		break;
	//	}
	//}

	//guildList->SetInt(guildRow, GUILD_LIST_REC_COL_LEVEL, guildLevel);
	//guildList->SetInt(guildRow, GUILD_LIST_REC_COL_CUR_EXP, guildCurExp);

	//// �������� ��¼��־
	//if (guildLevel > oldGuildLevel)
	//{
	//	std::wstring wstrlogArgs = std::wstring(guildName) + L"," + StringUtil::IntAsWideStr(guildLevel);
	//	RecordGuildLog(guildName, GUILD_LOG_TYPE_UP_LEVEL, wstrlogArgs.c_str());

	//	// ֪ͨ����Ա �������
	//	CommandGuildLevelChange(pPubKernel, guildName, guildLevel, true);
	//}

	//// ��������
	//GuildSortHelper_t guildSortHelperNew;
	//if (DumpGuildSortHelper(guildList, guildRow, &guildSortHelperNew))
	//{
	//	SortGuild(&guildSortHelperOld, &guildSortHelperNew);
	//}

	return 0;
}






// ����µİ�᳡��������Դ����
int GuildBaseManager::AddGuildStationGroupIDData(const wchar_t* guildName)
{
	//1���ǿ��ж�
	if (StringUtil::CharIsNull(guildName))
	{
		return 0;
	}
	
	//2����ȡ���᳡������ż�¼��
	IRecord* guildStationList = m_pGuildPubData->GetRecord(GUILD_STATION_GROUP_REC);
	if (guildStationList == NULL)
	{
		return 0;
	}

	//3����ȡ�Ƿ��Ѿ��м�¼
	int row = guildStationList->FindWideStr(GUILD_STATION_GROUP_REC_COL_GUILD_NAME, guildName);
	if (row < 0)
	{
		CVarList var;
		var << guildName << (10 + ++m_stationGroup);  // ����� << �����(��10��ʼ)
		guildStationList->AddRowValue(-1, var);
	}

	return 0;
}


void GuildBaseManager::RefreshGuildFightAbility(IPubKernel * pPubKernel, const wchar_t* guildName)
{
	std::wstring guildDataName = guildName + m_ServerId;
	IPubData* pGuildData = m_pPubSpace->GetPubData(guildDataName.c_str());
	if (pGuildData == NULL)
	{
		return;
	}
	int abilityAll = 0;
	// ���¹����Ա�б�
	IRecord* pGuildMemberList = pGuildData->GetRecord(GUILD_MEMBER_REC);
	if (pGuildMemberList != NULL)
	{

		int rowSize = pGuildMemberList->GetRows();
		LoopBeginCheck(a);
		for (int size = 0; size < rowSize; size++)
		{
			LoopDoCheck(a);
			abilityAll += pGuildMemberList->QueryInt(size, GUILD_MEMBER_REC_COL_FIGHT_ABILITY);
		}
	}

	IRecord* guildList = m_pGuildPubData->GetRecord(GUILD_LIST_REC);
	if (guildList == NULL)
	{
		return;
	}
	int guildRow = guildList->FindWideStr(GUILD_LIST_REC_COL_NAME, guildName);
	if (guildRow < 0)
	{
		return;
	}
	guildList->SetInt(guildRow, GUILD_LIST_REC_COL_FIGHT_ABILITY, abilityAll);

	// ���¹���ս�������а�
	IPubSpace* pPubSpace = pPubKernel->GetPubSpace(PUBSPACE_DOMAIN);
	if (NULL == pPubSpace)
	{
		return;
	}

	
	CVarList update_msg;
				    
	int nBoom = GuildNumManage::m_pInstance->GetGuildNumValue(guildName, GUILD_NUM_FANRONGDU);
	int nLevel = 1;// GuildBuildingManage::m_pInstance->GetBuildingLevel(guildName, GUILD_BUILDING_TYPE::BUILD_BUILDING_TYPE_SHU_YUAN);

	int target_ident = 0;
	const wchar_t* target_alias = L"";
	GuildHelper::GetGuildIdent(guildName, target_ident, target_alias);

	update_msg << RESERVED_NUMBER
		<< GUILD_FIGHT_POWER_RANK_LIST
		<< INSERT_IF_NOT_EXIST
		<< "guild_name" << guildName
		<< "guild_fight_power" << abilityAll
		<< "guild_identifying" << target_ident
		<< "guild_alias" << target_alias;
	
	DomainRankList::m_pDomainRankList->UpdateEntry(pPubKernel, pPubSpace, update_msg, 1);
}

bool GuildBaseManager::AutoAgreeJoinGuild(IPubKernel *pPubKernel, const wchar_t* guildName, const wchar_t* applyName, int sourceId, int sceneId)
{
	IRecord* guildList = m_pGuildPubData->GetRecord(GUILD_LIST_REC);
	if (guildList == NULL)
	{
		return false;
	}
	int guildRow = guildList->FindWideStr(GUILD_LIST_REC_COL_NAME, guildName);
	if (guildRow < 0)
	{
		return false;
	}

	IRecord * pGuildSetRc = m_pGuildPubData->GetRecord(GUILD_SET_REC);
	if (pGuildSetRc == NULL){
		return false;
	}
	int guildSetRow = pGuildSetRc->FindWideStr(GUILD_SET_REC_COL_GUILD_NAME, guildName);
	if (guildSetRow < 0)
	{
		return false;
	}


	int autoAgree = pGuildSetRc->QueryInt(guildSetRow, GUILD_SET_REC_COL_AUTO_AGREE);

	if (autoAgree != GUILD_AUTO_AGREE_ON){
		return false;
	}

	int minLevel = pGuildSetRc->QueryInt(guildSetRow, GUILD_SET_REC_COL_AUTO_AGREE_MIN_LEVEL);
	int maxLevel = pGuildSetRc->QueryInt(guildSetRow, GUILD_SET_REC_COL_AUTO_AGREE_MAX_LEVEL);
	int abilityLimit = pGuildSetRc->QueryInt(guildSetRow, GUILD_SET_REC_COL_AUTO_AGREE_ABILITY);

	int curLevel = 1;// GuildBuildingManage::m_pInstance->GetBuildingLevel(guildName, GUILD_BUILDING_TYPE::BUILD_BUILDING_TYPE_XIANG_FANG);
	GuildUpLevelConfigVector::iterator it = m_GuildUpLevelConfig.begin();
	int maxMember = 0;
	for (; it != m_GuildUpLevelConfig.end(); it++)
	{
		if (it->m_Level == curLevel)
		{
			maxMember = it->m_MaxMember;
		}

	}
	if (!CanJoinGuild(guildName, maxMember))
	{
		return false;
	}

	GuildSortHelper_t guildSortHelperOld;
	if (!DumpGuildSortHelper(guildList, guildRow, &guildSortHelperOld))
	{
		return false;
	}

	std::wstring guildDataName = guildName + m_ServerId;
	IPubData* pGuildData = m_pPubSpace->GetPubData(guildDataName.c_str());
	if (pGuildData == NULL)
	{
		return false;
	}

	

	IRecord* joinRecord = pGuildData->GetRecord(GUILD_JOIN_REC);
	if (joinRecord == NULL)
	{
		return false;
	}

	int row = joinRecord->FindWideStr(GUILD_JOIN_REC_COL_PLAYER_NAME, applyName);
	if (row < 0)
	{
		return false;
	}

	const char* pApplyNameUid = joinRecord->QueryString(row, GUILD_JOIN_REC_COL_PLAYER_NAME_UID);
	int	career = joinRecord->QueryInt(row, GUILD_JOIN_REC_COL_CAREER);
	int sex = joinRecord->QueryInt(row, GUILD_MEMBER_REC_COL_SEX);
	int level = joinRecord->QueryInt(row, GUILD_JOIN_REC_COL_LEVEL);
	int fightAbility = joinRecord->QueryInt(row, GUILD_JOIN_REC_COL_FIGHT_ABILITY);
	int vipLv = joinRecord->QueryInt(row, GUILD_JOIN_REC_COL_VIP);
	if (minLevel <= level &&level <= maxLevel && fightAbility >= abilityLimit)
	{
		int applyOnline = ONLINE;
		// �������߼��빫����
		if (!AddApplyJoinGuild(guildName, pApplyNameUid, applyName, sex, career, level, fightAbility, applyOnline, vipLv))
		{
			return false;
		}
		else
		{
			// ����ɹ�����¼��־
			RecordGuildLog(guildName, GUILD_LOG_TYPE_JOIN, applyName);
		}

		// ɾ�������������¼
		if (!DelAllApplyRecord(applyName))
		{
			return false;
		}

		// ��������߲����ߣ���ӵ���񱣴棬�����������������빫��
		if (applyOnline == OFFLINE)
		{
			AddOutlineJoinGuild(guildName, pApplyNameUid, applyName);
		}

		CVarList msg;
		msg << PUBSPACE_GUILD << GUILD_LOGIC << PS_GUILD_MSG_ACCEPT_JOIN
			<< guildName << applyName << applyName << applyOnline << GUILD_MSG_REQ_SUCC;
		msg << GUILD_POSITION_MEMBER << sex << level << career << 0 << 0 << fightAbility << 0;
		pPubKernel->SendPublicMessage(sourceId, sceneId, msg);
		RefreshGuildFightAbility(pPubKernel, guildName);
		// ��������
		GuildSortHelper_t guildSortHelperNew;
		if (DumpGuildSortHelper(guildList, guildRow, &guildSortHelperNew))
		{
			SortGuild(&guildSortHelperOld, &guildSortHelperNew);
		}
		return true;
	}
	return false;
}



void GuildBaseManager::GetGuildLogWstring(const IVarList& args, std::wstring& logWsting)
{

	for (size_t index = 0; index < args.GetCount(); ++index)
	{
		std::wstring data;
		switch (args.GetType(index))
		{

			case VTYPE_BOOL:
				data = StringUtil::IntAsWideStr(args.BoolVal(index));
				break;
			case VTYPE_INT:
				data = StringUtil::IntAsWideStr(args.IntVal(index));
				break;
			case VTYPE_INT64:
				data = StringUtil::StringAsWideStr(StringUtil::Int64AsString(args.Int64Val(index)).c_str());
				break;
			case VTYPE_FLOAT:
				data = StringUtil::FloatAsWideStr(args.FloatVal(index));
				break;
			case VTYPE_DOUBLE:

				data = StringUtil::StringAsWideStr(StringUtil::DoubleAsString(args.DoubleVal(index)).c_str());
				break;
			case VTYPE_STRING:
				data = StringUtil::StringAsWideStr(args.StringVal(index));
				break;
			case VTYPE_WIDESTR:
				data = args.WideStrVal(index);
				break;

				break;
			default:
				continue;
		}

		logWsting += data + L",";

	}




}

void GuildBaseManager::SendAllGuildLog( int type, const IVarList& args)
{
	std::wstring log;
	GetGuildLogWstring(args,log);

	if (StringUtil::CharIsNull(log.c_str()))
	{
		return;
	}
	IRecord* pGuildListRec = m_pGuildPubData->GetRecord(GUILD_LIST_REC);
	if (pGuildListRec == NULL)
	{
		return ;
	}


	for (int row = 0; row < pGuildListRec->GetRows(); row++)
	{
		const wchar_t*guildName = pGuildListRec->QueryWideStr(row, GUILD_LIST_REC_COL_NAME);
		RecordGuildLog(guildName,type,log.c_str());
	}




}

void GuildBaseManager::SendSameNationGuildLog( int nation, int type, const IVarList& args)
{
	std::wstring log;
	GetGuildLogWstring(args, log);
	if (StringUtil::CharIsNull(log.c_str()))
	{
		return;
	}

	IRecord* pGuildListRec = m_pGuildPubData->GetRecord(GUILD_LIST_REC);
	if (pGuildListRec == NULL)
	{
		return;
	}


	for (int row = 0; row < pGuildListRec->GetRows(); row++)
	{

		const wchar_t*guildName = pGuildListRec->QueryWideStr(row, GUILD_LIST_REC_COL_NAME);
		RecordGuildLog(guildName, type, log.c_str());
	}


}

void GuildBaseManager::SendGuildLog( const wchar_t *guildName, int type, const IVarList& args)
{
	std::wstring log;
	GetGuildLogWstring(args, log);
	if (StringUtil::CharIsNull(log.c_str())){
		return;
	}
	RecordGuildLog(guildName, type, log.c_str());
}

int GuildBaseManager::RecordGuildLog(const wchar_t* guildName, int type, const wchar_t* args)
{
	// ��ṫ���ռ�
	std::wstring guildDataName = guildName + m_ServerId;
	IPubData* pGuildData = m_pPubSpace->GetPubData(guildDataName.c_str());
	if (pGuildData == NULL)
	{
		return 0;
	}

	// ��־��
	IRecord* pLogRecord = pGuildData->GetRecord(GUILD_LOG_REC);
	if (pLogRecord == NULL)
	{
		return 0;
	}

	// �鿴���Ƿ���
	if (pLogRecord->GetRows() >= pLogRecord->GetRowMax())
	{
		// ��������ɾ����һ������
		pLogRecord->RemoveRow(0);
	}

	// ��ȡʱ���
	int64_t time = ::time(NULL);
	// ���������
	pLogRecord->AddRowValue(-1, CVarList() << type << time << args);

	return 0;
}

// bool GuildBaseManager::LoadGuldImpeach(IPubKernel* pPubKernel)
// {
// 
// 	std::string xmlPath = pPubKernel->GetResourcePath();
// 	xmlPath += GUILD_IMPEACH_XML_PATH;
// 
// 	char* pfilebuff = GetFileBuff(xmlPath.c_str());
// 	if (pfilebuff == NULL)
// 	{
// 		extend_warning_pub(LOG_ERROR, "[GuildImpeach::LoadConfig] failed");
// 		return false;
// 	}
// 
// 	try
// 	{
// 		xml_document<> doc;
// 		doc.parse<rapidxml::parse_default>(pfilebuff);
// 		xml_node<>* pNodeRoot = doc.first_node("Object");
// 		if (pNodeRoot == NULL)
// 		{
// 			delete[] pfilebuff;
// 			return false;
// 		}
// 		xml_node<>* pNode = pNodeRoot->first_node("Property");
// 		if (pNode == NULL)
// 		{
// 			delete[] pfilebuff;
// 			return false;
// 		}
// 
// 
// 		m_autoImpeachMaxTime = convert_int(QueryXmlAttr(pNode, "AutoImpeachMaxTimes"), 0);
// 		m_memOffLineTimes = convert_int(QueryXmlAttr(pNode, "MemOffLineTimes"), 0);
// 		return true;
// 	}
// 	catch (parse_error&)
// 	{
// 		delete[] pfilebuff;
// 		return false;
// 	}
// 
// 	return false;
// }
// 
// 
// //��ʼ����
// int GuildBaseManager::OnStartImpeach(IPubKernel* pPubKernel, const wchar_t* guildName, int sourceId, int sceneId, const IVarList& args)
// {
// 	if (guildName == NULL)
// 	{
// 		return 0;
// 	}
// 
// 	IPubSpace * pPubSpace = pPubKernel->GetPubSpace(GetName());
// 	if (pPubSpace == NULL)
// 	{
// 		return 0;
// 	}
// 
// 	std::wstring guildDataName = guildName + m_ServerId;
// 	IPubData *pData = pPubSpace->GetPubData(guildDataName.c_str());
// 	if (pData == NULL)
// 	{
// 		return 0;
// 	}
// 
// 	//������Ա������¼��
// 	IRecord *impeachRec = pData->GetRecord(GUILD_IMPEACH_MEMBER_REC);
// 	if (impeachRec == NULL)
// 	{
// 		return 0;
// 	}
// 
// 	//�������0�ʹ���ǰ���н����еĵ���
// 	if (impeachRec->GetRows() > 0)
// 	{
// 		//֪ͨ������
// 		pPubKernel->CommandByName(args.WideStrVal(4), CVarList() << COMMAND_GUILD_IMPEACH << PS_GUILD_IMPEACH_START_FAIL);
// 		return 0;
// 	}
// 
// 	CVarList rowData;
// 	rowData << args.StringVal(3)
// 		<< args.WideStrVal(4)
// 		<< args.IntVal(5)
// 		<< args.IntVal(6)
// 		<< args.Int64Val(7);
// 	impeachRec->AddRowValue(-1, rowData);
// 
// 	//�����ɹ�������Ӧ�������ٷֱ�С��
// 	pData->SetAttrInt(GUILD_IMPEACH_ANSWER_NUM, args.IntVal(8));
// 	//�ܵ����İ�������ʱ��
// 	pData->SetAttrInt64(GUILD_IMPEACH_OFFLINE_TIME, args.Int64Val(9));
// 	//ͶƱ�ռ��ʱ��
// 	pData->SetAttrInt64(GUILD_IMPEACH_VOTE_TIME, args.Int64Val(10));
// 
// 	RecordGuildLog(guildName, GUILD_LOG_TYPE_IMPEACH_START, args.WideStrVal(4));
// 
// 	StartImpeachTimer(pPubKernel, guildName);
// 
// 	//֪ͨ���а���Ա
// 	IRecord *memberRecord = pData->GetRecord(GUILD_MEMBER_REC);
// 	if (memberRecord == NULL)
// 	{
// 		return 0;
// 	}
// 	int memberRecordLength = memberRecord->GetRows();
// 	LoopBeginCheck(a);
// 	for (int i = 0; i < memberRecordLength; ++i)
// 	{
// 		LoopDoCheck(a);
// 
// 		pPubKernel->CommandByName(memberRecord->QueryWideStr(i, GUILD_MEMBER_REC_COL_NAME), CVarList() << COMMAND_GUILD_IMPEACH << PS_GUILD_IMPEACH_START_SUCCEED);
// 	}
// 
// 	return 0;
// }
// 
// //ע�ᵯ����ɼ�鶨ʱ��
// void GuildBaseManager::StartImpeachTimer(IPubKernel* pPubKernel, const wchar_t* guildName)
// {
// 	IPubSpace * pPubSpace = pPubKernel->GetPubSpace(GetName());
// 	if (pPubSpace == NULL)
// 	{
// 		return;
// 	}
// 
// 	std::wstring guildDataName = guildName + m_ServerId;
// 	IPubData *pData = pPubSpace->GetPubData(guildDataName.c_str());
// 	if (pData == NULL)
// 	{
// 		return;
// 	}
// 
// 	//������Ա������¼��
// 	IRecord *impeachRec = pData->GetRecord(GUILD_IMPEACH_MEMBER_REC);
// 	if (impeachRec == NULL)
// 	{
// 		return;
// 	}
// 	if (impeachRec->GetRows() <= 0)
// 	{
// 		return;
// 	}
// 
// 	if (!pPubKernel->FindTimer(GUILD_IMPEACH_EXAMINE_TIME_NAME, GetName(), guildDataName.c_str()))
// 	{
// 		pPubKernel->RemoveTimer(GUILD_IMPEACH_EXAMINE_TIME_NAME, GetName(), guildDataName.c_str());
// 	}
// 	pPubKernel->AddTimer(GUILD_IMPEACH_EXAMINE_TIME_NAME, OnImpeachExamine, GetName(), guildDataName.c_str(), GUILD_IMPEACH_EXAMINE_TIME, 10000);
// }
// 
// //������ɼ�鶨ʱ���ص�
// int GuildBaseManager::OnImpeachExamine(IPubKernel* pPubKernel, const char* space_name, const wchar_t* data_name, int time)
// {
// 	IPubSpace * pPubSpace = pPubKernel->GetPubSpace(m_pInstance->GetName());
// 	if (pPubSpace == NULL)
// 	{
// 		return 0;
// 	}
// 
// 	IPubData *pData = pPubSpace->GetPubData(data_name);
// 	if (pData == NULL)
// 	{
// 		return 0;
// 	}
// 
// 	//������Ա������¼��
// 	IRecord *impeachRec = pData->GetRecord(GUILD_IMPEACH_MEMBER_REC);
// 	if (impeachRec == NULL)
// 	{
// 		return 0;
// 	}
// 	if (impeachRec->GetRows() <= 0)
// 	{
// 		return 0;
// 	}
// 
// 	//����ʣ��ʱ��
// 	int64_t startTime = 0;
// 	int initiatorRow = impeachRec->FindInt(GUILD_IMPEACH_MEMBER_INITIATOR, GUILD_IMPEACH_INITIATOR);
// 	if (initiatorRow != -1)
// 	{
// 		startTime = impeachRec->QueryInt64(initiatorRow, GUILD_IMPEACH_MEMBER_TIME);
// 	}
// 	//ʣ��ʱ��ת�ɺ���
// 	int64_t remainTime = (pData->QueryAttrInt64(GUILD_IMPEACH_VOTE_TIME) - (::time(NULL) - startTime)) * 1000;
// 	if (remainTime <= 0)
// 	{
// 		//�Ƴ���ʱ��
// 		if (pPubKernel->FindTimer(GUILD_IMPEACH_EXAMINE_TIME_NAME, m_pInstance->GetName(), data_name))
// 		{
// 			pPubKernel->RemoveTimer(GUILD_IMPEACH_EXAMINE_TIME_NAME, m_pInstance->GetName(), data_name);
// 		}
// 		//���㵯��
// 		m_pInstance->SettleImpeach(pPubKernel, data_name);
// 	}
// 	else if (remainTime <= GUILD_IMPEACH_EXAMINE_TIME)
// 	{
// 		//���ļ�ʱ�����ʱ��Ϊʣ��ʱ��
// 		if (pPubKernel->FindTimer(GUILD_IMPEACH_EXAMINE_TIME_NAME, m_pInstance->GetName(), data_name))
// 		{
// 			pPubKernel->RemoveTimer(GUILD_IMPEACH_EXAMINE_TIME_NAME, m_pInstance->GetName(), data_name);
// 		}
// 		pPubKernel->AddTimer(GUILD_IMPEACH_EXAMINE_TIME_NAME, OnImpeachExamine, m_pInstance->GetName(), data_name, int(remainTime), 2);
// 	}
// 
// 	return 0;
// }


void GuildBaseManager::ClearPetition(IPubKernel* pPubKernel, const wchar_t* guildName, int type)
{
	if (StringUtil::CharIsNull(guildName) ){
		return;
	}

	std::wstring guildDataName = guildName + m_ServerId;
	IPubData *guildData = m_pPubSpace->GetPubData(guildDataName.c_str());
	if (guildData == NULL){
		return;
	}

	IRecord* pGuildPettionRec = guildData->GetRecord(GUILD_PETITION_REC);
	if (pGuildPettionRec == NULL){
		return;
	}

	for (int row = pGuildPettionRec->GetRows() - 1; row>=0; --row)
	{
		int typeRow = pGuildPettionRec->QueryInt(row, GUILD_PETITION_REC_COL_TYPE);
		if (typeRow == type){
			continue;
		}
		pGuildPettionRec->RemoveRow(row);
	}

}

void GuildBaseManager::RemovePetition(IPubKernel* pPubKernel, const wchar_t* guildName, const wchar_t* pszName)
{
	if (StringUtil::CharIsNull(guildName)){
		return;
	}

	std::wstring guildDataName = guildName + m_ServerId;
	IPubData *guildData = m_pPubSpace->GetPubData(guildDataName.c_str());
	if (guildData == NULL){
		return;
	}

	IRecord* pGuildPettionRec = guildData->GetRecord(GUILD_PETITION_REC);
	if (pGuildPettionRec == NULL){
		return;
	}

	for (int row = pGuildPettionRec->GetRows() - 1; row >= 0; --row)
	{
		const wchar_t* pszPlayer_ = pGuildPettionRec->QueryWideStr(row, GUILD_PETITION_REC_COL_NAME);
		if ( wcscmp( pszName, pszPlayer_ ) != 0 ){
			continue;
		}
		pGuildPettionRec->RemoveRow(row);
	}
}

IPubData* GuildBaseManager::FindGuildData(const wchar_t* guildName) const
{
	if (StringUtil::CharIsNull(guildName))
	{
		return NULL;
	}

	std::wstring guildDataName = guildName + m_ServerId;
	IPubData* pGuildData = m_pPubSpace->GetPubData(guildDataName.c_str());
	return pGuildData;
}

//���㵯��
// bool GuildBaseManager::SettleImpeach(IPubKernel* pPubKernel, const wchar_t* data_name)
// {
// 	if (data_name == NULL)
// 	{
// 		return false;
// 	}
// 
// 	//�Ƴ���ʱ��
// 	if (pPubKernel->FindTimer(GUILD_IMPEACH_EXAMINE_TIME_NAME, GetName(), data_name))
// 	{
// 		pPubKernel->RemoveTimer(GUILD_IMPEACH_EXAMINE_TIME_NAME, GetName(), data_name);
// 	}
// 
// 	//�����
// 	std::wstring guildName = data_name;
// 	guildName = guildName.substr(0, guildName.length() - m_ServerId.length());
// 
// 	IPubSpace * pPubSpace = pPubKernel->GetPubSpace(GetName());
// 	if (pPubSpace == NULL)
// 	{
// 		return false;
// 	}
// 	IPubData *pData = pPubSpace->GetPubData(data_name);
// 	if (pData == NULL)
// 	{
// 		return false;
// 	}
// 
// 	//������Ա������¼��
// 	IRecord *impeachRec = pData->GetRecord(GUILD_IMPEACH_MEMBER_REC);
// 	if (impeachRec == NULL)
// 	{
// 		return false;
// 	}
// 
// 	//������
// 	int initiatorRow = impeachRec->FindInt(GUILD_IMPEACH_MEMBER_INITIATOR, GUILD_IMPEACH_INITIATOR);
// 	if (initiatorRow == -1)
// 	{
// 		//������,�Ҳ�����������ε���û����
// 		impeachRec->ClearRow();
// 		return false;
// 	}
// 	std::wstring impeachName = impeachRec->QueryWideStr(initiatorRow, GUILD_IMPEACH_MEMBER_NAME);
// 
// 	//����Ա��
// 	IRecord *memberRecord = pData->GetRecord(GUILD_MEMBER_REC);
// 	if (memberRecord == NULL)
// 	{
// 		return false;
// 	}
// 
// 	//ͳ����ЧƱ��
// 	int poll = 0;
// 	int impeachRecLength = impeachRec->GetRows();
// 	int memberRecordLength = memberRecord->GetRows();
// 	LoopBeginCheck(d);
// 	for (int i = 0; i < impeachRecLength; ++i)
// 	{
// 		LoopDoCheck(d);
// 		const char *uid = impeachRec->QueryString(i, GUILD_IMPEACH_MEMBER_UID);
// 		if (StringUtil::CharIsNull(uid))
// 		{
// 			continue;
// 		}
// 		if (memberRecord->FindString(GUILD_MEMBER_REC_COL_NAME_UID, uid) != -1)
// 		{
// 			poll += impeachRec->QueryInt(i, GUILD_IMPEACH_MEMBER_NUM);
// 		}
// 	}
// 
// 	//�������������¼����
// 	impeachRec->ClearRow();
// 
// 	//�ж��Ƿ�����Ʊ��,��1����Ϊ�ѷ��������ѵ�ͶƱ��ȥ
// 	int needPoll = pData->QueryAttrInt(GUILD_IMPEACH_ANSWER_NUM);
// 	if (poll < needPoll)
// 	{
// 		//��¼��������������־
// 		std::wstring wstrlogArgs = impeachName + L"," + StringUtil::IntAsWideStr(GUILD_IMPEACH_RESULT_VOTE_FAIL);
// 		RecordGuildLog(guildName.c_str(), GUILD_LOG_TYPE_IMPEACH_FAIL, impeachName.c_str());
// 		//֪ͨ����������
// 		pPubKernel->SendPublicMessage(0, 0,
// 			CVarList() << PUBSPACE_GUILD << GUILD_LOGIC << PS_GUILD_IMPEACH_RESULT << guildName.c_str() << impeachName.c_str() << GUILD_IMPEACH_RESULT_VOTE_FAIL);
// 		return false;
// 	}
// 
// 	//������Ա���㹻
// 	if (memberRecord->GetRows() < START_IMPEACH_MEMBER_MIN)
// 	{
// 		//��¼��������������־
// 		std::wstring wstrlogArgs = impeachName + L"," + StringUtil::IntAsWideStr(GUILD_IMPEACH_RESULT_MEMBER_FAIL);
// 		RecordGuildLog(guildName.c_str(), GUILD_LOG_TYPE_IMPEACH_FAIL, impeachName.c_str());
// 		//֪ͨ����������
// 		pPubKernel->SendPublicMessage(0, 0,
// 			CVarList() << PUBSPACE_GUILD << GUILD_LOGIC << PS_GUILD_IMPEACH_RESULT << guildName.c_str() << impeachName.c_str() << GUILD_IMPEACH_RESULT_MEMBER_FAIL);
// 		return false;
// 	}
// 
// 	//����������
// 	int row = memberRecord->FindInt(GUILD_MEMBER_REC_COL_POSITION, GUILD_POSITION_CAPTAIN);
// 	if (row == -1)
// 	{
// 		return false;
// 	}
// 	const wchar_t *captainName = memberRecord->QueryWideStr(row, GUILD_MEMBER_REC_COL_NAME);
// 	if (captainName == NULL)
// 	{
// 		return false;
// 	}
// 
// 	//�ƽ�����
// 	if (TransferCaptain(guildName.c_str(), captainName, impeachName.c_str()) != GUILD_MSG_REQ_SUCC)
// 	{
// 		//��¼��������������־
// 		std::wstring wstrlogArgs = impeachName + L"," + StringUtil::IntAsWideStr(GUILD_IMPEACH_RESULT_FAIL);
// 		RecordGuildLog(guildName.c_str(), GUILD_LOG_TYPE_IMPEACH_FAIL, impeachName.c_str());
// 		//֪ͨ����������
// 		pPubKernel->SendPublicMessage(0, 0,
// 			CVarList() << PUBSPACE_GUILD << GUILD_LOGIC << PS_GUILD_IMPEACH_RESULT << guildName.c_str() << impeachName.c_str() << GUILD_IMPEACH_RESULT_FAIL);
// 		return false;
// 	}
// 
// 	//��¼��������������־
// 	std::wstring wstrlogArgs = impeachName + L"," + StringUtil::IntAsWideStr(GUILD_IMPEACH_RESULT_SUCCEED);
// 	RecordGuildLog(guildName.c_str(), GUILD_LOG_TYPE_IMPEACH_SUCCEED, impeachName.c_str());
// 
// 	//����ת��֪ͨ
// 	SendCaptainResult(pPubKernel, 0, 0, captainName, GUILD_MSG_REQ_SUCC, impeachName.c_str(), GUILD_POSITION_MEMBER, guildName.c_str());
// 
// 	//֪ͨ�������������ʼ�
// 	pPubKernel->SendPublicMessage(0, 0,
// 		CVarList() << PUBSPACE_GUILD << GUILD_LOGIC << PS_GUILD_IMPEACH_RESULT << guildName.c_str() << impeachName.c_str() << GUILD_IMPEACH_RESULT_SUCCEED);
// 
// 	return true;
// }

//����ͨ����ֵ����Ϊ�¶����ֵ
bool GuildBaseManager::UpdateMemberPositionVal(IPubData* pPubGuildData)
{
	if (NULL == pPubGuildData)
	{
		return false;
	}
	IRecord* pGuildMemberList = pPubGuildData->GetRecord(GUILD_MEMBER_REC);
	if (pGuildMemberList == NULL)
	{
		return false;
	}

	int curMember = pGuildMemberList->GetRows();
	LoopBeginCheck(a);
	for (int i = 0; i < curMember; ++i)
	{
		LoopDoCheck(a);
		int nPosition = pGuildMemberList->QueryInt(i, GUILD_MEMBER_REC_COL_POSITION);
		if (GUILD_POSITION_ELDER == nPosition)
		{
			pGuildMemberList->SetInt(i, GUILD_MEMBER_REC_COL_POSITION, GUILD_POSITION_MEMBER);
		}
	}
	return true;
}



//��������
// int GuildBaseManager::OnUndoImpeach(IPubKernel* pPubKernel, const wchar_t* guildName, const char *pSelfUid)
// {
// 	if (guildName == NULL)
// 	{
// 		return 0;
// 	}
// 
// 	IPubSpace * pPubSpace = pPubKernel->GetPubSpace(GetName());
// 	if (pPubSpace == NULL)
// 	{
// 		return 0;
// 	}
// 
// 	std::wstring guildDataName = guildName + m_ServerId;
// 	IPubData *pData = pPubSpace->GetPubData(guildDataName.c_str());
// 	if (pData == NULL)
// 	{
// 		return 0;
// 	}
// 
// 	//������Ա������¼��
// 	IRecord *impeachRec = pData->GetRecord(GUILD_IMPEACH_MEMBER_REC);
// 	if (impeachRec == NULL)
// 	{
// 		return 0;
// 	}
// 
// 	//������
// 	if (StringUtil::CharIsNull(pSelfUid))
// 	{
// 		return 0;
// 	}
// 
// 	//��ǰ������
// 	int initiatorRow = impeachRec->FindInt(GUILD_IMPEACH_MEMBER_INITIATOR, GUILD_IMPEACH_INITIATOR);
// 	if (initiatorRow == -1)
// 	{
// 		//������,�Ҳ�����������ε���û����
// 		return 0;
// 	}
// 	const char *impeachPlayerUid = impeachRec->QueryString(initiatorRow, GUILD_IMPEACH_MEMBER_UID);
// 
// 	//�ж��Ƿ��Ƿ�����
// 	if (strcmp(pSelfUid, impeachPlayerUid) != 0)
// 	{
// 		return 0;
// 	}
// 
// 	//�Ƴ���ʱ��
// 	if (!pPubKernel->FindTimer(GUILD_IMPEACH_EXAMINE_TIME_NAME, GetName(), guildName))
// 	{
// 		pPubKernel->RemoveTimer(GUILD_IMPEACH_EXAMINE_TIME_NAME, GetName(), guildName);
// 	}
// 
// 	impeachRec->ClearRow();
// 
// 	return 0;
// }
// 
// //��Ӧ����
// int GuildBaseManager::OnRespondImpeach(IPubKernel* pPubKernel, const wchar_t* guildName, int sourceId, int sceneId, const IVarList& args)
// {
// 	if (guildName == NULL)
// 	{
// 		return 0;
// 	}
// 
// 	const char* pSelfUid = args.StringVal(3);
// 	if (StringUtil::CharIsNull(pSelfUid))
// 	{
// 		return 0;
// 	}
// 
// 	const wchar_t *pSelfName = args.WideStrVal(4);
// 	if (StringUtil::CharIsNull(pSelfName))
// 	{
// 		return 0;
// 	}
// 
// 	IPubSpace * pPubSpace = pPubKernel->GetPubSpace(GetName());
// 	if (pPubSpace == NULL)
// 	{
// 		return 0;
// 	}
// 
// 	std::wstring guildDataName = guildName + m_ServerId;
// 	IPubData *pData = pPubSpace->GetPubData(guildDataName.c_str());
// 	if (pData == NULL)
// 	{
// 		return 0;
// 	}
// 
// 	//������Ա������¼��
// 	IRecord *impeachRec = pData->GetRecord(GUILD_IMPEACH_MEMBER_REC);
// 	if (impeachRec == NULL)
// 	{
// 		return 0;
// 	}
// 
// 	if (impeachRec->GetRows() < 1)
// 	{
// 		return 0;
// 	}
// 
// 	int row = impeachRec->FindString(GUILD_IMPEACH_MEMBER_UID, pSelfUid);
// 	if (row == -1)
// 	{
// 		CVarList rowData;
// 		rowData << pSelfUid << pSelfName << GUILD_IMPEACH_RESPOND << 1 << ::time(NULL);
// 		impeachRec->AddRowValue(-1, rowData);
// 	}
// 	else
// 	{
// 		int curPollNum = impeachRec->QueryInt(row, GUILD_IMPEACH_MEMBER_NUM);
// 		impeachRec->SetInt(row, GUILD_IMPEACH_MEMBER_NUM, curPollNum + 1);
// 	}
// 
// 	//����Ա��
// 	IRecord *memberRecord = pData->GetRecord(GUILD_MEMBER_REC);
// 	if (memberRecord == NULL)
// 	{
// 		return false;
// 	}
// 
// 	//ͳ����ЧƱ��
// 	int poll = 0;
// 	int impeachRecLength = impeachRec->GetRows();
// 	LoopBeginCheck(d);
// 	for (int i = 0; i < impeachRecLength; ++i)
// 	{
// 		LoopDoCheck(d);
// 		const char *uid = impeachRec->QueryString(i, GUILD_IMPEACH_MEMBER_UID);
// 		if (StringUtil::CharIsNull(uid))
// 		{
// 			continue;
// 		}
// 		if (memberRecord->FindString(GUILD_MEMBER_REC_COL_NAME_UID, uid) != -1)
// 		{
// 			poll += impeachRec->QueryInt(i, GUILD_IMPEACH_MEMBER_NUM);
// 		}
// 	}
// 
// 	//�ж��Ƿ�����Ʊ��
// 	int needPoll = pData->QueryAttrInt(GUILD_IMPEACH_ANSWER_NUM);
// 	if (poll >= needPoll)
// 	{
// 		//���㵯��
// 		m_pInstance->SettleImpeach(pPubKernel, guildDataName.c_str());
// 	}
// 
// 	return 0;
// }
// void GuildBaseManager::AutoImpeach(IPubKernel* pPubKernel, const char* space_name, const wchar_t* guildName, const wchar_t* capName)
// {
// 	IPubSpace* pPubSpace = pPubKernel->GetPubSpace(space_name);
// 	if (pPubSpace == NULL){
// 		return;
// 	}
// 	std::wstring guildDataName = guildName + StringUtil::IntAsWideStr(pPubKernel->GetServerId());
// 	IPubData* pGuildData = pPubSpace->GetPubData(guildDataName.c_str());
// 
// 	if (pGuildData == NULL){
// 		return;
// 	}
// 
// 	IRecord *memberRecord = pGuildData->GetRecord(GUILD_MEMBER_REC);
// 	if (memberRecord == NULL)
// 	{
// 		return;
// 	}
// 
// 	struct MemberInfo
// 	{
// 		bool operator <(const MemberInfo& other)
// 		{
// 			return devote > other.devote;
// 		}
// 		int row;
// 		int devote{ 0 };
// 		int position{ -1 };
// 	};
// 
// 	int64_t now = ::time(NULL);
// 	std::list<MemberInfo> memList;
// 
// 	int countMax = memberRecord->GetRows();
// 	for (int i = 0; i < countMax; i++)
// 	{
// 		int position = memberRecord->QueryInt(i, GUILD_MEMBER_REC_COL_CAREER);
// 
// 		if (position == GUILD_POSITION_CAPTAIN){
// 			continue;
// 		}
// 		int onLineState = memberRecord->QueryInt(i, GUILD_MEMBER_REC_COL_ONLINE);
// 		if (onLineState != 1) //������
// 		{
// 			int64_t outLine = memberRecord->QueryInt64(i, GUILD_MEMBER_REC_COL_OUTLINE_DATE);
// 			if (now - outLine > m_memOffLineTimes){
// 				continue;
// 			}
// 		}
// 		int devote = memberRecord->QueryInt(i, GUILD_MEMBER_REC_COL_DEVOTE);
// 
// 		MemberInfo info;
// 		info.row = i;
// 		info.position = position;
// 		info.devote = devote;
// 		memList.push_back(info);
// 
// 	}
// 
// 	if (memList.empty())
// 	{
// 		return;
// 	}
// 
// 	memList.sort();
// 
// 	for (auto it : memList)
// 	{
// 		if (it.position == GUILD_POSITION_ASSISTANT_CAPTAIN)
// 		{
// 			const wchar_t * name = memberRecord->QueryWideStr(it.row, GUILD_MEMBER_REC_COL_NAME);
// 			m_pInstance->TransferCaptain(guildName, capName, name);
// 			return;
// 		}
// 	}
// 
// 	auto it = memList.begin();
// 	const wchar_t * name = memberRecord->QueryWideStr(it->row, GUILD_MEMBER_REC_COL_NAME);
// 	m_pInstance->TransferCaptain(guildName, capName, name);
// 
// }
// 
// void GuildBaseManager::AutoImpeachTimer(IPubKernel* pPubKernel, const char* space_name, const wchar_t* data_name)
// {
// 
// 	auto timeNow = ::time(NULL);
// 	IPubSpace* pPubSpace = pPubKernel->GetPubSpace(space_name);
// 	if (pPubSpace == NULL){
// 		return;
// 	}
// 
// 	// ����б�
// 	IRecord* pGuildList = PubGuildManager::m_pInstance->m_pGuildPubData->GetRecord(GUILD_LIST_REC);
// 	if (pGuildList == NULL){
// 		return;
// 	}
// 
// 	// �������а��
// 	int nRows = pGuildList->GetRows();
// 	for (int i = 0; i < nRows; ++i)
// 	{
// 		//// �������
// 		std::wstring wstrGuildName = pGuildList->QueryWideStr(i, GUILD_LIST_REC_COL_NAME);
// 		std::wstring guildDataName = wstrGuildName + StringUtil::IntAsWideStr(pPubKernel->GetServerId());
// 		IPubData* pGuildData = pPubSpace->GetPubData(guildDataName.c_str());
// 
// 		if (pGuildData == NULL){
// 			continue;
// 		}
// 		std::wstring capName = pGuildList->QueryWideStr(i, GUILD_LIST_REC_COL_CAPTAIN);
// 		if (StringUtil::CharIsNull(capName.c_str())){
// 			continue;
// 		}
// 
// 		IRecord *memberRecord = pGuildData->GetRecord(GUILD_MEMBER_REC);
// 		if (memberRecord == NULL)
// 		{
// 			continue;
// 		}
// 
// 		int row = memberRecord->FindWideStr(GUILD_MEMBER_REC_COL_NAME, capName.c_str());
// 		if (row < 0){
// 			continue;
// 		}
// 		int onLineState = memberRecord->QueryInt(row, GUILD_MEMBER_REC_COL_ONLINE);
// 		if (onLineState == 1) //����
// 		{
// 			continue;
// 		}
// 		int64_t lastOnLineTiem = memberRecord->QueryInt64(row, GUILD_MEMBER_REC_COL_OUTLINE_DATE);
// 		if (timeNow - lastOnLineTiem > m_autoImpeachMaxTime)
// 		{
// 			AutoImpeach(pPubKernel, space_name, wstrGuildName.c_str(), capName.c_str());
// 		}
// 
// 	}
// 
// 
// }


void GuildBaseManager::OnZeroClock(IPubKernel* pPubKernel)
{
	IRecord* pGuildList = m_pGuildPubData->GetRecord(GUILD_LIST_REC);
	if (pGuildList != NULL)
	{
		int nRows = pGuildList->GetRows();
		LoopBeginCheck(a);
		for (int i = 0; i < nRows; ++i)
		{
			LoopDoCheck(a);
			std::wstring guildName = pGuildList->QueryWideStr(i, GUILD_LIST_REC_COL_NAME);
			const std::wstring guildDataName = guildName + m_ServerId;
			IPubData *guildData = m_pPubSpace->GetPubData(guildDataName.c_str());
			
			// ���ô�������
			if (guildData == NULL) continue;
				
			IRecord *pRec = guildData->GetRecord(GUILD_MEMBER_REC);
			if (NULL == pRec)
			{
				::extend_warning_pub(LOG_ERROR, "[GuildBaseManager::OnZeroClock] not found guild member record");
				continue;
			}

			int nRows_ = pRec->GetRows();
			for (int k = 0; k < nRows_; ++k)
			{
				pRec->SetInt(k, GUILD_MEMBER_REC_COL_TEACH_NUM, 0);
				pRec->SetInt(k, GUILD_MEMBER_REC_COL_BE_TEACH_NUM, 0);
			}
		}
	}
}