//--------------------------------------------------------------------
// �ļ���:		JadeDefine.h
// ��  ��:		����
// ˵  ��:		
//
// ��������:    2018��03��23��
// ������:      ������      
//--------------------------------------------------------------------
#ifndef __JADE_DEFINE_H__
#define __JADE_DEFINE_H__

#define JADE_BOX_WSTR L"JadeBox"
#define JADE_BOX_STR  "JadeBox"
#define MAX_JADE_BOX_CAPACITY	10		// ���������������

// ����ȼ����԰���ʶ
#define JADE_PROPPAK_ID "JadeProp"

// ������Ϣ
enum JadeC2SMsg
{
	JADE_C2S_LEVEL_UP,		// �������� string ����id
	JADE_C2S_STEP_UP,		// ��������	string ����id
	JADE_C2S_COMPOSE,		// ����ϳ�	string ����id
	JADE_C2S_WEAR,			// ��������	string ����id
};


enum JadeS2CMsg
{
	JADE_S2C_LEVEL_UP_SUC,	// ����ɹ�
	JADE_S2C_STEP_UP_SUC,// �������׳ɹ�
};


#endif	// __Jade_DEFINE_H__
