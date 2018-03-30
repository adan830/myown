//--------------------------------------------------------------------
// �ļ���:		ICacheString.h
// ��  ��:		cache string,buffer,�����ַ�����������ȫʹ��
// ˵  ��:		
// ��������:	2017��06��02��
// ������:		kevin
// ��Ȩ����:	WhalesGame Technology co.Ltd
//--------------------------------------------------------------------
#ifndef __I_CACHE_STRING_H__
#define	__I_CACHE_STRING_H__

//namespace cache{

// interface: cache string.
struct ICacheString
{
public:
	~ICacheString(){}
public:
	/// \brief �����ַ�������
	virtual const char*  GetString() const = 0;
	virtual char*  GetBuffer() = 0;
public:
	/// \brief �ַ�����ֵ
	/// \param str �ַ���
	virtual bool SetString( const char* str ) = 0;
	/// \brief �ַ�����ֵ
	/// \param ref �ַ���
	virtual bool SetStringEx(const ICacheString& ref) = 0;
	/// \brief �ַ�����ֵ,��ʽ��
	/// \param format ��ʽ������
	virtual bool SetFormat(const char* format, ...) = 0;
public:
	/// \brief �ַ�׷��
	/// \param str �ַ���
	virtual bool AppendString(const char* str) = 0;
	/// \brief �ַ�׷��
	/// \param p_buff �ַ���
	virtual bool AppendStringEx(const ICacheString& p_buff) = 0;
	/// \brief �ַ�׷��,��ʽ��
	/// \param format ��ʽ������
	virtual bool AppendFormat(const char* format, ...) = 0;
	/// \brief �ַ�׷�� �س���
	virtual bool AppendLF() = 0;
public:
	/// \brief �ַ��ָ��ַ���
	/// \param str �ַ���
	/// \param split_pos �ָ��
	virtual bool AppendSplitString(const char* split_pos,const char* str) = 0;
	/// \brief �ַ��ָ��ַ���
	/// \param str �ַ���
	/// \param split_pos �ָ��
	virtual bool AppendSplitFormat(const char* split_pos,const char* format, ...) = 0;
public:
	/// \brief �Ƚϣ��ַ����Ƿ���ͬ
	/// \param str �ַ���
	virtual bool IsEqual( const char* str ) const = 0;
public:
	/// \brief �ַ���ת�� ��д
	virtual void toUpper() = 0;
	/// \brief �ַ���ת�� Сд
	virtual void toLower() = 0;
	/// \brief �ַ���ת�� ��ת
	virtual bool toReversal() = 0;
public:
	/// \brief �ַ����Ƿ�Ϊ��
	virtual bool IsEmpty() const = 0;
	/// \brief �ַ����Ƿ�Ϊ��
	/// \brief �ַ�������
	virtual unsigned int GetSize() const = 0;
	/// \brief �ַ�������
	virtual unsigned int GetCapacity() const = 0;
public:
	/// \brief �ַ������
	virtual void Clear() = 0;
};

// interface: cache buff.
struct ICacheBuff
{
public:
	~ICacheBuff(){}
public:
	/// \brief ���û�����
	/// \param p_buff �ַ���
	/// \param buff_len ����
	virtual bool SetBuff(const char* p_buff,unsigned int buff_len) = 0;
	/// \brief ׷�ӻ���
	/// \param p_buff �ַ���
	/// \param buff_len ����
	virtual bool AppendBuff(const char* p_buff, unsigned int buff_len) = 0;
public:
	/// \brief ���������
	virtual void Clear() = 0;
	/// \brief �Ƿ�Ϊ��
	virtual bool IsEmpty() const = 0;
public:
	/// \brief ���ػ�����
	virtual const char*  GetBuff() const = 0;
	// \brief ���ػ���������
	virtual unsigned int GetBuffSize() const = 0;
	// \brief ���ػ���������
	virtual unsigned int GetBufftCapacity() const = 0;
public:
	/// \brief ���ػ�����,�ɶ�д
	virtual char*  GetBuffRaw() = 0;
	// \brief ���ػ���������,�ɶ�д
	virtual unsigned int& GetBuffRawLen() = 0;
};

//}
#endif