//--------------------------------------------------------------------
// �ļ���:		record.h
// ��  ��:		����ع���
// ˵  ��:		
//				
// ��������:	2015��07��31��
// ��������:	2015��07��31��
// ������:		  ( )
//    :	    
//--------------------------------------------------------------------

#ifndef __Record_OPT_H__
#define __Record_OPT_H__

#include "FsSnsLogic/ISnsLogic.h"
#include "../server/ISnsData.h"
#include "../server/IPubData.h"
#include "../server/IPubKernel.h"

// �������
bool _create_record(ISnsKernel *pKernel,    
					ISnsData* pPubData, 	   
					const char* recName, 	   
					int row_max, 	   
					const IVarList& col_types,   
					const bool IsSave = false);

// �������
bool _create_record(IPubKernel *pKernel, 	   
					IPubData* pPubData,   
					const char* recName,    
					int row_max,    
					const IVarList& col_types,   
					const bool IsSave = false);
//����ɱ��
bool _create_pub_record_safe(IPubData* pPubData, const char* recordName,
	IVarList& cols, IVarList& newColDefaultValue, int rowMax, int recordKey = -1, bool isSave = true);

// �жϱ�ṹ�Ƿ�ı�
bool check_record_changed(IRecord* pRecord, IVarList& cols, int rowMax);

// �жϱ�ṹ�Ƿ�һ��
bool is_same_record(IRecord* pDestRecord, IRecord* pSrcRecord);

// �������ṹ�޸�
// ֻ֧���к�����������
bool change_pub_record(IPubData* pPubData, const char* recordName, 
					   IVarList& cols, IVarList& newColDefaultValue, int rowMax, int recordKey = -1,bool isSave = true);

#endif //__Record_OPT_H__
