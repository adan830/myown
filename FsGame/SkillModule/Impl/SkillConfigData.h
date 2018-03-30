
//--------------------------------------------------------------------
// �ļ���:      interface\skill_configdata.h
// ��  ��:      �������õ����ݽṹ֮���ת��
// ˵  ��:      
// ��������:    2014��10��27��
// ������:       
//    :       
//--------------------------------------------------------------------



#ifndef __SKILL_CONFIGDATA_H__
#define __SKILL_CONFIGDATA_H__

#include "FsGame/Define/SkillDefine.h"
#include "public/IVarList.h"
#include "utils/math/math.h"


struct SkillHitRangeParams;

namespace SkillUtilFuncs
{
    extern bool GetSkillHitRangeData(const IVarList& configdata, int iHitRangeType, float fRangeScale, SkillHitRangeParams& hitRangeParams);

    // �������ò�������ƫ�ƣ�ת���ɶ���ͼ�β���
    extern bool GetSphereParams(const SkillHitRangeParams& hitRangeParams, const FmVec3& basePos, const float orient, FmVec2& pos, float& fShapeOrient, float& r, float& R, float& angle);
    extern bool GetCylinderParams(const SkillHitRangeParams& hitRangeParams, const FmVec3& basePos, const float orient, FmVec2& pos, float& fShapeOrient, float& r, float& R, float& angle);
    extern bool GetRectangularParams(const SkillHitRangeParams& hitRangeParams, const FmVec3& basePos, const float orient, FmVec2& pos, float& fShapeOrient, float& width, float& length);
	extern bool GetPosCylinderParams(const SkillHitRangeParams& hitRangeParams, float& fRadius);
	
	// ����ɸѡĿ���ϵ
    extern int GetTargetRelationType(const IVarList& params);
    extern int GetTargetRelationType(const char* szConfigRelationType);
    extern int GetTargetClassType(const IVarList& params);
    extern int GetTargetClassType(const char* szConfigClassType);
    extern int ConvertTargetInnerClassType(int iConfigClassType);
}

#endif // __SKILL_CONFIGDATA__H__
