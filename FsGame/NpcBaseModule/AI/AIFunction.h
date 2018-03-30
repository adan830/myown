//--------------------------------------------------------------------
// �ļ���:      AIFunction.h
// ��  ��:      AIϵͳ��Ϊ����
// ˵  ��:
// ��������:    2014��12��31��
// ������:       
// �޸���:        
//    :       
//--------------------------------------------------------------------
#ifndef _AIFUNCTION_H_
#define _AIFUNCTION_H_

#include "Fsgame/Define/header.h"
#include "FsGame/SystemFunctionModule/MotionModule.h"
#include "AISystem.h"

class AISystem;
class AIFunction
{
public:
	static bool Init(IKernel* pKernel);

	static bool Shut(IKernel* pKernel);
public:
	//�ƶ���Ŀ��㣨ÿ���ƶ����һ������ FindPathPointList��
	static int StartFindPathMove(IKernel* pKernel, const PERSISTID& self);

	//����һ��Ŀ���(ͨ��Ѱ·����FindPathPointList)
	static int StartMoveToPoint(IKernel* pKernel, const PERSISTID& self, float posX, float posZ);

	// ׷��Ŀ��
	static int	StartMoveToObject(IKernel * pKernel, const PERSISTID & self, const PERSISTID& target, 
								  float minDistance, float maxDistance);

	//Ѳ���ƶ�����һ��Ŀ���(ֱ��PatrolPointRecѰ·�ƶ�)
	// bNeedDynaicFindPath�Ƿ���Ҫ��̬Ѱ·
	static bool StartPatrolToPoint(IKernel* pKernel, const PERSISTID& self, float posX, float posZ, bool bNeedDynaicFindPath = true);

	//NPC�ƶ���Ӧ
	static int MotionCallBack(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);

	//��ʼ������
	static int InitSkill(IKernel* pKernel, const PERSISTID & self, const char* skillStr);

	//���ɹ�����ж�·����
	static bool CreateNpcPath(IKernel* pKernel, const PERSISTID& self, const IVarList& args);

    //���㵱ǰ�س����㣨FindPathPointList��·����ʱ��(��λ�Ǻ���)
    static int CountFindPathPointListTime(IKernel *pKernel, const PERSISTID &self);

	// �ƶ��ص�
	static int MotionCallBackFunction(IKernel* pKernel, const PERSISTID& self, int result);
private:
	static AISystem * m_pAISystem;
};
#endif
