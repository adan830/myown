/***********************************************************************
 ���֮��Ľ�������ϵͳ����
***********************************************************************/
#ifndef __RequestModule_h__
#define __RequestModule_h__

#include "Fsgame/Define/header.h"
#include "FsGame/Define/RequestDefine.h"
#include <vector>
#include <list>

class CoolDownModule;


//��Ӧ����ص��ĺ���
//�ܷ�������Ļص�
typedef bool (_cdecl* REQUEST_CAN_REQUEST_CALLBACK)(IKernel* pKernel, const PERSISTID& self,
                                                    REQUESTTYPE type, const wchar_t* targetname, const IVarList& paras);

//�ܷ�����Ļص�
typedef bool (_cdecl* REQUEST_CAN_BEREQUEST_CALLBACK)(IKernel* pKernel, const PERSISTID& self,
                                                      REQUESTTYPE type, const wchar_t* srcname, const IVarList& paras);

//�ͻ���ͬ�������Ļص�
typedef bool (_cdecl* REQUEST_REQUEST_SUCCEED_CALLBACK)(IKernel* pKernel, const PERSISTID& self,
                                                        REQUESTTYPE type, const wchar_t* targetname, const IVarList& paras);


//�������������߷�������ǰ��Ҫ�������и��ӵĲ����Ļص�
typedef bool (_cdecl* REQUEST_PARA_CALL_BACK)(IKernel* pKernel, const PERSISTID& self,
                                              REQUESTTYPE type, IVarList& paras);

//����Ľ���ص�����
typedef bool (_cdecl* REQUEST_RESULT_CALLBACK)(IKernel* pKernel, const PERSISTID& self,
                                               REQUESTTYPE type, const wchar_t* targetname, int result);

class RequestModule : public ILogicModule
{
public:
    RequestModule();

public:
    //��ʼ��
    virtual bool Init(IKernel* pKernel);
    //�ͷ�
    virtual bool Shut(IKernel* pKernel);

private:
	// ���볡������
	static int OnReady(IKernel* pKernel, const PERSISTID& self,
		const PERSISTID& sender, const IVarList& args);
public:
    //ע������ص�
    //type:��������
    //can_request_func:�ܷ񷢳�����Ļص�
    //can_berequest_func:�ܷ���Ӧ����Ļص�
    //request_suc_func:����ɹ���Ļص�
    //pPropList:�����б���canbe_request�ص��Ĳ����л��յ������ɫ����Щ����
    virtual bool RegisterRequestCallBack(REQUESTTYPE type,
                                         REQUEST_CAN_REQUEST_CALLBACK can_request_func,
                                         REQUEST_CAN_BEREQUEST_CALLBACK can_berequest_func,
                                         REQUEST_REQUEST_SUCCEED_CALLBACK request_suc_func,
                                         REQUEST_PARA_CALL_BACK req_para_func = NULL,
                                         REQUEST_PARA_CALL_BACK ans_para_func = NULL,
                                         REQUEST_RESULT_CALLBACK request_result_func = NULL);

    //������Ϊtarget_name������������Ϊtype������
    virtual bool Request(IKernel* pKernel, const PERSISTID& self,
                         const wchar_t* target_name, REQUESTTYPE type);

    static int OnCustomRequest(IKernel* pKernel, const PERSISTID& self,
                               const PERSISTID& sender, const IVarList& args);

    static int OnCustomRequestAnswer(IKernel* pKernel, const PERSISTID& self,
                                     const PERSISTID& sender, const IVarList& args);

    //�ܹ���Ӧ������ҷ��͵����������ʽ��usgined cmdid, int request_type, widestr srcname
    static int OnCommandBeRequest(IKernel* pKernel, const PERSISTID& self,
                                  const PERSISTID& sender, const IVarList& args);

    //��Ӧ������ҷ��͵�����Ļظ�(COMMAND_BEREQUST���ܻᱻ�첽����ͨ���������ж��Ƿ���ȷ����Է�����������߼�)
    static int OnCommandBeRequestEcho(IKernel* pKernel, const PERSISTID& self,
                                      const PERSISTID& sender, const IVarList& args);

    //��������һظ�������
    static int OnCommandRequestAnswer(IKernel* pKernel, const PERSISTID& self,
                                      const PERSISTID& sender, const IVarList& args);


    static int OnCommandGetAnswerPara(IKernel* pKernel, const PERSISTID& self,
                                      const PERSISTID& sender, const IVarList& args);

    static int OnCommandRetAnswerPara(IKernel* pKernel, const PERSISTID& self,
                                      const PERSISTID& sender, const IVarList& args);

    /*�ٴ�ȷ�����󣬵��������߻ظ�ʱ�������ߵ�״̬�����Ѿ��ı�*/
    static int OnCommandGetRequestPara(IKernel* pKernel, const PERSISTID& self,
                                       const PERSISTID& sender, const IVarList& args);

    /*�ٴ�ȷ�ϱ�����*/
    static int OnCommandRetRequestPara(IKernel* pKernel, const PERSISTID& self,
                                       const PERSISTID& sender, const IVarList& args);

    // ��������Ƿ����
    static int HB_CheckRequest(IKernel* pKernel, const PERSISTID& self,
                               int time);
protected:
	//�����Ƿ�ʧЧ
	bool IsRequestInvalid(IKernel* pKernel, const PERSISTID& self, const wchar_t* srcname, int request_type, const IVarList& args);

    //�ܷ�������(������OnCustomRequest��ͬ)
    bool CanRequest(IKernel* pKernel, const PERSISTID& self,
                    REQUESTTYPE type, const wchar_t* targetname, const IVarList& paras);

    //�ܷ��������(������OnCommandBeRequest��ͬ)
    bool CanBeRequest(IKernel* pKernel, const PERSISTID& self,
                      REQUESTTYPE type, const wchar_t* srcname, const IVarList& paras);

    //����ɹ�
    void RequestSucceed(IKernel* pKernel, const PERSISTID& self,
                        REQUESTTYPE type, const wchar_t* targetname, const IVarList& paras);

    //��ò���
    void GetRequestParas(IKernel* pKernel, const PERSISTID& self,
                         REQUESTTYPE type, IVarList& paras);
    //��ò���
    void GetAnswerParas(IKernel* pKernel, const PERSISTID& self,
                        REQUESTTYPE type, IVarList& paras);

    //�����ʧ�ܵĽ������ 0ʧ�� -1�������� -2�ܾ�
    void RequestResultReturn(IKernel* pKernel, const PERSISTID& self,
                             REQUESTTYPE type, const wchar_t* targetname, int result);

    static CoolDownModule* m_pCoolDownModule;

    std::vector<std::list<REQUEST_CAN_REQUEST_CALLBACK> >   m_vecRequestCallBack;
    std::vector<std::list<REQUEST_CAN_BEREQUEST_CALLBACK> > m_vecBeRequestCallBack;
    std::vector<std::list<REQUEST_REQUEST_SUCCEED_CALLBACK> >   m_vecRequestSucCallBack;
    std::vector<std::list<REQUEST_PARA_CALL_BACK> > m_vecAnswerParaCallBack;
    std::vector<std::list<REQUEST_PARA_CALL_BACK> > m_vecRequestParaCallBack;
    std::vector<std::list<REQUEST_RESULT_CALLBACK> >    m_vecRequestRltCallBack;

public:
    static RequestModule* m_pRequestModule;
};
#endif