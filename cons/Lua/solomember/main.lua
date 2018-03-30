--�������������ؽű�

--����״̬
local WAPS_UNKNOWN = 0	--δ֪
local WAPS_STARTING = 1	--��������
local WAPS_READY = 2	--����������δ���ŷ���
local WAPS_OPENING = 3	--���ڿ��ŷ���
local WAPS_OPENED = 4	--�ѿ��ŷ���
local WAPS_CLOSING = 5	--���ڹر�
local WAPS_CLOSED = 6	--�ѹرշ���
local WAPS_QUITING = 7	--�����˳�

--��Դ��������
local USE_XML = 0   --ʹ��xml
local USE_INI = 1   --ʹ��ini

--��ں���
function main(member)
	nx_set_value("member", member)

	--������־ϵͳ
	member.UseQuickLog = true
	--Ӧ�ý��̼�¼
	member.UseAppRecord = true

	--�¼�ѭ����ʱʱ��
	member.EventLoopExceedTime = 2000
	member.CompletionStatusExceedTime = 2000

	--�ӱ��ظ�������ǿ��ˢ�´���npc
	member.ForceRefreshVisual = false

	--��Ļ����ַ���ɫ
	member:SetConsoleColor("Gray")

	--��ȡ��������
	local ini = nx_create("IniDocument")

	--�����ļ���
	--ini.FileName = "game_member.ini"
	ini.FileName = nx_main_config()

	--��������
	if not ini:LoadFromFile() then
		nx_msgbox("can't read config file!")
		nx_destroy(ini)
		return 0
	end

	--��CPU����
	local bind_core = ini:ReadInteger("Main", "BindCore", -1)
	--��Ϸ�������
	local district_id = ini:ReadInteger("Main", "DistrictID", 1)
	--��Ϸ���������
	local server_id = ini:ReadInteger("Main", "ServerID", 1)
	--�������������
	local member_id = ini:ReadInteger("Main", "MemberID", 0)
	--������ײ��Ϣ�ļ��ĸ�Ŀ¼
	local collide_root_path = ini:ReadString("Main", "CollideRootPath", "")
	--���ִ����Ƿ���ʾ
	local debug_halt = nx_boolean(ini:ReadString("Main", "DebugHalt", "true"))
  --WSASend������������ĸ���(���16��,Ĭ����8)
	local wsasend_arrays = ini:ReadInteger("Main", "WSASendArrays", 8)
	--WSASend�������ͻ����С(���64M,Ĭ����1M)
	local wsasend_buflen = ini:ReadInteger("Main", "WSASendBufLen", 1048576)
	if collide_root_path ~= "" then
		collide_root_path = nx_function("ext_get_full_path", collide_root_path)
	end

	--������ַ
	local inner_addr = "127.0.0.1"
	local inner_port = 0
	--local inner_port = 16300 + member_id

	inner_addr = ini:ReadString("Inner", "Addr", inner_addr)
	inner_port = ini:ReadInteger("Inner", "Port", inner_port)

	--�㲥��ַ
	local broad_addr = "127.255.255.255"
	local broad_port = 15000

	broad_addr = ini:ReadString("Broadcast", "Addr", broad_addr)
	broad_port = ini:ReadInteger("Broadcast", "Port", broad_port)

	--ά�����ӵ�ַ
	local echo_addr = "127.0.0.1"
	local echo_port = 15300 + member_id

	echo_addr = ini:ReadString("Echo", "Addr", echo_addr)
	echo_port = ini:ReadInteger("Echo", "Port", echo_port)

	--�߼�ģ��
	local logic_loader = nx_create("LogicLoader")

	local logic_path = ini:ReadString("Logic", "Path", "")
	local modules = ini:ReadInteger("Logic", "Modules", 0)

	--sns
	local use_sns = nx_boolean(ini:ReadString("Main", "UseSns", "false"))
	local sns_server_count = ini:ReadInteger("Main", "SnsServerCount", 0)

	--�Զ�����Ϣ�ظ�
	local custom_ack = nx_boolean(ini:ReadString("Main", "CustomAck", "false"))
	--�߼�ģ��
	local logic_loader = nx_create("LogicLoader")

	local logic_path = ini:ReadString("Logic", "Path", "")
	local modules = ini:ReadInteger("Logic", "Modules", 0)

	if logic_path ~= "" and modules == 0 then
		local fs = nx_create("FileSearch")

		fs:SearchFile(nx_work_path() .. logic_path .. "\\", "*.dll")

		local dll_table = fs:GetFileList()

		for i = 1, table.getn(dll_table) do
			if logic_loader:Load(nx_work_path() .. logic_path .. "\\", dll_table[i]) then
				member:WriteLine("Load loigc module " .. dll_table[i])
			else
				member:WriteLine("Load loigc module " .. dll_table[i] .. " failed")
			end
		end

		nx_destroy(fs)
	else
		for i = 0, modules do
			local dll_name = ini:ReadString("Logic", "Module" .. nx_string(i), "")

			if dll_name ~= "" then
				if logic_loader:Load(nx_work_path() .. logic_path, dll_name) then
					member:WriteLine("Load loigc module " .. dll_name)
				else
					member:WriteLine("Load loigc module " .. dll_name .. " failed")
				end
			end
		end
	end

	nx_destroy(ini)

	--������ײ��Ϣ
	local collide_manager = nx_create("CollideManager")

	--�󶨽ű�
	nx_bind_script(member, "main", "echo_init")

	member.AppName = "member_" .. nx_string(server_id) .. "_" .. nx_string(member_id)
	member.BindCore = bind_core

	--������
	--member.FastScanTime = 200

	member.DistrictId = district_id
  member.WSASendArrays = wsasend_arrays
  member.WSASendBufLen = wsasend_buflen
	member.ServerId = server_id
	member.MemberId = member_id
	member.LogicLoader = logic_loader
	member.CollideManager = collide_manager
	member.ScriptPath = nx_resource_path() .. "lua\\"
	member.DebugHalt = debug_halt
	member.CollideRootPath = collide_root_path
	member.InnerAddr = inner_addr
	member.InnerPort = inner_port
	member.OuterAddr = outer_addr
	member.OuterPort = outer_port
	member.BroadAddr = broad_addr
	member.BroadPort = broad_port
	member.EchoAddr = echo_addr
	member.EchoPort = echo_port
	--��֤������־����
	member.LogLauncher = "game_log game_log.ini daemon"
	--�Ƿ�����Ұ��Ϣ�Ż�
	--member.SightOptimize = true
	--�Ƿ񲻷���Ϊȱʡ��ֵ�Ķ�������
	member.HideEmptyProperty = false
	member.UseSns = use_sns

	--�ϲ���Ϣ
	member.MergeProperty = true
	member.MergeObjects = false

	--�Զ�����Ϣ�ظ�
	member.CustomAck = custom_ack

	--�����ָ��С
	member.SceneGridSize = 16
	--member.SlowScanTime = 5000
	--member.SceneTimerRandLimit = 300

	--��ʡ����
	member.ObjectPropertyFameUpdate = true
	member.DelayClientReadyResendView = true

	--����ʱˢ��
	member.DelayCreateFrame = 0

	member.AppearAtOnce = true

	member.UseConfigType = USE_XML

	--�������ֹ����ļ�
	member:LoadNameFilter(nx_resource_path() .. "filter.txt")
	--����ȫ�ֵ���Ϣ
	member:LoadLocations(nx_resource_path() .. "location.xml")

	--������������
	member:Start()

	--ʵ��ʹ�õ����������˿�
	inner_port = member.InnerPort

	--��ʾӦ�ó�����
	member:WriteLine("app name " .. member.AppName)
	--��ʾͨѶ��ַ�Ͷ˿�
	member:WriteLine("echo socket " .. echo_addr .. ":" .. nx_string(echo_port))
	member:WriteLine("inner socket " .. inner_addr .. ":" .. nx_string(inner_port))
	member:WriteLine("broadcast socket " .. broad_addr .. ":" .. nx_string(broad_port))
	--�������
	member:WriteLine("game server " .. nx_string(server_id)
		.. " member " .. nx_string(member_id) .. " started")

	--���ý��̿���״̬
	member:SetAppState(WAPS_OPENED);

	return 1
end

--�ر�
function close()
	local member = nx_value("member")

	--�رճ�������
	member:WriteLine("member server shutdown...")
	member:Stop()

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
	self:SendEchoLine("welcome server " .. nx_string(self.ServerId)
		.. " member " .. nx_string(self.MemberId) .. "\r")
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
	elseif command == "end" then
		--�ر�����
		self:CloseEcho()
	elseif command == "perf" then
		--�����߼�ģ������ͳ������
		self:DumpPerformance("perf" .. nx_string(self.MemberId) .. ".txt")
		echo_response(self, "dump logic module performance...")
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
