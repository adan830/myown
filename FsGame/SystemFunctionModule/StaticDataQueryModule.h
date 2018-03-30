//--------------------------------------------------------------------
// �ļ���:      StaticDataQueryModule.h
// ��  ��:      ʵ�����Բ�ѯ
// ˵  ��:
// ��������:    2010��6��21��
// ������:      huangmw@snailgame.net
//    :       
//--------------------------------------------------------------------
#ifndef STATICDATA_QUERY_MODULE_H
#define STATICDATA_QUERY_MODULE_H

class IVarList;
class CXmlFile;

#include "Fsgame/Define/header.h"
#include "utils/StringPod.h"
#include "utils/XmlFile.h"
#include <vector>
#include <string>

#define INI CXmlFile
#define CONVERT_INT convert_int
#define STATIC_DATA_FILE "ini/SystemFunction/static_data.xml";

class StaticDataQueryModule: public ILogicModule
{
public:
    // ��ʼ��
    virtual bool Init(IKernel* pKernel);

    // �ر�
    virtual bool Shut(IKernel* pKernel);

    //���¼���ָ���ľ�̬���Ա�
    bool ReloadStaticData(const int iClassType);

    // ֱ��ͨ�������Բ飬���ֻ�ǵ��β�ѯ����ֱ���������鼴��
    // ��ѯ�������Ƿ���ڱ���,ʧ�ܷ���-1, �ɹ���������ţ���0��ʼ��
    int FindCol(const int iClassType, const char* pColName);

    // ��ȡ����������
    int GetType(const int iClassType, const char* pColName);

    // ��ѯ��������
    const char* GetColName(const int iClassType, const int nCol);

    // ��ѯ��������
    int QueryInt(const int iClassType, const std::string&  strConfigID, const char* pColName);
    int64_t QueryInt64(const int iClassType, const std::string&  strConfigID, const char* pColName);

    // ��ѯ���ݸ�����
    float QueryFloat(const int iClassType, const std::string& strConfigID, const char* pColName);

    // ��ѯ�����ַ�����,ʧ�ܷ���""���ɹ������ַ���ָ�룬������Ϊ�Ǿ�̬��
    // ��һ��Ҫ����const std::string&,ֻҪ��Ҫ�ı����ݼ���
    const char* QueryString(const int iClassType, const std::string& strConfigID, const char* pColName);

    // ֱ��ͨ������Ų�ѯ����,��β�ѯʱ��������������Ż���ʡȥMAP�й���
    // ��ȡ����������
    int GetType(const int iClassType, const std::string& strConfigID, const int col);

    // ��ѯ��������
    int QueryInt(const int iClassType, const std::string& strConfigID, const int col);
    int64_t QueryInt64(const int iClassType, const std::string& strConfigID, const int col);

    // ��ѯ���ݸ�����
    float QueryFloat(const int iClassType, const std::string& strConfigID, const int col);

    // ��ѯ�����ַ�����,ʧ�ܷ���NULL���ɹ������ַ���ָ�룬������Ϊ�Ǿ�̬��
    // ��һ��Ҫ����const std::string&,ֻҪ��Ҫ�ı����ݼ���
    const char* QueryString(const int iClassType, const std::string& strConfigID, const int col);

    //��ѯ��ǰ���ݶ���������count
    virtual int GetDataCount(const int iClassType);

    //��ѯ��ǰ���ݵ��������
    virtual size_t GetMaxRows(const int iClassType);

    //��̬���е���������
    struct StaticData;
    //��ȡ�����ļ��е�һ������,����������Ϊ���ֵ
    bool GetOneRowInData(const int iClassType, const std::string& strConfigID, StaticData** result, size_t& resultnum);

	//��������ļ��е�һ������,�����CVarList��
	bool GetOneRowData(const int iClassType, const std::string& strConfigID, IVarList& propNameList, IVarList& propValueList, int defType = VTYPE_INT);

	//��������ļ��е�һ������,�����CVarList��
	bool GetOneRowDataStr(const int iClassType, const std::string& strConfigID, IVarList& propNameList, IVarList& propValueList);

    //��ȡ�����ļ������������кŵ�������Ϣ
    const TStringPod<char, size_t>* GetColNameToColIndex(const int iClassType);


    //����////////////////////////////////////////////////////////////
    // ֱ��ͨ������Ų�ѯ����,��β�ѯʱ��������������Ż���ʡȥMAP�й���
    // ��ȡ����������
    int GetType(const int iClassType, int strConfigID, const int col);

    // ��ѯ��������
    int QueryInt(const int iClassType, int  strConfigID, const char* pColName);
    int64_t QueryInt64(const int iClassType, int  strConfigID, const char* pColName);

    // ��ѯ���ݸ�����
    float QueryFloat(const int iClassType, int strConfigID, const char* pColName);

    // ��ѯ�����ַ�����,ʧ�ܷ���""���ɹ������ַ���ָ�룬������Ϊ�Ǿ�̬��
    // ��һ��Ҫ����const std::string&,ֻҪ��Ҫ�ı����ݼ���
    const char* QueryString(const int iClassType, int strConfigID, const char* pColName);


    // ��ѯ��������
    int QueryInt(const int iClassType, int strConfigID, const int col);
    int64_t QueryInt64(const int iClassType, int strConfigID, const int col);

    // ��ѯ���ݸ�����
    float QueryFloat(const int iClassType, int strConfigID, const int col);

    // ��ѯ�����ַ�����,ʧ�ܷ���NULL���ɹ������ַ���ָ�룬������Ϊ�Ǿ�̬��
    // ��һ��Ҫ����const std::string&,ֻҪ��Ҫ�ı����ݼ���
    const char* QueryString(const int iClassType, int strConfigID, const int col);

public:
    //������Դ
    bool LoadResource();

private:


    //������Դ
    bool FreeResource();

    //����̬���Ա�
    bool FreeStaticData(const int iClassType);

    //���뾲̬���ݱ�
    bool LoadStaticData(const int iClassType, const char* path);

    //���붨�����������Ķ�
    bool LoadIndexDefineSection(const int iClassType, const char* path, const CXmlFile& ini);


    //���붨�����������͵Ķ�
    bool LoadColTypeDefineSection(const int iClassType, const char* path, const CXmlFile& ini, bool bIndex);

    //�����������ñ�����Ĭ��ֵ
    //bool SetDefaultVaule(StaticData &var, int type);

    //ȡ��ָ����ָ����ָ�����Ե�����
    bool GetData(const int iClassType, const std::string& strConfigID, const char* pColName, StaticData** ppVar, int data_type);

    //ȡ��ָ����ָ����ָ���е�����
    bool GetData(const int iClassType, const std::string& strConfigID, const int col, StaticData** ppVar, int data_type);

    //��ȡ��Դ·��
    const char* GetResourcePath();

    //����iClassType��m_StaticTables��������Ӧ��ϵ
    bool SetIndexUseClassType(int iClassType, size_t iIndex);

    //��ȡm_StaticTables��������
    bool GetIndexUseClassType(int iClassType, size_t& iIndex);

	static void ReloadConfig(IKernel *pKernel);
public:
    //��̬���е���������
    enum {_INVALID_VALUE = INT_MAX};
    struct StaticData
    {
        union
        {
            int iValue;
            int64_t i64Value;
            float fValue;
            char* pStrValue;
        };
        bool JudgeEffective() const
        {
            if (iValue != _INVALID_VALUE)
            {
                return true;
            }
            return false;
        }
    };
private:
    // ��̬���ݱ�
    struct StaticTable
    {
        StaticTable()
        {
            nColNum = 0;
            pColTypes = NULL;
            mapConfigIDToIndex.clear();
        }

        size_t nColNum;
        int* pColTypes;
        TStringPod<char, size_t> colIndexList; //�����ڱ���к�
        std::vector<StaticData*> vecRowData;

        std::map<std::string, size_t> mapConfigIDToIndex;//index_row��DataRows��������Ӧ��ϵ

        //����index_row��DataRows��������Ӧ��ϵ
        bool SetIndexUseRowIndex(const std::string& strConfigID, size_t iIndex)
        {
            std::map<std::string, size_t>::iterator itFind = mapConfigIDToIndex.find(strConfigID);
            if (itFind != mapConfigIDToIndex.end())
            {
                return false;
            }

            mapConfigIDToIndex[strConfigID] = iIndex;
            return true;
        }

        //��ȡDataRows��������
        bool GetIndexUseRowIndex(const std::string& strConfigID, size_t& iIndex)
        {
            std::map<std::string, size_t>::iterator itFind = mapConfigIDToIndex.find(strConfigID);
            if (itFind == mapConfigIDToIndex.end())
            {
                return false;
            }

            iIndex = itFind->second;
            return true;
        }
    };



private:
    std::vector<StaticTable*> m_vecStaticTable;

    //iClassType��m_StaticTables��������Ӧ��ϵ
    std::map<int, size_t> m_mapClassTypeToIndex;

public:
    static StaticDataQueryModule* m_pInstance;

    static IKernel* m_pKernel;
};

#endif
