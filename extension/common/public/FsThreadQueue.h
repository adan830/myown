//--------------------------------------------------------------------
// �ļ���:		FsThreadQueue.h
// ��  ��:		�̰߳�ȫ����
// ˵  ��:		
// ��������:		2013��5��10��
// �޸�����:		2016��9��14��
// ������:		���
//--------------------------------------------------------------------
#ifndef __FS_THREAD_QUEUE_H__
#define __FS_THREAD_QUEUE_H__
#include "utils/FsLockedQueue.h"

class FsThreadQueue
{
private:
	typedef std::function<void()> Action;
	FsLockedQueue<Action>	m_queueActions;

public:
	void RunInQueueThread(const Action& action)
	{
		m_queueActions.EnQueue( action );
	}

	void RunQueue()
	{
		Action action;
		while ( m_queueActions.DeQueue(action) )
		{
			action();
		}
	}
};

#endif // END __FS_THREAD_QUEUE_H__