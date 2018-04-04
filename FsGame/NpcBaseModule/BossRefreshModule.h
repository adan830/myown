//--------------------------------------------------------------------
// �ļ���:      BossRefreshModule.h
// ��  ��:      Bossˢ��ģ��
// ˵  ��:		
// ��������:    2016��6��28��
// ������:      ������
// �޸���:        
//    :       
//--------------------------------------------------------------------

#ifndef __BossRefreshModule_H__
#define __BossRefreshModule_H__

#include "Fsgame/Define/header.h"
#include <vector>
#include <map>
#include "CommonModule/CommRuleDefine.h"

#define SCENE_BOSS_REC "scene_boss_rec"    //����BOSS��Ϣ��
enum
{
	SCENE_BOSS_CONFIG = 0,         //boss���
	SCENE_BOSS_OBJ = 1,            //boss����
    SCENE_BOSS_POS_X,
    SCENE_BOSS_POS_Z,
	SCENE_BOSS_POS_ORIENT,
};

enum
{
	/*!
	* @brief	boss״̬����
	* @param	int n Boss����
	����ѭ��n��
	* @param	string	bossid
	* @param	int		ˢ�µ���ʱ(1��ʾ��ˢ��)
	* @param	int		����id
	* @param	wstring	��ɱ���
	*/
    SC_SUBMSG_QUERY_BOSS_IN_SCENE = 1,      //��ѯBOSS��Ϣ  

	/*!
	* @brief	boss״̬�ı�
	* @param	string	bossid
	* @param	int		״̬ 
	*/
	SC_SUBMSG_BOSS_STATE_CHANGE,
};

enum
{
    UPDATE_BOSS_AMOUNT_ADD = 0,
    UPDATE_BOSS_AMOUNT_DEL = 1,
};

using namespace std;
class SceneInfoModule;
class NpcCreatorModule;
class AsynCtrlModule;
class BossRefreshModule: public ILogicModule  
{
public:
	// ��ʼ��
	virtual bool Init(IKernel* pKernel);

	// �ر�
	virtual bool Shut(IKernel* pKernel);

	// �Ƿ�Ϊˢboss����
	bool IsRefreshBossScene(IKernel* pKernel);

	// ��ѯĳ��������Ұ��boss���� outData ��ʽ:outData << bossid << lefttime << bossid << lefttime..
	bool QueryBossInfo(IKernel * pKernel, int nSceneId, IVarList& outData);
private:
    //boss������Ϣ
    struct BossInfo
    {
        PosInfo BossPos;
		std::string sBossConfig;
		int nInterval;	//��С���
    };
public:
	//��������
	bool LoadResource(IKernel* pKernel);

	//��������
	static int OnCreate(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);

    //NPC���볡��
    static int OnBossEntry(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);

    //NPC����
    static int OnCommandBossBeKilled(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);

	//��������BOSS����
	static int HB_BossCreate(IKernel* pKernel, const PERSISTID& self,int silce);

    //����ͻ�����Ϣ
    static int OnCustomQuerySceneBoss(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);

	// ��ҹ���˳�����
	static int OnCustomQuitGroup(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);

    //��ȡ����������BOSS��Ϣ��
    IRecord * GetPubBossInSceneRec(IKernel * pKernel, const char* strRecName);

	// ����boss״̬�ı�
	void SendBossStateChangeMsg(IKernel * pKernel, const char* strBossId, int nState);
private:
	//����ˢ����Ϣ
	struct SceneRefreshInfo
	{
		SceneRefreshInfo() : nOpen(0), beginTime(0),
			endTime(0)
		{
			VecSceneBossInfo.clear();
		};
		int nOpen; //�Ƿ񿪷�ʱ����ڲ�ˢ��
		int beginTime;	//����ʱ��
		int endTime;	//����ʱ��
		//int nInterval;	//��С���
		vector<BossInfo> VecSceneBossInfo;
	};

	typedef map<int, SceneRefreshInfo> SceneInfoMap;

	// ��ѯboss��ˢ�¼��
	int QueryBossInterval(const SceneRefreshInfo& sceneInfo, const char* strBoss);

    //��ѯ������BOSS��Ϣ
    int QueryBossInfoInScene(IKernel* pKernel, const PERSISTID& self);

	// ��ѯboss����
	const BossInfo* FindBossInfo(IKernel* pKernel, const string& strBossConfig);

	//����boss
	void CreateBoss(IKernel* pKernel, const char* strBossConfig, IRecord* pBossRecord, int nRowIndex);

	// ��ȡ����boss����
	bool LoadSceneBoss(IKernel* pKernel);

    //�Ƿ�ͳ�ƴ˳�����BOSS����
    bool ContainSceneID(int sceneID);

    //�Ƿ��ڲ�ˢ��ʱ�����
    bool NotRefresh(int beginTime, int endTime);

	// ���¼��ؼ�������
	static void ReloadConfig(IKernel* pKernel);

	// ��ȡ������������
	static const std::wstring & GetDomainName(IKernel * pKernel);
public:
	static BossRefreshModule * m_pBossRefreshModule;
private:
    static NpcCreatorModule * m_pNpcCreatorModule;
    static AsynCtrlModule * m_pAsynCtrlModule;
private:
	SceneInfoMap	m_MapSceneBossInfo;
	static std::wstring	m_domainName;
};

#endif // __BossRefreshModule_H__
