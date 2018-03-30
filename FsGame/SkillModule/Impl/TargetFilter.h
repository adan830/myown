//--------------------------------------------------------------------
// �ļ���:      Server\FsRoomLogic\skill_module\impl\target_filter.h
// ��  ��:      ���ݾ��룬�������״��Ŀ����й��˵�ϵͳ����
// ˵  ��:
// ��������:
// ������:
//    :       
//--------------------------------------------------------------------
#ifndef __TargetFilter_h__
#define __TargetFilter_h__

#include "Fsgame/Define/header.h"
#include "FsGame/Define/SkillDefine.h"
#include "utils/shape_collision.h"



//�����ļ������
struct FilterConditionParamStruct
{
    //bool bCheckHeight;      // �Ƿ���Ŀ��ĸ߶Ȳ�
    //float fHeightDiff;      // �����̵ĸ߶Ȳ�
    float fSelfBodyRadius;      // ���Լ�λ��Ϊ���ĵ�Բ�İ뾶�����˷�Χ��

    int iRelationType;
    int iHitTargetType;
    bool bSortByHP;         // ��Ѫ���ٷֱ�����Ѫ���ٷֱ�Խ�͵���
    bool bSortByDist;       // �Ƿ�Զ�������������
    bool bIncludeDead;      // �Ƿ������������

    explicit FilterConditionParamStruct()
       // , bCheckHeight(false)   // Ĭ�ϲ����߶Ȳ�
       // , fHeightDiff(0.0f)     // Ĭ�Ͽ����̵ĸ߶Ȳ�Ϊ0.0f
       : fSelfBodyRadius(0.5f)     // Ĭ�����˰뾶Ϊ0.0f

        , iRelationType(0)
        , iHitTargetType(0)
        , bSortByHP(false)
        , bSortByDist(false)      // Ĭ�ϲ��������
        , bIncludeDead(false)
    {

    }

    explicit FilterConditionParamStruct(const FilterConditionParamStruct& ins)
    {
        if (this != &ins)
        {
            //bCheckHeight = ins.bCheckHeight;
            //fHeightDiff = ins.fHeightDiff;
            fSelfBodyRadius = ins.fSelfBodyRadius;

            iRelationType = ins.iRelationType;
            iHitTargetType = ins.iHitTargetType;
            bSortByHP = ins.bSortByHP;
            bSortByDist = ins.bSortByDist;
            bIncludeDead = ins.bIncludeDead;
        }
    }
};


struct FilterParamStruct
{  
    float fTargetBodyRadius;    // Ŀ�����˰뾶
    float fShapeOrient;		// ��״����,���ھ��Σ��˷���ͳ��ȵķ���ͬ��

    explicit FilterParamStruct()
        : fTargetBodyRadius(0.0f)
        , fShapeOrient(0.0f)
    {
    }

    explicit FilterParamStruct(const FilterParamStruct& ins)
    {
        if (this != &ins)
        {
            fTargetBodyRadius = ins.fTargetBodyRadius;
            fShapeOrient = ins.fShapeOrient;
        }
    }
};

class TargetFilter
{
public:
    static int GetHitRangeTargets(IKernel* pKernel, const PERSISTID& self, const FmVec3& basePos, PERSISTID locktarget,
                           const SkillHitRangeParams& hitRangeParams, const FilterConditionParamStruct& filterCondition,
                           int iCount, IVarList& result);


    static ETargetFilterResult IsInHitRange(IKernel* pKernel, const FmVec3& basePos, const FmVec3& targetPos,
        const SkillHitRangeParams& hitRangeParams, const FilterParamStruct& filterParams);

public:

    /// \brief �ж�Ŀ������Ƿ����㼼�ܸ��ֹ�ϵ����
    /// \param [in] pKernel �����ں�ָ��
    /// \param [in] self �жϵ�����
    /// \param [in] target �жϵ�Ŀ��
    /// \param [in] iHitRelationType  ������������Ĺ�ϵ����
    /// \param [in] iHitTargetType ���ܿ������Ķ�������
    /// \param [in] iAddHitTargetType ���ܿ������ĸ��Ӷ���
    static ETargetFilterResult SatisfyRelation(IKernel* pKernel, const PERSISTID& self,
                                        const PERSISTID& target, int iHitRelationType, int iHitTargetType = 0);

protected:
    /// \brief ��ȡһ����Χ�ڵĶ���
    /// \param [in] pKernel �ں�ָ��
    /// \param [in] self
    /// \param [in] basePos ��Χ��״��������
    /// \param [in] locktarget ��������
    /// \param [in] SkillHitRangeParams ���÷�Χ�����ṹ
    /// \param [in] iInternelClassType ���õ��߼���������
    /// \param [in] iCount ��Ҫ��ȡ������
    /// \param [out] result ��ȡ������б�
    /// \return ���ػ�ȡ���������
    static int GetTargetList(IKernel* pKernel, const PERSISTID& self, const FmVec3& basePos, PERSISTID locktarget,
                      const SkillHitRangeParams& hitRangeParams, int iInternelClassType, int iCount, IVarList& result);

    /// \brief �������Ŀ��Ĺ�ϵ�����Ƿ����
    /// \param [in] pKernel �ں�ָ��
    /// \param [in] self    �жϵ�����
    /// \param [in] target  �жϵ�Ŀ��
    /// \param [in] iRelationType ������������Ĺ�ϵ����
    /// \return ���ؼ����
    static ETargetFilterResult  CheckHitTargetRelationType(IKernel* pKernel, IGameObj* pSelfObj, IGameObj* pTargetObj, int iRelationType);

    /// \brief �������Ŀ������ж��������Ƿ�Ϸ�
    /// \param [in] pKernel �ں�ָ��
    /// \param [in] self    �жϵ�����
    /// \param [in] target  �жϵ�Ŀ��
    /// \param [in] iHitTargetType ���������������������
    static ETargetFilterResult CheckHitTargetType(IKernel* pKernel, IGameObj* pSelfObj, IGameObj* pTargetObj, int iHitTargetType);

	// �������
	static bool CheckGroupID(IKernel* pKernel, IGameObj* pSelfObj, IGameObj* pTargetObj);

	// ���ʩ������Ŀ��ȼ� ������֮���pk����
	static bool CheckLevelLimit(IKernel* pKernel, IGameObj* pSelfObj, IGameObj* pTargetObj);
private:
    static bool SortObjectByDist(IKernel* pKernel, const PERSISTID& self, IVarList& result);
    static bool SortObjectByHP(IKernel* pKernel, IVarList& result);
};
#endif
