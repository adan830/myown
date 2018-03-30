//--------------------------------------------------------------------
// �ļ���:		IHttpsCallee.h
// ��  ��:		Httpsģ��ӿ�
// ˵  ��:		
// ��������:	2015��01��06��
// ������:		xiecy
// ��Ȩ����:	������ţ�������޹�˾
//--------------------------------------------------------------------
#ifndef _I_VOICE_CALLEE_H
#define _I_VOICE_CALLEE_H

#include "../public/Macros.h"
#include "../public/IVarList.h"
#include "../server/IKernel.h"
#include "../server/ICreator.h"
#include "../server/https/IHttpsKernel.h"
#include "../server/ITcpKernel.h"

/*
�̴���ILogicModule��ֻ��Ϊ�˹��ù����Ľӿڣ����ǲ�����Ҫȥʵ����
*/
class IHttpsCallee : public ILogicModule
{
public:
	virtual ~IHttpsCallee() = 0;

	/// \brief	�߼�ģ���ʼ��
	/// \param	pHttpKernel	HTTPS kernel
	/// \return	bool		���ش�����
	virtual bool OnHttpsStart(IHttpsKernel* pHttpKernel) = 0;

	/// \brief	���յ��ⲿ������
	/// \param	connid		����id
	/// \param	addr		��ַ
	/// \param	port		�˿ں�
	/// \return	void
	virtual void OnOuterAccept(int connector_id, const char* addr, int port) = 0;

	/// \brief	���յ��ⲿ�����ӹر�
	/// \param	connid		����id
	/// \param	addr		��ַ
	/// \param	port		�˿ں�
	/// \return	void
	virtual void OnOuterClose(int connector_id, const char* addr, int port) = 0;

	/// \brief	�յ��ⲿ����
	/// \param	connid		����id
	/// \param	args		varlist
	/// \return	int
	virtual int OnOuterCommand( int connid, const IVarList& args ) = 0;

	//---------------------------------------------------------------------------

	// ��ʼ��
	virtual bool Init(IKernel* pKernel) { return true; }

	// �ر�
	virtual bool Shut(IKernel* pKernel) { return true; }

	// ��������֮ǰ�ĳ�ʼ��������OnCreateClass�Ѿ�ִ����ϣ�
	virtual bool BeforeLaunch(IKernel* pKernel) { return true; }
};

inline IHttpsCallee::~IHttpsCallee() {}

#endif // _I_VOICE_CALLEE_H

