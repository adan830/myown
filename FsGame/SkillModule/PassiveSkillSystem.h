//----------------------------------------------------------
// �ļ���:      PassiveSkillUpgrade.h
// ��  ��:      ������������ϵͳ
// ˵  ��:		
// ��������:    2015��6��24��
// ������:       
// �޸���:    
//    :       
//----------------------------------------------------------

#ifndef _PASSIVESKILLUPGRADE_H_
#define _PASSIVESKILLUPGRADE_H_

#include "Fsgame/Define/header.h"
#include <vector>
#include <map>
#include "utils/Singleton.h"
#include "FsGame/Define/Skilldefine.h"

class SkillDataQueryModule;
struct PassiveSkillLevelInfo;
struct PassiveSkill;
class ContainerModule;
#ifndef FSROOMLOGIC_EXPORTS
class SystemMailModule;
#endif
class CapitalModule;

class PassiveSkillSystem
{
public:
	// ��ʼ��
	bool Init(IKernel* pKernel);

	/*!
	 * @brief	���ӻ���±������� ���ﱻ�������õ�
	 * @param	self �����(�����ǳ���Ҳ���������)
	 * @param	nSkillId ����id
	 * @param	nSkillLevel ���ܵȼ�
	 * @param	nSkillSrc  ������Դ
	 * @return	bool
	 */
	bool UpdatePassiveSkill(IKernel* pKernel, const PERSISTID &self, int nSkillId, int nSkillLevel, int nSkillSrc);

	/*!
	 * @brief	�Ƴ���������
	 * @param	self �����(�����ǳ���Ҳ���������)
	 * @param	nSkillId����id
	 * @return	bool
	 */
	bool RemovePassiveSkill(IKernel* pKernel, const PERSISTID &self, int nSkillId);

	// ���ұ������ܵȼ�,û�ҵ�����0
	int QueryPassiveSkillLevel(IKernel* pKernel, const PERSISTID &self, int nSkillId);

	// ��Ӧ��������������Ϣ(����־)
	static int OnCustomOnPassiveSkillUpgrade(IKernel* pKernel, const PERSISTID &self,
		const PERSISTID & sender, const IVarList & args);

	// ��Ӧ��������������Ϣ(����־)
	static int OnCustomResetPassiveSkill(IKernel* pKernel, const PERSISTID &self,
		const PERSISTID & sender, const IVarList & args);

	// ѧϰ���б�������(���ԡ�����ʹ��)
	void LearnAllPassiveSkill(IKernel* pKernel, const PERSISTID &self);
private:
	enum 
	{
		PS_ADD_EFFECT,			// ������Ч
		PS_REMOVE_EFFECT,		// �Ƴ���Ч

		MAX_PS_EFFECT_INDEX = 32 // ���32λ
	};
	// ���ü��ܵ�
	struct ResetCost 
	{
		ResetCost() : strItemId(""), nItemNum(0){}
		std::string strItemId;			// ���ѵ���Ʒ
		int			nItemNum;			// ��Ʒ����
	};

	//�ص����� ��Ҽ����������
	static int OnPlayerRecover(IKernel* pKernel, const PERSISTID& self,
		const PERSISTID& sender, const IVarList& args);

	// ���ܵ�
	struct SkillPoint
	{
		SkillPoint() : nMeridianLevel(0), nAddSkillPoint(0){}
		int			nMeridianLevel;		// ��ҵȼ�
		int			nAddSkillPoint;		// ���ӵļ��ܵ�
	};

	typedef std::vector<SkillPoint> SkillPointVec;

	// ��鼼�ܵ���������
	bool CheckUpgradeTerm(IKernel* pKernel, IGameObj* pSelfObj, const PassiveSkill* pMainInfo, const PassiveSkillLevelInfo* pSkillInfo, const int nRequestSkillLevel);

	// �۳���ҲƲ�(Ǯ,����,���ܵ�)
	bool UpgradeCostPlayerAssets(IKernel* pKernel, IGameObj* pSelfObj, const PassiveSkillLevelInfo* pSkillInfo);

	// ��Ӧ����������ı仯
	void OnPassiveSkillChange(IKernel* pKernel, const PERSISTID &self, int nSkillId, const PassiveSkillLevelInfo* pSkillInfo);

	// �黹ĳ��������Ǯ�����ܵ㡢����
	void GiveBackOneSkillAssets(IKernel* pKernel, int& outSkillPoint, int nSkillId, int nSkillLevel);

	// ��ȡ���ܵ�����
	bool LoadSkillPointAddConfig(IKernel* pKernel);

	// ��ѯ��Ӧ�ȼ����ӵļ��ܵ�
	int QueryAddSkillPoint(int nPlayerLevel);

	// ����������Ʒ��鼰�۳�
	bool ResetCostItem(IKernel* pKernel, const PERSISTID &self);

	// ɾ���������ܵĴ���
	bool OnRemovePassiveSkill(IKernel* pKernel, const PERSISTID &self, int nSkillId, int nSkillLevel);

	// ��ӱ�������Ӱ�������
	void AddAllPassiveSkillPro(IKernel* pKernel, IGameObj* pSelfObj);

	// ִ�б�����������
	void ExecuteResetPassiveSkill(IKernel* pKernel, const PERSISTID& self);

	// ����Ƿ���ѧϰ�ı�������
	bool IsHavePassiveSkill(IKernel* pKernel, const PERSISTID& self);

	// ͳ��ĳ���������ĵļ��ܵ�
	int StatOneSkillCostPoint(int nSkillId, int nSkillLevel, const PassiveSkill* pMainInfo);
private:
	SkillPointVec		m_vecSkillPointRec;	// ��Ҽ��ܵ����ñ�

	static SkillDataQueryModule* m_pSkillDataQueryModule;
	static ContainerModule*		m_pContainerModule;
	static CapitalModule*		m_pCapitalModule;

#ifndef FSROOMLOGIC_EXPORTS
	static SystemMailModule*	m_pSystemMailModule;
#endif // FSROOMLOGIC_EXPORTS
};

typedef HPSingleton<PassiveSkillSystem> PassiveSkillSystemSingleton;

#endif