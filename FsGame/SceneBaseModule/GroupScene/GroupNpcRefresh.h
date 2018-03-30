//--------------------------------------------------------------------
// �ļ���:      GroupNpcRefresh.h
// ��  ��:      ���鳡����������ˢ�¹���
// ˵  ��:
// ��������:    2015��8��4��
// ������:       
// �޸���:        
//    :       
//--------------------------------------------------------------------
#ifndef _GROUP_NPC_REFRESH_H_
#define _GROUP_NPC_REFRESH_H_

#include "Fsgame/Define/header.h"
#include "FsGame/Define/GroupSceneDefine.h"
#include <vector>
#include <map>
#include "Fsgame/Define/FightDefine.h"

typedef bool (__cdecl* GROUP_SCENE_REFRESH_FUNC)(IKernel* pKernel, const PERSISTID& self, const IVarList& args);

class NpcCreatorModule;
class SecretSceneModule;
class TeamSceneModule;
class GroupNpcRefresh : public ILogicModule
{
public:
    //��ʼ��
    virtual bool Init(IKernel* pKernel);
    //�ر�
    virtual bool Shut(IKernel* pKernel);
public:
    //��¼Ұ������
    static int RecordNpc(IKernel* pKernel,const PERSISTID& npc, const PERSISTID& player, const IVarList& args);
    //ˢ��BOSS
    bool RefreshBoss(IKernel * pKernel ,const PERSISTID& self, int sceneID, int killComm, int killElite);
    //����BOSS
    void CreateBoss(IKernel* pKernel, const PERSISTID& self);
    //BOSS������
    static int AttackBoss(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);
    //BOSS����ɱ
    static int KillBoss(IKernel* pKernel, const PERSISTID& player, const PERSISTID& boss, int sceneID);
    //ˢBOSS����
    int RefreshBossType(int sceneID);
    //ɱ������׼
    int GetKillNpcAmount(int sceneID);
    //��������
    bool LoadResource(IKernel* pKernel);
	//ֹͣ�����������obj��Ϊ
	void StopGroupObj(IKernel *pKernel, int groupID, int waitTime = CONTROL_BUFF_FOREVER);
	//��ʼ���������obj��Ϊ
	void StartGroupObj(IKernel *pKernel, int groupID, int buffTimeType = CONTROL_BUFF_TIMES);
	//(�ӳ�3���ֹ��npc��Ϣû�д�����)�������npc
	void DelayClearGroupNpc(IKernel*pKernel,int groupID);

private:
    //ע��ˢ�¹���
    static bool RegisterRefreshRule(GROUP_SCENE_TYPE sceneType, GROUP_SCENE_REFRESH_FUNC func);

    //�ؿ��ؾ�����
    static bool CommonSecretRule(IKernel* pKernel, const PERSISTID& self, const IVarList& args);
    //�Խ�ͤ����
    static bool TreasureSecretRule(IKernel* pKernel, const PERSISTID& self, const IVarList& args);
    //��ӻ��������
    static bool TeamActivityRule(IKernel* pKernel, const PERSISTID& self, const IVarList& args);
    //�����ؾ�����
    static bool PetSecretRule(IKernel* pKernel, const PERSISTID& self, const IVarList& args);
    //�콫ͭǮ����
    static bool MoneySecretRule(IKernel* pKernel, const PERSISTID& self, const IVarList& args);
    //�������ݹ���
    static bool ItemSecretRule(IKernel* pKernel, const PERSISTID& self, const IVarList& args);
	//��������ؾ�ʱ��
	static int HB_ClearGroupNpc(IKernel* pKernel, const PERSISTID& self, int silce);

private:
    static std::vector<GROUP_SCENE_REFRESH_FUNC> v_funcList;
private:
    struct GroupNpcInfo 
    {
        GroupNpcInfo():sceneID(0), refreshBossType(0), killAmount(0), 
            bossConfigID(""), bossPosi(""){};
        int sceneID;
        int refreshBossType;
        int killAmount;
        std::string bossConfigID;
        std::string bossPosi;
    };
    std::map<int, GroupNpcInfo> m_mapGroupNpcConfig;
public:
    static GroupNpcRefresh*     m_pGroupNpcRefresh;
private:
    static NpcCreatorModule*    m_pNpcCreatorModule;

#ifndef FSROOMLOGIC_EXPORTS
    static SecretSceneModule*   m_pSecretSceneModule;
    static TeamSceneModule*     m_pTeamSceneModule;
#endif // FSROOMLOGIC_EXPORTS
};

#endif