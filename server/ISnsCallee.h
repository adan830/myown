

#ifndef _SERVER_ISNSCALLEE_H
#define _SERVER_ISNSCALLEE_H

#include "../public/Macros.h"
#include "../public/IVarList.h"

// �������ݻص��ӿ�

class ISnsKernel;

class ISnsCallee
{
public:
	virtual ~ISnsCallee() = 0;

	// \brief Sns����������������ʱ�߼�ģ���Ѽ������
	// \param pKernel ����ָ�룬��ͬ
	virtual int OnServerStarted(ISnsKernel* pKernel){return 1;}

	// \brief Sns�߼����󱻴���
	// \param pKernel ����ָ�룬��ͬ
	// \param args ������
	virtual int OnSnsModuleCreate(ISnsKernel* pKernel, const IVarList& args) {return 1;}

	// \brief Sns���������Ե�Sns�����Ѿ��������׼������(������OnSnsLoad֮��)
	// \param pKernel ����ָ�룬��ͬ
	// \param args ������
	virtual int OnServerReady(ISnsKernel* pKernel, const IVarList& args) {return 1;}

	// \brief sns�������ڴ��д���
	// \param args
	// \uid  sns uid //sns���ݹؼ���
	// \args  
	virtual int OnSnsDataCreate(ISnsKernel* pKernel, const char* uid,const IVarList& args) {return 1;}

	// \brief ����sns����
	// \uid  sns uid //sns���ݹؼ���
	// \args  
	virtual int OnSnsDataLoad(ISnsKernel* pKernel, const char* uid,const IVarList& args) {return 1;}

	// \brief sns ���ݴ��ڴ���ж��
	// \uid  sns uid //sns���ݹؼ���
	// \args  
	virtual int OnSnsDataUnload(ISnsKernel* pKernel,const char* uid, const IVarList& args) {return 1;}

	// \brief sns ���ݴ����ݿ���ɾ��
	// \uid  sns uid //sns���ݹؼ���
	// \args  
	virtual int OnSnsDataRemove(ISnsKernel* pKernel, const char* uid,const IVarList& args) {return 1;}

	// \brief �յ���Ϣ
	// \param args
	// \distribute_id ����id
	// \server_id  ���������
	// \memeber_id  �������������
	// \scene_id  ���ͳ�����id
	// \uid  sns uid //sns���ݹؼ���
	// \args  member���͵���Ϣ��
	virtual int OnMemberMessage(ISnsKernel* pKernel, int distribute_id,int server_id,int memeber_id,int scene_id, const char* uid,const IVarList& args) {return 1;}

	/// \brief Sns�������ر���ǰ��Ϣ֪ͨ
	/// \param nServerCloseDelay ��ʣ��Ķ���ʱ��������ر�
	virtual int OnServerCloseBeforeNotify(ISnsKernel* pKernel, int nServerCloseDelay){ return 1; }

	/// \brief ������ÿ֡����
	/// \param nFrameTicks ��֡�ĺ�����
	virtual int OnFrameExecute(ISnsKernel* pCrossKernel, int nFrameTicks) { return 1; }

	/// \brief sns�������ر�֪ͨ
	virtual int OnServerClose(ISnsKernel* pKernel){ return 1; }
};

inline ISnsCallee::~ISnsCallee() {}

#endif // _SERVER_IPUBCALLEE_H

