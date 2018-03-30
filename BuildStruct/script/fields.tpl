//--------------------------------------------------------------------
// �ļ���:		Fields.h
// ��  ��:		�ֶ�������
// ˵  ��:		�ֶ����ƶ���
// ������:		
// ��  ��:      
//--------------------------------------------------------------------
#ifndef _FIELDS_DEFINE_H_
#define _FIELDS_DEFINE_H_

// �ֶζ���===============================================================
{{#properties}}
//<property name="{{name}}" 
// 	type="{{type}}" 
//	public="{{public}}" 
//	private="{{private}}" 
//	save="{{save}}" 
//	desc="{{desc}}"/>
#define FIELD_PROP_{{name:x-name}}			"{{name}}"
{{/properties}}

// ����==============================================================
{{#records}}
{{#attributes}}
//<record name="{{name}}"
//	cols="{{cols}}" 
//	maxrows="{{maxrows}}" 
//	public="{{public}}" 
//	private="{{private}}" 
//	save="{{save}}" 
//	desc="{{desc}}"/>
{{/attributes}}
#define FIELD_RECORD_{{rec_name:x-name}}			"{{rec_name}}"
// {{rec_name}} column index define
enum COLUMNS_OF_{{rec_name:x-name}}
{
{{#columns}}
	// <column type="{{type}}" desc="{{desc}}"/>
	COLUMN_{{rec_name:x-name}}_{{name:x-name}},
{{/columns}}
};
{{/records}}

#endif // _FIELDS_DEFINE_H_