//--------------------------------------------------------------------
// �ļ���:      MotionModule.h
// ��  ��:      �ƶ�
// ��������:    2014-10-15
// ������:      liumf
//    :        
//--------------------------------------------------------------------
#ifndef _MOTION_MODULE_H_
#define _MOTION_MODULE_H_

#include "Fsgame/Define/header.h"
#include <map>
#include "Fsgame/SystemFunctionModule/MotionHelper.h"

// Ѱ·�ƶ����
enum MotionResult
{
	MOTION_RESULT_NONE				= 0,	// û�н��
	MOTION_RESULT_SUCCEED			= 1,	// �ƶ�Ѱ·�ɹ�
	MOTION_RESULT_SELF_INEXIST		= 2,	// ��������
	MOTION_RESULT_CANT_FIND_GRID	= 3,	// �Ҳ�����ŵ�(Ŀ�긽��û�п�ʹ�õ����վ)
	MOTION_RESULT_CANT_ARRIVE		= 4,	// ���ɵ���
	MOTION_RESULT_CANT_MOVE			= 5,	// ��ǰ�����ƶ�
	MOTION_RESULT_CANT_STAND		= 6,	// Ŀ��㲻��վ��
	MOTION_RESULT_STOP				= 7,	// ֹͣ�ƶ�
	MOTION_RESULT_FAILED			= 8,	// ���������ƶ�ʧ��
	MOTION_RESULT_SELF_CLOSE_IN		= 9,	// ������Χ �޷��ƶ�
	MOTION_RESULT_BREAK				= 10,	// Ѱ·�����ж�
};

// �ƶ��ص�����
typedef int (__cdecl* MOTION_CALL_BACK_FUNC)(IKernel* pKernel, const PERSISTID& self, int);
#define REGISTER_MOTION_CALLBACK(back) \
{ \
	MotionModule* pMotionModule = dynamic_cast<MotionModule*>(pKernel->GetLogicModule("MotionModule")); \
	if (pMotionModule != NULL) \
	{ \
		pMotionModule->RegisterMotionCallBackFunction(#back, back); \
	} \
}

/////////////////////////////////////////////////////////////////
// #ifndef FSROOMLOGIC_EXPORTS
// #include <vector>
// using namespace std;
// #include "FsGame/SocialSystemModule/BattleTeamModule.h"
// #endif
/////////////////////////////////////////////////////////////////

enum MotionResult;
class CPoint;
class MotionModule: public ILogicModule
{
public:
	// ��ʼ��
	virtual bool Init(IKernel* pKernel);

	// �ر�
	virtual bool Shut(IKernel* pKernel);
private:

	static int OnEntry(IKernel* pKernel, const PERSISTID& self,
		const PERSISTID& sender, const IVarList& args);

	// ����
	static int OnRecover(IKernel* pKernel, const PERSISTID& self,
		const PERSISTID& sender, const IVarList& args);

	// �ƶ��ص�
	static int OnMotion(IKernel* pKernel, const PERSISTID& self,
		const PERSISTID& sender, const IVarList& args);

	// �ȴ��ϰ����ƿ�
	static int HB_WaitMotion(IKernel* pKernel, const PERSISTID& self, int time);

	// ���һִ֡��
	static int HB_MotionEnd(IKernel* pKernel, const PERSISTID& self, int time);

	// �����ٶȱ仯
	static int C_OnSpeedChanged(IKernel* pKernel, const PERSISTID& self,
		const char* property, const IVar& old);

	// �ɷ��ƶ����ͱ仯
	static int C_OnCantMoveChanged(IKernel* pKernel, const PERSISTID& self,
		const char* property, const IVar& old);

	//�����ƶ�
	static int OnPlayerRequestMove(IKernel* pKernel, const PERSISTID& self,
		const PERSISTID& sender, const IVarList& args);

	// ��Ӧ�����ƶ�ֹͣ
	static int OnRequestMoveStop(IKernel* pKernel, const PERSISTID& self, const IVarList& args);

	// ��Ӧ�����ƶ�
	static int OnRequestMoveMotion(IKernel* pKernel, const PERSISTID& self, int nMMode, const IVarList& args);

	// ������
	static int ForceOffline(IKernel* pKernel, const PERSISTID& self);

	// �ͻ�����Ϣ����
// 	static int OnCustomHeartBeat(IKernel* pKernel, const PERSISTID& self,
// 		const PERSISTID& sender, const IVarList& args);
public:

	// ע���ƶ��ص�����
	bool RegisterMotionCallBackFunction(const char* strFunc, MOTION_CALL_BACK_FUNC pFunc);


 	//************************************
 	// Method:    MotionToObject
 	// FullName:  MotionModule::MotionToObject
 	// Access:    public 
 	// Returns:   bool
 	// Qualifier: ��Ŀ������ƶ�
 	// Parameter: IKernel * pKernel ������
 	// Parameter: const PERSISTID & self ����
 	// Parameter: const PERSISTID & target Ŀ��
 	// Parameter: float minDistance ͣ����С���� ��Ŀ��
 	// Parameter: float maxDistance ͣ�������� ��Ŀ��
 	// Parameter: const char * strCallBackFunc �ص�����
 	// Parameter: MotionMethod method Ѱ·��ʽ
 	//************************************
 	bool MotionToObject(IKernel* pKernel, const PERSISTID& self, 
 		const PERSISTID& target, float minDistance = 0.0f, 
		float maxDistance = 0.0f, const char* strCallBackFunc = "",
		MotionMethod method = MOTION_METHOD_ALL);

	//************************************
	// Method:    MotionToPoint
	// FullName:  MotionModule::MotionToPoint
	// Access:    public 
	// Returns:   bool
	// Qualifier: //��Ŀ����ƶ�
	// Parameter: IKernel * pKernel	������	
	// Parameter: const PERSISTID & self ����
	// Parameter: float targetX Ŀ���x����
	// Parameter: float targetZ Ŀ���z����
	// Parameter: float minDistance ͣ����С���� ��Ŀ���
	// Parameter: float maxDistance ͣ�������� ��Ŀ���
	// Parameter: const char * strCallBackFunc �ص�����
	// Parameter: MotionMethod method Ѱ·��ʽ
	//************************************
	bool MotionToPoint(IKernel* pKernel, const PERSISTID& self,
		float targetX, float targetZ, float minDistance = 0.0f, 
		float maxDistance = 0.0f, const char* strCallBackFunc = "",
		MotionMethod method = MOTION_METHOD_ALL);

private:
	// ��Ŀ����ƶ�
	// bForecastCollision ��ǰԤ���Ƿ�ᷢ����ײ
	bool ContinueMotion(IKernel* pKernel, const PERSISTID& self, bool bForecastCollision = true);
	// ֱ�ߵ������
	MotionResult MotionLineToGrid(IKernel* pKernel, const PERSISTID& self, int gridIndex);
	// �ƶ��ɹ�(���ܵ�����ʱĿ��㣬�����ѵ����յ㣬ͨ�������ƶ�������һ�����)
	bool MotionSucceed(IKernel* pKernel, const PERSISTID& self);
	// ���óɹ�
	bool RoundBarrierSucceed(IKernel* pKernel, const PERSISTID& self);
	// ��ײ����
	MotionResult DealCollision(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender);
	// �ƹ��ϰ���
	MotionResult RoundBarrier(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender);
	// �޷�����Ŀ����Ĵ���
	MotionResult DealCantArriveTarget(IKernel* pKernel, const PERSISTID& self);
	// Ѱ·����
	// ֻ�����������ӿ��е��� MotionToPoint ContinueMotion MotionStop
	// Ŀ���Ǳ�֤Ѱ·��Ȼ��Ѱ·��� �����������һЩ��ȫ���ж��к��Զ�����Ѱ·�ж���û��Ѱ·���
	bool MotionEnd(IKernel * pKernel, const PERSISTID & self, MotionResult result);
	bool MotionEnd(IKernel * pKernel, const PERSISTID & self);

	// ����Ѱ·״̬����
	// ��������ǰ�Ľ��
	int ResetMotionState(IKernel * pKernel, const PERSISTID & self);

	// ȡ��ǰ���赽��ķ���
	int GetNeedArriveGrid(IKernel * pKernel, const PERSISTID & self);
	// Ѱ����Ŀ������Ŀ�ʹ�÷���
	int GetTargetNearestEnableGrid(IKernel * pKernel, const PERSISTID & self);
	// �����������ڲ�����Ŀ���������Ч��
	int GetNearestEnableGridInAllQuadrant(IKernel * pKernel,
		const PERSISTID & self,
		float targetX, float targetZ,
		float minDistance, float maxDistance);
	// ��ȡ��������Ŀ���������Ч����
	int GetNearestEnableGridInQuadrant(IKernel * pKernel, 
		const PERSISTID & self,
		CPoint& vectorPoint, CPoint& targetPoint, 
		int length, float targetX, float targetZ,
		float minDistance, float maxDistance,int rowTotal);


	// Ѱ�������������߷���
	int GetSelfEnableGrid(IKernel * pKernel, const PERSISTID & self);
	// ��ȡ�������������ȼ���
	bool GetSelfGridList(IKernel * pKernel, const PERSISTID & self, int targetGrid, IVarList& gridList, int step = 1);
	// ����������������ߵ��Ƿ������
	bool NeedDealCollision(IKernel * pKernel, const PERSISTID & self, int targetGrid, PERSISTID& collision);

	// Ѱ·,��һ������·��
	bool FindPathToGrid(IKernel* pKernel, const PERSISTID& self, int targetGrid);
	// ��̬Ѱ·��ͨ��A*Ѱ·
	bool FindPathToGridDynamic(IKernel* pKernel, const PERSISTID& self, int targetGrid);
	// ��̬Ѱ·��ͨ����������·��Ѱ·
	bool FindPathToGridStatic(IKernel* pKernel, const PERSISTID& self, float targetX, float targetZ);
	// ��ȡ��̬Ѱ·��·����
	int GetPathGrid_loops(IKernel* pKernel, const PERSISTID& self);

	// �����Ƿ��ܹ�ֱ�ߵ������(ֻ���ǳ�����ײ)
	bool WalkLineEnabled(IKernel* pKernel, const PERSISTID& self, float targetX, float targetZ);

	// �ܷ�վ��
	bool CanStand(IKernel* pKernel, float x, float z, IVarList& heightList);
	bool CanStandFast(IKernel* pKernel, float x, float z);
	// �ܷ��ƶ�
	bool CanMove(IKernel * pKernel, const PERSISTID & target);

	// ˢ���ٶ�
	void UpdateMoveSpeed(IKernel* pKernel, const PERSISTID& self);
public:
	static MotionModule* m_pMotionModule;

private:
	typedef std::map<std::string, MOTION_CALL_BACK_FUNC> MapCallBack;
	static MapCallBack m_mapCallBack;

public:
	static double m_distTotalWarning;	// ���λ�����У������ֵ
	static double m_distTotalMax;		// ���λ�����У�����ֵ
	static int m_heartBeatWarning;		// ��������ֵ
	static int m_heartBeatMax;			// �������ֵ
	static int m_cantLoginTime;			// ǿ�����ߺ� ���ܵ�¼��ʱ��
	static bool m_bCheckPlayerState;	// �Ƿ������״̬�쳣

#ifndef FSROOMLOGIC_EXPORTS
private:// ��ս��ײ
	int OnMotionBattleTeam(IKernel* pKernel, const PERSISTID& self,
		const PERSISTID& sender, const IVarList& args);
#endif

public: // �ⲿʹ��
	
	//************************************
	// Method:    GetGridCanStandPoint
	// Access:    public static 
	// Returns:   int ��������
	// Qualifier: ����Ŀ��㸽���Ŀ�ʹ������
	// Parameter: IKernel * pKernel ������
	// Parameter: const PERSISTID & self ����
	// Parameter: float destX	Ŀ���x����
	// Parameter: float destZ	Ŀ���z����
	// Parameter: float & nearX Ŀ��㸽����ʹ��x����
	// Parameter: float & nearZ Ŀ��㸽����ʹ��z����
	//************************************
	static int GetEnableGridNearTarget(IKernel* pKernel, const PERSISTID& self, float destX, float destZ, float& nearX, float& nearZ);

	//�����Ƿ����
	static bool GridEnabled(IKernel * pKernel, const int iIndex, int groupId);

	// �����Ƿ���� 
	// �����self �Ƿ����
	// self��ռ��������ͬ�����ӵ�ռ�������ͬ
	// iIndex ��ǰռ�ø���
	// bSpace �Ƿ�鿴�߼�ռ��
	static bool GridEnabled(IKernel * pKernel,const PERSISTID& self, const int iIndex, bool bSpace = true); 

	// �໥��ļ��������ռ��
	static bool IsGridSpace(IKernel* pKernel, const PERSISTID& self, const IVarList& gridList);

	// ֹͣ�ƶ�
	static bool MotionStop(IKernel* pKernel, const PERSISTID& self);
};

#endif // _PLAYER_BASE_MODULE_H_
