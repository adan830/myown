// -------------------------------------------
// �ļ����ƣ� ShopGold.h
// �ļ�˵���� Ԫ���̵�
// �������ڣ� 
// �� �� �ˣ�  tongzt
// �޸��ˣ�
// -------------------------------------------
#ifndef _ShopGold_H_
#define _ShopGold_H_

#include "ShopBase.h"

class ShopGold : public ShopBase
{
public:
	ShopGold();

public:


public:
	// ��ʼ��
	virtual bool Init(IKernel* pKernel);

	// ע�ᶨʱ��
	virtual void RegistResetTimer(IKernel *pKernel, const PERSISTID &self);

	// ��Ʒˢ�¶�ʱ��
	static int ItemResetTimer(IKernel *pKernel, const PERSISTID &self, int slice);

	// �̵����ö�ʱ��
	static int ShopResetTimer(IKernel *pKernel, const PERSISTID &self, int slice);

};

#endif