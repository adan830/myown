-- �ļ���: robot_ui.lua
-- ˵��: ui ʹ�ú�������
-- ����:
-- ��Ȩ����:

require("./robot_def")
require("./robot_ui_def")
require("./robot_cmd_define")
require("./robot_menu_cmd_func")

--����˵���Ӧ����
function on_new_menue_clicked( menu_id )
    if menu_id >= M_ID_ROBOT_PROCESS_BASE and menu_id<= M_ID_ROBOT_PROCESS_BASE+ROBOT_PROCESS_CUSTOM_COUNT then
        menu_custom_task_case(menu_id)
        return true
    end
	-- ����message handler
	local cmd_handler = tb_menu_cmd_handler[menu_id]
	if cmd_handler ~= nil then
		cmd_handler()
	end
    return true
end

--�����˵�ǰ����
function on_new_popup_menue()
    -- �Ʒ�ע��
	ex_add_menu_item( M_ID_CHARGE_REGISTER, "Charge=>�Ʒ�ע��", false,2)
    -- �Ʒѵ�½
	ex_add_menu_item( M_ID_CHARGE_LOGIN, "Charge=>�Ʒѵ�½", false,2)
	-- ��¼
	ex_add_menu_item( M_ID_LOGIN, "Normal=>��ͨ��¼", false,1)
	-- ������ɫ
	ex_add_menu_item( M_ID_CREATE_ROLE, "������ɫ", false)
	-- ѡ���ɫ
	ex_add_menu_item( M_ID_CHOOSE_ROLE, "ѡ���ɫ", false)
    -- �˳��˺�
	ex_add_menu_item( M_ID_LOG_OUT, "�˳��˺�", false)
    ex_add_menu_item( 0, "", false)
    
    -- �л�����
    ex_add_menu_item( M_ID_ROBOT_SWITCH_SCENE, "�л�����", false)
    -- GM���� 
    ex_add_menu_item( M_ID_ROBOT_GM_CMD, "GM����", false)
    -- �Զ�����Ϣ
    ex_add_menu_item( M_ID_ROBOT_MSG_CMD, "�Զ�����Ϣ", false)
    ex_add_menu_item( 0, "", false)

    -- ��ʼ����
	ex_add_menu_item( M_ID_START_MOVE, "��ʼ����", false)
	ex_add_menu_item( MD_ID_STOP_MOVE, "ֹͣ����", false) 
    ex_add_menu_item( 0, "", false)
    -- ����
    ex_add_menu_item( M_ID_STOP_TASK_WORK, "ֹͣ����", false)
    ex_add_menu_item( M_ID_ROBOT_PROCESS_BASE, "����", false)
   

	
	-- ����
	-- ex_add_menu_item( M_ID_PAY_ITEM, "����Ҹ���", false)
	
	-- ------------------------------------------------------------------------
	-- gm ���� 80
	--ex_add_menu_item( M_ID_GM_FULL_PACK, "gm ���� 80", false)
	-- gm ���� 20 ���¸���
	--ex_add_menu_item( M_ID_GM_ADJUST_PACK, "gm ���� 70 ���¸���", false)
	-- gm ���� 40
	--ex_add_menu_item( M_ID_GM_NORMAL_PACK, "gm ���� 40", false)
	-- gm ���� 10 ���¸���
	--ex_add_menu_item( M_ID_GM_ADJUST_PACK_40, "gm ���� 40 ���¸���", false)
	-- һ����װ
	--ex_add_menu_item( M_ID_GM_PERFECT_EQUIP, "gm һ����װ", false)

	-- �����л�����¡����ս��
	--ex_add_menu_item( M_ID_SWITCH_TO_SCENE, "�����л�����¡����ս��", false)


	-- ��������
	--ex_add_menu_item( M_ID_CREATE_ROOM, "��������", false)
	-- ���뷿��
	--ex_add_menu_item( M_ID_JOIN_ROOM, "���뷿��", false)
	-- ��ʼ����
	--ex_add_menu_item( M_ID_BEGIN_FIGHT, "��ʼ����", false)

	-- ���߼���1-1000�Ż�����
	--ex_add_menu_item( M_ID_GM_LOAD_OFFLINE_ROBOT, "gm ���߼���1-1000�Ż�����", false)

	-- ��ʼ�ӽ�����
	--ex_add_menu_item( M_ID_BEGIN_TASK, "��ʼ�ӽ�����", false)
	-- ֹͣ��������
	--ex_add_menu_item( M_ID_END_TASK, "ֹͣ��������", false)

	--����������
	--ex_add_menu_item( M_ID_GET_ARENA_INFO, "��������������", false)

	--��ʼ�����߼��ӳ�
	--ex_add_menu_item( M_ID_BEGIN_MEASURE_LOGIC_LATENCY, "��ʼ�����߼��ӳ�", false)
	--ֹͣ�����߼��ӳ�
	--ex_add_menu_item( M_ID_END_MEASURE_LOGIC_LATENCY, "ֹͣ�����߼��ӳ�", false)

	--��������
	--ex_add_menu_item( M_ID_CREATE_GUILD, "��������", false)
	--������������
	--ex_add_menu_item( M_ID_FULL_GUILD_DATA, "������������", false)
	--���湫������
	--ex_add_menu_item( M_ID_SAVE_GUILD_DATA, "���湫������", false)

	--������� role info
	--ex_add_menu_item( M_ID_REQ_ROLE_INFO, "������� role info", false)
	--������� fight info
	--ex_add_menu_item( M_ID_REQ_FIGHT_INFO, "������� fight info", false)
	--������� full role data
	--ex_add_menu_item( M_ID_REQ_FULL_ROLE_DATA, "������� full role data", false)

	--��ʼ��ȡ������
	--ex_add_menu_item( M_ID_BEGIN_PULL_ORDERNO, "��ʼ��ȡ������", false)
	--������ȡ������
	--ex_add_menu_item( M_ID_END_PULL_ORDERNO, "������ȡ������", false)

end
