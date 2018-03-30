/********************************************
* File  :		RankCacheConst.h
* Author:		Eddie LIU
* Date  :		2015-7-27
* 
* Copyright:	Snail Game (ltao@snail.com)
********************************************/

#ifndef _RANK_CACHE_CONST_H
#define _RANK_CACHE_CONST_H

// ��������
enum{
	REQ_SET_SCORE = 0,		// �������а��û�����
	REQ_SET_EXTRA,			// �������а��û���������
	REQ_GET_SCORE,			// ��ȡ���а��û�����
	REQ_GET_EXTRA,			// ��ȡ���а��û���������
	REQ_GET_RANK,			// ��ȡ���а��û�����
	REQ_GET_RANGE,			// ��ȡ���а�ĳ����Χ��������Ϣ
	REQ_DEL,				// ɾ�����а�ĳ���û�
	REQ_SET_SCORE_EXTRA,	// �������а��û����ֺ͸�������
	REQ_SET_GLOBAL_DATA,	// ����ȫ�����ݱ��ĳһ�е�ĳ��ֵ
	REQ_GET_GLOBAL_DATA,	// ��ȡȫ�����ݱ��ĳһ�еĶ��ֵ
	REQ_GET_RANGE_BY_SCORE,
	REQ_REBUILD,			// �ؽ����а񣨲����ã�
	REQ_DESTROY,			// ɾ��ĳ�����а�
	REQ_DESTROY_ALL,		// ɾ���������а�
	REQ_DESTROY_GLOBAL_DATA,// ɾ��ȫ�����ݱ�
	REQ_SAVE,
	REQ_SHUTDOWN,
	REQ_REWRITE,
	REQ_LIST,
	REQ_DUMP,
	REQ_UNKNOWN
};

#endif //_RANK_CACHE_CONST_H