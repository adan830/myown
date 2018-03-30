//--------------------------------------------------------------------
// �ļ���:		IniFile.h
// ��  ��:		
// ˵  ��:		
// ��������:	2006��1��30��
// ������:		 
//    :	   
//--------------------------------------------------------------------

#ifndef _UTILS_INIFILE_H
#define _UTILS_INIFILE_H

#include "../public/Macros.h"
#include "../public/IVarList.h"
#include "../public/FastStr.h"
#include "../utils/ArrayPod.h"

// ��д�����ļ�

class CIniFile
{
protected:
	struct item_t
	{
		size_t nNamePos;
		unsigned int nHash;
		size_t nValuePos;
	};

	struct section_t
	{
		size_t nNamePos;
		unsigned int nHash;
		size_t nItemStart;
		size_t nItemCount;
	};

public:
	enum { READ_RAW_FILE = 1 };

	static CIniFile* NewInstance(const char* filename);

public:
	CIniFile();
	CIniFile(const char* filename);
	CIniFile(const char* filename, int read_raw_file);
	~CIniFile();

	// ɾ��
	void Release();

	// �����ļ���
	void SetFileName(const char* filename);
	// ����ļ���
	const char* GetFileName() const;
	// �����ļ�
	bool LoadFromFile();
	// �����ַ���
	bool LoadFromString(const char* str, size_t size);
	// �����ļ�
	bool SaveToFile() const;

	// ��Ӷ�
	bool AddSection(const char* section);
	// ���Ҷ�
	bool FindSection(const char* section) const;
	// ɾ����
	bool DeleteSection(const char* section);
	// ��öε�����
	size_t GetSectionCount() const;
	// ��ö����б�
	size_t GetSectionList(IVarList& result) const;

	// ���Ҷ��µļ�
	bool FindItem(const char* section, const char* key) const;
	// ɾ����
	bool DeleteItem(const char* section, const char* key);
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

	// ��д�����ݣ�����λ������������ӣ�
	bool WriteInteger(const char* section, const char* key, int value);
	bool WriteString(const char* section, const char* key, const char* value);
	bool WriteFloat(const char* section, const char* key, float value);

	// ����п����ظ��ļ����ݣ�����β���������ӣ�
	bool AddInteger(const char* section, const char* key, int value);
	bool AddString(const char* section, const char* key, const char* value);
	bool AddFloat(const char* section, const char* key, float value);

	// ����ڴ�ռ��
	size_t GetMemoryUsage() const;
	
private:
	bool Build(char* buffer, size_t buffer_size);
	bool FindSectionIndex(const char* section, size_t& index) const;
	size_t AddToBuffer(const char* val);
	bool SetData(const char* section, const char* key, const char* data);
	bool AddData(const char* section, const char* key, const char* data);
	const char* GetData(const char* section, const char* key) const;
	const CIniFile::section_t* GetSection(const char* name) const;
	const CIniFile::item_t* GetItem(const section_t* pSection, 
		const char* name) const;

protected:
	TFastStr<char, 32> m_strFileName;
	int m_nReadRawFile;
	char* m_pBuffer;
	size_t m_nLength;
	TArrayPod<section_t, 8> m_Sections;
	TArrayPod<item_t, 8> m_Items;
};

#endif // _UTILS_INIFILE_H
