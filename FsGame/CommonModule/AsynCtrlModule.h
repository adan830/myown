

#ifndef __AsynCtrlModule_H__
#define __AsynCtrlModule_H__

#include "Fsgame/Define/header.h"
#include <map>

#define ASYNCTRLMODULE_CUSTOM_STRING_1       "ui_form_asyn_ctrl_module_1"   // ���볡������ȼ�����{0:EnterLevel}

// ��¡����
const static int CLONE_SCENE = 1024; 

// �����첽��ö��
enum ASYN_TYPE
{
	ASYN_TYPE_SWITCHLOCATE,							// ����
};

class AsynCtrlModule : public ILogicModule
{
public:
	// ��ʼ��
	virtual bool Init(IKernel* pKernel);

	// �ͷ�
	virtual bool Shut(IKernel* pKernel);

	// ���볡��
	static int OnReady(IKernel * pKernel, const PERSISTID & self, 
		const PERSISTID & sender, const IVarList & args);

	//
	static int OnContinue(IKernel * pKernel, const PERSISTID & self, 
		const PERSISTID & sender, const IVarList & args);

public:
	// �Ƿ�Ⱥ��첽�����Ĺ�����
	bool IsWaiting(IKernel* pKernel, const PERSISTID& self);

	// ��ʼ�Ⱥ��첽�����Ĺ���
	bool BeginWait(IKernel* pKernel, const PERSISTID& self, int asyn_type);

	// �����Ⱥ��첽�����Ĺ���
	bool EndWait(IKernel* pKernel, const PERSISTID& self);

	// �Ƿ���л�����DestSceneIDĿ�곡��ԭ����
	bool CanSwitchLocate(IKernel* pKernel, const wchar_t* selfName, const int DestSceneID);

	// �����л�
	bool SceneToScene(const int curScene, const int targetScene);

	//*********************�����г���*****************************
	// �л�������ָ���ĳ����ź�����
	//bFlag=true ǿ�д���
	bool SwitchLocate(IKernel* pKernel, const PERSISTID& self, int scene, 
		float x, float y, float z, float orient, const bool bFlag = false);

	// �л��������ĳ�����
	//bFlag=true ǿ�д���
	bool SwitchBorn(IKernel* pKernel, const PERSISTID& self, int scene, const bool bFlag = false);

	// �ƶ�����ǰ����������
	bool MoveToBorn(IKernel* pKernel, const PERSISTID& self, int scene);

private:
	// �첽�л�����������
	static int H_Asyn_SwitchLocate(IKernel* pKernel, const PERSISTID& self, int slice);

	static int H_Asyn_SwitchBorn(IKernel* pKernel, const PERSISTID& self, int slice);

public:
	// ������Դ
	bool LoadResource(IKernel *pKernel);

private:
	// ��ͻ������Ϣkey��ǰ����ID,Ԫ��Ϊ��ͻ����ID
	std::map<int, std::string> m_MapConflictScene;

public:
	static AsynCtrlModule* m_pAsynCtrlModule;

};

#endif	// __AsynCtrlModule_H__