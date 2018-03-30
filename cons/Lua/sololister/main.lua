--�б���������ؽű�

--����״̬
local WAPS_UNKNOWN = 0	--δ֪
local WAPS_STARTING = 1	--��������
local WAPS_READY = 2	--����������δ���ŷ���
local WAPS_OPENING = 3	--���ڿ��ŷ���
local WAPS_OPENED = 4	--�ѿ��ŷ���
local WAPS_CLOSING = 5	--���ڹر�
local WAPS_CLOSED = 6	--�ѹرշ���
local WAPS_QUITING = 7	--�����˳�

--�汾���趨 2015.12.26
local client_version = 3;

--��ں���
function main(lister)
	nx_set_value("lister", lister)

	--������־ϵͳ
	lister.UseQuickLog = true
	--Ӧ�ý��̼�¼
	lister.UseAppRecord = true

	--��Ļ����ַ���ɫ
	lister:SetConsoleColor("LightCyan")

	--��ȡ��������
	local ini = nx_create("IniDocument")

	--�����ļ���
	--ini.FileName = "game_lister.ini"
	ini.FileName = nx_main_config()

	--��������
	if not ini:LoadFromFile() then
		nx_msgbox("can't read config file!")
		nx_destroy(ini)
		return 0
	end

	--IDC���
	local idc = ini:ReadInteger("Main", "IDC", 1)
	--��Ϸ�������
	local district_id = ini:ReadInteger("Main", "DistrictID", 1)
	--�б���������
	local lister_id = ini:ReadInteger("Main", "ListerID", 0)
	--���������
	local max_connects = ini:ReadInteger("Main", "MaxConnects", 1024)
	--��С�ͻ��˰汾
	--local client_version = ini:ReadInteger("Main", "ClientVersion", 0)

	--������ַ
	local outer_addr = ini:ReadString("Outer", "Addr", "127.0.0.1")
	local outer_port = ini:ReadInteger("Outer", "Port", 2001)

	--������ַ
	local inner_addr = "127.0.0.1"
	local inner_port = 16004

	inner_addr = ini:ReadString("Inner", "Addr", inner_addr)
	inner_port = ini:ReadInteger("Inner", "Port", inner_port)

	--�㲥��ַ
	local broad_addr = "127.255.255.255"
	local broad_port = 15000

	broad_addr = ini:ReadString("Broadcast", "Addr", broad_addr)
	broad_port = ini:ReadInteger("Broadcast", "Port", broad_port)

	--ά�����ӵ�ַ
	local echo_addr = "127.0.0.1"
	local echo_port = 15400 + lister_id

	echo_addr = ini:ReadString("Echo", "Addr", echo_addr)
	echo_port = ini:ReadInteger("Echo", "Port", echo_port)

	nx_destroy(ini)

	--�󶨽ű�
	nx_bind_script(lister, "main", "echo_init")

	--������
	lister.AppName = "lister_" .. nx_string(lister_id)
	--�����б����
	lister.IDC = idc
	lister.DistrictId = district_id
	lister.ListerId = lister_id
	lister.MaxConnects = max_connects
	lister.ClientVersion = client_version
	--ȱʡΪ�������κ�IP
	lister.BlockOrAllow = -1
	lister.InnerAddr = inner_addr
	lister.InnerPort = inner_port
	lister.OuterAddr = outer_addr
	lister.OuterPort = outer_port
	lister.BroadAddr = broad_addr
	lister.BroadPort = broad_port
	lister.EchoAddr = echo_addr
	lister.EchoPort = echo_port
	--��֤������־����
	lister.LogLauncher = "game_log game_log.ini daemon"

	--�����б����
	lister:Start()

	--��ʾӦ�ó�����
	lister:WriteLine("app name " .. lister.AppName)
	--��ʾͨѶ��ַ�Ͷ˿�
	lister:WriteLine("echo socket " .. echo_addr .. ":" .. nx_string(echo_port))
	lister:WriteLine("inner socket " .. inner_addr .. ":" .. nx_string(inner_port))
	lister:WriteLine("broadcast socket " .. broad_addr .. ":" .. nx_string(broad_port))
	lister:WriteLine("outer socket " .. outer_addr .. ":" .. nx_string(outer_port))

	--�������
	lister:WriteLine("game lister server district " .. nx_string(district_id) .. " id "
		.. nx_string(lister_id) .. " started")

	--���ý��̿���״̬
	lister:SetAppState(WAPS_OPENED)

	--���ز���IP������
	load_ip(lister)

	return 1
end

--���ز���IP������
function load_ip(lister)
	if not nx_file_exists("test_ip.txt") then
		nx_log("loat test ip failed")
		return false
	end

	lister:ClearBlockIP()

	for line in io.lines(nx_work_path() .. "test_ip.txt") do
		--���ҷָ���
		local pos = string.find(line, ",")

		if pos == nil then
			pos = string.find(line, "\t")

			if pos == nil then
				pos = string.find(line, " ")
			end
		end

		if pos ~= nil then
			local ip_beg = string.sub(line, 1, pos - 1)
			local ip_end = string.sub(line, pos + 1)

			if lister:AddBlockIP(ip_beg, ip_end) then
				nx_log("add test ip: " .. ip_beg .. " to " .. ip_end)
			else
				nx_log("add test ip failed: " .. line)
			end
		end
	end

	return true
end

--�ر�
function close()
	local lister = nx_value("lister")

	--�ر��б����
	lister:WriteLine("lister server shutdown...")
	lister:Stop()

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
	self:SendEchoLine("welcome lister server district " .. nx_string(self.DistrictId)
		.. " id " .. nx_string(self.ListerId) .. "\r")
	return 1
end

--ά�����ӹر�
function echo_close(self, addr, port)
	self:WriteLine("echo client " .. nx_string(addr)
		.. ":" .. nx_string(port) .. " closed")
	return 1
end

--���ͻ�Ӧ��Ϣ
function echo_response(self, info)
	self:SendEchoLine(">" .. info .. "\r")
	self:WriteLine(info)
	return true
end

--����ά����������
function echo_input(self, command)
	if command == "quit" then
		--�˳�����
		echo_response(self, "server will shutdown...")
		close()
	elseif command == "test" then
		--�������״̬��ֻ����ָ���Ĳ���IP��¼��Ϸ
		self.BlockOrAllow = 1
		echo_response(self, "server entry test status...")
	elseif command == "open" then
		--�뿪����״̬�������κ�IP��¼��Ϸ
		self.BlockOrAllow = -1
		echo_response(self, "server leave test status...")
	elseif command == "end" then
		--�ر�����
		self:CloseEcho()
	else
		echo_response(self, "undefined command: " .. command)
	end

	return 1
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
