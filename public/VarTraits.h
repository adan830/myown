//-------------------------------------------------------------------- 
// �ļ���:		VarTraits.h 
// ��  ��:		
// ˵  ��:		
// ��������:	2007��2��5��	
// ������:		½����
// ��Ȩ����:	������ţ�������޹�˾
//--------------------------------------------------------------------

#ifndef _PUBLIC_VARTRAITS_H
#define _PUBLIC_VARTRAITS_H

#include "VarType.h"
#include "PersistId.h"

#include <string>

// TVarTraits
// ��ȡ��������

template<typename TYPE>
struct TVarTraits
{
	enum { Type = VTYPE_UNKNOWN };
};

template<>
struct TVarTraits<bool>
{
	enum { Type = VTYPE_BOOL };
};

template<>
struct TVarTraits<char>
{
	enum { Type = VTYPE_INT };
};

template<>
struct TVarTraits<unsigned char>
{
	enum { Type = VTYPE_INT };
};

template<>
struct TVarTraits<short>
{
	enum { Type = VTYPE_INT };
};

template<>
struct TVarTraits<unsigned short>
{
	enum { Type = VTYPE_INT };
};

template<>
struct TVarTraits<int>
{
	enum { Type = VTYPE_INT };
};

template<>
struct TVarTraits<unsigned int>
{
	enum { Type = VTYPE_INT };
};

template<>
struct TVarTraits<long>
{
#if defined(FX_SYSTEM_WINDOWS) || defined(FX_SYSTEM_32BIT)
	enum { Type = VTYPE_INT };
#else
	enum { Type = VTYPE_INT64 };
#endif
};

template<>
struct TVarTraits<unsigned long>
{
#if defined(FX_SYSTEM_WINDOWS) || defined(FX_SYSTEM_32BIT)
	enum { Type = VTYPE_INT };
#else
	enum { Type = VTYPE_INT64 };
#endif
};

#if defined(FX_SYSTEM_WINDOWS) || defined(FX_SYSTEM_32BIT)
template<>
struct TVarTraits<int64_t>
{
	enum { Type = VTYPE_INT64 };
};

template<>
struct TVarTraits<uint64_t>
{
	enum { Type = VTYPE_INT64 };
};
#endif

template<>
struct TVarTraits<float>
{
	enum { Type = VTYPE_FLOAT };
};

template<>
struct TVarTraits<double>
{
	enum { Type = VTYPE_DOUBLE };
};

template<>
struct TVarTraits<char*>
{
	enum { Type = VTYPE_STRING };
};

template<>
struct TVarTraits<const char*>
{
	enum { Type = VTYPE_STRING };
};

template<>
struct TVarTraits<std::string>
{
	enum { Type = VTYPE_STRING };
};

template<>
struct TVarTraits<const std::string&>
{
	enum { Type = VTYPE_STRING };
};

template<>
struct TVarTraits<wchar_t*>
{
	enum { Type = VTYPE_WIDESTR };
};

template<>
struct TVarTraits<const wchar_t*>
{
	enum { Type = VTYPE_WIDESTR };
};

template<>
struct TVarTraits<std::wstring>
{
	enum { Type = VTYPE_WIDESTR };
};

template<>
struct TVarTraits<const std::wstring&>
{
	enum { Type = VTYPE_WIDESTR };
};

template<>
struct TVarTraits<PERSISTID>
{
	enum { Type = VTYPE_OBJECT };
};

template<>
struct TVarTraits<const PERSISTID&>
{
	enum { Type = VTYPE_OBJECT };
};

template<>
struct TVarTraits<void*>
{
	enum { Type = VTYPE_USERDATA };
};

#endif // _PUBLIC_VARTRAITS_H
