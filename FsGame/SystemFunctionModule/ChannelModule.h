//----------------------------------------------------------
// �ļ���:      ChannelModule.h
// ��  ��:      Ƶ��ϵͳ�ӿ�
// ˵  ��:
// ��������:    2014��10��17��
// ������:       
// �޸���:    
//    :       
//----------------------------------------------------------

#ifndef _ChannelModule_H_
#define _ChannelModule_H_

#include "Fsgame/Define/header.h"

class ChannelModule : public ILogicModule
{
public:
    //��ʼ��
    virtual bool Init(IKernel * pKernel);
    virtual bool Shut(IKernel * pKernel);

    //����Ƶ��
    bool JoinChannel(IKernel * pKernel, const PERSISTID & opplayer,
                     const char * channel_type, const wchar_t * channel_name);

    //�뿪Ƶ��
    bool LeaveChannel(IKernel * pKernel, const PERSISTID & opplayer,
                      const char * channel_type, const wchar_t * channel_name);

    //���Ƶ�����
    int GetChannel(IKernel * pKernel, const char * channel_type, const wchar_t * channel_name);

	//������������¼
	//bool SaveChannelInfo(IKernel *pKernel, const IVarList &args);

	//�·�������Ϣ
	static int OnCustomChannelInfo(IKernel* pKernel, const PERSISTID& self, 
							const PERSISTID& sender, const IVarList& args);

private:
	// ��ȡ������������
	static const std::wstring & GetDomainName(IKernel * pKernel);

	//��ȡ��Ƶ��������Ϣ�洢����
	virtual const std::string GetChatRecordName(IPubData *pChannelData, const int channel, int subType = -1);

	//�·�������ʷ��Ϣ
	//bool SendChannelToClient(IKernel* pKernel, const PERSISTID& self);


public:
    static ChannelModule * m_pChannelModule;
    static std::wstring m_domainName;               // ��ȡ������������
};
#endif
