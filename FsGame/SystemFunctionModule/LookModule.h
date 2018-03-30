//----------------------------------------------------------
// �ļ���:      LookModule.h
// ��  ��:      �鿴��Ϣ
// ˵  ��:
// ��������:
// ������:             
//----------------------------------------------------------
#ifndef __LookModule_h__
#define __LookModule_h__

#include "Fsgame/Define/header.h"

class ContainerModule;
class LookModule : public ILogicModule
{
public:
	//��ʼ��
	virtual bool Init(IKernel* pKernel);
	
	//�ͷ�
	virtual bool Shut(IKernel* pKernel);
	
public:	
	//��Ϣ�ص�
	//�鿴������ɫ��װ��
	static int OnCustomLookEquips(IKernel * pKernel, const PERSISTID & self, 
		const PERSISTID & sender, const IVarList & args);

	//�鿴������ɫ�ı����е�ĳ����Ʒ
	static int OnCustomLookItem(IKernel * pKernel, const PERSISTID & self, 
        const PERSISTID & sender, const IVarList & args);
        
    //�鿴������ɫչʾ��ĳ����Ʒ
    static int OnCustomLookChatItem(IKernel * pKernel, const PERSISTID & self, 
        const PERSISTID & sender, const IVarList & args);

	//����ص�
	//��������ɫ�鿴װ��
	static int OnCommandBeLookEquips(IKernel * pKernel, const PERSISTID & self,
		const PERSISTID & sender, const IVarList & args);

	//�鿴������ɫ��װ���ظ�
	static int OnCommandLookEquipsEcho(IKernel * pKernel, const PERSISTID & self,
		const PERSISTID & sender, const IVarList & args);

	//��������ɫ�鿴�����е�ĳ����Ʒ
	static int OnCommandBeLookItem(IKernel * pKernel, const PERSISTID & self,
		const PERSISTID & sender, const IVarList & args);

	//�鿴������ɫ�ı����е�ĳ����Ʒ�Ļظ�
	static int OnCommandLookItemEcho(IKernel * pKernel, const PERSISTID & self,
		const PERSISTID & sender, const IVarList & args);


public:

	//�鿴������ɫ��װ��
	void LookEquips(IKernel * pKernel, const PERSISTID & self, const wchar_t * destname);

	//��������ɫ�鿴װ��
	void BeLookEquips(IKernel * pKernel, const PERSISTID & self, const wchar_t * srcname);

	//�鿴������ɫ��װ���ظ�
	void LookEquipsEcho(IKernel * pKernel, const PERSISTID & self, const wchar_t * destname, 
		const IVarList & infos);

	//�鿴������ɫ�ı����е�ĳ����Ʒ
	void LookItem(IKernel * pKernel, const PERSISTID & self, const wchar_t * destname, const char * uniqueid);

	//��������ɫ�鿴�����е�ĳ����Ʒ
	void BeLookItem(IKernel * pKernel, const PERSISTID & self, const wchar_t * srcname, const char * uniqueid);

	//�鿴������ɫ�ı����е�ĳ����Ʒ�Ļظ�
	void LookItemEcho(IKernel * pKernel, const PERSISTID & self, const wchar_t * destname, const char * info, int type, const char* uniqueid);

private:

    // �鿴�������չʾ����Ʒ
    static bool LookRoleShowoffItem(IKernel * pKernel, const PERSISTID & self, const wchar_t * destname,
        const char * item_unique_id);

    // ��ȡ��Ʒչʾ��¼��
    static IRecord* GetShowoffItemRecord(IKernel * pKernel, const PERSISTID & self);
    
public:
	static LookModule * m_pLookModule;
	static ContainerModule* m_pContainerModule;
};
#endif