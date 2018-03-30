//--------------------------------------------------------------------
// �ļ���:		IRecord.h
// ��  ��:		�������ӿ�
// ˵  ��:		
// ��������:	2010��2��21��
// ������:		½����
// ��Ȩ����:	������ţ�������޹�˾
//--------------------------------------------------------------------

#ifndef _SERVER_IRECORD_H
#define _SERVER_IRECORD_H

#include "../public/Macros.h"
#include "../public/Var.h"
#include "../public/VarList.h"

// �������ӿ�

class IRecordIndex;

class IRecord
{
public:
	virtual ~IRecord() = 0;

	/// \brief ��ñ�����
	virtual int GetCols() const = 0;
	/// \brief ��ñ�����
	virtual int GetRows() const = 0;
	/// \brief ��ñ��������
	virtual int GetRowMax() const = 0;
	/// \brief �������������
	/// \param col �����
	virtual int GetColType(int col) const = 0;
	
	/// \brief ���һ�У����ز�����к�
	/// \param row �кţ�-1��ʾ�������ӣ�
	virtual int AddRow(int row) = 0;
	/// \brief ���һ�в���ʼ�������ز�����к�
	/// \param row �кţ�-1��ʾ�������ӣ�
	/// \param value �е�����
	virtual int AddRowValue(int row, const IVarList& value) = 0;
	/// \brief ɾ��һ��
	/// \param row �кţ�-1��ʾ�����ɾ����
	virtual bool RemoveRow(int row) = 0;
	/// \brief ������б�����
	virtual bool ClearRow() = 0;
	
	/// \brief дһ�еı�����
	/// \param row �к�
	/// \param value һ�е���ֵ
	virtual bool SetRowValue(int row, const IVarList& value) = 0;
	/// \brief д������
	/// \param obj ����
	/// \param row �к�
	/// \param col �к�
	/// \param value ��ֵ
	virtual bool SetInt(int row, int col, int value) = 0;
	virtual bool SetInt64(int row, int col, int64_t value) = 0;
	virtual bool SetFloat(int row, int col, float value) = 0;
	virtual bool SetDouble(int row, int col, double value) = 0;
	virtual bool SetString(int row, int col, const char* value) = 0;
	virtual bool SetWideStr(int row, int col, const wchar_t* value) = 0;
	virtual bool SetObject(int row, int col, const PERSISTID& value) = 0;
	
	/// \brief ��һ�б�����
	/// \param row �к�
	/// \param col �к�
	/// \param value ���ص�һ�б�����
	virtual bool QueryRowValue(int row, IVarList& value) = 0;
	/// \brief ��������
	/// \param row �к�
	/// \param col �к�
	virtual int QueryInt(int row, int col) = 0;
	virtual int64_t QueryInt64(int row, int col) = 0;
	virtual float QueryFloat(int row, int col) = 0;
	virtual double QueryDouble(int row, int col) = 0;
	virtual const char* QueryString(int row, int col) = 0;
	virtual const wchar_t* QueryWideStr(int row, int col) = 0;
	virtual PERSISTID QueryObject(int row, int col) = 0;
	
	/// \brief ����ָ���������ĳ�У�����-1��ʾû��
	/// \param col �к�
	/// \param value ��ֵ
	virtual int FindInt(int col, int value, int start_row = 0) = 0;
	virtual int FindInt64(int col, int64_t value, int start_row = 0) = 0;
	virtual int FindFloat(int col, float value, int start_row = 0) = 0;
	virtual int FindDouble(int col, double value, int start_row = 0) = 0;
	virtual int FindString(int col, const char* value, int start_row = 0) = 0;
	virtual int FindWideStr(int col, const wchar_t* value, 
		int start_row = 0) = 0;
	virtual int FindObject(int col, const PERSISTID& value, 
		int start_row = 0) = 0;
	/// \brief �����ִ�Сд����
	/// \param col �к�
	/// \param value ��ֵ
	virtual int FindStringCI(int col, const char* value, 
		int start_row = 0) = 0;
	virtual int FindWideStrCI(int col, const wchar_t* value, 
		int start_row = 0) = 0;


	// �������
	virtual bool AddRecordIndex(const char* name){ return true; }
	// ɾ������
	virtual bool RemoveRecordIndex(const char* name){ return true; }
	// ��ȡ�����ӿ�
	virtual IRecordIndex* GetRecordIndex(const char* name) const { return NULL; }
};

inline IRecord::~IRecord() {}

#endif // _SERVER_IRECORD_H
