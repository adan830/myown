//--------------------------------------------------------------------
// �ļ���:		IFuncCreator.h
// ��  ��:		
// ˵  ��:		
// ��������:	2008��1��9��
// ������:		½����
// ��Ȩ����:	������ţ�������޹�˾
//--------------------------------------------------------------------

#ifndef _PUBLIC_IFUNCCREATOR_H
#define _PUBLIC_IFUNCCREATOR_H

#include "Macros.h"

// IFuncCreator
// �ű���չ����������

class IFuncCreator
{
public:
	IFuncCreator(IFuncCreator* pNext) { m_pNext = pNext; }
	
	virtual ~IFuncCreator() = 0;

	const char* m_strName;
	void* m_pMidFunc;
	bool m_bReturnTable;
	
	// �����һ��
	IFuncCreator* GetNext() const { return m_pNext; }

private:
	IFuncCreator();

private:
	IFuncCreator* m_pNext;
};

inline IFuncCreator::~IFuncCreator() {}

#endif // _PUBLIC_IFUNCCREATOR_H

