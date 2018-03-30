//--------------------------------------------------------------------
// �ļ���:      BufferModule.h
// ��  ��:      Bufferϵͳ
// ˵  ��:
// ��������:    2014��11��4��
// ������:		 
//    :       
//--------------------------------------------------------------------

#ifndef _BufferModule_H_
#define _BufferModule_H_

#include "Fsgame/Define/header.h"
#include "FsGame/Define/BufferDefine.h"

struct BuffBaseData;

class BufferModule : public ILogicModule
{
public:
    /**
    @brief  ��ʼ��
    @param  pKernel [IN] �������ָ��
    @return ���س�ʼ�����
    */
    virtual bool Init(IKernel* pKernel);

    /**
    @brief  �ر�
    @param  pKernel [IN] �������ָ��
    @return ���عرս��
    */
    virtual bool Shut(IKernel* pKernel);

public:
    // --------------------------��������ӿ�-----------------------------------
    /**
    @brief  ����ĳ���ָ��ID������BUFFER
    @param  pKernel [IN] �������ָ��
    @param  self    [IN] ���������
    @param  bufferid [IN] ָ����ID
    @param  result [OUT] �ҵ���BUFF
    @return ���ҳɹ�����true�����򷵻�false
    */
    bool FindBuffer(IKernel* pKernel,
                    const PERSISTID& self,
                    const char* szBufferID,
                    IVarList& result);


    /**
    @brief  ���BUFFER����
    @param  pKernel [IN] �������ָ��
    @param  self [IN] �����ҵĶ���
    @return ���ض����buffer����
    */
    PERSISTID GetBufferContainer(IKernel* pKernel, const PERSISTID& self);

    /**
    *@brief  ���buff
    *@param  [IN] pKernel �����ں�ָ��
    *@param  [IN] self buff������
    *@param  [IN] sender buff�ͷ���
    *@param  [IN] szBufferConfigID buff��ConfigID
    *@return bool ��ӳɹ�����true�����򷵻�false
    */
    bool AddBuffer(IKernel* pKernel,
                   const PERSISTID& self,
                   const PERSISTID& sender,
                   const char* szBufferConfigID);

    /**
    *@brief  ���ָ�������ĳ��BUFFER
    *@param  [IN] pKernel �������ָ��
    *@param  [IN] self ָ������
    *@param  [IN] szBufferID buff��ConfigID
    *@return bool ����ɹ�����true�����򷵻�false
    */
    bool RemoveBuffer(IKernel* pKernel,
                      const PERSISTID& self,
                      const char* szBufferID);

    // ���BUFFͬʱ�޸�ʱ��
    PERSISTID AddBuffWithLifeTime(IKernel* pKernel,
                                  const PERSISTID& self,
                                  const PERSISTID& sender,
                                  const char* szBufferID,
                                  const int iLifeTime);


    // �����ɫ���ϵ�����BUFFER
    void ClearAllBuffer(IKernel* pKernel, const PERSISTID& self);

	PERSISTID GetBufferOwner(IKernel* pKernel, const PERSISTID& buffer);

	// buff�����¼�CD����(ֻ������buff����CD)
	//bool DealCoolDown(IKernel* pKernel, const PERSISTID& self, const BuffBaseData* pBuffBaseData);

	// �����������buff nClearType(��ӦBufferClear)
	int RemoveBuffByClearType(IKernel* pKernel, const PERSISTID &self, int nClearType);
protected:
    static int OnBufferContainerCreate(IKernel* pKernel, const PERSISTID& container,
                                       const PERSISTID& buffer, const IVarList& args);

	static int OnBufferOnRecover(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);
	// Buffer�������
	static int OnBufferLoad(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);

    static int OnLoad(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);
    static int OnStore(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);
    static int OnReady(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);
    static int OnLeaveScene(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);

    // ��ɱ
    static int OnCommandBeKill(IKernel* pKernel, const PERSISTID& self,
                               const PERSISTID& sender, const IVarList& args);

	//�¼���Ӧ
	//˵����
	//1:BUFFER����ʱ���ܻ�ı��������Ի���
	//  ���ı�������¼�����ʱ����Щ�¼�ֻ��������BeAttack��BeDamage��BeKill���¼����¼���������Щ�¼���sender
	//2:ֻ����Ӧս��������ص�Command
	static int OnCommandEffectBuffer(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);

    //buffer�����ʼ����ʱ����
    static int H_BufferUpdate(IKernel* pKernel, const PERSISTID& buffer, int slice);
private:
    // ------------------------ģ���ڲ����ܴ�����-----------------------------

    // ���ĳ��BUFFER
    bool InterAddBuffer(IKernel* pKernel, const PERSISTID& self,
                        const PERSISTID& sender, const PERSISTID& buffer);

    // ��ʼ��buffer��������
    bool InitBufferData(IKernel* pKernel, const PERSISTID& self, 
                        const PERSISTID& sender, const PERSISTID& buffer);

    // ���ĳ��BUFFER
    bool InterRemoveBuffer(IKernel* pKernel, const PERSISTID& self, const PERSISTID& buffer,
                           int iRemoveReason = BUFFER_REMOVE_NONE);

    //˲��״̬��Ч��
    int InstantActiveBuffer(IKernel* pKernel, const PERSISTID& self, const PERSISTID& buffer);

    //����״̬��Ч��
    bool ActiveBuffer(IKernel* pKernel, const PERSISTID& self, const PERSISTID& buffer);

    //�ر�״̬��Ч��
    bool DeactiveBuffer(IKernel* pKernel, const PERSISTID& self, const PERSISTID& buffer);

    //��ʱ��ƴ�ʱ��Ч��
    bool TimerBuffer(IKernel* pKernel, const PERSISTID& self, const PERSISTID& eventsender, const PERSISTID& buffer);
 
    // �滻����߼�
    bool DoBufferReplace(IKernel* pKernel, const PERSISTID& self, const PERSISTID& container, const PERSISTID& buffer);

    //���BUFFER��ʣ��ʱ�䣬��λΪms
    int GetBufferLeaveTime(int timeType, int lifeTime, int exitTime, time_t addTime);

    //�����¼�
    void BufferEventEffect(IKernel* pKernel, const EBufferEventType type, const PERSISTID& self,
                           const PERSISTID& sender, const PERSISTID& buffer, const IVarList& params = CVarList());
	// ������ʾbuff�Ŀ�������BuffDisplayID
	bool ChangeBuffDisplay(IKernel* pKernel, const PERSISTID& self, const std::string& szBufferConfigID);

	// buff��ֱ���滻����
	bool DoBuffDirectReplace(IKernel* pKernel, const PERSISTID& curbuff, const PERSISTID& oldbuff);

	// �Ƿ���Ҫ���buff
	bool IsNeedClearBuff(IKernel* pKernel, const char* strBufferConfigID, int nClearType, int& outReason);
private:
	bool PreloadBuffConfig(IKernel* pKernel);

public:
    static BufferModule* m_pInstance;
};

#endif
