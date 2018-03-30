//------------------------------------------------------------------------------
// �ļ���:      FightInterface.h
// ��  ��:      ս��ϵͳ�����ӿڡ����������ͷţ�buffer���ܣ�ս���˺�����ȡ�
// ˵  ��:
// ��������:    2013��2��22��
// ������:       
// ��ע:
//    :       
//------------------------------------------------------------------------------

#ifndef __FIGHTINTERFACE_H__
#define __FIGHTINTERFACE_H__

#include "Fsgame/Define/header.h"
#include "utils/singleton.h"
//#include "FsGame/Interface/DeclareProperty.h"
#include "FsGame/Define/FightDefine.h"
#include "FsGame/Define/SkillDefine.h"
//#include "FsGame/Define/BufferDefine.h"

class FightInterface
{
public:
    #pragma region // ����������ؽӿ�

    /**
    *@brief  �ж϶����Ƿ�ѧϰ��ָ���ļ���
    *@param  [in] pKernel �����ں�ָ��
    *@param  [in] self �����Ķ���
    *@param  [in] szSkillConfigID ���ܵ�ConfigID
    *@return bool ������ָ���ļ��ܷ���true�����򷵻�false
    */
    bool Contains(IKernel* pKernel, const PERSISTID& self, const char* szSkillConfigID);

    /**
    *@brief  ���һ�ȡ�����Ƿ�ѧϰ��ָ���ļ���
    *@param  [in] pKernel �����ں�ָ��
    *@param  [in] self �����Ķ���
    *@param  [in] szSkillConfigID ���ܵ�ConfigID
    *@return PERSISTID ����ܲ鵽ָ�����ܣ��򷵻ض�Ӧ�ļ��ܶ���id
    */
    PERSISTID FindSkill(IKernel* pKernel, const PERSISTID& self, const char* szSkillConfigID);

	/**
	*@brief  ѧϰ����
	*@param  [in] pKernel �����ں�ָ��
	*@param  [in] self ѧϰ���ܵĶ���
	*@param  [in] szSkillConfigID ���ܵ�ConfigID
	*@return PERSISTID ��ӳɹ�����skill�����򷵻�PERSISTID()
	*/
	PERSISTID AddSkill(IKernel* pKernel, const PERSISTID& self, const char* szSkillConfigID);

    /**
    *@brief  ʹ�ü���(��ʱֻ�ṩNPCʹ��)
    *@param  [IN] pKernel �����ں�ָ��
    *@param  [IN] self ����ʩ����
    *@param  [IN] szSkillConfigID ����ID
    *@param  [in] args �����б������ǰ���һ���������ö��󣬿�����һ����������Ŀ��㣬Ҳ�����ǿ��б�
    *@return int ���ؼ���ʩ��״̬������USESKILL_RESULT_SUCCEED��ʾʩ�ųɹ������򷵻ض�Ӧ�����
    */
    int UseSkill(IKernel* pKernel, const PERSISTID& self, const char* szSkillConfigID, const IVarList& args);

    /**
    *@brief  ʹ�ü��ܣ���ָ����ʩ��һ�����ܡ���Ҫ���ʩ�ŵļ������÷�����ָ����ʩ�ŵ�Ҫ��
    *@param  [in] pKernel �����ں�ָ��
    *@param  [in] self ����ʩ����
    *@param  [in] skill ʩ�ŵļ��ܶ���
    *@param  [in] args �����б������ǰ���һ���������ö��󣬿�����һ����������Ŀ��㣬Ҳ�����ǿ��б�
    *@return int ���ؼ���ʩ��״̬������USESKILL_RESULT_SUCCEED��ʾʩ�ųɹ������򷵻ض�Ӧ�����
    */
    int UseSkill(IKernel* pKernel, const PERSISTID& self, const PERSISTID& skill, const IVarList& args);

	/**
    *@brief  ʹ�ü���(��ʱֻ�ṩNPCʹ��)
    *@param  [IN] pKernel �����ں�ָ��
    *@param  [IN] self ����ʩ����
	*@param  [OUT] fSkillMinDis ������С����
	*@param  [OUT] fSkillMaxDis ����������
	*@param  [IN] strSkillId ����Ҫʹ�õļ���
    *@return bool Ѱ���Ƿ�ɹ�
    */
    bool ReadyUseRandomSkill(IKernel* pKernel, const PERSISTID& self, float& fSkillMinDis, float& fSkillMaxDis, const char* strSkillId = "");

    /**
    *@brief  ��ϼ���
    *@param  [in] pKernel �����ں�ָ��
    *@param  [in] self ����ʩ����
    *@param  [in] breaker ���ܴ����
    *@param  [in] bForce �Ƿ�ǿ�ƴ��
    *@param  [in] iBreakReason ���ԭ��
    *@return bool �ɹ���Ϸ���true�����򷵻�false
    */
    bool BreakSkill(IKernel* pKernel, const PERSISTID& self, const PERSISTID& breaker, bool bForce = false, int iBreakReason=SKILL_BREAK_REASON_TYPE_NONE);

	/**
	*@brief  ǿ��һ�����ܿ�ʼ��ȴ
	*@param  [in] pKernel �����ں�ָ��
	*@param	  [in] self �����ͷ���
	*@param	  [in]	skill Ҫ��ǿ����ȴ�ļ��ܶ���
	*@return bool �ɹ���ȴ����true,���򷵻�false
	*/
	bool ForceBeginCoolDown(IKernel* pKernel, const PERSISTID& self, const char* szSkillConfigID);

    /**
    *@brief  �ж��Ƿ�������ʹ�ü���
    *@param  [in] pKernel �����ں�ָ��
    *@param  [in] self ����ʩ����
    *@return bool ����ʩ�ż��ܷ���true�����򷵻�false
    */
    int IsSkillUseBusy(IKernel* pKernel, const PERSISTID& self);

    /**
    *@brief  ָ�������Ƿ�������ȴ
    *@param  [in] pKernel �����ں�ָ��
    *@param  [in] self �����Ķ���
    *@param  [in] skill ���ܶ���
    *@return bool ����������ȴ�з���true�����򷵻�false
    */
    bool IsCoolDown(IKernel* pKernel, const PERSISTID& self, const PERSISTID& skill);
    bool IsCoolDown(IKernel* pKernel, const PERSISTID& self, const char* szSkillConfigID);

	/*!
	* @brief	���㱾�μ��ܲ����ĳ��ֵ
	* @param	[in] pKernel �����ں�ָ��
	* @param	[in] skill ����
	* @param	[in] nDamValue �˺�ֵ
	* @return	int �������ӵĳ��ֵ
	*/
	int ComputeHateValue(IKernel* pKernel, const PERSISTID& skill, int nDamValue);

	/*!
	 * @brief	ͨ������id,�ж��Ƿ�Ϊ��ҵ���ͨ���� 
	 * @param	[in] pKernel �����ں�ָ��
	 * @param	[in] self ʩ����
	 * @param	[in] strSkillId ����id
	 * @return	int ����ֵ��USOperationState
	 */
	//bool IsPlayerNormalAttackSkill(IKernel* pKernel, const PERSISTID& self, const char* strSkillId);

	/*!
	 * @brief	�����м��ܵȼ�����Ϊ��ͼ� 
	 * @param	[in] pKernel �����ں�ָ��
	 * @param	[in] self ʩ����
	 * @return	void 
	 */
	//void SetSkillLevelMinLevel(IKernel* pKernel, const PERSISTID& self);

	/*!
	 * @brief	�����м��ܵȼ�����Ϊ��ͼ� 
	 * @param	[in] pKernel �����ں�ָ��
	 * @param	[in] self ʩ����
	 * @param	[in] strSkill ��������
	 * @return	int ʣ���CD������ -1 ��ȡʧ�� 
	 */
	//int GetSkillLeftCDSeconds(IKernel* pKernel, const PERSISTID& self, const char* strSkill);

	/*!
	 * @brief	��ȡ���ܵ���CD 
	 * @param	[in] pKernel �����ں�ָ��
	 * @param	[in] self ʩ����
	 * @param	[in] strSkill ��������
	 * @return	int ��CD������ -1 ��ȡʧ�� 
	 */
	int QuerySkillCD(IKernel* pKernel, const PERSISTID& self, const char* strSkill);
#ifndef FSROOMLOGIC_EXPORTS
	/*!
	* @brief	�Ƿ����Ṧ��
	* @param	[in] pKernel �����ں�ָ��
	* @param	[in] self ʩ����
	* @return	bool 
	*/
//	bool IsInFlySkill(IKernel* pKernel, const PERSISTID& self);
#endif
    #pragma endregion // ����������ؽӿ�
public:
    #pragma region // buffer��ؽӿ�

    /**
    *@brief  ��ȡ���������buffer
    *@param  [in] pKernel �����ں�ָ��
    *@param  [in] self �����ҵĶ���
    *@param  [out] bufferIDList ���ػ�ȡ��bufferID�б�
    *@return bool ����true����ȡ��buff
    */
    bool GetAllBuffer(IKernel* pKernel, const PERSISTID& self, IVarList& bufferIDList);
    bool GetAllBufferObject(IKernel* pKernel, const PERSISTID& self, IVarList& bufferObjectList);

    /**
    *@brief  ����buffer����
    *@param  [in] pKernel �����ں�ָ��
    *@param  [in] self �����Ķ���
    *@param  [in] szBufferConfigID buffer��ConfigID
    *@param  [out] result ���ҽ����������
    *@return bool ������ҵ�ָ��buffer����true�����򷵻�false
    */
    bool FindBuffer(IKernel* pKernel, const PERSISTID& self, const char* szBufferConfigID, IVarList& result);

    /**
    *@brief  ���buffer ����ǰ,ȷ���޵е�Ŀ���Ƿ��ܹ���buff,���ܼӵ�Ҫ���ñ���
    *@param  [in] pKernel �����ں�ָ��
    *@param  [in] self �����buffer�Ķ���
    *@param  [in] sender buffer�������
    *@param  [in] szBufferConfigID ����ӵ�buffer config id
    *@return bool ��ӳɹ�true�����򷵻�false
    */
    bool AddBuffer(IKernel* pKernel, const PERSISTID& self,
        const PERSISTID& sender, const char* szBufferConfigID);

    /**
    *@brief  ���buffer ����ǰ,ȷ���޵е�Ŀ���Ƿ��ܹ���buff,���ܼӵ�Ҫ���ñ���
    *@param  [in] pKernel �����ں�ָ��
    *@param  [in] self �����buffer�Ķ���
    *@param  [in] sender buffer�������
    *@param  [in] szBufferConfigID ����ӵ�buffer config id
    *@param  [in] BUFFʱ��
    *@return bool ��ӳɹ�true�����򷵻�false
    */
    bool AddBuffer(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender,
                   const char* szBufferConfigID, const int iLifeTime);

	/**
	*@brief  ɾ��buffer
	*@param  [in] pKernel �����ں�ָ��
	*@param  [in] self �����Ķ���
	*@param  [in] szBufferID ɾ����BUff������ID
	*@return bool ɾ���ɹ�����true�����򷵻�false
	*/
	bool RemoveBuffer(IKernel* pKernel, const PERSISTID& self, const char* szBufferID);

    /**
    *@brief  �����ɫ���ϵ�����BUFFER
    *@param  [in] pKernel �����ں�ָ��
    *@param  [in] self �����Ķ���
    *@return
    */
    void ClearAllBuffer(IKernel* pKernel, const PERSISTID& self);

	/**
    *@brief  ��ӿ�����buffer
    *@param  [in] pKernel �����ں�ָ��
    *@param  [in] self �����buffer�Ķ���
    *@param  [in] sender buffer�������
    *@param  [in] bufftypelist ��ControlBuff ����: bufftypelist << CB_CANT_MOVE << CB_CANT_ATTACK;
	*@param  [in] iLifeTime buff����ʱ�� CONTROL_BUFF_FOREVERΪ����
    *@return bool ��ӳɹ�true�����򷵻�false
    */
    bool AddControlBuffer(IKernel* pKernel, const PERSISTID& self, const PERSISTID& sender, const IVarList& bufftypelist, const int iLifeTime);

	/**
    *@brief  �Ƴ�������buffer
    *@param  [in] pKernel �����ں�ָ��
    *@param  [in] self �����buffer�Ķ���
    *@param  [in] bufftypelist ��ControlBuff ����: bufftypelist << CB_CANT_MOVE << CB_CANT_ATTACK;
    *@return bool ��ӳɹ�true�����򷵻�false
    */
	bool RemoveControlBuffer(IKernel* pKernel, const PERSISTID& self, const IVarList& bufftypelist, int buffTimeType = CONTROL_BUFF_FOREVER);

	/**
    *@brief  �����Ƴ������Ƴ�buff
    *@param  [in] pKernel �����ں�ָ��
    *@param  [in] self		����
    *@param  [in] nClearType (��ӦBufferClear)
    *@return bool ��ӳɹ�true�����򷵻�false
    */
    bool RemoveBuffByClearType(IKernel* pKernel, const PERSISTID& self, int nClearType);

    #pragma endregion // buffer��ؽӿ�

public:
    #pragma region // ս���˺�������ؽӿ�

    /**
    *@brief  ֱ���˺�Ŀ��
    *@param  [in] pKernel �����ں�ָ��
    *@param  [in] self ������
    *@param  [in] target ����Ŀ��
    *@param  [in] object ���ܶ����buffer���󣬶����޷�ȷ���ģ������ǿ�
    *@param  [in] uuid ���ܶ����buffer������Ӧ��uuid�������޷�ȷ����object�������ǿ�
    *@param  [in] bCanTriggerEvent �Ƿ��ܴ����¼�
    *@param  [in] iDamageValue ��ɵ��˺�ֵ
    *@return int ������ʵ�˺�ֵ
    */
    int Damage(IKernel* pKernel, const PERSISTID& self, const PERSISTID& target,
               const PERSISTID& object, const int64_t uuid, bool bCanTriggerEvent,
               int iDamageValue);

    #pragma endregion // ս���˺�������ؽӿ�
public:
	#pragma region
	/*!
	 * @brief	���³��������ļ������ݼ�Ч��
	 * @param	self �����(�����ǳ���Ҳ���������)
	 * @param	strSkillId ����id
	 * @param	nSkillLevel ���ܵȼ�
	 * @param	nSkillSrc  ������Դ(���������õ�)
	 * @return	bool
	 */
	bool UpdateSkillData(IKernel* pKernel, const PERSISTID &self, const char* strSkillId, int nSkillLevel, int nSkillSrc = 0);

	/*!
	 * @brief	�Ƴ����������ļ���
	 * @param	self �����(�����ǳ���Ҳ���������)
	 * @param	strSkillId ����id
	 * @return	bool
	 */
	bool RemoveSkillData(IKernel* pKernel, const PERSISTID &self, const char* strSkillId);

	/*!
	 * @brief	ͨ���������ж���������
	 * @param	strSkillId ����id
	 * @return	int �������Ͷ�ӦSkillType
	 */
	int GetSkillTypeByConfig(IKernel* pKernel, const char* strSkillId);

	/*!
	* @brief	��ȡ��ɱ��������б�
	* @param	outAttackerList ��������б�
	* @param	self ����ɱĿ��
	* @param	killer ��ɱ��(Ϊ��ʱ,���ص��б��д��л�ɱ��)
	* @return	bool �Ƿ��ѯ�ɹ�
	*/
//	bool GetAttackerList(IKernel* pKernel, IVarList& outAttackerList, const PERSISTID& self, const PERSISTID& killer);
	#pragma endregion
protected:
private:
	/**
	*@brief  ɾ������
	*@param  [in] pKernel �����ں�ָ��
	*@param  [in] self ��ɾ�����ܵĶ���
	*@param  [in] skill ����ɾ���ļ���
	*@return bool ɾ���ɹ�����true�����򷵻�false
	*/
	bool RemoveSkill(IKernel* pKernel, const PERSISTID& self, const PERSISTID& skill);

	/**
	*@brief  ɾ������
	*@param  [in] pKernel �����ں�ָ��
	*@param  [in] self ��ɾ�����ܵĶ���
	*@param  [in] szSkillConfigID ����ɾ���ļ���id
	*@return bool ɾ���ɹ�����true�����򷵻�false
	*/
	bool RemoveSkill(IKernel* pKernel, const PERSISTID& self, const char* szSkillConfigID);

}; // end of class FightInterface

typedef HPSingleton<FightInterface> FightInterfaceSingleton;

#define FightInterfaceInstance FightInterfaceSingleton::Instance()

#endif // __FIGHTINTERFACE_H__


