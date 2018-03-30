//--------------------------------------------------------------------
// �ļ���:		Domain_Task.h
// ��  ��:		������Ϣ��
// ˵  ��:		
// ��������:	2016��07��01��
// ������:		  tongzt
// �޸���:        
//--------------------------------------------------------------------

#ifndef __Domain_Task_h__
#define __Domain_Task_h__

#include "IDomainData.h"
//#include <vector>
//#include <string>

class Domain_Task : public IDomainData
{
public:
	// ����
	Domain_Task();

public:
	// ȡ����
	virtual const std::wstring & GetDomainName(IPubKernel *pPubKernel);

	// ����
	virtual bool GetSave(){return true;}

	// ����
	virtual int OnCreate(IPubKernel *pPubKernel, IPubSpace *pPubSpace); 

	// ����
	virtual int OnLoad(IPubKernel *pPubKernel, IPubSpace *pPubSpace);

	///  \brief ���յ���������������Ϣ
	///  \param source_id ����Ϣ�ķ�������ʶ
	///  \param msg ��Ϣ����
	virtual int OnMessage(IPubKernel *pPubKernel, IPubSpace *pPubSpace, 
		int source_id, int scene_id, const IVarList & msg);

private:
	// ����
	void CreateRecord(IPubKernel *pPubKernel, IPubData *pPubData, 
		const char *rec_name, IVarList& cols_type, const int cols, 
		const int max_row, IVarList& new_col_val, const int rec_key, bool is_save = true);

	// ����ʱ��
	static int ResetTaskTimer(IPubKernel *pPubKernel, const char *space_name, 
		const wchar_t *data_name, int time);

public:
	static Domain_Task * m_pDomain_Task;

};

#endif // __Domain_Task_h__
