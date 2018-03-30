//--------------------------------------------------------------------
// �ļ���:		PlayerData.h
// ��  ��:		������ݲ�ѯ����
// ˵  ��:		
// ��������:		2016��9��27��
// ������:		���
// ��Ȩ����:		���ݾ����������޹�˾
//--------------------------------------------------------------------
#ifndef __MODEL_PLAYER_DATA_H__
#define __MODEL_PLAYER_DATA_H__
#include <string>
#include <vector>

// ��ѯ����������Խṹ
struct CmdPropQuery
{
	int64_t     queryId;				// ��ѯid
	std::string roleId;					// ��ɫid    
	std::vector<std::string>  props;	// �����б�	
};

// ��ѯ�������ݽ��
struct CmdPropData
{
	int64_t     queryId;						// ��ѯid
	int			result;							// �����EmDataQueryResult��
	std::string roleId;							// ��ɫid
	std::map<std::string, std::string>  props;	// ����ֵ�б�
};

// ��ѯ������ұ�ṹ����
struct CmdRecordQuery
{
	int64_t     queryId;				// ��ѯid
	std::string roleId;					// ��ɫid
	std::string recordName;				// ����
	int offset;							// ƫ��ֵ
	int count;							// ��ѯ����
};

// ��ѯ��ṹ���ݽ��
struct CmdRecordData
{
	int64_t     queryId;				// ��ѯid
	std::string roleId;					// ��ɫid
	std::string recordName;				// ����
	int	result;							// �����EmDataQueryResult��
	int offset;							// ƫ��ֵ
	int count;							// ��ѯ����
	int cols;							// ���ر���
	std::vector<std::string> data;		// ���ص�ֵ( data.size() % cols == 0 )
};
#endif 