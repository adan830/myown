@echo off

rem call cmd
set OPENSSL_CONF=openssl.cnf
mkdir certs

rem ���ɸ���Կ
openssl genrsa -out ca.key 1024
rem ���ɸ�֤��(��ǩ֤��)
openssl req -new -x509 -key ca.key -out ca.crt

rem ����gmcc����֤��
openssl genrsa -out certs/gmcc.key 1024
rem ����gmcc֤��ǩ������
openssl req -new -key certs/gmcc.key -out certs/gmcc.csr 

rem ������Ҫ����һ�����ɵ�֤������csr�ļ�������CA�������ϣ���CA��ִ��
openssl ca -in certs/gmcc.csr -out certs/gmcc.crt

pause
@echo on