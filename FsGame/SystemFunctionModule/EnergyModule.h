//--------------------------------------------------------------------
// �ļ���:		EnergyModule.h
// ��  ��:		����ģ��(HP,MP)
// ˵  ��:		
//				
// ��������:	2018��03��09��
// ��������:	
// ������:		liumf   
//--------------------------------------------------------------------

#ifndef __ENERGY_MODULE_H__
#define __ENERGY_MODULE_H__

#include "Fsgame/Define/header.h"
#include <vector>
#include "Define/GameDefine.h"

class EnergyModule : public ILogicModule
{
public:
	// ��ʼ��
	virtual bool Init(IKernel* pKernel);

	virtual bool Shut(IKernel* pKernel){ return true; }

	// ����HP MP
	static bool UpdateEnergy(IKernel* pKernel, const PERSISTID &self, EnergyType type, int64_t nChangeValue);

	// �Ƿ��㹻����
	static bool IsMPEnough(IKernel* pKernel, const PERSISTID &self, int nDecMP);
private:
	/// \brief ����
	static int OnPlayerRecover(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);

	static int OnPlayerReady(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);

	static int OnPlayerStore(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);

	// ���ö�ȡ
	bool LoadConfig(IKernel* pKernel);

	// �ظ�hp������
	static int HB_RestoreHP(IKernel * pKernel, const PERSISTID & self, int slice);
						
	// �ظ�mp������
	static int HB_RestoreMP(IKernel * pKernel, const PERSISTID & self, int slice);

	// ���¼�������
	static void ReloadConfig(IKernel* pKernel);

	// ����HP
	void UpdateHP(IKernel* pKernel, IGameObj* pSelfObj, int64_t nChangeValue);

	// ����MP
	void UpdateMP(IKernel* pKernel, IGameObj* pSelfObj, int64_t nChangeValue);
private:
	static EnergyModule* m_pEnergyModule;
};

#endif //__ENERGY_MODULE_H__
