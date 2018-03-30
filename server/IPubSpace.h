//--------------------------------------------------------------------
// �ļ���:		IPubSpace.h
// ��  ��:		
// ˵  ��:		
// ��������:	2008��11��11��	
// ������:		½����
// ��Ȩ����:	������ţ�������޹�˾
//--------------------------------------------------------------------

#ifndef _SERVER_IPUBSPACE_H
#define _SERVER_IPUBSPACE_H

#include "../public/Macros.h"
#include "../public/IVarList.h"
#include "IPubData.h"

// �������ݿռ�ӿ�

class IPubSpace
{
public:
	virtual ~IPubSpace() = 0;
	
	/// \brief ��ù������ݿռ���
	virtual const char* GetSpaceName() = 0;
	/// \brief ������ݿ����
	virtual const char* GetTableName() = 0;
	/// \brief �Ƿ�����޸�����
	virtual bool GetCanModify() = 0;

	/// \brief ���ҹ���������
	/// \param name ������������
	virtual bool FindPubData(const wchar_t* name) = 0;
	/// \brief ��ù�������������
	virtual int GetPubDataCount() = 0;
	/// \brief ��ù��������������б�
	/// \param result ���صĹ������������б�
	virtual int GetPubDataList(IVarList& result) = 0;
	/// \brief ���ָ�����ֵĹ���������
	/// \param name ������������
	virtual IPubData* GetPubData(const wchar_t* name) = 0;
	
	/// \brief ��ӹ���������
	/// \param name ������������
	virtual bool AddPubData(const wchar_t* name, bool save) = 0;
	/// \brief ɾ������������
	/// \param name ������������
	virtual bool RemovePubData(const wchar_t* name) = 0;
};

inline IPubSpace::~IPubSpace() {}

#endif // _SERVER_IPUBSPACE_H

