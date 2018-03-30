
#ifndef _CAU_H__
#define _CAU_H__

#include <string>
#ifdef FSROOMLOGIC_EXPORTS
#include "server/IRoomKernel.h"
#else
#include "server/IKernel.h"
#endif

namespace Cau
{
//�������Ͷ���
enum PARAM_TYPE
{
PARAM_TYPE_SELF = 0,            //�ͷ���
PARAM_TYPE_SKILL = 1,           //�ͷż���
PARAM_TYPE_TARGET = 2,          //Ŀ�����
PARAM_TYPE_TARGET_SKILL = 3,    //Ŀ�굱ǰ���ܶ���
PARAM_TYPE_RANDOM = 4,          //�����
PARAM_TYPE_INPUT = 5,           //ʹ�ô������
PARAM_TYPE_INPUT_RANDOM = 6,    //���������
PARAM_TYPE_EQUIP = 7,           //װ������
};


//��ʽ������������
const int MAX_INPUT_PARAM = 8;
//��������
//���㲻�����ŵļ򵥹�ʽ
float GetSimpleFormulaValue(const std::string& szFormula, const float(&param)[MAX_INPUT_PARAM]);

//��������ŵĸ��ӹ�ʽ
float GetFormulaValue(const std::string& szFormula, const float(&param)[MAX_INPUT_PARAM]);

//ȡ�ö��������ֵ
float QueryPropValue(IKernel* pKernel, const PERSISTID& obj, const char* szPropName);
// ֻ��ȡָ�����Ե�ֵ
float QueryValueByPropName(IKernel* pKernel, const PERSISTID& obj, const char* szPropName);

// ֻ��ȡָ�����Ե�ֵ
float QueryValueByPropName(IGameObj* pGameObj, const char* szPropName);

//ȡ�ô��������ֵ
const char* GetInputParamValue(int (&src)[MAX_INPUT_PARAM], size_t nParamIndex, const IVarList& InputParam);

//ȡ���ַ�����ʾ���������ֵ
float GetRandomValue(const char* param);

// ���Ȼ�ȡ����ֵ
double ModifyRateValue(IKernel* pKernel, const PERSISTID& obj, double dRate, const char* szPropName);

} // end of namespace Cau


#endif // _CAU_H__
