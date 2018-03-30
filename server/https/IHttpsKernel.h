//--------------------------------------------------------------------
// �ļ���:		IHttpsKernel.h
// ��  ��:		
// ˵  ��:		
// ��������:	2014��12��24��
// ������:		xiecy
// ��Ȩ����:	������ţ�������޹�˾
//--------------------------------------------------------------------
#ifndef _I_HTTPS_KERNEL_H_
#define _I_HTTPS_KERNEL_H_

#include "IHttpsResponse.h"

// HTTPS�������߼��ӿڰ汾
#define HTTPS_LOGIC_MODULE_VERSION 0x0001

class IVarList;

class IHttpsKernel
{
public:

	virtual ~IHttpsKernel() = 0;

	/// \brief	��־��ӡ
	/// \param	info		��־����
	/// \return	void		����void
	virtual void WriteInfo(const char* info) = 0;

	/// \brief	���HTTPS�������ַ
	/// \param	reqUrlIndex	����HTTPS��ַ������
	/// \param	url			����HTTPS��ַ
	/// \return	void		����void
	virtual void AddHttpsRequestUrl(int reqUrlIndex, const char* url) = 0;

	/// \brief	������Ϣ����Ϸ������
	/// \param	connid		����ID
	/// \param	args		���
	/// \return	bool		����bool
	virtual bool SendMsgToGameServer(int connid, const IVarList& args) = 0;

	/// \brief	����https����
	/// \param	connid		����ID
	/// \param	reqUrlIndex	����HTTPS��ַ������
	/// \param	command		��������(Post��ʽ��������,Get��ʽ���������Ӳ���)
	/// \param	paramlist	�������(Post��ʽ���в�����Get��ʽ���޲���)
	/// \param	msgId		��ϢID
	/// \param	flagId		������Ϣ�ı�ʶ
	/// \param	cb			�ص���ַ
	/// \param	content		�ص�����
	/// \return	bool		����bool
	virtual bool SendHttpsRequest(int connid, int reqUrlIndex, const char* command, const char* paramlist,
		int msgId, const char* flagId = "",HTTPS_ON_RES_CALLBACK cb = NULL,void* content = NULL) = 0;

	/// \brief	����https����,���Զ���header
	/// \param	connid		����ID
	/// \param	reqUrlIndex	����HTTPS��ַ������
	/// \param	command		��������(Post��ʽ��������,Get��ʽ���������Ӳ���)
	/// \param	paramlist	�������(Post��ʽ���в�����Get��ʽ���޲���)
	/// \param	custom_header_name		�����Զ���header����(CVarList() << "If-Modified-Since")
	/// \param	custom_header_content	�����Զ���header����(CVarList() << "Fri, 13 May 2016 03:40:07 GMT")
	/// \param	msgId		��ϢID
	/// \param	flagId		������Ϣ�ı�ʶ
	/// \param	cb			�ص���ַ
	/// \param	content		�ص�����
	/// \return	bool		����bool
	virtual bool SendHttpsRequestEx(int connid, int reqUrlIndex, const char* command, const char* paramlist,
		const IVarList& custom_header_name, const IVarList& custom_header_content,
		int msgId, const char* flagId = "",HTTPS_ON_RES_CALLBACK cb = NULL,void* content = NULL) = 0;
};

inline IHttpsKernel::~IHttpsKernel() {}


#endif // _I_HTTPS_KERNEL_H_