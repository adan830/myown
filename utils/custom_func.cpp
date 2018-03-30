#include "custom_func.h"
#include "../FsGame/Define/ServerCustomDefine.h"
//#include "../public/VarList.h"
#include "FsGame/Define/SnsDefine.h"
//#include "string_util.h"
//#include "../FsGame/SocialSystemModule/GuildModule.h"
//#include "../FsGame/Define/CommandDefine.h"


// ���͸�self���
bool CustomSysInfo(IKernel * pKernel, const PERSISTID & self, 
	const int info_id, const IVarList & para_list)
{
	if (!pKernel->Exists(self) 
		|| pKernel->Type(self) != TYPE_PLAYER)
	{
		return false;
	}

	CVarList system_info;
	system_info << SERVER_CUSTOMMSG_SYSINFO
				<< info_id
				<< para_list;
	return pKernel->Custom(self, system_info);
}

// ���͸�����Ϊname����ң��������ã�
bool CustomSysInfoByName(IKernel * pKernel, const wchar_t * name, 
	const int info_id, const IVarList & para_list)
{
	CVarList system_info;
	system_info << SERVER_CUSTOMMSG_SYSINFO
				<< info_id
				<< para_list;
	return pKernel->CustomByName(name, system_info);
}

// ���͸�����Ϊname����ң��������ã�
bool CustomSysInfoByName(IPubKernel * pKernel, const wchar_t * name, 
	const int info_id, const IVarList & para_list)
{
	CVarList system_info;
	system_info << SERVER_CUSTOMMSG_SYSINFO
				<< info_id
				<< para_list;
	return pKernel->CustomByName(name, system_info);
}

// ���͸�obj�����Ұ��Χ�ڵ����
bool CustomSysInfoByKen(IKernel * pKernel, const PERSISTID & obj, 
	const int info_id, const IVarList & para_list)
{
	if (!pKernel->Exists(obj) 
		|| pKernel->Type(obj) != TYPE_PLAYER)
	{
		return false;
	}

	CVarList system_info;
	system_info << SERVER_CUSTOMMSG_SYSINFO
				<< info_id
				<< para_list;
	return pKernel->CustomByKen(obj, system_info);
}

// ���͸�scene�����ڵ���ң����sceneid<=0 ���͵�ȫ���磩
bool CustomSysInfoByScene(IKernel * pKernel, int scene, 
	const int info_id, const IVarList & para_list)
{
#ifdef FSROOMLOGIC_EXPORTS

	return true;

#else

	CVarList system_info;
	system_info << SERVER_CUSTOMMSG_SYSINFO
				<< info_id
				<< para_list;

	if (scene <= 0)
	{
		return pKernel->CustomByWorld(system_info);
	}
	else
	{
		return pKernel->CustomByScene(scene, system_info);
	}

#endif
}

// ���͸�group�����ڵ����
bool CustomSysInfoByGroup(IKernel * pKernel, int group, const int info_id, const IVarList & para_list)
{
	CVarList system_info;
	system_info << SERVER_CUSTOMMSG_SYSINFO
				<< info_id
				<< para_list;
	return pKernel->CustomByGroup(group, system_info);
}

// ���͸�channelƵ�������
bool CustomSysInfoByChannel(IKernel * pKernel, int channel, 
	const int info_id, const IVarList & para_list)
{
	if (channel < 0)
	{
		return false;
	}

#ifdef FSROOMLOGIC_EXPORTS

	return true;

#else

	CVarList system_info;
	system_info << SERVER_CUSTOMMSG_SYSINFO
				<< info_id
				<< para_list;
	return pKernel->CustomByChannel(channel, system_info);

#endif
}

// ���͸�������ң��������ã�
bool CustomSysInfoBroadcast(IKernel * pKernel, const int info_id, const IVarList & para_list)
{
#ifdef FSROOMLOGIC_EXPORTS

	return true;

#else

	CVarList system_info;
	system_info << SERVER_CUSTOMMSG_SYSINFO
				<< info_id
				<< para_list;
	return pKernel->CustomByWorld(system_info);

#endif

}

// ���͸�������ң��������ã�
bool CustomSysInfoBroadcast(IPubKernel * pPubKernel, const int info_id, const IVarList & para_list)
{
	CVarList system_info;
	system_info << SERVER_CUSTOMMSG_SYSINFO
				<< info_id
				<< para_list;
	return pPubKernel->CustomByWorld(system_info);
}

bool CustomSysInfoGMBroadcast(IKernel* pKernel, int tips_type, const wchar_t* info, const IVarList& para_list)
{
#ifdef FSROOMLOGIC_EXPORTS
	return true;
#else
	return pKernel->CustomByWorld(CVarList() << SERVER_CUSTOMMSG_SYSINFO << tips_type << info << para_list);
#endif
}

bool CustomSpeech(IKernel * pKernel, const PERSISTID & self, const char * fast_string_id, const IVarList & para_list)
{
	if ( !pKernel->Exists(self) || pKernel->Type(self) != TYPE_PLAYER)
	{
		return false;
	}
	return pKernel->Custom(self, CVarList() << SERVER_CUSTOMMSG_SPEECH << fast_string_id << para_list);
}

//֪ͨĳ������Χ��Ҳ���һ����Ч
bool CustomEffectByKen(IKernel* pKernel, const char* effect, const PERSISTID& self, const PERSISTID& target, float x, float y, float z)
{
	if (!pKernel->Exists(self))
	{
		return false;
	}
	if (!effect || !*effect)
	{
		return false;
	}
	return pKernel->CustomByKen(self, CVarList() << SERVER_CUSTOMMSG_EFFECT << self /*<< ONCEEFFECT*/ << effect << target << x << y << z);
}

// Ч��
bool CustomDisplayStage(IKernel* pKernel,
						const PERSISTID& self,
						const PERSISTID& target,
						const int nCustomType, // CustomDisplayType
//						const int eEffectType, // ESkillStageEffectType
						const IVarList& params,
						const bool IsSendAll) // �Ƿ�Χ����	
{
	if (!pKernel->Exists(self))
	{
		return false;
	}

	CVarList msg;
	msg << SERVER_CUSTOMMSG_DISPLAY << nCustomType<< target;

	msg << params;

	if (IsSendAll)
	{
		pKernel->CustomByKen(self, msg);
	}
	else
	{
		if (pKernel->Type(self) == TYPE_PLAYER)
		{
			pKernel->Custom(self, msg);
		}
	}
	return true;
}

// ������Ϣ(��������Χ����)
bool SafeCustom(IKernel* pKernel, const PERSISTID& self, const IVarList& msg)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return false;
	}

	if (0 == msg.GetCount())
	{
		return false;
	}

	if (pSelfObj->GetClassType() == TYPE_PLAYER && pSelfObj->QueryInt("Online") == ONLINE)
	{
		pKernel->Custom(self, msg);
	}

	return true;
}

// ��Χ������Ϣ
bool SafeCustomByKen(IKernel* pKernel, const PERSISTID& self, const IVarList& msg)
{
	if (!pKernel->Exists(self) || 0 == msg.GetCount())
	{
		return false;
	}
	pKernel->CustomByKen(self, msg);

	return true;
}