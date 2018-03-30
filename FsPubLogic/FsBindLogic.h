//--------------------------------------------------------------------
// �ļ���:		FsBindLogic.h
// ��  ��:		�����߼�����ҵ��
// ˵  ��:		
// ��������:	2017��08��02��
// ������:		kevin
// ��Ȩ����:	WhalesGame Technology co.Ltd
//--------------------------------------------------------------------
#ifndef __FS_PUB_BIND_LOGIC_H__
#define	__FS_PUB_BIND_LOGIC_H__

//#include "../FsGame/Define/BaseDefine.h"
//#include "../FsGame/SystemFunctionModule/TimeHelper.hpp"
#include "../utils/Singleton.h"
#include "Public/IPubLogic.h"
//#include "../FsGame/Define/ActivityEvent.h"
#include "../FsGame/Define/PubCmdDefine.h"

struct BindMemeberInfo
{
public:
	bool is_open_;
	int id_;
	int scene_;
public:
	BindMemeberInfo() :is_open_(false), id_(0), scene_(0){}
	~BindMemeberInfo(){}
};
class FsBindLogic :public ISingleton<FsBindLogic>
{
private:
	enum { MAX_MEMBER_NUM = 24 };
	BindMemeberInfo member_list_[MAX_MEMBER_NUM];
private:
// 	TimeChangeResult time_charge_;
// 	LogicTimeHelper  time_helper_;
private:
	CVarList   cmd_args_;
public:
	std::wstring bind_pub_data_name_;
public:
	FsBindLogic();
	~FsBindLogic();
public:
	// init,close
	bool Init(IPubKernel* pPubKernel);
	bool Start(IPubKernel* pPubKernel);
	bool Close(IPubKernel* pPubKernel);
public:
	// ���յ���������������Ϣ
	bool OnPublicRegist(IPubKernel* pPubKernel, int source_id, int scene_id, const IVarList& args);
public:
	bool SendToMemberMaster(IPubKernel* pPubKernel, const IVarList& args);
	bool SendToMemberAll(IPubKernel* pPubKernel, const IVarList& args);
public:
	// for command
	bool SendPublicCommand(IPubKernel* pPubKernel, int cmd_id, const IVarList& args);
	// for activity
	//bool GetActivityNode(int id, IActivityNode*& p_dc);
};

#endif