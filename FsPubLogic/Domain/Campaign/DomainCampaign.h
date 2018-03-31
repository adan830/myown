//--------------------------------------------------------------------
// �ļ���:      DomainCampaign.h
// ��  ��:      �����
// ˵  ��:		
// ��������:    2017��2��13��
// ������:      ������
// �޸���:        
// ��Ȩ����:      
//--------------------------------------------------------------------

#ifndef __DOMAIN_CAMPAIGN_H__
#define __DOMAIN_CAMPAIGN_H__

#include "../IDomainData.h"
#include <vector>
#include <string>

class DomainCampaignBase;																																					 

class DomainCampaign : public IDomainData
{
public:
	DomainCampaign() : IDomainData(L"")
	{
		m_bGmControl = false;
	}
	
public:
	virtual const std::wstring & GetDomainName(IPubKernel * pPubKernel);

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
private:
	enum
	{
		CHECK_STATE_INTERVAL = 5000,		// ���״̬Ƶ�� 
	};

	// ״̬����
	struct StateData 
	{
		int			nState;				// ״ֵ̬
		int			nStartTime;			// ״̬��ʼʱ��
		int			nEndTime;			// ״̬����ʱ��
	};

	typedef std::vector<StateData>	StateVec;	

	// �����
	struct CampaignData 
	{
		int					nCampaignId;		// �id
		int					nGameType;			// �淨����
		std::vector<int>	vecSceneId;			// �����
		std::vector<int>	vecWeek;			// �����
		StateVec			vecStateData;		// ״̬����
	};

	typedef std::vector<CampaignData>	CampaignDataVec;

	// ��ȡ����
	bool LoadResource(IPubKernel* pPubKernel);

	// ���״̬����
	static int CheckCampaignStateTimer(IPubKernel* pPubKernel, const char* space_name, const wchar_t* data_name, int time);

	// �жϵ�ǰ�Ļ״̬
	int JudgeCampaignState(int64_t& nEndTime, const CampaignData& camdata);

	// ��ѯ�����
	const CampaignData* QueryCampaignData(int nCampaignId);

	// �������ںͳ�������
	bool ParseXmlSection(std::vector<int>& vecOutData, const char* strData);

	// �����״̬����
	bool ParseCampaignState(StateVec& vecOutData, const char* strState, int nState);

	// ��Ӧ�״̬�ı�
	bool OnCampaignEnd(IPubKernel* pPubKernel, IPubData* pPubData, int nCampaignType);

	// ��Ӧgm������ƻ״̬�ı�
	bool OnGMControlCampaignState(IPubKernel* pPubKernel, IPubData* pPubData, int nCampaignId, int nCampaignState);

	// ���������ı�״̬��Ϣ
	void SendStateChangeToCampaignScene(IPubKernel* pPubKernel, const CampaignData* pData, int nCampaignState);

	typedef std::vector<DomainCampaignBase*> DomainCampaignVec;
private:
	CampaignDataVec			m_vecCampaignData;		// �����
	bool					m_bGmControl;			// �Ƿ���gm�������
	DomainCampaignVec		m_vecDomainCampaign;	// ���л��

	static DomainCampaign * m_pDomainCampaign;
};

#endif
