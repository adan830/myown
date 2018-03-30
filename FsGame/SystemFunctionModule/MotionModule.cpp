//--------------------------------------------------------------------
// �ļ���:      MotionModule.cpp
// ��  ��:      �ƶ�
// ��������:    2014-10-15
// ������:      liumf
// �޸�����:	2015-5-6
// �޸���:		 
// �޸�����:	�ع�
//    :        
//--------------------------------------------------------------------
#include "MotionModule.h"
#include "utils/util_func.h"
#include "FsGame/Define/GameDefine.h"
#include "FsGame/Define/CommandDefine.h"
#include "utils/geometry_func.h"
#include "utils/string_util.h"
#include <ctime>
#include "atltypes.h"
#include "server/LoopCheck.h"
#include "FsGame/CommonModule/LuaExtModule.h"
#include "FsGame/CommonModule/SwitchManagerModule.h"
#include "FsGame/SystemFunctionModule/StaticDataQueryModule.h"
#include "FsGame/Define/StaticDataDefine.h"
#include "utils/custom_func.h"
#include "FsGame/Define/ServerCustomDefine.h"
#include "FsGame/Define/ClientCustomDefine.h"
//#include "FsGame/Define/PlayerSysInfoDefine.h"
#include "FsGame/SystemFunctionModule/PlayerBaseModule.h"
#include "FsGame/CommonModule/PathFindingModule.h"
#include "FsGame/CommonModule/LogModule.h"
//#include "FsGame/Define/OffLineDefine.h"
#include "../CommonModule/EnvirValueModule.h"

#include "utils/cache/CacheHelper.hpp"

#define MAX_SKILL_SPEED 40.0f

// ���ļ����õ��Ķ��壬�����ļ�������ʹ��
// ���Բ�Ҫ����ͷ�ļ���
enum MotionState
{
	MOTION_STATE_STOP		= 0,	// ֹͣ�ƶ�
	MOTION_STATE_WAIT		= 1,	// �ȴ��ƶ�
	MOTION_STATE_MOVING		= 2,	// �����ƶ�	
};

// �ȴ��ϰ����ƿ�ʱ��
const int WAIT_MOTION_TIME = 500;

// �ƿ��ϰ������߹��ĸ��ӱ�
const char* const ROUND_PATH_LIST_RECORD = "RoundPathListRec";
enum
{
	ROUND_PATH_LIST_RECORD_COL_grid = 0,	// ��������	
	ROUND_PATH_LIST_RECORD_COL_count = 1,	// ���õĴ���
};
// ��¼���߹��ĸ�������
const int ROUND_PATH_COUNT = 5;
// �����ƿ��ϰ����������
const int ROUND_MAX_COUNT = 5;
// �ƿ��ϰ����ܴ���,�ﵽ�˴������ж��޷�����
const int ROUND_TOTAL_COUNT_MAX = 50;

// ��̬Ѱ··����
const char* const FIND_ROUND_PATH_RECORD = "FindRoundPathRec";
enum
{
	FIND_ROUND_PATH_RECORD_COL_grid = 0, // ��������
	FIND_ROUND_PATH_RECORD_COL_count = 1, // ��ȡ����
};
const int FIND_ROUND_PATH_MAX_COUNT = 10; // Ѱ·���е�Ѱ·����󱻻�ȡ�Ĵ���

// ��ǰѰ·���ﲻ�˵ĵ�
const char* const CANT_ARRIVE_GRID_LIST_RECORD = "CantArriveGridListRec";
enum
{
	CANT_ARRIVE_GRID_LIST_RECORD_COL_grid = 0, // ��������
};

// �ƶ�У���������ʱ��
const int CHECK_DIST_TOTAL_TIME = 60000;

// ���ͻ����쳣����
double MotionModule::m_distTotalWarning = 0.0f;		// �ƶ�У������ֵ
double MotionModule::m_distTotalMax = 0.0f;			// �ƶ�У�����ֵ
int MotionModule::m_heartBeatWarning = 0;			// ��������ֵ
int MotionModule::m_heartBeatMax = 0;				// �������ֵ
int MotionModule::m_cantLoginTime = 0;				// �������¼ʱ��
bool MotionModule::m_bCheckPlayerState = true;		// �Ƿ������״̬

MotionModule::MapCallBack MotionModule::m_mapCallBack;
MotionModule * MotionModule::m_pMotionModule = NULL;

// ����������Ĳ���Ϣ
int nx_floor_info(void* state)
{
	IKernel* pKernel = ILuaExtModule::GetKernel(state);

	// ����������
	CHECK_ARG_NUM(state, nx_floor_info, 1);

	// ����������
	CHECK_ARG_OBJECT(state, nx_floor_info, 1);

	// ��ȡ����
	PERSISTID self = pKernel->LuaToObject(state, 1);

	if (!pKernel->Exists(self))
	{
		return 0;
	}

	len_t x = pKernel->GetPosiX(self);
	len_t z = pKernel->GetPosiZ(self);
	len_t y = pKernel->GetPosiY(self);

	/// \brief��ø�������
	int nRows = (int)pKernel->GetPathGridRows();
	/// \brief��ø�������
	int nCols = (int)pKernel->GetPathGridCols();

	::CustomSysInfo(pKernel, self, 0,
		CVarList() << (std::string("position: ") + util_double_as_string(x) + " " + util_double_as_string(y).c_str() + " " + util_double_as_string(z).c_str()));

	::CustomSysInfo(pKernel, self, 0,
		CVarList() << "FloorIndex|CanMove|CanStand|FloorHeight|FloorSpace");

	::CustomSysInfo(pKernel, self, 0,
		CVarList() << (std::string("total rows: ") + util_int_as_string(nRows).c_str() + std::string("total cols: ") + util_int_as_string(nCols).c_str()));

	// ������
	int count = pKernel->GetFloorCount(x, z);

	LoopBeginCheck(a);
	for (int i = count - 1; i >= 0; --i)
	{
		LoopDoCheck(a);
		if (pKernel->GetFloorExists(x, z, i))
		{
			len_t height = pKernel->GetFloorHeight(x, z, i);
			len_t space = pKernel->GetFloorSpace(x, z, i);
			bool can_move = pKernel->GetFloorCanMove(x, z, i);
			bool can_stand = pKernel->GetFloorCanStand(x, z, i);

			std::string result;

			result = StringUtil::Int64AsString(i);
			result += "|";
			result += can_move ? "true" : "false";
			result += "|";
			result += can_stand ? "true" : "false";
			result += "|";
			result += util_double_as_string(height);
			result += "|";
			result += util_double_as_string(space);

			// �������Ϣ
			::CustomSysInfo(pKernel, self, 0, CVarList() << result.c_str());
		}
	}

	return 0;
}

// �ƶ���Ŀ���
inline int nx_motion_point(void *state)
{
	IKernel *pKernel = LuaExtModule::GetKernel(state);
	if (pKernel == NULL)
	{
		return 1;
	}

	// ����������
	CHECK_ARG_NUM(state, nx_motion_point, 3);

	// ���ÿ����������
	CHECK_ARG_OBJECT(state, nx_motion_point, 1);
	CHECK_ARG_FLOAT(state, nx_motion_point, 2);
	CHECK_ARG_FLOAT(state, nx_motion_point, 3);
	PERSISTID player = pKernel->LuaToObject(state, 1);
	float x = pKernel->LuaToFloat(state, 2);
	float z = pKernel->LuaToFloat(state, 3);

	// ��Ҷ���
	IGameObj* pPlayerObj = pKernel->GetGameObj(player);
	if (pPlayerObj == NULL)
	{
		return 1;
	}

	pPlayerObj->SetInt("MotionState", MOTION_STATE_STOP);
	MotionModule::m_pMotionModule->MotionToPoint(pKernel, player, x, z);

	return 1;
}

// ���ü������
// ���� 1 int �����������ֵ -1��ʾ�رռ��
// ���� 2 int ����������ֵ ����������
// ���� 3 double ����ƶ�У��ֵ����ֵ
// ���� 4 double ����ƶ�У��ֵ���ֵ ����������
inline int nx_check_player_state(IKernel* pKernel, const PERSISTID& self, const IVarList& args)
{
	// ��������ֵ
	int heartWarning = args.IntVal(0);
	if (heartWarning < 0) 
	{
		MotionModule::m_bCheckPlayerState = false;
		return 0;
	}
	
	MotionModule::m_bCheckPlayerState = true;
	MotionModule::m_heartBeatWarning = heartWarning;

	// �������ֵ
	int heartMax = args.IntVal(1);
	if (heartMax > 0)
	{
		MotionModule::m_heartBeatMax = heartMax;
	}

	// �ƶ�У��ֵ����ֵ
	double distWarning = args.DoubleVal(2);
	if (distWarning > 0)
	{
		MotionModule::m_distTotalWarning = distWarning;
	}

	// �ƶ�У��ֵ���ֵ
	double distMax = args.DoubleVal(3);
	if (distMax > 0)
	{
		MotionModule::m_distTotalMax = distMax;
	}
	
	return 0;
}

// ��ʼ��
bool MotionModule::Init(IKernel* pKernel)
{
	m_pMotionModule = this;
	
	Assert(m_pMotionModule != NULL);

	pKernel->AddEventCallback("player", "OnRequestMove", OnPlayerRequestMove);
	pKernel->AddEventCallback("player", "OnMotion", OnMotion);
	pKernel->AddEventCallback("NormalNpc", "OnMotion", OnMotion);
	pKernel->AddEventCallback("player", "OnRecover", OnRecover, -1);
	pKernel->AddEventCallback("player", "OnContinue", OnRecover, -1);
	pKernel->AddEventCallback("NormalNpc","OnEntry", OnEntry, 999);

	// �ͻ�����Ϣ
	//pKernel->AddIntCustomHook("player", CLIENT_CUSTOMMSG_HEART_BEAT, OnCustomHeartBeat);

	DECL_CRITICAL(MotionModule::C_OnSpeedChanged);
	DECL_CRITICAL(MotionModule::C_OnCantMoveChanged);

	DECL_HEARTBEAT(MotionModule::HB_WaitMotion); 
	DECL_HEARTBEAT(MotionModule::HB_MotionEnd); 
// 	DECL_HEARTBEAT(MotionModule::HB_UpdatePlayerState);
// 	DECL_HEARTBEAT(MotionModule::HB_CheckMotionBreak);

	///////////////////////////////////////////////////////////////////////
// #ifndef FSROOMLOGIC_EXPORTS
// 	DECL_HEARTBEAT(MotionModule::HB_TryMotion);
// #endif // FSROOMLOGIC_EXPORTS
	//////////////////////////////////////////////////////////////////////

	// ��ȡ���λ��У������ֵ�����ֵ
	m_distTotalWarning = static_cast<double>(EnvirValueModule::EnvirQueryInt(ENV_VALUE_CHECK_DIST_TOTAL_WARNING));
	m_distTotalMax = static_cast<double>(EnvirValueModule::EnvirQueryInt(ENV_VALUE_CHECK_DIST_TOTAL_MAX));
	m_heartBeatWarning = EnvirValueModule::EnvirQueryInt(ENV_VALUE_HEART_BEAT_WARNING);
	m_heartBeatMax = EnvirValueModule::EnvirQueryInt(ENV_VALUE_HEART_BEAT_MAX);
	//m_cantLoginTime = EnvirValueModule::EnvirQueryInt(ENV_VALUE_CANT_LOGIN_TIME_OVER_DIST_TOTAL);
	m_bCheckPlayerState = true;

	DECL_LUA_EXT(nx_motion_point);
	DECL_LUA_EXT(nx_floor_info);
#ifndef FSROOMLOGIC_EXPORTS
	DECL_DISPATCH_GM_FUNCTION(nx_check_player_state);
#endif // FSROOMLOGIC_EXPORTS

	return true;
}

// �ͷ�
bool MotionModule::Shut(IKernel* pKernel)
{
	return true;
}

// ���볡��
int MotionModule::OnEntry(IKernel* pKernel, const PERSISTID& self, 
						  const PERSISTID& sender, const IVarList& args)
{
	if (!pKernel->Exists(self))
	{
		return 0;
	}

	// ˢ���ٶ�
	m_pMotionModule->UpdateMoveSpeed(pKernel, self);

	// ������Իص�
	ADD_CRITICAL(pKernel, self, "CantMove", "MotionModule::C_OnCantMoveChanged");
	ADD_CRITICAL(pKernel, self, "RunSpeedAdd", "MotionModule::C_OnSpeedChanged");
	ADD_CRITICAL(pKernel, self, "RunSpeed", "MotionModule::C_OnSpeedChanged");
	return 0;
}

// �������
int MotionModule::OnRecover(IKernel* pKernel, const PERSISTID& self, 
							const PERSISTID& sender, const IVarList& args)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return 0;
	}

	float fCheckSyncRange	= EnvirValueModule::EnvirQueryFloat(ENV_VALUE_CHECK_SYNC_RANGE);
	float fRunSpeed			= EnvirValueModule::EnvirQueryFloat(ENV_VALUE_DEFAULT_RUN_SPEED);

	//����ͬ������ƫ��
	pSelfObj->SetFloat("CheckSyncRange", fCheckSyncRange);

	// ���������ٶ�
	if (pSelfObj->FindAttr("RunSpeed"))
	{
		const float speed = pSelfObj->QueryFloat("RunSpeed");
		if (::util_float_equal_zero(speed))
		{
			pSelfObj->SetFloat("RunSpeed", fRunSpeed);
		}
	}

	// ����ת���ٶ�
	if (pSelfObj->FindAttr("RollSpeed"))
	{
		const float speed = pSelfObj->QueryFloat("RollSpeed");
		if (::util_float_equal_zero(speed))
		{
			pSelfObj->SetFloat("RollSpeed", PI2);
		}
	}

	// ˢ���ٶ�
	m_pMotionModule->UpdateMoveSpeed(pKernel, self);  

	// ������Իص�
	ADD_CRITICAL(pKernel, self, "CantMove", "MotionModule::C_OnCantMoveChanged");
	ADD_CRITICAL(pKernel, self, "RunSpeedAdd", "MotionModule::C_OnSpeedChanged");
	ADD_CRITICAL(pKernel, self, "RunSpeed", "MotionModule::C_OnSpeedChanged");

	// ȥ���ƶ���Ҽ�� [3/28/2016 liumf]
// 	if (pSelfObj->QueryInt("Online") == PLAYER_ONLINE)
// 	{
// 		// ��Ӽ���ƶ�У������
// 		ADD_HEART_BEAT(pKernel, self, "MotionModule::HB_UpdatePlayerState", CHECK_DIST_TOTAL_TIME);
// 	}

	return 0;
}

//�����ƶ�
int MotionModule::OnPlayerRequestMove(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return 0;
	}

	// �ƶ�ģʽ
	int mode = args.IntVal(0);
	if (pSelfObj->QueryInt("Dead") > 0)
	{
		return 0;
	}

	int nResult = 0;
	switch (mode)
	{
	case MMODE_STOP:
		{
			nResult = OnRequestMoveStop(pKernel, self, args);
		}
		break;
	case MMODE_MOTION:
	case MMODE_FLY:
	case MMODE_JUMPTO:
		{
			nResult = OnRequestMoveMotion(pKernel, self, mode, args);
		}
		break;
	}

	return nResult;
}

// ����ֹͣ
int MotionModule::OnRequestMoveStop(IKernel* pKernel, const PERSISTID& self, const IVarList& args)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return 0;
	}

	len_t x = args.FloatVal(2);
	len_t y = args.FloatVal(3);
	len_t z = args.FloatVal(4);
	len_t orient = args.FloatVal(5);
	//len_t y = pKernel->GetMapHeight(x, z);
	len_t server_move_speed = pSelfObj->QueryFloat("MoveSpeed");
	bool bCheck = false;
	if (pKernel->Type(self) == TYPE_PLAYER)
	{
		//pKernel->MoveTo(self, x, y, z, orient);
		//pKernel->Stop(self);
		// �߶�ֵ��С�ڵر��ĸ߶�
//  		float fGroundY = GetWalkHeight(pKernel, x, z, 0);
//  		y = __max(y, fGroundY);

		bCheck = pKernel->CheckStop(self, server_move_speed, x, y, z, orient);
#ifndef FSROOMLOGIC_EXPORTS
//#ifdef _DEBUG
		if (!bCheck)
		{
			// ��ʱ���� ���ͣ�ڵ������� ǿ��������
			float fPosX = pSelfObj->GetPosiX();
			float fPosY = pSelfObj->GetPosiY();
			float fPosZ = pSelfObj->GetPosiZ();
			float fMapHeight = pKernel->GetMapHeight(fPosX, fPosZ);
			if (fPosY < fMapHeight)
			{
				pKernel->MoveTo(self, fPosX, fMapHeight, fPosZ, pSelfObj->GetOrient());
			}
			/*if (pKernel->GetServerId() == 7440005)
			{
			char strInfo[128] = { 0 };
			SPRINTF_S(strInfo, "ֹͣ�����%d ���� : x=%f, y=%f, z=%f clienty=%f", bCheck ? 1 : 0, pSelfObj->GetPosiX(), pSelfObj->GetPosiY(), pSelfObj->GetPosiZ(), y);
			::CustomSysInfo(pKernel, self, TIPSTYPE_GMINFO_MESSAGE, "0", CVarList() << StringUtil::StringAsWideStr(strInfo));
			}*/
		}
//#endif
#endif
	}

	return 0;
}

// �����ƶ�
int MotionModule::OnRequestMoveMotion(IKernel* pKernel, const PERSISTID& self, int nMMode, const IVarList& args)
{
	if (nMMode < MMODE_MOTION || nMMode > MMODE_LOCATE)
	{
		return 0;
	}
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return 0;
	}

	// ��Ҳ����ƶ�
	if (pSelfObj->QueryInt(FIELD_PROP_CANT_MOVE) > 0)
	{
		return 0;
	}

	len_t sx = args.FloatVal(2);
	len_t sz = args.FloatVal(3);
	len_t dx = args.FloatVal(4);
	len_t dz = args.FloatVal(5);
	len_t syclient = args.FloatVal(6);

	// �ƶ���Ϣ���һλ���һ�����ַ����Ĳ���
	len_t move_speed = pSelfObj->QueryFloat("MoveSpeed");
// 	int nCount = (int)args.GetCount();
// 	// �ͷ�ĳЩ����ʱ�ƶ��ٶȻ�˲�����
// 	if (nCount > 8)
// 	{
// 		move_speed = args.FloatVal(7);
// 
// 		move_speed = __min(MAX_SKILL_SPEED, move_speed);
// 	}

	float sy = pSelfObj->GetPosiY();
	len_t rotate_speed = pSelfObj->QueryFloat("RollSpeed");
	if (pKernel->Type(self) == TYPE_PLAYER)
	{
		bool bCheck = false;
		if (MMODE_MOTION == nMMode)
		{
			bCheck = pKernel->CheckMotion(self, move_speed, rotate_speed, sx, sy, sz, dx, dz, 0);
		}
		else if (MMODE_FLY == nMMode)
		{
			pKernel->CheckFly(self, move_speed, rotate_speed, sx, sy, sz, dx, syclient, dz);
		}
		else if (MMODE_JUMPTO ==  nMMode)
		{
			pKernel->CheckJumpTo(self, move_speed, rotate_speed, sx, sy, sz, dx, dz);
		}
		
		if (!bCheck)
		{
			char strInfo[128] = {0};
			SPRINTF_S(strInfo, "OnRequestMoveMotion mode = %d move_speed = %f", nMMode, move_speed);
			::extend_warning(LOG_INFO, strInfo);
		}
		pKernel->Command(self, self, CVarList() << COMMAND_PLAYER_MOTION << sx << syclient << sz << dx << dz);
	}

	return 0;
}

// �ƶ��ص�
int MotionModule::OnMotion(IKernel* pKernel, const PERSISTID& self,
						   const PERSISTID& sender, const IVarList& args)
{
	if (m_pMotionModule == NULL)
	{
		return 0;
	}

	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return 0;
	}
	int target_result = args.IntVal(1);
	int target_type   = pSelfObj->GetClassType();
	int target_state  = pSelfObj->QueryInt("MotionState");

	int target_grid_pos = pKernel->GetGridIndexByPos(pKernel->GetPosiX(self), pKernel->GetPosiZ(self));
	int target_grid_dst = pKernel->GetGridIndexByPos(pSelfObj->QueryFloat("TargetX"), pSelfObj->QueryFloat("TargetZ"));

	
	if (target_type == TYPE_PLAYER && target_result != MRESULT_SUCCEED)
	{
#ifndef FSROOMLOGIC_EXPORTS
//#ifdef _DEBUG
		/*if (pKernel->GetServerId() == 7440005)
		{
		char strInfo[128] = { 0 };
		SPRINTF_S(strInfo, "��ǰ�������˶������%d x=%f, y=%f, z=%f", target_result, pSelfObj->GetPosiX(), pSelfObj->GetPosiY(), pSelfObj->GetPosiZ());
		::CustomSysInfo(pKernel, self, TIPSTYPE_GMINFO_MESSAGE, "0", CVarList() << StringUtil::StringAsWideStr(strInfo));
		}*/
//#endif
#endif
	}

	// û�н���Ѱ·
	if (target_state == MOTION_STATE_STOP)
	{
		return 0;
	}
	
	switch (target_result)
	{
	case MRESULT_SUCCEED:           // �ɹ�����
		m_pMotionModule->MotionSucceed(pKernel, self);
		break;
	case MRESULT_COLLIDE_SCENE:     // ��������
	case MRESULT_COLLIDE_OBJECT:    // ��������
	case MRESULT_COLLIDE_PATH_WALK:
	case MRESULT_COLLIDE_PATH_LOGIC:
	case MRESULT_COLLIDE_PATH_OBJECT:
		{
			MotionResult result = m_pMotionModule->DealCollision(pKernel, self, sender);
			if (result != MOTION_RESULT_SUCCEED)
			{
				// ����ϰ���ʧ�� ֱ��Ѱ·����
				m_pMotionModule->MotionEnd(pKernel, self, result);
			}
		}
		
		break;
	}

	return 0;
}

// ע���ƶ��ص�����
bool MotionModule::RegisterMotionCallBackFunction(const char* strFunc, MOTION_CALL_BACK_FUNC pFunc)
{
	MapCallBack::iterator iter = m_mapCallBack.find(strFunc);
	if (iter != m_mapCallBack.end()) 
	{
		// �Ѿ���ע����
		Assert(0);
		return false;
	}

	m_mapCallBack.insert(std::make_pair(strFunc, pFunc));

	return true;
}

//��Ŀ������ƶ�
bool MotionModule::MotionToObject(IKernel* pKernel, const PERSISTID& self, const PERSISTID& target,
								  float minDistance/* = 0.0f*/, float maxDistance/* = 0.0f*/, 
								  const char* strCallBackFunc/* = ""*/,
								  MotionMethod method/* = MOTION_METHOD_ALL*/)
{
	// ���Ŀ���Ƿ����
	if (!pKernel->Exists(self) || !pKernel->Exists(target))   
	{
		return false;
	}

	float x = pKernel->GetPosiX(target);
	float z = pKernel->GetPosiZ(target);

	return MotionToPoint(pKernel, self, x, z, minDistance, maxDistance, strCallBackFunc, method);
}

//��Ŀ����ƶ�
bool MotionModule::MotionToPoint(IKernel* pKernel, const PERSISTID& self, float targetX, float targetZ,
								 float minDistance/* = 0.0f*/, float maxDistance/* = 0.0f*/, 
								 const char* strCallBackFunc/* = ""*/,
								 MotionMethod method/* = MOTION_METHOD_ALL*/)
{
	// ���Ŀ���Ƿ���� 
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return false;
	}

	// �ж��Ƿ�����Ѱ·
	int motioning = pSelfObj->QueryInt("MotionState");
	if (motioning != MOTION_STATE_STOP)
	{
		// ���Ŀ���Ƿ�һ��
		float oldTargetX = pSelfObj->QueryFloat("TargetX");
		float oldTargetZ = pSelfObj->QueryFloat("TargetZ");
		if (util_float_equal(targetX, oldTargetX) 
			&& util_float_equal(targetZ, oldTargetZ))
		{
			// ����Ѱ·��Ŀ��һ��, ��ֱ�ӷ���Ѱ·�ɹ�����������Ѱ·���߼�
			return true;
		}
	}

	// ����״̬
	ResetMotionState(pKernel, self);

	// ����Ŀ���
	pSelfObj->SetFloat("TargetX", targetX);
	pSelfObj->SetFloat("TargetZ", targetZ);

	// ��С���
	if (minDistance < 0.0f)
	{
		minDistance = 0.0f;
	}
	if (maxDistance < 0.0f)
	{
		maxDistance = 0.0f;
	}
	if (maxDistance < minDistance)
	{
		// ����ֵ ��֤�߼���ȷ
		float temp = maxDistance;
		maxDistance = minDistance;
		minDistance = temp;
	}
  
	// ��Ŀ����С����
	pSelfObj->SetFloat("PathDistanceMin", minDistance);

	// ��Ŀ��������
	pSelfObj->SetFloat("PathDistanceMax", maxDistance);
	
	// ���ûص�����
	pSelfObj->SetString("CallBackFunction", strCallBackFunc);

	// ����Ѱ·��ʽ
	pSelfObj->SetInt("MotionMethod", method);

	// �����赲�����ó���Ѱ·����������·��
	int nearestGrid = GetTargetNearestEnableGrid(pKernel, self);
	if (nearestGrid > 0)
	{
		float nearTargetX = 0.0f;
		float nearTargetZ = 0.0f;
		pKernel->GetPosByIndex(nearestGrid, nearTargetX, nearTargetZ);
		if (!WalkLineEnabled(pKernel, self, nearTargetX, nearTargetZ))
		{
			FindPathToGridStatic(pKernel, self, nearTargetX, nearTargetZ);
		}
	}

	// ��Ŀ���ƶ�
	ContinueMotion(pKernel, self);

	return true;
}

// ��Ŀ����ƶ�
// bForecastCollision ��ǰԤ���Ƿ�ᷢ����ײ
bool MotionModule::ContinueMotion(IKernel* pKernel, const PERSISTID& self, bool bForecastCollision/*= true*/)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		Assert(0);
		return false;
	}

	// ���Ŀ������Ч������
	float selfX = pKernel->GetPosiX(self);
	float selfZ = pKernel->GetPosiZ(self);
	float targetX = pSelfObj->QueryFloat("TargetX");
	float targetZ = pSelfObj->QueryFloat("TargetZ");
	float minDistance = pSelfObj->QueryFloat("PathDistanceMin");
	float maxDistance = pSelfObj->QueryFloat("PathDistanceMax");
	float distance = util_dot_distance(selfX, selfZ, targetX, targetZ);

	if (distance >= minDistance && distance <= maxDistance)
	{
		// ����Ч�����ڣ��ɹ�����Ŀ��㣬����Ѱ·
		MotionEnd(pKernel, self, MOTION_RESULT_SUCCEED);
		return true;
	}
	else
	{
		// ����С��һ�����ӵľ���
		float len = pKernel->GetPathGridSize();
		if (distance <= len)
		{
			// ����Ч�����ڣ��ɹ�����Ŀ��㣬����Ѱ·
			MotionEnd(pKernel, self, MOTION_RESULT_SUCCEED);
			return true;
		}
	}

	// ��ȡ��Ҫ����ĵ�
	int targetGrid = GetNeedArriveGrid(pKernel, self);
	if (targetGrid <= 0)
	{
		// Ŀ�긽���Ҳ�����ŵ���ֱ��ȥĿ���
		// Ŀ������ڸ���
		targetGrid = pKernel->GetGridIndexByPos(targetX, targetZ);
	}

	// ��ǰ�Ѿ�����Ŀ������ĵ�,Ѱ·�ɹ�
	int curGrid = pKernel->GetGridIndexByPos(selfX, selfZ);
	if (curGrid == targetGrid)
	{
		// �ɹ�����Ŀ��㣬����Ѱ·
		MotionEnd(pKernel, self, MOTION_RESULT_SUCCEED);
		return true;
	}

	// ����Ŀ���һ�����Ӳ������ߣ�����������
	PERSISTID collision;
	if (bForecastCollision && NeedDealCollision(pKernel, self, targetGrid, collision))      
	{
		// �����ƹ��ϰ���
		if (DealCollision(pKernel, self, collision) == MOTION_RESULT_SUCCEED)
		{
			// �ɹ�������ԭ����Ŀ��㣬�ȱ���
			//::extend_warning(LOG_ERROR, "Go Ahead RoundBarrier %d", self.nSerial);
			return true;
		}
		else
		{
			// �����ƹ��ϰ���ʧ�� �򲻽��б��ã�����ֱ�ߵ���Ŀ���
			// �߼�������
		}
	}

	// ֱ�ߵ���Ŀ�����
	MotionResult result = MotionLineToGrid(pKernel, self, targetGrid);
	if (result != MOTION_RESULT_SUCCEED)
	{
		MotionEnd(pKernel, self, result);
		return false;
	}

	return true;
}

// ֱ�ߵ������
MotionResult MotionModule::MotionLineToGrid(IKernel* pKernel, const PERSISTID& self, int gridIndex)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return MOTION_RESULT_FAILED;
	}

	if (gridIndex <= 0)
	{
		return MOTION_RESULT_CANT_FIND_GRID;
	}

	float x = 0;
	float z = 0;
	pKernel->GetPosByIndex(gridIndex, x, z);

	// �ܷ��ƶ�
	if (!CanMove(pKernel, self))
	{
		return MOTION_RESULT_CANT_MOVE;
	}

	// �����ƶ�״̬
	pSelfObj->SetInt("MotionState", MOTION_STATE_MOVING);

	// ��ȡ�ٶ�
	float moveSpeed = pSelfObj->QueryFloat("MoveSpeed");
	if (moveSpeed <= 0.0f)
	{
		return MOTION_RESULT_FAILED;
	}
	if (!pKernel->Motion(self, moveSpeed, PI2, x, z))
	{
		//extend_warning(LOG_ERROR, "[%s][%d] Motion error", __FILE__, __LINE__);
		return MOTION_RESULT_FAILED;
	}

	return MOTION_RESULT_SUCCEED;
}

// �ƶ��ɹ�(���ܵ�����ʱĿ��㣬�����ѵ����յ㣬ͨ�������ƶ��������)
bool MotionModule::MotionSucceed(IKernel* pKernel, const PERSISTID& self)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return false;
	}

	// �ܿ��ϰ���ɹ�
	int gridIndex = pSelfObj->QueryInt("RoundGrid");
	if (gridIndex > 0)
	{	
		RoundBarrierSucceed(pKernel, self);
	}

	// ������Ŀ���ƶ�
	return ContinueMotion(pKernel, self);
}

// ���óɹ�
bool MotionModule::RoundBarrierSucceed(IKernel* pKernel, const PERSISTID& self)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return false;
	}

	int gridIndex = pSelfObj->QueryInt("RoundGrid");
	if (gridIndex <= 0)
	{
		return false;
	}

	// ��¼����ʱ���ڵ�
	IRecord* pRecord = pSelfObj->GetRecord(ROUND_PATH_LIST_RECORD);
	if (pRecord)
	{
		// ���ô���
		int roundCount = 0;

		// ɾ���Ѿ����ڵļ�¼
		int row = pRecord->FindInt(ROUND_PATH_LIST_RECORD_COL_grid, gridIndex);
		if (row >= 0)
		{
			roundCount = pRecord->QueryInt(row, ROUND_PATH_LIST_RECORD_COL_count);
			pRecord->RemoveRow(row);
		}

		// ���������������ɾ����һ������
		int rowCount = pRecord->GetRows();
		if (rowCount >= ROUND_PATH_COUNT)
		{
			// ɾ�����������
			pRecord->RemoveRow(0);
		}

		// ��������
		pRecord->AddRowValue(-1, CVarList() << gridIndex << ++roundCount);

		//extend_warning(LOG_WARNING, "Round Record Count %d %d %d", self.nSerial, gridIndex, roundCount);

		// ͬһ����ô������ֱ࣬���ж��޷�����
		if (roundCount > ROUND_MAX_COUNT)
		{
			pSelfObj->SetInt("RoundTotalCount", ROUND_TOTAL_COUNT_MAX);
		}
	}

	// ���óɹ������øõ㣨���ñ���״̬��
	pSelfObj->SetInt("RoundGrid", 0);

	return true;
}

// ��ײ����
MotionResult MotionModule::DealCollision(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender)
{	
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return MOTION_RESULT_SELF_INEXIST;
	}

	// ��ײ����
	int senderMotionState = MOTION_STATE_STOP;
	float senderSpeed = 0.0f;
	IGameObj* pSenderObj = pKernel->GetGameObj(sender);
	if (pSenderObj != NULL)
	{
		senderMotionState = pSenderObj->FindAttr("MotionState") ? pSenderObj->QueryInt("MotionState") : MOTION_STATE_STOP;
		senderSpeed = pSenderObj->FindAttr("MoveSpeed") ? pSenderObj->QueryFloat("MoveSpeed") : 0.0f;
	}
	else
	{
		return MOTION_RESULT_SELF_INEXIST;
	}

	// �Ƿ���Ҫ���ݵȴ�
	bool bWait = pSenderObj->FindAttr("MotionState")												// ��ײĿ����ڴ�����
		&& senderMotionState == MOTION_STATE_MOVING													// ��ײĿ�������ƶ�
		&& (pSelfObj->QueryInt64("WaitTarget") != sender.nData64									// �ϴ���ײ���Ǵ˶���
		|| (!util_float_equal(pSelfObj->QueryFloat("WaitTargetX"), pKernel->GetPosiX(sender))		// �����Ǵ˶���
		&& !util_float_equal(pSelfObj->QueryFloat("WaitTargetZ"), pKernel->GetPosiZ(sender))))		// ���Ƕ���������иı�
		&& pSelfObj->QueryFloat("MoveSpeed") <= senderSpeed;										// �ٶȲ����ڱ�ײĿ��

	// ��Ҫ�ȴ��Է����뿪
	if (bWait)
	{
		// ��¼״̬
		pSelfObj->SetInt("MotionState", MOTION_STATE_WAIT);
		// ֹͣ�ƶ�
		pKernel->Stop(self);
		// ��¼�ȴ��Ķ���
		pSelfObj->SetInt64("WaitTarget", sender.nData64);
		// ��¼�ȴ�����ǰ�����
		pSelfObj->SetFloat("WaitTargetX", pKernel->GetPosiX(sender));
		pSelfObj->SetFloat("WaitTargetZ", pKernel->GetPosiZ(sender));
		
		ADD_HEART_BEAT(pKernel, self, "MotionModule::HB_WaitMotion", WAIT_MOTION_TIME);

		return MOTION_RESULT_SUCCEED;
	}

	// �����ô���
	if (pSelfObj->QueryInt("RoundTotalCount") < ROUND_TOTAL_COUNT_MAX)
	{
		// �����ƹ��ϰ���
		return RoundBarrier(pKernel, self, sender);
	}

	// ���ô������ֱ࣬�ӵ��������Ѱ·���򲻵��ѣ�ֻ���ߴ��߼���
	// ����Ƿ��������
	int method = pSelfObj->QueryInt("MotionMethod");
	if ((MOTION_METHOD_DYNAMIC & method) == 0 
		&& (MOTION_METHOD_STATIC & method) == 0)
	{
		// ��������ã�����Ѱ·ʧ��
		return MOTION_RESULT_SELF_CLOSE_IN;
	}

	return DealCantArriveTarget(pKernel, self);
}

// �ȴ��ϰ����ƿ�
int MotionModule::HB_WaitMotion(IKernel* pKernel, const PERSISTID& self, int time)
{
	if (!pKernel->Exists(self))
	{
		Assert(0);
		return false;
	}

	DELETE_HEART_BEAT(pKernel, self, "MotionModule::HB_WaitMotion");

	m_pMotionModule->ContinueMotion(pKernel, self); 

	return 0;
}

// �ƹ��ϰ���
MotionResult MotionModule::RoundBarrier(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return MOTION_RESULT_FAILED;
	}

	// Ѱ�����������ߵĵ�
	int gridIndex = GetSelfEnableGrid(pKernel, self);
	if (gridIndex <= 0)
	{
		return MOTION_RESULT_SELF_CLOSE_IN;
	}

	// ��ǰ���ڵĸ���
	int curGridIndex = pKernel->GetGridIndexByPos(pKernel->GetPosiX(self), pKernel->GetPosiZ(self));

	if (curGridIndex <= 0)
	{
		extend_warning(LOG_ERROR, "[%s][%d] RoundBarrier error", __FILE__, __LINE__);
		return MOTION_RESULT_SELF_INEXIST;
	}
	
	// �ƶ�
	MotionResult ret = MotionLineToGrid(pKernel, self, gridIndex);
	if (ret != MOTION_RESULT_SUCCEED)
	{
		return ret;
	}

	// ��¼����ʱ���ڵ�
	pSelfObj->SetInt("RoundGrid", curGridIndex);
	// ��¼���ô���
	int tempCount =  pSelfObj->QueryInt("RoundTotalCount");
	pSelfObj->SetInt("RoundTotalCount", tempCount + 1);

	return MOTION_RESULT_SUCCEED;
}

// �޷�����Ŀ����Ĵ���
MotionResult MotionModule::DealCantArriveTarget(IKernel* pKernel, const PERSISTID& self)
{
	// ���������Χ ��ֱ�ӷ���Ѱ·ʧ��
	if (GetSelfEnableGrid(pKernel, self) <= 0)
	{
		//::extend_warning(LOG_ERROR, "npc can't move because of no path. config:%s SceneId:%d, x:%f z:%f", 
		//	pKernel->GetConfig(self), 
		//	pKernel->GetSceneId(), 
		//	pKernel->GetPosiX(self), 
		//	pKernel->GetPosiZ(self));  
		return MOTION_RESULT_CANT_ARRIVE;
	}

	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return MOTION_RESULT_SELF_INEXIST;
	}

	LoopBeginCheck(a);
	do
	{
		LoopDoCheck(a);

		// ���Ѱ·��
		IRecord* pFindRoundPathRecord = pSelfObj->GetRecord(FIND_ROUND_PATH_RECORD);
		if (pFindRoundPathRecord != NULL)
		{
			pFindRoundPathRecord->ClearRow();
		}

		// ��ձ��ñ�
		IRecord* pRoundPathListRecord = pSelfObj->GetRecord(ROUND_PATH_LIST_RECORD);
		if (pRoundPathListRecord != NULL)
		{
			pRoundPathListRecord->ClearRow();
		}
		
		// ��ձ��ô���
		pSelfObj->SetInt("RoundTotalCount", 0);

		// Ŀ�긽����
		int nearestGrid = GetTargetNearestEnableGrid(pKernel, self);
		if (nearestGrid <= 0)
		{
			// ���Ժܶ�κ� �Ҳ�����ŵ��ֱ��Ѱ·ʧ��
			return MOTION_RESULT_CANT_FIND_GRID;
		}

		// ����Ѱ·
		if (!FindPathToGrid(pKernel, self, nearestGrid))
		{
			IRecord* pRecord = pSelfObj->GetRecord(CANT_ARRIVE_GRID_LIST_RECORD);
			if (pRecord != NULL && pRecord->GetRows() < pRecord->GetRowMax())
			{
				// Ѱ·ʧ�ܣ������õ��޷������¼������
				pRecord->AddRowValue(-1, CVarList() << nearestGrid);
			}
			else
			{
				// �����쳣��ֱ�ӷ���Ѱ·ʧ��
				return MOTION_RESULT_CANT_ARRIVE;
			}

			break;
		}
	} while (0);

	// ����Ѱ·
	ContinueMotion(pKernel, self, false);

	return MOTION_RESULT_SUCCEED;
}

// Ѱ·����
// ֻ�����������ӿ��е��� MotionToPoint ContinueMotion MotionStop
// Ŀ���Ǳ�֤Ѱ·��Ȼ��Ѱ·��� �����������һЩ��ȫ���ж��к��Զ�����Ѱ·�ж���û��Ѱ·���
bool MotionModule::MotionEnd(IKernel * pKernel, const PERSISTID & self, MotionResult result)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return false;
	}

	// ֹͣ�ƶ�
	pKernel->Stop(self);

	pSelfObj->SetInt("MotionResult", result);

	// ���һִ֡��
	ADD_HEART_BEAT(pKernel, self, "MotionModule::HB_MotionEnd", WAIT_MOTION_TIME);
	return true;
}

// ���һִ֡��
int MotionModule::HB_MotionEnd(IKernel* pKernel, const PERSISTID& self, int time)
{
	if (!pKernel->Exists(self))
	{
		return false;
	}

	DELETE_HEART_BEAT(pKernel, self, "MotionModule::HB_MotionEnd");

	m_pMotionModule->MotionEnd(pKernel, self);

	return 0;
}

// ����Ѱ·���
bool MotionModule::MotionEnd(IKernel * pKernel, const PERSISTID & self)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return false;
	}

	// ����״̬����
	int result = ResetMotionState(pKernel, self);
	if (result == MOTION_RESULT_NONE)
	{
		// û�н�� ���ܱ��µ�Ѱ·���õ������÷��ͽ��
		return true;
	}

	// ���ͽ��
	const char* callBack = pSelfObj->QueryString("CallBackFunction");
	if (StringUtil::CharIsNull(callBack))
	{
		// ����Ҫ�ص�
		return true;
	}

	// ���һص�������ִ��
	MapCallBack::iterator iter = m_mapCallBack.find(callBack);
	if (iter != m_mapCallBack.end())
	{
		MOTION_CALL_BACK_FUNC callBackFunc = iter->second;
		if (callBackFunc != NULL)
		{
			callBackFunc(pKernel, self, result);			
		}
	}

	return true;
}

// ����Ѱ·״̬����
// ��������ǰ�Ľ��
int MotionModule::ResetMotionState(IKernel * pKernel, const PERSISTID & self)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return 0;
	}

	// ����״̬
	pSelfObj->SetInt("MotionState", MOTION_STATE_STOP);
	pSelfObj->SetInt("RoundGrid", 0);
	pSelfObj->SetInt64("WaitTarget", 0);
	pSelfObj->SetInt("NearTargetGrid", 0);
	pSelfObj->SetInt("RoundTotalCount", 0);
	pSelfObj->SetInt("MotionMethod", static_cast<int>(MOTION_METHOD_ALL));
	pSelfObj->SetFloat("WaitTargetX", 0.0f);
	pSelfObj->SetFloat("WaitTargetZ", 0.0f);
	pSelfObj->SetFloat("SelfLastX", 0.0f);
	pSelfObj->SetFloat("SelfLastZ", 0.0f);

	// ��ձ�
	IRecord* pRecord = pSelfObj->GetRecord(ROUND_PATH_LIST_RECORD);
	if (pRecord != NULL)
	{
		pRecord->ClearRow();
	}
	pRecord = pSelfObj->GetRecord(FIND_ROUND_PATH_RECORD);
	if (pRecord != NULL)
	{
		pRecord->ClearRow();
	}
	pRecord = pSelfObj->GetRecord(CANT_ARRIVE_GRID_LIST_RECORD);
	if (pRecord != NULL)
	{
		pRecord->ClearRow();
	}

	// ɾ������
// 	DELETE_HEART_BEAT(pKernel, self, "MotionModule::HB_WaitMotion");
// 	DELETE_HEART_BEAT(pKernel, self, "MotionModule::HB_CheckMotionBreak");

	int result = pSelfObj->QueryInt("MotionResult");
 
	pSelfObj->SetInt("MotionResult", MOTION_RESULT_NONE);

	return result;
}

// ȡ��ǰ���赽��ķ���
int MotionModule::GetNeedArriveGrid(IKernel * pKernel, const PERSISTID & self)
{
	// ������·����
	//LOOPS_WORKTIME_BEGIN();
	int targetGrid = GetPathGrid_loops(pKernel, self);
	if (targetGrid <= 0)
	{
		// ��Ŀ���������Ч����
		int nearestGrid = GetTargetNearestEnableGrid(pKernel, self);
		if (nearestGrid <= 0)
		{
			// Ŀ�긽���ĵ�ȫ����ռ�ã��޷�����Ŀ�꣬Ѱ·ʧ��
			return 0;
		}

		targetGrid = nearestGrid;
	}

	return targetGrid;
}

// Ѱ��Ŀ�긽�����ſ�ʹ�÷���
// ���ţ���Ŀ������Ƕ�С
int MotionModule::GetTargetNearestEnableGrid(IKernel * pKernel, const PERSISTID & self)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return 0;
	}

	// ȡĿ��㸽����
	int nearTargetGrid = pSelfObj->QueryInt("NearTargetGrid");

	// ȡ���ɵ���·����
	IRecord* pCantArriveRecord = pSelfObj->GetRecord(CANT_ARRIVE_GRID_LIST_RECORD);
	if (pCantArriveRecord == NULL)
	{
		return 0;
	}

	// Ŀ������
	if (GridEnabled(pKernel, self, nearTargetGrid))
	{
		// Ŀ�����Ե���
		if (pCantArriveRecord->FindInt(
			CANT_ARRIVE_GRID_LIST_RECORD_COL_grid, 
			nearTargetGrid) < 0)
		{
			return nearTargetGrid;
		}

	}

	// Ŀ���
	float targetX = pSelfObj->QueryFloat("TargetX");
	float targetZ = pSelfObj->QueryFloat("TargetZ");

	// ��С����
	float minDistance = pSelfObj->QueryFloat("PathDistanceMin");    
	float maxDistance = pSelfObj->QueryFloat("PathDistanceMax");    

	// Ŀ������ڸ���
	int index = pKernel->GetGridIndexByPos(targetX, targetZ);

	// ÿ������ռ��
	float len = pKernel->GetPathGridSize();
	if (util_float_equal_zero(len))
	{
		return 0;
	}

	// ���뻻��ɸ�����
	int grildLen = static_cast<int>(maxDistance / len);
	if (grildLen <= 0)
	{
		if (GridEnabled(pKernel, self, index))
		{
			// Ŀ�����Ե���
			if (pCantArriveRecord->FindInt(
				CANT_ARRIVE_GRID_LIST_RECORD_COL_grid, 
				index) < 0)
			{
				pSelfObj->SetInt("NearTargetGrid", index);
				return index;
			}
		}

		return 0;
	}

	// ������������
	return GetNearestEnableGridInAllQuadrant(pKernel, self, targetX, targetZ, minDistance, maxDistance);
}

// �����������ڲ�����Ŀ���������Ч��
int MotionModule::GetNearestEnableGridInAllQuadrant(IKernel * pKernel,
													const PERSISTID & self,
													float targetX, float targetZ,
													float minDistance, float maxDistance)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return 0;
	}

	// �Լ����ڵ�
	float selfX = pKernel->GetPosiX(self);
	float selfZ = pKernel->GetPosiZ(self);  

	// ��ͼ�ܸ��ӵ�������
	int iRowTotal = (int)pKernel->GetPathGridRows();
	if (iRowTotal <= 0)
	{
		return 0;
	}

	// Ŀ������ڸ���
	int index = pKernel->GetGridIndexByPos(targetX, targetZ);

	// Ŀ���
	CPoint pathPoint;
	pathPoint.x = (index - 1) % iRowTotal + 1;
	pathPoint.y = (index - 1) / iRowTotal;

	// ԭ�����ڸ���
	int selfIndex = pKernel->GetGridIndexByPos(selfX, selfZ);

	// ԭ��
	CPoint selfPoint;
	selfPoint.x = (selfIndex - 1) % iRowTotal + 1;
	selfPoint.y = (selfIndex - 1) / iRowTotal;

	// ԭ�㵽Ŀ��������
	CPoint vectorPoint;
	vectorPoint.x = selfPoint.x - pathPoint.x;
	vectorPoint.y = selfPoint.y - pathPoint.y;

	// ÿ������ռ��
	float len = pKernel->GetPathGridSize();
	if (util_float_equal_zero(len))
	{
		return 0;
	}

	// ���뻻��ɸ�����
	int grildLen = static_cast<int>(maxDistance / len);

	// �����Ŀ���������������ϣ����ȱ��������������
	if (vectorPoint.x == 0)
	{
		vectorPoint.x = 1;
	}
	if (vectorPoint.y == 0)
	{
		vectorPoint.y = 1;
	}

	CPoint tempVectorPoint = vectorPoint;
	int nearIndex = 0;

	// �����ĸ������ڵķ���
	// �ȱ����������ޣ��ٱ�����ý�������
	// ѭ������
	LoopBeginCheck(el);
	for (int i = 0; i < 4; ++i)
	{
		LoopDoCheck(el);
		switch (i)
		{
		case 0: // ��һ������
			//NULL;
			break;
		case 1:	// �ڶ�������
			if (abs(vectorPoint.x) < abs(vectorPoint.y))
			{
				tempVectorPoint.x = 0 - vectorPoint.x;
			}
			else
			{
				tempVectorPoint.y = 0 - vectorPoint.y;
			}
			break;
		case 2:	// ������������
			if (abs(vectorPoint.x) > abs(vectorPoint.y))
			{
				tempVectorPoint.x = 0 - vectorPoint.x;
			} 
			else
			{
				tempVectorPoint.y = 0 - vectorPoint.y;
			}
			break;
		case 3:	// ���ĸ�����
			tempVectorPoint.x = 0 - vectorPoint.x;
			tempVectorPoint.y = 0 - vectorPoint.y;
			break;
		default:
			//NULL;
			break;
		}

		// ȡ�������������յ��������������Ÿ�������
		nearIndex = GetNearestEnableGridInQuadrant(pKernel, 
			self,
			tempVectorPoint, 
			pathPoint, 
			grildLen, 
			targetX, targetZ, 
			minDistance, maxDistance, 
			iRowTotal);

		if (nearIndex > 0)
		{
			//extend_warning(LOG_WARNING, "FindTargetNearGrid index %d targetIdx %d i %d", nearIndex, index, i);
			pSelfObj->SetInt("NearTargetGrid", nearIndex);
			return nearIndex;
		}
	}

	return 0;
}

// ��ȡ��������Ŀ���������Ч����
int MotionModule::GetNearestEnableGridInQuadrant(IKernel * pKernel,
												 const PERSISTID & self,
												 CPoint& vectorPoint, 
												 CPoint& targetPoint, 
												 int length, 
												 float targetX, float targetZ,
												 float minDistance, float maxDistance,
												 int rowTotal)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return 0;
	}

	// Ŀ���
	if (vectorPoint.x == 0 || vectorPoint.y == 0 || length <= 0)
	{
		return 0;
	}

	// ȡ���ɵ���·����
	IRecord* pCantArriveRecord = pSelfObj->GetRecord(CANT_ARRIVE_GRID_LIST_RECORD);
	if (pCantArriveRecord == NULL)
	{
		return 0;
	}


	// ����
	int rowTotalX = 1;
	int rowTotalY = rowTotal;

	// �ߵ��������ȴ���
	if (abs(vectorPoint.x) < abs(vectorPoint.y))
	{
		int temp = 0;
		temp = vectorPoint.x;
		vectorPoint.x = vectorPoint.y;
		vectorPoint.y = temp;

		temp = targetPoint.x;
		targetPoint.x = targetPoint.y;
		targetPoint.y = temp;

		rowTotalX = rowTotal;
		rowTotalY = 1;
	}

	// ����ֵ����
	CPoint absPoint;
	absPoint.x = abs(vectorPoint.x);
	absPoint.y = abs(vectorPoint.y);
	if (absPoint.x == 0 || absPoint.y == 0)
	{
		return 0;
	}

	// ��λ����
	CPoint unitPoint;
	unitPoint.x = vectorPoint.x / absPoint.x;
	unitPoint.y = vectorPoint.y / absPoint.y;

	// ��������Ŀ�����
	float tempTargetDistance = 0.0f;
	// ����������
	int curIndex = 0;
	// ������
	CPoint point = targetPoint;

	// j�ı�������
	int jCount = 0;
	// ѭ������
	LoopBeginCheck(em);
	for (int i = 1; i < length + 1; ++i)
	{
		LoopDoCheck(em);
		point.x = targetPoint.x +  unitPoint.x * i;

		jCount = i * 2 + 1;
		// ѭ������
		LoopBeginCheck(en);
		for (int j = 0; j < jCount; ++j)
		{
			LoopDoCheck(en);
			if (j <= i)
			{
				point.y = targetPoint.y + unitPoint.y * j;
			}
			else
			{
				point.x = targetPoint.x - unitPoint.x * (jCount - j);
			}

			// ת��Ϊ��������
			curIndex = point.x * rowTotalX + point.y * rowTotalY;

			// �����Ӳ�����
			if (!GridEnabled(pKernel, self, curIndex))
			{
				continue;
			}

			// ���Ӳ����Ե���
			if (!(pCantArriveRecord->FindInt(
				CANT_ARRIVE_GRID_LIST_RECORD_COL_grid, 
				curIndex) < 0))
			{
				continue;
			}

			// ������Ӿ���
			float x = 0.0f;
			float z = 0.0f;
			pKernel->GetPosByIndex(curIndex, x, z);

			// �õ㲻��վ��
			//if (!CanStand(pKernel, x, z, CVarList()))
			if (!CanStandFast(pKernel, x, z))
			{
				continue;
			}

			// �����㵽Ŀ������
			tempTargetDistance = util_dot_distance(x, z, targetX, targetZ);

			// ����Ч������
			if (tempTargetDistance >= minDistance 
				&& tempTargetDistance < maxDistance)
			{
				//extend_warning(LOG_WARNING, "FindTargetNearGridInQuadrant i %d j %d", i, j);
				return curIndex;
			}
		}
	}

	return 0;
}

// Ѱ����������Ŀ�긽�����ſ����߷���
int MotionModule::GetSelfEnableGrid(IKernel * pKernel, const PERSISTID & self)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return 0;
	}

	//Ŀ���
	int pathIndex = GetNeedArriveGrid(pKernel, self);
	if (pathIndex <= 0)
	{
		// Ŀ�긽���Ҳ�����ŵ���ֱ��ȥĿ���
		// Ŀ���
		float targetX = pSelfObj->QueryFloat("TargetX");
		float targetZ = pSelfObj->QueryFloat("TargetZ"); 

		// Ŀ������ڸ���
		pathIndex = pKernel->GetGridIndexByPos(targetX, targetZ);
	}

	CVarList varIndex;
	if (!GetSelfGridList(pKernel, self, pathIndex, varIndex))
	{
		return 0;
	}

	// ���߹��ĸ��ӱ�
	IRecord* pRecord = pSelfObj->GetRecord(ROUND_PATH_LIST_RECORD);

	// ��¼���Ż�ͷ·������·���ߵ�ʱ����
	int firstEnableGridIndex = 0;

	int idex = 0;
	// ѭ������
	LoopBeginCheck(eo);
	for (size_t i = 0; i < varIndex.GetCount(); ++i)
	{
		LoopDoCheck(eo);
		idex = varIndex.IntVal(i);

		// �������ȼ���ߵĸ�������
		if (m_pMotionModule->GridEnabled(pKernel, self, idex, false))
		{
			float x = 0.0f;
			float z = 0.0f;
			pKernel->GetPosByIndex(idex, x, z);
			// �õ��ܷ�վ��
			//if (!CanStand(pKernel, x, z, CVarList()))
			if (!CanStandFast(pKernel, x, z ))
			{
				continue;
			}

			// �ɷ�ֱ�ߵ���
			if (!WalkLineEnabled(pKernel, self, x, z))
			{
				continue;
			}

			if (firstEnableGridIndex == 0)
			{
				firstEnableGridIndex = idex;
			}

			// ���߻�ͷ·
			if (pRecord == NULL || pRecord->FindInt(ROUND_PATH_LIST_RECORD_COL_grid, idex) < 0)
			{
				return idex;
			}
		}
	}

	// ��·���ߣ��߻�ͷ·�����߸�֪��·��
	return firstEnableGridIndex;
}

// ��ȡ��������Ŀ�긽���������ȼ���
// targetGrid Ŀ���
// varList �����б�
// step ����
bool MotionModule::GetSelfGridList(IKernel * pKernel, const PERSISTID & self, int targetGrid, IVarList& varList, int step/* = 1*/)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return false;
	}

	// ȡռ�õĸ�����
	int iGrid = pSelfObj->QueryInt("PathGrid");
	// ��ǰ���ڸ�������
	int indexGrid = pKernel->GetGridIndexByPos(pKernel->GetPosiX(self), pKernel->GetPosiZ(self));
	if (iGrid <=0)
	{
		varList << indexGrid;
		return true;
	}
	// ȡ��ͼ�ܸ��ӵ�����
	int iRowTotal = (int)pKernel->GetPathGridRows();
	if (iRowTotal <= 0)
	{
		return false;
	}

	CPoint curPoint;
	curPoint.x = (indexGrid - 1) % iRowTotal + 1;
	curPoint.y = (indexGrid - 1) / iRowTotal;

	CPoint pathPoint;
	pathPoint.x = (targetGrid - 1) % iRowTotal + 1;
	pathPoint.y = (targetGrid - 1) / iRowTotal;

	CPoint tempPoint = curPoint - pathPoint;
	if (tempPoint.x == 0 && tempPoint.y == 0)
	{
		return false;
	}

	// �㷨��ֻ���ź������������ķ����ƶ�
	// ���ȼ� ����Ŀ�� ���Ⱦ��볤�ķ��� ����Ŀ�� ���Ⱦ���̵ķ���
	if (tempPoint.x == 0)
	{
		tempPoint.x = tempPoint.y;
	}
	else if (tempPoint.y == 0)
	{
		tempPoint.y = tempPoint.x;
	}

	int absTempX = abs(tempPoint.x);
	int absTempY = abs(tempPoint.y);
	if (absTempX == 0 || absTempY == 0)
	{
		return false;
	}
	
	if (absTempY > absTempX)
	{
		varList << (indexGrid - (tempPoint.y / absTempY) * iGrid * iRowTotal * step)
				<< (indexGrid - (tempPoint.x / absTempX) * iGrid * step)
				<< (indexGrid + (tempPoint.x / absTempX) * iGrid * step)
				<< (indexGrid + (tempPoint.y / absTempY) * iGrid * iRowTotal * step);
	}
	else
	{
		varList << (indexGrid - (tempPoint.x / absTempX) * iGrid * step)
				<< (indexGrid - (tempPoint.y / absTempY) * iGrid * iRowTotal * step)
				<< (indexGrid + (tempPoint.y / absTempY) * iGrid * iRowTotal * step)
				<< (indexGrid + (tempPoint.x / absTempX) * iGrid * step);
	}
	
	return true;
}

// ��Ҫ��ǰ������ײ�������������Ŀ��ĸ����������ߵ��Ƿ������(�ӽ���ֱ������Ŀ��ĵ�һ������)
bool MotionModule::NeedDealCollision(IKernel * pKernel, const PERSISTID & self, int targetGrid, PERSISTID& collision)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return false;
	}

	CVarList varIndex;
	if (!GetSelfGridList(pKernel, self, targetGrid, varIndex))
	{
		return false;
	}

	// û�л�ȡ�����ߵ�
	if (varIndex.IsEmpty())
	{
		return false;
	}

	// ��ȡ�������ߵ�
	int gridIndex = varIndex.IntVal(0);

	// �Ƿ񱻶���ռ��
	if (!GridEnabled(pKernel, self, gridIndex, false))
	{
		CVarList ret;
		int groupId = pSelfObj->QueryInt("GroupID");
		pKernel->GetPathGridBalker(gridIndex, ret, groupId);
		if (!ret.IsEmpty() && ret.GetType(0) == VTYPE_OBJECT)
		{
			collision = ret.ObjectVal(0);
			return true;
		}
	}

	// ���߹��ĸ��ӱ�
	IRecord* pRecord = pSelfObj->GetRecord(ROUND_PATH_LIST_RECORD);
	if (pRecord == NULL)
	{
		return false;
	}

	// ��ͷ·
	if (pRecord->FindInt(ROUND_PATH_LIST_RECORD_COL_grid, gridIndex) >= 0)
	{
		return true;
	}

	return false;
}

// Ѱ·��ͨ��������һ������·��
bool MotionModule::FindPathToGrid(IKernel* pKernel, const PERSISTID& self, int targetGrid)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return false;
	}

	// ����Ƿ��������
	int method = pSelfObj->QueryInt("MotionMethod");
	if ((MOTION_METHOD_DYNAMIC & method) == 0 
		&& (MOTION_METHOD_STATIC & method) == 0)
	{
		// ��������ã�����
		return false;
	}

	float targetX = 0.0f;
	float targetZ = 0.0f;
	pKernel->GetPosByIndex(targetGrid, targetX, targetZ);

	bool ret = false;
	LoopBeginCheck(a);
	do
	{
		LoopDoCheck(a);
		// �޳����赲
		if (WalkLineEnabled(pKernel, self, targetX, targetZ))
		{
			// ���ȶ�̬Ѱ·
			if (!FindPathToGridDynamic(pKernel, self, targetGrid))
			{
				// ��̬Ѱ·ʧ�ܣ���̬Ѱ·
				ret = FindPathToGridStatic(pKernel, self, targetX, targetZ);
			}

			break;
		}

		// �г����赲�����Ⱦ�̬Ѱ·
		if (!FindPathToGridStatic(pKernel, self, targetX, targetZ))
		{
			// ��̬Ѱ·ʧ�ܣ���̬Ѱ·
			ret = FindPathToGridDynamic(pKernel, self, targetGrid);
		}

	} while (0);
	
	return ret;
}

// ��̬Ѱ·��ͨ��A*Ѱ·
bool MotionModule::FindPathToGridDynamic(IKernel* pKernel, const PERSISTID& self, int targetGrid)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return false;
	}

	// ����Ƿ��������
	int method = pSelfObj->QueryInt("MotionMethod");
	if ((MOTION_METHOD_DYNAMIC & method) == 0)
	{
		// ��������ã�����
		return false;
	}
	// �����
	MontionPos pos_sef;
	MontionPos pos_target;
	MotionHelper::ParseMotionPosAll(pKernel, pos_sef, pKernel->GetPosiX(self), pKernel->GetPosiZ(self), pKernel->GetPosiY(self));
	MotionHelper::ParseMotionPos(pKernel, pos_target, targetGrid);

	// ·����
	CVarList pathList;
	if (!MotionHelper::FindPathToTargets_Dynamic(pKernel, self, pos_sef, pos_target, pathList))
	{
		return false;
	}
	// ·������
	int count = (int)pathList.GetCount();
	if (count == 0)
	{
		return false;
	}

	// ��ȡ·����
	IRecord* pRecord = pSelfObj->GetRecord(FIND_ROUND_PATH_RECORD);
	if (pRecord == NULL)
	{
		return false;
	}

	// ����ձ�
	pRecord->ClearRow();

	// ����·������
	// ͨ��A*Ѱ·������ȡֵ
	float tempX = 0.0f;
	float tempZ = 0.0f;
	int tempGrid = 0;

	float newX = 0.0f;
	float newY = 0.0f;
	float newZ = 0.0f;
	float tempSrcX = pos_sef.pos_x_;
	float tempSrcZ = pos_sef.pos_z_;
	float walkStep = pSelfObj->QueryFloat("WalkStep");

	// ѭ������
	LoopBeginCheck(ep);
	for (int i = count - 1; i >= 0; --i)
	{
		LoopDoCheck(ep);
		// ��ȡ��������
		tempGrid = pathList.IntVal(i);

		// ��ȡ����
		pKernel->GetPosByIndex(tempGrid, tempX, tempZ);

		// ���Ƿ����վ��
		//if (!CanStand(pKernel, tempX, tempZ, CVarList()))
		if (!CanStandFast(pKernel, tempX, tempZ ))
		{
			return false;
		}

		// �ж��Ƿ����ֱ�ߵ���
		if (!pKernel->TraceLineWalk(walkStep, tempSrcX, 
			pKernel->GetMapHeight(tempSrcX, tempSrcZ), 
			tempSrcZ, tempX, tempZ, newX, newY, newZ))
		{
			return false;
		}

		tempSrcX = tempX;
		tempSrcZ = tempZ;

		pRecord->AddRowValue(-1, CVarList() << tempGrid << 0);
	}

	return true;
}

// ��̬Ѱ·��ͨ����������·��Ѱ·
bool MotionModule::FindPathToGridStatic(IKernel* pKernel, const PERSISTID& self, float targetX, float targetZ)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return false;
	}

	// ����Ƿ��������
	int method = pSelfObj->QueryInt("MotionMethod");
	if ((MOTION_METHOD_STATIC & method) == 0)
	{
		// ��������ã�����
		return false;
	}

	// ·����
	CVarList pathList;

	// �����
	MontionPos pos_sef;
	MontionPos pos_target;
	MotionHelper::ParseMotionPosAll(pKernel, pos_sef, pKernel->GetPosiX(self), pKernel->GetPosiZ(self), pKernel->GetPosiY(self));
	MotionHelper::ParseMotionPosEx(pKernel, pos_target, targetX, targetZ);

	float newX = 0.0f;
	float newY = 0.0f;
	float newZ = 0.0f;
	float tempSrcX = pos_sef.pos_x_;
	float tempSrcZ = pos_sef.pos_z_;
	float walkStep = pSelfObj->QueryFloat("WalkStep");

	int pointCount = 0;
	CVarList tempPathList;

	if (!MotionHelper::FindPathToTargets_Static(pKernel, self, pos_sef, pos_target, pointCount, tempPathList))
	{
		return false;
	}
	if ( pointCount > 0 )
	{
		// ѭ������
		LoopBeginCheck(eq);
		for (int i = 0; i < pointCount; ++i)
		{
			LoopDoCheck(eq);
			float tempX = tempPathList.FloatVal(i*3);
			float tempZ = tempPathList.FloatVal(i*3+2);
			int tempIndex = pKernel->GetGridIndexByPos(tempX, tempZ);
			if (tempIndex == pos_sef.grid_)
			{
				continue;
			}
			// ���Ƿ��������
			if (!pKernel->TraceLineWalk(walkStep, tempSrcX, pKernel->GetMapHeight(tempSrcX, tempSrcZ), tempSrcZ, tempX, tempZ, newX, newY, newZ))
			{
//#if _DEBUG // ����Ҫ��ʱ�������������
//				extend_warning(LOG_ERROR, "[%s][%d] StaticPath error srcX:%f, srcZ:%f, pathX:%f pathZ:%f can't walk line SceneId:%d SceneConfig:%s",
//					__FILE__, __LINE__, tempSrcX, tempSrcZ, tempX, tempZ, pKernel->GetSceneId(), pKernel->GetSceneConfig(pKernel->GetSceneId()));
//#endif
				return false;
			}
			// ���Ƿ����վ��
			//if (!CanStand(pKernel, tempX, tempZ, CVarList()))
			if (!CanStandFast(pKernel, tempX, tempZ ))
			{
//#if _DEBUG // ����Ҫ��ʱ�������������
//				extend_warning(LOG_ERROR, "[%s][%d] StaticPath error selfX:%f, selfZ:%f, pathX:%f pathZ:%f can't Stand SceneId:%d SceneConfig:%s",
//					__FILE__, __LINE__, selfX, selfZ, tempX, tempZ, pKernel->GetSceneId(), pKernel->GetSceneConfig(pKernel->GetSceneId()));
//#endif
				return false;
			}

			tempSrcX = tempX;
			tempSrcZ = tempZ;

			pathList << tempIndex;
		}
	}

	// ·������
	int count = (int)pathList.GetCount();
	if (count == 0)
	{
		return false;
	}

	// ��ȡ·����
	IRecord* pRecord = pSelfObj->GetRecord(FIND_ROUND_PATH_RECORD);
	if (pRecord == NULL)
	{
		return false;
	}

	// ����ձ�
	pRecord->ClearRow();

	// ����·������
	// ѭ������
	LoopBeginCheck(er);
	for (int i = 0; i < count; ++i)
	{
		LoopDoCheck(er);
		pRecord->AddRowValue(-1, CVarList() << pathList.IntVal(i) << 0);
	}

	return true;
}

// ��ȡ��̬Ѱ·��·����
int MotionModule::GetPathGrid_loops(IKernel* pKernel, const PERSISTID& self)
{
	//LOOPS_WORKTIME_CHECK(GetPathGrid);
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return 0;
	}

	// ��ȡ��
	IRecord* pRecord = pSelfObj->GetRecord(FIND_ROUND_PATH_RECORD);
	if (pRecord == NULL)
	{
		return 0;
	}
	
	// ����ʾ��Ϊ��
	if (pRecord->GetRows() <= 0)
	{
		return 0;
	}


	// �������ʴ���
	int findCount = pRecord->QueryInt(0, FIND_ROUND_PATH_RECORD_COL_count);
	if (findCount > FIND_ROUND_PATH_MAX_COUNT)
	{
		pRecord->ClearRow();
		return 0;
	}

	// ��ȡ��һ��·����
	int firstGrid = pRecord->QueryInt(0, FIND_ROUND_PATH_RECORD_COL_grid);

	// ��¼���ʴ���
	pRecord->SetInt(0, FIND_ROUND_PATH_RECORD_COL_count, ++findCount);

	// ����ռ�ø�����
	int gridSize = pSelfObj->QueryInt("PathGrid");

	// ��ͼ������
	int iTotalRow = (int)pKernel->GetPathGridRows();
	if (iTotalRow <= 0)
	{
		return 0;
	}

	// ��ǰ��������
	int curGrid = pKernel->GetGridIndexByPos(pKernel->GetPosiX(self), pKernel->GetPosiZ(self));

	// ����Ŀ��㣬ɾ����ǰ·���㣬��ȡ��һ��·����
	if (GridEnabled(pKernel, self, firstGrid, false) )
	{
		// Ŀ�����ã����뵽��Ŀ���
		if (curGrid == firstGrid)
		{
			pRecord->RemoveRow(0);
			return GetPathGrid_loops(pKernel, self);
		}
	}
	else
	{
		// Ŀ��㲻���ã�������Χ����

		// ��ǰ��������
		size_t curX = (curGrid - 1) % iTotalRow + 1;
		size_t curY = (curGrid - 1) / iTotalRow;

		// ·�����������
		size_t firstX = (firstGrid - 1) % iTotalRow + 1;
		size_t firstY = (firstGrid - 1) / iTotalRow;


		// �Ѿ����ﵱǰ����ߵ�����Χ��ɾ��֮
		if (curX >= (firstX - gridSize)
			&& curX <= firstX + gridSize
			&& curY >= firstY - gridSize
			&& curY <= firstY + gridSize)
		{
			pRecord->RemoveRow(0);
			return GetPathGrid_loops(pKernel, self);
		}

	}

	
	return firstGrid;

}

// �����Ƿ��ܹ�ֱ�ߵ���(ֻ���ǳ�����ײ)
bool MotionModule::WalkLineEnabled(IKernel* pKernel, const PERSISTID& self, float targetX, float targetZ)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return false;
	}

	// �����
	float selfX = pKernel->GetPosiX(self);
	float selfY = pKernel->GetPosiY(self);
	float selfZ = pKernel->GetPosiZ(self);
	float newX = 0.0f;
	float newY = 0.0f;
	float newZ = 0.0f;

	len_t walkStep = pSelfObj->QueryFloat("WalkStep");

	return pKernel->TraceLineWalk(walkStep, selfX, selfY, selfZ, targetX, targetZ, newX, newY, newZ);
}

// �õ��ܷ�վ��
bool MotionModule::CanStand(IKernel* pKernel, float x, float z, IVarList& heightList)
{
	// �����б�
	heightList.Clear();

	bool bCanMove = false;

	// �������в���
	int count = pKernel->GetFloorCount(x, z);

	// ѭ������
	LoopBeginCheck(es);
	for (int i = 0; i < count; ++i)
	{
		LoopDoCheck(es);
		if (pKernel->GetFloorCanStand(x, z, i))
		{
			// ��վ��
			bCanMove = true;

			// �߶��б�
			heightList << pKernel->GetFloorHeight(x, z, i);
		}
	}

	// ����
	return bCanMove;
}
bool MotionModule::CanStandFast(IKernel* pKernel, float x, float z)
{
	bool bCanMove = false;
	// �������в���
	int count = pKernel->GetFloorCount(x, z);
	// ѭ������
	LoopBeginCheck(es);
	for (int i = 0; i < count; ++i)
	{
		LoopDoCheck(es);
		if (pKernel->GetFloorCanStand(x, z, i))
		{
			// ��վ��
			bCanMove = true;
			break;
		}
	}
	// ����
	return bCanMove;
}

// �ܷ��ƶ�
bool MotionModule::CanMove(IKernel * pKernel, const PERSISTID & target)
{
	IGameObj* pTargetObj = pKernel->GetGameObj(target);
	if (pTargetObj == NULL)
	{
		return false;
	}

	if(pTargetObj->QueryInt("Dead") > 0)
	{
		return false;
	}

	if (pTargetObj->QueryInt("CantMove") > 0)
	{
		return false;
	}

	return true;
}

// ���������ܶ�����
int MotionModule::C_OnCantMoveChanged(IKernel* pKernel, const PERSISTID& self,
								   const char* property, const IVar& old)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return 0;
	}

	// ��������
	if (pSelfObj->QueryInt("CantMove") >= MOVE_NO)
	{
		m_pMotionModule->MotionStop(pKernel, self);
/////////////////////////////////////////////////////////////
// #ifndef FSROOMLOGIC_EXPORTS
// 		m_pBattleTeamModule->Stop(pKernel, self);
// #endif // FSROOMLOGIC_EXPORTS
/////////////////////////////////////////////////////////////
	}

	return 0;
}

// �����ٶȱ仯
int MotionModule::C_OnSpeedChanged(IKernel* pKernel, const PERSISTID& self,
								   const char* property, const IVar& old)
{
	if (!pKernel->Exists(self))
	{
		return 0;
	}

	m_pMotionModule->UpdateMoveSpeed(pKernel, self);

	return 0;
}

// ˢ���ٶ�
void MotionModule::UpdateMoveSpeed(IKernel* pKernel, const PERSISTID& self)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return;
	}

	float speed = pSelfObj->QueryFloat("RunSpeed") + pSelfObj->QueryFloat("RunSpeedAdd");

	pSelfObj->SetFloat("MoveSpeed", speed);

	// �������Ѱ· ������Ѱ·��Ӧ�����µ��ٶ�
	int motionState = pSelfObj->QueryInt("MotionState");
	if (motionState != MOTION_STATE_STOP)
	{
		// Ŀ���
		float targetX = pSelfObj->QueryFloat("TargetX");
		float targetZ = pSelfObj->QueryFloat("TargetZ");

		// ��Ŀ����С����
		float minDistance = pSelfObj->QueryFloat("PathDistanceMin");

		// ��Ŀ��������
		float maxDistance = pSelfObj->QueryFloat("PathDistanceMax");

		// �ص�����
		const char* strCallBackFunc = pSelfObj->QueryString("CallBackFunction");
		if (strCallBackFunc == NULL)
		{
			strCallBackFunc = "";
		}

		// ����Ѱ·��ʽ
		MotionMethod method = static_cast<MotionMethod>(pSelfObj->QueryInt("MotionMethod"));

		// ����Ѱ·״̬
		m_pMotionModule->ResetMotionState(pKernel, self);

		// ��ֹͣ�ƶ����˴�ֱ�ӵ�������ֹͣ�ƶ���
		// ��Ϊ�ı��ٶȺ���Ҫ��ͣ��֮ǰ��Ѱ· ���¿�ʼѰ·����Ӧ��ԭ�����ٶ�
		// ��Ϊ�˴�������Ѱ·�����Բ��÷���Ѱ·���, ��˲��õ���MotionStop�ӿ�
		pKernel->Stop(self);

		// ������Ѱ·
		m_pMotionModule->MotionToPoint(pKernel, self, targetX, targetZ, 
			minDistance, maxDistance, strCallBackFunc, method);
	}
}

// ֹͣ�ƶ�
bool MotionModule::MotionStop(IKernel* pKernel, const PERSISTID& self)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return false;
	}

	// ���������� ����Ҫֹͣ�ƶ�
	if(pSelfObj->FindAttr("LifeTime") && pSelfObj->QueryInt("LifeTime") > 0)
	{
		return false;
	}

// #ifndef FSROOMLOGIC_EXPORTS
// 	m_pBattleTeamModule->Stop(pKernel, self);
// #endif // FSROOMLOGIC_EXPORTS

	if (m_pMotionModule != NULL 
		&& pSelfObj->QueryInt("MotionState") != MOTION_STATE_STOP)
	{
		// �����ƶ��������
		return m_pMotionModule->MotionEnd(pKernel, self, MOTION_RESULT_STOP);
	}

	return pKernel->Stop(self);
}