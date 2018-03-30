//--------------------------------------------------------------------
// �ļ���:      ItemEffectBase.h
// ��  ��:      ��ƷЧ������
// ˵  ��:		���е���Ʒ��Ч�����̳д˻���
// ��������:    2016��3��4��
// ������:      
//    :       
//--------------------------------------------------------------------
#ifndef _ITEM_EFFECT_BASE_H_
#define _ITEM_EFFECT_BASE_H_

#include "Fsgame/Define/header.h"
#include "FsGame/Define/ToolItemDefine.h"

// ���м����¼�����
class ItemEffectBase
{
public:
	ItemEffectBase();
	virtual ~ItemEffectBase();

	// ������ƷЧ��
	static ItemEffectBase* CreateItemEffect(const ItemEffectType nEffectType);

	// ��ʼ��
	virtual bool OnInit(IKernel* pKernel);

	// ����Ӧ��, ʧ�ܷ���false
	virtual bool TryApplyItemEffect(IKernel* pKernel, const PERSISTID& user, const PERSISTID& item, const ItemEffectParam& effect_entry);

	// ����Ʒ��Ч��Ӧ�õ��������
	virtual bool ApplyItemEffect(IKernel* pKernel, const PERSISTID& user, const PERSISTID& item, const ItemEffectParam& effect_entry, IVarList& showItems) = 0;
};

#endif