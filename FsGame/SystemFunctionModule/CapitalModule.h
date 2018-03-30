//--------------------------------------------------------------------
// �ļ���:		CapitalModule.h
// ��  ��:		��ɫ�ʽ����
// ˵  ��:		
// ��������:	2014��06��10��
// ��������:	2014��06��10��
// ������:		  ( )
// �޸���:     
//    :	    
//--------------------------------------------------------------------

#ifndef __CapitalModule_H__
#define __CapitalModule_H__

#include "Fsgame/Define/header.h"
#include <vector>
#include <string>

#include "FsGame/Define/CapitalDefine.h"
#include "FsGame/Define/LogDefine.h"

//���ӻ��������ӷ��ؽ��
enum CAPITAL_MONITOR
{
    CAPITAL_MONITOR_NORMAL = 0,
    CAPITAL_MONITOR_WARN,
    CAPITAL_MONITOR_TOPLIMIT,
};

class LogModule;

class CapitalModule : public ILogicModule
{
public:
	//��ʼ��
	virtual bool Init(IKernel* pKernel);
	//�ͷ�
	virtual bool Shut(IKernel* pKernel);

	//��Rule/CapitalDefine.ini�ļ��ж�ȡ�ʽ����͵Ķ���;
	bool LoadResource(IKernel* pKernel);

	// ��ȡ�һ���������
	bool LoadExchangeRuleConfig(IKernel* pKernel);

	// �����Ƿ��Ѷ���
	bool IsCapitalDefined(const std::string& name);
	bool IsCapitalDefined(const int& type);

	// ���ݻ������ֲ�ѯ��������
	int GetCapitalType(const std::string& name);
	// ���ݻ������Ͳ�ѯ��������
	std::string GetCapitalName(const int& type);

	//����ӿ�
	//����ʽ����͵���������
	virtual int GetCapitalTypeCount(IKernel* pKernel);

	//���ĳ�����͵��ʽ�ĵ�ǰֵ(strType:�ʽ�����ͣ���CapitalDefine.h�ļ��еĶ���)
	virtual __int64 GetCapital(IKernel* pKernel, const PERSISTID& self, int capital_type);

	//����ĳ�����͵��ʽ�ĵ�ǰֵ
	// log_type-��¼��־����
	// comment - ��ע��Ϣ
	virtual bool IncCapital(IKernel* pKernel, const PERSISTID& self, int capital_type, __int64 value,
		EmFunctionEventId log_type, const wchar_t* comment = L"");

	//����ĳ�����͵��ʽ�ĵ�ǰֵ
	virtual int DecCapital(IKernel* pKernel, const PERSISTID& self, int capital_type, __int64 value,
		int logType, const char* itemConfigId = "", const char* itemInstanceId = "", int itemCount = 0,
		int cmd = -1, const IVarList* var = NULL, const char * reason="");

	//�Ƿ��ܹ�����ĳ�����͵��ʽ�ĵ�ǰֵ
	//���δ�����ֵ������ʵ�ʿ����ӵ�ֵ����
	//��ʯ�ڴ˷������ж�ʧ�ܣ�����������Ϸ���������������
	virtual bool CanIncCapital(IKernel* pKernel, const PERSISTID& self, int capital_type, __int64 value);

	//�Ƿ��ܹ�����ĳ�����͵��ʽ�ĵ�ǰֵ
	//���δ����Сֵ������ʵ�ʿɼ��ٵ�ֵ����
	virtual bool CanDecCapital(IKernel* pKernel, const PERSISTID& self, int capital_type, __int64 value);

	//�Ƿ�����Ч���ʽ�����
	virtual bool IsValid(IKernel* pKernel, const PERSISTID& self, int capital_type);

	//��ȡĳ�������ʽ�����ֵ
	virtual __int64 GetMaxValue(IKernel* pKernel, int capital_type);

	//��ȡĳ�������ʽ����������
	virtual const char* GetCapitalProp(int capital_type);
	//���ӻ������������Ϸ���
	const CAPITAL_MONITOR CapitalMonitor(IKernel *pKernel, const PERSISTID &self, const int capitalType, const __int64 addNum, __int64 &normalAddNum, __int64 &abnormalAddNum);
	//�����Ƿ�������ӻ���
	virtual bool IsDailyCanAddCapital(IKernel *pKernel, const PERSISTID &self,int capital_type);
#ifndef FSROOMLOGIC_EXPORTS
	// ֱ����������ֵ�������ֱ�����������سɹ��ĸ���
	int AddCapital(IKernel *pKernel, const PERSISTID &player, const IVarList &capital_list, EmFunctionEventId capitalEventID);
	// ֱ����������ֵ�������ֱ�����������سɹ��ĸ���
	int AddCapital(IKernel *pKernel, const PERSISTID &player, const char* pszCaptialInfo, EmFunctionEventId capitalEventID);
#endif
private:

    //0�����õ����ۼ������ʽ��¼ֵ
    static int ResetDayToplimit(IKernel *pKernel, const PERSISTID &self, int slice);

	//����ÿ�������ʽ�����
	bool SetCapitalMonitorMaxVal(const int capitalType, const __int64 warnDayVal, const __int64 maxDayVal);
public:

	static CapitalModule* m_pCapitalModule;
#ifndef FSROOMLOGIC_EXPORTS
	static LogModule* m_pLogModule;
#endif // FSROOMLOGIC_EXPORTS
protected:
	//�ʽ����Ͷ���
	struct S_CAPTITALTYPE
	{
		std::string     prop;       //��������
		__int64         maxvalue;   //���ֵ
        __int64         warnDayVal; //�վ�����
        __int64         maxDayVal;  //�����ֵ
        S_CAPTITALTYPE():prop(""),
                         maxvalue(0),
                         warnDayVal(0),
                         maxDayVal(0)
        {

        }
	};
	std::vector<S_CAPTITALTYPE>     m_vecCaptialType;

	enum CAPITAL //�ʲ���������
	{
		CAPITAL_NAME = 0,//�ʲ�����
		CAPITAL_TYPE = 1 , //�ʲ�����
		CAPITAL_VALUE = 2,//�ʲ�����
	};

	// ���ҹ���
	struct CapitalExchangeRule 
	{
		int				nExchangeId;		// �һ�����id
		std::string		strSrcCapital;		// ����ԭ��������
		std::string		strDesCapital;		// ��õ�Ŀ���������
		int				nExchangeRate;		// �һ�����
	};

	typedef std::vector<CapitalExchangeRule>  CapitalExRuleVec;

	CapitalExRuleVec m_vecExchangeRules;

private:
	// ִ��ʵ�ʵ����ӽ�Ǯ
	bool Inner_IncCapital(IKernel* pKernel, const PERSISTID& self, int capital_type, __int64 value);

	// ִ��ʵ�ʵĿ۳���Ǯ
	int Inner_DecCapital(IKernel* pKernel, const PERSISTID& self, int capital_type, __int64 value);

	//�����ʽ�ֵ
	virtual bool SetCaptialValue(IKernel* pKernel, const PERSISTID& self, const int capital_type, const __int64 newvalue);

   

    //��Ҵ����ݿ�ָ�������ɣ�������ߣ�
    static int OnPlayerRecover(IKernel *pKernel, const PERSISTID &self, const PERSISTID &sender, const IVarList &args);

    //GM����ÿ�������ʽ�����
    static int OnSetCapitalMonitorMaxVal(IKernel *pKernel, const PERSISTID &self, const PERSISTID &sender, const IVarList &args);

	static void ReloadConfig(IKernel* pKernel);

	// ��Ӧ�ͻ�����Ϣ
	static int OnCustomExchangeCapital(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);

	// ��ѯ�һ�����
	const CapitalExchangeRule* QueryExchangeRule(int nExchangeId);
};

#endif //__CapitalModule_H__