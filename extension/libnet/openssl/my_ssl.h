//--------------------------------------------------------------------
// �ļ���:		my_ssl.h
// ��  ��:		ssl��ʼ������ʵ��
// ˵  ��:		
// ��������:		2016��10��29��
// ������:		���
// ��Ȩ����:		���ݾ����������޹�˾
//--------------------------------------------------------------------
#ifndef __SSL_SINGTON_IMP_H__
#define __SSL_SINGTON_IMP_H__
#include <openssl/ssl.h>

class MySSL
{
private:
	MySSL(MySSL&) = delete;
	MySSL(MySSL&&) = delete;
	MySSL& operator=(MySSL&) = delete;
	MySSL& operator=(MySSL&&) = delete;

	SSL_CTX* m_sslCtx;
public:
	MySSL();

	bool Init();
	void Release();

	SSL_CTX* Context() { return m_sslCtx; }
	SSL* CreateSSL();
	void FreeSSL(SSL* ssl);
	void SetFd(SSL* ssl, size_t fd);
	void SetRFd(SSL* ssl, size_t fd);
	void SetWFd(SSL* ssl, size_t fd);
};

#endif // END __SSL_SINGTON_IMP_H__