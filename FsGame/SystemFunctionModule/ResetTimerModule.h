//--------------------------------------------------------------------
// �ļ���:      ResetTimerModule.h
// ��  ��:      ʱ������ģ��
// ˵  ��:
// ��������:    2016��03��28��
// ������:        tongzt
// �޸���:         
//--------------------------------------------------------------------

#ifndef _RESET_TIMER_MODULE_H_
#define _RESET_TIMER_MODULE_H_

#include "Fsgame/Define/header.h"
#include "FsGame/Define/ResetTimerDefine.h"


/* ----------------------
 * ��ʱˢ����
 * ----------------------*/
class ResetTimerModule : public ILogicModule
{
public:
    virtual bool Init(IKernel *pKernel);
    virtual bool Shut(IKernel *pKernel);

    bool LoadResource(IKernel *pKernel);

public:
    // ע��ʱ���ṩע��ˢ��
    static bool Register(IKernel *pKernel, const PERSISTID &self, 
		int type);

    // ��������ȡ����ʱˢ��
    static bool UnRegister(IKernel *pKernel, const PERSISTID &self, 
		int type);

	// ע�ắ��ָ��
	static void InitFunc(const ResetTimerTypes fun_type, ResetTimer func );

	// ��ѯʱ��ڵ�֮��ˢ�´���
	static int GetResetTimes(IKernel *pKernel, const PERSISTID &self, 
		int nType, const double startTime, const double endTime);

	// �ɷ���ˢ��
	static bool CanWeekReset(
		const ResetData *data, 
		const int cur_hour, 
		const double cur_date, 
		const int cur_weekday, 
		const int last_hour, 
		const double last_date, 
		const int last_weekday);

	// �ɷ���ˢ��
	static bool CanDayReset(
		const ResetData *data, 
		const int cur_hour, 
		const double cur_date, 
		const int last_hour, 
		const double last_date);

private:
    // ��ҽ��볡��
    static int OnEntry(IKernel *pKernel, const PERSISTID &self, 
		const PERSISTID &sender, const IVarList &args);

    // ���׼������
    static int OnReady(IKernel *pKernel, const PERSISTID &self, 
		const PERSISTID &sender, const IVarList &args);

    // ��һָ�����
    static int OnRecover(IKernel *pKernel, const PERSISTID &self, 
		const PERSISTID &sender, const IVarList &args);

    // �״�ˢ������
    static int HB_ResetFirst(IKernel *pKernel, const PERSISTID &self, 
		int slice);

    // ��ʱˢ������
    static int HB_ResetTimer(IKernel *pKernel, const PERSISTID &self, 
		int slice);
    
	static void ReloadConfig(IKernel* pKernel);
public:
    static ResetFuncMap         m_funcMap;
    static ResetdDataMap        m_ResetDataMap;
    static ResetTimerModule*    m_resetTimerModule;
};

/*!
 * @brief ���嶨ʱ��
 * @param fun_type ��������
 * @param func ���ܺ���
 */
inline void DECL_RESET_TIMER(const ResetTimerTypes fun_type, ResetTimer func)
{
	ResetTimerModule::InitFunc(fun_type, func);
}

/*!
 * @brief ע�ᶨʱ��
 * @pKernel ����ָ��
 * @self ��������
 * @param fun_type ��������
 */
inline void REGIST_RESET_TIMER(IKernel *pKernel, const PERSISTID &self, int fun_type)
{
	ResetTimerModule::Register(pKernel, self, fun_type);
}

#endif // _RESET_TIMER_MODULE_H_
