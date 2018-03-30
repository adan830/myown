//--------------------------------------------------------------------
// �ļ���:		PlayerBaseModule.h
// ��  ��:		��һ�������
// ˵  ��:		
// ��������:	2014��10��17��
// ������:		 
//    :	   
//--------------------------------------------------------------------
#ifndef __PlayerBaseModule_H__
#define __PlayerBaseModule_H__

#include "Fsgame/Define/header.h"
#include <vector>
#include "Define/PlayerBaseDefine.h"
#include <set>

// ��Ұ汾�Ŷ�Ӧ��������
enum PlayerVersion
{
	PLAYER_VERSION_MIN          = 0,  // ��С�汾��


	PLAYER_VERSION_MAX,               // ���汾��    
};

//�ͻ�����ʾ�ı�
//const char * const CROSSBATTLE_MATCHBATTLE_NO_EXIT_ROLE = "CrossBattle_MatchBattle_No_Exit_Role";  //��ǰ�ڿ��ƥ���в���С��

// ��������汾�Ź��ܺ�������
typedef bool (__cdecl* PLAYER_VERSION_FUNC)(IKernel *pKernel, const PERSISTID &self, const IVarList& args);

class PlayerBaseModule: public ILogicModule 
{
public:
	// ��ʼ��
	virtual bool Init(IKernel* pKernel);

	// �ر�
	virtual bool Shut(IKernel* pKernel);

public:
	//����
	static int OnCreateClass(IKernel* pKernel, int index);
	/// \brief ����
	static int OnRecover(IKernel* pKernel, const PERSISTID& self, 
		const PERSISTID& sender, const IVarList& args);

	/// \brief ��������
	static int OnContinue(IKernel* pKernel, const PERSISTID& self, 
		const PERSISTID& sender, const IVarList& args);

	/// \brief ����
	static int OnEntry(IKernel* pKernel, const PERSISTID& self, 
		const PERSISTID& sender, const IVarList& args);

	/// \brief ������Ϸ
	static int OnEntryGame(IKernel* pKernel, const PERSISTID& self, 
		const PERSISTID& sender, const IVarList& args);

	/// \brief ����
	static int OnStore(IKernel* pKernel, const PERSISTID& self, 
		const PERSISTID& sender, const IVarList& args);

	/// \brief ����
	static int OnStore2(IKernel* pKernel, const PERSISTID& self,
		const PERSISTID& sender, const IVarList& args);

	/// \brief �뿪
	static int OnLeave(IKernel* pKernel, const PERSISTID& self,
		const PERSISTID& sender, const IVarList& args);

	static int OnDisconnect(IKernel* pKernel, const PERSISTID& self, 
		const PERSISTID& sender, const IVarList& args);

	//ѡ�����
	static int OnPlayerSelect(IKernel* pKernel, const PERSISTID& self,
		const PERSISTID& sender, const IVarList& args);

	//��ȡ������ʱ���
	static int OnCustomGetServerTime(IKernel* pKernel, const PERSISTID& self,
		const PERSISTID& sender, const IVarList& args);

	// ��ȡ�������
	static int OnCustomGetPlayerPropData(IKernel* pKernel, const PERSISTID& self,
							const PERSISTID& sender, const IVarList& args);

	// ��С�ˣ����ؽ�ɫѡ��
	static int OnCheckedSelectRole(IKernel* pKernel,const PERSISTID& self,
		const PERSISTID& player, const IVarList& args);

	//��ȡ�豸��
	static int GetToken(IKernel* pKernel, const PERSISTID& self,
		const PERSISTID& sender, const IVarList& args);

	//�ͻ�������������Ļ�����������
	static int OnCustomSetVisualPlayers(IKernel* pKernel, const PERSISTID& self,
		const PERSISTID& sender, const IVarList& args);

	// ��Ӧ���˺��¼�
	static int OnCommandBeDamage(IKernel* pKernel, const PERSISTID& self,
		const PERSISTID& sender, const IVarList& args);

	// �����ɫ
	static int OnCommandBlockPlayer(IKernel* pKernel, const PERSISTID& self,
		const PERSISTID& sender, const IVarList& args);
	// �ͻ��˲�ѯ����
	static int OnCommandClientQueryPropData(IKernel* pKernel, const PERSISTID& self,
		const PERSISTID& sender, const IVarList& args);

	// ������
	static int CB_ForceOffline(IKernel* pKernel, const PERSISTID& self, int time);

	// ����������ҽ�ɫ timeΪ��ֹʱ��
	static bool BlockPlayer(IKernel* pKernel, const PERSISTID& self, int64_t time);
	// �������ɫ time=0 ��� timeΪ��ֹʱ��
	static bool BlockPlayer(IKernel* pKernel, const wchar_t* playerName, int64_t time);

    // 0����µ�¼ʱ��
	static int ResetLoginTimeTimer(IKernel* pKernel, const PERSISTID& self, int slice);

	// �ͻ�����Ϣʵ�����
	static int OnCustomCheckStrength(IKernel* pKernel, const PERSISTID& self,
		const PERSISTID& sender, const IVarList& args);

	// �ͻ��˿�ʼ(����)��������
	static int OnCustomChangeSpeed(IKernel* pKernel, const PERSISTID& self,
		const PERSISTID& sender, const IVarList& args);

    // �ͻ���ת����̨
    static int OnClientBackgroundTask(IKernel *pKernel, const PERSISTID &self, const PERSISTID &sender, const IVarList &args);

	// �Ƿ��ǻ������ж�
	static bool IsRobot(IKernel *pKernel, const PERSISTID &player);

	// ����ȫ�ֱ�־
// 	static void AddPlayerGlobalFlag(IKernel *pKernel, const PERSISTID &player, EPlayerGlobalFlag flag);
// 	// �Ƴ�ȫ�ֱ�־
// 	static void RemovePlayerGlobalFlag(IKernel *pKernel, const PERSISTID &player, EPlayerGlobalFlag flag);
// 	// �������б�־
// 	static void SetPlayerGlobalFlag(IKernel *pKernel, const PERSISTID &player, int nValue );
// 	// ���Ա�־״̬
// 	static bool TestPlayerGlobalFlag(IKernel *pKernel, const PERSISTID &player, EPlayerGlobalFlag flag);
// 	// ���Ա�־����
// 	static bool TestPlayerGlobalFlags(IKernel* pKernel, const PERSISTID &player, int64_t nValue);
// 	// ���Ա�־״ֵ̬
// 	static bool TestPlayerGlobalValue(IKernel *pKernel, const PERSISTID &player, int64_t nValue);

private:
	// ��������uid
	void UpdateAccountUID(IKernel* pKernel, const PERSISTID& self);

	// ע�������汾�ź���
	static bool RegisterVersionFun(PlayerVersion version, PLAYER_VERSION_FUNC fun);

	// �����ѯ�������
	void HandlerPlayerPropQuery(IKernel* pKernel, const wchar_t* pszSender, IGameObj& player, const IVarList& query, int nOffset, int nUID);
public:
	// ������Ұ汾�������߼�
	static bool HandlePlayerVersion(IKernel *pKernel, const PERSISTID &self, const IVarList& args);

private:

public:
	static PlayerBaseModule * m_pPlayerBaseModule;

	//�жϺ�����ַ��
	static std::vector<PLAYER_VERSION_FUNC> m_funList;

	// �ͻ��˿�ͨ��������ֱ�Ӳ�ѯ�����Լ���
	std::set<std::string>	m_propPubset;
};

#endif // __PlayerBaseModule_H__
