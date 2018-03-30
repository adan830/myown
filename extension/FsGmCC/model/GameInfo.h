//--------------------------------------------------------------------
// �ļ���:		GameInfo.h
// ��  ��:		���Է�����״̬��Ϣ
// ˵  ��:		
// ��������:		2016��12��03��
// ������:		���
// ��Ȩ����:		���ݾ����������޹�˾
//--------------------------------------------------------------------
#ifndef __MODEL_GAME_SERVER_INFO_H__
#define __MODEL_GAME_SERVER_INFO_H__
#include <string>
#include <map>
#include <time.h>
#include "utils\FsSpinLock.h"
#include <vector>

enum EmMemberState
{
	EM_MEMBER_STATE_CLOSING = 1,			// �ر���
	EM_MEMBER_STATE_CLOSED,					// �ر���
	EM_MEMBER_STATE_OPENING,				// ������
	EM_MEMBER_STATE_OPENED,					// �ѿ���
	EM_MEMBER_STATE_DEAD,					// ����
};

enum EmMemberWorkState
{
	EM_MEMBER_WORK_STATE_IDLE,			// ����
	EM_MEMBER_WORK_STATE_NORMAL,		// һ��
	EM_MEMBER_WORK_STATE_BUSY,			// æ
	EM_MEMBER_WORK_STATE_FULL,			// ��Ա
};

struct MemberInfo
{
	std::string		name;				// ����
	int				memberId;			// ��Աid
	EmMemberState	state;				// ״̬
	EmMemberWorkState workState;		// ����״̬
	time_t			reportTime;			// ����ϱ�ʱ��
};

class GameInfo
{
	std::string m_strName;		// ��Ϸ��������
	long long m_nAreaId;		// ����id
	int m_nOnline;				// ��ǰ��������
	EmMemberState m_eState;		// �����״̬
	int m_nCheckInterVal;		// ���߼��ʱ��
	// ��Ա�������б�
	std::map<int, MemberInfo> m_mapMembers;

	mutable FsLocker	m_gameLock;
public:
	GameInfo() : m_nAreaId(0), m_nOnline(0), m_nCheckInterVal(60), m_eState(EM_MEMBER_STATE_CLOSED)
	{

	}

public:
	const std::string& GetName() const;
	void SetName(const char* pszName);
	void SetName(const std::string& strName);

	long long GetAreaId() const;
	void SetAreaId(long long nAreaId);

	int GetOnlineCount() const;
	void SetOnlineCount(int nCount);

	int GetCheckVal() const;
	void SetCheckVal(int nValue);

	int GetMemberIds( std::vector<int>& refIds ) const;
	std::string GetMemberName(int nMemberId) const;
	EmMemberState GetMemberState(int nMemberId) const;
	EmMemberWorkState GetMemberWorkState(int nMemberId) const;

	void SetMember(const char* pszName, int nMemberId, EmMemberState nState, EmMemberWorkState workState);
	void DeadAll();

	void SetState(EmMemberState state);
	EmMemberState GetState() const;

	// ���·���״̬
	void Update();
};
#endif 