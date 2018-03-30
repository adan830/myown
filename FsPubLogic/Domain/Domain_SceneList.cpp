//--------------------------------------------------------------------
// �ļ���:      Domain_SceneList.cpp
// ��  ��:      ���б������ĳ����б�
// ˵  ��:		
// ��������:    2014��10��17��
// ������:        
// �޸���:        
//    :       
//--------------------------------------------------------------------

#include "Domain_SceneList.h"
#include "../../public/VarList.h"
#include "../../FsGame/Define/PubDefine.h"
#include "../../FsGame//Define/SceneListDefine.h"

const char * scene_list_rec_name = "scene_list_rec";
enum 
{
	SCENE_LIST_COL_scene_id,
	SCENE_LIST_COL_config_id,
	SCENE_LIST_COL_member_id,
};

int Domain_SceneList::OnCreate(IPubKernel* pPubKernel, IPubSpace * pPubSpace)
{	
	return OnLoad(pPubKernel, pPubSpace);
}

int Domain_SceneList::OnLoad(IPubKernel* pPubKernel, IPubSpace * pPubSpace)
{
	if (NULL == pPubKernel || NULL == pPubSpace)
	{
		return 1;
	}

	IPubData * pSceneListData = pPubSpace->GetPubData(GetDomainName(pPubKernel).c_str());
	if (pSceneListData == NULL)
		return 1;

	if (!pSceneListData->FindRecord(scene_list_rec_name))
	{
		CreateTable(pSceneListData, scene_list_rec_name);
	}

	pSceneListData->ClearRecordRow(scene_list_rec_name);

	// ������������Ӧ�ĳ�����
	if (!pSceneListData->FindRecord(SERVER_MEMBER_FIRST_LOAD_SCENE_REC))
	{
		CVarList cols;
		cols << VTYPE_INT << VTYPE_INT;
		pSceneListData->AddRecord(SERVER_MEMBER_FIRST_LOAD_SCENE_REC, 
			SERVER_MEMBER_FIRST_LOAD_SCENE_REC_MAX_ROW, 
			SERVER_MEMBER_FIRST_LOAD_SCENE_COL_TOTAL, 
			cols);
	}

	pSceneListData->ClearRecordRow(SERVER_MEMBER_FIRST_LOAD_SCENE_REC);

	return 1;
}

/// \brief ���յ���������������Ϣ
/// \param source_id ����Ϣ�ķ�������ʶ
/// \param msg ��Ϣ����
int Domain_SceneList::OnMessage(IPubKernel* pPubKernel, IPubSpace * pPubSpace,
								int source_id, int scene_id, const IVarList& msg)
{
	IPubData * pSceneListData = pPubSpace->GetPubData(GetDomainName(pPubKernel).c_str());
	if (pSceneListData == NULL)
		return 1;

	int msgid = msg.IntVal(2);
	switch (msgid)
	{
	case SP_DOMAIN_MSG_SCENELIST_ADD_SCENE:
		{
			int sceneId = msg.IntVal(3);
			int memberId = msg.IntVal(5);

			CVarList value;
			value << sceneId << msg.StringVal(4) << memberId;

			pSceneListData->AddRecordRowValue(scene_list_rec_name, -1, value);

			IRecord* pMemberSceneRec = pSceneListData->GetRecord(SERVER_MEMBER_FIRST_LOAD_SCENE_REC);
			if (pMemberSceneRec != NULL)
			{
				int row = pMemberSceneRec->FindInt(SERVER_MEMBER_FIRST_LOAD_SCENE_COL_member_id, memberId);
				if (row < 0)
				{
					// memberId ֻ��Ӧһ������
					pMemberSceneRec->AddRowValue(-1, CVarList() << memberId << sceneId);
				}
			}
		}
		break;
	case SP_DOMAIN_MSG_SCENELIST_DEL_SCENE:
		{
			int remove_scene_id = msg.IntVal(3);

			// �����б�
			IRecord* pSceneListRec = pSceneListData->GetRecord(scene_list_rec_name);
			if (pSceneListRec == NULL)
			{
				break;
			}

			// ɾ��������¼
			int row = pSceneListRec->FindInt(SCENE_LIST_COL_scene_id, remove_scene_id);
			if (row >= 0)
			{
				pSceneListData->RemoveRecordRow(scene_list_rec_name, row);
			}
			
			// member��Ӧ�ĳ�����
			IRecord* pMemberSceneRec = pSceneListData->GetRecord(SERVER_MEMBER_FIRST_LOAD_SCENE_REC);
			if (pMemberSceneRec == NULL)
			{
				break;
			}
			
			// ���Ҽ�¼
			row = pMemberSceneRec->FindInt(SERVER_MEMBER_FIRST_LOAD_SCENE_COL_scene_id, remove_scene_id);
			if (row < 0)
			{
				// �޼�¼
				break;
			}

			// ���ұ�ɾ���ĳ�����Ӧ��member Id
			int memberId = pMemberSceneRec->QueryInt(row, SERVER_MEMBER_FIRST_LOAD_SCENE_COL_member_id);

			// ���Ҵ�member��Ӧ����������
			int memberRow = pSceneListRec->FindInt(SCENE_LIST_COL_member_id, memberId);
			if (memberRow < 0)
			{
				// ���������� ɾ���˼�¼
				pMemberSceneRec->RemoveRow(row);
			}
			else
			{
				// ��ѯ���еĳ���ID
				int sceneId = pSceneListRec->QueryInt(memberRow, SCENE_LIST_COL_scene_id);

				// �޸�member��Ӧ�ĳ�����
				pSceneListRec->SetInt(row, SERVER_MEMBER_FIRST_LOAD_SCENE_COL_scene_id, sceneId);
			}
		
		}
		break;
	}
	return 1;
}

void Domain_SceneList::CreateTable(IPubData * pSceneListData, const char * table_name)
{
	if (pSceneListData == NULL)
	{
		return;
	}

	if (!pSceneListData->FindRecord(table_name))
	{
		CVarList coltypes;
		coltypes << VTYPE_INT << VTYPE_STRING << VTYPE_INT;

		pSceneListData->AddRecord(table_name, 4096, (int)coltypes.GetCount(), coltypes);
	}	
}
