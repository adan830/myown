//--------------------------------------------------------------------
// �ļ���:      AISystem.h
// ��  ��:      AIϵͳ
// ˵  ��:
// ��������:    2014��12��31��
// ������:       
// �޸���:        
//    :       
//--------------------------------------------------------------------
#pragma once
#include "Fsgame/Define/header.h"
#include "SDK/rapidxml/rapidxml.hpp"
#include "AIDefine.h"
#include "Template/AITemplateBase.h"
#include "Rule/AIRuleBase.h"
#include "FsGame/SkillModule/FightModule.h"
#include "FsGame/SystemFunctionModule/MotionModule.h"
#include "FsGame/SkillModule/Skillmodule.h"
#include "AIHeartBeat.h"
#include "AIFunction.h"
#include "FsGame/CommonModule/LogModule.h"
#include "FsGame/SystemFunctionModule/DropModule.h"
#include "FsGame/CommonModule/ContainerModule.h"
#include "FsGame/CommonModule/LevelModule.h"
#include "FsGame/SystemFunctionModule/CapitalModule.h"
//#include "FsGame/SocialSystemModule/CopyOffLineModule.h"
#include "utils/geometry_func.h"
#include <time.h>

using namespace rapidxml;

class AIHeartBeat;
class AIBasicAction;
class AISystem : public ILogicModule
{
public:
	//��ʼ��
	virtual bool Init(IKernel* pKernel);
	// �ر�
	virtual bool Shut(IKernel* pKernel);

public:
    //����Ѳ��
    static int OnPatrol(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);
	//��ȡģ��
	AITemplateBase &GetAITemlate(IKernel* pKernel, const PERSISTID& self);
	//��ȡ���͵�
	static bool GetConvoyPoint(IKernel* pKernel, const PERSISTID& self, const char* npcId);
	//��ȡ���ع�
	static bool GetInterceptNpc(IKernel* pKernel, const PERSISTID& self, const char* npcId, int step, CVarList& args);
	//��ȡ�����·����
	static bool GetPatrolNpcPoint(IKernel* pKernel, const PERSISTID& self);
	//���ù��＼��
	static bool InsertNpcSkill(IKernel* pKernel, const PERSISTID& self, const char* skillStr);
	//������ر������	
	static bool	GetBoxDeadDrop(const std::string &configId, DropWeight &drop);
	//��������һ��
	static int OnAutoSpring(IKernel* pKernel, const PERSISTID& self);
	// ������Ϊ����
	static int OnActionEnd(IKernel* pKernel, const PERSISTID& self, int msg);
	// ����С��
	static bool CreateBattleTeam(IKernel* pKernel, const PERSISTID& self, int iNation, int nTeamId, int iLevel);
	//��������·����
	bool SetBattleNpcPath(IKernel* pKernel, const PERSISTID& self, int nCamp);
	//��������NPCƫ��λ��
	bool GetOtherNpcPos(IKernel* pKernel, float PosX, float PosZ, int iIndex, float& OtherPosX, float& OtherPosZ);
	//�趨��������״̬
	int SetBattleNpcTeamStatus(IKernel* pKernel, const PERSISTID& self, int iStatus);
    //��ȡ����������Ϣ
    static bool GetSkillConfig(IKernel* pKernel, const PERSISTID& self, const char* skillID, NpcSkill& npcSkill);
    //���ս����Ϣ
    static bool ClearFight(IKernel* pKernel, const PERSISTID& self);
	// ���ܰ��Ƿ����
	static bool IsSkillPkgExist(const char* skill_pkg);
	//���npc��ͨ����
	static const char* GetNpcNomalSkill (const char* skillCfg);
	// ��ȡ������Χ
	static float GetSpringRange(IGameObj* pSelfObj);

	// ȡ�õȼ����԰�
	static const NpcLevelPackage* GetNpcLevelPackage(const char *p_config_id, const int n_level);

	// ȡ��Npc���԰�
	static const int GetNpcPackage(IGameObj* pSelfObj);
//��̬����
public:

    //ȡ��AISystemʵ��ָ��
    static AISystem *Instance()
    {
        if(m_pAISystem == NULL)
        {
            extend_warning(LOG_CRITICAL, "AISystemģ�黹û�г�ʼ����ʹ�ã�");
            assert(0);
            m_pAISystem = new AISystem();
            return m_pAISystem;
        }
        return m_pAISystem;
    }

//עɾ�ľ�̬�ص�����
private:
    //������볡��ʱ����
    static int OnEntry(IKernel *pKernel, const PERSISTID &self, const PERSISTID &sender, const IVarList &args);

    //���󱻴���ʱ����
    static int OnSpring(IKernel *pKernel, const PERSISTID &self, const PERSISTID &sender, const IVarList &args);

	//��������
	static int OnDestroy(IKernel *pKernel, const PERSISTID &self, const PERSISTID &sender, const IVarList &args);

    //��Ҹ���
    static int OnPlayerRelive(IKernel *pKernel, const PERSISTID &self, const PERSISTID &sender, const IVarList &args);

    //���˺�
    static int OnBeDamaged(IKernel *pKernel, const PERSISTID &self, const PERSISTID &sender, const IVarList &args);

    //����ɱ��ʱ����
    static int OnBeKilled(IKernel *pKernel, const PERSISTID &self, const PERSISTID &sender, const IVarList &args);

    //���󸴻�
    static int OnRelive(IKernel *pKernel, const PERSISTID &self, const PERSISTID &sender, const IVarList &args);

	// ʵ�ü��ܺ��ƶ�
	static int OnFightOverMove(IKernel *pKernel, const PERSISTID &self, const PERSISTID &sender, const IVarList &args);

    // �������Ĵ���
    static int OnCreateArgs(IKernel *pKernel, const PERSISTID &self, const PERSISTID &sender, const IVarList &args);

	//��������������
	static int OnCopyDataComplete(IKernel *pKernel, const PERSISTID &self, const PERSISTID &sender, const IVarList &args);
private:
    //��ʼ��ģ��
    void InitTemplateVector(IKernel *pKernel);
    //ע��ģ��
    bool RegistTemplate(IKernel *pKernel, AITemplateBase *pTemplateBase);
	//���＼��ʹ��Ȩֵ����
	static bool LoadNPCSkill(IKernel* pKernel);
    //���＼�ܰ�
    static bool LoadNpcSkillPackage(IKernel *pKernel);
	//����Ѳ��·��
	static bool LoadPatrolPath(IKernel* pKernel);
	//���ػ���NPC·��
	static bool LoadConvoyPath(IKernel* pKernel);
	//�������ع���
	static bool LoadInterceptInfo(IKernel* pKernel);
	//��սNPC����
	static bool LoadBattleNpcBornIndex(IKernel* pKernel);
	//����·������
	static bool LoadBattleNpcMoveType(IKernel* pKernel);
	//��ȡ����С������
	static bool LoadBattleNpcTeam(IKernel* pKernel);
	//��ȡС�Ӷ�������
	static bool LoadBattleTeamFormation(IKernel* pKernel);
	//���ر�������
	static bool LoadBoxConfig(IKernel *pKernel);
	// �ȼ����԰�����
	static bool LoadLevelPackage(IKernel *pKernel);

	//��Ŀ��㷶Χ��Ѱ��һ�������
	static bool RandomFindAroundPos(float& fOutX, float& fOutZ);
	// ��ѯС������
	const BattleTeam* QueryTeamConfig(int nTeamId);
	// ��ѯС�Ӷ���
	const BattleTeamFormation* QueryTeamFormation(const int scene_id);

public:
	//ս��ϵͳ
	static FightModule* m_pFightModule;
	//�ƶ�
	static MotionModule* m_pMotionModule;
	//����
	static SkillModule* m_pSkillModule;
	//����
	static DropModule* m_pDropModule;
	//����
	static ContainerModule* m_pContainerModule;
	//����
	static LevelModule* m_pLevelModule;
	//��Ǯ
	static CapitalModule* m_pCapitalModule;

#ifndef FSROOMLOGIC_EXPORTS
	//����ģ��
	//static CopyOffLineModule* m_pCopyOffLineModule;
#endif // FSROOMLOGIC_EXPORTS

private:

    //AI����
    static AISystem *m_pAISystem;

    //�Ҳ�����Ӧģ��ʱ�û���ģ��
    static AITemplateBase s_AITemplateBase;

	//���е�ģ�����
	std::vector< AITemplateBase* >  m_vctAITemplate;
	//��������ʹ��Ȩֵ
	std::map<std::string,NpcSkill> m_mapSkillConfig;
    //NPC�����������
    std::map<std::string, NpcSkillPackage> m_mapSkillPackage;
	//��������
	std::map<std::string, std::vector<Point> > m_mapConvoyStep;
	//��������
	std::map<std::string,std::vector<Intercept>> m_mapIntercept;
	//Ѳ��NPC����
	std::map<std::string, std::vector<PatrolPos>> m_mapPatrolInfo;
	//��սNPC����
	std::map<int, BattleNpcStruct> m_mapBattleNpcInfo;
	//���߷�ʽ����
	std::map<int, MoveTypeDef> m_mapMoveTypeInfo;
	// С������
	BattleTeamVec m_vecBattleTeamConfig;
	// ��������
	BattleTeamFormationVec m_BattleTeamFormationVec;
	//���ر���
	MAP_BOXWEIGHT	m_mapBoxInfo;
	// �ȼ����԰�
	NpcLevelPackageVec m_NpcLevelPackageVec;
};
