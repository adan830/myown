//--------------------------------------------------------------------
// �ļ���:		SafeAreaNpcModule.h
// ��  ��:		��ȫ��NPC
// ˵  ��:		
// ��������:	2014��06��10��
// ��������:	2015��03��30��
// ������:		  ( )
// �޸���:        ( )
//    :	    
//--------------------------------------------------------------------

#ifndef __SafeAreaNpc_H__
#define __SafeAreaNpc_H__

#include "Fsgame/Define/header.h"
#include "utils/shape_collision.h"

class AsynCtrlModule;
class SafeAreaNpcModule: public ILogicModule  
{
public:
	// ��ʼ��
	virtual bool Init(IKernel* pKernel);
	// �ͷ�
	virtual bool Shut(IKernel* pKernel);

public:
	// �����ص�
	static int OnSpring(IKernel* pKernel, const PERSISTID& self, 
		const PERSISTID& sender, const IVarList& args);

	// ���������ص�
	static int OnEndSpring(IKernel* pKernel, const PERSISTID& self,
		const PERSISTID& sender, const IVarList& args);

	// ��ȫ��NPC���볡��
	static int OnEntry( IKernel * pKernel, const PERSISTID & self,
		const PERSISTID & sender, const IVarList & args );

	// ��ȫ��NPC����
	static int OnDestroy(IKernel* pKernel, const PERSISTID& self,
		const PERSISTID& sender, const IVarList& args);

	// ����뿪����
	static int OnLeaveScene(IKernel* pKernel, const PERSISTID& self,
		const PERSISTID& sender, const IVarList& args);

	// ��ҽ��볡��
	static int OnPlayerEnterScene(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);

	// ��Ӧ����仯 
	static int OnCommandChangeGroup(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);
private:
	struct RectangularSafeArea 
	{
		int				nAreaId;			// ��ȫ��id
		int				nSceneId;			// ���ڳ���id
		int				nNation;			// ������Ӫ
		int				nSafeRule;			// ��ȫ������
		Rectangle2D		kRectangle;			// ����
	};

	typedef std::vector<RectangularSafeArea> RectangularSafeAreaVec;

	enum 
	{
		 NOT_IN_RECT_SAFE_AREA = 0,		// û�ھ��ΰ�ȫ��
	};

	// ��ȫ������
// 	enum SafeType
// 	{
// 		CANT_BE_ATTACK,					// ���ܱ�����
// 		AVOID_PVP,						// ���ܽ���PVP
// 	};

	//���½��밲ȫ��״̬
	void UpdateEntryState(IKernel* pKernel, const PERSISTID& player, int nNation, int nSafeRule);

	//�����뿪��ȫ��״̬
	void UpdateLeaveState(IKernel* pKernel, const PERSISTID& player, int nNation, int nSafeRule);

	// �Ƿ񱣻�(������֤)
	bool IsSafe(IKernel* pKernel, const PERSISTID& player, int nNation, int nSafeRule, const int type);

    //��Ӧ�ͻ���������ʾ��Ϣ
	void ResponseClientMsg(IKernel* pKernel, const PERSISTID& player, const int info_id);

	// ��ȡ���ΰ�ȫ������
	bool LoadRectanularSafeAreaConfig(IKernel* pKernel);

	// ��������
	bool ParsePos(FmVec2& outPos, const char* strPos);

	// �Ƿ���Ҫ�����ΰ�ȫ�� 
	bool NeedCheckRectangularSafeArea(IKernel* pKernel);

	// ����Ƿ�����뿪���ΰ�ȫ��
	static int HB_CheckRectangularSafeArea(IKernel* pKernel, const PERSISTID& self, int slice);

	// ����Ƿ��ھ��ΰ�ȫ���� 	 int ��ȫ��id
	void CheckEnterRectangularSafeArea(IKernel* pKernel, const PERSISTID& self);

	// ����Ƿ���ָ���İ�ȫ����
	bool CheckInRectSafeArea(IKernel* pKernel, const PERSISTID& self, const Rectangle2D& rect);

	// ����id��ѯ��ȫ������
	const RectangularSafeArea* QueryRectangularSafeAreaInfo(int nIndex);
public:
	static SafeAreaNpcModule * m_pSafeAreaNpc;

private:
	static AsynCtrlModule * m_pAsynCtrlModule;
	RectangularSafeAreaVec			m_vecRectangularSafeArea;			//��ȫ������
};

#endif // __SafeAreaNpc_H__
