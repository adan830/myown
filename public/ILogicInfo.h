//--------------------------------------------------------------------
// �ļ���:		ILogicInfo.h
// ��  ��:		
// ˵  ��:		
// ��������:	2007��12��25��
// ������:		½����
// ��Ȩ����:	������ţ�������޹�˾
//--------------------------------------------------------------------

#ifndef _PUBLIC_ILOGICINFO_H
#define _PUBLIC_ILOGICINFO_H

#include "Macros.h"
#include "IVarList.h"

// ICallbackInfo
// �߼�������Ϣ

class ICallbackInfo
{
public:
	virtual ~ICallbackInfo() = 0;

	virtual const char* GetName() const = 0;
	virtual void* GetMidFunc() const = 0;
	virtual bool GetReturnTable() const = 0;
};

inline ICallbackInfo::~ICallbackInfo() {}

// ILogicInfo
// �߼�����Ϣ

class ILogicCreator;
class ILogic;

class ILogicInfo
{
public:
	virtual ~ILogicInfo() = 0;

	// ��ô�����
	virtual ILogicCreator* GetCreator() const = 0;
	
	// �������ֿռ�
	virtual const char* GetSpaceName() const = 0;

	// ��������
	virtual const char* GetLogicName() const = 0;
	
	// ��������
	virtual size_t GetCallbackCount() const = 0;
	// ��÷��������б�
	virtual size_t GetCallbackList(IVarList& result) const = 0;
	// �ڱ����л�÷�����Ϣ
	virtual const ICallbackInfo* GetCallbackInfo(const char* name) const = 0;
};

inline ILogicInfo::~ILogicInfo() {}

#endif // _PUBLIC_ILOGICINFO_H

