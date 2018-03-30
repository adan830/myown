// --------------------------------------------------------------
//  �ļ���:     extend_func.h
//  ��  ��:     ��չ������
//  ˵  ��:     ��Ϸ�߼��л�϶౻ʹ�õ��Ĺ��ܺ�������
//  ��������:   2009��2��1��
//  ������:     �����
//     :      
// --------------------------------------------------------------
#ifndef _GAME_extend_func_H
#define _GAME_extend_func_H

#include "../public/IVarList.h"
#include "../SDK/tinyxml/tinyxml.h"
#include "../SDK/rapidxml/rapidxml.hpp"
#include "../public/Inlines.h"
#include <string>
#include "../FsGame/Define/LogDefine.h"

#ifdef FSROOMLOGIC_EXPORTS
#include "../server/IRoomKernel.h"
#else
#include "../server/IKernel.h"
#endif

#define ZERO    (1.0e-7f)
#define PI      (3.14159265f)
#define PI2     (6.2831853f)
#define PI_2    (1.570796325f)
#define PI_3    (1.0471975512f)
#define PI_4    (0.7853981625f)
#define PI_6    (0.5235987756f)

#ifndef INT_MAX
#define INT_MAX (2147483647)
#endif

using namespace rapidxml;

// FastString ����
typedef TFastStr<char, 128> fast_string;
typedef TFastStr<wchar_t, 64> fast_wstring;

// ���ڶ���
enum CycleType
{
	CYCLE_TYPE_NONE		= 0,
	CYCLE_TYPE_DAY		= 1,	// ÿ��
	CYCLE_TYPE_WEEK		= 2,	// ÿ��
	CYCLE_TYPE_MONTH	= 3,	// ÿ��
	CYCLE_TYPE_YEAR		= 4,	// ÿ��
};

// �Ƚ�
template <class T> bool compare(const T & var1, const T & var2, const char * op)
{
    if (stricmp(op, "==") == 0)
    {
        return var1 == var2;
    }
    else if (stricmp(op, ">=") == 0)
    {
        return var1 >= var2;
    }
    else if (stricmp(op, "<=") == 0)
    {
        return var1 <= var2;
    }
    else if (stricmp(op, ">") == 0)
    {
        return var1 > var2;
    }
    else if (stricmp(op, "<") == 0)
    {
        return var1 < var2;
    }
    else if (stricmp(op, "!=") == 0)
    {
        return var1 != var2;
    }
    return false;
}

// �Ƚ϶���������Ƿ���������
bool compare_property(IKernel * pKernel, const PERSISTID & obj, const char * prop,
                      const char * compare_value, const char * op);

// ������־����
bool extend_log_level(IKernel* pKernel, LogLevelEnum level, const char* IncluedInfo);

// ���������Ϣ
void extend_warning(IKernel * pKernel, const char * info);

// ���������Ϣ
void extend_warning(LogLevelEnum level, const char* format, ...);

// ��������ģ�����������Ϣ
void extend_warning_pub(LogLevelEnum level, const char* format, ...);

// �ַ������͵Ķ������ͣ�"TYPE_PLAYER", "TYPE_SCENE"...��ת��Ϊ����
int convert_class_type(const char * sz_classtype);

// �ַ������͵��������ͣ�"string", "widestr", "int", "int64" ...��ת��Ϊ����
int convert_var_type(const char * sz_vartype);

// �ַ������͵��ַ�������(��ȫ���)
const char* convert_string(const char * szstring, const char* defaultstring);

// �ַ������͵Ĳ������ͣ�"true", "false"��ת��Ϊ������
bool convert_boolean(const char * szboolean);

// �ַ������͵���������ת��Ϊ����
int convert_int(const char * szint, int default_int = 0);

// �ַ������͵���������ת��Ϊ64λ����
int64_t convert_int64(const char * szint64, int64_t default_int64);

// �ַ������͵ĸ�����ת��Ϊ������
float convert_float(const char * szfloat, float default_float = 0.0f);

// �ַ������͵ĸ�����ת��Ϊ˫���ȸ�����
double convert_double(const char * szdouble, double default_double);

// ��XML�ļ���Ϊ�߼��ഴ������
// xmlfile��һ���Է���������Դ·��Ϊ���·�����ļ���
bool createclass_from_xml(IKernel * pKernel, int index, const char * xmlfile);

// ��ȡ��һ��ָ�����͵ĸ�����
PERSISTID find_firsttype_parent(IKernel * pKernel, const PERSISTID & obj, int type);

// ���ĳ����Ϸ����npc��player�������˵ĺ���
PERSISTID get_sceneobj_master(IKernel * pKernel, const PERSISTID & self);
IGameObj* get_sceneobj_master(IKernel* pKernel, IGameObj* pNpcObj);

// ��ȡ��������
PERSISTID get_pet_master(IKernel* pKernel, const PERSISTID& self);

// ִ���߼�ģ��Ľű�
bool run_logic_lua_script(IKernel * pKernel, const char * script, const char * func,
                          const IVarList & args, int res_num = 0, IVarList * res = NULL);

// ��ĳ����Χ�ڻ������Ŀ����ߵ�
bool get_random_position(IKernel * pKernel, float center_x, float center_z, float radius,
                         float & random_x, float & random_y, float & random_z, float & random_orient);

// ���浽XML��ʽ���ַ����У��Զ����ʽ��û��XML�ļ����õ�ͷ��Ϣ�ȣ�
bool save_to_string(IKernel * pKernel, const PERSISTID & self, fast_string& szContent,
                    bool bIncludeUnsave, bool bIncudeInvisible);


bool save_to_xml(IKernel * pKernel, const PERSISTID & self, TiXmlElement & element,
                 bool bIncludeUnsave, bool bIncudeInvisible);

bool out_put_xml(IKernel * pKernel, const PERSISTID & self, const char* file);

bool output_object_xml(IKernel * pKernel, const PERSISTID & self, TiXmlElement* root);

// ��ȡ��������Ϣ
bool player_info_to_string(IKernel * pKernel, const PERSISTID & player, fast_string & szContent);
// ȡ�����������ϵ�����
bool player_prop_to_string(IKernel * pKernel, const PERSISTID & player, fast_string & szContent);
// ȡһ��װ������
bool player_equip_to_string(IKernel * pKernel, const PERSISTID & equip, fast_string & szContent);
// ȡһ����Ʒ����
bool player_item_to_string(IKernel * pKernel, const PERSISTID & item, fast_string & szContent);

// �����汣���XML���ݵ��ַ���������Ʒ
PERSISTID load_from_string(IKernel * pKernel, const PERSISTID & parent, const std::string & szContent, int index = 0);

PERSISTID load_from_xml(IKernel * pKernel, const PERSISTID & parent, const TiXmlElement & element, int index = 0);

// ��������ֵΪ�ջ���0
bool is_save_prop_null(const char * prop);

// ���ɵ�������ƷΨһID
void gen_item_unique_id(IKernel * pKernel, const PERSISTID & item);

// �յ��ͻ��˵���˵�ʱ��֤�˵���Ч��
bool IsFuncidValid(IKernel * pKernel, const PERSISTID & player, const PERSISTID & npc, int funcid);

// �½��˵���ʱ��ע��һ�£��Է���ͻ��˷��ص���֤
bool RegFuncid(IKernel * pKernel, const PERSISTID & player, const PERSISTID & npc, int funcid);

// ���ע��Ĳ˵�funcid
bool ClearRegistedFuncid(IKernel * pKernel, const PERSISTID & player);

// ����뵱ǰ�߶���ӽ��Ĳ�߶�
float GetCurFloorHeight(IKernel * pKernel, float x, float y, float z, PERSISTID self = PERSISTID());

//// ����������뵱ǰ�߶���ӽ��Ĳ�߶�
float GetCurFloorHeightFromObj(IKernel * pKernel, const PERSISTID & obj, float x, float y, float z);

// ��ȡx,zλ�ô������ϵ�һ����
float GetHeight(IKernel * pKernel, float x, float z, PERSISTID self = PERSISTID());

// ��ȡx, y, zλ���µ�һ��Ĳ��
float GetRootFLoorHeight(IKernel * pKernel, float x, float y, float z);

// ��ȡx,zλ�ô������ϵ�һ��վ������
float GetFirstStandHeight(IKernel * pKernel, float x, float z);

// ��ȡx,zλ�ô������ϵ�һ������߲��
float GetWalkHeight(IKernel * pKernel, float x, float z, int nFloor);

// �����Ч�Ŀ�վ���߶�
float get_valid_height(IKernel * pKernel, float dx, float dy, float dz);

// ��ȡx��������ƶ������ϵ�����
float GetPosiX(IKernel * pKernel, const PERSISTID & self, bool bRealPos = false);

// ��ȡy��������ƶ������ϵ�����
float GetPosiY(IKernel * pKernel, const PERSISTID & self, bool bRealPos = false);

// ��ȡz��������ƶ������ϵ�����
float GetPosiZ(IKernel * pKernel, const PERSISTID & self, bool bRealPos = false);

// ��ȡ�ǶȰ����ƶ������ϵ�����
float GetOrient(IKernel * pKernel, const PERSISTID & self, bool bRealPos = false);

// ��ȡ��������ƶ������ϵ�����
// ��ȡ��������ƶ������ϵ�����
void GetLocation(IKernel * pKernel, const PERSISTID & self, float & x, float & y, float & z, float & orient, bool bRealPos = false);

// ��ȡlink֮�����ʵ����
bool GetRealPos(IKernel* pKernel,PERSISTID self,float& x,float& y,float& z,float& orient);

// objת��Ϊstring
fast_string ObjToString(const PERSISTID & obj);

// �ַ���ת��Ϊobj
PERSISTID StringToObj(const fast_string & str);

//��ȡ��ɫ��š���ǰ�ı�Ź�����6λ�����������ҷֱ��ʾΪ����һλ��Ӫ��ţ��ڶ�����λ�����š����ġ���λְҵ��š�����λ�Ա���
int GetRoleIndex(IKernel* pKernel, const PERSISTID& player);

//����Ƿ��ڶ���ǰ��
bool CheckObjectFront(float fStartX,float fStartZ,float fStartOrient,float fEndX,float fEndZ);

// ��鵱ǰʱ�������ʱ������ķ�����
int GetPassMinutes(const int64_t& date);

//�ļ���
char* GetFileBuff(const char* file_name);

//��ѯXML����
const char* QueryXmlAttr(xml_node<>* pItem, const char* name, const char* def = NULL);

//����������
void GetCenterPos(IKernel* pKernel, float& centerx, float& centerz);

//�����ַ���ʱ�� [Сʱ�����ӣ�����]ת����
int util_analyzing_time_str(const char* pTimeStr);

//�����ַ���ʱ�� [���ڣ�Сʱ�����ӣ�����]ת����
bool util_analyzing_week_time_str(const char* pTimeStr, int& iWeek, int& iSec);

//ƴ���ض��ַ���
std::string util_get_string(const char* pUid, const int iNation, const int iType);

//�����ַ� [uid�����ң�����]
bool util_analyzing_uid_nation_type(const char* pStr, std::string& uid, int& nation, int& type, time_t& time);

//ƴ���ض��ַ���[���ң�����]
std::string util_get_string(const int iNation, const int iType);

//ƴ���ض��ַ���
std::string util_get_string(const char* pStr, const int iNation, const int iType, const time_t iCurTime);

// ��ȡ����ʱ��
// nNextCyc ���� �¼������� ���� �ϼ�������
int64_t get_cycle_time(int nCycType, int nDay, int nSec, int nNextCyc = 0);

//#ifdef _DEBUG
#define OPEN_COST_TIME 
#ifdef OPEN_COST_TIME

#include <Windows.h>
#include <vector>

class CostTime
{
public:
	CostTime(const char* flag):m_flag(flag)
	{
		::QueryPerformanceFrequency(&m_frequency);
		::QueryPerformanceCounter(&m_counter);
		m_beginTime = double(m_counter.QuadPart) / double(m_frequency.QuadPart);
		m_endTime = m_beginTime;
	}

	~CostTime()
	{
		::QueryPerformanceFrequency(&m_frequency);
		::QueryPerformanceCounter(&m_counter);
		m_endTime = double(m_counter.QuadPart) / double(m_frequency.QuadPart);
		double lasttime = m_endTime - m_beginTime;
		char strtemp[256];
		SPRINTF_S(strtemp, "%s Last Time %f\r\n", m_flag.c_str(), lasttime);
		m_vecCostTime.push_back(strtemp);
	}
private:
	LARGE_INTEGER m_frequency;
	LARGE_INTEGER m_counter;
	double m_beginTime;
	double m_endTime;
	std::string m_flag;
public:
	static std::vector<std::string> m_vecCostTime;
};

// ��¼����������ĺ�ʱ
#define RECORD_COST_TIME(flag) CostTime costTime_##flag(#flag)
// ��¼����κ�ʱ
#define RECORD_COST_TIME_BEGIN(flag) CostTime costTime_##flag(#flag)
#define RECORD_COST_TIME_END(flag) costTime_##flag.~costTime_##flag()
// �����ʱ ��¼��ʱ��������߼�
#define TRACE_COST_TIME(pKernel) \
{\
	for(int i = 0; i < (int)CostTime::m_vecCostTime.size(); ++i) \
{\
	pKernel->Trace(CostTime::m_vecCostTime[i].c_str()); \
}\
	CostTime::m_vecCostTime.clear(); \
}

#endif // OPEN_COST_TIME
//#endif // _DEBUG
#endif // _GAME_extend_func_H

