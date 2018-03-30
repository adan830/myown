//--------------------------------------------------------------------
// �ļ���:		IEntity.h
// ��  ��:		
// ˵  ��:		
// ��������:	2007��1��31��
// ������:		½����
// ��Ȩ����:	������ţ�������޹�˾
//--------------------------------------------------------------------

#ifndef _PUBLIC_IENTITY_H
#define _PUBLIC_IENTITY_H

#include "Macros.h"
#include "PersistId.h"
#include "ICore.h"
#include "IVarTable.h"

// ʵ��ӿ�

class IEntInfo;
class IEntScript;
//class IVar;

class IEntity
{
public:
	// �ڲ�������֤�������Ч�ԣ����ɵ���
	static void __validate_parent_exists() {}

public:
	IEntity()
	{
		m_pCore = NULL;
		m_pCustoms = NULL;
		//m_pVar = NULL;
		m_pEntInfo = NULL;
		m_pEntScript = NULL;
		m_nMsgHooks = 0;
		m_bDeleted = false;
		m_bCanDelByScript = false;
	}

	virtual ~IEntity() = 0;

	// ��ʼ�����ɴ�����������ʵ�壩
	virtual bool Init(const IVarList& args) = 0;
	// �رգ���ɾ���������õĸ���ʵ�壬����Դ�ͷ�Ӧ�����������н��У�
	virtual bool Shut() = 0;
	
	// ÿִ֡���߼����������ӵ����ж��У�
	virtual void Execute(float seconds) {}
	// ÿִ֡����ʾ���������ӵ����ж��У�
	virtual void Display(float offset_seconds) {}

	// ������Ϣ����
	virtual bool MsgProc(void* hwnd, unsigned int msg, unsigned int param1, 
		unsigned int param2, int& result) { return false; }

	// �ͷ�����
	virtual void Release()
	{
		m_pCore->DeleteEntity(GetID());
	}

	// ����ڴ�ռ��
	virtual size_t GetMemoryUsage() { return 0; }
	
	// ��ú��Ľӿ�
	ICore* GetCore() const
	{
		return m_pCore;
	}

	// ����Զ������Ա�
	IVarTable* GetCustoms() const
	{
		return m_pCustoms;
	}
	//IVar* GetVar() const
	//{
	//	return m_pVar;
	//}
	
	// ���ʵ������Ϣ
	IEntInfo* GetEntInfo() const
	{
		return m_pEntInfo;
	}

	// ��ð󶨵Ľű�
	IEntScript* GetScript() const
	{
		return m_pEntScript;
	}
	
	// ��ö���ID
	PERSISTID GetID() const
	{
		return m_PersistID;
	}

	// �Ƿ���ɾ��
	bool GetDeleted() const
	{
		return m_bDeleted;
	}

	// �Ƿ���Ա��ű�ɾ��
	bool GetCanDelByScript() const
	{
		return m_bCanDelByScript;
	}

private:
	IEntity(const IEntity&);
	IEntity& operator=(const IEntity&);
	
	void SetCore(ICore* value)
	{
		m_pCore = value;
	}
	
	void SetCustoms(IVarTable* value)
	{
		m_pCustoms = value;
	}

	//void SetVar(IVar* value)
	//{
	//	m_pVar = value;
	//}
	
	void SetEntInfo(IEntInfo* value)
	{
		m_pEntInfo = value;
	}

	void SetScript(IEntScript* value)
	{
		m_pEntScript = value;
	}
	
	void SetID(const PERSISTID& value)
	{
		m_PersistID = value;
	}

	void SetDeleted(bool value)
	{
		m_bDeleted = value;
	}

	void SetCanDelByScript(bool value)
	{
		m_bCanDelByScript = value;
	}

	void IncMsgHooks()
	{
		++m_nMsgHooks;
	}

	void DecMsgHooks()
	{
		--m_nMsgHooks;
	}

	int GetMsgHooks() const
	{
		return m_nMsgHooks;
	}
	
private:
	ICore* m_pCore;
	IVarTable* m_pCustoms;
	//IVar* m_pVar;
	IEntInfo* m_pEntInfo;
	IEntScript* m_pEntScript;
	int m_nMsgHooks;
	bool m_bDeleted;
	bool m_bCanDelByScript;
	PERSISTID m_PersistID;

	friend class CEntFactory;
};

inline IEntity::~IEntity() {}

#endif // _PUBLIC_IENTITY_H

