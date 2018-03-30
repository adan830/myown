
#ifndef _SERVER_ISNSKERNEL_H
#define _SERVER_ISNSKERNEL_H
// ISnsKernel
// ���Ĺ��ܽӿ�
#include "KnlConst.h"
#include "../public/Macros.h"
#include "../public/IVarList.h"

// ���������߼��ӿڰ汾��
#define SNS_LOGIC_MODULE_VERSION 0x3001

class ISnsKernel;
class IVarList;
struct PERSISTID;
class ISnsData;

typedef int (__cdecl * SNS_HEARTBEAT_FUNC)(ISnsKernel * pKernel, 
									   const char* sns_name, int time) ;
//
//typedef int (__cdecl * LUA_EXT_FUNC)(void * state) ;
//
//#define DECL_LUA_EXT(p)			pKernel->DeclareLuaExt(#p, p)
#define DECL_SNS_HEARTBEAT(p)		pKernel->DeclareHeartBeat(#p, p)

class ISnsKernel
{
	public:
	// �������������Ҫֱ�ӵ���
	virtual bool DeclareHeartBeat(const char * func_name, SNS_HEARTBEAT_FUNC func) = 0;

public:
	virtual ~ISnsKernel() = 0;

	// �����ϵͳLOG�ļ�
	virtual void Trace(const char * info) = 0;

	// �������Ļ
	virtual void Echo(const char * info) = 0;

	// �����Դ�ļ�Ŀ¼
	virtual const char * GetResourcePath() = 0;

	//��ȡ sns cache
	//virtual int SetSnsCache(int nSize) = 0;
	virtual int GetSnsCache() = 0;
	//��ȡ ����
	virtual int GetSnsCount() = 0;

	//�ж� sns �����Ƿ����ڴ���
	virtual bool FindSns(const char* uid) = 0;
	//����sns ����
	virtual bool CreateSns(const char* uid) = 0;
	// ���ش�������Sns���ݴ���
	// �����������param���κδ��� ��ֻ�Ǽ򵥵Ļش�
	virtual bool CreateSns(const char* uid,const IVarList& param) = 0;
	//�Ƿ���ڣ����ݿ��в���
	virtual bool IsExistSns(const char* uid) = 0;
	//�Ƿ��ܼ��ظ�sns����
	virtual bool CanLoadSns(const char* uid) = 0;
	//���� sns ����
	virtual bool LoadSns(const char* uid) = 0;
	virtual bool LoadSns(const char* uid,const IVarList& param,bool bSafeOnLoad = false) = 0;
	// ���� sns ����
	virtual bool SaveSns(const char* uid) = 0;
	//�������е�sns ����
	virtual bool SaveSnsAll() = 0;
	//ɾ��Sns
	virtual bool RemoveSns(const char* uid) = 0;
	// ���� snslog ����
	virtual bool SaveSnsLog(const char* uid,int type, 
		const wchar_t * content, const wchar_t * comment) = 0;

	virtual ISnsData * GetSnsData(const char* uid)=0;

	/*
	//��������
	virtual bool AddAttr(const char* uid, const char * attr_name, int type, bool saving = false) = 0;
	//�Ƴ�����
	virtual bool RemoveAttr(const char* uid, const char * attr_name) = 0;	
	//�����Ƿ����
	virtual bool IsAttrExists(const char* uid, const char * attr_name) = 0;
	
	//��ѯ����
	virtual int QueryAttrInt(const char* uid, const char * attr_name) = 0;
	virtual int64_t QueryAttrInt64(const char* uid, const char* name) = 0;
	virtual float QueryAttrFloat(const char* uid, const char * attr_name) = 0;
	virtual double QueryAttrDouble(const char* uid, const char* name) = 0;
	virtual const char* QueryAttrString(const char* uid, const char * attr_name) = 0;
	virtual const wchar_t* QueryAttrWideStr(const char* uid, const char * attr_name) = 0;
	virtual PERSISTID QueryAttrObject(const char* uid, const char * attr_name) = 0;

	//��������
	virtual bool SetAttrInt(const char* uid, const char * attr_name, int value) = 0;
	virtual bool SetAttrInt64(const char* uid, const char * attr_name, int64_t value) = 0;
	virtual bool SetAttrFloat(const char* uid, const char * attr_name, float value) = 0;
	virtual bool SetAttrDouble(const char* uid, const char * attr_name, double value) = 0;
	virtual bool SetAttrString(const char* uid, const char * attr_name, const char * value) = 0;
	virtual bool SetAttrWideStr(const char* uid, const char * attr_name, const wchar_t* value) = 0;
	virtual bool SetAttrObject(const char* uid, const char * attr_name, const PERSISTID & value) = 0;


	// �ж�ָ������Ƿ�������ڴ���
	virtual bool IsExistRecord(const char* uid, const char * rec_name) = 0;
	// ���һ���������ƣ��������������,�����ԣ��Ƿ񱣴�
	virtual bool AddRecord(const char* uid, const char * rec_name, 
		int cols, int maxRows,const IVarList& colType ,bool saving = false) = 0;
	// ȡ������
	virtual int GetRecordCols(const char* uid, const char * rec_name) = 0;
	// ȡ������
	virtual int GetRecordRows(const char* uid, const char * rec_name) = 0;
	// ȡ���������
	virtual int GetRecordMax(const char* uid, const char * rec_name) = 0;
	// �������������
	virtual int GetRecordColType(const char* uid, const char * rec_name, int col) = 0;
	// ���һ�У��������кţ��к�Ϊ-1��ʾ������У������ز�����к�
	virtual int AddRecordRow(const char* uid, const char * rec_name, int row) = 0;
	// ���һ�в���ʼ�����������кţ��к�Ϊ-1��ʾ������У������ز�����к�
	virtual int AddRecordRowValue(const char* uid, const char * rec_name,
		int row, const IVarList & value) = 0;
	// ɾ��һ�У��������к�
	virtual bool RemoveRecordRow(const char* uid, const char * rec_name, int row) = 0;
	// ������б�����
	virtual bool ClearRecord(const char* uid, const char * rec_name) = 0;

	// д������
	virtual bool SetRecordRowValue(const char* uid, const char * rec_name,
		int row, const IVarList & value) = 0;
	virtual bool SetRecordInt(const char* uid, const char * rec_name, 
		int row, int col, int value) = 0;
	virtual bool SetRecordInt64(const char* uid, const char * rec_name, 
		int row, int col, int64_t value) = 0;
	virtual bool SetRecordFloat(const char* uid, const char * rec_name, 
		int row, int col, float value) = 0;
	virtual bool SetRecordDouble(const char* uid, const char * rec_name, 
		int row, int col, double value) = 0;
	virtual bool SetRecordString(const char* uid, const char * rec_name, 
		int row, int col, const char * value) = 0;
	virtual bool SetRecordWideStr(const char* uid, const char * rec_name, 
		int row, int col, const wchar_t * value) = 0;
	virtual bool SetRecordObject(const char* uid, const char * rec_name, 
		int row, int col, const PERSISTID & value) = 0;

	
	// ��������
	virtual bool QueryRecordRowValue(const char* uid, const char * rec_name, 
		int row, IVarList & value) = 0;
	virtual int QueryRecordInt(const char* uid, const char * rec_name, 
		int row, int col) = 0;
	virtual int64_t QueryRecordInt64(const char* uid, const char * rec_name, 
		int row, int col) = 0;
	virtual float QueryRecordFloat(const char* uid, const char * rec_name, 
		int row, int col) = 0;
	virtual double QueryRecordDouble(const char* uid, const char * rec_name, 
		int row, int col) = 0;
	virtual const char * QueryRecordString(const char* uid, const char * rec_name, 
		int row, int col) = 0;
	virtual const wchar_t * QueryRecordWideStr(const char* uid, const char * rec_name, 
		int row, int col) = 0;
	virtual PERSISTID QueryRecordObject(const char* uid, const char * rec_name, 
		int row, int col) = 0;

	// ����ָ���������ĳ�У�����-1��ʾû��
	virtual int FindRecordInt(const char* uid, const char * rec_name, 
		int col, int value) = 0;
	virtual int FindRecordInt64(const char* uid, const char * rec_name, 
		int col, int64_t value) = 0;
	virtual int FindRecordFloat(const char* uid, const char * rec_name, 
		int col, float value) = 0;
	virtual int FindRecordDouble(const char* uid, const char * rec_name, 
		int col, double value) = 0;
	virtual int FindRecordString(const char* uid, const char * rec_name, 
		int col, const char * value) = 0;
	virtual int FindRecordWideStr(const char* uid, const char * rec_name, 
		int col, const wchar_t * value) = 0;
	virtual int FindRecordObject(const char* uid, const char * rec_name, 
		int col, const PERSISTID & value) = 0;

	*/

	////���� sns ��Ϣ�� name��Ӧ����� ������������ӦOnCommand
	virtual bool SendToScenePlayer(int server_id,int memeber_id,int scene_id,const char* uid,const IVarList& msg) = 0;
	//���� sns ��Ϣ��member������ ������������ӦOnSnsMessage
	virtual bool SendToSceneMessage(int server_id,int memeber_id,int scene_id,const IVarList & msg) = 0;



	/*
	

	��ΰΰ(��ΰΰ) 11:34:06
	{"appPackage":"com.example.servicetest","badge":0,"client":"android","expandMessage":"http://www.baidu.com","message":"��������","sound":"default","title":"����","token":["12-34-53-62-01","12-42-56-12"]}
	---------------------------------------------
	����Ǹ�ʽ���ǹ̶��ģ�ֵ�Լ�����


	#include "../../public/Converts.h"
	#include "../../public/Inlines.h"

	//��Ϣ����
	wchar_t buffer[1024];
	SafeSwprintf(buffer,sizeof(buffer),
		L"{appPackage\":\"%s\",\"badge\":%d,\"client\":\"%s\",\"expandMessage\":\"%s\",\"message\":\"%s\",\"sound\":\"%s\",\"title\":\"%s\",\"token\":[\"%s\",\"%s\"]}",
		L"com.example.servicetest",0,L"android",L"http://www.baidu.com",L"��������",L"default",L"����",L"12-34-53-62-01",L"12-42-56-12");

	//��С��С�ڿ����ȵ�3��
	char utf8_buffer[3072];
	Port_WideStrToUTF8(buffer, utf8_buffer, sizeof(utf8_buffer));


	pKernel->MessagePush(utf8_buffer);
	
	*/
	//������Ϣ�����ͷ��������ַ���Ҫ��utf8 ����,�ο�������룩
	virtual bool MessagePush(const char *msg) = 0;

	/*
	����������������OnCreate ����
	�п�����������ʱ��sns �Ѵ��ڴ����Ƴ�����Ҫ�ж� FindSns��CanLoadSns
	*/
	//�������
	virtual bool AddHeartBeat(const char* name,const char * func, int time) = 0;
	// ��Ӽ�������
	virtual bool AddCountBeat(const char* name,const char * func, int time, int count) = 0;
	// ɾ����������
	virtual bool RemoveHeartBeat(const char* name,const char * func) = 0;
	// ������������
	virtual bool FindHeartBeat(const char* name,const char * func) = 0;

};

inline ISnsKernel::~ISnsKernel() {}

#endif