//--------------------------------------------------------------------
// �ļ���:		IVarTable.h
// ��  ��:		
// ˵  ��:		
// ��������:	2007��2��6��
// ������:		½����
// ��Ȩ����:	������ţ�������޹�˾
//--------------------------------------------------------------------

#ifndef _PUBLIC_IVARTABLE_H
#define _PUBLIC_IVARTABLE_H

#include "Macros.h"
#include "IVarList.h"
//#include "SharePtr.h"

// ��ӿ�

class IVar;

class IVarTable
{
public:
	virtual ~IVarTable() = 0;

	// ��ñ�������
	virtual size_t GetCount() const = 0;
	// ��ü����б�
	virtual void GetKeyList(IVarList& result) const = 0;
	// ��ü�ֵ
	virtual IVar* GetValue(const char* key) const = 0;
	// ��ֵ�Ƿ����
	virtual bool Exists(const char* key) const = 0;
	// ��Ӽ�
	virtual IVar* Add(const char* key, const IVar& value) = 0;
	// ɾ����
	virtual bool Remove(const char* key) = 0;
	// ���
	virtual void Clear() = 0;
	// ���ü�ֵ������������ӣ�
	virtual void Assign(const char* key, const IVar& value) = 0;
};

inline IVarTable::~IVarTable() {}

/*
class IVar;

typedef TSharePtr<IVar> IVarPointer;

class IVarTable
{
public:
	virtual ~IVarTable() = 0;

	// ��ñ�������
	virtual size_t GetCount() const = 0;
	// ��ü����б�
	virtual size_t GetKeyList(IVarList& result) const = 0;
	// ��ü�ֵ
	virtual const IVarPointer* GetVarPointer(const char* key) const = 0;
	// ��ֵ�Ƿ����
	virtual bool Exists(const char* key) const = 0;
	// ��Ӽ�
	virtual const IVarPointer* Add(const char* key, 
		const IVarPointer& vp) = 0;
	// ɾ����
	virtual bool Remove(const char* key) = 0;
	// ���
	virtual void Clear() = 0;
};

inline IVarTable::~IVarTable() {}
*/

#endif // _IVARTABLE_H

