//--------------------------------------------------------------------
// �ļ���:		base64.h
// ��  ��:		
// ˵  ��:		
// ��������:		2016��10��22��
// ������:		���
//--------------------------------------------------------------------

#ifndef __UTILS_BASE64_H__
#define __UTILS_BASE64_H__

#ifdef __cplusplus
extern "C"
{
#endif
	// Base64 ���롣Base64 �����ǰ�ÿ 3 ���ֽ�ת���� 4 ��ascii �ַ��������ַ���ӳ�䣩��
	// ���ı�����󣬶�����˵����ֱ���Ķ�����β����ʱ������һ������ "=" �ַ��Ĳ��롣
	size_t base64_encode(unsigned char* dst, const unsigned char* src, size_t srclen);
	size_t base64_decode(unsigned char* dst, const unsigned char* src, size_t srclen);
#ifdef __cplusplus
}
#endif

#endif // __UTILS_BASE64_H__
