//-------------------------------------------------------------------- 
// �ļ���:		ISheet.h 
// ��  ��:		
// ˵  ��:		
// ��������:	2006��4��6��	
// ������:		½����
// ��Ȩ����:	������ţ�������޹�˾
//--------------------------------------------------------------------
/*
#ifndef _SERVER_ISHEET_H
#define _SERVER_ISHEET_H

#include "../public/Macros.h"
#include "../public/IVarList.h"

// ֻ�����ݱ�ӿ�

class ISheet
{
public:
	virtual ~ISheet() = 0;
	
	/// \brief �Ƿ���Ա��޸�
	virtual void SetCanModify(bool value) = 0;
	virtual bool GetCanModify() const = 0;
	
	/// \brief ��ñ�����
	virtual const char* GetName() const = 0;
	/// \brief ��Ӽ�¼��
	/// \param key ����
	/// \param row_data һ������
	virtual bool AddRow(const char* key, const IVarList& row_data) = 0;
	/// \brief ��ü�¼����
	virtual size_t RowCount() const = 0;
	/// \brief ����еĹؼ���
	/// \param row �к�
	virtual const char* GetKey(size_t row) const = 0;
	/// \brief ���ָ����������
	/// \param row �к�
	virtual const IVarList* GetRow(size_t row) const = 0;
	/// \brief ����ָ������������
	/// \param key ����
	virtual const IVarList* FindRow(const char* key) const = 0;
	
	/// \brief ����ӱ�
	/// \param name �ӱ���
	virtual ISheet* AddSubSheet(const char* name) = 0;
	/// \brief ����ӱ�����
	virtual size_t SubSheetCount() const = 0;
	/// \brief ���ָ�����ӱ�
	/// \param index �ӱ�����
	virtual ISheet* GetSubSheet(size_t index) const = 0;
	/// \brief �����ӱ�
	/// \param name �ӱ���
	virtual ISheet* FindSubSheet(const char* name) const = 0;
};

inline ISheet::~ISheet() {}

#endif // _SERVER_ISHEET_H
*/
