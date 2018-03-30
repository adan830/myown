//--------------------------------------------------------------------
// �ļ���:		ICrossKernel.h
// ��  ��:		�������������Ľӿ�
// ˵  ��:		
// ��������:	2012��10��31��	
// ������:		����
// ��Ȩ����:	������ţ�������޹�˾
//--------------------------------------------------------------------

#ifndef _SERVER_ICROSSKERNEL_H
#define _SERVER_ICROSSKERNEL_H

#include "../public/Macros.h"
#include "../public/IVarList.h"
#include "ICrossData.h"

// ���������߼��ӿڰ汾��
#define CROSS_LOGIC_MODULE_VERSION 0x1000

class ICrossKernel;

// ��ʱ������
typedef int (__cdecl* CROSS_TIMER_FUNC)(ICrossKernel* pCrossKernel, 
										 const wchar_t* data_name, int time) ;
	
// ����������Ľӿ�

class ICrossKernel
{
public:
	virtual ~ICrossKernel() = 0;
	
	/// \brief ��ӿ���������
	/// \param name ������������
	virtual bool AddCrossData(const wchar_t* name, bool save) = 0;
	/// \brief ɾ������������
	/// \param name ������������
	virtual bool RemoveCrossData(const wchar_t* name) = 0;
	/// \brief ���ҿ�������
	/// \param ������������
	virtual bool FindCrossData(const wchar_t* name) = 0;
	/// \brief ��ÿ�����������
	virtual int GetCrossDataCount() = 0;
	/// \brief ��ÿ������������б�
	/// \param result �������������б�
	virtual int GetCrossDataList(IVarList& result) = 0;
	/// \brief ���ָ�����ƵĿ���������
	/// \param ����������
	virtual ICrossData* GetCrossData(const wchar_t* name) = 0;
	/// \brief �������������
	/// \param data_name ����������
	virtual bool SaveCrossData(const wchar_t* name) = 0;

	/// \brief ����Ϣ����Ϸ������
	/// \param server_id ���������
	/// \param scene_id ������ţ�Ϊ0��ʾ��һ������
	/// \param msg ��Ϣ����
	virtual bool SendCrossMessage(int server_id, int scene_id, 
		const IVarList& msg) = 0;

	/// \brief ���Ҷ�ʱ��
	/// \param name ��ʱ������
	/// \param data_name ��������������
	virtual bool FindTimer(const char* name, const wchar_t* data_name) = 0;
	/// \brief ��Ӷ�ʱ��
	/// \param name ��ʱ������
	/// \param func ��ʱ������ָ��
	/// \param data_name ��������������
	/// \param time ��ʱʱ��
	/// \param count ��ʱ����
	virtual bool AddTimer(const char* name, CROSS_TIMER_FUNC func, 
		const wchar_t* data_name, int time, int count) = 0;
	/// \brief ɾ����ʱ��
	/// \param name ��ʱ������
	/// \param data_name ��������������
	virtual bool RemoveTimer(const char* name, const wchar_t* data_name) = 0;

	/// \brief ������Ϸ��־
	/// \param name ����
	/// \param type ����
	/// \param content ����
	/// \param comment ��ע
	virtual bool SaveLog(const wchar_t* name, int type, 
		const wchar_t* content, const wchar_t* comment) = 0;
	
	/// \brief �����ϵͳLOG�ļ�
	/// \param info ��Ϣ����
	virtual void Trace(const char* info) = 0;
	/// \brief ���������̨��Ļ
	/// \param info ��Ϣ����
	virtual void Echo(const char* info) = 0;

};

inline ICrossKernel::~ICrossKernel() {}

#endif // _SERVER_ICROSSKERNEL_H

