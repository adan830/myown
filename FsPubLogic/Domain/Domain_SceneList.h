//--------------------------------------------------------------------
// �ļ���:      Domain_SceneList.h
// ��  ��:      ���б������ĳ����б�
// ˵  ��:		
// ��������:    2014��10��17��
// ������:        
// �޸���:        
//    :       
//--------------------------------------------------------------------

#ifndef __Domain_SceneList_h__
#define __Domain_SceneList_h__

#include "IDomainData.h"
#include "../../utils/extend_func.h"

class Domain_SceneList : public IDomainData
{
public:

	Domain_SceneList() : IDomainData(L"", 0) {}

	// ������������
	virtual std::wstring & GetDomainName(IPubKernel* pPubKernel) 
	{
		if (m_domainName.empty())
		{
			wchar_t wstr[256];
			int server_id = pPubKernel->GetServerId();
			SWPRINTF_S(wstr, L"Domain_SceneList%d", server_id);
			m_domainName = wstr;
		}

		return m_domainName;
	}

	//�Ƿ񱣴浽���ݿ�
	virtual bool GetSave(){return false;}

	virtual int OnCreate(IPubKernel* pPubKernel, IPubSpace * pPubSpace);

	virtual int OnLoad(IPubKernel* pPubKernel, IPubSpace * pPubSpace);

	/// \brief ���յ���������������Ϣ
	/// \param source_id ����Ϣ�ķ�������ʶ
	/// \param msg ��Ϣ����
	virtual int OnMessage(IPubKernel* pPubKernel, IPubSpace * pPubSpace,
		int source_id, int scene_id, const IVarList& msg);

	void CreateTable(IPubData * pSceneListData, const char * table_name);
};

#endif //__Domain_SceneList_h__