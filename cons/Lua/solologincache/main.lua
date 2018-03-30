--��¼������������ؽű�

--����״̬
local WAPS_UNKNOWN = 0	--δ֪
local WAPS_STARTING = 1	--��������
local WAPS_READY = 2	--����������δ���ŷ���
local WAPS_OPENING = 3	--���ڿ��ŷ���
local WAPS_OPENED = 4	--�ѿ��ŷ���
local WAPS_CLOSING = 5	--���ڹر�
local WAPS_CLOSED = 6	--�ѹرշ���

--��ں���
function main(login_cache)

	--��־
	local logger = nx_create("SoloLog")
	
	nx_set_value("login_cache", login_cache)
	
	--������־ϵͳ
	login_cache.UseQuickLog = true
	--Ӧ�ý��̼�¼
	login_cache.UseAppRecord = true
	
	--��Ļ����ַ���ɫ
	login_cache:SetConsoleColor("LightRed")
	
	--������־Ŀ¼
	if not nx_path_exists(nx_work_path() .. "log") then
		nx_path_create(nx_work_path() .. "log")
	end
	
	logger.UseThread = true
	--��־�ļ����Ŀ¼
	logger.LogFileDir = nx_work_path() .. "log\\"
	--�Ƿ�ÿ��һ����־�ļ�
	logger.EveryDayAlone = false
	
	logger.EveryHourLog = 1

	--��ȡ��������
	local ini = nx_create("IniDocument")
	
	--�����ļ���
	--ini.FileName = "game_logincache.ini"
	ini.FileName = nx_main_config()
	
	--��������
	if not ini:LoadFromFile() then
		nx_msgbox("can't read config file!")
		nx_destroy(ini)
		return 0
	end
	
	--���������
	local server_id = ini:ReadInteger("Main", "ServerID", 1)

	--WSASend������������ĸ���(���16��,Ĭ����8)
	local wsasend_arrays = ini:ReadInteger("Main", "WSASendArrays", 8)
	--WSASend�������ͻ����С(���64M,Ĭ����1M)
	local wsasend_buflen = ini:ReadInteger("Main", "WSASendBufLen", 1048576)
	
	--������ַ
	local outer_addr = ini:ReadString("Outer", "Addr", "127.0.0.1")
	local outer_port = ini:ReadInteger("Outer", "Port", 2001)
	
	--ά�����ӵ�ַ
	local echo_addr = "127.0.0.1"
	local echo_port = 17200 + server_id
	
	echo_addr = ini:ReadString("Echo", "Addr", echo_addr)
	echo_port = ini:ReadInteger("Echo", "Port", echo_port)

	nx_destroy(ini)
	
	--�󶨽ű�
	nx_bind_script(login_cache, "main", "echo_init")
	
	login_cache.AppName = "login_" .. nx_string(server_id)
	login_cache.ServerId = server_id
	login_cache.OuterAddr = outer_addr
	login_cache.OuterPort = outer_port
	login_cache.EchoAddr = echo_addr
	login_cache.EchoPort = echo_port
	
	--�����Ż�
	login_cache.WSASendArrays = wsasend_arrays
	login_cache.WSASendBufLen = wsasend_buflen
	
	--��¼������֤��ʽ
	login_cache.UseLoginStringCheck = false
	login_cache.UsePassWordCheck = false
	login_cache.UseDeviceUidCheck = true
	
	--��¼�˺���֤�ɹ���־
	login_cache.RecordQuerySucceed = true
	
	--��֤������־����
	--login_cache.LogLauncher = "game_log game_log.ini daemon"
	
	--������־����
	--logger:Start()
	
	--�����������
	login_cache:Start()
	
	--��ʾӦ�ó�����
	login_cache:WriteLine("app name " .. login_cache.AppName)
	--��ʾͨѶ��ַ�Ͷ˿�
	login_cache:WriteLine("echo socket " .. echo_addr .. ":" .. nx_string(echo_port))
	login_cache:WriteLine("outer socket " .. outer_addr .. ":" .. nx_string(outer_port))
	--�������
	login_cache:WriteLine("login cache server " .. nx_string(server_id) .. " login_cache ".." started")
	
	--���ý��̿���״̬
	login_cache:SetAppState(WAPS_OPENED);

	return 1
end

--�ر�
function close()
	local login_cache = nx_value("login_cache")
	
	--�رս������
	login_cache:WriteLine("login_cache server shutdown...")
	login_cache:Stop()
	
	--��������
	nx_quit()
	
	return true
end

--ά�����ӽű�
--���ͻ�Ӧ��Ϣ
function echo_response(self, info)
	self:SendEchoLine(">" .. info .. "\r")
	self:WriteLine(info)
	return true
end
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
	elseif command == "open" then
		self:SetAppState(WAPS_OPENED);
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
		self:SendEchoLine(">" .. "run entity " .. nx_string(entity) .. " method " .. nx_string(method) .. " return table" .. "\r")
		
		for i = 1, table.getn(res) do
			self:SendEchoLine(">" .. "result" .. nx_string(i) .. ": " .. nx_string(res[i]) .. "\r")
		end
	else
		self:SendEchoLine(">" .. "run entity " .. nx_string(entity) .. " method " .. nx_string(method) .. " return " .. nx_string(res) .. "\r")
	end
	
	return 1
end
