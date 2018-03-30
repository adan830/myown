//----------------------------------------------------------
// �ļ���:      LandPosModule.h
// ��  ��:		��ҵ�½�����ģ��
// ˵  ��:		
// ��������:    2015��4��8��
// ������:       
// �޸���:    
//    :       
//----------------------------------------------------------
#include "LandPosModule.h"
#include "public\Inlines.h"
#include "utils\string_util.h"
#include "..\Define\SnsDefine.h"
//#include "..\Define\OffLineDefine.h"
#include "AsynCtrlModule.h"
//#include "..\Define\NationDefine.h"
#include "utils\extend_func.h"
#include "FsGame\CommonModule\ActionMutex.h"

LandPosModule*	LandPosModule::m_pLandPosModule = NULL;
AsynCtrlModule* LandPosModule::m_pAsynCtrlModule = NULL;

// ��ʼ��
bool LandPosModule::Init(IKernel* pKernel)
{
	m_pLandPosModule = this;
	m_pAsynCtrlModule = (AsynCtrlModule*)pKernel->GetLogicModule("AsynCtrlModule");

	Assert(NULL != m_pLandPosModule && NULL != m_pAsynCtrlModule);

	pKernel->AddEventCallback("player","OnStore",LandPosModule::OnPlayerStore);
	return true;
}

// �ر�
bool LandPosModule::Shut(IKernel* pKernel)
{
	return true;
}

/*!
 * @brief	������ҵ�½��
 * @param	self		��Ҷ���
 * @param	nSceneId	��½�ĳ���id
 * @param	fPosX,fPosZ	��½��
 * @return	bool
 */
bool LandPosModule::SetPlayerLandPosi(IKernel* pKernel, const PERSISTID& self, int nSceneId, float fPosX, float fPosY, float fPosZ)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj || 0 == nSceneId)
	{
		return false;
	}
	// ��¼����ʱ����������
	char positionInfo[256] = {0};
	SPRINTF_S(positionInfo, "%f,%f,%f,%d", fPosX, fPosY, fPosZ, nSceneId);
	pSelfObj->SetString("BeforeOffLine", positionInfo);

	return true;
}

/*!
 * @brief	��ҴӸ���������,�����ϴν����λ��,ȡ������ĵ�½��
 * @param	self		��Ҷ���
 * @return	bool
 */
bool LandPosModule::PlayerReturnLandPosi(IKernel* pKernel, const PERSISTID& self)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return false;
	}

	//ActionMutexModule::ExitAction(pKernel, self);

	const char* strLastPos = pSelfObj->QueryString("BeforeOffLine");
	// �����쳣�ط�,������Ϊ���ַ�,�лػʳ�
	if (StringUtil::CharIsNull(strLastPos))
	{
		int nMainSceneId = 2;// GetNationMainSceneId(nNation);
		m_pAsynCtrlModule->SwitchBorn(pKernel, self, nMainSceneId, true);
		return false;
	}

	CVarList pos;
	StringUtil::SplitString(pos, strLastPos, ",");
	if (pos.GetCount() != 4)
	{
		return false;
	}
	float x = pos.FloatVal(0);
	float y = pos.FloatVal(1);
	float z = pos.FloatVal(2);
	int nSceneId = pos.IntVal(3);

	pSelfObj->SetString("BeforeOffLine", "");
	
	m_pAsynCtrlModule->SwitchLocate(pKernel, self, nSceneId, x,  y, z, pSelfObj->GetOrient(), true);

	return true;
}

/// \brief ����
int LandPosModule::OnPlayerStore(IKernel* pKernel, const PERSISTID& self,
	const PERSISTID& sender, const IVarList& args)
{
	int reason = args.IntVal(0);
	if (reason != STORE_EXIT)
	{
		return 0;
	}
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return 0;
	}

	const char* strLandPosi = pSelfObj->QueryString("BeforeOffLine");

	CVarList pos;
	StringUtil::SplitString(pos, strLandPosi, ",");
	if (pos.GetCount() != 4)
	{
		return 0;
	}
	float x = pos.FloatVal(0);
	float y = pos.FloatVal(1);
	float z = pos.FloatVal(2);
	int nSceneId = pos.IntVal(3);

	pKernel->SetLandPosi(self, nSceneId, x, y, z, pSelfObj->GetOrient());
	return 0;
}