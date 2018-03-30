//--------------------------------------------------------------------
// �ļ���:      FunctionEventModule.h
// ��  ��:      ��Ϸ��������ģ��
// ˵  ��:
// ��������:    2017��02��10��
// ������:      ���
//--------------------------------------------------------------------

#ifndef _FUNCTION_EVENT_MODULE_H_
#define _FUNCTION_EVENT_MODULE_H_

#include "Fsgame/Define/header.h"
#include "FsGame/Define/FunctionEventDefine.h"

/* ----------------------
 * �¼���ʱ����
 * ----------------------*/
class FunctionEventModule : public ILogicModule
{
public:
    virtual bool Init(IKernel *pKernel);
    virtual bool Shut(IKernel *pKernel);

	// ��ѯ������Ʒ��״̬
	static int	GetItemBindState(int nFunctionId);
	// ��ѯ��Ʒ�Ƿ�����ʹ��
	static bool	GetItemBindPriorUse(int nFunctionId);

private:
	// ���¼�������
	static void ReloadConfig(IKernel *pKernel);

private:
#pragma pack(push, 1) 
	struct FuncInfo
	{
		bool bind_state;
		bool bind_prior_use;
	};
#pragma pack(pop)
	static FuncInfo	ms_funcInfo[FUNCTION_EVENT_ID_END];
};

#endif // _FUNCTION_EVENT_MODULE_H_
