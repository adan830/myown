#ifndef _Level_Define_H_
#define _Level_Define_H_

#define PUB_WORLD_LEVEL	 "pub_world_level"		 // ����������ȼ�

// ����Ϣ
enum WorldLevelC2SMsg
{
	WLM_C2S_QUERY_ADD_EXP_DATA = 0,		// ��ѯ����ȼ����Ӿ�������
};

enum WorldLevelS2CMsg
{
	/*!
	* @brief	�ظ�����ȼ����Ӿ�������
	* @param	int 	����ȼ�
	*/
	WLM_S2C_REV_ADD_EXP_DATA = 0,
};
#endif