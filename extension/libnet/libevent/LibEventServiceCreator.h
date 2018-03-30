//--------------------------------------------------------------------
// �ļ���:		LibEventServiceCreator.h
// ��  ��:		libevent������񴴽���
// ˵  ��:		
// ��������:		2016��5��20��
// ������:		���
// ��Ȩ����:		���ݾ����������޹�˾
//--------------------------------------------------------------------
#ifndef __LIB_EVENT_SERVICE_CREATOR_H__
#define __LIB_EVENT_SERVICE_CREATOR_H__

#include "LibEventService.h"
#include "LibEventHttpService.h"

class LibEventServiceCreator : public INetServiceCreator
{
public:

	virtual INetService* Create(INetIoDesc* pDesc, 
		INetIoHandler* pHandler, 
		INetConnectionFactory* pFactory, 
		INetActionPacker* packer, 
		INetFilter* pFilter)
	{
		LibEventService* service = new LibEventService(pDesc);
		service->SetHandler(pHandler)
			.SetFactory(pFactory)
			.SetPacker(packer)
			.SetFilter(pFilter);

		return service;
	}

	virtual void Free(INetService* service)
	{
		if (NULL != service)
		{
			delete service;
		}
	}
};

class LibEventHttpServiceCreator : public INetServiceCreator
{
public:

	virtual INetService* Create(INetIoDesc* pDesc,
		INetIoHandler* pHandler,
		INetConnectionFactory* pFactory,
		INetActionPacker* packer,
		INetFilter* pFilter)
	{
		LibEventHttpService* service = new LibEventHttpService(pDesc);
		service->SetHandler(pHandler)
			.SetFactory(pFactory)
			.SetPacker(packer)
			.SetFilter(pFilter);

		return service;
	}

	virtual void Free(INetService* service)
	{
		if (NULL != service)
		{
			delete service;
		}
	}

};

#endif // END __LIB_EVENT_SERVICE_CREATOR_H__