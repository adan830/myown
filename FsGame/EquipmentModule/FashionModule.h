//---------------------------------------------------------
//�ļ���:       FashionModule.h
//��  ��:       ʱװģ��
//˵  ��:               
//��������:      2017��02��21��
//������:        tongzt 
//�޸���:    
//---------------------------------------------------------

#ifndef FSGAME_FASHION_MODULE_H_
#define FSGAME_FASHION_MODULE_H_

#include "Fsgame/Define/header.h"
#include "FashionDefine.h"


class FashionModule : public ILogicModule
{
public:

    virtual bool Init(IKernel* pKernel);

    virtual bool Shut(IKernel *pKernel);

    bool LoadResource(IKernel *pKernel);

private:
	// ����ʱװ����
	bool LoadFashionConfig(IKernel* pKernel);

	// ȡ��ʱװ����
	const FashionCfg* GetFashionCfg(const int id);

	// �ɷ����
	bool CanUnlock(IKernel* pKernel, const PERSISTID& self, 
		const int fashion_id);

	// ǰ���Ƿ��ѽ���
	bool IsPreUnlock(IKernel* pKernel, const PERSISTID& self, 
		const int pre_fashion_id);

public:
	// ����
	void Unlock(IKernel* pKernel, const PERSISTID& self,
		const int fashion_id);

	// ��
	void PutOn(IKernel* pKernel, const PERSISTID& self,
		const int fashion_id);

	// ��
	void TakeOff(IKernel* pKernel, const PERSISTID& self,
		const int fashion_id);

	// �Ƿ�����
	bool IsSuit(int limit, const int v);

	// �ҳ��ʺ���ҵ�ʱװid
	const int FindRightFashion(IKernel* pKernel, const PERSISTID& self,
		const IVarList& fashion_list);

	// �Ƿ��ʹ����Ʒ����
	bool CanUse(IKernel* pKernel, const PERSISTID& self,
		const IVarList& fashion_list);

	// GM����ʱװ
	void GMLock(IKernel* pKernel, const PERSISTID& self,
		const int fashion_id);

	// ����������ָ��ʱװ
	bool GmUnlock(IKernel* pKernel, const PERSISTID& self,
		const int fashion_id, const int valid_day, const int log_type);

	// һ������ʱװ
	bool OnekeyUnlock(IKernel* pKernel, const PERSISTID& self, 
		const int log_type, const char *fashion_str, const int valid_day = 0, const char *delims = ",");

	// һ������ʱװ
	void OnekeyLock(IKernel* pKernel, const PERSISTID& self,
		const char *fashion_str, const char *delims = ",");

	// ʱЧ���
	void ValidTimeCheck(IKernel* pKernel, const PERSISTID& self);

private:
    // ������ݻָ����
	static int OnPlayerRecover(IKernel* pKernel, const PERSISTID& self,
        const PERSISTID& sender, const IVarList& args);
 
    // ������ҵ���Ϣ
	static int OnCustomMsg(IKernel* pKernel, const PERSISTID& self,
        const PERSISTID& sender, const IVarList& args); 

	// ʱЧ�Լ��
	static int HB_ValidTime(IKernel* pKernel, const PERSISTID& self,
		int slice);

public:
	// ����
	static void ReloadConfig(IKernel* pKernel);

public:
    static FashionModule *m_pFashionModule;
    
private:
	FashionCfgVec m_FashionCfgVec;

}; // End of class

#endif