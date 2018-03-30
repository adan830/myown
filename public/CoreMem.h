//--------------------------------------------------------------------
// �ļ���:		CoreMem.h
// ��  ��:		
// ˵  ��:		
// ��������:	2007��2��6��
// ������:		½����
// ��Ȩ����:	������ţ�������޹�˾
//--------------------------------------------------------------------

#ifndef _PUBLIC_COREMEM_H
#define _PUBLIC_COREMEM_H

#include "ICore.h"

extern ICore* g_pCore;

// ��ģ�鰲ȫ���ڴ����

namespace core_mem {
	inline void* CoreAllocDebug(size_t size, const char* file, int line)
	{
		return g_pCore->MemoryAllocDebug(size, file, line);
	}

	inline void* CoreAlloc(size_t size)
	{
		return g_pCore->MemoryAlloc(size);
	}

	inline void CoreFree(void* ptr, size_t size)
	{
		g_pCore->MemoryFree(ptr, size);
	}

	template<typename TYPE>
	TYPE* CoreNewDebug(TYPE*, const char* file, int line)
	{
		TYPE* ptr = (TYPE*)g_pCore->MemoryAllocDebug(
			sizeof(TYPE), file, line);

		new (ptr) TYPE;

		return ptr;
	}

	template<typename TYPE>
	TYPE* CoreNew(TYPE*)
	{
		TYPE* ptr = (TYPE*)g_pCore->MemoryAlloc(sizeof(TYPE));

		new (ptr) TYPE;

		return ptr;
	}

	template<typename TYPE>
	void CoreDelete(TYPE* ptr)
	{
		if (ptr)
		{
			ptr->~TYPE();
		}

		g_pCore->MemoryFree(ptr, sizeof(TYPE));
	}
} // namespace core_mem

// �ڴ����
#ifdef _DEBUG
#define CORE_ALLOC(size) \
	core_mem::CoreAllocDebug(size, __FILE__, __LINE__)
#define CORE_FREE(ptr, size) core_mem::CoreFree(ptr, size)
#define CORE_NEW(type) \
	core_mem::CoreNewDebug((type*)0, __FILE__, __LINE__)
#define CORE_DELETE(ptr) core_mem::CoreDelete(ptr)
#else
#define CORE_ALLOC(size) core_mem::CoreAlloc(size)
#define CORE_FREE(ptr, size) core_mem::CoreFree(ptr, size)
#define CORE_NEW(type) core_mem::CoreNew((type*)0)
#define CORE_DELETE(ptr) core_mem::CoreDelete(ptr)
#endif // _DEBUG

// ����ģ������

class TCoreAlloc
{
public:
	TCoreAlloc() {}
	~TCoreAlloc() {}

	// �����ڴ�
	void* Alloc(size_t size) { return CORE_ALLOC(size); }
	// �ͷ��ڴ�
	void Free(void* ptr, size_t size) { CORE_FREE(ptr, size); }
	// ����
	void Swap(TCoreAlloc& src) {}
};

/*
namespace core_mem {

template<typename TYPE>
TYPE* CoreAllocDebug(TYPE*, size_t count, const char* file, size_t line)
{
	return (TYPE*)g_pCore->MemoryAllocDebug(
		sizeof(TYPE), count, file, line);
}

template<typename TYPE>
TYPE* CoreAlloc(TYPE*, size_t count)
{
	return (TYPE*)g_pCore->MemoryAlloc(sizeof(TYPE), count);
}

template<typename TYPE>
void CoreFree(TYPE* p)
{
	g_pCore->MemoryFree(p);
}

template<typename TYPE>
TYPE* CoreNewDebug(TYPE*, size_t count, const char* file, size_t line)
{
	TYPE* p = (TYPE*)g_pCore->MemoryAllocDebug(
		sizeof(TYPE), count, file, line);
	
	TYPE* ptr = p;
	
	for (size_t i = 0; i < count; ++i)
	{
		new (ptr) TYPE();
		++ptr;
	}
	
	return p;
}

template<typename TYPE>
TYPE* CoreNew(TYPE*, size_t count)
{
	TYPE* p = (TYPE*)g_pCore->MemoryAlloc(sizeof(TYPE), count);
	
	TYPE* ptr = p;
	
	for (size_t i = 0; i < count; ++i)
	{
		new (ptr) TYPE();
		++ptr;
	}
	
	return p;
}

template<typename TYPE>
void CoreDelete(TYPE* p)
{
	if (NULL == p)
	{
		return;
	}

	size_t size;
	size_t count;
	
	if (!g_pCore->MemoryGetInfo(p, size, count))
	{
		return;
	}

	if (size < sizeof(TYPE))
	{
		Assert(0);
	}
	
	char* pointer = (char*)p;

	for (size_t i = 0; i < count; ++i)
	{
		((TYPE*)pointer)->~TYPE();

		pointer += size;	// ����sizeof(TYPE)����Ϊ�����ǻ���ָ��
	}
	
	g_pCore->MemoryFree(p);
}

} // namespace core_mem

// �ڴ����
#ifdef _DEBUG
	#define CORE_ALLOC(type, count) \
		core_mem::CoreAllocDebug((type*)0, count, __FILE__, __LINE__)
	#define CORE_FREE(p) core_mem::CoreFree(p)
	#define CORE_NEW(type) \
		core_mem::CoreNewDebug((type*)0, 1, __FILE__, __LINE__)
	#define CORE_NEW_ARRAY(type, count) \
		core_mem::CoreNewDebug((type*)0, count, __FILE__, __LINE__)
	#define CORE_DELETE(p) core_mem::CoreDelete(p)
	#define CORE_DELETE_ARRAY(p) core_mem::CoreDelete(p)
#else
	#define CORE_ALLOC(type, count) core_mem::CoreAlloc((type*)0, count)
	#define CORE_FREE(p) core_mem::CoreFree(p)
	#define CORE_NEW(type) core_mem::CoreNew((type*)0, 1)
	#define CORE_NEW_ARRAY(type, count) core_mem::CoreNew((type*)0, count)
	#define CORE_DELETE(p) core_mem::CoreDelete(p)
	#define CORE_DELETE_ARRAY(p) core_mem::CoreDelete(p)
#endif // _DEBUG

// ����ģ�����������ɾ��

template<typename DATA>
struct TCoreAllocator
{
	static DATA* New()
	{
		return CORE_NEW(DATA);
	}
	
	static void Delete(DATA* p)
	{
		CORE_DELETE(p);
	}
};
*/

#endif // _PUBLIC_COREMEM_H
