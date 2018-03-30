//--------------------------------------------------------------------
// �ļ���:		util_time.h
// ��  ��:		ʱ��ṹ�帨��
// ˵  ��:		
//				
// ��������:		2016��07��06��
// ������:		lihl
//    :	    
//--------------------------------------------------------------------

#ifndef _GAME_UTIL_TIME_H
#define _GAME_UTIL_TIME_H

struct STShortTime
{
	unsigned char hour;				// ʱ(0-23)
	unsigned char minute;			// ��(0-59)
	unsigned char seconds;			// ��(0-59)

	bool operator == ( const STShortTime& right) const
	{
		return hour == right.hour
			&& minute == right.minute
			&& seconds == right.seconds;
	}

	bool operator > (const STShortTime& right) const
	{
		return hour > right.hour ||
			(hour == right.hour &&
				(minute > right.minute ||
					(minute == right.minute && seconds > right.seconds) )
				);
	}

	bool operator < (const STShortTime& right) const
	{
		return hour < right.hour ||
			(hour == right.hour &&
			(minute < right.minute ||
				(minute == right.minute && seconds < right.seconds))
				);
	}

	bool operator != (const STShortTime& right) const
	{
		return !(*this == right);
	}

	bool operator >= (const STShortTime& right) const
	{
		return !(*this < right);
	}

	bool operator <= (const STShortTime& right) const
	{
		return !(*this > right);
	}
};

// �����ڵ�ʱ��
struct STWeekTime : public STShortTime
{
	unsigned char week_day;				// ��(0-6)

	bool operator == (const STWeekTime& right) const
	{
		return week_day == right.week_day 
			&& STShortTime::operator==( right );
	}

	bool operator > (const STWeekTime& right) const
	{
		return week_day > right.week_day
			|| STShortTime::operator==(right);
	}

	bool operator < (const STWeekTime& right) const
	{
		return week_day < right.week_day
			|| STShortTime::operator==(right);
	}

	bool operator != (const STWeekTime& right) const
	{
		return !(*this == right);
	}

	bool operator >= (const STWeekTime& right) const
	{
		return !(*this < right);
	}

	bool operator <= (const STWeekTime& right) const
	{
		return !(*this > right);
	}
};


struct STShortTimeRange
{
	STShortTime start;
	STShortTime end;

	bool InRange(const STShortTime& st) const
	{
		return st >= start && st <= end;
	}
};

struct STWeekTimeRange
{
	STWeekTime start;
	STWeekTime end;

	bool InRange(const STWeekTime& st) const
	{
		return st >= start && st <= end;
	}
};

#endif// _GAME_UTIL_TIME_H

