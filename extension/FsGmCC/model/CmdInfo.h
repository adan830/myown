//--------------------------------------------------------------------
// �ļ���:		CmdInfo.h
// ��  ��:		Gm��Ϣ����
// ˵  ��:		
// ��������:		2016��9��17��
// ������:		���
// ��Ȩ����:		���ݾ����������޹�˾
//--------------------------------------------------------------------
#ifndef __MODEL_CMD_INFO_H__
#define __MODEL_CMD_INFO_H__
#include <string>
#include <vector>

struct CmdInfo
{
	std::string roleId;
	std::string command;
	std::string operateRoleId;
	std::vector<std::string>  cmdArgs;
	int64_t		seq;
};
#endif 