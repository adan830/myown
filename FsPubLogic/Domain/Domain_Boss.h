//--------------------------------------------------------------------
// �ļ���:      Domain_Boss.h
// ��  ��:      Boss��¼��
// ˵  ��:		
// ��������:    2018��03��30��
// ������:        
// �޸���:        
//    :       
//--------------------------------------------------------------------

#ifndef __Domain_Boss_h__
#define __Domain_Boss_h__

#include "IDomainData.h"
#include <vector>
#include <string>

class Domain_Boss : public IDomainData
{
public:
	Domain_Boss();
public:
	virtual const std::wstring & GetDomainName(IPubKernel * pPubKernel);
	virtual std::wstring GetName(IPubKernel* pPubKernel){return GetDomainName(pPubKernel);}

	virtual bool GetSave();
	virtual int OnCreate(IPubKernel * pPubKernel, IPubSpace * pPubSpace); 
	virtual int OnLoad(IPubKernel * pPubKernel, IPubSpace * pPubSpace);

	///  \brief ���յ���������������Ϣ
	///  \param source_id ����Ϣ�ķ�������ʶ
	///  \param msg ��Ϣ����
	virtual int OnMessage(IPubKernel * pPubKernel,
						  IPubSpace * pPubSpace,
						  int source_id,
						  int scene_id,
						  const IVarList & msg);
};

#endif	 // Domain_Boss
