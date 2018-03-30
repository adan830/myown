//-------------------------------------------------------------------
// �� �� ����IDumpManager.h
// ��    �ݣ�
// ˵    ����
// �������ڣ�2013��12��12��
// �� �� �ˣ�Liangxk
// ��Ȩ���У�������ţ�������޹�˾
//-------------------------------------------------------------------


#if !defined __IDUMPMANAGER_H__
#define __IDUMPMANAGER_H__

#include "idumpentity.h"
#include <windows.h>
#include "dump_version.h"

//#if defined _DEBUG
/// ����ѭ�����Ժ�
//#define START_LOOP_TEST
//#endif // _DEBUG

// ��������ȡ��ɫ����
#define READ_ROLE_MAX_ONCE 200 

// access�����еĽ�ɫ�������ֵ
#define WAIT_READ_ROLE_COUNT 0x4000

enum entity_status
{
	ENTITY_STATUS_UNKNOWN,
	ENTITY_STATUS_STARTED,
	ENTITY_STATUS_RUNNING,
	ENTITY_STATUS_STOPED,
};



class IDumpManager : public IDumpEntity
{
public:
	virtual bool Init(const IVarList& args) = 0;
	virtual bool Shut(/* = 0 */) = 0;
	virtual bool LoadConfig(const char* path) = 0;
	

    //-----------------------------------------------------------
    /// \brief       ����������Ϣ
    /// \param       [in] name		��������
    /// \param       [in] index		��������
    /// \param       [in] serial	�������к�
    /// \param       [in] msg		��Ϣ����
    /// \note        
	virtual bool SendCustomMsg(const char* name, int index, int serial, const IVarList& msg) = 0;

    //-----------------------------------------------------------
    /// \brief       ע�����������Ϣ��ģ��
    /// \param       [in] strConnName	��������
    /// \param       [in] pEntity		ʵ�����
    /// \note        
	virtual bool AddModuleConnect(const char* strConnName, IDumpEntity* pEntity) = 0;

	/// ����
	virtual void LogWarning(const char* module, const char* info, ...) = 0;

	/// ���� �쳣
	virtual void LogError(const char* module, const char* info, ...) = 0;

	/// ����
	virtual void LogPerf(const char* module, const char* info, ...) = 0;

	/// ��� 
	virtual void LogTrace(const char* module, const char* info, ...) = 0;
	virtual void LogEcho(const char* module, const char* info, ...) = 0;
	virtual void LogEchoTrace(const char* module, const char* info, ...) = 0;


    //-----------------------------------------------------------
    /// \brief       ��������ʼ����ȡ�����ƶ���
    /// \param       [in] module		ģ��
    /// \param       [in] access_name	����������
    /// \param       [in] strName		��������
    /// \param       [in] table			�������������ݿ����
    /// \param       [in] key			�������
	/// \param       [in] query_fields	��Ҫ��ѯ�������ֶ� ��:'role,save_data'
    /// \param       [in] isRole		true��ʾ��ѯ��ɫ, false��ʾ��ѯ����͹�������
    /// \note        
	virtual bool InitReadBinQueue(PERSISTID module, const char* access_name, const char* strName, 
		const char* table, const char* key, const char* query_fields, bool isRole) = 0;

    //-----------------------------------------------------------
    /// \brief       �Ƴ������Ķ�ȡ�����ƶ���
    /// \param       [in] strName
    /// \note        
	virtual bool RemoveRequestRoleList(const char* strName) = 0;

    //-----------------------------------------------------------
    /// \brief       ���ȡ�����ƶ�����ѹ���ѯ������
    /// \param       [in] strListName	��������
    /// \param       [in] name_list		�����б�
    /// \param       [in] begin_index	name_list��ʼ�±�
    /// \param       [in] end_index		name_list�����±�
    /// \note        
	virtual bool PushRoleName(const char* strListName, const IVarList& name_list, int begin_index, int end_index) = 0;


    //-----------------------------------------------------------
    /// \brief       ע��ִ��sql������Ļص�����
    /// \param       [in] module		ģ��
    /// \param       [in] cmd			sql����id
    /// \param       [in] access_name	����������
    /// \param       [in] callback		�ص���������
    /// \note        
	virtual bool RegistExeSqlCallBack(PERSISTID module, int cmd, const char* access_name, const char* callback) = 0;

    //-----------------------------------------------------------
    /// \brief       ִ��sql���, ������sqlִ�н��
    /// \param       [in] cmd			sql����id
    /// \param       [in] sql			sql���
    /// \note        
	virtual bool PushExecuteSql(int cmd, const char* sql) = 0;

    //-----------------------------------------------------------
    /// \brief       ִ�в�ѯ���, ����sqlִ�н��
    /// \param       [in] cmd			sql����id
    /// \param       [in] sql			sql���
    /// \note        
	virtual bool PushQuerySql(int cmd, const char* sql) = 0;

    //-----------------------------------------------------------
    /// \brief       ִ��д������, sql��佫��������еȴ�ִ��
    /// \param       [in] cmd			sql����id
	/// \param       [in] sql			��Ҫ���µ�sql���� ��:"`key`='value',...,`keyn`='valuen'"
    /// \param       [in] key			��������
    /// \param       [in] value			����ƥ���ֵ ����where����
    /// \param       [in] table			����
    /// \note        
	virtual bool PushWriteSql(int cmd, const char* sql, const char* key, const char* value, const char* table) = 0;

    //-----------------------------------------------------------
    /// \brief       ִ��д�����, ��sql���������, ֱ�ӷ���������, �����������æ, �򷵻�false
    /// \param       [in] cmd
    /// \param       [in] sql
    /// \param       [in] key
    /// \param       [in] value
    /// \param       [in] table
    /// \note        
	virtual bool PushWriteSqlDirect(int cmd, const char* sql, const char* key, const char* value, const char* table) = 0;


	virtual bool OnTimer() = 0;

	/// ����֡ѭ��

    //-----------------------------------------------------------
    /// \brief       ע��ģ���֡ѭ��
    /// \param       [in] module		ģ��
	/// \param       [in] ms			���ʱ��(����)
	/// \note        ģ����Ҫʵ���麯�� virtual void OnUserLoop()
	virtual bool AddExecuteCallback(PERSISTID module, int ms = 0) = 0;

    //-----------------------------------------------------------
    /// \brief       �Ƴ�ģ���֡ѭ��
    /// \param       [in] module
    /// \param       [in] strFunc
    /// \note        
	virtual bool RemoveExecuteCallback(PERSISTID module, const char* strFunc) = 0;

	virtual bool Start() = 0;
	virtual bool Stop() = 0;

	virtual void RunCommand(const char* cmd) = 0;

	const char* GetVersion() const { return CURRENT_TOOLSET_VERSION; }
};

#define DPLOG_WARNNING(mgr,s1,...) (mgr)->LogWarning(__FUNCTION__,s1,__VA_ARGS__)
#define DPLOG_PERF(mgr,s1,...) (mgr)->LogPerf(__FUNCTION__,s1,__VA_ARGS__)
#define DPLOG_ERROR(mgr,s1,...) (mgr)->LogError(__FUNCTION__,s1,__VA_ARGS__)
#define DPLOG_TRACE(mgr,s1,...) (mgr)->LogTrace(__FUNCTION__,s1,__VA_ARGS__)
#define DPLOG_ECHO(mgr,s1,...) (mgr)->LogEcho(__FUNCTION__,s1,__VA_ARGS__)
#define DPLOG_ECHOTRACE(mgr,s1,...) (mgr)->LogEchoTrace(__FUNCTION__,s1,__VA_ARGS__)
#endif // __IDUMPMANAGER_H__
