//--------------------------------------------------------------------
// �ļ���:		LuaScriptModule.cpp
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

#include "LuaScriptModule.h"
#include "utils/extend_func.h"
#include <time.h>
#include "public/AutoMem.h"
#include "utils/util_func.h"

#if defined _DEBUG
	struct degug_invoke_lua_t
	{
		int count;
		__int64 use_time;

		degug_invoke_lua_t() : count(0), use_time(0) {}
	};

	std::map<std::string, degug_invoke_lua_t> g_script_times;
#endif

// ע��LUA����
bool RegLuaFunc(const char* key, LUASCRIPT_FUNC func)
{
	if (LuaScriptModule::m_pLuaScriptModule)
	{
		return LuaScriptModule::m_pLuaScriptModule->RegLuaFunc(key, func);
	}

	return false;
}

// ����LUA�ű�, ��Ϸ�߼���Ӧ�õ�������ӿ�
bool RunLua(IKernel* pKernel, const char* src, const char* func, const IVarList& in, int rtNums, IVarList* pOut)
{
	if (LuaScriptModule::m_pLuaScriptModule)
	{
		return LuaScriptModule::m_pLuaScriptModule->RunLuaScript(pKernel, src, func, in, rtNums, pOut);
	}

	return false;
}

LuaScriptModule* LuaScriptModule::m_pLuaScriptModule = NULL;

// ��ʼ��
bool LuaScriptModule::Init(IKernel* pKernel)
{
	if (NULL == pKernel)
	{
		Assert(false);

		return false;
	}

	// ָ��
	m_pLuaScriptModule = this;

	// ��ʼ���ű�����
	INIT_SCRIPT_INVOKE(pKernel);

	return true;
}

// �ر�
bool LuaScriptModule::Shut(IKernel* pKernel)
{
	return true;
}

// ִ�нű�
bool LuaScriptModule::RunLuaScript(IKernel* pKernel, const char* src, const char* func, const IVarList& in, 
								   int rtNums, IVarList* pOut)
{
	static char buf[512];

	SPRINTF_S(buf, "%s_%s", src, func);

	char* key = strlwr(buf);

	LuaFuncMap::iterator it = m_functions.find(key);

	if (it != m_functions.end())
	{
		LUASCRIPT_FUNC f = it->second;
		f(pKernel, in, rtNums, pOut);

		return true;
	}

	if (!pKernel->LuaFindScript(src))
	{
		if (!pKernel->LuaLoadScript(src))
		{
			return false;
		}
	}

	if (!pKernel->LuaFindFunc(src, func))
	{
		return false;
	}

#if defined _DEBUG
	__int64 st = util_get_time_64();
#endif

	bool bret = pKernel->LuaRunFunc(src, func, in, rtNums, pOut);

#if defined _DEBUG
	st = util_get_time_64() - st;
	degug_invoke_lua_t& arg = g_script_times[buf];
	arg.count += 1;
	arg.use_time += st;
#endif

	return bret;
}

// ע��ű��ӿ�
bool LuaScriptModule::RegLuaFunc(const char* key, LUASCRIPT_FUNC func, bool bCover)
{
	// ������ע���ָ��, keyҲ�����ǿջ��ָ��
	if (func == NULL || key == NULL || key[0] == NULL)
	{
		return false;
	}

	// keyת��ΪСд
	size_t len = strlen(key) + 1;
	TAutoMem<char,128> auto_str(len);
	char* buf = auto_str.GetBuffer();
	strcpy_s(buf, len, key);
	buf[len-1] = '\0';
	char* reg_key = strlwr(buf);


	// ����Ѿ�ע�������key, ��ô����bCover(�Ƿ񸲸�ԭ���ĺ���)
	LuaFuncMap::iterator it = m_functions.find(reg_key);

	if (it != m_functions.end())
	{
		if (!bCover)
		{
			return false;
		}

		it->second = func;
		return true;
	}

	// ����һ��ע��
	m_functions[reg_key] = func;
	
	return true;
}

// �����������Խű�����(��debug��Ч)
#if defined _DEBUG

void extend_get_file_name(IKernel* pKernel, char* filename, size_t size)
{
	time_t now = time(NULL);
	tm* t = localtime(&now);
	
#ifndef FSROOMLOGIC_EXPORTS
	int memberId = pKernel->GetMemberId();
#else
	int memberId = 0;
#endif // FSROOMLOGIC_EXPORTS

	SafeSprintf(filename, size, "script_invoke_%d__%d_%d_%d.log", memberId, 
		t->tm_year+1900, t->tm_mon+1, t->tm_mday);
	
}



void extend_output_script_times(IKernel* pKernel, const PERSISTID& player, bool bFile/* = false*/)
{
	if (bFile)
	{
		char filename[256];
		extend_get_file_name(pKernel, filename, sizeof(filename));

		FILE* file = fopen(filename, "w+");
		if (file == NULL)
		{
			return;
		}

		char buf[512];

		LoopBeginCheck(a);
		for (std::map<std::string, degug_invoke_lua_t>::iterator it = g_script_times.begin(); it != g_script_times.end(); ++it)
		{
			LoopDoCheck(a);

			degug_invoke_lua_t& arg = it->second;
			SPRINTF_S(buf, "%s\t%d\t%I64d\n", it->first.c_str(), 
				arg.count, arg.use_time);

			fwrite(buf, strlen(buf), 1, file);
		}

		fclose(file);
	}
	else
	{
		LoopBeginCheck(b);
		for (std::map<std::string, degug_invoke_lua_t>::iterator it = g_script_times.begin(); it != g_script_times.end(); ++it)
		{
			LoopDoCheck(b);

			degug_invoke_lua_t& arg = it->second;
			CVarList var;
			var << "sysinfo" << 1 << "{@0:file}={@1:num}   usetime={@2:num}" << it->first 
				<< arg.count << arg.use_time;
			pKernel->Custom(player, var);
		}
	}
}

void on_member_start(IKernel* pKernel)
{
	char filename[256];
	extend_get_file_name(pKernel, filename, sizeof(filename));

	FILE* file = fopen(filename, "rb");
	if (file == NULL)
	{
		return;
	}

	// ȡ�ļ�����
	::fseek(file, 0, SEEK_END);
	size_t length = ::ftell(file);
	::fseek(file, 0, SEEK_SET);

	char* buf = NEW char[length+1];
	if (length > 1 && buf != NULL)
	{
		// ȫ�����ص��ڴ�
		fread((void*)buf, 1, length, file);

		buf[length] = 0;

		const char* src = buf;
		const char* num = NULL;
		const char* t	= NULL;
		int c;
		LoopBeginCheck(c);
		for (size_t i = 0; i < length-1; ++i)
		{
			LoopDoCheck(c);

			c = buf[i];
			if ( c == 0)
			{
				continue;
			}

			if (c == '\t')
			{
				if (num == NULL)
				{
					num = &buf[i+1];
				}
				else
				{
					t = &buf[i+1];
				}
				
				buf[i] = NULL;
				continue;
			}
			if (c == 0x0d && buf[i+1] == 0x0a/* c == '\n'*/)
			{
				buf[i] = 0;
				buf[++i] = 0;

				if (src != NULL && num != NULL && t != NULL)
				{
					degug_invoke_lua_t& arg = g_script_times[src];
					arg.count += atoi(num);
					arg.use_time += _atoi64(t);
				}

				src = NULL;
				num = NULL;
				t   = NULL;

				continue;
			}

			if (src == NULL)
			{
				src = &buf[i];
			}
		}

		delete[] buf;
	}

	fclose(file);
}

#endif