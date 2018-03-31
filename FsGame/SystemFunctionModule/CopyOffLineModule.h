//----------------------------------------------------------
// �ļ���:      CopyOfflineModule.h
// ��  ��:      ΪNPC���������������
// ˵  ��:		
// ��������:    2015��3��16��
// ������:       
// �޸���:    
//    :       
//----------------------------------------------------------
#ifndef _CopyOfflineModule_H_
#define _CopyOfflineModule_H_

#include "Fsgame/Define/header.h"
#include "../Define/OffLineDefine.h"
#include <vector>

class CopyOffLineModule : public ILogicModule
{
public:
	// ��ʼ��
	bool Init(IKernel* pKernel);
	// �ر�
	bool Shut(IKernel* pKernel);

	/*!
	 * @brief	���������������
	 * @param	self	NPC����
	 * @return	bool
	 */
	bool CopyOffLineData(IKernel* pKernel, const PERSISTID& self);

	/*!
	 * @brief	���������¼��
	 * @param	int nGroupId
	 * @return	void
	 */
	void ClearOfflinePubRec(IKernel* pKernel, int nGroupId);

	/*!
	 * @brief	���ĳһ����¼�������
	 * @param	int nGroupId
	 * @param	PERSISTID self
	 * @return	void
	 */
	void ClearOfflinePubRecOneRow(IKernel* pKernel, int nGroupId, const PERSISTID& self);

	// ��������
	bool LoadResource(IKernel* pKernel);

	/*!
	 * @brief	���������������ͨ�����uid
	 * @param	self	NPC����
	 * @param	strUid	���uid
	 * @return	bool
	 */
	bool CopyPlayerDataByUid(IKernel* pKernel, const PERSISTID& self, const char* strUid);
#ifdef _DEBUG
	// ���Խӿ�
	bool TestCopyOffLineData(IKernel* pKernel, const PERSISTID& self, const PERSISTID& npc);
#endif

	// ��ȡ������ҵȼ���buff
	const char* GetLevelBuffer(const int level);

	// ���ȡ����ͬһ�������
	bool FindNameList(int nNameNum, std::vector<std::wstring>& vecOutNameList);
private:
	enum
	{
		DEFAULT_PROPERTY_LEVEL_INTERVAL = 5,// Ĭ������ ÿ��5��һ����ֵ
		MAX_GREATER_THAN_LEVEL = 10			// ���ȼ���
	};
	// ������Ϣ
	struct MirDataInfo
	{
		MirDataInfo():  nType(0), bIsModify(false), bIsLoadConfig(false){}
		std::string		strName;			// ����
		int				nType;				// �������Ͷ�Ӧ(DataType)
		std::string		strSnsTable;		// ������Ӧ��sns��
		bool			bIsModify;			// �Ƿ���Ҫ�޸�
		bool			bIsLoadConfig;		// �Ƿ���Ҫ���¶�ȡ������
	};

	typedef std::vector<MirDataInfo> MirDataInfoVec;

	// ϵ������
	enum RatioType
	{
		GREATER_THAN_RATIO,					// ���ڵȼ����ϵ��
		LESS_THAN_RATIO,					// С�ڵȼ����ϵ��
		LESS_THAN_ZERO_RATIO,				// �ȼ���С��0��ϵ��

		MAX_RATIO_NUM
	};

	// ϵ����������
	struct RatioInfo 
	{
		RatioInfo() : nLevelDiff(0), fMinDCRatio(0.0f), fMaxDCRatio(0.0f)
		{
			memset(fRatio, 0, MAX_RATIO_NUM * sizeof(float));
		}
		int		nLevelDiff;					// �ȼ���(����NPC��Level - ��ҵ�Level��ֵ)
		float	fRatio[MAX_RATIO_NUM];		// ϵ����ֵ
		float	fMinDCRatio;				// Ĭ��������С�仯��
		float	fMaxDCRatio;				// Ĭ���������仯��
		float   fArenaRatio;				// ������npcʵ��ϵ��
	};

	// �ؼ��������ļ�
	static int nx_reload_copy_playertonpc_config(void* state);

	// ��ȡ������Ϣ����
	bool LoadDataInfoConfig(IKernel* pKernel);

	// ��ȡĬ�ϵ���������
	bool LoadDefaultName(IKernel* pKernel);

    // ���صȼ�buff����
	bool LoadLevelBuffCfg(IKernel* pKernel);

	// ����npcai����Ҫѧ�ļ���
	bool LoadNotAIUseSkill(IKernel* pKernel);

	// ���������ݷ������´�����Ϣ
	static int OnPublicMessage(IKernel* pKernel, const PERSISTID& self,
		const PERSISTID& sender, const IVarList& args);

	// ����SNS�������´�����Ϣ
	static int OnSNSMessage(IKernel* pKernel, const PERSISTID& self,
		const PERSISTID& sender, const IVarList& args);

	// ������������
	bool CopyPropertyData(IKernel* pKernel, IGameObj* pSelfObj, const char* strName, const IVarList& data, int& nIndex);

	// ������������
	bool CopyContainerData(IKernel* pKernel, IGameObj* pSelfObj, const char* strName, const IVarList& data, int& nIndex);
 
	// ����������������
	bool CopySkillContainerData(IKernel* pKernel, IGameObj* pSelfObj, const char* strName, const IVarList& data, int& nIndex);

	// ����������
	bool CopyRecordData(IKernel* pKernel, IGameObj* pSelfObj, const char* strName, const IVarList& data, int& nIndex);

	// ���ó��＼������
// 	void SetPetSkillData(IKernel* pKernel, IGameObj* pSelfObj, const PERSISTID& petitem, const char* strSkillData);
// 
// 	// ���ó����츳����
// 	void SetPetTalentData(IKernel* pKernel, IGameObj* pSelfObj, const PERSISTID& petitem, const char* strTalentData);

	// ��ȡdomain������
	static const std::wstring& GetDomainName(IKernel * pKernel);

	// ��Ĭ���������ݸ�ֵ��NPC
	bool CopyDataFromDefaultConfig(IKernel* pKernel, const PERSISTID& self);

	// ΪNPC����һ��Ĭ�ϵ���������
	//void CopyDefaultProperty(IKernel* pKernel, IGameObj* pSelfObj, const char* strName, int nSecIndex, float fDCRatio = 1.0f);

	// ΪNPC����һ��Ĭ�ϵļ�������
//	void CopyDefaultSkillContainer(IKernel* pKernel, IGameObj* pSelfObj, const char* strName, int nSecIndex);

	// �������ݺ�Ĳ���
	void OnAferCopyData(IKernel* pKernel, IGameObj* pSelfObj, int nPlayerLevel);
	
	// ��ȡ���Ա仯ϵ��
	const float GetPropertyRatio(IKernel* pKernel, IGameObj* pSelfObj, int nPlayerLevel);

	// ����Ĭ�����Ա仯��
	const float ComputeDCRatio();
	
	// �ȼ�buff����
	struct LevelBuffer
	{
		LevelBuffer(): m_MinLevel(0), m_MaxLevel(0), m_BuffID(""){} 
	    int		        m_MinLevel;	  // �ȼ���(С)
	    int				m_MaxLevel;	  // �ȼ���(��)
	    std::string		m_BuffID;	  // ��ӦBuffID
	};
	typedef std::vector<LevelBuffer> levelBufferVec;

private:
	MirDataInfoVec				m_vDataInfo;			// ������Ϣ
	static std::wstring			m_wsDomainName;			// ��������
	std::vector<std::wstring>   m_vDefaultName;			// Ĭ�����Ե����ֿ�
	RatioInfo					m_kRatioInfo;			// ϵ����������
public:
	static CopyOffLineModule*	m_pCopyOffLineModule;	
};

#endif