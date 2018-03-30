//---------------------------------------------------------
//�ļ���:       ToolItemModule.h
//��  ��:       �ɱ�ʹ�õĵ�������Ʒ
//˵  ��:       
//          
//��������:      2014��11��24��
//������:         
//�޸���:
//   :         
//---------------------------------------------------------

#ifndef FSGAME_ITEM_TOOL_ITEM_MODULE_H
#define FSGAME_ITEM_TOOL_ITEM_MODULE_H

#include "Fsgame/Define/header.h"
#include "FsGame/Define/ToolItemDefine.h"
#include <vector>
#include <map>

class DropModule;
class CapitalModule;
class FlowModule;
class ContainerModule;
class CoolDownModule;
class EquipmentModule;
class BufferModule;
class LogModule;
class ItemEffectBase;

class ToolItemModule: public ILogicModule
{
public:

    virtual bool Init(IKernel* pKernel);
    virtual bool Shut(IKernel* pKernel);

public:
	// �ж�ʹ����Ʒ�Ƿ���cd��
	bool IsItemCoolDown(IKernel* pKernel, const PERSISTID& self, const PERSISTID& item);

    // ʹ����Ʒ, ����ʹ�ûָ������ĵ���
    bool UseItem(IKernel* pKernel, const PERSISTID& user,
        const PERSISTID& sender,  const PERSISTID& item, int nCount );

	// ϡ����Ʒ��ʾ
	// strFrom��Դ
	int TipsSpecialItem(IKernel* pKernel, const PERSISTID& self, 
		const char* itemConfigID, const int color, const EmFunctionEventId src);

	// ��Ʒ�Ƿ���ƥ��ѯ��
	static bool IsMatchState(IKernel* pKernel, const PERSISTID& item, int nMatch);

	// ��Ʒ�Ƿ�֧�ְ�
	static bool IsBindAble(IKernel* pKernel, const char* pszItem);
	static bool IsBindAble(IKernel* pKernel, const PERSISTID& item);
	// ��Ʒ�Ƿ�֧�ְ�(֧��װ������Ʒ��nColor: ��pszItem��װ������Ʒʱ����Ҫ����ʾƷ��)
	static bool IsBindAble(IKernel* pKernel, const char* pszItem, int nColor);

	// �Ƿ�Ϊ��װ������Ʒ
	static bool IsCanWearItem(IKernel* pKernel, const char* strItemConfig);
private:
    //////////////////////////////////////////////////////////////////////////
    // ��Ʒʹ��Ч��
    //////////////////////////////////////////////////////////////////////////

    // ʹ����Ʒ, �������ĵ���Ʒ����
    int InnerUseItem(IKernel *pKernel, const PERSISTID &user, const PERSISTID &item, int request_amount);

	// չʾ��ȡ����Ʒ
	static void ShowGetItem(IKernel *pKernel, const PERSISTID &user, const IVarList& showItems);

    //////////////////////////////////////////////////////////////////////////
    // ��������
    //////////////////////////////////////////////////////////////////////////
    // ���߿ɷ�ʹ��
    bool IsItemUsable(IKernel* pKernel, const PERSISTID& user,
        const PERSISTID& item, int amount);

    // ������Ʒ, ����ʵ�����ĵ�����
    int ConsumItem(IKernel* pKernel, const PERSISTID& item, int request_amount); 

    //////////////////////////////////////////////////////////////////////////
    // ��Դ����
    //////////////////////////////////////////////////////////////////////////

    // ���ʹ�ð�����Ŀ
    const std::vector<ItemEffectParam> *GetEffectEntry(const std::string &package_name);

    bool GetEffectID(std::string &the_name, int *the_id);
public:
    //��ȡ����Ч����Ϣ
    bool GetItemEffectInfo(IKernel* pKernel, const char* itemConfig, int effectType, CVarList& args);
public:
    // ������Դ
    bool LoadResource(IKernel *pKernel);

    // ��Ʒʹ��Ч���ķ���
    bool LoadEffectTypeConfig(IKernel *pKernel);

    // ��Ʒʹ��Ч����
    bool LoadEffectPackage(IKernel *pKernel);

	// ���¼��ص�������
	static void ReloadToolItemConfig(IKernel* pKernel);
public:

    static ToolItemModule* m_pToolItemModule;

private:
    typedef std::map<std::string, int> EffectTypeMap;
    typedef std::map<std::string, std::vector<ItemEffectParam> > EffectParamMap;

    EffectTypeMap m_EffectType;
    EffectParamMap m_EffectParam;

	ItemEffectBase* m_pArrItemEffect[MAX_ITEM_EFFECT_NUM];		// ��Ʒʹ��Ч��

    static DropModule *m_pDropModule;
    static FlowModule *m_pFlowModule;
    static CapitalModule *m_pCapitalModule;
    static ContainerModule *m_pContainerModule;
    static CoolDownModule *m_pCoolDownModule;
    static EquipmentModule *m_pEquipmentModule;
    static BufferModule *m_pBufferModule;
    static LogModule *m_pLogModule;
};

#endif // _ToolItemModule_H

