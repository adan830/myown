-- �ļ���: robot_main.lua
-- ˵��: �����˺�������
-- ����:
-- ��Ȩ����:  

-- �����˶���� member
-- client_id
-- this
-- account
-- password
-- entry_addr
-- entry_port
-- login_type

-- �����˶���� function

-- ���ӽ��������
-- robot_obj.ex_connect_entry( robot_obj )

-- ��¼
-- robot_obj.ex_login

-- �Ͽ������������������
-- robot_obj.ex_disconnect( robot_obj )

-- ��ӡlog��editbox ( string, number, bool)
-- robot_obj.ex_log( robot_obj, ...)

-- ��������������Ϣ
-- robot_obj.ex_send( robot_obj, bytes)

-- ���û����˵�״̬
-- robot_obj.ex_set_state( robot_obj, str_state)

-- ��Ӷ�ʱ��
-- robot_obj.ex_add_hb( robot_obj, hb_name, func_name, time_gap, hb_max)

-- �Ƴ���ʱ��
-- robot_obj.ex_remove_hb( robot_obj, hb_name)

require("./robot_def")
require("./robot_cmd_define")
require("./robot_ui_cmd_func")
require("./robot_srv_msg_func")

----------------------------------------------------------------
-- �¼��ص�
----------------------------------------------------------------
--�����˴����ص�
function on_create( robot_obj )

	-- robot �˺����� client_id ��1 ��ʼ����
	robot_obj.account = ROBOT_PREFIX_ACCOUNT .. ex_string(robot_obj.client_id)
	-- �˺�����һ��
	robot_obj.password = ROBOT_PREFIX_ACCOUNT .. ex_string(robot_obj.client_id)
	robot_obj.name = ROBOT_PREFIX_NAME .. ex_string(robot_obj.client_id)

	-- ����� ip port
	robot_obj.entry_addr = ROBOT_ENTRY_IP
	robot_obj.entry_port = ROBOT_ENTRY_PORT
	robot_obj.login_type = ROBOT_LOGIN_TYPE
	-- ������������
	robot_obj.reconnect_times = 3
	-- ������������һ·��index
	robot_obj.next_pt_index = 0;
	-- �����˴�������յ㻹�Ǵ��յ������
	robot_obj.from_start = 1;
	
    -- ����group��server_id
    robot_obj.group      = CHARGE_GROUP;
    robot_obj.server_id  = CHARGE_SERVER_ID;
    -- ���� charge
	robot_obj.trans_url  = TRANS_URL;
	robot_obj.charge_url = CHARGE_URL;
	robot_obj.register_url = REGISTER_URL;
	
	robot_obj.check_ld_time = CHECK_AUTO_LOGIN_DISCONNECT_TIME;
	robot_obj.login_exceed_time = EXCEED_LOGIN_TIME;
	robot_obj.is_random = IS_RANDOM_TYPE;
	
	
--	robot_obj.st_url = ST_URL;
	

	-- �������񽻽�
	--robot_obj.task_ctrl = 0;
	-- �������к�
	--robot_obj.task_sequence = 1
	-- ����������к�
	--robot_obj.max_task_seq = 30
end

--���������ӻص�
function on_connect( robot_obj, login_result )
	-- ���ӳɹ�
	if login_result == true then
		robot_obj.ex_log( robot_obj, "connect entry OK")
		robot_obj.ex_set_state(robot_obj,"connect entry OK")

		--�����¼
		local bOK = robot_obj.ex_login(robot_obj)
		if ( bOK == false ) then
			robot_obj.ex_end( robot_obj, false );
		end
	-- ����ʧ��
	else
		robot_obj.ex_log( robot_obj, "connect entry fail")
		robot_obj.ex_set_state(robot_obj,"connect entry fail")
		robot_obj.ex_end( robot_obj, false );
		
		--������������
		if robot_obj.reconnect_times > 0 then
			-- 1s ����������
			robot_obj.ex_add_hb( robot_obj, "reconnect_entry", "reconnect_entry", 1000, 1, 0)
			-- �ݼ���������
			robot_obj.reconnect_times = robot_obj.reconnect_times - 1
		else
			robot_obj.ex_log( robot_obj, "reconnect time exhaust")
		end
	end
	
end

--���������ӶϿ��ص�
function on_disconnect( robot_obj )		
	-- ���δ��� ����ʧ�ܴ���
	robot_obj.ex_end(robot_obj, false );
	robot_obj.logoning = false;
	robot_obj.ex_log( robot_obj, "disconnect entry")
	robot_obj.ex_set_state(robot_obj,"disconnect entry")
end

--��¼�ɹ�
function on_login_succeed( robot_obj, ... )
	robot_obj.ex_log( robot_obj, "[--lua--]login sucess")
	
	-- ��½���Խ���
	robot_obj.ex_end( robot_obj, true);
	robot_obj.logoning = false;
end

--���볡��
function on_entry_scene( robot_obj, ident, propcount, sceneId )
	robot_obj.ex_log( robot_obj, "[--lua--]entry scene " .. sceneId )
	-- ��½���Խ���
	if robot_obj.logoning == true then
		robot_obj.ex_end( robot_obj, true);
	end

	-- ���õ�ǰ��������
	robot_obj.preSceneId = robot_obj.sceneId
	robot_obj.sceneId = sceneId

	-- ���ready��ʱ�� 2000ms��
	robot_obj.ex_add_hb( robot_obj, "entry_scene_timer", "entry_scene_timer", 2000, 1, 0)
end

--�����Ŷ���Ϣ
function on_queue( robot_obj, nQueue, nPosition, nPriorCount )

	local player_before = nPosition+nPriorCount

	if player_before ~= 0 then
		-- vip player
		if nQueue == 1 then
			robot_obj.ex_log( robot_obj, "[--lua--] on_queue vip queue players before = ", player_before)
		-- normal player
		else
			robot_obj.ex_log( robot_obj, "[--lua--] on_queue normal queue players before = ", player_before)
		end		
	else
		robot_obj.ex_log( robot_obj, "[--lua--] on_queue end queue, ready to login")
	end
end

--���ܵ���Ϣ�ص�
function on_message( robot_obj, tb_bytes )

	-- ��ȡ��Ϣͷ
	local msg_id = tb_bytes.read_char(tb_bytes)

	-- ���� srv msg handler
	local srv_msg_handler = tb_srv_msg_handler[msg_id]
	if srv_msg_handler ~= nil then
		srv_msg_handler(robot_obj,tb_bytes)
	else
		--robot_obj.ex_log( robot_obj, "[--lua--] not has srv msg handler msg_id = ", msg_id)
	end
end

--����cmd�ص�
function on_ui_command( robot_obj, cmd_id, ...)
    if cmd_id >= CMD_ID_ROBOT_PROCESS_BASE and cmd_id<=CMD_ID_ROBOT_PROCESS_BASE+ROBOT_PROCESS_CUSTOM_COUNT then
        on_custom_task_case(robot_obj, cmd_id, ...)
        return true
    end
    -- ��ӡlog
	robot_obj.ex_log( robot_obj, "[--lua--] recv cmdid = ", cmd_id)
	-- ����message handler
	local cmd_handler = tb_cmd_msg_handler[cmd_id]
	if cmd_handler ~= nil then
		cmd_handler(robot_obj, cmd_id, ...)
	else
		robot_obj.ex_log( robot_obj, "[--lua--] not has ui cmd handler cmdid = ", cmd_id)
	end
    return true
end
-- for system config
function on_system_config( robot_obj, is_sync, is_param_random,param_interval_time,param_interval_addit,entry_ip,entry_port,login_type,prefix_account,prefix_name)
    ROBOT_MODE_TYPE_SYNC = is_sync
    ROBOT_MODE_TIME_RANDOM = is_param_random
    ROBOT_MODE_TIME_INTERVAL=param_interval_time
    ROBOT_MODE_TIME_INTERVAL_ADDIT = param_interval_addit

    ROBOT_ENTRY_IP = entry_ip
    ROBOT_ENTRY_PORT = entry_port
    ROBOT_LOGIN_TYPE = login_type

    ROBOT_PREFIX_ACCOUNT = prefix_account
    ROBOT_PREFIX_NAME    = prefix_name

    robot_obj.ex_log( robot_obj, "system config: mode_sync="..tostring(ROBOT_MODE_TYPE_SYNC)..",is_time_random="..tostring(ROBOT_MODE_TIME_RANDOM)..",interval_time="..tostring(ROBOT_MODE_TIME_INTERVAL)..",interval_addit="..tostring(ROBOT_MODE_TIME_INTERVAL_ADDIT) )
    robot_obj.ex_log( robot_obj, "system config: entry_ip="..tostring(ROBOT_ENTRY_IP)..",port="..tostring(ROBOT_ENTRY_PORT)..",login_type="..tostring(ROBOT_LOGIN_TYPE)..",prefix_account="..tostring(ROBOT_PREFIX_ACCOUNT)..",prefix_name="..tostring(ROBOT_PREFIX_NAME) )
end
----------------------------------------------------------------
-- �ӳٶ�ʱ������
----------------------------------------------------------------
--���Ϳͻ���ready��ʱ��
function entry_scene_timer( robot_obj, hb_name, elapse_time )
	--���볡��
	robot_obj.ex_client_ready(robot_obj)
	--�Ƴ���ʱ��
	robot_obj.ex_remove_hb(robot_obj,"entry_scene_timer")
end

--���Ϳͻ���ready��ʱ��
function reconnect_entry( robot_obj, hb_name, elapse_time )

	--��������
	robot_obj.ex_log( robot_obj, "[--lua--] reconnect entry.")
	robot_obj.ex_connect_entry( robot_obj )

	--�Ƴ���ʱ��
	robot_obj.ex_remove_hb(robot_obj,"reconnect_entry")
end

-- ��½��ʱ��ʱ��
function logon_timeout_timer( robot_obj, hb_name, elapse_time )
	-- ��ʱ�Ͽ�����
	if robot_obj.logoning == true then
		robot_obj.ex_disconnect( robot_obj);
	end
	
	--�Ƴ���ʱ��
	robot_obj.ex_remove_hb(robot_obj,"logon_timeout_timer")
end
