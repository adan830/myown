//------------------------------------------------------------------------------
// �ļ���:      buffer.cpp
// ��  ��:      Bufferģ�鹫���ӿ�ʵ��
// ˵  ��:
// ��������:    2013��2��22��
// ������:       
// ��ע:
//    :       
//------------------------------------------------------------------------------


#include "FsGame/Interface/FightInterface.h"

#include "FsGame/Define/FightPropertyDefine.h"
#include "FsGame/Define/SkillEventFuncIdDefine.h"
#include "FsGame/SkillModule/BufferModule.h"
#include "FsGame/SkillModule/Impl/SkillDataQueryModule.h"

bool FightInterface::GetAllBuffer(IKernel* pKernel, const PERSISTID& self, IVarList& bufferIDList)
{
    if (!pKernel->Exists(self))
    {
        return false;
    }

    PERSISTID bufferContainer = BufferModule::m_pInstance->GetBufferContainer(pKernel, self);
    if (!pKernel->Exists(bufferContainer))
    {
        return false;
    }

    CVarList bufferList;
    pKernel->GetChildList(bufferContainer, TYPE_ITEM, bufferList);
    size_t count = bufferList.GetCount();

    LoopBeginCheck(a);
    for (size_t i=0; i<count; i++)
    {
        LoopDoCheck(a);

        PERSISTID buffer = bufferList.ObjectVal(i);
        if (!pKernel->Exists(buffer))
        {
            continue;
        }
        const char* szConfigID = pKernel->GetConfig(buffer);
        bufferIDList << szConfigID;
    }
    return bufferIDList.GetCount() > 0;
}

bool FightInterface::GetAllBufferObject(IKernel* pKernel, const PERSISTID& self, IVarList& bufferObjectList)
{
    if (!pKernel->Exists(self))
    {
        return false;
    }

    PERSISTID bufferContainer = BufferModule::m_pInstance->GetBufferContainer(pKernel, self);
    if (!pKernel->Exists(bufferContainer))
    {
        return false;
    }

    bufferObjectList.Clear();
    pKernel->GetChildList(bufferContainer, TYPE_ITEM, bufferObjectList);
    return bufferObjectList.GetCount() > 0;
}

bool FightInterface::FindBuffer(IKernel* pKernel, const PERSISTID& self, const char* szBufferID, IVarList& result)
{
    if (!pKernel->Exists(self))
    {
        return false;
    }

	if (NULL == szBufferID || StringUtil::CharIsNull(szBufferID))
    {
        return false;
    }
    return BufferModule::m_pInstance->FindBuffer(pKernel, self, szBufferID, result);
}

bool FightInterface::AddBuffer(IKernel* pKernel,
                               const PERSISTID& self,
                               const PERSISTID& sender,
                               const char* szBufferConfigID)
{
    if (!pKernel->Exists(self))
    {
        return false;
    }

	if (NULL == szBufferConfigID || StringUtil::CharIsNull(szBufferConfigID))
    {
        return false;
    }
    return BufferModule::m_pInstance->AddBuffer(pKernel, self, sender, szBufferConfigID);
}

bool FightInterface::AddBuffer(IKernel* pKernel,
                               const PERSISTID& self,
                               const PERSISTID& sender,
                               const char* szBufferConfigID,
                               const int iLifeTime)
{
    if (!pKernel->Exists(self))
    {
        return false;
    }

	if (NULL == szBufferConfigID || StringUtil::CharIsNull(szBufferConfigID))
    {
        return false;
    }

    PERSISTID buff = BufferModule::m_pInstance->AddBuffWithLifeTime(pKernel, self, sender,
                                                                        szBufferConfigID, iLifeTime);

    return (pKernel->Exists(buff));
}

bool FightInterface::RemoveBuffer(IKernel* pKernel,
								const PERSISTID& self,
								const char* szBufferID)
{
	if (!pKernel->Exists(self) || NULL == szBufferID || StringUtil::CharIsNull(szBufferID))
	{
		return false;
	}

	return BufferModule::m_pInstance->RemoveBuffer(pKernel, self, szBufferID);
}

// �����ɫ���ϵ�����BUFFER
void FightInterface::ClearAllBuffer(IKernel* pKernel, const PERSISTID& self)
{
    if (!pKernel->Exists(self))
    {
        return;
    }

    BufferModule::m_pInstance->ClearAllBuffer(pKernel, self);
}

/**
*@brief  ���buffer ����ǰ,ȷ���޵е�Ŀ���Ƿ��ܹ���buff,���ܼӵ�Ҫ���ñ���
*@param  [in] pKernel �����ں�ָ��
*@param  [in] self �����buffer�Ķ���
*@param  [in] sender buffer�������
*@param  [in] bufftypelist ��ControlBuff ����: bufftypelist << CB_CANT_MOVE << CB_CANT_ATTACK;
*@param  [in] iLifeTime buff����ʱ��
*@return bool ��ӳɹ�true�����򷵻�false
*/
bool FightInterface::AddControlBuffer(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& bufftypelist, const int iLifeTime)
{
	if (!pKernel->Exists(self) || iLifeTime < 0)
	{
		return false;
	}

	int nCount = (int)bufftypelist.GetCount();
	if (nCount <= 0)
	{
		return false;
	}
	LoopBeginCheck(a);
	for (int i = 0;i < nCount;++i)
	{
		LoopDoCheck(a);
		int nType = bufftypelist.IntVal(i);
		if (nType < CB_CANT_MOVE || nType >= MAX_CB_BUFF_NUM)
		{
			continue;
		}
		const char* strBuff = "";
		if (iLifeTime == CONTROL_BUFF_FOREVER)
		{
			strBuff = CONTROL_FOREVER_BUFF_ID[nType];
		}
		else
		{
			strBuff = CONTROL_TIME_BUFF_ID[nType];
		}
		
		BufferModule::m_pInstance->AddBuffWithLifeTime(pKernel, self, sender, strBuff, iLifeTime);
	}

	return true;
}

/**
*@brief  �Ƴ�������buffer
*@param  [in] pKernel �����ں�ָ��
*@param  [in] self �����buffer�Ķ���
*@param  [in] bufftypelist ��ControlBuff ����: bufftypelist << CB_CANT_MOVE << CB_CANT_ATTACK;
*@return bool ��ӳɹ�true�����򷵻�false
*/
bool FightInterface::RemoveControlBuffer(IKernel* pKernel, const PERSISTID& self, const IVarList& bufftypelist, int buffTimeType)
{
	if (!pKernel->Exists(self))
	{
		return false;
	}

	int nCount = (int)bufftypelist.GetCount();
	if (nCount <= 0)
	{
		return false;
	}
	LoopBeginCheck(a);
	for (int i = 0;i < nCount;++i)
	{
		LoopDoCheck(a);
		int nType = bufftypelist.IntVal(i);
		if (nType < CB_CANT_MOVE || nType >= MAX_CB_BUFF_NUM)
		{
			continue;
		}
		const char* strBuff = "";
		
		if (buffTimeType == CONTROL_BUFF_FOREVER)
		{
			strBuff = CONTROL_FOREVER_BUFF_ID[nType];
		}
		else
		{
			strBuff = CONTROL_TIME_BUFF_ID[nType];
		}
		
		BufferModule::m_pInstance->RemoveBuffer(pKernel, self, strBuff);
	}
	return true;
}

/**
*@brief  �����Ƴ������Ƴ�buff
*@param  [in] pKernel �����ں�ָ��
*@param  [in] self		����
*@param  [in] nClearType (��ӦBufferClear)
*@return bool ��ӳɹ�true�����򷵻�false
*/
bool FightInterface::RemoveBuffByClearType(IKernel* pKernel, const PERSISTID& self, int nClearType)
{
	if (!pKernel->Exists(self))
	{
		return false;
	}

	BufferModule::m_pInstance->RemoveBuffByClearType(pKernel, self, nClearType);
	return true;
}
