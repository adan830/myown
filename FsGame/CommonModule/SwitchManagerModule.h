//----------------------------------------------------------
// �ļ���:      SwitchManagerModule.h
// ��  ��:      ���ܿ��ض���
// ˵  ��:
// ������:        
// ��������:    2015��6��30��
//    :        
//----------------------------------------------------------
#ifndef __SWITCHMANAGER_MODULE_H_
#define __SWITCHMANAGER_MODULE_H_

#include "Fsgame/Define/header.h"
#include "FsGame/Define/SwitchManagerDefine.h"

class SwitchManagerModule : public ILogicModule
{
public:
    bool Init(IKernel* pKernel);
    bool Shut(IKernel* pKernel);

public:
    // ��⹦���Ƿ���
	static bool CheckFunctionEnable(IKernel* pKernel, const int function_id, const PERSISTID& self = PERSISTID());
    // ����״̬
    static bool SetFunctionEnable(IKernel* pKernel, const int function_id, const int newState);

	// ���ع������ݵ����ݿ�
	bool LoadResource(IKernel* pKernel);

	//��Ϣ����
	static bool GetSwitchFunctionInfo(IKernel* pKernel, const char* content);
private:
    const std::wstring GetDomainName(IKernel* pKernel);
    const CVarList& QueryPubFunctionState(IKernel* pKernel, const int function_id);

	// �ӳټ��������ļ� 
	static int CB_DomainDB(IKernel* pKernel, const PERSISTID& self, int slice);

	// ��������
	static int OnSceneCreate(IKernel* pKernel, const PERSISTID& scene, 
		const PERSISTID& sender, const IVarList& args);

	//���湦�ܿ������õ����ݿ�
	bool SaveSwitchConfigToDB(IKernel* pKernel, const int function_id,
		const int newState, const std::string& function_name);

	// ���ͻ�����ʾ��Ϣ
	static void SendCustomSysInfo(IKernel* pKernel, const PERSISTID& self, const int functin_id);
private:
    std::wstring m_DomainName;  // ������������
public:
    static SwitchManagerModule* m_SwitchManagerModule;
};
#endif //__SWITCHMANAGER_MODULE_H_