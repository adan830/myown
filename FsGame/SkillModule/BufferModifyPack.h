//--------------------------------------------------------------------
// �ļ���:      BufferModifyPack.h
// ��  ��:      Buffer���԰��߼�
// ˵  ��:
// ��������:    2016��3��3��
// ������:		 ������
//    :       
//--------------------------------------------------------------------

#ifndef _BUFFER_MODIFYPACK_H_
#define _BUFFER_MODIFYPACK_H_

#include "Fsgame/Define/header.h"
#include "Fsgame/Define/ModifyPackDefine.h"
#include "utils/Singleton.h"

class BufferModifyPack
{
public:
    /**
    @brief  ��ʼ��
    @param  pKernel [IN] �������ָ��
    @return ���س�ʼ�����
    */
   bool Init(IKernel* pKernel);
private:
	//�ص����� ��Ҽ����������
	static int OnPlayerRecover(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);

	// ����buff���԰�
	bool UpdateBufferModifyPack(IKernel* pKernel, const PERSISTID& self, const PERSISTID& buffer, MapPropValue& mapPropValue);

	// �Ƴ�buff���԰�
	bool RemoveBufferModifyPack(IKernel* pKernel, const PERSISTID& self, const PERSISTID& buffer, MapPropValue& mapPropValue);

	// ����buff
	static int OnBuffAfterAdd(IKernel* pKernel, const PERSISTID& buffcontainer, const PERSISTID& buffer, const IVarList& args);

	// �Ƴ�buff
	static int OnBuffRemove(IKernel* pKernel, const PERSISTID& buffcontainer, const PERSISTID& buffer, const IVarList& args);
};

typedef HPSingleton<BufferModifyPack> BufferModifyPackSingleton;

#endif
