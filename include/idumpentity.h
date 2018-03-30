//--------------------------------------------------------------------
// �ļ���:		IEntity.h
// ��  ��:		
// ˵  ��:		
// ��������:	2007��1��31��
// ������:		½����
// ��Ȩ����:	������ţ�������޹�˾
//--------------------------------------------------------------------

#ifndef _PUBLIC_IDUMPENTITY_H
#define _PUBLIC_IDUMPENTITY_H

#include "../../public/IEntity.h"
#include "../../public/Macros.h"
#include "../../public/PersistId.h"
#include "../../public/ICore.h"
#include "../../public/IVarTable.h"


enum module_status_type
{
	MST_UNKNOWN,	/// δ֪״̬
	MST_STARTED,	/// ������
	MST_RUNNING,	/// ���ڴ�����
	MST_STOPED,		/// ��ֹͣ

	MST_MAX,
};

// ʵ��ӿ�

class IDumpManager;
//class IVar;

class IDumpEntity : public IEntity
{
public:
	IDumpEntity()
	{
		m_nModuleStatus = MST_UNKNOWN;
	}

	virtual ~IDumpEntity();

	//// ��ʼ�����ɴ�����������ʵ�壩
	//virtual bool Init(const IVarList& args) = 0;
	//// �رգ���ɾ���������õĸ���ʵ�壬����Դ�ͷ�Ӧ�����������н��У�
	//virtual bool Shut() = 0;
	//
	//// ÿִ֡���߼����������ӵ����ж��У�
	//virtual void Execute(float seconds) {}	

	//// ����ڴ�ռ��
	//virtual size_t GetMemoryUsage() { return 0; }

	/// ����������Ϣ����
	virtual void OnManagerCommand(const IVarList& args, IVarList& ret) {};
	/// �û��Զ���ѭ������
	virtual void OnUserLoop() {}
	virtual void OnEvent() {}
	/// ������������
	virtual void OnConnectAccept(const char* name, int index, int serial, const char* addr, int port) {}
	/// �յ�������Ϣ
	virtual void OnConnectReceive(const char* name, int index, int serial, const IVarList& msg) {}
	/// �Ͽ���������
	virtual void OnConnectClose(const char* name, int index, int serial, const char* addr, int port) {}

	

	/// ģ��״̬
	int GetModuleStatus() { return m_nModuleStatus; }
	void SetModuleStatus(int status) { m_nModuleStatus = status; }

	bool IsStatusReady() { return m_nModuleStatus == MST_STARTED; }
	bool IsStatusProcessing() { return m_nModuleStatus == MST_RUNNING; }
	bool IsStatusStoped() { return m_nModuleStatus == MST_STOPED; }

	IDumpManager* GetDumpMgr() {
		return m_pDumpManager;
	}
	void SetDumpMgr(IDumpManager* pMgr) { m_pDumpManager = pMgr; }

private:
	IDumpEntity(const IDumpEntity&);
	IDumpEntity& operator=(const IDumpEntity&);

protected:
	IDumpManager* m_pDumpManager;
	int m_nModuleStatus;	
};

inline IDumpEntity::~IDumpEntity() {}

#endif // _PUBLIC_IDUMPENTITY_H

