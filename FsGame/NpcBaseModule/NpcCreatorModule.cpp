//--------------------------------------------------------------------
// �ļ���:      NpcCreatorModule.cpp
// ��  ��:      NPC����
// ˵  ��:		
// ��������:    2014��10��17��
// ������:        
// �޸���:        
//    :       
//--------------------------------------------------------------------
#include "FsGame/NpcBaseModule/NpcCreatorModule.h"
//#include "FsGame/Define/SceretSceneDefine.h"
#include "FsGame/Define/GameDefine.h"
#include <time.h>
#include <math.h>
#include <io.h>
#include <list>
#include <algorithm>
#include "utils/XmlFile.h"
#include "utils/string_util.h"
#include "FsGame/NpcBaseModule/AI/AIDefine.h"
//#include "FsGame/SceneBaseModule/GroupScene/GroupSceneBase.h"
//#include "FsGame/Define/BranchSceneDefine.h"
#include "FsGame/Define/ServerCustomDefine.h"
#include "utils/custom_func.h"
#include "FsGame/Define/FightDefine.h"
#include "FsGame/Interface/FightInterface.h"

#define NPC_POOL "ini/npc/npc_config/npc_pool.xml"
#define TASK_CREATOR "ini/npc/npc_config/TaskCreator.xml"
#define GUILD_INBREAK "ini/npc/npc_config/guild_inbreak.xml"

// ��������
NpcCreatorModule* NpcCreatorModule::m_pNpcCreatorModule = NULL;

// ��ʼ��
bool NpcCreatorModule::Init(IKernel* pKernel)
{
	// ģ��ָ��
	m_pNpcCreatorModule = this;

	// ע��ص�
	pKernel->AddClassCallback("RandomNpcCreator", "OnCreateClass",
		OnRandomNpcCreatorCreateClass);

	pKernel->AddClassCallback("RandomNpcCreatorItem", "OnCreateClass",
		OnRandomNpcCreatorItemCreateClass);

	pKernel->AddEventCallback("RandomNpcCreator", "OnSpring",
		OnRandomNpcCreatorSpring);

	pKernel->AddEventCallback("RandomNpcCreator", "OnEndSpring",
		OnRandomNpcCreatorEndSpring);

	pKernel->AddEventCallback("scene", "OnCreate", OnSceneCreate);

	pKernel->AddEventCallback("npc", "OnDestroy", OnNpcDestroy);

// 	pKernel->AddEventCallback("FunctionNpc", "OnSpring", OnNpcSpring);
// 
// 	pKernel->AddEventCallback("FunctionNpc", "OnEndSpring", OnNpcEndSpring);
	// ����ע��
	DECL_HEARTBEAT(NpcCreatorModule::H_Refresh);
	DECL_HEARTBEAT(NpcCreatorModule::H_EndRefresh);
	DECL_HEARTBEAT(NpcCreatorModule::H_DelayCreateStaticNpc);
	DECL_HEARTBEAT(NpcCreatorModule::H_DelayCreateRandomNpc);
	DECL_HEARTBEAT(NpcCreatorModule::H_GroupSceneCreateStaticNpc);
	DECL_HEARTBEAT(NpcCreatorModule::H_GroupSceneCreateRandomNpc);
	//DECL_HEARTBEAT(NpcCreatorModule::H_GroupCreateNpcByTime);

    DECL_CRITICAL(NpcCreatorModule::C_OnCurStepChanged);
	

	// ������Դ
	extend_warning(pKernel, "Load NpcCreatorModule resource begin...");

	LoadNpcInfo(pKernel);
    LoadNpcPool(pKernel);
	
	if (!LoadTaskCreatorRes(pKernel))
	{
		Assert(false);
		return false;
	}
	//LoadGuildInbreak(pKernel);

	extend_warning(pKernel, "Load NpcCreatorModule resource end.");

	srand((unsigned int)time(NULL));
	return true;
}

void NpcCreatorModule::ClearXMLData()
{
    LoopBeginCheck(check_a);
	for(auto mapIter = m_clone_npc_info.begin(); mapIter != m_clone_npc_info.end() ;++mapIter )
	{
        LoopDoCheck(check_a);
        LoopBeginCheck(check_b);
		for(int iVec = 0; iVec < (int)mapIter->second.size();iVec++)
		{
            LoopDoCheck(check_b);
			if( mapIter->second[iVec] )
				delete  mapIter->second[iVec];
		}
	}

	
  

    LoopBeginCheck(check_e);
    for(std::vector<char*>::iterator vecfileiter = m_vecbuff.begin(); vecfileiter != m_vecbuff.end() ;++vecfileiter )
    {
        LoopDoCheck(check_e);
        delete *vecfileiter;
    }
}

void NpcCreatorModule::ParseCreateNpcByTime(const std::string& sceneCfgName, const std::vector<xml_document<>*>& tempDocVec)
{
	
	int iDocCount = (int)tempDocVec.size();
	LoopBeginCheck(a);
	for (int fileIndex = 0; fileIndex < iDocCount; fileIndex++)
	{
		LoopDoCheck(a);
		xml_node<>* pNodeRoot = tempDocVec[fileIndex]->first_node("object");
		if (pNodeRoot == NULL)
			continue;
		// �����������������
		xml_node<>* pNode = pNodeRoot->first_node("createByTime");

		if (pNode != NULL)
		{

			xml_node<>* pCreator = pNode->first_node("creator");

			std::vector<RefreshNpcRule> npcInfo;
			LoopBeginCheck(check_l1);
			while (pCreator != NULL){
				LoopDoCheck(check_l1);
				int waitTime = convert_int(QueryXmlAttr(pCreator, "waitTime"), 0);
				int refreshTime = convert_int(QueryXmlAttr(pCreator, "refreshtime"), 0);
				int totalStep = convert_int(QueryXmlAttr(pCreator, "totalstep"), 0);

				RefreshNpcRule m_GuardNpc;

				m_GuardNpc._totalStep = totalStep;
				m_GuardNpc._refreshTime = refreshTime;
				m_GuardNpc._waitTime = waitTime;
				xml_node<>* pItem = pCreator->first_node("item");

				if (pItem == NULL) {
					continue;
				}

				LoopBeginCheck(check_l2);
				while (pItem != NULL)
				{
					LoopDoCheck(check_l2);
					// ����
					float x = convert_float(QueryXmlAttr(pItem, "x"));
					float y = convert_float(QueryXmlAttr(pItem, "y"));
					float z = convert_float(QueryXmlAttr(pItem, "z"));
					int amount = convert_int(QueryXmlAttr(pItem, "amount"));
					// NPC���
					const char* npc_id = QueryXmlAttr(pItem, "id");

					float orient = convert_float(QueryXmlAttr(pItem, "ay"));

					m_GuardNpc._npcInfo.push_back(NpcInfo(npc_id, x, y, z, orient, amount));

					pItem = pItem->next_sibling("item");
				}

				npcInfo.push_back(m_GuardNpc);
				pCreator = pCreator->next_sibling("creator");

			};
			m_refresh_npc_by_time.insert(std::make_pair(sceneCfgName, npcInfo));
		}

	}


}

void NpcCreatorModule::ParseWheelNpc(const std::string& sceneCfgName, const std::vector<xml_document<>*>& tempDocVec)
{
	int iDocCount = (int)tempDocVec.size();
	LoopBeginCheck(a);
	for (int fileIndex = 0; fileIndex < iDocCount; fileIndex++)
	{
		LoopDoCheck(a);
		xml_node<>* pNodeRoot = tempDocVec[fileIndex]->first_node("object");
		if (pNodeRoot == NULL)
			continue;
		// �����������������
		xml_node<>* pNode = pNodeRoot->first_node("wheelcreator");
		LoopBeginCheck(check_l1);
		while (pNode != NULL)
		{
				LoopBeginCheck(check_l1);
				//key ɱ������
				std::map<int ,std::vector<WheelNpc> > wheelNpcGroup;
				xml_node<>* pNpcGroup = pNode->first_node("stepcreate");
				while (pNpcGroup != NULL)
				{
					std::vector<WheelNpc> wheelNpc;
					xml_node<>* pItem = pNpcGroup->first_node("item");
					LoopBeginCheck(check_l2);
					while (pItem != NULL)
					{
						LoopDoCheck(check_l2);

						// ����
						float x = convert_float(QueryXmlAttr(pItem, "x"));
						float y = convert_float(QueryXmlAttr(pItem, "y"));
						float z = convert_float(QueryXmlAttr(pItem, "z"));
						int amount = convert_int(QueryXmlAttr(pItem, "amount"));
						int killNum = convert_int(QueryXmlAttr(pItem, "needKillNum"));

						// NPC���
						const char* npc_id = QueryXmlAttr(pItem, "id");

						float orient = convert_float(QueryXmlAttr(pItem, "ay"));

						wheelNpc.push_back(WheelNpc(npc_id, x, y, z, orient, amount, killNum));

						pItem = pItem->next_sibling("item");
					}
						
					int groupNeedKillNum = convert_int(QueryXmlAttr(pNpcGroup, "needKillNum"));

					if (!wheelNpc.empty())
					{
						wheelNpcGroup.insert(make_pair(groupNeedKillNum,wheelNpc));
					}
					pNpcGroup = pNpcGroup->next_sibling("stepcreate");
				}
				if (!wheelNpcGroup.empty())
				{
					m_wheel_npc.insert(std::make_pair(sceneCfgName, wheelNpcGroup));
				}
				pNode = pNode->next_sibling("wheelcreator");
			};
		}

}



// �ر�
bool NpcCreatorModule::Shut(IKernel* pKernel)
{
    ClearXMLData();
    m_mapNpcPool.clear();
    v_vecSceneNpc.clear();
	m_TaskCreatorMap.clear();
	return true;
}

// �������������
int NpcCreatorModule::OnRandomNpcCreatorCreateClass(IKernel* pKernel, int index)
{
	createclass_from_xml(pKernel, index, "struct\\npc\\creator\\randomnpc_creator.xml");
	return 0;
}

// �������������������
int NpcCreatorModule::OnRandomNpcCreatorItemCreateClass(IKernel* pKernel, int index)
{
	createclass_from_xml(pKernel, index, "struct\\npc\\creator\\randomnpc_creatoritem.xml");

	return 0;
}

// ����
int NpcCreatorModule::OnRandomNpcCreatorSpring(IKernel* pKernel, const PERSISTID& creator, 
											   const PERSISTID& sender, const IVarList& args)
{
    const char* script = pKernel->GetScript(sender);
	// ����
	if (pKernel->Type(sender) != TYPE_PLAYER && strcmp(script, "ConvoyNpc") != 0)
	{
        //��Һͻ���NPC�����Դ���
		return 0;
	}

	//����ͨ������������NPC������
    IGameObj* pCreator = pKernel->GetGameObj(creator);
    if (pCreator == NULL)
    {
        return 0;
    }
    if (pCreator->QueryInt("CreateType") != 0)
    {
        return 0;
    }

    //�ؿ��ؾ�����н�����
    IGameObj* pPlayer = pKernel->GetGameObj(sender);
    if (pPlayer == NULL)
    {
        return 0;
    }
    if (pPlayer->FindAttr("Action"))
    {
        int action = pPlayer->QueryInt("Action");
        int sceneAction = pCreator->QueryInt("SceneAction");
        if (sceneAction > 0 && action != sceneAction)
        {
            //�ǹؿ��ؾ������û���н����˴�������������
            return 0;
        }
    }

	// ��¼��ǰ���д�����Χ������
	int new_count = pCreator->QueryInt("SpringCount") + 1;

	if (new_count < 1)
	{
		new_count = 1;
	}

	pCreator->SetInt("SpringCount", new_count);

	if (new_count >= 1)
	{
		// ��ʼˢ��
		BeginRefresh(pKernel, creator);

        if (pCreator->QueryInt("TotalStep") > 1)
        {
            ADD_CRITICAL(pKernel, creator, "CurStep", "NpcCreatorModule::C_OnCurStepChanged");
        }
	}
		
	return 0;
}

// ��������
int NpcCreatorModule::OnRandomNpcCreatorEndSpring(IKernel* pKernel, const PERSISTID& creator, 
												  const PERSISTID& sender, const IVarList& args)
{
	// ����
	if (pKernel->Type(sender) != TYPE_PLAYER)
	{
		return 0;
	}
    IGameObj* pCreator = pKernel->GetGameObj(creator);
    if (pCreator == NULL)
    {
        return 0;
    }

    int isDestroy = pCreator->QueryInt("IsDestroy");
    if (isDestroy == 1)
    {
        return 0;
    }

	// ��¼��ǰ���ڴ�����Χ������
	int new_count = pCreator->QueryInt("SpringCount") - 1;

	if (new_count < 0)
	{
		new_count = 0;
	}

	pCreator->SetInt("SpringCount", new_count);

	if (new_count == 0)
	{
		// ֹͣˢ��
		EndRefresh(pKernel, creator);
	}

	return 0;
}

// int NpcCreatorModule::OnNpcSpring(IKernel* pKernel, const PERSISTID& creator, const PERSISTID& sender, const IVarList& args)
// {
// 	IGameObj* pSenderObj = pKernel->GetGameObj(sender);
// 	if (NULL == pSenderObj || pSenderObj->GetClassType() != TYPE_PLAYER)
// 	{
// 		return 0;
// 	}
// 
// 	pKernel->AddVisualPriority(sender, creator, true);
// 	return 0;
// }
// 
// int NpcCreatorModule::OnNpcEndSpring(IKernel* pKernel, const PERSISTID& creator, const PERSISTID& sender, const IVarList& args)
// {
// 	IGameObj* pSenderObj = pKernel->GetGameObj(sender);
// 	if (NULL == pSenderObj || pSenderObj->GetClassType() != TYPE_PLAYER)
// 	{
// 		return 0;
// 	}
// 
// 	pKernel->RemoveVisualPriority(sender, creator);
// 	return 0;
// }

// ������̬NPC�ʹ�����
int NpcCreatorModule::OnSceneCreate(IKernel* pKernel, const PERSISTID& scene, 
									const PERSISTID& sender, const IVarList& args)
{
	// ����Ǹ��� ���Ȳ���npc��ȥ
	if (pKernel->GetSceneClass() == 2)
	{
		return 0;
	}

	// �õ�����NPC���������ļ�
	fast_string creator_file;
	fast_string scene_config = pKernel->GetConfig(scene);

	// ʵ���ļ���
	size_t n_pos = scene_config.find_last_of('\\');

	scene_config = scene_config.substr(n_pos + 1, scene_config.length() - n_pos - 1);
	auto it = m_pNpcCreatorModule->m_group_npc_info.find(scene_config.c_str());
	if (it == m_pNpcCreatorModule->m_group_npc_info.end()){
		return 0;
	}
		// �����ļ����������ļ�
	for (auto its:it->second)
	{
		m_pNpcCreatorModule->CreateStaticNpc(pKernel, scene, its.m_createStaticGroup);
		m_pNpcCreatorModule->CreateRandomNpc(pKernel, scene, its.m_createRandGroup);
	}

    //�ؾ������ڴ���ʱ��¼������NPC����
    m_pNpcCreatorModule->GetSceneNpcNum(pKernel, scene);

	return 0;
}

// NPC��ɾ��
int NpcCreatorModule::OnNpcDestroy(IKernel* pKernel, const PERSISTID& npc, 
								   const PERSISTID& sender, const IVarList& args)
{
    IGameObj* pNpc = pKernel->GetGameObj(npc);
    if (pNpc == NULL)
    {
        return 0;
    }
	// ȡ��������
	if (!pNpc->FindData("CreatorItem"))
	{
		return 0;
	}

	PERSISTID creator_item = pNpc->QueryDataObject("CreatorItem");

	if (!pKernel->Exists(creator_item))
	{
		return 0;
	}
    IGameObj* pCreatorItem = pKernel->GetGameObj(creator_item);
    if (pCreatorItem == NULL)
    {
        return 0;
    }

	PERSISTID creator = pKernel->Parent(creator_item);
    IGameObj* pCreatorObj = pKernel->GetGameObj(creator);

	if (!pKernel->Exists(creator) || NULL == pCreatorObj)
	{
		return 0;
	}

	// �ͷ�λ��
	int pos_row = pNpc->QueryDataInt("CreatorPosition");
    IRecord* positionRec = pCreatorItem->GetRecord(POSITION_INFO_REC);
    if (positionRec == NULL)
    {
        return 0;
    }

	if (pos_row < positionRec->GetRows())
	{
        positionRec->SetInt(pos_row, POSIINFOREC_COL_USED, 0);
        positionRec->SetInt64(pos_row, POSIINFOREC_COL_TIME, 0);
	}

	// ����creator_item������NPC�ĵ�ǰ����
	int max_amount = pCreatorItem->QueryInt("MaxAmount");
	int cur_amount = pCreatorItem->QueryInt("CurAmount");

	if (cur_amount > 0)
	{
		cur_amount--;
	}

	pCreatorItem->SetInt("CurAmount", cur_amount);

	IRecord* objectRec = pCreatorObj->GetRecord(CREATE_OBJECT_LIST_REC);
    if (objectRec == NULL)
    {
        return 0;
    }
    // ��������������NPC�б���ɾ��
	int row = objectRec->FindObject(0, npc);

	if (row < 0)
	{
		return 0;
	}

    objectRec->RemoveRow(row);

	// ��Ϊֹͣˢ�±�ɾ���ģ������κδ���
	if (!pNpc->FindData("CreatorEndRefresh"))
	{
		// ��Ϊ���ﱻɱ���ģ����õ�ǰʱ��Ϊ����ʱ��
		if (pos_row < positionRec->GetRows())
		{
            positionRec->SetInt64(pos_row, POSIINFOREC_COL_TIME, util_get_time_64());
		}

		// ֪ͨ������ˢ��
		if (cur_amount < max_amount)
		{
			BeginRefresh(pKernel, creator);
		}
	}

	return 0;
}

// ����
int NpcCreatorModule::H_Refresh(IKernel* pKernel, const PERSISTID& creator, int slice)
{
	// ����
	if (!pKernel->Exists(creator))
	{
		return 0;
	}
    IGameObj* pCreator = pKernel->GetGameObj(creator);
    if (pCreator == NULL)
    {
        return 0;
    }

	int nGroupID = pCreator->QueryInt("GroupID");

	// ��Ҫ�´������ٸ���
	bool bneedrefresh = false;

	// �´�ˢ�µ�ʱ��
	__int64 now = util_get_time_64();

	__int64 min_time = 3600 * 24 * 30;

	CVarList temp;

	//��������Ĭ��0��1��ֻ������Ӣ�֣�2������Ӣ�ָ�������С��
	int iCreateType = pCreator->QueryInt("CreateType");
	// ��������
	PERSISTID scene = pKernel->GetScene();
	
	CVarList creator_items;
	pKernel->GetChildList(creator, TYPE_HELPER, creator_items);

	// ����
	int creator_item_counts = (int)creator_items.GetCount();

    LoopBeginCheck(check_g);
	for (int i = 0; i < creator_item_counts; i++)
	{
        LoopDoCheck(check_g);
		PERSISTID creator_item = creator_items.ObjectVal(i);
        IGameObj* pCreatorItem = pKernel->GetGameObj(creator_item);
        if (pCreatorItem == NULL)
        {
            continue;
        }
        // �����Ƚ�
        int max_amount = pCreatorItem->QueryInt("MaxAmount");
        int cur_amount = pCreatorItem->QueryInt("CurAmount");

        if (pCreator->QueryInt("TotalStep") > 1)
        {
            //������ˢ��
            int curStep = pCreator->QueryInt("CurStep");//��ǰ����
            int step = pCreatorItem->QueryInt("Step");//������������
            if (curStep != step)
            {
                continue;
            }
            //��¼�˴������Ĺ�����Ϣ��������������ɱ������
            if (!pCreator->FindData("CurNum"))
            {
                pCreator->AddDataInt("CurNum", 0);
            }
            pCreator->SetDataInt("CurNum", max_amount);
            if (!pCreator->FindData("KillNum"))
            {
                pCreator->AddDataInt("KillNum", 0);
            }  
        }

		if (cur_amount >= max_amount)
		{
			continue;
		}

		// ˢ������
		int refresh_period = pCreatorItem->QueryInt("RefreshPeriod");

		// ��Ҫ����NPC������
		const char* config = pCreatorItem->QueryString("NpcID");

        if (StringUtil::CharIsNull(config))
		{
			continue;
		}

		int refresh_count = max_amount - cur_amount;

		// λ��ʹ�õĶ���
		PERSISTID& pos_obj = creator_item;

		// ��������(��¼����Ҫˢ�µ�����)
		int real_refresh_count = refresh_count;

		if (real_refresh_count <= 0)
		{
			continue;
		}

		IRecord* pRecord = pCreatorItem->GetRecord(POSITION_INFO_REC);

		if (NULL == pRecord)
		{
			continue;
		}

		// ���position��
		int max_postion_rows = pRecord->GetRows();

		if (max_postion_rows < max_amount)
		{
			// ���ô���ˢ��
			continue;
		}

		// ѭ��������λ�ü���list
		std::list<int> posList;

        LoopBeginCheck(check_h);
		for (int e = 0; e < max_postion_rows; ++e)
		{
            LoopDoCheck(check_h);
			int bUse = pRecord->QueryInt(e, POSIINFOREC_COL_USED);

			if (bUse <= 0)
			{
				// �ж�ˢ�������Ƿ�ﵽ
				__int64 last_dead_time = pRecord->QueryInt64(e, POSIINFOREC_COL_TIME);
				
				__int64 lapse_time = now - last_dead_time;

				if ( lapse_time < (__int64)refresh_period)
				{
					__int64 next_time = (__int64)refresh_period - (now - last_dead_time);

					if (next_time < min_time)
					{
						min_time = next_time;
					}

					// ˢ��ʱ��δ��
					bneedrefresh = true;

					// ����Ҫˢ�µ�������1
					real_refresh_count -= 1;

					continue;
				}

				if ( -1 == refresh_period && 0 != last_dead_time )
				{
					// ˢ��ʱ�����޳�ʱ��ֻ����ˢ��һ��
					bneedrefresh = false;
					
					real_refresh_count = 0;

					posList.clear();

					break;
				}

				// �ﵽʱ���˾ͼ�¼ˢ��λ��list
				posList.push_back(e);
			}
		}

		// ���û����������ˢ�ģ���ˢ��
		if (real_refresh_count <= 0)
		{
			// �Ƿ���Ҫ�ٴε���ˢ������
			if (bneedrefresh)
			{
				if (pKernel->FindHeartBeat(creator, "NpcCreatorModule::H_Refresh"))
				{
					pKernel->RemoveHeartBeat(creator, "NpcCreatorModule::H_Refresh");
				}
				
				pKernel->AddCountBeat(creator, "NpcCreatorModule::H_Refresh", (int)min_time, 1);
			}

			return 0;
		}

		// ����λ������
		int can_refresh_pos_count = (int)posList.size();
		if (can_refresh_pos_count == 0)
		{
			return 0;
		}

		if (real_refresh_count > can_refresh_pos_count)
		{
			real_refresh_count = can_refresh_pos_count;
		}

		// ˢ��������
		int r_count = 0;

		PERSISTID RandCreator = pCreator->QueryObject("RandHelper");
		int iRandRow = pCreator->QueryInt("RandRow");
		// ������Ҫˢ�µ�����
        LoopBeginCheck(check_i);
		for (int j = 0; j < real_refresh_count; ++j)
		{
            LoopDoCheck(check_i);
			int row_index = 0;
			if (pKernel->Exists(RandCreator) && creator_item == RandCreator)
			{
				row_index = iRandRow;
			}
			else
			{
				// ���һ��λ��
				int r_pos = rand()%can_refresh_pos_count;

				// ȡ��λ������
				std::list<int>::iterator it = posList.begin();

                LoopBeginCheck(check_j);
				for (int t = 0; t < r_pos; ++t)
				{
                    LoopDoCheck(check_j);
					++it;
				}

				// �õ��к�
				row_index = *it;

				// ɾ��
				posList.erase(it);
			}
		
			// ����Npc
			float r = pRecord->QueryFloat(row_index, POSIINFOREC_COL_R);
			float y = pRecord->QueryFloat(row_index, POSIINFOREC_COL_Y);

			float x, z;
			if (r <= 0)
			{
				x = pRecord->QueryFloat(row_index, POSIINFOREC_COL_X);
				z = pRecord->QueryFloat(row_index, POSIINFOREC_COL_Z);
			}
			else
			{
				x = pRecord->QueryFloat(row_index, POSIINFOREC_COL_X) + (util_random_float(2.0f) - 1.0f) * r;
				z = pRecord->QueryFloat(row_index, POSIINFOREC_COL_Z) + (util_random_float(2.0f) - 1.0f) * r;
				y = GetCurFloorHeight(pKernel, x, y, z);
			}

            float o = pRecord->QueryFloat(row_index, POSIINFOREC_COL_O);
			//�Ƕ�-1���һ��
			if (o < 0)
			{
				o = util_random_float(2*PI);
			}

            std::string newConfig;
            m_pNpcCreatorModule->GetConfigID(pKernel, creator, config, newConfig);
            if (StringUtil::CharIsNull(newConfig.c_str()))
            {
                newConfig = config;
            }
            
			// ����������NPC
			CVarList create_args;
			create_args << CREATE_TYPE_PROPERTY_VALUE
						<< FIELD_PROP_GROUP_ID 
						<< nGroupID;

			// ���ﰴ�ȼ�ˢ���԰�
			if (pCreator->FindData("CreatorLevel"))
			{
				create_args << FIELD_PROP_NPC_PACKAGE_LEVEL
							<< pCreator->QueryDataInt("CreatorLevel");
			}

			PERSISTID npc = pKernel->CreateObjectArgs("", newConfig.c_str(), 0, x, y, z, o, create_args);
			if (pKernel->Exists(npc))
			{
                IGameObj* pNpc = pKernel->GetGameObj(npc);
                if (pNpc == NULL)
                {
                    continue;
                }
				// ����NPC���ĸ�����������
                pNpc->AddDataObject("CreatorItem", creator_item);
                pNpc->AddDataInt("CreatorPosition", row_index);
                pNpc->AddDataObject("Creator", creator);

				if(pNpc->FindAttr("BornX"))
				{
					pNpc->SetFloat("BornX", x);
					pNpc->SetFloat("BornZ", z);
				}

				// ��ӵ������������ɵĶ����б���
                IRecord* objectRec = pCreator->GetRecord(CREATE_OBJECT_LIST_REC);
                if (objectRec != NULL)
                {
                    objectRec->AddRowValue(-1, CVarList() << npc);
                }
				
				pRecord->SetInt(row_index, POSIINFOREC_COL_USED, 1);

				// ���ﰴ�ȼ�ˢ���԰�
				if (pCreator->FindData("CreatorLevel"))
				{
					pNpc->SetInt(FIELD_PROP_LEVEL, pCreator->QueryDataInt("CreatorLevel"));
				}

				// ������
				r_count++;
			}

			// ʣ������
			can_refresh_pos_count--;

			if (can_refresh_pos_count <= 0)
			{
				break;
			}
		}

		// ��ǰ��������
		pCreatorItem->SetInt("CurAmount", cur_amount + r_count);
	}

	// �Ƿ���Ҫ�ٴε��ñ�����
	if (bneedrefresh)
	{
		if (pKernel->FindHeartBeat(creator, "NpcCreatorModule::H_Refresh"))
		{
			pKernel->RemoveHeartBeat(creator, "NpcCreatorModule::H_Refresh");
		}
		
		pKernel->AddCountBeat(creator, "NpcCreatorModule::H_Refresh", (int)min_time, 1);
	}

	return 1;
}

// ����ˢ������
int NpcCreatorModule::H_EndRefresh(IKernel* pKernel, const PERSISTID& creator, int slice)
{

	return 0;
    IGameObj* pCreatorObj = pKernel->GetGameObj(creator);
    if (NULL == pCreatorObj)
    {
        return 0;
    }
	int i;

	// ɾ�����ڵĶ��󣬵��������ս�������ھͲ�ɾ��
    IRecord* objectRec = pCreatorObj->GetRecord(CREATE_OBJECT_LIST_REC);
    if (objectRec == NULL)
    {
        return 0;
    }
	int rows = objectRec->GetRows();

    LoopBeginCheck(check_k);
	for (i = rows - 1; i >= 0; i--)
	{
        LoopDoCheck(check_k);
        PERSISTID npc = objectRec->QueryObject(i, 0);

		if (!pKernel->Exists(npc))
		{
			continue;
		}
        IGameObj* pNpc = pKernel->GetGameObj(npc);
        if (pNpc == NULL)
        {
            continue;
        }
        // ֻ�з�����NPC���øñ�־
        int nDead = 0;

        if (pNpc->FindAttr("Dead"))
        {
            nDead = pNpc->QueryInt("Dead");
        }

        if (nDead <= 0)
        {
            PERSISTID creator_item = pNpc->QueryDataObject("CreatorItem");

            // ���һ�����(onnpcdestroy�ص��ڻ���һЩ����Ĵ���)
            pNpc->AddDataInt("CreatorEndRefresh", 0);

            // ɾ��NPC
            pKernel->DestroySelf(npc);
        }
	}
		
	// ����NPCû�б�ɾ��
	if (objectRec->GetRows() > 0)
	{
		pKernel->SetBeatCount(creator, "NpcCreatorModule::H_EndRefresh", 1);
	}

	return 1;
}

// ��ʼˢ��
void NpcCreatorModule::BeginRefresh(IKernel* pKernel, const PERSISTID& creator)
{
	if (!pKernel->FindHeartBeat(creator, "NpcCreatorModule::H_Refresh"))
	{
		pKernel->AddCountBeat(creator, "NpcCreatorModule::H_Refresh",
										500 + ::util_random_int(6) * 100, 1);
	}

	if (pKernel->FindHeartBeat(creator, "NpcCreatorModule::H_EndRefresh"))
	{
		pKernel->RemoveHeartBeat(creator, "NpcCreatorModule::H_EndRefresh");
	}
}

// ����ˢ��
void NpcCreatorModule::EndRefresh(IKernel* pKernel, const PERSISTID& creator)
{
	if (pKernel->FindHeartBeat(creator, "NpcCreatorModule::H_Refresh"))
	{
		pKernel->RemoveHeartBeat(creator, "NpcCreatorModule::H_Refresh");
	}

	if (!pKernel->FindHeartBeat(creator, "NpcCreatorModule::H_EndRefresh"))
	{
		pKernel->AddCountBeat(creator, "NpcCreatorModule::H_EndRefresh", 60 * 1000, 1);
	}
}

bool NpcCreatorModule::CreateStaticNpc(IKernel* pKernel, const PERSISTID& scene, std::vector<CreateStaticNpcInfo>& staticNpc, int nSubSceneNo, int nGroupId)
{
	int curSceneID = pKernel->GetSceneId();
// 	int nSceneType = GroupSceneBase::GetSceneType(pKernel, curSceneID); 
// 	if (nGroupId == -1 && nSceneType != GROUP_SCENE_NOT_SECRET)
//     {
//         // �ؾ�������������̬npc
//         return false;
//     }
	LoopBeginCheck(check_l);
	for (auto it : staticNpc)
	{
		LoopDoCheck(check_l);
		// ����npc
		CreateStaticNpcUnit(pKernel, scene, it, nSubSceneNo, nGroupId);
	}

	return true;  
}

// ����xml���ô���ĳ����̬NPC
PERSISTID NpcCreatorModule::CreateStaticNpcUnit(IKernel* pKernel, const PERSISTID& scene, 
	CreateStaticNpcInfo&pItem, int nSubSceneNo, int nGroupId)
{
	IGameObj* pSceneObj = pKernel->GetGameObj(scene);
	if ( NULL == pSceneObj)
	{
		return PERSISTID();
	}

	// ������Ӫ
	int nSceneNation = 0/*pSceneObj->QueryInt(FIELD_PROP_NATION)*/;
	// �����ʹ�ýǶ�

	
	if (pItem.m_npcID.empty())
	{
		return PERSISTID();
	}
	int groupID = 0;
	if (nGroupId != -1)
	{
		groupID = nGroupId;
		
	}
	else
	{
		groupID = pItem.m_groupID;
	}

	PERSISTID npc_obj;

	// <!--Nation 1 �������� 2 ��͢���� 0������-->
	if (0 == pItem.m_nation || nSceneNation == pItem.m_nation)
	{	
		float y1 = GetCurFloorHeight(pKernel, pItem.m_position.m_x, pItem.m_position.m_y, pItem.m_position.m_z);
		// ����������NPC
		npc_obj = pKernel->CreateObjectArgs("", pItem.m_npcID.c_str(), 0, pItem.m_position.m_x, y1, pItem.m_position.m_z, pItem.m_position.m_ay, CVarList() << CREATE_TYPE_PROPERTY_VALUE << "GroupID" << groupID);
// 		if (pKernel->Exists(npc_obj))
// 		{
// 			pKernel->SetInt(npc_obj, "NoSpring", 0);
// 		}
	}

	return npc_obj;
}

// �������NPC
bool NpcCreatorModule::CreateRandomNpc(IKernel* pKernel, const PERSISTID& scene,
	std::vector<CreateSpringNpcInfo>&npcCfg, int nSubSceneNo, int nGroupId)
{
	

    LoopBeginCheck(check_m);
	for (auto it : npcCfg)
	{
		LoopDoCheck(check_m);
		CreateARandomCreator(pKernel, scene, it, nSubSceneNo, nGroupId);

	}
	return true;
}

void NpcCreatorModule::CleanCfg()
{
	ClearXMLData();

	m_clone_npc_info.clear();

	m_group_npc_info.clear();

	m_refresh_npc_by_time.clear();

	m_wheel_npc.clear();

};


//���Ŀ¼���ļ����б�
void NpcCreatorModule::GetDirFileList(IKernel* pKernel,const fast_string& npcCfgPath, std::vector<xml_document<>*>& outFileList)
{

	fast_string creator_file = npcCfgPath;
	fast_string file_path = fast_string(pKernel->GetResourcePath()) + "ini\\Scene\\creator\\npc_creator\\";
	file_path = file_path + creator_file + "\\";
	fast_string find_file_path = file_path + "*.xml";
	_finddata_t findfile;
	int nFind = int(_findfirst(find_file_path.c_str(), &findfile));
	if (nFind == -1){
		return;
	}
	// ѭ������
	LoopBeginCheck(check_o);
	do
	{
		LoopDoCheck(check_o);
		// ʵ���ļ���
		creator_file = file_path + findfile.name;

		xml_document<>* doc = new xml_document<>();
		try
		{
			char* pfilebuff = GetFileBuff(creator_file.c_str());
			if (NULL == pfilebuff){
				continue;
			}
			m_vecbuff.push_back(pfilebuff);
			doc->parse<rapidxml::parse_default>(pfilebuff);
		}
		catch (parse_error& e)
		{
			::extend_warning(pKernel, e.what());
			delete doc;
			return;
		}
		// �����ļ���vector
		outFileList.push_back(doc);

	} while (_findnext(nFind, &findfile) == 0);
	_findclose(nFind);
}

void NpcCreatorModule::PraseCloneNpc(IKernel* pKernel, int sceneID, const fast_string& cfgPath)
{
	// �Ǹ�����XML����map
	

	auto it = m_clone_npc_info.find(sceneID);
	if (it != m_clone_npc_info.end())
	{
		::extend_warning(pKernel, "[Error]NpcCreatorModule::LoadCloneNpcInfo: found same scene:");
		return;
	}

	// ���ini�����ļ�·��
	std::vector<xml_document<>*> tempDocVec;
	GetDirFileList(pKernel, cfgPath, tempDocVec);
	if (tempDocVec.empty()){
		return;
	}
	// ����map
	auto pib = m_clone_npc_info.insert(std::make_pair(sceneID, tempDocVec));
	if (!pib.second)
	{
		::extend_warning(pKernel, "[Error]LoadXmlConfigError: insert error:");
	}

}

void NpcCreatorModule::ParseGroupNpc(IKernel* pKernel, const std::string& npcCfgName)
{
	auto it = m_group_npc_info.find(npcCfgName);
	if (it != m_group_npc_info.end())
	{
		::extend_warning(pKernel, "[Error]NpcCreatorModule::LoadCloneNpcInfo: found same scene:");
		::extend_warning(pKernel, npcCfgName.c_str());
	}
	std::vector<xml_document<>*> tempDocVec;
	GetDirFileList(pKernel, npcCfgName.c_str(), tempDocVec);
	if (tempDocVec.empty()){
		return;
	}

	
	ParseSceneNpc(pKernel,npcCfgName, tempDocVec);
	//��������ʱ��ˢ�µ�npc
	ParseCreateNpcByTime(npcCfgName, tempDocVec);
	ParseWheelNpc(npcCfgName, tempDocVec);
}


// ���븱��NPC��Ϣ
bool NpcCreatorModule::LoadNpcInfo(IKernel* pKernel)
{
	CleanCfg();

	// ���·��
	fast_string pathname = pKernel->GetResourcePath();
	pathname += "scenes.ini";

	CUtilIni inifile(pathname.c_str());
	if (!inifile.LoadFromFile())
	{
		::extend_warning(pKernel, "[Error]NpcCreatorModule::LoadCloneNpcInfo: No found file:");
		::extend_warning(pKernel, pathname.c_str());
		return false;
	}

	int count = (int)inifile.GetSectionCount();
    LoopBeginCheck(check_n);
	for (int i = 0; i < count; i++)
	{
        LoopDoCheck(check_n);
		const char* pSectName = inifile.GetSectionByIndex(i);
		int nClonable = inifile.ReadInteger(i, "Clonable", 0);
		// ���ini�����ļ�·��
		fast_string creator_file = inifile.ReadString(i, "Config", "");
		
		// �Ǹ�����XML����map
		int sceneid = ::atoi(pSectName);
		if (nClonable == 1)
		{
			PraseCloneNpc(pKernel, sceneid, creator_file);
		}


		size_t n_pos = creator_file.find_last_of('\\');
		fast_string npcBasePath = creator_file.substr(n_pos + 1, creator_file.length() - n_pos - 1);
		int nGrouping = inifile.ReadInteger(i, "Grouping", 0);
		if (nGrouping >= 1)
		{
			for (int i = 0; i < 20; i++)
			{
				std::string npcCfgName;
				if (i == 0)
				{
					npcCfgName = npcBasePath.c_str();
				}
				else
				{
					npcCfgName = npcBasePath.c_str() + std::string("_") + StringUtil::IntAsString(i);
				}
				ParseGroupNpc(pKernel, npcCfgName);
			}
		}
		
	}

	return true;
}

// ��������ˢ��������
bool NpcCreatorModule::LoadTaskCreatorRes(IKernel* pKernel)
{
	m_TaskCreatorMap.clear();

	// ����Npc����
	std::string file_path = pKernel->GetResourcePath();
	file_path += TASK_CREATOR;
	try
	{
		char* pfilebuff = GetFileBuff(file_path.c_str());

		if (StringUtil::CharIsNull(pfilebuff))
		{
			std::string err_msg = file_path;
			err_msg.append(" does not exists.");
			::extend_warning(LOG_ERROR, err_msg.c_str());
			return false;
		}

		xml_document<> doc;

		doc.parse<rapidxml::parse_default>(pfilebuff);

		xml_node<>* pNodeRoot = doc.first_node("object");

		if (pNodeRoot == NULL)
		{
			std::string err_msg = file_path;
			err_msg.append(" invalid npc creator file.");
			::extend_warning(LOG_ERROR, err_msg.c_str());
			return false;
		}

		xml_node<>* pNode = pNodeRoot->first_node("randomcreator");
		if (pNode == NULL)
		{
			std::string err_msg = file_path;
			err_msg.append(" invalid npc creator file.");
			::extend_warning(LOG_ERROR, err_msg.c_str());
			return false;
		}

		xml_node<>* pCreator = pNode->first_node("creator");
		// �ӵ�ǰpCreator����ʼ����
		LoopBeginCheck(a);
		while (pCreator != NULL)
		{
			CreateSpringNpcInfo createSpringNpcInfo;

			LoopDoCheck(a);
			// ��ʼ������źͷ���id
			int nSubSceneNo = -1;
			int nGroupId = -1;

			// ����
			createSpringNpcInfo.m_springPosition.m_x = convert_float(QueryXmlAttr(pCreator, "x"));
			createSpringNpcInfo.m_springPosition.m_y = convert_float(QueryXmlAttr(pCreator, "y"));
			createSpringNpcInfo.m_springPosition.m_z = convert_float(QueryXmlAttr(pCreator, "z"));

			// �⻷
			createSpringNpcInfo.m_maxScope = convert_float(QueryXmlAttr(pCreator, "MaxScope"), 30);
			//�ڻ�
			createSpringNpcInfo.m_minScope = convert_float(QueryXmlAttr(pCreator, "MinScope"), 25);
			// �����뾶
			createSpringNpcInfo.m_springRange = convert_float(QueryXmlAttr(pCreator, "SpringRange"), 50.0f);
			//0-�����󴴽�NPC��1-���볡���ʹ���
			createSpringNpcInfo.m_createType = convert_int(QueryXmlAttr(pCreator, "createtype"), 0);

			//����Ĳ���
			createSpringNpcInfo.m_totalStep = convert_int(QueryXmlAttr(pCreator, "totalstep"), 0);
			//NPC�Ƿ����� 0-�� 1-��
			createSpringNpcInfo.m_isDestroy = convert_int(QueryXmlAttr(pCreator, "IsDestroy", 0)) > 0 ? true : false;
			//�ؿ��ؾ��н�����
			createSpringNpcInfo.m_sceneAction = convert_int(QueryXmlAttr(pCreator, "SceneAction", 0));

			// ���������
			const char *creator_no = QueryXmlAttr(pCreator, "No", "");
			createSpringNpcInfo.m_no = creator_no;

			int totalNpcNum = 0;
			createSpringNpcInfo.m_totalNpcNum = totalNpcNum;
			createSpringNpcInfo.m_totalNpcNum = ParseRandomNpcItem(pKernel, createSpringNpcInfo.m_NpcGroup, pCreator);

			// ��װ
			CVarList result;
			util_split_string(result, creator_no, "_");
			if (result.GetCount() > 1)
			{
				const int key = result.IntVal(1);
				m_TaskCreatorMap.insert(std::make_pair(key, createSpringNpcInfo));
			}

			// �õ���һ����������Ϣ
			pCreator = pCreator->next_sibling("creator");			
		}
		
		delete[] pfilebuff;

	}
	catch (parse_error& e)
	{
		::extend_warning(pKernel, "[Error]NpcCreatorModule::LoadTaskCreatorRes: ");
		::extend_warning(pKernel, file_path.c_str());
		::extend_warning(pKernel, e.what());

		return false;
	}

	return true;
}

// ����XML��������NPC
bool NpcCreatorModule::CreateCloneNpcFromXml(IKernel* pKernel, const PERSISTID& scene)
{
	if (pKernel->GetSceneClass() != 2)
	{
		//���Ǹ���
		return false;
	}

	// ������̬npc��������
	if (pKernel->FindHeartBeat(scene, "NpcCreatorModule::H_DelayCreateStaticNpc"))
	{
		pKernel->RemoveHeartBeat(scene, "NpcCreatorModule::H_DelayCreateStaticNpc");
	}

	pKernel->AddCountBeat(scene,"NpcCreatorModule::H_DelayCreateStaticNpc", 300, 5);

	// ��ʼ�����������������
	if (pKernel->FindHeartBeat(scene, "NpcCreatorModule::H_DelayCreateRandomNpc"))
	{
		pKernel->RemoveHeartBeat(scene, "NpcCreatorModule::H_DelayCreateRandomNpc");
	}

	pKernel->AddCountBeat(scene,"NpcCreatorModule::H_DelayCreateRandomNpc", 300,5);


	return true;
}

// Ϊ���鳡������NPC
bool NpcCreatorModule::CreateGroupNpcFromXml(IKernel* pKernel, const PERSISTID& scene,
	int nSubSceneNo, int nGroupId, const char* npcCfgName)
{
    IGameObj* pSceneObj = pKernel->GetGameObj(scene);
    if (NULL == pSceneObj)
    {
        return false;
    }
	
	
	AddSceneGroupNpcCfgName(nSubSceneNo, nGroupId, npcCfgName);
	IRecord* staticRec = pSceneObj->GetRecord(GROUP_STATIC_CREATE_FLOW);
    if (staticRec != NULL)
    {
		staticRec->AddRowValue(-1, CVarList() << nSubSceneNo << nGroupId << 0 << 0 );
    }
    IRecord* randomRec = pSceneObj->GetRecord(GROUP_RANDOM_CREATE_FLOW);
    if (randomRec != NULL)
    {
		randomRec->AddRowValue(-1, CVarList() << nSubSceneNo << nGroupId << 0 << 0 );
    }
	
    //��̬NPC����������
    if (pKernel->FindHeartBeat(scene, "NpcCreatorModule::H_GroupSceneCreateStaticNpc"))
    {
        pKernel->RemoveHeartBeat(scene, "NpcCreatorModule::H_GroupSceneCreateStaticNpc");
    }
    pKernel->AddCountBeat(scene,"NpcCreatorModule::H_GroupSceneCreateStaticNpc", 300, 5);
	

	// ��ʼ�����������������
	if (pKernel->FindHeartBeat(scene, "NpcCreatorModule::H_GroupSceneCreateRandomNpc"))
	{
		pKernel->RemoveHeartBeat(scene, "NpcCreatorModule::H_GroupSceneCreateRandomNpc");
	}
	pKernel->AddCountBeat(scene,"NpcCreatorModule::H_GroupSceneCreateRandomNpc", 300, 5);

	CreateRandomNpcByTime(pKernel, scene, nSubSceneNo, nGroupId);

	CreateWheelNpc(pKernel, nSubSceneNo, nGroupId);

	return true;
}


void NpcCreatorModule::AddSceneGroupNpcCfgName(int sceneId, int groupId, const std::string& npcCfgName)
{
	auto it = m_sceneGroupNpcCfgName.find(sceneId);
	if (it != m_sceneGroupNpcCfgName.end())
	{
		auto its = it->second.find(groupId);
		if (its != it->second.end())
		{
			it->second.erase(its);
		}
		it->second[groupId] = npcCfgName;
	}
	else
	{
		m_sceneGroupNpcCfgName[sceneId][groupId] = npcCfgName;
	}

	
};


const char* NpcCreatorModule::GetSceneGroupNpcCfgName(int sceneID, int groupID)
{
	auto it = m_sceneGroupNpcCfgName.find(sceneID);
	if (it == m_sceneGroupNpcCfgName.end()){
		return "";
	}
	auto its = it->second.find(groupID);
	if (its == it->second.end()){
		return "";
	}
	return its->second.c_str();

}
void NpcCreatorModule::CreateWheelNpc(IKernel*pKernel, int sceneId, int groupId, int killNumNow)
{
	const char* npcCfgName = GetSceneGroupNpcCfgName(sceneId, groupId);
	auto it = m_wheel_npc.find(npcCfgName);
	if (it != m_wheel_npc.end())
	{
		auto its = it->second.find(killNumNow);
		if (its != it->second.end())
		{
			std::vector<WheelNpc>::iterator itt = its->second.begin();
			LoopBeginCheck(a);
			for (; itt != its->second.end(); ++itt)
			{
				LoopDoCheck(a);
				float x = itt->_x;
				float y = itt->_y;
				float z = itt->_z;
				int amount = itt->_mount;
				int needKillNum = itt->_needKillNum;
				// �õ�ʵ�ʸ߶�
				y = GetCurFloorHeight(pKernel, x, y, z);
				
				LoopBeginCheck(b);
				for (int i = 0; i < amount; i++)
				{
					LoopDoCheck(b);
					// ����������NPC
					PERSISTID npc_obj = pKernel->CreateObjectArgs("", itt->_id.c_str(), 0, x, y, z, itt->_ay, CVarList() << CREATE_TYPE_PROPERTY_VALUE << "GroupID" << groupId);
					if (!pKernel->Exists(npc_obj))
					{
						continue;
					}

					if (needKillNum != 0)
					{
						IGameObj *pNpc = pKernel->GetGameObj(npc_obj);
						if (pNpc != NULL)
						{
							pNpc->AddDataInt("NeedKillNum", needKillNum);
							FightInterfaceInstance->AddBuffer(pKernel, npc_obj, npc_obj, CONTROL_BUFFID);
						}

					}
				}
			}
		}
	}
}






// ����creator����id�Ķ�Ӧ��ϵ
bool NpcCreatorModule::RegisterRandomCreator(IKernel* pKernel, const PERSISTID& scene, 
											 const char* id, const PERSISTID& creator)
{
    IGameObj* pSceneObj = pKernel->GetGameObj(scene);
    if (NULL == pSceneObj)
    {
        return false;
    }
	if (!id || !pKernel->Exists(scene) || !pKernel->Exists(creator))
	{
		return false;
	}
    IRecord* creatorRec = pSceneObj->GetRecord("creator_id_rec");
    if (creatorRec == NULL)
    {
        return false;
    }
	int row = creatorRec->FindString(0, id);
	if (row >= 0)
	{
		return true;
	}

    return creatorRec->AddRowValue(-1, CVarList() << id << creator) >= 0;
}

void NpcCreatorModule::ParseSceneNpc(IKernel* pKernel, std::string sceneCfgName, const std::vector<xml_document<>*>& tempDocVec)
{
	std::vector<CreateSceneNpcInfo> m_crateFileList;
	int iDocCount = (int)tempDocVec.size();
	LoopBeginCheck(a);
	for (int fileIndex = 0; fileIndex < iDocCount; fileIndex++)
	{
		LoopDoCheck(a);
		xml_node<>* pNodeRoot = tempDocVec[fileIndex]->first_node("object");
		if (pNodeRoot == NULL)
			continue;
		CreateSceneNpcInfo CreateSceneNpcInfo;
		// �����������������
		

		ParseRandomNpcCreate(pKernel, CreateSceneNpcInfo.m_createRandGroup, pNodeRoot);
		ParseStaticNpcCreate(pKernel, CreateSceneNpcInfo.m_createStaticGroup, pNodeRoot);


		m_crateFileList.push_back(CreateSceneNpcInfo);
	}

	m_group_npc_info[sceneCfgName] = m_crateFileList;


}

void NpcCreatorModule::ParseRandomNpcCreate(IKernel* pKernel, std::vector<CreateSpringNpcInfo>& springNpcGroup, xml_node<>*pNodeRoot)
{
	if (pNodeRoot == NULL){
		return;
	}

	xml_node<>* pNode = pNodeRoot->first_node("randomcreator");
	if (pNode == NULL){
		return;
	}
	
	 xml_node<>* pCreator = pNode->first_node("creator");
	// �ӵ�ǰpCreator����ʼ����
	LoopBeginCheck(check_z);
	while (pCreator != NULL)
	{
		CreateSpringNpcInfo createSpringNpcInfo;

		LoopDoCheck(check_z);
		// ��ʼ������źͷ���id
		int nSubSceneNo = -1;
		int nGroupId = -1;

		// ����
		float x = convert_float(QueryXmlAttr(pCreator, "x"));
		float y = convert_float(QueryXmlAttr(pCreator, "y"));
		float z = convert_float(QueryXmlAttr(pCreator, "z"));

		// �õ�ʵ�ʸ߶�
		//y = GetCurFloorHeight(pKernel, x, y, z);
		createSpringNpcInfo.m_springPosition.m_x = x;
		createSpringNpcInfo.m_springPosition.m_y = y;
		createSpringNpcInfo.m_springPosition.m_z = z;
		// �⻷
		createSpringNpcInfo.m_maxScope = convert_float(QueryXmlAttr(pCreator, "MaxScope"), 30);
		//�ڻ�
		createSpringNpcInfo.m_minScope = convert_float(QueryXmlAttr(pCreator, "MinScope"), 25);
		// �����뾶
		createSpringNpcInfo.m_springRange = convert_float(QueryXmlAttr(pCreator, "SpringRange"), 50.0f);
		//0-�����󴴽�NPC��1-���볡���ʹ���
		createSpringNpcInfo.m_createType = convert_int(QueryXmlAttr(pCreator, "createtype"), 0);
		//����Ĳ���
		createSpringNpcInfo.m_totalStep = convert_int(QueryXmlAttr(pCreator, "totalstep"), 0);

		//NPC�Ƿ����� 0-�� 1-��
		createSpringNpcInfo.m_isDestroy =convert_int(QueryXmlAttr(pCreator, "IsDestroy", 0)) > 0 ?true:false;
		//�ؿ��ؾ��н�����
		createSpringNpcInfo.m_sceneAction = convert_int(QueryXmlAttr(pCreator, "SceneAction", 0));
		// ���������
		createSpringNpcInfo.m_no = QueryXmlAttr(pCreator, "No");
		int totalNpcNum = 0;
		createSpringNpcInfo.m_totalNpcNum = totalNpcNum;
		createSpringNpcInfo.m_totalNpcNum =  ParseRandomNpcItem(pKernel, createSpringNpcInfo.m_NpcGroup, pCreator);
		// �õ���һ����������Ϣ
		pCreator = pCreator->next_sibling("creator");
		springNpcGroup.push_back(createSpringNpcInfo);
	}
	
}


void NpcCreatorModule::ParseStaticNpcCreate(IKernel* pKernel, std::vector<CreateStaticNpcInfo>& staticNpcGroup, xml_node<>*pCreator)
{
	if (pCreator == NULL)
	{
		return;
	}
	
	// ������̬����������
	xml_node<>* pNode = pCreator->first_node("staticcreator");

	if (NULL != pNode)
	{
		CreateStaticNpcInfo staticNpcInfo;
		xml_node<>* pItem = pNode->first_node("item");

		LoopBeginCheck(check_dd);
		while (pItem != NULL)
		{
			LoopDoCheck(check_dd);
			// NPC���
			staticNpcInfo.m_npcID = QueryXmlAttr(pItem, "id");
			if (staticNpcInfo.m_npcID.empty()){
				continue;
			}
			NpcPosition positoin;
				// ����
			staticNpcInfo.m_position.m_x = convert_float(QueryXmlAttr(pItem, "x"));
			staticNpcInfo.m_position.m_y = convert_float(QueryXmlAttr(pItem, "y"));
			staticNpcInfo.m_position.m_z = convert_float(QueryXmlAttr(pItem, "z"));
			// ģ����ת
			const char* ay = QueryXmlAttr(pItem, "ay", "0.0");
			// �����ʹ�ýǶ�
			staticNpcInfo.m_position.m_ay = (float)::atof(ay);
			staticNpcInfo.m_groupID = StringUtil::StringAsInt(QueryXmlAttr(pItem, "Group"), -1);


			// ��Ӫ
			staticNpcInfo.m_nation = StringUtil::StringAsInt(QueryXmlAttr(pItem, "Nation"));
			staticNpcGroup.push_back(staticNpcInfo);
			// �õ���һ��npc������Ϣ
			pItem = pItem->next_sibling("item");

		}
	}

}

int NpcCreatorModule::ParseRandomNpcItem(IKernel* pKernel, std::vector<RandomCreator>& createSpringNpcInfoGroup, xml_node<>*pCreator)
{
	if (pCreator == NULL){
		return 0;
	}
	int totalNpcNum = 0;
	xml_node<>* pItem = pCreator->first_node("item");
	LoopBeginCheck(check_q);
	while (pItem != NULL)
	{
		LoopDoCheck(check_q);
		RandomCreator npcGroup;
		// NPC���
		npcGroup.m_npcID = QueryXmlAttr(pItem, "id");
		// ������ɵ�����
		npcGroup.m_createNum = convert_int(QueryXmlAttr(pItem, "amount"));
		totalNpcNum = totalNpcNum + npcGroup.m_createNum;
		// ˢ��ʱ��
		npcGroup.m_refreshTime = convert_int(QueryXmlAttr(pItem, "refreshtime"));
		npcGroup.m_step = convert_int(QueryXmlAttr(pItem, "step"));
		npcGroup.m_limit = convert_int(QueryXmlAttr(pItem, "limit"));
		
		// λ�ñ����ݶ�ȡ
		ParseRandomNpcPosition(pKernel, npcGroup.m_positionGroup, pItem);
		pItem = pItem->next_sibling("item");
		createSpringNpcInfoGroup.push_back(npcGroup);
	}
	return totalNpcNum;
}

void NpcCreatorModule::ParseRandomNpcPosition(IKernel* pKernel, std::vector<NpcPosition> & positionGroup, xml_node<>*pItem)
{
	if (pItem == NULL){
		return;
	}
	xml_node<>* pPosition = pItem->first_node("position");
	if (pPosition != NULL)
	{
		NpcPosition position;
		// ʹ�ù̶�λ����Ϣ
		LoopBeginCheck(check_r);
		while (pPosition != NULL)
		{
			LoopDoCheck(check_r);
			float x1, y1, z1;

			// λ��
			x1 = convert_float(QueryXmlAttr(pPosition, "x"));
			z1 = convert_float(QueryXmlAttr(pPosition, "z"));
			y1 = convert_float(QueryXmlAttr(pPosition, "y"));
			// ȡ������Ĳ�߶�
			//y1 = GetCurFloorHeight(pKernel, x1, y1, z1);

			float o1 = convert_float(QueryXmlAttr(pPosition, "ay"));
			position.m_x = x1;
			position.m_y = y1;
			position.m_z = z1;
			position.m_ay = o1;
			pPosition = pPosition->next_sibling("position");
			positionGroup.push_back(position);
		}
	}
}

bool NpcCreatorModule::CreateARandomCreator(IKernel* pKernel, const PERSISTID& scene, CreateSpringNpcInfo&pCreate, int nSubSceneNo, int nGroupId)
{
	
    IGameObj* pSceneObj = pKernel->GetGameObj(scene);
    if (pSceneObj == NULL)
    {
        return false;
    }
	
	if (pSceneObj->QueryInt("Grouping") >= 1)
	{
		int sceneID = pKernel->GetSceneId();

// 		int sceneType = GroupSceneBase::GetSceneType(pKernel, sceneID);
// 		if (nGroupId == -1)
// 		{
// 			
//             if ((sceneType != GROUP_SCENE_NOT_SECRET && sceneType != GROUP_SCENE_SECRET_PET) ||
// 				(sceneID >= GUIDE_SCENEID_MIN && sceneID <= GUIDE_SCENEID_MAX))
// 			{
// 				//�ؾ��������������������������ڴ˴���NPC������
// 				return true;
// 			}
// 		}
	}

	// ����һ��������
	PERSISTID creator = pKernel->CreateTo(scene, "RandomNpcCreator", pCreate.m_springPosition.m_x, pCreate.m_springPosition.m_y, pCreate.m_springPosition.m_z, 0.0f);
    IGameObj* pCreatorObj = pKernel->GetGameObj(creator);
    if (pCreatorObj == NULL){
        return false;
    }
	if (pKernel->Exists(creator))
	{
		// ע��������
		if (!pCreate.m_no.empty())
		{
			RegisterRandomCreator(pKernel, scene, pCreate.m_no.c_str(), creator);
		}

		// ��������
		pCreatorObj->SetFloat("MaxScope", pCreate.m_maxScope);
		pCreatorObj->SetFloat("MinScope", pCreate.m_minScope);
		pCreatorObj->SetFloat("SpringRange", pCreate.m_springRange);

		pCreatorObj->SetInt("NoSpring", 0);
		pCreatorObj->SetInt("Springer", 0);

		pCreatorObj->SetInt("Invisible", 1);
		pCreatorObj->SetInt("CreateType", pCreate.m_createType);
		pCreatorObj->SetInt("TotalStep", pCreate.m_totalStep);
		pCreatorObj->SetInt("IsDestroy", pCreate.m_isDestroy);
		pCreatorObj->SetInt("SceneAction", pCreate.m_sceneAction);
		if (pCreate.m_totalStep > 1)
		{
			pCreatorObj->SetInt("CurStep", 1);
		}
		//�������ϵĹ�������
		int create_amount = 0;

		// ��Ҫ������ɵ�NPCλ��������
		int pos_count = 0;

		for (auto it : pCreate.m_NpcGroup)
		{
			PERSISTID creator_item = pKernel->Create(creator, "RandomNpcCreatorItem");
			if (!pKernel->Exists(creator_item)){
				continue;
			}

			IGameObj* pItemObj = pKernel->GetGameObj(creator_item);
			if (pItemObj == NULL){
				continue;
			}

			IRecord* positionRec = pItemObj->GetRecord(POSITION_INFO_REC);
			if (positionRec == NULL){
				continue;
			}

			pItemObj->SetString("NpcID", it.m_npcID.c_str());
			pItemObj->SetInt("MaxAmount", it.m_createNum);
			pItemObj->SetInt("RefreshPeriod",it.m_refreshTime);
			pItemObj->SetInt("Step", it.m_step);
			pItemObj->SetInt("Limit", it.m_limit);
			pItemObj->SetInt("GroupID", nGroupId);

			
			if (!it.m_positionGroup.empty())
			{
				// ʹ�ù̶�λ����Ϣ
				for (auto position : it.m_positionGroup){
					 
					float y1 = GetCurFloorHeight(pKernel, position.m_x, position.m_y, position.m_z);
					positionRec->AddRowValue(-1, CVarList() << position.m_x << y1 << position.m_z << position.m_ay << 0 << (__int64)0 << -1.0f);
				}
			}
			else
			{
				if (it.m_createNum > 0 && pCreate.m_createType == 0)
				{
					LoopBeginCheck(check_s);
					for (int j = 0; j < it.m_createNum; j++)
					{
						LoopDoCheck(check_s);
						float x1, y1, z1, o1;
						float orient = util_random_float(PI2);
						// ���ȡ��Χ�ĵ�,��ֹ��ѭ��
						LoopBeginCheck(check_t);
						for (int k = 0; k < 8; ++k)
						{
							LoopDoCheck(check_t);
							x1 = pCreate.m_springPosition.m_x +(pCreate.m_maxScope + util_random_float(pCreate.m_maxScope - pCreate.m_minScope)) * ::sinf(orient);
							z1 = pCreate.m_springPosition.m_z +(pCreate.m_maxScope + util_random_float(pCreate.m_maxScope - pCreate.m_minScope)) * ::cosf(orient);
							y1 = pKernel->GetMapHeight(x1, z1);
							if (pKernel->CanWalk(x1, z1))
							{
								break;
							}
						}
						//��������������ĵ㻹�Ǵ�������
						if (!pKernel->CanWalk(x1, z1))
						{
							x1 = pCreate.m_springPosition.m_x;
							z1 = pCreate.m_springPosition.m_z;
						}

						o1 = PI2 * ::util_random_float(1.0f);
						// ��ӵ�λ�ñ��
						positionRec->AddRowValue(-1,
							CVarList() << x1 << y1 << z1 << o1 << 0 << (__int64)0 << -1.0f);
					}
				}
			}
		
		}

		pCreatorObj->SetInt("MonsterAmount", pCreate.m_totalNpcNum);

		//����Ǵ���0���Ƚ�������趨��
		if (pCreate.m_createType > 0)
		{
			PreSetCreateCenterPos(pKernel, creator);
		}

		// �Ƿ��鳡���Ļ��������
		if (pSceneObj->QueryInt("Grouping") >= 1)
		{
			pCreatorObj->SetInt("GroupID", nGroupId);
		}

		if (pCreate.m_createType > 0)
		{
			BeginRefresh(pKernel, creator);
		}
	}

	return true;
}

// ��������ˢ����
const PERSISTID NpcCreatorModule::CreateTaskrCreator(IKernel* pKernel, const PERSISTID& scene,
	const int task_id, const int group_id, const int level)
{
	IGameObj* pSceneObj = pKernel->GetGameObj(scene);
	if (pSceneObj == NULL)
	{
		return PERSISTID();
	}

	// ����ˢ����
	std::map<int, CreateSpringNpcInfo>::const_iterator find_it = m_TaskCreatorMap.find(task_id);
	if (find_it == m_TaskCreatorMap.end())
	{
		return PERSISTID();
	}

	const CreateSpringNpcInfo &pCreate = find_it->second;
	
	// ����һ��������
	PERSISTID creator = pKernel->CreateTo(scene, "RandomNpcCreator", pCreate.m_springPosition.m_x, pCreate.m_springPosition.m_y, pCreate.m_springPosition.m_z, 0.0f);
	IGameObj* pCreatorObj = pKernel->GetGameObj(creator);
	if (pCreatorObj == NULL)
	{
		return PERSISTID();
	}
	
	// ע��������
	if (!pCreate.m_no.empty())
	{
		RegisterRandomCreator(pKernel, scene, pCreate.m_no.c_str(), creator);
	}

	// ��ʱ����
	ADD_DATA_INT(pCreatorObj, "CreatorLevel");
	pCreatorObj->SetDataInt("CreatorLevel", level);

	// ��������
	pCreatorObj->SetFloat("MaxScope", pCreate.m_maxScope);
	pCreatorObj->SetFloat("MinScope", pCreate.m_minScope);
	pCreatorObj->SetFloat("SpringRange", pCreate.m_springRange);

	pCreatorObj->SetInt("NoSpring", 0);
	pCreatorObj->SetInt("Springer", 0);

	pCreatorObj->SetInt("Invisible", 1);
	pCreatorObj->SetInt("CreateType", pCreate.m_createType);
	pCreatorObj->SetInt("TotalStep", pCreate.m_totalStep);
	pCreatorObj->SetInt("IsDestroy", pCreate.m_isDestroy);
	pCreatorObj->SetInt("SceneAction", pCreate.m_sceneAction);
	if (pCreate.m_totalStep > 1)
	{
		pCreatorObj->SetInt("CurStep", 1);
	}
	//�������ϵĹ�������
	int create_amount = 0;

	// ��Ҫ������ɵ�NPCλ��������
	int pos_count = 0;

	for (auto it : pCreate.m_NpcGroup)
	{
		PERSISTID creator_item = pKernel->Create(creator, "RandomNpcCreatorItem");
		if (!pKernel->Exists(creator_item))
		{
			continue;
		}

		IGameObj* pItemObj = pKernel->GetGameObj(creator_item);
		if (pItemObj == NULL)
		{
			continue;
		}

		IRecord* positionRec = pItemObj->GetRecord(POSITION_INFO_REC);
		if (positionRec == NULL)
		{
			continue;
		}

		pItemObj->SetString("NpcID", it.m_npcID.c_str());
		pItemObj->SetInt("MaxAmount", it.m_createNum);
		pItemObj->SetInt("RefreshPeriod", it.m_refreshTime);
		pItemObj->SetInt("Step", it.m_step);
		pItemObj->SetInt("Limit", it.m_limit);
		pItemObj->SetInt("GroupID", group_id);


		if (!it.m_positionGroup.empty())
		{
			// ʹ�ù̶�λ����Ϣ
			for (auto position : it.m_positionGroup)
			{

				float y1 = GetCurFloorHeight(pKernel, position.m_x, position.m_y, position.m_z);
				positionRec->AddRowValue(-1, CVarList() << position.m_x << y1 << position.m_z << position.m_ay << 0 << (__int64)0 << -1.0f);
			}
		}
		else
		{
			if (it.m_createNum > 0 && pCreate.m_createType == 0)
			{
				LoopBeginCheck(check_s);
				for (int j = 0; j < it.m_createNum; j++)
				{
					LoopDoCheck(check_s);
					float x1, y1, z1, o1;
					float orient = util_random_float(PI2);
					// ���ȡ��Χ�ĵ�,��ֹ��ѭ��
					LoopBeginCheck(check_t);
					for (int k = 0; k < 8; ++k)
					{
						LoopDoCheck(check_t);
						x1 = pCreate.m_springPosition.m_x + (pCreate.m_maxScope + util_random_float(pCreate.m_maxScope - pCreate.m_minScope)) * ::sinf(orient);
						z1 = pCreate.m_springPosition.m_z + (pCreate.m_maxScope + util_random_float(pCreate.m_maxScope - pCreate.m_minScope)) * ::cosf(orient);
						y1 = pKernel->GetMapHeight(x1, z1);
						if (pKernel->CanWalk(x1, z1))
						{
							break;
						}
					}
					//��������������ĵ㻹�Ǵ�������
					if (!pKernel->CanWalk(x1, z1))
					{
						x1 = pCreate.m_springPosition.m_x;
						z1 = pCreate.m_springPosition.m_z;
					}

					o1 = PI2 * ::util_random_float(1.0f);
					// ��ӵ�λ�ñ��
					positionRec->AddRowValue(-1,
						CVarList() << x1 << y1 << z1 << o1 << 0 << (__int64)0 << -1.0f);
				}
			}
		}

	}

	pCreatorObj->SetInt("MonsterAmount", pCreate.m_totalNpcNum);

	//����Ǵ���0���Ƚ�������趨��
	if (pCreate.m_createType > 0)
	{
		PreSetCreateCenterPos(pKernel, creator);
	}

	// �Ƿ��鳡���Ļ��������
	if (pSceneObj->QueryInt("Grouping") >= 1)
	{
		pCreatorObj->SetInt("GroupID", group_id);
	}

	if (pCreate.m_createType > 0)
	{
		BeginRefresh(pKernel, creator);
	}

	return creator;
}

// �ֲ�������̬npc������
int NpcCreatorModule::H_DelayCreateStaticNpc(IKernel* pKernel, const PERSISTID& creator, int slice)
{

	//// ��¼������ʼʱ��
	//unsigned long begin_time = ::util_get_ticks();

	//// �õ�������������
	//PERSISTID scene = creator;

	//// ����
	//if (!pKernel->Exists(scene))
	//{
	//	return 0;
	//}
 //   IGameObj* pSceneObj = pKernel->GetGameObj(scene);
 //   if (pSceneObj == NULL)
 //   {
 //       return 0;
 //   }

	//int iCloneLevel = 0;

	//if (pSceneObj->FindAttr("Level"))
	//{
	//	iCloneLevel = pSceneObj->QueryInt("Level");
	//}

	//std::vector<xml_document<>*>* cfgXml = NULL;

	//if (pKernel->GetSceneClass() == 2)
	//{
 //       //����������ʹ��ԭ�ͳ�����������Ϣ
	//	// ����id
	//	int nCloneSceneID = pKernel->GetSceneId();

	//	// ����ԭ��id
	//	int nPrototypeSceneID = pKernel->GetPrototypeSceneId(nCloneSceneID);
	//
	//	// �õ�npc���������ļ�
	//	auto it = m_pNpcCreatorModule->m_clone_npc_info.find(nPrototypeSceneID);

	//	if (it == m_pNpcCreatorModule->m_clone_npc_info.end())
	//	{
	//		return 0;
	//	}
	//	cfgXml = &(it->second);
	//}
	//else
	//{
	//	// ����id
	//	int nSceneID = pKernel->GetSceneId();
	//	std::string npcCfgName = StringUtil::IntAsString(nSceneID);
	//	// ֱ��ͨ�����鳡��id�õ�npc�����ļ�
	//	auto it = m_pNpcCreatorModule->m_group_npc_info.find(npcCfgName);

	//	if (it == m_pNpcCreatorModule->m_group_npc_info.end())
	//	{
	//		return 0;
	//	}
	//	cfgXml = &(it->second);
	//}
	//
	//if (cfgXml == NULL){
	//	return 0;
	//}

	//std::vector<xml_document<>*>& DocVec = *cfgXml;

	//int nCount = int(DocVec.size());

	//int i = 0;

	//// �õ���ǰ���ڴ������ļ�index�����û�У�����Ϊ��0��ʼ
	//if (pSceneObj->FindData("create_static_npc_file_count"))
	//{
	//	i = pSceneObj->QueryDataInt("create_static_npc_file_count");
	//}

	//// �����ļ�
 //   LoopBeginCheck(check_u);
	//for (;i < nCount;)
	//{
 //       LoopDoCheck(check_u);
	//	xml_node<>* pNodeRoot = DocVec[i]->first_node("object");

	//	if (pNodeRoot == NULL)
	//	{
	//		return 0;
	//	}

	//	// ������̬����������
	//	xml_node<>* pNode = pNodeRoot->first_node("staticcreator");

	//	if (NULL != pNode)
	//	{
	//		xml_node<>*  pItem =  NULL;

	//		int static_npc_index = 0;

	//		if (pSceneObj->FindData("create_static_npc_item_index") )
	//		{	
	//			// �õ���ǰ���ڴ�����npc��index
	//			static_npc_index = pSceneObj->QueryDataInt("create_static_npc_item_index");

 //               pNode = pNode->first_node("item");
 //               LoopBeginCheck(check_v);
 //               for(int j = 0; j < static_npc_index; ++j)
	//			{
 //                   LoopDoCheck(check_v);
	//				pNode = pNode->next_sibling("item");
	//			}

	//			pItem = pNode;

	//		}
	//		else
	//		{
	//			pItem = pNode->first_node("item");
	//		}

 //           LoopBeginCheck(check_w);
	//		while (pItem != NULL)
	//		{
 //               LoopDoCheck(check_w);
	//			// ��ʼ������źͷ���id
	//			int nSubSceneNo = -1;
	//			int nGroupId = -1;

	//			if (pSceneObj->FindData("Create_SubSceneNo"))
	//			{
	//				nSubSceneNo = pSceneObj->QueryDataInt("Create_SubSceneNo");
	//			}

	//			if (pSceneObj->FindData("Create_GroupId"))
	//			{
	//				nGroupId = pSceneObj->QueryDataInt("Create_GroupId");
	//			}

	//			// ����npc
	//			m_pNpcCreatorModule->CreateStaticNpcUnit(pKernel, scene, pItem, nSubSceneNo, nGroupId);

	//			// �õ���һ��npc������Ϣ
	//			pItem = pItem->next_sibling("item");

	//			// npc������ŵ���
	//			static_npc_index++;

	//			// �õ���ǰϵͳʱ��
	//			unsigned long cur_time = ::util_get_ticks();

	//			// ��ǰ����ִ��ʱ�䳬�����ʱ�䣬������������
	//			if ((cur_time - begin_time) > MAX_HERAT_BEAT_TIME)
	//			{
	//				// �������õ�ǰ�����Ĵ���
	//				pKernel->SetBeatCount(scene,"NpcCreatorModule::H_DelayCreateStaticNpc", 5);

	//				// �������ڴ�����npc file���
	//				if (!pSceneObj->FindData("create_static_npc_file_count"))
	//				{
	//					pSceneObj->AddDataInt("create_static_npc_file_count", 0);
	//				}

	//				pSceneObj->SetDataInt("create_static_npc_file_count", i);

	//				// �������ڴ�����npc item��index
	//				if (!pSceneObj->FindData("create_static_npc_item_index"))
	//				{
	//					pSceneObj->AddDataInt("create_static_npc_item_index", 0);
	//				}

	//				pSceneObj->SetDataInt("create_static_npc_item_index", static_npc_index);	

	//				return 1;
	//			}
	//		}
	//	} 

	//	// �ļ�No������
	//	i++;

	//	// һ��xml�еľ�̬npc�����Ѿ�������ϣ����npc����
	//	pSceneObj->RemoveData("create_static_npc_item_index");

	//	// �������ڴ�����npc file���
	//	if (!pSceneObj->FindData("create_static_npc_file_count"))
	//	{
	//		pSceneObj->AddDataInt("create_static_npc_file_count", 0);
	//	}
	//	pSceneObj->SetDataInt("create_static_npc_file_count", i);
	//}
	//	
	return 1;
}

// �ֲ��������npc������
int NpcCreatorModule::H_DelayCreateRandomNpc(IKernel* pKernel, const PERSISTID& creator, int slice)
{
	//// ��¼������ʼʱ��
	//unsigned long begin_time = ::util_get_ticks();

	//// �õ�������������
	//PERSISTID scene = creator;

	//// ����
	//if (!pKernel->Exists(scene))
	//{
	//	return 0;
	//}
 //   IGameObj* pSceneObj = pKernel->GetGameObj(scene);
 //   if (pSceneObj == NULL)
 //   {
 //       return 0;
 //   }

	//std::vector<xml_document<>*> *cfgXml = NULL;

	//if (pKernel->GetSceneClass() == 2)
	//{
	//	// ������������

	//	int nCloneSceneID = pKernel->GetSceneId();

	//	int nPrototypeSceneID = pKernel->GetPrototypeSceneId(nCloneSceneID);

	//	auto it = m_pNpcCreatorModule->m_clone_npc_info.find(nPrototypeSceneID);

	//	if (it == m_pNpcCreatorModule->m_clone_npc_info.end())
	//	{
	//		return 0;
	//	}
	//	cfgXml = &(it->second);
	//}
	//else
	//{
	//	// ����Ĭ��Ϊ�Ƿ��鳡��
	//	int nSceneID = pKernel->GetSceneId();
	//	std::string npcCfgName = StringUtil::IntAsString(nSceneID);
	//	auto it = m_pNpcCreatorModule->m_group_npc_info.find(npcCfgName);
	//	if (it == m_pNpcCreatorModule->m_group_npc_info.end())
	//	{
	//		return false;
	//	}
	//	cfgXml = &(it->second);
	//}

	//std::vector<xml_document<>*>& DocVec = *cfgXml;

	//int nCount = int(DocVec.size());

	//int i = 0;

	//if (pSceneObj->FindData("create_random_npc_file_count"))
	//{
	//	i = pSceneObj->QueryDataInt("create_random_npc_file_count");
	//}

 //   LoopBeginCheck(check_x);
	//for (; i < nCount;)
	//{
 //       LoopDoCheck(check_x);
	//	extend_warning(pKernel,"NpcCreatorModule::H_DelayCreateRandomNpc");	

	//	xml_node<>* pNodeRoot = DocVec[i]->first_node("object");

	//	if (pNodeRoot == NULL)
	//	{
	//		return 0;
	//	}

	//	// �����������������
	//	xml_node<>* pNode = pNodeRoot->first_node("randomcreator");

	//	if (pNode != NULL)
	//	{
	//		xml_node<>* pCreator =  NULL;

	//		// ��ǰ��������indexֵ
	//		int random_creator_index = 0;

	//		if (pSceneObj->FindData("create_random_creator_index") )
	//		{
	//			// �õ���ǰ���ڴ�����npc��index
	//			random_creator_index = pSceneObj->QueryDataInt("create_random_creator_index");

	//			pNode = pNode->first_node("creator");
 //               LoopBeginCheck(check_y);
 //               for(int j = 0; j < random_creator_index; j++)
	//			{
 //                   LoopDoCheck(check_y);
	//				pNode = pNode->next_sibling("creator");
	//			}
	//			
	//			pCreator = pNode;

	//		}
	//		else
	//		{
	//			pCreator = pNode->first_node("creator");
	//		}

	//		// �ӵ�ǰpCreator����ʼ����
 //           LoopBeginCheck(check_z);
	//		while (pCreator != NULL)
	//		{
 //               LoopDoCheck(check_z);
	//			// ��ʼ������źͷ���id
	//			int nSubSceneNo = -1;
	//			int nGroupId = -1;

	//			// ����npc������
	//			m_pNpcCreatorModule->CreateARandomCreator(pKernel, scene, pCreator, nSubSceneNo, nGroupId);

	//			// �õ���һ����������Ϣ
	//			pCreator = pCreator->next_sibling("creator");

	//			// ����ֵ����
	//			random_creator_index++;

	//			unsigned long cur_time = ::util_get_ticks();

	//			// ��ǰ����ִ��ʱ�䳬�����ʱ�䣬������������
	//			if ((cur_time - begin_time) > MAX_HERAT_BEAT_TIME)
	//			{
	//				// �������õ�ǰ�����Ĵ���
	//				pKernel->SetBeatCount(scene,"NpcCreatorModule::H_DelayCreateRandomNpc", 5);

	//				// �������ڴ�����file���
	//				if (!pSceneObj->FindData("create_random_npc_file_count"))
	//				{
	//					pSceneObj->AddDataInt("create_random_npc_file_count", 0);
	//				}

	//				pSceneObj->SetDataInt("create_random_npc_file_count", i);

	//				// ��������ǰλ��
	//				if (!pSceneObj->FindData("create_random_creator_index"))
	//				{
	//					pSceneObj->AddDataInt("create_random_creator_index", 0);
	//				}

	//				pSceneObj->SetDataInt("create_random_creator_index", random_creator_index);	

	//				return 1;
	//			}
	//		}
	//	}

	//	// �ļ��������
	//	i ++;

	//	// һ���ļ��е�����������Ѿ�������ϣ�ɾ����ʱ����
	//	pSceneObj->RemoveData("create_random_creator_index");

	//	// �������ڴ�����file���
	//	if (!pSceneObj->FindData("create_random_npc_file_count"))
	//	{
	//		pSceneObj->AddDataInt("create_random_npc_file_count", 0);
	//	}
	//	pSceneObj->SetDataInt("create_random_npc_file_count", i);
	//}

	return 1;
}

// ���鳡���зֲ�������̬npc������
int NpcCreatorModule::H_GroupSceneCreateStaticNpc(IKernel* pKernel, const PERSISTID& creator, int slice)
{
	// ��¼������ʼʱ��
	unsigned long begin_time = ::util_get_ticks();

	// �õ�������������
	PERSISTID scene = creator;

	// ����
    IGameObj* pSceneObj = pKernel->GetGameObj(scene);
	if (!pKernel->Exists(scene) || NULL == pSceneObj)
	{
		return 0;
	}

	// ȡ�ñ�ָ��
	IRecord* pPrecord = pSceneObj->GetRecord(GROUP_STATIC_CREATE_FLOW);

	if (NULL == pPrecord)
	{
		return 0;
	}

	// ����id
	int nSceneID = pKernel->GetSceneId();
	
	// ֱ��ͨ�����鳡��id�õ�npc�����ļ�
	int rows = pPrecord->GetRows();

    LoopBeginCheck(check_aa);
	for (int n = 0; n < rows; n++)
	{
        LoopDoCheck(check_aa);
		int iSubSceneNo = pPrecord->QueryInt(n, CREATE_FLOW_SUB_SCENE_NO);
		int iGroupID = pPrecord->QueryInt(n, CREATE_FLOW_GROUP_ID);
		int iFileIndex = pPrecord->QueryInt(n, CREATE_FLOW_FILE_INDEX);
		int iItemIndex = pPrecord->QueryInt(n, CREATE_FLOW_ITEM_INDEX);
		// iGroupID �Ƿ�������
	/*	if (iGroupID <= 0)
		{
			continue;
		}*/
		const char* npcCfgName = m_pNpcCreatorModule->GetSceneGroupNpcCfgName(iSubSceneNo, iGroupID);
		auto it = m_pNpcCreatorModule->m_group_npc_info.find(npcCfgName);
		if (it == m_pNpcCreatorModule->m_group_npc_info.end())
		{
			continue;
		}

		
		auto& fileGroup = it->second;
		// �����ļ�
        LoopBeginCheck(check_bb);
		auto fileInfo = fileGroup.begin();
		std::advance(fileInfo, iFileIndex);
		for (; fileInfo != fileGroup.end(); fileInfo++)
		{
			auto item = (*fileInfo).m_createStaticGroup.begin();
			std::advance(item, iItemIndex);
			for (; item != (*fileInfo).m_createStaticGroup.end(); item++)
			{
					// ��ʼ������źͷ���id
					int nSubSceneNo = -1;
					int nGroupId = -1;

					// ����npc
					m_pNpcCreatorModule->CreateStaticNpcUnit(pKernel, scene, *item, iSubSceneNo, iGroupID);

					// npc������ŵ���
					iItemIndex++;

					// �õ���ǰϵͳʱ��
					unsigned long cur_time = ::util_get_ticks();

					// ��ǰ����ִ��ʱ�䳬�����ʱ�䣬������������
					if ((cur_time - begin_time) > MAX_HERAT_BEAT_TIME)
					{
						// �������õ�ǰ�����Ĵ���
						pKernel->SetBeatCount(scene, "NpcCreatorModule::H_GroupSceneCreateStaticNpc", 5);

						// �������ڴ�����npc file���
						pPrecord->SetInt(n, CREATE_FLOW_FILE_INDEX, iFileIndex);

						// �������ڴ�����npc item��index
						pPrecord->SetInt(n, CREATE_FLOW_ITEM_INDEX, iItemIndex);

						return 1;
					}
				}
			// �ļ�No������
			iFileIndex++;

			// һ��xml�еľ�̬npc�����Ѿ�������ϣ����npc����
			pPrecord->SetInt(n, CREATE_FLOW_ITEM_INDEX, 0);
			iItemIndex=0;

			// �������ڴ�����npc file���
			pPrecord->SetInt(n, CREATE_FLOW_FILE_INDEX, iFileIndex);
		}
	
		pPrecord->SetInt(n, CREATE_FLOW_GROUP_ID, 0);
		pPrecord->SetInt(n, CREATE_FLOW_SUB_SCENE_NO, 0);
	}

	int delete_index = -1;

    LoopBeginCheck(check_ee);
	while ((delete_index = pPrecord->FindInt(CREATE_FLOW_GROUP_ID,0)) != -1)
	{
        LoopDoCheck(check_ee);
		pPrecord->RemoveRow(delete_index);
	}

	return 1;
}

// ���鳡���зֲ�����random npc������
int NpcCreatorModule::H_GroupSceneCreateRandomNpc(IKernel* pKernel, const PERSISTID& creator, int slice)
{
	// ��¼������ʼʱ��
	unsigned long begin_time = ::util_get_ticks();

	// �õ�������������
	PERSISTID scene = creator;

	// ����
	if (!pKernel->Exists(scene))
	{
		return 0;
	}
    IGameObj* pSceneObj = pKernel->GetGameObj(scene);
    if (pSceneObj == NULL)
    {
        return 0;
    }

	// ȡ�ñ�ָ��
	IRecord* pPrecord = pSceneObj->GetRecord(GROUP_RANDOM_CREATE_FLOW);

	if (NULL == pPrecord)
	{
		return 0;
	}

	// ����Ĭ��Ϊ�Ƿ��鳡��
	int nSceneID = pKernel->GetSceneId();

	

	int rows = pPrecord->GetRows();

    LoopBeginCheck(check_ff);
	for (int n = 0; n < rows; n++)
	{
		LoopDoCheck(check_ff);
		int iSubSceneNo = pPrecord->QueryInt(n, CREATE_FLOW_SUB_SCENE_NO);
		int iGroupID = pPrecord->QueryInt(n, CREATE_FLOW_GROUP_ID);
		int iFileIndex = pPrecord->QueryInt(n, CREATE_FLOW_FILE_INDEX);
		UINT iCreateIndex = pPrecord->QueryInt(n, CREATE_FLOW_ITEM_INDEX);

		// iGroupID �Ƿ�������
		if (iGroupID <= 0)
		{
			continue;
		}
		const char* npcCfgName = m_pNpcCreatorModule->GetSceneGroupNpcCfgName(iSubSceneNo, iGroupID);

		auto it = m_pNpcCreatorModule->m_group_npc_info.find(npcCfgName);
		if (it == m_pNpcCreatorModule->m_group_npc_info.end())
		{
			continue;
		}

		auto& DocVec = it->second;

		int iDocCount = int(DocVec.size());


		// �����ļ�
		LoopBeginCheck(check_gg);
		for (; iFileIndex < iDocCount;)
		{
			LoopDoCheck(check_gg);
			for (; iCreateIndex < DocVec[iFileIndex].m_createRandGroup.size();)
			{
				auto its = DocVec[iFileIndex].m_createRandGroup[iCreateIndex];

				// ����npc������
				m_pNpcCreatorModule->CreateARandomCreator(pKernel, scene, its, iSubSceneNo, iGroupID);

				// ����ֵ����
				iCreateIndex++;

				unsigned long cur_time = ::util_get_ticks();

				// ��ǰ����ִ��ʱ�䳬�����ʱ�䣬������������
				if ((cur_time - begin_time) > MAX_HERAT_BEAT_TIME)
				{
					// �������õ�ǰ�����Ĵ���
					pKernel->SetBeatCount(scene, "NpcCreatorModule::H_GroupSceneCreateRandomNpc", 5);

					// �������ڴ�����npc file���
					pPrecord->SetInt(n, CREATE_FLOW_FILE_INDEX, iFileIndex);

					// �������ڴ�����npc item��index
					pPrecord->SetInt(n, CREATE_FLOW_ITEM_INDEX, iCreateIndex);

					return 1;
				}
			
			}

		// �ļ��������
		iFileIndex++;

		// һ���ļ��е�����������Ѿ�������ϣ�ɾ����ʱ����
		pPrecord->SetInt(n, CREATE_FLOW_ITEM_INDEX, 0);
		iCreateIndex = 0;

		// �������ڴ�����npc file���
		pPrecord->SetInt(n, CREATE_FLOW_FILE_INDEX, iFileIndex);
	}
		pPrecord->SetInt(n, CREATE_FLOW_GROUP_ID, 0);
		pPrecord->SetInt(n, CREATE_FLOW_SUB_SCENE_NO, 0);
	}

	int delete_index = -1;

    LoopBeginCheck(check_jj);
	while ((delete_index = pPrecord->FindInt(CREATE_FLOW_GROUP_ID,0)) != -1)
	{
        LoopDoCheck(check_jj);
		pPrecord->RemoveRow(delete_index);
	}

	return 1;
}

// int NpcCreatorModule::H_GroupCreateNpcByTime(IKernel*pKernel, const PERSISTID& creator, int slice)
// {
// 	IGameObj* pCreate = pKernel->GetGameObj(creator);
// 	if (pCreate == NULL)
// 	{
// 		return 0;
// 	}
// 	int	littleStepMax = pCreate->QueryInt("LittleStepMax");
// 	int littleStepNow = pCreate->QueryInt("LittlestepNow");
// 	int sceneID = pCreate->QueryInt("SceneId");
// 	int GroupId = pCreate->QueryInt("GroupID");
// 	DWORD GlobalStep = pCreate->QueryInt("GlobalStep");
// 	//�Ƿ�ʼ��һ��ˢ��npc
// 	bool IsNextGlobalStep = false;
// 	if (littleStepMax <= littleStepNow)
// 	{
// 		GlobalStep++;
// 		IsNextGlobalStep = true;
// 
// 	}
// 	const char* npcCfgName = m_pNpcCreatorModule->GetSceneGroupNpcCfgName(sceneID, GroupId);
// 	auto it = m_pNpcCreatorModule->m_refresh_npc_by_time.find(npcCfgName);
// 	if (it != m_pNpcCreatorModule->m_refresh_npc_by_time.end())
// 	{
// 		if (GlobalStep < it->second.size())
// 		{
// 			RefreshNpcRule& refreshNpc = it->second[GlobalStep];
// 			
// 
// 			if (IsNextGlobalStep)
// 			{
// 				pCreate->SetInt("LittlestepNow",0);
// 				pCreate->SetInt("GlobalStep", GlobalStep);
// 				pCreate->SetInt("LittleStepMax", refreshNpc._totalStep);
// 				if (pKernel->FindHeartBeat(creator, "NpcCreatorModule::H_GroupCreateNpcByTime"))
// 				{
// 					DELETE_HEART_BEAT(pKernel, creator, "NpcCreatorModule::H_GroupCreateNpcByTime");
// 				}
// 
// 				ADD_COUNT_BEAT(pKernel, creator, "NpcCreatorModule::H_GroupCreateNpcByTime", refreshNpc._waitTime, 1);
// 				
// 				if (GlobalStep == it->second.size() - 1)
// 				{
// 					CustomSysInfoByGroup(pKernel,
// 					GroupId,
// 					TIPSTYPE_EVENT_CHAT_BROAD_MESSAGE, SECRET_TIPS_LAST_STEP, CVarList());
// 				}
// 				return 0;
// 			}
// 			else
// 			{
// 				littleStepNow++;
// 				pCreate->SetInt("LittlestepNow", littleStepNow);
// 				if (pKernel->FindHeartBeat(creator, "NpcCreatorModule::H_GroupCreateNpcByTime"))
// 				{
// 					DELETE_HEART_BEAT(pKernel, creator, "NpcCreatorModule::H_GroupCreateNpcByTime");
// 				}
// 
// 				ADD_COUNT_BEAT(pKernel, creator, "NpcCreatorModule::H_GroupCreateNpcByTime", refreshNpc._refreshTime, 1);
// 				if (littleStepNow == 1)
// 				{
// 					if (GlobalStep + 1< it->second.size())
// 					{
// 						RefreshNpcRule& nextStep = it->second[GlobalStep + 1];
// 							CVarList msg;
// 							msg << SERVER_CUSTOMMSG_SECRET_SCENE << SECRET_SCENE_STEP_LIFE_TIMES << littleStepMax*refreshNpc._refreshTime + nextStep._waitTime;
// 						pKernel->CustomByGroup(GroupId, msg);
// 					}
// 					
// 					CVarList msg;
// 					msg << SERVER_CUSTOMMSG_SECRET_SCENE << SECRET_SCENE_LIFE_STEP_TIMES << (int)(it->second.size() - GlobalStep - 1);
// 					pKernel->CustomByGroup(GroupId,msg);
// 				}
// 
// 			}
// 
// 
// 			std::vector<NpcInfo>::iterator itt = refreshNpc._npcInfo.begin();
// 			LoopBeginCheck(a);
// 			for (; itt != refreshNpc._npcInfo.end(); ++itt)
// 			{
// 				LoopDoCheck(a);
// 				// ����
// 				float x = itt->_x;
// 				float y = itt->_y;
// 				float z = itt->_z;
// 				int amount = itt->_mount;
// 
// 				// �õ�ʵ�ʸ߶�
// 				y = GetCurFloorHeight(pKernel, x, y, z);
// 
// 				LoopBeginCheck(a);
// 				for (int i = 0; i < amount; i++) {
// 					LoopDoCheck(a);
// 					// ����������NPC
// 					PERSISTID npc_obj = pKernel->CreateObjectArgs("", itt->_id.c_str(), 0, x, y, z, itt->_ay, CVarList() << CREATE_TYPE_PROPERTY_VALUE << "GroupID" << GroupId);
// 					if (!pKernel->Exists(npc_obj))
// 					{
// 						continue;
// 					}
// 				}
// 			}
// 		}
// 	}
// 	return true;
// }

int NpcCreatorModule::C_OnCurStepChanged(IKernel* pKernel, const PERSISTID& self, const char* property, const IVar& old)
{
    BeginRefresh(pKernel, self);
    return 1;
}


void NpcCreatorModule::CleanSceneCfgName(IKernel*pKernel, int groupID)
{
	int sceneID = pKernel->GetSceneId();
	auto it = m_sceneGroupNpcCfgName.find(sceneID);
	if (it != m_sceneGroupNpcCfgName.end())
	{
		auto its = it->second.find(groupID);
		if (its != it->second.end())
		{
			it->second.erase(its);
		}
	}
}

//��¼ɱ��NPC����
void NpcCreatorModule::RecordKillNpc(IKernel* pKernel, const PERSISTID& npc)
{
    IGameObj* pNpcObj = pKernel->GetGameObj(npc);
    if (pNpcObj == NULL)
    {
        return;
    }
    if (!pNpcObj->FindData("Creator") || !pNpcObj->FindData("CreatorItem"))
    {
        return;
    }
    PERSISTID creator = pNpcObj->QueryDataObject("Creator");
    PERSISTID creator_item = pNpcObj->QueryDataObject("CreatorItem");
    IGameObj* pCreatorObj = pKernel->GetGameObj(creator);
    IGameObj* pItemObj = pKernel->GetGameObj(creator_item);
    if (pCreatorObj == NULL || pItemObj == NULL)
    {
        return;
    }
    if (pKernel->Exists(creator) && pKernel->Exists(creator_item))
    {
        if (pCreatorObj->QueryInt("TotalStep") > 1)
        {
            int totalStep = pCreatorObj->QueryInt("TotalStep");
            int curStep = pCreatorObj->QueryInt("CurStep");

            int cur_num = pCreatorObj->QueryDataInt("CurNum");
            int kill_num = pCreatorObj->QueryDataInt("KillNum");
            int limit_num = pItemObj->QueryInt("Limit");
            int step = pItemObj->QueryInt("Step");
            if (curStep == step)
            {
                kill_num = kill_num + 1;
                pCreatorObj->SetDataInt("KillNum", kill_num);
                if (kill_num >= limit_num && curStep < totalStep && limit_num > 0)
                {
                    pCreatorObj->SetInt("CurStep", curStep + 1);
                    pCreatorObj->SetDataInt("KillNum", 0);
                }
            }
		}
    }
}

bool NpcCreatorModule::LoadNpcPool(IKernel* pKernel)
{
    m_mapNpcPool.clear();
    std::string xmlPath = pKernel->GetResourcePath();
    xmlPath += NPC_POOL;

    try
    {
       char* pfilebuff = GetFileBuff(xmlPath.c_str());

        if (NULL == pfilebuff)
        {
            return false;
        }

        xml_document<> doc;

        doc.parse<rapidxml::parse_default>(pfilebuff);

        xml_node<>* pNodeRoot = doc.first_node("Object");
        if (NULL == pNodeRoot)
        {
            return false;
        }
        xml_node<>* pNode = pNodeRoot->first_node("Property");
        if (NULL == pNode)
        {
            return false;
        }

        LoopBeginCheck(check_kk);
        while (pNode != NULL)
        {
            LoopDoCheck(check_kk);
            const char* id = QueryXmlAttr(pNode, "ID");
            xml_node<>* pItem = pNode->first_node("Item");
            std::vector<NpcPool> vecPool;
            LoopBeginCheck(check_ll);
            while (pItem != NULL)
            {
                LoopDoCheck(check_ll);
                const char* npcID = QueryXmlAttr(pItem, "ConfigID");
                int canSame = convert_int(QueryXmlAttr(pItem, "CanSame"), 0);
                int weight = convert_int(QueryXmlAttr(pItem, "Weight"), 0);

                NpcPool info;
                info.configID = npcID;
                info.isSame = canSame;
                info.weight = weight;
                if (StringUtil::CharIsNull(info.configID.c_str()))
                {
					pItem = pItem->next_sibling("Item");
                    continue;
                }

                vecPool.push_back(info);

                pItem = pItem->next_sibling("Item");

            }

            m_mapNpcPool.insert(make_pair(id, vecPool));

            pNode = pNode->next_sibling("Property");
        }
    }
    catch(parse_error& e)
    {
        ::extend_warning(pKernel, e.what());

        return false;
    }

    return true;
}

int NpcCreatorModule::GetConfigID(IKernel* pKernel, const PERSISTID& pCreator, const char* npcPoolID, std::string& newConfigID)
{
    std::map<std::string,std::vector<NpcPool> >::iterator iter = m_pNpcCreatorModule->m_mapNpcPool.find(npcPoolID);
    if (iter == m_pNpcCreatorModule->m_mapNpcPool.end())
    {
        return 0;
    }
    std::vector<NpcPool> tarVec;
    m_pNpcCreatorModule->GetNpcFromPool(pKernel, pCreator, iter->second, tarVec);
    
    int totalWeight = m_pNpcCreatorModule->CalWeight(tarVec);
    int random = util_random_int(totalWeight);

    int size = (int)tarVec.size();
    LoopBeginCheck(check_mm);
    for (int i = 0; i < size; i++)
    {
        LoopDoCheck(check_mm);
        NpcPool& info = tarVec[i];

        int minWeight = info.minWeight;
        int maxWeight = info.maxWeight;
        if (random >= minWeight && random <= maxWeight)
        {
            newConfigID = info.configID.c_str();
            break;
        }
    }
    
    return 0;
}

//������Ȩֵ
int NpcCreatorModule::CalWeight(std::vector<NpcPool>& vec)
{
    int totalWeight = 0;
    int size = (int)vec.size();
    LoopBeginCheck(check_nn);
    for (int i = 0; i < size; i++)
    {
        LoopDoCheck(check_nn);
        NpcPool& info = vec[i];
        int weight = info.weight;

        info.minWeight = totalWeight;
        
        info.maxWeight = totalWeight + weight - 1;

        totalWeight = totalWeight + weight;
    }

    return totalWeight;
}

void NpcCreatorModule::GetNpcFromPool(IKernel* pKernel, const PERSISTID& pCreator, std::vector<NpcPool> srcVec, std::vector<NpcPool>& tarVec)
{
    IGameObj* pCreatorObj = pKernel->GetGameObj(pCreator);
    if (NULL == pCreatorObj)
    {
        return;
    }
    IRecord* creatRec = pCreatorObj->GetRecord(CREATE_OBJECT_LIST_REC);
    if (NULL == creatRec)
    {
        return;
    }
   
    std::vector<NpcPool>::iterator iter = srcVec.begin();
    LoopBeginCheck(check_oo);
    for ( ;iter != srcVec.end(); ++iter)
    {
        LoopDoCheck(check_oo);
        NpcPool& info = *iter;
        const char* configID = info.configID.c_str();
        if (info.isSame == 1)
        {
            bool isBorn = false;

            int rows = creatRec->GetRows();

            LoopBeginCheck(check_pp);
            for (int i = 0; i < rows; i ++)
            {
                LoopDoCheck(check_pp);
                PERSISTID obj = creatRec->QueryObject(i, 0);
                if (!pKernel->Exists(obj))
                {
                    continue;
                }
                const char* str = pKernel->GetConfig(obj);
                if (strcmp(configID, str) == 0)
                {
                    isBorn = true;
                    break;
                }
                
            }
            if (!isBorn)
            {
                tarVec.push_back(info);
            }
        }
        else
        {
            tarVec.push_back(info);
        }
    }
}

//��ǰ�趨���λ��
bool NpcCreatorModule::PreSetCreateCenterPos(IKernel* pKernel, const PERSISTID& creator)
{
	CVarList creator_items;
	pKernel->GetChildList(creator, TYPE_HELPER, creator_items);
    IGameObj* pCreatorObj = pKernel->GetGameObj(creator);
    if (pCreatorObj == NULL)
    {
        return false;
    }

	//��ȡ���ĵ�
	len_t CenterX = 0.0f;
	len_t CenterZ = 0.0f;
	// ����
	int creator_item_counts = (int)creator_items.GetCount();
    LoopBeginCheck(check_qq);
	for (int i = 0; i < creator_item_counts; i++)
	{
        LoopDoCheck(check_qq);
		PERSISTID creator_item = creator_items.ObjectVal(i);
        IGameObj* pItemObj = pKernel->GetGameObj(creator_item);
		if (pKernel->Exists(creator_item) && pItemObj != NULL)
		{
            int refreshCount = pItemObj->QueryInt("MaxAmount");
            if (refreshCount > 1)
            {
                break;
            }
            IRecord* positionRec = pItemObj->GetRecord(POSITION_INFO_REC);
            if (positionRec == NULL)
            {
                continue;
            }
			int iPosCount = positionRec->GetRows();
			if (iPosCount > 0)
			{
				//���һ������
				int iRand = util_random_int(iPosCount);
				//�������
				pCreatorObj->SetInt("RandRow", iRand);
				pCreatorObj->SetObject("RandHelper", creator_item);

				len_t x = positionRec->QueryFloat(iRand, POSIINFOREC_COL_X);
				len_t z = positionRec->QueryFloat(iRand, POSIINFOREC_COL_Z);
				CenterX = x;
				CenterZ = z;
				break;
			}
		}
	}

	len_t MaxScope = pCreatorObj->QueryFloat("MaxScope");
	len_t MinScope = pCreatorObj->QueryFloat("MinScope");
	//���û���趨λ�õĴ��������·���λ��
    LoopBeginCheck(check_rr);
	for (int i = 0; i < creator_item_counts; i++)
	{
        LoopDoCheck(check_rr);
		PERSISTID creator_item = creator_items.ObjectVal(i);
        IGameObj* pItemObj = pKernel->GetGameObj(creator_item);
		if (NULL == pItemObj)
		{
			continue;
		}
        IRecord* positionRec = pItemObj->GetRecord(POSITION_INFO_REC);
        if (positionRec == NULL)
        {
            continue;
        }
		// �����Ƚ�
		int max_amount = pItemObj->QueryInt("MaxAmount");
        int iPosCount = positionRec->GetRows();
        if (iPosCount <= 0 && max_amount > 0)
        {
            LoopBeginCheck(check_ss);
            for (int j = 0; j < max_amount; j++)
            {
                LoopDoCheck(check_ss);
                float x1, y1, z1, o1;

                float orient = util_random_float(PI2);
                // ���ȡ��Χ�ĵ�,��ֹ��ѭ��
                LoopBeginCheck(check_tt);
                for (int k = 0; k < 8; ++k)
                {
                    LoopDoCheck(check_tt);
                    x1 = CenterX + (MinScope + util_random_float(MaxScope - MinScope)) * ::sinf(orient);
                    z1 = CenterZ + (MinScope + util_random_float(MaxScope - MinScope)) * ::cosf(orient);
                    y1 = pKernel->GetMapHeight(x1, z1);
                    if (pKernel->CanWalk(x1, z1))
                    {
                        break;
                    }
                }

                //��������������ĵ㻹�Ǵ�������
                if (!pKernel->CanWalk(x1, z1))
                {
                    x1 = CenterX;
                    z1 = CenterZ;
                }

                o1 = PI2 * ::util_random_float(1.0f);

                // ��ӵ�λ�ñ��
                positionRec->AddRowValue(-1, 
                    CVarList() << x1 << y1 << z1 << o1 << 0 << (__int64)0 << -1.0f);
            }
        }
	}

	return true;
}

bool NpcCreatorModule::CreateRandomNpcByTime(IKernel*pKernel, const PERSISTID& scene, int sceneId, int groupId)
{
	
	if (!pKernel->Exists(scene))
	{
		return 0;
	}
	IGameObj* pSceneObj = pKernel->GetGameObj(scene);
	if (pSceneObj == NULL)
	{
		return 0;
	}
	const char * npcCfgName = GetSceneGroupNpcCfgName(sceneId, groupId);
	auto it = m_refresh_npc_by_time.find(npcCfgName);
	if (it != m_refresh_npc_by_time.end())
	{
		auto its = it->second.begin();
		if (its != it->second.end())
		{
				
			PERSISTID creator = pKernel->Create(scene, "NpcCreatorByTime");
			IGameObj* pCreatorObj = pKernel->GetGameObj(creator);
			if (pCreatorObj == NULL){
				return false;
			}
			pCreatorObj->SetInt("GlobalStep", 0);
			pCreatorObj->SetInt("LittleStepMax",its->_totalStep);
			pCreatorObj->SetInt("LittlestepNow",0);
			pCreatorObj->SetInt("SceneId",sceneId);
				// �Ƿ��鳡���Ļ��������
			if (pSceneObj->QueryInt("Grouping") >= 1)
			{
				pCreatorObj->SetInt("GroupID", groupId);

			}
			if (pKernel->FindHeartBeat(creator, "NpcCreatorModule::H_GroupCreateNpcByTime"))
			{
				DELETE_HEART_BEAT(pKernel, creator, "NpcCreatorModule::H_GroupCreateNpcByTime");
			}
			ADD_COUNT_BEAT(pKernel, creator, "NpcCreatorModule::H_GroupCreateNpcByTime", its->_waitTime, 1);

		}


	}


	return true;

}

//��ó�����NPC������Ϣ���������鳡��ʹ�ã�
int NpcCreatorModule::GetSceneNpcNum(IKernel* pKernel, const PERSISTID& scene)
{
 //   IGameObj* pSceneObj = pKernel->GetGameObj(scene);
 //   if (NULL == pSceneObj)
 //   {
 //       return 0;
 //   }
 //   int sceneID = pKernel->GetSceneId();
 //   if (GroupSceneBase::GetSceneType(pKernel, sceneID) == GROUP_SCENE_NULL)
 //   {
 //       return 0;
 //   }
 //   std::vector<int>::iterator iter = 
 //       find(m_pNpcCreatorModule->v_vecSceneNpc.begin(), m_pNpcCreatorModule->v_vecSceneNpc.end(), sceneID);
 //   if (iter != m_pNpcCreatorModule->v_vecSceneNpc.end())
 //   {
 //       //�Ѵ��ڣ�����
 //       return 0;
 //   }
	//std::string npcCfgName = StringUtil::IntAsString(sceneID);
	//auto fileit = m_group_npc_info.find(npcCfgName);
 //   if (fileit == m_group_npc_info.end())
 //   {
 //       return 0;
 //   }
 //   int eliteNum = 0;
 //   int bossNum = 0;
 //   int boxNum = 0;
 //   int commonNum = 0;

 //   auto& DocVec = fileit->second;

 //   int iDocCount = int(DocVec.size());

 //   // �����ļ�
 //   LoopBeginCheck(check_uu);
 //   for (int iFileIndex = 0; iFileIndex < iDocCount; ++iFileIndex)
 //   {
 //       LoopDoCheck(check_uu);
 //       xml_node<>* pNodeRoot = DocVec[iFileIndex]->first_node("object");

 //       if (pNodeRoot == NULL)
 //       {
 //           continue;
 //       }

 //       // �����������������
 //       xml_node<>* pNode = pNodeRoot->first_node("randomcreator");

 //       if (pNode == NULL)
 //       {
 //           continue;
 //       }

 //       // ��������ָ��
 //       xml_node<>* pCreator = pNode->first_node("creator");

 //       // �ӵ�ǰpCreator����ʼ����
 //       LoopBeginCheck(check_vv);
 //       while (pCreator != NULL)
 //       {
 //           LoopDoCheck(check_vv);
 //           xml_node<>* pItem = pCreator->first_node("item");
 //           LoopBeginCheck(check_ww);
 //           while (pItem != NULL)
 //           {
 //               LoopDoCheck(check_ww);
 //               // ������ɵ�����
 //               int npc_amount = convert_int(QueryXmlAttr(pItem, "amount"));
 //               int npcType = convert_int(QueryXmlAttr(pItem, "npctype"));
 //               switch(npcType)
 //               {
 //               case CREATE_NPC_TYPE_ELITE:
 //                   eliteNum = eliteNum + npc_amount;
 //                   break;
 //               case CREATE_NPC_TYPE_BOSS:
 //                   bossNum = bossNum + npc_amount;
 //                   break;
 //               case CREATE_NPC_TYPE_BOX:
 //                   boxNum = boxNum + npc_amount;
 //                   break;
 //               default:
 //                   commonNum = commonNum + npc_amount;
 //                   break;
 //               }

 //               pItem = pItem->next_sibling("item");
 //           }

 //           // �õ���һ����������Ϣ
 //           pCreator = pCreator->next_sibling("creator");
 //       }
 //   }
 //   pSceneObj->SetInt("EliteNum", eliteNum);
 //   pSceneObj->SetInt("BossNum", bossNum);
 //   pSceneObj->SetInt("BoxNum", boxNum);
 //   pSceneObj->SetInt("CommonNum", commonNum);
    
//    m_pNpcCreatorModule->v_vecSceneNpc.push_back(sceneID);
    return 0;
}

// bool NpcCreatorModule::LoadGuildInbreak(IKernel* pKernel)
// {
// 	m_guildInbreak.clear();
// 	// ����Npc����
// 	std::string file_path = pKernel->GetResourcePath();
// 	file_path += GUILD_INBREAK;
// 	try
// 	{
// 		char* pfilebuff = GetFileBuff(file_path.c_str());
// 
// 		if (StringUtil::CharIsNull(pfilebuff))
// 		{
// 			std::string err_msg = file_path;
// 			err_msg.append(" does not exists.");
// 			::extend_warning(LOG_ERROR, err_msg.c_str());
// 			return false;
// 		}
// 
// 		xml_document<> doc;
// 
// 		doc.parse<rapidxml::parse_default>(pfilebuff);
// 
// 		xml_node<>* pNodeRoot = doc.first_node("object");
// 
// 		if (pNodeRoot == NULL)
// 		{
// 			std::string err_msg = file_path;
// 			err_msg.append(" invalid npc creator file.");
// 			::extend_warning(LOG_ERROR, err_msg.c_str());
// 			return false;
// 		}
// 
// 		xml_node<>* pNode = pNodeRoot->first_node("randomcreator");
// 		if (pNode == NULL)
// 		{
// 			std::string err_msg = file_path;
// 			err_msg.append(" invalid npc creator file.");
// 			::extend_warning(LOG_ERROR, err_msg.c_str());
// 			return false;
// 		}
// 
// 		xml_node<>* pCreator = pNode->first_node("creator");
// 		// �ӵ�ǰpCreator����ʼ����
// 		LoopBeginCheck(a);
// 		while (pCreator != NULL)
// 		{
// 			CreateSpringNpcInfo createSpringNpcInfo;
// 
// 			LoopDoCheck(a);
// 			// ��ʼ������źͷ���id
// 			int nSubSceneNo = -1;
// 			int nGroupId = -1;
// 
// 			// ����
// 			createSpringNpcInfo.m_springPosition.m_x = convert_float(QueryXmlAttr(pCreator, "x"));
// 			createSpringNpcInfo.m_springPosition.m_y = convert_float(QueryXmlAttr(pCreator, "y"));
// 			createSpringNpcInfo.m_springPosition.m_z = convert_float(QueryXmlAttr(pCreator, "z"));
// 
// 			// �⻷
// 			createSpringNpcInfo.m_maxScope = convert_float(QueryXmlAttr(pCreator, "MaxScope"), 30);
// 			//�ڻ�
// 			createSpringNpcInfo.m_minScope = convert_float(QueryXmlAttr(pCreator, "MinScope"), 25);
// 			// �����뾶
// 			createSpringNpcInfo.m_springRange = convert_float(QueryXmlAttr(pCreator, "SpringRange"), 50.0f);
// 			//0-�����󴴽�NPC��1-���볡���ʹ���
// 			createSpringNpcInfo.m_createType = convert_int(QueryXmlAttr(pCreator, "createtype"), 0);
// 
// 			//����Ĳ���
// 			createSpringNpcInfo.m_totalStep = convert_int(QueryXmlAttr(pCreator, "totalstep"), 0);
// 			//NPC�Ƿ����� 0-�� 1-��
// 			createSpringNpcInfo.m_isDestroy = convert_int(QueryXmlAttr(pCreator, "IsDestroy", 0)) > 0 ? true : false;
// 			//�ؿ��ؾ��н�����
// 			createSpringNpcInfo.m_sceneAction = convert_int(QueryXmlAttr(pCreator, "SceneAction", 0));
// 
// 			// ���������
// 			const char *creator_no = QueryXmlAttr(pCreator, "No", "");
// 			createSpringNpcInfo.m_no = creator_no;
// 
// 			int totalNpcNum = 0;
// 			createSpringNpcInfo.m_totalNpcNum = totalNpcNum;
// 			createSpringNpcInfo.m_totalNpcNum = ParseRandomNpcItem(pKernel, createSpringNpcInfo.m_NpcGroup, pCreator);
// 
// 			m_guildInbreak.insert(std::make_pair(creator_no, createSpringNpcInfo));
// 
// 			// �õ���һ����������Ϣ
// 			pCreator = pCreator->next_sibling("creator");
// 		}
// 
// 		delete[] pfilebuff;
// 
// 	}
// 	catch (parse_error& e)
// 	{
// 		::extend_warning(pKernel, "[Error]NpcCreatorModule::LoadTaskCreatorRes: ");
// 		::extend_warning(pKernel, file_path.c_str());
// 		::extend_warning(pKernel, e.what());
// 
// 		return false;
// 	}
// 
// 	return true;
// }


//��ÿɳ�����BOSS
// void NpcCreatorModule::GetBossFromPool(IKernel* pKernel, const char* npcPool, std::string& newConfig)
// {
//     const char* bossConfig = "";
//     IGameObj* pSceneObj = pKernel->GetSceneObj();
//     if (NULL == pSceneObj)
//     {
//         return;
//     }
//     IRecord* record = pSceneObj->GetRecord(SCENE_BOSS_REC);
//     if (NULL == record)
//     {
//         return;
//     }
//     std::map<std::string,std::vector<NpcPool> >::iterator iter = m_mapNpcPool.find(npcPool);
//     if (iter == m_mapNpcPool.end())
//     {
//         return;
//     }
//     std::vector<NpcPool>& npcVec = iter->second;
//     int size = (int)npcVec.size();
//     std::vector<NpcPool> targetVec;
//     LoopBeginCheck(check_xx);
//     for (int i = 0; i < size; ++i)
//     {
//         LoopDoCheck(check_xx);
//         NpcPool& info = npcVec[i];
//         const char* configID = info.configID.c_str();
//         if (info.isSame == 1)
//         {
//             int row = record->FindString(SCENE_BOSS_CONFIG, configID);
//             if (row >= 0)
//             {
//                 continue;
//             }
//             targetVec.push_back(info);
//         }
//         else
//         {
//             targetVec.push_back(info);
//         }
//     }
//     int total = m_pNpcCreatorModule->CalWeight(targetVec);
//     int random = util_random_int(total);
// 
//     int targetSize = (int)targetVec.size();
//     LoopBeginCheck(check_yy);
//     for (int i = 0; i < targetSize; i++)
//     {
//         LoopDoCheck(check_yy);
//         NpcPool& info = targetVec[i];
// 
//         int minWeight = info.minWeight;
//         int maxWeight = info.maxWeight;
//         if (random >= minWeight && random <= maxWeight)
//         {
//             newConfig = info.configID;
//             break;
//         }
//     }
// }
// 
// void NpcCreatorModule::upDateWheelNpc(IKernel*pKernel, int sceneID, int groupID, int killNumNow)
// {
// 
// 	const char* npcCfgName = GetSceneGroupNpcCfgName(sceneID, groupID);
// 	//�жϵ�ǰ�����Ƿ��г���npc
// 	auto it = m_wheel_npc.find(npcCfgName);
// 	if (it == m_wheel_npc.end()){
// 		return;
// 	}
// 
// 
// 	CVarList res;
// 	pKernel->GroupChildList(groupID, TYPE_NPC, res);
// 	LoopBeginCheck(a);
// 	for (size_t i = 0; i < res.GetCount(); i++)
// 	{
// 		LoopDoCheck(a);
// 		PERSISTID& obj = res.ObjectVal(i);
// 		if (pKernel->Exists(obj))
// 		{
// 			IGameObj * sceneObj = pKernel->GetGameObj(obj);
// 			if (sceneObj->GetClassType() == TYPE_NPC)
// 			{
// 				if (sceneObj->FindData("NeedKillNum"))
// 				{
// 					int needNum = sceneObj->QueryDataInt("NeedKillNum");
// 					if (needNum <= 0){
// 						continue;
// 					}
// 						needNum -= 1;
// 					if (needNum <= 0)
// 					{
// 						FightInterfaceInstance->RemoveBuffer(pKernel, obj, CONTROL_BUFFID);
// 					}
// 					else
// 					{
// 						sceneObj->SetDataInt("NeedKillNum", needNum);
// 					}
// 				}
// 			}
// 		}
// 	}
// 
// 
// 	CreateWheelNpc(pKernel, sceneID, groupID, killNumNow);
// 
// }
