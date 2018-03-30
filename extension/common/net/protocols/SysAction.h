//--------------------------------------------------------------------
// �ļ���:		SysAction.h
// ��  ��:		ϵͳ��������
// ˵  ��:		
// ��������:		2016��5��30��
// ������:		���
// ��Ȩ����:		���ݾ����������޹�˾
//--------------------------------------------------------------------
#ifndef __SYS_ACTION_H__
#define __SYS_ACTION_H__

#include "action/IAction.h"
#include "utils/FsStream.h"

enum EM_SYS_ACTION
{
	SYS_ACTION_ENTER = 1,		// �������
	SYS_ACTION_EXIT = 2,		// �˳�
	SYS_ACTION_UNREGISTER = 3,	// δע��
	SYS_ACTION_RAW_DATA = 4,	// ԭʼ����
	SYS_ACTION_ERR_DATA = 5,	// �����
	SYS_ACTION_EMTPY = 6,		// �հ�

	SYS_ACTION_MAX = 64,
};


class SYSActionEnter : public EmptyAction<SYS_ACTION_ENTER>
{
	std::string m_strHost;
	unsigned short m_nPort;
	int m_ud;
	bool m_bAccept;

public:
	SYSActionEnter() : m_nPort(0), m_ud(0), m_bAccept(true)
	{
	}
	void SetPeerHost(const char* pHost)
	{
		if (NULL != pHost)
		{
			m_strHost = pHost;
		}
	}

	const char* GetPeerHost() const
	{
		return m_strHost.c_str();
	}

	void SetPeerPort(unsigned short nPort)
	{
		m_nPort = nPort;
	}

	unsigned short GetPeerPort() const
	{
		return m_nPort;
	}

	void SetUD(int ud)
	{
		m_ud = ud;
	}

	int GetUD() const
	{
		return m_ud;
	}

	void SetAccept(bool bAccept)
	{
		m_bAccept = bAccept;
	}

	bool GetAccept() const
	{
		return m_bAccept;
	}
};

class SYSActionExit : public EmptyAction<SYS_ACTION_EXIT>
{
	std::string m_strHost;
	unsigned short m_nPort;
	int m_ud;
	bool m_bAccept;
public:
	SYSActionExit() : m_nPort(0), m_ud(0), m_bAccept(true)
	{
	}
	void SetPeerHost(const char* pHost)
	{
		if (NULL != pHost)
		{
			m_strHost = pHost;
		}
	}

	const char* GetPeerHost() const
	{
		return m_strHost.c_str();
	}

	void SetPeerPort(unsigned short nPort)
	{
		m_nPort = nPort;
	}

	unsigned short GetPeerPort() const
	{
		return m_nPort;
	}
	void SetUD(int ud)
	{
		m_ud = ud;
	}

	int GetUD() const
	{
		return m_ud;
	}

	void SetAccept(bool bAccept)
	{
		m_bAccept = bAccept;
	}

	bool GetAccept() const
	{
		return m_bAccept;
	}
};

class SYSActionUnRegister : public Action<SYS_ACTION_UNREGISTER, BinaryMessage>
{
	int m_nRawActionId;
public:
	SYSActionUnRegister() : m_nRawActionId(0)
	{

	}
	void SetRawActionID(int nRawId)
	{
		m_nRawActionId = nRawId;
	}

	int GetRawActionID() const
	{
		return m_nRawActionId;
	}

};

class SYSActionRawData : public Action<SYS_ACTION_RAW_DATA, BinaryMessage>
{
	int m_nRawActionId;
public:
	SYSActionRawData() : m_nRawActionId(0)
	{

	}
	void SetRawActionID( int nRawId )
	{
		m_nRawActionId = nRawId;
	}

	int GetRawActionID() const
	{
		return m_nRawActionId;
	}
};

class SYSActionErrData : public Action<SYS_ACTION_ERR_DATA, BinaryMessage>
{
};

class SYSActionEmtpy : public Action<SYS_ACTION_EMTPY, BinaryMessage>
{

};
#endif // END __I_ACTION_H__