//--------------------------------------------------------------------
// �ļ���:      PassiveSkillModifyPack.h
// ��  ��:      �����������԰��߼�
// ˵  ��:
// ��������:    2016��3��3��
// ������:		 ������
//    :       
//--------------------------------------------------------------------

#ifndef _PASSIVESKILLMODIFYPACK_H_
#define _PASSIVESKILLMODIFYPACK_H_

#include "Fsgame/Define/header.h"
#include "Fsgame/Define/ModifyPackDefine.h"
#include "utils/Singleton.h"

class PassiveSkillModifyPack
{
public:
    /**
    @brief  ��ʼ��
    @param  pKernel [IN] �������ָ��
    @return ���س�ʼ�����
    */
   bool Init(IKernel* pKernel);
private:
	// ���±�������Ӱ�����������
	static int AddPassiveSkillModifyPack(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);

	// �Ƴ���������Ӱ�����������
	static int RemovePassiveSkillModifyPack(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);
};

typedef HPSingleton<PassiveSkillModifyPack> PassiveSkillModifyPackSingleton;

#endif
