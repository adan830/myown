//--------------------------------------------------------------------
// �ļ���:		AIOnCreateArgs.cpp
// ��  ��:		AIģ��Դ���������NPC�ļ��д���
// ˵  ��:		
// ��������:	2015��03��16��
// ������:		  
//    :	   
//--------------------------------------------------------------------
#include "AISystem.h"
#include "FsGame/Define/SkillEventFuncIdDefine.h"

// ��������������
int AISystem::OnCreateArgs(IKernel* pKernel, const PERSISTID& self,
						   const PERSISTID& sender, const IVarList& args)
{
	// ��ö���ָ��
	IGameObj* pSelf = pKernel->GetGameObj(self);

	if (NULL == pSelf)
	{
		return 0;
	}

	// �õ�����
	int type = args.IntVal(0);
	switch(type)
	{
	case CREATE_TYPE_BATTLELEAD_NPC:
		{
			//���׷����Χ���ڴ�����Χ��׷����Χ�͵��ڴ�����Χ + 1.
			if(pSelf->FindAttr("ChaseRange"))
			{
				if(pSelf->QueryFloat("ChaseRange") <= pSelf->QueryFloat("SpringRange"))
				{
					pSelf->SetFloat("ChaseRange", (pSelf->QueryFloat("SpringRange") + 10));
				}
			}

			int iMoveType = args.IntVal(1);
			PERSISTID opt_obj = args.ObjectVal(2);
			int iIndex = args.IntVal(3);
			int iCamp = args.IntVal(4);
			int nLifeTime = args.IntVal(5);
		
			//���ò����ƶ�ֵ
			if (!pSelf->FindData("MoveType"))
			{
				pSelf->AddDataInt("MoveType", 0);
			}
			pSelf->SetDataInt("MoveType", iMoveType);

			pSelf->SetInt("LifeTime", nLifeTime);

			//��������Ҷ���
			if (!pSelf->FindData("BattleOptObj"))
			{
				pSelf->AddDataObject("BattleOptObj", PERSISTID());
			}
			pSelf->SetDataObject("BattleOptObj", opt_obj);

			//���������ɱ������Ͳ��ɹ���
			pSelf->SetInt("Invisible",1);
			pSelf->SetInt("CantBeAttack",1);
			pSelf->SetInt("CantAttack",1);

			const char* BattleTeamRec = ("BattleTeamCampRec" + util_int_as_string(iCamp)).c_str();
			IGameObj* pOptObj = pKernel->GetGameObj(opt_obj);
			if (pOptObj != NULL)
			{
				IRecord* pRecord = pOptObj->GetRecord(BattleTeamRec);
				if (pRecord != NULL)
				{
					//�����Ϣ������
					CVarList arg_list;
					arg_list << iIndex + 1 << self << AI_BATTLETEAM_STATUS_BORN << iMoveType << 0.0f << 0.0f << 0.0f << 0.0f;
					pRecord->SetRowValue(iIndex, arg_list);
				}
			}
		}
		break;
	case CREATE_TYPE_BATTLE_NPC:
		{
			//���׷����Χ���ڴ�����Χ��׷����Χ�͵��ڴ�����Χ + 1.
			if(pSelf->FindAttr("ChaseRange"))
			{
				if(pSelf->QueryFloat("ChaseRange") <= pSelf->QueryFloat("SpringRange"))
				{
					pSelf->SetFloat("ChaseRange", (pSelf->QueryFloat("SpringRange") + 10));
				}
			}

			int iMoveType = args.IntVal(1);
			PERSISTID LeadNpc = args.ObjectVal(2);
			int iIndex = args.IntVal(3);
			int iGroupId = args.IntVal(4);
			//PERSISTID Player = args.ObjectVal(5);

            //�����ս��Ա����Ĭ�����أ�������������ݺ���Զ���ʾ
            pSelf->SetInt("Invisible",1);

			pSelf->SetInt("GroupID", iGroupId);


			//���ò����ƶ�ֵ
			if (!pSelf->FindData("MoveType"))
			{
				pSelf->AddDataInt("MoveType", 0);
			}
			pSelf->SetDataInt("MoveType", iMoveType);

			//������ӳ�
			if (!pSelf->FindData("LeadNpc"))
			{
				pSelf->AddDataObject("LeadNpc", PERSISTID());
			}
			pSelf->SetDataObject("LeadNpc", LeadNpc);

			//�������к�
			if (!pSelf->FindData("BattleSerial"))
			{
				pSelf->AddDataInt("BattleSerial", 0);
			}
			pSelf->SetDataInt("BattleSerial", iIndex);

			//�ӳ�����������ҳ�Ա��Ϣ
			IGameObj* pLeadNpcObj = pKernel->GetGameObj(LeadNpc);
			if (pLeadNpcObj != NULL)
			{
				IRecord* pRecord = pLeadNpcObj->GetRecord("BattleChildNpcRec");
				if (pRecord != NULL)
				{
					CVarList arg_list;
					arg_list << self << AI_BATTLEPLAYER_STATUS_BORN;
					pRecord->AddRowValue(-1, arg_list);
				}
			}
		}
		break;

	case CREATE_TYPE_PROPERTY_VALUE://������ֵ�������� Key-Valueֵ�Է�ʽ��ֻ֧���滻��
		{
			size_t iCount = args.GetCount();
            LoopBeginCheck(a);
			for (size_t i = 1; i < iCount; i+=2)
			{
                LoopDoCheck(a);
				const char* pPropName = args.StringVal(i);
				// ��ѯ�Ƿ��������
				if (!pSelf->FindAttr(pPropName))
				{
					continue;
				}

				int PorpType = pSelf->GetAttrType(pPropName);
				if (VTYPE_INT == PorpType)
				{
					int iNewValue = args.IntVal(i + 1);
					pSelf->SetInt(pPropName, iNewValue);
				}
				else if (VTYPE_FLOAT == PorpType)
				{
					float fNewValue = args.FloatVal(i + 1);
					pSelf->SetFloat(pPropName, fNewValue);
				}
				else if (VTYPE_DOUBLE == PorpType)
				{
					double dNewValue = args.DoubleVal(i + 1);
					pSelf->SetDouble(pPropName, dNewValue);
				}
				else if (VTYPE_INT64 == PorpType)
				{
					int64_t iNewValue = args.Int64Val(i + 1);
					pSelf->SetInt64(pPropName, iNewValue);
				}
				else if (VTYPE_STRING == PorpType)
				{
					const char* pNewValue = args.StringVal(i + 1);
					pSelf->SetString(pPropName, pNewValue);
				}
				else if (VTYPE_WIDESTR == PorpType)
				{
					const wchar_t* pNewValue = args.WideStrVal(i + 1);
					pSelf->SetWideStr(pPropName, pNewValue);
				}
				else if (VTYPE_OBJECT == PorpType)
				{
					PERSISTID obj = args.ObjectVal(i + 1);
					pSelf->SetObject(pPropName, obj);
				}
				else
				{
					continue;
				}
			}
		}

		break;
	case CREATE_TYPE_SKILL_NPC://����NPC
		{
			// ����
			pSelf->SetInt("Type", TYPE_NPC);

			PERSISTID owner = args.ObjectVal(1);
			if (!pKernel->Exists(owner))
			{
				return 0;
			}

			// ����ʩ����
			pSelf->SetObject("Master", owner);
			int iGroupID = args.IntVal(2);
			pSelf->SetInt("GroupID", iGroupID);

		}
		break;
	default:
		break;
	}

	return 0;
}
