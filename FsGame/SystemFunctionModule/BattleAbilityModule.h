//--------------------------------------------------------------------
// �ļ���:      BattleAbilityModule.h
// ��  ��:      ս����ͳ��
// ˵  ��:
// ��������:    2017/04/12
// ������:      ������
//    :       
//--------------------------------------------------------------------

#ifndef __BATTLE_ABILITY_MODULE_H__
#define __BATTLE_ABILITY_MODULE_H__

#include "Fsgame/Define/header.h"
#include <vector>
#include <map>
#include "Define/PlayerBaseDefine.h"

class BattleAbilityModule : public ILogicModule
{
public:
	//��ʼ��
	virtual bool Init(IKernel* pKernel);

	//�ͷ�
	virtual bool Shut(IKernel* pKernel);
public:
	// ��ȡ����
	bool LoadConfig(IKernel* pKernel);
private:
	// ����ֵս������
	struct ValueBAData  
	{
		ValueBAData() : strProName("")
		{
			ZeroMemory(arrRate, sizeof(arrRate));
		}
		std::string		strProName;			// ������
		float	arrRate[PLAYER_JOB_MAX];	// ս����ϵ��
	};

	typedef std::vector<ValueBAData>   ValueBADataVec;

	// �ٷֱ�ս������
	struct RateBAData
	{
		std::string			strProName;		// ������ 
		std::vector<float>	vecLevelRate;	// �ȼ�ϵ���б�

		bool operator == (const RateBAData &other) const
		{
			return strProName == other.strProName;
		}
	};

	typedef std::vector<RateBAData>		RateBADataVec;

	struct FormulaBAData
	{
		std::string		strFormulaId;	  // ��ʽid
		int				nCalType;		  // ��������	(BAFormulaCalType)
		std::string		strFormula;		  // ��ʽ
	};

	// ��ʽս������(���ܡ���������)
	typedef std::map<std::string, FormulaBAData>		FormulaBADataMap;

	// �������
	static int OnPlayerRecover(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);

	// ��Ӧ����ս����
	static int OnCommandUpdateBattleAbility(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);

	static void ReloadConfig(IKernel* pKernel);

	// ��ȡ����ֵս������
	bool LoadValueBAConfig(IKernel* pKernel);

	// ��ȡ�ٷֱ�ս������
	//bool LoadRateBAConfig(IKernel* pKernel);

	// ��ȡ��ʽս������
	bool LoadFormulaBAConfig(IKernel* pKernel);

	// ��������ս����
	void UpdatePropertyBattleAbility(IKernel* pKernel, IGameObj* pSelfObj);

	// ������������ս����
	void UpdateSkillBattleAbility(IKernel* pKernel, IGameObj* pSelfObj);

	// ���±�������ս����
	void UpdatePassiveSkillBattleAbility(IKernel* pKernel, IGameObj* pSelfObj);

	// ����ĳһ�����ֵ�������ӵ�ս��
	float CalValuePropertyBattleAbility(IKernel* pKernel, IGameObj* pSelfObj, const ValueBAData& data);

	// ����ĳһ����Ұٷֱ��������ӵ�ս��
	float CalRatePropertyBattleAbility(IKernel* pKernel, IGameObj* pSelfObj, const RateBAData& data);

	// ����ĳ�������������ӵ�ս��
	float CalSkillBattleAbility(IKernel* pKernel, IGameObj* pSelfObj, const PERSISTID& skill);

	// ����ĳ�������������ӵ�ս��
	float CalPassiveSkillBattleAbility(IKernel* pKernel, IGameObj* pSelfObj, int nPassiveSkillId);

	// ͳ����ս����
	void StatisticsTotalBattleAbility(IKernel* pKernel, const PERSISTID& self, IGameObj* pSelfObj);

	// ����һ���ٷֱ�����
	void ParseRateBAData(const char* strProName, float fRate);

	// ��ѯ����ֵ
	float QueryProperty(IGameObj* pSelfObj, const char* strProName);

	// ����ĳ�����͵�ս��ֵ
	void RefreshOneTypeBAValue(IGameObj* pSelfObj, int nBAType, float fBAValue);

	// ���㹫ʽս����
	float CalFormulaBattleAbility(IKernel* pKernel, IGameObj* pSelfObj, const PERSISTID& skill, const FormulaBAData& data);

	// ��ѯĳ�����Ե�ս��ϵ��
	float QueryPropertyBARate(IGameObj* pSelfObj, const char* strProName);
public:
	static BattleAbilityModule* m_pInstance;
private:
	ValueBADataVec		m_vecValueBAData;		// ����ֵս������
	RateBADataVec		m_vecRateBAData;		// �ٷֱ�ս������
	FormulaBADataMap	m_mapFormulaBAData;		// ��ʽս������
};
#endif // __BATTLE_ABILITY_MODULE_H__