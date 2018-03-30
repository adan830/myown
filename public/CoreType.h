//--------------------------------------------------------------------
// �ļ���:		CoreType.h
// ��  ��:		
// ˵  ��:		
// ��������:	2012��6��4��
// ������:		½����
// ��Ȩ����:	������ţ�������޹�˾
//--------------------------------------------------------------------

#ifndef _PUBLIC_CORETYPE_H
#define _PUBLIC_CORETYPE_H

#include "Macros.h"
#include "CoreMem.h"
#include "FastStr.h"
#include "Var.h"
#include "VarList.h"

typedef TFastStr<char, 16, TCharTraits<char>, TCoreAlloc> core_string;
typedef TFastStr<wchar_t, 16, TCharTraits<wchar_t>, TCoreAlloc> core_wstring;
typedef TFastStr<char, 128, TCharTraits<char>, TCoreAlloc> core_long_string;
typedef TFastStr<wchar_t, 128, TCharTraits<wchar_t>, TCoreAlloc> core_long_wstring;
typedef TVar<4, TCoreAlloc> core_var;
typedef TVarList<8, 128, TCoreAlloc> core_var_list;

#endif // _PUBLIC_CORETYPE_H
