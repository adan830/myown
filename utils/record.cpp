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

#include "record.h"
#include "extend_func.h"
#include "string_util.h"

// �������
bool _create_record(ISnsKernel* pKernel, 
					ISnsData* pPubData,
					const char* recName,
					int row_max, 
					const IVarList& col_types,
					const bool IsSave)
{
	//������
	if (pPubData->FindRecord(recName))
	{
		bool ReBuild = false;
		IRecord* pRec = pPubData->GetRecord(recName);
		if (NULL == pRec)
		{
			ReBuild = true;
		}
		else
		{
			// �����ݱ��������ԣ�ɾ���ɱ�
			if (pRec->GetRowMax() != row_max)
			{
				ReBuild = true;
			}

			if (pPubData->GetRecordCols(recName) != (int)col_types.GetCount())
			{
				ReBuild = true;
			}
			else
			{
				LoopBeginCheck(a)
				for (int i = 0; i< (int)col_types.GetCount(); ++i)
				{
					LoopDoCheck(a)
					int iType = pPubData->GetRecordColType(recName, i);
					if (iType != col_types.IntVal(i))
					{
						ReBuild = true;
					}
				}
			}
		}

		// �����ݱ��������ԣ�ɾ���ɱ�
		if (ReBuild)
		{
			pPubData->RemoveRecord(recName);
			pPubData->AddRecord(recName, row_max, (int)col_types.GetCount(), col_types);
		}
	}
	else
	{
		pPubData->AddRecord(recName, row_max, (int)col_types.GetCount(), col_types);
	}

	pPubData->SetRecordSave(recName, IsSave);

	return true;
}

// �������
bool _create_record(IPubKernel* pKernel, 
					IPubData* pPubData,
					const char* recName,
					int row_max, 
					const IVarList& col_types,
					const bool IsSave)
{
	//������
	if (pPubData->FindRecord(recName))
	{
		bool ReBuild = false;
		IRecord* pRec = pPubData->GetRecord(recName);
		if (NULL == pRec)
		{
			ReBuild = true;
		}
		else
		{
			// �����ݱ��������ԣ�ɾ���ɱ�
			if (pRec->GetRowMax() != row_max)
			{
				ReBuild = true;
			}

			if (pPubData->GetRecordCols(recName) != (int)col_types.GetCount())
			{
				ReBuild = true;
			}
			else
			{
				LoopBeginCheck(b)
				for (int i = 0; i< (int)col_types.GetCount(); ++i)
				{
					LoopDoCheck(b)
					int iType = pPubData->GetRecordColType(recName, i);
					if (iType != col_types.IntVal(i))
					{
						ReBuild = true;
					}
				}
			}
		}

		// �����ݱ��������ԣ�ɾ���ɱ�
		if (ReBuild)
		{
			pPubData->RemoveRecord(recName);
			pPubData->AddRecord(recName, row_max, (int)col_types.GetCount(), col_types);
		}
	}
	else
	{
		pPubData->AddRecord(recName, row_max, (int)col_types.GetCount(), col_types);
	}

	pPubData->SetRecordSave(recName, IsSave);

	return true;
}
bool _create_pub_record_safe(IPubData* pPubData, const char* recordName,
	IVarList& cols, IVarList& newColDefaultValue, int rowMax, int recordKey, bool isSave)
{



	//������
	if (pPubData->FindRecord(recordName))
	{
		change_pub_record(pPubData, recordName, cols, newColDefaultValue, rowMax, recordKey, isSave);
	}
	else
	{
		pPubData->AddRecord(recordName, rowMax, (int)cols.GetCount(), cols);
		if (recordKey != -1){
			pPubData->SetRecordKey(recordName, recordKey);
		}
		pPubData->SetRecordSave(recordName, isSave);
	}

	return true;


}

// ����ṹ�Ƿ�ı�
bool check_record_changed(IRecord* pRecord, IVarList& cols, int rowMax)
{
	if (pRecord == NULL)
	{
		return false;
	}

	// ��������ı�
	if (pRecord->GetRowMax() != rowMax)
	{
		return true;
	}

	// ��������ı�
	int colsCount = pRecord->GetCols();
	if ( colsCount != static_cast<int>(cols.GetCount()))
	{
		return true;
	}

	// �������Ͳ�һ��
	LoopBeginCheck(a);
	for (int i = 0; i < colsCount; ++i)
	{
		LoopDoCheck(a);

		if (pRecord->GetColType(i) != cols.IntVal(i))
		{
			return true;
		}
	}

	return false;
}

// �жϱ�ṹ�Ƿ�һ��
bool is_same_record(IRecord* pDestRecord, IRecord* pSrcRecord)
{
	if (pDestRecord == NULL
		|| pSrcRecord == NULL)
	{
		return false;
	}

	// �������
	if (pDestRecord->GetRowMax() != pSrcRecord->GetRowMax())
	{
		return false;
	}

	// �������
	int colsCount = pDestRecord->GetCols();
	if ( colsCount != pSrcRecord->GetCols())
	{
		return false;
	}

	// �������Ͳ�һ��
	LoopBeginCheck(a);
	for (int i = 0; i < colsCount; ++i)
	{
		LoopDoCheck(a);

		if (pDestRecord->GetColType(i) != pSrcRecord->GetColType(i))
		{
			return false;
		}
	}

	return true;
}

// �������ṹ�޸�
// ֻ֧���к�����������
bool change_pub_record(IPubData* pPubData, const char* recordName, 
	IVarList& cols, IVarList& newColDefaultValue, int rowMax, int recordKey, bool isSave)
{
	if (pPubData == NULL
		|| StringUtil::CharIsNull(recordName))
	{
		return false;
	}

	// ԭʼ��
	IRecord* pRecord = pPubData->GetRecord(recordName);
	if (pRecord == NULL)
	{
		return false;
	}

	// ��ṹ�Ƿ�һ��
	if (!check_record_changed(pRecord, cols, rowMax))
	{
		return true;
	}

	// ������ʱ��
	std::string tempRecordName = "temp_";
	tempRecordName.append(recordName);
	if (!pPubData->AddRecord(tempRecordName.c_str(), 
		rowMax, static_cast<int>(cols.GetCount()), cols))
	{
		return false;
	}

	// ��ȡ��ʱ��
	IRecord* pTempRecord = pPubData->GetRecord(tempRecordName.c_str());
	if (pTempRecord == NULL)
	{
		return false;
	}

	// ��ԭʼ������ת�Ƶ���ʱ����
	CVarList rowValueList;
	LoopBeginCheck(a);
	int rows = pRecord->GetRows();
	for (int i = 0; i < rows; ++i)
	{
		LoopDoCheck(a);

		// ȡԭʼ������
		rowValueList.Clear();
		pRecord->QueryRowValue(i, rowValueList);

		// ����������
		rowValueList << newColDefaultValue;

		// ��ӵ���ʱ����
		pTempRecord->AddRowValue(-1, rowValueList);
	}

	// ɾ��ԭʼ��
	if (!pPubData->RemoveRecord(recordName))
	{
		return false;
	}

	// �ؽ�ԭʼ��
	if (!pPubData->AddRecord(recordName, 
		rowMax, static_cast<int>(cols.GetCount()), cols))
	{
		return false;
	}

	// ���»�ȡԭʼ��
	pRecord = pPubData->GetRecord(recordName);
	if (pRecord == NULL)
	{
		return false;
	}
	if (recordKey != -1){
		pPubData->SetRecordKey(recordName, recordKey);
	}
	pPubData->SetRecordSave(recordName, isSave);

	// ����ʱ������ת�Ƶ��µı���
	LoopBeginCheck(b);
	rows = pTempRecord->GetRows();
	for (int i = 0; i < rows; ++i)
	{
		LoopDoCheck(b);

		// ȡ��ʱ��������
		rowValueList.Clear();
		pTempRecord->QueryRowValue(i, rowValueList);

		// ��ӵ��±���
		pRecord->AddRowValue(-1, rowValueList);
	}

	// ɾ����ʱ��
	pPubData->RemoveRecord(tempRecordName.c_str());

	return true;
}
