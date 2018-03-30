//--------------------------------------------------------------------
// �ļ���:		CreateRoleModule.h
//--------------------------------------------------------------------

#ifndef __CreateRoleModule_H__
#define __CreateRoleModule_H__

#include "Fsgame/Define/header.h"
#include <string>
#include <vector>
#include <map>

class MotionModule;
class ContainerModule;
class LevelModule;
class StaticDataQueryModule;
class EquipmentModule;
class ItemBaseModule;
class CapitalModule;



//����role���Ե�command����Ϣ���
enum
{
	SUB_EAI_SET_OS_TYPE = 0, //��Ч����ostype
	SUB_EDIT_PLAYER_OS_TYPE = 1,//���߱༭������Իص�������Ϣ���
};


// ������ɫ
class CreateRoleModule : public ILogicModule
{
public:
	// ��ʼ��
	virtual bool Init(IKernel* pKernel);

	// �ر�
	virtual bool Shut(IKernel* pKernel);

public:
	// ������ɫ
	static int OnCreateRole(IKernel* pKernel, const PERSISTID& self,
		const PERSISTID& sender, const IVarList& args);
	// �״ν����ɫ
	static int OnFirstRecover(IKernel* pKernel, const PERSISTID& self,
		const PERSISTID& sender, const IVarList& args);

	// ��ɫ����
	static int OnStore(IKernel* pKernel, const PERSISTID& self,
		const PERSISTID& sender, const IVarList& args);

	// ������ҵ�½ʱ�������Ϣ
	static int OnSaveRoleInfo(IKernel* pKernel, const PERSISTID& self, 
		const PERSISTID& sender, const IVarList& args);

	// ���ر���������Ϣ
	bool LoadSaveRoleInfo(IKernel* pKernel);
	
	// �������Ļ���������
	bool LoadArenaRobotInfo(IKernel* pKernel);

	// ������ҵ�½ʱ�������Ϣ
	bool SaveRoleInfo(IKernel* pKernel, const PERSISTID& self);
	
	//��ʼ����ɫ��Ϣ
	int RoleInit(IKernel* pKernel,const PERSISTID& self);
private:
	// ��������
	struct BornConfig
	{
		int				nNationId;			// ��ӪId
		int				nSceneId;			// ����id
		std::string 	strBornPoint;		// ������
	};
	typedef std::vector<BornConfig> BornConfigVec;

	//��ʼ������
	void _initRoleInfoContainers(IKernel* pKernel, const PERSISTID& self);
	//��ʼ����
	void _initRoleStrRecord(IKernel* pKernel, const PERSISTID& self);
	// ��ʼ��ĳ��������needReset�������Ϊtrue�����ڳ�ʼ�����У�����������ڣ����������
	bool _initRoleInfoContainer(IKernel* pKernel, const PERSISTID& self, const wchar_t* name,
		const char* script, int capacity, bool needReset = true);
	// ��ʼ����ɫ������Ϣ
	void _initRoleInfoTreasure(IKernel* pKernel, const PERSISTID& self);
	// ��ʼ����ɫ��¼���ȼ��������״ε�¼ʱ��
//	void _initRoleInfoFestival(IKernel* pKernel, const PERSISTID& self);
	// �ؼ��������ļ�
	static int nx_reload_create_role_config(void* state);
    //���뿨��
    static int EscapeFromLock(IKernel* pKernel, const PERSISTID& self,
        const PERSISTID& sender, const IVarList& args);	
public:
	static CreateRoleModule *	m_pCreateRoleModule;
private:
    static MotionModule *		m_pMotionModule;
    static ContainerModule *	m_pContainerModule;
	static LevelModule *		m_pLevelModule;
	static StaticDataQueryModule *	m_pStaticDataQueryModule;
	static EquipmentModule *	m_pEquipmentModule;
	static ItemBaseModule *		m_pItemBaseModule;
	static CapitalModule *		m_pCapitalModule;

	std::vector<std::string> m_vSaveRoleProps;
	
	// �����������˵ĳ�ʼװ��
	struct ArenaRobotEquipment
	{
		ArenaRobotEquipment(): role_job(0), role_level(0), equip(""), equip_strlevel(""), equip_jewel(""), equip_soul(""){}
	    int role_job;
	    int role_level;
        std::string equip;
        std::string equip_strlevel;
        std::string equip_jewel;
        std::string equip_soul;
	};
	
	// key = job_lelvel
	typedef std::map<std::string, std::vector<ArenaRobotEquipment>> ArenaEquipMap;
    ArenaEquipMap m_ArenaRobotEquipment;
};

#endif // __CreateRoleModule_H__
