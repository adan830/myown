//--------------------------------------------------------------------
// �ļ���:		ICrossCallee.h
// ��  ��:		���������߼��ص��ӿڶ���
// ˵  ��:		
// ��������:	2012��11��02��
// ������:		����
// ��Ȩ����:	������ţ�������޹�˾
//--------------------------------------------------------------------

#ifndef _SERVER_ICROSSCALLEE_H
#define _SERVER_ICROSSCALLEE_H

#include "../public/Macros.h"
#include "../public/IVarList.h"

// �������ݻص��ӿ�

class ICrossKernel;

class ICrossCallee
{
public:
	virtual ~ICrossCallee() = 0;

	/// \brief �������ݷ���������
	/// \param pCrossKernel ����ָ�룬��ͬ
	virtual int OnCrossStart(ICrossKernel* pCrossKernel) = 0;

	/// \brief ����������������
	/// \param wsDataName ������������
	virtual int OnCrossDataLoaded(ICrossKernel* pCrossKernel, const wchar_t* wsDataName) = 0;

	/// \brief ���յ���������������Ϣ
	/// \param nServerId ����Ϣ�ķ�����id
	/// \param nSceneId ����Ϣ�ĳ���id
	/// \param msg ��Ϣ����
	virtual int OnCrossMessage(ICrossKernel* pCrossKernel, int nServerId, 
		int nSceneId, const IVarList& msg) = 0;

	/// \brief ����������
	/// \param nServerId ��������ʶ
	/// \param wsDataName ���������ӿ�����������
	/// \param msg ��Ϣ����
	virtual int OnAddServer(ICrossKernel* pCrossKernel, int nServerId, 
		const wchar_t* wsDataName, const IVarList& msg) = 0;

	/// \brief �Ƴ�������
	/// \param nServerId ��������ʶ
	/// \param wsDataName ���������ӿ�����������
	/// \param msg ��Ϣ����(����Ľ�����Ϣidc1:ip1:port1,idc2:ip2:port2)
	virtual int OnRemoveServer(ICrossKernel* pCrossKernel, int nServerId, 
		const wchar_t* wsDataName, const IVarList& msg) = 0;

	/// \brief ������ÿ֡����
	/// \param nFrameTicks ��֡�ĺ�����
	virtual int OnFrameExecute(ICrossKernel* pCrossKernel, int nFrameTicks) = 0;

	/// \brief �������ر�֪ͨ
	virtual int OnServerClose(ICrossKernel* pCrossKernel) = 0;
};

inline ICrossCallee::~ICrossCallee() {}

#endif // _SERVER_ICROSSCALLEE_H

