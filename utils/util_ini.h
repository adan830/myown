//--------------------------------------------------------------------
// �ļ���:		ReadIni.h
// ��  ��:		
// ˵  ��:		
// ��������:	2006��1��30��
// ������:		 
//    :	   
//--------------------------------------------------------------------

#ifndef __UTIL_INI_H__
#define __UTIL_INI_H__
#include "../public/Macros.h"
#include "../public/IVarList.h"
#include "extend_func.h"

#include <vector>
#include <string>

// CUtilIni 
// ��ȡ�����ļ�

class CUtilIni
{
private:
	struct item_t
	{
		const char* strName;
		unsigned int nHash;
		const char* strValue;
	};
	
	struct section_t
	{
		const char* strName;
		unsigned int nHash;
		size_t nItemStart;
		size_t nItemCount;
	};

public:
	enum { READ_RAW_FILE = 1 };

	static CUtilIni* NewInstance(const char* filename);
	
public:
	CUtilIni();
	CUtilIni(const char* filename);
	~CUtilIni();

	// ɾ��
	void Release();

	// �����ļ���
	void SetFileName(const char* filename);
	// ����ļ���
	const char* GetFileName() const;
	// �����ļ�
	bool LoadFromFile();

	// ���Ҷ�
	bool FindSection(const char* section) const;
	// ���Ҷε�����ֵ
	bool FindSectionIndex(const char* section, size_t& index) const;
	// ����ָ�����еļ�����ֵ
	bool FindSectionItemIndex(size_t sect_index, const char* key, 
		size_t& item_index) const;
	// ��öε�����
	size_t GetSectionCount() const;
	// ���ָ������
	const char* GetSectionByIndex(size_t index) const;
	// ��ö����б�
	//size_t GetSectionList(IVarList& result) const;
	// ���ָ�����µļ�����
	size_t GetSectionItemCount(size_t sect_index) const;
	// ���ָ�����µ�ָ������
	const char* GetSectionItemKey(size_t sect_index, size_t item_index) const;
	// ���ָ�����µ�ָ����ֵ
	const char* GetSectionItemValue(size_t sect_index, size_t item_index) const;

	// ��ö�������ָ������ֵ�б�
	size_t GetItemValueList(size_t sect_index, const char* key,
		IVarList& result) const;

	// ��ȡ�����ݣ����������ڵ�ʱ�򷵻�ȱʡֵ(def)
	int ReadInteger(size_t sect_index, const char* key, int def) const;
	char* ReadString(size_t sect_index, const char* key, char* value, 
		size_t maxlen) const;
	const char* ReadString(size_t sect_index, const char* key, 
		const char* def) const;
	float ReadFloat(size_t sect_index, const char* key, float def) const;

    size_t GetSectionList(IVarList& result) const;
	// ���Ҷ��µļ�
	bool FindItem(const char* section, const char* key) const;
	// ��ö������м�������
	size_t GetItemCount(const char* section) const;
	// ��ö������м����б�
	size_t GetItemList(const char* section, IVarList& result) const;
	// ��ö�������ָ������ֵ�б�
	size_t GetItemValueList(const char* section, const char* key,
		IVarList& result) const;
	
	// ��ȡ�����ݣ����������ڵ�ʱ�򷵻�ȱʡֵ(def)
	int ReadInteger(const char* section, const char* key, int def) const;
	char* ReadString(const char* section, const char* key, char* value, 
		size_t maxlen) const;
	const char* ReadString(const char* section, const char* key, 
		const char* def) const;
	float ReadFloat(const char* section, const char* key, float def) const;

private:
	const char* GetData(const char* section, const char* key) const;
    const char* GetData(const size_t sect_index, const char* key) const;

	const CUtilIni::section_t* GetSection(const char* name) const;
	const CUtilIni::item_t* GetItem(const section_t* pSection, 
		const char* name) const;

private:
	fast_string m_strFileName;
	char* m_pContent;
	std::vector<section_t> m_Sections;
	std::vector<item_t> m_Items;
};

#endif // __UTIL_INI_H__
