//--------------------------------------------------------------------
// �ļ���:		ICallee.h
// ��  ��:		
// ˵  ��:		
// ��������:	2004��1��4��
// ������:		½����
// ��Ȩ����:	������ţ�������޹�˾
//--------------------------------------------------------------------

#ifndef _SERVER_IROOMCALLEE_H
#define _SERVER_IROOMCALLEE_H

#include "../public/Macros.h"
#include "../public/PersistId.h"
#include "../public/IVarList.h"
#include "LenType.h"

// ����ص��ӿ�

class IKernel;
class IGameObj;

class IRoomCallee
{
public:
	virtual ~IRoomCallee() = 0;

	/// \brief �ഴ������ϵͳ��ʼ��ʱ����
	/// \param pKernel ����ָ�룬��ͬ
	/// \param index �߼����˳������ֵ
	virtual int OnCreateClass(IKernel* pKernel, int index) { return 0; }

	/// \brief ������ɫ�����ط�0��ʾ����ʧ��
	/// \param pSelf ��ҽ�ɫ����
	/// \param msg ������ɫʱ�ͻ��˷����Ĳ���
	/// \return ���ط��͵��ͻ��˵Ĵ�����
	virtual int OnCreateRole(IKernel* pKernel, IGameObj* pSelf,
		const IVarList& msg) { return 0; }

	/// \brief ���󴴽�������ձ�ϵͳ����ʱ����
	/// \param pSelf ����������ͬ
	/// \param sender ������
	virtual int OnCreate(IKernel* pKernel, IGameObj* pSelf, 
		const PERSISTID& sender) { return 0; }

	/// \brief ����ʼ�������Ķ��󴴽�
	/// \param sender ������
	/// \param args ��������
	virtual int OnCreateArgs(IKernel* pKernel, IGameObj* pSelf, 
		const PERSISTID& sender, const IVarList& args) { return 0; }

	/// \brief �������٣�����ϵͳɾ��ʱ����
	/// \param sender ������
	virtual int OnDestroy(IKernel* pKernel, IGameObj* pSelf, 
		const PERSISTID& sender) { return 0; }

	/// \brief ѡ������൱�ڷ�һ�������������������
	/// \param sender ����ѡ��Ķ���
	/// \param function_id ѡ���ܺ�
	virtual int OnSelect(IKernel* pKernel, IGameObj* pSelf, 
		const PERSISTID& sender, int function_id) { return 0; } 

	/// \brief ������������������뱾����Ĵ�����Χʱ����
	/// \param sender ��������
	virtual int OnSpring(IKernel* pKernel, IGameObj* pSelf, 
		const PERSISTID& sender) { return 0; } 

	/// \brief ֹͣ���������������뿪������Ĵ�����Χʱ����
	/// \param sender ��������
	virtual int OnEndSpring(IKernel* pKernel, IGameObj* pSelf, 
		const PERSISTID& sender) { return 0; } 

	/// \brief ���������������󱻼��뵽������ʱ����
	/// \param sender ����
	virtual int OnEntry(IKernel* pKernel, IGameObj* pSelf, 
		const PERSISTID& sender) { return 0; } 

	/// \brief �뿪�������������Ƴ�����ʱ����
	/// \param sender ����
	virtual int OnLeave(IKernel* pKernel, IGameObj* pSelf, 
		const PERSISTID& sender) { return 0; } 

	/// \brief ���Ա������Ƿ񲻿��Լ����������󣬷��ط�0��ʾ���ܼ��룬������Ϊֹͣ
	///		�ڴ˻ص��в������κθı����״̬�Ĳ���
	/// \param sender Ҫ���������Ķ���
	/// \param pos ����������λ�ã���1��ʼ��0��ʾ����λ��
	virtual int OnNoAdd(IKernel* pKernel, IGameObj* pSelf, 
		const PERSISTID& sender, int pos) { return 0; }

	/// \brief �������м�����������
	/// \param sender �����������Ķ���
	/// \param pos ����������λ�ã���1��ʼ
	virtual int OnAdd(IKernel* pKernel, IGameObj* pSelf, 
		const PERSISTID& sender, int pos) { return 0; }

	/// \brief �������м�����������֮��
	/// \param sender �����������Ķ���
	/// \param pos ����������λ�ã���1��ʼ
	virtual int OnAfterAdd(IKernel* pKernel, IGameObj* pSelf, 
		const PERSISTID& sender, int pos) { return 0; }

	/// \brief ���Ա������Ƿ񲻿������߶��󣬷��ط�0��ʾ��������
	///		�ڴ˻ص��в������κθı����״̬�Ĳ���
	/// \param sender Ҫ�Ƴ������Ķ���
	virtual int OnNoRemove(IKernel* pKernel, IGameObj* pSelf, 
		const PERSISTID& sender) { return 0; }

	/// \brief ��������������������֮ǰ
	/// \param sender ���Ƴ������Ķ���
	virtual int OnBeforeRemove(IKernel* pKernel, IGameObj* pSelf, 
		const PERSISTID& sender) { return 0; }

	/// \brief ��������������������
	/// \param sender ���Ƴ������Ķ���
	virtual int OnRemove(IKernel* pKernel, IGameObj* pSelf, 
		const PERSISTID& sender) { return 0; }

	/// \brief ���������ƶ�����λ��֮��
	/// \param sender ���ƶ��Ķ���
	/// \param old_pos �ɵ�λ�ã���1��ʼ
	/// \param new_pos �µ�λ�ã���1��ʼ
	virtual int OnChange(IKernel* pKernel, IGameObj* pSelf, 
		const PERSISTID& sender, int old_pos, int new_pos) { return 0; }

	/// \brief �ͻ��˾��������յ��ͻ��˷��ľ�����Ϣʱ��Ҷ���ű����뵽����������
	/// \param sender ��������
	/// \param first ���ε�¼�Ƿ��һ�ν��볡��
	virtual int OnReady(IKernel* pKernel, IGameObj* pSelf, 
		const PERSISTID& sender, int first) { return 0; }

	/// \brief �������������ϵͳ����������
	/// \param sender ��������Ķ���
	/// \param msg ��������Ĳ���
	virtual int OnCommand(IKernel* pKernel, IGameObj* pSelf, 
		const PERSISTID& sender, const IVarList& msg) { return 0; }

	/// \brief ������������ļ����
	/// \param sender ��������
	virtual int OnLoad(IKernel* pKernel, IGameObj* pSelf, 
		const PERSISTID& sender) { return 0; }

	/// \brief ��������ݿ�ָ��������
	/// \param sender ��������
	virtual int OnRecover(IKernel* pKernel, IGameObj* pSelf, 
		const PERSISTID& sender) { return 0; }

	/// \brief ������ݽ������浽���ݿ�֮ǰ��һ��Ҫ������ص���������ҵ�RoleInfo�ڲ�����
	/// \param sender ��������
	/// \param type ���ں�������±����棬��KnlConst.h
	virtual int OnStore(IKernel* pKernel, IGameObj* pSelf, 
		const PERSISTID& sender, int type) { return 0; }

	/// \brief ϵͳ�ڲ������Ķ���֪ͨ��Ϣ
	/// \param sender ��������
	/// \param msg ֪ͨ��Ϣ����
	virtual int OnSysInfo(IKernel* pKernel, IGameObj* pSelf, 
		const PERSISTID& sender, const IVarList& msg) { return 0; }

	/// \brief �յ��ͻ�����ͨ������Ϣ������0��ʾ�߼�����δ�����ɺ��Ĳ�����ȱʡ����
	/// \param content ������Ϣ����
	virtual int OnSpeech(IKernel* pKernel, IGameObj* pSelf,
		const wchar_t* content) { return 0; }

	/// \brief �յ��ͻ����Զ�����Ϣ
	/// \param msg ��Ϣ����
	virtual int OnCustom(IKernel* pKernel, IGameObj* pSelf, 
		const PERSISTID& sender, const IVarList& msg) { return 0; }

	/// \brief ���ѡ����������֮ǰ�����ط�0��ʾ������ѡ��
	/// \param sender ��ѡ��Ķ���
	/// \param function_id ѡ���ܺ�
	virtual int OnDoSelect(IKernel* pKernel, IGameObj* pSelf, 
		const PERSISTID& sender, int function_id) { return 0; }

	/// \brief �����ƶ�����
	/// \param mode �ƶ�ģʽ����KnlConst.h
	/// \param res �ƶ��������KnlConst.h
	virtual int OnMotion(IKernel* pKernel, IGameObj* pSelf, 
		const PERSISTID& sender, int mode, int res) { return 0; }

	/// \brief ���ƶ���ת�����
	virtual int OnRotate(IKernel* pKernel, IGameObj* pSelf, 
		const PERSISTID& sender) { return 0; }

	/// \brief �յ��ͻ��˵��ƶ�����
	/// \param mode �ƶ�ģʽ
	/// \param arg_num �����������
	/// \param args �����������
	virtual int OnRequestMove(IKernel* pKernel, IGameObj* pSelf, 
		int mode, int arg_num, len_t args[], const char* info) { return 0; }

	/// \brief �ͻ��˶��ߣ�����ֵ��ʾ��Ҷ������������Ϸ�е�����
	/// \param sender ��������
	virtual int OnDisconnect(IKernel* pKernel, IGameObj* pSelf, 
		const PERSISTID& sender) { return 0; }

	/// \brief �ͻ��˼�������
	/// \param sender ��������
	virtual int OnContinue(IKernel* pKernel, IGameObj* pSelf, 
		const PERSISTID& sender) { return 0; }

	/// \brief ��ҽ�����Ϸ
	virtual int OnEntryGame(IKernel* pKernel, IGameObj* pSelf, 
		const PERSISTID& sender) { return 0; }

	/// \brief ��ҽ��볡��֮ǰ���ͻ����յ����볡����Ϣ֮ǰ
	/// \param sender ��������
	virtual int OnBeforeEntryScene(IKernel* pKernel, IGameObj* pSelf, 
		const PERSISTID& sender) { return 0; }

	/// \brief ��ҽ��볡��֮�����յ����볡����������Ϣ
	/// \param sender ��������
	virtual int OnAfterEntryScene(IKernel* pKernel, IGameObj* pSelf, 
		const PERSISTID& sender) { return 0; }

	/// \brief ����뿪����
	/// \param sender ��������
	virtual int OnLeaveScene(IKernel* pKernel, IGameObj* pSelf, 
		const PERSISTID& sender) { return 0; }

	/// \brief Ӧ�ͻ��˵�Ҫ�����ɵ�ǰ��Ϸ������Ϣ��������
	/// \param type �ͻ�����Ҫ����Ϣ����
	/// \param info ������Ϣ
	virtual int OnGetWorldInfo(IKernel* pKernel, IGameObj* pSelf,
		int type, const wchar_t* info) { return 0; }

	/// \brief �����ż����
	/// \param name �ռ�������
	/// \param uid �ռ���Ψһ��ʶ
	/// \param account �������ʺţ��ڷ�������ʺŵ�ϵͳ�ʼ�ʱ��
	/// \param result �����1��ʾ�ɹ�������ֵʧ��
	/// \param serial_no �緢�ųɹ����Ǵ��ż������ݿ��е���ˮ��
	/// \param type: �ż�����
	virtual int OnSendLetter(IKernel* pKernel, IGameObj* pSelf, 
		const wchar_t* name, const char* uid, const char* account, int result, 
		const char* serial_no, int type) { return 0; }

	/// \brief ��ѯ�ż��������
	/// \param total ����ҵ�ǰ���ż�����
	virtual int OnQueryLetter(IKernel* pKernel, IGameObj* pSelf,
		int total) { return 0; }

	/// \brief ���յ��ż������ݿ�����ɾ����
	/// \param source ����������
	/// \param source_uid ������Ψһ��ʶ������Ϊ�գ�
	/// \param time �ż�����ʱ��
	/// \param type �ż�����
	/// \param content �ż�����
	/// \param appendix ��������
	/// \param send_date ��ʵ�ķ���ʱ��
	/// \param serial_no �ż�����ˮ��
	virtual int OnRecvLetter(IKernel* pKernel, IGameObj* pSelf, 
		const wchar_t* source, const char* source_uid, const wchar_t* time, 
		int type, const wchar_t* content, const wchar_t* appendix,
		double send_date, const char* serial_no) { return 0; }

	/// \brief �����ż�ʧ��
	/// \param serial_no �ż�����ˮ��
	virtual int OnRecvLetterFail(IKernel* pKernel, IGameObj* pSelf, 
		const char* serial_no) { return 0; }

	/// \breif �鿴�ż������ݿⲻɾ����
	/// \param source ����������
	/// \param source_uid ������Ψһ��ʶ������Ϊ�գ�
	/// \param time �ż�����ʱ��
	/// \param type �ż�����
	/// \param content �ż�����
	/// \param appendix ��������
	/// \param send_date ��ʵ�ķ���ʱ��
	/// \param serial_no �ż�����ˮ��
	virtual int OnLookLetter(IKernel* pKernel, IGameObj* pSelf, 
		const wchar_t* source, const char* source_uid, const wchar_t* time, 
		int type, const wchar_t* content, const wchar_t* appendix,
		double send_date, const char* serial_no) { return 0; }

	/// \brief �����������ż����
	/// \param type �����ż�������
	/// \param amount �����ż�������
	/// \param serial_no ָ���ż�����ˮ��
	virtual int OnCleanLetter(IKernel* pKernel, IGameObj* pSelf,
		int type, int amount, const char* serial_no) { return 0; }

	/// \brief �������˻��ż����
	/// \param serial_no �ż�����ˮ��
	/// \param result ����������ɹ�Ϊ1��
	virtual int OnBackLetter(IKernel* pKernel, IGameObj* pSelf,
		const char* serial_no, int result) { return 0; }

	/// \brief ��ѯ��������Ʒ��������
	/// \param pSelf ��������
	/// \param player �������
	/// \param uid ���Ψһ��ʶ
	/// \param id ��Ʒ���
	/// \param name ��Ʒ����
	/// \param num ��Ʒ����
	virtual int OnQueryGift(IKernel* pKernel, IGameObj* pSelf, 
		const wchar_t* player, const char* uid, int id, const char* name, 
		int num) { return 0; }

	/// \brief ���������Ʒ��������
	/// \param pSelf ��������
	/// \param player �������
	/// \param uid ���Ψһ��ʶ
	/// \param result �����1��ʾ�ɹ�������ֵʧ��
	/// \param id ��Ʒ���
	/// \param name ��Ʒ����
	/// \param num ��Ʒ����
	virtual int OnGetGift(IKernel* pKernel, IGameObj* pSelf, 
		const wchar_t* player, const char* uid, int result, int id, 
		const char* name, int num) { return 0; }

	/// \brief ���۵㿨��������
	/// \param pSelf ��������
	/// \param player �������
	/// \param uid ���Ψһ��ʶ
	/// \param result �����1��ʾ�ɹ�������ֵʧ��
	/// \param card_no �㿨��
	/// \param price �۸�
	/// \param card_type �㿨����
	virtual int OnSellCard(IKernel* pKernel, IGameObj* pSelf, 
		const wchar_t* player, const char* uid, int result, 
		const char* card_no, int price, const char* card_type) { return 0; }

	/// \brief �������۵㿨��������
	/// \param pSelf ��������
	/// \param player �������
	/// \param uid ���Ψһ��ʶ
	/// \param result �����1��ʾ�ɹ�������ֵʧ��
	/// \param card_no �㿨��
	virtual int OnUnsellCard(IKernel* pKernel, IGameObj* pSelf,
		const wchar_t* player, const char* uid, int result, 
		const char* card_no) { return 0; }

	/// \brief ����㿨��������
	/// \param pSelf ��������
	/// \param player �������
	/// \param uid ���Ψһ��ʶ
	/// \param result �����1��ʾ�ɹ�������ֵʧ��
	/// \param card_no �㿨��
	/// \param fill_account ����������ʺ�
	virtual int OnBuyCard(IKernel* pKernel, IGameObj* pSelf,
		const wchar_t* player, const char* uid, int result, 
		const char* card_no, const char* fill_account) { return 0; }

	// ����������Ʒ��������
	/// \param pSelf ��������
	/// \param player �������
	/// \param uid ���Ψһ��ʶ
	/// \param result �����1��ʾ�ɹ�������ֵʧ��
	/// \param item_no ������Ʒ���
	/// \param amount ������Ʒ����
	/// \param points ��Ʒ����
	/// \param acc_points ����ʻ�ʣ�����
	virtual int OnBuyItem(IKernel* pKernel, IGameObj* pSelf,
		const wchar_t* player, const char* uid, int result, const char* item_no, 
		int amount, int points, int acc_points) { return 0; }

	/// \brief ����������Ʒ���͸������ʺţ�������
	/// \param pSelf ��������
	/// \param player �������
	/// \param uid ���Ψһ��ʶ
	/// \param result �����1��ʾ�ɹ�������ֵʧ��
	/// \param item_no ������Ʒ���
	/// \param amount ������Ʒ����
	/// \param points ��Ʒ����
	/// \param acc_points ����ʻ�ʣ�����
	/// \param give_account ������Ʒ���ʺ�
	virtual int OnBuyItemGive(IKernel* pKernel, IGameObj* pSelf,
		const wchar_t* player, const char* uid, int result, 
		const char* item_no, int amount, int points, int acc_points, 
		const char* give_account) { return 0; }

	// ����������Ʒ���ڶ�������������
	/// \param pSelf ��������
	/// \param player �������
	/// \param uid ���Ψһ��ʶ
	/// \param result �����1��ʾ�ɹ�������ֵʧ��
	/// \param prop_id �ۺ�ID
	/// \param item_no ������Ʒ���
	/// \param amount ������Ʒ����
	/// \param points ��Ʒ����
	/// \param acc_points ����ʻ�ʣ�����
	/// \param give_account ������Ʒ���ʺ�
	/// \param order_id  ����Ķ���ID
	virtual int OnBuyItem2(IKernel* pKernel, IGameObj* pSelf,
		const wchar_t* player, const char* uid, int result, int prop_id,
		const char* item_no, int amount, int points, int acc_points,
		const char* give_account, int order_id) { return 0; }

	/// \brief ����ʻ��ϵ�ʣ�������������
	/// \param pSelf ��������
	/// \param player �������
	/// \param uid ���Ψһ��ʶ
	/// \param result �����1��ʾ�ɹ�������ֵʧ��
	/// \param acc_points ����ʻ�ʣ�����
	virtual int OnGetPoints(IKernel* pKernel, IGameObj* pSelf, 
		const wchar_t* player, const char* uid, int result, 
		int acc_points) { return 0; }

	/// \brief ����ʻ��ϵ��¿���ֹʱ�䣨������
	/// \param pSelf ��������
	/// \param player �������
	/// \param uid ���Ψһ��ʶ
	/// \param type �¿����� [JIANGHU_LIMIT_TIME(2) ��������, ZHIZUN_LIMIT_TIME(3) ��������]
	/// \param result �����1��ʾ�ɹ�������ֵʧ��
	/// \param limit_time �¿���ֹʱ��
	virtual int OnGetLimitTime(IKernel* pKernel, IGameObj* pSelf, 
		const wchar_t* player, const char* uid, int type, int result, 
		double limit_time) { return 0; }

	/// \brief ����ʻ���Ϣ��������
	/// \param pSelf ��������
	/// \param player �������
	/// \param uid ���Ψһ��ʶ
	/// \param result �����1��ʾ�ɹ�������ֵʧ��
	/// \param level �˻��ȼ�
	/// \param points �˻����
	/// \param properties �˻�����
	virtual int OnGetAccountInfo(IKernel* pKernel, IGameObj* pSelf, 
		const wchar_t* player, const char* uid, int result, 
		int level, int points, const char* properties) { return 0; }

	/// \brief ����ʻ��ϵķ�������Ϸʱ�䣨������
	/// \param pSelf ��������
	/// \param player �������
	/// \param uid ���Ψһ��ʶ
	/// \param result �����1��ʾ�ɹ�������ֵʧ��
	/// \param play_time ��������Ϸʱ��
	virtual int OnGetPlayTime(IKernel* pKernel, IGameObj* pSelf, 
		const wchar_t* player, const char* uid, int result, 
		int play_time) { return 0; }

	/// \brief �յ��Ʒ�ϵͳ��֪ͨ��������
	/// \param pSelf ��������
	/// \param player �������
	/// \param uid ���Ψһ��ʶ
	/// \param notify_type ֪ͨ����
	/// \param remark ��ע��Ϣ
	virtual int OnChargeNotify(IKernel* pKernel, IGameObj* pSelf, 
		const wchar_t* player, const char* uid, int notify_type, 
		const char* remark) { return 0; }

	/// \brief �ѻ�ȡ������Ʒ����Ϣ��������
	/// \param pSelf ��������
	/// \param item_no Ϊ�ձ�ʾ��ȫ����Ʒ����Ϣ�������ǵ�����Ʒ����Ϣ
	/// \param amount �յ���Ʒ��Ϣ����
	virtual int OnItemInfo(IKernel* pKernel, IGameObj* pSelf, 
		const char* item_no, int amount) { return 0; }

	/// \brief �����ƶ�ʱ��ͼ����ı�
	/// \param pSelf ��һ�NPC����
	/// \param old_area �ƶ�ǰ����������
	/// \param new_area �ƶ������������
	virtual int OnMapChanged(IKernel* pKernel, IGameObj* pSelf,
		const char* old_area, const char* new_area) { return 0; }
	
	/// \brief ������������Ұ
	/// \param pSelf ��Ҷ���
	/// \param sender ��������
	virtual int OnEntryVisual(IKernel* pKernel, IGameObj* pSelf,
		const PERSISTID& sender) { return 0; }

	/// \brief �����뿪�����Ұ
	/// \param pSelf ��Ҷ���
	/// \param sender ��������
	virtual int OnLeaveVisual(IKernel* pKernel, IGameObj* pSelf,
		const PERSISTID& sender) { return 0; }
	
	/// \brief ���Կͷ�ϵͳ���Զ�����Ϣ��ÿ�������������ĵ�һ����������
	/// \param pSelf ��������
	/// \param msg ��Ϣ����
	virtual int OnGmccCustom(IKernel* pKernel, IGameObj* pSelf,
		const IVarList& msg) { return 0; }

	/// \brief ϵͳ�ڲ�������ͳ����Ϣ��������
	/// \param pSelf ��������
	/// \param msg ��Ϣ����
	/// ����������Ϣ<"create_role"><account><role_name>
	/// ɾ��������Ϣ<"delete_role"><account><role_name>
	virtual int OnSystemStatInfo(IKernel* pKernel, IGameObj* pSelf,
		const IVarList& msg) { return 0; }

	/// \brief �յ��Ʒ�ϵͳ���Զ�����Ϣ
	/// \param pSelf ��������
	/// \param msg ��Ϣ����
	virtual int OnChargeMessage(IKernel* pKernel, IGameObj* pSelf,
		const IVarList& msg) { return 0; }
	
	/// \brief �յ�����ϵͳ����Ϣ����һ����������
	/// \param pSelf ��������
	/// \param index,serial ͨѶ���ӱ�ʶ
	/// \param msg ��Ϣ����
	virtual int OnManageMessage(IKernel* pKernel, IGameObj* pSelf,
		int index, int serial, const IVarList& msg) { return 0; }

	/// \brief �յ����ӷ���������Ϣ����һ����������
	/// \param pSelf ��������
	/// \param index ����������
	/// \param msg ��Ϣ����
	virtual int OnExtraMessage(IKernel* pKernel, IGameObj* pSelf,
		int index, const IVarList& msg) { return 0; }

	/// \brief �յ���������ϵͳ���Զ�����Ϣ
	/// \param pSelf ��������
	/// \param msg ��Ϣ����
	virtual int OnPublicMessage(IKernel* pKernel, IGameObj* pSelf,
		const IVarList& msg) { return 0; }
	/// \brief ��������ϵͳ����������ͬ���������
	/// \param pSelf ��������
	virtual int OnPublicComplete(IKernel* pKernel, IGameObj* pSelf)
		{ return 0; }

	/// \brief ��Ϸ�������ر���ǰ��Ϣ֪ͨ
	/// \param pSelf ��������
	/// \param nServerCloseDelay ��ʣ��Ķ���ʱ��������ر�
	virtual int OnServerCloseBeforeNotify(IKernel* pKernel, IGameObj* pSelf, int nServerCloseDelay)
		{ return 0; }

	/// \brief ��Ϸ�������ر�֪ͨ
	/// \param pSelf ��������
	virtual int OnServerClose(IKernel* pKernel, IGameObj* pSelf)
		{ return 0; }

	/// \brief �յ��������������ɹ�
	/// \param pSelf ��������
	/// \param owner_name ���봴������������������
	/// \param proto_scene_id ԭ�ͳ�����
	/// \param clone_scene_id ���������ţ�Ϊ0��ʾ����ʧ��
	virtual int OnCloneScene(IKernel* pKernel, IGameObj* pSelf,
		const wchar_t* owner_name, int proto_scene_id, int clone_scene_id) 
		{ return 0; }
	/// \brief ������������ȡ��ĸ�λ
	/// \param pSelf ������������
	/// \param owner_name ���봴������������������
	/// \param down_time ������������ʱ�䣨�룩
	virtual int OnCloneReset(IKernel* pKernel, IGameObj* pSelf,
		const wchar_t* owner_name, int down_time) { return 0; }

	/// \brief �޸Ĳ����ߵ�������ݣ����ط�0ֵ��ʾ�޸ĳɹ���
	/// \param pSelf ��Ҷ���
	/// \param command �޸��������
	virtual int OnEditPlayer(IKernel* pKernel, IGameObj* pSelf,
		const IVarList& command) { return 0; }
	/// \brief �ָ�������ݣ����ط�0ֵ��ʾ�޸ĳɹ���
	/// \param pSelf ��Ҷ���
	/// \param command �޸��������
	virtual int OnRecreatePlayer(IKernel* pKernel, IGameObj* pSelf,
		const IVarList& command) { return 0; }

	/// \brief ������ҽ�ɫ
	/// \param pSelf ��������
	/// \param result �������������1ֵ��ʾ�޸ĳɹ�,����ֵΪ�����룩
	/// \param name  ��ɫ����
	/// \param account ��ɫ�˺�
	/// \param new_account ��ɫ�����˺�
	/// \param command �޸��������
	virtual int OnTradeRole(IKernel* pKernel, IGameObj* pSelf, int result,
		const wchar_t* name, const char* account,
		const char* new_account, IVarList& command) { return 0; }

	/// \brief ��ɫ����
	/// \param pSelf ��Ҷ���
	/// \param result �������������1ֵ��ʾ�޸ĳɹ�,����ֵΪ�����룩
	/// \param name  ��ɫ����
	/// \param new_account ��ɫ�����˺�
	/// \param command �޸��������
	virtual int OnChangeName(IKernel* pKernel, IGameObj* pSelf, int result, 
		const wchar_t* name, const wchar_t* new_name, IVarList& command) { return 0; }

	/// \brief ���ָ���ʺŵĽ�ɫ��
	/// \param pSelf ��������
	/// \param command �������
	/// \param account ��ɫ�ʺ�
	/// \param role_num ��ɫ����
	/// \param role_names ��ɫ���б�
	virtual int OnGetAccountRole(IKernel* pKernel, IGameObj* pSelf,
		const IVarList& command, const char* account, int role_num, 
		const IVarList& role_names) { return 0; }

	/// \brief �����ҵĽ�ɫ��Ϣ
	/// \param pSelf ��������
	/// \param command �������
	/// \param role_name ��ɫ����
	/// \param account ��ɫ�ʺ�
	/// \param role_info ��ɫ��Ϣ
	virtual int OnGetRoleInfo(IKernel* pKernel, IGameObj* pSelf,
		const IVarList& command, const wchar_t* role_name, 
		const char* account, const wchar_t* role_info) { return 0; }

	/// \brief ���������ɫ���߼��ص�
	/// \param pSelf ��������
	/// \param name ��ɫ����
	/// \param account ��ɫ�˺�
	/// \param original_name ԭʼ��ɫ����
	/// \param original_serverid ԭʼ��ɫ�ķ�����id
	/// \param result ���������1��ʾ�ɹ�������ֵΪ������
	virtual int OnCreateCrossRole(IKernel* pKernel, IGameObj* pSelf,
		const IVarList& command, const wchar_t* name, const char* account, 
		const wchar_t* original_name, int original_serverid, int result) { return 0; }
	/// \brief ɾ�������ɫ���߼��ص�
	/// \param pSelf ��������
	/// \param original_name ԭʼ��ɫ����
	/// \param original_serverid ԭʼ��ɫ�ķ�����id
	/// \param result ɾ�������1��ʾ�ɹ�������ֵΪ������
	virtual int OnDeleteCrossRole(IKernel* pKernel, IGameObj* pSelf,
		const IVarList& command, const wchar_t* original_name, 
		int original_serverid, int result) { return 0; }
	/// \brief ��������ɫ���߼��ص�
	/// \param pSelf ��������
	/// \param result ��������1��ʾ�ɹ�������ֵΪ������
	virtual int OnClearCrossRole(IKernel* pKernel, IGameObj* pSelf,
		int result) { return 0; }
	/// \brief �յ�������Ϸ����������Ϣ
	/// \param pSelf ��������
	/// \param serverid �����ߵķ�����id
	/// \param sceneid �����ߵĳ���id
	/// \param msg ��������Ĳ���
	virtual int OnCrossCustom(IKernel* pKernel, IGameObj* pSelf,
		int serverid, int sceneid, const IVarList& msg) { return 0; }
	/// \brief �յ�����������Զ�����Ϣ
	/// \param pSelf ��������
	/// \param msg ��������Ĳ���
	virtual int OnCrossMessage(IKernel* pKernel, IGameObj* pSelf,
		const IVarList& msg) { return 0; }
	/// \brief ���������߼��ص�
	/// \param pSelf ��������
	/// \param player ��ҵ�����id
	/// \param addrinfo Ŀ��������ĵ�¼��ַ��Ϣ
	virtual int OnTransferRole(IKernel* pKernel, IGameObj* pSelf,
		const IVarList& command, const PERSISTID& player,
		const char* addrinfo, int result) { return 0; }
	/// \brief ����ϲ����߼��ص�
	/// \param pSelf ��Ҷ��󣬻ص�������������������ݡ�
	/// \param pMergePlayer ���ϲ�����Ҷ���
	virtual int OnMergeRole(IKernel* pKernel, IGameObj* pSelf,
		IGameObj* pMergePlayer) { return 0; }
	/// \brief ����Ͷ����Ϣ�����ʧ��
	/// \param pSelf ����
	/// \param msg ��������, ���ͣ����������֣�����
	//�߼������ж������Ƿ����1���жϵ�һ�������ǲ���int ,���ǲ��ô�������ά��©��ӵ�bug��,��һ������Ӧ���� RESEND_TO_PLAYER_ENUM ö��ֵ
	virtual int OnResendToPlayerFailed(IKernel* pKernel, IGameObj* pSelf,
		const IVarList& msg) { return 0; }
	/// \brief �յ�Sns�������Զ�����Ϣ
	/// \param pSelf ��������
	/// \param msg ��������Ĳ���
	virtual int OnSnsMessage(IKernel* pKernel, IGameObj* pSelf,
		const IVarList& msg) { return 0; }
	

	/// \brief stub������֤�ص�
	/// \param pSelf ��������
	/// \param check_type ��ѯ����
	/// \param dataList ���ݽ����
	virtual int OnStubMsgToMember(IKernel* pKernel, IGameObj* pSelf, int msg_type, const IVarList& dataList) { return 0; }


	//ս����ػص�
 	/// \brief ս���ϲ����߼��ص�
	/// \param pSelf ��Ҷ��󣬻ص�������������������ݡ�
	/// \param pMergePlayer ���ϲ�����Ҷ���
	virtual int OnMergeBattleRole(IKernel* pKernel, IGameObj* pSelf,
		IGameObj* pMergePlayer){return 0;}
 
	/// \brief member���͵�custom ��Ϣ
	/// \param pSelf ��Ҷ���
	/// \param command ��Ϣ��
	virtual int OnCustomMember(IKernel* pKernel, IGameObj* pSelf,const IVarList& command){return 0;}

	/// \brief ƽ̨���ص�custom ��Ϣ
	/// \param pSelf ��Ҷ���
	/// \param command ��Ϣ��
	virtual int OnCustomPlatform(IKernel* pKernel, IGameObj* pSelf, const IVarList& command){return 0;}

	/// \brief �����������֮ǰ������ϵͳɾ��ʱ����
	/// \param sender ������
	virtual int OnBeforeDestroyRoom(IKernel* pKernel, IGameObj* pSelf, 
		const IVarList& command) { return 0; }
};

inline IRoomCallee::~IRoomCallee() {}

#endif // _SERVER_ICALLEE_H

