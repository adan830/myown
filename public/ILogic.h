//--------------------------------------------------------------------
// �ļ���:		ILogic.h
// ��  ��:		
// ˵  ��:		
// ��������:	2007��12��25��
// ������:		½����
// ��Ȩ����:	������ţ�������޹�˾
//--------------------------------------------------------------------

#ifndef _PUBLIC_ILOGIC_H
#define _PUBLIC_ILOGIC_H

#include "Macros.h"
#include "ILogicInfo.h"
#include "ILogicCreator.h"

// ILogic
// ʵ��ӿ�

class IEntity;

class ILogic
{
public:
	ILogic()
	{
		m_pEntity = NULL;
		m_pLogicInfo = NULL;
	}

	virtual ~ILogic() = 0;

	// ��ʼ��
	virtual bool Init(const IVarList& args) { return true; }
	// �ر�
	virtual bool Shut() { return true; }
	
	// �ͷ�����
	virtual void Release()
	{
		m_pLogicInfo->GetCreator()->Destroy(this);
	}
	
	// ��ð󶨶���
	IEntity* GetEntity() const
	{
		return m_pEntity;
	}
	
	// ����߼�����Ϣ
	ILogicInfo* GetLogicInfo() const
	{
		return m_pLogicInfo;
	}

private:
	ILogic(const ILogic&);
	ILogic& operator=(const ILogic&);
	
	void SetEntity(IEntity* value)
	{
		m_pEntity = value;
	}
	
	void SetLogicInfo(ILogicInfo* value)
	{
		m_pLogicInfo = value;
	}

private:
	IEntity* m_pEntity;
	ILogicInfo* m_pLogicInfo;

	friend class CCore;
};

inline ILogic::~ILogic() {}

#endif // _PUBLIC_ILOGIC_H

