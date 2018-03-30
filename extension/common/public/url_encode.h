//--------------------------------------------------------------------
// �ļ���:		url_encode.h
// ��  ��:		
// ˵  ��:		
// ��������:		2016��10��22��
// ������:		���
//--------------------------------------------------------------------

#ifndef __UTILS_URL_ENCODE_H__
#define __UTILS_URL_ENCODE_H__

#ifdef __cplusplus
extern "C"
{
#endif

	// UrlEncode ���ٷֺű��룩�������ǰ�������ַ������� UTF-8 ���룬Ȼ��ѻ����ַ�������ֽ��ðٷֺż�16���Ƶ���ʽ���롣
	// UrlEncode �����һ�����������ǣ��������е�16�����ַ��Ƿ���ô�д��ĸ��ʾ ����ֵ��Ҫ��utf8���� ���볤��*3
	//�ٷֺű��� 
	//http://zh.wikipedia.org/zh-cn/%E7%99%BE%E5%88%86%E5%8F%B7%E7%BC%96%E7%A0%81

	bool url_encode(unsigned char* dst, const unsigned char* src, size_t srclen);
	bool url_decode(unsigned char* dst, const unsigned char* src, size_t srclen);

#ifdef __cplusplus
}
#endif

#endif // __UTILS_BASE64_H__
