//--------------------------------------------------------------------
// �ļ���:		ISnsLogic.h
// ��  ��:		���ݷ������߼����ݿռ�ӿ�
// ˵  ��:		
// ��������:	2014��06��10��
// ��������:	2014��06��10��
// ������:		  ( )
// �޸���:     
//    :	    
//--------------------------------------------------------------------

#ifndef __ISnsLogic_h__
#define __ISnsLogic_h__

#include "../../server/ISnsCallee.h"
#include "../../server/ISnsKernel.h"

class ISnsLogic
{
public:
	ISnsLogic(const std::wstring & sns_name, const int sns_id = 0) :m_snsName(sns_name), m_snsId(sns_id){}

	virtual ~ISnsLogic(){}

public:
	virtual const std::wstring & GetSnsName(ISnsKernel * pSnsKernel){ return m_snsName; }

	virtual int GetSnsId(ISnsKernel * pSnsKernel){ return m_snsId; }
	virtual bool GetSave(){ return true; }

	// \brief Sns�߼����󱻴���
	// \param pKernel ����ָ�룬��ͬ
	// \param args ������
	virtual int OnModuleCreate(ISnsKernel* pSnsKernel, const IVarList& args) = 0;

	// \brief Sns���������Ե�Sns�����Ѿ��������׼������(������OnSnsLoad֮��)
	// \param pKernel ����ָ�룬��ͬ
	// \param args ������
	virtual int OnReady(ISnsKernel* pSnsKernel, const IVarList& args) = 0;

	// \brief sns�������ڴ��д���
	// \param args
	// \uid  sns uid //sns���ݹؼ���
	// \args  
	virtual int OnCreate(ISnsKernel* pSnsKernel, const char* uid, const IVarList& args) = 0;

	// \brief ����sns����
	// \uid  sns uid //sns���ݹؼ���
	// \args  
	virtual int OnLoad(ISnsKernel* pSnsKernel, const char* uid, const IVarList& args) = 0;

	// \brief �յ���Ϣ
	// \param args
	// \distribute_id ����id
	// \server_id  ���������
	// \memeber_id  �������������
	// \scene_id  ���ͳ�����id
	// \uid  sns uid //sns���ݹؼ���
	// \args  member���͵���Ϣ��
	virtual int OnMessage(ISnsKernel* pSnsKernel, int distribute_id, int server_id,
		int memeber_id, int scene_id, const char* uid, const IVarList& args) = 0;
	virtual bool OnUpdateConfig(ISnsKernel* pSnsKernel, int distribute_id, int server_id,
		int memeber_id, int scene_id, const char* uid, const IVarList& args){return false;};

protected:
	std::wstring m_snsName;
	int m_snsId;

};

#endif //__ISnsLogic_h__
