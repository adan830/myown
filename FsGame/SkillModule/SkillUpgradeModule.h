//---------------------------------------------------------
//�ļ���:       SkillUpgradeModule.h
//��  ��:       ���ܺͼ�������ģ��
//˵  ��:  
//          
//��������:      2016��4��5��
//������:         ������
//�޸���:
//   :         
//---------------------------------------------------------

#ifndef FSGAME_SKILL_SKILL_UPGRADE_H_
#define FSGAME_SKILL_SKILL_UPGRADE_H_

#include "Fsgame/Define/header.h"
#include <string>
#include <map>
#include <vector>

class ContainerModule;
class CapitalModule;
class LogModule;
class DailyActivityModule;

class SkillUpgradeModule : public ILogicModule
{
public:

    virtual bool Init(IKernel* pKernel);

    virtual bool Shut(IKernel *pKernel);


public:
	enum
	{
		PUBLIC_CAREER_SKILL = 0,			// ͨ�ü��ܱ�־
	};
    // ������ݻָ�
    static int OnPlayerRecover(IKernel* pKernel, const PERSISTID& player,
        const PERSISTID& sender, const IVarList& args);

    // �������Կͻ��˵���Ϣ
    static int OnSkillSetupMsg(IKernel *pKernel, const PERSISTID &player,
        const PERSISTID & sender, const IVarList & args);

    // ����������ܵ�����
    int OnUpgradeSkill(IKernel *pKernel, const PERSISTID &player, int nUpgradeType, const IVarList & args);

    // ��ҽ�ɫ����
    static int OnPlayerLevelUp(IKernel* pKernel, const PERSISTID& self,
        const char* property, const IVar& old);

	// ��ȡ��ҵ���ͨ����
	int GetUsableNormalSkill(IKernel *pKernel, const PERSISTID &player, std::vector<std::string> *out_skill);

	// ����������Դ
	bool LoadResource(IKernel* pKernel);
private:
	// ������ʽ
	enum UpgradeType
	{
		UPGRADE_ONE_LEVEL,			// ��һ��
		UPGRADE_MULTI_LEVEL			// ������߼�
	};

    //����ͨ���ܵĶ���, ��ͨ���ܲ������κμ���
    struct NormalSkillDef
    {	
		NormalSkillDef() : id(""), career_id(0), level_to_unlock(0){}
        std::string id;
        int career_id;
        int level_to_unlock;
    };

	// ���ܵĻ�������
	struct SkillBaseDef
	{
		SkillBaseDef() : id(""), seq_no(0), career_id(0), level_to_unlock(0), gold_type_to_upgrade(0),
		gold_amount_to_upgrade(0), max_level(0){}
		std::string id;         // ����ID
		int seq_no;             // ���ܵ����
		int career_id;			// ��������ְҵID
		int level_to_unlock;    // �����˼�����Ҫ����ҵȼ�
		int gold_type_to_upgrade;      // �����˼�����Ҫ�Ľ������
		int gold_amount_to_upgrade;    // �����˼�����Ҫ�Ľ��
		int max_level;                 // �˼��ܵ���ߵȼ�
	};

	// ���ܵȼ��ļ۸�, �����������ȼ�N��Ҫ�Ļ���
	struct SkillLevelCostDef
	{
		SkillLevelCostDef() : skill_seq_no(0), skill_level(0), capital_type(0), capital_amount(0){}
		int skill_seq_no;    // �������, ÿ��ְҵ���ĸ�����, ���ǵ���ŷֱ��� 1,2,3,4
		int skill_level;    // ���ܵȼ�
		int capital_type;   // �˵ȼ���Ҫ���ʽ�����
		int capital_amount; // �˵ȼ���Ҫ���ʽ�����
	};
private:
	// �������������߼�
	bool UpgradeSkillLevel(IKernel * pKernel, const PERSISTID & player, const char * skill_id, int nUpgradeType, int &nOldCittaLevel, int& nNewCittaLevel);
    
	// ��ȡ��������
	const SkillBaseDef* GetSkillDef(const char *skill_id);

	// ��ȡ������������
	const SkillLevelCostDef* SkillUpgradeModule::GetSkillLevelCost(int skill_seqno, int level);

	// ������ҵ���ͨ����
	int SetPlayerNormalSkill(IKernel *pKernel, const PERSISTID &player);

	// ��Ҽ���ѧϰ
	int LearnPlayerSkill(IKernel *pKernel, const PERSISTID &player);

	// ���뼼������
	bool LoadSkill(IKernel* pKernel);

	// ���뼼�ܵȼ��۸�
	bool LoadSkillLevelCost(IKernel* pKernel);

	//��������ͨ����
	bool LoadNormalSkill(IKernel* pKernel);
	
	// ��ѯĳ�����ܵ�ս����
// 	int QuerySkillBA(const char* strSkillId, int nSkillLevel);
// 
// 	// ���㼼���������ս�����ӳ�
// 	int CalBAOnSkillUpgrade(const char* strSkillId, int nPrevLevel, int nNewLevel);
// 
// 	// ������ҵļ���ս����
// 	int CalPlayerSkillBA(IKernel *pKernel, const PERSISTID &self);
// 
// 	// ��ս�����Ӹ����
// 	int AddBAToPlayer(IKernel* pKernel, const PERSISTID& self, int the_battle_ability);

	static void ReloadConfig(IKernel* pKernel);
public:

    static SkillUpgradeModule *m_pSkillUpgradeModule;

private:
    static CapitalModule *m_pCapitalModule;

    // ���ܵĵȼ��۸��趨
    typedef std::map<std::string, SkillLevelCostDef> SkillLevelCostMap;
    SkillLevelCostMap m_mapSkillLevelCost;

    // ȫ���ļ���
    typedef std::map<std::string, SkillBaseDef> SkillMap;
    SkillMap m_mapSkillData;

    // ȫ����ͨ����
    typedef std::map<std::string, NormalSkillDef> NormalSkillMap;
    NormalSkillMap m_mapNormalSkill;
}; // End of class

#endif