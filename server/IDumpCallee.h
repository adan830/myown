//--------------------------------------------------------------------
// �ļ���:		IDumpCallee.h
// ��  ��:		
// ˵  ��:		
// ��������:	2010��4��7��
// ������:		½����
// ��Ȩ����:	������ţ�������޹�˾
//--------------------------------------------------------------------

#ifndef _SERVER_IDUMPCALLEE_H
#define _SERVER_IDUMPCALLEE_H

#include "../public/Macros.h"
#include "../public/IVarList.h"

// ����ת���ص��ӿ�

class IDumpKernel;

class IDumpCallee
{
public:
	virtual ~IDumpCallee() = 0;

	/// \brief ����ת������������
	/// \param pDumpKernel ����ָ�룬��ͬ
	virtual int OnDumperStart(IDumpKernel* pDumpKernel) = 0;

	/// \brief ������ÿ֡����
	/// \param frame_ticks ��֡�ĺ�����
	virtual int OnFrameExecute(IDumpKernel* pDumpKernel, int frame_ticks) = 0;

	/// \brief �������ر�֪ͨ
	virtual int OnServerClose(IDumpKernel* pDumpKernel) = 0;

	/// \brief �յ��ϲ�Ŀ�������
	/// \param args �������
	virtual int OnReceiveCommand(IDumpKernel* pDumpKernel, 
		const IVarList& args) = 0;

	/// \brief �������д洢���̽��
	/// \param access_name ����������
	/// \param stored_proc_name �洢������
	/// \param succeed �Ƿ����гɹ�
	/// \param result ���ؽ������
	virtual int OnRunStoredProc(IDumpKernel* pDumpKernel,
		const char* access_name, const char* stored_proc_name, 
		int succeed, const IVarList& result) = 0;

	/// \brief �������ݱ�������Ϣ
	/// \param access_name ����������
	/// \param table_name ���ݱ���
	/// \param record_num ��¼����
	/// \param first_write_time ����д��ʱ��
	/// \param last_write_time ���д��ʱ��
	virtual int OnReadTableInfo(IDumpKernel* pDumpKernel,
		const char* access_name, const char* table_name, int succeed,
		int record_num, double first_write_time, double last_write_time) = 0;

	/// \brief ����������ȡ�Ľ����¼����
	/// \param access_name ����������
	/// \param table_name ���ݱ���
	/// \param beg_time ��ʼʱ��
	/// \param end_time ����ʱ��
	/// \param last_time ʵ�ʷ��ؼ�¼�����ʱ��
	/// \param record_num ��¼����
	/// \param key_field �ؼ��ֶ���
	/// \param keys ��¼�Ĺؼ���
	virtual int OnReadBatch(IDumpKernel* pDumpKernel, 
		const char* access_name, const char* table_name, int succeed,
		double beg_time, double end_time, double last_time, int record_num, 
		const char* key_field, const IVarList& keys) = 0;

	/// \brief ��ȡ���ݿ��¼
	/// \param access_name ����������
	/// \param table_name ���ݱ���
	/// \param key_field �ؼ��ֶ���
	/// \param key_total һ�ζ�ȡ�ļ�¼����
	/// \param key_index ���ζ�ȡ��¼��˳��ֵ
	/// \param field_num �ֶ�����
	/// \param field_type �ֶ�����
	/// \param field_values �ֶ�����
	virtual int OnReadRecord(IDumpKernel* pDumpKernel, 
		const char* access_name, const char* table_name, int succeed, 
		const char* key_field, int key_total, int key_index, int field_num, 
		int field_type[], const char* field_names[], 
		const IVarList& field_values) = 0;

	/// \brief ����д�����ݼ�¼���
	/// \param access_name ����������
	/// \param table_name ���ݱ���
	/// \param key_field �ؼ��ֶ���
	/// \param key_value �ؼ�ֵ
	/// \param succeed �Ƿ�ɹ�
	virtual int OnWriteRecord(IDumpKernel* pDumpKernel, 
		const char* access_name, const char* table_name, int succeed,
		const char* key_field, const wchar_t* key_value) = 0;

	virtual int OnQuerySql(IDumpKernel* pDumpKernel, const char* access_name, 
		const char* table_name, int succeed, const char* key_field, int key_num, 
		const IVarList& keys) = 0;

	virtual int OnTableColNames(IDumpKernel* pDumpKernel, const char* access_name,
		const char* table_name, int succeed, int key_nums, const IVarList& keys) = 0;

	virtual int OnRecvGameServerMsg(IDumpKernel* pDumpKernel, int index, const IVarList& msg) = 0;

	virtual int OnServerConnected(IDumpKernel* pDumpKernel, int index) = 0;
};

inline IDumpCallee::~IDumpCallee() {}

#endif // _SERVER_IDUMPCALLEE_H

