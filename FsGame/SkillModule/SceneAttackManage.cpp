//--------------------------------------------------------------------
// �ļ���:		SceneAttackManage.cpp
// ��  ��:		�����ɹ���������
// ˵  ��:		
//				
// ��������:	2015��05��28��
// ������:		 
//    :	    
//--------------------------------------------------------------------

#include <assert.h>
#include "SceneAttackManage.h"
#include "utils/string_util.h"
#include "FsGame/Define/CommandDefine.h"
#include "utils/extend_func.h"
#include "Define/Fields.h"
#include "SocialSystemModule/TeamModule.h"
#include "Define/Classes.h"

//�жϺ�����ַ��
std::vector<SCENE_PK_TYPE_FUNC> SceneAttackManage::m_funCheckRelationList;

//��ʼ��
void SceneAttackManage::Init(IKernel *pKernel)
{
    m_funCheckRelationList.resize(SCENE_ATTACK_TYPE_MAX, NULL);

	RegisterJudgeFun(SCENE_ATTACK_PUBLIC_SCENE, CheckByPlayerPKModel);
    RegisterJudgeFun(SCENE_ATTACK_CAMP, CheckCampPK);
	//RegisterJudgeFun(SCENE_ATTACK_GUILD, CheckGuildPK);
	RegisterJudgeFun(SCENE_ATTACK_PEACE, CheckPeace);
	RegisterJudgeFun(SCENE_ATTACK_PVE, CheckPVE);
}

//�ж�һ�������Ƿ���Թ���
bool SceneAttackManage::CheckTargetRelation(IKernel *pKernel, const PERSISTID &self, const PERSISTID &target, TestRelationType nTestRelationType)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return false;
	}

	PERSISTID attacker = get_pet_master(pKernel, self);
																																			  
    SceneAttackType type = (SceneAttackType)pKernel->GetSceneObj()->QueryInt("AttackType");
	if (type < 0 || type >= SCENE_ATTACK_TYPE_MAX )
    {
        return false;
    }

	return m_funCheckRelationList[type](pKernel, attacker, target, nTestRelationType);
}

//ע�ṥ��ģʽ�����жϺ���
bool SceneAttackManage::RegisterJudgeFun(SceneAttackType type, SCENE_PK_TYPE_FUNC fun)
{
    if(type < 0 || type >= SCENE_ATTACK_TYPE_MAX)
    {
        return false;
    }
    if(fun == NULL)
    {
        return false;
    }
    m_funCheckRelationList[type] = fun;
    return true;
}

// �Զ�����Ӫ��ϵ�ж�
bool SceneAttackManage::CheckCampPK(IKernel *pKernel, const PERSISTID &self, const PERSISTID &target, TestRelationType nTestRelationType)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	IGameObj *pTarget = pKernel->GetGameObj(target);
	if (pSelfObj == NULL || NULL == pTarget)
	{
		return false;
	}

	int nSelfCamp = pSelfObj->QueryInt(FIELD_PROP_CAMP);
	int nTargetCamp = pTarget->QueryInt(FIELD_PROP_CAMP);

	// ��Ӫֵ��ͬΪ�ѷ�,��ͬΪ�з� 
	bool bResult = false;
	if (TRT_FRIEND == nTestRelationType)
	{
		bResult = nSelfCamp == nTargetCamp;
	}
	else
	{
		bResult = nSelfCamp != nTargetCamp;
	}

	return bResult;
}

// �Զ��幫���ϵ�ж�
// bool SceneAttackManage::CheckGuildPK(IKernel *pKernel, const PERSISTID &self, const PERSISTID &target, TestRelationType nTestRelationType)
// {
// 	IGameObj* pSelfObj = pKernel->GetGameObj(self);
// 	IGameObj *pTarget = pKernel->GetGameObj(target);
// 	if (pSelfObj == NULL || NULL == pTarget)
// 	{
// 		return false;
// 	}
// 
// 	const wchar_t* wsSelfGuildName = pSelfObj->QueryWideStr(FIELD_PROP_GUILD_NAME);
// 	const wchar_t* wsTargetGuildName = pTarget->QueryWideStr(FIELD_PROP_GUILD_NAME);
// 
// 	// ��Ӫֵ��ͬΪ�ѷ�,��ͬΪ�з� 
// 	bool bResult = false;
// 	if (TRT_FRIEND == nTestRelationType)
// 	{
// 		bResult = StringUtil::CharIsEqual(wsSelfGuildName, wsTargetGuildName);
// 	}
// 	else
// 	{
// 		bResult = !StringUtil::CharIsEqual(wsSelfGuildName, wsTargetGuildName);;
// 	}
// 
// 	return bResult;
// }

// ��ƽ
bool SceneAttackManage::CheckPeace(IKernel *pKernel, const PERSISTID &self, const PERSISTID &target, TestRelationType nTestRelationType)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	IGameObj *pTarget = pKernel->GetGameObj(target);
	if (pSelfObj == NULL || NULL == pTarget)
	{
		return false;
	}

	bool bResult = false;
	if (TRT_FRIEND == nTestRelationType)
	{
		bResult = true;
	}
	else
	{
		bResult = false;;
	}

	return bResult;
}

// ֻ��PVE
bool SceneAttackManage::CheckPVE(IKernel *pKernel, const PERSISTID &self, const PERSISTID &target, TestRelationType nTestRelationType)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	IGameObj* pTargetObj = pKernel->GetGameObj(target);
	if (pSelfObj == NULL || NULL == pTargetObj)
	{
		return false;
	}

	int nSelfNation = pSelfObj->QueryInt(FIELD_PROP_CAMP);
	int nTargetNation = pTargetObj->QueryInt(FIELD_PROP_CAMP);
	bool bResult = false;
	// ������Ϊ��
	if (pSelfObj->GetClassType() == TYPE_NPC)
	{
		if (TRT_FRIEND == nTestRelationType)
		{
			bResult = nSelfNation == nTargetNation;
		}
		else
		{
			bResult = nSelfNation != nTargetNation;
		}
	}
	else if (TYPE_PLAYER == pSelfObj->GetClassType())  // �����������
	{
		if (TYPE_NPC == pTargetObj->GetClassType())	// Ŀ��Ϊnpc
		{
			if (TRT_FRIEND == nTestRelationType)
			{
				bResult = false;
			}
			else
			{
				bResult = nSelfNation != nTargetNation;
			}
		}
		else if (TYPE_PLAYER == pTargetObj->GetClassType())
		{
			if (TRT_FRIEND == nTestRelationType)
			{
				bResult = true;
			}
			else
			{
				bResult = false;;
			}
		}
	}

	return bResult;
}

//PKģʽ�߼�
bool SceneAttackManage::CheckByPlayerPKModel(IKernel *pKernel, const PERSISTID &self, const PERSISTID &target, TestRelationType nTestRelationType)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	IGameObj* pTargetObj = pKernel->GetGameObj(target);
	if (pSelfObj == NULL || NULL == pTargetObj)
	{
		return false;
	}

	int nSelfNation = pSelfObj->QueryInt(FIELD_PROP_CAMP);
	int nTargetNation = pTargetObj->QueryInt(FIELD_PROP_CAMP);
	bool bResult = false;
	// ������Ϊ��
	if (pSelfObj->GetClassType() == TYPE_NPC)
	{
		if (TRT_FRIEND == nTestRelationType)
		{
			bResult = nSelfNation == nTargetNation;
		}
		else
		{
			bResult = nSelfNation != nTargetNation;
		}
	}
	else if (TYPE_PLAYER == pSelfObj->GetClassType())  // �����������
	{
		if (TYPE_NPC == pTargetObj->GetClassType())	// Ŀ��Ϊnpc
		{
			if (TRT_FRIEND == nTestRelationType)
			{
				bResult = false;
			}
			else
			{
				bResult = nSelfNation != nTargetNation;
			}
		}
		else if (TYPE_PLAYER == pTargetObj->GetClassType())	 // Ŀ��Ϊ���
		{
			// �ڹ���������, �з������PKģʽ�Ĵ���
			int nSelfCamp = pSelfObj->QueryInt(FIELD_PROP_CAMP);
			int nTargetCamp = pTargetObj->QueryInt(FIELD_PROP_CAMP);
			if (nSelfCamp > 0 && nTargetCamp > 0 && pSelfObj->QueryInt(FIELD_PROP_GROUP_ID) == pTargetObj->QueryInt(FIELD_PROP_GROUP_ID))
			{
				return CheckCampPK(pKernel, self, target, nTestRelationType);
			}

			if (TRT_FRIEND == nTestRelationType)
			{
				bResult = IsFriendlyInPublicScene(pKernel, pSelfObj, pTargetObj);
			}
			else
			{
				bResult = IsEnemyInPublicScene(pKernel, pSelfObj, pTargetObj);
			}
		}
	}

	return bResult;
}

// �Ƿ�Ϊ�ѷ�
bool SceneAttackManage::IsFriendlyInPublicScene(IKernel *pKernel, IGameObj* pSelfObj, IGameObj* pTargetObj)
{
	if (NULL == pSelfObj || NULL == pTargetObj)
	{
		return false;
	}

	// �Լ�Ϊ�ѷ�
	if (pSelfObj == pTargetObj)
	{
		return true;
	}

	// ֻ�ж���Ϊ�ѷ�
	const wchar_t* wsTargetName = pTargetObj->QueryWideStr(FIELD_PROP_NAME);
	bool bResult = TeamModule::IsTeamMate(pKernel, pSelfObj->GetObjectId(), wsTargetName);

	return bResult;
}

// �Ƿ�Ϊ�з�
bool SceneAttackManage::IsEnemyInPublicScene(IKernel *pKernel, IGameObj* pSelfObj, IGameObj* pTargetObj)
{
	if (NULL == pSelfObj || NULL == pTargetObj)
	{
		return false;
	}
	
	// ���ѿ϶����ǵз�,�Լ�Ϊ�ѷ�
	const wchar_t* wsTargetName = pTargetObj->QueryWideStr(FIELD_PROP_NAME);
	if (TeamModule::IsTeamMate(pKernel, pSelfObj->GetObjectId(), wsTargetName) || pSelfObj == pTargetObj)
	{
		return false;
	}
	
	bool bResult = false;

	int nPKModel = pSelfObj->QueryInt(FIELD_PROP_PKMODEL);
	switch (nPKModel)
	{
	case PT_PEACE:
		bResult = false;
		break;
	case PT_ATT_CRIME:
		bResult = IsEnemyInAttCrimeModel(pSelfObj, pTargetObj);
		break;
	case PT_GUILD:
		//bResult = IsEnemyInGuildModel(pSelfObj, pTargetObj);
		break;
	case PT_KILL_ALL:
		bResult = true;
		break;
	}

	return bResult;
}

// ��ƽģʽ�ж�
bool SceneAttackManage::IsEnemyInAttCrimeModel(IGameObj* pSelfObj, IGameObj* pTargetObj)
{
	if (NULL == pSelfObj || NULL == pTargetObj)
	{
		return false;
	}
 
	bool bResult = false;
	int nTargetRedState = pTargetObj->QueryInt(FIELD_PROP_CRIME_STATE);
	// Ŀ���Ǻ���,��ƽģʽ�¾��ܴ�
	if (nTargetRedState > 0)
	{
		bResult = true;
	}

	return bResult;
}

// ����ģʽ�µ��ж�
// bool SceneAttackManage::IsEnemyInGuildModel(IGameObj* pSelfObj, IGameObj* pTargetObj)
// {																																												  
// 	if (NULL == pSelfObj || NULL == pTargetObj)
// 	{
// 		return false;
// 	}
// 
// 	const wchar_t* wsSelfGuildName = pSelfObj->QueryWideStr(FIELD_PROP_GUILD_NAME);
// 	const wchar_t* wsTargetGuildName = pTargetObj->QueryWideStr(FIELD_PROP_GUILD_NAME);
// 	if (StringUtil::CharIsNull(wsSelfGuildName) /*|| StringUtil::CharIsNull(wsTargetGuildName)*/)
// 	{
// 		return false;
// 	}
// 
// 	bool bResult = wcscmp(wsSelfGuildName, wsTargetGuildName) != 0;
// 	
// 	return bResult;
// }