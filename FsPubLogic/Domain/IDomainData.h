//--------------------------------------------------------------------
// �ļ���:		IDomainData.h
// ��  ��:		�������ݷ������߼��������ݿռ�ӿ�
// ˵  ��:		
// ��������:	2014��06��10��
// ��������:	2014��06��10��
// ������:		  ( )
// �޸���:     
//    :	    
//--------------------------------------------------------------------

#ifndef __IPubDomainData_h__
#define __IPubDomainData_h__

#include "../../server/IPubKernel.h"
#include "../../public/Inlines.h"
#include <string>

class IDomainData
{
public:
	IDomainData(const std::wstring & domain_name, const int domain_id=0) :
	  m_domainName(domain_name), m_domainId(domain_id)
	  {
	  }

	  virtual ~IDomainData()
	  {
	  }

public:
	virtual const std::wstring & GetDomainName(IPubKernel * pPubKernel)
	{
		return m_domainName;
	}

	virtual int GetDomainId(IPubKernel * pPubKernel)
	{
		return m_domainId;
	}
	virtual bool GetSave() = 0;

	/// \brief �������ݿռ��е�������������
	/// \param pPubKernel ����ָ�룬��ͬ
	virtual int OnCreate(IPubKernel* pPubKernel, IPubSpace * pPubSpace) = 0;

	/// \brief ����������������
	/// \param data_name ������������
	virtual int OnLoad(IPubKernel* pPubKernel, IPubSpace * pPubSpace) = 0;

	/// \brief ���յ���������������Ϣ
	/// \param source_id ����Ϣ�ķ�������ʶ
	/// \param msg ��Ϣ����
	virtual int OnMessage(IPubKernel* pPubKernel, IPubSpace * pPubSpace,
		int source_id, int scene_id, const IVarList& msg) = 0;

public:
	//��������
	virtual bool OnUpdateConfig(IPubKernel* pPubKernel, IPubSpace * pPubSpace,
		int source_id, int scene_id, const IVarList& msg){return false;};
public:
	// for command
	virtual bool OnPublicCommand(IPubKernel* pPubKernel, int cmd_id, const IVarList& args) { return false; }
protected:
	std::wstring m_domainName;
	int m_domainId;
};

#endif //__IPubDomainData_h__
