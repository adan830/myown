//--------------------------------------------------------------------
// �ļ���:      ActivateFunctionModule.h
// ��  ��:      ���ܿ���
// ˵  ��:
// ��������:    2018��3��21��
// ������:      ������ 
//    :       
//--------------------------------------------------------------------

#ifndef _ACTIVATE_FUNCTION_MODULE_H
#define _ACTIVATE_FUNCTION_MODULE_H

#include "Fsgame/Define/header.h"
#include "FsGame/Define/GameDefine.h"
#include "FsGame/Define/ClientCustomDefine.h"
#include<map>
#include<set>
#include<vector>

//���ܿ�����ʽ
enum ACTIVATETYPE
{
	ACTIVATE_FUNCTION_BY_LEVEL = 0,			// ���ݵȼ�����
    ACTIVATE_FUNCTION_BY_TASK,				// ����������
    ACTIVATE_FUNCTION_MAX,
};

// ��������
enum AFMFunction
{
	NONE_FUNCTION_TYPE = 0,
	AFM_WING_FUNCTION,		// �����
	AFM_BAPTISE,			// ϴ������
	AFM_FORGING,			// ���칦��
	AFM_SMELT,				// ��������
	AFM_DAILY_TASK,			// �ճ�����
	AFM_JADE_FUNCTION,		// ���幦��
	AFM_RIDE,				// ���﹦��
};

class ActivateFunctionModule:public ILogicModule
{
public:

  virtual bool Init(IKernel *pKernel);
  virtual bool Shut(IKernel* pKernel) ;

  //�鿴�����Ƿ񼤻�
  static bool CheckActivateFunction(IKernel *pKernel, const PERSISTID &self, const int& type);
private:
	// ���ܿ�������
	struct ActivateData
	{
		int nActivateType;		// ��������(ACTIVATETYPE)
		int nValue;				// ��������
		int	nFunctionType;		// ��������
	};

	typedef std::vector<ActivateData> ActivateDataVec;
	typedef std::vector<int> FuncVec;
	typedef std::vector<int>::iterator FuncVecIter;

	// ���ö�ȡ
	bool LoadRes(IKernel *pKernel);
	static void ReloadRes(IKernel*pKernel);
	//���񼤻��
	static int ActivateFunctionByTask(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);

	//�ȼ������
	static int ActivateFunctionByLevel(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);

	// �����
	void ActiveFunction(IKernel*pKernel, const PERSISTID&self, int nFuncType);

	//�������й���
	bool ActivateAllFunction(IKernel* pKernel, const PERSISTID& self);

	//������й���
	bool CloseAllFunction(IKernel* pKernel, const PERSISTID& self);

	// ��ѯ��������
	bool QueryFunctionType(std::vector<int>& outAcFunc, int nType, int nValue);

public:
	static ActivateFunctionModule* m_pInstance;

private:
	ActivateDataVec m_vecActivateData;		// ���ܿ�������
 
};









#endif
