//--------------------------------------------------------------------
// �ļ���:		ILogicModule_t.h
//��  ��:       ��Ϸ�߼�ģ��
//˵  ��:       �̳��ڣ�ILogicModule_t
//
//��������:     2017��07��21��
//������:       kevin 
// ��Ȩ����:	WhalesGame Technology co.Ltd      
//--------------------------------------------------------------------

#ifndef _I_SERVER_LOGIC_GAME_MODULE_H
#define _I_SERVER_LOGIC_GAME_MODULE_H

#include "utils/Singleton.h"

// Game�߼�ģ��ӿ�

class ILogicModule_t : public ILogicModule
{
public:
	virtual ~ILogicModule_t() = 0;
public:
	// command
	virtual void OnModuleCommand(IKernel* pKernel, int cmd_id, const IVarList& args) = 0;
};

inline ILogicModule_t::~ILogicModule_t() {}

//����module���ҡ�
template<class T>
inline T* __WgFindLogicModuleByName(IKernel * pKernel,const char* module_name)
{
	T* p_dc = static_cast<T*>(pKernel->GetLogicModule(module_name));
	if (p_dc == NULL)
	{
		Assert(0);
	}
	return  p_dc;
}
inline ILogicModule_t* WgFindModuleByName(IKernel * pKernel, const char* module_name)
{
	ILogicModule_t* p_dc = static_cast<ILogicModule_t*>(pKernel->GetLogicModule(module_name));
	if (p_dc == NULL)
	{
		Assert(0);
	}
	return  p_dc;
}
#define  WgFindLogicModuleByName(pKernel,module_name)  __WgFindLogicModuleByName<module_name>(pKernel,#module_name)


#endif // _SERVER_ILOGICMODULE_H

