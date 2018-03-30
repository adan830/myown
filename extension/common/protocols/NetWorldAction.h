//--------------------------------------------------------------------
// �ļ���:		NetWorldAction.h
// ��  ��:		�����������ͨ�ŵ�Э��
// ˵  ��:		
// ��������:		2016��06��08��
// ������:		���
// ��Ȩ����:		���ݾ����������޹�˾
//--------------------------------------------------------------------
#ifndef __NET_WORLD_AND_CHARGE_ACTION_H__
#define __NET_WORLD_AND_CHARGE_ACTION_H__

#include "action/INetAction.h"
#include "NetVarListAction.h"
#include <string>
#include "public/VarList.h"
#include "MsgEncoder.h"
#include "NetChargeCustom.h"

// �Ʒѵ�½ �ı���ʽ: ��������Ϣ($s_sceneid_connid_serial_����ֵ_uid) + Э����("register" or "login"....) + ����
// ��������Ϣ��һ����������
template<typename T>
class NetWorldAction : public NetVarListAction<0, T>
{
public:
	std::string coninfo;	// ���ӷ�������Ϣ( $s_sceneid_connid_serial_����ֵ_uid )
	std::string protocol;

	virtual bool LoadFromVarList(const IVarList& args)
	{
		if (args.GetCount() < 2)
		{
			return false;
		}

		if (!SafeLoadString(coninfo, args, 0))
		{
			return false;
		}

		if (!SafeLoadString(protocol, args, 1))
		{
			return false;
		}
		return m_req.LoadFromVarList( args );
	}

	virtual bool WriteToVarList(IVarList& args)
	{
		args.AddString(coninfo.c_str());
		const char* key = ActionKey();
		if (NULL != key)
		{
			args.AddString( key );
		}
		else
		{
			args.AddString(protocol.c_str());
		}
		return m_req.WriteToVarList( args );
	}
};

template<typename REQ, typename ACK>
class NetWorldReqAction : public NetWorldAction<REQ>
{
public:
	NetWorldReqAction() 
	{
	}

	virtual IResponse* Response()
	{
		return &m_ack;
	}

	virtual bool LoadFromVarList( const IVarList& args )
	{
		if (args.GetCount() < 2)
		{
			return false;
		}

		if (!SafeLoadString(coninfo, args, 0))
		{
			return false;
		}

		if (!SafeLoadString(protocol, args, 1))
		{
			return false;
		}

		if (IsRequest())
		{
			return m_req.LoadFromVarList(args);
		}

		return m_ack.LoadFromVarList(args);
	}

	virtual bool WriteToVarList( IVarList& args )
	{
		args.AddString(coninfo.c_str());
		args.AddString(protocol.c_str());

		if (IsRequest())
		{
			return m_req.WriteToVarList(args);
		}

		return m_ack.WriteToVarList(args);
	}

protected:
	ACK		m_ack;
};

typedef NetVarListRequest NetWorldRequest;
typedef NetVarListResponse NetWorldResponse;

//--------------------------------------------------------------------
// ������ע��
//--------------------------------------------------------------------
class NetWorldMessage_ServerRgister : public NetWorldRequest
{
public:
	int  value_of_2;		// δ֪���� 1
	int  value_of_3;		// δ֪���� 7440005

	std::string srvinfo;	// ��������Ϣ server_7440005
	std::string enc;		// md5��		cb10de5f8d730ec666dde710665cf07

	int	value_of_6;			// δ֪����

	virtual bool LoadFromVarList(const IVarList& args)
	{
		if (args.GetCount() < 7)
		{
			return false;
		}

		value_of_2 = args.IntVal(2);
		value_of_3 = args.IntVal(3);
		value_of_6 = args.IntVal(6);

		if (!SafeLoadString(srvinfo, args, 4))
		{
			return false;
		}

		if (!SafeLoadString(enc, args, 5))
		{
			return false;
		}
		return true;
	}

	virtual bool WriteToVarList(IVarList& args)
	{
		args.AddInt(value_of_2);
		args.AddInt(value_of_3);
		args.AddString(srvinfo.c_str());
		args.AddString(enc.c_str());
		args.AddInt(value_of_6);
		return true;
	}
};
class NetWorldAction_ServerRegister : public NetWorldAction<NetWorldMessage_ServerRgister>
{
public:

	virtual const char* ActionKey() const
	{
		return "register";
	}
};



//--------------------------------------------------------------------
// ��½
//--------------------------------------------------------------------
class NetWorldMessage_Login_Req : public NetWorldRequest
{
public:
	int			loginType;			// ��½���� index:2
	std::string name;				// �û��� index:3
	std::string token;				// token�� index:4
	std::string ip;					// �ͻ���ip index:5
	int			port;				// �ͻ������Ӷ˿� index:6
	std::string loginString;		// ��½��(��Ҫ����) index:7
	int			clientType;			// �ͻ������� index:8

	virtual bool LoadFromVarList(const IVarList& args)
	{
		if (args.GetCount() < 9)
		{
			return false;
		}

		loginType = args.IntVal(2);

		if (!SafeLoadString(name, args, 3))
		{
			return false;
		}

		if (!SafeLoadString(token, args, 4))
		{
			return false;
		}

		if (!SafeLoadString(ip, args, 5))
		{
			return false;
		}

		port = args.IntVal(6);
		if (!SafeLoadString(loginString, args, 7))
		{
			return false;
		}
		clientType = args.IntVal(8);
		return true;

	}
	virtual bool WriteToVarList(IVarList& args)
	{
		args.AddInt(loginType);
		args.AddString(name.c_str());
		args.AddString(token.c_str());
		args.AddString(ip.c_str());
		args.AddInt(port);
		args.AddString(loginString.c_str());
		args.AddInt(clientType);
		return true;
	}
};

class NetWorldMessage_Login_Ack: public NetWorldResponse
{
public:
	int			acc_type;			// �˺����� index:2
	std::string name;				// �ʺ� index:3
	int			result;				// ��� index:4
	int			acc_id;				// �Ʒ�id index:5
	std::string logon_id;			// �˴ε�¼�����кţ�����logout��Ϣʱ���ظ��Ʒ�
	std::string user_name;			// ��ʱû��
	int			gm_level;			// gm�ȼ�
	std::string passport;			// ��ʱû��
	int			points;				// �˺Ŷ�Ӧ�ļƷѵ���
	double		acc_limit;			// �˺���Чʱ�䣬�¿��շ�ģʽʹ��
	int			is_free;			// �Ƿ�����û����¿��շ�ģʽʹ��
	std::string acc_info;			// �˺������չ��Ϣ��������ʱ�䡢VIP����
	int			login_type;			// ��¼���ͣ��ͼƷ�����Լ���ֶ�1�����������¼��5��VIP��¼�ȣ�
	std::string	vaild_string;		// һ���������֤���������������¼�����������������logincache��������
	int			issur_id;			// �����̱��

	NetWorldMessage_Login_Ack()
	{
		acc_type = 0;
		result = 0;
		acc_id = 0;
		gm_level = 0;
		points = 0;
		acc_limit = 0.0f;
		is_free = 0;
		login_type = 0;
		issur_id = 0;
	}

	virtual bool LoadFromVarList(const IVarList& args)
	{
		if (args.GetCount() < 17)
		{
			return false;
		}

		int nIndex = 2;
		acc_type = args.IntVal( nIndex++ );
		if (!SafeLoadString(name, args, nIndex++))
		{
			return false;
		}

		result = args.IntVal(nIndex++);
		acc_id = args.IntVal(nIndex++);
		if (!SafeLoadString(logon_id, args, nIndex++))
		{
			return false;
		}		
		if (!SafeLoadString(user_name, args, nIndex++))
		{
			return false;
		}
		gm_level = args.IntVal(nIndex++);
		if (!SafeLoadString(passport, args, nIndex++))
		{
			return false;
		}
		points = args.IntVal(nIndex++);
		acc_limit = args.DoubleVal(nIndex++);
		is_free = args.IntVal(nIndex++);
		if (!SafeLoadString(acc_info, args, nIndex++))
		{
			return false;
		}
		login_type = args.IntVal(nIndex++);
		if (!SafeLoadString(vaild_string, args, nIndex++))
		{
			return false;
		}
		issur_id = args.IntVal(nIndex++);

		return true;

	}
	virtual bool WriteToVarList(IVarList& args)
	{
		args.AddInt(acc_type);
		args.AddString(name.c_str());
		args.AddInt(result);
		args.AddInt(acc_id);
		args.AddString(logon_id.c_str());
		args.AddString(user_name.c_str());
		args.AddInt(gm_level);
		args.AddString(passport.c_str());
		args.AddInt(points);
		args.AddDouble(acc_limit);
		args.AddInt(is_free);
		args.AddString(acc_info.c_str());
		args.AddInt(login_type);
		args.AddString(vaild_string.c_str());
		args.AddInt(issur_id);
		return true;
	}
};

class NetWorldAction_Login : public NetWorldReqAction<NetWorldMessage_Login_Req, NetWorldMessage_Login_Ack>
{
public:

	virtual const char* ActionKey() const
	{
		return "login";
	}
};


//--------------------------------------------------------------------
// �˳�
//--------------------------------------------------------------------
class NetWorldMessage_Logout : public NetWorldRequest
{
public:
	int			value_of_1;			// δ֪����1 index:2
	std::string value_of_2;			// δ֪����2 index:3
	int			value_of_3;			// δ֪����3 index:4
	std::string name;				// �˺� index:5

	virtual bool LoadFromVarList(const IVarList& args)
	{
		if (args.GetCount() < 6)
		{
			return false;
		}

		value_of_1 = args.IntVal(2);

		if (!SafeLoadString(value_of_2, args, 3))
		{
			return false;
		}

		value_of_3 = args.IntVal(4);

		if (!SafeLoadString(name, args, 5))
		{
			return false;
		}
		return true;

	}
	virtual bool WriteToVarList(IVarList& args)
	{
		args.AddInt(value_of_1);
		args.AddString(value_of_2.c_str());
		args.AddInt(value_of_3);
		args.AddString(name.c_str());
		return true;
	}
};

class NetWorldAction_Logout : public NetWorldAction<NetWorldMessage_Logout>
{
public:

	virtual const char* ActionKey() const
	{
		return "logout";
	}
};

//--------------------------------------------------------------------
// ��������
//--------------------------------------------------------------------
class NetWorldMessage_Keep : public NetWorldRequest
{
public:
	virtual bool LoadFromVarList(const IVarList& args)
	{
		return true;
	}

	virtual bool WriteToVarList(IVarList& args)
	{
		return true;
	}
};
class NetWorldAction_Keep : public NetWorldAction<NetWorldMessage_Keep>
{
public:

	virtual const char* ActionKey() const
	{
		return "keep";
	}
};

//--------------------------------------------------------------------
// �Զ���Э��
//--------------------------------------------------------------------
class NetWorldMessage_Custom : public NetWorldRequest
{
public:
	virtual bool LoadFromVarList(const IVarList& args)
	{
		cargs.Clear();
		if (args.GetCount() < 2)
			return false;

		cargs.Append(args, 2, args.GetCount() - 2);
		return true;
	}

	virtual bool WriteToVarList(IVarList& args)
	{
		args.Append(cargs, 0, cargs.GetCount());
		return true;
	}

	CVarList cargs;
};
class NetWorldAction_Custom : public NetWorldAction<NetWorldMessage_Custom>
{
public:
	virtual const char* ActionKey() const
	{
		return "custom";
	}
};


/*
// ��Ϸ׼������
class NetWorldMessage_GameReady : public NetWorldRequest
{
public:
	bool IsReady;
	NetWorldMessage_GameReady() : IsReady(0)
	{

	}
	virtual bool LoadFromVarList(const IVarList& args)
	{
		if (args.GetCount() < 3)
		{
			return false;
		}

		IsReady = args.IntVal(2) != 0;
		return true;
	}

	virtual bool WriteToVarList(IVarList& args)
	{
		args.AddInt(IsReady ? 1 : 0);
		return true;
	}
};
class NetWorldAction_GameReady : public NetWorldAction<NetWorldMessage_GameReady>
{
public:

	virtual const char* ActionKey() const
	{
		return "game_ready";
	}
};

// ������Ϸ״̬LOGIN->GAME
class NetWorldMessage_GameState : public NetWorldRequest
{
public:
	virtual bool LoadFromVarList(const IVarList& args)
	{
		return true;
	}

	virtual bool WriteToVarList(IVarList& args)
	{
		return true;
	}
};
class NetWorldAction_GameState : public NetWorldAction<NetWorldMessage_GameState>
{
public:

	virtual const char* ActionKey() const
	{
		return "game_state";
	}
};
*/
//--------------------------------------------------------------------
// δ֪Э��
//--------------------------------------------------------------------
typedef NetVarListMessage_Unkown NetWorldMessage_Unkown;
typedef NetVarListAction_Unknown NetWorldAction_Unknown;


// ��Ϣ�����
class NetWorldMessagePacker : public NetVarListPacker
{
public:
	NetWorldMessagePacker() : NetVarListPacker(true, true)
	{
	}

protected:
	virtual INetVarListActionProtocol* CreateAction(const char* key) const
	{
		INetVarListActionProtocol* pAction = NULL;
		if (strcmp(key, "login") == 0)
		{
			pAction = new NetWorldAction_Login();
		}
		else if (strcmp(key, "keep") == 0)
		{
			pAction = new NetWorldAction_Keep();
		}
		else if (strcmp(key, "logout") == 0)
		{
			pAction = new NetWorldAction_Logout();
		}
		else if (strcmp(key, "register") == 0)
		{
			pAction = new NetWorldAction_ServerRegister();
		}
		else if (strcmp(key, "custom") == 0)
		{
			pAction = new NetWorldAction_Custom();
		}

		return pAction;
	}
};

#endif // END __NET_WORLD_AND_CHARGE_ACTION_H__