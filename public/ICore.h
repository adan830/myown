//--------------------------------------------------------------------
// �ļ���:		ICore.h
// ��  ��:		
// ˵  ��:		
// ��������:	2007��1��30��
// ������:		½����
// ��Ȩ����:	������ţ�������޹�˾
//--------------------------------------------------------------------

#ifndef _PUBLIC_ICORE_H
#define _PUBLIC_ICORE_H

#include "Macros.h"
#include "PersistId.h"
#include "IVar.h"
#include "IVarList.h"
#include <stdarg.h>

// ���Ľӿڰ汾��
#define FX_MODULE_CORE_VERSION 1003

// ���Ľӿ�

class IEntity;
class IInterface;
class IEntInfo;
class IFileSys;
class IIniFile;

class ICore
{
public:
	struct performance_t
	{
		double dTotalTime;
		double dWinMsgProcTime;
		double dAsyncProcTime;
		double dExecuteTime;
		double dDisplayTime;
		double dScriptTime;
	};
	
	typedef bool (*TRACE_FUNC)(void* context, const char* info);
	typedef double (*FRAME_TIME_FUNC)(double prev_frame_time, 
		double total_frame_time, double* elapse_time, 
		double* total_elapse_time);

public:
	virtual ~ICore() = 0;

	// ��ģ���ڴ����
	virtual void* MemoryAllocDebug(size_t size, const char* file, 
		int line) = 0;
	virtual void* MemoryAlloc(size_t size) = 0;
	virtual void MemoryFree(void* ptr, size_t size) = 0;
	
	// ��ó���ʵ�����
	virtual void* GetInstance() = 0;
	// ��ô��ھ��
	virtual void* GetWindowHandle() = 0;
	// ��ù���·��
	virtual const char* GetWorkPath() = 0;
	// ��ýű�·��
	virtual const char* GetScriptPath() = 0;
	// �����Դ·��
	virtual const char* GetResourcePath() = 0;

	// ִ���߼�֡��
	virtual void SetExecuteFPS(int value) = 0;
	virtual int GetExecuteFPS() = 0;

	// ִ����ʾ֡��
	virtual void SetDisplayFPS(int value) = 0;
	virtual int GetDisplayFPS() = 0;
	
	// ��ѭ������ʱ��
	virtual void SetSleep(int value) = 0;
	virtual int GetSleep() = 0;

	// ���Ƴ����˳�
	virtual void SetQuit(bool value) = 0;
	virtual bool GetQuit() const = 0;
	
	// ��������Ϣ
	virtual bool ProcessWinMsg(void* hwnd, unsigned int msg, 
		unsigned int param1, unsigned int param2, int& result) = 0;
	// ��Ӳ��ظ��Ĵ�����Ϣ�������滻��
	virtual bool PutSoleWinMsg(void* hwnd, unsigned int msg,
		unsigned int param1, unsigned int param2) = 0;
	// ��ѭ������
	virtual bool Execute() = 0;
	
	// ������־���ļ�
	virtual bool TraceLog(const char* log) = 0;
	virtual bool TraceLogW(const wchar_t* log) = 0;
	// �ض�����־��ָ������
	virtual bool SetTraceFunc(TRACE_FUNC func, void* context) = 0;
	// ����������Ϣ���ļ�
	virtual bool DumpFile(const char* type, const char* file) = 0;
	
	// ����ļ�ϵͳ
	virtual IFileSys* GetFileSys() = 0;
	
	// ���ҹ��ܽӿ�
	virtual bool FindInterface(const char* name) = 0;
	// ��ù��ܽӿ�
	virtual IInterface* GetInterface(const char* name) = 0;
	// ���ͬһ���ֿռ��еĹ��ܽӿ�
	virtual IInterface* GetInterfaceSameSpace(
		IInterface* pInterface, const char* name) = 0;
	// �ͷŹ��ܽӿ�
	virtual void ReleaseInterface(IInterface* pInterface) = 0;
	
	// ����ʵ������Ϣ
	virtual IEntInfo* GetEntInfo(const char* name) = 0;
	// �����ʵ��
	virtual IEntity* GetMainEntity() = 0;
	// ���ʵ��
	virtual IEntity* GetEntity(const PERSISTID& id) = 0;
	// ������е�ʵ�����ID
	virtual size_t GetAllEntity(IVarList& result) = 0;
	// �������ַ��ϵĵ�һ��ʵ��
	virtual IEntity* LookupEntity(const char* name) = 0;
	// �������ַ��ϵ�����ʵ��
	virtual size_t LookupEntityMore(const char* name, 
		IVarList& result) = 0;
	// ����ʵ��
	virtual IEntity* CreateEntity(const char* name) = 0;
	// ����������ʵ��
	virtual IEntity* CreateEntityArgs(const char* name, 
		const IVarList& args) = 0;
	// ����ͬһ���ֿռ��е�ʵ��
	virtual IEntity* CreateEntitySameSpace(IEntity* pEntity, 
		const char* name, const IVarList& args) = 0;
	// ɾ��ʵ��
	virtual bool DeleteEntity(const PERSISTID& id) = 0;
	// �Ƿ�����ʵ�屻�ű�ɾ��
	virtual void SetCanDelByScript(IEntity* pEntity, bool value) = 0;
	
	// ���ʵ������
	virtual bool GetProperty(IEntity* pEntity, 
		const char* prop, IVar& value) = 0;
	// ����ʵ������
	virtual bool SetProperty(IEntity* pEntity,
		const char* prop, const IVar& value) = 0;
	// ����ʵ�巽��
	virtual bool InvokeMethod(IEntity* pEntity,
		const char* func, const IVarList& args, IVarList& res) = 0;
	
	// ���нű���չ����
	virtual bool RunFunction(const char* func, 
		const IVarList& args, IVarList& res) = 0;
	
	// ͬ�����нű�
	virtual bool RunScript(const char* script, const char* func,
		const IVarList& args, IVarList* res = NULL) = 0;
	// �����첽����
	virtual bool ExecAsyncProc(const char* script, const char* func,
		const IVarList& args, IVarList* res = NULL) = 0;
	// �����첽����
	virtual bool FindAsyncProc(const char* script, const char* func,
		const PERSISTID& id) = 0;
	// ��ֹ�첽����
	virtual bool KillAsyncProc(const char* script, const char* func,
		const PERSISTID& id) = 0;
	// �����첽�¼������ر������Ĺ�������
	virtual size_t GenAsyncEvent(const PERSISTID& id, 
		const char* event, const IVarList& args) = 0;
	// ʵ��󶨽ű�
	virtual bool BindScript(IEntity* pEntity, const char* script) = 0;
	// ʵ����߼���
	virtual bool BindLogic(IEntity* pEntity, const char* logic,
		const IVarList& args) = 0;
	// ���ҽű��ص�
	virtual bool FindCallback(IEntity* pEntity, const char* event) = 0;
	// ִ�нű��ص�
	virtual bool ExecCallback(IEntity* pEntity, const char* event, 
		const IVarList& args, IVarList* res = NULL) = 0;
	// ���¼������нű�
	virtual bool ReloadAllScript() = 0;
	
	// ��ӵ����ж���
	virtual bool AddExecute(IEntity* pEntity) = 0;
	// �����ж����Ƴ�
	virtual bool RemoveExecute(IEntity* pEntity) = 0;

	// ��Ӵ�����Ϣ����msg����С��0x4000��priorԽ�󼶱�Խ�ߣ��ȼ������ȣ�
	virtual bool AddMsgProc(IEntity* pEntity, unsigned int msg, 
		int prior = 0) = 0;
	// ���������������Ϣ��msg����С��0x4000��priorԽ�󼶱�Խ�ߣ��ȼ������ȣ�
	virtual bool AddMsgProcInstant(IEntity* pEntity, unsigned int msg, 
		int prior = 0) = 0;
	// �Ƴ�������Ϣ����
	virtual bool RemoveMsgProc(IEntity* pEntity, unsigned int msg) = 0;

	// ȫ�ֱ���
	virtual bool FindGlobalValue(const char* name) = 0;
	virtual bool RemoveGlobalValue(const char* name) = 0;
	virtual bool SetGlobalValue(const char* name, const IVar& value) = 0;
	virtual IVar& GetGlobalValue(const char* name) = 0;
	virtual size_t GetGlobalCount() = 0;
	virtual size_t GetGlobalList(IVarList& result) = 0;
	
	// ������־
	virtual bool LogWrite(int type, const char* format, va_list vlist) = 0;
	virtual void LogSetDropMode(bool value) = 0;
	virtual bool LogRegisterThread(int type, size_t start_addr, 
		size_t stack_size) = 0;
	
	// ���õ���ʵ��
	virtual bool SetSingle(IEntity* pEntity) = 0;
	// ��õ���ʵ��
	virtual IEntity* GetSingle(const char* entity_name) = 0;
	
	// �Ƿ�ƽ��ÿ֡ʱ��
	virtual void SetSmoothFrameTime(bool value) = 0;
	virtual bool GetSmoothFrameTime() const = 0;

	// ��õ�ǰ֡ʱ��
	virtual double GetFrameSeconds() const = 0;
	// ��õ�ǰ�ڲ���ʱ��ʱ��
	virtual double GetTotalSeconds() const = 0;
	
	// �����Ƿ��¼�ļ�������־
	virtual void SetLogFileIO(bool value) = 0;
	// �����Ƿ��¼�ű���ʱ��־
	virtual void SetLogScript(bool value) = 0;
	
	// ����������ܼ�����
	virtual performance_t* GetPerformance() = 0;

	// ���ô���֡ʱ��ĺ���
	virtual bool SetFrameTimeFunc(FRAME_TIME_FUNC func) = 0;

	// �Ƿ�������ʽ�汾
	virtual void SetRetailVersion(bool value) = 0;
	virtual bool GetRetailVersion() const = 0;
	
	// ���������ļ���д�ӿ�
	virtual IIniFile* CreateIniFile(const char* filename, 
		int read_raw_file = 0) = 0;

	// ���ճ�ʱ��δʹ�õĽű�����
	virtual int CollectUnuseScript(int seconds) = 0;
};

inline ICore::~ICore() {}

#endif // _PUBLIC_ICORE_H

