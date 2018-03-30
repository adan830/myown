//---------------------------------------------------------
//�ļ���:       SnsDataModule.h
//��  ��:       ���ߵ��������
//˵  ��:       
//          
//��������:      2014��12��24��
//������:         
//�޸���:
//   :         
//---------------------------------------------------------

#ifndef FSGAME_SNS_DATA_MODULE_H
#define FSGAME_SNS_DATA_MODULE_H

#include "Fsgame/Define/header.h"

#include <map>
#include <vector>

class SnsDataModule : public ILogicModule
{
public:

	// ��ʼ��
	virtual bool Init(IKernel* pKernel);

	// �ͷ�
	virtual bool Shut(IKernel* pKernel);

    static bool QuerySnsData(IKernel* pKernel, const IVarList& args);

    // ��һ����ҵ���������д��SNS
    static bool SaveSnsData(IKernel* pKernel, const PERSISTID &player);
    
private:

    struct ElementDef 
    {
		ElementDef(): name(""), type(0), section(""){}
        std::string name;
        int type;
        std::string section;
    };

    //////////////////////////////////////////////////////////////////////////
    // �ص�����
    //////////////////////////////////////////////////////////////////////////

    //������ݼ������
    static int OnPlayerEntry(IKernel *pKernel, const PERSISTID &player,
        const PERSISTID &sender, const IVarList &args);

    // player �� OnStore �¼�
	static int OnPlayerStoreEvent(IKernel* pKernel, const PERSISTID& self,
        const PERSISTID& sender, const IVarList& args);

    // Player ��Command��Ϣ�ص�
    static int OnPlayerCommandMsg(IKernel* pKernel, const PERSISTID& self, 
        const PERSISTID& sender, const IVarList& args);

    // Player �Ŀͻ�����Ϣ������
    static int OnPlayerCustomMsg(IKernel* pKernel, const PERSISTID& self,
        const PERSISTID& sender, const IVarList& args);

    // ����public����������Ϣ
    static int OnPublicMessage(IKernel * pKernel, const PERSISTID & self,
        const PERSISTID & sender, const IVarList & args);

    //////////////////////////////////////////////////////////////////////////
    // ���ܺ���
    //////////////////////////////////////////////////////////////////////////

    // ����Ϣ��SNS������
    static bool SendMsgToSnsServer(IKernel* pKernel, const PERSISTID& self,
        const IVarList &msg);

    static bool SendMsgToSnsServer(IKernel* pKernel, const char* uid,
        const IVarList &msg);
        
    // ��ȡ��ҵ���������
    static bool ReadPlayerAttr(IKernel* pKernel, const PERSISTID& self,
        IVarList &result);

    // ��ȡ���װ����Ϣ
//     static bool ReadPlayerEquipment(IKernel* pKernel, const PERSISTID& self,
//         IVarList &result);
    
    // ��ȡ��Ҽ�����Ϣ
    static bool ReadPlayerSkill(IKernel* pKernel, const PERSISTID& self,
        IVarList &result);
        
    // ��ȡ���������Ϣ
//     static bool ReadPlayerRide(IKernel* pKernel, const PERSISTID& self,
//         IVarList &result);
//         
//     // ��ȡ��ҳ�����Ϣ
//     static bool ReadPlayerPet(IKernel* pKernel, const PERSISTID& self,
//         IVarList &result);

    // ��ȡ��ұ���������Ϣ
//     static bool ReadPlayerPassiveSkill(IKernel* pKernel, const PERSISTID& self,
//         IVarList &result);

	// ��ȡ���ϴ������
// 	static bool ReadPlayerEquipmentBaptise(IKernel* pKernel, const PERSISTID& self, 
// 		IVarList &result);
public:
    // ��ѯ�������
    static bool OnQueryPlayerData(IKernel* pKernel, const PERSISTID& self, 
        const PERSISTID& sender, const IVarList& args);
    
    // ��������Դ洢��SNS
    // args �ĸ�ʽ: [name][value]...[name][value]
    // name ���ַ�������, ���õ�name������SnsData.xml �� element_def ҳ��
    static bool SavePlayerAttr(IKernel* pKernel, const PERSISTID& player, const IVarList& args); 

	// ���������ļ�
	bool LoadResource(IKernel *pKernel);
	

private:
    // ��������
    static bool OnCommandRequestData(IKernel* pKernel, const PERSISTID& self, 
        const IVarList& args);

    // �õ�����
    static bool OnCommandGotData(IKernel* pKernel, const PERSISTID& self,
        const IVarList& args);

	// ��ѯ��������������
	static bool OnCommandGotChatData(IKernel* pKernel, const PERSISTID& self,
		const IVarList& args);

    //////////////////////////////////////////////////////////////////////////
    // ������صĺ���
    //////////////////////////////////////////////////////////////////////////

    // ��ȡsection������
    static const std::vector<SnsDataModule::ElementDef> * GetSectionElements(const std::string &sec_name);

    // ��ȡ������id, ���󷵻�-1 
    int GetVarID(const std::string &var_name);

    // ���������ļ�-��������
    bool LoadVarDef(IKernel *pKernel);

    // ���������ļ�-section �����
    bool LoadElementsDef(IKernel *pKernel);

    // �ͻ����������а�����ҵ�����
	static int ClientRequestRankData(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);

	static void ReloadConfig(IKernel* pKernel);
public:

    typedef std::map<std::string, int> VarDefMap;
    typedef std::map<std::string, std::vector<ElementDef> > SectionElementsMap;

    static VarDefMap m_VarDef;
    static SectionElementsMap m_SectionElements;

    static SnsDataModule* m_pSnsDataModule;
};
#endif
