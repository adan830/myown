//--------------------------------------------------------------------
// �ļ���:		util_func.h
// ��  ��:		���ߺ���
// ˵  ��:		
// ��������:	2008��10��30��
// ������:		 
//    :	   
//--------------------------------------------------------------------

#ifndef _GAME_UTIL_FUNC_H
#define _GAME_UTIL_FUNC_H

#include "../../public/IVarList.h"
#include "extend_func.h"

inline unsigned int SetbitofUint(unsigned int src, unsigned int bitnum) 
{
	unsigned int m = 0x01;
	return  src | (m << bitnum);
}
inline unsigned int ResetbitofUint(unsigned int src, unsigned int bitnum)
{
	unsigned int m = 0x01;
	return src & (~(m << bitnum));
}
inline unsigned int GetbitofUint(unsigned int src, unsigned int bitnum)
{
	unsigned int m = 0x01;
	return  (src >> bitnum ) & m;
}

bool date_from_tm(unsigned int year, unsigned int month, unsigned int day,	
				  unsigned int hour, unsigned int minute, unsigned int second, double& date);

bool decode_date(double date, unsigned int& wYear, unsigned int& wMonth,
				 unsigned int& wDay, unsigned int& wHour,
				 unsigned int& wMinute, unsigned int& wSecond);


// ����0-(scale-1)���������
int util_random_int(int scale);

// ����0-scale)�����������
float util_random_float(float scale);

// �����������Ĵ�С�󷵻�
void util_sort_int(int& minvalue, int& maxvalue);
void util_sort_float(float& minvalue, float& maxvalue);

// ȥ���ַ���ǰ��ո�
std::string util_trim_string(std::string str, const char delims = ' ');

// ȥ�����ַ���ǰ��ո�
std::wstring util_trim_wstring(std::wstring str, const wchar_t delims = L' ');

// �ָ��ַ���,��Բ���ŵ�
bool util_split_string_with_parentheses(IVarList& result, const std::string& str, char delims);

// �Էָdelims����ƥ�����ָ��ַ���
void util_split_string(IVarList& result,
					   const std::string& str,
					   const std::string& delims,
					   unsigned int max_splits = 0);


// �ָ��ַ������ʽ(exp=value,exp1=value1)
bool util_split_express_string(IVarList& result,
					   const std::string& str,
					   const char* exp_equa = "=",
					   const char* exp_split = ",");


// �Էָdelims�е������ַ����ָ��ַ���
void util_split_string_of(IVarList& result,
						  const std::string& str,
						  const std::string& delims = "/\\",
						  unsigned int max_splits = 0);

// �Էָdelims����ƥ�����ָ��ַ���
void util_split_wstring(IVarList& result, 
						const std::wstring& str,
						const std::wstring& delims, 
						unsigned int max_splits = 0);

// �Էָdelims�е������ַ����ָ��ַ���
void util_split_wstring_of(IVarList& result, 
						   const std::wstring& str,
						   const std::wstring& delims = L"/\\", 
						   unsigned int max_splits = 0);

// �ַ���ת���ɿ��ַ���
std::wstring util_string_as_widestr(const char* s);

// ���������ַ���
fast_string util_int_as_string(int value);

// ˫������ת�����ַ���
std::string util_double_as_string(double value, int decimals = 0);

// ˫����ת���ɿ��ַ���
std::wstring util_double_as_widestr(double value, int decimals = 0);

// ������ת�����ַ���
fast_string util_int64_as_string(__int64 value);

// ������ת���ɿ��ַ���
std::wstring util_int64_as_widestr(__int64 value);

// ���ַ���ת�����ַ���
fast_string util_widestr_as_string(const wchar_t * ws);

// ���ַ�ת����UTF-8
fast_string util_widestr_as_utf8(const wchar_t* ws);

// UTF-8ת��Ϊ���ַ�
std::wstring util_utf8_as_widestr(const char* s);

std::wstring util_int_as_widestr(int value);

// �ַ���ת����int64
__int64 util_string_as_int64(const std::string& val);
// ���ַ���ת����int64
__int64 util_widestr_as_int64(const std::wstring& val);

// �ַ����滻
std::string util_replace_string(const std::string& srcString, const std::string& oldStr, const std::string& newStr);
// ���ַ����滻
std::wstring util_replace_wstring(const std::wstring& srcString, const std::wstring& oldStr, const std::wstring& newStr);

// �ַ����滻 ���׼���滻���ַ����ĺ�һλ�Ƿ�Ϊ��ĸ���»���,�ǵĻ���ֹ�滻,ֻ��ս����ʽ��ʹ��,��ֹ��ʽ����
std::string util_replace_string_with_check(const std::string& srcString, const std::string& oldStr, const std::string& newStr);

// ȡ��ǰʱ��
double util_get_now();

// ȡ����
double util_get_date();

// ȡһ���ڵ�ʱ�䣨0.0f - 1.0f��
double util_get_time();

// ȡʱ��������
double util_get_time_diff(double date1, double date2);

// ȡ������
void util_get_year_month_day(int& year, int& month, int& day);

// ȡʱ����
void util_get_hour_minute_second(int& hour, int& minute, int& second);

// ȡ���ڣ�0Ϊ�����죩 utcx
int util_get_day_of_week();

// ȡ���ڣ�0Ϊ�����죩��ʱ����
int util_get_day_of_week(int& hour, int& minute, int& second);
int util_get_day_of_week_ex(time_t now, int& hour, int& minute, int& second);

// ȡ���x�܎�ĳ�rĳ��ĳ��ĕr�g(0��������)( ��ǰ�r�g��0 ȡ��ǰ�r�g)
time_t util_get_first_day_of_week(time_t now, int wday, int hour, int minute, int sec);
// ȡ������˶�������
int util_get_day_elapsed_seconds();
// ��������
int util_calc_seconds( int hour, int minutes , int seconds);


// ȡ����(һ���µĵڼ���)
int util_get_day_of_month();

// �ж��Ƿ�Ϊ����
bool util_is_leap_year(const int year);

// ��ȡ���������
int util_get_days_of_year(const int year);

// ת���������ڵ���������
bool util_encode_date(int year, int month, int day, double& date);

// ת������ʱ�両��ʱ��
bool util_encode_time(int hour, int minute, int second, double& time);

// ת���������ڵ���������
bool util_decode_date(double date, int& year, int& month, int& day);

// ת������ʱ�䵽����ʱ��
bool util_decode_time(double date, int& hour, int& minute, int& second);

// ��õ�ǰʱ��,��ȷ��s  (utc_0)
time_t util_get_utc_time();

// ��õ�ǰʱ��,��ȷ��s  (utc_x)
time_t util_get_utc_time_x();

// ʱ��ת����utc_0 = > utc_x
time_t util_convert_time_utc_0_x(time_t utc_time);

// ʱ��ת����utc_x = > utc_0
time_t util_convert_time_utc_x_0(time_t utc_time);

// ��õ�ǰʱ��,��ȷ��ms
int64_t util_get_utc_ms_time();

// tmʱ�����ձ�׼ʱ�����㣬�ýṹ�ռ��ɵ����߷��䣬ת��Ϊutc����
time_t util_make_utc_time(struct tm*  tmtime);

// ��time_tת����struct tm�ṹ��ʱ�����ձ�׼ʱ�����㣬�ýṹ�ռ��ɵ����߷���
void util_get_utc_time(time_t* ttime, struct tm* tmtime);

// ��time_tת����struct tm�ṹ�����Ҽ��ϱ�����ʱ�����ýṹ�ռ��ɵ����߷���
void util_get_local_time(time_t* ttime, struct tm* tmtime);

// �߾��ȼ���΢��
__int64  __fastcall  usTimeCounter();

// ���ϵͳTicks
unsigned long util_get_ticks();

// ���64λ�ĵ�ǰʱ��,��ȷ��ms
__time64_t util_get_time_64();

// ���64λ��ָ��ʱ��,��ȷ��ms
__time64_t util_get_time_64(int year, int month, int day, int hour, int minute, int second, int Millisecond = 0);

//����ʱ��
bool util_decode_time64(__int64 nTime, int& nYear, int& nMonth, int& nDay,
						int& nHour, int& nMin, int& nSec);
//��/��/��ת��utcʱ��
__time64_t util_encode_time64(int year, int month, int day, int hour, int minute, int second);


// ��time_tת����struct tm�ṹ�����Ҽ��ϱ�����ʱ�����ýṹ�ռ��ɵ����߷���
void util_get_local_time(time_t* ttime, struct tm* tmtime);

// ��time_tת����yy-mm-dd hour:min:sec��ʽ utc0=>utcx
void util_convert_time_to_string(time_t* ttime, char* strtime, size_t len);

// ��time_tת����yy-mm-dd hour:min:sec��ʽ utcx=>utcx
void util_convert_time_to_string_xx(time_t* ttime, char* strtime, size_t len);

//��һ���ַ���ת�������ڣ������Ǳ�׼��yy-mm-dd hour:min:sec��ʽ  utcx=>utc0
void util_convert_string_to_time(const char* strtime, time_t* ttime,bool is_only_date=false);

//��һ���ַ���ת�������ڣ������Ǳ�׼��yy-mm-dd hour:min:sec��ʽ  utcx=>utcx
void util_convert_string_to_time_xx(const char* strtime, time_t* ttime, bool is_only_date = false);

// ��ȡ�ַ�����ʾ�ĵ�ǰ���ڣ���ʽΪ YYYY-MM-DD
void util_convert_date_to_string(char* strtime, int str_size);
// ��ȡ�ַ�����ʾ�ĵ�ǰ���ڣ���ʽΪ YYYY-MM-DD
void util_convert_date_to_string(time_t* ttime, char* strtime, int str_size);

// ��ȡ�ַ�����ʾ�ĵ�ǰ���ڣ���ʽΪ YYYY-MM-DD hh:mm:ss
void util_convert_date_to_string2(std::string &out_str);
// ��ȡ�ַ�����ʾ�ĵ�ǰ���ڣ���ʽΪ YYYY-MM-DD hh:00:00
void util_convert_date_to_string2_sp(time_t* ttime, char* strtime, int str_size);

// �����־��ʽʱ���ַ���
void util_get_log_time_dump(char* strtime, time_t slice = 0);

// ����ִ���ʾ������ ��ʽΪ hour:min:sec
int util_convert_string_to_sec(const char * strtime);

// �ж��Ƿ�ͬһ��
bool util_is_sameday(time_t time1, time_t time2);

// �ж��Ƿ�����
bool util_is_yestoday(time_t today, time_t yestoday);

// �ж��Ƿ�����
bool util_is_tomorrow(time_t today, time_t tomorrow);

// �ж��Ƿ�ͬһ��
bool util_is_sameweek(time_t time1, time_t time2);
// �ж��Ƿ�ͬһ��
bool util_is_samemonth(time_t time1, time_t time2);
// �ж��Ƿ�ͬһ��
bool util_is_sameyear(time_t time1, time_t time2);

// �������������time1-time2
int util_subtract_days(time_t time1, time_t time2);

// ��õ����ʱ���ܹ�����
int util_convert_zerotime_to_sec();

// ����64λΨһ����id
uint64_t util_gen_unique_id();

// ������ľ���
float util_dot_distance(float x_1, float y_1, float x2, float y2);

// ������ľ����ƽ��
float util_dot_distance2(float x_1, float y_1, float x2, float y2);

// ����ά�ռ������ľ���
float util_dot_distance(float x_1, float y_1, float z1, float x2, float y2, float z2);

// ����ά�ռ������ľ����ƽ��
float util_dot_distance2(float x_1, float y_1, float z1, float x2, float y2, float z2);

// ������������ʸ���Ļ���ֵ(����0~2*pi��ֵ����y��������Ϊ0��)
float util_dot_angle(float x_1, float y_1, float x2, float y2);

// ��-pi~pi�Ļ���ֵת��Ϊ0~2*pi�Ļ���ֵ
float util_normalize_angle(float angle);

// ��angle1��angle2�ĽǶȲ����0~pi�Ļ���ֵ
float util_get_angle_diff(const float & angle1, const float & angle2);

// �Ƚ������������Ƿ����Ϊ���
bool util_float_equal(float f1, float f2);

bool util_float_equal_zero(float f);

bool util_double_equal_zero(double f);

// ͨ������ConfigID�õ�Ѱ·��ʹ�õĳ���Resource
void util_get_scene_resource_by_configid(std::string& strRet, const std::string& config_id);

// �ж�һ���ַ����ĺϷ���
#define IsInvalidString(szChar)     (NULL == (szChar) || '\0' == *(szChar))

// ���ټ�����ŷ�֮1
inline float Invfast_sqrt(float x)
{
	float xhalf = 0.5f * x;

	int i = *(int *)&x;             // ��������BITǿ�и���������

	i = 0x5f375a86 - (i >> 1);      // Quake-III Arena (����֮��3)�ĵײ����

	x = *(float *)&i;               // convert bits back to float

	x = x * (1.5f - xhalf * x * x); // ţ�ٵ�����Newton step, repeating increases accuracy

	return x;
}

// fast_sqrt
inline float fast_sqrt(const float x)
{
	return x * Invfast_sqrt(x);
}

// fast fabs
inline float fast_fabs(const float num)
{
	unsigned int ret = *((unsigned int*)&num);

	ret = ret << 1;
	ret = ret >> 1;

	return *((float*)&ret);
}

#if 0
inline int fast_abs(const int num)
{
	int ret = num;
	int tmp = num;

	tmp = tmp >> 31;
	ret = ret ^ tmp;
	ret = ret - tmp;

	return ret;
}
#endif


// ȡʱ�������
void util_get_hour_minute_second_milliseconds(int& hour, int& minute, int& second, int& nMilliseconds);

// ����Ψһ������id
std::string util_gen_unique_name();

// ����64λΨһ����id
uint64_t util_gen_unique_id();

// 4��intѹ����һ��int (-128  -- 127 ֮ǰ����ֵ)
int util_compress_int4(int iUpHigh = 0, int iUpLow = 0, int iDownHigh = 0, int iDownLow = 0);

// 2��intѹ����һ��int ��-32758 -- 32767 ֮�����ֵ��
int util_compress_int2(int iHighValue = 0, int iLowValue = 0);
//�����һ������ʱ������
int64_t util_get_next_hour_sec();

// ʱ����Դ��룬��֤������ʱ�䣬�ַ���ʱ�䣬ʱ����Ӱ�졣
bool util_time_test_utc_0_X();

#endif // _GAME_UTIL_FUNC_H
 
