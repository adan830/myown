//--------------------------------------------------------------------
// �ļ���:		IPubLogic.h
// ��  ��:		�������ݷ������߼��������ݿռ�ӿ�
// ˵  ��:		
// ��������:	2014��06��10��
// ��������:	2014��06��10��
// ������:		  ( )
// �޸���:     
//    :	    
//--------------------------------------------------------------------

#ifndef __IPubLogic_h__
#define __IPubLogic_h__

#include "../../server/IPubKernel.h"

class IPubLogic
{
public:
	/// \brief �������ݿռ�����
	virtual const char * GetName() = 0;

	/// \brief �������ݷ���������
	/// \param pPubKernel ����ָ�룬��ͬ
	virtual int OnPublicStart(IPubKernel* pPubKernel) = 0;

	/// \brief �������ݷ������ر�
	/// \param pPubKernel ����ָ�룬��ͬ
	virtual int OnServerClose(IPubKernel* pPubKernel) {return 0;}

	/// \brief �������ݿռ��е�������������
	/// \param pPubKernel ����ָ�룬��ͬ
	virtual int OnPubSpaceLoaded(IPubKernel* pPubKernel) = 0;

	/// \brief ����������������
	/// \param data_name ������������
	virtual int OnPubDataLoaded(IPubKernel* pPubKernel, const wchar_t * data_name) = 0;

	/// \brief ���յ���������������Ϣ
	/// \param source_id ����Ϣ�ķ�������ʶ
	/// \param msg ��Ϣ����
	virtual int OnPublicMessage(IPubKernel* pPubKernel, int source_id, 
		int scene_id, const IVarList& msg) = 0;

public:
	// ��������
	virtual bool OnUpateConfig(IPubKernel* pPubKernel, int source_id, 
		int scene_id, const IVarList& msg){ return false;};
	// for command
	virtual bool OnPublicCommand(IPubKernel* pPubKernel, int cmd_id, const IVarList& args) { return false; }

};

#endif //__IPubLogic_h__