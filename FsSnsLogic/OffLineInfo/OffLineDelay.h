//----------------------------------------------------------
// �ļ���:      OffLineDelay.h
// ��  ��:      ��������
// ˵  ��:
// ��������:    2014��11��21��
// ������:       
// �޸���:    
//    :       
//----------------------------------------------------------
#ifndef __OFFLINE_DELAY_H_
#define __OFFLINE_DELAY_H_

#include "../../server/ISnsCallee.h"
#include "../../server/ISnsKernel.h"
#include "../ISnsLogic.h"
#include <vector>
#include "utils/cache/CacheString.hpp"

class OffLineDelay : public ISnsLogic 
{
public:
	OffLineDelay();

	int OnModuleCreate(ISnsKernel* pKernel, const IVarList& args);
	int OnReady(ISnsKernel* pKernel, const IVarList& args);

	// \brief sns�������ڴ��д���
	// \param args
	// \uid  sns uid //sns���ݹؼ���
	// \args  
	int OnCreate(ISnsKernel* pSnsKernel, const char* uid, const IVarList& args);

	int OnLoad(ISnsKernel* pKernel, const char* uid, const IVarList& args);
	
	//�յ�����member����Ϣ
	int OnMessage(ISnsKernel* pKernel, int distribute_id,int server_id,
		int memeber_id,int scene_id, const char* uid,const IVarList& args);

public:
	static int Offline_delay_heart(ISnsKernel * pKernel, 
		const char* uid, int time);
private:
	// ������Ϣ
	struct MirDataInfo
	{
		MirDataInfo() : strName(""), nType(0), strSnsTable(""){}
		std::string		strName;			// ����
		int				nType;				// �������Ͷ�Ӧ(MirDataType)
		std::string		strSnsTable;		// ������Ӧ��sns��
	};

	typedef std::vector<MirDataInfo> MirDataInfoVec;

	int OnPushPlayerOffline(ISnsKernel* pKernel, const char* uid, const IVarList& args);

	// ��ȡ������Ϣ����
	bool LoadDataInfoConfig(ISnsKernel* pKernel);

	// ���ɲ�ѯ����
	void GenerateQueryData(ISnsData* pSnsData, IVarList& outData);

	// ��ȡĳ����������
	void FillPropertyData(ISnsData* pSnsData, const char* strProName, IVarList& outData);

	// ��ȡ�����������
	void FillContainerOrRecData(ISnsData* pSnsData, const char* strProName, const char* strRec, IVarList& outData);

	// ������ѯ�������
	void LoadPlayerPropertys(ISnsKernel* pKernel, const char* attr_names, const IVarList& req_msg, IVarList& dst_msg);
	void LoadPlayerPropertys_Detal(ISnsData * pSnsData, const IVarList& attr_list_name, const IVarList& attr_list_type, int64_t ident_id, IVarList& dst_msg);
private:
	MirDataInfoVec			m_vDataInfo;		// ������Ϣ
	CacheString<512> temp_string_;
};

#endif