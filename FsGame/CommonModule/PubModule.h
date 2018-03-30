//--------------------------------------------------------------------
// �ļ���:		PubModule.h
// ��  ��:		�������ݲ���
// ˵  ��:		
// ��������:	2014��06��10��
// ��������:	2014��06��10��
// ������:		  ( )
// �޸���:     
//    :	    
//--------------------------------------------------------------------

#ifndef __PubModule_H__
#define __PubModule_H__

#include "Fsgame/Define/header.h"

class PubModule : public ILogicModule
{
public:
	virtual bool Init(IKernel * pKernel);

	virtual bool Shut(IKernel * pKernel)
	{
		return true;
	}

public:
	// ��ȡ�������ݿռ�
	static  IPubData* GetPubData(IKernel * pKernel, const wchar_t* domain_name);
	//����ӿ�
	static bool SetProp(IKernel * pKernel, const char * pubspace, const wchar_t * pubdata,
		const char * prop, const IVarList & value);
	static bool IncProp(IKernel * pKernel, const char * pubspace, const wchar_t * pubdata,
		const char * prop, const IVarList & value);

	//�������
	static bool AddRec(IKernel * pKernel, const char * pubspace_name, const wchar_t * pubdata_name,
		const char * rec, int row_max, int cols, const IVarList& col_types);
	static bool ClearRec(IKernel * pKernel, const char * pubspace_name, const wchar_t * pubdata_name,
		const char * rec);
	static bool RemoveRecRow(IKernel * pKernel, const char * pubspace, const wchar_t * pubdata,
		const char * rec, int row);
	static bool AddRecRowValue(IKernel * pKernel, const char * pubspace, const wchar_t * pubdata,
		const char * rec, const IVarList& args);
	static bool SetRecValue(IKernel * pKernel, const char * pubspace, const wchar_t * pubdata,
		const char * rec, int row, int col, const IVarList & value);
	static bool IncRecValue(IKernel * pKernel, const char * pubspace, const wchar_t * pubdata,
		const char * rec, int row, int col, const IVarList & value);


	//���ؽ�ֵ�������
	static bool RemoveRecRowByKey(IKernel * pKernel, const char * pubspace, const wchar_t * pubdata,
		const char * rec, int keycol, const IVarList & key);
	static bool SetRecValueByKey(IKernel * pKernel, const char * pubspace, const wchar_t * pubdata,
		const char * rec, int keycol, const IVarList & key, int col, const IVarList & value);
	static bool IncRecValueByKey(IKernel * pKernel, const char * pubspace, const wchar_t * pubdata,
		const char * rec, int keycol, const IVarList & key, int col, const IVarList & value);

	// ��ѯ���
	static int GetPubRecColType(IKernel * pKernel, const char * pubspace, const wchar_t * pubdata, 
		const char * rec, int keycol);

public:
	static PubModule* m_pPubModule;
};

#endif //__PubModule_H__
