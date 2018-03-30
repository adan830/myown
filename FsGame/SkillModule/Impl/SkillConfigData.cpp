
//--------------------------------------------------------------------
// �ļ���:      skill_module\impl\skill_configdata.cpp
// ��  ��:      
// ˵  ��:      
// ��������:    2014��4��4��
// ������:       
//    :       
//--------------------------------------------------------------------

#include "FsGame/SkillModule/impl/SkillConfigData.h"

#include "utils/util_func.h"
#include "utils/extend_func.h"


namespace SkillUtilFuncs
{
    // �������ò�������ƫ�ƣ�ת���ɶ���ͼ�β���
    bool GetSphereParams(const SkillHitRangeParams& hitRangeParams, 
        const FmVec3& basePos, const float orient,
        FmVec2& pos, float& fShapeOrient,
        float& r, float& R, float& angle)
    {
        // �Ȼ���ԭ����ת��˳ʱ����ת
        float fOrient = orient - hitRangeParams.skillhitrangeunion.sphere.fHitRangeSphereBasepointOffsetOrient;
        // �����ߵ�ͼ���������ߵķ���
        FmVec2 direct(sin(fOrient), cos(fOrient));// ע�⣺������(x,z��������z+�ļн�
        // ͼ������λ��
        pos = FmVec2(basePos.x, basePos.z) + direct * hitRangeParams.skillhitrangeunion.sphere.fHitRangeSphereDistCentre;
        // �ٻ���ͼ�����ĵ���תͼ�γ���˳ʱ����ת
        fShapeOrient = fOrient - hitRangeParams.skillhitrangeunion.sphere.fHitRangeSphereOffsetOrient;
        r = hitRangeParams.skillhitrangeunion.sphere.fHitRangeSphereDistMin;
        R = hitRangeParams.skillhitrangeunion.sphere.fHitRangeSphereDistMax;
        angle = hitRangeParams.skillhitrangeunion.sphere.fHitRangeSphereTurnAngle;

        return true;
    }

    bool GetCylinderParams(const SkillHitRangeParams& hitRangeParams,
        const FmVec3& basePos, const float orient, 
        FmVec2& pos, float& fShapeOrient,
        float& r, float& R, float& angle)
    {
        // �Ȼ���ԭ����ת��˳ʱ����ת
        float fOrient = orient - hitRangeParams.skillhitrangeunion.cylinder.fHitRangeCylinderBasepointOffsetOrient;
        // �����ߵ�ͼ���������ߵķ���
        FmVec2 direct(sin(fOrient), cos(fOrient)); // ע�⣺������(x,z��������z+�ļн�
        // ͼ������λ��
        pos = FmVec2(basePos.x, basePos.z) + direct * hitRangeParams.skillhitrangeunion.cylinder.fHitRangeCylinderDistCentre;
        // �ٻ���ͼ�����ĵ���תͼ�γ���˳ʱ����ת
        fShapeOrient = fOrient - hitRangeParams.skillhitrangeunion.cylinder.fHitRangeCylinderOffsetOrient;
        r = hitRangeParams.skillhitrangeunion.cylinder.fHitRangeCylinderDistMin;
        R = hitRangeParams.skillhitrangeunion.cylinder.fHitRangeCylinderDistMax;
        angle = hitRangeParams.skillhitrangeunion.cylinder.fHitRangeCylinderTurnAngle;
        return true;
    }

    bool GetRectangularParams(const SkillHitRangeParams& hitRangeParams,
        const FmVec3& basePos, const float orient, 
        FmVec2& pos, float& fShapeOrient,
        float& width, float& length)
    {
        // �Ȼ���ԭ����ת��˳ʱ����ת
        float fOrient = orient - hitRangeParams.skillhitrangeunion.rectangular.fHitRangeRectangularBasepointOffsetOrient;
        // �����ߵ�ͼ���������ߵķ���
        FmVec2 direct(sin(fOrient), cos(fOrient));// ע�⣺������(x,z��������z+�ļн�
        // ͼ������λ��
        pos = FmVec2(basePos.x, basePos.z) + direct * hitRangeParams.skillhitrangeunion.rectangular.fHitRangeRectangularDistCentre;
        // �ٻ���ͼ�����ĵ���תͼ�γ���˳ʱ����ת
        fShapeOrient = orient - hitRangeParams.skillhitrangeunion.rectangular.fHitRangeRectangularOffsetOrient;
        length = hitRangeParams.skillhitrangeunion.rectangular.fHitRangeRectangularLength;
        width = hitRangeParams.skillhitrangeunion.rectangular.fHitRangeRectangularWidth;
        return true;
    }

	bool GetPosCylinderParams(const SkillHitRangeParams& hitRangeParams, float& fRadius)
	{
		fRadius = hitRangeParams.skillhitrangeunion.poscylinder.fHitRangeRadius;
		return true;
	}

    //////////////////////////////////////////////////////////////////////////
    int GetTargetRelationType(const IVarList& params)
    {
        int iRelationType = HIT_TARGET_RELATION_TYPE_NONE;
        const int g_HitTargetRelationTypeArray[] =
        {
            HIT_TARGET_RELATION_TYPE_SELF,       // ��Ӧ�����е���ţ�0
            HIT_TARGET_RELATION_TYPE_FRIEND,     // ��Ӧ�����е���ţ�1
            HIT_TARGET_RELATION_TYPE_ENIMY,      // ��Ӧ�����е���ţ�2
        };

        int count = sizeof(g_HitTargetRelationTypeArray) / sizeof(const int);
		LoopBeginCheck(a)
        for (size_t t = 0; t < params.GetCount(); ++t)
        {
			LoopDoCheck(a)
            int index = atoi(params.StringVal(t));
            if (index >= 0 && index < count)
            {
                iRelationType |= (EHitTargetRelationType)g_HitTargetRelationTypeArray[index];
            }
        }

        return iRelationType;
    }

    int GetTargetRelationType(const char* szConfigRelationType)
    {
        CVarList vRes;
        ::util_split_string(vRes, szConfigRelationType, ",");		
        return GetTargetRelationType(vRes);
    }

    int GetTargetClassType(const IVarList& params)
    {
        int iHitTargetType = HIT_TARGET_TYPE_NONE;
        const int g_HitTargetTypeArray[] =
        {
            HIT_TARGET_TYPE_PLAYER,              // ��� ��Ӧ�����е���ţ�0
            HIT_TARGET_TYPE_MONSTER,             // ���� ��Ӧ�����е���ţ�1
            HIT_TARGET_TYPE_FUNCTION_NPC,        // ����NPC ��Ӧ�����е���ţ�2
            HIT_TARGET_TYPE_TASK_NPC,            // ����NPC ��Ӧ�����е���ţ�3
            HIT_TARGET_TYPE_BROKEN_NPC,          // ����NPC ��Ӧ�����е���ţ�4
            HIT_TARGET_TYPE_PET_NPC,             // ����npc ��Ӧ�����е���ţ�5
        };

        
        int count = sizeof(g_HitTargetTypeArray) / sizeof(const int);
		LoopBeginCheck(b)
        for (size_t t = 0; t < params.GetCount(); ++t)
        {
			LoopDoCheck(b)
            int index = atoi(params.StringVal(t));
            if (index >= 0 && index < count)
            {
                iHitTargetType |= (EHitTargetType)g_HitTargetTypeArray[index];
            }
        }
        return iHitTargetType;
    }

    int GetTargetClassType(const char* szConfigClassType)
    {
        CVarList vRes;
        ::util_split_string(vRes, szConfigClassType, ",");
        return GetTargetClassType(vRes);
    }

    int ConvertTargetInnerClassType(int iConfigClassType)
    {
        int iInternelClassType = 0;
        if (HIT_TARGET_TYPE_NONE == iConfigClassType)
        {
            iInternelClassType = TYPE_PLAYER | TYPE_NPC;
        }
        else
        {
            // �Ƿ�������
            if ((HIT_TARGET_TYPE_PLAYER & iConfigClassType) != 0)
            {
                iInternelClassType = TYPE_PLAYER;
            }

            // �Ƿ����npc
            if (HIT_TARGET_TYPE_PLAYER != iConfigClassType)
            {
                iInternelClassType |= TYPE_NPC;
            }
        }

        return iInternelClassType;
    }

} // end of namespace SkillUtilFuncs
