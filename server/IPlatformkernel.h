//--------------------------------------------------------------------
// �ļ���:		IPlatformKernel.h
// ��  ��:		ƽ̨�߼����Ľӿ�
// ˵  ��:		
// ��������:	2013��11��26��	
// ������:		���н�
// ��Ȩ����:	������ţ�������޹�˾
//--------------------------------------------------------------------

#ifndef _SERVER_IPLATFORMKERNEL_H
#define _SERVER_IPLATFORMKERNEL_H

#include "../public/Macros.h"
#include "../public/IVarList.h"
//#include "IPlatformData.h"

// ƽ̨�����߼��ӿڰ汾��
#define PLATFORM_LOGIC_MODULE_VERSION 0x1001

class IPlatformKernel;
class IRecord;

// ��ʱ������
typedef void (*Platform_TIMER_FUNC)(void* context, int timer_id, float seconds) ;


// sqlite �����
typedef int (*sqlite3_callback)(void*, int, char**, char**);

typedef int (*sqlite3_rs_callback)(void*, int, int, char**);


// ƽ̨�߼����Ľӿ�

class IPlatformKernel
{
public:

	virtual ~IPlatformKernel() = 0;

	/// \brief �����Դ�ļ�·��
	virtual const char* GetResourcePath() = 0;

	/// \brief sqlite ���� �޷���ֵ����
	/// \param pSql sql���
	virtual bool Excute(const char* pSql)	= 0;

	/// \brief sqlite �����з���ֵ����
	/// \param pSql	  sql���
	/// \param pback  �ص�����
	/// \param pdata  
	/// \param errmsg ������Ϣ
	virtual bool Excute(const char* pSql, sqlite3_callback pback, void* pdata, char** perrmsg) =0;	

	/// \brief sqlite �����з��ؽ����
	/// \param pSql	  sql���
	/// \param pback  �ص�����
	/// \param pdata  
	/// \param errmsg ������Ϣ
	virtual bool Excute(const char* pSql, sqlite3_rs_callback pback, void* pdata, char**perrmsg) = 0;

	/// \brief sqlite ���� 
	/// \param pSql sql���
	/// \return IRecord��ע�� IRecord���ݶ���string ���ͣ�
	virtual IRecord* ExcuteEx(const char* pSql) = 0;

	/// \brief ��ȡһ�������
	virtual int GetRoomId() = 0;

	/// \brief ����ս������
	/// \param room_id  ����id
	/// \param map_id   ������ͼ
	/// \param role_uid ��ɫid�б�
	/// \param args		���Ӳ���
	/// \param bPoll	�Ƿ�ʹ����ѯ�ķ�ʽ��Ĭ������ײ�ʹ�ö�̬���ط�ʽ���䣩
	virtual bool EntryRoom(int room_id, int map_id, const char* role_uid, const IVarList & args, bool bPoll = false) = 0;

	/// \brief �����Զ�����Ϣ�� member
	/// \param role_uid ��ɫuid
	/// \param args ������ ��player��OnCustomPlatform �ص���
	virtual bool SendCustomToMember(const char* role_uid, const IVarList& args) = 0;

	/// \brief ������Ϣ��ָ�������� 
	/// \param server_id  ָ���ķ�����id ��-1 �������з�������
	/// \param args ������ ����һ������ OnCommand �ص���
	virtual bool SendCustomToMember(int server_id, const IVarList& args) = 0;

	/// \brief ������Ϣ��ָ����������uid��ɫ 
	/// \param world_id  ָ�������������id
	/// \param role_uid ָ���Ľ�ɫuid
	/// \param args ������ ��GamePlayer�� OnCustomPlatform �ص�����world_id��role_uid���߼��Լ���֤ƥ�䣬���߲���֤���յ��Զ�����Ϣ
	virtual bool SendCustomToMember(int world_id, const char* role_uid,const IVarList& args) = 0;

	/// \brief �����Զ�����Ϣ�� room
	/// \param room_id ����id
	/// \param role_uid ָ���Ľ�ɫuid
	/// \param args ��������GamePlayer�� OnCustomPlatform �ص���
	virtual bool SendCustomToRoom(int room_id, const char* role_uid, const IVarList& args) = 0;

	/// \brief �����Զ�����Ϣ�� room
	/// \param room_id ����id
	/// \param args �������������� OnCommand �ص���
	virtual bool SendCustomToRoom(int room_id, const IVarList& args) = 0;

	/// \brief ���÷���ʱ��
	/// \param room_id ����id
	/// \param nSeconds ���������
	virtual bool SetRoomContinueTime(int room_id, int nSeconds) = 0;

	/// \brief �����������
	/// \param func ����������
	/// \param time ����ʱ��(������� 0.005��)
	/// ע�����ӵĺ��������ǹ̶��ģ�����
	/// int func(void* context, int timer_id, float seconds) 
	/// time�ĵ�λΪ��
	/// \ret �������������� ����� -1�� ˵���������ʧ��
	virtual int AddHeartBeat(Platform_TIMER_FUNC func, float seconds) = 0;

	/// \brief ɾ������
	/// \param time_id  ��������
	virtual bool RemoveHeartBeat(int timer_id) = 0;

	//\brief  ���俪������
	virtual bool SetRoomMaxPlayers(int room_id, int nAmount)  = 0;
	
	// \brief ��ȡworld �������ĸ���
	virtual int  GetWorldNumber()   = 0;
	
	// \brief ��ȡս���������б�����server_id
	virtual bool  GetBattleServerList(IVarList& args)   = 0;

	// \brief ��ȡս������������ķ����б�
	// \param room_server_id room������id
	// \param res ���صķ�����б�
	virtual int GetBattleRoomList(int room_server_id, IVarList& res) = 0;

	// \brief ��ȡս�����ķ�����
	virtual int  GetBattleServerRooms(int server_id)   = 0;

	// \brief ��ȡ��������Ҹ���
	virtual int  GetRoomPlayers(int room_id)   = 0;

	// \brief ��ȡս��������Ҹ���
	virtual int  GetBattleServerPlayers(int server_id)   = 0;

	/// \brief ������Ϣ�����ӷ�����
	/// \param index ���ӷ���������
	/// \param room_id ����id
	/// \param msg ��Ϣ�����б�
	virtual bool SendExtraMessage(int index, const IVarList& msg)=0; 	

	/// \brief �����ϵͳLOG�ļ�
	/// \param info ��Ϣ����
	virtual void Trace(const char* info) = 0;
	/// \brief ���������̨��Ļ
	/// \param info ��Ϣ����
	virtual void Echo(const char* info) = 0;

};

inline IPlatformKernel::~IPlatformKernel() {}

#endif // _SERVER_IPlatformKERNEL_H

