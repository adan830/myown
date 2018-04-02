//--------------------------------------------------------------------
// �ļ���:		WingDefine.h
// ��  ��:		���
// ˵  ��:		
//
// ��������:    2018��03��15��
// ������:      ������      
//--------------------------------------------------------------------
#ifndef __WING_DEFINE_H__
#define __WING_DEFINE_H__

// ���ȼ����԰���ʶ
#define WING_PROPPAK_ID "WingProp"

// ������Ϣ
enum WingC2SMsg
{
	WING_C2S_LEVEL_UP,		// �������
	WING_C2S_STEP_UP,		// �������
	WING_C2S_WEAR,			// �������
	WING_C2S_HIDE,			// ���س��
	WING_C2S_CHANGE_MODEL,	// �ı���� int nWingModel 
};


enum WingS2CMsg
{
	WING_S2C_LEVEL_UP_SUC,	// ���ɹ�
	WING_S2C_STEP_UP_RESULT,// ������׽�� int ���׽�� int ���ӵ�ף��ֵ(ʧ��ʱ����)
};

// ���׽��
enum 
{
	WING_STEP_UP_FAILED,	// ����ʧ�� ��ף��ֵ	
	WING_STEP_UP_SUC,		// ���׳ɹ�
};

// ������
enum 
{
	NO_WEAR_WING,			
	WEAR_WING,				
};


#endif	// __WING_DEFINE_H__
