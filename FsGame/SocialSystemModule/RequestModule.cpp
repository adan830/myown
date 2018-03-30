#include "FsGame/SocialSystemModule/RequestModule.h"
#include "utils/custom_func.h"
#include "utils/extend_func.h"
#include "utils/util_func.h"
#include "FsGame/Define/RequestDefine.h"
#include "FsGame/Define/ClientCustomDefine.h"
#include "FsGame/Define/ServerCustomDefine.h"
#include "FsGame/Define/CommandDefine.h"
#include "FsGame/Define/TeamDefine.h"
#include "FsGame/SystemFunctionModule/CoolDownModule.h"
#include "FsGame/SocialSystemModule/TeamModule.h"
#include "public/VarList.h"

#include <ctime>
#include "Define/Fields.h"

//�������ʱ��:��
#define MAX_REQUEST_LAST_TIME 180

RequestModule* RequestModule::m_pRequestModule = NULL;
CoolDownModule* RequestModule::m_pCoolDownModule = NULL;

RequestModule::RequestModule()
{
    m_vecRequestCallBack.resize(REQUESTTYPE_COUNT);
    m_vecBeRequestCallBack.resize(REQUESTTYPE_COUNT);
    m_vecRequestSucCallBack.resize(REQUESTTYPE_COUNT);
    m_vecRequestParaCallBack.resize(REQUESTTYPE_COUNT);
    m_vecAnswerParaCallBack.resize(REQUESTTYPE_COUNT);
    m_vecRequestRltCallBack.resize(REQUESTTYPE_COUNT);
}

//��ʼ��
bool RequestModule::Init(IKernel* pKernel)
{
    m_pRequestModule = this;

    m_pCoolDownModule = (CoolDownModule*)pKernel->GetLogicModule("CoolDownModule");
	Assert(m_pCoolDownModule != NULL && m_pRequestModule != NULL);
	pKernel->AddEventCallback("player", "OnReady", OnReady);

    pKernel->AddIntCustomHook("player", CLIENT_CUSTOMMSG_REQUEST, RequestModule::OnCustomRequest);
    pKernel->AddIntCustomHook("player", CLIENT_CUSTOMMSG_REQUEST_ANSWER, RequestModule::OnCustomRequestAnswer);

    pKernel->AddIntCommandHook("player", COMMAND_BEREQUEST, RequestModule::OnCommandBeRequest);
    pKernel->AddIntCommandHook("player", COMMAND_BEREQUEST_ECHO, RequestModule::OnCommandBeRequestEcho);
    pKernel->AddIntCommandHook("player", COMMAND_REQUEST_ANSWER, RequestModule::OnCommandRequestAnswer);
    pKernel->AddIntCommandHook("player", COMMAND_GET_ANSWER_PARA, RequestModule::OnCommandGetAnswerPara);
    pKernel->AddIntCommandHook("player", COMMAND_RET_ANSWER_PARA, RequestModule::OnCommandRetAnswerPara);
    pKernel->AddIntCommandHook("player", COMMAND_GET_REQUEST_PARA, RequestModule::OnCommandGetRequestPara);
    pKernel->AddIntCommandHook("player", COMMAND_RET_REQUEST_PARA, RequestModule::OnCommandRetRequestPara);

    //�������
    DECL_HEARTBEAT(RequestModule::HB_CheckRequest);
    return true;
}

//�ͷ�
bool RequestModule::Shut(IKernel* pKernel)
{
    return true;
}

//ע������ص�
bool RequestModule::RegisterRequestCallBack(REQUESTTYPE type,
                                            REQUEST_CAN_REQUEST_CALLBACK can_request_func,
                                            REQUEST_CAN_BEREQUEST_CALLBACK can_berequest_func,
                                            REQUEST_REQUEST_SUCCEED_CALLBACK request_suc_func,
                                            REQUEST_PARA_CALL_BACK req_para_func,
                                            REQUEST_PARA_CALL_BACK ans_para_func,
                                            REQUEST_RESULT_CALLBACK request_result_func)
{
    if (type <= REQUESTTYPE_NONE
		|| type >= REQUESTTYPE_COUNT)
    {
        return false;
    }

    //�ܷ�������Ļص�����
    bool bexists = false;
    std::list<REQUEST_CAN_REQUEST_CALLBACK>::iterator it = m_vecRequestCallBack[type].begin();
	LoopBeginCheck(a);
    for (; it != m_vecRequestCallBack[type].end(); ++it)
    {
		LoopDoCheck(a);
        if (*it == can_request_func)
        {
            bexists = true;
            break;
        }
    }

    if (!bexists)
    {
        m_vecRequestCallBack[type].push_back(can_request_func);
    }

    //�ܷ��������Ļص�����
    bexists = false;
    std::list<REQUEST_CAN_BEREQUEST_CALLBACK>::iterator it1 = m_vecBeRequestCallBack[type].begin();
	LoopBeginCheck(b);
    for (; it1 != m_vecBeRequestCallBack[type].end(); ++it1)
    {
		LoopDoCheck(b);
        if (*it1 == can_berequest_func)
        {
            bexists = true;
            break;
        }
    }

    if (!bexists)
    {
        m_vecBeRequestCallBack[type].push_back(can_berequest_func);
    }


    //����ɹ���Ļص�����
    bexists = false;
    std::list<REQUEST_REQUEST_SUCCEED_CALLBACK>::iterator it2 = m_vecRequestSucCallBack[type].begin();
	LoopBeginCheck(c);
    for (; it2 != m_vecRequestSucCallBack[type].end(); ++it2)
    {
		LoopDoCheck(c);
        if (*it2 == request_suc_func)
        {
            bexists = true;
            break;
        }
    }

    if (!bexists)
    {
        m_vecRequestSucCallBack[type].push_back(request_suc_func);
    }

    if (req_para_func != NULL)
    {
        //��ȡ�����Ļص�
        bexists = false;
        std::list<REQUEST_PARA_CALL_BACK>::iterator it4 = m_vecRequestParaCallBack[type].begin();
		LoopBeginCheck(d);
        for (; it4 != m_vecRequestParaCallBack[type].end(); ++it4)
        {
			LoopDoCheck(d);
            if (*it4 == req_para_func)
            {
                bexists = true;
                break;
            }
        }

        if (!bexists)
        {
            m_vecRequestParaCallBack[type].push_back(req_para_func);
        }
    }

    if (ans_para_func != NULL)
    {
        //��ȡ�����Ļص�
        bexists = false;
        std::list<REQUEST_PARA_CALL_BACK>::iterator it3 = m_vecAnswerParaCallBack[type].begin();
		LoopBeginCheck(e);
        for (; it3 != m_vecAnswerParaCallBack[type].end(); ++it3)
        {
			LoopDoCheck(e);
            if (*it3 == ans_para_func)
            {
                bexists = true;
                break;
            }
        }

        if (!bexists)
        {
            m_vecAnswerParaCallBack[type].push_back(ans_para_func);
        }
    }
    if (request_result_func != NULL)
    {
        //��ȡ�����Ļص�
        bexists = false;
        std::list<REQUEST_RESULT_CALLBACK>::iterator it5 = m_vecRequestRltCallBack[type].begin();
		LoopBeginCheck(f);
        for (; it5 != m_vecRequestRltCallBack[type].end(); ++it5)
        {
			LoopDoCheck(f);
            if (*it5 == request_result_func)
            {
                bexists = true;
                break;
            }
        }																									    

        if (!bexists)
        {
            m_vecRequestRltCallBack[type].push_back(request_result_func);
        }
    }

    return true;
}

// ���볡������
int RequestModule::OnReady(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args)
{
	if (!pKernel->Exists(self))
	{
		return 0;
	}

	IGameObj *pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return 0;
	}

	IRecord *pReqRec = pSelfObj->GetRecord(REQUEST_REC);
	if (NULL == pReqRec)
	{
		return 0;
	}
	int rows = pReqRec->GetRows();
	// ����������Ϣ����������󵹼�ʱ����
	if (rows > 0 && !pKernel->FindHeartBeat(self, "RequestModule::HB_CheckRequest"))
	{
		pKernel->AddHeartBeat(self, "RequestModule::HB_CheckRequest", 1000);
		//�������󵹼�ʱ
		LoopBeginCheck(g);
		for (int i = 0; i < rows; ++i)
		{
			LoopDoCheck(g);
			pReqRec->SetInt(i, REQUEST_REC_TIME, MAX_REQUEST_LAST_TIME);
		}
	}
	return 0;
}

//������Ϊtarget_name������������Ϊtype������
bool RequestModule::Request(IKernel* pKernel, const PERSISTID& self,
                            const wchar_t* target_name, REQUESTTYPE type)
{
	if (!pKernel->Exists(self))
	{
		return false;
	}

	IGameObj *pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return false;
	}
    const wchar_t* source_name = pSelfObj->QueryWideStr("Name");
    if (wcscmp(source_name, target_name) == 0)
    {
        // ���ܶ���������
		return false;
    }

    //ְҵ
    int profession = pSelfObj->QueryInt("Job");
    /*int sex = pSelfObj->QueryInt("Sex");
    if (profession > 3 || profession < 1 
		|| sex > 2 || sex < 1)
    {
        return false;
    }
    int pics[3][2] = {{1, 2}, {3, 4}, {5, 6}};

    int pic = pics[profession - 1][sex - 1];*/
    //ս����
    int battle = pSelfObj->QueryInt("BattleAbility");
	
	int param = 0;
	//������������������Ӧ���ж��Ƿ�ͬһ����
	//if (REQUESTTYPE_INVITE_SINGLEPLAYER == type)
	//{
		param = pSelfObj->QueryInt("TeamID");
	//}
    CVarList var_list;
    if (pKernel->GetPlayerScene(target_name) > 0)
    {
        var_list << COMMAND_GET_ANSWER_PARA << (int)type << source_name << param 
			<< profession << battle << pSelfObj->QueryInt("Nation") << pSelfObj->QueryInt("Level") << pSelfObj->QueryInt("Sex");
        pKernel->CommandByName(target_name, var_list);
    }
    else
    {
        //[{@0:����}]��������״̬, ����ʧ��
		::CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17305, CVarList() << target_name);
    }

    return true;
}

int RequestModule::OnCustomRequest(IKernel* pKernel, const PERSISTID& self,
                                   const PERSISTID& sender, const IVarList& args)
{
    REQUESTTYPE requesttype = (REQUESTTYPE)args.IntVal(1);
	if (requesttype <= REQUESTTYPE_NONE
		|| requesttype >= REQUESTTYPE_COUNT)
	{
		return 0;
	}
    const wchar_t* targetname = args.WideStrVal(2);

    m_pRequestModule->Request(pKernel, self, targetname, requesttype);

    return 0;
}

int RequestModule::OnCustomRequestAnswer(IKernel* pKernel, const PERSISTID& self,
                                         const PERSISTID& sender, const IVarList& args)
{
	if (!pKernel->Exists(self))
	{
		return 0;
	}

	IGameObj *pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return 0;
	}

    //�������߿ͻ��˷��͹����Ķ�����Ļش���Ϣ
    REQUESTTYPE requesttype = (REQUESTTYPE)args.IntVal(1);
	if (requesttype <= REQUESTTYPE_NONE
		|| requesttype >= REQUESTTYPE_COUNT)
	{
		return 0;
	}

    //��������
    const wchar_t* srcname = args.WideStrVal(2);
    int result = args.IntVal(3);

    //�ٴ�����õ�srcname������(��ʱֻΪ��������޸Ĵ�����)
    CVarList msg;
    msg << COMMAND_GET_REQUEST_PARA 
		   << requesttype
           << pSelfObj->QueryWideStr("Name") 
		   << result;

    if (pKernel->GetPlayerScene(srcname) > 0)
    {
        pKernel->CommandByName(srcname, msg);
    }
    else
    {
        //�Է���������״̬, {@1:����}����ʧ��
		::CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17305, CVarList() << srcname);
    }

    return 0;
}

//�ܹ���Ӧ������ҷ��͵����������ʽ��usgined cmdid, int requesttype, widestr srcname
int RequestModule::OnCommandBeRequest(IKernel* pKernel, const PERSISTID& self,
	const PERSISTID& sender, const IVarList& args)
{
	if (!pKernel->Exists(self))
	{
		return 0;
	}

	IGameObj *pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return 0;
	}

	REQUESTTYPE requesttype = (REQUESTTYPE)args.IntVal(1);
	const wchar_t* srcname = args.WideStrVal(2);
	int nSrcTeamId = args.IntVal(3);
	int nSrcLv = args.IntVal(4);
	int nSrcBA = args.IntVal(5);
	int nSrcSex = args.IntVal(6);
	int nSrcJob = args.IntVal(7);

	//�ɷ������ɸ�ģ�鴦���߼�
	CVarList src_paras;
	src_paras << REQUEST_STATE_REQ;
	src_paras.Append(args, 3, args.GetCount() - 3);
	if (!m_pRequestModule->CanBeRequest(pKernel, self, requesttype, srcname, src_paras))
	{
		IRecord *pReqRec = pSelfObj->GetRecord(REQUEST_REC);
		if (pReqRec != NULL)
		{
			int rows = pReqRec->GetRows();
			LoopBeginCheck(h);
			for (int i = rows - 1; i >= 0; --i)
			{
				LoopDoCheck(h);
				const wchar_t* name = pReqRec->QueryWideStr(i, REQUEST_REC_NAME);
				int reqtype = pReqRec->QueryInt(i, REQUEST_REC_TYPE);
				if (wcscmp(srcname, name) == 0 && reqtype == requesttype)
				{
					//ɾ������Ҵ�������
					pReqRec->RemoveRow(i);
				}
			}
		}
		//ʧ�ܣ����ͻ�ִ��������
		CVarList msg;
		msg << COMMAND_BEREQUEST_ECHO
			<< requesttype
			<< pSelfObj->QueryWideStr("Name")
			<< 0;
		pKernel->CommandByName(srcname, msg);
		return 0;
	}
	
    
    //�ɹ�����ͻ��˷�������
    if (requesttype == REQUESTTYPE_JOIN_SINGLEPLAYER || requesttype == REQUESTTYPE_INVITE_SINGLEPLAYER)
    {
        pKernel->Custom(self, CVarList() << SERVER_CUSTOMMSG_TEAM << CLIENT_CUSTOMMSG_TEAM_REFRESH);
		pKernel->Custom(self, CVarList() << SERVER_CUSTOMMSG_TEAM << CLIENT_CUSTOMMSG_TEAM_REQ_ADD_TEAM 
			<< requesttype << srcname << nSrcLv << nSrcBA << nSrcSex << nSrcJob);
    }

    //���ͻ�ִ��������
    CVarList msg;
    msg << COMMAND_BEREQUEST_ECHO 
		   << requesttype 
		   << pSelfObj->QueryWideStr("Name") 
		   << 1;
    pKernel->CommandByName(srcname, msg);
    return 0;
}

//��Ӧ������ҷ��͵�����Ļظ�(COMMAND_BEREQUEST���ܻᱻ�첽����ͨ���������ж��Ƿ���ȷ����Է�����������߼�)
int RequestModule::OnCommandBeRequestEcho(IKernel* pKernel, const PERSISTID& self,
                                          const PERSISTID& sender, const IVarList& args)
{
	if (!pKernel->Exists(self))
	{
		return 0;
	}

    REQUESTTYPE requesttype = (REQUESTTYPE)args.IntVal(1);
    const wchar_t* targetname = args.WideStrVal(2);
    int result = args.IntVal(3);

    if(result == -1)
    {
        //[{@0:����}]�ܾ����ܴ�����������Ϣ����ʧ��
		::CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17303, CVarList() << targetname);
    }

    //�������
    m_pRequestModule->RequestResultReturn(pKernel, self, requesttype, targetname, result);

    return 0;
}

//����Ľ������result��3�������� 2ͬ�� 1�ȴ� 0ʧ�� -1�������� -2�ܾ�
void RequestModule::RequestResultReturn(IKernel* pKernel, const PERSISTID& self,
                                        REQUESTTYPE type, const wchar_t* targetname, int result)
{
    if (type <= REQUESTTYPE_NONE
		|| type >= REQUESTTYPE_COUNT)
    {
        return ;
    }


    std::list <REQUEST_RESULT_CALLBACK>::const_iterator it = m_vecRequestRltCallBack[type].begin();
	LoopBeginCheck(i);
    for (; it != m_vecRequestRltCallBack[type].end(); ++it)
    {
		LoopDoCheck(i);
        if (!((*it)(pKernel, self, type, targetname, result)))
        {
            return ;
        }
    }

    return ;
}


//��������һظ�������
int RequestModule::OnCommandRequestAnswer(IKernel* pKernel, const PERSISTID& self,
                                          const PERSISTID& sender, const IVarList& args)
{
	if (!pKernel->Exists(self))
	{
		return 0;
	}

    REQUESTTYPE requesttype = (REQUESTTYPE)args.IntVal(1);
    const wchar_t* targetname = args.WideStrVal(2);
    int result = args.IntVal(3);
    CVarList paras;
    paras << REQUEST_STATE_ANS;
    paras.Append(args, 4, args.GetCount() - 4);

    if (requesttype > REQUESTTYPE_NONE
		&& requesttype < REQUESTTYPE_COUNT)
    {
        if (result == 0)
        {
            //[{@0:����}]�ܾ������{@1:����}����
			::CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17302, CVarList() << targetname << "");

            //�������
            m_pRequestModule->RequestResultReturn(pKernel, self, requesttype, targetname, -2);

        }
        else if (result == 2)
        {
            //���[{@0:����}]��{@1:����}�����ѳ�ʱ...
			::CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17304, CVarList() << targetname << "");

            //�������
            m_pRequestModule->RequestResultReturn(pKernel, self, requesttype, targetname, -2);

        }
        else
        {
            if (!m_pRequestModule->CanRequest(pKernel, self, requesttype, targetname, paras))
            {
                //�������
                m_pRequestModule->RequestResultReturn(pKernel, self, requesttype, targetname, 0);
                return 0;
            }

            //�������
            m_pRequestModule->RequestResultReturn(pKernel, self, requesttype, targetname, 2);

            m_pRequestModule->RequestSucceed(pKernel, self, requesttype, targetname, paras);
        }
    }

    return 0;
}
int RequestModule::OnCommandGetAnswerPara(IKernel* pKernel, const PERSISTID& self,
                                          const PERSISTID& sender, const IVarList& args)
{
	if (!pKernel->Exists(self))
	{
		return 0;
	}
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return 0;
	}

    REQUESTTYPE requesttype = (REQUESTTYPE)args.IntVal(1);
    const wchar_t* srcname = args.WideStrVal(2);
    int teamid = args.IntVal(3);
    int pic = args.IntVal(4);//ͷ��
    int battle = args.IntVal(5);//ս����
	int nation = args.IntVal(6);// ����
	int level = args.IntVal(7); // �ȼ�
	int sex = args.IntVal(8);	//�Ա�
	if (!m_pRequestModule->CanBeRequest(pKernel, self, requesttype, srcname, CVarList() << 0 << teamid << nation))
	{
		return 0;
	}

	if (pKernel->GetPlayerScene(pSelfObj->QueryWideStr("Name")) <= 0)
	{
		//[{@0:����}]��������״̬, ����ʧ��
		CustomSysInfoByName(pKernel, srcname, SYSTEM_INFO_ID_17301, CVarList() << pSelfObj->QueryWideStr("Name"));
		 
		 return 0;
	}

    //��ò���
    CVarList paras;
    m_pRequestModule->GetAnswerParas(pKernel, self, requesttype, paras);
	
	IRecord *pReqRec = pSelfObj->GetRecord(REQUEST_REC);
	//�ж��Ƿ��յ�����ҷ��͵���ͬ����
	if (pReqRec != NULL)
	{
		int rows = pReqRec->GetRows();
		LoopBeginCheck(j);
		for (int i = 0; i < rows; ++i)
		{
			LoopDoCheck(j);
			const wchar_t* name = pReqRec->QueryWideStr(i, REQUEST_REC_NAME);
			int reqType = pReqRec->QueryInt(i, REQUEST_REC_TYPE);
			if (wcscmp(srcname, name) == 0 && reqType == requesttype)
			{
				// ����[{@0:name}]������������Ϣ���ȴ���Ӧ��������
				CustomSysInfoByName(pKernel, srcname, SYSTEM_INFO_ID_17306, CVarList() << pSelfObj->QueryWideStr("Name"));
				return 0;
			}
		}
		
		CVarList row_value;
		row_value << rows << srcname << requesttype << MAX_REQUEST_LAST_TIME << teamid << pic << battle << level << sex << nation;
		pReqRec->AddRowValue(-1, row_value);

		// ���������Ϣ��������
		if (!pKernel->FindHeartBeat(self, "RequestModule::HB_CheckRequest") )
		{
			pKernel->AddHeartBeat(self, "RequestModule::HB_CheckRequest", 1000);
		}
	
	}

    CVarList msg;
    msg << COMMAND_RET_ANSWER_PARA 
		   << requesttype 
		   << pSelfObj->QueryWideStr("Name") 
		   << paras;
    if (!pKernel->CommandByName(srcname, msg))
    {
        return 0;
    }
    return 0;
}

int RequestModule::OnCommandRetAnswerPara(IKernel* pKernel, const PERSISTID& self,
                                          const PERSISTID& sender, const IVarList& args)
{
	if (!pKernel->Exists(self))
	{
		return 0;
	}

	IGameObj *pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return 0;
	}

    REQUESTTYPE type = (REQUESTTYPE)args.IntVal(1);
    const wchar_t* target_name = args.WideStrVal(2);

    CVarList target_paras;
    target_paras << REQUEST_STATE_REQ;
    target_paras.Append(args, 3, args.GetCount() - 3);

    //�ܷ������������
    if (!m_pRequestModule->CanRequest(pKernel, self, type, target_name, target_paras))
    {

		CVarList msg;
		msg << COMMAND_TEAM_MSG << COMMAND_DEL_PLAYER_APPLAY<<type<<pSelfObj->QueryWideStr("Name");
		pKernel->CommandByName(target_name, msg);

        return 0;
    }

    //��ò���
    CVarList paras;
    m_pRequestModule->GetRequestParas(pKernel, self, type, paras);

    //��������Ŀ�����������
    CVarList msg;
    msg << COMMAND_BEREQUEST << type << pSelfObj->QueryWideStr(FIELD_PROP_NAME) 
		<< pSelfObj->QueryInt(FIELD_PROP_TEAM_ID)
		<< pSelfObj->QueryInt(FIELD_PROP_LEVEL) 
		<< pSelfObj->QueryInt(FIELD_PROP_BATTLE_ABILITY)
		<< pSelfObj->QueryInt(FIELD_PROP_SEX)
		<< pSelfObj->QueryInt(FIELD_PROP_JOB);
    if (paras.GetCount() > 0)
    {
        msg << paras;
    }
    if (!pKernel->CommandByName(target_name, msg))
    {
        //��������ʧ��, ���[{@0:����}]�����ڻ�Է�������
		::CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17301, CVarList() << target_name);
        return 0;
    }
    //�������
    m_pRequestModule->RequestResultReturn(pKernel, self, type, target_name, 3);
    //�ɹ���������
	::CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17306, CVarList() << target_name);

    return 0;
}
/*�ٴ�ȷ�����󣬵��������߻ظ�ʱ�������ߵ�״̬�����Ѿ��ı�,�����»�ȡpara*/
int RequestModule::OnCommandGetRequestPara(IKernel* pKernel, const PERSISTID& self,
                                           const PERSISTID& sender, const IVarList& args)
{
	if (!pKernel->Exists(self))
	{
		return 0;
	}

	IGameObj *pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return 0;
	}

    REQUESTTYPE requesttype = (REQUESTTYPE)args.IntVal(1);
    const wchar_t* targetname = args.WideStrVal(2);
    int result = args.IntVal(3);

	if (TeamModule::IsFull(pKernel,self))
	{
		//[{@0:name}]�����������������ʧ��
		CustomSysInfoByName(pKernel, targetname, SYSTEM_INFO_ID_17401, CVarList() << pSelfObj->QueryWideStr("Name"));
		return 0;

	}

    //��ò���
    CVarList paras;
    m_pRequestModule->GetRequestParas(pKernel, self, requesttype, paras);	

    CVarList msg;
    msg << COMMAND_RET_REQUEST_PARA 
		    << requesttype 
		    << pSelfObj->QueryWideStr("Name") 
		    << result 
		    << paras;
    if (!pKernel->CommandByName(targetname, msg))
    {
        return 0;
    }

    return 0;
}

/*�ٴ�ȷ�ϱ�����*/
int RequestModule::OnCommandRetRequestPara(IKernel* pKernel, const PERSISTID& self,
                                           const PERSISTID& sender, const IVarList& args)
{
	if (!pKernel->Exists(self))
	{
		return 0;
	}

	IGameObj *pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return 0;
	}

    REQUESTTYPE requesttype = (REQUESTTYPE)args.IntVal(1);
    const wchar_t* srcname = args.WideStrVal(2);
    int result = args.IntVal(3);

    CVarList src_paras;
    src_paras << REQUEST_STATE_ANS;
    src_paras.Append(args, 4, args.GetCount() - 4);
    if (result == 1)
    {
        if (!m_pRequestModule->CanBeRequest(pKernel, self, requesttype, srcname, src_paras))
        {
            //ʧ��
            m_pRequestModule->RequestResultReturn(pKernel, self, requesttype, srcname, 0);
            return 0;
        }
    }
	
	//�ж���Ϣ�Ƿ��Ѿ�ʧЧ
	if (m_pRequestModule->IsRequestInvalid(pKernel, self, srcname, requesttype, src_paras))
	{
		return 0;
	}

    //��ò���
    CVarList paras;
    m_pRequestModule->GetAnswerParas(pKernel, self, requesttype, paras);
	
    CVarList msg;
    msg << COMMAND_REQUEST_ANSWER 
		   << requesttype 
		   << pSelfObj->QueryWideStr("Name") 
		   << result 
		   << paras;
    if (!pKernel->CommandByName(srcname, msg))
    {
        return 0;
    }

    return 0;
}

//�ܷ�������(������OnCustomRequest��ͬ)
bool RequestModule::CanRequest(IKernel* pKernel, const PERSISTID& self,
                               REQUESTTYPE type, const wchar_t* targetname, const IVarList& paras)
{
    if (type <= REQUESTTYPE_NONE
		|| type >= REQUESTTYPE_COUNT)
    {
        return false;
    }

    std::list <REQUEST_CAN_REQUEST_CALLBACK>::const_iterator it = m_vecRequestCallBack[type].begin();
	LoopBeginCheck(k);
    for (; it != m_vecRequestCallBack[type].end(); ++it)
    {
		LoopDoCheck(k);
        if (!((*it)(pKernel, self, type, targetname, paras)))
        {
            return false;
        }
    }

    return true;
}

//�ܷ��������(������OnCommandBeRequest��ͬ)
bool RequestModule::CanBeRequest(IKernel* pKernel, const PERSISTID& self,
                                 REQUESTTYPE type, const wchar_t* srcname, const IVarList& paras)
{
    if (type <= REQUESTTYPE_NONE
		|| type >= REQUESTTYPE_COUNT)
    {
        return false;
    }

    std::list <REQUEST_CAN_BEREQUEST_CALLBACK>::const_iterator it = m_vecBeRequestCallBack[type].begin();
	LoopBeginCheck(l);
    for (; it != m_vecBeRequestCallBack[type].end(); ++it)
    {
		LoopDoCheck(l);
        if (!((*it)(pKernel, self, type, srcname, paras)))
        {
            return false;
        }
    }

    return true;
}

//����ɹ�
void RequestModule::RequestSucceed(IKernel* pKernel, const PERSISTID& self,
                                   REQUESTTYPE type, const wchar_t* targetname, const IVarList& paras)
{
    if (type <= REQUESTTYPE_NONE
		|| type >= REQUESTTYPE_COUNT)
    {
        return;
    }

    std::list <REQUEST_REQUEST_SUCCEED_CALLBACK>::const_iterator it = m_vecRequestSucCallBack[type].begin();
	LoopBeginCheck(m);
    for (; it != m_vecRequestSucCallBack[type].end(); ++it)
    {
		LoopDoCheck(m);
        if (!((*it)(pKernel, self, type, targetname, paras)))
        {
            return;
        }
    }

    return;
}

//��ò���
void RequestModule::GetRequestParas(IKernel* pKernel, const PERSISTID& self,
                                    REQUESTTYPE type, IVarList& paras)
{
    if (type <= REQUESTTYPE_NONE
		|| type >= REQUESTTYPE_COUNT)
    {
        return;
    }
    std::list <REQUEST_PARA_CALL_BACK>::const_iterator it = m_pRequestModule->m_vecRequestParaCallBack[type].begin();
	LoopBeginCheck(n);
    for (; it != m_pRequestModule->m_vecRequestParaCallBack[type].end(); ++it)
    {
		LoopDoCheck(n);
        if (!((*it)(pKernel, self, type, paras)))
        {
            return;
        }
    }
    return;
}

//��ò���
void RequestModule::GetAnswerParas(IKernel* pKernel, const PERSISTID& self,
                                   REQUESTTYPE type, IVarList& paras)
{
    if (type <= REQUESTTYPE_NONE
		|| type >= REQUESTTYPE_COUNT)
    {
        return;
    }

    std::list <REQUEST_PARA_CALL_BACK>::const_iterator it = m_pRequestModule->m_vecAnswerParaCallBack[type].begin();
	LoopBeginCheck(o);
    for (; it != m_pRequestModule->m_vecAnswerParaCallBack[type].end(); ++it)
    {
		LoopDoCheck(o);
        if (!((*it)(pKernel, self, type, paras)))
        {
            return;
        }
    }

    return;
}

int RequestModule::HB_CheckRequest(IKernel* pKernel, const PERSISTID& self, int time)
{
	if (!pKernel->Exists(self))
	{
		return 0;
	}

	IGameObj *pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return 0;
	}

	IRecord *pReqRec = pSelfObj->GetRecord(REQUEST_REC);
	if (NULL == pReqRec)
	{
		return 0;
	}
    int count = pReqRec->GetRows();
	LoopBeginCheck(p);
    for (int i = count - 1; i >= 0; --i)
    {
		LoopDoCheck(p);
        int lastTime = pReqRec->QueryInt(i, REQUEST_REC_TIME);
        if (lastTime >= 1)
        {
            //�������ʱ���1��
            pReqRec->SetInt(i, REQUEST_REC_TIME, --lastTime);
        }
        else if (lastTime < 1)
        {
           const wchar_t* srcname = pReqRec->QueryWideStr(i, REQUEST_REC_NAME);
            int requesttype = pReqRec->QueryInt(i, REQUEST_REC_TYPE);
            const wchar_t* self_name = pSelfObj->QueryWideStr("Name");

			//ɾ����������
			pReqRec->RemoveRow(i);
			
			//����ҷ��������ʱ
			CVarList msg;
			msg << COMMAND_GET_REQUEST_PARA 
				   << requesttype
				   << self_name 
				   << 2;
            pKernel->CommandByName(srcname, msg);
        }
    }

    //���������е�������Ϣ��ɾ������
    if (count == 0 && pKernel->FindHeartBeat(self, "RequestModule::HB_CheckRequest"))
    {
        pKernel->RemoveHeartBeat(self, "RequestModule::HB_CheckRequest");
    }
	 
    return 0;
}

//����״̬�Ƿ���ͬ
bool RequestModule::IsRequestInvalid(IKernel* pKernel, const PERSISTID& self, const wchar_t* srcname, int request_type, const IVarList& args)
{
	if (!pKernel->Exists(self))
	{
		return false;
	}

	IGameObj *pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return false;
	}
	bool bInValid = true;

	IRecord *pReqRec = pSelfObj->GetRecord(REQUEST_REC);
	//ɾ��������д���ҵ�����
	if (pReqRec != NULL)
	{
		int param = 0;
		int type = -1;
		int rows = pReqRec->GetRows();
		LoopBeginCheck(q);
		for (int i = rows - 1; i >= 0; --i)
		{
			LoopDoCheck(q);
			const wchar_t* name = pReqRec->QueryWideStr(i, REQUEST_REC_NAME);
			type = pReqRec->QueryInt(i, REQUEST_REC_TYPE);
			param = pReqRec->QueryInt(i, REQUEST_REC_STATE);
			if (wcscmp(srcname, name) == 0 && type == request_type)
			{
				//ɾ������Ҵ�������
				pReqRec->RemoveRow(i);
				//��ϢδʧЧ
				bInValid = false;
				break;
			}
		}

		//�ж���Ϣ�Ƿ��Ѿ�ʧЧ
		switch (type)
		{
			//�������
			case REQUESTTYPE_INVITE_SINGLEPLAYER:
			{
				int teamID = args.IntVal(1);
				int request = args.IntVal(6);
				int is_captain = args.IntVal(7);
				//����ID���ȣ���ǰ׼��״̬�������룬������Ȩ��
				if (teamID != param|| 
					(teamID > 0 && (request != TEAM_REQUEST_YES && is_captain == 0)))
				{
					bInValid = true;
				}
			}
			break;
		}
		//δ�ҵ���ǰ��Ϣ���������߷�������ʱ��״̬�����ʱ��״̬��ͬ
		if (bInValid)
		{
			//֪ͨĿ�����������ʧЧ
			::CustomSysInfo(pKernel, self, SYSTEM_INFO_ID_17305, CVarList());
			return true;
		}
	}
	return bInValid;
}