//--------------------------------------------------------------------
// �ļ���:      RankListDefine
// ��  ��:      ���а�ͷ�ļ�
// ˵  ��:      ����������а�
// ��������:    2014-10-15
// ������:      liumf
// ��Ȩ����:    ������ţ���ֿƼ��ɷ����޹�˾
//--------------------------------------------------------------------
#ifndef _RANKLISTDEFINE_H
#define _RANKLISTDEFINE_H

#include "Fsgame/Define/header.h"
#include <vector>

// ���а�������
enum RankClassType
{
	RANK_CLASS_NONE = -1,
	RANK_CLASS_PLAYER,			// ������а�����
};

// ������а�����
enum RankType
{
	RANK_LIST_NONE = -1,
	RANK_LIST_LEVEL,				// �ȼ����а�
 	RANK_LIST_FIGHT_POWER,			// ս�������а�
// 	RANK_LIST_KILL_NUM,				// ɱ�����а�
// 	RANK_LIST_CF_FIGHT_HURT,		// ��ս����˺����а�
// 	RANK_LIST_CF_BEAR_HURT,			// ��ս�����˺����а�

	MAX_RANK_LIST_NUM
};

// ���а����������
struct RankListProp
{
	RankType	nRankType;			// ���а����(��Ӧ�����±�)
	std::string strPubRecName;		// ���а��Ӧ����������
	std::vector<std::string> vecMainPro;   	// ��������(���ܲ�ֻһ��)	
	int			nMainLimit;			// ����������--���ڵ��ڸ������������а�
	int			nCommandId;			// ��Ӧ���ڲ���Ϣ��(��ӦCommandDefine.h)

	//Ĭ�Ϲ���
	RankListProp(): 
		nRankType(RANK_LIST_NONE),
		strPubRecName(""),
		nMainLimit(1),
		nCommandId(0)
	{
		vecMainPro.clear();
	}
};

// �򹫹�������������Ϣ
enum RankListMessage
{
	/*!
	 * @brief	ÿ����Ϣ��ǰ6�������ֱ�Ϊ1."domain" 2.string ���������� 3.int ��Ϣ��
	4.string ���ݱ��� 5.string ��ҵ�uid 6.int ���������Ե�ֵ.���治ͬ�����ݿɸ��ݲ�ͬ�����а��������

	1.��ҵȼ��� param 7.���ս����
	 */
	PUBDATA_MSG_RANKLIST_UPDATE,

	/*!
	 * @brief	��ѯ�ȼ����а�
	 * @param	string ���а����
	 * @param	
	 */
	PUBDATA_MSG_RANKLIST_QUERY,

	/*!
	 * @brief	ɾ������
	 * @param	string ���а����
	 * @param	
	 */
	PUBDATA_MSG_RANKLIST_DELETE,
};

#endif