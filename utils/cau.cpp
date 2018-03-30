#include "utils/cau.h"
#include "utils/exptree.h"
#include "utils/string_util.h"
#include "utils/util_func.h"
#include "utils/extend_func.h"

#include <cmath>


//ֻ��ȡָ�����Ե�ֵ
float Cau::QueryValueByPropName(IKernel* pKernel, const PERSISTID& obj, const char* szPropName)
{
    IGameObj* pGameObj = pKernel->GetGameObj(obj);

    return QueryValueByPropName(pGameObj, szPropName);
}

// ֻ��ȡָ�����Ե�ֵ
float Cau::QueryValueByPropName(IGameObj* pGameObj, const char* szPropName)
{
	if (NULL == pGameObj)
	{
		return 0.0f;
	}
	if (StringUtil::CharIsNull(szPropName))
	{
		return 0.0f;
	}
	if (!pGameObj->FindAttr(szPropName))
	{
		return 0.0f;
	}

	float fValue = 0.0f;
	int type = pGameObj->GetAttrType(szPropName);
	switch (type)
	{
	case VTYPE_INT:
		fValue = (float)pGameObj->QueryInt(szPropName);
		break;
	case VTYPE_INT64:
		fValue = (float)pGameObj->QueryInt64(szPropName);
		break;
	case VTYPE_FLOAT:
		fValue = pGameObj->QueryFloat(szPropName);
		break;
	case VTYPE_DOUBLE:
		fValue = (float)pGameObj->QueryDouble(szPropName);
		break;
	default:
		break;
	}

	return fValue;
}	

//��ȡ���������ֵ
float Cau::QueryPropValue(IKernel* pKernel, const PERSISTID& obj, const char* szPropName)
{
    std::string strPropAddName = szPropName;
    strPropAddName += "Add";

    float fPropValue = QueryValueByPropName(pKernel, obj, szPropName);
    float fPropAddValue = QueryValueByPropName(pKernel, obj, strPropAddName.c_str());
    return fPropValue+fPropAddValue;
}

//���ش��������ֵ
const char* Cau::GetInputParamValue(int (&src)[MAX_INPUT_PARAM], size_t nParamIndex, const IVarList& InputParam)
{
    //������Դ����ֵ
    if (nParamIndex > MAX_INPUT_PARAM)
    {
        nParamIndex = MAX_INPUT_PARAM;
    }

    //�������������
    size_t nInputIndex = 0;

    //�������Դ�Ǵ���ģ��������������ֵ��1
    for (size_t i = 0; i < nParamIndex; ++i)
    {
        if (src[i] == PARAM_TYPE_INPUT || src[i] == PARAM_TYPE_INPUT_RANDOM)
        {
            ++nInputIndex;
        }
    }

    if (nInputIndex < InputParam.GetCount())
    {
        return InputParam.StringVal(nInputIndex);
    }

    //û����ô�ഫ�����
    return "";
}

//ȡ���ַ�����ʾ���������ֵ
float Cau::GetRandomValue(const char* param)
{
    float result = 0.0f;

    if (param == NULL)
    {
        return result;
    }

    if (::strchr(param, ':') == NULL)
    {
        //ȡ0��ʼ�������
        result = util_random_float(convert_float(param, 0.0f));
    }
    else
    {
        CVarList range;

        //�зֺŷָ����������Χ
        util_split_string(range, param, ":", 2);

        //ȡ��Χ�ڵ������
        float min = convert_float(range.StringVal(0), 0.0f);
        float max = convert_float(range.StringVal(1), 0.0f);

        if (max < min)
        {
            //ת��Ϊ�Ϸ��ķ�Χ
            float temp = min;
            min = max;
            max = temp;
        }

        result = min + util_random_float(max - min);
    }

    return result;
}


//��������ŵĸ��ӹ�ʽ
float Cau::GetFormulaValue(const std::string& szFormula, const float(&param)[MAX_INPUT_PARAM])
{
    ExpTree exp;
    float fvalue = exp.CalculateParam(szFormula.c_str(), param);
    return fvalue;
}

double Cau::ModifyRateValue(IKernel* pKernel, const PERSISTID& obj, double dRate, const char* szPropName)
{
    if (NULL == szPropName || '\0' == *szPropName)
    {
        return 0.0;
    }

    IGameObj* pObj = pKernel->GetGameObj(obj);
    if (NULL == pObj)
    {
        return 0.0;
    }

    if (!pObj->FindAttr(szPropName))
    {
        return 0.0;
    }

    double dValue = 0.0;
    int nType = pObj->GetAttrType(szPropName);
    //���ٷֱȼӵ���Ҫ�����ʵ����Ҫ�Ӷ���
    switch (nType)
    {
    case VTYPE_INT:
        dValue = (dRate / 100) * (double)pObj->QueryInt(szPropName);
        break;
    case VTYPE_FLOAT:
        dValue = (dRate / 100) * (double)pObj->QueryFloat(szPropName);
        break;
    case VTYPE_DOUBLE:
        dValue = (dRate / 100) * pObj->QueryDouble(szPropName);
        break;
    default:
        break;
    }

    return dValue;
}
