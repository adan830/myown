//------------------------------------------------------------------------------
// �ļ���:      fight_data_access.h
// ��  ��:      ս�����ݻ�ȡ
// ˵  ��:
// ��������:    2013��2��16��
// ������:       
// ��ע:
//    :       
//------------------------------------------------------------------------------

#ifndef __FIGHTDATAACCESS_H__
#define __FIGHTDATAACCESS_H__

#include "Fsgame/Define/header.h"
#include "FsGame/Define/FightDefine.h"
#include "FsGame/define/staticdatadefine.h"
#include "utils/singleton.h"
#include "utils/cau.h"
#include "SkillDefineDataStruct.h"
#include <cmath>
#include <algorithm>


class FightDataAccess
{
public:
    bool Init(IKernel* pKernel);

public:
    //��ȡ������������
    EHIT_TYPE GetHitType(IKernel* pKernel, const PERSISTID& self, const PERSISTID& target, const PERSISTID& src);

	// ��ù�ʽ���������
	bool GetFormulaValue(IKernel* pKernel, const PERSISTID& self, const PERSISTID& target, const PERSISTID& skill, int iFormulaIndex, float& fValue);
private:
	// ��Ӧ�����˺�ǰ,���㼼�ܹ�ʽϵ��
	static int OnCommandBeforeDamge(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);
}; // end of class FightDataAccess


typedef HPSingleton<FightDataAccess> FightDataAccessSingleton;

#endif // __FIGHTDATAACCESS_H__

