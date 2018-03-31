//--------------------------------------------------------------------
// �ļ���:		GameDefine.h
// ��  ��:		ͨ�ó��ö���ģ��
// ˵  ��:		һ�������ַ��Ⱥ궨��
//				
// ��������:	2014��06��27��
// ��������:	2014��06��27��
// ������:		  ( )
//    :	    
//--------------------------------------------------------------------

#ifndef __GameDefine_h__ 
#define __GameDefine_h__

#include "utils/util_macros.h"
#include "Classes.h"
#include "Fields.h"
#include "InternalFields.h"

// �����������˵�ʱ����Ҫ�˺�
//#define CREATE_ROBOT

//ÿ�η��͵�����  
#ifdef _DEBUG

#define PER_SEND_COUNT  2 //����Ϊ0

#else

#define PER_SEND_COUNT  30 //����Ϊ0

#endif

#define RUN_SPEED  PI2

#define SAFE_DELETE(p) if(NULL != p) {delete(p);(p) = NULL;}

#define SAFE_ARRAY_DELETE(p) if(NULL != p) {delete[](p);(p) = NULL;}

#define _STR(s) #s
#define STR(s) _STR(s)

#define TRY_MODIFY_MOVE_TIME 100
#define TRY_MODIFY_MOVE_TIME_MAX 10000

//ָ������ǰ���ԣ�֮��ƫ�ƶ�̬��·
#define TRY_RETRY_PREPATH_MAX 3

#define DISTANCE_ADJUST_MAX 15.0f

#define OUTDOOR_SCENE_MAX  500    //Ұ����󳡾����-500



// Ĭ��MemberId
const int MEMBER_ID_DEFAULT = 0;

// �Ƿ�Ϊ�ڲ�����״̬
#define INTERNAL_TEST_STATE

// ��Ϸ��������ռ�ø�����
enum OBJECT_TYPES_PATH_GRID_ENUM
{
	TYPE_PLAYER_PATH_GRID = 1,	        // ���
	TYPE_PLAYER_DEAD_PATH_GRID = 0,	    // ����״̬��ռ�õĸ�����
	TYPE_PLAYER_PATH_GRID_OFF = 0,	    // ��Ҳ�ռ��
	TYPE_PLAYER_PATH_GRID_ON = 1,	    // ���ռ��
};

// ��������
enum CAPITAL_TYPE
{
	CAPITAL_MIN_TYPE = 0,

	CAPITAL_COPPER = 0,			// $ 0 = ͭ��
	CAPITAL_GOLD = 1,			// $ 1 = Ԫ��
	CAPITAL_TSILVER = 2,		// $ 2 = ������Ԫ�������У�
	CAPITAL_BIND_GOLD = 3,		// $ 3 = ��Ԫ��
	CAPITAL_SMELT = 4,			// $ 4 = ����ֵ
	CAPITAL_AREAN = 5,			// $ 4 = ��������

	CAPITAL_MAX_TYPE
};

// ��͸
enum
{
	PATH_THROUGH_OFF = 0,				// ��
	PATH_THROUGH_ON = 1,				// ��
};

// ���״̬
enum EmPlayerBronState
{
	EM_PLAYER_STATE_BRON,			// ����״̬
	EM_PLAYER_STATE_INIT,			// ��ʼ���ɹ���δ����clientInfo)
	EM_PLAYER_STATE_NORMAL,			// ����״��
};

// ��Ϣ����
enum CustomDisplayType
{
	CUSTOM_ADD_HP = 0,				// ��HP	int hp
	CUSTOM_DEC_HP_WITH_EFFECT,		// ��HP+��HP��Ч�� int hp 
	CUSTOM_PLAY_CG,					// ����CG string cg float x z x z
}; 

// �ɷ��ƶ�
enum CantMoveType
{
	MOVE_OK = 0, // �����ƶ�
	MOVE_NO = 1, // ���� 0 �������ƶ�
};

// ����
enum WeekDay
{
	EWEEK_SUNDAY,
	EWEEK_MONDAY,
	EWEEK_TUESDAY,
	EWEEK_WEDESDAY,
	EWEEK_THURSDAY,
	EWEEK_FRIDAY,
	EWEEK_SATURDAY,
};

// ��������
enum EnergyType
{
	ENERGY_NONE = -1,
	ENERGY_HP,
	ENERGY_MP,	

	ENERGY_MAX
};

// �Ƿ���ʾ���Ѷ���
enum
{
	NEW_NOTIFY_HIDE = 0,
	NEW_NOTIFY_SHOW = 1,
};

// �����󸽼���ʾ���� MaxVisuals = VisualPlayers + MAX_VISUALS_ADD
const int MAX_VISUALS_ADD = 10;


void OnConfigError( bool bError, const char* msg );
#endif

#define ADD_COUNT_BEAT(pKernel, self, sFunc, nTime, nCount)\
if(!pKernel->FindHeartBeat(self, sFunc))\
{\
	pKernel->AddCountBeat(self,sFunc,nTime,nCount);\
}

//�������
#define ADD_HEART_BEAT(pKernel, self, sFunc, nTime)\
if(!pKernel->FindHeartBeat(self, sFunc))\
{\
	pKernel->AddHeartBeat(self,sFunc,nTime);\
}

//ɾ������
#define DELETE_HEART_BEAT(pKernel, self, sFunc)\
if(pKernel->FindHeartBeat(self, sFunc))\
{\
	pKernel->RemoveHeartBeat(self,sFunc);\
}

//��ӻص�
#define ADD_CRITICAL(pKernel, self, property, sFunc)\
if(!pKernel->FindCritical( self, property, sFunc))\
{ \
	pKernel->AddCritical( self, property, sFunc );\
}

//ɾ�����Իص�
#define DELETE_CRITICAL(pKernel, self, property, sFunc)\
	if(pKernel->FindCritical( self, property, sFunc))\
{ \
	pKernel->RemoveCriticalFunc( self, property, sFunc );\
}

//�����ʱ����
#define ADD_DATA(pKernel, self, property, propertyType)\
	if(!pKernel->FindData(self, property))\
{ \
	pKernel->AddData(self, property, propertyType);\
}

//ɾ����ʱ����
#define REMOVE_DATA(pKernel, self, property)\
	if(pKernel->FindData(self, property))\
{ \
	pKernel->RemoveData(self, property);\
}


//��ӻص�
#define ADD_RECHOOK(pKernel, self, rec, sFunc)\
	if(!pKernel->FindRecHook( self, rec, sFunc))\
{ \
	pKernel->AddRecHook( self, rec, sFunc );\
}

//�����ʱ����
#define ADD_DATA_INT(pSelfObj, property)\
	if(pSelfObj != NULL && !pSelfObj->FindData(property))\
{ \
	pSelfObj->AddDataInt(property, 0);\
}
#define ADD_DATA_INT64(pSelfObj, property)\
	if(pSelfObj != NULL && !pSelfObj->FindData(property))\
{ \
	pSelfObj->AddDataInt64(property, 0);\
}
#define ADD_DATA_FLOAT(pSelfObj, property)\
	if(pSelfObj != NULL && !pSelfObj->FindData(property))\
{ \
	pSelfObj->AddDataFloat(property, 0.0f);\
}
#define ADD_DATA_DOUBLE(pSelfObj, property)\
	if(pSelfObj != NULL && !pSelfObj->FindData(property))\
{ \
	pSelfObj->AddDataDouble(property, 0.0f);\
}
#define ADD_DATA_STRING(pSelfObj, property)\
	if(pSelfObj != NULL && !pSelfObj->FindData(property))\
{ \
	pSelfObj->AddDataString(property, "");\
}
#define ADD_DATA_WIDESTR(pSelfObj, property)\
	if(pSelfObj != NULL && !pSelfObj->FindData(property))\
{ \
	pSelfObj->AddDataWideStr(property, L"");\
}
#define ADD_DATA_OBJECT(pSelfObj, property)\
	if(pSelfObj != NULL && !pSelfObj->FindData(property))\
{ \
	pSelfObj->AddDataObject(property, PERSISTID());\
}

//ɾ����ʱ����
#define REMOVE_DATA_TYPE(pSelfObj, property)\
if(pSelfObj != NULL && pSelfObj->FindData(property))\
{ \
	pSelfObj->RemoveData(property);\
}