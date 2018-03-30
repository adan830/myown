//--------------------------------------------------------------------
// �ļ���:		util.cpp
// ��  ��:		���ù���
// ˵  ��:		
//				
// ��������:	2014��06��27��
// ��������:	2014��06��27��
// ������:		  ( )
//    :	    
//--------------------------------------------------------------------
#include "util.h"
#include "util_func.h"
#include <string>
#include <time.h>
#include "string_util.h"
#include "../public/IVarList.h"

// ����[0,int max]��Χ�������
int util_range_random_int()
{
	std::uniform_int_distribution<int> dis;
	return dis(randomSeed);
}

// ����[0, max]��Χ�������
int util_range_random_int( int max )
{
	if ( max < 0 )
	{
		return 0;
	}
	std::uniform_int_distribution<int> dis(0, max);
	return dis(randomSeed);
}

// ����[min, max]��Χ�������
int util_range_random_int( int min, int max )
{
	if ( min > max )
	{
		int temp;
		temp = min;
		min = max;
		max = temp;
	}

	if ( min < 0 )
	{
		min = 0;
	}

	if ( max <= 0 )
	{
		return 0;
	}

	std::uniform_int_distribution<int> dis(min, max);
	return dis(randomSeed);
	
}

// ����[0.0f, 32767.0f]��Χ���������
float util_range_random_float()
{
	return float( rand() );
}

// ����[0.0f, max)��Χ���������
float util_range_random_float( float max )
{

	if ( max < 0.0f )
	{
		return 0.0f;
	}

	std::uniform_real_distribution<float> dis(0.0, max);
	return dis(randomSeed);

	
}

// ����[min, max)��Χ���������
float util_range_random_float( float min, float max )
{
	if ( min > max )
	{
		float temp;
		temp = min;
		min = max;
		max = temp;
	}

	if ( min < 0.0f )
	{
		min = 0.0f;
	}


	if ( max <= 0.0f )
	{
		return 0.0f;
	}
	std::uniform_real_distribution<float> dis(min, max);
	return dis(randomSeed);
}

// �������һ����Χ�ɵĲ��ظ�������[0, 32767]
void util_different_Random_int( const int min, const int max, const unsigned int num, std::vector<int> & iVecOut )
{
	iVecOut.clear();
	typedef std::vector<int>::iterator iVecOutIterator;
	LoopBeginCheck(a)
	while ( ( ( int )iVecOut.size() < num) ) 
	{
		LoopDoCheck(a)
		int number = util_range_random_int( min, max );
		iVecOutIterator it;
		LoopBeginCheck(b)
		for( it = iVecOut.begin(); it !=iVecOut.end() ; ++it )
		{
			LoopDoCheck(b)
			if( *it == number )
			{
				break;
			}
		}

		if( it ==iVecOut.end() )
		{
			iVecOut.push_back(number);
		}
	}
}

// Ȩ���ۼӺ�
int util_power_sum_int( std::vector<element> & iVecIn )
{
	typedef std::vector<element>::iterator iVecInIterator;
	int sum = 0;
	LoopBeginCheck(c)
	for( iVecInIterator it = iVecIn.begin(); it !=iVecIn.end() ; ++it )
	{
		LoopDoCheck(c)
		sum += (*it).iPower;
	}

	return sum;
}

// ��Բ�ܻ��Ƚ�ֵת��Ϊ0-pi*2֮���ֵ
float util_to_normalize_circular( float angle )
{
	float value = ::fmod( angle, PI2 );
	if ( value < 0 )
	{
		value = value + PI2;
	}

	return value;
}

// ��Բ�ܽǶ�ֵת��Ϊ0-360��֮���ֵ
float util_to_normalize_angle( float angle )
{
	float value = ::fmod( angle, ANGLE360 );
	if ( value < 0 )
	{
		value = value + ANGLE360;
	}

	return value;
}

//�Ƕ�ת����
float util_angle_to_circular( float angle )
{
	return util_to_normalize_angle( angle ) / ANGLE360 * PI2;
}

//����ת�Ƕ�
float util_circular_to_angle( float angle )
{
	return util_to_normalize_circular( angle ) / PI2 * ANGLE360;
}

//����Ȩ��ֵȡ�����ֵ
bool util_random_element_int( std::vector<element> & iVecIn, element& Element )
{
	int iSumPower = util_power_sum_int( iVecIn );
	typedef std::vector<element>::iterator iVecInIterator;
	int iRandPower = util_range_random_int( iSumPower );
	int iTempSumPower = 0;
	LoopBeginCheck(d)
	for( iVecInIterator it = iVecIn.begin(); it !=iVecIn.end() ; ++it )
	{
		LoopDoCheck(d)
		iTempSumPower += (*it).iPower;
		if( iTempSumPower >= iRandPower )
		{
			Element = *it;

			return true;
		}
	}

	return false;
}

// ���ظ�����
bool util_different_insert_int( const int num, std::vector<int> & iVecOut )
{
	typedef std::vector<int>::iterator iVecOutIterator;
	iVecOutIterator it;
	LoopBeginCheck(e)
	for( it = iVecOut.begin(); it !=iVecOut.end() ; ++it )
	{
		LoopDoCheck(e)
		if( *it == num )
		{
			break;
		}
	}

	if( it == iVecOut.end() )
	{
		iVecOut.push_back(num);

		return true;
	}

	return false;
}

// ȡ�ο���Сֵ
void util_get_min_float( float fDisMin, std::vector<float> & iVecOut )
{
	typedef std::vector<float>::iterator iVecOutIterator;
	iVecOutIterator it;
	LoopBeginCheck(f)
	for( it = iVecOut.begin(); it !=iVecOut.end() ; ++it )
	{
		LoopDoCheck(f)
		if( *it < fDisMin )
		{
			fDisMin = *it;
		}
	}
}

//�Ƿ����б���
bool util_find_in_list_int( std::vector<int> & vtList, int iIndex )
{
	LoopBeginCheck(g)
	for ( std::vector<int>::reverse_iterator it = vtList.rbegin(); it != vtList.rend(); ++it )
	{
		LoopDoCheck(g)
		if ( *it == iIndex )
		{
			return true;
		}
	}

	return false;
}

// ���ظ�����std::vector< std::vector<int> >
void util_different_insert_vector_int( std::vector<int> & vtList, std::vector< std::vector<int> > & iVecOut )
{
	typedef std::vector< std::vector<int> >::iterator iVecOutIterator;
	iVecOutIterator it;
	LoopBeginCheck(h)
	for( it = iVecOut.begin(); it !=iVecOut.end() ; ++it )
	{
		LoopDoCheck(h)
		if( *it == vtList )
		{
			break;
		}
	}

	if( it == iVecOut.end() )
	{
		iVecOut.push_back(vtList);
	}
}

//��ȡ�������ʱ���(����ʱ��)
int64_t util_get_current_zero_unix_timestamp(const int iTimezone)
{
	time_t iCurTime = ::time(NULL);
	return iCurTime - (iCurTime % UTIL_ONE_DAY_S  + iTimezone * UTIL_ONE_HOUR_S) % UTIL_ONE_DAY_S;
}

//��ȡָ��ʱ������ʱ���(����ʱ��)
int64_t util_get_current_zero_unix_timestamp(const int64_t iTimestamp, const int iTimezone)
{
	return iTimestamp - (iTimestamp % UTIL_ONE_DAY_S  + iTimezone * UTIL_ONE_HOUR_S) % UTIL_ONE_DAY_S;
}

// ���ظ�����
bool util_different_insert_string( const std::string& str, std::vector<std::string> & iVecOut )
{
	typedef std::vector<std::string>::iterator iVecOutIterator;
	iVecOutIterator it;
	LoopBeginCheck(i)
	for( it = iVecOut.begin(); it !=iVecOut.end() ; ++it )
	{
		LoopDoCheck(i)
		if( *it == str )
		{
			break;
		}
	}

	if( it == iVecOut.end() )
	{
		iVecOut.push_back(str);

		return true;
	}

	return false;
}

//�Ƿ����б���
bool util_find_in_list_string( std::vector<std::string> & vtList, const std::string& str )
{
	LoopBeginCheck(j)
	for ( std::vector<std::string>::reverse_iterator it = vtList.rbegin(); it != vtList.rend(); ++it )
	{
		LoopDoCheck(j)
		if ( *it == str )
		{
			return true;
		}
	}

	return false;
}

//���ɱ߽籣��ֵ[left, right]
int util_my_rand(int left, int right)
{
	if (left >= right)
	{
		return left;
	}
	else 
	{
		int64_t time = ::time(NULL);
		return left + time % (right - left);
	}
}

//�����ȫ����[minProtect, limit] ȡ������ƽ��ֵ
int util_average_rand(const int curNum, const int n, const int minProtect)
{
	if (n <= 1)
	{
		return curNum;
	}

	int limit = (curNum -(n - 1) * minProtect) / (n - 1);
	
	return util_my_rand((int)(minProtect), (int)(limit));
}

//����ת��Ϊ�ָ��ַ�(VTYPE_OBJECT����)
std::wstring util_args_to_wstring(const IVarList& args)
{
	std::wstring tempStr = L"";
	size_t iCount = args.GetCount();
	LoopBeginCheck(k);
	for (size_t i = 0; i < iCount; ++i)
	{
		LoopDoCheck(k);
		int iType = args.GetType(i);
		if (VTYPE_INT == iType)
		{
			if (!tempStr.empty())
			{
				tempStr = tempStr + L":";
			}
			tempStr = tempStr + StringUtil::IntAsWideStr(args.IntVal(i));
		}
		else if (VTYPE_FLOAT == iType)
		{
			if (!tempStr.empty())
			{
				tempStr = tempStr + L":";
			}
			tempStr = tempStr + StringUtil::FloatAsWideStr(args.FloatVal(i));
		}
		else if (VTYPE_DOUBLE == iType)
		{
			if (!tempStr.empty())
			{
				tempStr = tempStr + L":";
			}
			tempStr = tempStr + StringUtil::StringAsWideStr(StringUtil::DoubleAsString(args.DoubleVal(i)).c_str());
		}
		else if (VTYPE_INT64 == iType)
		{
			if (!tempStr.empty())
			{
				tempStr = tempStr + L":";
			}
			tempStr = tempStr + StringUtil::StringAsWideStr(StringUtil::Int64AsString(args.Int64Val(i)).c_str());
		}
		else if (VTYPE_STRING == iType)
		{
			if (!tempStr.empty())
			{
				tempStr = tempStr + L":";
			}
			tempStr = tempStr + StringUtil::StringAsWideStr(args.StringVal(i));
		}
		else if (VTYPE_WIDESTR == iType)
		{
			if (!tempStr.empty())
			{
				tempStr = tempStr + L":";
			}
			tempStr = tempStr + args.WideStrVal(i);
		}
	}

	return tempStr;
}