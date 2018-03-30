//--------------------------------------------------------------------
// �ļ���:		ReviveModule.h
// ��  ��:		��������
// ˵  ��:		
//				
// ��������:	2018��03��13��
// ��������:	
// ������:		liumf   
//--------------------------------------------------------------------

#ifndef __REVIVE_MODULE_H__
#define __REVIVE_MODULE_H__

#include "Fsgame/Define/header.h"
#include <vector>
#include "Define/GameDefine.h"
#include <map>

class ReviveModule : public ILogicModule
{
public:
	// ��ʼ��
	virtual bool Init(IKernel* pKernel);

	virtual bool Shut(IKernel* pKernel){ return true; }
private:
	// �����������
	struct RevivePanelConfig_s
	{
		RevivePanelConfig_s() :m_sceneID(0),
			m_revivePanel("")
		{}
		int m_sceneID;                    // ԭ�ͳ���ID
		std::string m_revivePanel;        // �������
	};

	typedef std::map< int, RevivePanelConfig_s > RevivePanelConfigMap; // <ԭ�ͳ���ID, �����������>

	/// \brief ����
	static int OnPlayerRecover(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);

	static int OnPlayerReady(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);

	//���󸴻���Ϣ���� 
	static int OnCustomRevive(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& msg);

	// ��Ӧ���󸴻�
	void OnCustomRequestRevive(IKernel* pKernel, const PERSISTID& self, const IVarList& msg);

	// �������
	static int OnCommandBeKill(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);

	// ��������
	bool LoadConfig(IKernel* pKernel);

	// ���¼���
	static void ReLoadConfig(IKernel* pKernel);

	// ���ظ����������
	bool LoadRevivePanelConfig(IKernel* pKernel);

	//Ӧ������������
	RevivePanelConfig_s* GetRevivePanelConfig(IKernel* pKernel);

	// ԭ�ظ���
	void LocalRevive(IKernel* pKernel, const PERSISTID& self);

	// �ͽ�����
	void NearbyRevive(IKernel* pKernel, const PERSISTID& self);

	// ���и���
	void CityRevive(IKernel* pKernel, const PERSISTID& self);

	// �������
	void PlayerRevive(IKernel* pKernel, const PERSISTID& self);

	// У�鸴������
	bool CheckReviveType(IKernel* pKernel, const int reviveType);
private:
	static ReviveModule* m_pReviveModule;

	RevivePanelConfigMap  m_mapRevivePanelConfig;    //�����������
};

#endif //__ENERGY_MODULE_H__
