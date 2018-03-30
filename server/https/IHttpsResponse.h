//--------------------------------------------------------------------
// �ļ���:		IHttpsResponse.h
// ��  ��:		
// ˵  ��:		
// ��������:	2014��12��24��
// ������:		xiecy
// ��Ȩ����:	������ţ�������޹�˾
//--------------------------------------------------------------------

#ifndef _I_HTTPS_RESPONSE_H_
#define _I_HTTPS_RESPONSE_H_

#include <queue>
#include <string>

class IHttpsResponse;

typedef void (*HTTPS_ON_RES_CALLBACK)(IHttpsResponse*, void *);

//HTTPS����ظ�
class IHttpsResponse
{
public:

	virtual ~IHttpsResponse() = 0;

	/// \brief ��ȡ����ID
	virtual int GetConnid() = 0;

	/// \brief ��ȡ��ϢID
	virtual int GetMsgId() = 0;

	/// \brief ��ȡhttps�����ʶ
	virtual const char* GetHttpsResquestId() = 0;

	/// \brief ��ȡhttps���󷵻ص���Ϣ
	virtual const char* GetHttpsResponse() = 0;
};

inline IHttpsResponse::~IHttpsResponse() {}

#endif //_I_HTTPS_RESPONSE_H_