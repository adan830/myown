//--------------------------------------------------------------------
// �ļ���:		INetType.h
// ��  ��:		�������Ͷ���
// ˵  ��:		
// ��������:		2016��5��28��
// ������:		���
// ��Ȩ����:		���ݾ����������޹�˾
//--------------------------------------------------------------------
#ifndef __INET_TYPE_DEFINE_H__
#define __INET_TYPE_DEFINE_H__

#include <stdlib.h>

#if defined( WIN32) || defined( WIN64 )
typedef intptr_t NETFD;
#else
#define NETFD int
#endif

typedef unsigned char	SERID;
typedef long long		SESSID;
typedef SESSID NID;

#define  NULL_NID		0

#endif	// END __INET_TYPE_DEFINE_H__