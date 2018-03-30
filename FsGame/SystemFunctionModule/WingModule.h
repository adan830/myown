//--------------------------------------------------------------------
// �ļ���:      WingModule.h
// ��  ��:      ���ģ��
// ˵  ��:
// ��������:    2018��3��15��
// ������:      ������ 
//    :       
//--------------------------------------------------------------------
#ifndef _WING_MODULE_H_
#define _WING_MODULE_H_

#include "Fsgame/Define/header.h"
#include <vector>
#include "CommonModule/CommRuleDefine.h"

class CapitalModule;
class ContainerModule;

class WingModule : public ILogicModule
{
public:
	//��ʼ��
	virtual bool Init(IKernel* pKernel);

	//�ͷ�
	virtual bool Shut(IKernel* pKernel){ return true; }

	// �ͻ��������Ϣ
	static int  OnCustomWingMsg(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);

	// ��ѯ��ҵ����԰�
	int QueryWingLvPropId(IKernel* pKernel, const PERSISTID& self);
private:
	struct LevelData 
	{
		Consume_Vec		vecConsumeCapital;	// �������ĵ�Ǯ
		Consume_Vec		vecConsumeItem;		// ���ĵ���Ʒ
		int				nPropertyPak;		// ���԰�id
	};
	
	typedef std::vector<LevelData> LevelDataVec;

	struct StepData
	{
		Consume_Vec		vecConsumeItem;		// ���ĵ���Ʒ
		std::string		strProbability;		// ���ʹ�ʽ
		int				nMinBless;			// ʧ�ܻ�õ���Сף��ֵ
		int				nMaxBless;			// ʧ�ܻ�õ����ף��ֵ
		int				nMaxWingLv;			// �����������ȼ�
		int				nStepUpBlessVal;	// ������Ҫ���ף��ֵ
		int				nWingModel;			// �������
	};

	typedef std::vector<StepData> StepDataVec;

	enum 
	{
		MAX_BLESS_VALUE = 100,			// ���ף��ֵ(���ʻ���)
	};
private:
	//�ص����� ��Ҽ����������
	static int OnPlayerRecover(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);

	static int OnPlayerReady(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);

	// ��Ӧ����ܼ���
	static int OnCommandActivateWing(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);

	// ��Ӧ�������
	static int OnWingLevelUp(IKernel* pKernel, const PERSISTID& self, const PERSISTID& item, const IVarList& args);

	// ��������ף��ֵ
	static int ResetStepBlessVal(IKernel *pKernel, const PERSISTID &self, int slice);

	// ��Ӧ�������
	void OnCustomLevelUp(IKernel* pKernel, const PERSISTID& self, const IVarList& args);

	// ��Ӧ�������
	void OnCustomStepUp(IKernel* pKernel, const PERSISTID& self, const IVarList& args);

	// ��ȡ����
	bool LoadResource(IKernel* pKernel);

	// ��ȡ��������
	bool LoadLevelResource(IKernel* pKernel);

	// ��ȡ��������
	bool LoadStepResource(IKernel* pKernel);

	// ���������������
	bool CheckUpgradeTerm(IKernel* pKernel, IGameObj* pSelfObj);

	// ���ĳ����������Ʒ�ͽ�Ǯ
	bool LevelCostPlayerAssets(IKernel* pKernel, const PERSISTID& self, int nLevel);

	// ������ҳ������
	bool UpdateWingModifyProp(IKernel* pKernel, const PERSISTID& self);

	// ��ѯ��������
	const StepData* QueryStepData(int nStep);

	// ��ѯ��������
	const LevelData* QueryLevelData(int nLevel);

	static void ReloadConfig(IKernel* pKernel);
public:
	static WingModule* m_pWingModule;

private:         
	LevelDataVec	m_vecLevelData;		// ���ȼ�����
	StepDataVec		m_vecStepData;		// �����������

	static CapitalModule* m_pCapitalModule;
	static ContainerModule* m_pContainerModule;
};

#endif // _WING_MODULE_H_
