//--------------------------------------------------------------------
// �ļ���:      LevelModule.h
// ��  ��:      ������ȼ����ģ��
// ˵  ��:
// ��������:    2014��10��17��
// ������:       
//    :       
//--------------------------------------------------------------------
#ifndef FSGAME_COMMON_LEVEL_MODULE_H_
#define FSGAME_COMMON_LEVEL_MODULE_H_

#include "Fsgame/Define/header.h"
#include "FsGame/Define/LevelDefine.h"
#include <vector>

class LevelModule : public ILogicModule
{
public:
	//��ʼ��
	virtual bool Init(IKernel* pKernel);

	//�ͷ�
	virtual bool Shut(IKernel* pKernel);

	// �ͻ�������ȼ������Ϣ
	static int  OnCustomMsgLevelLock(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);
public:
	/// \brief ��ӽ�ɫ�ľ��麯��
	/// \param pKernel ���ܽӿ�ָ��
	/// \param self ����
	/// \param type �����������
	/// \param exp ��Ӿ���ֵ
	bool AddExp(IKernel* pKernel, const PERSISTID& self, int nSrcFrom, int64_t exp);

	/// \brief ��ȡ��ɫ�������辭�麯��
	/// \param pKernel ���ܽӿ�ָ��
	/// \param self ����
	/// \param level �ȼ�
	int64_t GetLevelUpgradeExp(IKernel* pKernel, const PERSISTID& self, int level);

	// ��ȡ������Ϣ
	bool LoadResource(IKernel* pKernel);

	// ȡ�ý�ɫ�ȼ�����ֵ
	int GetPlayerMaxLevel(IKernel* pKernel);
	/// \brief �������Ӿ���ֵ
	/// \param pKernel ���ܽӿ�ָ��
	/// \param self ����
	/// \param exp ��Ӿ���ֵ
	static bool TryAddExp(IKernel* pKernel, const PERSISTID& self, int64_t exp);
private:
	//------------------------------�ص�----------------------------------
	//�ȼ��ı�ص�
	static int C_OnLevelChanged(IKernel* pKernel, const PERSISTID& self, const char* property,
		const IVar& old);

	//ˢ���������辭��
	static int OnRecover(IKernel* pKernel, const PERSISTID& self,
		const PERSISTID& sender, const IVarList& args);
private:
	// ����
	bool LevelUp(IKernel* pKernel, const PERSISTID& self, int newlevel, int64_t curExp);

	// ˢ�µȼ��仯�������
	bool RefreshLevelUp(IKernel* pKernel, const PERSISTID& obj, const int old_level, int curLevel);

    // ���������Ϣ��¼
	void SaveLevelUpInfo(IKernel* pKernel, const PERSISTID& self, const int oldLevel, const int newLevel);

	static void ReloadConfig(IKernel* pKernel);
public:
	static LevelModule* m_pLevelModule;

private:         
	//��ɫ��߼���
	int m_nMaxLevel;

	//�����������辭��
	std::vector<int64_t> m_vecExpToUpgrade;
};

#endif // FSGAME_COMMON_LEVEL_MODULE_H_
