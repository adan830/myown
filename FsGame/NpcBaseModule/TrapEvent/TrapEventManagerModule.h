//--------------------------------------------------------------------
// �ļ���:      TrapEventManager.h
// ��  ��:      ���崥���¼�������
// ˵  ��:
// ��������:    2015��07��13��
// ������:        
//    :       
//--------------------------------------------------------------------
#ifndef _TrapEventManager_H_
#define _TrapEventManager_H_

#include "Fsgame/Define/header.h"
#include "FsGame/Define/TrapEventFuncIdDefine.h"

class TrapEventBase;
class TrapEventManagerModule : public ILogicModule
{
public:
    TrapEventManagerModule()  {
		memset(m_pEventContainer, 0, sizeof(m_pEventContainer));
	}
    ~TrapEventManagerModule() {}

public:
    // ��ʼ��
    virtual bool Init(IKernel* pKernel);

    // ����
    virtual bool Shut(IKernel* pKernel);

public:
    // �����¼�������
    bool InitEventExecutes(IKernel* pKernel);

	// ������ִ�в���
	bool ExecuteOperate(IKernel* pKernel, const PERSISTID& self, const PERSISTID& object, const SpringEvent funcID);
private:
	// �¼�ע��
	bool RegistTrapEvent(const SpringEvent func_id);
private:
	TrapEventBase*		m_pEventContainer[SPRINGEVENT_MAX];			// ���崥���¼�����
public:
    static TrapEventManagerModule*   m_pInstance;
};

#endif // _SkillEventManager_H_
