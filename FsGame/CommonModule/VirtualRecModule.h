//--------------------------------------------------------------------
// �ļ���:      VirtualRecModule.h
// ��  ��:      �����
// ˵  ��:		
// ��������:    2014��10��20��
// ������:        
// �޸���:        ( )
//    :       
//--------------------------------------------------------------------
#ifndef _VirtualRecModule_H_
#define _VirtualRecModule_H_

#include "Fsgame/Define/header.h"
#include <vector>
#include <string>
#include <unordered_map>

typedef std::unordered_map<std::string, std::vector<int>>                              RecColTypeMap;
typedef std::pair<std::unordered_map<std::string, std::vector<int>>::iterator, bool>  PairRecColTypeMap;
typedef std::vector<int>                                                             RecColType;
typedef std::string                                                                  RecName;

class VirtualRecModule : public ILogicModule
{
public:
    //��ʼ��
    virtual bool Init(IKernel* pKernel);

    //�ͷ�
    virtual bool Shut(IKernel* pKernel);

public:
    //�·��������
    virtual bool SendVirtualRecord(IKernel* pKernel, const PERSISTID& self,
                                   const char* virtual_rec, const IVarList& args);

    //֪ͨ�ͻ�������������
    virtual bool ClearVirtualRecord(IKernel* pKernel, const PERSISTID& self,
                                    const char* virtual_rec);

    //���±��ĳ��ĳ������
    virtual bool UpdateVirtualRecord(IKernel* pKernel, const PERSISTID& self,
                                     const char* virtual_rec, int row, int col, const IVarList& args);

    //���±��ĳ������
    virtual bool UpdateVirtualRecordRow(IKernel* pKernel, const PERSISTID& self,
                                        const char* virtual_rec, int row, const IVarList& args);

    //ɾ�����ĳ������
    virtual bool RemoveVirtualRecordRow(IKernel* pKernel, const PERSISTID& self,
                                        const char* virtual_rec, int row);

public:
    static VirtualRecModule* m_pVirtualRecModule;

protected:
    bool LoadResource(IKernel* pKernel);

    RecColTypeMap m_mapVirtualRecStruct;//������Ľṹ
};

#endif