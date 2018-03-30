//--------------------------------------------------------------------
// �ļ���:		ISnsData.h
// ��  ��:		
// ˵  ��:		
// ��������:	2008��11��11��	
// ������:		½����
// ��Ȩ����:	������ţ�������޹�˾
//--------------------------------------------------------------------

#ifndef _SERVER_ISNSDATA_H
#define _SERVER_ISNSDATA_H

#include "../public/Macros.h"
#include "../public/IVarList.h"
#include "IRecord.h"

// �������ݽӿ�

class ISnsData
{
public:
	virtual ~ISnsData() = 0;

	/// \brief �����������
	//virtual const wchar_t* GetName() = 0;
	/// \brief ���Ψһ��ʶ��
	virtual const char* GetUid() = 0;
	/// \brief �Ƿ���Ҫ����
	virtual bool GetSave() = 0;
	/// \brief �Ƿ�����޸�����
	virtual bool GetCanModify() = 0;
	
	//��������
	virtual bool AddAttr(const char * attr_name, int type, bool saving = false) = 0;
	//�Ƴ�����
	virtual bool RemoveAttr(const char * attr_name) = 0;	

	/// \brief ���������Ƿ񱣴�
	/// \param name ������
	/// \param value �Ƿ񱣴�
	virtual bool SetAttrSave(const char* name, bool value) = 0;

	/// \brief ��������
	/// \param name ������
	virtual bool FindAttr(const char* name) = 0;
	/// \brief �����������
	/// \param name ������
	virtual int GetAttrType(const char* name) = 0;
	/// \brief ��������Ƿ񱣴�
	/// \param name ������
	virtual bool GetAttrSave(const char* name) = 0;
	/// \brief �����������
	virtual int GetAttrCount() = 0;
	/// \brief ������������б�
	/// \param result ���ص��������б�
	virtual int GetAttrList(IVarList& result) = 0;
	/// \brief ��ѯ����ֵ
	/// \param name ������
	virtual int QueryAttrInt(const char* name) = 0;
	virtual int64_t QueryAttrInt64(const char* name) = 0;
	virtual float QueryAttrFloat(const char* name) = 0;
	virtual double QueryAttrDouble(const char* name) = 0;
	virtual const char* QueryAttrString(const char* name) = 0;
	virtual const wchar_t* QueryAttrWideStr(const char* name) = 0;
	virtual PERSISTID  QueryAttrObject(const char* name) = 0;

	/// \brief ��������ֵ
	/// \param name ������
	/// \param value ����ֵ
	virtual bool SetAttrInt(const char* name, int value) = 0;
	virtual bool SetAttrInt64(const char* name, int64_t value) = 0;
	virtual bool SetAttrFloat(const char* name, float value) = 0;
	virtual bool SetAttrDouble(const char* name, double value) = 0;
	virtual bool SetAttrString(const char* name, const char* value) = 0;
	virtual bool SetAttrWideStr(const char* name, const wchar_t* value) = 0;
	virtual bool SetAttrObject(const char* name, const PERSISTID& value) = 0;

	/// \brief ��ӱ�
	/// \param name ����
	/// \param row_max ���������Ϊ0��ʾ�����ƣ�
	/// \param cols ����
	/// \param col_types �������б�

	virtual bool AddRecord(const char* name, int row_max, int cols, 
		const IVarList& col_types) = 0;
	/// \brief ɾ����
	/// \param name ����
	virtual bool RemoveRecord(const char* name) = 0;
	/// \brief Ϊ������һ�����������������ͱ���Ϊ�ַ�������ַ�����
	/// \param name �����
	/// \param col �����к�
	/// \param case_insensitive �Ƿ��Сд������
	virtual bool SetRecordKey(const char* name, int col,
		bool case_insensitive = false) = 0;
	/// \brief ���ñ��Ƿ񱣴�
	/// \param name ����
	/// \param value �Ƿ񱣴�
	virtual bool SetRecordSave(const char* name, bool value) = 0;

	/// \brief ���������һ��
	/// \param name ����
	/// \param row �кţ�Ϊ-1��ʾ������У�
	virtual int AddRecordRow(const char* name, int row) = 0;
	/// \brief ���������һ�в���ʼ��
	/// \param name ����
	/// \param row �кţ�Ϊ-1��ʾ������У�
	/// \param value ������
	virtual int AddRecordRowValue(const char* name, int row, 
		const IVarList& value) = 0;
	/// \brief ��ɾ��һ��
	/// \param name ����
	/// \param row �к�
	virtual bool RemoveRecordRow(const char* name, int row) = 0;
	/// \brief �����
	/// \param name ����
	virtual bool ClearRecordRow(const char* name) = 0;
	
	/// \brief ��ñ����ʽӿ�
	/// \param name �����
	virtual IRecord* GetRecord(const char* name) = 0;

	/// \brief ���Ƿ����
	/// \param name ����
	virtual bool FindRecord(const char* name) = 0;
	/// \brief ��ñ�����
	virtual int GetRecordCount() = 0;
	/// \brief ��ñ������б�
	/// \param result ���صı�����б�
	virtual int GetRecordList(IVarList& result) = 0;
	/// \brief ��ñ��Ƿ񱣴�
	/// \param name ����
	virtual bool GetRecordSave(const char* name) = 0;
	/// \brief ���������
	/// \param name ����
	/// \param col �к�
	virtual int GetRecordColType(const char* name, int col) = 0;
	/// \brief ��ñ��������
	/// \param name ����
	virtual int GetRecordMax(const char* name) = 0;
	/// \brief ��ñ�����
	/// \param name ����
	virtual int GetRecordRows(const char* name) = 0;
	/// \brief ��ñ�����
	/// \param name ����
	virtual int GetRecordCols(const char* name) = 0;

	/// \brief ����ָ��������ı���
	/// \param name ����
	/// \param col �к�
	/// \param value ���ҵ���ֵ
	virtual int FindRecordInt(const char* name, int col, int value) = 0;
	virtual int FindRecordInt64(const char* name, int col, int64_t value) = 0;
	virtual int FindRecordFloat(const char* name, int col, float value) = 0;
	virtual int FindRecordDouble(const char* name, int col, double value) = 0;
	virtual int FindRecordString(const char* name, int col, 
		const char* value) = 0;
	virtual int FindRecordWideStr(const char* name, int col, 
		const wchar_t* value) = 0;
	virtual int FindRecordObject(const char* name, int col, 
		const PERSISTID& value) = 0;
	/// \brief ��Сд�����в���
	/// \param name ����
	/// \param col �к�
	/// \param value ���ҵ���ֵ
	virtual int FindRecordStringCI(const char* name, int col, 
		const char* value) = 0;
	virtual int FindRecordWideStrCI(const char* name, int col, 
		const wchar_t* value) = 0;

	/// \brief ��ѯһ�б�����
	/// \param name ����
	/// \param row �к�
	/// \param value ���ص�������
	virtual bool QueryRecordRowValue(const char* name, int row, 
		IVarList& value) = 0;
	/// \brief ��ѯ������
	/// \param name ����
	/// \param row �к�
	/// \param col �к�
	virtual int QueryRecordInt(const char* name, int row, int col) = 0;
	virtual int64_t QueryRecordInt64(const char* name, int row, int col) = 0;
	virtual float QueryRecordFloat(const char* name, int row, int col) = 0;
	virtual double QueryRecordDouble(const char* name, int row, int col) = 0;
	virtual const char* QueryRecordString(const char* name, 
		int row, int col) = 0;
	virtual const wchar_t* QueryRecordWideStr(const char* name, 
		int row, int col) = 0;
	virtual PERSISTID QueryRecordObject(const char* name, 
		int row, int col) = 0;

	/// \brief ����һ�б�����
	/// \param name ����
	/// \param row �к�
	/// \param value ������
	virtual bool SetRecordRowValue(const char* name, int row, 
		const IVarList& value) = 0;
	/// \brief ���ñ�����
	/// \param name ����
	/// \param row �к�
	/// \param col �к�
	/// \param value ��ֵ
	virtual bool SetRecordInt(const char* name, int row, int col, 
		int value) = 0;
	virtual bool SetRecordInt64(const char* name, int row, int col, 
		int64_t value) = 0;
	virtual bool SetRecordFloat(const char* name, int row, int col, 
		float value) = 0;
	virtual bool SetRecordDouble(const char* name, int row, int col, 
		double value) = 0;
	virtual bool SetRecordString(const char* name, int row, int col, 
		const char* value) = 0;
	virtual bool SetRecordWideStr(const char* name, int row, int col, 
		const wchar_t* value) = 0;
	virtual bool SetRecordObject(const char* name, int row, int col, 
		const PERSISTID& value) = 0;
};

inline ISnsData::~ISnsData() {}

#endif // _SERVER_IPUBDATA_H

