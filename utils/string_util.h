//--------------------------------------------------------------------
// �ļ���:		Code\FxGameLogic\utils\StringUtil.h
// ��  ��:		�ַ���������
// ˵  ��:		
// ��������:	
// ������:		
//    :	   
//--------------------------------------------------------------------
#ifndef _STRINGUTIL_H
#define _STRINGUTIL_H

#include "public/Macros.h"
#include "public/IVarList.h"
#include <string>
#include <map>

// �ַ�������

class StringUtil
{
public:

    //�ַ����Ƿ�Ϊ��
    static bool	CharIsNull(const char* strChar);
	static bool	CharIsNull(const wchar_t* strChar);
	//��Ʒ���ǿ�
	static bool ItemIsNull(const char* strConfig);

	//�Ƚ�
	static bool CharIsEqual(const char* srcChar, const char* dstChar);
	static bool CharIsEqual(const wchar_t* srcChar, const wchar_t* dstChar);

	// �ַ���ת���ɿ��ַ���
	static std::wstring StringAsWideStr(const char* s);
	// �ַ���ת����utf8
	static std::string StringAsUtf8String(const char* s);
	// utf8ת�ַ���
	static std::string Utf8StringAsString(const char* s);
	// ��ÿ��ַ���ת�����ַ�����ĳ���
	static std::string WideStrAsString(const wchar_t* ws);
    // ��ÿ��ַ���ת����UTF8�ַ���
    static std::string WideStrAsUTF8String(const wchar_t* ws);
	// ���UTF8�ַ������ɿ��ַ���ת
	static std::wstring UTF8StringAsWideStr(const char* s);

	//��һ���ַ���ת����const char*,����䴮Ϊ���򷵻�def_value
	static const char* ParseStrAsStr(const char* str, const char* def_value);
	//��һ���ַ���ת����long,����䴮Ϊ���򷵻�def_value
	static long ParseStrAsLong(const char* str, long def_value);
	//��һ���ַ���ת����float,����䴮Ϊ���򷵻�def_value
	static float ParseStrAsFloat(const char* str, float def_value);
	//��һ���ַ���ת����bool,����䴮Ϊ���򷵻�def_value
	static bool ParseStrAsBool(const char* str, bool def_value);

	// ����ת��Ϊ�ַ���
	static std::string IntAsString(int value);
    static std::string UIntAsString(unsigned int value);
    static std::string Int64AsString(int64_t value);

	// ������ת��Ϊ�ַ���
	static std::string FloatAsString(float value, int decimals = 0);
    // ˫������ת�����ַ���
    static std::string DoubleAsString(double value, int decimals = 0);

	// �ַ���ת��Ϊ����
	static int StringAsInt(const char* value, int def_value = 0);
	// �ַ���ת��Ϊ64����
	static int64_t StringAsInt64(const char* value);
	// ���ַ���ת��Ϊ����
	static int64_t WideStrAsInt64(const wchar_t* value);
	// �ַ���ת��Ϊ������
	static float StringAsFloat(const char* value, float def_value = 0.0f);
	// �ַ���ת��Ϊ����
	static int StringAsInt(const std::string& value, int def_value = 0);
	// �ַ���ת��Ϊ������
	static float StringAsFloat(const std::string& value, float def_value = 0.0f);
	// ����ת��Ϊ���ַ���
	static std::wstring IntAsWideStr(int value);
	// ������ת��Ϊ���ַ���
	static std::wstring FloatAsWideStr(float value, int decimals = 0);
	// ���ַ���ת��Ϊ����
	static int WideStrAsInt(const wchar_t* value);
	// ���ַ���ת��Ϊ������
	static float WideStrAsFloat(const wchar_t* value);
	// ���ַ���ת��Ϊ����
	static int WideStrAsInt(const std::wstring& value);
	// ���ַ���ת��Ϊ������
	static float WideStrAsFloat(const std::wstring& value);

	// ת��Ϊ��д���ַ���
	static std::string ToUpper(const char* str);

	// ת��ΪСд���ַ���
	static std::string ToLower(const char* str);

	// ȡ�Ӵ���pos��0��ʼ
	static std::string Substr(const char* str, size_t pos, size_t len);

	//��str�ַ������pat������replace
	static void StrReplace(std::string& str,const std::string& pat,const std::string& replace);

	//ȥ��str�ַ�����ǰ���ָ���ַ�
    static std::string Trim(const std::string& str, const char ch = ' ');

	// ��ָ�����ַ��ָ��ַ���
	static size_t SplitString(const char* str, char splitter, char* buffer, 
		size_t buffer_size, const char* result[], size_t result_max);

	// ��ָ�����ַ����������ָ��ַ���
    static void SplitString(IVarList & result, const std::string& str,
        const std::string& delims, unsigned int max_splits = 0);

	// ��ָ�����ַ����е������ַ����ָ��ַ���
    static void SplitStringOf(IVarList & result, const std::string& str,
        const std::string& delims = "/\\", unsigned int max_splits = 0);

	// ȡ��ָ���ַ����ָ����Ӵ��ֽ⵽Vector��
	static size_t ParseToVector(const char* str, char splitter, IVarList& result);
	// [add by lihl] ȡ��ָ�����ַ����ָ����ִ��ֽ⵽Map��<splitter1Ϊ��һ�ηָ���splitter2Ϊ��һ�ηָ����Ӵ��ٴηָ���)
	static size_t ParseToMap(const char* str, char splitter1, char splitter2, std::map<std::string, std::string>& result);


	// ���ַ��汾�ĺ�����ͬ��
	static std::wstring ToUpperW(const wchar_t* str);
	static std::wstring ToLowerW(const wchar_t* str);
	static std::wstring SubstrW(const wchar_t* str, size_t pos, size_t len)	;

	static void StrReplaceW(std::wstring& str,const std::wstring& pat, const std::wstring& replace);

    static std::wstring TrimW(const std::wstring& str, const wchar_t ch = ' ');

	static size_t SplitStringW(const wchar_t* str, wchar_t splitter, 
		wchar_t* buffer, size_t buffer_size, const wchar_t* result[], size_t result_max);

	// ��ָ�����ַ����������ָ��ַ���
    static void SplitStringW(IVarList & result, const std::wstring& str,
        const std::wstring& delims, unsigned int max_splits = 0);

	// ��ָ�����ַ����е������ַ����ָ��ַ���
    static void SplitStringOfW(IVarList & result, const std::wstring& str,
        const std::wstring& delims = L"/\\", unsigned int max_splits = 0);

	static size_t ParseToVectorW(const wchar_t* str, wchar_t splitter, IVarList& result);

	// varlistת��utf8�ַ���
	static std::string ConvertUtf8String(const IVarList& data, size_t nIndex);

	// �ж��ַ����Ƿ�����ĸ�����ֵ����
	static bool StringIsAlnum(const char* pszStr);
};

#endif// _STRINGUTIL_H
