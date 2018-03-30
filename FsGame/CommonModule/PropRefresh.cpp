//------------------------------------------------------------------------------
// �ļ���:      PropRefresh.cpp
// ��  ��:      ��������ˢ���߼�ʵ��
// ˵  ��:
// ��������:    2013��1��5��
// ������:       
// ��ע:
//    :       
//------------------------------------------------------------------------------

#include "FsGame/CommonModule/PropRefreshModule.h"

#include "utils/cau.h"
#include "utils/string_util.h"
#include "../../public/Inlines.h"

//#include "FsGame/Define/EquipDefine.h"

bool PropRefreshModule::RefreshData(IKernel* pKernel, const PERSISTID& obj, const MapPropValue& mapPropValue, ERefreshDataType eRefreshType)
{
    if (!pKernel->Exists(obj))
    {
        return false;
    }
    if (mapPropValue.empty())
    {
        return false;
    }

    IGameObj *pObj = pKernel->GetGameObj(obj);
    if(pObj == NULL)
    {
        return false;
    }

    //ְҵ
    int job = -1;
    if(pObj->FindAttr("Job"))
    {
        job = pObj->QueryInt("Job");
    }

    //����ˢ��װ����������
    MapPropValue::const_iterator it = mapPropValue.begin();
    LoopBeginCheck(a);
    for (; it != mapPropValue.end(); ++it)
    {
        LoopDoCheck(a);
        std::string strPropName = it->first;

        double value = it->second;
        if (EREFRESH_DATA_TYPE_REMOVE == eRefreshType)
        {
            value = -value;
        }
        
        // ��ѯ�Ƿ��������
        if (!pObj->FindAttr(strPropName.c_str()))
        {
            continue;
        }

        int type = pObj->GetAttrType(strPropName.c_str());
        switch (type)
        {
        case VTYPE_INT:
			{
				int oldValue = pObj->QueryInt(strPropName.c_str());
				pObj->SetInt(strPropName.c_str(), (int)value+oldValue);
			}
            break;
            
        case VTYPE_INT64:
            {
                int64_t oldValue = pObj->QueryInt64(strPropName.c_str());
                pObj->SetInt64(strPropName.c_str(), (__int64)value+oldValue);
            }
            break;

        case VTYPE_FLOAT:
			{
				float oldValue = pObj->QueryFloat(strPropName.c_str());
				pObj->SetFloat(strPropName.c_str(), (float)value+oldValue);
			}
            break;

        case VTYPE_DOUBLE:
            {
                double fOldValue = pObj->QueryDouble(strPropName.c_str());
                pObj->SetDouble(strPropName.c_str(), value + fOldValue);
            }
            break;
        default:
            break;
        }
    }

    return true;
}

// ���ˢ�����ݣ�������ְҵ���������
bool PropRefreshModule::PlayerRefreshData(IKernel* pKernel, const PERSISTID& obj, const MapPropValue& mapPropValue, ERefreshDataType eRefreshType)
{
	if (!pKernel->Exists(obj))
    {
        return false;
    }
    if (mapPropValue.empty())
    {
        return false;
    }

    IGameObj *pObj = pKernel->GetGameObj(obj);
    if(pObj == NULL)
    {
        return false;
    }

    //����ˢ��װ����������
    MapPropValue::const_iterator it = mapPropValue.begin();
    LoopBeginCheck(a);
    for (; it != mapPropValue.end(); ++it)
    {
        LoopDoCheck(a);
        std::string strPropName = it->first;

        double value = it->second;
        if (EREFRESH_DATA_TYPE_REMOVE == eRefreshType)
        {
            value = -value;
        }
        
        // ��ѯ�Ƿ��������
        if (!pObj->FindAttr(strPropName.c_str()))
        {
            continue;
        }

        int type = pObj->GetAttrType(strPropName.c_str());
        switch (type)
        {
        case VTYPE_INT:
			{
				int oldValue = pObj->QueryInt(strPropName.c_str());
				pObj->SetInt(strPropName.c_str(), (int)value+oldValue);
			}
            break;
            
        case VTYPE_INT64:
            {
                int64_t oldValue = pObj->QueryInt64(strPropName.c_str());
                pObj->SetInt64(strPropName.c_str(), (__int64)value+oldValue);
            }
            break;

        case VTYPE_FLOAT:
			{
				float oldValue = pObj->QueryFloat(strPropName.c_str());
				pObj->SetFloat(strPropName.c_str(), (float)value+oldValue);
			}
            break;

        case VTYPE_DOUBLE:
            {
                double fOldValue = pObj->QueryDouble(strPropName.c_str());
                pObj->SetDouble(strPropName.c_str(), value + fOldValue);
            }
            break;
        default:
            break;
        }
    }

    return true;
}

bool PropRefreshModule::CountRefreshData(IKernel* pKernel, const PERSISTID& obj,
                                         const char* szPropName, double dValue,
                                         int nComputeType, MapPropValue& mapPropValue)
{
    if (!pKernel->Exists(obj))
    {
        return false;
    }

	if (DoubleEqual(dValue, 0.0))
	{
		return true;
	}

    //�������Ҫ��������ֵ��¼����
    if (EModify_ADD_RATE == nComputeType)
    {
        //���ٷֱȼӵ���Ҫ�����ʵ����Ҫ�Ӷ���
        dValue = Cau::ModifyRateValue(pKernel, obj, dValue, szPropName);
    }

    std::map<std::string, double>::iterator it = mapPropValue.find(szPropName);
    if (it != mapPropValue.end())
    {
        //�о��ۼ�
        (*it).second = (*it).second + dValue;
    }
    else
    {
        //û�оͲ���
        mapPropValue.insert(std::make_pair(szPropName, dValue));
    }
    return true;
}