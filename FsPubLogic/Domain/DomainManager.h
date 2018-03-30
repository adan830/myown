//--------------------------------------------------------------------
// �ļ���:		DomainManager.h
// ��  ��:		�������ݷ������߼�ģ��:ȫ�ֹ������ݵĹ���
// ˵  ��:		
// ��������:	2014��06��10��
// ��������:	2014��06��10��
// ������:		  ( )
// �޸���:     
//    :	    
//--------------------------------------------------------------------

#ifndef __PubDomainManager_h__
#define __PubDomainManager_h__

#include "IDomainData.h"
#include "../Public/IPubLogic.h"
#include <string>
#include "../../FsGame/Define/PubDefine.h"
#include <unordered_map>

class DomainManager : public IPubLogic
{
public:
	DomainManager();
	virtual ~DomainManager();

	static IPubLogic* Instance();

	/// \brief �������ݿռ�����
	virtual const char * GetName(){return PUBSPACE_DOMAIN;}

	/// \brief �������ݷ���������
	/// \param pPubKernel ����ָ�룬��ͬ
	virtual int OnPublicStart(IPubKernel* pPubKernel);

	/// \brief �������ݷ������ر�
	/// \param pPubKernel ����ָ�룬��ͬ
	virtual int OnServerClose(IPubKernel* pPubKernel);

	/// \brief ����������������
	/// \param data_name ������������
	virtual int OnPubDataLoaded(IPubKernel* pPubKernel, const wchar_t * data_name);

	/// \brief �������ݿռ��е�������������
	/// \param pPubKernel ����ָ�룬��ͬ
	virtual int OnPubSpaceLoaded(IPubKernel* pPubKernel);

	/// \brief ���յ���������������Ϣ
	/// \param source_id ����Ϣ�ķ�������ʶ
	/// \param msg ��Ϣ����
	virtual int OnPublicMessage(IPubKernel* pPubKernel, int source_id, 
		int scene_id, const IVarList& msg);

public:
	// ��������
	virtual bool OnUpdateConfig(IPubKernel* pPubKernel, int source_id, 
		int scene_id, const IVarList& msg);

	/// \brief ���ָ�����Ƶ�DomainData
	/// \param data_name ����
	/// \param msg ��Ϣ����
	virtual IDomainData * FindDomainData(const wchar_t * data_name);

	// for command
	virtual bool OnPublicCommand(IPubKernel* pPubKernel, int cmd_id, const IVarList& args);
protected:
	typedef std::unordered_map<std::wstring, IDomainData*> MapData;
	MapData m_mapDataList;

private:
	static DomainManager* m_pInstance;
};

#endif //__PubDomainManager_h__
