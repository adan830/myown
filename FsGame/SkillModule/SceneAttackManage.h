//--------------------------------------------------------------------
// �ļ���:		SceneAttackManage.h
// ��  ��:		�����ɹ���������
// ˵  ��:		
//				
// ��������:	2015��05��28��
// ������:		 
//    :	    
//--------------------------------------------------------------------
#ifndef __SceneAttackManage_H__
#define __SceneAttackManage_H__

#include <vector>
#include "Fsgame/Define/header.h"
#include "FsGame/Define/PKModelDefine.h"

class TeamModule;

//��������ģʽ�����жϺ���
typedef bool(__cdecl* SCENE_PK_TYPE_FUNC)(IKernel *pKernel, const PERSISTID &self, const PERSISTID &target, TestRelationType nTestRelationType);

class SceneAttackManage
{
public:
    ~SceneAttackManage();

    //��ʼ��
    static void Init(IKernel *pKernel);

    // ��֤Ŀ���ϵ����
	static bool CheckTargetRelation(IKernel *pKernel, const PERSISTID &self, const PERSISTID &target, TestRelationType nTestRelationType);
private:
    // ˽��
    SceneAttackManage();

    // ˽�еĿ������캯��
    SceneAttackManage(const SceneAttackManage &a);

    // ˽�еĸ�ֵ����
    SceneAttackManage & operator =(const SceneAttackManage &a);

    //ע�ṥ��ģʽ�����жϺ���
    static bool RegisterJudgeFun(SceneAttackType type, SCENE_PK_TYPE_FUNC fun);

//---------------------------------------------------------------------------------------------------
    // �Զ�����Ӫ��ϵ�ж�
	static bool CheckCampPK(IKernel *pKernel, const PERSISTID &self, const PERSISTID &target, TestRelationType nTestRelationType);

	// �Զ��幫���ϵ�ж�
	//static bool CheckGuildPK(IKernel *pKernel, const PERSISTID &self, const PERSISTID &target, TestRelationType nTestRelationType);

	// ��ƽ
	static bool CheckPeace(IKernel *pKernel, const PERSISTID &self, const PERSISTID &target, TestRelationType nTestRelationType);

	// ֻ��PVE
	static bool CheckPVE(IKernel *pKernel, const PERSISTID &self, const PERSISTID &target, TestRelationType nTestRelationType);

	//PKģʽ�߼�
	static bool CheckByPlayerPKModel(IKernel *pKernel, const PERSISTID &self, const PERSISTID &target, TestRelationType nTestRelationType);

	// ���������жϹ�ϵ�Ľӿ�
	// �Ƿ�Ϊ�ѷ�
	static bool IsFriendlyInPublicScene(IKernel *pKernel, IGameObj* pSelfObj, IGameObj* pTargetObj);

	// �Ƿ�Ϊ�з�
	static bool IsEnemyInPublicScene(IKernel *pKernel, IGameObj* pSelfObj, IGameObj* pTargetObj);

	// �����ж�,������Ŀ������Ҳ��Ҳ��Ƕ��ѵ������
	// ��ƽģʽ�ж�
	static bool IsEnemyInAttCrimeModel(IGameObj* pSelfObj, IGameObj* pTargetObj);

	// ����ģʽ�µ��ж�																																												 
	//static bool IsEnemyInGuildModel(IGameObj* pSelfObj, IGameObj* pTargetObj);
//--------------------------------------------------------------------------------------------------    
private:
	// ģ��ָ��
	static TeamModule* m_pTeamModule;

    //�жϺ�����ַ��
    static std::vector<SCENE_PK_TYPE_FUNC> m_funCheckRelationList;
};


#endif //__SceneAttackManage_H__