
//--------------------------------------------------------------------
// �ļ���:      \utils\string_buffer.h
// ��  ��:      
// ˵  ��:      StringBuffer������������һ�������Ľϴ��ڴ棬���ڶԼ��غ�
//              �����޸ĵ�Ƶ����ѯ���ַ����洢�����Ա���ʹ��std::string��ɵ�
//              �ڴ��˷�����
//              StringArray�����滻CVarList�洢�ַ���ʹ�á�StringArray��
//              ��һ��StringBufferָ���һ����̬���飬�������鱣���ַ�����
//              StringBuffer�е�ƫ��λ�ã�����Ҫ�����ַ���ʱ�������������ҵ�
//              ƫ�Ƶ�ַ���ٽ���StringBufferȥ��ȡ��Ӧ���ַ���ֵ��
//
//              �����ԣ��������ü�������Ҫ�õ�std::string��CVarList������£�
//              ����StringBuffer��StringArray��������70%���ϵĴ洢�ռ䡣����
//              �������Լ����������ݼ��ص�ǰ��Աȷ�����
// ��������:    2014��9��17��
// ������:      longch
//    :       
//--------------------------------------------------------------------


#ifndef __STRING_BUFFER_H__
#define __STRING_BUFFER_H__

#include "public/VarList.h"

class StringBuffer
{
public:
    static const size_t STRING_BUFFER_INVALID_INDEX;

    StringBuffer();
    ~StringBuffer();

    bool Init(size_t init_size, size_t grow_size);

    size_t GetSize() const
    {
        return m_nSize;
    }
    size_t GetCapacity() const
    {
        return m_nCapacity;
    }
    bool IsInit() const
    {
        return m_bInitialize;
    }
    size_t GetLastStringPos() const
    {
        return m_nLastStringPos;
    }

    const char* Get(size_t offset) const;
    
    size_t Add(const char* ss);

    size_t GetMemoryUsage() const
    {
        size_t size = sizeof(*this) + m_nCapacity;
        return size;
    }

    size_t GetTotalStringLength() const
    {
        return m_nBufferPos;
    }
private:
    char* GetBuffer(size_t offset);

private:
    StringBuffer(const StringBuffer& ss);
    StringBuffer operator=(const StringBuffer& ss);
public:
    bool m_bInitialize;
    size_t m_nCapacity;
    size_t m_nSize;
    size_t m_nGrowSize;

    size_t m_nLastStringPos;
    size_t m_nBufferPos;
    char* m_pBuffer;
}; // end of class StringBuffer


class StringAtom
{
public:
    StringAtom(const char* ss = NULL)
    {
        m_str = ss;
    }

    StringAtom(const StringAtom& ss)
    {
        m_str = ss.m_str;
    }

    size_t length() const
    {
        if (NULL == m_str)
        {
            return 0;
        }
        return strlen(m_str);
    }
    size_t size() const
    {
        return length();
    }
    size_t capacity() const
    {
        return length();
    }
    const char* c_str() const
    {
        return m_str;
    }

private:
    const char* m_str;
};

class StringArray
{
public:
    StringArray();
    StringArray(const StringArray& src);
    StringArray& operator=(const StringArray& src);
    ~StringArray();

    bool Init(size_t init_size, size_t grow_size, StringBuffer* string_buffer);

    size_t GetCount() const
    {
        return m_nCount;
    }
    size_t GetCapacity() const
    {
        return m_nCapacity;
    }
    bool IsInitialize() const
    {
        return m_bInitialize;
    }

    // StringArray��CVarList�໥ת��
    void AddVarList(const IVarList& value);
    void ConvertToVarList(IVarList& value) const;

    // Add����ǰ����Ҫ����Init��������true֮�󣬲��ܱ�֤Add�����ǰ�ȫ��
    const char* Add(const char* value);

    bool Find(const char* value);

    const char* StringVal(size_t index) const;

    StringArray& operator<<(const char* value) 
    { 
        Add(value); 
        return *this; 
    }

    size_t GetMemoryUsage() const
    {
        size_t size = sizeof(*this) + m_nCapacity;
        return size;
    }

private:

    struct ArrayIndexNode 
    {
        size_t offset;// ����StringBuffer��ƫ��ֵ����Ҫ����ʵ��ָ�룬��ֹ�ڴ���������󣬵�ַ��Ч
        ArrayIndexNode() : offset(-1) {}
        bool IsValid() const {return (size_t)StringBuffer::STRING_BUFFER_INVALID_INDEX != offset;}
    };

    ArrayIndexNode* GetNode();

private:

    bool m_bInitialize;
    size_t m_nCapacity;
    size_t m_nCount;
    size_t m_nGrowSize;

    ArrayIndexNode* m_pArray;

    StringBuffer* m_pStringBufferRef;
}; // end of class StringArray


#endif // __STRING_BUFFER_H__
