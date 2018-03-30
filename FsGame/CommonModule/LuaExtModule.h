//--------------------------------------------------------------------
// �ļ���:		LuaExtModule.h
// ��  ��:		����LUA�ű�����չ����
// ˵  ��:		
// ��������:	2014��06��10��
// ��������:	2014��06��10��
// ������:		  ( )
// �޸���:     
//    :	    
//--------------------------------------------------------------------

#ifndef __LuaExtModule_H__
#define __LuaExtModule_H__

#include "FsGame/Interface/ILuaExtModule.h"
#include <map>

class LuaExtModule : public ILuaExtModule
{
public:
	virtual bool Init(IKernel* pKernel);
	virtual bool Shut(IKernel* pKernel);

public:
	//��Ϣ�ص��޸����ѭ����ʱ
	static int CommandSeMaxCirculateCount(IKernel* pKernel, const PERSISTID& self,
		const PERSISTID& sender, const IVarList& args);
	//�޸����ѭ������
	static void SeMaxCirculateCount(IKernel* pKernel, const PERSISTID& self, int count);

#ifndef FSROOMLOGIC_EXPORTS
	// �ַ�GM����
	static int OnCommandDispatchGM(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args);
#endif // FSROOMLOGIC_EXPORTS

	// ע��GM�ַ�����
	static int RegisterCallBackFunction(const char* strFunc, GM_CALL_BACK_FUNC func);

private:
	// GM������õĺ���
	typedef std::map<std::string, GM_CALL_BACK_FUNC> MapGMFunc;
	static MapGMFunc m_mapGMFunc;
};

#endif // __LuaExtModule_H__