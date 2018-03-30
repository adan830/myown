//--------------------------------------------------------------------
// �ļ���:		IEntInfo.h
// ��  ��:		
// ˵  ��:		
// ��������:	2007��2��7��
// ������:		½����
// ��Ȩ����:	������ţ�������޹�˾
//--------------------------------------------------------------------

#ifndef _PUBLIC_IENTINFO_H
#define _PUBLIC_IENTINFO_H

#include "Macros.h"
#include "IVarList.h"

// IPropInfo
// ʵ��������Ϣ

class IPropInfo
{
public:
	virtual ~IPropInfo() = 0;

	virtual const char* GetName() const = 0;
	virtual int GetType() const = 0;
	virtual void* GetGetFunc() const = 0;
	virtual void* GetSetFunc() const = 0;
};

inline IPropInfo::~IPropInfo() {}

// IMethodInfo
// ʵ�巽����Ϣ

class IMethodInfo
{
public:
	virtual ~IMethodInfo() = 0;

	virtual const char* GetName() const = 0;
	virtual void* GetMidFunc() const = 0;
	virtual bool GetReturnTable() const = 0;
};

inline IMethodInfo::~IMethodInfo() {}

// IEntInfo
// ʵ������Ϣ

class IEntCreator;
class IEntity;

class IEntInfo
{
public:
	virtual ~IEntInfo() = 0;

	// ��ô�����
	virtual IEntCreator* GetCreator() const = 0;
	
	// ��ø�������
	virtual const char* GetParentName() const = 0;

	// �������ֿռ�
	virtual const char* GetSpaceName() const = 0;

	// ��������
	virtual const char* GetEntityName() const = 0;
	
	// ��ø�����Ϣ
	virtual const IEntInfo* GetParent() const = 0;
	
	// �Ƿ�����ĳ�����������
	virtual bool IsKindOf(const char* name) const = 0;

	// �Ƿ�����ͬһ���ֿռ��ĳ�����������
	virtual bool IsKindSameSpace(
		const IEntity* pEntity, const char* name) const = 0;

	// ʵ������
	virtual size_t GetPropertyCount() const = 0;
	// ������������б�
	virtual size_t GetPropertyList(IVarList& result) const = 0;
	// �ڱ����л��������Ϣ
	virtual const IPropInfo* GetPropertyInfo(const char* name) const = 0;
	// �ӱ���͸������������Ϣ
	virtual const IPropInfo* FindPropertyInfo(const char* name) const = 0;
	// ��ñ���͸�������������б�
	virtual size_t GetPropertyAll(IVarList& result) const = 0;
	
	// ʵ�巽��
	virtual size_t GetMethodCount() const = 0;
	// ��÷��������б�
	virtual size_t GetMethodList(IVarList& result) const = 0;
	// �ڱ����л�÷�����Ϣ
	virtual const IMethodInfo* GetMethodInfo(const char* name) const = 0;
	// �ӱ���͸�����ҷ�����Ϣ
	virtual const IMethodInfo* FindMethodInfo(const char* name) const = 0;
	// ��ñ���͸���ķ��������б�
	virtual size_t GetMethodAll(IVarList& result) const = 0;
};

inline IEntInfo::~IEntInfo() {}

#endif // _PUBLIC_IENTINFO_H

