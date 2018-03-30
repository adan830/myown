//--------------------------------------------------------------------
// �ļ���:		IKernel.h
// ��  ��:		���Ĺ��ܽӿ�
// ˵  ��:		
// ��������:	2004��1��4��
// ������:		½����
// ��Ȩ����:	������ţ�������޹�˾
//--------------------------------------------------------------------

#ifndef _SERVER_IROOMKERNEL_H
#define _SERVER_IROOMKERNEL_H
#define _SERVER_IKERNEL_H


#include "../public/Macros.h"
#include "../public/Var.h"
#include "../public/VarList.h"
#include "LenType.h"
#include "RoomKnlConst.h"
#include "IGameObj.h"
#include "IRoomLogicModule.h"

#ifdef _DEBUG
	// ��������Ϸ�߼��ӿڰ汾
	#define GAME_LOGIC_MODULE_VERSION 0x1000
#else
	// ��������Ϸ�߼��ӿڰ汾
	#define GAME_LOGIC_MODULE_VERSION 0x1001
#endif




// ���Ĺ��ܽӿ�

class IKernel
{
public:
	virtual ~IKernel() = 0;

	/// \brief �����Դ�ļ�·��
	virtual const char* GetResourcePath() = 0;

	/// \brief ���ָ�����Ƶ��߼�ģ��
	/// \param name �߼�ģ������
	virtual IRoomLogicModule* GetLogicModule(const char* name) = 0;

	/// \brief ����߼���
	/// \param logic_class �߼�����
	/// \param type �������ͣ���KnlConst.h��
	/// \param parent_class ���߼���
	virtual bool AddLogicClass(const char* logic_class, int type,
		const char* parent_class) = 0;
	/// \brief ����߼���ص�
	/// \param logic_class �߼�����
	/// \param event �¼�����
	/// \param func �ص�����ָ��
	/// \param prior ���ȼ�����ֵԽС���ȼ�Խ�ߣ�
	virtual bool AddClassCallback(const char* logic_class, 
		const char* event, ROOM_LOGIC_CLASS_FUNC func, int prior = 0) = 0;
	/// \brief ����߼��¼��ص�
	/// \param logic_class �߼�����
	/// \param event �¼�����
	/// \param func �ص�����ָ��
	/// \param prior ���ȼ�����ֵԽС���ȼ�Խ�ߣ�
	virtual bool AddEventCallback(const char* logic_class,
		const char* event, ROOM_LOGIC_EVENT_FUNC func, int prior = 0) = 0;
	/// \brief ����ַ�����ŵĶ����������
	/// \param logic_class �߼�����
	/// \param msg_id ��Ϣ���
	/// \param func ������ָ��
	/// \param prior ���ȼ�����ֵԽС���ȼ�Խ�ߣ�
	virtual bool AddCommandHook(const char* logic_class,
		const char* msg_id, ROOM_LOGIC_EVENT_FUNC func, int prior = 0) = 0;
	/// \brief ���������ŵĶ����������
	/// \param logic_class �߼�����
	/// \param msg_id ��Ϣ���(0-1023)
	/// \param func ������ָ��
	/// \param prior ���ȼ�����ֵԽС���ȼ�Խ�ߣ�
	virtual bool AddIntCommandHook(const char* logic_class,
		int msg_id, ROOM_LOGIC_EVENT_FUNC func, int prior = 0) = 0;
	/// \brief ����ַ�����ŵĶ���ͻ�����Ϣ������
	/// \param logic_class �߼�����
	/// \param msg_id ��Ϣ���
	/// \param func ������ָ��
	/// \param prior ���ȼ�����ֵԽС���ȼ�Խ�ߣ�
	virtual bool AddCustomHook(const char* logic_class,
		const char* msg_id, ROOM_LOGIC_EVENT_FUNC func, int prior = 0) = 0;
	/// \brief ���������ŵĶ���ͻ�����Ϣ������
	/// \param logic_class �߼�����
	/// \param msg_id ��Ϣ���(0-1023)
	/// \param func ������ָ��
	/// \param prior ���ȼ�����ֵԽС���ȼ�Խ�ߣ�
	virtual bool AddIntCustomHook(const char* logic_class,
		int msg_id, ROOM_LOGIC_EVENT_FUNC func, int prior = 0) = 0;
	/// \brief ִ��ָ��������¼��ص�����
	/// \param event �¼�����
	/// \param self ������
	/// \param sender ��������
	/// \param args �����б�
	virtual bool RunEventCallback(const char* event, const PERSISTID& self, 
		const PERSISTID& sender, const IVarList& args) = 0;

	/// \brief ����LUA��չ����
	/// \param name ������
	/// \param func ����ָ��
	virtual bool DeclareLuaExt(const char* name, ROOM_LUA_EXT_FUNC func) = 0;
	/// \brief ������������
	/// \param name ������
	/// \param func ����ָ��
	virtual bool DeclareHeartBeat(const char* name, ROOM_HEARTBEAT_FUNC func) = 0;
	/// \brief �������Իص�����
	/// \param name ������
	/// \param func ����ָ��
	virtual bool DeclareCritical(const char* name, ROOM_CRITICAL_FUNC func) = 0;
	/// \brief ������ص�����
	/// \param name ������
	/// \param func ����ָ��
	virtual bool DeclareRecHook(const char* name, ROOM_RECHOOK_FUNC func) = 0;

	/// \brief ������������
	/// \param name ������
	virtual ROOM_HEARTBEAT_FUNC LookupHeartBeat(const char* name) = 0;
	/// \brief �������Իص�����
	/// \param name ������
	virtual ROOM_CRITICAL_FUNC LookupCritical(const char* name) = 0;
	/// \brief ���ұ��ص�����
	/// \param name ������
	virtual ROOM_RECHOOK_FUNC LookupRecHook(const char* name) = 0;

	/// \brief �����ϵͳLOG�ļ�
	/// \param info ��Ϣ����
	virtual void Trace(const char* info) = 0;
	/// \brief ���������̨��Ļ
	/// \param info ��Ϣ����
	virtual void Echo(const char* info) = 0;
	
	/// \brief ��������
	/// \param ����
	virtual bool CheckName(const wchar_t* name) = 0;

	/// \brief ��õ�ǰƽ̨��
	virtual int GetPlatformId() = 0;
	/// \brief ��ó������������
	virtual int GetRoomId() = 0;

	/// \brief ��ö���ָ��
	/// \param id �����
	virtual IGameObj* GetGameObj(const PERSISTID& id) = 0;
	/// \brief ��ó�������ָ��
	virtual IGameObj* GetSceneObj() = 0;

	/// \brief ȡ��ǰ��������
	virtual PERSISTID GetScene() = 0;
	/// \brief ��õ�ǰ����ID
	virtual int GetSceneId() = 0;
	/// \brief ���������ͨ����ID
	virtual int GetSceneMaxId() = 0;
	/// \brief �����ͨ�����߼�����
	/// \param id �������
	virtual const char* GetSceneScript(int id) = 0;
	/// \brief �����ͨ����������
	/// \param id �������
	virtual const char* GetSceneConfig(int id) = 0;
	/// \brief ����ָ������������ͨ������ţ�����-1��ʾû�ҵ�
	/// \param config ����������
	virtual int FindSceneId(const char* config) = 0;
	/// \brief �ж�ָ����ŵĳ����Ƿ����
	/// \param id �������
	virtual bool GetSceneExists(int id) = 0;
	/// \brief ���ָ�������ڵ������������
	/// \param id �������
	virtual int GetSceneOnlineCount(int id) = 0;
	/// \brief ���ָ�������ڵ�����������������ߺ����ߣ�
	/// \param id �������
	virtual int GetScenePlayerCount(int id) = 0;

	/// \brief ���ָ�������ڵ�������������б�idΪ0��ʾ���г�����
	/// \param id �������
	/// \param result ������������б�
	virtual int GetSceneOnlinePlayerList(int id, IVarList& result) = 0;

	/// \brief ���ָ�������ڵ���������б�
	/// \param id �������
	/// \param result ��������б��ڴ���� �������ߺ����ߵģ�
	virtual int GetScenePlayerList(int id, IVarList& result) = 0;

	/// \brief ȡ��ǰ���������ͣ�0-��ͨ������1-����������ԭ�ͣ�2-����������
	virtual int GetSceneClass() = 0;
	/// \brief ���󴴽�������������OnCloneScene�ص��ﷵ�ؽ��
	/// \param prototype_scene_id ԭ�ͳ������
	/// \param owner_name ����������
	/// \param down_time ����ʱ�䣨�룩
	/// \param reuse �Ƿ�����ظ�ʹ��δ���յĸ���
	virtual bool RequestCloneScene(int prototype_scene_id, 
		const wchar_t* owner_name, int down_time, bool reuse) = 0;
	/// \brief ���ø��������Ļ���ʱ��
	/// \param clone_scene_id �����������
	/// \param down_time ����ʱ�䣨�룩
	virtual bool SetCloneSceneDownTime(int clone_scene_id, int down_time) = 0;
	/// \brief ���ָ��ԭ�ͳ����ŵĸ�������
	/// \param prototype_scene_id ԭ�ͳ�����ţ�Ϊ0��ʾ�������еĸ�������
	virtual int GetCloneSceneCount(int prototype_scene_id) = 0;
	/// \brief ���ָ��ԭ�ͳ����ŵĸ����б�
	/// \param prototype_scene_id ԭ�ͳ�����ţ�Ϊ0��ʾ�������еĸ���
	/// \param result �����������б�
	virtual int GetCloneSceneList(int prototype_scene_id, IVarList& result) = 0;
	/// \brief ��ø���������ԭ�ͳ������
	/// \param clone_scene_id �����������
	virtual int GetPrototypeSceneId(int clone_scene_id) = 0;
	/// \brief ָ���������Ƿ��Ǹ���������ԭ�ͳ���
	/// \param scene_id ��ͨ�������
	virtual bool IsPrototypeScene(int scene_id) = 0;

	/// \brief ���ݽ�ɫ����ý�ɫΨһ��ʶ�����ؿ��ַ�����ʾ��ɫ������
	/// \param role_name ��ɫ��
	virtual const char* SeekRoleUid(const wchar_t* role_name) = 0;
	/// \brief ���ݽ�ɫΨһ��ʶ��ý�ɫ�������ؿ��ַ�����ʾ��ɫ������
	/// \param role_uid ��ɫΨһ��ʶ
	virtual const wchar_t* SeekRoleName(const char* role_uid) = 0;
	/// \brief ��ý�ɫ�Ƿ��ѱ�ɾ��
	/// \param role_name ��ɫ��
	virtual bool GetRoleDeleted(const wchar_t* role_name) = 0;
	/// \brief ��ý�ɫ�Ƿ���ɾ��������
	/// \param role_name ��ɫ��
	virtual bool GetRoleDeleteProtect(const wchar_t* role_name) = 0;
	
	/// \brief ȡ��ͼ��Χ
	/// \param left,top,right,bottom �������Ϻ����½ǵ�����
	virtual bool GetMapBound(len_t& left, len_t& top, len_t& right, 
		len_t& bottom) = 0;
	/// \brief ȡ��ͼ�߶�yֵ������ͼ���ص�����ߵĸ߶ȣ�
	/// \param x,z λ��
	virtual len_t GetMapHeight(len_t x, len_t z) = 0;
	/// \brief ��÷�Χ����Ƿ���Ч
	/// \param name �߼���Χ��
	/// \param x,z λ��
	virtual bool GetMapRegion(const char* name, len_t x, len_t z) = 0;
	/// \brief ȡ��������
	/// \param x,z λ��
	virtual const char* GetMapArea(len_t x, len_t z) = 0;
	/// \brief ȡ�ر�����
	/// \param x,z λ��
	virtual int GetMapType(len_t x, len_t z) = 0;
	/// \brief ȡ�������ͣ�����ͼֻ���ص�ǰλ���Ƿ�����ɫ�����߲㣩
	/// \param x,z λ��
	virtual int GetWalkType(len_t x, len_t z) = 0;
	/// \brief ����һ�����Ƿ���������򣨶���ͼֻ���ص�ǰλ���Ƿ�����ɫ�����߲㣩
	/// \param x,z λ��
	virtual bool CanWalk(len_t x, len_t z) = 0;
	/// \brief ����һ��������Ƿ�����ߣ������ڶ���ͼʹ�ô˺�����
	/// \param obj ����
	/// \param x,z λ��
	virtual bool ObjectCanWalk(const PERSISTID& obj, len_t x, len_t z) = 0;
	/// \brief ֱ�߿����ߣ������ڶ���ͼʹ�ô˺�����
	/// \param step ����
	/// \param src_x,src_z ��ʼ��
	/// \param dst_x,dst_z ������
	virtual bool LineCanWalk(len_t step, len_t src_x, len_t src_z, 
		len_t dst_x, len_t dst_z) = 0;
	/// \brief ����һ��ֱ�߶�������ߣ������ڶ���ͼʹ�ô˺�����
	/// \param obj ����
	/// \param step ����
	/// \param src_x,src_z ��ʼ��
	/// \param dst_x,dst_z ������
	virtual bool ObjectLineCanWalk(const PERSISTID& obj, len_t step, 
		len_t src_x, len_t src_z, len_t dst_x, len_t dst_z) = 0;

	/// \brief ֱ���ƶ����ԣ��ܵ���Ŀ��㷵��true�����򷵻�false�Ϳ��Ե����λ�ã�
	/// \param walk_step �����߸߶Ȳ�
	/// \param src_x,src_y,src_z ��ʼ��
	/// \param dst_x,dst_z Ŀ���
	/// \return new_x,new_y,new_z ���Ե���ĵ�λ��
	virtual bool TraceLineWalk(len_t walk_step, len_t src_x, len_t src_y,
		len_t src_z, len_t dst_x, len_t dst_z, len_t& new_x, len_t& new_y,
		len_t& new_z) = 0;

	/// \brief ��ײ�����Ƿ����
	/// \param x,z λ��
	virtual bool GetCollideEnable(len_t x, len_t z) = 0;
	/// \brief ���ָ��λ�õ���ߵ�߶�
	/// \param x,z λ��
	virtual len_t GetApexHeight(len_t x, len_t z) = 0;
	/// \brief ���ָ��λ�õ���ߵ����ڲ�
	/// \param x,z λ��
	virtual int GetApexFloor(len_t x, len_t z) = 0;
	/// \brief ����Ƿ������
	/// \param x,z λ��
	virtual bool GetWalkEnable(len_t x, len_t z) = 0;
	/// \brief ��ÿ����ߵĸ߶�
	/// \param x,z λ��
	virtual len_t GetWalkHeight(len_t x, len_t z) = 0;
	/// \brief ˮ���Ƿ����
	/// \param x,z λ��
	virtual bool GetWalkWaterExists(len_t x, len_t z) = 0;
	/// \brief ��þ�ȷ��ˮ��߶�
	/// \param x,z λ��
	virtual len_t GetWalkWaterHeight(len_t x, len_t z) = 0;
	/// \brief ����ܵĲ�����
	/// \param x,z λ��
	virtual int GetFloorCount(len_t x, len_t z) = 0;
	/// \brief ��ò��Ƿ����
	/// \param x,z λ��
	/// \param floor ��������0��floor_count-1��
	virtual bool GetFloorExists(len_t x, len_t z, int floor) = 0;
	/// \brief ��ò��Ƿ���ƶ�
	/// \param x,z λ��
	/// \param floor ��������0��floor_count-1��
	virtual bool GetFloorCanMove(len_t x, len_t z, int floor) = 0;
	/// \brief ��ò��Ƿ��վ��
	/// \param x,z λ��
	/// \param floor ��������0��floor_count-1��
	virtual bool GetFloorCanStand(len_t x, len_t z, int floor) = 0;
	/// \brief ��ò�߶�
	/// \param x,z λ��
	/// \param floor ��������0��floor_count-1��
	virtual len_t GetFloorHeight(len_t x, len_t z, int floor) = 0;
	/// \brief ��ò�����ϰ��ռ�߶�
	/// \param x,z λ��
	/// \param floor ��������0��floor_count-1��
	virtual len_t GetFloorSpace(len_t x, len_t z, int floor) = 0;
	/// \brief ��ò��Ƿ���ǽ��
	/// \param x,z λ��
	/// \param floor ��������1��floor_count-1��
	virtual bool GetFloorHasWall(len_t x, len_t z, int floor) = 0;
	/// \brief ���ָ���߶�λ���Ƿ����ǽ��
	/// \param x,y,z λ��
	virtual bool GetWallExists(len_t x, len_t y, len_t z) = 0;

	// �������еĵ��б�
	/// \brief ���泡���еĵ�
	/// \param name ����
	/// \param x,y,z,orient λ�úͷ���
	virtual bool SavePoint(const char* name, len_t x, len_t y, len_t z, 
		len_t orient) = 0;
	/// \brief ���賡���еĵ�
	/// \param name ����
	/// \param x,y,z,orient λ�úͷ���
	virtual bool ResetPoint(const char* name, len_t x, len_t y, len_t z, 
		len_t orient) = 0;
	/// \brief ɾ�������еĵ�
	/// \param name ����
	virtual bool DeletePoint(const char* name) = 0;
	/// \brief ȡ�ó����еĵ�����
	/// \param name ����
	/// \param x,y,z,orient ���ص�λ�úͷ���
	virtual bool GetPointCoord(const char* name, len_t& x, len_t& y, 
		len_t& z, len_t& orient) = 0;
	/// \brief ��ó����е����е�����
	/// \param result ��������б�
	virtual int GetScenePointList(IVarList& result) = 0;

	// �������еĶ����б�
	/// \brief ���س����еĶ��������ļ�
	/// \param ini_name ���������ļ���
	virtual bool LoadObjects(const char* ini_name) = 0;
	/// \brief �������
	/// \param name ������
	/// \param script �߼�����
	/// \param config ������
	/// \param point �����еĵ���
	/// \param capacity ����
	virtual PERSISTID SaveObject(const char* name, const char* script, 
		const char* config, const char* point, int capacity) = 0;
	/// \brief �������
	/// \param name ������
	/// \param script �߼�����
	/// \param config ������
	/// \param point �����еĵ���
	/// \param capacity ����
	virtual PERSISTID ResetObject(const char* name, const char* script, 
		const char* config, const char* point, int capacity) = 0;
	/// \brief ɾ������
	/// \param name ������
	virtual bool DeleteObject(const char* name) = 0;
	
	/// \brief �����д�������
	/// \param script �߼����������Ϊ����������������ȡ"script"����ֵ
	/// \param config ������
	/// \param capacity ����
	/// \param x,y,z,orient λ�úͷ���
	virtual PERSISTID CreateObject(const char* script, const char* config, 
		int capacity, len_t x, len_t y, len_t z, len_t orient) = 0;
	/// \brief ����ʼ�����������д�������
	/// \param script �߼����������Ϊ����������������ȡ"script"����ֵ
	/// \param config ������
	/// \param capacity ����
	/// \param x,y,z,orient λ�úͷ���
	/// \param args ���������б�
	virtual PERSISTID CreateObjectArgs(const char* script, 
		const char* config, int capacity, len_t x, len_t y, len_t z, 
		len_t orient, const IVarList& args) = 0;

	/// \brief Ԥ���ض����������ݼ����ļ�
	/// \param name ���ݼ����ļ���
	virtual bool PreloadConfig(const char* name) = 0;
	/// \brief Ԥ����TXT��ʽ�Ķ����������ݼ����ļ�
	/// \param name ���ݼ����ļ���
	/// \param start_line ��Ч������ʼ�к�
	virtual bool PreloadConfigTxt(const char* name, int start_line = 3) = 0;
	/// \brief ����Ԥ���ص���������
	/// \param config ������
	/// \param prop ������
	virtual bool FindConfigProperty(const char* config, const char* prop) = 0;
	/// \brief ��ȡԤ���ص���������ֵ
	/// \param config ������
	/// \param prop ������
	virtual const char* GetConfigProperty(const char* config, 
		const char* prop) = 0;
	/// \brief ����Ԥ���ص���������ֵ
	/// \param config ������
	/// \param prop ������
	/// \param value ����ֵ
	virtual bool SetConfigProperty(const char* config, const char* prop, 
		const char* value) = 0;
	/// \brief ������õ��������б�
	/// \param config ������
	/// \param result ���ص��������б�
	virtual int GetConfigPropertyList(const char* config, IVarList& result) = 0;
	/// \brief ����Ԥ���ص����ñ��
	/// \param config ������
	/// \param rec �����
	virtual bool FindConfigRecord(const char* config, const char* rec) = 0;
	/// \brief ��ȡԤ���ص����ñ��ֵ
	/// \param config ������
	/// \param rec �����
	/// \param rows ��������ݣ��ַ�����
	virtual int GetConfigRecord(const char* config, const char* rec, 
		IVarList& result) = 0;
	/// \brief ����Ԥ���ص����ñ��ֵ
	/// \param config ������
	/// \param rec �����
	/// \param rows ��������ݣ��ַ�����
	virtual bool SetConfigRecord(const char* config, const char* rec, 
		const IVarList& rows) = 0;
	/// \brief ������õı�����б�
	/// \param config ������
	/// \param result ���صı�����б�
	virtual int GetConfigRecordList(const char* config, IVarList& result) = 0;
	/// \brief װ�ض������������
	/// \param obj ������
	/// \param config ������
	/// \param reload �Ƿ�Ҫ���¼����ļ�
	virtual bool LoadConfig(const PERSISTID& obj, const char* config, 
		bool reload = false) = 0;
	/// \brief �������д�������
	/// \param obj ��������
	/// \param script �߼����������Ϊ����������������ȡ"script"����ֵ
	/// \param config ������
	/// \param capacity ����
	virtual PERSISTID CreateFromConfig(const PERSISTID& obj, 
		const char* script, const char* config, int capacity = 0) = 0;
	/// \brief ����ʼ�������������д�������
	/// \param obj ��������
	/// \param script �߼����������Ϊ����������������ȡ"script"����ֵ
	/// \param config ������
	/// \param args ���������б�
	/// \param capacity ����
	virtual PERSISTID CreateFromConfigArgs(const PERSISTID& obj, 
		const char* script, const char* config, const IVarList& args, 
		int capacity = 0) = 0;

	/// \brief ����߼��������ֵ������-1��ʾδ�ҵ���
	/// \param class_name �߼�����
	virtual int GetClassIndex(const char* class_name) = 0;
	/// \brief ����߼������������ֵ������-1��ʾδ�ҵ���������ֵ�������Եĺ�����IGameObj�У�
	/// \param class_index �߼�������
	/// \param name ������
	virtual int GetClassAttrIndex(int class_index, const char* name) = 0;
	/// \brief ��������
	/// \param class_index �߼�������
	/// \param name ������
	/// \param type ��������
	/// \param saving �Ƿ�Ҫ����
	/// \param not_save_empty �Ƿ�Ϊ��ֵʱ�����浽���ݿ�
	/// \return ��������������ʧ�ܷ���-1
	virtual int Add(int class_index, const char* name, int type, 
		bool saving = false, bool not_save_empty = false) = 0;
	/// \brief ��ӿ�������
	/// \param class_index �߼�������
	/// \param name ������
	/// \param type ��������
	/// \param vis_type �������ͣ��������Ϳ�������"BYTE"��"WORD"����������Ϊ�գ�
	/// \param bPublic �������Ƿ�ɼ�
	/// \param bPrivate ˽�����Ƿ�ɼ�
	/// \param saving �Ƿ�Ҫ����
	/// \param not_save_empty �Ƿ�Ϊ��ֵʱ�����浽���ݿ�
	/// \return ��������������ʧ�ܷ���-1
	virtual int AddVisible(int class_index, const char* name, int type, 
		const char* vis_type, bool bPublic, bool bPrivate, 
		bool saving = false, bool not_save_empty = false) = 0;
	/// \brief �������Կ���
	/// \param class_index �߼�������
	/// \param name ������
	/// \param vis_type �������ͣ��������Ϳ�������"BYTE"��"WORD"����������Ϊ�գ�
	/// \param bPublic �������Ƿ�ɼ�
	/// \param bPrivate ˽�����Ƿ�ɼ�
	virtual bool SetVisible(int class_index, const char* name, 
		const char* vis_type, bool bPublic, bool bPrivate) = 0;
	/// \brief ���������Ƿ�ʱˢ��
	/// \param class_index �߼�������
	/// \param name ������
	/// \param realtime �Ƿ�Ҫ��ʱˢ��
	virtual bool SetRealtime(int class_index, const char* name, 
		bool realtime) = 0;
	/// \brief �����Ƿ񱣴�
	/// \param class_index �߼�������
	/// \param name ������
	/// \param saving �Ƿ�Ҫ����
	virtual bool SetSaving(int class_index, const char* name, bool saving) = 0;
	
	/// \brief ���һ����
	/// \param class_index �߼�������
	/// \param name �����
	/// \param cols ����
	/// \param max_rows �������
	/// \param saving �Ƿ񱣴�
	/// \param not_save_empty �������Ϊ��ʱ����Ҫ����
	virtual bool AddRecord(int class_index, const char* name, int cols, 
		int max_rows, bool saving = false, bool not_save_empty = false) = 0;
	/// \brief Ϊ������һ�����������������ͱ���Ϊ�ַ�������ַ�����
	/// \param class_index �߼�������
	/// \param name �����
	/// \param col �����к�
	/// \param case_insensitive �Ƿ��Сд������
	virtual bool SetRecordKey(int class_index, const char* name, int col,
		bool case_insensitive = false) = 0;
	/// \brief ���������ԣ�����������ţ���������
	/// \param class_index �߼�������
	/// \param name �����
	/// \param col �����
	/// \param type ��������
	virtual bool SetRecordColType(int class_index, const char* name, int col, 
		int type) = 0;
	/// \brief ���Ƿ���ʾ
	/// \param class_index �߼�������
	/// \param name �����
	/// \param bPublic �������Ƿ�ɼ�
	/// \param bPrivate ˽�����Ƿ�ɼ�
	virtual bool SetRecordVisible(int class_index, const char* name, 
		bool bPublic, bool bPrivate) = 0;
	/// \brief �����е���ʾ����
	/// \param class_index �߼�������
	/// \param name �����
	/// \param col �����
	/// \param vis_type �������ͣ�����Ϊ"HIDE"��ʾ���в��ɼ����������Ϳ�������"BYTE"��"WORD"����������Ϊ�գ�
	virtual bool SetRecordColVisType(int class_index, const char* name, 
		int col, const char* vis_type) = 0;
	/// \brief ���Ƿ񱣴�
	/// \param class_index �߼�������
	/// \param name �����
	/// \param saving �Ƿ񱣴�
	/// \param not_save_empty �������Ϊ��ʱ����Ҫ����
	virtual bool SetRecordSaving(int class_index, const char* name, 
		bool saving, bool not_save_empty = false) = 0;

	/// \brief ���ȫ�ֳ����л���ĳ����ź�������Ϣ
	/// \param location ȫ���л�����
	/// \param scene ���صĳ������
	/// \param x,y,z,orient ���ص�λ�úͷ���
	virtual bool GetSwitchLocation(const char* location, int& scene, 
		len_t& x, len_t& y, len_t& z, len_t& orient) = 0;
	/// \brief �л�������ָ���ĳ����ź�����
	/// \param obj ��Ҷ���
	/// \param scene �������
	/// \param x,y,z,orient λ�úͷ���
	virtual bool SwitchLocate(const PERSISTID& obj, int scene, len_t x, 
		len_t y, len_t z, len_t orient) = 0;
	/// \brief �л���ָ�������ĳ�����
	/// \param obj ��Ҷ���
	/// \param scene �������
	virtual bool SwitchBorn(const PERSISTID& obj, int scene) = 0;
	/// \brief ���ó����ĳ�����λ��
	/// \param x,y,z,orient λ�úͷ���
	virtual bool SetSceneBorn(len_t x, len_t y, len_t z, len_t orient) = 0;
	/// \brief ��ó����ĳ�����λ��
	/// \param x,y,z,orient ���ص�λ�úͷ���
	virtual bool GetSceneBorn(len_t& x, len_t& y, len_t& z, len_t& orient) = 0;

	/// \brief ������Ķ�ά�ռ���루x, z��
	/// \param obj ����һ
	/// \param other �����
	virtual len_t Distance2D(const PERSISTID& obj, const PERSISTID& other) = 0;
	/// \brief ���������ά�ռ���루x, y, z��
	/// \param obj ����һ
	/// \param other �����
	virtual len_t Distance3D(const PERSISTID& obj, const PERSISTID& other) = 0;
	/// \brief һ��������Ա�����ǰ����ĽǶ�
	/// \param obj ������
	/// \param other ��������
	virtual len_t Angle(const PERSISTID& obj, const PERSISTID& other) = 0;
	/// \brief һ������Ա�����ǰ����ĽǶ�
	/// \param obj ������
	/// \param x,z ��λ��
	virtual len_t DotAngle(const PERSISTID& obj, len_t x, len_t z) = 0;

	/// \brief �л�����������ָ��λ��
	/// \param ������
	/// \param x,y,z,orient λ�úͷ���
	virtual bool MoveTo(const PERSISTID& obj, len_t x, len_t y, len_t z, 
		len_t orient) = 0;
	/// \brief ��λ����������ָ��λ�ã�Ч����MoveTo���ƣ�ֻ�Ƿ��͵���Moving��Ϣ��
	/// \param ������
	/// \param x,y,z,orient λ�úͷ���
	virtual bool Locate(const PERSISTID& obj, len_t x, len_t y, len_t z, 
		len_t orient) = 0;
	/// \brief ��ת�Ƕ�
	/// \param obj ����
	/// \param angle ��ת�Ƕ�
	/// \param rotate_speed ת���ٶ�
	/// \param mode �����˶�ģʽ
	virtual bool Rotate(const PERSISTID& obj, len_t angle, 
		len_t rotate_speed, int mode = 0) = 0;
	/// \brief ���ƶ����ڵر��ƶ�
	/// \param obj ����
	/// \param move_speed �ƶ��ٶ�
	/// \param rotate_speed ת���ٶ�
	/// \param x,z Ŀ���λ��
	/// \param mode �����˶�ģʽ
	virtual bool Motion(const PERSISTID& obj, len_t move_speed, 
		len_t roate_speed, len_t x, len_t z, int mode = 0) = 0; 
	/// \brief ���ƶ����ڵر��ƶ���������ת
	/// \param obj ����
	/// \param move_speed �ƶ��ٶ�
	/// \param x,z Ŀ���λ��
	/// \param mode �����˶�ģʽ
	virtual bool MotionNoRotate(const PERSISTID& obj, len_t move_speed, 
		len_t x, len_t z, int mode = 0) = 0;
	/// \brief ���ƶ�����Ծ
	/// \param obj ����
	/// \param move_speed �ƶ��ٶ�
	/// \param rotate_speed ת���ٶ�
	/// \param jump_speed �����ٶ�
	/// \param gravity �������ٶ�
	/// \param x,z Ŀ���λ��
	/// \param mode �����˶�ģʽ
	virtual bool Jump(const PERSISTID& obj, len_t move_speed, 
		len_t rotate_speed, len_t jump_speed, len_t gravity, len_t x, len_t z,
		int mode = 0) = 0;
	/// \brief �ı������Ծ���ٶȺ�Ŀ�귽��
	/// \param obj ����
	/// \param move_speed �ƶ��ٶ�
	/// \param rotate_speed ת���ٶ�
	/// \param x,z Ŀ���λ��
	/// \param mode �����˶�ģʽ
	virtual bool JumpTo(const PERSISTID& obj, len_t move_speed, 
		len_t rotate_speed, len_t x, len_t z, int mode = 0) = 0;
	/// \brief ���ƶ����ڿ����ƶ�
	/// \param obj ����
	/// \param move_speed �ƶ��ٶ�
	/// \param rotate_speed ת���ٶ�
	/// \param x,y,z Ŀ���λ��
	/// \param mode �����˶�ģʽ
	virtual bool Fly(const PERSISTID& obj, len_t move_speed, 
		len_t rotate_speed, len_t x, len_t y, len_t z, int mode = 0) = 0;
	/// \brief ���ƶ�����ˮ���ƶ�
	/// \param ����
	/// \param move_speed �ƶ��ٶ�
	/// \param rotate_speed ת���ٶ�
	/// \param x,y,z Ŀ���λ��
	/// \param mode �����˶�ģʽ
	virtual bool Swim(const PERSISTID& obj, len_t move_speed, 
		len_t rotate_speed, len_t x, len_t y, len_t z, int mode = 0) = 0;
	/// \brief ���ƶ�����ˮ���ƶ�
	/// \param ����
	/// \param move_speed �ƶ��ٶ�
	/// \param rotate_speed ת���ٶ�
	/// \param x,z Ŀ���λ��
	/// \param mode �����˶�ģʽ
	virtual bool Drift(const PERSISTID& obj, len_t move_speed, 
		len_t rotate_speed, len_t x, len_t z, int mode = 0) = 0;
	/// \brief ���ƶ�����ǽ���ƶ�
	/// \param ����
	/// \param move_speed �ƶ��ٶ�
	/// \param x,y,z Ŀ���λ��
	/// \param orient ��������ǽ��ķ���
	/// \param mode �����˶�ģʽ
	virtual bool Climb(const PERSISTID& obj, len_t move_speed, len_t x, 
		len_t y, len_t z, len_t orient, int mode = 0) = 0;
	/// \brief ���ƶ�����
	/// \param obj ����
	/// \param move_speed �ƶ��ٶ�
	/// \param rotate_speed ת���ٶ�
	/// \param x,z Ŀ���λ��
	/// \param mode �����˶�ģʽ
	virtual bool Slide(const PERSISTID& obj, len_t move_speed, 
		len_t roate_speed, len_t x, len_t z, int mode = 0) = 0; 
	/// \brief ���ƶ����³�
	/// \param obj ����
	/// \param move_speed �ƶ��ٶ�
	/// \param rotate_speed ת���ٶ�
	/// \param sink_speed ��ʼ�³��ٶ�
	/// \param floatage �������ٶ�
	/// \param x,z Ŀ���λ��
	/// \param mode �����˶�ģʽ
	virtual bool Sink(const PERSISTID& obj, len_t move_speed, 
		len_t rotate_speed, len_t sink_speed, len_t floatage, len_t x, 
		len_t z, int mode = 0) = 0;
	/// \brief ֹͣ�˶����ƶ�����ת��
	/// \param obj ����
	/// \param mode �����˶�ģʽ
	virtual bool Stop(const PERSISTID& obj, int mode = 0) = 0;
	/// \brief ֹͣ�ƶ�
	/// \param obj ����
	virtual bool StopWalk(const PERSISTID& obj) = 0;
	/// \brief ֹͣ��ת
	/// \param obj ����
	virtual bool StopRotate(const PERSISTID& obj) = 0;

	/// \brief У��ˮƽ�����ƶ�
	/// \param obj ����
	/// \param move_speed �ƶ��ٶ�
	/// \param rotate_speed ת���ٶ�
	/// \param sx,sy,sz ��ʼλ��
	/// \param dx,dz Ŀ���λ��
	/// \param mode �����˶�ģʽ
	virtual bool CheckMotion(const PERSISTID& obj, len_t move_speed, 
		len_t rotate_speed, len_t sx, len_t sy, len_t sz, len_t dx, len_t dz,
		int mode = 0) = 0;
	/// \brief У����Ծ
	/// \param obj ����
	/// \param move_speed �ƶ��ٶ�
	/// \param rotate_speed ת���ٶ�
	/// \param jump_speed �����ٶ�
	/// \param gravity �������ٶ�
	/// \param sx,sy,sz ��ʼλ��
	/// \param dx,dz Ŀ���λ��
	/// \param mode �����˶�ģʽ
	virtual bool CheckJump(const PERSISTID& obj, len_t move_speed, 
		len_t rotate_speed, len_t jump_speed, len_t gravity, len_t sx, 
		len_t sy, len_t sz, len_t dx, len_t dz, int mode = 0) = 0;
	/// \brief У����Ծ�иı�Ŀ�귽��
	/// \param obj ����
	/// \param move_speed �ƶ��ٶ�
	/// \param rotate_speed ת���ٶ�
	/// \param sx,sy,sz ��ʼλ��
	/// \param dx,dz Ŀ���λ��
	/// \param mode �����˶�ģʽ
	virtual bool CheckJumpTo(const PERSISTID& obj, len_t move_speed, 
		len_t rotate_speed, len_t sx, len_t sy, len_t sz, len_t dx, len_t dz,
		int mode = 0) = 0;
	/// \brief У������˶�
	/// \param obj ����
	/// \param move_speed �ƶ��ٶ�
	/// \param rotate_speed ת���ٶ�
	/// \param sx,sy,sz ��ʼλ��
	/// \param dx,dy,dz Ŀ���λ��
	/// \param mode �����˶�ģʽ
	virtual bool CheckFly(const PERSISTID& obj, len_t move_speed, 
		len_t rotate_speed, len_t sx, len_t sy, len_t sz, len_t dx, len_t dy, 
		len_t dz, int mode = 0) = 0;
	/// \brief У��ˮ���˶�
	/// \param obj ����
	/// \param move_speed �ƶ��ٶ�
	/// \param rotate_speed ת���ٶ�
	/// \param sx,sy,sz ��ʼλ��
	/// \param dx,dy,dz Ŀ���λ��
	/// \param mode �����˶�ģʽ
	virtual bool CheckSwim(const PERSISTID& obj, len_t move_speed, 
		len_t rotate_speed, len_t sx, len_t sy, len_t sz, len_t dx, len_t dy, 
		len_t dz, int mode = 0) = 0;
	/// \brief У��ˮ���˶�
	/// \param obj ����
	/// \param move_speed �ƶ��ٶ�
	/// \param rotate_speed ת���ٶ�
	/// \param sx,sy,sz ��ʼλ��
	/// \param dx,dz Ŀ���λ��
	/// \param mode �����˶�ģʽ
	virtual bool CheckDrift(const PERSISTID& obj, len_t move_speed, 
		len_t rotate_speed, len_t sx, len_t sy, len_t sz, len_t dx, len_t dz, 
		int mode = 0) = 0;
	/// \brief У����ǽ
	/// \param obj ����
	/// \param move_speed �ƶ��ٶ�
	/// \param rotate_speed ת���ٶ�
	/// \param sx,sy,sz ��ʼλ��
	/// \param dx,dy,dz Ŀ���λ��
	/// \param orient ��������ǽ��ķ���
	/// \param mode �����˶�ģʽ
	virtual bool CheckClimb(const PERSISTID& obj, len_t move_speed, 
		len_t rotate_speed, len_t sx, len_t sy, len_t sz, len_t dx, len_t dy, 
		len_t dz, len_t orient, int mode = 0) = 0;
	/// \brief У�黬��
	/// \param obj ����
	/// \param move_speed �ƶ��ٶ�
	/// \param rotate_speed ת���ٶ�
	/// \param sx,sy,sz ��ʼλ��
	/// \param dx,dz Ŀ���λ��
	/// \param mode �����˶�ģʽ
	virtual bool CheckSlide(const PERSISTID& obj, len_t move_speed, 
		len_t rotate_speed, len_t sx, len_t sy, len_t sz, len_t dx, len_t dz, 
		int mode = 0) = 0;
	/// \brief У���³�
	/// \param obj ����
	/// \param move_speed �ƶ��ٶ�
	/// \param rotate_speed ת���ٶ�
	/// \param sink_speed ��ʼ�³��ٶ�
	/// \param floatage �������ٶ�
	/// \param sx,sy,sz ��ʼλ��
	/// \param dx,dz Ŀ���λ��
	/// \param mode �����˶�ģʽ
	virtual bool CheckSink(const PERSISTID& obj, len_t move_speed, 
		len_t rotate_speed, len_t sink_speed, len_t floatage, len_t sx, 
		len_t sy, len_t sz, len_t dx, len_t dz, int mode = 0) = 0;
	/// \brief У��ֹͣ
	/// \param obj ����
	/// \param move_speed �ƶ��ٶ�
	/// \param x,y,z,orient ֹͣʱ��Ŀ��ͷ���
	/// \param mode �����˶�ģʽ
	virtual bool CheckStop(const PERSISTID& obj, len_t move_speed, len_t x, 
		len_t y, len_t z, len_t orient, int mode = 0) = 0;

	/// \brief ���ӵ���������
	/// \param obj ������
	/// \param link �����ӵĶ���
	/// \param x,y,z,orient ���ӵ����λ�úͷ���
	virtual bool LinkTo(const PERSISTID& obj, const PERSISTID& link, 
		len_t x, len_t y, len_t z, len_t orient) = 0;
	/// \brief �ı����ӵ����������λ��
	/// \param obj ������
	/// \param link �����ӵĶ���
	/// \param x,y,z,orient ���ӵ����λ�úͷ���
	virtual bool LinkMove(const PERSISTID& obj, const PERSISTID& link, 
		len_t x, len_t y, len_t z, len_t orient) = 0;
	/// \brief ȡ�����ӵ���������
	/// \param obj ������
	virtual bool Unlink(const PERSISTID& obj) = 0;
	/// \brief ��õ�ǰ���ӵ���������
	/// \param obj ������
	virtual PERSISTID GetLinkObject(const PERSISTID& obj) = 0;
	/// \brief ��õ�ǰ���ӵ����������λ��
	/// \param obj ������
	/// \param x,y,z,orient λ�úͳ���
	virtual bool GetLinkPosition(const PERSISTID& obj, len_t& x, len_t& y,
		len_t& z, len_t& orient) = 0;

	/// \brief ��Ӷ������Ұ���ȼ�,�ò��Խ��ڵ�ǰ������Ч��
	/// \param obj ������
	/// \param prior ���ȿ����Ķ���
	/// \param force ǿ�����(�絽�������޾Ͱ����Ƚ��ȳ�ԭ������Ƴ����)
	virtual bool AddVisualPriority(const PERSISTID& obj, const PERSISTID& prior, bool force = false) = 0;
	/// \brief �Ƴ��������Ұ���ȼ�
	/// \param obj ������
	/// \param prior ���ȿ����Ķ���
	virtual bool RemoveVisualPriority(const PERSISTID& obj, const PERSISTID& prior) = 0;
	/// \brief ��ȡ�������Ұ���ȼ��Ķ����б�
	/// \param obj ������
	/// \param result ��Ұ���ȼ��б�
	virtual int GetVisualPriority(const PERSISTID& obj, IVarList& result) = 0;

	/// \brief ��������X
	/// \param obj ����
	virtual len_t GetPosiX(const PERSISTID& obj) = 0;
	/// \brief ��������Y
	/// \param obj ����
	virtual len_t GetPosiY(const PERSISTID& obj) = 0;
	/// \brief ��������Z
	/// \param obj ����
	virtual len_t GetPosiZ(const PERSISTID& obj) = 0;
	/// \brief ������
	/// \param obj ����
	virtual len_t GetOrient(const PERSISTID& obj) = 0;
	/// \brief ȡ��������ͷ���
	/// \param obj ����
	/// \param x,y,z,orient Ŀ��ͷ��򷵻�ֵ
	virtual bool GetLocation(const PERSISTID& obj, len_t& x, len_t& y, 
		len_t& z, len_t& orient) = 0;
	/// \brief ȡ����ǰ���ڲ㣨0-15��ͨ�㣬200ˮ�棬300���У�400ˮ�£�
	/// \param obj ����
	virtual int GetFloor(const PERSISTID& obj) = 0;

	/// \brief Ŀ���X
	/// \param obj ����
	virtual len_t GetDestX(const PERSISTID& obj) = 0;
	/// \brief Ŀ���Y
	/// \param obj ����
	virtual len_t GetDestY(const PERSISTID& obj) = 0;
	/// \brief Ŀ���Z
	/// \param obj ����
	virtual len_t GetDestZ(const PERSISTID& obj) = 0;
	/// \brief Ŀ�귽��
	/// \param obj ����
	virtual len_t GetDestOrient(const PERSISTID& obj) = 0;
	/// \brief �ƶ�ģʽ
	/// \param obj ����
	virtual int GetMoveMode(const PERSISTID& obj) = 0;
	/// \brief �ƶ��ٶ�
	/// \param obj ����
	virtual len_t GetMoveSpeed(const PERSISTID& obj) = 0;
	/// \brief ��ת�ٶ�
	/// \param obj ����
	virtual len_t GetRotateSpeed(const PERSISTID& obj) = 0;
	/// \brief �����ٶ�
	/// \param obj ����
	virtual len_t GetJumpSpeed(const PERSISTID& obj) = 0;
	/// \brief �������ٶ�
	/// \param obj ����
	virtual len_t GetGravity(const PERSISTID& obj) = 0;

	/// \brief ����һ���Ӷ���
	/// \param obj ����������
	/// \param script �߼�����
	virtual PERSISTID Create(const PERSISTID& obj, const char* script) = 0;
	/// \brief ����ʼ����������һ���Ӷ���
	/// \param obj ����������
	/// \param script �߼�����
	/// \param args ��������
	virtual PERSISTID CreateArgs(const PERSISTID& obj, const char* script, 
		const IVarList& args) = 0;
	/// \brief ����һ������
	/// \param obj ����������
	/// \param script �߼�����
	/// \param capacity ����
	virtual PERSISTID CreateContainer(const PERSISTID& obj, 
		const char* script, int capacity) = 0;
	/// \brief ����ʼ����������һ������
	/// \param obj ����������
	/// \param script �߼�����
	/// \param capacity ����
	/// \param args ��������
	virtual PERSISTID CreateContainerArgs(const PERSISTID& obj, 
		const char* script, int capacity, const IVarList& args) = 0;
	/// \brief ��������������
	/// \param obj ��������
	/// \param capacity Ŀ������
	virtual bool ExpandContainer(const PERSISTID& obj, int capacity) = 0;
	/// \brief ��С����������
	/// \param obj ��������
	/// \param capacity Ŀ������
	virtual bool ShrinkContainer(const PERSISTID& obj, int capacity) = 0;
	/// \brief ���ƶ���(Ŀ������������ԭ��)
	/// \param container Ŀ����������
	/// \param prototype ԭ�Ͷ���
	/// \param pos ������Ŀ�������е�λ��
	virtual PERSISTID CreateClone(const PERSISTID& container, 
		const PERSISTID& prototype, int pos) = 0;
	/// \brief ��ָ���ص㴴���Ӷ���
	/// \param obj ��������
	/// \param script �߼�����
	/// \param x,y,z,orient λ�úͷ���
	/// \param capacity ����
	virtual PERSISTID CreateTo(const PERSISTID& obj, const char* script, 
		len_t x, len_t y, len_t z, len_t orient, int capacity = 0) = 0;
	/// \brief ����ʼ��������ָ���ص㴴���Ӷ���
	/// \param obj ��������
	/// \param script �߼�����
	/// \param x,y,z,orient λ�úͷ���
	/// \param args ��������
	/// \param capacity ����
	virtual PERSISTID CreateToArgs(const PERSISTID& obj, const char* script, 
		len_t x, len_t y, len_t z, len_t orient, const IVarList& args, 
		int capacity = 0) = 0;
	/// \brief ����һ���Ӷ���(�������Ӷ���)
	/// \param obj ������
	/// \param target �����ٵ��Ӷ���
	virtual bool Destroy(const PERSISTID& obj, const PERSISTID& target) = 0;
	/// \brief ���ٶ�������
	/// \param obj �����ٵĶ���
	virtual bool DestroySelf(const PERSISTID& obj) = 0;
	/// \brief ѡ����󴥷��������OnDoSelect�ص���Ŀ������OnSelect�ص�
	/// \param self ������
	/// \param obj Ŀ�����
	/// \param funcid ���ܱ��
	virtual int Select(const PERSISTID& self, const PERSISTID& obj, 
		int funcid) = 0;
	
	/// \brief ���ö����Ƿ񲻱��浽���ݿ�
	/// \param obj ����
	/// \param value �Ƿ񲻱���
	virtual bool SetUnsave(const PERSISTID& obj, bool value) = 0;
	/// \brief ��ö����Ƿ񲻱��浽���ݿ�
	/// \param obj ����
	virtual bool GetUnsave(const PERSISTID& obj) = 0;

	/// \brief �������ƶ���һ��������
	/// \param obj ���ƶ��Ķ���
	/// \param container ��������
	virtual bool Place(const PERSISTID& obj, const PERSISTID& container) = 0;
	/// \brief ������ŵ�������ָ��λ��
	/// \param obj ���ƶ��Ķ���
	/// \param container ��������
	/// \param pos �����е�λ�ã���1��ʼ��
	virtual bool PlacePos(const PERSISTID& obj, const PERSISTID& container, 
		int pos) = 0;
	/// \brief ������ŵ�������ָ��λ��
	/// \param obj ���ƶ��Ķ���
	/// \param container ������������
	/// \param x,y,z,orient λ�úͷ���
	virtual bool PlaceTo(const PERSISTID& obj, const PERSISTID& container, 
		len_t x, len_t y, len_t z, len_t orient) = 0;
	/// \brief ����֮�佻��ָ��λ���ϵ��Ӷ���
	/// \param container1 ����һ
	/// \param pos1 ����һ��λ��
	/// \param container2 ������
	/// \param pos2 ��������λ��
	virtual bool Exchange(const PERSISTID& container1, int pos1, 
		const PERSISTID& container2, int pos2) = 0;
	/// \brief �ƶ�����������ָ��λ��
	/// \param obj ���ƶ��Ķ���
	/// \param new_pos �µ�����λ�ã���1��ʼ��
	virtual bool ChangePos(const PERSISTID& obj, int new_pos) = 0;

	/// \brief ��������������
	/// \param container ��������
	virtual int GetCapacity(const PERSISTID& container) = 0;
	/// \brief ȡ����ű���
	/// \param obj ����
	virtual const char* GetScript(const PERSISTID& obj) = 0;
	/// \brief ȡ����������
	/// \param obj ����
	virtual const char* GetConfig(const PERSISTID& obj) = 0;
	/// \brief ȡ�����������е�λ��
	/// \param obj ����
	/// \return ��1��ʼ
	virtual int GetIndex(const PERSISTID& obj) = 0;

	/// \brief ��ö�������
	/// \param obj ����
	/// \return �������ͣ���KnlConst.h��
	virtual int Type(const PERSISTID& obj) = 0;
	/// \brief �����Ƿ����
	/// \param obj ����
	virtual bool Exists(const PERSISTID& obj) = 0;
	/// \brief ȡ������
	/// \param obj ����
	virtual PERSISTID Parent(const PERSISTID& obj) = 0;
	/// \brief ��ñ��������Ĵ���
	/// \param obj ����
	virtual int GetWeakRefs(const PERSISTID& obj) = 0;
	/// \brief ��ñ��������������б�
	/// \param obj ����
	/// \param result �����б�
	virtual int GetWeakBoxList(const PERSISTID& obj, IVarList& result) = 0;

	/// \brief ȡ����ָ��λ�õ��Ӷ���
	/// \param obj ��������
	/// \param pos �����е�λ�ã���1��ʼ��
	virtual PERSISTID GetItem(const PERSISTID& obj, int pos) = 0;
	/// \brief �����ֲ����Ӷ���
	/// \param obj ��������
	/// \param name �Ӷ��������
	virtual PERSISTID GetChild(const PERSISTID& obj, const wchar_t* name) = 0;
	/// \brief ��õ�һ���Ӷ���
	/// \param obj ��������
	/// \param it ����������
	virtual PERSISTID GetFirst(const PERSISTID& obj, unsigned int& it) = 0;
	/// \brief �����һ���Ӷ���
	/// \param obj ��������
	/// \param it ����������
	virtual PERSISTID GetNext(const PERSISTID& obj, unsigned int& it) = 0;
	/// \brief �����Ӷ�������
	/// \param obj ��������
	virtual int GetChildCount(const PERSISTID& obj) = 0;
	/// \brief ����Ӷ����б�
	/// \param obj ��������
	/// \param classtype �������ͣ�Ϊ0��ʾ�������ͣ�
	/// \param result ��������б�
	virtual int GetChildList(const PERSISTID& obj, int classtype, 
		IVarList& result) = 0;
	/// \brief ��������Ӷ���
	/// \param obj ��������
	virtual bool ClearChild(const PERSISTID& obj) = 0;
	/// \brief ����ָ�����ƺ����͵��Ӷ���
	/// \param obj ��������
	/// \param name �Ӷ��������
	/// \param classtype �������ͣ�Ϊ0��ʾ�������ͣ�
	virtual PERSISTID FindChild(const PERSISTID& obj, const wchar_t* name, 
		int classtype) = 0;
	/// \brief ���Ҷ��ָ�����ƺ����͵��Ӷ��󣬷�������
	/// \param obj ��������
	/// \param name �Ӷ��������
	/// \param classtype �������ͣ�Ϊ0��ʾ�������ͣ�
	/// \param result ��������б�
	virtual int FindChildMore(const PERSISTID& obj, const wchar_t* name, 
		int classtype, IVarList& result) = 0;
	/// \brief ʹ������������Ӷ���
	/// \param obj ��������
	/// \param config ������
	/// \param classtype �������ͣ�Ϊ0��ʾ�������ͣ�
	virtual PERSISTID FindChildByConfig(const PERSISTID& obj, 
		const char* config, int classtype) = 0;
	/// \brief ʹ����������ö���Ӷ���
	/// \param obj ��������
	/// \param config ������
	/// \param classtype �������ͣ�Ϊ0��ʾ�������ͣ�
	/// \param result ��������б�
	virtual int FindChildMoreByConfig(const PERSISTID& obj, 
		const char* config, int classtype, IVarList& result) = 0;

	/// \brief ����������������ö���
	/// \param obj ���ö���
	/// \param container ����������
	/// \param pos λ�ã���1��ʼ��
	virtual bool AddWeakChild(const PERSISTID& obj, 
		const PERSISTID& container, int pos) = 0;
	/// \brief �����������Ƴ����ö���
	/// \param obj ���ö���
	/// \param container ����������
	virtual bool RemoveWeakChild(const PERSISTID& obj, 
		const PERSISTID& container) = 0;
	/// \brief ����������������ö���
	/// \param container ����������
	virtual bool ClearWeakChild(const PERSISTID& container) = 0;
	
	/// \brief ���ָ��������Χ�Ķ����б�
	/// \param obj ���Ķ���
	/// \param radius �뾶
	/// \param classtype �������ͣ�Ϊ0��ʾ�������ͣ�
	/// \param max ���������Ϊ0��ʾ������������
	/// \param result ��������б�
	/// \param invisible �Ƿ�������ɼ�����
	virtual int GetAroundList(const PERSISTID& obj, len_t radius, 
		int classtype, int max, IVarList& result, bool invisible = false) = 0;
	/// \brief ��õ���Χ�Ķ����б�
	/// \param obj �����ڷǷ��鳡������Ϊ�գ�
	/// \param x,z λ��
	/// \param radius �뾶
	/// \param classtype �������ͣ�Ϊ0��ʾ�������ͣ�
	/// \param max ���������Ϊ0��ʾ������������
	/// \param result ��������б�
	/// \param invisible �Ƿ�������ɼ�����
	virtual int GetPointAroundList(const PERSISTID& obj, len_t x, len_t z, 
		len_t radius, int classtype, int max, IVarList& result, 
		bool invisible = false) = 0;

	/// \brief ���ָ��������Χ3D��Χ�ڵĶ����б�
	/// \param obj ���Ķ���
	/// \param radius �뾶
	/// \param classtype �������ͣ�Ϊ0��ʾ�������ͣ�
	/// \param max ���������Ϊ0��ʾ������������
	/// \param result ��������б�
	/// \param invisible �Ƿ�������ɼ�����
	virtual int GetAroundList3D(const PERSISTID& obj, len_t radius, 
		int classtype, int max, IVarList& result, bool invisible = false) = 0;
	/// \brief ��õ���Χ3D��Χ�ڵĶ����б�
	/// \param obj �����ڷǷ��鳡������Ϊ�գ�
	/// \param x,y,z λ��
	/// \param radius �뾶
	/// \param classtype �������ͣ�Ϊ0��ʾ�������ͣ�
	/// \param max ���������Ϊ0��ʾ������������
	/// \param result ��������б�
	/// \param invisible �Ƿ�������ɼ�����
	virtual int GetPointAroundList3D(const PERSISTID& obj, len_t x, len_t y,
		len_t z, len_t radius, int classtype, int max, IVarList& result, 
		bool invisible = false) = 0;

	/// \brief �����ָ���߶���ײ�Ķ����б�
	/// \param obj �����ڷǷ��鳡������Ϊ�գ�
	/// \param beg_x,beg_y,beg_z �߶���ʼ��
	/// \param end_x,end_y,end_z �߶ν�����
	/// \param line_radius �߶ΰ뾶
	/// \param classtype �������ͣ�Ϊ0��ʾ�������ͣ�
	/// \param max ���������Ϊ0��ʾ������������
	/// \param result ��������б�
	/// \param invisible �Ƿ�������ɼ�����
	virtual int TraceObjectList(const PERSISTID& obj, len_t beg_x, 
		len_t beg_y, len_t beg_z, len_t end_x, len_t end_y, len_t end_z, 
		len_t line_radius, int classtype, int max, IVarList& result, 
		bool invisible = false) = 0;
	
	/// \brief ���Ψһ�����
	virtual int NewGroupId() = 0;
	/// \brief ���ָ����ĵ�һ���Ӷ���
	/// \param group ���
	/// \param it ����������
	virtual PERSISTID GroupFirst(int group, unsigned int& it) = 0;
	/// \brief ���ָ�������һ���Ӷ���
	/// \param group ���
	/// \param it ����������
	virtual PERSISTID GroupNext(int group, unsigned int& it) = 0;
	/// \brief ���ָ������Ӷ����б�
	/// \param group ���
	/// \param classtype �������ͣ�Ϊ0��ʾ�������ͣ�
	/// \param result ��������б�
	virtual int GroupChildList(int group, int classtype, IVarList& result) = 0;
	/// \brief ��������ָ�����ֺ����͵Ķ���
	/// \param group ���
	/// \param name �Ӷ��������
	/// \param classtype �������ͣ�Ϊ0��ʾ�������ͣ�
	virtual PERSISTID GroupFind(int group, const wchar_t* name, 
		int classtype) = 0;
	/// \brief �������ڶ��ָ�����ֺ����͵Ķ���
	/// \param group ���
	/// \param name �Ӷ��������
	/// \param classtype �������ͣ�Ϊ0��ʾ�������ͣ�
	/// \param result ��������б�
	virtual int GroupFindMore(int group, const wchar_t* name, int classtype, 
		IVarList& result) = 0;
	/// \brief ������еķ���Ҷ���
	/// \param group ���
	virtual bool ClearGroup(int group) = 0;
	
	/// \brief �����������
	/// \param obj ����
	/// \param func ����������
	/// \param time ����ʱ��
	/// ע�����ӵĺ��������ǹ̶��ģ�����
	/// int func(const PERSISTID& obj, int time) 
	/// time�ĵ�λΪ����
	// ����ɾ��ʱ�������ϵĸ��ֻص��ᱻ�Զ�ɾ��(��������)
	virtual bool AddHeartBeat(const PERSISTID& obj, const char* func, 
		int time) = 0;
	/// \brief ��Ӽ�������
	/// \param obj ����
	/// \param func ����������
	/// \param time ����ʱ��
	/// \param count ��������
	virtual bool AddCountBeat(const PERSISTID& obj, const char* func, 
		int time, int count) = 0;
	/// \brief ɾ����������
	/// \param obj ����
	/// \param func ����������
	virtual bool RemoveHeartBeat(const PERSISTID& obj, const char* func) = 0;
	/// \brief �����������
	/// \param obj ����
	virtual bool ClearHeartBeat(const PERSISTID& obj) = 0;
	/// \brief ������������
	/// \param obj ����
	/// \param func ����������
	virtual bool FindHeartBeat(const PERSISTID& obj, const char* func) = 0;
	/// \brief �������ʱ��
	/// \param obj ����
	/// \param func ����������
	virtual int GetBeatTime(const PERSISTID& obj, const char* func) = 0;
	/// \brief ������������
	/// \param obj ����
	/// \param func ����������
	/// \param count ��������
	virtual bool SetBeatCount(const PERSISTID& obj, const char* func, 
		int count) = 0;
	/// \brief �����������
	/// \param obj ����
	/// \param func ����������
	virtual int GetBeatCount(const PERSISTID& obj, const char* func) = 0;
	/// \brief ���ҹؼ����Իص��Ƿ����
	/// \param obj ����
	/// \param property ������
	/// \param func �ص�������
	virtual bool FindCritical(const PERSISTID& obj, const char* property, 
		const char* func) = 0;
	/// \brief ��ӹؼ����Իص���
	/// \param obj ����
	/// \param property ������
	/// \param func �ص�������
	/// ע�����ӵĺ��������ǹ̶��ģ�����
	/// int func(IKernel* pKernel, const PERSISTID& obj, const char* property, const IVar& old_value)
	/// old_valueΪ���Ըı�ǰ��ֵ
	virtual bool AddCritical(const PERSISTID& obj, const char* property, 
		const char* func) = 0;
	/// \brief ɾ���ؼ����Իص�
	/// \param obj ����
	/// \param property ������
	virtual bool RemoveCritical(const PERSISTID& obj, const char* property) = 0;
	/// \brief ɾ��ָ���������Ĺؼ����Իص�
	/// \param obj ����
	/// \param property ������
	/// \param func �ص�������
	virtual bool RemoveCriticalFunc(const PERSISTID& obj, 
		const char* property, const char* func) = 0;
	
	/// \brief ���ұ��ӻص��Ƿ����
	/// \param obj ����
	/// \param record �������
	/// \param func �ص�������
	virtual bool FindRecHook(const PERSISTID& obj, const char* record, 
		const char* func) = 0;
	/// \brief ��ӱ��ӻص���
	/// \param obj ����
	/// \param record �������
	/// \param func �ص�������
	/// ע�����ӵĺ��������ǹ̶��ģ�����
	/// int func(IKernel* pKernel, const PERSISTID& obj, const char* record, 
	///		int op_type, int row, int col)
	/// op_typeΪ��������ͣ�row,col������ֵ
	virtual bool AddRecHook(const PERSISTID& obj, const char* record, 
		const char* func) = 0;
	/// \brief ɾ�����ӻص�
	/// \param obj ����
	/// \param record �������
	virtual bool RemoveRecHook(const PERSISTID& obj, const char* record) = 0;
	/// \brief ɾ��ָ���������ı��ӻص�
	/// \param obj ����
	/// \param record �������
	/// \param func �ص�������
	virtual bool RemoveRecHookFunc(const PERSISTID& obj, const char* record,
		const char* func) = 0;

	/// \brief �������������Ӵ�
	/// \param player ��Ҷ���
	/// \param id �������
	/// \param container ��������
	virtual bool AddViewport(const PERSISTID& player, int id, 
		const PERSISTID& container) = 0;
	/// \brief ���ɾ�������Ӵ�
	/// \param player ��Ҷ���
	/// \param id �������
	virtual bool RemoveViewport(const PERSISTID& player, int id) = 0;
	/// \brief ��ҵ��Ӵ��Ƿ����
	/// \param player ��Ҷ���
	/// \param id �������
	virtual bool FindViewport(const PERSISTID& player, int id) = 0;
	/// \brief �������Ӵ���Ӧ������
	/// \param player ��Ҷ���
	/// \param id �������
	virtual PERSISTID GetViewportContainer(const PERSISTID& player, int id) = 0;
	/// \brief �����������Ӵ�
	/// \param player ��Ҷ���
	virtual bool ClearViewport(const PERSISTID& player) = 0;
	/// \brief ȡ�������Ӵ���
	/// \param container ��������
	virtual int GetViewers(const PERSISTID& container) = 0;
	/// \brief �ر������������Ӵ�
	/// \param container ��������
	virtual bool CloseViewers(const PERSISTID& container) = 0;
	
	/// \brief ��ͨ����
	/// \param obj ��һ����������ڶ���
	/// \param info ������Ϣ
	virtual void Speech(const PERSISTID& obj, const wchar_t* info) = 0;
	
	/// \brief ���õ�½����
	/// \param obj ��Ҷ���
	/// \param point ȫ�ֵ�����Ϊ�ձ�ʾ�����½�㣩
	virtual bool SetLandPoint(const PERSISTID& obj, const char* point) = 0;
	/// \brief ���õ�½λ��
	/// \param obj ��Ҷ���
	/// \param scene �������
	/// \param x,y,z,orient λ�úͷ���
	virtual bool SetLandPosi(const PERSISTID& obj, int scene, len_t x, 
		len_t y, len_t z, len_t orient) = 0;
	

	/// \brief �ڵ�ǰ�����ڲ������
	/// \param name �������
	virtual PERSISTID FindPlayer(const wchar_t* name) = 0;
	/// \brief ȡ������ڳ�����
	/// \param name �������
	virtual int GetPlayerScene(const wchar_t* name) = 0;
	/// \brief ȡ��Ϸ���������
	virtual int GetPlayerCount() = 0;


	/// \brief ��ϵͳ��Ϣ����ҿͻ���
	/// \param type ��Ϣ����
	/// \param info ��Ϣ����
	virtual bool SysInfo(const PERSISTID& player, int type, 
		const wchar_t* info) = 0;
	/// \brief ��ϵͳ��Ϣ��ָ�����ֵ���ҿͻ���
	/// \param name �������
	/// \param type ��Ϣ����
	/// \param info ��Ϣ����
	virtual bool SysInfoByName(const wchar_t* name, int type, 
		const wchar_t* info) = 0;
	/// \brief ��ϵͳ��Ϣ��ָ��������Ұ��Χ�ڵ���ҿͻ���
	/// \param obj �ο�����
	/// \param type ��Ϣ����
	/// \param info ��Ϣ����
	virtual bool SysInfoByKen(const PERSISTID& obj, int type, 
		const wchar_t* info) = 0;

	/// \brief ���Զ�����Ϣ���ͻ���
	/// \param player ��Ҷ���
	/// \param msg ��Ϣ�����б�
#ifdef _DEBUG
	virtual bool Custom(const PERSISTID& player, const IVarList& msg) = 0;
#else
	virtual bool Custom(const PERSISTID& player, const IVarList& msg, CUSTOM_MERGE_FLAG_ENUM merge_flag = CMF_NOT_MERGE) = 0;
#endif
	/// \brief ���Զ�����Ϣ���ͻ���
	/// \param player ��Ҷ���
	/// \param msg1 ��Ϣ�����б�1
	/// \param msg2 ��Ϣ�����б�2
	virtual bool Custom2(const PERSISTID& player, const IVarList& msg1, 
		const IVarList& msg2) = 0;
	/// \brief ���Զ�����Ϣ��ָ��������ҵĿͻ���
	/// \param name �������
	/// \param msg ��Ϣ�����б�
	virtual bool CustomByName(const wchar_t* name, const IVarList& msg) = 0;
	/// \brief ���Զ�����Ϣ��ָ��������ҵĿͻ���
	/// \param name �������
	/// \param msg1 ��Ϣ�����б�1
	/// \param msg2 ��Ϣ�����б�2
	virtual bool CustomByName2(const wchar_t* name, const IVarList& msg1, 
		const IVarList& msg2) = 0;
	/// \brief ���Զ�����Ϣ��ָ��������Ұ����ҵĿͻ���
	/// \param obj �ο�����
	/// \param msg ��Ϣ�����б�
#ifdef _DEBUG
	virtual bool CustomByKen(const PERSISTID& obj, const IVarList& msg) = 0;
#else
	virtual bool CustomByKen(const PERSISTID& obj, const IVarList& msg, CUSTOM_MERGE_FLAG_ENUM merge_flag = CMF_NOT_MERGE) = 0;
#endif
	/// \brief ���Զ�����Ϣ����ǰ������������ҵĿͻ���
	/// \param msg ��Ϣ�����б�
	virtual bool CustomByRoom(const IVarList& msg) = 0;
	/// \brief ���Զ�����Ϣ��ָ������������ҵĿͻ���
	/// \param group ���
	/// \param msg ��Ϣ�����б�
	virtual bool CustomByGroup(int group, const IVarList& msg) = 0;

	/// \brief �����������
	/// \param obj ��������Ķ���
	/// \param target ��������Ķ���
	/// \param msg ��������б�
	virtual int Command(const PERSISTID& obj, const PERSISTID& target, 
		const IVarList& msg) = 0;
	/// \brief �������ָ�����ֵ���Ҷ���
	/// \param name �������
	/// \param msg ��������б�
	virtual int CommandByName(const wchar_t* name, const IVarList& msg) = 0;
	/// \brief �������ָ��������Ұ�ڵ���Ҷ���
	/// \param obj �ο�����
	/// \param msg ��������б�
	virtual int CommandByKen(const PERSISTID& obj, const IVarList& msg) = 0;
	/// \brief �������ָ�������ڵ���Ҷ���
	/// \param scene �������
	/// \param msg ��������б�
	virtual int CommandByScene(int scene, const IVarList& msg) = 0;
	/// \brief �������ָ�����ڵ���Ҷ���
	/// \param group ���
	/// \param msg ��������б�
	virtual int CommandByGroup(int group, const IVarList& msg) = 0;
	/// \brief �������ָ��������ĳ�������Ҷ���
	/// \param scene �������
	/// \param group ���
	/// \param msg ��������б�
	virtual int CommandBySceneGroup(int scene, int group, 
		const IVarList& msg) = 0;

	/// \brief �������ָ����������
	/// \param scene �������
	/// \param msg ��������б�
	virtual int CommandToScene(int scene, const IVarList& msg) = 0;

	
	/// \brief ������Ϸ��־
	/// \param name ����
	/// \param type ����
	/// \param content ����
	/// \param comment ��ע
	virtual bool SaveLog(const wchar_t* name, int type, 
		const wchar_t* content, const wchar_t* comment) = 0;
	/// \brief ������Ʒ��־
	/// \param id ��Ʒ���
	/// \param name ��Ʒ����
	/// \param user �û�����
	/// \param op ��Ʒ����
	/// \param reason ����ԭ��
	/// \param comment ��ע
	virtual bool ItemLog(const wchar_t* id, const wchar_t* name, 
		const wchar_t* user, int op, const wchar_t* reason, 
		const wchar_t* comment) = 0;
	/// \brief ����������־
	/// \param from ����������Ϣ�������
	/// \param to ����������Ϣ�������
	/// \param type ��Ϣ����
	/// \param content ����
	/// \param comment ��ע
	virtual bool ChatLog(const wchar_t* from, const wchar_t* to, int type, 
		const wchar_t* content, const wchar_t* comment) = 0;
	/// \brief ����GM������־
	/// \param name ����
	/// \param type ����
	/// \param content ����
	/// \param comment ��ע
	virtual bool GmLog(const wchar_t* name, int type, const wchar_t* content, 
		const wchar_t* comment) = 0;
	/// \brief �����Զ�����ʽ����־���Զ�������־��ˮ�ţ�
	/// \param table_name ��־���ݿ����
	/// \param fields �����ֶ�����
	/// �ֶ��������ͣ�����(int)����ֵ(double)���ַ��������ַ����������ַ�������ʽ"YYYYMMDD hh:mm:ss"��
	virtual bool CustomLog(const char* table_name, const IVarList& fields) = 0;
	/// \brief �������ɫͬʱ�洢���Զ�����ʽ��־���Զ�������־��ˮ�ţ�
	/// \param table_name ��־���ݿ����
	/// \param fields �����ֶ�����
	/// �ֶ��������ͣ�����(int)����ֵ(double)���ַ��������ַ����������ַ�������ʽ"YYYYMMDD hh:mm:ss"��
	virtual bool CustomLogWithRole(const char* table_name, 
		const IVarList& fields) = 0;
	
	/// \brief ����LUA�ű�
	/// \param script �ű���
	virtual bool LuaLoadScript(const char* script) = 0;
	/// \brief ����LUA�ű�
	/// \param script �ű���
	virtual bool LuaFindScript(const char* script) = 0;
	/// \brief ����LUA�ű��ĺ���
	/// \param script �ű���
	/// \param func ������
	virtual bool LuaFindFunc(const char* script, const char* func) = 0;
	/// \brief ͬ������LUA�ű�������res_num����ֵ����������Ϊ-1��ʾ����������
	/// \param script �ű���
	/// \param func ������
	/// \param args �����б�
	/// \param res_num ��Ҫ���صĽ������
	/// \param result ����ִ�н��������Ϊ�գ�
	virtual bool LuaRunFunc(const char* script, const char* func, 
		const IVarList& args, int res_num = 0, IVarList* result = NULL) = 0;
	/// \brief LUA�ű����󲶻�
	/// \param state �ű�״̬��
	/// \param error ������Ϣ 
	virtual void LuaErrorHandler(void* state, const char* error) = 0;
	/// \brief ���LUA�ű���������
	/// \param state �ű�״̬��
	virtual int LuaGetArgCount(void* state) = 0;
	/// \brief �ж�LUA�ű���������
	/// \param state �ű�״̬��
	/// \param index ������������1��ʼ��
	virtual bool LuaIsInt(void* state, int index) = 0;
	virtual bool LuaIsInt64(void* state, int index) = 0;
	virtual bool LuaIsFloat(void* state, int index) = 0;
	virtual bool LuaIsDouble(void* state, int index) = 0;
	virtual bool LuaIsString(void* state, int index) = 0;
	virtual bool LuaIsWideStr(void* state, int index) = 0;
	virtual bool LuaIsObject(void* state, int index) = 0;
	/// \brief ���LUA�ű�����
	/// \param state �ű�״̬��
	/// \param index ������������1��ʼ��
	virtual int LuaToInt(void* state, int index) = 0;
	virtual int64_t LuaToInt64(void* state, int index) = 0;
	virtual float LuaToFloat(void* state, int index) = 0;
	virtual double LuaToDouble(void* state, int index) = 0;
	virtual const char* LuaToString(void* state, int index) = 0;
	virtual const wchar_t* LuaToWideStr(void* state, int index) = 0;
	virtual PERSISTID LuaToObject(void* state, int index) = 0;
	/// \brief ѹ��LUA�ű�ִ�н�����������ҵ�˳��
	/// \param state �ű�״̬��
	/// \param value �������
	virtual void LuaPushBool(void* state, bool value) = 0;
	virtual void LuaPushNumber(void* state, double value) = 0;
	virtual void LuaPushInt(void* state, int value) = 0;
	virtual void LuaPushInt64(void* state, int64_t value) = 0;
	virtual void LuaPushFloat(void* state, float value) = 0;
	virtual void LuaPushDouble(void* state, double value) = 0;
	virtual void LuaPushString(void* state, const char* value) = 0;
	virtual void LuaPushWideStr(void* state, const wchar_t* value) = 0;
	virtual void LuaPushObject(void* state, const PERSISTID& value) = 0;
	virtual void LuaPushTable(void* state, const IVarList& value) = 0;


	/// \brief ������Ӫ������Ϣ
	/// \param id ����
	virtual bool FindManageInfo(const char* id) = 0;
	/// \brief �����Ӫ������Ϣ��ֵ
	/// \param id ����
	virtual const wchar_t* GetManageInfoName(const char* id) = 0;
	virtual int GetManageInfoType(const char* id) = 0;
	virtual const wchar_t* GetManageInfoProperty(const char* id) = 0;
	virtual int GetManageInfoStatus(const char* id) = 0;
	virtual int GetManageInfoVersion(const char* id) = 0;
	/// \brief �����Ӫ������Ϣ��ǰ���İ汾��
	virtual int GetManageInfoMaxVersion() = 0;
	/// \brief ���»���������Ϣ��ֵ��nx_config��
	/// \param id ����
	/// \param name ����(��ѡ)
	/// \param prop ��ֵ
	/// \param type ����
	/// \param status ״̬
	virtual bool UpdateManageBase(const char* id, const wchar_t* name, 
		int type, const wchar_t* prop, int status) = 0;
	/// \brief ������Ӫ������Ϣ��ֵ��nx_config��
	/// \param id ����
	/// \param name ���֣���ѡ��
	/// \param prop ��ֵ
	/// \param type ����
	/// \param status ״̬
	/// \param version �汾��
	virtual bool UpdateManageInfo(const char* id, const wchar_t* name, 
		int type, const wchar_t* prop, int status, int version) = 0;
	/// \brief ɾ����Ӫ������Ϣ��ֵ
	/// \param id ����
	virtual bool RemoveManageInfo(const char* id) = 0;
	/// \brief ������е���Ӫ������Ϣ����
	/// \param result �����ַ�������б�
	virtual int GetManageInfoList(IVarList& result) = 0;
	/// \brief ������д���ָ���汾�ŵĵ���Ӫ������Ϣ����
	/// \brief version �汾��
	/// \param result �����ַ�������б�
	virtual int GetManageInfoListAboveVersion(int version, 
		IVarList& result) = 0;

	/// \brief ��ȡ����������ǰ����
	/// \param obj ����
	/// \param func ����������
	virtual int GetCountBeatIndex(const PERSISTID& obj, const char* func) = 0;

	/// \brief ս��������ز���
 
	/// \brief ս�������� ����ս�����
 	/// \param name ��ɫ��
	/// \param command ��������� (��OnRequestLeaveRoom() �ص���ԭ������)
	/// \param bDelRole �Ƿ�ɾ����ɫ( true ɾ����ɫ false �ڷ��䱻���ٵ�ʱ��ͳһɾ��)
	virtual bool  RequestLeaveRoom(const wchar_t* name, const IVarList& command, bool bDelRole = false) = 0;

	/// \brief ����custom ��Ϣ��member
	/// \brief room_id	����id
	/// \brief role_uid ��ɫuid
	/// \brief command ���������
	virtual bool SendCustomToMeber(int room_id, const char* role_uid, const IVarList& command) = 0;

	/// \brief ����custom ��Ϣ�� platform
	/// \brief command ���������
	virtual bool SendCustomToPlatform(const IVarList& command) = 0;
	/// \brief ������Ϣ�����ӷ�����
	/// \param index ���ӷ���������
	/// \param room_id ����id
	/// \param msg ��Ϣ�����б�
	virtual bool SendExtraMessage(int index, const IVarList& msg) = 0;
	/// \brief �������
	/// \brief ���·�����Ӵ�С
	virtual len_t GetPathGridSize() = 0;
	/// \brief ��ø�������
	virtual size_t GetPathGridRows() = 0;
	/// \brief ��ø�������
	virtual size_t GetPathGridCols() = 0;
	/// \brief ��ø���ռ�ж���
	/// \brief ����ֵ��������
	/// \param ret ���ض���PERSISTID
	virtual int GetPathGridBalker(size_t index, IVarList& ret, int object_groupid) = 0;
	/// \brief �߼�ռ�ø��ӣ������Ѿ��ڸ��ӵĶ���Ӱ�죩
	virtual void SetPathGridLogicBalk(size_t index, bool balk) = 0;
	/// \brief ��ѯָ������״̬
	/// \brief ����ֵ �����Ƿ����� false
	/// \param index ����
	/// \param can_walk �����Ƿ����
	/// \param logic_balk �Ƿ��߼�ռ��
	/// \param object_occupy �Ƿ����ռ��
	virtual bool QueryPathGridStatus(size_t index, bool& can_walk, bool& logic_balk, bool& object_occupy, int object_groupid) = 0;
	/// \brief ��ö������ڸ���
	virtual void QueryObjectGridIndex(const PERSISTID& obj, IVarList& ret) = 0;
	/// \brief ����������ڸ���
	virtual int GetGridIndexByPos(len_t x,len_t z) = 0;
	/// \brief ��ø�������Ӧ������
	virtual void GetPosByIndex(int index, len_t& x, len_t& z) = 0;
	/// \brief ����ܱߵĸ��� 
	/// \param index,
	/// \param ret,�����ܱ���Ч��������;
	virtual void GetAroundGridIndex(int index, IVarList& ret) = 0;

	/// \brief�����Ƿ���վ����ĳ��
	/// \return MOTION_RESULT_ENUM ���ֵ
	/// \param x 
	/// \param z 
	/// \param ret ����ռ�õĸ���
	virtual int CanStandPointByPathGrid(const PERSISTID& obj,len_t x,len_t z, IVarList& ret) = 0;

	/// \brief���Ѱ··��	
	/// \briefA* Ѱ· �ȽϺģ��߼������ã�����ֵ�����ĵĺ�����
	/// \param nBeginIndex ���������� �����Ͻ���0�����½�����������-1��
	/// \param nEndIndex Ŀ��������� �����Ͻ���0�����½�����������-1��
	/// \param path ���صĽ�����п���Ϊ��
	/// \param get_range ����������������Χ��0��ȫ��ͼ
	/// \param max_try ����Դ��� 0 �������ƣ�Ŀ��㲻��ȥ�г����е��ᵼ�¼���
	/// \param slash �Ƿ�֧��б�ߣ��⽫����̽����λ����һ����Ѱ·ʱ����΢��һ����㣬б�ߴ��������Ƿ�б�߶����߲ſɴ�����
	/// \param nFormula  ����ʽ�㷨��Ŀǰ֧�֣�0,1��0���������㷨������������Ч����΢��Щ����Ŀ�Ͽ����� 0 �� 1 ���ԣ���ͬ�ĵ�ͼ������ܲ�ͬ
	/// \param close_path  ��Ŀ�ĵ㲻����������Ƿ񷵻����·�̣��������������ģ�

	virtual int GetPath(int nBeginIndex, int nEndIndex, int object_groupid, IVarList & path, 
		unsigned int get_range = 5, unsigned int max_try = 50,	int slash = 1, int nFormula = 0, int close_path =0) = 0;

#ifndef _DEBUG
	/// \brief ͨ�����uid,��ȡ���������ƣ�
	/// \param uid	���id
	virtual const wchar_t* GetServerNameByPlayerUid( const char *uid) = 0;
#endif

};

inline IKernel::~IKernel() {}

#endif // _SERVER_IKERNEL_H
