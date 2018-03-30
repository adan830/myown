//-------------------------------------------------------------------- 
// �ļ���:		LinuxLockVal.h 
// ��  ��:		
// ˵  ��:		
// ��������:	2010��12��15��
// ������:		½����
// ��Ȩ����:	������ţ�������޹�˾
//--------------------------------------------------------------------

#ifndef _SYSTEM_LINUXLOCKVAL_H
#define _SYSTEM_LINUXLOCKVAL_H

#include <unistd.h>

typedef struct { volatile int counter; } atomic_t;

#ifdef CONFIG_SMP
	#define LOCK_PREFIX \
		".section .smp_locks,\"a\"\n"   \
		"  .align 8\n"                  \
		"  .quad 661f\n" /* address */  \
		".previous\n"                   \
		"661:\n\tlock; "
#else
	#define LOCK_PREFIX ""
#endif // CONFIG_SMP

inline void atomic_inc(atomic_t* v)
{
	__asm__ __volatile__(
		LOCK_PREFIX "incl %0"
		:"=m" (v->counter)
		:"m" (v->counter));
}

inline void atomic_dec(atomic_t* v)
{
	__asm__ __volatile__(
		LOCK_PREFIX "decl %0"
		:"=m" (v->counter)
		:"m" (v->counter));
}

#define atomic_set(v, i) (((v)->counter) = (i))

#define __xg(x) ((volatile long*)(x))

inline unsigned long __cmpxchg(volatile void *ptr, unsigned long old_value,
	unsigned long new_value, int size)
{
	unsigned long prev;
	
	switch (size)
	{
	case 1:
		__asm__ __volatile__(LOCK_PREFIX "cmpxchgb %b1,%2"
			: "=a"(prev)
			: "q"(new_value), "m"(*__xg(ptr)), "0"(old_value)
			: "memory");
		return prev;
	case 2:
		__asm__ __volatile__(LOCK_PREFIX "cmpxchgw %w1,%2"
			: "=a"(prev)
			: "r"(new_value), "m"(*__xg(ptr)), "0"(old_value)
			: "memory");
		return prev;
	case 4:
		__asm__ __volatile__(LOCK_PREFIX "cmpxchgl %k1,%2"
			: "=a"(prev)
			: "r"(new_value), "m"(*__xg(ptr)), "0"(old_value)
			: "memory");
		return prev;
	case 8:
		__asm__ __volatile__(LOCK_PREFIX "cmpxchgq %1,%2"
			: "=a"(prev)
			: "r"(new_value), "m"(*__xg(ptr)), "0"(old_value)
			: "memory");
		return prev;
	}
	
	return old_value;
}

#define cmpxchg(ptr, o, n)\
	((__typeof__(*(ptr)))__cmpxchg((ptr),(unsigned long)(o),\
	(unsigned long)(n), sizeof(*(ptr))))

#define atomic_cmpxchg(v, oldv, newv) ((int)cmpxchg(&((v)->counter), oldv, newv))

// ����������

class CLockVal
{
public:
	CLockVal()
	{
		m_atom.counter = 0;
	}

	// ��һ
	int Inc()
	{
		atomic_inc(&m_atom);
		return m_atom.counter;
	}

	// ��һ
	int Dec()
	{
		atomic_dec(&m_atom);
		return m_atom.counter;
	}

	// ����
	int Set(int val)
	{
		atomic_set(&m_atom, val);
		return m_atom.counter;
	}

	// ��ȡ
	int Get() const
	{
		return m_atom.counter;
	}

	// �ȽϺ󽻻�������ԭֵ(����ǰֵ����oldv���򽫵�ǰֵ��Ϊnewv��������oldv)
	int CompareExchange(int oldv, int newv)
	{
		return atomic_cmpxchg(&m_atom, oldv, newv);
	}
	
public:
	atomic_t m_atom;
};

#endif // _SYSTEM_LINUXLOCKVAL_H
