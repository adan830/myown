//--------------------------------------------------------------------
// �ļ���:		IDumpKernel.h
// ��  ��:		
// ˵  ��:		
// ��������:	2010��4��7��	
// ������:		½����
// ��Ȩ����:	������ţ�������޹�˾
//--------------------------------------------------------------------

#ifndef _SERVER_IDUMPKERNEL_H
#define _SERVER_IDUMPKERNEL_H

#include "../public/Macros.h"
#include "../public/IVarList.h"

// ����ת���߼��ӿڰ汾��
#define DUMP_LOGIC_MODULE_VERSION 0x3002

// ���ݿ��ֶ����Ͷ���
enum DUMP_FIELD_TYPE_ENUM
{
	DUMP_FIELD_TYPE_NULL,
	DUMP_FIELD_TYPE_INT,
	DUMP_FIELD_TYPE_FLOAT,
	DUMP_FIELD_TYPE_DOUBLE,
	DUMP_FIELD_TYPE_STRING,
	DUMP_FIELD_TYPE_WIDESTR,
	DUMP_FIELD_TYPE_BINARY,
	DUMP_FIELD_TYPE_DATE,
};

class IDumpKernel;

// ��ʱ������
typedef int (__cdecl* DUMP_TIMER_FUNC)(IDumpKernel* pDumpKernel, 
	void* pContext, int time) ;
	
// ����ת�����Ľӿ�

class IDumpKernel
{
public:
	virtual ~IDumpKernel() = 0;
	
	/// \brief ���Ҷ�ʱ��
	/// \param name ��ʱ������
	virtual bool FindTimer(const char* name, void* pContext) = 0;
	/// \brief ��Ӷ�ʱ��
	/// \param name ��ʱ������
	/// \param func ��ʱ������ָ��
	/// \param time ��ʱʱ��
	/// \param count ��ʱ����
	virtual bool AddTimer(const char* name, DUMP_TIMER_FUNC func, 
		void* pContext, int time, int count) = 0;
	/// \brief ɾ����ʱ��
	/// \param name ��ʱ������
	virtual bool RemoveTimer(const char* name, void* pContext) = 0;

	/// \brief ���ͶԿ�������Ļ�Ӧ��Ϣ
	/// \param args ��Ϣ����
	virtual bool SendResponse(const IVarList& args) = 0;

	/// \brief ���д洢����
	/// \param access_name ����������
	/// \param stored_proc_name �洢������
	/// \param args ������(string)����������(int)���Ƿ񷵻ز���(int)������ֵ
	virtual bool RunStoredProc(const char* access_name, 
		const char* stored_proc_name, const IVarList& args) = 0;

	/// \brief ִ��SQL���
	/// \param access_name ����������
	/// \param sql ����
	virtual bool ExecuteSql(const char* access_name, const wchar_t* sql) = 0;

	/// \brief ������ݱ�������Ϣ
	/// \param access_name ����������
	/// \param table_name ���ݱ���
	/// \param time_field ʱ���ֶ���
	virtual bool ReadTableInfo(const char* access_name, 
		const char* table_name, const char* time_field) = 0; 

	/// \brief ������ȡ���ݱ��еļ�¼��Ϣ
	/// \param access_name ����������
	/// \param table_name ���ݱ���
	/// \param time_field ʱ���ֶ���
	/// \param key_field �ؼ��ֶ���
	/// \param beg_time ��ʼʱ��
	/// \param end_time ����ʱ��
	virtual bool ReadBatch(const char* access_name, const char* table_name, 
		const char* time_field, const char* key_field, double beg_time, 
		double end_time, int max_key_num) = 0;

	/// \brief �����ݿ��ȡ�����¼
	/// \param access_name ����������
	/// \param table_name ���ݱ���
	/// \param key_field �ؼ��ֶ���
	/// \param keys �ؼ�ֵ��
	virtual bool ReadRecord(const char* access_name, const char* table_name,
		const char* key_field, const IVarList& keys) = 0;

	/// \brief �����ݿ�д���¼
	/// \param access_name ����������
	/// \param table_name ���ݱ���
	/// \param key_field �ؼ��ֶ���
	/// \param key_value �ؼ�ֵ
	/// \param field_num �ֶ�����
	/// \param field_types �ֶ�����
	/// \param field_values �ֶ�ֵ
	virtual bool WriteRecord(const char* access_name, const char* table_name, 
		const char* key_field, const wchar_t* key_value, int field_num, 
		int field_types[], const char* field_names[], 
		const IVarList& field_values) = 0;

	virtual bool QuerySql(const char* access_name, const char* table_name, const char* key_field, 
		const char* condition, int nMaxKeyNums) = 0;

	virtual bool SendServerMsg(int index, const IVarList& msg) = 0;

	virtual bool GetTableSchem(const char* access_name, const char* table_name) = 0;
	
	/// \brief ���������Դ�ļ�·��
	virtual const char* GetResourcePath() = 0;

	virtual bool IsAccessIdle() = 0;

	virtual bool Quit() = 0;
};

inline IDumpKernel::~IDumpKernel() {}

#endif // _SERVER_IDUMPKERNEL_H

