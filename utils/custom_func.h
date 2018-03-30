#ifndef _GAME_CUSTOM_FUNC_H
#define _GAME_CUSTOM_FUNC_H

#include "../public/IVarList.h"
#include "../server/IPubKernel.h"
#include "FsGame/Define/SystemInfo_Campaign.h"
#include "FsGame/Define/SystemInfo_Common.h"
#include "FsGame/Define/SystemInfo_Equipment.h"
#include "FsGame/Define/SystemInfo_Item.h"
#include "FsGame/Define/SystemInfo_Npc.h"
#include "FsGame/Define/SystemInfo_OPActivity.h"
#include "FsGame/Define/SystemInfo_Scene.h"
#include "FsGame/Define/SystemInfo_Skill.h"
#include "FsGame/Define/SystemInfo_Social.h"
#include "FsGame/Define/SystemInfo_SysFunction.h"
#include "FsGame/Define/SystemInfo_Task.h"

#ifdef FSROOMLOGIC_EXPORTS
#include "server/IRoomKernel.h"
#else
#include "server/IKernel.h"
#endif


// ���͸�self���
bool CustomSysInfo(IKernel * pKernel, const PERSISTID & self, const int info_id, const IVarList & para_list);

// ���͸�����Ϊname����ң��������ã�
bool CustomSysInfoByName(IKernel * pKernel, const wchar_t * name, const int info_id, const IVarList & para_list);

// ���͸�����Ϊname����ң��������ã�
bool CustomSysInfoByName(IPubKernel * pKernel, const wchar_t * name, const int info_id, const IVarList & para_list);

// ���͸�obj�����Ұ��Χ�ڵ����
bool CustomSysInfoByKen(IKernel * pKernel, const PERSISTID & obj, const int info_id, const IVarList & para_list);

// ���͸�scene�����ڵ���ң����sceneid<=0 ���͵�ȫ���磩
bool CustomSysInfoByScene(IKernel * pKernel, int scene, const int info_id, const IVarList & para_list);

// ���͸�group�����ڵ����
bool CustomSysInfoByGroup(IKernel * pKernel, int group, const int info_id, const IVarList & para_list);

// ���͸�channelƵ�������
bool CustomSysInfoByChannel(IKernel * pKernel, int channel, const int info_id, const IVarList & para_list);

// ���͸�������ң��������ã�
bool CustomSysInfoBroadcast(IKernel * pKernel, const int info_id, const IVarList & para_list);

// ���͸�������ң��������ã�
bool CustomSysInfoBroadcast(IPubKernel * pPubKernel, const int info_id, const IVarList & para_list);


//֪ͨĳ������Χ��Ҳ���һ����Ч
bool CustomEffectByKen(IKernel* pKernel, 
	const char* effect, 
	const PERSISTID& self, 
	const PERSISTID& target, 
	float x = 0.0f, 
	float y = 0.0f, 
	float z = 0.0f);


// ����
bool CustomSpeech(IKernel * pKernel, const PERSISTID & self, const char * fast_string_id, const IVarList & para_list);



// Ч��
bool CustomDisplayStage(IKernel* pKernel,
						const PERSISTID& self,
						const PERSISTID& target,
						const int nCustomType, // CustomDisplayType
						//const int eEffectType, // ESkillStageEffectType
						const IVarList& params,
						const bool IsSendAll); // �Ƿ�Χ����

bool CustomSysInfoGMBroadcast(IKernel* pKernel, const int info_id, const IVarList& para_list);

// ������Ϣ(��������Χ����)
bool SafeCustom(IKernel* pKernel, const PERSISTID& self, const IVarList& msg);

// ��Χ������Ϣ
bool SafeCustomByKen(IKernel* pKernel, const PERSISTID& self, const IVarList& msg);
#endif // _GAME_CUSTOM_FUNC_H

