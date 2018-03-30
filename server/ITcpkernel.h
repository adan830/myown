//--------------------------------------------------------------------
// �ļ���:		ITcpkernel.h
// ��  ��:		
// ˵  ��:		
// ��������:	2014��12��24��
// ������:		xiecy
// ��Ȩ����:	������ţ�������޹�˾
//--------------------------------------------------------------------

#ifndef _I_TCP_KERNEL_H_
#define _I_TCP_KERNEL_H_

class IVarList;

class ITcpKernel
{
public:
	virtual ~ITcpKernel() = 0;

	//������Ϣ����Ϸ������
	virtual bool SendMsgToGameServer(int connid, const IVarList& args) = 0;

	// ��ȡApp���˺�
	virtual const char* GetTaskDevAccount() = 0;

	// ��ȡApp����
	virtual const char* GetTaskDevPwd() = 0;

	// ��ȡ��ʱͨѶ�Ƶ�Ӧ��ID
	virtual const char* GetTaskAppKeyIM() = 0;

	// ��ȡ����ͨѶ�Ƶ�Ӧ��ID
	virtual const char* GetTaskAppKeyVoice()= 0;
};

inline ITcpKernel::~ITcpKernel() {}

#endif //_I_TCP_KERNEL_H_