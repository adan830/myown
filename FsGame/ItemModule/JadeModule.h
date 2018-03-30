//--------------------------------------------------------------------
// �ļ���:      JadeModule.h
// ��  ��:      ����ģ��
// ˵  ��:
// ��������:    2018��3��23��
// ������:      ������ 
//    :       
//--------------------------------------------------------------------
#ifndef _JADE_MODULE_H_
#define _JADE_MODULE_H_

#include "Fsgame/Define/header.h"
#include <vector>
#include <map>
#include "CommonModule/CommRuleDefine.h"

class ContainerModule;
class ItemBaseModule;

class JadeModule : public ILogicModule
{
public:
	//��ʼ��
	virtual bool Init(IKernel* pKernel);

	//�ͷ�
	virtual bool Shut(IKernel* pKernel){ return true; }

	// �ͻ��������Ϣ
	static int  OnCustomJadeMsg(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);
private:
	// �׼�����
	struct StepData
	{
		int				nStepLv;			// �׼�
		Consume_Vec		vecConsumeItem;		// ������Ʒ
		int				nPieceNum;			// ������Ƭ����
		int				nJadeLvLimit;		// ����ȼ�����
		int				nSkillLv;			// ���弼�ܵȼ�
	};

	typedef std::vector<StepData> StepDataVec;

	// �ȼ�����
	struct LevelData
	{
		int				nLevel;				// �ȼ�
		Consume_Vec		vecConsumeItem;		// ������Ʒ
		int				nPackageId;			// ���԰�id
		int				nPlayerLvLimit;		// ��ҵȼ�����
	};

	typedef std::vector<LevelData> LevelDataVec;

	// ��������
	struct JadeMainData
	{
		std::string		strJadeId;			// ����id
		std::string		strPiece;			// ������Ƭ
		int				nPieceNum;			// ��Ƭ����
		std::string		strSkillid;			// ����id

		StepDataVec		vecStepData;		// ��������
		LevelDataVec    vecLevelData;		// ��������
	};

	typedef std::map<std::string, JadeMainData*> JadeMainDataMap;
	typedef std::map<std::string, JadeMainData*>::iterator JadeMainDataMapIter;

private:
	//�ص����� ��Ҽ����������
	static int OnPlayerRecover(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);

	static int OnPlayerReady(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);

	// ��Ӧ��������
	void OnCustomLevelUp(IKernel* pKernel, const PERSISTID& self, const IVarList& args);

	// ��Ӧ��������
	void OnCustomStepUp(IKernel* pKernel, const PERSISTID& self, const IVarList& args);

	// ��Ӧ��Ƭ�ϳ�����
	void OnCustomCompose(IKernel* pKernel, const PERSISTID& self, const IVarList& args);

	// ��Ӧ��������
	void OnCustomWear(IKernel* pKernel, const PERSISTID& self, const IVarList& args);

	// ��ȡ����
	bool LoadResource(IKernel* pKernel);

	// ��ȡ������
	bool LoadMainResource(IKernel* pKernel);

	// ��ȡ��������
	bool LoadStepResource(IKernel* pKernel);

	// ��ȡ��������
	bool LoadLevelResource(IKernel* pKernel);

	// ���������������
	void UpdateJadeModifyProp(IKernel* pKernel, const PERSISTID& self);

	// ��ѯ��������
	const StepData* QueryStepData(std::string strJadeId, int nStep);

	// ��ѯ��������
	const LevelData* QueryLevelData(std::string strJadeId, int nLevel);

	// ��ѯ��������
	const JadeMainData*	QueryJadeMainData(std::string strJadeId);

	// �����������
	void ClearMainData();

	static void ReloadConfig(IKernel* pKernel);
public:
	static JadeModule* m_pJadeModule;

private:         
	JadeMainDataMap	m_mapJadeMainData;	// ��������

	static ContainerModule* m_pContainerModule;
	static ItemBaseModule* m_pItemBaseModule;
};

#endif // _JADE_MODULE_H_
