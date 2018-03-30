//----------------------------------------------------------
// �ļ���:      ChatModule.h
// ��  ��:      ����ģ��
// ˵  ��:
// ��������:    2014��10��17��
// ������:       
// �޸���:    
//    :       
//----------------------------------------------------------
#ifndef _ChatModule_H_
#define _ChatModule_H_

#include "Fsgame/Define/header.h"
#include <vector>
#include <string>

// ����Ƶ��
//static const char * CHANNEL_TYPE_NATION = "channel_type_nation";

//����
enum
{
	CHAT_ITEM_ID,					//С����id
	CD_WHISPER_TIME,				//˽��cd
	CD_WORLD_TIME,					//��������cd
	CD_GUILD_TIME,					//���cd
	CD_TEAM_TIME,					//���cd
	CD_WORLD_NEW_TIME,				//С����cd
	CD_SCENE_TIME,					//����cd
	LIMIT_WORLD_LEVEL,              //����Ƶ���ȼ�����
};

//�������Ͷ���
enum
{
	//˽��
	CHATTYPE_WHISPER = 0,
	//����
	CHATTYPE_WORLD,
	//ϵͳ 
	CHATTYPE_SYSTEM,
	// ���
	CHATTYPE_GUILD,
	// ����
	CHATTYPE_TEAM,
	// �������
	CHATTYPE_TEAM_INVITE,
	// С����
	CHATTYPE_WORLD_NEW,

	CHATTYPE_MAX,
};

//�������ݵ����Ͷ���
enum
{
	//��������
	CHAT_CONTENT_TYPE_WORD = 0,
	//��������
	CHAT_CONTENT_TYPE_VOICE = 1,
};

class ChannelModule;
//class GuildModule;
class LogModule;
class ContainerModule;
class SnsDataModule;
class CoolDownModule;
//class VipModule;

class ChatModule : public ILogicModule
{
public:
	// ��ʼ��
	virtual bool Init(IKernel* pKernel);

	// �ͷ�
	virtual bool Shut(IKernel* pKernel);
public:

	// ����ָ����ҽ��� ������ʱ�䵥λΪ����
	bool SetSilence(IKernel* pKernel, const PERSISTID& self, int silenceTime);

public:
	// �������������Ϣ
	bool LoadResource(IKernel* pKernel);
	static void ReloadResource(IKernel* pKernel);
	// �ͷ���Դ��Ϣ
	bool ReleaseRes();

	//��ȡ������ض����ļ�
	bool LoadCheckWords(IKernel* pKernel);

    // ����Ƿ������֣�����Url
    bool CheckBadWordsIgnoreUrl(std::wstring& strInfo);

	// ����Ƿ�������
	bool CheckBadWords(std::wstring& strInfo);

	/// \brief ����
	static int OnRecover(IKernel* pKernel, const PERSISTID& self, 
		const PERSISTID& sender, const IVarList& args);

	// ��������
	static int OnContinue(IKernel* pKernel, const PERSISTID& self,
		const PERSISTID& sender, const IVarList& args);

	// ������Ϣ����������Ƶ��
	int ChatToGuild(IKernel* pKernel, const PERSISTID& self, IVarList& args);

	// ������Ϣ���������Ƶ��ͨ�������
	int ChatToGuild(IKernel* pKernel, const wchar_t* guildName, IVarList& args);

	// ����������Ϣ
//	int InteractToScene(IKernel* pKernel, const PERSISTID& self, const PERSISTID& skill);

	//��������Ϊ����
	static int ChatContentTypeByWord(IKernel* pKernel, const PERSISTID& self, const IVarList& args);
private:
	// �Զ�����Ӧ����
	static int OnCustomChat(IKernel* pKernel, const PERSISTID& self, 
		const PERSISTID& sender, const IVarList& args);

	// ������ز�ѯ
	static int OnCustomQuery(IKernel* pKernel, const PERSISTID& self, 
		const PERSISTID& sender, const IVarList& args);

	// �糡��������Ϣ
	static int OnCommandChat(IKernel* pKernel, const PERSISTID& self,
		const PERSISTID& sender, const IVarList& args);

	//��������Ϊ����
	static int ChatContentTypeByVoice(IKernel* pKernel, const PERSISTID& self, const IVarList& args);

	//˽��
	static void OnChatChit( IKernel* pKernel, const PERSISTID& self, 
		const wchar_t* targetname, const wchar_t* content);

	// ����
	static void OnChatWorld(IKernel* pKernel, const PERSISTID& self, 
		const wchar_t* content);
	// new
	static void OnChatWorldNew(IKernel* pKernel, const PERSISTID& self, 
		const wchar_t* content);

	// ϵͳ
	static void OnChatSystem(IKernel* pKernel, const PERSISTID& self, 
		const IVarList& args);

	// ���
// 	static void OnChatGuild(IKernel* pKernel, const PERSISTID& self, 
// 		const wchar_t* content);

	// ����
	static void OnChatTeam(IKernel* pKernel, const PERSISTID& self, 
		const wchar_t* content);

	static void OnChatTeamInvite(IKernel* pKernel, const PERSISTID& self, 
		const wchar_t* content);

	//����˽��
	static void OnChatVoiceChit(IKernel* pKernel, const PERSISTID& self,
		const wchar_t* targetname, const char* msg_id, int msg_time);

	// ��������
	static void OnChatVoiceWorld(IKernel* pKernel, const PERSISTID& self,
		const char* msg_id, int msg_time);

	// �������
// 	static void OnChatVoiceGuild(IKernel* pKernel, const PERSISTID& self,
// 		const char* msg_id, int msg_time);
// 
// 	// ��������
// 	static void OnChatVoiceTeam(IKernel* pKernel, const PERSISTID& self,
// 		const char* msg_id, int msg_time);

	// ����cd�ж�
	static bool CheckChannelInCD(IKernel* pKernel, const PERSISTID& self,
		const int channel);

	//�ж�����Ƿ�����
	static bool CheckPlayerOnline(IKernel* pKernel, const PERSISTID& self,
		const wchar_t* targetName, bool bTipCustom = true);

	//�ж�����Ƿ�����
	static bool CheckPlayerOnline(IKernel* pKernel, const wchar_t* name);

	//�����Ϣ �ֶ�
	static bool SetChatInfo( IKernel* pKernel, const PERSISTID& self, CVarList& var);

	//���������
	static bool CheckWhisper(IKernel* pKernel, const PERSISTID& self,
		const wchar_t* targetName);

	//��������
	static int H_Silence(IKernel* pKernel, const PERSISTID& self, int slice);

    // ���ͨ������չʾ�Լ��ĵ���(װ������Ʒ��), ��������������ȡ������Ϣ, 
    static int ExtractItemInfo(IKernel* pKernel, const PERSISTID& self,
        const wchar_t *content);

    // ��ȡչʾ��װ����Ϣ������� @items_info ��
    // @items_info �Ĵ�Ÿ�ʽ�� [unique_id][prop..]...[unique_id][prop...]
    static bool InnerQueryItemInfo(IKernel* pKernel, const PERSISTID& self,
        const IVarList& items_list, CVarList& items_info);
        
    // ����չʾ����Ʒ
    static bool SaveShowoffItem(IKernel* pKernel, const PERSISTID& self,
        const CVarList& items_info);

	//����������Ϣ
	static bool SaveChatInfo(IKernel *pKernel, const int channel, 
						const PERSISTID& self, const std::wstring &content, int subType = -1);

public:
	static ChatModule* m_pChatModule;
	std::vector<std::string> m_chatParam;
	std::vector<std::wstring> m_chatCheckWords;
	std::vector<int> m_chatSaveChannel;	//��Ҫ������Ϣ��Ƶ��
	std::wstring m_strWord;

private:
	
	static ContainerModule* m_pContainerModule;
	static CoolDownModule* m_pCDModule;		// ��ȴģ��
	//static VipModule* m_pVipModule;			// vipģ��

#ifndef FSROOMLOGIC_EXPORTS
	static SnsDataModule *m_pSnsDataModule;
	static ChannelModule* m_pChannelModule;
	//static GuildModule* m_pGuildModule;
	static LogModule* m_pLogModule;
#endif // FSROOMLOGIC_EXPORTS
};

#endif // _ChatModule_H_
