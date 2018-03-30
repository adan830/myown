//--------------------------------------------------------------------
// �ļ���:      AIHeartBeat.h
// ��  ��:      AIϵͳ����
// ˵  ��:
// ��������:    2014��12��31��
// ������:       
// �޸���:        
//    :       
//--------------------------------------------------------------------
#pragma once
#include "Fsgame/Define/header.h"
class AIHeartBeat
{
public:
	AIHeartBeat(void);
public:
	//ս������
	static int HB_AIFighting(IKernel* pKernel, const PERSISTID& self, int slice);

	//Ѳ������
	static int HB_AIPatrol(IKernel* pKernel, const PERSISTID& self, int slice);

	// ����Npc�ж����
	static int HB_AIEndFight(IKernel* pKernel, const PERSISTID& self, int slice);

	//��������
	static int HB_AIBornEnd(IKernel* pKernel, const PERSISTID& self, int slice);

	//ת����
	static int HB_AIRatate(IKernel* pKernel, const PERSISTID& self, int slice);

	//������������
	static int HB_AIFind(IKernel* pKernel, const PERSISTID& self, int slice);

	//��ʱ�����������
	static int HB_AIClonePlayer(IKernel* pKernel, const PERSISTID& self, int slice);

    //��ͨ�ֻس����㳬ʱ����
    static int HB_BACK_BORN_TIMEOUT(IKernel *pKernel, const PERSISTID &self, int slice);

	// Ѻ��Npc��������
// 	static int HB_EscortNpcFollow(IKernel *pKernel, const PERSISTID &self, int slice);
// 
// 	// ��λ����Npc����
// 	static int HB_LocationConvyNpc(IKernel* pKernel, const PERSISTID& self, int slice);
// 
// 	// �ӳٻ���Npc�ƶ�
// 	static int HB_DelayConvoy(IKernel* pKernel, const PERSISTID& self, int slice);

	// ���ܽ�������
	static int HB_EndEscape(IKernel* pKernel, const PERSISTID& self, int slice);

	// �����������
	static int HB_CheckEscape(IKernel* pKernel, const PERSISTID& self, int slice);
};
