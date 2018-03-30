//--------------------------------------------------------------------
// �ļ���:		VarType.h
// ��  ��:		
// ˵  ��:		
// ��������:	2007��1��30��
// ������:		 
// ��Ȩ����:	������ţ�������޹�˾
//--------------------------------------------------------------------

#ifndef _PUBLIC_VARTYPE_H
#define _PUBLIC_VARTYPE_H

// ��������

enum VARIANT_TYPE
{
	VTYPE_UNKNOWN,	// δ֪
	VTYPE_BOOL,		// ����
	VTYPE_INT,		// 32λ����
	VTYPE_INT64,	// 64λ����
	VTYPE_FLOAT,	// �����ȸ�����
	VTYPE_DOUBLE,	// ˫���ȸ�����
	VTYPE_STRING,	// �ַ���
	VTYPE_WIDESTR,	// ���ַ���
	VTYPE_OBJECT,	// �����
	VTYPE_POINTER,	// ָ��
	VTYPE_USERDATA,	// �û�����
	VTYPE_TABLE,	// ��
	VTYPE_MAX,
};

#endif // _VARTYPE_H
