//--------------------------------------------------------------------
// �ļ���:      EnvirValueModule.h
// ��  ��:      �����еĳ���
// ˵  ��:
// ��������:    2016��03��09��
// ������:       
// �޸���:        
//    :       
//--------------------------------------------------------------------

#ifndef _ENVIR_VALUE_MODULE_H_
#define _ENVIR_VALUE_MODULE_H_

#include "Fsgame/Define/header.h"
#include <string>
#include <map>
#include "public/Var.h"
#include "utils/EnvirDefine.h"

/* ----------------------
 * �¼���ʱ����
 * ----------------------*/
class EnvirValueModule : public ILogicModule
{
public:
    virtual bool Init(IKernel *pKernel);
    virtual bool Shut(IKernel *pKernel);

	// ��ѯint���� type ��Ӧ EnvirValueTypes
	static int	EnvirQueryInt(int type);

	// ��ѯfloat���� type ��Ӧ EnvirValueTypes
	static float EnvirQueryFloat(int type);

	// ��ѯstring���� type ��Ӧ EnvirValueTypes
	static const char*	EnvirQueryString(int type);
private:
	// ���¼�������
	static void ReloadConfig(IKernel *pKernel);
private:
	static CVar	m_arrEnvirData[ENVIR_VALUE_MAX];
};

#endif // _EVENT_TIMER_MODULE_H_
