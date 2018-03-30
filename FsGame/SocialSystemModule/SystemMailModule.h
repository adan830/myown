//----------------------------------------------------------
// �ļ���:      SystemMailModule.h
// ��  ��:      �ʼ�ϵͳ
// ˵  ��:
// ������:		  
// �޸���:        ( )
// ��������:    
//    :       
//----------------------------------------------------------

#ifndef FSGAME_SocialSystemModule_SYSTEM_MAIL_MODULE_H_
#define FSGAME_SocialSystemModule_SYSTEM_MAIL_MODULE_H_

#include <map>
#include "Fsgame/Define/header.h"
#include "FsGame/Define/FunctionEventDefine.h"
#include <vector>
class CapitalModule;

// Ԥ�����õ��ʼ�
struct ConfigMailItem
{
	ConfigMailItem(): sender(L""), mail_title(L""), mail_content(L""), items(""){}
	std::wstring		sender;
	std::wstring        mail_title;
	std::wstring        mail_content;
	std::string			items;
};

typedef std::map<std::string, ConfigMailItem> ConfigMailMap;

// �ʼ�ϵͳ����
struct SystemMailConfig
{
	SystemMailConfig(): mail_appendix_life(0), mail_no_appendix_life(0){}
	int mail_appendix_life;		// �и������ʼ�����ʱ��(��λ����)
	int mail_no_appendix_life;	// �޸������ʼ�����ʱ��(��λ����)


	void LoadConfig(IKernel* pKernel);
};

enum 
{
	AUTO_SEND_MAIL_BY_TASK,
	AUTO_SEND_MAIL_BY_LEVEL,
	AUTO_SEND_MAIL_MAX,
};


struct AutoSendMail
{
	int m_conditionValue{0};
	std::string m_mailName;
};


// ϵͳ�ʼ�ģ��
class SystemMailModule : public ILogicModule
{
public:
    //��ʼ��
    virtual bool Init(IKernel* pKernel);
    //�ͷ�
    virtual bool Shut(IKernel* pKernel);

	static const std::wstring & GetDomainName(IKernel * pKernel);
    // ��ҽ�����Ϸ
    static int OnRecover(IKernel* pKernel, const PERSISTID& self,
						const PERSISTID& sender, const IVarList& args);

    // �������
    static int OnDisconnect(IKernel* pKernel, const PERSISTID& self,
						const PERSISTID& sender, const IVarList& args);

    //���׼��
    static int OnReady(IKernel* pKernel, const PERSISTID& self, 
						const PERSISTID& sender, const IVarList& args);

	// �õ�Ԥ�����õ��ʼ�
	static const ConfigMailItem& GetConfigMail(IKernel* pKernel, const std::string& mail_id);

	// ƴ��mail���ַ���
	static const std::wstring ComposeMailContent(IKernel* pKernel, const wchar_t* content, const IVarList &var);

	// ����ϵͳ�ʼ���ĳ����ɫ
    // source�����ߣ���ͬ��ϵͳ��������Ϊ�գ�
    // name ����������
    // title��content�ʼ����⼰����
    // addin_param������Ʒ�б��ʽ: �ʽ���:����,...;����ID:��������,...
    // ����:  CapitalCopper:100,CapitalGold:50;Item001:10,Equip002:2
	static bool SendMailToPlayer(IKernel* pKernel, const wchar_t* source, const wchar_t* name, 
				const wchar_t* title, const wchar_t* content, const std::string& addin_param, 
				EmFunctionEventId nMailFrom = FUNCTION_EVENT_ID_SYS);
	// �����ʼ�ģ�巢��ϵͳ�ʼ�
	// name		����������
	// mailid	�ʼ�ģ��
	// param	ģ�����
	static bool SendMailToPlayer(IKernel* pKernel, const char* mailid, const wchar_t* name, const IVarList& param, 
				const std::string& addin_param = std::string(),
				EmFunctionEventId nMailFrom = FUNCTION_EVENT_ID_SYS);

	// ����ϵͳ�ʼ���ĳ���˺�
    // source�����ߣ���ͬ��ϵͳ��������Ϊ�գ�
    // name ����������
    // title��content�ʼ����⼰����
    // addin_param������Ʒ�б��ʽ: �ʽ���:����,...;����ID:��������,...
    // ����:  CapitalCopper:100,CapitalGold:50;Item001:10,Equip002:2
	static bool SendMailToAccount(IKernel* pKernel, const wchar_t* source, const char* account, 
								const wchar_t* title, const wchar_t* content, const std::string& addin_param,
								EmFunctionEventId nMailFrom = FUNCTION_EVENT_ID_SYS);


	// �ͻ�����Ϣ������
	static int OnCustomClientMessage(IKernel* pKernel, const PERSISTID& self,
								const PERSISTID& sender, const IVarList& args);
	
    // �ʼ�����
    static int OnCustomMailCount(IKernel* pKernel, const PERSISTID& self,
        const PERSISTID& sender, const IVarList& args);
        
    // ��ѯ�ʼ��б�
    static int OnCustomRetrieve(IKernel* pKernel, const PERSISTID& self,
        const PERSISTID& sender, const IVarList& args);
	
	//��ȡ�ʼ�
	static int OnCustomReadMail(IKernel* pKernel, const PERSISTID& self,
								const PERSISTID& sender, const IVarList& args);
	// ɾ���ʼ�
	static int OnCustomDeleteMail(IKernel* pKernel, const PERSISTID& self,
        const PERSISTID& sender, const IVarList& args);
        
    // ɾ��ȫ���ʼ�
    static int OnCustomDeleteAll(IKernel* pKernel, const PERSISTID& self,
        const PERSISTID& sender, const IVarList& args);
        
	// ��ȡ����
	static int OnCustomPickMail(IKernel* pKernel, const PERSISTID& self,
        const PERSISTID& sender, const IVarList& args);

    // ��ȡȫ������
    static int OnCustomPickAll(IKernel* pKernel, const PERSISTID& self,
        const PERSISTID& sender, const IVarList& args);

    // ��ȡ�ʼ�
    static int InnerPickMail(IKernel* pKernel, const PERSISTID& self, const PERSISTID& toolbox, const IVarList& args);

	static bool ClearAllMail(IKernel* pKernel, const PERSISTID& self);
	static int SendTestMail(IKernel* pKernel, const PERSISTID& self,std::string &mailID);

	static void LoadResource(IKernel* pKernel);
	static bool LoadAutoSendMailCfg(IKernel *pKernel);

	// ���͹㲥ȫ���ʼ�
	static bool SendGlobalMail(IKernel* pKernel, 
								const wchar_t* pwszSender, 
								const wchar_t* pwszTitle, 
								const wchar_t* pwszContent, 
								const char* pszAttachment, 
								int nFrom, 
								int64_t nOriMailId = 0,
								int64_t nCreateTime = 0,
								int64_t nExperidTime = 0 );
	static void RecvGlobalMail(IKernel* pKernel, const PERSISTID& player);

	//���񼤻��
	//static int OnCommandTask(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);
	//�ȼ������
	//static int OnCommandLevel(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);
	//
	//static void SystemGlobalMail(IKernel*pKernel, const char* mailName, const IVarList& args, int nNation, int lifeTime = 604800);
	//����ȫ��
	//static int OnPlayerRecoreGlobaleMail(IKernel*pKernel, const PERSISTID& self);

	// �ڲ�����ص�
// 	static int OnCommandGuild(IKernel* pKernel, const PERSISTID& self,
// 		const PERSISTID& sender, const IVarList& args);


private:
	static IRecord *GetMailRecord(IKernel* pKernel, const PERSISTID& self);
		
	static int OnQueryLetter(IKernel* pKernel, const PERSISTID& self,const PERSISTID& sender, const IVarList& args);
	static int OnLookLetter(IKernel* pKernel, const PERSISTID& self,const PERSISTID& sender, const IVarList& args);
	
	//�յ���Ϣ��ѯ�ż���Ϣ
	static int OnQuerySelfLetter(IKernel* pKernel, const PERSISTID& self,const PERSISTID& sender, const IVarList& args);

	//������������
	static int H_Check_Letter(IKernel* pKernel, const PERSISTID& self, int slice);
	// ��ģ���ʱ��
	static int HB_TimeTick(IKernel* pKernel, const PERSISTID& self, int time);

	static int64_t _dayToSeconds(int day);
    static void ParseAppendix(const std::wstring& appendix, std::string &capital, std::string &item);

	static bool SetObjProperty(IKernel* pKernel, const PERSISTID& self, const IVarList& args);

    // ���δ���ʼ�
    static void CheckUnreadMail(IKernel* pKernel, const PERSISTID& self);
private:
	static CapitalModule* m_CapitalModuel;

	static ConfigMailMap m_ConfigMails;

	static SystemMailConfig m_SysMailConfig;

	static std::map<int, std::vector<AutoSendMail> > m_autoSendMailConfig;

	static std::wstring m_domainName;
};

#endif //