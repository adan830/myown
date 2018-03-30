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

#include <string>
#include <map>
#include "public/Var.h"
#include "server/IPubKernel.h"
#include "utils/EnvirDefine.h"

/* ----------------------
 * �¼���ʱ����
 * ----------------------*/
class EnvirValue
{
public:
    static bool Init(IPubKernel *pKernel);

	// ��ѯint���� type ��Ӧ EnvirValueTypes
	static int	EnvirQueryInt(int type);

	// ��ѯfloat���� type ��Ӧ EnvirValueTypes
	static float EnvirQueryFloat(int type);

	// ��ѯstring���� type ��Ӧ EnvirValueTypes
	static const char*	EnvirQueryString(int type);
private:
	static CVar	m_arrEnvirData[ENVIR_VALUE_MAX];
};

#endif // _EVENT_TIMER_MODULE_H_
