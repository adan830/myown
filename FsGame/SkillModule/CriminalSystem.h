//--------------------------------------------------------------------
// �ļ���:		CriminalSystem.h
// ��  ��:		���ͷ�ϵͳ
// ˵  ��:					
// ��������:	2017��07��24��
// ������:		liumf   
//--------------------------------------------------------------------

#ifndef __CRIMINAL_SYSTEM_H__
#define __CRIMINAL_SYSTEM_H__

#include "Fsgame/Define/header.h"
#include "utils/Singleton.h"
#include <vector>

class CriminalSystem
{
public:
	// ��ʼ��
	virtual bool Init(IKernel* pKernel);

	// �ر�
	virtual bool Shut(IKernel* pKernel);

	// ���ö�ȡ
	bool LoadResource(IKernel* pKernel);

	// ����Ԫ���������ֵ
	int OnCustomCostMoneyCleanCrime(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);

	// �������ֵ����
	void AddDecCrimeScore(IKernel* pKernel, IGameObj* pSelfObj, IGameObj* pDeadNpcObj);

	// ��ѯ�����ͷ�ϵ�� ���Ǻ�������false	  nFunctionType��������  nQueryType��ѯϵ������
	bool QueryCiminalRate(IKernel* pKernel, float& fOutPublishRate, const PERSISTID& self, int nFunctionType, int nQueryType);

	// ��Ҫ��������֤
	static bool IsValidPublicScenePVP(IKernel* pKernel, const PERSISTID& attacker, const PERSISTID& target, IGameObj*& pAttackerObj, IGameObj*& pTargetObj);
private:
	// �����
	struct CriminalConst 
	{
		int		nGrayToNormalTime;		// ������������ʱ��(s)
		int		nRedCrimeValue;			// ���������ֵ
		int		nKillCrimerChivalry;	// ɱ�������ӵ�����ֵ
		int		nTimeToCrimeRate;		// ���߶�ü�1�����ֵ ��λ(m)
		int		nClearCrimeCostRate;	// ���1�����ֵ���ĵĻ���
		int		nJailClearCrimeCostRate;	// ����ʱ���1�����ֵ���ĵĻ���
		int		nClearCrimeMoneyType;	// ��������
		int		nScroeToCrimeRate;		// ���ٷּ�1�����ֵ
		int		nMaxChivalry;			// ÿ�����ɱ�����õ�����ֵ
		int		nMaxCrimeValue;			// ������ֵ
		int		nForceBeJailCrimeVal;	// ǿ�����ε����ֵ
	};

	// ���ֵ����
	struct CriminalRule 
	{
		int		nMinLevelDiff;			// ��С�ȼ���
		int		nMaxLevelDiff;			// ���ȼ���
		int		nCriminalValue;			// ���ӵ����ֵ
	};

	typedef std::vector<CriminalRule> CriminalVec;

	// ���ֵɱ�ֻ���
	struct CrimieScore
	{
		int		nNpcType;				// npc����
		int		nScore;					// ����
	};

	typedef std::vector<CrimieScore> CrimieScoreVec;

	// ���ͷ�����
	struct CrimiePublishData
	{
		int		nFunctionType;			// ��������
		float	fCapitalPublishRate;	// ���˥��
		float	fExpPublishRate;		// ����˥��
	};

	typedef std::vector<CrimiePublishData> CrimPDVec;

	enum 
	{
		 CHECK_CRIME_STATE_TIME_INTERVAL = 5000,		// ���ֵ���߼��Ƶ��
	};

	/// \brief ����
	static int OnPlayerRecover(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);

	// �����������
	static int HB_CleanCrimeState(IKernel * pKernel, const PERSISTID & self, int slice);

	// ���߼������ֵ
	static int HB_DecCrimeValue(IKernel * pKernel, const PERSISTID & self, int slice);

	// ��Ӧ�˺�ĳ�����
	static int OnCommandDamagePlayer(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);

	// ��Ӧ��ɱ��
	//static int OnCommandBeKill(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);

	// ��Ӧɱ�����
	static int OnCommandKillPlayer(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);

	//ս��״̬�����ı�
	static int C_OnFightStateChanged(IKernel *pKernel, const PERSISTID &self, const char *property, const IVar &old);

	//���ֵ�仯
	static int C_OnCrimeValueChanged(IKernel *pKernel, const PERSISTID &self, const char *property, const IVar &old);

	// ��ȡ��������
	bool LoadConstConfig(IKernel* pKernel);

	// ��ȡ���ֵ���ӹ���
	bool LoadCriminalRule(IKernel* pKernel);

	// ��ȡ���ֵɱ�ֻ��ֹ���
	bool LoadCrimeScorelRule(IKernel* pKernel);

	// ��ȡ���ͷ���������
	bool LoadCrimiePublishData(IKernel* pKernel);

	// ��ѯ���ӵ����ֵ
	int QueryAddCriminalValue(IGameObj* pAttackerObj, IGameObj* pTargetObj);

	// ��ѯ���ӵĻ���
	int QueryAddDecCrimeScore(IGameObj* pTargetObj);

	// ��������ֵ
	//void AddChivalry(IKernel* pKernel, IGameObj* pPlayerObj);

	// ɱ������õ�����ֵ����
	//static int ResetAddChivalry(IKernel *pKernel, const PERSISTID &self, int slice);
private:
	CriminalConst		m_kCriminalConst;		// ���ϵͳ����
	CriminalVec			m_vecCriminalValue;		// ���ֵ���ӹ���
	CrimieScoreVec		m_vecCrimieScore;		// ���ֵ���ֹ���
	CrimPDVec			m_vecCrimiePublishData;	// �ͷ���������
};

typedef HPSingleton<CriminalSystem> CriminalSystemSingleton;
#endif //__GAINT_MODULE_H__
