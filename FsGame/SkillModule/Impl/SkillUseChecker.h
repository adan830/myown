//--------------------------------------------------------------------
// �ļ���:      Server\FsGame\SkillModule\impl\skillUseChecker.h
// ��  ��:      ��鼼��ʹ�õ�����������ʵ�ֺ���ȫ�������ڴ�
// ˵  ��:
// ��������:	2014��10��31��
// ������:		 
//    :       
//--------------------------------------------------------------------

#if !defined __SkillUseChecker_h__
#define __SkillUseChecker_h__

#include "Fsgame/Define/header.h"
#include "utils/math/math.h"
#include "utils/Singleton.h"

class SkillUseChecker
{
public:

    /**
    *@brief  �ж��Ƿ����ʹ�ü���
    *@param  [in] pKernel �����ں�ָ��
    *@param  [in] self ʩ����
    *@param  [in] x,y,z,orient �����ͷ�ʱʩ����λ�úͳ���
    *@param  [in] skill ���ܶ���
    *@param  [in] target �������ö���
    *@param  [in] dx,dy,dz �������õ�λ��
    *@return int ����1��ʾ�����ͷţ�����ֵ�����Ӧ�Ŵ���ŷ���
    */
    int CanUseSkill(IKernel* pKernel, const PERSISTID& self,
                    float x, float y, float z, float orient,
                    const PERSISTID& skill, const PERSISTID& target,
                    float dx, float dy, float dz);

    // ����ж�
    int CheckCastRange(IKernel* pKernel, const PERSISTID& self, const PERSISTID& target, const PERSISTID& skill);

    //�Ƿ������ϵ����
    bool CheckSkillRelation(IKernel* pKernel, const PERSISTID& self, const PERSISTID& skill, const PERSISTID& target);

    // �жϼ��ܸ߼�Ч�������ϵ
    bool CheckEventEffectRelation(IKernel* pKernel, const PERSISTID& self, const PERSISTID& target, const char* szSkillEffectPackID);

private:
    // ��������Ƽ����ͷŵ������ж�
	int HasPropertyLimit(IKernel* pKernel, const PERSISTID& self, const PERSISTID& target, const PERSISTID& skill);
}; // end of class SkillUseChecker

typedef HPSingleton<SkillUseChecker>  SkillUseCheckerSingleton;


#endif
