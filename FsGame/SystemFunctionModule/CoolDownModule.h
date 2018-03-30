//--------------------------------------------------------------------
// �ļ���:      Server\FsGame\SkillModule\CoolDownModule.h
// ��  ��:      ��ȴϵͳ����
// ˵  ��:
// ��������:    2008/01/28
// ������:      �����
//    :       
//--------------------------------------------------------------------

#ifndef __CoolDownModule_h__
#define __CoolDownModule_h__

#include "Fsgame/Define/header.h"
#include <vector>

//��ȴ����ж���
enum CoolDownRecColIndex
{
    COOLDOWN_REC_COL_ID,        //��ȴ����ID
    COOLDOWN_REC_COL_BEGINTIME, //��ȴ��ʼʱ��
    COOLDOWN_REC_COL_ENDTIME,   //��ȴ����ʱ��

    COOLDOWN_REC_COL_COUNT
};

class CoolDownModule : public ILogicModule
{
public:
	//��ʼ��
	virtual bool Init(IKernel* pKernel);

	//�ͷ�
	virtual bool Shut(IKernel* pKernel);

	static CoolDownModule* Instance();
public:

	/**
	@brief        ��ʼһ����ȴ�������ȴ
	@param        ����
	[IN]cooldowncategory:��ȴ������
	[IN]cooldowntime:��ȴʱ�䣬��λms
	@remarks
	@return       ����ɹ���ʼ��ȴ������true�����򣬷���false
	*/
	bool BeginCoolDown(IKernel* pKernel, const PERSISTID& self,
		int cooldowncategory, int cooldowntime );

	/**
	@brief        ����һ����ȴ�������ȴ
	@param        ����
	[IN]cooldowncategory:��ȴ������
	@remarks
	@return       ����ɹ���ʼ��ȴ������true�����򣬷���false
	*/
	bool EndCoolDown(IKernel* pKernel, const PERSISTID& self, int cooldowncategory);

	/**
	@brief        ָ�������Ƿ�������ȴ��
	@param        ����
	[IN]cooldowncategory:��ȴ������
	@remarks
	@return       ���ĳ����������ȴ�У�����true�����򷵻�false
	*/
	bool IsCoolDown(IKernel* pKernel, const PERSISTID& self, int cooldowncategory);

    // ȡ����ȴ��ʣ�������
    int64_t GetMillisecRemain(IKernel* pKernel, const PERSISTID& self, int cooldowncategory);

	//���ȫ��CD
	void ClearAllCD(IKernel* pKernel, const PERSISTID& self);

	//�������CD
	void ClearPlayerNormalSkillCD(IKernel* pKernel, const PERSISTID& self);

	// ��ȡ����
	bool LoadConfig(IKernel* pKernel);
private:
	// �������
	static int OnPlayerStore(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);

	//�����ȴ��Ϣ����ȴ��
	bool InnerBeginCoolDown(IRecord* pRecordCool, const int& cooldowncategory,
		const int64_t& begin_time, const int64_t& end_time);

	// ����Ƿ�������ȴ���������ȴ�����кţ����������ȴ����-1
	int InnerIsCoolDown(IRecord* pRecordCool, const int cooldowncategory, const int64_t now);

	//�����ҳ�ʱ����ȴ��
	bool ClearCoolDownRec(IRecord* pRecordCool);

	// �Ƿ���Ҫ�����CD����
	bool IsNeedSaveCategory(int nCategory);

	static void ReloadConfig(IKernel* pKernel);
public:
	static CoolDownModule* m_pInstance;
private:
	std::vector<int>	m_vSavedCategoryIds;		// ��Ҫ�����CD���
};
#endif