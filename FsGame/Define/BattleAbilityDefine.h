//--------------------------------------------------------------------
// �ļ���:		BattleAbilityDefine.h
// ��  ��:		ս����ͳ��
// ˵  ��:		
// ��������:	2017��04��12��
// ��������:	2017��04��12��
// ������:		������  
//--------------------------------------------------------------------

#ifndef __BATTLE_ABILITY_DEFINE_H__
#define __BATTLE_ABILITY_DEFINE_H__

#define BATTLLE_ABILITY_REC "battlle_ability_rec"		// ս������¼��

enum BatttleAbilityRecCol
{
	BAR_COL_TYPE,				// ����	BatttleAbilityType
	BAR_COL_VALUE				// ս����ֵ
};

// ս����ͳ������
enum BatttleAbilityType
{
	BA_PROPERTY_TYPE,			// �������ս����
	BA_SKILL_TYPE,				// ��������ս����
	BA_PASSIVE_SKILL_TYPE,		// ��������ս����(������������͡�ӡ��)

	MAX_BATTLE_ABILITY_TYPE
};


// ��ʽ����
enum BAFormulaCalType
{
	BFC_SKILL_CAL = 1,				// ������������
	BFC_PASSIVE_SKILL_CAL			// ������������
};
#endif //__BATTLE_ABILITY_DEFINE_H__