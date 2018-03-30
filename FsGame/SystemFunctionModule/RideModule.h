//----------------------------------------------------------
// �ļ���:      RideModule.h
// ��  ��:      ����ģ��
// ˵  ��:
// ��������:  
// ������:      
// �޸���:    
//----------------------------------------------------------

#ifndef __RIDE_MODULE_H__
#define __RIDE_MODULE_H__

#include "Fsgame/Define/header.h"
#include "RideDefine.h"

class RideModule : public ILogicModule
{
typedef std::string String;

public:
    virtual bool Init(IKernel* pKernel);
    virtual bool Shut(IKernel* pKernel);

	// ���������ļ�����
	bool LoadResource(IKernel *pKernel);

	// ��ȡ��������
	PERSISTID GetActivedRide(IKernel *pKernel, const PERSISTID &self);

	// ���¼�����������
	static void ReloadRideConfig(IKernel* pKernel);

private:
	// �׼����ü���
	bool LoadUpgradeStepRes(IKernel* pKernel);

	// ����Ƥ�������ļ�����
	bool LoadRideSkinRes(IKernel *pKernel);

	// ����Ƥ���������ü���
	bool LoadSkinUnlocRes(IKernel *pKernel);

private:
	// �������Ƥ������
	const RideSkin* GetRideSkin(const int skin_id);

	// ȡ��������Ϣ
	const RideStepCfg* GetRideStepCfg(const int step);

	// ȡ��Ƥ����������
	const SkinUnlockCfg* GetSkinUnlockCfg(const int id);

	// �ɷ�����
	bool CanUpgradeStep(const int step, const int star);

	// ��ʼ��ʱ�������԰�ID
	bool SetRideAddPckID(IKernel* pKernel, const PERSISTID& Player, 
		const PERSISTID& Ride);

	// ȡ��Ҫ��������Ҫ����������-ֵ
	bool GetPckMapPropValue(IKernel *pKernel, const PERSISTID &self, 
		const UpdatePkgTypes up_pkg_type, const IVarList& prop_names, 
		const IVarList& prop_values, std::map<std::string, double>& map_prop_value);

	// ȡ��������ʾ������ʱ�ĸ�����������ʽ
	const UpdatePkgTypes GetRideUpPckType(IKernel* pKernel, const PERSISTID& self, 
		const PERSISTID& ride);

	// ��������
	bool ActiveRide(IKernel *pKernel, const PERSISTID &self, 
		const PERSISTID &ride);

	// ʹ��ĳƤ��
	bool UseDefaultSkin(IKernel *pKernel, const PERSISTID &ride);

	// ǰ��Ƥ���Ƿ��ѽ���
	bool IsUnlockPreSkin(IKernel* pKernel, const PERSISTID& self,
		const int skin_id);

	// �ѽ���
	bool IsUnlock(IKernel* pKernel, const PERSISTID& self,
		const int skin_id);

	// �ɷ��л�Ϊ����Ƥ��
	bool CanChangeStepSkin(IKernel* pKernel, const PERSISTID& self);

public:

	// ��������ȡ��Ƥ��ID
	const int GetSkinByType(const int skin_type);

	// ��������ȡ��Ƥ��ID
	const int GetSkinByType(IKernel *pKernel, const PERSISTID &self, 
		const int skin_type);

	// ��ѯ��ǰ����׼�
	const int QueryRideStep(IKernel *pKernel, const PERSISTID &self);

	// ��������
	bool AddRide(IKernel *pKernel, const PERSISTID &self, 
		const char *rideConfig);

	// ��������Ƥ��
	bool AddRideSkin(IKernel *pKernel, const PERSISTID &self, 
		const int skin_id);

	// ɾ������Ƥ��
	bool RemoveRideSkin(IKernel *pKernel, const PERSISTID &self,
		const int skin_type);

	// ���Ｄ��״̬�¿ͻ�����ʾ����״̬
	bool OnRide(IKernel *pKernel, const PERSISTID &self);

	// ���Ｄ��״̬�¿ͻ����˳�����״̬
	bool DownRide(IKernel *pKernel, const PERSISTID &self);

	// �ɷ�������
	bool CanRide(IKernel* pKernel, const PERSISTID& self);

	// ȡ�����״̬
	const int GetRidingState(IKernel* pKernel, const PERSISTID& self);

	// �Ƿ��ܼ�������
	bool CanActiveRide(IKernel* pKernel, const PERSISTID& self, 
		const char *ride_config);


	/************************************************************************/
	/*									         �ͻ�����������			*/
	/************************************************************************/
	
public:
	// ��������Ƥ��
	bool  ChangeRideSkin(IKernel *pKernel, const PERSISTID &self, 
		const int new_skin_id, const int ride_index = 1);

	// ��������Ƥ����Ϣ
	bool  PushRideSkinMsg(IKernel *pKernel, const PERSISTID &self, 
		const int ride_index = 1);

	// �����������
	void RideUpDownOperate(IKernel* pKernel, const PERSISTID& self, 
		const int operate_type);

	// ȥ����Ƥ����ʶ
	void ClearSkinFlag(IKernel* pKernel, const PERSISTID& self, const int skin_id);

	// ��������Ƥ��
	bool UnlockSkin(IKernel *pKernel, const PERSISTID &self,
		const int skin_id, const int ride_index = 1);

	// ��������
	void UpgradeLevel(IKernel *pKernel, const PERSISTID &self);

	// ��������
	void UpgradeStep(IKernel *pKernel, const PERSISTID &self);

	// �������
	int PlayerOnline(IKernel* pKernel, const PERSISTID& self);


	/************************************************************************/
	/*									         ������������				*/
	/************************************************************************/

public:
	// ��������������
	void UpdateRideAddPkg(IKernel *pKernel, const PERSISTID &self, 
		const PERSISTID& ride, const char *pkg_id, const IVarList& up_msg);

	// Ϊ������������������
	void Upgrade2UpdateAddPkg(IKernel *pKernel, const PERSISTID &self, 
		const PERSISTID& ride);

	// ����ӵ�е�Ƥ��������
	void UpdateUseSkinPkg(IKernel* pKernel, const PERSISTID& self,
		const PERSISTID& ride, const IVarList& up_msg);


	/************************************************************************/
	/*									         �ص�����					*/
	/************************************************************************/

private:
    // �������
    static int OnPlayerRecover(IKernel* pKernel, const PERSISTID& self,
        const PERSISTID& sender, const IVarList& args);

    // ���׼������
    static int OnPlayerReady(IKernel* pKernel, const PERSISTID& self,
        const PERSISTID& sender, const IVarList& args);

	// ����뿪��Ӫս
	static int OnPlayerLeave(IKernel *pKernel, const PERSISTID &self,
		const PERSISTID &sender, const IVarList &args);

    // װ�����ഴ��ʱ�Ļص�
    static int OnRideBoxClassCreate(IKernel* pKernel, int index);

    // װ�����Ļص�
    static int OnRideBoxCreate(IKernel* pKernel, const PERSISTID& ridebox,
        const PERSISTID& sender, const IVarList& args);

    // �����ͷ�����ص�
    static int OnCommandPlayerBeginSkill(IKernel * pKernel, const PERSISTID & self,
        const PERSISTID & sender, const IVarList & args);

	// �����ͷ�����ص�
	static int OnCommandPlayerFinishSkill(IKernel * pKernel, const PERSISTID & self,
		const PERSISTID & sender, const IVarList & args);

	// �����debuffʱ����
    static int OnCommandAddBuffer(IKernel * pKernel, const PERSISTID & self,
        const PERSISTID & sender, const IVarList & args);

	static int OnCommandBeKill(IKernel* pKernel, const PERSISTID& self, 
		const PERSISTID& sender, const IVarList& args);

	static int OnCommandBeDamage(IKernel* pKernel, const PERSISTID& self, 
		const PERSISTID& sender, const IVarList& args);

	static int OnCustomMessage(IKernel* pKernel, const PERSISTID& self, 
		const PERSISTID& sender, const IVarList& args);

	// ����������ö�ʱ��
	static int MountResetTimer(IKernel *pKernel, const PERSISTID &self, 
		int slice);

	// ���ܿ��� 
	static int OnCommandActiveFunc(IKernel *pKernel, const PERSISTID &self,
		const PERSISTID &sender, const IVarList &args);
	
private:
	// ��������
	RideStepCfgList m_RideStepCfgList;

	// ����Ƥ������
	RideSkinMap m_RideSkinMap;

	// �׼�����
	int m_MaxStep;

	SkinUnlockCfgVec m_SkinUnlockCfgVec;
 

public:
	static RideModule* m_pRideModule;

};

#endif