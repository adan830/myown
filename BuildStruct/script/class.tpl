//--------------------------------------------------------------------
// �ļ���:		Classes.h
// ��  ��:		��������ģ��
// ˵  ��:		�����ƶ���
// ������:		
// ��  ��:      	
//--------------------------------------------------------------------
#ifndef __CLASS_DEFINE_H__ 
#define __CLASS_DEFINE_H__

{{#class}}
//<class id="{{id}}" 
//	type="{{type}}" 
//	desc="{{desc}}" 
//	path="{{path}}" />
#ifndef CLASS_NAME_{{id:x-name}}
#define CLASS_NAME_{{id:x-name}}				"{{id}}"
#endif
{{/class}}

#endif // __CLASS_DEFINE_H__