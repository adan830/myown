//---------------------------------------------------------
//�ļ���:       Domain_Look.h
//��  ��:       �鿴��Ϣ
//˵  ��: 
    
//��������: 
//������:            
//---------------------------------------------------------

#ifndef PUB_DOMAIN_SHOWOFF_H
#define PUB_DOMAIN_SHOWOFF_H

#include "IDomainData.h"
#include "FsGame/Define/PubDefine.h"

#include <string>

class Domain_Look : public IDomainData
{
public:

    Domain_Look();

    virtual ~Domain_Look();

public:

    // ������������
    virtual const std::wstring & GetDomainName(IPubKernel* pPubKernel);

    //�Ƿ񱣴浽���ݿ�
    virtual bool GetSave(){return true;}

    virtual int OnCreate(IPubKernel* pPubKernel, IPubSpace* pPubSpace);

    virtual int OnLoad(IPubKernel* pPubKernel, IPubSpace* pPubSpace);

    /// �������Գ�������������Ϣ
    virtual int OnMessage(IPubKernel* pPubKernel, IPubSpace* pPubSpace,
        int source_id, int scene_id, const IVarList& msg);

private:

    IRecord * GetShowoffRecord(IPubKernel* pPubKernel, IPubSpace* pPubSpace);

    // ������Ʒ��Ϣ������
    bool SaveShowoffItem(IPubKernel* pPubKernel, IPubSpace* pPubSpace,
        int source_id, int scene_id, const IVarList& msg);

    // �������չʾ����Ʒ��
    bool CreateShowoffRecord(IPubKernel* pPubKernel, IPubSpace* pPubSpace);

public:

    static Domain_Look* m_pShowoffInstance;

};

#endif