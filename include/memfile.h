//-------------------------------------------------------------------
// �� �� ����memfile.h
// ��    �ݣ��ڴ��ļ�����
// ˵    ����
// �������ڣ�2013��9��24��
// �� �� �ˣ�Liangxk
// ��Ȩ���У�������ţ�������޹�˾
//-------------------------------------------------------------------


#if !defined __MEMFILE_H__
#define __MEMFILE_H__


#include <stdio.h>
#include "../../public/faststr.h"

class MemFile
{
public:
	MemFile(const char* file);
	~MemFile();

public:

    //-----------------------------------------------------------
    /// \brief       �����Ϣ���ڴ��ļ�
    /// \param       [in] buff
    /// \note        
	bool Write(const char* buff);
    

    //-----------------------------------------------------------
    /// \brief       �����Ϣ���ڴ��ļ�
    /// \param       [in] wsBuf
    /// \note        
	bool Write(const wchar_t* wsBuf);


    //-----------------------------------------------------------
    /// \brief       ���滺�浽�����ļ�
    /// \param       [in] �޲���
    /// \note        
	int SaveToFile();


	const char* GetBuffer(int& size);

	void Close();
	void Open();
	bool Opened() { return m_pFile != 0; }

private:
	
	char* m_pData;		// ��������
	int m_nPos;			// ��ǰ�����α�
	int m_nTotalSize;	// �����ܴ�С
	TFastStr<char, 64> m_strFileName;	// �ļ�����
	FILE* m_pFile;		// �ļ�ָ��
};


#include "../../public/AutoMem.h"
#include "../../system/WinEncoding.h"
#include "../../public/Inlines.h"

#define MIN_BLOCK_SIZE 1024
#define DEFAULT_MEM_SIZE 8*1024




MemFile::MemFile(const char* file)
{
	m_strFileName = file;

	m_pData = NEW char[DEFAULT_MEM_SIZE];
	m_nPos = 0;
	m_nTotalSize = DEFAULT_MEM_SIZE;

	m_pFile = 0;
	Open();
}

MemFile::~MemFile()
{
	SaveToFile();

	if (m_pData)
	{
		delete[] m_pData;
	}

	if (m_pFile)
	{
		fclose(m_pFile);
	}
}

bool MemFile::Write(const char* str)
{
	if (str == 0 || m_pFile == 0)
	{
		return false;
	}

	int len = (int)strlen(str);

	// ֱ��д
	if (len > MIN_BLOCK_SIZE)
	{
		SaveToFile();
		fwrite(str, len, 1, m_pFile);
		return true;
	}

	if (m_nTotalSize-m_nPos < len)
	{
		if (!SaveToFile())
		{
			fwrite(str, len, 1, m_pFile);
			return true;
		}
	}

	// д���ڴ���
	memcpy(m_pData+m_nPos, str, len);

	m_nPos += len;

	return true;	
}

bool MemFile::Write(const wchar_t* wsStr)
{
	if (wsStr || m_pFile == 0)
	{
		return false;
	}

	size_t wslen = Port_GetWideStrToUTF8Len(wsStr);
	TAutoMem<char, 256> auto_buf1(wslen);
	char* data = auto_buf1.GetBuffer();
	Port_WideStrToUTF8(wsStr, data, wslen);

	return Write(data);
}

int MemFile::SaveToFile()
{
	if (m_pFile == 0 || m_nPos <= 0)
	{
		return 0;
	}

	fwrite(m_pData, m_nPos, 1, m_pFile);

	fflush(m_pFile);



	m_nPos = 0;

	return m_nPos;
}

void MemFile::Close()
{
	SaveToFile();

	if (m_pFile)
	{
		fclose(m_pFile);
		m_pFile = 0;
	}
}

void MemFile::Open()
{
	if (m_pFile == 0)
	{
		m_pFile = fopen(m_strFileName.c_str(), "w+");
	}
}

const char* MemFile::GetBuffer(int& size)
{
	size = m_nPos;
	return m_pData;
}

#endif // __MEMFILE_H__