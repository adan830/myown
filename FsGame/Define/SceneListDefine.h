//--------------------------------------------------------------------
// �ļ���:	    SceneListDefine.h
// ��  ��:		�����б���ض���
// ˵  ��:		
// ��������:	2015��7��28��
// ������:		 
// �޸���:     
//    :	    
//--------------------------------------------------------------------

#ifndef __SCENE_LIST_H__
#define __SCENE_LIST_H__

// ��¼�����������ϵ�һ�����صĳ���
const char* const SERVER_MEMBER_FIRST_LOAD_SCENE_REC = "member_scene_record";
enum
{
	SERVER_MEMBER_FIRST_LOAD_SCENE_COL_member_id,		// ����������member ID
	SERVER_MEMBER_FIRST_LOAD_SCENE_COL_scene_id,		// ����ID
	SERVER_MEMBER_FIRST_LOAD_SCENE_COL_TOTAL,
};

// �������
const int SERVER_MEMBER_FIRST_LOAD_SCENE_REC_MAX_ROW = 32;

#endif // __SCENE_LIST_H__