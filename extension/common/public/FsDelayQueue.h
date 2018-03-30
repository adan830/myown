//--------------------------------------------------------------------
// �ļ���:		FsDelayQueue.h
// ��  ��:		�ӳ��̰߳�ȫ����
// ˵  ��:		
// ��������:		2013��5��10��
// �޸�����:		2016��9��14��
// ������:		���
//--------------------------------------------------------------------
#ifndef __FS_DELAY_THREAD_QUEUE_H__
#define __FS_DELAY_THREAD_QUEUE_H__
#include "utils/FsLockedQueue.h"

class FsDelayQueue
{
private:
	typedef std::function<void()> Action;
	struct DelayInfo
	{
		time_t RunTime;
		Action action;
	};

	FsLockedQueue<DelayInfo>	m_queueActions;
	int m_nDelay;
	DelayInfo m_nLastAction;
public:
	FsDelayQueue()
		: m_nDelay( 3 )
	{
		m_nLastAction.RunTime = 0;
		//m_nLastAction.action = NULL;
	}

	void SetDelay(int nDelayS)
	{
		m_nDelay = nDelayS;
	}

	int GetDelay() const
	{
		return m_nDelay;
	}

	void RunInQueueThread(const Action& action)
	{
		DelayInfo info = { time(NULL) + m_nDelay, action };
		m_queueActions.EnQueue(info);
	}

	void RunQueue()
	{
		time_t tNow = time(NULL);

		if (m_nLastAction.RunTime != 0 )
		{
			if (m_nLastAction.RunTime <= tNow)
			{
				m_nLastAction.action();
				m_nLastAction.RunTime = 0;
			}
			else
			{
				// ʱ��δ����ִ��
				return;
			}
		}

		while (m_queueActions.DeQueue(m_nLastAction))
		{
			if (m_nLastAction.RunTime <= tNow)
			{
				m_nLastAction.action();
				m_nLastAction.RunTime = 0;
			}
			else
			{
				// ʱ��δ������
				break;
			}
		}
	}
};

#endif // END __FS_THREAD_QUEUE_H__