//--------------------------------------------------------------------
// �ļ���:      ResetTimerModule.cpp
// ��  ��:      ʱ������ģ��
// ˵  ��:
// ��������:    2016��03��28��
// ������:        tongzt
// �޸���:             
//--------------------------------------------------------------------

#include "ResetTimerModule.h"
#include "FsGame/CommonModule/LuaExtModule.h"
#include "FsGame/Define/CommandDefine.h"

#include "utils/util_func.h"
#include "utils/xmlfile.h"
#include "utils/exptree.h"
#include "utils/string_util.h"
#include "utils/math/fm_fixedpoint.h"
#include "public/inlines.h"
#include <algorithm>
#include <time.h>
#include "FsGame/CommonModule/ReLoadConfigModule.h"
#include "FsGame/Define/GameDefine.h"

const char * RESET_TIMER_CONFIG = "ini/SystemFunction/resettime/ResetTimeList.xml";

/* ------------------------------------------------
 * ����ˢ�¼������
 * ----------------------------------------------*/
int nx_reload_reset_interval_resource(void *state)
{
    IKernel *pKernel = LuaExtModule::GetKernel(state);

    ResetTimerModule::m_resetTimerModule->LoadResource(pKernel);

    return 1;
}


/* ------------------------------------------------
 * ��̬������ʼ��
 * ----------------------------------------------*/
ResetFuncMap        ResetTimerModule::m_funcMap;
ResetdDataMap       ResetTimerModule::m_ResetDataMap;
ResetTimerModule*   ResetTimerModule::m_resetTimerModule = NULL;


bool ResetTimerModule::Init(IKernel *pKernel)
{
    m_resetTimerModule = this;

    pKernel->AddEventCallback("player", "OnEntry",   OnEntry);
    pKernel->AddEventCallback("player", "OnReady",   OnReady);
  
    DECL_HEARTBEAT(ResetTimerModule::HB_ResetTimer);
    DECL_HEARTBEAT(ResetTimerModule::HB_ResetFirst);

    LoadResource(pKernel);

	RELOAD_CONFIG_REG("ResetTimerConfig", ResetTimerModule::ReloadConfig);
    return true;
}

bool ResetTimerModule::Shut(IKernel *pKernel)
{
	m_ResetDataMap.clear();
    return true;
}

bool ResetTimerModule::LoadResource(IKernel* pKernel)
{
	m_ResetDataMap.clear();
    std::string path = std::string(pKernel->GetResourcePath()) + RESET_TIMER_CONFIG;

    CXmlFile xml(path.c_str());
    if (!xml.LoadFromFile())
    {
        return false;
    }

	LoopBeginCheck(a);
    for (int i = 0; i < (int)xml.GetSectionCount(); ++i)
    {
		LoopDoCheck(a);
        const char* secName = xml.GetSectionByIndex(i);
		if (StringUtil::CharIsNull(secName))
        {
            continue;
        }

        const int type = atoi(secName);
		if (type >= RESET_TYPES_TOTAL 
			|| type <= RESET_TYPES_NONE)
		{
			continue;
		}

		ResetData data;
		data.reset_timer = xml.ReadString(secName, "Clock", "");
		const int weekday = xml.ReadInteger(secName, "ResetWeekday", -1);
		if (weekday > -1 && weekday < 7)
		{
			data.reset_type = TIMER_TYPE_WEEK;
			data.reset_weekday = weekday;
		}

		if (data.reset_timer.empty())
        {
            continue;
        }

        m_ResetDataMap.insert(std::make_pair(type, data));
    }

    return true;
}

// ע�ắ��ָ��
void ResetTimerModule::InitFunc(const ResetTimerTypes fun_type, ResetTimer func)
{
	if (fun_type <= RESET_TYPES_NONE 
		|| fun_type >= RESET_TYPES_TOTAL)
	{
		return;
	}

	if (NULL == func)
	{
		return;
	}

	// �Ѵ���
	ResetFuncMap::const_iterator find_it = m_funcMap.find(fun_type);
	if (find_it != m_funcMap.end())
	{
		return;
	}

	m_funcMap.insert(std::make_pair(fun_type, func));
}

bool ResetTimerModule::Register(IKernel *pKernel, const PERSISTID &self, int type)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return false;
	}

    IRecord* pRecord = pSelfObj->GetRecord(FIELD_RECORD_RESET_TIMER_REC);
    if (NULL == pRecord)
    {
        return false;
    }

    // ��¼���ˢ��ʱ��
    int row = pRecord->FindInt(COLUMN_RESET_TIMER_REC_TYPE, type);
    if (row >= 0)
    {
        // �Ѿ�����
        return true;
    }

    // ����ˢ��ʱ��
    ResetdDataMap::const_iterator itval = m_ResetDataMap.find(type);
    if (itval == m_ResetDataMap.end())
    {
        return false;
    }
    
    // ��ӵ�����
    pRecord->AddRowValue(-1, CVarList() << type << -1 << -1 << 0.0);

    return true;
}

bool ResetTimerModule::UnRegister(IKernel *pKernel, const PERSISTID &self, int type)
{
    if (!pKernel->Exists(self))
    {
        return false;
    }

	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return false;
	}

    IRecord *pRecord = pSelfObj->GetRecord(FIELD_RECORD_RESET_TIMER_REC);
    if (NULL == pRecord)
    {
        return false;
    }

    int row = pRecord->FindInt(COLUMN_RESET_TIMER_REC_TYPE, type);
    if (row >= 0)
    {
        pRecord->RemoveRow(row);
    }

    return true;
}

int ResetTimerModule::OnEntry(IKernel *pKernel, const PERSISTID &self, 
	const PERSISTID &sender, const IVarList &args)
{
    if (!pKernel->Exists(self))
    {
        return 0;
    }

    // ÿ5���Ӽ��һ��ˢ��ʱ��
    if (!pKernel->FindHeartBeat(self, "ResetTimerModule::HB_ResetFirst") && 
		!pKernel->FindHeartBeat(self, "ResetTimerModule::HB_ResetTimer"))
    {
        int curhour, curmin, cursec;
        util_get_hour_minute_second(curhour, curmin, cursec);

        int time = RESET_HEART_BEAT_CYCLE * 60 - (curmin % RESET_HEART_BEAT_CYCLE * 60 + cursec);

        pKernel->AddCountBeat(self, "ResetTimerModule::HB_ResetFirst", time * 1000, 1);
    }

    return 0;
}

int ResetTimerModule::OnReady(IKernel *pKernel, const PERSISTID &self, 
	const PERSISTID &sender, const IVarList &args)
{
    if (!pKernel->Exists(self))
    {
        return 0;
    }

    // ����Ƿ���Ҫˢ��
    HB_ResetTimer(pKernel, self, -1);

    return 0;
}

int ResetTimerModule::HB_ResetFirst(IKernel *pKernel, const PERSISTID &self, int slice)
{
    if (!pKernel->FindHeartBeat(self, "ResetTimerModule::HB_ResetTimer"))
    {
        pKernel->AddHeartBeat(self, "ResetTimerModule::HB_ResetTimer", RESET_HEART_BEAT_CYCLE * 60 * 1000);
    }

    HB_ResetTimer(pKernel, self, -1);

    return 0;
}

int ResetTimerModule::HB_ResetTimer(IKernel *pKernel, const PERSISTID &self, int slice)
{
    if (!pKernel->Exists(self))
    {
        return 0;
    }

	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return 0;
	}

	// ��ǰʱ��
	time_t date = ::time(NULL);
	tm* cur_time = ::localtime(&date);
	if (NULL == cur_time)
	{
		return 0;
	}

	const int cur_hour = cur_time->tm_hour;
    const double cur_date = util_get_date();
	const int cur_weekday = util_get_day_of_week();

    IRecord *pRecord = pSelfObj->GetRecord(FIELD_RECORD_RESET_TIMER_REC);
    if (NULL == pRecord)
    {
        return 0;
    }

    std::vector<int> vecRemove;
    CVarList lastResetList, resetList;
	LoopBeginCheck(b);
    for (int i = 0; i < pRecord->GetRows(); ++i)
    {
		LoopDoCheck(b);
        const int type = pRecord->QueryInt(   i, COLUMN_RESET_TIMER_REC_TYPE);
        const int last_weekday = pRecord->QueryInt(i, COLUMN_RESET_TIMER_REC_WEEKDAY);
        const int last_hour = pRecord->QueryInt(i, COLUMN_RESET_TIMER_REC_HOUR);
        const double last_date = pRecord->QueryDouble(i, COLUMN_RESET_TIMER_REC_DATE);
        
        // ����ˢ�º���
        ResetFuncMap::const_iterator itfunc = m_funcMap.find(type);
        if (itfunc == m_funcMap.end())
        {
            vecRemove.push_back(type);
            std::string errorMess = StringUtil::IntAsString(type);
            extend_warning(LOG_ERROR, "[ResetTimerModule::HB_ResetTimer]: ID %s Timer does not exist ...", errorMess.c_str());
            continue;
        }

        // ����ˢ��ʱ���б�
        ResetdDataMap::const_iterator data = m_ResetDataMap.find(type);
		if (data == m_ResetDataMap.end())
        {
            continue;
        }

		bool can_reset = false;

		// ��ˢ��
		if (data->second.reset_type == TIMER_TYPE_WEEK)
		{
			can_reset = CanWeekReset(&data->second, cur_hour, cur_date, cur_weekday, last_hour, last_date, last_weekday);
		}
		else if (data->second.reset_type == TIMER_TYPE_DAY)
		{
			can_reset = CanDayReset(&data->second, cur_hour, cur_date, last_hour, last_date);
		}

		if (can_reset || last_date == 0 || last_hour == -1 || last_weekday == -1)
        {
            // ִ��ˢ�²���
            (itfunc->second)(pKernel, self, slice);

            pRecord->SetInt(i, COLUMN_RESET_TIMER_REC_HOUR, cur_hour);
            pRecord->SetDouble(i, COLUMN_RESET_TIMER_REC_DATE, cur_date);
			pRecord->SetInt(i, COLUMN_RESET_TIMER_REC_WEEKDAY, cur_weekday);

// 			if (data->second.reset_type == TIMER_TYPE_DAY)
// 			{
// 				// ��֪��������ģ��,�޸Ĳ�������
// 				CVarList msg;
// 				msg << COMMAND_COMPENSATE_RESET << type;
// 				pKernel->Command(self, self, msg);
// 			}	
        }
    }

    if (!vecRemove.empty())
    {
		LoopBeginCheck(d);
        for (int i = 0; i < (int)vecRemove.size(); ++i)
        {
			LoopDoCheck(d);
            int removeType = vecRemove[i];
            int removeRow = pRecord->FindInt(COLUMN_RESET_TIMER_REC_TYPE, removeType);
            if (removeRow >= 0)
            {
                pRecord->RemoveRow(removeRow);
            }
        }
    }

    return 0;
}

// ��ѯʱ��ڵ�֮��ˢ�´���
int ResetTimerModule::GetResetTimes(IKernel *pKernel, const PERSISTID &self,
									int nType, const double startTime, const double endTime)
{
	double offset = endTime - startTime;
	if (offset < 0)
	{
		return 0;
	}

	int nDays = (int)offset;

	// ����ˢ��ʱ���б�
	ResetdDataMap::const_iterator data = m_ResetDataMap.find(nType);
	if (data == m_ResetDataMap.end())
	{
		return 0;
	}

	// �ų���ˢ��
	if (data->second.reset_type == TIMER_TYPE_WEEK)
	{
		return 0;
	}

	// ÿ��ˢ�´���
	CVarList resetList;
	std::string resetClockStr = data->second.reset_timer;
	util_split_string(resetList, resetClockStr, ",");
	int nResetTimes = (int)resetList.GetCount();

	// �������Ĵ���
	int nCount = nDays * nResetTimes;

	// �������
	int nIndexStart = -1;
	int	nIndexEnd = -1;
	double beginTime = startTime - (int)startTime;
	double finishTime = endTime - (int)endTime;
	LoopBeginCheck(e);
	for (int i = 0; i < nResetTimes; ++i)
	{
		LoopDoCheck(e);
		int resetTime = atoi(resetList.StringVal(i));
		double tempTime = resetTime / 24.0;
		if (tempTime > beginTime && nIndexStart < 0)
		{
			nIndexStart = i;
		}
		if (tempTime > finishTime && nIndexEnd < 0)
		{
			nIndexEnd = i;
		}
	}
	if (nIndexStart < 0)
	{
		nIndexStart = nResetTimes;
	}
	if (nIndexEnd < 0)
	{
		nIndexEnd = nResetTimes;
	}

	// ������� + �������
	if (beginTime > finishTime)
	{
		nCount += (nResetTimes - nIndexStart) + nIndexEnd;		// (ÿ���ܴ��� - ����¼֮ǰ����) + �����¼֮ǰ����
	}
	else
	{
		nCount += nIndexEnd - nIndexStart;
	}

	return nCount;
}

bool ResetTimerModule::CanWeekReset(
	const ResetData *data,
	const int cur_hour,
	const double cur_date,
	const int cur_weekday,
	const int last_hour,
	const double last_date,
	const int last_weekday)
{
	if (NULL == data)
	{
		return false;
	}

	// �������
	double diffday = util_get_time_diff(cur_date, last_date);
	if (diffday < 0)
	{
		return false;
	}

	CVarList timer_list;
	util_split_string(timer_list, data->reset_timer, ",");

	// ˢ��weekday��ͬ
	if (data->reset_weekday == cur_weekday)
	{
		// ����
		if (diffday > DBL_EPSILON)
		{					
			LoopBeginCheck(a);
			for (int i = 0; i < (int)timer_list.GetCount(); ++i)
			{
				LoopDoCheck(a);
				const int clock = atoi(timer_list.StringVal(i));

				// ���쵱ǰʱ����ǰ��ˢ��
				if (cur_hour >= clock)
				{
					return true;
				}
			}
		}
		else
		{
			// δ����
			if (last_hour >= cur_hour)
			{
				return false;
			}
			else
			{
				// ����ϴ�ˢ�º��´�ˢ��Ӧ�ڵ�ʱ��
				int next_hour = -1;

				// ����Ѿ��������һ��ˢ���򷵻�
				if (last_hour >= atoi(timer_list.StringVal(timer_list.GetCount() - 1)))
				{
					return false;
				}
				else
				{
					LoopBeginCheck(b);
					for (int i = 0; i < (int)timer_list.GetCount(); ++i)
					{
						LoopDoCheck(b);
						const int clock = atoi(timer_list.StringVal(i));
						if (last_hour < clock)
						{
							next_hour = clock;
							break;
						}
					}

					return (cur_hour >= next_hour) ? true : false;
				}
			}
		}
	}
	else
	{
		// ����7��+
		if (diffday > 6 + DBL_EPSILON)
		{
			return true;
		}
		else if (diffday > DBL_EPSILON)
		{
			int d = last_weekday + 1;
			d = d == 7 ? 0 : d;
			while (d < 7)
			{
				if (d == cur_weekday)
				{
					return false;
				}
				if (d == data->reset_weekday)
				{
					return true;
				}

				++d;
				d = d == 7 ? 0 : d;
			}
		}
	}

	return false;	
}

// �ɷ���ˢ��
bool ResetTimerModule::CanDayReset(
	const ResetData *data,
	const int cur_hour,
	const double cur_date,
	const int last_hour,
	const double last_date)
{	
	if (NULL == data)
	{
		return false;
	}

	// �������
	double diffday = util_get_time_diff(cur_date, last_date);
	if (diffday < 0)
	{
		return false;
	}

	CVarList timer_list;
	util_split_string(timer_list, data->reset_timer, ",");

	if (diffday > 1 + DBL_EPSILON)
	{
		// ��������+
		return true;
	}
	else if (diffday > DBL_EPSILON)
	{
		// ����һ��
		if (last_hour >= cur_hour)
		{
			// һ��-
			LoopBeginCheck(a);
			for (int i = 0; i < (int)timer_list.GetCount(); ++i)
			{
				LoopDoCheck(a);
				const int clock = atoi(timer_list.StringVal(i));

				// ����ʱ���¼�Ժ��ˢ��
				if (last_hour < clock)
				{
					return true;
				}

				// ���쵱ǰʱ����ǰ��ˢ��
				if (cur_hour >= clock)
				{
					return true;
				}
			}
		}
		else
		{
			// һ��+
			return true;
		}
	}
	else
	{
		// û����
		if (last_hour >= cur_hour)
		{
			return false;
		}
		else
		{
			// ����ϴ�ˢ�º��´�ˢ��Ӧ�ڵ�ʱ��
			int next_hour = -1;

			// ����Ѿ��������һ��ˢ���򷵻�
			if (last_hour >= atoi(timer_list.StringVal(timer_list.GetCount() - 1)))
			{
				return false;
			}
			else
			{
				LoopBeginCheck(b);
				for (int i = 0; i < (int)timer_list.GetCount(); ++i)
				{
					LoopDoCheck(b);
					const int clock = atoi(timer_list.StringVal(i));
					if (last_hour < clock)
					{
						next_hour = clock;
						break;
					}
				}

				return (cur_hour >= next_hour) ? true : false;
			}
		}
	}

	return false;
}

void ResetTimerModule::ReloadConfig(IKernel* pKernel)
{
	m_resetTimerModule->LoadResource(pKernel);
}