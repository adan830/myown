
//----------------------------------------------------------
// �ļ���:      PropertyInterface.h
// ��  ��:      ���ܡ�buffer��ͨ�����Բ�ѯ�ṩ�Ľӿ�
// ������:      Ԭ��
// ��������:    2014��7��
//    :        
//----------------------------------------------------------
#include "PropertyInterface.h"

bool FightPropertyInterface::Query(IGameObj* pGameObj,const char* name)
{
	if (!pGameObj)
	{
		return false;
	}
	return pGameObj->FindAttr(name);
}