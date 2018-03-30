//--------------------------------------------------------------------
// �ļ���:		LuaScriptModule.h
// ��  ��:		����LUA�ű�����չ����
// ˵  ��:		����lua�ű�ת�Ƶ�c++����
//				RunLuaScript�����������߼����ֵ���lua�ű�, ����ű��Ѿ���ֲ��c++��, ��ֱ�ӵ���ע��ĺ���, �����Ե���lua�ű�
//				OUTPUT_SCRIPT_INVOKE_INFO_TOFILE�궨��(dubug��Ч)ּ�ڵ�����Ϸ����������ÿ���ű�����Ƶ��, ���Բο��Ƿ񽫽ű�ת�Ƶ�c��
//				�����ÿ�콫�ű����ô��������һ����ͬ��log�ļ���(����member)
//				���Ҫ��lua�ļ��Ƶ�c��, ������Ҫ��ԭlua�ļ�·���Լ����ú�������(script.."_"..func)��Ϊkeyע�ᵽc�ӿ��м���
// ��������:	2014��06��10��
// ��������:	2014��06��10��
// ������:		  ( )
// �޸���:     
//    :	    
//--------------------------------------------------------------------

#ifndef __LuaScriptModule_H__
#define __LuaScriptModule_H__

#include "Fsgame/Define/header.h"
#include <map>
#include <unordered_map>

#if defined _DEBUG
void extend_output_script_times(IKernel* pKernel, const PERSISTID& player, bool bFile = false);

void on_member_start(IKernel* pKernel);

// ����ű����ô������ļ���
#define OUTPUT_SCRIPT_INVOKE_INFO_TOFILE(pKernel, player) extend_output_script_times(pKernel, player, true)

// ����ű����ô�����gm���
#define PRINT_SCRIPT_INVOKE_INFO(pKernel, player) extend_output_script_times(pKernel, player, false)
#define INIT_SCRIPT_INVOKE(pKernel) on_member_start(pKernel)

#else

#define OUTPUT_SCRIPT_INVOKE_INFO_TOFILE(pKernel, player)
#define PRINT_SCRIPT_INVOKE_INFO(pKernel, player)
#define INIT_SCRIPT_INVOKE(pKernel)

#endif

typedef int (*LUASCRIPT_FUNC)(IKernel* pKernel, const IVarList& in, int rtNums, IVarList* out);

// ע��LUA����
bool RegLuaFunc(const char* key, LUASCRIPT_FUNC func);

// ����lua�ӿ�
bool RunLua(IKernel* pKernel, const char* src, const char* func, const IVarList& in,
			int rtNums = 0, IVarList* pOut = NULL);


class LuaScriptModule : public ILogicModule
{
public:
	// ��ʼ��
	virtual bool Init(IKernel* pKernel);

	// �ͷ�
	virtual bool Shut(IKernel* pKernel);

public:
	// ִ��LUA�ű�
	bool RunLuaScript(IKernel* pKernel, const char* src, const char* func, const IVarList& in,
		int rtNums, IVarList* pOut);

	// ע��LUA�ű�
	bool RegLuaFunc(const char* key, LUASCRIPT_FUNC func, bool bCover = false);

public:
	static LuaScriptModule* m_pLuaScriptModule;

private:
	typedef std::unordered_map<std::string, LUASCRIPT_FUNC> LuaFuncMap;

	LuaFuncMap m_functions;
};

#define REG_LUA_FUNC(key, func) RegLuaFunc(key, func);

#endif // __LuaScriptModule_H__