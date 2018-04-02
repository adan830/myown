//---------------------------------------------------------
//�ļ���:       MapModule.h
//��  ��:       ��ͼ��ز���ģ��
//˵  ��:       
//
//��������:      2015��4��21��
//������:         
//�޸���:
//   :         
//---------------------------------------------------------

#ifndef _FSGAME_INTER_ACTIVE_MODULE_MAP_MODULE_H_
#define _FSGAME_INTER_ACTIVE_MODULE_MAP_MODULE_H_

#include "Fsgame/Define/header.h"
#include <vector>
#include <map>

class CapitalModule;
class AsynCtrlModule;
class MapModule: public ILogicModule
{
public:
	// ��ʼ��
	virtual bool Init(IKernel* pKernel);
	// ����
	virtual bool Shut(IKernel* pKernel);
private:
	// ���������ļ�
	bool LoadResource(IKernel* pKernel);

private:
	//���볡��
	static int OnPlayerEntry(IKernel * pKernel, const PERSISTID & self,
		const PERSISTID & sender, const IVarList & args);

	// ���տͻ�����Ϣ
	static int OnCustomCallBack(IKernel * pKernel, const PERSISTID & self,
		const PERSISTID & sender, const IVarList & args);

	// �����л�����
	static int OnCustomSwitchScene(IKernel * pKernel, const PERSISTID & self,
		const PERSISTID & sender, const IVarList & args);
	
	// �жϳ����Ƿ���ͨ
	// [out]int money �����ͨ���򷵻ؼ۸�
	bool CheckSceneThrough(int curSceneId, int toSceneId, int& money);

	// �����ܷ�ͨ����ͼ����
	bool CanThroughMap(int sceneId);

	static void ReLoadConfig(IKernel * pKernel);

private: // ��̬����
	static MapModule* m_pMapModule;
	static AsynCtrlModule* m_pAsynCtrlModule;

private:

#ifdef _DEBUG
private: // ����
	static int nx_map_switch_scene(void *state);
#endif
};

#endif // FSGAME_INTER_ACTIVE_MODULE_MAP_MODULE_H