//--------------------------------------------------------------------
// �ļ���:      NpcCreatorModule.h
// ��  ��:      NPC����
// ˵  ��:		
// ��������:    2014��10��17��
// ������:        
// �޸���:        
//    :       
//--------------------------------------------------------------------
#ifndef _NPCCREATE_MODULE_H_
#define _NPCCREATE_MODULE_H_

#include "Fsgame/Define/header.h"
#include <string>
#include <map>
#include <vector>
#include "utils/util_ini.h"
#include "utils/util_func.h"

#define MAX_HERAT_BEAT_TIME (500)

#define GROUP_STATIC_CREATE_FLOW "create_group_static_flow"
#define GROUP_RANDOM_CREATE_FLOW "create_group_random_flow"
#define CONTROL_BUFFID            "Buff_MJ_001"
#define POSITION_INFO_REC "position_info_rec"       //Ұ�������ֲ���Ϣ��
#define CREATE_OBJECT_LIST_REC "object_list_rec"    //���������ɵĶ����б�
// ���鳡��NPC�������ȱ�����
enum
{
	CREATE_FLOW_SUB_SCENE_NO = 0,
	CREATE_FLOW_GROUP_ID,
	CREATE_FLOW_FILE_INDEX,
	CREATE_FLOW_ITEM_INDEX,
	CREATE_FLOW_SCENE_CFG_NAME,
};

// ��Ҫˢ�µĶ����
enum PositionInfoRec
{
	POSIINFOREC_COL_X = 0,					// ����
	POSIINFOREC_COL_Y,					// ����
	POSIINFOREC_COL_Z,					// ����
	POSIINFOREC_COL_O,					// ����
	POSIINFOREC_COL_USED,				// �Ƿ��Ѿ���ʹ��
	POSIINFOREC_COL_TIME,				// �ϴ�����ʱ��
	POSIINFOREC_COL_R,					// ����뾶
	POSIINFOREC_COL_COUNT
};

// ���������
enum RandomParamListRec
{
	RPL_CONFIG = 0,		// ����ı��
	RPL_REFRESH_NUM,	// ˢ������	
	RPL_REFRESH_TIME,	// ˢ��ʱ��
};
//������ˢ�¹��������
enum
{
    CREATE_NPC_TYPE_COMMON = 0,
    CREATE_NPC_TYPE_ELITE = 1,
    CREATE_NPC_TYPE_BOSS = 2,
    CREATE_NPC_TYPE_BOX = 3,
};

// ÿ���������������������ͨ���������
class NpcCreatorModule : public ILogicModule
{
	/*-----------------------------------ϵͳ�ص�����----------------------------------------*/


private:
private:


	struct NpcPosition
	{
		float m_x{.0f};
		float m_y{.0f};
		float m_z{.0f};
		float m_ay{.0f};
	};

	struct RandomCreator
	{
		int m_no{0};
		std::string m_npcID;
		int m_createNum{0};
		int m_refreshTime{0};
		int m_step{0};
		int m_limit{0};
		std::string m_desc;
		std::vector<NpcPosition> m_positionGroup;


	};

	struct CreateSpringNpcInfo
	{
		std::string m_no;
		NpcPosition m_springPosition;// ����������
		float m_maxScope{.0f};
		float m_minScope{.0f};
		float m_springRange{.0f};
		int m_createType{0};
		int m_totalStep{0};
		bool m_isDestroy{true};
		int m_sceneAction{0};
		int m_totalNpcNum{0};
		std::vector<RandomCreator> m_NpcGroup;
	};

	struct CreateStaticNpcInfo
	{

		NpcPosition m_position;
		std::string m_npcID;
		int m_groupID;
		int m_nation;


	};

	struct CreateSceneNpcInfo
	{
		std::vector<CreateSpringNpcInfo> m_createRandGroup;
		std::vector<CreateStaticNpcInfo> m_createStaticGroup;
	};








public:
	// ϵͳ��ʼ��
	virtual bool Init(IKernel* pKernel);

	virtual bool Shut(IKernel* pKernel);

	// �������������Ļص�
	static int OnRandomNpcCreatorCreateClass(IKernel* pKernel, int index);

	static int OnRandomNpcCreatorItemCreateClass(IKernel* pKernel, int index);

	static int OnRandomNpcCreatorSpring(IKernel* pKernel, const PERSISTID& creator,
		const PERSISTID& sender, const IVarList& args);

	static int OnRandomNpcCreatorEndSpring(IKernel* pKernel, const PERSISTID& creator,
		const PERSISTID& sender, const IVarList& args);

	static int OnNpcSpring(IKernel* pKernel, const PERSISTID& creator,
		const PERSISTID& sender, const IVarList& args);

	static int OnNpcEndSpring(IKernel* pKernel, const PERSISTID& creator,
		const PERSISTID& sender, const IVarList& args);

	// �����ص�
	static int OnSceneCreate(IKernel* pKernel, const PERSISTID& scene,
		const PERSISTID& sender, const IVarList& args);

	// Npc��ɾ��
	static int OnNpcDestroy(IKernel* pKernel, const PERSISTID& npc,
		const PERSISTID& sender, const IVarList& args);
	//���Իص�
	static int C_OnCurStepChanged(IKernel* pKernel, const PERSISTID& self, const char* property, const IVar& old);
	//���
	void CleanSceneCfgName(IKernel*pKernel, int groupID);



	/*----------------------------------��������----------------------------------------*/
public:
	// ����
	static int H_Refresh(IKernel* pKernel, const PERSISTID& creator, int slice);

	static int H_EndRefresh(IKernel* pKernel, const PERSISTID& creator, int slice);

	// �����зֲ�������̬npc������
	static int H_DelayCreateStaticNpc(IKernel* pKernel, const PERSISTID& creator, int slice);

	// �����зֲ�����random npc������
	static int H_DelayCreateRandomNpc(IKernel* pKernel, const PERSISTID& creator, int slice);

	// ���鳡���зֲ�������̬npc������
	static int H_GroupSceneCreateStaticNpc(IKernel* pKernel, const PERSISTID& creator, int slice);

	// ���鳡���зֲ�����random npc������
	static int H_GroupSceneCreateRandomNpc(IKernel* pKernel, const PERSISTID& creator, int slice);
	//���鳡���а�ʱ�䴴�� npc
	//static int H_GroupCreateNpcByTime(IKernel*pKernel, const PERSISTID& creator, int slice);


	/*----------------------------------�ⲿ�ӿ�----------------------------------------*/
public:
	// ���ݳ���id��������Npc
	bool CreateCloneNpcFromXml(IKernel* pKernel, const PERSISTID& scene);

	// ������id��������Npc
	bool CreateGroupNpcFromXml(IKernel* pKernel, const PERSISTID& scene,
		int nSubSceneNo, int nGroupId, const char* npcCfgName);

	// ��������ˢ����
	const PERSISTID CreateTaskrCreator(IKernel* pKernel, const PERSISTID& scene,
		const int task_id, const int group_id, const int level);
// 	void CreateGuildInbreakCreator(IKernel* pKernel, const PERSISTID& scene, const char* cfgName,
// 		const int group_id, const int level);
	/*----------------------------------�ڲ��ӿ�----------------------------------------*/
protected:
	// ��ʼˢ��
	static void BeginRefresh(IKernel* pKernel, const PERSISTID& creator);

	// ����ˢ��
	static void EndRefresh(IKernel* pKernel, const PERSISTID& creator);

	// --------------��̬npc������غ���---------------

	// ��xml�ļ������ɳ�����Ҫ�ľ�̬����NPC
	// ������̬npc
	bool CreateStaticNpc(IKernel* pKernel, const PERSISTID& scene,
		std::vector<CreateStaticNpcInfo>& staticNpc, int nSubSceneNo = -1, int nGroupID = -1);

	// ����xml���ô���ĳ����̬npc
	PERSISTID CreateStaticNpcUnit(IKernel* pKernel, const PERSISTID& scene,
		CreateStaticNpcInfo&pItem, int nSubSceneNo = -1, int nGroupId = -1);

	// --------------�漴npc������������غ���---------------

	// ��xml�ļ������ɳ�����Ҫ�����NPC������
	// �������Npc
	bool CreateRandomNpc(IKernel* pKernel, const PERSISTID& scene,
		std::vector<CreateSpringNpcInfo>&npcCfg, int nSubSceneNo = -1, int nGroupID = -1);

	bool CreateARandomCreator(IKernel* pKernel, const PERSISTID& scene,
		CreateSpringNpcInfo&pCreate, int nSubSceneNo, int nGroupId);

	//��ǰ�趨��Ӣ��ˢ��λ�ã��������С�ָ���
	bool PreSetCreateCenterPos(IKernel* pKernel, const PERSISTID& creator);
	//����ˢ��ʱ�䴴��npc
	bool CreateRandomNpcByTime(IKernel*pkernel, const PERSISTID& scene, int sceneId, int groupId);

	void AddSceneGroupNpcCfgName(int sceneId, int groupId, const std::string& sceneCfgName);
	//����սnpc
	void CreateWheelNpc(IKernel*pKernel, int sceneId, int groupId, int killNumNow = 0);




public:
	// ���ظ���npc������Դ
	bool LoadNpcInfo(IKernel* pKernel);
	// ��������ˢ��������
	bool LoadTaskCreatorRes(IKernel* pKernel);
	//��������
	void CleanCfg();
	//��¡����NPC 
	void PraseCloneNpc(IKernel* pKernel, int sceneID, const fast_string& cfgPath);
	//���鳡��NPC )
	void ParseGroupNpc(IKernel* pKernel, const std::string& sceneCfgName);
	// ����ʱ��ˢ��npc
	void ParseCreateNpcByTime(const std::string& npcCfgName, const std::vector<xml_document<>*>& tempDocVec);
	//����սnpc
	void ParseWheelNpc(const std::string& npcCfgName, const std::vector<xml_document<>*>& tempDocVec);
	// ע������������������
	bool RegisterRandomCreator(IKernel* pKernel, const PERSISTID& scene, const char* id, const PERSISTID& creator);
	//�������npc����
	void ParseSceneNpc(IKernel* pKernel, std::string npcCfgName, const std::vector<xml_document<>*>& tempDocVec);

	void ParseRandomNpcCreate(IKernel* pKernel, std::vector<CreateSpringNpcInfo>& springNpcGroup, xml_node<>*pCreator);
	void ParseStaticNpcCreate(IKernel* pKernel, std::vector<CreateStaticNpcInfo>& staticNpcGroup, xml_node<>*pCreator);
	
	int ParseRandomNpcItem(IKernel* pKernel, std::vector<RandomCreator>&  createSpringNpcInfoGroup, xml_node<>*pCreator);
	void ParseRandomNpcPosition(IKernel* pKernel, std::vector<NpcPosition> & positionGroup, xml_node<>*pCreator);

	//���Ŀ¼���ļ����б�
	void GetDirFileList(IKernel* pKernel, const fast_string& pathCfg, std::vector<xml_document<>*>& outFileList);


	//��¼
	void RecordKillNpc(IKernel* pKernel, const PERSISTID& npc);
	//����NPC�������
	bool LoadNpcPool(IKernel* pKernel);
	//���NPC��configid
	int GetConfigID(IKernel* pKernel, const PERSISTID& pCreator, const char* npcPoolID, std::string& newConfigID);
	//��ó�����NPC������Ϣ���������鳡��ʹ�ã�
	int GetSceneNpcNum(IKernel* pKernel, const PERSISTID& scene);
	//��֯����npc
	//bool LoadGuildInbreak(IKernel* pKernel);
	

private:
	struct NpcPool
	{
		NpcPool() :configID(""), isSame(0), weight(0), minWeight(0), maxWeight(0){};
		std::string configID;    //NPC��configID
		int isSame;              //�Ƿ���ظ�
		int weight;              //Ȩֵ
		int minWeight;
		int maxWeight;
	};

	std::vector<int> v_vecSceneNpc;//�����Ѽ����NPC�����ķ��鳡����

	



	//ʱ��ˢ��npc
	struct NpcInfo
	{
		NpcInfo(const std::string& id, float x, float y, float z, float ay, int mount) :
		_id(id), _x(x), _y(y), _z(z), _ay(ay), _mount(mount) {}
		std::string _id;
		float _x;
		float _y;
		float _z;
		float _ay;
		int _mount;
	};

	struct RefreshNpcRule {
		RefreshNpcRule() : _totalStep(0), _refreshTime(0), _waitTime(0) {};
		RefreshNpcRule(int totalStep, int refreshTime, int waitTime) :
			_totalStep(totalStep), _refreshTime(refreshTime), _waitTime(waitTime) {};
		int _totalStep;
		int _refreshTime;
		int _waitTime;
		std::vector<NpcInfo> _npcInfo;

	};

	struct WheelNpc
	{
		WheelNpc(const std::string& id,float x,float y,float z,float ay,int mount,int needKill):
		_id(id), _x(x), _y(y), _z(z), _ay(ay), _mount(mount), _needKillNum(needKill)
		{
			
		}
		std::string _id;
		float _x;
		float _y;
		float _z;
		float _ay;
		int _mount;
		int _needKillNum;//��Ҫ��ɱnpc����
	};


public:
    //������Ȩֵ
    int CalWeight(std::vector<NpcPool>& vec);
    //��ÿɳ�����NPC
    void GetNpcFromPool(IKernel* pKernel, const PERSISTID& pCreator, std::vector<NpcPool> srcVec, std::vector<NpcPool>& tarVec);
    //��ÿɳ�����BOSS
//     void GetBossFromPool(IKernel* pKernel, const char* npcPool, std::string& newConfig);
// 	//���³���npc
// 	void upDateWheelNpc(IKernel*pKernel, int sceneID, int groupID,int killNumNow);
	
	const char* GetSceneGroupNpcCfgName(int sceneID ,int groupID);



	/*----------------------------------XML�����ӿ�----------------------------------------*/
private:
	void ClearXMLData();
	
	// �����д��������xml�����ļ�
	std::map<int, std::vector<xml_document<>* > > m_clone_npc_info;

	// ���鳡���д����Ķ����xml�����ļ�
	std::map<std::string, std::vector<CreateSceneNpcInfo> > m_group_npc_info;

	std::vector<char*> m_vecbuff;
	//ʱ��ˢ��npc
	std::map<std::string, std::vector<RefreshNpcRule>>   m_refresh_npc_by_time;
    //NPC�������
    std::map<std::string, std::vector<NpcPool>> m_mapNpcPool;
	//����սnpc
	std::map<std::string, std::map<int, std::vector<WheelNpc> > > m_wheel_npc;
	//܇݆NPC������������ < sceneid < <groupid> scenecfg> ��ǰ������
	std::map<int,std::map<int, std::string> > m_sceneGroupNpcCfgName;
	// �������ˢ����
	std::map<int, CreateSpringNpcInfo> m_TaskCreatorMap;
	//��֯����
//	std::map<std::string, CreateSpringNpcInfo> m_guildInbreak;
public:
	// ģ��ָ��
	static NpcCreatorModule* m_pNpcCreatorModule;
};

#endif // __NpcCreatorModule_h__