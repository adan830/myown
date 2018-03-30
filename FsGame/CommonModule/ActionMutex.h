//------------------------------------------------------------------
// �� �� ��:    ActionMutex.h
// ��    ��:    �����Ϊ����ģ��
// ˵    ��:
// �� �� ��:      
// ��������:
//    :        
//-------------------------------------------------------------------
#ifndef _ACTION_MUTEX_H__
#define _ACTION_MUTEX_H__

#include "Fsgame/Define/header.h"
#include "FsGame/Define/ActionMutexDefine.h"

class ActionMutexModule : public ILogicModule
{
public:
    // ��ʼ��
    virtual bool Init(IKernel* pKernel);
    // �ر�
    virtual bool Shut(IKernel* pKernel);

	/*!
	* @brief	�Ƿ�������Ϊ
	* @param int ��Ϊ����
	* @return  true������Ϊ��false����Ϊ
	*/
	static bool IsHaveAction(IKernel * pKernel, const PERSISTID & self, bool show_sysinfo = true);

	/*!
	* @brief	��¼��Ϊ
	* @param int ��Ϊ����
	*/
	static void NoteAction(IKernel * pKernel, const PERSISTID & self,
		const int action);

	/*!
	* @brief	�����Ϊ
	*/
	static void CleanAction(IKernel * pKernel, const PERSISTID & self);

	/*!
	* @brief	��ǰ�Ƿ�Ϊָ���
	* @param int ��Ϊ����
	*/
	static bool IsThisAction(IKernel * pKernel, const PERSISTID & self,
		const int action);

	/*!
	* @brief	��Ϊ�Ƿ�Ϸ�
	* @param int ��Ϊ����
	* @return  true�Ϸ���false���Ϸ�
	*/
	static bool IsValidAction(const int action);

	// �����Ϊ��Ϣ
	static int OnCommandCleanAction(IKernel * pKernel, const PERSISTID & self, 
		const PERSISTID & sender, const IVarList & var);
};

#endif // _ACTION_MUTEX_H__