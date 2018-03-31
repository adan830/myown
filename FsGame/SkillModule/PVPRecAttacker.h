//------------------------------------------------------------------------------
// �ļ���:      PVPRecAttacker.h
// ��  ��:		pvp������¼
// ˵  ��:
// ��������:    2017��10��17��
// ������:          
//------------------------------------------------------------------------------

#ifndef __PVP_REC_ATTACKER_H__
#define __PVP_REC_ATTACKER_H__

#include "Fsgame/Define/header.h"
#include "utils/singleton.h"

class PVPRecAttacker
{
public:
    bool Init(IKernel* pKernel);
public:
    //��ȡ���������б� killerΪ��,����ȫ��������,��Ϊ�ջ��޳���
    bool GetAttackerList(IKernel* pKernel, IVarList& outAttackerList, const PERSISTID& self, const PERSISTID& killer);
private:
	/// \brief ����
	static int OnPlayerRecover(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);

	// ��Ӧ���˺�
	static int OnCommandBeDamage(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);

	//ս��״̬�����ı�
	static int C_OnFightStateChanged(IKernel *pKernel, const PERSISTID &self, const char *property, const IVar &old);
}; // end of class PVPRecAttacker


typedef HPSingleton<PVPRecAttacker> PVPRecAttackerSingleton;

#endif // __FIGHTDATAACCESS_H__

