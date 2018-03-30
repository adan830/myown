//----------------------------------------------------------
// �ļ���:      ChannelModule.cpp
// ��  ��:      Ƶ��ϵͳ�ӿ�
// ˵  ��:
// ��������:    2014��10��17��
// ������:       
// �޸���:    
//    :       
//----------------------------------------------------------

#include "ChannelModule.h"
#include "FsGame/Define/PubDefine.h"
#include "utils/extend_func.h"
#include "utils/util_func.h"
//#include "../SocialSystemModule/ChatModule.h"
#include "FsGame/Define/ServerCustomDefine.h"
#include "FsGame/Define/ClientCustomDefine.h"
//#include "FsGame/Define/GuildDefine.h"
#include "utils/string_util.h"

std::wstring ChannelModule::m_domainName;

//Ƶ�����
ChannelModule * ChannelModule::m_pChannelModule = NULL;
bool ChannelModule::Init(IKernel * pKernel)
{
	m_pChannelModule = this;

	pKernel->AddIntCustomHook("player", CLIENT_CUSTOMMSG_REQUEST_CHAT_HISTORY, ChannelModule::OnCustomChannelInfo);
	return true;
}

bool ChannelModule::Shut(IKernel * pKernel)
{
	return true;
}

//�ͻ���������Ϣ
int ChannelModule::OnCustomChannelInfo(IKernel* pKernel, const PERSISTID& self, 
							   const PERSISTID& sender, const IVarList& args)
{
	if (!pKernel->Exists(self))
	{
		return 0;
	}

//	m_pChannelModule->SendChannelToClient(pKernel, self);

	return 0;
}

//����Ƶ��
bool ChannelModule::JoinChannel(IKernel * pKernel, const PERSISTID & opplayer,
								const char * channel_type, const wchar_t * channel_name)
{
	//��ȡƵ�����
	int channel = GetChannel(pKernel, channel_type, channel_name);
	if (channel < 0)
	{
		return false;
	}

	//����
	return pKernel->ChannelAdd(channel, opplayer);
}

bool ChannelModule::LeaveChannel(IKernel * pKernel, const PERSISTID & opplayer,
								 const char * channel_type, const wchar_t * channel_name)
{
	//��ȡƵ�����
	int channel = GetChannel(pKernel, channel_type, channel_name);
	if (channel < 0)
	{
		return false;
	}

	//����
	return pKernel->ChannelRemove(channel, opplayer);
}

//���Ƶ�����
int ChannelModule::GetChannel(IKernel * pKernel, const char * channel_type, const wchar_t * channel_name)
{
	//��ȡƵ�����
	IPubSpace * pDomainSpace = pKernel->GetPubSpace(PUBSPACE_DOMAIN);
	if (pDomainSpace == NULL)
	{
		return -1;
	}


	IPubData * pChannel = pDomainSpace->GetPubData(GetDomainName(pKernel).c_str());
	if (pChannel == NULL)
	{
		return -1;
	}

	if (!pChannel->FindRecord(channel_type))
	{
		return -1;
	}

	int row = pChannel->FindRecordWideStr(channel_type, 0, channel_name);
	if (row < 0)
	{
		return -1;
	}

	return pChannel->QueryRecordInt(channel_type, row, 1);
}


//������������¼
// bool ChannelModule::SaveChannelInfo(IKernel *pKernel, const IVarList &args)
// {
// 	int nChannel = args.IntVal(0);
// 
// 	CVarList pub_msg;
// 	//���ᵥ������
// 	if (nChannel == CHATTYPE_GUILD)
// 	{
// 		const wchar_t* guildName = args.WideStrVal(6);
// 		const wchar_t* srcname = args.WideStrVal(2);
// 		const char* playerUid = pKernel->SeekRoleUid(srcname);
// 		pub_msg << PUBSPACE_GUILD
// 				<< guildName
// 				<< SP_GUILD_MSG_CHAT_INFO
// 				<< playerUid
// 				<< srcname
// 				<< args.WideStrVal(3)
// 				<< args.StringVal(7)
// 				<< args.IntVal(8)
// 				<< args.IntVal(9)
// 				<< args.IntVal(1)
// 				<< args.Int64Val(10)
// 				<< args.IntVal(5);
// 
// 		pKernel->SendPublicMessage(pub_msg);
// 
// 		return true;
// 	}
// 
// 	// ���Һ�����Ƶ��
// 	pub_msg << PUBSPACE_DOMAIN
// 			<< GetDomainName(pKernel)
// 			<< SP_DOMAIN_MSG_CHANNEL_SAVE
// 			<< args;
// 
// 	pKernel->SendPublicMessage(pub_msg);
// 
// 	return true;
// }

//�·�������ʷ��Ϣ
// bool ChannelModule::SendChannelToClient(IKernel* pKernel, const PERSISTID& self)
// {
// 	//�·������¼
// 	IPubSpace *pDoMainSpace = pKernel->GetPubSpace(PUBSPACE_DOMAIN);
// 	if (NULL == pDoMainSpace)
// 	{
// 		return false;
// 	}
// 
// 	// ��Ҷ���
// 	IGameObj *pPlayer = pKernel->GetGameObj(self);
// 	if (NULL == pPlayer)
// 	{
// 		return 0;
// 	}
// 
// 	IPubData *pChannel = pDoMainSpace->GetPubData(GetDomainName(pKernel).c_str());
// 	if (NULL == pChannel)
// 	{
// 		return false;
// 	}
// 
// 	//����Ƶ��
// 	std::string worldChannel = m_pChannelModule->GetChatRecordName(pChannel, CHATTYPE_WORLD);
// 	IRecord *pRec = pChannel->GetRecord(worldChannel.c_str());
// 	if (NULL != pRec)
// 	{
// 		int nRows = pRec->GetRows();
// 		LoopBeginCheck(a)
// 		for (int i = 0; i < nRows; i++)
// 		{
// 			LoopDoCheck(a)
// 			CVarList var;
// 			pRec->QueryRowValue(i, var);
// 			std::wstring name = var.WideStrVal(0);		//����
// 			std::wstring content = var.WideStrVal(1);	//����
// 			std::wstring guild = var.WideStrVal(2);		//����
// 			std::string resource = var.StringVal(3);	//��Դ
// 			const int nNationPost = var.IntVal(4);		//����ְλ
// 			const int nVip	= var.IntVal(5);			//vip
// 			const int nNation = var.IntVal(6);				//����
// 			const int64_t time  = var.Int64Val(7);			 //����ʱ��
// 			const int level = var.IntVal(8);			//�ȼ�
// 			CVarList msg;
// 			msg << SERVER_CUSTOMMSG_HISTORY_SPEECH
// 				<< CHATTYPE_WORLD
// 				<< content.c_str()
// 				<< name
// 				<< level
// 				<< guild
// 				<< resource
// 				<< nNationPost
// 				<< nVip
// 				<< time
// 				<< nNation;
// 				
// 
// 			pKernel->Custom(self, msg);
// 		}
// 	}
// 
// 	//����Ƶ��
// 	int nNation = pKernel->QueryInt(self, "Nation");
// 	std::string NationChannel = m_pChannelModule->GetChatRecordName(pChannel, CHATTYPE_NATION, nNation);
// 	pRec = pChannel->GetRecord(NationChannel.c_str());
// 	if (NULL != pRec)
// 	{
// 		int nRows = pRec->GetRows();
// 		LoopBeginCheck(b)
// 		for (int i = 0; i < nRows; i++)
// 		{
// 			LoopDoCheck(b)
// 			CVarList var;
// 			pRec->QueryRowValue(i, var);
// 			std::wstring name = var.WideStrVal(0);		//����
// 			std::wstring content = var.WideStrVal(1);	//����
// 			std::wstring guild = var.WideStrVal(2);		//����
// 			std::string resource = var.StringVal(3);	//��Դ
// 			const int nNationPost = var.IntVal(4);		//����ְλ
// 			const int nVip	= var.IntVal(5);			//vip
// 			const int64_t time  = var.Int64Val(6);			 //����ʱ�� 
// 			const int level = var.IntVal(7);			//�ȼ�
// 			CVarList msg;
// 			msg << SERVER_CUSTOMMSG_HISTORY_SPEECH 
// 				<< CHATTYPE_NATION
// 				<< content.c_str()
// 				<< name
// 				<< level
// 				<< guild
// 				<< resource
// 				<< nNationPost
// 				<< nVip
// 				<< time;
// 
// 			pKernel->Custom(self, msg);
// 		}
// 	}
// 
// 	//����Ƶ��
// 	const wchar_t *guildName = pPlayer->QueryWideStr("GuildName");
// 	if (StringUtil::CharIsNull(guildName))
// 	{
// 		return false;
// 	}
// 
// 	IPubSpace *pGuildSpace = pKernel->GetPubSpace(PUBSPACE_GUILD);
// 	if (NULL == pDoMainSpace)
// 	{
// 		return false;
// 	}
// 
// 	std::wstring guildDataName = guildName + util_int_as_widestr(pKernel->GetServerId());
// 	IPubData *pPubDate = pGuildSpace->GetPubData(guildDataName.c_str());
// 	if (NULL == pPubDate)
// 	{
// 		return false;
// 	}
// 
// 	pRec = pPubDate->GetRecord(GUILD_CHAT_HISTORY_REC);
// 	if (NULL != pRec)
// 	{
// 		int nRows = pRec->GetRows();
// 		LoopBeginCheck(c)
// 		for (int i = 0; i < nRows; i++)
// 		{
// 			LoopDoCheck(c)
// 			CVarList var;
// 			pRec->QueryRowValue(i, var);
// 			std::wstring name = var.WideStrVal(0);		//����
// 			std::wstring content = var.WideStrVal(1);	//����
// 			std::wstring guild = var.WideStrVal(2);		//����
// 			std::string resource = var.StringVal(3);	//��Դ
// 			const int nNationPost = var.IntVal(4);		//����ְλ
// 			const int nVip	= var.IntVal(5);			//vip
// 			const int post = var.IntVal(6);				//ְλ
// 			const int64_t time = var.Int64Val(7);         //��¼����ʱ��
// 			const int level = var.IntVal(8);			//�ȼ�
// 			CVarList msg;
// 			msg << SERVER_CUSTOMMSG_HISTORY_SPEECH 
// 				<< CHATTYPE_GUILD
// 				<< content.c_str()
// 				<< name
// 				<< level
// 				<< guild
// 				<< resource
// 				<< nNationPost
// 				<< nVip
// 				<< time
// 				<< post;
// 
// 			pKernel->Custom(self, msg);
// 		}
// 	}
// 
// 	return true;
// }

const std::wstring & ChannelModule::GetDomainName(IKernel * pKernel)
{

	if (m_domainName.empty())
	{
		wchar_t wstr[256];
		const int server_id = pKernel->GetServerId();
		SWPRINTF_S(wstr, DOMAIN_CHANNEL, server_id);

		m_domainName = wstr;
	}

	return m_domainName;

}

//��ȡ��Ƶ��������Ϣ�洢����
//��ȡ��Ƶ��������Ϣ�洢����
const std::string ChannelModule::GetChatRecordName(IPubData *pChannelData, const int channel, int subType)
{
	//�ϳɸ���Ƶ������
	char chRecord[64] = {0};
	if (subType < 0)
	{
		SPRINTF_S(chRecord, "channel_save_table_%d", channel);
	}
	else
	{
		SPRINTF_S(chRecord, "channel_save_table_%d_%d", channel, subType);
	}
	return std::string(chRecord);
}
