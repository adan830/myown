//--------------------------------------------------------------------
// �ļ���:		SafeAreaNpc.cpp
// ��  ��:		��ȫ��NPC
// ˵  ��:		
// ��������:	2014��06��10��
// ��������:	2015��03��30��
// ������:		  ( )
// �޸���:        ( )
//    :	    
//--------------------------------------------------------------------

#include "SafeAreaNpcModule.h"
#include "FsGame/CommonModule/LuaScriptModule.h"
#include "FsGame/CommonModule/AsynCtrlModule.h"
//#include "FsGame/SocialSystemModule/ReviveManagerModule.h"
#include "FsGame/Define/GameDefine.h"
//#include "FsGame/Define/SafeAreaSysInfoDefine.h"
#include "FsGame/Define/GameDefine.h"
#include "utils/custom_func.h"
#include "utils/string_util.h"
#include "utils/exptree.h"
#include "utils/XmlFile.h"
#include "utils/util_func.h"
#include "../Define/FightDefine.h"
#include "../Interface/FightInterface.h"
#include "Define/CommandDefine.h"
#include "Define/SafeAreaSysInfoDefine.h"
#include "CommonModule/CommRuleModule.h"

#define SAFE_AREA_NPC_OLD           0   // �ϵİ�ȫ��
#define SAFE_AREA_NPC_ALL_NATION    1   // ������ȫ��
#define SAFE_AREA_NPC_OWN_NATION    2   // �����������
#define SAFE_AREA_NPC_ENEMY_NATION  3   // �����й����

#define SAFE_AREA_NPC_ENTRY  0   // ���밲ȫ��
#define SAFE_AREA_NPC_LEAVE  1   // �뿪��ȫ��

AsynCtrlModule * SafeAreaNpcModule::m_pAsynCtrlModule = NULL;
SafeAreaNpcModule * SafeAreaNpcModule::m_pSafeAreaNpc = NULL;

static const char* RECT_AREA_INDEX = "RectAreaIndex";

// ��ʼ��
bool SafeAreaNpcModule::Init(IKernel* pKernel)
{
	if (NULL == pKernel)
	{
		Assert(false);

		return false;
	}

	//��ʼ��ָ��
	m_pSafeAreaNpc = this;

	m_pAsynCtrlModule = static_cast<AsynCtrlModule*>(pKernel->GetLogicModule("AsynCtrlModule"));

	Assert( NULL != m_pAsynCtrlModule );

	pKernel->AddEventCallback("SafeAreaNpc","OnSpring", SafeAreaNpcModule::OnSpring);
	pKernel->AddEventCallback("SafeAreaNpc", "OnEndSpring", SafeAreaNpcModule::OnEndSpring);
	pKernel->AddEventCallback("SafeAreaNpc", "OnEntry", SafeAreaNpcModule::OnEntry);
	pKernel->AddEventCallback("SafeAreaNpc", "OnDestroy", SafeAreaNpcModule::OnDestroy);
	pKernel->AddEventCallback("player","OnLeaveScene",SafeAreaNpcModule::OnLeaveScene);

	pKernel->AddEventCallback("player", "OnEntry", SafeAreaNpcModule::OnPlayerEnterScene);
	pKernel->AddIntCommandHook("player", COMMAND_GROUP_CHANGE, SafeAreaNpcModule::OnCommandChangeGroup);

//	DECL_HEARTBEAT(SafeAreaNpcModule::HB_CheckRectangularSafeArea);
													   
// 	if (!LoadRectanularSafeAreaConfig(pKernel))
// 	{
// 		return false;
// 	}

	return true;
}

// �ͷ�
bool SafeAreaNpcModule::Shut(IKernel* pKernel)
{
	return true;
}

/*!
* @brief	�����ص�
* @param    ����ָ��
* @param    ��ȫ��NPC����
* @param    ��Ҷ���
* @param    �ص�Я������
* @return	bool
*/
int SafeAreaNpcModule::OnSpring(IKernel* pKernel, const PERSISTID& self, 
						  const PERSISTID& sender, const IVarList& args)
{
	//1����֤�����Ƿ����
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL || !pKernel->Exists(sender))
	{
		return 0;
	}

	//2����֤�Ƿ�Ϊ��Ҷ���
	if ( TYPE_PLAYER != pKernel->Type(sender) ) 
	{
		if (pKernel->Type(sender) != TYPE_NPC)
		{
			return 0;
		}

// 		const char *script = pKernel->GetScript(sender);
// 		if (strcmp(script, CLASS_NAME_ESCORT_NPC) != 0 
// 			&& strcmp(script, CLASS_NAME_BATTLE_NPC) != 0)
// 		{
// 			return 0;
// 		}
	}

	int nNation = 0/*pSelfObj->QueryInt(FIELD_PROP_NATION)*/;
	int nSafeRule = pSelfObj->QueryInt(FIELD_PROP_SAFE_RULE);

	//3�����½��밲ȫ������״̬
	m_pSafeAreaNpc->UpdateEntryState(pKernel, sender, nNation, nSafeRule);

	return 0;
}

/*!
* @brief	���������ص�
* @param    ����ָ��
* @param    ��ȫ��NPC����
* @param    ��Ҷ���
* @param    �ص�Я������
* @return	bool
*/
int SafeAreaNpcModule::OnEndSpring(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args)
{
	//1����֤�����Ƿ����
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL || !pKernel->Exists(sender) ) 
	{
		return 0;
	}

	//2����֤�Ƿ�Ϊ��Ҷ���
	if ( TYPE_PLAYER != pKernel->Type(sender) ) 
	{
		if (pKernel->Type(sender) != TYPE_NPC)
		{
			return 0;
		}

// 		if (strcmp(pKernel->GetScript(sender), CLASS_NAME_ESCORT_NPC) != 0)
// 		{
// 			return 0;
// 		}
	}

	int nNation = 0/*pSelfObj->QueryInt(FIELD_PROP_NATION)*/;
	int nSafeRule = pSelfObj->QueryInt(FIELD_PROP_SAFE_RULE);

	//3�������뿪��ȫ������״̬
	m_pSafeAreaNpc->UpdateLeaveState(pKernel, sender, nNation, nSafeRule);

	return 0;
}

/*!
* @brief	��ȫ��NPC����
* @param    ����ָ��
* @param    ��ȫ��NPC����
* @param    ��������
* @param    �ص�Я������
* @return	bool
*/
int SafeAreaNpcModule::OnDestroy(IKernel* pKernel, const PERSISTID& self,
						   const PERSISTID& sender, const IVarList& args)
{
	//1����֤��������Ƿ�Ϊ��
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return 0;
	}

	if (!pKernel->Exists(sender) ) 
	{
		return 0;
	}

	//2����֤�Ƿ�Ϊ:Npc�����볡������
	if ( TYPE_NPC != pKernel->Type(self)
		|| TYPE_SCENE != pKernel->Type(sender) ) 
	{
		return 0;
	}

	int nNation = 0/*pSelfObj->QueryInt(FIELD_PROP_NATION)*/;
	int nSafeRule = pSelfObj->QueryInt(FIELD_PROP_SAFE_RULE);

	//3����ȡNpc��Ч��Χ:Ĭ��100.0f
	float distance = 100.0f;
	if ( pSelfObj->FindAttr("SpringRange") )
	{
		distance = pSelfObj->QueryFloat("SpringRange");
		distance = distance * 2;
	}

	//4����ȡNpc��Ч��Χ�ڵ��˻��б�
	CVarList CResult;
	pKernel->GetAroundList(self, distance, TYPE_PLAYER, 0, CResult);
	int nCount = static_cast<int>(CResult.GetCount());

	//5���������ð�ȫ�����˻���Ӧ������
	LoopBeginCheck(a)
	for( int i = 0; i < nCount; ++i )
	{
		LoopDoCheck(a)
		//5.1����֤������Ч��
		PERSISTID obj = CResult.ObjectVal(i);
		if (!pKernel->Exists(obj)) 
		{
			continue;
		}

		//5.2�������뿪��ȫ������״̬
		m_pSafeAreaNpc->UpdateLeaveState(pKernel, obj, nNation, nSafeRule);
	}

	return 0;
}

/*!
* @brief	��ȫ��NPC���볡��
* @param    ����ָ��
* @param    ��ȫ��NPC����
* @param    ��������
* @param    �ص�Я������
* @return	bool
*/
int SafeAreaNpcModule::OnEntry(IKernel * pKernel, const PERSISTID & self,
						 const PERSISTID & sender, const IVarList & args)
{
	//1����֤��������Ƿ�Ϊ��
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (pSelfObj == NULL)
	{
		return 0;
	}

	if (!pKernel->Exists(sender) ) 
	{
		return 0;
	}

    //2����֤�Ƿ�Ϊ:Npc�����볡������
	if ( TYPE_NPC != pKernel->Type(self)
		|| TYPE_SCENE != pKernel->Type(sender) ) 
	{
		return 0;
	}

	//3����ȡNpc��Ч��Χ:Ĭ��0.0f
	float distance = 0.0f;
	if ( pSelfObj->FindAttr("SpringRange") )
	{
		distance = pSelfObj->QueryFloat("SpringRange");
	}

	int nNation = 0/*pSelfObj->QueryInt(FIELD_PROP_NATION)*/;
	int nSafeRule = pSelfObj->QueryInt(FIELD_PROP_SAFE_RULE);

	//4����ȡNpc��Ч��Χ�ڵ��˻��б�
	CVarList CResult;
	pKernel->GetAroundList(self, distance, TYPE_PLAYER, 0, CResult);
	int nCount = static_cast<int>(CResult.GetCount());
	
	//5���������ð�ȫ�����˻���Ӧ������
	LoopBeginCheck(b)
	for( int i = 0; i < nCount; ++i )
	{
		LoopDoCheck(b)
		//5.1����֤������Ч��
		PERSISTID obj = CResult.ObjectVal(i);
		if ( !pKernel->Exists(obj) ) 
		{
			continue;
		}

		//5.2�������뿪��ȫ������״̬
		m_pSafeAreaNpc->UpdateEntryState(pKernel, obj, nNation, nSafeRule);
	}

	return 0;
}

/*!
* @brief	����뿪����
* @param    ����ָ��
* @param    ��Ҷ���
* @param    sender����
* @param    �ص�Я������
* @return	bool
*/
int SafeAreaNpcModule::OnLeaveScene(IKernel* pKernel, const PERSISTID& self,
							  const PERSISTID& sender, const IVarList& args)
{
	//1������������֤
	if ( !pKernel->Exists(self) ) 
	{
		return 0;
	}

	//2�������ж�
    if ( TYPE_PLAYER != pKernel->Type(self) ) 
	{
		return 0;
	}

	//3����ȡ��Ҳ�������
	IGameObj * selfObj = pKernel->GetGameObj(self);
	if ( NULL == selfObj ) 
	{
		return 0;
	}

	//3����֤�뿪�����Ƿ��������Ұ�ȫ���ڵ���ʱ��ʶ
	if ( selfObj->FindData( "SafeAreaFlag" ) && selfObj->QueryDataInt("SafeAreaFlag") == 1 )
	{
		//3.1�������뿪��ȫ������״̬
		m_pSafeAreaNpc->UpdateLeaveState(pKernel, self, 0, 0);
	}

	return 0;
}

// ��ҽ��볡��
int SafeAreaNpcModule::OnPlayerEnterScene(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return 0;
	}
// 	if (m_pSafeAreaNpc->NeedCheckRectangularSafeArea(pKernel))
// 	{
// 		ADD_HEART_BEAT(pKernel, self, "SafeAreaNpcModule::HB_CheckRectangularSafeArea", 500);
// 		ADD_DATA_INT(pSelfObj, RECT_AREA_INDEX);
// 	}

	return 0;
}

// ��Ӧ����仯 
int SafeAreaNpcModule::OnCommandChangeGroup(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& args)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return 0;
	}

	int nSceneId = pKernel->GetSceneId();
	if (nSceneId > OUTDOOR_SCENE_MAX)
	{
		return 0;
	}
	int nChangeGroupType = args.IntVal(1);
	int nRectAreaIndex = pSelfObj->QueryDataInt(RECT_AREA_INDEX);
	// ����ھ��ΰ�ȫ����,�������ȥ���޵�״̬
	if (nRectAreaIndex > NOT_IN_RECT_SAFE_AREA)
	{
		CVarList bufftypelist;
		bufftypelist << CB_CANT_BE_ATTACK;

		const RectangularSafeArea* pData = m_pSafeAreaNpc->QueryRectangularSafeAreaInfo(nRectAreaIndex);
		if (NULL == pData)
		{
			return 0;
		}

		if (GROUP_CHANGE_TYPE_ENTRY == nChangeGroupType)
		{
			FightInterfaceInstance->RemoveControlBuffer(pKernel, self, bufftypelist);
			if (pSelfObj->FindData("SafeAreaFlag"))
			{
				pSelfObj->RemoveData("SafeAreaFlag");
			}
		}
		else
		{
			FightInterfaceInstance->AddControlBuffer(pKernel, self, self, bufftypelist, CONTROL_BUFF_FOREVER);
			if (!pSelfObj->FindData("SafeAreaFlag"))
			{
				pSelfObj->AddDataInt("SafeAreaFlag", 1);
			}
		}
	}	
	return 0;
}

/*!
* @brief	���½��밲ȫ��״̬
* @param    ����ָ��
* @param    ��Ҷ���
* @param	��ȫ��NPC����
*/
void SafeAreaNpcModule::UpdateEntryState(IKernel* pKernel, const PERSISTID& player, int nNation, int nSafeRule)
{
	//1����������У��
	if ( !pKernel->Exists(player) ) 
	{
		return;
	}
	IGameObj *playerObj = pKernel->GetGameObj(player);
	if (NULL == playerObj)
	{
		return;
	}

	//2����֤�Ƿ񱣻�
	if (!IsSafe(pKernel, player, nNation, nSafeRule, SAFE_AREA_NPC_ENTRY))
	{
		return;
	}

	//3�������Ƿ��ܹ����뱻��������(1:��)
	CVarList bufftypelist;
	bufftypelist << CB_CANT_BE_ATTACK;
	FightInterfaceInstance->AddControlBuffer(pKernel, player, player, bufftypelist,  CONTROL_BUFF_FOREVER);

	//4����ȫ����ͳһ��ռ���ӣ��ɴ�͸
	playerObj->SetInt("PathGrid",TYPE_PLAYER_PATH_GRID_OFF);
	playerObj->SetInt("PathThrough",PATH_THROUGH_ON);

	//5�������ҽ��밲ȫ����ʱ����(0:��ȫ����,1:��ȫ����)
	ADD_DATA_INT(playerObj, "SafeAreaFlag");
	playerObj->SetDataInt("SafeAreaFlag", 1);
	
	return;
}

/*!
* @brief	�����뿪��ȫ��״̬
* @param    ����ָ��
* @param    ��Ҷ���
* @param	��ȫ��NPC����
*/
void SafeAreaNpcModule::UpdateLeaveState(IKernel* pKernel, const PERSISTID& player, int nNation, int nSafeRule)
{
	//1����������У��
	if ( !pKernel->Exists(player) ) 
	{
		return;
	}
	IGameObj *playerObj = pKernel->GetGameObj(player);
	if (NULL == playerObj)
	{
		return;
	}

	//2���뿪�ض����ñ�����״̬,�˴�����ֻΪ����Ϣ
	IsSafe(pKernel, player, nNation, nSafeRule, SAFE_AREA_NPC_LEAVE);

	// ������״̬�²�ɾ��
// 	PERSISTID escort_npc = PERSISTID();
// 	if (playerObj->GetClassType() == TYPE_PLAYER)
// 	{
// 		escort_npc = playerObj->QueryObject(FIELD_PROP_LINK_NPC);
// 	}
// 	
// 	if (escort_npc.IsNull())
// 	{
// 		//3�������Ƿ��ܹ����뱻��������(0:��)
// 		CVarList bufftypelist;
// 		bufftypelist << CB_CANT_BE_ATTACK;
// 		FightInterfaceInstance->RemoveControlBuffer(pKernel, player, bufftypelist);
// 	}
	
	//4������ͬһ�ӿ����ô�͸����
	//ReviveManagerModule::SetObjectPathThroughState(pKernel, player);
	
	//5�������ҽ��밲ȫ����ʱ����
	if( playerObj->FindData("SafeAreaFlag"))
	{
		playerObj->RemoveData("SafeAreaFlag");
	}

	// ��ȥ�ھ��ΰ�ȫ����ʶ
	if (playerObj->FindData(RECT_AREA_INDEX))
	{
		playerObj->SetDataInt(RECT_AREA_INDEX, NOT_IN_RECT_SAFE_AREA);
	}
    
	return;
}


/*!
* @brief	�Ƿ񱣻�(������֤)
* @param    ����ָ��
* @param    ��Ҷ���
* @param	��ȫ��NPC����
* @return   �Ƿ񱣻�
*/
bool SafeAreaNpcModule::IsSafe(IKernel* pKernel, const PERSISTID& player, int nNation, int nSafeRule, const int type)
{
	//1������������֤
	IGameObj * pSceneObj = pKernel->GetGameObj(pKernel->GetScene());
	if( pSceneObj == NULL )
	{
		return false;
	}

	IGameObj * pPlayerObj = pKernel->GetGameObj(player);
	if( pPlayerObj == NULL)
	{
		return false;
	}

	switch (nSafeRule)
	{
	case SAFE_AREA_NPC_OLD:              //�ϵİ�ȫ��
		{
			if( type == SAFE_AREA_NPC_ENTRY )
			{
				ResponseClientMsg(pKernel, player, SYSTEM_INFO_ID_9001);
			}
			else
			{
				ResponseClientMsg(pKernel, player, SYSTEM_INFO_ID_9002);
			}
			return true;
		}
		break;
	case SAFE_AREA_NPC_ALL_NATION:      //�����������
		{
			if( type == SAFE_AREA_NPC_ENTRY )
			{
                ResponseClientMsg(pKernel, player, SYSTEM_INFO_ID_9005);
			}
			else
			{
				ResponseClientMsg(pKernel, player, SYSTEM_INFO_ID_9008);
			}
			return true;
		}
		break;
	case SAFE_AREA_NPC_OWN_NATION:      //�����������
		{
			if( pPlayerObj->QueryInt("Nation") == nNation)
			{
				if( type == SAFE_AREA_NPC_ENTRY )
				{
					ResponseClientMsg(pKernel, player, SYSTEM_INFO_ID_9003);
				}
				else
				{
					ResponseClientMsg(pKernel, player, SYSTEM_INFO_ID_9006);
				}
				return true;
			}
			else
			{
				if( type == SAFE_AREA_NPC_ENTRY )
				{
					ResponseClientMsg(pKernel, player, SYSTEM_INFO_ID_9004);
				}
				else
				{
					ResponseClientMsg(pKernel, player, SYSTEM_INFO_ID_9007);
				}
			}
		}
		break;
	case SAFE_AREA_NPC_ENEMY_NATION:    //�����й����
		{
			if( pPlayerObj->QueryInt("Nation") != nNation )
			{
				if( type == SAFE_AREA_NPC_ENTRY )
				{
					ResponseClientMsg(pKernel, player, SYSTEM_INFO_ID_9003);
				}
				else
				{
					ResponseClientMsg(pKernel, player, SYSTEM_INFO_ID_9006);
				}
				return true;
			}
			else
			{
				if( type == SAFE_AREA_NPC_ENTRY )
				{
					ResponseClientMsg(pKernel, player, SYSTEM_INFO_ID_9004);
				}
				else
				{
					ResponseClientMsg(pKernel, player, SYSTEM_INFO_ID_9007);
				}
			}
		}
		break;
	default:
		break;
	}

	return false;
}

//��Ӧ�ͻ���������ʾ��Ϣ
void SafeAreaNpcModule::ResponseClientMsg(IKernel* pKernel, const PERSISTID& player, const int info_id)
{
	CustomSysInfo(pKernel, player, info_id, CVarList());	
}

// ��ȡ���ΰ�ȫ������
bool SafeAreaNpcModule::LoadRectanularSafeAreaConfig(IKernel* pKernel)
{
	// �����ļ�·��
	std::string pathName = pKernel->GetResourcePath();
	pathName += "ini/rule/safearea/rectangular_safe_area.xml";

	CXmlFile xmlfile(pathName.c_str());
	if (!xmlfile.LoadFromFile())
	{
		std::string info = "Not Exist " + pathName;
		::extend_warning(LOG_ERROR, info.c_str());
		return false;
	}

	int nSectionCount = xmlfile.GetSectionCount();

	m_vecRectangularSafeArea.clear();
	m_vecRectangularSafeArea.reserve(nSectionCount);
	LoopBeginCheck(e);
	for (int i = 0; i < nSectionCount; ++i)
	{
		LoopDoCheck(e);
		const char* section = xmlfile.GetSectionByIndex(i);

		RectangularSafeArea data;
		data.nAreaId = StringUtil::StringAsInt(section);
		data.nSceneId = xmlfile.ReadInteger(section, "SceneID", 0);
		data.nNation = xmlfile.ReadInteger(section, "Nation", 0);
		data.nSafeRule = xmlfile.ReadInteger(section, "SafeRule", 0);

		const char* strPos = xmlfile.ReadString(section, "CenterPos", "");
		FmVec2 pos;
		ParsePos(pos, strPos);

		float orient = xmlfile.ReadFloat(section, "Orient", 0.0f);
		float width = xmlfile.ReadFloat(section, "Width", 0.0f);
		float height = xmlfile.ReadFloat(section, "Height", 0.0f);

		data.kRectangle = Rectangle2D(pos, orient, width, height);
		
		m_vecRectangularSafeArea.push_back(data);
	}
	return true;
}

// ��������
bool SafeAreaNpcModule::ParsePos(FmVec2& outPos, const char* strPos)
{
	if (StringUtil::CharIsNull(strPos))
	{
		return false;
	}

	CVarList pos;
	StringUtil::SplitString(pos, strPos, ",");
	if (pos.GetCount() != 2)
	{
		return false;
	}
	outPos.x = pos.FloatVal(0);
	outPos.y = pos.FloatVal(1);
	return true;
}

// �Ƿ���Ҫ�����ΰ�ȫ�� 
bool SafeAreaNpcModule::NeedCheckRectangularSafeArea(IKernel* pKernel)
{
	bool bIsCheck = false;
	int nSceneId = pKernel->GetSceneId();
	int nSize = (int)m_vecRectangularSafeArea.size();
	LoopBeginCheck(e);
	for (int i = 0; i < nSize; ++i)
	{
		LoopDoCheck(e);
		const RectangularSafeArea& data = m_vecRectangularSafeArea[i];
		if (data.nSceneId == nSceneId)
		{
			bIsCheck = true;
			break;
		}
	}

	return bIsCheck;
}

// ����Ƿ�����뿪���ΰ�ȫ��
int SafeAreaNpcModule::HB_CheckRectangularSafeArea(IKernel* pKernel, const PERSISTID& self, int slice)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj || !pSelfObj->FindData(RECT_AREA_INDEX))
	{
		return 0;
	}

	int nRectAreaIndex = pSelfObj->QueryDataInt(RECT_AREA_INDEX);
	// ԭ���ڰ�ȫ����,��֤�Ƿ����뿪��ȫ��,û�ڰ�ȫ��,����֤�Ƿ���밲ȫ��
	if (nRectAreaIndex > NOT_IN_RECT_SAFE_AREA)
	{
		const RectangularSafeArea* pData = m_pSafeAreaNpc->QueryRectangularSafeAreaInfo(nRectAreaIndex);
		if (NULL == pData)
		{
			return 0;
		}

		if (!m_pSafeAreaNpc->CheckInRectSafeArea(pKernel, self, pData->kRectangle))
		{
			m_pSafeAreaNpc->UpdateLeaveState(pKernel, self, pData->nNation, pData->nSafeRule);
		}
	}
	else
	{
		m_pSafeAreaNpc->CheckEnterRectangularSafeArea(pKernel, self);
	}
	return 0;
}

// ����Ƿ��ھ��ΰ�ȫ���� 
void SafeAreaNpcModule::CheckEnterRectangularSafeArea(IKernel* pKernel, const PERSISTID& self)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return;
	}
	int nSceneId = pKernel->GetSceneId();
	int nSize = (int)m_vecRectangularSafeArea.size();
	LoopBeginCheck(e);
	for (int i = 0; i < nSize; ++i)
	{
		LoopDoCheck(e);
		const RectangularSafeArea& data = m_vecRectangularSafeArea[i];
		if (data.nSceneId != nSceneId)
		{
			continue;
		}

		if (CheckInRectSafeArea(pKernel, self, data.kRectangle))
		{
			UpdateEntryState(pKernel, self, data.nNation, data.nSafeRule);	
			pSelfObj->SetDataInt(RECT_AREA_INDEX, data.nAreaId);
			break;
		}
	}

}

// ����Ƿ���ָ���İ�ȫ����
bool SafeAreaNpcModule::CheckInRectSafeArea(IKernel* pKernel, const PERSISTID& self, const Rectangle2D& rect)
{
	IGameObj* pSelfObj = pKernel->GetGameObj(self);
	if (NULL == pSelfObj)
	{
		return false;
	}

	float fPosX = pSelfObj->GetPosiX();
	float fPosZ = pSelfObj->GetPosiZ();
	float fRadius = pSelfObj->QueryFloat("CollideRadius");

	Cricle2D targetShape(FmVec2(fPosX, fPosZ), fRadius);

	int result = ShapeCollision::Intersects(targetShape, rect);
	return ECOLLIDE_RESULT_INTERSECT == result;
}

// ����id��ѯ��ȫ������
const SafeAreaNpcModule::RectangularSafeArea* SafeAreaNpcModule::QueryRectangularSafeAreaInfo(int nIndex)
{
	RectangularSafeArea* pFindData = NULL;
	int nSize = (int)m_vecRectangularSafeArea.size();
	LoopBeginCheck(e);
	for (int i = 0; i < nSize; ++i)
	{
		LoopDoCheck(e);
		const RectangularSafeArea& data = m_vecRectangularSafeArea[i];
		if (data.nAreaId == nIndex)
		{
			pFindData = &m_vecRectangularSafeArea[i];
			break;
		}
	}

	return pFindData;
}