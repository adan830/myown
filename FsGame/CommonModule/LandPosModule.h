//----------------------------------------------------------
// �ļ���:      LandPosModule.h
// ��  ��:		��ҵ�½�����ģ��
// ˵  ��:		
// ��������:    2015��4��8��
// ������:       
// �޸���:    
//    :       
//----------------------------------------------------------
#ifndef _LandPosModule_H_
#define _LandPosModule_H_

#include "Fsgame/Define/header.h"

class AsynCtrlModule;

class LandPosModule : public ILogicModule
{
public:
	// ��ʼ��
	bool Init(IKernel* pKernel);

	// �ر�
	bool Shut(IKernel* pKernel);

	/*!
	 * @brief	������ҵ�½��
	 * @param	self		��Ҷ���
	 * @param	nSceneId	��½�ĳ���id
	 * @param	fPosX,fPosZ	��½��
	 * @return	bool
	 */
	bool SetPlayerLandPosi(IKernel* pKernel, const PERSISTID& self, int nSceneId, float fPosX, float fPosY, float fPosZ);

	/*!
	 * @brief	��ҴӸ���������,�����ϴν����λ��,ȡ������ĵ�½��
	 * @param	self		��Ҷ���
	 * @return	bool
	 */
	bool PlayerReturnLandPosi(IKernel* pKernel, const PERSISTID& self);
private:
	/// \brief ����
	static int OnPlayerRecover(IKernel* pKernel, const PERSISTID& self,
		const PERSISTID& sender, const IVarList& args);

	/// \brief ����
	static int OnPlayerStore(IKernel* pKernel, const PERSISTID& self,
		const PERSISTID& sender, const IVarList& args);
public:
	static LandPosModule*	m_pLandPosModule;
	static AsynCtrlModule*  m_pAsynCtrlModule;
};

#endif