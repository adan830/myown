//--------------------------------------------------------------------
// �ļ���:      GroupSceneBase.h
// ��  ��:      ���鳡��������
// ˵  ��:
// ��������:    2015��8��4��
// ������:       
// �޸���:        
//    :       
//--------------------------------------------------------------------
#ifndef _GROUP_SCENE_BASE_H_
#define _GROUP_SCENE_BASE_H_

#include "Fsgame/Define/header.h"
//#include "Fsgame/Define/GroupSceneDefine.h"

class GroupSceneBase : public ILogicModule
{
public:
    //��ʼ��
    virtual bool Init(IKernel* pKernel);
    //�ر�
    virtual bool Shut(IKernel* pKernel);

public:
    //��ó�������
    static int GetSceneType(IKernel* pKernel, int sceneid);
    //���������Ϣ
   // CfgSecretInfo* GetResInfo(int sceneID);
    //�������ͻ�ȡ������Ϣ
    void GetResInfoByType(CVarList& list);
    //����groupid
    bool SetGroupID(IKernel* pKernel,const PERSISTID& self,const int groupID);
    //��������
    bool LoadResource(IKernel* pKernel);
    //�������NPC����
    void GetTrapConfig(int sceneID, std::string& trapNpcConfig, int& trapAmount);
public:
    static GroupSceneBase* m_pGroupSceneBase;
};
#endif;