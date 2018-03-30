//--------------------------------------------------------------------
// �ļ���:      SkillDataDefineStruct.h
// ��  ��:      �����������ݽṹ
// ˵  ��:
// ��������:    2014��10��20��
// ������:       
//    :       
//--------------------------------------------------------------------
#ifndef _SKILLDEFINEDATASTRUCT_H_
#define _SKILLDEFINEDATASTRUCT_H_
#include <map>
#include "public/IVarList.h"
#include "Fsgame/Define/header.h"

#include "utils/string_util.h"
#include "utils/XmlFile.h"
#include "utils/util_func.h"
#include "utils/extend_func.h"
#include "utils/string_buffer.h"
#include "FsGame/Define/SkillDefine.h"


//////////////////////////////////////////////////////////////////////////
// bool
#define  DECLARE_INLINE_BOOL_PROP_VARIANT(name) \
public:\
	inline const bool Get##name() const {return b##name;} \
public:\
	inline void Set##name(const int value){value > 0 ? b##name = true :b## name = false;}\
	inline void Set##name(IKernel* pKernel, const PERSISTID& skill) const\
	{\
	IGameObj* pSkillObj = pKernel->GetGameObj(skill);\
	if (pSkillObj && pSkillObj->FindAttr(#name)) pSkillObj->SetInt(#name, Get##name() ? 1:0);\
	}\
	bool b##name;


// int
#define DECLARE_INLINE_INT_PROP_VARIANT(name) \
public:\
	inline const int Get##name() const {return i##name;} \
public:\
	inline void Set##name(const int value){i##name = value;}\
	inline void Set##name(IKernel* pKernel, const PERSISTID& skill) const\
	{\
	IGameObj* pSkillObj = pKernel->GetGameObj(skill);\
	if (pSkillObj && pSkillObj->FindAttr(#name)) pSkillObj->SetInt(#name, Get##name());\
	}\
	int i##name;


// float
#define DECLARE_INLINE_FLOAT_PROP_VARIANT(name) \
public:\
	inline const float Get##name() const {return f##name;} \
public:\
	inline void Set##name(const float value){f##name = value;}\
	inline void Set##name(IKernel* pKernel, const PERSISTID& skill) const\
	{\
	IGameObj* pSkillObj = pKernel->GetGameObj(skill);\
	if (pSkillObj && pSkillObj->FindAttr(#name)) pSkillObj->SetFloat(#name, Get##name());\
	}\
	float f##name;


// string
#define DECLARE_INLINE_STRING_PROP_VARIANT(name) \
public:\
	inline const char* Get##name() const {const char* p = m_pStringBufferRef->Get(offset_##name); return p;} \
public:\
	inline void Set##name(const char* value){offset_##name = m_pStringBufferRef->Add(value);}\
	inline void Set##name(IKernel* pKernel, const PERSISTID& skill) const\
	{\
	IGameObj* pSkillObj = pKernel->GetGameObj(skill);\
	if (pSkillObj && pSkillObj->FindAttr(#name)) pSkillObj->SetString(#name, Get##name());\
	}\
	size_t offset_##name;


// CVarList
#define DECLARE_INLINE_CVARLIST_PROP_VARIANT(name) \
public:\
	inline const StringArray& Get##name() const {return v##name;} \
public:\
	inline void Set##name(const std::string &value, const char* flag = ",")\
	{\
	v##name.Init(4, 4, m_pStringBufferRef);\
	CVarList tmp;\
	util_split_string(tmp, value, flag);\
	v##name.AddVarList(tmp);\
	}\
	StringArray v##name;



#define _STRING_BUFFER_DEFINED_ \
	StringBuffer* m_pStringBufferRef

#define _STRING_BUFFER_INIT_ \
	m_pStringBufferRef = &stringBuffer

//////////////////////////////////////////////////////////////////////////
// skill data struct define

struct SkillBaseData
{
	_STRING_BUFFER_DEFINED_;

	DECLARE_INLINE_STRING_PROP_VARIANT(ID);//���ܱ��
	DECLARE_INLINE_STRING_PROP_VARIANT(script);//�ű�����
public:
	void SetPropData(CXmlFile &xml, const char* szSectionName, StringBuffer& stringBuffer)
	{
		_STRING_BUFFER_INIT_;

		SetID(szSectionName);
		Setscript(xml.ReadString(szSectionName, "script", "SkillNormal"));
	}

	void SetPropData(IKernel* pKernel, const PERSISTID& skill) const
	{
		Setscript(pKernel, skill);
	}
}; // end of struct SkillBaseData

struct SkillMainData
{
	_STRING_BUFFER_DEFINED_;

	DECLARE_INLINE_STRING_PROP_VARIANT(ID);//������
	DECLARE_INLINE_INT_PROP_VARIANT(CoolDownCategory);//��ȴ����
	DECLARE_INLINE_INT_PROP_VARIANT(CoolDownTime);//��ȴʱ��
	DECLARE_INLINE_INT_PROP_VARIANT(PublicCDTime);//������ȴ
	DECLARE_INLINE_INT_PROP_VARIANT(SelectType);//�����ͷ�ѡ������
	DECLARE_INLINE_INT_PROP_VARIANT(DamageType);// �����˺�����
	DECLARE_INLINE_INT_PROP_VARIANT(ConsumeMP);// ���ܺ���
	DECLARE_INLINE_STRING_PROP_VARIANT(SkillFormula);// ���ܹ�ʽ
	DECLARE_INLINE_BOOL_PROP_VARIANT(IsGrowProperty);// �Ƿ����������
	DECLARE_INLINE_FLOAT_PROP_VARIANT(RangeDistMax);//����̷�Χ-�����Ч����
	DECLARE_INLINE_FLOAT_PROP_VARIANT(RangeDistMin);//����̷�Χ-��С��Ч����
	DECLARE_INLINE_FLOAT_PROP_VARIANT(DamageRate);//�˺�����
	DECLARE_INLINE_BOOL_PROP_VARIANT(IsHitSelf);//�Ƿ���������
	DECLARE_INLINE_CVARLIST_PROP_VARIANT(HitTargetRelationType);//���������Ķ����ϵ (EHitTargetRelationType)
	DECLARE_INLINE_CVARLIST_PROP_VARIANT(HitTargetType);//���������Ķ�������
	DECLARE_INLINE_CVARLIST_PROP_VARIANT(HitRangeID);//���������÷�Χ
	DECLARE_INLINE_INT_PROP_VARIANT(HitCount);//�Ƿ�Χ����󲨼�����
	DECLARE_INLINE_BOOL_PROP_VARIANT(IncludeDead);//���Ƿ������������
	DECLARE_INLINE_BOOL_PROP_VARIANT(IsByHP);//�Ƿ�Ѫ���ٷֱ�����
	DECLARE_INLINE_BOOL_PROP_VARIANT(IsByDist);//���Ƿ�����ѡ���������Ķ���
	DECLARE_INLINE_BOOL_PROP_VARIANT(IsPrepare);//�Ƿ���Ҫ����
	DECLARE_INLINE_INT_PROP_VARIANT(PrepareTime);//������-����ʱ��ms
	DECLARE_INLINE_BOOL_PROP_VARIANT(CantBreakPrepareAction);//������-�����ܷ���

	DECLARE_INLINE_BOOL_PROP_VARIANT(IsLead);//�Ƿ���Ҫ����
	DECLARE_INLINE_INT_PROP_VARIANT(LeadTime);//������-����ʱ��ms
	DECLARE_INLINE_INT_PROP_VARIANT(LeadSepTime);//������-���ʱ��ms
	DECLARE_INLINE_BOOL_PROP_VARIANT(CantBreakLeadAction);//�����������ܷ���
	DECLARE_INLINE_CVARLIST_PROP_VARIANT(Skill_Effect);//���ܸ߼�Ч����ID		
	DECLARE_INLINE_INT_PROP_VARIANT(HitTime);//����ʱ��
	DECLARE_INLINE_INT_PROP_VARIANT(CastStiffTime);// ʹ�ü���Ӳֱʱ��(�ڴ�ʱ���ڶ�����ִ������AI��Ϊ,ֻ��npc�����������Ч)
	DECLARE_INLINE_INT_PROP_VARIANT(HateValue);//��޻���ֵ
	DECLARE_INLINE_FLOAT_PROP_VARIANT(HateRate);//���ϵ��
	DECLARE_INLINE_CVARLIST_PROP_VARIANT(ExtraHitTimes);//�������ʱ��
	DECLARE_INLINE_CVARLIST_PROP_VARIANT(ExtraHitDamage);//��������˺�����
	DECLARE_INLINE_INT_PROP_VARIANT(DisplaceRange);//ʩ����λ�Ƽ��ܷ�Χ��������
	DECLARE_INLINE_INT_PROP_VARIANT(DisplaceType);//ʩ����λ�Ƽ�������
	DECLARE_INLINE_FLOAT_PROP_VARIANT(DisplaceSpeed);//ʩ����λ���ٶ�
	DECLARE_INLINE_INT_PROP_VARIANT(TargetDisplaceRange);//Ŀ��λ�Ƽ��ܷ�Χ��������
	DECLARE_INLINE_INT_PROP_VARIANT(TargetDisplaceType);//Ŀ��λ�Ƽ�������
	DECLARE_INLINE_INT_PROP_VARIANT(ValidDistance);//��֤����
	DECLARE_INLINE_INT_PROP_VARIANT(ValidTime);//��֤ʱ��
	DECLARE_INLINE_BOOL_PROP_VARIANT(IsClientHit);//�Ƿ��пͻ�����������
	DECLARE_INLINE_CVARLIST_PROP_VARIANT(PassiveSkill);//Ӱ�켼�ܵı�������
	DECLARE_INLINE_BOOL_PROP_VARIANT(IsUnSilent);//�Ƿ�Գ�Ĭ��Ч�ļ���
public:
	void SetPropData(CXmlFile &xml, const char* szSectionName, StringBuffer& stringBuffer)
	{
		_STRING_BUFFER_INIT_;

		SetID(szSectionName);
		SetCoolDownCategory(xml.ReadInteger(szSectionName,"CoolDownCategory",0));
		SetCoolDownTime(xml.ReadInteger(szSectionName,"CoolDownTime",0));
		SetPublicCDTime(xml.ReadInteger(szSectionName,"PublicCDTime",0));
		SetSelectType(xml.ReadInteger(szSectionName,"SelectType",0));
		SetDamageType(xml.ReadInteger(szSectionName,"DamgeType",0));
		SetConsumeMP(xml.ReadInteger(szSectionName, "ConsumeMP", 0));
		SetSkillFormula(xml.ReadString(szSectionName, "SkillFormula", ""));
		SetIsGrowProperty(xml.ReadInteger(szSectionName,"IsGrowProperty",0));
		SetRangeDistMax(xml.ReadFloat(szSectionName,"RangeDistMax",1.0f));
		SetRangeDistMin(xml.ReadFloat(szSectionName,"RangeDistMin",0.0f));
		SetDamageRate(xml.ReadFloat(szSectionName,"DamageRate",1.0f));
		SetIsHitSelf(xml.ReadInteger(szSectionName,"IsHitSelf",0));
		SetHitTargetRelationType(xml.ReadString(szSectionName,"HitTargetRelationType",""),",");
		SetHitTargetType(xml.ReadString(szSectionName,"HitTargetType",""),",");
		SetHitRangeID(xml.ReadString(szSectionName,"HitRangeID",""), ",");
		SetHitCount(xml.ReadInteger(szSectionName,"HitCount",0));
		SetIncludeDead(xml.ReadInteger(szSectionName,"IncludeDead",0));
		SetIsByHP(xml.ReadInteger(szSectionName,"IsByHP",0));
		SetIsByDist(xml.ReadInteger(szSectionName,"IsByDist",0));
		SetIsPrepare(xml.ReadInteger(szSectionName,"IsPrepare",0));
		SetPrepareTime(xml.ReadInteger(szSectionName,"PrepareTime",0));
		SetCantBreakPrepareAction(xml.ReadInteger(szSectionName,"CantBreakPrepareAction",0));
		SetIsLead(xml.ReadInteger(szSectionName,"IsLead",0));
		SetLeadTime(xml.ReadInteger(szSectionName,"LeadTime",0));
		SetLeadSepTime(xml.ReadInteger(szSectionName,"LeadSepTime",0));
		SetCantBreakLeadAction(xml.ReadInteger(szSectionName,"CantBreakLeadAction",0));
		SetSkill_Effect(xml.ReadString(szSectionName,"Skill_Effect",""), ",");
		SetHitTime(xml.ReadInteger(szSectionName,"HitTime",0));
		SetHateValue(xml.ReadInteger(szSectionName, "HateValue", 0));
		SetHateRate(xml.ReadFloat(szSectionName, "HateRate", 0.0f));
		SetCastStiffTime(xml.ReadInteger(szSectionName,"CastStiffTime",0));
		SetExtraHitTimes(xml.ReadString(szSectionName,"ExtraHitTimes",""),",");
		SetExtraHitDamage(xml.ReadString(szSectionName,"ExtraHitDamage",""),",");
		SetDisplaceRange(xml.ReadInteger(szSectionName,"DisplaceRange",0));
		SetDisplaceType(xml.ReadInteger(szSectionName,"DisplaceType",0));
		SetDisplaceSpeed(xml.ReadFloat(szSectionName,"DisplaceSpeed",15.0f));
		SetTargetDisplaceRange(xml.ReadInteger(szSectionName,"TargetDisplaceRange",0));
		SetTargetDisplaceType(xml.ReadInteger(szSectionName,"TargetDisplaceType",0));
		SetValidDistance(xml.ReadInteger(szSectionName,"ValidDistance",0));
		SetValidTime(xml.ReadInteger(szSectionName,"ValidTime",0));
		SetIsClientHit(xml.ReadInteger(szSectionName,"IsClientHit",0));
		SetPassiveSkill(xml.ReadString(szSectionName,"PassiveSkill",""), ",");
		SetIsUnSilent(xml.ReadInteger(szSectionName,"IsUnSilent",0));
	}
}; // end of struct SkillMainData

struct SkillHitRangeData 
{
	_STRING_BUFFER_DEFINED_;

	DECLARE_INLINE_STRING_PROP_VARIANT(ID);//�������÷�ΧID
	DECLARE_INLINE_INT_PROP_VARIANT(HitRangeType);//�������÷�Χ����
public:
	const SkillHitRangeParams &GetHitRangeParams() const {return skillHitRangeParams;}
	void SetPropData(const CVarList& Params,int iHitRangeType)
	{
		skillHitRangeParams.eHitRangeType = (ESkillHitRangeType)iHitRangeType;
		SetHitRangeType(iHitRangeType);
		switch (iHitRangeType)
		{
		case SKILL_HIT_RANGE_TYPE_SPHERE:
			skillHitRangeParams.skillhitrangeunion.sphere.fHitRangeSphereDistCentre = StringUtil::ParseStrAsFloat(Params.StringVal(0), 0.0f);    // ���η�Χ-ʩ���߾��������ľ���
			skillHitRangeParams.skillhitrangeunion.sphere.fHitRangeSphereDistMax = StringUtil::ParseStrAsFloat(Params.StringVal(1), 0.0f);       // ���η�Χ-�����Ч����
			skillHitRangeParams.skillhitrangeunion.sphere.fHitRangeSphereDistMin = StringUtil::ParseStrAsFloat(Params.StringVal(2), 0.0f);       // ���η�Χ-��С��Ч����
			skillHitRangeParams.skillhitrangeunion.sphere.fHitRangeSphereTurnAngle = StringUtil::ParseStrAsFloat(Params.StringVal(3), 0.0f);     // ���η�Χ-������Чת��
			skillHitRangeParams.skillhitrangeunion.sphere.fHitRangeSphereUpAngle = StringUtil::ParseStrAsFloat(Params.StringVal(4), 0.0f);       // ���η�Χ-������Ч����
			skillHitRangeParams.skillhitrangeunion.sphere.fHitRangeSphereOffsetOrient = StringUtil::ParseStrAsFloat(Params.StringVal(5), 0.0f);  // ���η�Χ-˳ʱ��ת��
			skillHitRangeParams.skillhitrangeunion.sphere.fHitRangeSphereBasepointOffsetOrient = StringUtil::ParseStrAsFloat(Params.StringVal(6), 0.0f);  // ���η�Χ-����˳ʱ��ת��

			// �Ƕ���ת���ɻ�����
			skillHitRangeParams.skillhitrangeunion.sphere.fHitRangeSphereTurnAngle = skillHitRangeParams.skillhitrangeunion.sphere.fHitRangeSphereTurnAngle * PI / 180.0f;
			skillHitRangeParams.skillhitrangeunion.sphere.fHitRangeSphereUpAngle = skillHitRangeParams.skillhitrangeunion.sphere.fHitRangeSphereUpAngle * PI / 180.0f;
			skillHitRangeParams.skillhitrangeunion.sphere.fHitRangeSphereOffsetOrient = skillHitRangeParams.skillhitrangeunion.sphere.fHitRangeSphereOffsetOrient * PI / 180.0f;
			skillHitRangeParams.skillhitrangeunion.sphere.fHitRangeSphereBasepointOffsetOrient = skillHitRangeParams.skillhitrangeunion.sphere.fHitRangeSphereBasepointOffsetOrient * PI / 180.0f;

			break;
		case SKILL_HIT_RANGE_TYPE_CYLINDER:
			skillHitRangeParams.skillhitrangeunion.cylinder.fHitRangeCylinderDistCentre = StringUtil::ParseStrAsFloat(Params.StringVal(0), 0.0f);      // Բ����Χ-ʩ���߾�Բ�������߾���
			skillHitRangeParams.skillhitrangeunion.cylinder.fHitRangeCylinderDistMax = StringUtil::ParseStrAsFloat(Params.StringVal(1), 0.0f);         // Բ����Χ-�����Ч����
			skillHitRangeParams.skillhitrangeunion.cylinder.fHitRangeCylinderDistMin = StringUtil::ParseStrAsFloat(Params.StringVal(2), 0.0f);         // Բ����Χ-��С��Ч����
			skillHitRangeParams.skillhitrangeunion.cylinder.fHitRangeCylinderTurnAngle = StringUtil::ParseStrAsFloat(Params.StringVal(3), 0.0f);       // Բ����Χ-������Чת��
			//hitRangeParams.skillhitrangeunion.cylinder.fHitRangeCylinderHeight = ::convert_float(configdata.StringVal(4), 0.0f);          // Բ����Χ-Բ����Ч�߶�
			skillHitRangeParams.skillhitrangeunion.cylinder.fHitRangeCylinderOffsetOrient = StringUtil::ParseStrAsFloat(Params.StringVal(4), 0.0f);    // Բ����Χ-˳ʱ��ת��
			skillHitRangeParams.skillhitrangeunion.cylinder.fHitRangeCylinderBasepointOffsetOrient = StringUtil::ParseStrAsFloat(Params.StringVal(5), 0.0f);   // Բ����Χ-����˳ʱ��ת��

			// �Ƕ���ת���ɻ�����
			skillHitRangeParams.skillhitrangeunion.cylinder.fHitRangeCylinderTurnAngle = skillHitRangeParams.skillhitrangeunion.cylinder.fHitRangeCylinderTurnAngle * PI / 180.0f;
			skillHitRangeParams.skillhitrangeunion.cylinder.fHitRangeCylinderOffsetOrient = skillHitRangeParams.skillhitrangeunion.cylinder.fHitRangeCylinderOffsetOrient * PI / 180.0f;
			skillHitRangeParams.skillhitrangeunion.cylinder.fHitRangeCylinderBasepointOffsetOrient = skillHitRangeParams.skillhitrangeunion.cylinder.fHitRangeCylinderBasepointOffsetOrient * PI / 180.0f;

			break;
		case SKILL_HIT_RANGE_TYPE_RECTANGULAR:
			skillHitRangeParams.skillhitrangeunion.rectangular.fHitRangeRectangularDistCentre = StringUtil::ParseStrAsFloat(Params.StringVal(0), 0.0f);   // ���η�Χ-ʩ���߾�������ľ���
			skillHitRangeParams.skillhitrangeunion.rectangular.fHitRangeRectangularLength = StringUtil::ParseStrAsFloat(Params.StringVal(1), 0.0f);       // ���η�Χ-��Ч����
			skillHitRangeParams.skillhitrangeunion.rectangular.fHitRangeRectangularWidth = StringUtil::ParseStrAsFloat(Params.StringVal(2), 0.0f);        // ���η�Χ-��Ч���
			//hitRangeParams.skillhitrangeunion.rectangular.fHitRangeRectangularHeight = ::convert_float(configdata.StringVal(3), 0.0f);       // ���η�Χ-��Ч�߶�
			skillHitRangeParams.skillhitrangeunion.rectangular.fHitRangeRectangularOffsetOrient = StringUtil::ParseStrAsFloat(Params.StringVal(3), 0.0f); // ���η�Χ-˳ʱ��ת��
			skillHitRangeParams.skillhitrangeunion.rectangular.fHitRangeRectangularBasepointOffsetOrient = StringUtil::ParseStrAsFloat(Params.StringVal(4), 0.0f); // ���η�Χ-����˳ʱ��ת��

			// �Ƕ���ת���ɻ�����
			skillHitRangeParams.skillhitrangeunion.rectangular.fHitRangeRectangularOffsetOrient = skillHitRangeParams.skillhitrangeunion.rectangular.fHitRangeRectangularOffsetOrient * PI / 180.0f;
			skillHitRangeParams.skillhitrangeunion.rectangular.fHitRangeRectangularBasepointOffsetOrient = skillHitRangeParams.skillhitrangeunion.rectangular.fHitRangeRectangularBasepointOffsetOrient * PI / 180.0f;

			break;
		case SKILL_HIT_RANGE_TYPE_POS_CYLINDER:
			skillHitRangeParams.skillhitrangeunion.poscylinder.fHitRangeRadius = StringUtil::ParseStrAsFloat(Params.StringVal(0), 0.0f);   // ��ĳ��λ��Ϊ����Բ�İ뾶
			break;
		}
	}
	//
	void SetPropData(CXmlFile &xml, const char* szSectionName, StringBuffer& stringBuffer)
	{
		_STRING_BUFFER_INIT_;

		SetID(szSectionName);
		int iHitRangeType = xml.ReadInteger(szSectionName, "HitRangeType",0);
		CVarList Params;	//�����б�
		Params.Clear();
		switch (iHitRangeType){
		case SKILL_HIT_RANGE_TYPE_SPHERE:
			util_split_string(Params, xml.ReadString(szSectionName, "HitRangeBall",""), ",");
			break;
		case SKILL_HIT_RANGE_TYPE_CYLINDER:
			util_split_string(Params, xml.ReadString(szSectionName, "HitRangeCylinder",""),",");
			break;
		case SKILL_HIT_RANGE_TYPE_RECTANGULAR:
			util_split_string(Params, xml.ReadString(szSectionName, "HitRangeRectangular",""), ",");
			break;
		case SKILL_HIT_RANGE_TYPE_POS_CYLINDER:
			util_split_string(Params, xml.ReadString(szSectionName, "HitRangePosCylinder",""), ",");
			break;
		}

		SetPropData(Params,iHitRangeType);
	}
private:	
	SkillHitRangeParams skillHitRangeParams;
	//CVarList Params;	//�����б�

}; // end of struct SkillHitRangeData

struct SkillEffectData 
{
	_STRING_BUFFER_DEFINED_;

	DECLARE_INLINE_STRING_PROP_VARIANT(ID);//Ч�����
	DECLARE_INLINE_INT_PROP_VARIANT(EventType);//�¼��� ESkillEventType
	DECLARE_INLINE_INT_PROP_VARIANT(TargetType);//ǿ��Ŀ��ת�� ETargetType
	DECLARE_INLINE_INT_PROP_VARIANT(FuncNo);//���������
public:
	inline const StringArray& GetParams() const {return Params;}

	void SetPropData(CXmlFile &xml,const char* szSectionName, const std::map<int,std::vector<std::string> > &eventParamsMap, StringBuffer& stringBuffer)
	{
		_STRING_BUFFER_INIT_;

		SetID(szSectionName);
		SetEventType(xml.ReadInteger(szSectionName,"EventType",0));
		SetTargetType(xml.ReadInteger(szSectionName,"TargetType",0));
		SetFuncNo(xml.ReadInteger(szSectionName,"FuncNo",0));

		Params.Init(4, 4, &stringBuffer);
		Params << szSectionName;
		std::map<int,std::vector<std::string> >::const_iterator it = eventParamsMap.find(iFuncNo);
		if (it != eventParamsMap.end())
		{
			const std::vector<std::string> &propname = (*it).second;
			LoopBeginCheck(a);
			for (size_t j = 0; j < propname.size(); ++j)
			{
				LoopDoCheck(a);
				Params<<xml.ReadString(szSectionName,propname[j].c_str(),"");
			}
		}
	}

	void SetPropData(IKernel* pKernel,const PERSISTID& skill) const
	{
	}

private:
	StringArray Params;	//�����б�
}; // end of struct SkillEffectData

//////////////////////////////////////////////////////////////////////////
/// buffer data struct defined
struct BuffBaseData
{
	_STRING_BUFFER_DEFINED_;

	DECLARE_INLINE_STRING_PROP_VARIANT(ID);//Buff���
	DECLARE_INLINE_INT_PROP_VARIANT(LifeTimeType); //BUFF����ʱ�����
	DECLARE_INLINE_INT_PROP_VARIANT(EffectType); //��������(BUFFER_EFFECT_TYPE)
	DECLARE_INLINE_INT_PROP_VARIANT(EffectTimer); //����ʱ��
	DECLARE_INLINE_INT_PROP_VARIANT(AffectType); // buff��������EBufferAffectType
	DECLARE_INLINE_INT_PROP_VARIANT(ReplaceCategory); //�滻�����
	DECLARE_INLINE_INT_PROP_VARIANT(ReplaceType); //�滻���
	DECLARE_INLINE_INT_PROP_VARIANT(OffLineClear); //�����Ƿ����
	DECLARE_INLINE_INT_PROP_VARIANT(ChangeSceneClear); //�糡���Ƿ����
	DECLARE_INLINE_INT_PROP_VARIANT(SkillBeginClear); //���ܿ�ʼ�Ƿ����
	DECLARE_INLINE_INT_PROP_VARIANT(DeadClear); //�����Ƿ����
	DECLARE_INLINE_STRING_PROP_VARIANT(BuffDisplay); //��Ч�ļ����
	DECLARE_INLINE_CVARLIST_PROP_VARIANT(BuffEffect); //����Ч��
	DECLARE_INLINE_STRING_PROP_VARIANT(BasePackage); //�������԰�
	DECLARE_INLINE_INT_PROP_VARIANT(CalculateType); //��������(EModifyPackComputeType)
	DECLARE_INLINE_BOOL_PROP_VARIANT(IsModifyProp); //�Ƿ�Ҫ�޸Ķ�������
public:
	void SetPropData(CXmlFile &xml, const char* szSectionName, StringBuffer& stringBuffer)
	{
		_STRING_BUFFER_INIT_;

		SetID(szSectionName);
		SetLifeTimeType(xml.ReadInteger(szSectionName, "LifeTimeType", 0));
		SetEffectType(xml.ReadInteger(szSectionName, "EffectType", 0));
		SetEffectTimer(xml.ReadInteger(szSectionName, "EffectTimer", 0));
		SetAffectType(xml.ReadInteger(szSectionName, "AffectType", 0));
		SetReplaceCategory(xml.ReadInteger(szSectionName, "ReplaceCategory", 0));
		SetReplaceType(xml.ReadInteger(szSectionName, "ReplaceType", 0));
		SetOffLineClear(xml.ReadInteger(szSectionName, "OffLineClear", 0));
		SetChangeSceneClear(xml.ReadInteger(szSectionName, "ChangeSceneClear", 0));
		SetSkillBeginClear(xml.ReadInteger(szSectionName, "SkillBeginClear", 0));
		SetDeadClear(xml.ReadInteger(szSectionName, "DeadClear", 0));
		SetBuffDisplay(xml.ReadString(szSectionName, "BuffDisplay", ""));
		SetBuffEffect(xml.ReadString(szSectionName,"BuffEffect",""), ",");
		SetBasePackage(xml.ReadString(szSectionName, "BasePackage", ""));
		SetCalculateType(xml.ReadInteger(szSectionName, "CalculateType", 0));
		SetIsModifyProp(xml.ReadInteger(szSectionName, "IsModifyProp", 1));
	}
};	// end of struct BuffBaseData

struct BuffEffectData 
{
	_STRING_BUFFER_DEFINED_;

	DECLARE_INLINE_STRING_PROP_VARIANT(ID);//Ч�����
	DECLARE_INLINE_INT_PROP_VARIANT(EventType); // EBufferEventType
	DECLARE_INLINE_INT_PROP_VARIANT(FuncNo); //�����¼����ܺ�
public:
	inline const StringArray &GetParams() const {return Params;}
	void SetPropData(CXmlFile &xml,const char* szSectionName, const std::map<int,std::vector<std::string> > &eventParamsMap, StringBuffer& stringBuffer)
	{
		_STRING_BUFFER_INIT_;

		SetID(szSectionName);
		SetEventType(xml.ReadInteger(szSectionName, "EventType", 0));
		SetFuncNo(xml.ReadInteger(szSectionName, "FuncNo", 0));
		Params.Init(4, 4, &stringBuffer);
		Params << szSectionName;
		std::map<int,std::vector<std::string> >::const_iterator it = eventParamsMap.find(iFuncNo);
		if (it != eventParamsMap.end())
		{
			const std::vector<std::string> &propname = (*it).second;
			LoopBeginCheck(a);
			for (size_t j = 0; j < propname.size(); ++j)
			{
				LoopDoCheck(a);
				Params<<xml.ReadString(szSectionName,propname[j].c_str(),"");
			}
		}
	}
private:
	StringArray Params;	//�����б�
}; // end of struct BuffEffectData
#endif
