//--------------------------------------------------------------------
// �ļ���:		SnsManager.h
// ��  ��:		���ݷ������߼�ģ��:���ݵĹ���
// ˵  ��:		
// ��������:	2014��06��10��
// ��������:	2014��06��10��
// ������:		  ( )
// �޸���:     
//    :	    
//--------------------------------------------------------------------

#ifndef __SnsManager_h__
#define __SnsManager_h__

#include "ISnsLogic.h"
#include "../../FsGame/Define/SnsDefine.h"
#include <unordered_map>

class SnsManager : public ISnsCallee 
{
public:
	SnsManager();

	virtual ~SnsManager();

	static ISnsCallee* Instance();

	virtual const char * GetSnsName();

	// \brief Sns����������������ʱ�߼�ģ���Ѽ������
	// \param pKernel ����ָ�룬��ͬ
	virtual int OnServerStarted(ISnsKernel* pSnsKernel);

	// \brief Sns�߼����󱻴���
	// \param pKernel ����ָ�룬��ͬ
	// \param args ������
	virtual int OnSnsModuleCreate(ISnsKernel* pSnsKernel, const IVarList& args);

	// \brief Sns���������Ե�Sns�����Ѿ��������׼������(������OnSnsLoad֮��)
	// \param pKernel ����ָ�룬��ͬ
	// \param args ������
	virtual int OnServerReady(ISnsKernel* pSnsKernel, const IVarList& args);

	// \brief sns�������ڴ��д���
	// \param args
	// \uid  sns uid //sns���ݹؼ���
	// \args  
	virtual int OnSnsDataCreate(ISnsKernel* pSnsKernel, const char* uid, const IVarList& args);

	// \brief ����sns����
	// \uid  sns uid //sns���ݹؼ���
	// \args  
	virtual int OnSnsDataLoad(ISnsKernel* pSnsKernel, const char* uid, const IVarList& args);

	// \brief �յ���Ϣ
	// \param args
	// \distribute_id ����id
	// \server_id  ���������
	// \memeber_id  �������������
	// \scene_id  ���ͳ�����id
	// \uid  sns uid //sns���ݹؼ���
	// \args  member���͵���Ϣ��
	virtual int OnMemberMessage(ISnsKernel* pSnsKernel, int distribute_id,int server_id,
		int memeber_id,int scene_id, const char* uid,const IVarList& args);

	/// \brief sns�������ر�֪ͨ
	virtual int OnServerClose(ISnsKernel* pSnsKernel);

	//����ģ��
	virtual ISnsLogic * FindSnsLogic(const wchar_t * module_name);

private:
	// ����ͨ����Ϣ
	bool ProcessCommonMessage(ISnsKernel* pSnsKernel, int distribute_id, int server_id,
		int memeber_id, int scene_id, const char* uid, const IVarList& args);

	/*
	����ص�ͨ�ò���
	*/
	//�޸����ԣ���ʽ��string snsspace, wstring snsdata, int msgid, string prop, var value, ...;
	bool SetPubDataProp(ISnsKernel* pSnsKernel, ISnsData* pSnsData, int distribute_id,int server_id,
		int memeber_id,int scene_id, const char* uid,const IVarList& args);
	//�������ԣ���ʽ��string snsspace, wstring snsdata, int msgid, string prop, int value, ...;
	bool IncPubDataProp(ISnsKernel* pSnsKernel, ISnsData* pSnsData, int distribute_id,int server_id,
		int memeber_id,int scene_id, const char* uid,const IVarList& args);

	//���ĳ��������ݣ������ʽ��string snsspace, wstring snsdata, int msgid, string rec, ...;
	bool ClearPubDataRec(ISnsKernel* pSnsKernel, ISnsData* pSnsData, int distribute_id,int server_id,
		int memeber_id,int scene_id, const char* uid,const IVarList& args);
	//ɾ��ĳ�����ĳ�����ݣ������ʽ��string snsspace, wstring snsdata, int msgid, string rec, int row, ...;
	bool RemovePubDataRecRow(ISnsKernel* pSnsKernel, ISnsData* pSnsData, int distribute_id,int server_id,
		int memeber_id,int scene_id, const char* uid,const IVarList& args);
	//���ĳ�����һ�����ݣ������ʽ��string snsspace, wstring snsdata, int msgid, string rec, ...;
	bool AddPubDataRecRowValue(ISnsKernel* pSnsKernel, ISnsData* pSnsData, int distribute_id,int server_id,
		int memeber_id,int scene_id, const char* uid,const IVarList& args);
	//����ĳ�����ĳһ��ĳһ�е����ݣ������ʽ��string snsspace, wstring snsdata, int msgid, string rec, int row, int col, var value, ...;
	bool SetPubDataRecValue(ISnsKernel* pSnsKernel, ISnsData* pSnsData, int distribute_id,int server_id,
		int memeber_id,int scene_id, const char* uid,const IVarList& args);
	//����ĳ�����ĳһ��ĳһ�е����ݣ������ʽ��string snsspace, wstring snsdata, int msgid, string rec, int row, int col, var value, ...;
	bool IncPubDataRecValue(ISnsKernel* pSnsKernel, ISnsData* pSnsData, int distribute_id,int server_id,
		int memeber_id,int scene_id, const char* uid,const IVarList& args);

	//���ݹؼ�ֵɾ��ĳ�����ĳ�����ݣ������ʽ��string snsspace, wstring snsdata, int msgid, string rec, int keycol, var keyvalue;
	bool RemovePubDataRecRowByKey(ISnsKernel* pSnsKernel, ISnsData* pSnsData, int distribute_id,int server_id,
		int memeber_id,int scene_id, const char* uid,const IVarList& args);
	//���ݹؽ�ֵ���ù����ĳ�����ĳһ��ĳһ�е����ݣ������ʽ��string snsspace, wstring snsdata, int msgid, string rec, int keycol, var keyvalue, int col, var value, ...;
	bool SetPubDataRecValueByKey(ISnsKernel* pSnsKernel, ISnsData* pSnsData, int distribute_id,int server_id,
		int memeber_id,int scene_id, const char* uid,const IVarList& args);
	//���ݹؽ����ӹ����ĳ�����ĳһ��ĳһ�е����ݣ������ʽ��string snsspace, wstring snsdata, int msgid, string rec, int keycol, var keyvalue, int col, var value, ...;
	bool IncPubDataRecValueByKey(ISnsKernel* pSnsKernel, ISnsData* pSnsData, int distribute_id,int server_id,
		int memeber_id,int scene_id, const char* uid,const IVarList& args);

private:
	//�������ã������ʽ��string snsspace, wstring snsdata, int msgid
	bool UpdateConfig(ISnsKernel* pSnsKernel, ISnsData* pSnsData, int distribute_id,int server_id,	 
		int memeber_id,int scene_id, const char* uid,const IVarList& args);

protected:
	typedef std::unordered_map<std::wstring, ISnsLogic *> HashMapTable;
	HashMapTable m_mapSnsLogicList;

private:
	static SnsManager* m_pInstance;
private:
	//sms �Ƿ�׼������
	bool is_sns_ready_;
};

#endif //__SnsManager_h__