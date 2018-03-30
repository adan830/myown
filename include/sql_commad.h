//-------------------------------------------------------------------
// �� �� ����sql_commad.h
// ��    �ݣ�
// ˵    ����
// �������ڣ�2013��12��13��
// �� �� �ˣ�Liangxk
// ��Ȩ���У�������ţ�������޹�˾
//-------------------------------------------------------------------


#if !defined __SQL_COMMAD_H__
#define __SQL_COMMAD_H__


// 
enum sql_command_type
{
	SQL_COMMAND_UPDATE_ROLE_DUMP = 1001,/// ���½�ɫת����
	SQL_COMMAND_DUMP_TABLE_COLS,		/// ��ȡ��ɫת��������Ϣ
	SQL_COMMAND_GET_LAST_UPDATE_DUMP,	/// ��ѯ���һ�θ��½�ɫת�����ʱ��
	SQL_COMMAND_GET_DUMP_LIST,			/// ���ת�����������������

	/// DumpTool_RoleInfo
	SQL_COMMAND_GET_ROLE_LIST,			/// ��ȡ�������������н�ɫ������
	SQL_COMMAND_READ_ROLE_INFOS,		/// ��ȡ�ض���ɫ����������

	/// DumpTool_Config
	SQL_COMMAND_UPDATE_CONFIG,			/// ����nx_config��
	SQL_COMMAND_UPDATE_LOG,				/// ������־��Ϣ
	
	/// DumpTool_Guild
	SQL_COMMAND_QUERY_GUILD,			/// ��ѯ���а�������
	SQL_COMMAND_WRITE_GUILD,			/// ������Ϣд�����ݿ�
	SQL_COMMAND_CLEAR_GUILD,

	/// DumpTool_Rank
	SQL_COMMAND_WRITE_RANK,				/// д���а�
	SQL_COMMAND_CLEAR_RANK,				/// ������а�

	/// DumpTool_BackupRole
	SQL_COMMAND_GET_BACKUP_ROLE_LIST,	/// ��ȡ���ݿ��ɫ����


	SQL_COMMAND_MAX = 0x0000ffff,
};


struct query_sql_header_t
{
	int nMaxRow;
	int nTotalRow;
	int nRows;
	int nCols;
	char nColType[128];
	const char* sColKey[128];
};

#define ADD_CONNECT(strConn) do {\
	const char* module = GetEntInfo()->GetEntityName();\
	const char* func = "Init";\
	if (GetDumpMgr()->AddModuleConnect(strConn, this)) {\
	GetDumpMgr()->LogEchoTrace(module,func,"add conn",strConn); }\
	else {\
	GetDumpMgr()->LogEchoTrace(module,func,"add conn fail",strConn);\
	return false; }}\
	while (0)


#define G_QUERYROLEFIELDS "`name`, uid, account, create_time, delete_time, open_time, total_sec, deleted, `online`, charge_info, save_time, save_data"
#define G_QUERYPUBLICFIELDS "`name`, save_time, save_data"

#define INNER_ADD_READBIN_POOL(db,isRole,name,table,key_col,sel_key) do {\
	const char* module = GetEntInfo()->GetEntityName();\
	const char* func = "Init";\
	if (GetDumpMgr()->InitReadBinQueue(GetID(),db,module, table, key_col, sel_key, isRole)) {\
	GetDumpMgr()->LogEchoTrace(module,func,"created",name,db);}\
	else{\
	GetDumpMgr()->LogWarning(module,func,"create fail",name,db);\
	return false; }}\
	while(0)

#define ADD_READROLE_POOL(dbname) INNER_ADD_READBIN_POOL(dbname,true,"read role","nx_roles","name",G_QUERYROLEFIELDS)
#define ADD_READPUBLIC_POOL(dbname) INNER_ADD_READBIN_POOL(dbname,false,"read public","nx_domains","name",G_QUERYPUBLICFIELDS)
#define ADD_READGUILD_POOL(dbname) INNER_ADD_READBIN_POOL(dbname,false,"read guild","nx_guilds","name",G_QUERYPUBLICFIELDS)


#define REGIST_SQL_COMMAND(sqlcmd,db,callback) do {\
	const char* module = GetEntInfo()->GetEntityName();\
	const char* func = "Init";\
	if (GetDumpMgr()->RegistExeSqlCallBack(GetID(), sqlcmd, db, callback) ) {\
	GetDumpMgr()->LogEchoTrace(module,func,"add sql cmd",db,callback);}\
	else {\
	GetDumpMgr()->LogWarning(module,func,"add sql cmd fail",db,callback);\
	return false; }}\
	while(0)



#endif // __SQL_COMMAD_H__
