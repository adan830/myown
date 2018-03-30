//--------------------------------------------------------------------
// �ļ���:		ssl_env.h
// ��  ��:		ssl_env��������
// ˵  ��:		
// ��������:		2016��10��29��
// ������:		���
// ��Ȩ����:		���ݾ����������޹�˾
//--------------------------------------------------------------------
#ifndef __MY_SSL_ENV_H__
#define __MY_SSL_ENV_H__

enum EmSSLvMethod
{
	EM_ENV_SSL_METHOD_V1,			/* TLSv1.0 */
	EM_ENV_SSL_METHOD_V1_1,			/* TLSv1.1 */
	EM_ENV_SSL_METHOD_V1_2,			/* TLSv1.2 */
	EM_ENV_SSL_METHOD_V2,			/* TLSv2.0 */
	EM_ENV_SSL_METHOD_V3,			/* TLSv3.0 */
	EM_ENV_SSL_METHOD_V23,			/* TLSv2.0 & 3.0 */
	EM_ENV_SSL_METHOD_DV1,			/* DTLSv1.0 */
	EM_ENV_SSL_METHOD_DV1_2,		/* DTLSv1.2 */
	EM_ENV_SSL_METHOD_DV1_12,		/* DTLS 1.0 and 1.2 */
};

enum EmSSLVerifyMode
{
	EM_ENV_SSL_VERIFY_NONE                 = 0x00,
	EM_ENV_SSL_VERIFY_PEER                 = 0x01,
	EM_ENV_SSL_VERIFY_FAIL_IF_NO_PEER_CERT = 0x02,
	EM_ENV_SSL_VERIFY_CLIENT_ONCE          = 0x04,
};

#ifdef __cplusplus
extern "C" {
#else
#define const	// c not have const
#endif
	// ctx��X509_STORE_CTX
	typedef int(*ssl_verfiy_cb)(int, void* ctx);

	// ����֤����֤ģʽ���Զ�����֤�ص�(Ĭ��ΪEM_ENV_SSL_VERIFY_NONE, �ص�ΪNULL)
	void ssl_env_set_verify(EmSSLVerifyMode mode, ssl_verfiy_cb cb);
	// ��ȡ֤����֤ģʽ
	EmSSLVerifyMode ssl_env_get_verfiy_mode();
	// ��ȡ�ص�
	ssl_verfiy_cb ssl_env_get_verfiy_cb();

	// ���ô�������(Ĭ��ΪEM_ENV_SSL_METHOD_V23)
	void ssl_env_set_method(EmSSLvMethod method);
	EmSSLvMethod ssl_env_get_metod();

	// ���ø�֤��(Ĭ��Ϊcerts/ca.cert)
	void ssl_env_set_ca(const char* szCA);
	const char* ssl_env_get_ca();

	// ��������֤��·��(Ĭ��certs/trust)
	void ssl_env_set_trust(const char* szTrustPath);
	const char* ssl_env_get_trust();

	// �����Լ���֤��(Ĭ��ΪNULL,��ʹ��)
	void ssl_env_set_certificate_file( const char* cert );
	const char* ssl_env_get_certificate_file();

	// �����Լ���˽Կ
	void ssl_env_set_private_key(const char* key);
	const char* ssl_env_get_private_key();

	// �����Լ���˽Կ����
	void ssl_env_set_private_pass(const char* key);
	const char* ssl_env_get_private_pass();
#ifdef __cplusplus
}
#endif

#endif // END __OPEN_SSL_ENV_H__