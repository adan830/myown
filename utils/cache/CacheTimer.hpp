//--------------------------------------------------------------------
// �ļ���:		CacheTimer.h
// ��  ��:		cache timer ���׼�ʱ�������ܷ����������ʱ��Ӧ��
// ˵  ��:		
// ��������:	2017��06��03��
// ������:		kevin
// ��Ȩ����:	WhalesGame Technology co.Ltd
//--------------------------------------------------------------------

#ifndef __CACHE_TIMER_HPP__
#define	__CACHE_TIMER_HPP__

//#include <windows.h>
#include "CacheString.hpp"

namespace cache{

// work time param
struct CacheWorkParam 
{
public:
	unsigned int  count_;	//����
	unsigned long time_;	//ʱ��
public:
	CacheWorkParam() :count_(0),time_(0){}
public:
	//for init.
	void Init(unsigned int count, unsigned long time)
	{
		count_ = count;
		time_ = time;
	}
	void Set(const CacheWorkParam& ref)
	{
		count_ = ref.count_;
		time_ = ref.time_;
	}
	//for clear
	void Clear()
	{
		count_ = 0;
		time_  = 0;
	}
	// for valid
	bool IsValid() const
	{
		if (count_>0 || time_>0)
		{
			return true;
		}
		return false;
	}
};
// work timer
struct CacheWorkTimer
{
private:
	bool is_expired_;
	unsigned long start_time_;	//��ʼʱ��
private:
	CacheWorkParam limit_param_;
	CacheWorkParam work_prev_;
	CacheWorkParam work_param_;
public:
	CacheWorkTimer() :is_expired_(false), start_time_(0){}
public:
	/// \brief ��ʼ������ʱ��
	/// \param work_count ���д�����Ĭ��0��0��ʾ������
	/// \param work_time �������ʱ��,��λ���룬Ĭ��0��0��ʾ������
	void Init(unsigned int work_count = 0, unsigned long work_time = 0)
	{
		is_expired_ = false;
		start_time_ = GetCurTicket();

		limit_param_.Init(work_count, work_time);
		work_prev_.Clear();
		work_param_.Clear();
	}
	/// \brief Valid
	bool IsValid() const { return limit_param_.IsValid(); }
public:
	/// \brief ����
	void ReInit()
	{
		is_expired_ = false;
		start_time_ = GetCurTicket();
		work_prev_.Clear();
		work_param_.Clear();
	}
	/// \brief ���´���
	void Update()
	{
		work_prev_.Set(work_param_);
		work_param_.count_ += 1;
		if (limit_param_.count_>0 && work_param_.count_ > limit_param_.count_)
		{
			is_expired_ = true;
		}
		work_param_.time_ = GetCurTicket() - start_time_;
		if (limit_param_.time_ > 0 && work_param_.time_ > limit_param_.time_)
		{
			is_expired_ = true;
		}
	}
	/// \brief ����Ƿ����
	/// \return  true:����, false��δ���ڡ�
	bool IsExpired() const
	{
		return is_expired_;
	}
	/// \brief ���ع�������
	const CacheWorkParam& GetWorkPrev() const
	{
		return work_prev_;
	}
	const CacheWorkParam& GetWorkParam() const
	{
		return work_param_;
	}
private:
	/// \brief ����ʱ��
	unsigned long GetCurTicket()
	{ 
		return ::GetTickCount(); 
	}
};



}
#endif