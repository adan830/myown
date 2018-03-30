//--------------------------------------------------------------------
// �ļ���:		Domain_Team.h
// �� ��:		    �������ϵͳ
// ˵ ��:		
// ��������:	2014��11��19��
// ������:		 
//    :	    
//--------------------------------------------------------------------

#ifndef _Domain_Team_h_
#define _Domain_Team_h_

#include "IDomainData.h"
#include "public/IVarList.h"
#include "public/IVar.h"
#include <string>
#include <list>
class Domain_Team : public IDomainData
{
public:
    Domain_Team() : IDomainData(L"")
    {
    }

    virtual ~Domain_Team()
    {
    }

public:
    //�Ƿ񱣴浽���ݿ�
    virtual bool GetSave(){return false;}

	// ������������
	virtual const std::wstring & GetDomainName(IPubKernel* pPubKernel);

    virtual int OnCreate(IPubKernel* pPubKernel, IPubSpace * pPubSpace);

    virtual int OnLoad(IPubKernel* pPubKernel, IPubSpace * pPubSpace);

    /// \brief ���յ���������������Ϣ
    /// \param source_id ����Ϣ�ķ�������ʶ
    /// \param msg ��Ϣ����
    virtual int OnMessage(IPubKernel* pPubKernel, IPubSpace * pPubSpace,
        int source_id, int scene_id, const IVarList& msg);

	// ȡ��ָ������ID�ĳ�Ա�б�
	static void GetTeamMemsByTeamID(IPubKernel* pPubKernel, IPubSpace * pPubSpace,
		const int team_id, IVarList& mem_list);

private:
    //��������, ���ض���ţ�-1ʧ��
    int CreateTeam(IPubKernel* pPubKernel, IPubData * pTeamData, int source_id, int scene_id, 
        const wchar_t * captain_name, const wchar_t * member_name,const IVarList &args);

    //ɾ������
    bool DestroyTeam(IPubKernel* pPubKernel, IPubData * pTeamData, int source_id, int scene_id, 
        int nTeamID, const wchar_t * player_name);

    //�������
    bool JoinTeam(IPubKernel* pPubKernel, IPubData * pTeamData, int source_id, int scene_id, 
        int nTeamID, const wchar_t * player_name,int clone_id);
	bool JoinTeam(IPubKernel *pPubKernel,IRecord *pMainRec, IPubData * pTeamData, const wchar_t * playerName, int teamRow);
    //�˳�����(reason: 0�Լ��뿪��1���ӳ����� 2����)
    bool QuitTeam(IPubKernel* pPubKernel, IPubData * pTeamData, int source_id, int scene_id, 
        int nTeamID, const wchar_t * player_name, int reason,int64_t lastleavetime = 0,int needNewListen = 0);
	//�������
	bool OnStore(IPubKernel* pPubKernel, IPubData * pTeamData, int source_id, int scene_id, const wchar_t * player_name);

    //���³�Ա����
    bool UpdataMemberInfo(IPubKernel* pPubKernel, IPubData * pTeamData, int source_id, int scene_id, 
        int nTeamID, const wchar_t * player_name, int col, const IVar & value);

    //���¶�������
    bool UpdataTeamInfo(IPubKernel* pPubKernel, IPubData * pTeamData, int source_id, int scene_id, 
        int nTeamID, const wchar_t * player_name, int col, const IVar & value);

    //�ж��Ƿ��Ƕ����е����
    bool CheckIsTeamMember(IPubKernel* pPubKernel, IPubData * pTeamData, int source_id, int scene_id,
        const int team_id,const wchar_t* player_name);

    //���ö���ְλ
    bool SetTeamMember(IPubKernel* pPubKernel, IPubData * pTeamData, int source_id, int scene_id,
        int nTeamID,int nCol, int value, const wchar_t* name);
	//��Ҽ���ƥ��
	void PlayerAddMatch(IPubKernel* pPubKernel, const IVarList &value);
	//����뿪ƥ��
	void PlayerLeaveMatch(IPubKernel* pPubKernel, const IVarList &value);
	//�Զ�ƥ��
	 static int  Team_Auto_Match(IPubKernel *pPubKernel, const char *space_name, const wchar_t *data_name, int time);
	 //����ƥ���еĶ�����Ϣ
	 void SendMatchTeamInfo(IPubKernel* pPubKernel, IPubData * pTeamData,const wchar_t*name);

private:
    //�����µĶ���ţ�����-1ʧ��
    int GetTeamID();

    //��������ΨһID
    std::string GetUniqueTeamID();

    //��ö����Ա������
    std::string GetTeamMemberRecName(int nTeamID);
	struct Match_Player_Info
	{
		std::wstring playerName;
		int playerLevel;
		int index; // Ŀ��
	};
	static Domain_Team* _this_team;
	static std::list<Match_Player_Info> match_apply;
	

};

#endif //_Domain_Team_h_