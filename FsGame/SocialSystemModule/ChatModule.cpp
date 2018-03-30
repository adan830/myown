//----------------------------------------------------------
// �ļ���:      ChatModule.cpp
// ��  ��:      ����ģ��
// ˵  ��:
// ��������:    2014��10��17��
// ������:       
// �޸���:    
//    :       
//----------------------------------------------------------
#include "ChatModule.h"
#include "FsGame/Define/ClientCustomDefine.h"
#include "FsGame/Define/CommandDefine.h"
#include "FsGame/Define/ServerCustomDefine.h"
#include "FsGame/Define/PubDefine.h"
#include "FsGame/Define/CoolDownDefine.h"
#include "FsGame/SystemFunctionModule/ChannelModule.h"
#include "FsGame/SystemFunctionModule/CoolDownModule.h"
#include "FsGame/CommonModule/LuaExtModule.h"
#include "FsGame/CommonModule/LogModule.h"
#include "FsGame/CommonModule/ContainerModule.h"
#include "FsGame/CommonModule/SnsDataModule.h"
#include "FsGame/Define/SnsDataDefine.h"
#include "utils/XmlFile.h"
#include "utils/util_func.h"
#include "utils/custom_func.h"
#include "utils/string_util.h"
#include "public/CoreFile.h"
#include "time.h"

#ifndef FSROOMLOGIC_EXPORTS
#include "CommonModule/ReLoadConfigModule.h"
#endif // FSROOMLOGIC_EXPORTS
#include "FsGame/CommonModule/FunctionEventModule.h"
#include "Define/Classes.h"
#include "Interface/FightInterface.h"
#include "Define/Fields.h"
#include "Define/TeamDefine.h"

// ģ�鶨��
ChatModule* ChatModule::m_pChatModule = NULL;
ContainerModule* ChatModule::m_pContainerModule = NULL;
CoolDownModule* ChatModule::m_pCDModule = NULL;

#ifndef FSROOMLOGIC_EXPORTS
SnsDataModule* ChatModule::m_pSnsDataModule = NULL;
ChannelModule* ChatModule::m_pChannelModule = NULL;
LogModule* ChatModule::m_pLogModule = NULL;
#endif // FSROOMLOGIC_EXPORTS

#define CHAT_CONFIG_URL "ini/SocialSystem/Chat/ChatRule.xml"
//��������
int nx_reload_itemid_time__config( void* state)
{
	// ��ú���ָ��
	IKernel* pKernel = LuaExtModule::GetKernel(state);
	// ���ָ����ڣ�����ģ���Ѵ���
	if (NULL != ChatModule::m_pChatModule)
	{
		ChatModule::m_pChatModule->LoadResource(pKernel);
	}

	return 0;
}

//��������
int nx_reload_itemid_badwords__config( void* state)
{
	// ��ú���ָ��
	IKernel* pKernel = LuaExtModule::GetKernel(state);
	// ���ָ����ڣ�����ģ���Ѵ���
	if (NULL != ChatModule::m_pChatModule)
	{
		ChatModule::m_pChatModule->LoadCheckWords(pKernel);
	}

	return 0;
}

// ��ʼ��
bool ChatModule::Init(IKernel* pKernel)
{
	m_pChatModule = this;
	m_pContainerModule = (ContainerModule*)pKernel->GetLogicModule("ContainerModule");
	m_pCDModule = (CoolDownModule *)pKernel->GetLogicModule("CoolDownModule");

	Assert(m_pChatModule != NULL
		&& m_pContainerModule != NULL
		&& m_pCDModule != NULL );

#ifndef FSROOMLOGIC_EXPORTS
	m_pSnsDataModule = dynamic_cast<SnsDataModule*>(pKernel->GetLogicModule("SnsDataModule"));
	m_pChannelModule = (ChannelModule*)pKernel->GetLogicModule("ChannelModule");
	//m_pGuildModule = (GuildModule*)pKernel->GetLogicModule("GuildModule");
	m_pLogModule = (LogModule*)pKernel->GetLogicModule("LogModule");

	Assert(m_pSnsDataModule != NULL
		&& m_pChannelModule != NULL
/*		&& m_pGuildModule != NULL*/
		&& m_pLogModule != NULL);
#endif // FSROOMLOGIC_EXPORTS

	pKernel->AddEventCallback("player", "OnRecover", ChatModule::OnRecover, 1);
	pKernel->AddEventCallback("player", "OnContinue", ChatModule::OnContinue);

	pKernel->AddIntCustomHook("player", CLIENT_CUSTOMMSG_CHAT, ChatModule::OnCustomChat);

	pKernel->AddIntCustomHook("player", CLIENT_CUSTOMMSG_CHAT_QUERY, ChatModule::OnCustomQuery);

	pKernel->AddIntCommandHook("player", COMMAND_CHAT, ChatModule::OnCommandChat);

	DECL_HEARTBEAT(ChatModule::H_Silence);

	LoadResource(pKernel);

	LoadCheckWords(pKernel);

	DECL_LUA_EXT(nx_reload_itemid_time__config);
	DECL_LUA_EXT(nx_reload_itemid_badwords__config);

#ifndef FSROOMLOGIC_EXPORTS
	RELOAD_CONFIG_REG("ChatConfig", ReloadResource);
#endif

	return true;
}

// �ͷ�
bool ChatModule::Shut(IKernel* pKernel)
{
	ReleaseRes();
	return true;
}
// �ͷ���Դ��Ϣ
bool ChatModule::ReleaseRes()
{
	// �ͷ�m_chatNationParam
	m_chatParam.clear();

	// �ͷ�m_chatCheckWords
	m_chatCheckWords.clear();

	m_chatSaveChannel.clear();
	return true;
}

// ����ָ����ҽ���
bool ChatModule::SetSilence(IKernel* pKernel, const PERSISTID& self, int silenceTime)
{
	// ����ʱ�䲻��Ϊ��������λΪ����
	if (silenceTime <= 0)
	{
		return false;
	}

	if (!pKernel->Exists(self))
	{
		return false;
	}

	// ��Ҷ���
	IGameObj *pPlayer = pKernel->GetGameObj(self);
	if (NULL == pPlayer)
	{
		return false;
	}

	// ֻ�������
	if (pPlayer->GetClassType() != TYPE_PLAYER)
	{
		return false;
	}

	// ������,�������ѽ��ԣ���ɾ�������������ý���ʱ��
	if (pKernel->FindHeartBeat(self, "ChatModule::H_Silence"))
	{
		pKernel->RemoveHeartBeat(self, "ChatModule::H_Silence");
	}
	pKernel->AddHeartBeat(self, "ChatModule::H_Silence", 10 * 1000);
	// ʱ��ת��Ϊ����
	pPlayer->SetInt("SilenceTick", silenceTime * 60000);

	return true;
}

//������������ļ�
bool ChatModule::LoadResource(IKernel* pKernel)
{
	std::string pathName = pKernel->GetResourcePath();
	//�����ļ�·��
	pathName += CHAT_CONFIG_URL;

	CXmlFile xmlfile(pathName.c_str());
	if (!xmlfile.LoadFromFile())
	{
		//��־
		return false;
	}

	int iSectionCount = (int)xmlfile.GetSectionCount();
	std::string strSectionName = "";
	//std::string nationParam = "";
	LoopBeginCheck(d)
	for (int i = 0;i < iSectionCount;i++)
	{
		LoopDoCheck(d)
		strSectionName = xmlfile.GetSectionByIndex(i);
		if (StringUtil::CharIsNull(strSectionName.c_str()))
		{
			return 0;
		}

		int num = ::atoi(strSectionName.c_str());
		if (num < 0)
		{
			continue;
		}

		if (num == 1)
		{
			std::string strParam = xmlfile.ReadString(strSectionName.c_str(), "Param","");
			if (StringUtil::CharIsNull(strParam.c_str()))
			{
				return 0;
			}

			CVarList paramList;
			util_split_string(paramList, strParam, ",");

			int numCount = (int)paramList.GetCount();
			LoopBeginCheck(a)
			for (int j = 0;j < numCount;j++)
			{
				LoopDoCheck(a)
				m_chatParam.push_back(paramList.StringVal(j));
			}
		}
		if(num == 2)
		{
			std::string strParam = xmlfile.ReadString(strSectionName.c_str(), "Param", "");
			if (StringUtil::CharIsNull(strParam.c_str()))
			{
				return 0;
			}
			CVarList paramList;
			util_split_string(paramList, strParam, ",");
			int nCnt = (int)paramList.GetCount();

			LoopBeginCheck(b)
			for (int j = 0; j < nCnt; j++)
			{
				LoopDoCheck(b)
				int nChannel = atoi(paramList.StringVal(j));
				m_chatSaveChannel.push_back(nChannel);
			}
		}
	}
	return 0;

}


void ChatModule::ReloadResource(IKernel* pKernel)
{
	ChatModule::m_pChatModule->LoadCheckWords(pKernel);
}

//��ȡ������ض����ļ�
bool ChatModule::LoadCheckWords(IKernel* pKernel)
{
	m_chatCheckWords.clear();

	std::string path = pKernel->GetResourcePath();
	path += "filter.txt";

	FILE *file = core_file::fopen(path.c_str(), "rb");
	if (file == NULL)
	{
		return false;
	}

	core_file::fseek( file, 0, SEEK_END );
	long lengths = core_file::ftell( file );
	core_file::fseek(file, 0, SEEK_SET);

	char* strContent = NEW char[lengths +1 ];
	memset(strContent, 0, lengths +1 );

	size_t result = core_file::fread(strContent, sizeof(char),lengths, file );
	if (result == 0)
	{
		delete[] strContent;
		strContent = 0; 

		return false;
	}

	strContent[lengths] = '\0';
	core_file::fclose(file);

	CVarList items; StringUtil::SplitString(items, strContent, "\n");
	LoopBeginCheck(a)
	for (size_t i=0; i<items.GetCount(); i++)
	{
		LoopDoCheck(a)
		std::string word = items.StringVal(i);
		word = StringUtil::Trim(word, 10); // ȥ���س�
		word = StringUtil::Trim(word, 13); // ȥ������
		word = StringUtil::Trim(word, 32); // ȥ���ո�

		m_chatCheckWords.push_back(::util_string_as_widestr(word.c_str()));
	}

	m_strWord = ::util_string_as_widestr("*");
	return true;

}

// ����Ƿ������֣�����Url
bool ChatModule::CheckBadWordsIgnoreUrl(std::wstring& strInfo)
{
    // �����������е�����
    // ����: ....[url=1,7440003-001-1427438407-0006,����ս��][u][[e8c936]����ս��[-]][-][/url]...
    // ���� [url= ... [/url] ֮������ݲ�����
    
    std::wstring url_begin_with = L"[url=";
    std::wstring url_end_with = L"[/url]";
    
    bool has_bad_words = false;
    std::string::size_type read_index = 0;
    std::wstring result_str;
    
    LoopBeginCheck(a)
    while (true)
    {
        LoopDoCheck(a)
        
        std::string::size_type start_pos = strInfo.find(url_begin_with, read_index);
        if (start_pos == std::string::npos)
        {
            // ľ����ʼ��, ���ľ��url
            std::wstring temp_str = strInfo.substr(read_index);
            
            has_bad_words = CheckBadWords(temp_str) ? true : has_bad_words;
            result_str.append(temp_str);
            
            break;
        }
        
        // �ҵ�����ʼ�������Ų��ҽ�����
        std::string::size_type end_pos = strInfo.find(url_end_with, start_pos);
        if (end_pos == std::string::npos)
        {
            // ľ�н�β��, ���ľ��url
            std::wstring temp_str = strInfo.substr(read_index);

            has_bad_words = CheckBadWords(temp_str) ? true : has_bad_words;
            result_str.append(temp_str);
            
            break;
        }
        
        // �ҵ�һ��url
        
        // ���˷�url����
        if (start_pos != read_index)
        {
            std::wstring temp_str = strInfo.substr(read_index, start_pos - read_index);

            has_bad_words = CheckBadWords(temp_str) ? true : has_bad_words;
            result_str.append(temp_str);
        }
        
        // ȡ��url
        result_str.append(strInfo.substr(start_pos, end_pos-start_pos + url_end_with.length()));
        
        read_index = end_pos + url_end_with.length();
        
        if (read_index >= strInfo.length())
        {
            break;
        }
    }
    
    strInfo = result_str;
    
    return has_bad_words;
}

// ����Ƿ�������
bool ChatModule::CheckBadWords(std::wstring& strInfo)
{ 
	LoopBeginCheck(b)
	for (size_t i = 0; i < m_chatCheckWords.size(); i ++)
	{
		LoopDoCheck(b)
		std::wstring strTemp = m_chatCheckWords[i];
		if (strTemp.empty())
		{
			continue;
		}

		int nIndex = int(strInfo.find(strTemp));
		if (nIndex != -1)//�ҵ��Ƿ���
		{
			std::wstring strSetWord = L"";
			//�滻��г��
			LoopBeginCheck(c)
			for (size_t j = 0; j < strTemp.length(); j++)
			{
				LoopDoCheck(c)
				strSetWord += m_strWord;
			}
			strInfo.replace(nIndex, strTemp.length(), strSetWord);
			return true;
		}
	}

	return false;
}

//�������
int ChatModule::OnRecover(IKernel* pKernel, const PERSISTID& self, 
						  const PERSISTID& sender, const IVarList& args)
{
	if (!pKernel->Exists(self))
	{
		return 0;
	}

	// ��Ҷ���
	IGameObj *pPlayer = pKernel->GetGameObj(self);
	if (NULL == pPlayer)
	{
		return 0;
	}

	if (pPlayer->QueryInt("SilenceTick") > 0)
	{
		//������
		pKernel->AddHeartBeat(self, "ChatModule::H_Silence", 10 * 1000);
	}

	return 0;
}

//����
int ChatModule::OnContinue(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args)
{
	if (!pKernel->Exists(self))
	{
		return 0;
	}

	// ��Ҷ���
	IGameObj *pPlayer = pKernel->GetGameObj(self);
	if (NULL == pPlayer)
	{
		return 0;
	}

	if (pPlayer->QueryInt("SilenceTick") > 0)
	{
		if (pKernel->FindHeartBeat(self, "ChatModule::H_Silence"))
		{
			pKernel->RemoveHeartBeat(self, "ChatModule::H_Silence");
		}
		pKernel->AddHeartBeat(self, "ChatModule::H_Silence", 10 * 1000);
	}

	return 0;

}

// ������ز�ѯ
int ChatModule::OnCustomQuery(IKernel* pKernel, const PERSISTID& self, 
						 const PERSISTID& sender, const IVarList& args)
{

	// ��Ϣ��ʽ
	//
	//     [msg_id][sub_msg_id]...
	//

	if (!pKernel->Exists(self))
	{
		return 0;
	}

	
	int sub_msg_id = args.IntVal(1);
	switch (sub_msg_id)
	{
	case SNS_DATA_REQUEST_FORM_CHAT:
		{
#ifndef FSROOMLOGIC_EXPORTS
			// args �ĸ�ʽ
			//      [msg_id][sub_msg_id][target_name]
			const wchar_t *target_player_name = args.WideStrVal(2);
			if (StringUtil::CharIsNull(target_player_name))
			{
				return 0;
			}
			CVarList query_args;
			query_args << SNS_DATA_REQUEST_FORM_CHAT;
			query_args << target_player_name;
			query_args << 1;
			query_args << "PlayerAttribute";
			m_pSnsDataModule->OnQueryPlayerData(pKernel,self,sender,query_args);
#endif // FSROOMLOGIC_EXPORTS
		}

		break;
	}

	return 0;
}

// �ӵ��ͻ��˵�������Ϣ
int ChatModule::OnCustomChat(IKernel* pKernel, const PERSISTID& self,
	const PERSISTID& sender, const IVarList& args)
{
  	if (!pKernel->Exists(self))
	{
		return 0;
	}
	int chat_type = args.IntVal(1);
	//Ƶ���Ƿ�����
	if (chat_type < 0)
	{
		return 0;
	}

	// ��Ҷ���
	IGameObj *pPlayer = pKernel->GetGameObj(self);
	if (NULL == pPlayer)
	{
		return 0;
	}

	int silencetick = pPlayer->QueryInt("SilenceTick");
	if (silencetick > 0)
	{
		//�ڽ�����
		::CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17001, CVarList());
		return 0;
	}

	//Ƶ������cd��
	if (CheckChannelInCD(pKernel, self, chat_type))
	{
		return 0;
	}

	if (args.GetCount() < 2)
	{
		return 0;
	}

	//�������ݵ����� 0��ʾ�������죬 1��ʾ��������
	int content_type = args.IntVal(2);
	switch (content_type)
	{
		//��������
	case CHAT_CONTENT_TYPE_WORD:
	{
		ChatContentTypeByWord(pKernel, self, args);
	}
	break;
	//��������
	case CHAT_CONTENT_TYPE_VOICE:
	{
		ChatContentTypeByVoice(pKernel, self, args);
	}
	break;
	default:
		return 0;
	}

	return 0;
}

//������Ϣ
int ChatModule::OnCommandChat(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args)
{

	CVarList argList;
	int count = (int)args.GetCount();

	if (count < 3)
	{
		return 0;
	}
	const wchar_t *speak_player_name = args.WideStrVal(4);

	argList.Append(args, 1, count - 1);
	if(pKernel->Custom(self, argList)==0)
	{
		//����������Ϣʧ��
		::CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17002, CVarList());
		return 0;
	}
	return 0;

}


//��������Ϊ����
int ChatModule::ChatContentTypeByWord(IKernel* pKernel, const PERSISTID& self, const IVarList& args)
{
	if (!pKernel->Exists(self))
	{
		return 0;
	}

	// ��Ҷ���
	IGameObj *pPlayer = pKernel->GetGameObj(self);
	if (NULL == pPlayer)
	{
		return 0;
	}

	int chat_type = args.IntVal(1);

	//��������
	const wchar_t* content = args.WideStrVal(3);
	if (StringUtil::CharIsNull(content))
	{
		return 0;
	}

	// ��ȡ���������е�չʾ�ĵ�����Ϣ
	ExtractItemInfo(pKernel, self, content);

	std::wstring word_content = content;
	//�����֣����¼������־
	if (m_pChatModule->CheckBadWordsIgnoreUrl(word_content))
	{
#ifndef FSROOMLOGIC_EXPORTS
		// ��¼��־
		std::wstring log_info = L"Unlawful Word : ";
		const wchar_t* name_self = pPlayer->QueryWideStr("Name");
		if (NULL == name_self)
		{
			return 0;
		}

		log_info = log_info + name_self + L" Chat Content: " + content;
		//m_pLogModule->SavePlayerLog(pKernel, self, LOG_PLAYER_CHAT, log_info.c_str());
#endif // FSROOMLOGIC_EXPORTS
	}

	if (StringUtil::CharIsNull(word_content.c_str()))
	{
		return 0;
	}

	const wchar_t* pTarget = NULL;
	// ת����ʱ��
	std::wstring strTmpTarget_;
	switch (chat_type)
	{
		//˽��
	case CHATTYPE_WHISPER:
	{
		pTarget = args.WideStrVal(4);
		if (StringUtil::CharIsNull(pTarget))
		{
			return 0;
		}
		m_pChatModule->OnChatChit(pKernel, self, pTarget, word_content.c_str());

	}
	break;
	//����
	case CHATTYPE_WORLD:
	{
#ifndef FSROOMLOGIC_EXPORTS
		m_pChatModule->OnChatWorld(pKernel, self, word_content.c_str());

#endif // FSROOMLOGIC_EXPORTS
	}
	break;
	//ϵͳ
	case CHATTYPE_SYSTEM:
	{
		m_pChatModule->OnChatSystem(pKernel, self, args);
	}
	break;
	// ���
// 	case CHATTYPE_GUILD:
// 	{
// #ifndef FSROOMLOGIC_EXPORTS
// 		pTarget = pKernel->QueryWideStr(self, FIELD_PROP_GUILD_NAME);
// 		m_pChatModule->OnChatGuild(pKernel, self, word_content.c_str());
// #endif // FSROOMLOGIC_EXPORTS
// 	}
// 	break;
	// ����
	case CHATTYPE_TEAM:
	{
		int nTeamId_ = pKernel->QueryInt(self, FIELD_PROP_TEAM_ID);
		strTmpTarget_ = StringUtil::IntAsWideStr(nTeamId_);
		pTarget = strTmpTarget_.c_str();
		m_pChatModule->OnChatTeam(pKernel, self, word_content.c_str());
	}
	break;
	case CHATTYPE_TEAM_INVITE:
	{
		m_pChatModule->OnChatTeamInvite(pKernel, self, word_content.c_str());
	}break;

	// С����
	case CHATTYPE_WORLD_NEW:
	{
		// ȡ�ñ���
		PERSISTID tool_box = pKernel->GetChild(self, ITEM_BOX_NAME_WSTR);
		if (!pKernel->Exists(tool_box))
		{
			return 0;
		}

		std::string &strItemId = m_pChatModule->m_chatParam[CHAT_ITEM_ID];
		int nRemoved = 0;
		if (FunctionEventModule::GetItemBindPriorUse(FUNCTION_EVENT_ID_USE_ITEM))
		{
			nRemoved = ContainerModule::RemoveItemsBindPrior(pKernel, tool_box, strItemId.c_str(), 1, FUNCTION_EVENT_ID_USE_ITEM, true);
		}
		else
		{
			nRemoved = ContainerModule::RemoveItems(pKernel, tool_box, strItemId.c_str(), 1, FUNCTION_EVENT_ID_USE_ITEM, true);
		}

		if (nRemoved < 1)
		{
			::CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17003, CVarList());
			return 0;
		}
// 		int after = ContainerModule::GetItemCount(pKernel, tool_box, strItemId.c_str());
// 		pKernel->Command(self, self, CVarList() << COMMAND_SEVEN_DAY_OBJ << COMMAND_SEVEN_DAY_OBJ_USE_SPEAKER );
#ifndef FSROOMLOGIC_EXPORTS
		// ֪ͨ�ɾ�ģ���������ʹ�óɾ�
// 		AchievementModule::m_pAchievementModule->UpdateAchievement(pKernel, self,
// 			SUBTYPE_BASIC_BRUSH_TRUMPET, CVarList() << 1);

		m_pChatModule->OnChatWorldNew(pKernel, self, word_content.c_str());
#endif // FSROOMLOGIC_EXPORTS
	}
		break;

	default:
		return 0;
	}

	// д��ɹ��ļ�¼������־
#ifndef FSROOMLOGIC_EXPORTS
	//LogModule::m_pLogModule->OnTextChat(pKernel, self, chat_type, pTarget, content);
#endif
	return 0;
}

//��������Ϊ����
int ChatModule::ChatContentTypeByVoice(IKernel* pKernel, const PERSISTID& self, const IVarList& args)
{
	if (!pKernel->Exists(self))
	{
		return 0;
	}

	// ��Ҷ���
	IGameObj *pPlayer = pKernel->GetGameObj(self);
	if (NULL == pPlayer)
	{
		return 0;
	}

	int chat_type = args.IntVal(1);

	//��������Ϊ����
	const char* msg_id = args.StringVal(3);

	//����ʱ��
	const int msg_time = args.IntVal(4);

	switch (chat_type)
	{
	//˽��
	case CHATTYPE_WHISPER:
	{
		const wchar_t* targetName = args.WideStrVal(5);
		if (StringUtil::CharIsNull(targetName))
		{
			return 0;
		}
		m_pChatModule->OnChatVoiceChit(pKernel, self, targetName, msg_id, msg_time);
	}
	break;
	//����
	case CHATTYPE_WORLD:
	{
#ifndef FSROOMLOGIC_EXPORTS
		m_pChatModule->OnChatVoiceWorld(pKernel, self, msg_id, msg_time);
#endif // FSROOMLOGIC_EXPORTS
	}
	break;
	// ���
// 	case CHATTYPE_GUILD:
// 	{
// #ifndef FSROOMLOGIC_EXPORTS
// 		m_pChatModule->OnChatVoiceGuild(pKernel, self, msg_id, msg_time);
// #endif // FSROOMLOGIC_EXPORTS
// 	}
// 	break;
// 	// ����
// 	case CHATTYPE_TEAM:
// 	{
// #ifndef FSROOMLOGIC_EXPORTS		   
// 		m_pChatModule->OnChatVoiceTeam(pKernel, self, msg_id, msg_time);
// #endif // FSROOMLOGIC_EXPORTS
// 	}
// 	break;
	}
	return 0;
}


//˽��
void ChatModule::OnChatChit( IKernel* pKernel, const PERSISTID& self,  const wchar_t* targetname, const wchar_t* content )
{
	if (!CheckWhisper(pKernel, self, targetname))
	{	
		return;
	}

	// ��Ҷ���
	IGameObj *pPlayer = pKernel->GetGameObj(self);
	if (NULL == pPlayer)
	{
		return;
	}

	const wchar_t* srcName = pPlayer->QueryWideStr("Name");
	if (StringUtil::CharIsNull(srcName))
	{
		return;
	}

	int iNation = pPlayer->QueryInt("Nation");

	CVarList var ;
	var << COMMAND_CHAT << SERVER_CUSTOMMSG_SPEECH << CHATTYPE_WHISPER << CHAT_CONTENT_TYPE_WORD << content << srcName;
	if (m_pChatModule->SetChatInfo(pKernel, self,  var))
	{
		var << iNation;

		pKernel->CommandByName(targetname, var);

		// ���cd���¼
		std::string strSC = m_pChatModule->m_chatParam[CD_WHISPER_TIME];
		m_pCDModule->BeginCoolDown(pKernel, self, COOLDOWN_CHAT_WHISPER, atoi(strSC.c_str()) * 1000);

	}
	
	return;
}

// ����
void ChatModule::OnChatWorld(IKernel* pKernel, const PERSISTID& self,	 const wchar_t* content)
{
	if (!pKernel->Exists(self))
	{
		return;
	}

	// ��Ҷ���
	IGameObj *pPlayer = pKernel->GetGameObj(self);
	if (NULL == pPlayer)
	{
		return;
	}

	//Ƶ������ȼ�����
	int player_level = pPlayer->QueryInt("Level");
	int limit_nation_level = atoi(m_pChatModule->m_chatParam[LIMIT_WORLD_LEVEL].c_str());
	if(player_level < limit_nation_level)
	{
		::CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17004, CVarList());
		return;
	}

	int64_t now = ::util_get_time_64();

	const wchar_t* srcName = pPlayer->QueryWideStr("Name");
	if (StringUtil::CharIsNull(srcName))
	{
		return;
	}

	int iNation = pPlayer->QueryInt("Nation");

	CVarList var;

	var << SERVER_CUSTOMMSG_SPEECH << CHATTYPE_WORLD << CHAT_CONTENT_TYPE_WORD << content << srcName;
	if (m_pChatModule->SetChatInfo(pKernel, self, var))
	{
		var << iNation;

#ifndef FSROOMLOGIC_EXPORTS
		pKernel->CustomByWorld(var);
#endif // FSROOMLOGIC_EXPORTS

		// ���cd���¼
		std::string strSC = m_pChatModule->m_chatParam[CD_WORLD_TIME];
		m_pCDModule->BeginCoolDown(pKernel, self, COOLDOWN_CHAT_WORLD, atoi(strSC.c_str()) * 1000);
	}

	//������������
	SaveChatInfo(pKernel, CHATTYPE_WORLD, self, content);

	return;
}

//new
void ChatModule::OnChatWorldNew(IKernel* pKernel, const PERSISTID& self, const wchar_t* content)
{
	if (!pKernel->Exists(self))
	{
		return;
	}
	
	// ��Ҷ���
	IGameObj *pPlayer = pKernel->GetGameObj(self);
	if (NULL == pPlayer)
	{
		return;
	}

	const wchar_t* srcName = pPlayer->QueryWideStr("Name");
	const int nNation = pPlayer->QueryInt("Nation");
	if (StringUtil::CharIsNull(srcName))
	{
		return;
	}

	CVarList var;

	var << SERVER_CUSTOMMSG_SPEECH 
		<< CHATTYPE_WORLD_NEW
		<< CHAT_CONTENT_TYPE_WORD
		<< content 
		<< srcName;
	if (m_pChatModule->SetChatInfo(pKernel, self, var))
	{
		var << nNation;		//����

#ifndef FSROOMLOGIC_EXPORTS
		pKernel->CustomByWorld(var);
#endif // FSROOMLOGIC_EXPORTS

		// ���cd���¼
		std::string strSC = m_pChatModule->m_chatParam[CD_WORLD_NEW_TIME];
		m_pCDModule->BeginCoolDown(pKernel, self, COOLDOWN_CHAT_WORLD_NEW, atoi(strSC.c_str()) * 1000);
	}
}


//ϵͳ
void ChatModule::OnChatSystem( IKernel* pKernel, const PERSISTID& self,  const IVarList& args )
{
	std::wstring word_content = L"";

	if (args.GetCount() < 2)
	{
		return;
	}
	CVarList var ;
	var << SERVER_CUSTOMMSG_SPEECH << CHATTYPE_SYSTEM << CHAT_CONTENT_TYPE_WORD;
	word_content = util_utf8_as_widestr(args.StringVal(2));	
	if (StringUtil::CharIsNull(word_content.c_str()))
	{
		return;
	}
	var.AddWideStr(word_content.c_str());

#ifndef FSROOMLOGIC_EXPORTS
	pKernel->CustomByWorld(var);
#endif // FSROOMLOGIC_EXPORTS

	return;
}

// ������Ϣ���������Ƶ��
// int ChatModule::ChatToGuild(IKernel* pKernel, const wchar_t* guildName, IVarList& args)
// {
// #ifndef FSROOMLOGIC_EXPORTS
// 	// ��ȡ������
// 	if (StringUtil::CharIsNull(guildName))
// 	{
// 		return 0;
// 	}
// 
// 	// ��ȡ��Ա���
// 	int row = 0;
// 	IRecord* pRecord = m_pGuildModule->GetMemberRecord(pKernel, guildName);
// 	if (pRecord == NULL)
// 	{
// 		return 0;
// 	}
// 
// 	// ֪ͨÿ������Ա
// 	LoopBeginCheck(d);
// 	for (int r=0; r < pRecord->GetRows(); r++)
// 	{
// 		LoopDoCheck(d);
// 		const wchar_t* name = pRecord->QueryWideStr(r, GUILD_MEMBER_REC_COL_NAME);
// 		if (StringUtil::CharIsNull(name))
// 		{
// 			continue;
// 		}
// 
// 		bool online = CheckPlayerOnline(pKernel, name);
// 		if (online)
// 		{
// 			pKernel->CommandByName(name, args);
// 		}
// 	}
// #endif// FSROOMLOGIC_EXPORTS
// 	return 1;
// }

// ������Ϣ���������Ƶ��
// int ChatModule::ChatToGuild(IKernel* pKernel, const PERSISTID& self, IVarList& args)
// {
// #ifndef FSROOMLOGIC_EXPORTS
// 	if (!pKernel->Exists(self))
// 	{
// 		return 0;
// 	}
// 
// 	// ��Ҷ���
// 	IGameObj *pPlayer = pKernel->GetGameObj(self);
// 	if (NULL == pPlayer)
// 	{
// 		return 0;
// 	}
// 
// 	// ��ȡ������
// 	const wchar_t* guildName = pPlayer->QueryWideStr("GuildName");
// 	if (StringUtil::CharIsNull(guildName))
// 	{
// 		return 0;
// 	}
// 
// 	// ��ȡ��Ա���
// 	int row = 0;
// 	IRecord* pRecord = m_pGuildModule->GetMemberRecord(pKernel, self, row);
// 	if (pRecord == NULL)
// 	{
// 		return 0;
// 	}
// 	
// 	// ֪ͨÿ������Ա
// 	LoopBeginCheck(d)
// 	for (int r=0; r < pRecord->GetRows(); r++)
// 	{
// 		LoopDoCheck(d)
// 		const wchar_t* name = pRecord->QueryWideStr(r, GUILD_MEMBER_REC_COL_NAME);
// 		if (StringUtil::CharIsNull(name))
// 		{
// 			continue;
// 		}
// 
// 		bool online = CheckPlayerOnline(pKernel, self, name, false);
// 		if (online)
// 		{
// 			pKernel->CommandByName(name, args);
// 		}
// 	}
// #endif // FSROOMLOGIC_EXPORTS
// 	return 1;
// }

//��� 
// void ChatModule::OnChatGuild(IKernel* pKernel, const PERSISTID& self, const wchar_t* content)
// {
// #ifndef FSROOMLOGIC_EXPORTS
// 	IGameObj* playerObj = pKernel->GetGameObj(self);
// 	if (NULL == playerObj)
// 	{
// 		return;
// 	}
// 
// 	const wchar_t* guildName = playerObj->QueryWideStr("GuildName");
// 
// 	const wchar_t* srcName = playerObj->QueryWideStr("Name");
// 
// 	const char* uid = pKernel->SeekRoleUid(srcName);
// 
// 	if (guildName==NULL || srcName == NULL || uid == NULL)
// 	{
// 		return;
// 	}
// 
// 	if (wcslen(guildName) <= 0)
// 	{
// 		::CustomSysInfo(pKernel, self, TIPSTYPE_NORMAL_CHAT_MESSAGE, STR_CHAT_SYSINFO_NOT_IN_GUILD, CVarList());
// 		return;
// 	}
// 
// 	bool isGuildExist = m_pGuildModule->IsGuildExist(pKernel, guildName);
// 	if (!isGuildExist)
// 	{
// 		//���᲻����
// 		::CustomSysInfo(pKernel, self, TIPSTYPE_NORMAL_CHAT_MESSAGE, STR_CHAT_SYSINFO_GUILD_NOT_EXIST, CVarList());
// 		return;
// 	}
// 
// 	int row = 0;
// 	IRecord* pRecord = m_pGuildModule->GetMemberRecord(pKernel, self, row);
// 
// 	if (pRecord == NULL)
// 	{
// 		// ���᲻����
// 		::CustomSysInfo(pKernel, self, TIPSTYPE_NORMAL_CHAT_MESSAGE, STR_CHAT_SYSINFO_GUILD_NOT_EXIST, CVarList());
// 		return;
// 	}
// 
// 	int srcRow = pRecord->FindString(GUILD_MEMBER_REC_COL_NAME_UID, uid);
// 	if (srcRow < 0)
// 	{
// 		::CustomSysInfo(pKernel, self, TIPSTYPE_NORMAL_CHAT_MESSAGE, STR_CHAT_SYSINFO_NOT_IN_GUILD, CVarList());
// 		return;
// 	}
// 
// 	int srcPost = pRecord->QueryInt(srcRow, GUILD_MEMBER_REC_COL_POSITION);
// 
// 	CVarList var;
// 	var << COMMAND_CHAT << SERVER_CUSTOMMSG_SPEECH << CHATTYPE_GUILD << CHAT_CONTENT_TYPE_WORD << content << srcName;
// 	if (m_pChatModule->SetChatInfo(pKernel, self, var))
// 	{
// 		//������ְ��
// 		var.AddInt(srcPost);
// 
// 		// ֪ͨÿ������Ա
// 		LoopBeginCheck(e)
// 		for (int r=0; r < pRecord->GetRows(); r++)
// 		{
// 			LoopDoCheck(e)
// 			const wchar_t* name = pRecord->QueryWideStr(r, GUILD_MEMBER_REC_COL_NAME);
// 			if (StringUtil::CharIsNull(name))
// 			{
// 				continue;
// 			}
// 
// 			bool online = CheckPlayerOnline(pKernel, self, name, false);
// 			if (online)
// 			{
// 				pKernel->CommandByName(name, var);
// 			}
// 		}
// 
// 		// ���cd���¼
// 		std::string strSC = m_pChatModule->m_chatNationParam[CD_GUILD_TIME];
// 		m_pCDModule->BeginCoolDown(pKernel, self, COOLDOWN_CHAT_GUILD, atoi(strSC.c_str()) * 1000);
// 
// 		//������������
// 		SaveChatInfo(pKernel, CHATTYPE_GUILD, self, content, srcPost);
// 	}
// #endif // FSROOMLOGIC_EXPORTS
// 	return;
// }

// ����������Ϣ
// int ChatModule::InteractToScene(IKernel* pKernel, const PERSISTID& self, const PERSISTID& skill)
// {
// 	IGameObj* pSelfObj = pKernel->GetGameObj(self);
// 	IGameObj* pSkillObj = pKernel->GetGameObj(skill);
// 	if (NULL == pSelfObj || NULL == pSkillObj)
// 	{
// 		return 0;
// 	}
// 
// 	// �Ƿ�Ϊ��������
// 	const char* strSkillId = pSkillObj->GetConfig();
// 	int nSkillType = FightInterfaceInstance->GetSkillTypeByConfig(pKernel, strSkillId);
// 	if (nSkillType != INTERACT_SKILL_TYPE)
// 	{
// 		return 0;
// 	}
// 
// 	std::wstring wsSkillId = StringUtil::StringAsWideStr(strSkillId);
// 	const wchar_t* srcName = pSelfObj->QueryWideStr(FIELD_PROP_NAME);
// 	int nSceneId = pKernel->GetSceneId();
// 
// 	CVarList msg;
// 	msg << SERVER_CUSTOMMSG_SPEECH << CHATTYPE_INTERACT << CHAT_CONTENT_TYPE_WORD << wsSkillId << srcName;
// 	pKernel->CustomByScene(nSceneId, msg);
// 	return 0;
// }

//����
void ChatModule::OnChatTeam(IKernel* pKernel, const PERSISTID& self, const wchar_t* content)
{
	// 
	IGameObj* playerObj = pKernel->GetGameObj(self);
	if (NULL == playerObj)
	{

		return;
	}
	IRecord* pRecord = playerObj->GetRecord(TEAM_REC_NAME);
	if (NULL == pRecord)
	{
		return;
	}
	const wchar_t* srcName = playerObj->QueryWideStr("Name");
	if (StringUtil::CharIsNull(srcName))
	{
		return;
	}

	int count = pRecord->GetRows();
	if (count <= 0)
	{
		// ���ڶ�����
		::CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17009, CVarList());
		return;
	}

	CVarList var;
	var << COMMAND_CHAT << SERVER_CUSTOMMSG_SPEECH << CHATTYPE_TEAM << CHAT_CONTENT_TYPE_WORD << content << srcName;
	if (m_pChatModule->SetChatInfo(pKernel, self, var))
	{
		LoopBeginCheck(f)
		for( int r = 0; r < count; r++ )
		{
			LoopDoCheck(f)
			const char* uid = pRecord->QueryString(r, TEAM_REC_COL_UID);
			if (NULL == uid)
			{
				continue;
			}
			const wchar_t* name = pKernel->SeekRoleName(uid);
			if (NULL == name)
			{
				continue;
			}

			pKernel->CommandByName(name, var);
		}

		// ���cd���¼
		std::string strSC = m_pChatModule->m_chatParam[CD_TEAM_TIME];
		m_pCDModule->BeginCoolDown(pKernel, self, COOLDOWN_CHAT_TEAM, atoi(strSC.c_str()) * 1000);
	}
}


void ChatModule::OnChatTeamInvite(IKernel* pKernel, const PERSISTID& self, const wchar_t* content)
{
	IGameObj* playerObj = pKernel->GetGameObj(self);
	if (NULL == playerObj)
	{
		return;
	}
	IRecord* pRecord = playerObj->GetRecord(TEAM_REC_NAME);
	if (NULL == pRecord)
	{
		return;
	}
	const wchar_t* srcName = playerObj->QueryWideStr("Name");
	if (StringUtil::CharIsNull(srcName))
	{
		return;
	}

	int iNation = playerObj->QueryInt("Nation");

	CVarList var;

	var << SERVER_CUSTOMMSG_SPEECH << CHATTYPE_TEAM_INVITE << CHAT_CONTENT_TYPE_WORD << content << srcName;
	if (m_pChatModule->SetChatInfo(pKernel, self, var))
	{
		var << iNation;
#ifndef FSROOMLOGIC_EXPORTS
		pKernel->CustomByWorld(var);
#endif
	}
}

//����˽��
void ChatModule::OnChatVoiceChit(IKernel* pKernel, const PERSISTID& self, const wchar_t* targetname, const char* msg_id, int msg_time)
{
	if (!CheckWhisper(pKernel, self, targetname))
	{
		return;
	}

	// ��Ҷ���
	IGameObj *pPlayer = pKernel->GetGameObj(self);
	if (NULL == pPlayer)
	{
		return;
	}

	const wchar_t* srcName = pPlayer->QueryWideStr("Name");
	if (StringUtil::CharIsNull(srcName))
	{
		return;
	}

	int iNation = pPlayer->QueryInt("Nation");

	CVarList var;
	var << COMMAND_CHAT << SERVER_CUSTOMMSG_SPEECH << CHATTYPE_WHISPER << CHAT_CONTENT_TYPE_VOICE << msg_id << msg_time << srcName;
	if (m_pChatModule->SetChatInfo(pKernel, self, var))
	{
		var << iNation;

		pKernel->CommandByName(targetname, var);

		// ���cd���¼
		std::string strSC = m_pChatModule->m_chatParam[CD_WHISPER_TIME];
		m_pCDModule->BeginCoolDown(pKernel, self, COOLDOWN_CHAT_WHISPER, atoi(strSC.c_str()) * 1000);

	}

	return;
}

// ��������
void ChatModule::OnChatVoiceWorld(IKernel* pKernel, const PERSISTID& self, const char* msg_id, int msg_time)
{
	if (!pKernel->Exists(self))
	{
		return;
	}

	// ��Ҷ���
	IGameObj *pPlayer = pKernel->GetGameObj(self);
	if (NULL == pPlayer)
	{
		return;
	}

	//Ƶ������ȼ�����
	int player_level = pPlayer->QueryInt("Level");
	int limit_nation_level = atoi(m_pChatModule->m_chatParam[LIMIT_WORLD_LEVEL].c_str());
	if (player_level < limit_nation_level)
	{
		::CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17004, CVarList());
		return;
	}

	int64_t now = ::util_get_time_64();

	const wchar_t* srcName = pPlayer->QueryWideStr("Name");
	if (StringUtil::CharIsNull(srcName))
	{
		return;
	}

	int iNation = pPlayer->QueryInt("Nation");

	CVarList var;

	var << SERVER_CUSTOMMSG_SPEECH << CHATTYPE_WORLD << CHAT_CONTENT_TYPE_VOICE << msg_id << msg_time << srcName;
	if (m_pChatModule->SetChatInfo(pKernel, self, var))
	{
		var << iNation;

#ifndef FSROOMLOGIC_EXPORTS
		pKernel->CustomByWorld(var);
#endif // FSROOMLOGIC_EXPORTS

		// ���cd���¼
		std::string strSC = m_pChatModule->m_chatParam[CD_WORLD_TIME];
		m_pCDModule->BeginCoolDown(pKernel, self, COOLDOWN_CHAT_WORLD, atoi(strSC.c_str()) * 1000);
	}

	return;
}


//����������� 
// void ChatModule::OnChatVoiceGuild(IKernel* pKernel, const PERSISTID& self, const char* msg_id, int msg_time)
// {
// #ifndef FSROOMLOGIC_EXPORTS
// 	IGameObj* playerObj = pKernel->GetGameObj(self);
// 	if (NULL == playerObj)
// 	{
// 		return;
// 	}
// 
// 	const wchar_t* guildName = playerObj->QueryWideStr("GuildName");
// 
// 	const wchar_t* srcName = playerObj->QueryWideStr("Name");
// 
// 	const char* uid = pKernel->SeekRoleUid(srcName);
// 
// 	if (guildName == NULL || srcName == NULL || uid == NULL)
// 	{
// 		return;
// 	}
// 
// 	if (wcslen(guildName) <= 0)
// 	{
// 		::CustomSysInfo(pKernel, self, TIPSTYPE_NORMAL_CHAT_MESSAGE, STR_CHAT_SYSINFO_NOT_IN_GUILD, CVarList());
// 		return;
// 	}
// 
// 	bool isGuildExist = m_pGuildModule->IsGuildExist(pKernel, guildName);
// 	if (!isGuildExist)
// 	{
// 		//���᲻����
// 		::CustomSysInfo(pKernel, self, TIPSTYPE_NORMAL_CHAT_MESSAGE, STR_CHAT_SYSINFO_GUILD_NOT_EXIST, CVarList());
// 		return;
// 	}
// 
// 	int row = 0;
// 	IRecord* pRecord = m_pGuildModule->GetMemberRecord(pKernel, self, row);
// 
// 	if (pRecord == NULL)
// 	{
// 		// ���᲻����
// 		::CustomSysInfo(pKernel, self, TIPSTYPE_NORMAL_CHAT_MESSAGE, STR_CHAT_SYSINFO_GUILD_NOT_EXIST, CVarList());
// 		return;
// 	}
// 
// 	int srcRow = pRecord->FindString(GUILD_MEMBER_REC_COL_NAME_UID, uid);
// 	if (srcRow < 0)
// 	{
// 		::CustomSysInfo(pKernel, self, TIPSTYPE_NORMAL_CHAT_MESSAGE, STR_CHAT_SYSINFO_NOT_IN_GUILD, CVarList());
// 		return;
// 	}
// 
// 	int srcPost = pRecord->QueryInt(srcRow, GUILD_MEMBER_REC_COL_POSITION);
// 
// 	CVarList var;
// 	var << COMMAND_CHAT << SERVER_CUSTOMMSG_SPEECH << CHATTYPE_GUILD << CHAT_CONTENT_TYPE_VOICE << msg_id << msg_time << srcName;
// 	if (m_pChatModule->SetChatInfo(pKernel, self, var))
// 	{
// 		//������ְ��
// 		var.AddInt(srcPost);
// 
// 		// ֪ͨÿ������Ա
// 		LoopBeginCheck(e)
// 		for (int r = 0; r < pRecord->GetRows(); r++)
// 		{
// 			LoopDoCheck(e)
// 				const wchar_t* name = pRecord->QueryWideStr(r, GUILD_MEMBER_REC_COL_NAME);
// 			if (StringUtil::CharIsNull(name))
// 			{
// 				continue;
// 			}
// 
// 			bool online = CheckPlayerOnline(pKernel, self, name, false);
// 			if (online)
// 			{
// 				pKernel->CommandByName(name, var);
// 			}
// 		}
// 
// 		// ���cd���¼
// 		std::string strSC = m_pChatModule->m_chatNationParam[CD_GUILD_TIME];
// 		m_pCDModule->BeginCoolDown(pKernel, self, COOLDOWN_CHAT_GUILD, atoi(strSC.c_str()) * 1000);
// 	}
// #endif // FSROOMLOGIC_EXPORTS
// 	return;
// }

// ��������
// void ChatModule::OnChatVoiceTeam(IKernel* pKernel, const PERSISTID& self, const char* msg_id, int msg_time)
// {
// 	IGameObj* playerObj = pKernel->GetGameObj(self);
// 	if (NULL == playerObj)
// 	{
// 		return;
// 	}
// 	IRecord* pRecord = playerObj->GetRecord(TEAM_REC_NAME);
// 	if (NULL == pRecord)
// 	{
// 		return;
// 	}
// 	const wchar_t* srcName = playerObj->QueryWideStr("Name");
// 	if (StringUtil::CharIsNull(srcName))
// 	{
// 		return;
// 	}
// 
// 	int count = pRecord->GetRows();
// 	if (count <= 0)
// 	{
// 		// ���ڶ�����
// 		::CustomSysInfo(pKernel, self, TIPSTYPE_NORMAL_CHAT_MESSAGE, STR_CHAT_SYSINFO_NOT_IN_TEAM, CVarList());
// 		return;
// 	}
// 
// 	CVarList var;
// 	var << COMMAND_CHAT << SERVER_CUSTOMMSG_SPEECH << CHATTYPE_TEAM << CHAT_CONTENT_TYPE_VOICE << msg_id << msg_time << srcName;
// 	if (m_pChatModule->SetChatInfo(pKernel, self, var))
// 	{
// 		LoopBeginCheck(f);
// 		for (int r = 0; r < count; r++)
// 		{
// 			LoopDoCheck(f);
// 			const char* uid = pRecord->QueryString(r, TEAM_REC_COL_UID);
// 			if (NULL == uid)
// 			{
// 				continue;
// 			}
// 			const wchar_t* name = pKernel->SeekRoleName(uid);
// 			if (NULL == name)
// 			{
// 				continue;
// 			}
// 
// 			pKernel->CommandByName(name, var);
// 		}
// 
// 		// ���cd���¼
// 		std::string strSC = m_pChatModule->m_chatNationParam[CD_TEAM_TIME];
// 		m_pCDModule->BeginCoolDown(pKernel, self, COOLDOWN_CHAT_TEAM, atoi(strSC.c_str()) * 1000);
// 	}
// }

// ����cd�ж�
bool ChatModule::CheckChannelInCD(IKernel* pKernel, const PERSISTID& self, const int channel)
{
	//ϵͳû��cd
	if (channel == CHATTYPE_SYSTEM)
	{
		return false;
	}

	IGameObj* player = pKernel->GetGameObj(self);
	if (NULL == player)
	{
		return true;
	}
	
	// ��ȴ����
	int nCoolDownType = 0;

	switch(channel)
	{
	case CHATTYPE_WHISPER:
			nCoolDownType = COOLDOWN_CHAT_WHISPER;
		break;
	case CHATTYPE_WORLD:
			nCoolDownType = COOLDOWN_CHAT_WORLD;
		break;
	case CHATTYPE_GUILD:
			nCoolDownType = COOLDOWN_CHAT_GUILD;
		break;
// 	case CHATTYPE_TEAM:
// 			nCoolDownType = COOLDOWN_CHAT_TEAM;
// 		break;
	case CHATTYPE_WORLD_NEW:
			nCoolDownType = COOLDOWN_CHAT_WORLD_NEW;
		break;
	default:
		break;
	}

	// ����CD��
	int64_t cdTime = m_pCDModule->GetMillisecRemain(pKernel, self, nCoolDownType);
	if (cdTime <= 0)
	{
		return false;
	}

	// ��ȴ�У�ʣ��ʱ��{@0:��}��
	::CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17005, CVarList() << cdTime / 1000);
	return true;
}

//�ж�����Ƿ�����
bool ChatModule::CheckPlayerOnline(IKernel* pKernel, const PERSISTID& self, const wchar_t* targetName, bool bTipCustom/*= true*/)
{
	//�жϸ�����Ƿ����
	if (bTipCustom && strnlen(pKernel->SeekRoleUid(targetName), 50) == 0)
	{
		::CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17006, CVarList() << targetName);
		return false;
	}

	//�ж�����Ƿ�����
	if (bTipCustom && pKernel->GetPlayerScene(targetName) <= 0)
	{
		::CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17007, CVarList() << targetName);
		return false;
	}

	return true;
}

// ������������ж�����Ƿ����ߣ����ж��Ƿ���ڣ�
bool ChatModule::CheckPlayerOnline(IKernel* pKernel, const wchar_t* targetName)
{
	//�жϸ�����Ƿ����
	if (strnlen(pKernel->SeekRoleUid(targetName), 50) == 0)
	{
		return false;
	}

	//�ж�����Ƿ�����
	if (pKernel->GetPlayerScene(targetName) <= 0)
	{
		return false;
	}

	return true;
}

//�����Ϣ �ֶ�
bool ChatModule::SetChatInfo( IKernel* pKernel, const PERSISTID& self, CVarList& var )
{

	// ��Ҷ���
	IGameObj *pPlayer = pKernel->GetGameObj(self);
	if (NULL == pPlayer)
	{
		return false;
	}
	int level = pPlayer->QueryInt("Level");
	// ���
	const wchar_t* guildName = pPlayer->QueryWideStr("GuildName");
	// �Ա�
	int nSex = pPlayer->QueryInt(FIELD_PROP_SEX);
	// ְҵ
	int nJob = pPlayer->QueryInt(FIELD_PROP_JOB);
	// vip�ȼ�
	const int nVipLevel = pPlayer->QueryInt("VipLevel");
	var << level << guildName << nSex << nJob << nVipLevel;

	// ��ӵ�ǰʱ��
	int64_t now = ::time(NULL);
	var << now;

	return true;

}

//���������
bool ChatModule::CheckWhisper(IKernel* pKernel, const PERSISTID& self, const wchar_t* targetName)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return false;
	}

	if (wcslen(targetName) == 0)
	{
		::CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17006, CVarList());
		return false;
	}

	const wchar_t* srcname = pSelfObj->QueryWideStr("Name");
	if (StringUtil::CharIsNull(srcname))
	{
		return false;
	}

	if (wcscmp(srcname, targetName) == 0)
	{
		::CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17008, CVarList());
		return false;
	}
	//�жϸ�����Ƿ����
	if (strnlen(pKernel->SeekRoleUid(targetName), 50) == 0)
	{
		::CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17006, CVarList() << targetName);
		return false;
	}

	//�ж�����Ƿ�����
	if (pKernel->GetPlayerScene(targetName) <= 0)
	{
		::CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17007, CVarList() << targetName);
		return false;
	}


	return true;
}

//��������
int ChatModule::H_Silence(IKernel* pKernel, const PERSISTID& self, int slice)
{
	// ��Ҷ���
	IGameObj *pPlayer = pKernel->GetGameObj(self);
	if (NULL == pPlayer)
	{
		return 0;
	}

	int silencetick = pPlayer->QueryInt("SilenceTick");
	int newtick = silencetick - slice;
	if (newtick <= 0)
	{
		//���Խ���
		pPlayer->SetInt("SilenceTick", 0);
		pKernel->RemoveHeartBeat(self, "ChatModule::H_Silence");

		return 0;
	}

	//������
	pPlayer->SetInt("SilenceTick", newtick);
	return 0;
}


// ���ͨ������չʾ�Լ��ĵ���(װ������Ʒ��), ��������������ȡ������Ϣ, 
int ChatModule::ExtractItemInfo(IKernel* pKernel, const PERSISTID& self, const wchar_t *content)
{
    // �� @content ��, ������Ϣ������������:
    // װ����
    //     ... [url=1,7440003-001-1427438407-0006,����ս��][u][[e8c936]����ս��[-]][-][/url]...
    // ��ͨ��Ʒ:
    //     ... [url=2,7440003-001-1427438407-0006,����ս��][u][[e8c936]����ս��[-]][-][/url]...
    //
    // ��ʽ˵��: [url=����,��ƷUID,��Ʒ��]...[/url]

	// ��Ҷ���
	IGameObj *pPlayer = pKernel->GetGameObj(self);
	if (NULL == pPlayer)
	{
		return 0;
	}
    
    std::string str_content = util_widestr_as_string(content).c_str();
    
    CVarList items_list;

    const char *url_str = "url=";
    size_t url_str_len = strlen(url_str);
    
    size_t url_pos = 0;
    url_pos = str_content.find("url=", url_pos);
	LoopBeginCheck(j)
    while (url_pos != std::string::npos)
    {
		LoopDoCheck(j)
        // ������: [url=����,��ƷUID,��Ʒ��]
        
        size_t first_comm = str_content.find(",", url_pos);
        if (first_comm == std::string::npos)
        {
            url_pos = str_content.find("url=", url_pos + url_str_len);
            continue;
        }

        size_t second_comm = str_content.find(",", first_comm + 1);
        if (second_comm == std::string::npos)
        {
            url_pos = str_content.find("url=", url_pos + url_str_len);
            continue;
        }

        size_t third_comm = str_content.find(",", second_comm + 1);
        if (third_comm == std::string::npos)
        {
            url_pos = str_content.find("url=", url_pos + url_str_len);
        }

        // ���� �� UniqueID
        std::string str_link_type = str_content.substr(url_pos+url_str_len, first_comm-url_pos - url_str_len);
        std::string unique_id = str_content.substr(first_comm+1, second_comm - first_comm -1);
        std::string configID = str_content.substr(second_comm+1, third_comm - second_comm - 1);

        url_pos = str_content.find("url=", url_pos + url_str_len);

        
        int link_type = atoi(str_link_type.c_str());
        items_list << link_type;
        items_list << unique_id;
        items_list << configID;
    }
    
    CVarList items_info;
    items_info << pPlayer->QueryString("Uid");
    items_info << pPlayer->QueryWideStr("Name");
    
    if (items_list.GetCount() > 0)
    {
        InnerQueryItemInfo(pKernel, self, items_list, items_info);
        
        // ���͵�public������
        
        SaveShowoffItem(pKernel,self, items_info);
    }
    
    return 0;
}

// ��ȡչʾ��װ����Ϣ
bool ChatModule::InnerQueryItemInfo(IKernel* pKernel, const PERSISTID& self,
                              const IVarList& items_list, CVarList& items_info)
{
//     static const char *prop_list[] = {"ColorLevel", "BasePackage", "AddPackage",
//      "ModifyLevel", "ModifyAddPackage"};

    int item_count = (int)(items_list.GetCount()/3);
    
    // ���װ����
    PERSISTID equipment_box = pKernel->GetChild(self, L"EquipBox");
    if (!pKernel->Exists(equipment_box))
    {
        return false;
    }
    
    // ��ȡ@items_list �������Ʒ����
    int args_index = -1;
	LoopBeginCheck(h)
    for (int i=0; i<item_count; ++i)
    {
		LoopDoCheck(h)
        int item_type = items_list.IntVal(++args_index);
        const char *item_uid = items_list.StringVal(++args_index);
        const char* itemConfig = items_list.StringVal(++args_index);
        
        if (item_type != 1 && item_type != 2 && item_type != 8)
        {
            continue;
        }

        PERSISTID container = ContainerModule::GetBoxContainer(pKernel, self, itemConfig);
        if (container.IsNull())
        {
            continue;
        }
        
        // ���ڱ�������
        PERSISTID item = m_pContainerModule->FindItemByUid(pKernel, container, item_uid);

        if (!pKernel->Exists(item))
        {
            // ��������û��, �����ǲ�����װ����
            item = m_pContainerModule->FindItemByUid(pKernel, equipment_box, item_uid);

            if (!pKernel->Exists(item))
            {
                // û�д���Ʒ
                continue;
            }
        }
        
        fast_string szInfo;
        if (!(::player_item_to_string(pKernel, item, szInfo)))
        {
            // ��ȡ��Ʒ��Ϣʧ��
            continue;
        }
        
        items_info << item_uid;
        items_info << szInfo.c_str();
        
    }// ��Ʒѭ������
    
    return true;
}

// ����չʾ����Ʒ
bool ChatModule::SaveShowoffItem(IKernel* pKernel, const PERSISTID& self,
                                 const CVarList& items_info)
{
#ifndef FSROOMLOGIC_EXPORTS
//     wchar_t public_showoff_name[256] = {0};
//     SWPRINTF_S(public_showoff_name, DOMAIN_SHOWOFF_NAME_FORMAT_STR, pKernel->GetServerId());
// 
//     CVarList pub_msg;
//     pub_msg << PUBSPACE_DOMAIN;
//     pub_msg << public_showoff_name;
//     pub_msg << SP_DOMAIN_MSG_SHOWOFF;
//     pub_msg << SHOWOFF_PUB_SAVE_ITEM;
//     pub_msg << items_info;
// 
//     pKernel->SendPublicMessage(pub_msg);
#endif // FSROOMLOGIC_EXPORTS
    return false;
}

//����������Ϣ
bool ChatModule::SaveChatInfo(IKernel *pKernel, const int channel, 
							  const PERSISTID& self, const std::wstring &content, int subType)
{
#ifndef FSROOMLOGIC_EXPORTS
	// ��Ҷ���
	IGameObj *pPlayer = pKernel->GetGameObj(self);
	if (NULL == pPlayer)
	{
		return 0;
	}

	//�������豣���Ƶ��
	int nCnt = (int)m_pChatModule->m_chatSaveChannel.size();
	LoopBeginCheck(i)
	for (int i = 0; i < nCnt; i++)
	{
		LoopDoCheck(i)
		if (channel == m_pChatModule->m_chatSaveChannel[i])
		{
			const wchar_t* srcname = pPlayer->QueryWideStr("Name");
			if (NULL == srcname)
			{
				return false;
			}

			//int nNation = pPlayer->QueryInt("Nation");
			
			//��Ҫ���������
			CVarList var;
			var << channel
				<< subType
				<< srcname
				<< content;
			SetChatInfo(pKernel, self, var);

			//m_pChannelModule->SaveChannelInfo(pKernel, var);
			return true;
		}
	}
#endif // FSROOMLOGIC_EXPORTS
	return false;
}