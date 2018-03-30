#ifndef _SCENEBASE_MODULE_H_
#define _SCENEBASE_MODULE_H_

#include "Fsgame/Define/header.h"

#define DOMAIN_SCENELIST	L"Domain_SceneList"

class SceneBaseModule: public ILogicModule
{
public:
	// ��ʼ��
	virtual bool Init(IKernel* pKernel);

	// �ر�
	virtual bool Shut(IKernel* pKernel);
public:
	//����
	static int OnCreateClass(IKernel* pKernel, int index);
	
	//��������
	static int OnCreate(IKernel* pKernel, const PERSISTID& self, 
		const PERSISTID& sender, const IVarList& args);

	//��������
	static int OnDestroy(IKernel* pKernel, const PERSISTID& self, 
		const PERSISTID& sender, const IVarList& args);

	//���볡��
	static int OnEntry(IKernel *pKernel, const PERSISTID &self, const PERSISTID &sender, const IVarList &args);

	//�뿪����
	static int OnLeave(IKernel *pKernel, const PERSISTID &self, const PERSISTID &sender, const IVarList &args);

	// ��������Ҷ��߳���ǰ����(�ָ������볡��)
	static void KickAllPlayerOutScene( IKernel* pKernel, int nGroup );
	// �������з������صĶ���
	static void ClearNonPlayerInScene(IKernel* pKernel, int nGroup);
};

#endif // _SCENEBASE_MODULE_H_
