//--------------------------------------------------------------------
// �ļ���:      Server\FsRoomLogic\skill_module\FlowModule.h
// ��  ��:      ���̹���ϵͳ
// ˵  ��:      ���е����̶���ѭ���µ�״̬��
//              ��ʼ|----׼��----|----����----|����
// ��������:    2008/01/28
// ������:      �����
//    :       
//--------------------------------------------------------------------

#ifndef __FlowModule_h__
#define __FlowModule_h__

#include "Fsgame/Define/header.h"

#include "FsGame/Define/FlowCategoryDefine.h"
#include <map>
#include <string>

//���̽׶ν�����Ӧ��������
typedef bool (__cdecl* FLOW_RESPONSE_FUNC)(IKernel* pKernel, const PERSISTID& self);

class FlowModule : public ILogicModule
{
public:
    //��ʼ��
    virtual bool Init(IKernel* pKernel);
    //�ͷ�
    virtual bool Shut(IKernel* pKernel);

public:
    /**
    @brief        ע��һ�����̷���
    @param        ����
    [IN]category:���̷�������
    [IN]beforepreparefunc:����׼���׶ο�ʼʱ���õĺ���
    [IN]preparefunc:����׼���׶�ʱ�����ʱ���õĺ���
    [IN]beforeleadfunc:���������׶ο�ʼʱ���õĺ���
    [IN]leadfunc:ÿ����������ʱ���õĺ���
    [IN]beforehitfunc:�������н׶ο�ʼʱ���õĺ���
    [IN]hitfunc:����ʱ�����ʱ���õĺ���
    [IN]breakfunc:���̱����ʱ���õĺ���
    [IN]pausefunc:������ͣ����ʱ���õĺ���
    [IN]finishfunc:���̽���ʱ���õĺ���
    @remarks
    @return       ע��ɹ�����true,ʧ�ܷ���false
    */
    bool RegisterFlow(int iFlowCategory,
                      FLOW_RESPONSE_FUNC beforepreparefunc,
                      FLOW_RESPONSE_FUNC preparefunc,
                      FLOW_RESPONSE_FUNC beforeleadfunc,
                      FLOW_RESPONSE_FUNC leadfunc,
                      FLOW_RESPONSE_FUNC hitfunc,
                      FLOW_RESPONSE_FUNC breakfunc,
                      //FLOW_RESPONSE_FUNC pausefunc,
                      FLOW_RESPONSE_FUNC finishfunc);

    /**
    @brief        ��ʼһ����ͨ������
    @param        ����
    [IN]iFlowCategory:���̷���
    [IN]preparetime:׼��ʱ�䣬��λΪms
    [IN]hittime:����ʱ�䣬��λΪms
    @remarks      ע��:�����̷�����Ҫ�Ѿ���ע�ᣬÿ������ֻ��ͬʱӵ��һ������
    @return       �ɹ���ʼһ�����̣�����true��ʧ�ܷ���false
    */
    bool BeginFlow(IKernel* pKernel, const PERSISTID& self, int iFlowCategory,
                   int preparetime, int hittime, const char* flowtextid = "");

    /**
    @brief        ��ʼһ����ͨ������
    @param        ����
    [IN]iFlowCategory:���̷���
    [IN]preparetime:׼��ʱ�䣬��λΪms
    [IN]hittimes:���д���
    [IN]hittime:�����д�����ͬ��ʱ����
    @remarks      ע��:�����̷�����Ҫ�Ѿ���ע�ᣬÿ������ֻ��ͬʱӵ��һ������
    @return       �ɹ���ʼһ�����̣�����true��ʧ�ܷ���false
    */
    bool BeginFlow(IKernel* pKernel, const PERSISTID& self, int iFlowCategory,
                   int preparetime, const IVarList& hittime, const char* flowtextid = "");

    /**
    @brief        ��ʼһ����������
    @param        ����
    [IN]iFlowCategory:���̷���
    [IN]preparetime:׼��ʱ�䣬��λΪms
    [IN]leadlifetime:��������ʱ�䣬��λΪms
    [IN]leadseptime:�������ʱ�䣬��λΪms
    [IN]hittime:����ʱ�䣬��λΪms
    @remarks      ע��:�����̷�����Ҫ�Ѿ���ע�ᣬÿ������ֻ��ͬʱӵһ������
    @return       �ɹ���ʼһ�����̣�����true��ʧ�ܷ���false
    */
    bool BeginLeadFlow(IKernel* pKernel, const PERSISTID& self, int iFlowCategory,
                       int preparetime, int leadlifetime, int leadseptime, int hittime);

    // ��������
    bool EndStorage(IKernel* pKernel, const PERSISTID& self);

    /**
    @brief        ǿ�ƽ�����ǰ����
    @param        ����
    [IN]˵��
    @remarks
    @return
    */
    bool BreakFlow(IKernel* pKernel, const PERSISTID& self, int iFlowCategory);

    /**
    @brief        ǿ�ƽ������⵱ǰ����
    @param        ����
    [IN]˵��
    @remarks
    @return
    */
    bool BreakAnyFlow(IKernel* pKernel, const PERSISTID& self);

    /**
    @brief        ��ͣ��ǰ����(ֻ����׼�����е����̿�����ͣ)
    @param        ����
    [IN]˵��
    [IN]bbreak:Ϊtrueʱ��ʾ��ͣʱ�䵽����ֹ����
    @remarks
    @return
    */
    bool PauseFlow(IKernel* pKernel, const PERSISTID& self, int iFlowCategory, int slice, bool bbreak = false);

    /**
    @brief        ������ǰ����
    @param        ����
    [IN]˵��
    @remarks
    @return
    */
    bool ResumeFlow(IKernel* pKernel, const PERSISTID& self, int iFlowCategory);

    /**
    @brief        ��ǰ�Ƿ���һ�����̹�����
    @param        ����
    [IN]˵��
    @remarks
    @return
    */
    bool IsFlowing(IKernel* pKernel, const PERSISTID& self, int iFlowCategory);

    /**
    @brief        �Ƿ���һ������������
    @param        ����
    [IN]˵��
    @remarks
    @return
    */
    bool IsAnyFlowing(IKernel* pKernel, const PERSISTID& self);

    // �ͷ���������
    bool IsStoraging(IKernel* pKernel, const PERSISTID& self, int iFlowCategory);

    /**
    @brief        �Ƿ���һ�����̵�׼������
    @param        ����
    [IN]˵��
    @remarks
    @return
    */
    bool IsPreparing(IKernel* pKernel, const PERSISTID& self, int iFlowCategory);

    /**
    @brief        �Ƿ���һ�����̵���������
    @param        ����
    [IN]˵��
    @remarks
    @return
    */
    bool IsLeading(IKernel* pKernel, const PERSISTID& self, int iFlowCategory);

    /**
    @brief        �Ƿ���һ�����̵���������
    @param        ����
    [IN]˵��
    @remarks
    @return
    */
    bool IsHiting(IKernel* pKernel, const PERSISTID& self, int iFlowCategory);

    /**
    @brief        ��õ�ǰ���̵����̷�������
    @param        ����
    [IN]˵��
    @remarks
    @return
    */
    int QueryCategory(IKernel* pKernel, const PERSISTID& self);

private:

    static int OnRequestMove(IKernel* pKernel, const PERSISTID& self,
                             const PERSISTID& sender, const IVarList& args);

    static int OnCommandBreakFlow(IKernel* pKernel, const PERSISTID& self,
                                  const PERSISTID& sender, const IVarList& args);

    //�����������
    static int H_Flow_Prepare(IKernel* pKernel, const PERSISTID& self, int slice);
    static int H_Flow_Lead(IKernel* pKernel, const PERSISTID& self, int slice);
    static int H_Flow_Hit(IKernel* pKernel, const PERSISTID& self, int slice);

private:
    //��ʼ׼����
    void BeginPrepare(IKernel* pKernel, const PERSISTID& self, const char* flowtextid = "");
    //��ʼ������
    void BeginLead(IKernel* pKernel, const PERSISTID& self);
    //��ʼ������
    void BeginHit(IKernel* pKernel, const PERSISTID& self);
    //���̽���
    void Finish(IKernel* pKernel, const PERSISTID& self);

    //�������׼�����������������׼������������������ʼǰִ�У�
    FLOW_RESPONSE_FUNC GetBeforePrepareFunc(IKernel* pKernel, const PERSISTID& self);
    //�������׼��������׼���׶ν���ִ�У�
    FLOW_RESPONSE_FUNC GetPrepareFunc(IKernel* pKernel, const PERSISTID& self);
    //���������������(���������׶κ�����ִ�У�
    FLOW_RESPONSE_FUNC GetBeforeLeadFunc(IKernel* pKernel, const PERSISTID& self);
    //����������������������׶�ÿ����������ǰִ�У�
    FLOW_RESPONSE_FUNC GetLeadFunc(IKernel* pKernel, const PERSISTID& self);
    //�������н׶�ÿ����������ǰִ�еĺ���
    FLOW_RESPONSE_FUNC GetHitFunc(IKernel* pKernel, const PERSISTID& self);
    //������̴�Ϻ���
    FLOW_RESPONSE_FUNC GetBreakFunc(IKernel* pKernel, const PERSISTID& self);
    //���������ɺ���
    FLOW_RESPONSE_FUNC GetFinishFunc(IKernel* pKernel, const PERSISTID& self);

public:
    static FlowModule* m_pInstance; //Ϊ�����ĸ���̬����ʹ�õ�ָ��
private:
    struct S_FLOW_FUNC
    {
        FLOW_RESPONSE_FUNC beforepreparefunc;
        FLOW_RESPONSE_FUNC preparefunc;//׼��
        FLOW_RESPONSE_FUNC beforeleadfunc;
        FLOW_RESPONSE_FUNC leadfunc;   //����
        FLOW_RESPONSE_FUNC hitfunc;    //���
        FLOW_RESPONSE_FUNC breakfunc;  //���
        FLOW_RESPONSE_FUNC finishfunc; //���
    };

    //ĳ�����̵ĺ�������
    std::map<int, S_FLOW_FUNC> m_mapflowfunc;
};
#endif