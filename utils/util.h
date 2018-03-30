//--------------------------------------------------------------------
// �ļ���:		util.h
// ��  ��:		���ù���
// ˵  ��:		
//				
// ��������:	2014��06��27��
// ��������:	2014��06��27��
// ������:		  ( )
//    :	    
//--------------------------------------------------------------------

#ifndef _GAME_UTIL_H
#define _GAME_UTIL_H

#include "server/IKernel.h"
#include <stdlib.h>
#include <vector>
#include <string>
#include <random>
#include <time.h>
#define UTIL_RAND_MAX RAND_MAX
#define UTIL_FLOAT_EPSILON 0.000001

#define UTIL_ONE_DAY_S 86400
#define UTIL_ONE_HOUR_S 3600

#define ZERO         (1.0e-7f)
#define PI           (3.14159265f)
#define PI2          (6.2831853f)
#define PI_2         (1.570796325f)
#define PI_3         (1.0471975512f)
#define PI_4         (0.7853981625f)
#define PI_6         (0.5235987756f)

#define ANGLE0      (0.0f)
#define ANGLE360    (360.0f)
#define ANGLE_90    (90.0f)
#define ANGLE_60    (60.0f)
#define ANGLE_45    (45.0f)
#define ANGLE_30    (30.0f)

#ifndef INT_MAX

#define INT_MAX (2147483647)

#endif

struct element
{
	element()
	{
		iOrder = 0;
		pName = NULL;
		iPower = 0;
	}
	element( int Order, const char* Name, int Power )
	{
		iOrder = Order;
		pName = Name;
		iPower = Power;
	}
	int iOrder;
	const char* pName;
	int iPower;
};

// ����[0, 32767]��Χ�������
int util_range_random_int();

// ����[0, max]��Χ�������
int util_range_random_int( int max );

// ����[min, max]��Χ�������
int util_range_random_int( int min, int max );

// ����[0.0f, 32767.0f]��Χ���������
float util_range_random_float();

// ����[0.0f, max]��Χ���������
float util_range_random_float( float max );

// ����[min, max]��Χ���������
float util_range_random_float( float min, float max );

// �������һ����Χ�ɵĲ��ظ�������[0, 32767]
void util_different_Random_int( const int min, const int max, const unsigned int num, std::vector<int> & iVecOut );

// Ȩ���ۼӺ�
int util_power_sum_int( const std::vector<int> & iVecIn );

// ��Բ�ܻ��Ƚ�ֵת��Ϊ0-pi*2֮���ֵ
float util_to_normalize_circular( float angle );

// ��Բ�ܽǶ�ֵת��Ϊ0-360��֮���ֵ
float util_to_normalize_angle( float angle );

//�Ƕ�ת����
float util_angle_to_circular( float angle );

//����ת�Ƕ�
float util_circular_to_angle( float angle );

//����Ȩ��ֵȡ�����ֵ
bool util_random_element_int( std::vector<element> & iVecIn, element& Element );

// ���ظ�����
bool util_different_insert_int( const int num, std::vector<int> & iVecOut );

// ȡ�ο���Сֵ
void util_get_min_float( float fDisMin, std::vector<float> & iVecOut );

//�Ƿ����б���
bool util_find_in_list_int( std::vector<int> & vtList, int iIndex );

// ���ظ�����std::vector< std::vector<int> >
void util_different_insert_vector_int( std::vector<int> & vtList, std::vector< std::vector<int> > & iVecOut );

//��ȡ�������ʱ���(����ʱ��)
int64_t util_get_current_zero_unix_timestamp(const int iTimezone = 8);

//��ȡָ��ʱ������ʱ���(����ʱ��)
int64_t util_get_current_zero_unix_timestamp(const int64_t iTimestamp, const int iTimezone = 8);

// ���ظ�����
bool util_different_insert_string( const std::string& str, std::vector<std::string> & iVecOut );

//�Ƿ����б���
bool util_find_in_list_string( std::vector<std::string> & vtList, const std::string& str );

//���ɱ߽籣��ֵ[left, right]
int util_my_rand(int left, int right);

//�����ȫ����[minProtect, limit] ȡ������ƽ��ֵ
int util_average_rand(const int curNum, const int n, const int minProtect = 1);

//����ת��Ϊ�ָ��ַ�
std::wstring util_args_to_wstring(const IVarList& args);

static std::default_random_engine randomSeed(static_cast<unsigned int>(::time(NULL)));
#endif// _GAME_UTIL_H

