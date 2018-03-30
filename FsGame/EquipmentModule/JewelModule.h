//--------------------------------------------------------------------
// �ļ���:      JewelModule.h
// ��  ��:      ��ʯģ��
// ˵  ��:		
// ��������:    2014��12��5��
// ������:        
// �޸���:        
//    :       
//--------------------------------------------------------------------
#ifndef JewelModule_h__
#define JewelModule_h__

#include "Fsgame/Define/header.h"
#include "JewelDefine.h"

class ContainerModule;
class CapitalModule;
class AsynCtrlModule;

// �������װ��ģ��
class JewelModule : public ILogicModule
{
public:
    // ��ʼ��
    virtual bool Init(IKernel* pKernel);

	// ������Դ
	bool LoadResource(IKernel* pKernel);

	// ���ر�ʯ����ӳ������
	bool LoadJewelIndexCfg(IKernel* pKernel);

    // �ͷ�
    virtual bool Shut(IKernel* pKernel);

public:
 
	// ��ʯ�����Ϣ
    static int OnCustomJewel(IKernel* pKernel, const PERSISTID& self,
        const PERSISTID& sender, const IVarList& args);

	// ��ȡ������ϴ��ڵ���ĳ���ȼ��ı�ʯ����(��Ӫ���)
	static int GetJewelsByLevel(IKernel* pKernel, const PERSISTID& self, const int level);

	// ���¼��ر�ʯ����
	static void ReloadJewelConfig(IKernel* pKernel);

public:

	// ��ʯ��Ƕ
	int ExecuteJewelInlay(IKernel* pKernel, const PERSISTID& self, const IVarList& args);

	// ժ�±�ʯ
	int DismantleJewel(IKernel* pKernel, const PERSISTID& self, const IVarList& args);

	// �ϳɱ�ʯ
	int ComposeJewel(IKernel* pKernel, const PERSISTID& self, const IVarList& args);

	// ժ��
	bool Dismantle(IKernel* pKernel, const PERSISTID& self, 
		const int equip_idx, const int jewel_idx);

private:
	//�����Ƕ�˵ı�ʯ����
	int GetExecuteJewelNum(IKernel*pKernel, const PERSISTID& self);

	// �Ƿ������Ʊ�ʯ
	bool JewelHaveSame(IKernel* pKernel, const PERSISTID& self, int equiprow, int jewelindex, int newType);

	// ָ����λ�Ƿ����ʹ��ָ�����ͱ�ʯ
	bool CanUseJewel(const int jewel_type, const int equip_pos);

	// ����������ȡ��ʯID
	const char* GetJewelID(const int index);

	// ͳ�ƺϳɲ���
	bool CountMateria(IKernel* pKernel, const IVarList &res, 
		const int target_level, JewelComposeConsume &compose_material, bool buy = false);

	// �۳���ʯ
	 bool DecJewels(IKernel *pKernel, const PERSISTID &self,
		const JewelMap &jewels, const int log_id);

private:

    static ContainerModule* m_pContainerModule;
    static CapitalModule* m_pCapitalModule;
    static AsynCtrlModule* m_pAsynCtrlModule;

	JewelUsePosVec m_JewelUsePosVec;
	JewelIndexMap m_JewelIndexMap;

public:
    static JewelModule* m_pInstance;
    

};
#endif // JewelModule_h__