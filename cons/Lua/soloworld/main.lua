--��Ϸ���������ؽű�

--����״̬
local WAPS_UNKNOWN = 0	--δ֪
local WAPS_STARTING = 1	--��������
local WAPS_READY = 2	--����������δ���ŷ���
local WAPS_OPENING = 3	--���ڿ��ŷ���
local WAPS_OPENED = 4	--�ѿ��ŷ���
local WAPS_CLOSING = 5	--���ڹر�
local WAPS_CLOSED = 6	--�ѹرշ���
local WAPS_QUITING = 7	--�����˳�

local MONITOR_INDEX = 0			--�������
local WEBEXCHANGE_INDEX = 1		--���½�������
local DUMPER_INDEX = 2			--ת������������
local ESI_APPLE_ORDER_SERVER = 5   --ƻ���ڹ���ֵ������
local ESI_CHARGE_REWARD_SERVER = 7   --��ֵ����������
local ESI_GLOBAL_RANK_SERVER = 8 -- ȫ�����а�
local ESI_GMCC_SERVER	= 9		-- gmcc������

--�Ʒ�Ӧ�÷�������ַ�Ͷ˿�
local g_charge_addr = ""
local g_charge_port = 0

local NOT_BEAT_CHECK = 0      --��ʹ������
local NETWORK_LOST_CHECK = 1  --ʹ�õ��߼������
local MESSAGE_DELAY_CHECK =2  --ʹ�������ӳټ������

--��ں���
function main(world)
	nx_set_value("world", world)

	--������־ϵͳ
	world.UseQuickLog = true
	--Ӧ�ý��̼�¼
	world.UseAppRecord = true
	-- �򿪷������
	world.UseRoomService = false

	world.UseEncrypt = false

	--��Ļ����ַ���ɫ
	world:SetConsoleColor("LightGreen")
	--���Զ���
	world.OfflineContinue = true

	--�����Ƚϵ�ַ
	world.ReconnectCompareAddr = false

	--����豸Ψһid
	world.UseDevicedIdCheck = false

	--��ȡ��������
	local ini = nx_create("IniDocument")

	--�����ļ���
	--ini.FileName = "game_world.ini"
	ini.FileName = nx_main_config()

	--��������
	if not ini:LoadFromFile() then
		nx_msgbox("can't read config file!")
		nx_destroy(ini)
		return 0
	end

	--��Ϸ�������
	local district_id = ini:ReadInteger("Main", "DistrictID", 1)
	--���������
	local server_id = ini:ReadInteger("Main", "ServerID", 1)
	--����������
	local server_name = ini:ReadString("Main", "ServerName", "")
	--���������
	local max_players = ini:ReadInteger("Main", "MaxPlayers", 8192)
	--����������ݵļ��������
	local save_minutes = ini:ReadInteger("Main", "SaveMinutes", 20)
	--ˢ���˻���Ϣ�ļ��������
	local flush_minutes = ini:ReadInteger("Main", "FlushMinutes", 0)
  --�����߼�DLL�ļ�
	--local world_logic_dll = ini:ReadString("Main", "WorldLogicDll", "")
	--�Ƿ��Ŷ�
	local player_queue = nx_boolean(ini:ReadString("Main", "PlayerQueue", "false"))
	--�Ƿ�Ҫ����ԭʼ���ʺŵ����루�����ܣ�
	local original_pwd = nx_boolean(ini:ReadString("Main", "OriginalPassword", "false"))
	--�Ƿ�����ɾ������Ľ�ɫ��������
	local block_can_del = nx_boolean(ini:ReadString("Main", "BlockCanDelete", "false"))
	--�Ƿ�ֶ˼��
	local ostype_check = nx_boolean(ini:ReadString("Main", "UseOsTypeCheck", "false"))
	--��������
	local encrypt_factor = ini:ReadString("Main", "EncryptFactor", "")
	--�Ƿ�Ҫ��ȫ���Ľ�ɫ��
	local check_chinese_char_set = nx_boolean(ini:ReadString("Main", "CheckChineseCharSet", "false"))
	--WSASend������������ĸ���(���16��,Ĭ����8)
	local wsasend_arrays = ini:ReadInteger("Main", "WSASendArrays", 8)
	--WSASend�������ͻ����С(���64M,Ĭ����1M)
	local wsasend_buflen = ini:ReadInteger("Main", "WSASendBufLen", 1048576)

	--sns
	local use_sns = nx_boolean(ini:ReadString("Main", "UseSns", "false"))
	local sns_server_count = ini:ReadInteger("Main", "SnsServerCount", 0)

	--������ַ
	local inner_addr = "127.0.0.1"
	local inner_port = 0
	--local inner_port = 16001

	inner_addr = ini:ReadString("Inner", "Addr", inner_addr)
	inner_port = ini:ReadInteger("Inner", "Port", inner_port)

	--�㲥��ַ
	local broad_addr = "127.255.255.255"
	local broad_port = 15000

	broad_addr = ini:ReadString("Broadcast", "Addr", broad_addr)
	broad_port = ini:ReadInteger("Broadcast", "Port", broad_port)

	--ά�����ӵ�ַ
	local echo_addr = "127.0.0.1"
	local echo_port = 15001

	echo_addr = ini:ReadString("Echo", "Addr", echo_addr)
	echo_port = ini:ReadInteger("Echo", "Port", echo_port)

	--�Ʒѷ�����
	local charge_inuse = nx_boolean(ini:ReadString("Charge", "Inuse", ""))
	local charge_addr = ini:ReadString("Charge", "Addr", "")
	local charge_port = ini:ReadInteger("Charge", "Port", 0)

	g_charge_addr = charge_addr
	g_charge_port = charge_port

	--�ͷ�������
	local gmcc_inuse = nx_boolean(ini:ReadString("Gmcc", "Inuse", ""))
	local gmcc_addr = ini:ReadString("Gmcc", "Addr", "")
	local gmcc_port = ini:ReadInteger("Gmcc", "Port", 0)
	
	-- �ͻ���չ������( )
	local gmcc_extra_inuse = nx_boolean(ini:ReadString("GmccExtra", "Inuse", ""))
	local gmcc_extra_addr = ini:ReadString("GmccExtra", "Addr", "")
	local gmcc_extra_port = ini:ReadInteger("GmccExtra", "Port", 0)	

	--���������
	local manage_inuse = nx_boolean(ini:ReadString("ManageServer", "Inuse", ""))
	local manage_addr = ini:ReadString("ManageServer", "Addr", "")
	local manage_port = ini:ReadInteger("ManageServer", "Port", 0)

	--ת��������
	local dumpserver_inuse = nx_boolean(ini:ReadString("DumpServer", "Inuse", ""))
	local dumpserver_addr = ini:ReadString("DumpServer", "Addr", "")
	local dumpserver_port = ini:ReadInteger("DumpServer", "Port", 0)

	--ƻ���ڹ�
	local appleorder_inuse = nx_boolean(ini:ReadString("AppleOrder", "Inuse", ""))
	local appleorder_addr = ini:ReadString("AppleOrder", "Addr", "")
	local appleorder_port = ini:ReadInteger("AppleOrder", "Port", 0)

	--��ֵ����
	local chargereward_inuse = nx_boolean(ini:ReadString("ChargeReward", "Inuse", ""))
	local chargereward_addr = ini:ReadString("ChargeReward", "Addr", "")
	local chargereward_port = ini:ReadInteger("ChargeReward", "Port", 0)

	--����
	local voice_inuse = nx_boolean(ini:ReadString("Voice", "Inuse", ""))
	local voice_addr = ini:ReadString("Voice", "Addr", "")
	local voice_port = ini:ReadInteger("Voice", "Port", 0)

	--�����߼�ģ��
	--local world_logic_loader = nx_create("LogicLoader")

	--if world_logic_dll ~= "" then
	--	if world_logic_loader:Load(nx_work_path(), world_logic_dll) then
	--		world:WriteLine("load loigc module " .. world_logic_dll)
	--	else
	--		world:WriteLine("load loigc module " .. world_logic_dll .. " failed")
	--	end
	--end

	--ƽ̨������
	local platform_inuse = nx_boolean(ini:ReadString("Platform", "Inuse", ""))
	local platform_addr = ini:ReadString("Platform", "Addr", "")
	local platform_port = ini:ReadInteger("Platform", "Port", 0)

	--ȫ�����а�
	local globalrank_inuse = nx_boolean(ini:ReadString("GlobalRank", "Inuse", ""))
	local globalrank_addr = ini:ReadString("GlobalRank", "Addr", "")
	local globalrank_port = ini:ReadInteger("GlobalRank", "Port", 0)

  --��¼���������
  local login_cache_inuse = nx_boolean(ini:ReadString("LoginCache", "Inuse", "false"))
	local login_cache_addr = ini:ReadString("LoginCache", "Addr", "")
	local login_cache_port = ini:ReadInteger("LoginCache", "Port", 0)
	nx_destroy(ini)

	--�󶨽ű�
	nx_bind_script(world, "main", "echo_init")

	world.AppName = "world_" .. nx_string(server_id)
	world.DistrictId = district_id
	world.WSASendArrays = wsasend_arrays
	world.WSASendBufLen = wsasend_buflen
	world.ServerId = server_id
	--world.LogicLoader = world_logic_loader
	world.ServerName = nx_widestr(server_name)
	world.MaxPlayers = max_players
	world.SaveMinutes = save_minutes
	world.FlushMinutes = flush_minutes
	world.OriginalPassword = original_pwd
	world.BlockCanDelete = block_can_del
	world.EncryptFactor = encrypt_factor
	--Vip��Ȩ������Ϸ��DenyEntryScene��Ӧ,0:Ϊ������,1:Ϊ����
	world.VipEntryScene = 0
	--ÿ���ʺſ��Դ����Ľ�ɫ����
	world.AccountRoles = 4
	world.CheckChineseCharSet = check_chinese_char_set
	world.InnerAddr = inner_addr
	world.InnerPort = inner_port
	world.BroadAddr = broad_addr
	world.BroadPort = broad_port
	world.EchoAddr = echo_addr
	world.EchoPort = echo_port
	world.UseSns = use_sns
	world.PlayerQueue = player_queue
	world.UseOsTypeCheck = ostype_check

	--�������
	world.CheckInterval = 45

	--��֤������־����
	world.LogLauncher = "game_log game_log.ini daemon"

	  --��¼����
	if login_cache_inuse then
		world:SetLoginCacheAddress(login_cache_addr, login_cache_port)
	end
	--�������ֹ����ļ�
	world:LoadNameFilter(nx_resource_path() .. "filter.txt")

	--�����ͨ����
	load_scene_config(world)

	--�Ʒ�����
	if charge_inuse then
		world:SetChargeAddress(charge_addr, charge_port)
	end

	--�ͷ�����
	if gmcc_inuse then
		world:SetGmccAddress(gmcc_addr, gmcc_port)
	end
	
	--�ͻ���չ����
	if gmcc_extra_inuse then
		world:SetExtraAddress(ESI_GMCC_SERVER, gmcc_extra_addr, gmcc_extra_port)
	end

	--��������
	if manage_inuse then
		world:SetManageAddress(manage_addr, manage_port)
	end

	--ת������
	if dumpserver_inuse then
	    world:SetExtraAddress(DUMPER_INDEX, dumpserver_addr, dumpserver_port)
	end

	-- ƻ���ڹ�
	if appleorder_inuse then
		world:SetExtraAddress(ESI_APPLE_ORDER_SERVER, appleorder_addr, appleorder_port)
   	end

	-- ��ֵ����
	if chargereward_inuse then
		world:SetExtraAddress(ESI_CHARGE_REWARD_SERVER, chargereward_addr, chargereward_port)
	end


	--ƽ̨����
	if platform_inuse then
		world.UseRoomService = true
		world:SetPlatformAddress(platform_addr, platform_port)
	end

	--ȫ�����а�
	if globalrank_inuse then
	    world:SetExtraAddress(ESI_GLOBAL_RANK_SERVER, globalrank_addr, globalrank_port)
	end

	--������Ϸ����
	world:Start()

	--ʵ��ʹ�õ����������˿�
	inner_port = world.InnerPort

	--��ʾӦ�ó�����
	world:WriteLine("app name " .. world.AppName)
	--��ʾͨѶ��ַ�Ͷ˿�
	world:WriteLine("echo socket " .. echo_addr .. ":" .. nx_string(echo_port))
	world:WriteLine("inner socket " .. inner_addr .. ":" .. nx_string(inner_port))
	world:WriteLine("broadcast socket " .. broad_addr .. ":" .. nx_string(broad_port))

	if charge_inuse then
		world:WriteLine("charge socket " .. charge_addr .. ":" .. nx_string(charge_port))
	end

	if gmcc_inuse then
		world:WriteLine("gmcc socket " .. gmcc_addr .. ":" .. nx_string(gmcc_port))
	end
	
	if gmcc_extra_inuse then
		world:WriteLine("gmcc extra socket " .. gmcc_extra_addr .. ":" .. nx_string(gmcc_extra_port))
	end

	if manage_inuse then
		world:WriteLine("manage socket " .. manage_addr .. ":" .. nx_string(manage_port))
	end

	--�������
	world:WriteLine("game server " .. nx_string(server_id) .. " starting...")

	--���ý��̿���״̬
	world:SetAppState(WAPS_OPENED);

	return 1
end

--���س��������ļ�
function load_scene_config(world)
	local ini = nx_create("IniDocument")

	--���������ļ���
	ini.FileName = nx_resource_path() .. "scenes.ini"

	if not ini:LoadFromFile() then
		nx_destroy(ini)
		return false;
	end

	local sect_table = ini:GetSectionList()

	for i = 1, table.getn(sect_table) do
		local sect = sect_table[i]

		--�������
		local scene_id = nx_number(sect)
		--���ڳ������������
		local member_id = ini:ReadInteger(sect, "Member", 0)
		--�߼�����
		local script = ini:ReadString(sect, "Script", "scene")
		--����������
		local config = ini:ReadString(sect, "Config", "")
		--�Ƿ��Ǹ�������ԭ��
		local clonable = nx_boolean(ini:ReadInteger(sect, "Clonable", 0))

		if clonable then
			--����ԭ�ͳ���
			world:AddPrototypeScene(scene_id, member_id, script, config)
		else
			--��ͨ����
			world:AddScene(scene_id, member_id, script, config)
		end
	end

	nx_destroy(ini)

	return true
end

--�ر�
function close()
	local world = nx_value("world")

	if world.State == "Closed" then
		force_close()
	else
		--����ر�״̬���Զ��˳�����
		world.QuitOnClosed = true
		--�����رշ���������
		world:CloseServer()
		world:WriteLine("waiting server close...")
	end

	return true
end

--ǿ�ƹر�
function force_close()
	local world = nx_value("world")

	--�ر���Ϸ����
	world:WriteLine("world server shutdown...")
	world:Stop()

	--��������
	nx_quit()

	return true
end

--ά�����ӽű�

--��ʼ��
function echo_init(self)
	nx_callback(self, "on_open", "echo_open")
	nx_callback(self, "on_close", "echo_close")
	nx_callback(self, "on_input", "echo_input")
	nx_callback(self, "on_input_rc", "echo_input_rc")
	nx_callback(self, "on_input_getp", "echo_input_getp")
	nx_callback(self, "on_input_setp", "echo_input_setp")
	nx_callback(self, "on_input_getc", "echo_input_getc")
	nx_callback(self, "on_input_setc", "echo_input_setc")
	nx_callback(self, "on_input_method", "echo_input_method")
	nx_callback(self, "on_input_listp", "echo_input_listp")
	nx_callback(self, "on_input_listc", "echo_input_listc")
	nx_callback(self, "on_input_listm", "echo_input_listm")
	nx_callback(self, "on_input_listg", "echo_input_listg")
	nx_callback(self, "on_input_function", "echo_input_function")
	nx_callback(self, "on_input_debugger", "echo_input_debugger")

	return 1
end

--ά�����Ӵ�
function echo_open(self, addr, port)
	self:WriteLine("echo client " .. nx_string(addr)
		.. ":" .. nx_string(port) .. " connected")
	self:SendEchoLine("welcome server " .. nx_string(self.ServerId) .. "\r")
	return 1
end

--ά�����ӹر�
function echo_close(self, addr, port)
	self:WriteLine("echo client " .. nx_string(addr)
		.. ":" .. nx_string(port) .. " closed")
	return 1
end

--����ά����������
function echo_input(self, command)
	if command == "quit" then
		--�˳�����
		echo_response(self, "server will shutdown...")
		close()
	elseif command == "fquit" then
		--ǿ���˳�
		echo_response(self, "server will shutdown...")
		force_close()
	elseif command == "end" then
		--�ر�����
		self:CloseEcho()
	elseif command == "players" then
		--������������
		echo_response(self, "player: " .. nx_string(self.PlayerCount)
			.. ", online: " .. nx_string(self.OnlineCount)
			.. ", offline: " .. nx_string(self.OfflineCount)
			.. ", active: " .. nx_string(self.ActiveCount)
			.. ", queuing: " .. nx_string(self.QueueCount)
			.. ", vip_queuing: " .. nx_string(self.VipQueueCount))
	elseif command == "open" then
		--��������
		if self.State == "Opened" then
			echo_response(self, "game server already opened")
			return 0
		end

		if self.State ~= "CanOpen" then
			echo_response(self, "game server not ready, can't open")
			return 0
		end

		if self:OpenServer() then
			echo_response(self, "send open command to game server")
		end
	elseif command == "close" then
		--�رշ���
		if self.State ~= "Opened" then
			echo_response(self, "game server not open")
			return 0
		end

		if self:CloseServer() then
			echo_response(self, "send close command to game server")
		end
	elseif command == "status" then
		--��ǰ����״̬
		echo_response(self, "game server state is " .. self.State)
	elseif command == "perf" then
		--�����߼�ģ������ͳ������
		self:DumpPerformance("")
		echo_response(self, "dump logic module performance...")
	elseif command == "rc" then
		nx_callback(self, "on_input_rc", "echo_input_rc")
	elseif command == "quit_not_platform" then
		--�˳�����
		echo_response(self, "server will shutdown, but not close platform...")
		local world = nx_value("world")
		world.UseRoomService = false
		close()

	else
		echo_response(self, "undefined command: " .. command)
	end

	return 1
end

--���ͻ�Ӧ��Ϣ
function echo_response(self, info)
	self:SendEchoLine(">" .. info .. "\r")
	self:WriteLine(info)
	return true
end

--����ʵ��
function get_entity(entity_name)
	local id = nx_value(entity_name)

	if id ~= nil then
		return id
	end

	id = nx_lookup(entity_name)

	if not nx_is_null(id) then
		return id
	end

	id = nx_object(entity_name)

	if nx_is_valid(id) then
		return id
	end

	return nil
end

--ת�������ʵ�����
function get_new_value(old_v, value)
	local type = nx_type(old_v)

	if type == "boolean" then
		return nx_boolean(value)
	elseif type == "number" then
		return nx_number(value)
	elseif type == "string" then
		return nx_string(value)
	elseif type == "widestr" then
		return nx_widestr(value)
	elseif type == "int" then
		return nx_int(value)
	elseif type == "int64" then
		return nx_int64(value)
	elseif type == "float" then
		return nx_float(value)
	elseif type == "double" then
		return nx_double(value)
	elseif type == "object" then
		return nx_object(value)
	end

	return nil
end

--��ѯʵ���ڲ�����
function echo_input_getp(self, entity, prop)
	if entity == nil or prop == nil then
		return 0
	end

	local obj = get_entity(nx_string(entity))

	if obj == nil then
		return 0
	end

	local value = nx_property(obj, nx_string(prop))

	echo_response(self, "entity " .. nx_name(obj) .. " property "
		.. nx_string(prop) .. " value is " .. nx_string(value))

	return 1
end

--����ʵ���ڲ�����
function echo_input_setp(self, entity, prop, value)
	if entity == nil or prop == nil then
		return 0
	end

	local obj = get_entity(nx_string(entity))

	if obj == nil then
		return 0
	end

	local old_v = nx_property(obj, nx_string(prop))
	local new_v = get_new_value(old_v, value)

	if not nx_set_property(obj, nx_string(prop), new_v) then
		echo_response(self, "set property failed")
		return 0
	end

	new_v = nx_property(obj, nx_string(prop))
	echo_response(self, "set entity " .. nx_name(obj) .. " property "
		.. nx_string(prop) .. " value " .. nx_string(new_v))

	return 1
end

--��ѯʵ���Զ�������
function echo_input_getc(self, entity, prop)
	if entity == nil or prop == nil then
		return 0
	end

	local obj = get_entity(nx_string(entity))

	if obj == nil then
		return 0
	end

	local value = nx_custom(obj, nx_string(prop))

	echo_response(self, "entity " .. nx_name(obj) .. " custom "
		.. nx_string(prop) .. " value is " .. nx_string(value))

	return 1
end

--����ʵ���Զ�������
function echo_input_setc(self, entity, prop, value)
	if entity == nil or prop == nil then
		return 0
	end

	local obj = get_entity(nx_string(entity))

	if obj == nil then
		return 0
	end

	local old_v = nx_custom(obj, nx_string(prop))
	local new_v = get_new_value(old_v, value)

	if not nx_set_custom(obj, nx_string(prop), new_v) then
		echo_response(self, "set custom failed")
		return 0
	end

	new_v = nx_custom(obj, nx_string(prop))
	echo_response(self, "set entity " .. nx_name(obj) .. " custom "
		.. nx_string(prop) .. " value " .. nx_string(new_v))

	return 1
end

--ִ��ʵ�巽��
function echo_input_method(self, entity, method, ...)
	if entity == nil or method == nil then
		return 0
	end

	local obj = get_entity(nx_string(entity))

	if obj == nil then
		return 0
	end

	local res = nx_method(obj, nx_string(method), unpack(arg))

	if type(res) == "table" then
		echo_response(self, "run entity " .. nx_name(obj) .. " method "
			.. nx_string(method) .. " return table")

		for i = 1, table.getn(res) do
			echo_response(self, "table value " .. nx_string(i) .. ": " .. nx_string(res[i]))
		end
	else
		echo_response(self, "run entity " .. nx_name(obj) .. " method "
			.. nx_string(method) .. " return " .. nx_string(res))
	end

	return 1
end

--�г�ʵ��������ڲ�����
function echo_input_listp(self, entity)
	if entity == nil then
		return 0
	end

	local obj = get_entity(nx_string(entity))

	if obj == nil then
		return 0
	end

	local prop_table = nx_property_list(obj)
	local num = table.getn(prop_table)

	echo_response(self, "entity " .. nx_name(obj) .. " property number is " .. nx_string(num))

	for i = 1, num do
		echo_response(self, nx_string(prop_table[i]))
	end

	return 1
end

--�г�ʵ��������Զ�������
function echo_input_listc(self, entity)
	if entity == nil then
		return 0
	end

	local obj = get_entity(nx_string(entity))

	if obj == nil then
		return 0
	end

	local prop_table = nx_custom_list(obj)
	local num = table.getn(prop_table)

	echo_response(self, "entity " .. nx_name(obj) .. " custom number is " .. nx_string(num))

	for i = 1, num do
		echo_response(self, nx_string(prop_table[i]))
	end

	return 1
end

--�г�ʵ������з���
function echo_input_listm(self, entity)
	if entity == nil then
		return 0
	end

	local obj = get_entity(nx_string(entity))

	if obj == nil then
		return 0
	end

	local prop_table = nx_method_list(obj)
	local num = table.getn(prop_table)

	echo_response(self, "entity " .. nx_name(obj) .. " method number is " .. nx_string(num))

	for i = 1, num do
		echo_response(self, nx_string(prop_table[i]))
	end

	return 1
end

--�г�����ȫ�ֱ���
function echo_input_listg(self)
	local global_table = nx_value_list()
	local num = table.getn(global_table)

	echo_response(self, "global number is " .. nx_string(num))

	for i = 1, num do
		local name = global_table[i]
		local value = nx_value(name)

		echo_response(self, nx_string(name) .. "[" .. nx_type(value) .. "]: " .. nx_string(value))
	end

	return 1
end

--ִ�нű���չ����
function echo_input_function(self, func, ...)
	if func == nil or func == "" then
		return 0
	end

	local res = nx_function(func, unpack(arg))

	if type(res) == "table" then
		echo_response(self, "run function " .. nx_string(func) .. " return table")

		for i = 1, table.getn(res) do
			echo_response(self, "result" .. nx_string(i) .. ": " .. nx_string(res[i]))
		end
	else
		echo_response(self, "run function " .. nx_string(func) .. " return " .. nx_string(res))
	end

	return 1
end


--���Ժ���
function echo_input_debugger(self, entity, method, ...)
	if entity == nil or entity == "" or method == nil or method == "" then
		return 0
	end

	local obj = get_entity(nx_string(entity))

	if obj == nil then
		return 0
	end

	local res = nx_method(obj, nx_string(method), unpack(arg))

	if type(res) == "table" then
		--echo_response(self, "run entity " .. nx_string(entity) .. " method " .. nx_string(method) .. " return table")
			self:SendEchoLine(">" .. "run entity " .. nx_string(entity) .. " method " .. nx_string(method) .. " return table" .. "\r")

		for i = 1, table.getn(res) do
			--echo_response(self, "result" .. nx_string(i) .. ": " .. nx_string(res[i]))
				self:SendEchoLine(">" .. "result" .. nx_string(i) .. ": " .. nx_string(res[i]) .. "\r")
		end
	else
		--echo_response(self, "run entity " .. nx_string(entity) .. " method " .. nx_string(method) .. " return " .. nx_string(res))
		self:SendEchoLine(">" .. "run entity " .. nx_string(entity) .. " method " .. nx_string(method) .. " return " .. nx_string(res) .. "\r")
	end

	return 1
end
function echo_input_rc(self, addr, port)
	if addr == nil or port == nil then
		--��ʾ��ǰ��ַ
		echo_response(self, "current charge address is "
			.. nx_string(g_charge_addr) .. ":" .. nx_string(g_charge_port))
		return 1
	end

	local world = nx_value("world")

	if world:ReconnectCharge(addr, port) then
		--��¼�µĵ�ַ
		g_charge_addr = addr
		g_charge_port = port
		echo_response(self, "reconnect charge succeed")
	else
		echo_response(self, "reconnect charge failed")
	end

	return 1
end
