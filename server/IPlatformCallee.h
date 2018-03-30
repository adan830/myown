//--------------------------------------------------------------------
// �ļ���:		IPlatformCallee.h
// ��  ��:		���ʵ��ƥ�������߼��ص��ӿڶ���
// ˵  ��:		
// ��������:	2013��11��26��
// ������:		���н�
// ��Ȩ����:	������ţ�������޹�˾
//--------------------------------------------------------------------

#ifndef _SERVER_IPLATFORMCALLEE_H
#define _SERVER_IPLATFORMCALLEE_H

#include "../public/Macros.h"
#include "../public/IVarList.h"

// ���ʵ��ƥ�����ݻص��ӿ�

class IPlatformKernel;

class IPlatformCallee
{
public:
	virtual ~IPlatformCallee() = 0;

	/// \brief ƽ̨���ݷ���������
	/// \param pPlatformKernel ����ָ�룬��ͬ
	virtual int OnPlatformStart(IPlatformKernel* pPlatformKernel) = 0;

	/// \brief ������ұ�����
	/// \result ������ڱ� APPLY_TABLE_NAME, ���ᱻ����
	virtual bool OnCreateApplyTable(IPlatformKernel* pPlatformKernel) = 0;

	/// \brief ���յ������������ı�����Ϣ
	/// \param role_uid ��ɫuid
	/// \param command ��Ϣ����
	virtual int OnRequestApply(IPlatformKernel* pPlatformKernel, 
		const char* role_uid, const IVarList& command) = 0;

	/// \brief ���յ������������ı�����Ϣ
	/// \param leader_uid �ӳ�uid
	/// \param uid_list	 ��Աuid �б�
	/// \param command ��Ϣ����
	virtual int OnRequestApplyGroups(IPlatformKernel* pPlatformKernel, 
		const char* leader_uid, const IVarList& uid_list, const IVarList& command) = 0;

	/// \brief ��ʱƥ�䣨ƥ���߼�������ص����棩
	/// \param msg ��Ϣ����
	virtual int OnMatchingRole(IPlatformKernel* pPlatformKernel) = 0;

	/// \brief ����custom ��Ϣ
	/// \param command ��Ϣ����
	virtual int OnRoomCustom(IPlatformKernel* pPlatformKernel, const IVarList& command) = 0;

	/// \brief Member custom ��Ϣ
	/// \param command ��Ϣ����
	virtual int OnMemberCustom(IPlatformKernel* pPlatformKernel, const IVarList& command) = 0;

	/// \brief �������ر�֪ͨ
	virtual int OnServerClose(IPlatformKernel* pPlatformKernel) = 0;

	/// \brief extra ��������Ϣ����
	virtual int OnExtraMessage(IPlatformKernel* pPlatformKernel, int index , const IVarList& command) = 0;
};

inline IPlatformCallee::~IPlatformCallee() {}

#endif // _SERVER_IPlatformCALLEE_H

