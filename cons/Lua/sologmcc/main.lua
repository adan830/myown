--������������ؽű�

--����״̬
local WAPS_UNKNOWN = 0	--δ֪
local WAPS_STARTING = 1	--��������
local WAPS_READY = 2	--����������δ���ŷ���
local WAPS_OPENING = 3	--���ڿ��ŷ���
local WAPS_OPENED = 4	--�ѿ��ŷ���
local WAPS_CLOSING = 5	--���ڹر�
local WAPS_CLOSED = 6	--�ѹرշ���
local WAPS_QUITING = 7	--�����˳�

--��־����
local	EM_LOGGER_LEVEL_FATAL = 1
local	EM_LOGGER_LEVEL_ERROR = 2
local	EM_LOGGER_LEVEL_WARN = 3
local	EM_LOGGER_LEVEL_INFO = 4
local	EM_LOGGER_LEVEL_DEBUG = 5
local	EM_LOGGER_LEVEL_TRACE = 6

--��ں���
function main(SoloGmcc)

	nx_set_value("SoloGmcc", SoloGmcc)
	
	--��Ļ����ַ���ɫ
	--SoloGmcc:SetConsoleColor("LightGreen")

	
	--������־Ŀ¼
	if not nx_path_exists(nx_work_path() .. "log") then
		nx_path_create(nx_work_path() .. "log")
	end
	if not nx_path_exists(nx_work_path() .. "log\\gmcc") then
		nx_path_create(nx_work_path() .. "log\\gmcc")
	end
	
	--��־�����
	local log_pool = nx_create("LogPool")
	local logger = nx_create("SoloLog")
	nx_set_value("logger", logger)
	nx_set_value("log_pool", log_pool)

	
	--��־�ļ�ǰ׺
	log_pool.FilePrefix = nx_work_path() .. "log\\gmcc\\"
	--��������
	log_pool.TraceMax = 256
	--�Ƿ�ÿ��һ����־�ļ�
	log_pool.EveryDayAlone = true
	
	if nx_is_debug() then
		--�ڵ��԰��ｫ��־��Ϣ���������̨
		log_pool.Console = console
	end

	logger.UseThread = true
	--��־�ļ����Ŀ¼
	logger.LogFileDir = nx_work_path() .. "log\\"
	--�Ƿ�ÿ��һ����־�ļ�
	logger.EveryDayAlone = true
	--����Сʱ�з���־
	logger.EveryHourLog = 1
	

	--������־��¼
	log_pool:Start()
	
	nx_set_value("log_pool", log_pool)
	nx_set_value("logger", logger)

	--��ȡ��������
	local ini = nx_create("IniDocument")
	
	--�����ļ���
	ini.FileName = "game_gmcc.ini"
	
	--��������
	if not ini:LoadFromFile() then
		nx_msgbox("[gmcc] �޷���ȡ���������ļ�")
		nx_destroy(ini)
		return 0
	end
	
	--����http����ʵ��
	local httpService = nx_create("EntHttp")
	if ( nil == httpService ) then
		SoloGmcc:WriteMessage("create http service failed.", EM_LOGGER_LEVEL_FATAL);
		nx_msgbox("[gmcc] create httpService failed");		
		return 0;
	end	
	nx_set_value("EntHttp", httpService)
	
	--RPC���ӵ�ַ
	local rpc = nx_create("EntRpc")
	nx_set_value("EntRpc", rpc)

	-- ��������
	if not load_config( ini, SoloGmcc, httpService, rpc, true ) then
		nx_msgbox("[gmcc] load config failed");
		return 0;
	end
	
	local bIsOK = httpService:Startup();
	if bIsOK then
		SoloGmcc:WriteMessage("httpService:Startup() result:" .. nx_string(bIsOK), EM_LOGGER_LEVEL_INFO);
	else
		SoloGmcc:WriteMessage("httpService:Startup() result:" .. nx_string(bIsOK), EM_LOGGER_LEVEL_FATAL);
		nx_msgbox("[gmcc] start httpService failed");
		return 0;	
	end
	
	bIsOK = SoloGmcc:Startup();
	if bIsOK then
		SoloGmcc:WriteMessage("SoloGmcc:Startup() result:" .. nx_string(bIsOK), EM_LOGGER_LEVEL_INFO);
	else
		SoloGmcc:WriteMessage("SoloGmcc:Startup() result:" .. nx_string(bIsOK), EM_LOGGER_LEVEL_FATAL);
		nx_msgbox("[gmcc] start SoloGmcc failed");
		return 0;		
	end
	
	bIsOK = rpc:Startup();
	if bIsOK then
		SoloGmcc:WriteMessage("EntRpc:Startup() result:" .. nx_string(bIsOK), EM_LOGGER_LEVEL_INFO);
	else
		SoloGmcc:WriteMessage("EntRpc:Startup() result:" .. nx_string(bIsOK), EM_LOGGER_LEVEL_FATAL);
		nx_msgbox("[gmcc] start EntRpc failed");
		return 0;		
	end
	
	--����ά�����ӷ���
	local echo_sock = nx_create("EntEcho")
	nx_set_value("EntEcho", echo_sock)
	echo_sock.IP = ini:ReadString("Echo",  "addr", "127.0.0.1")
	echo_sock.Port = ini:ReadInteger("Echo", "port", 1242)
	echo_sock.ReadBuf = ini:ReadInteger("Echo", "msgsize", 65535)
	echo_sock.SendBuf = ini:ReadInteger("Echo", "msgsize", 65535)		
	
	--��ά���ű�(echo)
	nx_bind_script(echo_sock, "main", "echo_init")
	bIsOK = echo_sock:Startup();
	if bIsOK then
		SoloGmcc:WriteMessage("EntEcho:Startup() result:" .. nx_string(bIsOK), EM_LOGGER_LEVEL_INFO);
	else
		SoloGmcc:WriteMessage("EntEcho:Startup() result:" .. nx_string(bIsOK), EM_LOGGER_LEVEL_FATAL);
		nx_msgbox("[gmcc] start EntEcho failed");		
	end
	return 1
end

--��������
function load_config( ini, gmcc, http, rpc, bInit )
	if nil == gmcc or nil == http or nil == ini or nil == rpc then
		return false;
	end

	-- ������ʵ��
	if bInit then
		gmcc.IP = ini:ReadString("Main", "ip", "0.0.0.0")
		gmcc.Port = ini:ReadInteger("Main", "port", 1241)
		gmcc.SendBuf = ini:ReadInteger("Main", "msgsize", 65535)
		gmcc.ReadBuf = ini:ReadInteger("Main", "msgsize", 65535)
		gmcc.GameName = ini:ReadString("Main", "gamename", "unknown")	
		gmcc.Heartbeat = ini:ReadInteger("Main", "heartbeat", 80)	
	end
	gmcc.LogLevel = ini:ReadInteger("Main", "log_level", 4)	
	gmcc:SetWhiteTables(ini:ReadString("Main", "allowip", "*"))
	local isReplyKeep = ini:ReadString("Main", "reply_keep", "false")
	if ( isReplyKeep == "true") then
		gmcc.IsReplyKeep = true	
	end		

		
	--����http����ʵ��
	if bInit then
		http.IP = ini:ReadString("Http", "ip", "0.0.0.0")
		http.Port = ini:ReadInteger("Http", "port", 8088)
		http.ReadBuf = ini:ReadInteger("Http", "msgsize", 65535)
		http.SendBuf = ini:ReadInteger("Http", "msgsize", 65535)	
	end
	http:SetWhiteTables(ini:ReadString("Http", "allowip", "*"))

	local serverId_ = ini:ReadString("Http", "serverId", "0");
	http:SetValue("serverId", serverId_ )
	local load_url = function( v )
	
		local r, _= string.gsub(v,"{serverId}", serverId_);
		return r;
	end

	http:SetValue("profileUrl", load_url(ini:ReadString("Http", "profileUrl", "") ))
	http:SetValue("noticeUrl", load_url(ini:ReadString("Http", "noticeUrl", "") ))
	http:SetValue("giftUrl", load_url(ini:ReadString("Http", "giftUrl", "") ))
	http:SetValue("mallUrl", load_url(ini:ReadString("Http", "mallUrl", "") ))
	http:SetValue("mailConfirmUrl", load_url(ini:ReadString("Http", "mailConfirmUrl", "") ))
	http:SetValue("cmdConfirmUrl", load_url(ini:ReadString("Http", "cmdConfirmUrl", "") ))
	http:SetValue("activityUrl", load_url(ini:ReadString("Http", "activityUrl", "") ))
	http:SetValue("goodsUrl", load_url(ini:ReadString("Http", "goodsUrl", "") ))
	http:SetValue("preOrderUrl", load_url(ini:ReadString("Http", "preOrderUrl", "") ))
	http:SetValue("orderStateUrl", load_url(ini:ReadString("Http", "orderStateUrl", "") ))
	http:SetValue("rebateInfoUrl", load_url(ini:ReadString("Http", "rebateInfoUrl", "") ))
	http:SetValue("rebatePickUrl", load_url(ini:ReadString("Http", "rebatePickUrl", "") ))
	
	http.PushUrl = ini:ReadString("Http", "pushUrl", "")
	http.PushKey = ini:ReadString("Http", "pushKey", "")
	http.PushSecret = ini:ReadString("Http", "pushSecret", "")
	http.PushSound = ini:ReadString("Http", "pushSound", "")
	http.PushBuilderId = ini:ReadInteger("Http", "pushBuilderId", 0)
	http.PushTimeLive = ini:ReadInteger("Http", "pushTimeLive", 0)
	http.IsProduction = ini:ReadInteger("Http", "production", 0)
	
	http.ReqReuseNum = ini:ReadInteger("Http", "req_reuse_num", 1)
	http.ReqRetries = ini:ReadInteger("Http", "req_retries_num", 0)
	
	if bInit then
		rpc.ip = ini:ReadString("RPC",  "addr", "0.0.0.0")
		rpc.Port = ini:ReadInteger("RPC", "port", 1243)
		rpc.ReadBuf = ini:ReadInteger("RPC", "msgsize", 65535)
		rpc.SendBuf = ini:ReadInteger("RPC", "msgsize", 65535)	
	end
	rpc:SetWhiteTables(ini:ReadString("RPC", "allowip", "*"))
	
	return true;
end

--�ر�
function close()	
	local gmcc = nx_value("SoloGmcc")	
	local httpService = nx_value("EntHttp")
	local echo_sock = nx_value("EntEcho")
	local rpc = nx_value("EntRpc")
	
	if nil ~= echo_sock then
		echo_sock:Stop();
	end
		
	if nil ~= httpService then
		httpService:Stop()
	end	
	
	if nil ~= rpc then
		rpc:Stop()
	end		

	if nil ~= gmcc then
		gmcc:Stop()
	end

	local log_pool = nx_value("log_pool")
	local logger = nx_value("logger")
	if nil ~= logger then
		logger:Stop()
	end	
	if nil ~= log_pool then
		log_pool:Stop()
	end	
	return true
end

--ǿ�ƹر�
function force_close()
	close()	
	nx_quit()
	return true
end

-- ���������Դ
function on_cleanup()
	local httpService = nx_value("EntHttp")
	local echo_sock = nx_value("EntEcho")
	local rpc = nx_value("EntRpc")
	
	if nil ~= httpService then
		nx_destroy( httpService  );
	end
	
	if nil ~= echo_sock then
		nx_destroy( echo_sock );
	end
	
	if nil ~= rpc then
		nx_destroy( rpc );
	end	
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
	--nx_callback(self, "on_input_function", "echo_input_function")
	nx_callback(self, "on_input_debugger", "echo_input_debugger")
	return 1
end

--ά�����Ӵ�
function echo_open(self, addr, port)
	--self:WriteLine("echo client " .. nx_string(addr) 
	--	.. ":" .. nx_string(port) .. " connected")
	echo_response( self, "welcome server SoloEcho...")
	return 1
end

--ά�����ӹر�
function echo_close(self, addr, port)
	--self:WriteLine("echo client " .. nx_string(addr) 
	--	.. ":" .. nx_string(port) .. " closed")
	return 1
end

--��ʾ�����ĵ�
function echo_help(self)
	echo_response(self, "	quit		exit program" );
	echo_response(self, "	fquit 		force exit program, call nx_quit" );
	echo_response(self, "	help		show command desc" );
	echo_response(self, "	reload		reload all config" );
	echo_response(self, "	state		show current config and state" );
	echo_response(self, "	end			close this echo" );
end

-- ���¼�������
function echo_reload(self)
	--��ȡ��������
	local ini = nx_create("IniDocument")
	
	--�����ļ���
	ini.FileName = "game_gmcc.ini"
	
	--��������
	if not ini:LoadFromFile() then
		echo_response(self, "can't load game_gmcc.ini");
		nx_destroy(ini)
		return
	end

	local gmcc = nx_value("SoloGmcc")	
	local httpService = nx_value("EntHttp")
	local rpc = nx_value("EntRpc")
	
	if load_config( ini, gmcc, httpService, rpc, false) then
		echo_response(self, "reload config succeed");
	else
		echo_response(self, "reload config failed");
	end
end

--��ʾ��ǰ״̬
function echo_show_state(self, insName)
	if insName ~= nil and insName ~= "gmcc" and insName ~= "http" and insName ~= "rpc" then
		echo_response(self, "insName '" .. insName .. "' don't exists");
		return;
	end

	local gmcc = nx_value("SoloGmcc")	
	local http = nx_value("EntHttp")
	local rpc = nx_value("EntRpc")
	
	if nil == insName or "gmcc" == insName then
		echo_response(self, "gmcc status:");
		if nil == gmcc then
			echo_response(self, "	not found gmcc instance");
		else
			echo_response(self, "	ip:" .. gmcc.IP);
			echo_response(self, "	port:" .. gmcc.Port);
			echo_response(self, "	msg size:" .. gmcc.ReadBuf);
			echo_response(self, "	logLevel:" .. gmcc.LogLevel);
			echo_response(self, "	name:" .. gmcc.GameName);
			echo_response(self, "	heartbeat:" .. gmcc.Heartbeat);
			echo_response(self, "	reply_keep:" .. nx_string(gmcc.IsReplyKeep) );
		end
	end
	
	if nil == insName or "http" == insName then	
		echo_response(self, "http status:");
		if nil == http then
			echo_response(self, "	not found http instance");
		else
			echo_response(self, "	ip:" .. http.IP);
			echo_response(self, "	port:" .. http.Port);
			echo_response(self, "	msg size:" .. http.ReadBuf);
			
			echo_response(self, "	alias:" .. http:GetValue("serverAlias", ""));
			echo_response(self, "	profile url:" .. http:GetValue("profileUrl", ""));
			echo_response(self, "	notice url:" .. http:GetValue("noticeUrl", ""));
			echo_response(self, "	gift url:" .. http:GetValue("giftUrl", ""));
			echo_response(self, "	mall url:" .. http:GetValue("mallUrl", ""));
			echo_response(self, "	mail confirm url:" .. http:GetValue("mailConfirmUrl", ""));
			echo_response(self, "	cmd confirm url:" .. http:GetValue("cmdConfirmUrl", ""));
			echo_response(self, "	activityUrl url:" .. http:GetValue("activityUrl", ""));
			echo_response(self, "	goods url:" .. http:GetValue("goodsUrl", ""));
			echo_response(self, "	pre order url:" .. http:GetValue("preOrderUrl", ""));
			echo_response(self, "	order state url:" .. http:GetValue("orderStateUrl", ""));	
			echo_response(self, "	rebate info url:" .. http:GetValue("rebateInfoUrl", ""));	
			echo_response(self, "	rebate pick url:" .. http:GetValue("rebatePickUrl", ""));				
			
			echo_response(self, "	push url:" .. http.PushUrl);
			echo_response(self, "	push key:" .. http.PushKey);
			echo_response(self, "	push secret:" .. http.PushSecret);
			echo_response(self, "	push sound:" .. http.PushSound);
			echo_response(self, "	push builder id:" .. http.PushBuilderId);
			echo_response(self, "	push time live:" .. http.PushTimeLive);
			echo_response(self, "	production:" .. nx_string(http.IsProduction));
		end
	end	
		
	if nil == insName or "rpc" == insName then	
		echo_response(self, "rpc status:");
		if nil == rpc then
			echo_response(self, "	not found rpc instance");
		else
			echo_response(self, "	ip:" .. rpc.IP);
			echo_response(self, "	port:" .. rpc.Port);
			echo_response(self, "	msg size:" .. rpc.ReadBuf);
		end
	end
end

function echo_end( self )
	echo_response(self, "disconnect...");
	
	self:EndEcho();
end

--�ַ����ָ�����  
function string:split(sep)  
    local sep, fields = sep or " ", {}  
    local pattern = string.format("([^%s]+)", sep)  
    self:gsub(pattern, function (c) fields[#fields + 1] = c end)  
    return fields  
end 

--����ά����������
function echo_input(self, command)
	-- �ָ����
	local args = command:split(nil);
	local cmd = args[1];
	echo_response(self, "ready execute:" .. cmd)
	if cmd == "quit" then
		--�˳�����
		echo_response(self, "SoloEcho server will shutdown...")
		close(self )
	elseif cmd == "fquit" then
		--ǿ���˳�
		echo_response(self, "SoloEcho server will shutdown...")
		force_close()
	elseif cmd == "help" then
		--��ʾ֧�ֵ�����
		echo_help(self);
	elseif cmd == "reload" then
		--���¼�������
		echo_reload(self);
	elseif cmd == "state" then
		--��ʾ״̬
		echo_show_state(self, args[2]);
	elseif cmd == "end" then
		-- ����echo
		echo_end(self);
	else
		echo_response(self, "undefined command: " .. command)
	end
	
	return 1
end


--���ͻ�Ӧ��Ϣ
function echo_response(self, info)
	self:SendEchoLine(">" .. info .. "\r\n")
	self:WriteMessage(info,  EM_LOGGER_LEVEL_INFO)
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

