//-------------------------------------------------------------------- 
// �ļ���:		Fields.h 
// ��  ��:		�ֶ������� 
// ˵  ��:		�ֶ����ƶ��� 
// ������:		 
// ��  ��:       
//-------------------------------------------------------------------- 
#ifndef _FIELDS_DEFINE_H_ 
#define _FIELDS_DEFINE_H_ 
 
// �ֶζ���=============================================================== 
 
//<property name="PathFinding"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="�Ƿ�Ѱ·��"/> 
#define FIELD_PROP_PATH_FINDING			"PathFinding" 
 
//<property name="FindPathX"  
// 	type="float"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="��ǰĿ���X"/> 
#define FIELD_PROP_FIND_PATH_X			"FindPathX" 
 
//<property name="FindPathZ"  
// 	type="float"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="��ǰĿ���Z"/> 
#define FIELD_PROP_FIND_PATH_Z			"FindPathZ" 
 
//<property name="DestPathX"  
// 	type="float"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="����Ŀ��X"/> 
#define FIELD_PROP_DEST_PATH_X			"DestPathX" 
 
//<property name="DestPathZ"  
// 	type="float"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="����Ŀ��Z"/> 
#define FIELD_PROP_DEST_PATH_Z			"DestPathZ" 
 
//<property name="LittleStepX"  
// 	type="float"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="�鲽�ƶ�Ŀ��X"/> 
#define FIELD_PROP_LITTLE_STEP_X			"LittleStepX" 
 
//<property name="LittleStepZ"  
// 	type="float"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="�鲽�ƶ�Ŀ��Z"/> 
#define FIELD_PROP_LITTLE_STEP_Z			"LittleStepZ" 
 
//<property name="LastFindPath"  
// 	type="Int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="����Ѱ·"/> 
#define FIELD_PROP_LAST_FIND_PATH			"LastFindPath" 
 
//<property name="RetryPrePath"  
// 	type="Int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="ָ������ǰ���ԣ�֮��ƫ�ƶ�̬��·"/> 
#define FIELD_PROP_RETRY_PRE_PATH			"RetryPrePath" 
 
//<property name="PathFindStep"  
// 	type="Int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc=""/> 
#define FIELD_PROP_PATH_FIND_STEP			"PathFindStep" 
 
//<property name="ContinueMove"  
// 	type="Int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="�����鲽��־,0��1��"/> 
#define FIELD_PROP_CONTINUE_MOVE			"ContinueMove" 
 
//<property name="TargetX"  
// 	type="float"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="Ŀ���X"/> 
#define FIELD_PROP_TARGET_X			"TargetX" 
 
//<property name="TargetZ"  
// 	type="float"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="Ŀ���Z"/> 
#define FIELD_PROP_TARGET_Z			"TargetZ" 
 
//<property name="NearTargetGrid"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="Ŀ�긽���ĵ�"/> 
#define FIELD_PROP_NEAR_TARGET_GRID			"NearTargetGrid" 
 
//<property name="MotionState"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="Ѱ·�ƶ�״̬"/> 
#define FIELD_PROP_MOTION_STATE			"MotionState" 
 
//<property name="RoundGrid"  
// 	type="Int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="����ʱ���ڸ���"/> 
#define FIELD_PROP_ROUND_GRID			"RoundGrid" 
 
//<property name="PathDistanceMin"  
// 	type="float"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="��Ŀ����С����"/> 
#define FIELD_PROP_PATH_DISTANCE_MIN			"PathDistanceMin" 
 
//<property name="PathDistanceMax"  
// 	type="float"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="��Ŀ��������"/> 
#define FIELD_PROP_PATH_DISTANCE_MAX			"PathDistanceMax" 
 
//<property name="MotionResult"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="�ƶ����"/> 
#define FIELD_PROP_MOTION_RESULT			"MotionResult" 
 
//<property name="WaitTarget"  
// 	type="int64"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="�ȴ������߿�"/> 
#define FIELD_PROP_WAIT_TARGET			"WaitTarget" 
 
//<property name="WaitTargetX"  
// 	type="float"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="�ȴ������߿��������X"/> 
#define FIELD_PROP_WAIT_TARGET_X			"WaitTargetX" 
 
//<property name="WaitTargetZ"  
// 	type="float"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="�ȴ������߿��������Z"/> 
#define FIELD_PROP_WAIT_TARGET_Z			"WaitTargetZ" 
 
//<property name="RoundTotalCount"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="���õ��ܴ���"/> 
#define FIELD_PROP_ROUND_TOTAL_COUNT			"RoundTotalCount" 
 
//<property name="SpaceGrid"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="�������"/> 
#define FIELD_PROP_SPACE_GRID			"SpaceGrid" 
 
//<property name="CallBackFunction"  
// 	type="string"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="�ص�����"/> 
#define FIELD_PROP_CALL_BACK_FUNCTION			"CallBackFunction" 
 
//<property name="MotionMethod"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="Ѱ·��ʽ"/> 
#define FIELD_PROP_MOTION_METHOD			"MotionMethod" 
 
//<property name="SelfLastX"  
// 	type="float"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="�����ϴ������X"/> 
#define FIELD_PROP_SELF_LAST_X			"SelfLastX" 
 
//<property name="SelfLastZ"  
// 	type="float"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="�����ϴ������Z"/> 
#define FIELD_PROP_SELF_LAST_Z			"SelfLastZ" 
 
//<property name="Name"  
// 	type="string"  
//	public="false"  
//	private="false"  
//	save="true"  
//	desc="����"/> 
#define FIELD_PROP_NAME			"Name" 
 
//<property name="BaseCap"  
// 	type="int"  
//	public="false"  
//	private="true"  
//	save="false"  
//	desc="��������, Ϊ0ʱʹ���������"/> 
#define FIELD_PROP_BASE_CAP			"BaseCap" 
 
//<property name="CantAdd"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="�Ƿ��������Ʒ��������"/> 
#define FIELD_PROP_CANT_ADD			"CantAdd" 
 
//<property name="CantRemove"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="�Ƿ��ܽ���Ʒ������������"/> 
#define FIELD_PROP_CANT_REMOVE			"CantRemove" 
 
//<property name="CantArrange"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="�Ƿ���ִ���������"/> 
#define FIELD_PROP_CANT_ARRANGE			"CantArrange" 
 
//<property name="CanOP"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="��ҿɲ��������ı�־"/> 
#define FIELD_PROP_CAN_OP			"CanOP" 
 
//<property name="LockedSize"  
// 	type="int"  
//	public="false"  
//	private="true"  
//	save="true"  
//	desc="�����ĸ�����"/> 
#define FIELD_PROP_LOCKED_SIZE			"LockedSize" 
 
//<property name="EquipType"  
// 	type="string"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="װ����λ����, equipment_config.xml�ļ��еĶ���"/> 
#define FIELD_PROP_EQUIP_TYPE			"EquipType" 
 
//<property name="BasePackage"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="�������԰�"/> 
#define FIELD_PROP_BASE_PACKAGE			"BasePackage" 
 
//<property name="StrPackage"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="ǿ��������"/> 
#define FIELD_PROP_STR_PACKAGE			"StrPackage" 
 
//<property name="AddPackage"  
// 	type="int"  
//	public="false"  
//	private="true"  
//	save="true"  
//	desc="�������԰�"/> 
#define FIELD_PROP_ADD_PACKAGE			"AddPackage" 
 
//<property name="ClassLimit"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="ְҵ����"/> 
#define FIELD_PROP_CLASS_LIMIT			"ClassLimit" 
 
//<property name="ModifyLevel"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="������ȼ�����"/> 
#define FIELD_PROP_MODIFY_LEVEL			"ModifyLevel" 
 
//<property name="ModifyAddPackage"  
// 	type="string"  
//	public="false"  
//	private="true"  
//	save="true"  
//	desc="װ�����������������ֵ"/> 
#define FIELD_PROP_MODIFY_ADD_PACKAGE			"ModifyAddPackage" 
 
//<property name="EquipNotifyFlag"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="true"  
//	desc="���װ�����֪ͨ����(��λ����)"/> 
#define FIELD_PROP_EQUIP_NOTIFY_FLAG			"EquipNotifyFlag" 
 
//<property name="ApperanceM"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="true"  
//	desc="��װ�����id"/> 
#define FIELD_PROP_APPERANCE_M			"ApperanceM" 
 
//<property name="ApperanceW"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="true"  
//	desc="Ůװ�����id"/> 
#define FIELD_PROP_APPERANCE_W			"ApperanceW" 
 
//<property name="FashionM"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="true"  
//	desc="��ʱװ���id"/> 
#define FIELD_PROP_FASHION_M			"FashionM" 
 
//<property name="FashionW"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="true"  
//	desc="Ůʱװ���id"/> 
#define FIELD_PROP_FASHION_W			"FashionW" 
 
//<property name="Config"  
// 	type="string"  
//	public="false"  
//	private="true"  
//	save="true"  
//	desc="���"/> 
#define FIELD_PROP_CONFIG			"Config" 
 
//<property name="Script"  
// 	type="string"  
//	public="false"  
//	private="true"  
//	save="false"  
//	desc="���"/> 
#define FIELD_PROP_SCRIPT			"Script" 
 
//<property name="ColorLevel"  
// 	type="int"  
//	public="false"  
//	private="true"  
//	save="true"  
//	desc="��ɫ�ȼ�"/> 
#define FIELD_PROP_COLOR_LEVEL			"ColorLevel" 
 
//<property name="UniqueID"  
// 	type="string"  
//	public="false"  
//	private="true"  
//	save="true"  
//	desc="Ψһ���"/> 
#define FIELD_PROP_UNIQUE_ID			"UniqueID" 
 
//<property name="Amount"  
// 	type="int"  
//	public="false"  
//	private="true"  
//	save="true"  
//	desc="��ǰ����"/> 
#define FIELD_PROP_AMOUNT			"Amount" 
 
//<property name="ItemType"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="��Ʒ������"/> 
#define FIELD_PROP_ITEM_TYPE			"ItemType" 
 
//<property name="MaxAmount"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="�������"/> 
#define FIELD_PROP_MAX_AMOUNT			"MaxAmount" 
 
//<property name="LimitLevel"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="�ȼ�����"/> 
#define FIELD_PROP_LIMIT_LEVEL			"LimitLevel" 
 
//<property name="SellPrice"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="�ۼ�ͭ��"/> 
#define FIELD_PROP_SELL_PRICE			"SellPrice" 
 
//<property name="SpecialItem"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="�Ƿ�ϡ����Ʒ"/> 
#define FIELD_PROP_SPECIAL_ITEM			"SpecialItem" 
 
//<property name="BindStatus"  
// 	type="int"  
//	public="false"  
//	private="true"  
//	save="true"  
//	desc="��Ʒ�İ�״̬"/> 
#define FIELD_PROP_BIND_STATUS			"BindStatus" 
 
//<property name="Use"  
// 	type="string"  
//	public="false"  
//	private="true"  
//	save="false"  
//	desc="��Ʒ��ʹ������"/> 
#define FIELD_PROP_USE			"Use" 
 
//<property name="CoolDownID"  
// 	type="int"  
//	public="false"  
//	private="true"  
//	save="false"  
//	desc="��Ʒ����ȴID"/> 
#define FIELD_PROP_COOL_DOWN_ID			"CoolDownID" 
 
//<property name="CoolDownTime"  
// 	type="int"  
//	public="false"  
//	private="true"  
//	save="false"  
//	desc="��Ʒ����ȴʱ��(��λ��)"/> 
#define FIELD_PROP_COOL_DOWN_TIME			"CoolDownTime" 
 
//<property name="Purchase"  
// 	type="string"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="����۸�"/> 
#define FIELD_PROP_PURCHASE			"Purchase" 
 
//<property name="RemindAmount"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="true"  
//	desc="�Ѿ�����ʹ�õ�����"/> 
#define FIELD_PROP_REMIND_AMOUNT			"RemindAmount" 
 
//<property name="IsNoNeedLog"  
// 	type="int"  
//	public="false"  
//	private="true"  
//	save="false"  
//	desc="�Ƿ���Ҫ��¼��־"/> 
#define FIELD_PROP_IS_NO_NEED_LOG			"IsNoNeedLog" 
 
//<property name="ItemFrozenTime"  
// 	type="int64"  
//	public="false"  
//	private="true"  
//	save="true"  
//	desc="����ʱ��(����ʱ��֮���ʹ��)"/> 
#define FIELD_PROP_ITEM_FROZEN_TIME			"ItemFrozenTime" 
 
//<property name="BindAble"  
// 	type="int"  
//	public="false"  
//	private="true"  
//	save="false"  
//	desc="��Ʒ�Ƿ�֧�ְ�"/> 
#define FIELD_PROP_BIND_ABLE			"BindAble" 
 
//<property name="Level"  
// 	type="int"  
//	public="false"  
//	private="true"  
//	save="true"  
//	desc="����ȼ�"/> 
#define FIELD_PROP_LEVEL			"Level" 
 
//<property name="Step"  
// 	type="int"  
//	public="false"  
//	private="true"  
//	save="true"  
//	desc="����׼�"/> 
#define FIELD_PROP_STEP			"Step" 
 
//<property name="PointValue"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="��ֵ����"/> 
#define FIELD_PROP_POINT_VALUE			"PointValue" 
 
//<property name="UpMoney"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="������Ҫͭ��"/> 
#define FIELD_PROP_UP_MONEY			"UpMoney" 
 
//<property name="UpLevel"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="������Ҫ�������͵ȼ�"/> 
#define FIELD_PROP_UP_LEVEL			"UpLevel" 
 
//<property name="MountCost"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="��Ƕ�˱�ʯ�ķ���(ͭ��)"/> 
#define FIELD_PROP_MOUNT_COST			"MountCost" 
 
//<property name="ExtirpateCost"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="ժ���˱�ʯ�ķ���(ͭ��)"/> 
#define FIELD_PROP_EXTIRPATE_COST			"ExtirpateCost" 
 
//<property name="Index"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="��ʯ����"/> 
#define FIELD_PROP_INDEX			"Index" 
 
//<property name="BaseAddPkg"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="����������id"/> 
#define FIELD_PROP_BASE_ADD_PKG			"BaseAddPkg" 
 
//<property name="PreRideAddPkg"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="��һ��������id"/> 
#define FIELD_PROP_PRE_RIDE_ADD_PKG			"PreRideAddPkg" 
 
//<property name="RideAddPkg"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="��ǰ������id"/> 
#define FIELD_PROP_RIDE_ADD_PKG			"RideAddPkg" 
 
//<property name="RideActived"  
// 	type="int"  
//	public="false"  
//	private="true"  
//	save="true"  
//	desc="�Ƿ񼤻�"/> 
#define FIELD_PROP_RIDE_ACTIVED			"RideActived" 
 
//<property name="RideStar"  
// 	type="int"  
//	public="false"  
//	private="true"  
//	save="true"  
//	desc="�����Ǽ�"/> 
#define FIELD_PROP_RIDE_STAR			"RideStar" 
 
//<property name="RideStep"  
// 	type="int"  
//	public="false"  
//	private="true"  
//	save="true"  
//	desc="����׼�"/> 
#define FIELD_PROP_RIDE_STEP			"RideStep" 
 
//<property name="DefaultSkin"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="true"  
//	desc="����Ĭ��Ƥ��"/> 
#define FIELD_PROP_DEFAULT_SKIN			"DefaultSkin" 
 
//<property name="CurState"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="��ǰ״̬"/> 
#define FIELD_PROP_CUR_STATE			"CurState" 
 
//<property name="SubState"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="��ǰ״̬����״̬"/> 
#define FIELD_PROP_SUB_STATE			"SubState" 
 
//<property name="AITargetObejct"  
// 	type="object"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="��ǰAIĿ�����"/> 
#define FIELD_PROP_AITARGET_OBEJCT			"AITargetObejct" 
 
//<property name="AITemplate"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="AIģ��"/> 
#define FIELD_PROP_AITEMPLATE			"AITemplate" 
 
//<property name="Initiative"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="�Ƿ�����"/> 
#define FIELD_PROP_INITIATIVE			"Initiative" 
 
//<property name="FightAttackFreq"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="����Ƶ��"/> 
#define FIELD_PROP_FIGHT_ATTACK_FREQ			"FightAttackFreq" 
 
//<property name="LastAttackTime"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="���ϴι��������ڹ�ȥ�Ĺ����������ʱ��(�����ļ���ص��ۼ�ֵ)"/> 
#define FIELD_PROP_LAST_ATTACK_TIME			"LastAttackTime" 
 
//<property name="PatrolMode"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="Ѳ��ģʽ"/> 
#define FIELD_PROP_PATROL_MODE			"PatrolMode" 
 
//<property name="BornX"  
// 	type="float"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="������"/> 
#define FIELD_PROP_BORN_X			"BornX" 
 
//<property name="BornZ"  
// 	type="float"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="������"/> 
#define FIELD_PROP_BORN_Z			"BornZ" 
 
//<property name="BornOrient"  
// 	type="float"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="�����㳯��"/> 
#define FIELD_PROP_BORN_ORIENT			"BornOrient" 
 
//<property name="MinAttackDist"  
// 	type="float"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="��С��������"/> 
#define FIELD_PROP_MIN_ATTACK_DIST			"MinAttackDist" 
 
//<property name="MaxAttackDist"  
// 	type="float"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="��󹥻�����"/> 
#define FIELD_PROP_MAX_ATTACK_DIST			"MaxAttackDist" 
 
//<property name="PathID"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="Ѱ··��ID"/> 
#define FIELD_PROP_PATH_ID			"PathID" 
 
//<property name="Recuperate"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="����ʱ��"/> 
#define FIELD_PROP_RECUPERATE			"Recuperate" 
 
//<property name="EscapeWarningHPRate"  
// 	type="float"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="���ܾ���HP����"/> 
#define FIELD_PROP_ESCAPE_WARNING_HPRATE			"EscapeWarningHPRate" 
 
//<property name="EscapeLastTime"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="������Ϊ�ĳ���ʱ��"/> 
#define FIELD_PROP_ESCAPE_LAST_TIME			"EscapeLastTime" 
 
//<property name="EscapeCD"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="�������ܵ�CD"/> 
#define FIELD_PROP_ESCAPE_CD			"EscapeCD" 
 
//<property name="GlobalStep"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="��ǰ�󲨴ν��е��Ĳ�"/> 
#define FIELD_PROP_GLOBAL_STEP			"GlobalStep" 
 
//<property name="LittleStepMax"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="��ǰ��С���δ���"/> 
#define FIELD_PROP_LITTLE_STEP_MAX			"LittleStepMax" 
 
//<property name="LittlestepNow"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="��ǰ��С���δ���"/> 
#define FIELD_PROP_LITTLESTEP_NOW			"LittlestepNow" 
 
//<property name="SceneId"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="����id"/> 
#define FIELD_PROP_SCENE_ID			"SceneId" 
 
//<property name="MaxScope"  
// 	type="float"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="�����⻷�뾶"/> 
#define FIELD_PROP_MAX_SCOPE			"MaxScope" 
 
//<property name="MinScope"  
// 	type="float"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="�����ڻ��뾶"/> 
#define FIELD_PROP_MIN_SCOPE			"MinScope" 
 
//<property name="SpringCount"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="���봥����Χ������"/> 
#define FIELD_PROP_SPRING_COUNT			"SpringCount" 
 
//<property name="CreateType"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="0-�����󴴽�NPC��1-�������λ�þ�ӢNPC��2-�������λ�þ�Ӣ�����ι���"/> 
#define FIELD_PROP_CREATE_TYPE			"CreateType" 
 
//<property name="TotalStep"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="����Ĳ���"/> 
#define FIELD_PROP_TOTAL_STEP			"TotalStep" 
 
//<property name="CurStep"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="��ǰ����"/> 
#define FIELD_PROP_CUR_STEP			"CurStep" 
 
//<property name="RandRow"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="���������"/> 
#define FIELD_PROP_RAND_ROW			"RandRow" 
 
//<property name="RandHelper"  
// 	type="object"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="����ĸ���������"/> 
#define FIELD_PROP_RAND_HELPER			"RandHelper" 
 
//<property name="MonsterAmount"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="��������"/> 
#define FIELD_PROP_MONSTER_AMOUNT			"MonsterAmount" 
 
//<property name="IsDestroy"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="�������������Ƿ�����"/> 
#define FIELD_PROP_IS_DESTROY			"IsDestroy" 
 
//<property name="SceneAction"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="�ؿ��ؾ��н�����"/> 
#define FIELD_PROP_SCENE_ACTION			"SceneAction" 
 
//<property name="NpcID"  
// 	type="string"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="���ɵ�NPC��Id"/> 
#define FIELD_PROP_NPC_ID			"NpcID" 
 
//<property name="CurAmount"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="��ǰ����"/> 
#define FIELD_PROP_CUR_AMOUNT			"CurAmount" 
 
//<property name="RefreshPeriod"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="ˢ��ʱ������,��λΪ��"/> 
#define FIELD_PROP_REFRESH_PERIOD			"RefreshPeriod" 
 
//<property name="Limit"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="ɱ���ٹ�ˢ����һ��"/> 
#define FIELD_PROP_LIMIT			"Limit" 
 
//<property name="TranslateSceneID"  
// 	type="string"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="���ͳ�����ţ���ʽ��"/> 
#define FIELD_PROP_TRANSLATE_SCENE_ID			"TranslateSceneID" 
 
//<property name="TranslatePosX"  
// 	type="float"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="����X"/> 
#define FIELD_PROP_TRANSLATE_POS_X			"TranslatePosX" 
 
//<property name="TranslatePosY"  
// 	type="float"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="����Y"/> 
#define FIELD_PROP_TRANSLATE_POS_Y			"TranslatePosY" 
 
//<property name="TranslatePosZ"  
// 	type="float"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="����Z"/> 
#define FIELD_PROP_TRANSLATE_POS_Z			"TranslatePosZ" 
 
//<property name="TranslateAy"  
// 	type="float"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="���ͳ���"/> 
#define FIELD_PROP_TRANSLATE_AY			"TranslateAy" 
 
//<property name="OwnSkills"  
// 	type="string"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="ӵ�еļ��ܣ���ͨ���ܳ��⣩"/> 
#define FIELD_PROP_OWN_SKILLS			"OwnSkills" 
 
//<property name="ChaseRange"  
// 	type="float"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="׷��뾶"/> 
#define FIELD_PROP_CHASE_RANGE			"ChaseRange" 
 
//<property name="PatrolRange"  
// 	type="float"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="Ѳ�߰뾶"/> 
#define FIELD_PROP_PATROL_RANGE			"PatrolRange" 
 
//<property name="DropType"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="�������"/> 
#define FIELD_PROP_DROP_TYPE			"DropType" 
 
//<property name="DropToKillerCnt"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="���˺���������"/> 
#define FIELD_PROP_DROP_TO_KILLER_CNT			"DropToKillerCnt" 
 
//<property name="ExpAward"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="����"/> 
#define FIELD_PROP_EXP_AWARD			"ExpAward" 
 
//<property name="SkillStr"  
// 	type="string"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="����"/> 
#define FIELD_PROP_SKILL_STR			"SkillStr" 
 
//<property name="NpcBasePackage"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="�������԰�"/> 
#define FIELD_PROP_NPC_BASE_PACKAGE			"NpcBasePackage" 
 
//<property name="DropID"  
// 	type="string"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="����ID, ֧���ö��ŷָ��Ķ��ID"/> 
#define FIELD_PROP_DROP_ID			"DropID" 
 
//<property name="MinCapitalAward"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="�ʽ�������"/> 
#define FIELD_PROP_MIN_CAPITAL_AWARD			"MinCapitalAward" 
 
//<property name="MaxCapitalAward"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="�ʽ�������"/> 
#define FIELD_PROP_MAX_CAPITAL_AWARD			"MaxCapitalAward" 
 
//<property name="BornState"  
// 	type="int"  
//	public="true"  
//	private="true"  
//	save="false"  
//	desc="����״̬,1�ڳ���״̬��"/> 
#define FIELD_PROP_BORN_STATE			"BornState" 
 
//<property name="BornTime"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="��������ʱ��"/> 
#define FIELD_PROP_BORN_TIME			"BornTime" 
 
//<property name="BornStand"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="������Ѳ��"/> 
#define FIELD_PROP_BORN_STAND			"BornStand" 
 
//<property name="TowerRatio"  
// 	type="float"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="�˺�ֵ�ӳ�(�����ֵ���)"/> 
#define FIELD_PROP_TOWER_RATIO			"TowerRatio" 
 
//<property name="DropMeetLevel"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="����͹���ȼ������ڵ��ھͲ�����"/> 
#define FIELD_PROP_DROP_MEET_LEVEL			"DropMeetLevel" 
 
//<property name="FightMoveRate"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="ս���ƶ���������"/> 
#define FIELD_PROP_FIGHT_MOVE_RATE			"FightMoveRate" 
 
//<property name="FightMoveDis"  
// 	type="float"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="ս���ƶ��ķ�Χ"/> 
#define FIELD_PROP_FIGHT_MOVE_DIS			"FightMoveDis" 
 
//<property name="CantRestoreHP"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="��ս���ָܻ���Ѫ"/> 
#define FIELD_PROP_CANT_RESTORE_HP			"CantRestoreHP" 
 
//<property name="NpcDeadTime"  
// 	type="int"  
//	public="true"  
//	private="true"  
//	save="false"  
//	desc="����������ʱ��"/> 
#define FIELD_PROP_NPC_DEAD_TIME			"NpcDeadTime" 
 
//<property name="NpcPackageLevel"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="���԰��ȼ�"/> 
#define FIELD_PROP_NPC_PACKAGE_LEVEL			"NpcPackageLevel" 
 
//<property name="NpcType"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="NPC���� 0-200NormalNpc,201-400StaticNpc"/> 
#define FIELD_PROP_NPC_TYPE			"NpcType" 
 
//<property name="SafeRule"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="��������"/> 
#define FIELD_PROP_SAFE_RULE			"SafeRule" 
 
//<property name="Master"  
// 	type="object"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="����"/> 
#define FIELD_PROP_MASTER			"Master" 
 
//<property name="Skill"  
// 	type="object"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="����"/> 
#define FIELD_PROP_SKILL			"Skill" 
 
//<property name="SpringDestroy"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="�����Ƿ�ʹݻ�"/> 
#define FIELD_PROP_SPRING_DESTROY			"SpringDestroy" 
 
//<property name="SpringType"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="������������"/> 
#define FIELD_PROP_SPRING_TYPE			"SpringType" 
 
//<property name="SpringRelation"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="������ϵ����"/> 
#define FIELD_PROP_SPRING_RELATION			"SpringRelation" 
 
//<property name="SpringEvent"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="�����¼�"/> 
#define FIELD_PROP_SPRING_EVENT			"SpringEvent" 
 
//<property name="EventValue"  
// 	type="string"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="�˺���ֵ"/> 
#define FIELD_PROP_EVENT_VALUE			"EventValue" 
 
//<property name="EventRange"  
// 	type="float"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="�ݻپ���"/> 
#define FIELD_PROP_EVENT_RANGE			"EventRange" 
 
//<property name="EventCount"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="Ⱥ�崥��������"/> 
#define FIELD_PROP_EVENT_COUNT			"EventCount" 
 
//<property name="EndSpringEvent"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="�����¼�"/> 
#define FIELD_PROP_END_SPRING_EVENT			"EndSpringEvent" 
 
//<property name="DestroyEvent"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="�ݻ��¼�"/> 
#define FIELD_PROP_DESTROY_EVENT			"DestroyEvent" 
 
//<property name="SpringEffect"  
// 	type="string"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="������Ч"/> 
#define FIELD_PROP_SPRING_EFFECT			"SpringEffect" 
 
//<property name="Resource"  
// 	type="string"  
//	public="true"  
//	private="false"  
//	save="false"  
//	desc="����Ч��"/> 
#define FIELD_PROP_RESOURCE			"Resource" 
 
//<property name="ResourceB"  
// 	type="string"  
//	public="true"  
//	private="false"  
//	save="false"  
//	desc="������Ч��"/> 
#define FIELD_PROP_RESOURCE_B			"ResourceB" 
 
//<property name="EffectTime"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="��Ч����ʱ��"/> 
#define FIELD_PROP_EFFECT_TIME			"EffectTime" 
 
//<property name="TrapRatio"  
// 	type="float"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="�˺�ֵ�ӳ�(�����ֵ���)"/> 
#define FIELD_PROP_TRAP_RATIO			"TrapRatio" 
 
//<property name="DelayTime"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="�ӳٴ���"/> 
#define FIELD_PROP_DELAY_TIME			"DelayTime" 
 
//<property name="ArenaFlag"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="��������ʶ"/> 
#define FIELD_PROP_ARENA_FLAG			"ArenaFlag" 
 
//<property name="ArenaChanceResetDate"  
// 	type="string"  
//	public="false"  
//	private="false"  
//	save="true"  
//	desc="��������������������(YYYY-MM-DD)"/> 
#define FIELD_PROP_ARENA_CHANCE_RESET_DATE			"ArenaChanceResetDate" 
 
//<property name="ArenaChance"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="true"  
//	desc="���վ���������"/> 
#define FIELD_PROP_ARENA_CHANCE			"ArenaChance" 
 
//<property name="ArenaChanceUsed"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="true"  
//	desc="���վ������Ѿ���ս����"/> 
#define FIELD_PROP_ARENA_CHANCE_USED			"ArenaChanceUsed" 
 
//<property name="ArenaChallengeeObj"  
// 	type="Object"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="��ս����"/> 
#define FIELD_PROP_ARENA_CHALLENGEE_OBJ			"ArenaChallengeeObj" 
 
//<property name="ArenaChallengeeInfo"  
// 	type="string"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="��ս������Ϣ"/> 
#define FIELD_PROP_ARENA_CHALLENGEE_INFO			"ArenaChallengeeInfo" 
 
//<property name="ArenaCurvRankList"  
// 	type="string"  
//	public="false"  
//	private="false"  
//	save="true"  
//	desc="��һ�εľ�������"/> 
#define FIELD_PROP_ARENA_CURV_RANK_LIST			"ArenaCurvRankList" 
 
//<property name="ArenaSN"  
// 	type="string"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="��ǰ����������ˮ�ţ�ÿ��ս���������µı������"/> 
#define FIELD_PROP_ARENA_SN			"ArenaSN" 
 
//<property name="HighestArenaRank"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="true"  
//	desc="��õ��������"/> 
#define FIELD_PROP_HIGHEST_ARENA_RANK			"HighestArenaRank" 
 
//<property name="BattleAbility"  
// 	type="int"  
//	public="false"  
//	private="true"  
//	save="false"  
//	desc="ս����"/> 
#define FIELD_PROP_BATTLE_ABILITY			"BattleAbility" 
 
//<property name="HighestBattleAbility"  
// 	type="int"  
//	public="false"  
//	private="true"  
//	save="true"  
//	desc="��ʷ���ս����"/> 
#define FIELD_PROP_HIGHEST_BATTLE_ABILITY			"HighestBattleAbility" 
 
//<property name="BeforeGroup"  
// 	type="word"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="�������ǰ�ķ����"/> 
#define FIELD_PROP_BEFORE_GROUP			"BeforeGroup" 
 
//<property name="SumFlower"  
// 	type="int"  
//	public="false"  
//	private="true"  
//	save="true"  
//	desc="��ʷ�ջ�"/> 
#define FIELD_PROP_SUM_FLOWER			"SumFlower" 
 
//<property name="WeekFlower"  
// 	type="int"  
//	public="false"  
//	private="true"  
//	save="true"  
//	desc="�����ջ�"/> 
#define FIELD_PROP_WEEK_FLOWER			"WeekFlower" 
 
//<property name="DailyIntimacyAward"  
// 	type="int"  
//	public="false"  
//	private="true"  
//	save="true"  
//	desc="���ܶ���ȡ״̬"/> 
#define FIELD_PROP_DAILY_INTIMACY_AWARD			"DailyIntimacyAward" 
 
//<property name="GuildName"  
// 	type="widestr"  
//	public="true"  
//	private="true"  
//	save="true"  
//	desc="��������"/> 
#define FIELD_PROP_GUILD_NAME			"GuildName" 
 
//<property name="GuildPosition"  
// 	type="int"  
//	public="true"  
//	private="true"  
//	save="true"  
//	desc="����е�ְλ"/> 
#define FIELD_PROP_GUILD_POSITION			"GuildPosition" 
 
//<property name="QuitGuildDate"  
// 	type="int64"  
//	public="false"  
//	private="true"  
//	save="true"  
//	desc="�˳���������"/> 
#define FIELD_PROP_QUIT_GUILD_DATE			"QuitGuildDate" 
 
//<property name="GuildListBeginRow"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="��ȡ�����б����ʼ��"/> 
#define FIELD_PROP_GUILD_LIST_BEGIN_ROW			"GuildListBeginRow" 
 
//<property name="GuildLevel"  
// 	type="int"  
//	public="false"  
//	private="true"  
//	save="true"  
//	desc="���ȼ�"/> 
#define FIELD_PROP_GUILD_LEVEL			"GuildLevel" 
 
//<property name="OneKeyJoinTime"  
// 	type="int64"  
//	public="false"  
//	private="true"  
//	save="true"  
//	desc="һ������ʱ��"/> 
#define FIELD_PROP_ONE_KEY_JOIN_TIME			"OneKeyJoinTime" 
 
//<property name="GuildMutualTimes"  
// 	type="int"  
//	public="false"  
//	private="true"  
//	save="true"  
//	desc="��֯��������"/> 
#define FIELD_PROP_GUILD_MUTUAL_TIMES			"GuildMutualTimes" 
 
//<property name="GuildLastIssueMutualTimes"  
// 	type="int64"  
//	public="false"  
//	private="true"  
//	save="true"  
//	desc="��һ�η�����֯����ʱ��"/> 
#define FIELD_PROP_GUILD_LAST_ISSUE_MUTUAL_TIMES			"GuildLastIssueMutualTimes" 
 
//<property name="GuildShortName"  
// 	type="widestr"  
//	public="true"  
//	private="true"  
//	save="true"  
//	desc="��֯����"/> 
#define FIELD_PROP_GUILD_SHORT_NAME			"GuildShortName" 
 
//<property name="GuildIdentifying"  
// 	type="int"  
//	public="true"  
//	private="true"  
//	save="true"  
//	desc="��֯��ʶ"/> 
#define FIELD_PROP_GUILD_IDENTIFYING			"GuildIdentifying" 
 
//<property name="GuildDonateTimes"  
// 	type="int"  
//	public="true"  
//	private="true"  
//	save="true"  
//	desc="��֯���״���"/> 
#define FIELD_PROP_GUILD_DONATE_TIMES			"GuildDonateTimes" 
 
//<property name="GuildDailyGif"  
// 	type="int"  
//	public="true"  
//	private="true"  
//	save="true"  
//	desc="��ȡ��֯�����"/> 
#define FIELD_PROP_GUILD_DAILY_GIF			"GuildDailyGif" 
 
//<property name="ActiveJade"  
// 	type="object"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="��ǰ���������"/> 
#define FIELD_PROP_ACTIVE_JADE			"ActiveJade" 
 
//<property name="ActiveJadeId"  
// 	type="string"  
//	public="false"  
//	private="true"  
//	save="true"  
//	desc="��ǰ���������id"/> 
#define FIELD_PROP_ACTIVE_JADE_ID			"ActiveJadeId" 
 
//<property name="PlayerExp"  
// 	type="int64"  
//	public="false"  
//	private="true"  
//	save="true"  
//	desc="����"/> 
#define FIELD_PROP_PLAYER_EXP			"PlayerExp" 
 
//<property name="PlayerUpgradeExp"  
// 	type="int64"  
//	public="false"  
//	private="true"  
//	save="false"  
//	desc="������Ҫ����"/> 
#define FIELD_PROP_PLAYER_UPGRADE_EXP			"PlayerUpgradeExp" 
 
//<property name="ItemKNExpRate"  
// 	type="byte"  
//	public="false"  
//	private="false"  
//	save="true"  
//	desc="��Ʒ���ӵ�ɱ�ֶ౶����"/> 
#define FIELD_PROP_ITEM_KNEXP_RATE			"ItemKNExpRate" 
 
//<property name="KNExpRate"  
// 	type="float"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="ɱ�־������"/> 
#define FIELD_PROP_KNEXP_RATE			"KNExpRate" 
 
//<property name="WorldLevelAddExp"  
// 	type="int"  
//	public="false"  
//	private="true"  
//	save="true"  
//	desc="����ȼ���ǰ���Ӿ���ֵ"/> 
#define FIELD_PROP_WORLD_LEVEL_ADD_EXP			"WorldLevelAddExp" 
 
//<property name="WorldLevelTotalExp"  
// 	type="int"  
//	public="false"  
//	private="true"  
//	save="true"  
//	desc="����ȼ������ܾ���ֵ"/> 
#define FIELD_PROP_WORLD_LEVEL_TOTAL_EXP			"WorldLevelTotalExp" 
 
//<property name="AsynWait"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="�������첽�ȴ�����"/> 
#define FIELD_PROP_ASYN_WAIT			"AsynWait" 
 
//<property name="AsynWaitBeginTime"  
// 	type="int"  
//	public="false"  
//	private=""  
//	save="false"  
//	desc="�������첽�ȴ�ʱ��"/> 
#define FIELD_PROP_ASYN_WAIT_BEGIN_TIME			"AsynWaitBeginTime" 
 
//<property name="SkillPoint"  
// 	type="DWORD"  
//	public="false"  
//	private="true"  
//	save="true"  
//	desc="���ܵ�"/> 
#define FIELD_PROP_SKILL_POINT			"SkillPoint" 
 
//<property name="VipLevel"  
// 	type="int"  
//	public="true"  
//	private="true"  
//	save="true"  
//	desc="VIP�ȼ�"/> 
#define FIELD_PROP_VIP_LEVEL			"VipLevel" 
 
//<property name="VipExp"  
// 	type="int"  
//	public="false"  
//	private="true"  
//	save="true"  
//	desc="�ۼƳ�ֵ ���ܵ���Ӱ��"/> 
#define FIELD_PROP_VIP_EXP			"VipExp" 
 
//<property name="TotalPay"  
// 	type="float"  
//	public="false"  
//	private="true"  
//	save="true"  
//	desc="�ۼƳ�ֵ ���ܵ���Ӱ��"/> 
#define FIELD_PROP_TOTAL_PAY			"TotalPay" 
 
//<property name="TotalPayReal"  
// 	type="float"  
//	public="false"  
//	private="false"  
//	save="true"  
//	desc="�ۼƳ�ֵ ��ʵ��ֵ���"/> 
#define FIELD_PROP_TOTAL_PAY_REAL			"TotalPayReal" 
 
//<property name="PayTimes"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="true"  
//	desc="�ܳ�ֵ����"/> 
#define FIELD_PROP_PAY_TIMES			"PayTimes" 
 
//<property name="BoughtVipItem"  
// 	type="int64"  
//	public="false"  
//	private="true"  
//	save="true"  
//	desc="�Ѿ������VIP���"/> 
#define FIELD_PROP_BOUGHT_VIP_ITEM			"BoughtVipItem" 
 
//<property name="PickedVipItem"  
// 	type="int64"  
//	public="false"  
//	private="true"  
//	save="true"  
//	desc="�Ѿ���ȡ��VIP���"/> 
#define FIELD_PROP_PICKED_VIP_ITEM			"PickedVipItem" 
 
//<property name="WeekCardEndDate"  
// 	type="int64"  
//	public="false"  
//	private="true"  
//	save="true"  
//	desc="�ܿ���ֹ����"/> 
#define FIELD_PROP_WEEK_CARD_END_DATE			"WeekCardEndDate" 
 
//<property name="MonthCardEndDate"  
// 	type="int64"  
//	public="false"  
//	private="true"  
//	save="true"  
//	desc="�¿���ֹ����"/> 
#define FIELD_PROP_MONTH_CARD_END_DATE			"MonthCardEndDate" 
 
//<property name="QuarterCardEndDate"  
// 	type="int64"  
//	public="false"  
//	private="true"  
//	save="true"  
//	desc="������ֹ����"/> 
#define FIELD_PROP_QUARTER_CARD_END_DATE			"QuarterCardEndDate" 
 
//<property name="YearCardEndDate"  
// 	type="int64"  
//	public="false"  
//	private="true"  
//	save="true"  
//	desc="�꿨��ֹ����"/> 
#define FIELD_PROP_YEAR_CARD_END_DATE			"YearCardEndDate" 
 
//<property name="ForeverCardState"  
// 	type="int"  
//	public="false"  
//	private="true"  
//	save="true"  
//	desc="�Ƿ�������"/> 
#define FIELD_PROP_FOREVER_CARD_STATE			"ForeverCardState" 
 
//<property name="CardPickedFlag"  
// 	type="int"  
//	public="false"  
//	private="true"  
//	save="true"  
//	desc="ÿ����ȡ��־(�꿨���¿����ܿ�...., ÿһλ��ʾһ�ֿ�, 1Ϊ��ȡ״̬)"/> 
#define FIELD_PROP_CARD_PICKED_FLAG			"CardPickedFlag" 
 
//<property name="VipPickedFlag"  
// 	type="int"  
//	public="false"  
//	private="true"  
//	save="true"  
//	desc="Vipÿ�������ȡ��ʶ"/> 
#define FIELD_PROP_VIP_PICKED_FLAG			"VipPickedFlag" 
 
//<property name="PayedRebate"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="ɾ�����Գ�ֵ����"/> 
#define FIELD_PROP_PAYED_REBATE			"PayedRebate" 
 
//<property name="PayedRebateType"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="ɾ�����Գ�ֵ�������ͣ�web�Ϳͻ��˶���"/> 
#define FIELD_PROP_PAYED_REBATE_TYPE			"PayedRebateType" 
 
//<property name="Invisible"  
// 	type="int"  
//	public="true"  
//	private="true"  
//	save="false"  
//	desc=""/> 
#define FIELD_PROP_INVISIBLE			"Invisible" 
 
//<property name="Uid"  
// 	type="string"  
//	public="false"  
//	private="true"  
//	save="true"  
//	desc="��ɫΨһ��ʶ"/> 
#define FIELD_PROP_UID			"Uid" 
 
//<property name="GroupID"  
// 	type="int"  
//	public="false"  
//	private="true"  
//	save="false"  
//	desc="���鳡�����"/> 
#define FIELD_PROP_GROUP_ID			"GroupID" 
 
//<property name="AccountID"  
// 	type="int"  
//	public="false"  
//	private="true"  
//	save="false"  
//	desc="�ʺű��"/> 
#define FIELD_PROP_ACCOUNT_ID			"AccountID" 
 
//<property name="IssuerID"  
// 	type="int"  
//	public="false"  
//	private="true"  
//	save="false"  
//	desc="��Ӫ�̱��"/> 
#define FIELD_PROP_ISSUER_ID			"IssuerID" 
 
//<property name="Account"  
// 	type="string"  
//	public="false"  
//	private="true"  
//	save="false"  
//	desc="�ʺ�"/> 
#define FIELD_PROP_ACCOUNT			"Account" 
 
//<property name="ServerID"  
// 	type="int"  
//	public="false"  
//	private="true"  
//	save="false"  
//	desc="���������"/> 
#define FIELD_PROP_SERVER_ID			"ServerID" 
 
//<property name="Sex"  
// 	type="byte"  
//	public="true"  
//	private="true"  
//	save="true"  
//	desc="�Ա� 0:����,1:Ů��"/> 
#define FIELD_PROP_SEX			"Sex" 
 
//<property name="Job"  
// 	type="byte"  
//	public="true"  
//	private="true"  
//	save="true"  
//	desc="ְҵ"/> 
#define FIELD_PROP_JOB			"Job" 
 
//<property name="LastObject"  
// 	type="Object"  
//	public="false"  
//	private="true"  
//	save="false"  
//	desc="ѡ�����"/> 
#define FIELD_PROP_LAST_OBJECT			"LastObject" 
 
//<property name="RoleCreateTime"  
// 	type="int64"  
//	public="false"  
//	private="true"  
//	save="true"  
//	desc="����ʱ��"/> 
#define FIELD_PROP_ROLE_CREATE_TIME			"RoleCreateTime" 
 
//<property name="RoleLoginTime"  
// 	type="int64"  
//	public="false"  
//	private="false"  
//	save="true"  
//	desc="��½ʱ��"/> 
#define FIELD_PROP_ROLE_LOGIN_TIME			"RoleLoginTime" 
 
//<property name="RoleLogoutTime"  
// 	type="int64"  
//	public="false"  
//	private="false"  
//	save="true"  
//	desc="�ǳ�ʱ��"/> 
#define FIELD_PROP_ROLE_LOGOUT_TIME			"RoleLogoutTime" 
 
//<property name="LoginDays"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="true"  
//	desc="��½����"/> 
#define FIELD_PROP_LOGIN_DAYS			"LoginDays" 
 
//<property name="ClientType"  
// 	type="string"  
//	public="false"  
//	private="false"  
//	save="true"  
//	desc="�ͻ�������"/> 
#define FIELD_PROP_CLIENT_TYPE			"ClientType" 
 
//<property name="Token"  
// 	type="string"  
//	public="false"  
//	private="false"  
//	save="true"  
//	desc="mac��ַ"/> 
#define FIELD_PROP_TOKEN			"Token" 
 
//<property name="ServerName"  
// 	type="widestr"  
//	public="false"  
//	private="false"  
//	save="true"  
//	desc="��ǰ����������"/> 
#define FIELD_PROP_SERVER_NAME			"ServerName" 
 
//<property name="AppPackage"  
// 	type="string"  
//	public="false"  
//	private="false"  
//	save="true"  
//	desc="����"/> 
#define FIELD_PROP_APP_PACKAGE			"AppPackage" 
 
//<property name="PrepareRole"  
// 	type="byte"  
//	public="false"  
//	private="false"  
//	save="true"  
//	desc="Ԥ������ɫ0:��ͨ 1:������"/> 
#define FIELD_PROP_PREPARE_ROLE			"PrepareRole" 
 
//<property name="AccountUID"  
// 	type="string"  
//	public="false"  
//	private="true"  
//	save="false"  
//	desc="ĳЩ����������UID"/> 
#define FIELD_PROP_ACCOUNT_UID			"AccountUID" 
 
//<property name="PlayerVersion"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="true"  
//	desc="��Ұ汾��"/> 
#define FIELD_PROP_PLAYER_VERSION			"PlayerVersion" 
 
//<property name="CurAction"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="��ҵ�ǰ��Ϊ"/> 
#define FIELD_PROP_CUR_ACTION			"CurAction" 
 
//<property name="StrenSuitPkg"  
// 	type="int"  
//	public="false"  
//	private="true"  
//	save="true"  
//	desc="ǿ����װ���԰�"/> 
#define FIELD_PROP_STREN_SUIT_PKG			"StrenSuitPkg" 
 
//<property name="SilenceTick"  
// 	type="int"  
//	public="false"  
//	private="true"  
//	save="true"  
//	desc="����ʱ�䣨���룩"/> 
#define FIELD_PROP_SILENCE_TICK			"SilenceTick" 
 
//<property name="PKModel"  
// 	type="byte"  
//	public="true"  
//	private="true"  
//	save="false"  
//	desc="PKģʽ"/> 
#define FIELD_PROP_PKMODEL			"PKModel" 
 
//<property name="CriminalValue"  
// 	type="dword"  
//	public="false"  
//	private="true"  
//	save="true"  
//	desc="���ֵ"/> 
#define FIELD_PROP_CRIMINAL_VALUE			"CriminalValue" 
 
//<property name="CrimeState"  
// 	type="byte"  
//	public="true"  
//	private="true"  
//	save="false"  
//	desc="���״̬"/> 
#define FIELD_PROP_CRIME_STATE			"CrimeState" 
 
//<property name="CrimieOnlineTime"  
// 	type="dword"  
//	public="false"  
//	private="true"  
//	save="true"  
//	desc="���״̬����ʱ��"/> 
#define FIELD_PROP_CRIMIE_ONLINE_TIME			"CrimieOnlineTime" 
 
//<property name="DecCrimeScore"  
// 	type="dword"  
//	public="false"  
//	private="true"  
//	save="true"  
//	desc="�������ֵ�Ļ���"/> 
#define FIELD_PROP_DEC_CRIME_SCORE			"DecCrimeScore" 
 
//<property name="ActivatedRide"  
// 	type="int"  
//	public="true"  
//	private="true"  
//	save="true"  
//	desc="��ǰ��˵�Ƥ��ID"/> 
#define FIELD_PROP_ACTIVATED_RIDE			"ActivatedRide" 
 
//<property name="CurMainTask"  
// 	type="dword"  
//	public="false"  
//	private="true"  
//	save="true"  
//	desc="��ǰ���ڽ��е���������ID"/> 
#define FIELD_PROP_CUR_MAIN_TASK			"CurMainTask" 
 
//<property name="TeamID"  
// 	type="int"  
//	public="false"  
//	private="true"  
//	save="true"  
//	desc="������"/> 
#define FIELD_PROP_TEAM_ID			"TeamID" 
 
//<property name="TeamCaptain"  
// 	type="widestr"  
//	public="false"  
//	private="false"  
//	save="true"  
//	desc="�ӳ�"/> 
#define FIELD_PROP_TEAM_CAPTAIN			"TeamCaptain" 
 
//<property name="AutoMatch"  
// 	type="int"  
//	public="false"  
//	private="true"  
//	save="false"  
//	desc="�Զ�ƥ��"/> 
#define FIELD_PROP_AUTO_MATCH			"AutoMatch" 
 
//<property name="WingLevel"  
// 	type="byte"  
//	public="false"  
//	private="true"  
//	save="true"  
//	desc="���ȼ�"/> 
#define FIELD_PROP_WING_LEVEL			"WingLevel" 
 
//<property name="WingStep"  
// 	type="byte"  
//	public="false"  
//	private="true"  
//	save="true"  
//	desc="���׼�"/> 
#define FIELD_PROP_WING_STEP			"WingStep" 
 
//<property name="WingStepBless"  
// 	type="byte"  
//	public="false"  
//	private="true"  
//	save="true"  
//	desc="�������ף��ֵ"/> 
#define FIELD_PROP_WING_STEP_BLESS			"WingStepBless" 
 
//<property name="CantMove"  
// 	type="byte"  
//	public="true"  
//	private="true"  
//	save="false"  
//	desc="�Ƿ��ܹ��ƶ� 0:���ƶ� 1:�����ƶ�"/> 
#define FIELD_PROP_CANT_MOVE			"CantMove" 
 
//<property name="CantRotate"  
// 	type="byte"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="�Ƿ��ܹ�ת�� 0:��ת�� 1:����ת��"/> 
#define FIELD_PROP_CANT_ROTATE			"CantRotate" 
 
//<property name="CantAttack"  
// 	type="byte"  
//	public="false"  
//	private="true"  
//	save="false"  
//	desc="�Ƿ��ܹ�����"/> 
#define FIELD_PROP_CANT_ATTACK			"CantAttack" 
 
//<property name="CantBeAttack"  
// 	type="byte"  
//	public="true"  
//	private="true"  
//	save="false"  
//	desc="�Ƿ��ܹ������� 0:�ɱ����� 1�����ɱ�����"/> 
#define FIELD_PROP_CANT_BE_ATTACK			"CantBeAttack" 
 
//<property name="CantUseSkill"  
// 	type="byte"  
//	public="false"  
//	private="true"  
//	save="false"  
//	desc="�Ƿ��ܹ�ʹ�ü���"/> 
#define FIELD_PROP_CANT_USE_SKILL			"CantUseSkill" 
 
//<property name="ImmunoControlBuff"  
// 	type="byte"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="���߿�����buff"/> 
#define FIELD_PROP_IMMUNO_CONTROL_BUFF			"ImmunoControlBuff" 
 
//<property name="Silent"  
// 	type="byte"  
//	public="false"  
//	private="true"  
//	save="false"  
//	desc="��Ĭ(�����ͷż���,����������ͨ����)"/> 
#define FIELD_PROP_SILENT			"Silent" 
 
//<property name="FixBeDamaged"  
// 	type="byte"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="���̶���ֵ�˺�"/> 
#define FIELD_PROP_FIX_BE_DAMAGED			"FixBeDamaged" 
 
//<property name="GaintState"  
// 	type="byte"  
//	public="true"  
//	private="true"  
//	save="false"  
//	desc="����״̬"/> 
#define FIELD_PROP_GAINT_STATE			"GaintState" 
 
//<property name="StiffState"  
// 	type="byte"  
//	public="false"  
//	private="true"  
//	save="false"  
//	desc="�ܻ�Ӳֱ״̬"/> 
#define FIELD_PROP_STIFF_STATE			"StiffState" 
 
//<property name="Dead"  
// 	type="byte"  
//	public="true"  
//	private="true"  
//	save="true"  
//	desc="����״̬0:δ����,1:������"/> 
#define FIELD_PROP_DEAD			"Dead" 
 
//<property name="BuffDisplayID"  
// 	type="string"  
//	public="true"  
//	private="true"  
//	save="true"  
//	desc="buff��ʾ��id"/> 
#define FIELD_PROP_BUFF_DISPLAY_ID			"BuffDisplayID" 
 
//<property name="FightState"  
// 	type="byte"  
//	public="false"  
//	private="true"  
//	save="false"  
//	desc="ս��״̬ 0��1��"/> 
#define FIELD_PROP_FIGHT_STATE			"FightState" 
 
//<property name="FightTime"  
// 	type="int64"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="��ʼս��ʱ��"/> 
#define FIELD_PROP_FIGHT_TIME			"FightTime" 
 
//<property name="FightActionState"  
// 	type="byte"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="ս����Ϊ״̬"/> 
#define FIELD_PROP_FIGHT_ACTION_STATE			"FightActionState" 
 
//<property name="Camp"  
// 	type="byte"  
//	public="true"  
//	private="true"  
//	save="false"  
//	desc="��Ӫ"/> 
#define FIELD_PROP_CAMP			"Camp" 
 
//<property name="HP"  
// 	type="int64"  
//	public="true"  
//	private="true"  
//	save="false"  
//	desc="��ǰHPֵ"/> 
#define FIELD_PROP_HP			"HP" 
 
//<property name="MaxHP"  
// 	type="int64"  
//	public="true"  
//	private="true"  
//	save="false"  
//	desc="�����Ѫֵ"/> 
#define FIELD_PROP_MAX_HP			"MaxHP" 
 
//<property name="MP"  
// 	type="int64"  
//	public="false"  
//	private="true"  
//	save="false"  
//	desc="��ǰMPֵ"/> 
#define FIELD_PROP_MP			"MP" 
 
//<property name="MaxMP"  
// 	type="int64"  
//	public="false"  
//	private="true"  
//	save="false"  
//	desc="���MPֵ"/> 
#define FIELD_PROP_MAX_MP			"MaxMP" 
 
//<property name="PhysAttack"  
// 	type="int"  
//	public="false"  
//	private="true"  
//	save="false"  
//	desc="������"/> 
#define FIELD_PROP_PHYS_ATTACK			"PhysAttack" 
 
//<property name="MagicAttack"  
// 	type="int"  
//	public="false"  
//	private="true"  
//	save="false"  
//	desc="��������"/> 
#define FIELD_PROP_MAGIC_ATTACK			"MagicAttack" 
 
//<property name="PhysDefend"  
// 	type="int"  
//	public="false"  
//	private="true"  
//	save="false"  
//	desc="�������"/> 
#define FIELD_PROP_PHYS_DEFEND			"PhysDefend" 
 
//<property name="MagicDefend"  
// 	type="int"  
//	public="false"  
//	private="true"  
//	save="false"  
//	desc="��������"/> 
#define FIELD_PROP_MAGIC_DEFEND			"MagicDefend" 
 
//<property name="Hit"  
// 	type="int"  
//	public="false"  
//	private="true"  
//	save="false"  
//	desc="����"/> 
#define FIELD_PROP_HIT			"Hit" 
 
//<property name="Miss"  
// 	type="int"  
//	public="false"  
//	private="true"  
//	save="false"  
//	desc="����"/> 
#define FIELD_PROP_MISS			"Miss" 
 
//<property name="Crit"  
// 	type="int"  
//	public="false"  
//	private="true"  
//	save="false"  
//	desc="����(����)"/> 
#define FIELD_PROP_CRIT			"Crit" 
 
//<property name="CritRate"  
// 	type="int"  
//	public="false"  
//	private="true"  
//	save="false"  
//	desc="������"/> 
#define FIELD_PROP_CRIT_RATE			"CritRate" 
 
//<property name="Toug"  
// 	type="int"  
//	public="false"  
//	private="true"  
//	save="false"  
//	desc="���� ���ĵֿ�"/> 
#define FIELD_PROP_TOUG			"Toug" 
 
//<property name="TougRate"  
// 	type="int"  
//	public="false"  
//	private="true"  
//	save="false"  
//	desc="���ĵֿ���"/> 
#define FIELD_PROP_TOUG_RATE			"TougRate" 
 
//<property name="PhysPenetrate"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="����͸"/> 
#define FIELD_PROP_PHYS_PENETRATE			"PhysPenetrate" 
 
//<property name="MagicPenetrate"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="������͸"/> 
#define FIELD_PROP_MAGIC_PENETRATE			"MagicPenetrate" 
 
//<property name="AddHurtRate"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="����"/> 
#define FIELD_PROP_ADD_HURT_RATE			"AddHurtRate" 
 
//<property name="DecHurtRate"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="����"/> 
#define FIELD_PROP_DEC_HURT_RATE			"DecHurtRate" 
 
//<property name="FlowPrepareTime"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="׼��ʱ��"/> 
#define FIELD_PROP_FLOW_PREPARE_TIME			"FlowPrepareTime" 
 
//<property name="FlowHitTimes"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="�������"/> 
#define FIELD_PROP_FLOW_HIT_TIMES			"FlowHitTimes" 
 
//<property name="FlowLeadTime"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="��������ʱ��"/> 
#define FIELD_PROP_FLOW_LEAD_TIME			"FlowLeadTime" 
 
//<property name="FlowLeadSepTime"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="�������ʱ��"/> 
#define FIELD_PROP_FLOW_LEAD_SEP_TIME			"FlowLeadSepTime" 
 
//<property name="FlowHits"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="��ǰ���д���(׼����ʼ��������ʱ��ȡflow_hittime_rec������һ�е�ʱ����ֵ)"/> 
#define FIELD_PROP_FLOW_HITS			"FlowHits" 
 
//<property name="FlowCategory"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="��ǰʹ�õ����̵ķ�������"/> 
#define FIELD_PROP_FLOW_CATEGORY			"FlowCategory" 
 
//<property name="FlowType"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="�������ͣ�0����ͨ�������̣�1��������������"/> 
#define FIELD_PROP_FLOW_TYPE			"FlowType" 
 
//<property name="FlowBeginTime"  
// 	type="int64"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="���̿�ʼ��ʱ��"/> 
#define FIELD_PROP_FLOW_BEGIN_TIME			"FlowBeginTime" 
 
//<property name="Flowing"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="�Ƿ���������"/> 
#define FIELD_PROP_FLOWING			"Flowing" 
 
//<property name="GaintVal"  
// 	type="int"  
//	public="true"  
//	private="true"  
//	save="false"  
//	desc="��ǰ����ֵ"/> 
#define FIELD_PROP_GAINT_VAL			"GaintVal" 
 
//<property name="MaxGaintVal"  
// 	type="dword"  
//	public="true"  
//	private="true"  
//	save="false"  
//	desc="������ֵ"/> 
#define FIELD_PROP_MAX_GAINT_VAL			"MaxGaintVal" 
 
//<property name="GaintValAdd"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="���帽��ֵ"/> 
#define FIELD_PROP_GAINT_VAL_ADD			"GaintValAdd" 
 
//<property name="GaintDecHurtRate"  
// 	type="int"  
//	public="false"  
//	private="true"  
//	save="false"  
//	desc="�������"/> 
#define FIELD_PROP_GAINT_DEC_HURT_RATE			"GaintDecHurtRate" 
 
//<property name="Str"  
// 	type="int"  
//	public="false"  
//	private="true"  
//	save="false"  
//	desc="����"/> 
#define FIELD_PROP_STR			"Str" 
 
//<property name="Dex"  
// 	type="int"  
//	public="false"  
//	private="true"  
//	save="false"  
//	desc="����"/> 
#define FIELD_PROP_DEX			"Dex" 
 
//<property name="Sta"  
// 	type="int"  
//	public="false"  
//	private="true"  
//	save="false"  
//	desc="����"/> 
#define FIELD_PROP_STA			"Sta" 
 
//<property name="Ing"  
// 	type="int"  
//	public="false"  
//	private="true"  
//	save="false"  
//	desc="����"/> 
#define FIELD_PROP_ING			"Ing" 
 
//<property name="Spi"  
// 	type="int"  
//	public="false"  
//	private="true"  
//	save="false"  
//	desc="����"/> 
#define FIELD_PROP_SPI			"Spi" 
 
//<property name="TempHP"  
// 	type="int64"  
//	public="false"  
//	private="false"  
//	save="true"  
//	desc="��ʱ����HP��ǰֵ"/> 
#define FIELD_PROP_TEMP_HP			"TempHP" 
 
//<property name="TempMP"  
// 	type="int64"  
//	public="false"  
//	private="false"  
//	save="true"  
//	desc="��ʱ����MP��ǰֵ"/> 
#define FIELD_PROP_TEMP_MP			"TempMP" 
 
//<property name="HPAdd"  
// 	type="int"  
//	public="false"  
//	private="true"  
//	save="false"  
//	desc="������Ѫ"/> 
#define FIELD_PROP_HPADD			"HPAdd" 
 
//<property name="HPRate"  
// 	type="int"  
//	public="false"  
//	private="true"  
//	save="false"  
//	desc="��Ѫ���� 2������"/> 
#define FIELD_PROP_HPRATE			"HPRate" 
 
//<property name="MPAdd"  
// 	type="int"  
//	public="false"  
//	private="true"  
//	save="false"  
//	desc="����MP"/> 
#define FIELD_PROP_MPADD			"MPAdd" 
 
//<property name="MPRate"  
// 	type="int"  
//	public="false"  
//	private="true"  
//	save="false"  
//	desc="����MP�ٷֱ�"/> 
#define FIELD_PROP_MPRATE			"MPRate" 
 
//<property name="PhysAttackAdd"  
// 	type="int"  
//	public="false"  
//	private="true"  
//	save="false"  
//	desc="���� ������"/> 
#define FIELD_PROP_PHYS_ATTACK_ADD			"PhysAttackAdd" 
 
//<property name="MagicttackAdd"  
// 	type="int"  
//	public="false"  
//	private="true"  
//	save="false"  
//	desc="���� ��������"/> 
#define FIELD_PROP_MAGICTTACK_ADD			"MagicttackAdd" 
 
//<property name="PhysAttackRate"  
// 	type="int"  
//	public="false"  
//	private="true"  
//	save="false"  
//	desc="������ǿ�� 2������"/> 
#define FIELD_PROP_PHYS_ATTACK_RATE			"PhysAttackRate" 
 
//<property name="MagicAttackRate"  
// 	type="int"  
//	public="false"  
//	private="true"  
//	save="false"  
//	desc="��������ǿ�� 2������"/> 
#define FIELD_PROP_MAGIC_ATTACK_RATE			"MagicAttackRate" 
 
//<property name="PhysDefendAdd"  
// 	type="int"  
//	public="false"  
//	private="true"  
//	save="false"  
//	desc="���� �������"/> 
#define FIELD_PROP_PHYS_DEFEND_ADD			"PhysDefendAdd" 
 
//<property name="MagicDefendAdd"  
// 	type="int"  
//	public="false"  
//	private="true"  
//	save="false"  
//	desc="���� ��������"/> 
#define FIELD_PROP_MAGIC_DEFEND_ADD			"MagicDefendAdd" 
 
//<property name="PhysDefendRate"  
// 	type="int"  
//	public="false"  
//	private="true"  
//	save="false"  
//	desc="�������ǿ�� 2������"/> 
#define FIELD_PROP_PHYS_DEFEND_RATE			"PhysDefendRate" 
 
//<property name="MagicDefendRate"  
// 	type="int"  
//	public="false"  
//	private="true"  
//	save="false"  
//	desc="��������ǿ�� 2������"/> 
#define FIELD_PROP_MAGIC_DEFEND_RATE			"MagicDefendRate" 
 
//<property name="PhysPenetrateAdd"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="����͸ֵ"/> 
#define FIELD_PROP_PHYS_PENETRATE_ADD			"PhysPenetrateAdd" 
 
//<property name="PhysPenetrateRate"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="����͸��"/> 
#define FIELD_PROP_PHYS_PENETRATE_RATE			"PhysPenetrateRate" 
 
//<property name="MagicPenetrateAdd"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="������͸ֵ"/> 
#define FIELD_PROP_MAGIC_PENETRATE_ADD			"MagicPenetrateAdd" 
 
//<property name="MagicPenetrateRate"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="������͸��"/> 
#define FIELD_PROP_MAGIC_PENETRATE_RATE			"MagicPenetrateRate" 
 
//<property name="HitAdd"  
// 	type="int"  
//	public="false"  
//	private="true"  
//	save="false"  
//	desc="���� ����"/> 
#define FIELD_PROP_HIT_ADD			"HitAdd" 
 
//<property name="HitRate"  
// 	type="int"  
//	public="false"  
//	private="true"  
//	save="false"  
//	desc="������"/> 
#define FIELD_PROP_HIT_RATE			"HitRate" 
 
//<property name="MissAdd"  
// 	type="int"  
//	public="false"  
//	private="true"  
//	save="false"  
//	desc="���� �ر�"/> 
#define FIELD_PROP_MISS_ADD			"MissAdd" 
 
//<property name="MissRate"  
// 	type="int"  
//	public="false"  
//	private="true"  
//	save="false"  
//	desc="�ر���"/> 
#define FIELD_PROP_MISS_RATE			"MissRate" 
 
//<property name="CritAdd"  
// 	type="int"  
//	public="false"  
//	private="true"  
//	save="false"  
//	desc="���� ����(����)"/> 
#define FIELD_PROP_CRIT_ADD			"CritAdd" 
 
//<property name="TougAdd"  
// 	type="int"  
//	public="false"  
//	private="true"  
//	save="false"  
//	desc="���� ���ĵֿ�"/> 
#define FIELD_PROP_TOUG_ADD			"TougAdd" 
 
//<property name="CurSkill"  
// 	type="object"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="��ǰ����ʹ�õļ���"/> 
#define FIELD_PROP_CUR_SKILL			"CurSkill" 
 
//<property name="CurSkillUUID"  
// 	type="int64"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="��ǰ����ʹ�ü��ܵ�64λΨһ��־"/> 
#define FIELD_PROP_CUR_SKILL_UUID			"CurSkillUUID" 
 
//<property name="PreSkillUUID"  
// 	type="int64"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="ǰһ��ʹ�ü��ܵ�64λΨһ��־"/> 
#define FIELD_PROP_PRE_SKILL_UUID			"PreSkillUUID" 
 
//<property name="CurSkillTarget"  
// 	type="object"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="��ǰ�ļ���Ŀ��"/> 
#define FIELD_PROP_CUR_SKILL_TARGET			"CurSkillTarget" 
 
//<property name="CurSkillTargetX"  
// 	type="float"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="��ǰ�ļ�������"/> 
#define FIELD_PROP_CUR_SKILL_TARGET_X			"CurSkillTargetX" 
 
//<property name="CurSkillTargetY"  
// 	type="float"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="��ǰ�ļ�������"/> 
#define FIELD_PROP_CUR_SKILL_TARGET_Y			"CurSkillTargetY" 
 
//<property name="CurSkillTargetZ"  
// 	type="float"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="��ǰ�ļ�������"/> 
#define FIELD_PROP_CUR_SKILL_TARGET_Z			"CurSkillTargetZ" 
 
//<property name="CurSkillStartTime"  
// 	type="int64"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="��ǰ���ܵĿ�ʼʱ��"/> 
#define FIELD_PROP_CUR_SKILL_START_TIME			"CurSkillStartTime" 
 
//<property name="MaxFlyEnergy"  
// 	type="int"  
//	public="false"  
//	private="true"  
//	save="false"  
//	desc="最大轻功精力值"/> 
#define FIELD_PROP_MAX_FLY_ENERGY			"MaxFlyEnergy" 
 
//<property name="FlyEnergyRestore"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="轻功精力值回复速度(/s)"/> 
#define FIELD_PROP_FLY_ENERGY_RESTORE			"FlyEnergyRestore" 
 
//<property name="FlyEnergy"  
// 	type="int"  
//	public="false"  
//	private="true"  
//	save="false"  
//	desc="轻功精力值"/> 
#define FIELD_PROP_FLY_ENERGY			"FlyEnergy" 
 
//<property name="JumpSec"  
// 	type="byte"  
//	public="true"  
//	private="true"  
//	save="false"  
//	desc="当前轻功段数"/> 
#define FIELD_PROP_JUMP_SEC			"JumpSec" 
 
//<property name="JumpHeight"  
// 	type="float"  
//	public="true"  
//	private="true"  
//	save="false"  
//	desc="跳跃高度"/> 
#define FIELD_PROP_JUMP_HEIGHT			"JumpHeight" 
 
//<property name="CantUseFlySkill"  
// 	type="byte"  
//	public="flase"  
//	private="true"  
//	save="false"  
//	desc="是否可以使用轻功"/> 
#define FIELD_PROP_CANT_USE_FLY_SKILL			"CantUseFlySkill" 
 
//<property name="MoveSpeed"  
// 	type="float"  
//	public="true"  
//	private="true"  
//	save="false"  
//	desc="�ƶ��ٶ�"/> 
#define FIELD_PROP_MOVE_SPEED			"MoveSpeed" 
 
//<property name="RunSpeed"  
// 	type="float"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="�ܶ��ٶ�"/> 
#define FIELD_PROP_RUN_SPEED			"RunSpeed" 
 
//<property name="RunSpeedAdd"  
// 	type="float"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="�ƶ��ٶȵĸı�"/> 
#define FIELD_PROP_RUN_SPEED_ADD			"RunSpeedAdd" 
 
//<property name="RollSpeed"  
// 	type="float"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="��ת�ٶ�"/> 
#define FIELD_PROP_ROLL_SPEED			"RollSpeed" 
 
//<property name="Weapon"  
// 	type="int"  
//	public="true"  
//	private="true"  
//	save="true"  
//	desc="����"/> 
#define FIELD_PROP_WEAPON			"Weapon" 
 
//<property name="Cloth"  
// 	type="int"  
//	public="true"  
//	private="true"  
//	save="true"  
//	desc=""/> 
#define FIELD_PROP_CLOTH			"Cloth" 
 
//<property name="Hair"  
// 	type="int"  
//	public="true"  
//	private="true"  
//	save="true"  
//	desc=""/> 
#define FIELD_PROP_HAIR			"Hair" 
 
//<property name="Wing"  
// 	type="int"  
//	public="true"  
//	private="true"  
//	save="true"  
//	desc=""/> 
#define FIELD_PROP_WING			"Wing" 
 
//<property name="ID"  
// 	type="int"  
//	public="true"  
//	private="false"  
//	save="false"  
//	desc="����������Դ����"/> 
#define FIELD_PROP_ID			"ID" 
 
//<property name="VisualRange"  
// 	type="float"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="���������֮��Ŀ��ӷ�Χ"/> 
#define FIELD_PROP_VISUAL_RANGE			"VisualRange" 
 
//<property name="LocallLevelLim"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="��������������"/> 
#define FIELD_PROP_LOCALL_LEVEL_LIM			"LocallLevelLim" 
 
//<property name="AttackType"  
// 	type="byte"  
//	public="true"  
//	private="false"  
//	save="false"  
//	desc="������������"/> 
#define FIELD_PROP_ATTACK_TYPE			"AttackType" 
 
//<property name="ProtoSceneID"  
// 	type="int"  
//	public="true"  
//	private="false"  
//	save="false"  
//	desc="ԭ�ͳ���ID"/> 
#define FIELD_PROP_PROTO_SCENE_ID			"ProtoSceneID" 
 
//<property name="CanShowRide"  
// 	type="byte"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="�ɷ���ʾ���� 0������ʾ 1����ʾ"/> 
#define FIELD_PROP_CAN_SHOW_RIDE			"CanShowRide" 
 
//<property name="CanFly"  
// 	type="byte"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="�ɷ���� 0���ɷ��� 1�ɷ���"/> 
#define FIELD_PROP_CAN_FLY			"CanFly" 
 
//<property name="LifeTime"  
// 	type="int"  
//	public="false"  
//	private="true"  
//	save="true"  
//	desc="����ʱ��(ms) ���Ϊ0,��־���Ч"/> 
#define FIELD_PROP_LIFE_TIME			"LifeTime" 
 
//<property name="Sender"  
// 	type="object"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="BUFFER��ʩ����"/> 
#define FIELD_PROP_SENDER			"Sender" 
 
//<property name="AddTime"  
// 	type="int64"  
//	public="false"  
//	private="true"  
//	save="true"  
//	desc="BUFFER��ӵ�ʱ�� ��λs(time_t)"/> 
#define FIELD_PROP_ADD_TIME			"AddTime" 
 
//<property name="Active"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="true"  
//	desc="BUFFER�Ƿ���Ч"/> 
#define FIELD_PROP_ACTIVE			"Active" 
 
//<property name="Counter"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="true"  
//	desc="BUFFER�ѼƴεĴ���"/> 
#define FIELD_PROP_COUNTER			"Counter" 
 
//<property name="Timer"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="true"  
//	desc="BUFFER�Ѵ��ڵ�ʱ�� ��λms"/> 
#define FIELD_PROP_TIMER			"Timer" 
 
//<property name="HurtCount"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="���������д������,���ڼ����˺�"/> 
#define FIELD_PROP_HURT_COUNT			"HurtCount" 
 
//<property name="CurHits"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="���ܵ�ǰ�������"/> 
#define FIELD_PROP_CUR_HITS			"CurHits" 
 
//<property name="SkillAttackAdd"  
// 	type="int"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="���ܹ�����"/> 
#define FIELD_PROP_SKILL_ATTACK_ADD			"SkillAttackAdd" 
 
//<property name="SkillDamageRate"  
// 	type="float"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="�����˺�����"/> 
#define FIELD_PROP_SKILL_DAMAGE_RATE			"SkillDamageRate" 
 
//<property name="SkillFormula"  
// 	type="float"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="���ܹ�ʽ�����ֵ"/> 
#define FIELD_PROP_SKILL_FORMULA			"SkillFormula" 
 
//<property name="Fixed"  
// 	type="byte"  
//	public="true"  
//	private="true"  
//	save="true"  
//	desc=""/> 
#define FIELD_PROP_FIXED			"Fixed" 
 
//<property name="Photo"  
// 	type="string"  
//	public="true"  
//	private="true"  
//	save="true"  
//	desc="ͼ��"/> 
#define FIELD_PROP_PHOTO			"Photo" 
 
//<property name="State"  
// 	type="string"  
//	public="true"  
//	private="true"  
//	save="false"  
//	desc="״̬"/> 
#define FIELD_PROP_STATE			"State" 
 
 
// ����============================================================== 
 
 
//<record name="AIFindPathRec" 
//	cols="2"  
//	maxrows="1024"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="Ѱ·��"/> 
 
#define FIELD_RECORD_AIFIND_PATH_REC			"AIFindPathRec" 
// AIFindPathRec column index define 
enum COLUMNS_OF_AIFIND_PATH_REC 
{ 
 
	// <column type="float" desc="x"/> 
	COLUMN_AIFIND_PATH_REC_0000, 
 
	// <column type="float" desc="z"/> 
	COLUMN_AIFIND_PATH_REC_0001, 
 
}; 
 
 
//<record name="RoundPathListRec" 
//	cols="2"  
//	maxrows="128"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="�ƿ��ϰ������߹��ĸ���"/> 
 
#define FIELD_RECORD_ROUND_PATH_LIST_REC			"RoundPathListRec" 
// RoundPathListRec column index define 
enum COLUMNS_OF_ROUND_PATH_LIST_REC 
{ 
 
	// <column type="dword" desc="��������"/> 
	COLUMN_ROUND_PATH_LIST_REC_0000, 
 
	// <column type="dword" desc="�ظ��߹��Ĵ���"/> 
	COLUMN_ROUND_PATH_LIST_REC_0001, 
 
}; 
 
 
//<record name="FindRoundPathRec" 
//	cols="2"  
//	maxrows="128"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="��̬Ѱ··��"/> 
 
#define FIELD_RECORD_FIND_ROUND_PATH_REC			"FindRoundPathRec" 
// FindRoundPathRec column index define 
enum COLUMNS_OF_FIND_ROUND_PATH_REC 
{ 
 
	// <column type="dword" desc="��������"/> 
	COLUMN_FIND_ROUND_PATH_REC_0000, 
 
	// <column type="dword" desc="�����ʴ���"/> 
	COLUMN_FIND_ROUND_PATH_REC_0001, 
 
}; 
 
 
//<record name="CantArriveGridListRec" 
//	cols="1"  
//	maxrows="128"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="�޷�����ĸ����б�"/> 
 
#define FIELD_RECORD_CANT_ARRIVE_GRID_LIST_REC			"CantArriveGridListRec" 
// CantArriveGridListRec column index define 
enum COLUMNS_OF_CANT_ARRIVE_GRID_LIST_REC 
{ 
 
	// <column type="dword" desc="��������"/> 
	COLUMN_CANT_ARRIVE_GRID_LIST_REC_0000, 
 
}; 
 
 
//<record name="ride_skin_rec" 
//	cols="3"  
//	maxrows="100"  
//	public="false"  
//	private="true"  
//	save="true"  
//	desc="����Ƥ����"/> 
 
#define FIELD_RECORD_RIDE_SKIN_REC			"ride_skin_rec" 
// ride_skin_rec column index define 
enum COLUMNS_OF_RIDE_SKIN_REC 
{ 
 
	// <column type="int" desc="Ƥ�����"/> 
	COLUMN_RIDE_SKIN_REC_ID, 
 
	// <column type="byte" desc="״̬"/> 
	COLUMN_RIDE_SKIN_REC_STATE, 
 
	// <column type="byte" desc="��Ƥ����ʶ"/> 
	COLUMN_RIDE_SKIN_REC_FLAG, 
 
}; 
 
 
//<record name="PatrolPointRec" 
//	cols="3"  
//	maxrows="64"  
//	public=""  
//	private=""  
//	save="false"  
//	desc="����Ѳ�߱�"/> 
 
#define FIELD_RECORD_PATROL_POINT_REC			"PatrolPointRec" 
// PatrolPointRec column index define 
enum COLUMNS_OF_PATROL_POINT_REC 
{ 
 
	// <column type="float" desc=""/> 
	COLUMN_PATROL_POINT_REC_X, 
 
	// <column type="float" desc=""/> 
	COLUMN_PATROL_POINT_REC_Z, 
 
	// <column type="dword" desc="ͣ��ʱ�䣬��λms"/> 
	COLUMN_PATROL_POINT_REC_STAY_TIME, 
 
}; 
 
 
//<record name="AttackerList" 
//	cols="4"  
//	maxrows="2000"  
//	public=""  
//	private=""  
//	save="false"  
//	desc="���������"/> 
 
#define FIELD_RECORD_ATTACKER_LIST			"AttackerList" 
// AttackerList column index define 
enum COLUMNS_OF_ATTACKER_LIST 
{ 
 
	// <column type="object" desc="����"/> 
	COLUMN_ATTACKER_LIST_0000, 
 
	// <column type="dword" desc="���ֵ"/> 
	COLUMN_ATTACKER_LIST_0001, 
 
	// <column type="int64" desc="�������ʱ��"/> 
	COLUMN_ATTACKER_LIST_0002, 
 
	// <column type="int64" desc="��һ�ι���ʱ��"/> 
	COLUMN_ATTACKER_LIST_0003, 
 
}; 
 
 
//<record name="FindPathPointList" 
//	cols="2"  
//	maxrows="64"  
//	public=""  
//	private=""  
//	save="false"  
//	desc="Ѱ··�����"/> 
 
#define FIELD_RECORD_FIND_PATH_POINT_LIST			"FindPathPointList" 
// FindPathPointList column index define 
enum COLUMNS_OF_FIND_PATH_POINT_LIST 
{ 
 
	// <column type="float" desc="X����"/> 
	COLUMN_FIND_PATH_POINT_LIST_X, 
 
	// <column type="float" desc="Y����"/> 
	COLUMN_FIND_PATH_POINT_LIST_Y, 
 
}; 
 
 
//<record name="boss_skill_rec" 
//	cols="2"  
//	maxrows="10"  
//	public=""  
//	private=""  
//	save="false"  
//	desc="NPC���ܱ�"/> 
 
#define FIELD_RECORD_BOSS_SKILL_REC			"boss_skill_rec" 
// boss_skill_rec column index define 
enum COLUMNS_OF_BOSS_SKILL_REC 
{ 
 
	// <column type="string" desc="����ID"/> 
	COLUMN_BOSS_SKILL_REC_0000, 
 
	// <column type="int" desc="��ǰȨֵ"/> 
	COLUMN_BOSS_SKILL_REC_0001, 
 
}; 
 
 
//<record name="object_list_rec" 
//	cols="1"  
//	maxrows="256"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="�����������ɵĶ���"/> 
 
#define FIELD_RECORD_OBJECT_LIST_REC			"object_list_rec" 
// object_list_rec column index define 
enum COLUMNS_OF_OBJECT_LIST_REC 
{ 
 
	// <column type="object" desc=""/> 
	COLUMN_OBJECT_LIST_REC_0000, 
 
}; 
 
 
//<record name="damage_rec" 
//	cols="3"  
//	maxrows="100"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="�˺���"/> 
 
#define FIELD_RECORD_DAMAGE_REC			"damage_rec" 
// damage_rec column index define 
enum COLUMNS_OF_DAMAGE_REC 
{ 
 
	// <column type="widestr" desc="�������"/> 
	COLUMN_DAMAGE_REC_PLAYER_NAME, 
 
	// <column type="byte" desc="ְҵ"/> 
	COLUMN_DAMAGE_REC_JOB, 
 
	// <column type="dword" desc="���˺�"/> 
	COLUMN_DAMAGE_REC_DAMAGE, 
 
}; 
 
 
//<record name="position_info_rec" 
//	cols="7"  
//	maxrows="1024"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="�������λ����Ϣ"/> 
 
#define FIELD_RECORD_POSITION_INFO_REC			"position_info_rec" 
// position_info_rec column index define 
enum COLUMNS_OF_POSITION_INFO_REC 
{ 
 
	// <column type="float" desc="X"/> 
	COLUMN_POSITION_INFO_REC_0000, 
 
	// <column type="float" desc="Y"/> 
	COLUMN_POSITION_INFO_REC_0001, 
 
	// <column type="float" desc="Z"/> 
	COLUMN_POSITION_INFO_REC_0002, 
 
	// <column type="float" desc="Orient"/> 
	COLUMN_POSITION_INFO_REC_0003, 
 
	// <column type="int" desc="used:�Ƿ��ѱ�ʹ��"/> 
	COLUMN_POSITION_INFO_REC_0004, 
 
	// <column type="int64" desc="�ϴ�����ʱ��"/> 
	COLUMN_POSITION_INFO_REC_0005, 
 
	// <column type="float" desc="���ˢ�°뾶"/> 
	COLUMN_POSITION_INFO_REC_0006, 
 
}; 
 
 
//<record name="SpringInfoRec" 
//	cols="2"  
//	maxrows="64"  
//	public=""  
//	private=""  
//	save="false"  
//	desc="����Ŀ����Ϣ��"/> 
 
#define FIELD_RECORD_SPRING_INFO_REC			"SpringInfoRec" 
// SpringInfoRec column index define 
enum COLUMNS_OF_SPRING_INFO_REC 
{ 
 
	// <column type="object" desc="�����"/> 
	COLUMN_SPRING_INFO_REC_0000, 
 
	// <column type="string" desc="���ӵ�BUFF"/> 
	COLUMN_SPRING_INFO_REC_0001, 
 
}; 
 
 
//<record name="activate_function_rec" 
//	cols="1"  
//	maxrows="500"  
//	public="false"  
//	private="true"  
//	save="true"  
//	desc="���ܼ����"/> 
 
#define FIELD_RECORD_ACTIVATE_FUNCTION_REC			"activate_function_rec" 
// activate_function_rec column index define 
enum COLUMNS_OF_ACTIVATE_FUNCTION_REC 
{ 
 
	// <column type="byte" desc="��������"/> 
	COLUMN_ACTIVATE_FUNCTION_REC_TYPE, 
 
}; 
 
 
//<record name="arena_fight_result" 
//	cols="5"  
//	maxrows="20"  
//	public="false"  
//	private="false"  
//	save="true"  
//	desc="������ս�����"/> 
 
#define FIELD_RECORD_ARENA_FIGHT_RESULT			"arena_fight_result" 
// arena_fight_result column index define 
enum COLUMNS_OF_ARENA_FIGHT_RESULT 
{ 
 
	// <column type="int" desc="ս�����"/> 
	COLUMN_ARENA_FIGHT_RESULT_RESULT, 
 
	// <column type="widestr" desc="�з�����"/> 
	COLUMN_ARENA_FIGHT_RESULT_NAME, 
 
	// <column type="int" desc="ս��ǰ����"/> 
	COLUMN_ARENA_FIGHT_RESULT_OLDRANK, 
 
	// <column type="int" desc="ս��������"/> 
	COLUMN_ARENA_FIGHT_RESULT_CURRANK, 
 
	// <column type="int64" desc="ս��ʱ��"/> 
	COLUMN_ARENA_FIGHT_RESULT_TIME, 
 
}; 
 
 
//<record name="battlle_ability_rec" 
//	cols="2"  
//	maxrows="100"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="ս����ͳ�Ʊ�"/> 
 
#define FIELD_RECORD_BATTLLE_ABILITY_REC			"battlle_ability_rec" 
// battlle_ability_rec column index define 
enum COLUMNS_OF_BATTLLE_ABILITY_REC 
{ 
 
	// <column type="byte" desc="ս��������"/> 
	COLUMN_BATTLLE_ABILITY_REC_BATYPE, 
 
	// <column type="float" desc="ս������ֵ"/> 
	COLUMN_BATTLLE_ABILITY_REC_BABVALUE, 
 
}; 
 
 
//<record name="capital_rec" 
//	cols="3"  
//	maxrows="30"  
//	public="false"  
//	private="true"  
//	save="true"  
//	desc="�ʲ���"/> 
 
#define FIELD_RECORD_CAPITAL_REC			"capital_rec" 
// capital_rec column index define 
enum COLUMNS_OF_CAPITAL_REC 
{ 
 
	// <column type="string" desc="�ʲ�����"/> 
	COLUMN_CAPITAL_REC_0000, 
 
	// <column type="byte" desc="�ʲ�����"/> 
	COLUMN_CAPITAL_REC_0001, 
 
	// <column type="int64" desc="�ʲ�����"/> 
	COLUMN_CAPITAL_REC_0002, 
 
}; 
 
 
//<record name="dayToplimitCapital" 
//	cols="3"  
//	maxrows="30"  
//	public="false"  
//	private="true"  
//	save="true"  
//	desc="�����ʲ����Ӽ�¼��"/> 
 
#define FIELD_RECORD_DAY_TOPLIMIT_CAPITAL			"dayToplimitCapital" 
// dayToplimitCapital column index define 
enum COLUMNS_OF_DAY_TOPLIMIT_CAPITAL 
{ 
 
	// <column type="string" desc="�ʲ�����"/> 
	COLUMN_DAY_TOPLIMIT_CAPITAL_0000, 
 
	// <column type="byte" desc="�ʲ�����"/> 
	COLUMN_DAY_TOPLIMIT_CAPITAL_0001, 
 
	// <column type="int64" desc="�����ʲ�����"/> 
	COLUMN_DAY_TOPLIMIT_CAPITAL_0002, 
 
}; 
 
 
//<record name="valid_damage_rec" 
//	cols="2"  
//	maxrows="1000"  
//	public="false"  
//	private="false"  
//	save="true"  
//	desc="��Ч�˺���¼��"/> 
 
#define FIELD_RECORD_VALID_DAMAGE_REC			"valid_damage_rec" 
// valid_damage_rec column index define 
enum COLUMNS_OF_VALID_DAMAGE_REC 
{ 
 
	// <column type="string" desc="���uid"/> 
	COLUMN_VALID_DAMAGE_REC_UID, 
 
	// <column type="int64" desc="�˺�ʱ��"/> 
	COLUMN_VALID_DAMAGE_REC_TIME, 
 
}; 
 
 
//<record name="valid_kill_rec" 
//	cols="2"  
//	maxrows="1000"  
//	public="false"  
//	private="false"  
//	save="true"  
//	desc="��Ч��ɱ��Ҽ�¼��"/> 
 
#define FIELD_RECORD_VALID_KILL_REC			"valid_kill_rec" 
// valid_kill_rec column index define 
enum COLUMNS_OF_VALID_KILL_REC 
{ 
 
	// <column type="string" desc="���uid"/> 
	COLUMN_VALID_KILL_REC_UID, 
 
	// <column type="int64" desc="��ɱʱ��"/> 
	COLUMN_VALID_KILL_REC_TIME, 
 
}; 
 
 
//<record name="EquStrengthenRec" 
//	cols="8"  
//	maxrows="6"  
//	public="false"  
//	private="true"  
//	save="true"  
//	desc="װ������ǿ���ȼ�"/> 
 
#define FIELD_RECORD_EQU_STRENGTHEN_REC			"EquStrengthenRec" 
// EquStrengthenRec column index define 
enum COLUMNS_OF_EQU_STRENGTHEN_REC 
{ 
 
	// <column type="BYTE" desc="װ����������(1-6)"/> 
	COLUMN_EQU_STRENGTHEN_REC_0000, 
 
	// <column type="WORD" desc="ǿ���ȼ�"/> 
	COLUMN_EQU_STRENGTHEN_REC_0001, 
 
	// <column type="STRING" desc="ÿ��װ����Ƕ�����,û����''ռλ��ʾ"/> 
	COLUMN_EQU_STRENGTHEN_REC_0002, 
 
	// <column type="WORD" desc="�����������"/> 
	COLUMN_EQU_STRENGTHEN_REC_0003, 
 
	// <column type="STRING" desc="ÿ��װ����Ƕ�ı�ʯ1,û����''ռλ��ʾ"/> 
	COLUMN_EQU_STRENGTHEN_REC_0004, 
 
	// <column type="STRING" desc="ÿ��װ����Ƕ�ı�ʯ2,û����''ռλ��ʾ"/> 
	COLUMN_EQU_STRENGTHEN_REC_0005, 
 
	// <column type="STRING" desc="ÿ��װ����Ƕ�ı�ʯ3,û����''ռλ��ʾ"/> 
	COLUMN_EQU_STRENGTHEN_REC_0006, 
 
	// <column type="STRING" desc="ÿ��װ����Ƕ�ı�ʯ4,û����''ռλ��ʾ"/> 
	COLUMN_EQU_STRENGTHEN_REC_0007, 
 
}; 
 
 
//<record name="BaptiseRec" 
//	cols="3"  
//	maxrows="6"  
//	public="false"  
//	private="true"  
//	save="true"  
//	desc="ϴ����"/> 
 
#define FIELD_RECORD_BAPTISE_REC			"BaptiseRec" 
// BaptiseRec column index define 
enum COLUMNS_OF_BAPTISE_REC 
{ 
 
	// <column type="BYTE" desc="װ����������(1-6)"/> 
	COLUMN_BAPTISE_REC_POS, 
 
	// <column type="STRING" desc="����ϴ��ֵ��jsonת�����ַ�����"/> 
	COLUMN_BAPTISE_REC_VALUE_INFO, 
 
	// <column type="STRING" desc="��������״̬��jsonת�����ַ�����"/> 
	COLUMN_BAPTISE_REC_LOCK_INFO, 
 
}; 
 
 
//<record name="BaptiseResultRec" 
//	cols="2"  
//	maxrows="6"  
//	public="false"  
//	private="true"  
//	save="true"  
//	desc="װ��ϴ�������"/> 
 
#define FIELD_RECORD_BAPTISE_RESULT_REC			"BaptiseResultRec" 
// BaptiseResultRec column index define 
enum COLUMNS_OF_BAPTISE_RESULT_REC 
{ 
 
	// <column type="BYTE" desc="װ����������(1-6)"/> 
	COLUMN_BAPTISE_RESULT_REC_POS, 
 
	// <column type="STRING" desc="ϴ�������jsonת�����ַ�����"/> 
	COLUMN_BAPTISE_RESULT_REC_RESULT_INFO, 
 
}; 
 
 
//<record name="friend_rec" 
//	cols="21"  
//	maxrows="50"  
//	public="false"  
//	private="true"  
//	save="true"  
//	desc="�����б�"/> 
 
#define FIELD_RECORD_FRIEND_REC			"friend_rec" 
// friend_rec column index define 
enum COLUMNS_OF_FRIEND_REC 
{ 
 
	// <column type="string" desc="�Է�UID"/> 
	COLUMN_FRIEND_REC_UID, 
 
	// <column type="widestr" desc="�Է�����"/> 
	COLUMN_FRIEND_REC_NAME, 
 
	// <column type="word" desc="�Է��ȼ�"/> 
	COLUMN_FRIEND_REC_LEVEL, 
 
	// <column type="byte" desc="�Է�ְҵ"/> 
	COLUMN_FRIEND_REC_JOB, 
 
	// <column type="byte" desc="�Է��Ա�"/> 
	COLUMN_FRIEND_REC_SEX, 
 
	// <column type="dword" desc="ս����"/> 
	COLUMN_FRIEND_REC_POWER, 
 
	// <column type="widestr" desc="�������"/> 
	COLUMN_FRIEND_REC_GUILD_NAME, 
 
	// <column type="byte" desc="�Է�����״̬"/> 
	COLUMN_FRIEND_REC_ONLINE, 
 
	// <column type="int64" desc="���һ������ʱ��"/> 
	COLUMN_FRIEND_REC_LAST_TIME, 
 
	// <column type="word" desc="���ܶȵȼ�"/> 
	COLUMN_FRIEND_REC_INTIMACY_LEVEL, 
 
	// <column type="dword" desc="���ܶ�"/> 
	COLUMN_FRIEND_REC_INTIMACY_EXP, 
 
	// <column type="dword" desc="��ʷ�ջ�"/> 
	COLUMN_FRIEND_REC_SUM_FLOWER, 
 
	// <column type="dword" desc="�����ջ�"/> 
	COLUMN_FRIEND_REC_WEEK_FLOWER, 
 
	// <column type="dword" desc="�ҵ��ͻ���"/> 
	COLUMN_FRIEND_REC_SEND_FLOWER, 
 
	// <column type="dword" desc="�ҵ��ջ���"/> 
	COLUMN_FRIEND_REC_RECEIVE_FLOWER, 
 
	// <column type="dword" desc="����teamID"/> 
	COLUMN_FRIEND_REC_TEAM_ID, 
 
	// <column type="byte" desc="��ϵ"/> 
	COLUMN_FRIEND_REC_RELATION, 
 
	// <column type="widestr" desc="��ν"/> 
	COLUMN_FRIEND_REC_TITLE, 
 
	// <column type="int64" desc="Ѱ��Ԯ��ʱ��"/> 
	COLUMN_FRIEND_REC_SEEK_SWORN_AID_TIME, 
 
	// <column type="dword" desc="vip�ȼ�"/> 
	COLUMN_FRIEND_REC_VIP_LEVEL, 
 
	// <column type="dword" desc="ÿ���������ܶ�ֵ"/> 
	COLUMN_FRIEND_REC_DAILY_INTIMACY, 
 
}; 
 
 
//<record name="blacklist_rec" 
//	cols="4"  
//	maxrows="50"  
//	public="false"  
//	private="true"  
//	save="true"  
//	desc="�������б�"/> 
 
#define FIELD_RECORD_BLACKLIST_REC			"blacklist_rec" 
// blacklist_rec column index define 
enum COLUMNS_OF_BLACKLIST_REC 
{ 
 
	// <column type="string" desc="�Է�UID"/> 
	COLUMN_BLACKLIST_REC_UID, 
 
	// <column type="widestr" desc="�Է�����"/> 
	COLUMN_BLACKLIST_REC_NAME, 
 
	// <column type="byte" desc="�Է�ְҵ"/> 
	COLUMN_BLACKLIST_REC_JOB, 
 
	// <column type="byte" desc="�Է��Ա�"/> 
	COLUMN_BLACKLIST_REC_SEX, 
 
}; 
 
 
//<record name="enemy_rec" 
//	cols="12"  
//	maxrows="50"  
//	public="false"  
//	private="true"  
//	save="true"  
//	desc="�����б�"/> 
 
#define FIELD_RECORD_ENEMY_REC			"enemy_rec" 
// enemy_rec column index define 
enum COLUMNS_OF_ENEMY_REC 
{ 
 
	// <column type="string" desc="����UID"/> 
	COLUMN_ENEMY_REC_UID, 
 
	// <column type="widestr" desc="�Է�����"/> 
	COLUMN_ENEMY_REC_NAME, 
 
	// <column type="word" desc="�Է��ȼ�"/> 
	COLUMN_ENEMY_REC_LEVEL, 
 
	// <column type="byte" desc="�Է�ְҵ"/> 
	COLUMN_ENEMY_REC_JOB, 
 
	// <column type="byte" desc="�Է��Ա�"/> 
	COLUMN_ENEMY_REC_SEX, 
 
	// <column type="dword" desc="ս����"/> 
	COLUMN_ENEMY_REC_POWER, 
 
	// <column type="widestr" desc="�������"/> 
	COLUMN_ENEMY_REC_GUILD_NAME, 
 
	// <column type="string" desc="����˺�"/> 
	COLUMN_ENEMY_REC_ACCOUNT, 
 
	// <column type="byte" desc="�Է�����״̬"/> 
	COLUMN_ENEMY_REC_ONLINE, 
 
	// <column type="dword" desc="���ֵ"/> 
	COLUMN_ENEMY_REC_HATRED, 
 
	// <column type="int64" desc="��ɱʱ��"/> 
	COLUMN_ENEMY_REC_BE_KILL_TIME, 
 
	// <column type="dword" desc="vip�ȼ�"/> 
	COLUMN_ENEMY_REC_VIP_LEVEL, 
 
}; 
 
 
//<record name="friend_apply_rec" 
//	cols="9"  
//	maxrows="20"  
//	public="false"  
//	private="true"  
//	save="true"  
//	desc="��������δ�����"/> 
 
#define FIELD_RECORD_FRIEND_APPLY_REC			"friend_apply_rec" 
// friend_apply_rec column index define 
enum COLUMNS_OF_FRIEND_APPLY_REC 
{ 
 
	// <column type="string" desc="�Է�UID"/> 
	COLUMN_FRIEND_APPLY_REC_UID, 
 
	// <column type="widestr" desc="�Է�����"/> 
	COLUMN_FRIEND_APPLY_REC_NAME, 
 
	// <column type="word" desc="�Է��ȼ�"/> 
	COLUMN_FRIEND_APPLY_REC_LEVEL, 
 
	// <column type="byte" desc="�Է�ְҵ"/> 
	COLUMN_FRIEND_APPLY_REC_JOB, 
 
	// <column type="byte" desc="�Է��Ա�"/> 
	COLUMN_FRIEND_APPLY_REC_SEX, 
 
	// <column type="dword" desc="ս����"/> 
	COLUMN_FRIEND_APPLY_REC_POWER, 
 
	// <column type="widestr" desc="�������"/> 
	COLUMN_FRIEND_APPLY_REC_GUILD_NAME, 
 
	// <column type="byte" desc="0-�������룬1-�������"/> 
	COLUMN_FRIEND_APPLY_REC_APPLY_TYPE, 
 
	// <column type="dword" desc="vip�ȼ�"/> 
	COLUMN_FRIEND_APPLY_REC_VIP_LEVEL, 
 
}; 
 
 
//<record name="friend_recommend_mid_rec" 
//	cols="8"  
//	maxrows="100"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="���Ϻ����Ƽ������ı�(������)"/> 
 
#define FIELD_RECORD_FRIEND_RECOMMEND_MID_REC			"friend_recommend_mid_rec" 
// friend_recommend_mid_rec column index define 
enum COLUMNS_OF_FRIEND_RECOMMEND_MID_REC 
{ 
 
	// <column type="string" desc="�Է�UID"/> 
	COLUMN_FRIEND_RECOMMEND_MID_REC_UID, 
 
	// <column type="widestr" desc="�Է�����"/> 
	COLUMN_FRIEND_RECOMMEND_MID_REC_NAME, 
 
	// <column type="word" desc="�Է��ȼ�"/> 
	COLUMN_FRIEND_RECOMMEND_MID_REC_LEVEL, 
 
	// <column type="byte" desc="�Է�ְҵ"/> 
	COLUMN_FRIEND_RECOMMEND_MID_REC_JOB, 
 
	// <column type="byte" desc="�Է��Ա�"/> 
	COLUMN_FRIEND_RECOMMEND_MID_REC_SEX, 
 
	// <column type="dword" desc="ս����"/> 
	COLUMN_FRIEND_RECOMMEND_MID_REC_POWER, 
 
	// <column type="widestr" desc="�������"/> 
	COLUMN_FRIEND_RECOMMEND_MID_REC_GUILD_NAME, 
 
	// <column type="dword" desc="vip�ȼ�"/> 
	COLUMN_FRIEND_RECOMMEND_MID_REC_VIP_LEVEL, 
 
}; 
 
 
//<record name="friend_invitation_send_rec" 
//	cols="1"  
//	maxrows="100"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="�Ѿ�����������(������)"/> 
 
#define FIELD_RECORD_FRIEND_INVITATION_SEND_REC			"friend_invitation_send_rec" 
// friend_invitation_send_rec column index define 
enum COLUMNS_OF_FRIEND_INVITATION_SEND_REC 
{ 
 
	// <column type="string" desc="�Է�UID"/> 
	COLUMN_FRIEND_INVITATION_SEND_REC_UID, 
 
}; 
 
 
//<record name="friend_recommend_rec" 
//	cols="8"  
//	maxrows="5"  
//	public="false"  
//	private="true"  
//	save="false"  
//	desc="�����Ƽ���(������)"/> 
 
#define FIELD_RECORD_FRIEND_RECOMMEND_REC			"friend_recommend_rec" 
// friend_recommend_rec column index define 
enum COLUMNS_OF_FRIEND_RECOMMEND_REC 
{ 
 
	// <column type="string" desc="�Է�UID"/> 
	COLUMN_FRIEND_RECOMMEND_REC_UID, 
 
	// <column type="widestr" desc="�Է�����"/> 
	COLUMN_FRIEND_RECOMMEND_REC_NAME, 
 
	// <column type="word" desc="�Է��ȼ�"/> 
	COLUMN_FRIEND_RECOMMEND_REC_LEVEL, 
 
	// <column type="byte" desc="�Է�ְҵ"/> 
	COLUMN_FRIEND_RECOMMEND_REC_JOB, 
 
	// <column type="byte" desc="�Է��Ա�"/> 
	COLUMN_FRIEND_RECOMMEND_REC_SEX, 
 
	// <column type="dword" desc="ս����"/> 
	COLUMN_FRIEND_RECOMMEND_REC_POWER, 
 
	// <column type="widestr" desc="�������"/> 
	COLUMN_FRIEND_RECOMMEND_REC_GUILD_NAME, 
 
	// <column type="dword" desc="vip�ȼ�"/> 
	COLUMN_FRIEND_RECOMMEND_REC_VIP_LEVEL, 
 
}; 
 
 
//<record name="nearby_recommend_rec" 
//	cols="9"  
//	maxrows="20"  
//	public="false"  
//	private="true"  
//	save="false"  
//	desc="�����Ƽ���(������)"/> 
 
#define FIELD_RECORD_NEARBY_RECOMMEND_REC			"nearby_recommend_rec" 
// nearby_recommend_rec column index define 
enum COLUMNS_OF_NEARBY_RECOMMEND_REC 
{ 
 
	// <column type="string" desc="�Է�UID"/> 
	COLUMN_NEARBY_RECOMMEND_REC_UID, 
 
	// <column type="widestr" desc="�Է�����"/> 
	COLUMN_NEARBY_RECOMMEND_REC_NAME, 
 
	// <column type="word" desc="�Է��ȼ�"/> 
	COLUMN_NEARBY_RECOMMEND_REC_LEVEL, 
 
	// <column type="byte" desc="�Է�ְҵ"/> 
	COLUMN_NEARBY_RECOMMEND_REC_JOB, 
 
	// <column type="byte" desc="�Է��Ա�"/> 
	COLUMN_NEARBY_RECOMMEND_REC_SEX, 
 
	// <column type="dword" desc="ս����"/> 
	COLUMN_NEARBY_RECOMMEND_REC_POWER, 
 
	// <column type="widestr" desc="�������"/> 
	COLUMN_NEARBY_RECOMMEND_REC_GUILD_NAME, 
 
	// <column type="dword" desc="����teamID"/> 
	COLUMN_NEARBY_RECOMMEND_REC_TEAM_ID, 
 
	// <column type="dword" desc="vip�ȼ�"/> 
	COLUMN_NEARBY_RECOMMEND_REC_VIP_LEVEL, 
 
}; 
 
 
//<record name="system_recommend_rec" 
//	cols="9"  
//	maxrows="20"  
//	public="false"  
//	private="true"  
//	save="false"  
//	desc="ϵͳ�Ƽ�����(������)"/> 
 
#define FIELD_RECORD_SYSTEM_RECOMMEND_REC			"system_recommend_rec" 
// system_recommend_rec column index define 
enum COLUMNS_OF_SYSTEM_RECOMMEND_REC 
{ 
 
	// <column type="string" desc="�Է�UID"/> 
	COLUMN_SYSTEM_RECOMMEND_REC_UID, 
 
	// <column type="widestr" desc="�Է�����"/> 
	COLUMN_SYSTEM_RECOMMEND_REC_NAME, 
 
	// <column type="word" desc="�Է��ȼ�"/> 
	COLUMN_SYSTEM_RECOMMEND_REC_LEVEL, 
 
	// <column type="byte" desc="�Է�ְҵ"/> 
	COLUMN_SYSTEM_RECOMMEND_REC_JOB, 
 
	// <column type="byte" desc="�Է��Ա�"/> 
	COLUMN_SYSTEM_RECOMMEND_REC_SEX, 
 
	// <column type="dword" desc="ս����"/> 
	COLUMN_SYSTEM_RECOMMEND_REC_POWER, 
 
	// <column type="widestr" desc="�������"/> 
	COLUMN_SYSTEM_RECOMMEND_REC_GUILD_NAME, 
 
	// <column type="dword" desc="����teamID"/> 
	COLUMN_SYSTEM_RECOMMEND_REC_TEAM_ID, 
 
	// <column type="dword" desc="vip�ȼ�"/> 
	COLUMN_SYSTEM_RECOMMEND_REC_VIP_LEVEL, 
 
}; 
 
 
//<record name="friend_receive_flower_rec" 
//	cols="5"  
//	maxrows="20"  
//	public="false"  
//	private="true"  
//	save="true"  
//	desc="�ջ���¼��"/> 
 
#define FIELD_RECORD_FRIEND_RECEIVE_FLOWER_REC			"friend_receive_flower_rec" 
// friend_receive_flower_rec column index define 
enum COLUMNS_OF_FRIEND_RECEIVE_FLOWER_REC 
{ 
 
	// <column type="string" desc="�Է�UID"/> 
	COLUMN_FRIEND_RECEIVE_FLOWER_REC_UID, 
 
	// <column type="widestr" desc="�Է�����"/> 
	COLUMN_FRIEND_RECEIVE_FLOWER_REC_NAME, 
 
	// <column type="word" desc="�ͻ���"/> 
	COLUMN_FRIEND_RECEIVE_FLOWER_REC_COUNT, 
 
	// <column type="byte" desc="�Է�ְҵ"/> 
	COLUMN_FRIEND_RECEIVE_FLOWER_REC_JOB, 
 
	// <column type="byte" desc="�Է��Ա�"/> 
	COLUMN_FRIEND_RECEIVE_FLOWER_REC_SEX, 
 
}; 
 
 
//<record name="friend_send_flower_rec" 
//	cols="5"  
//	maxrows="20"  
//	public="false"  
//	private="true"  
//	save="true"  
//	desc="�ͻ���¼��"/> 
 
#define FIELD_RECORD_FRIEND_SEND_FLOWER_REC			"friend_send_flower_rec" 
// friend_send_flower_rec column index define 
enum COLUMNS_OF_FRIEND_SEND_FLOWER_REC 
{ 
 
	// <column type="string" desc="�Է�UID"/> 
	COLUMN_FRIEND_SEND_FLOWER_REC_UID, 
 
	// <column type="widestr" desc="�Է�����"/> 
	COLUMN_FRIEND_SEND_FLOWER_REC_NAME, 
 
	// <column type="word" desc="�ͻ���"/> 
	COLUMN_FRIEND_SEND_FLOWER_REC_COUNT, 
 
	// <column type="byte" desc="�Է�ְҵ"/> 
	COLUMN_FRIEND_SEND_FLOWER_REC_JOB, 
 
	// <column type="byte" desc="�Է��Ա�"/> 
	COLUMN_FRIEND_SEND_FLOWER_REC_SEX, 
 
}; 
 
 
//<record name="friend_applay_sworn_rec" 
//	cols="1"  
//	maxrows="50"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="�������"/> 
 
#define FIELD_RECORD_FRIEND_APPLAY_SWORN_REC			"friend_applay_sworn_rec" 
// friend_applay_sworn_rec column index define 
enum COLUMNS_OF_FRIEND_APPLAY_SWORN_REC 
{ 
 
	// <column type="widestr" desc="�Է�Name"/> 
	COLUMN_FRIEND_APPLAY_SWORN_REC_NAME, 
 
}; 
 
 
//<record name="guild_skill_rec" 
//	cols="1"  
//	maxrows="100"  
//	public="false"  
//	private="true"  
//	save="true"  
//	desc="��֯����"/> 
 
#define FIELD_RECORD_GUILD_SKILL_REC			"guild_skill_rec" 
// guild_skill_rec column index define 
enum COLUMNS_OF_GUILD_SKILL_REC 
{ 
 
	// <column type="dword" desc="��������"/> 
	COLUMN_GUILD_SKILL_REC_0000, 
 
}; 
 
 
//<record name="guild_teach_req_rec" 
//	cols="3"  
//	maxrows="1024"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="���������"/> 
 
#define FIELD_RECORD_GUILD_TEACH_REQ_REC			"guild_teach_req_rec" 
// guild_teach_req_rec column index define 
enum COLUMNS_OF_GUILD_TEACH_REQ_REC 
{ 
 
	// <column type="widestr" desc="�Է��ǳ�"/> 
	COLUMN_GUILD_TEACH_REQ_REC_NAME, 
 
	// <column type="dword" desc="��������"/> 
	COLUMN_GUILD_TEACH_REQ_REC_TYPE, 
 
	// <column type="int64" desc="����ʱ��"/> 
	COLUMN_GUILD_TEACH_REQ_REC_TIME, 
 
}; 
 
 
//<record name="shop_manager_rec" 
//	cols="7"  
//	maxrows="5"  
//	public="false"  
//	private="false"  
//	save="true"  
//	desc="�̵�����"/> 
 
#define FIELD_RECORD_SHOP_MANAGER_REC			"shop_manager_rec" 
// shop_manager_rec column index define 
enum COLUMNS_OF_SHOP_MANAGER_REC 
{ 
 
	// <column type="byte" desc="�̵�����"/> 
	COLUMN_SHOP_MANAGER_REC_0000, 
 
	// <column type="byte" desc="��ǰˢ�´���"/> 
	COLUMN_SHOP_MANAGER_REC_0001, 
 
	// <column type="byte" desc="��ǰ����ˢ�´���"/> 
	COLUMN_SHOP_MANAGER_REC_0002, 
 
	// <column type="byte" desc="���ˢ��������"/> 
	COLUMN_SHOP_MANAGER_REC_0003, 
 
	// <column type="byte" desc="�̵�ʹ�õĻ�������"/> 
	COLUMN_SHOP_MANAGER_REC_0004, 
 
	// <column type="dword" desc="ˢ�¼۸�"/> 
	COLUMN_SHOP_MANAGER_REC_0005, 
 
	// <column type="byte" desc="�Ƿ��һ���ֶ�ˢ��"/> 
	COLUMN_SHOP_MANAGER_REC_0006, 
 
}; 
 
 
//<record name="shop_gold_rec" 
//	cols="6"  
//	maxrows="12"  
//	public="false"  
//	private="false"  
//	save="true"  
//	desc="Ԫ���̵�ˢ����Ʒ��"/> 
 
#define FIELD_RECORD_SHOP_GOLD_REC			"shop_gold_rec" 
// shop_gold_rec column index define 
enum COLUMNS_OF_SHOP_GOLD_REC 
{ 
 
	// <column type="string" desc="��ƷID"/> 
	COLUMN_SHOP_GOLD_REC_0000, 
 
	// <column type="dword" desc="��ƷƷ��"/> 
	COLUMN_SHOP_GOLD_REC_0001, 
 
	// <column type="dword" desc="��Ʒ��Ŀ"/> 
	COLUMN_SHOP_GOLD_REC_0002, 
 
	// <column type="dword" desc="�۸�"/> 
	COLUMN_SHOP_GOLD_REC_0003, 
 
	// <column type="dword" desc="Ԫ���۸�"/> 
	COLUMN_SHOP_GOLD_REC_0004, 
 
	// <column type="float" desc="�ۿ�"/> 
	COLUMN_SHOP_GOLD_REC_0005, 
 
}; 
 
 
//<record name="shop_commerce_purchase_record" 
//	cols="3"  
//	maxrows="1024"  
//	public="false"  
//	private="true"  
//	save="true"  
//	desc="�����̻���Ʒ��¼"/> 
 
#define FIELD_RECORD_SHOP_COMMERCE_PURCHASE_RECORD			"shop_commerce_purchase_record" 
// shop_commerce_purchase_record column index define 
enum COLUMNS_OF_SHOP_COMMERCE_PURCHASE_RECORD 
{ 
 
	// <column type="string" desc="��Ʒ����"/> 
	COLUMN_SHOP_COMMERCE_PURCHASE_RECORD_ITEM_ID, 
 
	// <column type="dword" desc="�Ѿ�����ĸ���"/> 
	COLUMN_SHOP_COMMERCE_PURCHASE_RECORD_PUSRCHASE_NUM, 
 
	// <column type="dword" desc="�Ѿ����۵ĸ���"/> 
	COLUMN_SHOP_COMMERCE_PURCHASE_RECORD_SALED_NUM, 
 
}; 
 
 
//<record name="shop_buy_plus_item_record" 
//	cols="4"  
//	maxrows="1024"  
//	public="false"  
//	private="false"  
//	save="true"  
//	desc="������Ӫ�����¼"/> 
 
#define FIELD_RECORD_SHOP_BUY_PLUS_ITEM_RECORD			"shop_buy_plus_item_record" 
// shop_buy_plus_item_record column index define 
enum COLUMNS_OF_SHOP_BUY_PLUS_ITEM_RECORD 
{ 
 
	// <column type="string" desc="��Ʒ����"/> 
	COLUMN_SHOP_BUY_PLUS_ITEM_RECORD_0000, 
 
	// <column type="dword" desc="�Ѿ�����ĸ���"/> 
	COLUMN_SHOP_BUY_PLUS_ITEM_RECORD_0001, 
 
	// <column type="int64" desc="ʱ���"/> 
	COLUMN_SHOP_BUY_PLUS_ITEM_RECORD_0002, 
 
	// <column type="dword" desc="ˢ������"/> 
	COLUMN_SHOP_BUY_PLUS_ITEM_RECORD_0003, 
 
}; 
 
 
//<record name="payed_order" 
//	cols="8"  
//	maxrows="1024"  
//	public="false"  
//	private="false"  
//	save="true"  
//	desc="����֧����¼(��¼�����1024��)"/> 
 
#define FIELD_RECORD_PAYED_ORDER			"payed_order" 
// payed_order column index define 
enum COLUMNS_OF_PAYED_ORDER 
{ 
 
	// <column type="string" desc="����id"/> 
	COLUMN_PAYED_ORDER_ORDER_ID, 
 
	// <column type="string" desc="��Ʒid"/> 
	COLUMN_PAYED_ORDER_PRODUCT_ID, 
 
	// <column type="string" desc="��Ʒ����"/> 
	COLUMN_PAYED_ORDER_ITEMS, 
 
	// <column type="string" desc="������Ʒ"/> 
	COLUMN_PAYED_ORDER_REWARDS, 
 
	// <column type="string" desc="�׳����"/> 
	COLUMN_PAYED_ORDER_GIFTS, 
 
	// <column type="float" desc="֧���Ľ��"/> 
	COLUMN_PAYED_ORDER_AMOUNT, 
 
	// <column type="int64" desc="��������ʱ��"/> 
	COLUMN_PAYED_ORDER_CREATE_TIME, 
 
	// <column type="int64" desc="�������ʱ��"/> 
	COLUMN_PAYED_ORDER_PAYED_TIME, 
 
}; 
 
 
//<record name="payed_products" 
//	cols="4"  
//	maxrows="256"  
//	public="false"  
//	private="false"  
//	save="true"  
//	desc="���������Ʒ"/> 
 
#define FIELD_RECORD_PAYED_PRODUCTS			"payed_products" 
// payed_products column index define 
enum COLUMNS_OF_PAYED_PRODUCTS 
{ 
 
	// <column type="string" desc="��Ʒid"/> 
	COLUMN_PAYED_PRODUCTS_PRODUCT_ID, 
 
	// <column type="int" desc="�ܹ������"/> 
	COLUMN_PAYED_PRODUCTS_TATAL_TIMES, 
 
	// <column type="int" desc="�������"/> 
	COLUMN_PAYED_PRODUCTS_PURCHASE_TIMES, 
 
	// <column type="int64" desc="���һ��֧����ɵ��µ�ʱ��"/> 
	COLUMN_PAYED_PRODUCTS_ORDER_TIME, 
 
}; 
 
 
//<record name="item_drop_record" 
//	cols="3"  
//	maxrows="30"  
//	public="false"  
//	private="true"  
//	save="false"  
//	desc="������ҵ�����Ʒ"/> 
 
#define FIELD_RECORD_ITEM_DROP_RECORD			"item_drop_record" 
// item_drop_record column index define 
enum COLUMNS_OF_ITEM_DROP_RECORD 
{ 
 
	// <column type="string" desc="������Ʒid"/> 
	COLUMN_ITEM_DROP_RECORD_0000, 
 
	// <column type="int64" desc="������Ʒ����"/> 
	COLUMN_ITEM_DROP_RECORD_0001, 
 
	// <column type="WORD" desc="������Ʒcdʱ��"/> 
	COLUMN_ITEM_DROP_RECORD_0002, 
 
}; 
 
 
//<record name="system_mail_rec" 
//	cols="12"  
//	maxrows="256"  
//	public="false"  
//	private="false"  
//	save="true"  
//	desc="�ʼ���"/> 
 
#define FIELD_RECORD_SYSTEM_MAIL_REC			"system_mail_rec" 
// system_mail_rec column index define 
enum COLUMNS_OF_SYSTEM_MAIL_REC 
{ 
 
	// <column type="string" desc="�ż���ˮ��"/> 
	COLUMN_SYSTEM_MAIL_REC_0000, 
 
	// <column type="widestr" desc="������"/> 
	COLUMN_SYSTEM_MAIL_REC_0001, 
 
	// <column type="int64" desc="����ʱ��"/> 
	COLUMN_SYSTEM_MAIL_REC_0002, 
 
	// <column type="int64" desc="�ʼ�ʱ��"/> 
	COLUMN_SYSTEM_MAIL_REC_0003, 
 
	// <column type="byte" desc="�ż�����"/> 
	COLUMN_SYSTEM_MAIL_REC_0004, 
 
	// <column type="widestr" desc="�ż���Ŀ"/> 
	COLUMN_SYSTEM_MAIL_REC_0005, 
 
	// <column type="widestr" desc="�ż�����"/> 
	COLUMN_SYSTEM_MAIL_REC_0006, 
 
	// <column type="string" desc="��Ǯ����"/> 
	COLUMN_SYSTEM_MAIL_REC_0007, 
 
	// <column type="string" desc="��Ʒ����"/> 
	COLUMN_SYSTEM_MAIL_REC_0008, 
 
	// <column type="byte" desc="�Ѷ����"/> 
	COLUMN_SYSTEM_MAIL_REC_0009, 
 
	// <column type="byte" desc="������ȡ���"/> 
	COLUMN_SYSTEM_MAIL_REC_0010, 
 
	// <column type="string" desc="���Ը���"/> 
	COLUMN_SYSTEM_MAIL_REC_0011, 
 
}; 
 
 
//<record name="reset_timer_rec" 
//	cols="4"  
//	maxrows="250"  
//	public="false"  
//	private="false"  
//	save="true"  
//	desc="��ʱˢ�±�"/> 
 
#define FIELD_RECORD_RESET_TIMER_REC			"reset_timer_rec" 
// reset_timer_rec column index define 
enum COLUMNS_OF_RESET_TIMER_REC 
{ 
 
	// <column type="dword" desc="ˢ������"/> 
	COLUMN_RESET_TIMER_REC_TYPE, 
 
	// <column type="word" desc="�ϴ�ˢ��weekday"/> 
	COLUMN_RESET_TIMER_REC_WEEKDAY, 
 
	// <column type="word" desc="�ϴ�ˢ��Hour"/> 
	COLUMN_RESET_TIMER_REC_HOUR, 
 
	// <column type="double" desc="�ϴ�ˢ��Date"/> 
	COLUMN_RESET_TIMER_REC_DATE, 
 
}; 
 
 
//<record name="TaskProgressRec" 
//	cols="3"  
//	maxrows="512"  
//	public="false"  
//	private="true"  
//	save="true"  
//	desc="������ȱ�"/> 
 
#define FIELD_RECORD_TASK_PROGRESS_REC			"TaskProgressRec" 
// TaskProgressRec column index define 
enum COLUMNS_OF_TASK_PROGRESS_REC 
{ 
 
	// <column type="dword" desc="������"/> 
	COLUMN_TASK_PROGRESS_REC_ID, 
 
	// <column type="word" desc="����״̬"/> 
	COLUMN_TASK_PROGRESS_REC_STATUS, 
 
	// <column type="string" desc="��ǰ����"/> 
	COLUMN_TASK_PROGRESS_REC_PROGRESS, 
 
}; 
 
 
//<record name="TaskSubmitRec" 
//	cols="1"  
//	maxrows="5000"  
//	public="false"  
//	private="true"  
//	save="true"  
//	desc="���ύ�����"/> 
 
#define FIELD_RECORD_TASK_SUBMIT_REC			"TaskSubmitRec" 
// TaskSubmitRec column index define 
enum COLUMNS_OF_TASK_SUBMIT_REC 
{ 
 
	// <column type="dword" desc="������"/> 
	COLUMN_TASK_SUBMIT_REC_ID, 
 
}; 
 
 
//<record name="TaskNpcRec" 
//	cols="1"  
//	maxrows="512"  
//	public="false"  
//	private="false"  
//	save="true"  
//	desc="����NPC��¼��"/> 
 
#define FIELD_RECORD_TASK_NPC_REC			"TaskNpcRec" 
// TaskNpcRec column index define 
enum COLUMNS_OF_TASK_NPC_REC 
{ 
 
	// <column type="object" desc="����NPC����"/> 
	COLUMN_TASK_NPC_REC_OBJECT, 
 
}; 
 
 
//<record name="TaskConfigRec" 
//	cols="2"  
//	maxrows="50"  
//	public="false"  
//	private="true"  
//	save="true"  
//	desc="�������ñ�"/> 
 
#define FIELD_RECORD_TASK_CONFIG_REC			"TaskConfigRec" 
// TaskConfigRec column index define 
enum COLUMNS_OF_TASK_CONFIG_REC 
{ 
 
	// <column type="word" desc="��������"/> 
	COLUMN_TASK_CONFIG_REC_TYPE, 
 
	// <column type="word" desc="����ɴ���"/> 
	COLUMN_TASK_CONFIG_REC_COUNT, 
 
}; 
 
 
//<record name="team_rec" 
//	cols="22"  
//	maxrows="4"  
//	public="false"  
//	private="true"  
//	save="true"  
//	desc="�����Ա�б�"/> 
 
#define FIELD_RECORD_TEAM_REC			"team_rec" 
// team_rec column index define 
enum COLUMNS_OF_TEAM_REC 
{ 
 
	// <column type="string" desc="UID"/> 
	COLUMN_TEAM_REC_0000, 
 
	// <column type="widestr" desc="��Ա��"/> 
	COLUMN_TEAM_REC_0001, 
 
	// <column type="dword" desc="�ȼ�"/> 
	COLUMN_TEAM_REC_0002, 
 
	// <column type="byte" desc="ְҵ"/> 
	COLUMN_TEAM_REC_0003, 
 
	// <column type="byte" desc="�Ա�"/> 
	COLUMN_TEAM_REC_0004, 
 
	// <column type="dword" desc="ս����"/> 
	COLUMN_TEAM_REC_0005, 
 
	// <column type="int64" desc="�ϴ�����ʱ��"/> 
	COLUMN_TEAM_REC_0006, 
 
	// <column type="byte" desc="�����ְλ"/> 
	COLUMN_TEAM_REC_0007, 
 
	// <column type="dword" desc="�������"/> 
	COLUMN_TEAM_REC_0008, 
 
	// <column type="string" desc="Buffers"/> 
	COLUMN_TEAM_REC_0009, 
 
	// <column type="int64" desc="HP"/> 
	COLUMN_TEAM_REC_0010, 
 
	// <column type="widestr" desc="��������"/> 
	COLUMN_TEAM_REC_0011, 
 
	// <column type="int64" desc="MaxHP"/> 
	COLUMN_TEAM_REC_0012, 
 
	// <column type="byte" desc="����״̬"/> 
	COLUMN_TEAM_REC_0013, 
 
	// <column type="byte" desc="׼��״̬"/> 
	COLUMN_TEAM_REC_0014, 
 
	// <column type="dword" desc="����ID"/> 
	COLUMN_TEAM_REC_0015, 
 
	// <column type="byte" desc="Online"/> 
	COLUMN_TEAM_REC_0016, 
 
	// <column type="float" desc="x����"/> 
	COLUMN_TEAM_REC_0017, 
 
	// <column type="float" desc="z����"/> 
	COLUMN_TEAM_REC_0018, 
 
	// <column type="byte" desc="�ܷ��߳�����"/> 
	COLUMN_TEAM_REC_0019, 
 
	// <column type="byte" desc="ս��״̬"/> 
	COLUMN_TEAM_REC_0020, 
 
	// <column type="word" desc="vip"/> 
	COLUMN_TEAM_REC_0021, 
 
}; 
 
 
//<record name="nearby_team_rec" 
//	cols="8"  
//	maxrows="10"  
//	public="false"  
//	private="true"  
//	save="false"  
//	desc="���������"/> 
 
#define FIELD_RECORD_NEARBY_TEAM_REC			"nearby_team_rec" 
// nearby_team_rec column index define 
enum COLUMNS_OF_NEARBY_TEAM_REC 
{ 
 
	// <column type="byte" desc="������"/> 
	COLUMN_NEARBY_TEAM_REC_0000, 
 
	// <column type="widestr" desc="�ӳ�����"/> 
	COLUMN_NEARBY_TEAM_REC_0001, 
 
	// <column type="dword" desc="�ӳ��ȼ�"/> 
	COLUMN_NEARBY_TEAM_REC_0002, 
 
	// <column type="byte" desc="�ӳ�ְҵ"/> 
	COLUMN_NEARBY_TEAM_REC_0003, 
 
	// <column type="byte" desc="�ӳ��Ա�"/> 
	COLUMN_NEARBY_TEAM_REC_0004, 
 
	// <column type="dword" desc="�ӳ�ս����"/> 
	COLUMN_NEARBY_TEAM_REC_0005, 
 
	// <column type="dword" desc="����Ŀ��"/> 
	COLUMN_NEARBY_TEAM_REC_0006, 
 
	// <column type="byte" desc="��������"/> 
	COLUMN_NEARBY_TEAM_REC_0007, 
 
}; 
 
 
//<record name="request_rec" 
//	cols="9"  
//	maxrows="30"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="��������߱�(��¼��ǰ��������͹�����������)"/> 
 
#define FIELD_RECORD_REQUEST_REC			"request_rec" 
// request_rec column index define 
enum COLUMNS_OF_REQUEST_REC 
{ 
 
	// <column type="byte" desc="��������"/> 
	COLUMN_REQUEST_REC_0000, 
 
	// <column type="widestr" desc="����������"/> 
	COLUMN_REQUEST_REC_0001, 
 
	// <column type="byte" desc="��������"/> 
	COLUMN_REQUEST_REC_0002, 
 
	// <column type="byte" desc="����ʱ��"/> 
	COLUMN_REQUEST_REC_0003, 
 
	// <column type="byte" desc="����״̬"/> 
	COLUMN_REQUEST_REC_0004, 
 
	// <column type="byte" desc="ְҵ"/> 
	COLUMN_REQUEST_REC_0005, 
 
	// <column type="dword" desc="ս����"/> 
	COLUMN_REQUEST_REC_0006, 
 
	// <column type="word" desc="�ȼ�"/> 
	COLUMN_REQUEST_REC_0007, 
 
	// <column type="word" desc="�Ա�"/> 
	COLUMN_REQUEST_REC_0008, 
 
}; 
 
 
//<record name="buff_add_rec" 
//	cols="2"  
//	maxrows="256"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="��buffͳ�Ʊ�"/> 
 
#define FIELD_RECORD_BUFF_ADD_REC			"buff_add_rec" 
// buff_add_rec column index define 
enum COLUMNS_OF_BUFF_ADD_REC 
{ 
 
	// <column type="int64" desc="��buff��ʱ��"/> 
	COLUMN_BUFF_ADD_REC_0000, 
 
	// <column type="dword" desc="debuff������"/> 
	COLUMN_BUFF_ADD_REC_0001, 
 
}; 
 
 
//<record name="flow_hittime_rec" 
//	cols="2"  
//	maxrows="64"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc=""/> 
 
#define FIELD_RECORD_FLOW_HITTIME_REC			"flow_hittime_rec" 
// flow_hittime_rec column index define 
enum COLUMNS_OF_FLOW_HITTIME_REC 
{ 
 
	// <column type="word" desc=""/> 
	COLUMN_FLOW_HITTIME_REC_0000, 
 
	// <column type="word" desc=""/> 
	COLUMN_FLOW_HITTIME_REC_0001, 
 
}; 
 
 
//<record name="flow_mutli_hit_timer_rec" 
//	cols="2"  
//	maxrows="64"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc=""/> 
 
#define FIELD_RECORD_FLOW_MUTLI_HIT_TIMER_REC			"flow_mutli_hit_timer_rec" 
// flow_mutli_hit_timer_rec column index define 
enum COLUMNS_OF_FLOW_MUTLI_HIT_TIMER_REC 
{ 
 
	// <column type="int64" desc="��ʼʱ��"/> 
	COLUMN_FLOW_MUTLI_HIT_TIMER_REC_0000, 
 
	// <column type="int64" desc="��ʱʱ��"/> 
	COLUMN_FLOW_MUTLI_HIT_TIMER_REC_0001, 
 
}; 
 
 
//<record name="cooldown_rec" 
//	cols="3"  
//	maxrows="36"  
//	public="false"  
//	private="true"  
//	save="true"  
//	desc="��ȴ��"/> 
 
#define FIELD_RECORD_COOLDOWN_REC			"cooldown_rec" 
// cooldown_rec column index define 
enum COLUMNS_OF_COOLDOWN_REC 
{ 
 
	// <column type="dword" desc="��ȴ����ID"/> 
	COLUMN_COOLDOWN_REC_0000, 
 
	// <column type="int64" desc="��ȴ��ʼʱ�䣬��ʵ64λʱ������"/> 
	COLUMN_COOLDOWN_REC_0001, 
 
	// <column type="int64" desc="��ȴ����ʱ�䣬��ʵ64λʱ������"/> 
	COLUMN_COOLDOWN_REC_0002, 
 
}; 
 
 
//<record name="PropModifyRec" 
//	cols="3"  
//	maxrows="1024"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="��������������"/> 
 
#define FIELD_RECORD_PROP_MODIFY_REC			"PropModifyRec" 
// PropModifyRec column index define 
enum COLUMNS_OF_PROP_MODIFY_REC 
{ 
 
	// <column type="string" desc="����������"/> 
	COLUMN_PROP_MODIFY_REC_0000, 
 
	// <column type="float" desc="��������ֵ"/> 
	COLUMN_PROP_MODIFY_REC_0001, 
 
	// <column type="string" desc="������Դ"/> 
	COLUMN_PROP_MODIFY_REC_0002, 
 
}; 
 
 
//<record name="passive_skill_rec" 
//	cols="3"  
//	maxrows="1000"  
//	public="false"  
//	private="true"  
//	save="true"  
//	desc=""/> 
 
#define FIELD_RECORD_PASSIVE_SKILL_REC			"passive_skill_rec" 
// passive_skill_rec column index define 
enum COLUMNS_OF_PASSIVE_SKILL_REC 
{ 
 
	// <column type="DWORD" desc="����id"/> 
	COLUMN_PASSIVE_SKILL_REC_0000, 
 
	// <column type="DWORD" desc="���ܵȼ�"/> 
	COLUMN_PASSIVE_SKILL_REC_0001, 
 
	// <column type="DWORD" desc="����������Դ"/> 
	COLUMN_PASSIVE_SKILL_REC_0002, 
 
}; 
 
 
//<record name="PropModifyByEquip" 
//	cols="3"  
//	maxrows="300"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="����ˢ�±�"/> 
 
#define FIELD_RECORD_PROP_MODIFY_BY_EQUIP			"PropModifyByEquip" 
// PropModifyByEquip column index define 
enum COLUMNS_OF_PROP_MODIFY_BY_EQUIP 
{ 
 
	// <column type="string" desc="������"/> 
	COLUMN_PROP_MODIFY_BY_EQUIP_0000, 
 
	// <column type="float" desc="����ֵ"/> 
	COLUMN_PROP_MODIFY_BY_EQUIP_0001, 
 
	// <column type="string" desc="��Դ"/> 
	COLUMN_PROP_MODIFY_BY_EQUIP_0002, 
 
}; 
 
 
//<record name="fashion_unlock_rec" 
//	cols="4"  
//	maxrows="100"  
//	public="false"  
//	private="true"  
//	save="true"  
//	desc="�ѽ���ʱװ��"/> 
 
#define FIELD_RECORD_FASHION_UNLOCK_REC			"fashion_unlock_rec" 
// fashion_unlock_rec column index define 
enum COLUMNS_OF_FASHION_UNLOCK_REC 
{ 
 
	// <column type="word" desc="ʱװid"/> 
	COLUMN_FASHION_UNLOCK_REC_ID, 
 
	// <column type="word" desc="ʱװ����"/> 
	COLUMN_FASHION_UNLOCK_REC_TYPE, 
 
	// <column type="int64" desc="����ʱ��"/> 
	COLUMN_FASHION_UNLOCK_REC_UNLOCK_TIME, 
 
	// <column type="int64" desc="ʱЧʱ��"/> 
	COLUMN_FASHION_UNLOCK_REC_VALID_TIME, 
 
}; 
 
 
//<record name="ApperanceRec" 
//	cols="3"  
//	maxrows="20"  
//	public="false"  
//	private="false"  
//	save="true"  
//	desc="��۹���"/> 
 
#define FIELD_RECORD_APPERANCE_REC			"ApperanceRec" 
// ApperanceRec column index define 
enum COLUMNS_OF_APPERANCE_REC 
{ 
 
	// <column type="WORD" desc="���id"/> 
	COLUMN_APPERANCE_REC_ID, 
 
	// <column type="BYTE" desc="�����Դ"/> 
	COLUMN_APPERANCE_REC_SOURCE, 
 
	// <column type="WORD" desc="������ͣ�װ����λ��"/> 
	COLUMN_APPERANCE_REC_TYPE, 
 
}; 
 
 
//<record name="creator_id_rec" 
//	cols="2"  
//	maxrows="4096"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="��������id��Ӧ��"/> 
 
#define FIELD_RECORD_CREATOR_ID_REC			"creator_id_rec" 
// creator_id_rec column index define 
enum COLUMNS_OF_CREATOR_ID_REC 
{ 
 
	// <column type="string" desc="������id"/> 
	COLUMN_CREATOR_ID_REC_0000, 
 
	// <column type="object" desc="����������"/> 
	COLUMN_CREATOR_ID_REC_0001, 
 
}; 
 
 
//<record name="create_group_static_flow" 
//	cols="4"  
//	maxrows="1024"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="���鳡����̬npc��������"/> 
 
#define FIELD_RECORD_CREATE_GROUP_STATIC_FLOW			"create_group_static_flow" 
// create_group_static_flow column index define 
enum COLUMNS_OF_CREATE_GROUP_STATIC_FLOW 
{ 
 
	// <column type="word" desc="SubSceneNo"/> 
	COLUMN_CREATE_GROUP_STATIC_FLOW_0000, 
 
	// <column type="word" desc="�����"/> 
	COLUMN_CREATE_GROUP_STATIC_FLOW_0001, 
 
	// <column type="word" desc="���ڴ������ļ����"/> 
	COLUMN_CREATE_GROUP_STATIC_FLOW_0002, 
 
	// <column type="word" desc="���ڴ�����npc����"/> 
	COLUMN_CREATE_GROUP_STATIC_FLOW_0003, 
 
}; 
 
 
//<record name="create_group_random_flow" 
//	cols="4"  
//	maxrows="1024"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="���鳡�������������������"/> 
 
#define FIELD_RECORD_CREATE_GROUP_RANDOM_FLOW			"create_group_random_flow" 
// create_group_random_flow column index define 
enum COLUMNS_OF_CREATE_GROUP_RANDOM_FLOW 
{ 
 
	// <column type="word" desc="SubSceneNo"/> 
	COLUMN_CREATE_GROUP_RANDOM_FLOW_0000, 
 
	// <column type="word" desc="�����"/> 
	COLUMN_CREATE_GROUP_RANDOM_FLOW_0001, 
 
	// <column type="word" desc="���ڴ������ļ����"/> 
	COLUMN_CREATE_GROUP_RANDOM_FLOW_0002, 
 
	// <column type="word" desc="���ڴ�����npc����"/> 
	COLUMN_CREATE_GROUP_RANDOM_FLOW_0003, 
 
}; 
 
 
//<record name="kiki_rec" 
//	cols="1"  
//	maxrows="1024"  
//	public="false"  
//	private="false"  
//	save="false"  
//	desc="���Ա��"/> 
 
#define FIELD_RECORD_KIKI_REC			"kiki_rec" 
// kiki_rec column index define 
enum COLUMNS_OF_KIKI_REC 
{ 
 
	// <column type="int" desc="������"/> 
	COLUMN_KIKI_REC_0000, 
 
}; 
 
 
#endif // _FIELDS_DEFINE_H_