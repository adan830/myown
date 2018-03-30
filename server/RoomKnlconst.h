//--------------------------------------------------------------------
// �ļ���:		KnlConst.h
// ��  ��:		����Ĳ�����صĶ���
// ˵  ��:		
// ��������:	2006��1��14��
// ������:		½����
// ��Ȩ����:	������ţ�������޹�˾
//--------------------------------------------------------------------

#ifndef _SERVER_ROOM_KNLCONST_H
#define _SERVER_ROOM_KNLCONST_H

#include "KnlConst.h"
#include "../public/PersistId.h"

class IKernel;
class IRoomCallee;
class IVarList;
class IVar;

// LUA�ű���չ����
typedef int (__cdecl* ROOM_LUA_EXT_FUNC)(void* state);
// ��������
typedef int (__cdecl* ROOM_HEARTBEAT_FUNC)(IKernel* pKernel, const PERSISTID& self,
	int time);
// ���Իص�����
typedef int (__cdecl* ROOM_CRITICAL_FUNC)(IKernel* pKernel, const PERSISTID& self,
	const char* property, const IVar& old);
// ����Ӻ���
typedef int (__cdecl* ROOM_RECHOOK_FUNC)(IKernel* pKernel, const PERSISTID& self, 
	const char* record, int op_type, int row, int col);
// ����ű���չ����
typedef int (__cdecl* ROOM_TASK_FUNC)(void* context);
// �߼��ഴ���ص�����
typedef int (__cdecl* ROOM_LOGIC_CLASS_FUNC)(IKernel* pKernel, int index);
// �߼��¼��ص�����
typedef int (__cdecl* ROOM_LOGIC_EVENT_FUNC)(IKernel* pKernel, 
	const PERSISTID& self, const PERSISTID& sender, const IVarList& msg);

// ����LUA�ű���չ����
#define DECL_LUA_EXT(p) pKernel->DeclareLuaExt(#p, p)
// ������������
#define DECL_HEARTBEAT(p) pKernel->DeclareHeartBeat(#p, p)
// �������Իص�����
#define DECL_CRITICAL(p) pKernel->DeclareCritical(#p, p)
// ����������ص�����
#define DECL_RECHOOK(p) pKernel->DeclareRecHook(#p, p)

// ��������ű���չ����
//#define DECL_TASKFUNC_0(res, p) pKernel->DeclareTaskFunc( \
//	#p, p, res, CVarList())
//#define DECL_TASKFUNC_1(res, p, a0) pKernel->DeclareTaskFunc( \
//	#p, p, res, CVarList() << a0)
//#define DECL_TASKFUNC_2(res, p, a0, a1) pKernel->DeclareTaskFunc( \
//	#p, p, res, CVarList() << a0 << a1)
//#define DECL_TASKFUNC_3(res, p, a0, a1, a2) pKernel->DeclareTaskFunc( \
//	#p, p, res, CVarList() << a0 << a1 << a2)
//#define DECL_TASKFUNC_4(res, p, a0, a1, a2, a3) pKernel->DeclareTaskFunc( \
//	#p, p, res, CVarList() << a0 << a1 << a2 << a3)
//#define DECL_TASKFUNC_5(res, p, a0, a1, a2, a3, a4) pKernel->DeclareTaskFunc( \
//	#p, p, res, CVarList() << a0 << a1 << a2 << a3 << a4)
//#define DECL_TASKFUNC_6(res, p, a0, a1, a2, a3, a4, a5) pKernel->DeclareTaskFunc( \
//	#p, p, res, CVarList() << a0 << a1 << a2 << a3 << a4 << a5)
//#define DECL_TASKFUNC_7(res, p, a0, a1, a2, a3, a4, a5, a6) pKernel->DeclareTaskFunc( \
//	#p, p, res, CVarList() << a0 << a1 << a2 << a3 << a4 << a5 << a6)
//#define DECL_TASKFUNC_8(res, p, a0, a1, a2, a3, a4, a5, a6, a7) pKernel->DeclareTaskFunc( \
//	#p, p, res, CVarList() << a0 << a1 << a2 << a3 << a4 << a5 << a6 << a7)
//#define DECL_TASKFUNC_9(res, p, a0, a1, a2, a3, a4, a5, a6, a7, a8) pKernel->DeclareTaskFunc( \
//	#p, p, res, CVarList() << a0 << a1 << a2 << a3 << a4 << a5 << a6 << a7 << a8)

//// ��Ϸ��������
//enum OBJECT_TYPES_ENUM
//{
//	TYPE_SCENE = 1,		// ����
//	TYPE_PLAYER = 2,	// ���
//	TYPE_NPC = 4,		// NPC
//	TYPE_ITEM = 8,		// ��Ʒ
//	TYPE_HELPER = 16,	// ��������
//	TYPE_WEAKBOX = 32,	// ����������
//};
//
//// ��Ϸ�������ײ���η���
//enum SHAPE_TYPES_ENUM
//{
//	SHAPE_CYLINDER = 0,	// Բ����
//	SHAPE_SPHERE = 1,	// ����
//	SHAPE_POLYGON = 2,	// ���������
//};
//
//// OnStore�ص������ͣ�type��������ȡֵ
//enum STORE_TYPES_ENUM
//{
//	STORE_EXIT,		// ����뿪��Ϸ
//	STORE_TIMING,	// ��ʱ����
//	STORE_SWITCH,	// �л�����ǰ����
//	STORE_MANUAL,	// �˹����棨����SavePlayerData��
//	STORE_EDIT,		// �޸Ĳ����߽�ɫ���ݺ�ı���
//	STORE_RECREATE,	// �ָ���ɫ���ݺ�ı���
//};
//
//// ������ƶ�ģʽ
//enum MOTION_MODE_ENUM
//{
//	MMODE_STOP,		// ֹͣ
//	MMODE_MOTION,	// �ر��ƶ�
//	MMODE_JUMP,		// ��Ծ
//	MMODE_JUMPTO,	// �ı���Ծ��Ŀ�귽��
//	MMODE_FLY,		// �����ƶ�
//	MMODE_SWIM,		// ˮ���ƶ�
//	MMODE_DRIFT,	// ˮ���ƶ�
//	MMODE_CLIMB,	// ��ǽ
//	MMODE_SLIDE,	// �ڲ������߷�Χ�ڻ���
//	MMODE_SINK,		// �³�
//	MMODE_LOCATE,	// ǿ�ƶ�λ
//};
//
//// OnMotion�ص����ƶ������result)������ȡֵ��δ��ע�����ڲ�ʹ��ֵ
//enum MOTION_RESULT_ENUM
//{
//	MRESULT_STATIC,
//	MRESULT_CONTINUE,
//	MRESULT_SUCCEED,		// �ɹ�����
//	MRESULT_COLLIDE_SCENE,	// ��������
//	MRESULT_COLLIDE_OBJECT,	// ��������
//};
//
//// �����������ͣ����ڱ���RecHook��
//enum RECORD_OPERATE_TYPES_ENUM
//{
//	RECOP_INIT,			// AddRecHookʱ�����Ĺ���
//	RECOP_ADD_ROW,		// �����
//	RECOP_REMOVE_ROW,	// ɾ����
//	RECOP_CLEAR_ROW,	// ���
//	RECOP_GRID_CHANGE,	// ��Ԫ���ݸı�
//	RECOP_SET_ROW,		// ĳһ�е����ݸı�
//};
//
//// �����ɫ�浵����
//enum CROSS_SAVE_TYPES_ENUM
//{
//	CROSS_SAVE_IN  = 200, // �ش���ɫ�浵��ս�����Ĳ������ͣ�
//	CROSS_SAVE_OUT = 300, // ������ɫ�浵����ͨ���Ĳ������ͣ�
//};
//
//// �����ɫ״̬
//enum CROSS_STATUS_ENUM
//{
//	CS_INVALID = 0,	  // ��Ч
//	CS_WAITING = 100, // �ȴ�ת��
//	CS_NORMAL  = 101, // ��ת��(ת�������������״̬)
//	CS_BACKING = 102, // ���ڻش���
//	CS_GOING   = 103, // ����ת��
//	CS_CROSSED = 104, // ��ת��
//	// normal server: CS_GOING -> CS_CROSSED -> CS_INVALID
//	// fight server : CS_WAITING -> CS_NORMAL -> CS_BACKING -> CS_INVALID
//};
//
//enum LIMIT_TIME_TYPE_ENUM
//{
//	JIANGHU_LIMIT_TIME = 2,	//��������
//	ZHIZUN_LIMIT_TIME  =3,	//��������
//};
//
//enum RESEND_TO_PLAYER_ENUM
//{
//	RTP_NULL,
//	RTP_SEND_LETTER,
//	RTP_QUERY_LETTER,
//	RTP_RECV_LETTER,
//	RTP_RECV_LETTER_FAIL,
//	RTP_LOOK_LETTER,
//	RTP_CLEAN_LETTER,
//	RTP_REJECT_LETTER,
//};

#endif // _SERVER_KNLCONST_H


