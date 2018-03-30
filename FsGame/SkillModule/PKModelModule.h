//--------------------------------------------------------------------
// �ļ���:		PKModelModule.h
// ��  ��:		PKģʽ
// ˵  ��:		
//				
// ��������:	2017��07��24��
// ��������:	
// ������:		liumf   
//--------------------------------------------------------------------

#ifndef __PK_Model_MODULE_H__
#define __PK_Model_MODULE_H__

#include "Fsgame/Define/header.h"
#include <vector>

class PKModelModule : public ILogicModule
{
public:
	// ��ʼ��
	virtual bool Init(IKernel* pKernel);

	// �ر�
	virtual bool Shut(IKernel* pKernel);

	// �Ƿ�ɹ���
	bool static IsBeAttacked(IKernel* pKernel, const PERSISTID &self, const PERSISTID &target);

	// �Ƿ�Ϊ�ѷ�
	bool static IsFriendly(IKernel* pKernel, const PERSISTID &self, const PERSISTID &target);
private:
	struct ScenePKModel 
	{
		ScenePKModel() : nSceneId(0),
					nDefaultModel(0)
		{

		}
		int		nSceneId;			// ����id
		int		nDefaultModel;		// Ĭ������
	};

	typedef std::vector<ScenePKModel> ScenePKModelVec;
	/// \brief ����
	static int OnPlayerRecover(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);

	static int OnPlayerReady(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);

	// ��Ӧ�ͻ�����Ϣ
	static int OnCustomPKModel(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);

	// ��ӦPKģʽ�л�
	void OnCustomChangePKModel(IKernel* pKernel, const PERSISTID& self, const IVarList& args);

	// ��Ӧ����Ĭ��pkģʽ
	//void OnCustomSetDefaultModel(IKernel* pKernel, const PERSISTID& self, const IVarList& args);

	// ����Ĭ�ϵ�pkģʽ
	//void ResetDefaultPKModel(IKernel* pKernel, const PERSISTID& self);

	// ���¼�������
	static void ReloadConfig(IKernel* pKernel);
private:
	static PKModelModule* m_pPKModelModule;
	ScenePKModelVec		m_vecScenePKModel;	
};

#endif //__GAINT_MODULE_H__
