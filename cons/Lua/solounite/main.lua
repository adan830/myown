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
local WAPS_TESTING = 8	--����������״̬

local MONITOR_INDEX = 0			--�������
local WEBEXCHANGE_INDEX = 1		--���½�������
local DUMPER_INDEX = 2			--ת������������
local ESI_APPLE_ORDER_SERVER = 5   --ƻ���ڹ���ֵ������
local ESI_CHARGE_REWARD_SERVER = 7   --��ֵ����������
local ESI_GLOBAL_RANK_SERVER = 8 -- ȫ�����а�
local ESI_GMCC_SERVER	= 9		-- gmcc������

--��Դ��������
local USE_XML = 0   --ʹ��xml
local USE_INI = 1   --ʹ��ini

local NOT_BEAT_CHECK = 0      --��ʹ������
local NETWORK_LOST_CHECK = 1  --ʹ�õ��߼������
local MESSAGE_DELAY_CHECK =2  --ʹ�������ӳټ������

--�汾�� 2015.12.26
local Client_Version = 3

--��ں���
function main(world)
	local member = nx_create("SoloMember")
	local public = nx_create("SoloPublic")
	local entry = nx_create("SoloEntry")
	local store = nx_create("SoloStore")
	local logger = nx_create("SoloLog")
	local lister = nx_create("SoloLister")
	local sns_manager
	local sns_store
	local sns

	nx_set_value("world", world)
	nx_set_value("member", member)
	nx_set_value("public", public)
	nx_set_value("entry", entry)
	nx_set_value("store", store)
	nx_set_value("logger", logger)
	nx_set_value("lister", lister)

	--������־Ŀ¼
	if not nx_path_exists(nx_work_path() .. "log") then
		nx_path_create(nx_work_path() .. "log")
	end

	logger.UseThread = true
	--��־�ļ����Ŀ¼
	logger.LogFileDir = nx_work_path() .. "log\\"
	--�Ƿ�ÿ��һ����־�ļ�
	logger.EveryDayAlone = false

	--logger.EveryHourLog = 1

	--������־ϵͳ
	world.UseQuickLog = true

	--�Ƿ��˻��������
	world.UseEncrypt = false

	--Ӧ�ý��̼�¼
	world.UseAppRecord = true
	member.UseAppRecord = false
	public.UseAppRecord = false
	entry.UseAppRecord = false
	store.UseAppRecord = false
	lister.UseAppRecord = false

	--ʹ���߳�
	world.UseThread = false
	member.UseThread = true
	public.UseThread = true
	entry.UseThread = true
	store.UseThread = true
	lister.UseThread = true

	--�¼�ѭ����ʱʱ��
	member.EventLoopExceedTime = 2000
	member.CompletionStatusExceedTime = 2000
	public.EventLoopExceedTime = 2000
	public.CompletionStatusExceedTime = 2000

	--��Ļ����ַ���ɫ
	world:SetConsoleColor("White")
	world.OfflineContinue = true

	--�����Ƚϵ�ַ
	world.ReconnectCompareAddr = false

	--����豸Ψһid
	world.UseDevicedIdCheck = true
	--��ȡ��������
	local ini = nx_create("IniDocument")

	--�����ļ���
	--ini.FileName = "game_unite.ini"
	ini.FileName = nx_main_config()

	--��������
	if not ini:LoadFromFile() then
		nx_msgbox("can't load configure file")
		nx_destroy(ini)
		return 0
	end

	--��Ϸ�������
	local district_id = ini:ReadInteger("Main", "DistrictID", 1)
	--���������
	local server_id = ini:ReadInteger("Main", "ServerID", 1)
	--����������
	local server_name = ini:ReadString("Main", "ServerName", "")
	--���������߼�DLL�ļ�
	local public_logic_dll = ini:ReadString("Main", "PublicLogicDll", "")
	--���������
	local max_players = ini:ReadInteger("Main", "MaxPlayers", 8192)
	--������ײ��Ϣ�ļ��ĸ�Ŀ¼
	local collide_root_path = ini:ReadString("Main", "CollideRootPath", "")
	--ʹ��MYSQL���ݿ�
	local use_mysql = nx_boolean(ini:ReadString("Main", "UseMysql", "false"))
	--��¼���ݿ��ȡ����
	local log_db_perf = nx_boolean(ini:ReadString("Main", "LogDBPerformance", "false"))
	--ͨѶ���ܼ�
	local msg_encode_key = ini:ReadString("Main", "MessageEncodeKey", "")
	--ͨѶУ��DLL
	local msg_verify_dll = ini:ReadString("Main", "MessageVerifyDll", "")
	--���ִ����Ƿ���ʾ
	local debug_halt = nx_boolean(ini:ReadString("Main", "DebugHalt", "true"))
	--ˢ���˻���Ϣ�ļ��������
	local flush_minutes = ini:ReadInteger("Main", "FlushMinutes", 0)
	--�Ƿ�Ҫ����ԭʼ���ʺŵ����루�����ܣ�
	local original_pwd = nx_boolean(ini:ReadString("Main", "OriginalPassword", "false"))
	--�Ƿ�����ɾ������Ľ�ɫ��������
	local block_can_del = nx_boolean(ini:ReadString("Main", "BlockCanDelete", "false"))
		--�Ƿ�Ҫ��ȫ���Ľ�ɫ��
	local check_chinese_char_set = nx_boolean(ini:ReadString("Main", "CheckChineseCharSet", "false"))
	--��������
	local encrypt_factor = ini:ReadString("Main", "EncryptFactor", "")
	--��־���Զ��з�ʱ��
	local log_split_week = ini:ReadInteger("Main", "SplitWeek", 0)
	--�Ƿ��Ŷ�
	local player_queue = nx_boolean(ini:ReadString("Main", "PlayerQueue", "false"))
	--��־�Ƿ�ʹ��int���������Լӿ�����ٶ�
	local log_auto_serial = nx_boolean(ini:ReadString("Main", "LogAutoSerial", "false"))
		--�Ƿ�ֶ˼��
	local ostype_check = nx_boolean(ini:ReadString("Main", "UseOsTypeCheck", "false"))

	if collide_root_path ~= "" then
		collide_root_path = nx_function("ext_get_full_path", collide_root_path)
	end

	--sns
	local use_sns = nx_boolean(ini:ReadString("Main", "UseSns", "false"))
	local sns_server_count = ini:ReadInteger("Main", "SnsServerCount", 0)

	if use_sns then
		sns_manager = nx_create("SoloSnsManager")
		sns_store = nx_create("SoloSnsStore")
		sns = nx_create("SoloSns")

		nx_set_value("sns_manager", sns_manager)
		nx_set_value("sns_store", sns_store)
		nx_set_value("sns", sns)

		--Ӧ�ý��̼�¼
		sns_manager.UseAppRecord = false
		sns_store.UseAppRecord = false
		sns.UseAppRecord = false

		--ʹ���߳�
		sns_manager.UseThread = true
		sns_store.UseThread = true
		sns.UseThread = true
	end

	--�Զ�����Ϣ�ظ�
	local custom_ack = nx_boolean(ini:ReadString("Main", "CustomAck", "false"))

	--������ַ
	local inner_addr = "127.0.0.1"
	local inner_port = 0

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

	--���а�ͻ�����ڵ�ַ
	local rank_addr = ini:ReadString("Rank", "Addr", "127.0.0.1:80");

	--�Ʒѷ�����
	local charge_inuse = nx_boolean(ini:ReadString("Charge", "Inuse", ""))
	local charge_addr = ini:ReadString("Charge", "Addr", "")
	local charge_port = ini:ReadInteger("Charge", "Port", 0)

	--�ͷ�������(�ÿͻ���չ���������)
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

	--ȫ�ֽ�ɫ��������
	local global_inuse = nx_boolean(ini:ReadString("Global", "Inuse", ""))
	local global_addr = ini:ReadString("Global", "Addr", "")
	local global_port = ini:ReadInteger("Global", "Port", 0)
	local global_domian = ini:ReadString("Global", "Domain", "nx_base")

	--���������
	local corss_inuse = nx_boolean(ini:ReadString("CrossServer", "Inuse", ""))
	local cross_addr = ini:ReadString("CrossServer", "Addr", "")
	local cross_port = ini:ReadInteger("CrossServer", "Port", 0)

	--���ݿ�����
	local account_base = ini:ReadString("Account", "ConnectString", "")
	local account_pool = ini:ReadInteger("Account", "ConnectPool", 0)

	local role_base = ini:ReadString("Role", "ConnectString", "")
	local role_pool = ini:ReadInteger("Role", "ConnectPool", 0)

	local log_base = ini:ReadString("Log", "ConnectString", "")
	local log_pool = ini:ReadInteger("Log", "ConnectPool", 0)

	local public_base = ini:ReadString("Public", "ConnectString", "")
	local public_pool = ini:ReadInteger("Public", "ConnectPool", 0)

	local manage_base = ini:ReadString("Manage", "ConnectString", "")
	local manage_pool = ini:ReadInteger("Manage", "ConnectPool", 0)

	local sns_base = ini:ReadString("Sns", "ConnectString", "")
	local sns_pool = ini:ReadInteger("Sns", "ConnectPool", 0)

	--������ַ
	local outer_addr = ini:ReadString("Outer", "Addr", "127.0.0.1")
	local outer_port = ini:ReadInteger("Outer", "Port", 2001)

	--�汾��֤������
	local game_id  = ini:ReadInteger("License", "GameID", 0)
	local lic_addr = ini:ReadString("License", "Addr", "127.0.0.1")
	local lic_port = ini:ReadInteger("License", "Port", 2001)

	--���ͷ�����
	local push_addr = ini:ReadString("MsgPush", "Addr", "127.0.0.1")
	local push_port = ini:ReadInteger("MsgPush", "Port", 1000)
	local use_push = nx_boolean(ini:ReadString("MsgPush", "Inuse", ""))

    --ת��������
	local dumpserver_inuse = nx_boolean(ini:ReadString("DumpServer", "Inuse", ""))
	local dumpserver_addr = ini:ReadString("DumpServer", "Addr", "")
	local dumpserver_port = ini:ReadInteger("DumpServer", "Port", 0)

	--Extra��ַ
	local Extra_index = ini:ReadInteger("Extra", "index", 0)
	local Extra_ExtraOutBufferLen = ini:ReadInteger("Extra", "ExtraOutBufferLen", 0)
	local Extra_ExtraInBufferLen = ini:ReadInteger("Extra", "ExtraInBufferLen", 0)

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

	--ƽ̨������
	local platform_inuse = nx_boolean(ini:ReadString("Platform", "Inuse", ""))
	local platform_addr = ini:ReadString("Platform", "Addr", "")
	local platform_port = ini:ReadInteger("Platform", "Port", 0)

	--ȫ�����а�
	local globalrank_inuse = nx_boolean(ini:ReadString("GlobalRank", "Inuse", ""))
	local globalrank_addr = ini:ReadString("GlobalRank", "Addr", "")
	local globalrank_port = ini:ReadInteger("GlobalRank", "Port", 0)


	--��Ȩ�ļ�
	local license = outer_addr .. ".lic"

	--��Ϸ�߼�ģ��
	local logic_loader = nx_create("LogicLoader")

	local logic_path = ini:ReadString("Logic", "Path", "")
	local modules = ini:ReadInteger("Logic", "Modules", 0)

	if logic_path ~= "" and modules == 0 then
		local fs = nx_create("FileSearch")

		fs:SearchFile(nx_work_path() .. logic_path .. "\\", "*.dll")

		local dll_table = fs:GetFileList()

		for i = 1, table.getn(dll_table) do
			if logic_loader:Load(nx_work_path() .. logic_path .. "\\", dll_table[i]) then
				world:WriteLine("load loigc module " .. dll_table[i])
			else
				world:WriteLine("load loigc module " .. dll_table[i] .. " failed")
			end
		end

		nx_destroy(fs)
	else
		for i = 0, modules do
			local dll_name = ini:ReadString("Logic", "Module" .. nx_string(i), "")

			if dll_name ~= "" then
				if logic_loader:Load(nx_work_path() .. logic_path, dll_name) then
					world:WriteLine("load loigc module " .. dll_name)
				else
					world:WriteLine("load loigc module " .. dll_name .. " failed")
				end
			end
		end
	end

	--Sns�߼�ģ��
	local sns_logic_loader;
	if use_sns then
        sns_logic_loader = nx_create("LogicLoader")

        local sns_logic_path = ini:ReadString("SnsLogic", "Path", "")
        local sns_modules = ini:ReadInteger("SnsLogic", "Modules", 0)

        if sns_logic_path ~= "" and sns_modules == 0 then
            local fs = nx_create("FileSearch")

            fs:SearchFile(nx_work_path() .. sns_logic_path .. "\\", "*.dll")

            local dll_table = fs:GetFileList()

            for i = 1, table.getn(dll_table) do
                if sns_logic_loader:Load(nx_work_path() .. sns_logic_path .. "\\", dll_table[i]) then
                    world:WriteLine("load loigc module " .. dll_table[i])
                else
                    world:WriteLine("load loigc module " .. dll_table[i] .. " failed")
                end
            end

            nx_destroy(fs)
        else
            for i = 0, sns_modules do
                local dll_name = ini:ReadString("SnsLogic", "Module" .. nx_string(i), "")

                if dll_name ~= "" then
                    if sns_logic_loader:Load(nx_work_path() .. sns_logic_path, dll_name) then
                        world:WriteLine("load loigc module " .. dll_name)
                    else
                        world:WriteLine("load loigc module " .. dll_name .. " failed")
                    end
                end
            end
        end
    end

	nx_destroy(ini)

	--���������߼�ģ��
	local public_logic_loader = nx_create("LogicLoader")

	if public_logic_dll ~= "" then
		if public_logic_loader:Load(nx_work_path(), public_logic_dll) then
			world:WriteLine("load loigc module " .. public_logic_dll)
		else
			world:WriteLine("load loigc module " .. public_logic_dll .. " failed")
		end
	end

	--������ײ��Ϣ
	local collide_manager = nx_create("CollideManager")

	--�������ݿ�ϵͳ
	local db_system

	if use_mysql then
		db_system = nx_create("MysqlDBSystem")
	else
		db_system = nx_create("SqlDBSystem")
	end

	--�󶨽ű�
	nx_bind_script(world, "main", "echo_init")

	--��Ϸ����
	world.AppName = "unite_" .. nx_string(server_id)
	world.DistrictId = district_id
	world.ServerId = server_id
	world.ServerName = nx_widestr(server_name)
	world.MaxPlayers = max_players
	--ÿ���ʺſ��Դ����Ľ�ɫ����
	world.AccountRoles = 4
	world.CheckChineseCharSet = check_chinese_char_set
	world.FlushMinutes = flush_minutes
	world.InnerAddr = inner_addr
	world.InnerPort = 0
	--world.InnerPort = inner_port + 1
	world.BroadAddr = broad_addr
	world.BroadPort = broad_port
	world.EchoAddr = echo_addr
	world.EchoPort = echo_port
	if corss_inuse then
	 world.CrossAddr = cross_addr
	 world.CrossPort = cross_port
	end
	--��ɫɾ������48Сʱ
	world.RoleProtectBase = 2.0
	--�Ƿ�Ҫ����ԭʼ���ʺŵ����루�����ܣ�
	world.OriginalPassword = original_pwd
	--�Ƿ�����ɾ������Ľ�ɫ��������
	world.BlockCanDelete = block_can_del
	world.EncryptFactor = encrypt_factor

	world.UseSns = use_sns

	--�Ŷ�
	world.PlayerQueue = player_queue
	world.UseOsTypeCheck = ostype_check

	--ƽ̨����
	if platform_inuse then
		world.UseRoomService = true
		world:SetPlatformAddress(platform_addr, platform_port)
	end


	--�������ֹ����ļ�
	world:LoadNameFilter(nx_resource_path() .. "filter.txt")

	--�����ͨ����
	load_scene_config(world)

	--�������ݷ���
	public.AppName = "public_" .. nx_string(server_id)
	public.DistrictId = district_id
	public.ServerId = server_id
	public.LogicLoader = public_logic_loader
	public.InnerAddr = inner_addr
	public.InnerPort = 0
	--public.InnerPort = inner_port + 2
	public.BroadAddr = broad_addr
	public.BroadPort = broad_port

	--��ӹ������ݿռ�
	public:AddPubSpace("domain", "nx_domains")
	public:AddPubSpace("guild", "nx_guilds")

	--�洢����
	store.AppName = "store_" .. nx_string(server_id) .. "_0"
	store.DistrictId = district_id
	store.ServerId = server_id
	store.StoreId = 0
	store.AccountBase = account_base
	store.AccountPools = account_pool
	store.RoleBase = role_base
	store.RolePools = role_pool
	store.LogBase = log_base
	store.LogPools = log_pool
	store.PublicBase = public_base
	store.PublicPools = public_pool
	store.ManageBase = manage_base
	store.ManagePools = manage_pool
	store.DBSystem = db_system
	store.InnerAddr = inner_addr
	store.InnerPort = 0
	--store.InnerPort = inner_port + 100
	store.BroadAddr = broad_addr
	store.BroadPort = broad_port
	store.WakeAddr = "127.0.0.1"
	store.WakePort = 0
	--store.WakePort = 16800
	--��¼���ݿ��ȡ������־
	store.LogDBPerformance = log_db_perf

	--��־�ֱ�
	store.SplitWeek = log_split_week

	init_custom_log(store)
	init_split_log(store, log_auto_serial)

	--�������
	entry.AppName = "entry_" .. nx_string(server_id) .. "_0"
	entry.IDC = 0
	entry.DistrictId = district_id
	entry.ServerId = server_id
	entry.EntryId = 0
	entry.MaxConnects = 1024
	entry.License = license
	entry.EncodeKey = entry:ConvertEncodeKey(msg_encode_key)
	entry.DecodeKey = entry:ConvertEncodeKey(msg_encode_key)
	entry.VerifyDll = msg_verify_dll
	entry.InnerAddr = inner_addr
	--entry.InnerPort = inner_port + 200
	entry.InnerPort = 0
	entry.BroadAddr = broad_addr
	entry.BroadPort = broad_port
	entry.OuterAddr = outer_addr
	entry.OuterPort = outer_port

	--����ͳ��
	--1��ʾ���,0��ʾ�ر�,Ĭ�Ϲر�
	--maxXXX��ʾ�����ֵ
	entry.OutputDelay = 1
	entry.MaxDataLen = (entry.OutBufferMax*9)/10
	--����
	entry.MaxDelayTime  = 500

	world.ListerAddr = outer_addr
	world.ListerPort = outer_port
	--���ջ�������С
	entry.InBufferLen = 8 * 1024
	--���ͻ�������С
	entry.OutBufferLen = 32 * 1024
	--���ͻ���������
	entry.OutBufferMax = 256 * 1024
	--license ��������ַ����
	entry.GameId = game_id
	entry.LicAddr = lic_addr
	entry.LicPort = lic_port
	--��¼��ͻ��˵��շ���Ϣ��message.log�ļ�
	entry.LogConnect = true
	entry.LogSend = true
	entry.LogReceive = true
	entry.UseBufferPool = true
	--entry.CompressMessage = true
	entry.CompressType = 2
	--���������Ƿ񾫼�
	entry.SimpleProtocal = false

	world.CheckInterval = 45
	entry.ClientBeatType=NOT_BEAT_CHECK
	entry.ClientBeatInterval=30
	entry.ClientCheckInterval=45
	entry.WaitRetEncodeInterval=0

	entry.CacheAddr = rank_addr

	--��������
	member.AppName = "member_" .. nx_string(server_id) .. "_0"
	member.DistrictId = district_id
	member.ServerId = server_id
	member.MemberId = 0
	member.LogicLoader = logic_loader
	member.CollideManager = collide_manager
	member.ScriptPath = nx_resource_path() .. "lua\\"
	member.DebugHalt = debug_halt
	member.CollideRootPath = collide_root_path
	member.InnerAddr = inner_addr
	member.InnerPort = 0
	--member.InnerPort = inner_port + 300
	member.BroadAddr = broad_addr
	member.BroadPort = broad_port
	--�Ƿ񲻷���Ϊȱʡ��ֵ�Ķ�������
	member.HideEmptyProperty = false
	--�ϲ���Ϣ
	member.MergeProperty = true
	member.MergeObjects = false

	member.UseSns = use_sns
	member.AppearAtOnce = true

	--�ӱ��ظ�������ǿ��ˢ�´���npc
	member.ForceRefreshVisual = false

	member.UseConfigType = USE_XML

	--�Զ�����Ϣ�ظ�
	member.CustomAck = custom_ack
	--�����ָ��С
	member.SceneGridSize = 16
	--member.SlowScanTime = 5000
	--member.SceneTimerRandLimit = 31

	--��ʡ����
	member.ObjectPropertyFameUpdate  = true
	member.DelayClientReadyResendView = true

	--����ʱˢ��
	member.DelayCreateFrame = 0

	--�������ֹ����ļ�
	member:LoadNameFilter(nx_resource_path() .. "filter.txt")
	--����ȫ�ֵ���Ϣ
	member:LoadLocations(nx_resource_path() .. "location.xml")

	--������
	lister.AppName = "lister_" .. nx_string(server_id)
	--�����б����
	lister.IDC = 0
	lister.DistrictId = district_id
	lister.ListerId = 0
	lister.MaxConnects = 1024
	lister.ClientVersion = Client_Version
	--ȱʡΪ�������κ�IP
	lister.BlockOrAllow = -1
	lister.InnerAddr = inner_addr
	lister.InnerPort = 0
	lister.OuterAddr = outer_addr
	lister.OuterPort = 4000
	lister.BroadAddr = broad_addr
	lister.BroadPort = broad_port


	--�Ʒ�����
	if charge_inuse then
		world:SetChargeAddress(charge_addr, charge_port)
	end

	--�ͷ�����
	if gmcc_inuse then
		world:SetGmccAddress(gmcc_addr, gmcc_port)
	end
	
	--gmcc��չ����
	if gmcc_extra_inuse then
		world:SetExtraAddress(ESI_GMCC_SERVER, gmcc_extra_addr, gmcc_extra_port)
	end

	--��������
	if manage_inuse then
		world:SetManageAddress(manage_addr, manage_port)
	end

	--ȫ�ֽ�ɫ������������
	if global_inuse then
		world:SetGlobalAddress(global_addr, global_port)
		world.GlobalDomainName = global_domian
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

	--ȫ�����а�
	if globalrank_inuse then
	    world:SetExtraAddress(ESI_GLOBAL_RANK_SERVER, globalrank_addr, globalrank_port)
	end


	if use_sns then
        --sns ����
        sns_manager.AppName = "snsmgr_" .. nx_string(server_id)
        sns_manager.DistrictId = district_id
        sns_manager.ServerId = server_id
        sns_manager.BroadAddr = broad_addr
        sns_manager.BroadPort = broad_port
        sns_manager.InnerAddr = inner_addr
        sns_manager.InnerPort = 0
        sns_manager.SnsServerCount = sns_server_count
				sns_manager.PushAddr = push_addr
				sns_manager.PushPort = push_port
				sns_manager.UsePush = use_push

        sns_store.AppName = "snsstor_" .. nx_string(server_id) .. "_0"
        sns_store.DistrictId = district_id
        sns_store.ServerId = server_id
        sns_store.SnsStoreId = 0
        sns_store.SnsBase = sns_base
        sns_store.SnsPools = sns_pool
        sns_store.LogBase = log_base
        sns_store.LogPools = log_pool
        sns_store.PublicBase = public_base
        sns_store.PublicPools = public_pool
        sns_store.ManageBase = manage_base
        sns_store.ManagePools = manage_pool
        sns_store.DBSystem = db_system
        sns_store.InnerAddr = inner_addr
        sns_store.InnerPort = 0
        --sns_store.InnerPort = inner_port + 100
        sns_store.BroadAddr = broad_addr
        sns_store.BroadPort = broad_port
        sns_store.WakeAddr = "127.0.0.1"
        sns_store.WakePort = 0
        --sns_store.WakePort = 16800
        --��¼���ݿ��ȡ������־
        sns_store.LogDBPerformance = log_db_perf

        --init_custom_log(sns_store)

        sns.AppName = "sns_" .. nx_string(server_id) .. "_0"
        sns.DistrictId = district_id
        sns.ServerId = server_id
        sns.SnsId = 0
        sns.LogicLoader = sns_logic_loader
        sns.ScriptPath = nx_resource_path() .. "lua\\"
        sns.BroadAddr = broad_addr
        sns.BroadPort = broad_port
        sns.InnerAddr = inner_addr
        sns.InnerPort = 0
        sns.DBSystem = db_system
        sns.SnsBase = sns_base
        sns.SnsServerCount = sns_server_count
				sns.SnsPerformance = true
				sns.SnsOvertimeValue = 100
    	end

	--������־����
	logger:Start()
	--������Ϸ����
	world:Start()
	--�����洢����
	store:Start()
	--�����������
	entry:Start()
	--�����������ݷ���
	public:Start()
	--������������
	member:Start()
	--�����б����
	lister:Start()
	--����SNS����
	if use_sns then
        sns_manager:Start()
        sns_store:Start()
        sns:Start()
	end
	--��ʾӦ�ó�����
	world:WriteLine("app name " .. world.AppName)
	--��ʾͨѶ��ַ�Ͷ˿�
	world:WriteLine("echo socket " .. echo_addr .. ":" .. nx_string(echo_port))
	--world:WriteLine("inner socket " .. inner_addr .. ":" .. nx_string(inner_port))
	world:WriteLine("broadcast socket " .. broad_addr .. ":" .. nx_string(broad_port))
	world:WriteLine("outer socket " .. outer_addr .. ":" .. nx_string(outer_port))

	if charge_inuse then
		world:WriteLine("charge socket " .. charge_addr .. ":" .. nx_string(charge_port))
	end

	if gmcc_inuse then
		world:WriteLine("gmcc socket " .. gmcc_addr .. ":" .. nx_string(gmcc_port))
	end

	if manage_inuse then
		world:WriteLine("manage socket " .. manage_addr .. ":" .. nx_string(manage_port))
	end

	--ת������
	if dumpserver_inuse then
        world:WriteLine("dump socket " .. dumpserver_addr .. ":" .. nx_string(dumpserver_port))
	end

	--apple�ڹ�
	if appleorder_inuse then
		world:WriteLine("appleorder socket " .. appleorder_addr .. ":" .. nx_string(appleorder_port).. " [index="..nx_string(ESI_APPLE_ORDER_SERVER).."]")
	end

	--Extra
    world:SetExtraInBufferLen(Extra_index,  Extra_ExtraInBufferLen)
    world:SetExtraOutBufferLen(Extra_index,  Extra_ExtraOutBufferLen)

	--�������
	world:WriteLine("game server " .. nx_string(server_id) .. " starting...")

	--���ý��̿���״̬
	world:SetAppState(WAPS_OPENED);

	--���ز���IP������
	--[[
    if load_black_ip(entry) then
		--�������״̬��ֻ����ָ���Ĳ���IP��¼��Ϸ
		entry.BlockOrAllow = 0
		entry:WriteLine("server in test status")
	end
	]]--

	--���ز���IP������
	--[[if load_white_ip(entry) then
		--�������״̬��ֻ����ָ���Ĳ���IP��¼��Ϸ
		entry.BlockOrAllow = 1
		entry:WriteLine("server in test status")
		entry:SetAppState(WAPS_TESTING);
	end]]--
	return 1
end

--���ز���IP������
function load_white_ip(entry)
	if not nx_file_exists(nx_work_path() .. "white_ip.txt") then
		return false
	end

	entry:ClearBlockIP()

	for line in io.lines(nx_work_path() .. "white_ip.txt") do
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

			if entry:AddBlockIP(ip_beg, ip_end) then
				nx_log("add test ip: " .. ip_beg .. " to " .. ip_end)
			else
				nx_log("add test ip failed: " .. line)
			end
		end
	end

	return true
end

--���ز���IP������
function load_black_ip(entry)
	if not nx_file_exists(nx_work_path() .. "black_ip.txt") then
		return false
	end

	entry:ClearBlockIP()

	for line in io.lines(nx_work_path() .. "black_ip.txt") do
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

			if entry:AddBlockIP(ip_beg, ip_end) then
				nx_log("add test ip: " .. ip_beg .. " to " .. ip_end)
			else
				nx_log("add test ip failed: " .. line)
			end
		end
	end

	return true
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

--�����Զ������Ϸ��־���ݿ���ṹ
function init_custom_log(game_store)

	--��Ϸ�淨��־��
	game_store:AddLogTable("game_player_action_log")
	game_store:AddLogField("game_player_action_log", "c_account", "varchar", 50)
	game_store:AddLogField("game_player_action_log", "n_nation", "int", 11)
	game_store:AddLogField("game_player_action_log", "s_role_name", "nvarchar", 50)
	game_store:AddLogField("game_player_action_log", "d_create", "datetime", 8)
	game_store:AddLogField("game_player_action_log", "n_action_type", "int", 11)
	game_store:AddLogField("game_player_action_log", "n_scene_id", "int", 11)
	game_store:AddLogField("game_player_action_log", "n_action_state", "int", 4)
	game_store:AddLogField("game_player_action_log", "s_comment", "nvarchar", 255)


	--��ҵ�¼��־��
	game_store:AddLogTable("role_log")
	game_store:AddLogField("role_log", "c_account", "varchar", 50)
	game_store:AddLogField("role_log", "s_role_name", "nvarchar", 50)
	game_store:AddLogField("role_log", "c_ip", "varchar", 50)
	game_store:AddLogField("role_log", "d_login_time", "datetime", 8)
	game_store:AddLogField("role_log", "d_logout_time", "datetime", 8)
	game_store:AddLogField("role_log", "c_mac", "varchar", 200)
	game_store:AddLogField("role_log", "c_os_type", "varchar", 50)
	game_store:AddLogField("role_log", "s_comment", "nvarchar", 255)


	--������־��
	game_store:AddLogTable("task_log")
	game_store:AddLogField("task_log", "c_account", "varchar", 50)
	game_store:AddLogField("task_log", "s_role_name", "nvarchar", 50)
	game_store:AddLogField("task_log", "d_create", "datetime", 8)
	game_store:AddLogField("task_log", "c_task_id", "varchar", 50)
	game_store:AddLogField("task_log", "n_type", "int", 4)
	game_store:AddLogField("task_log", "n_state", "int", 4)
	game_store:AddLogField("task_log", "s_comment", "nvarchar", 255)


    --��Ϸ��Ǯ�춯��
    game_store:AddLogTable("money_log")
	game_store:AddLogField("money_log", "time", "datetime", 8)
	game_store:AddLogField("money_log", "store_id", "int", 4)
	game_store:AddLogField("money_log", "recover_id", "int", 4)
	game_store:AddLogField("money_log", "self_account", "varchar", 50)
	game_store:AddLogField("money_log", "self_name", "nvarchar", 50)
	game_store:AddLogField("money_log", "self_uid", "varchar", 32)
	game_store:AddLogField("money_log", "event_id", "int", 4)
	game_store:AddLogField("money_log", "event_type", "int", 4)
	game_store:AddLogField("money_log", "event_serial", "varchar", 32)
	game_store:AddLogField("money_log", "money_type", "int", 4)
	game_store:AddLogField("money_log", "`before`", "int", 11)
	game_store:AddLogField("money_log", "num", "int", 11)
	game_store:AddLogField("money_log", "`after`", "int", 11)
	game_store:AddLogField("money_log", "sender_name", "nvarchar", 50)
	game_store:AddLogField("money_log", "sender_uid", "varchar", 32)
	game_store:AddLogField("money_log", "scene_id", "int", 11)
	game_store:AddLogField("money_log", "guild_id", "varchar", 50)
    game_store:AddLogField("money_log", "guild_name", "nvarchar", 50)
    game_store:AddLogField("money_log", "n_state", "int", 4)
	game_store:AddLogField("money_log", "comment", "nvarchar", 255)


	--��Ʒ�춯��־��
    game_store:AddLogTable("item_log")
	game_store:AddLogField("item_log", "time", "datetime", 8)
	game_store:AddLogField("item_log", "store_id", "int", 4)
	game_store:AddLogField("item_log", "recover_id", "int", 4)
	game_store:AddLogField("item_log", "self_account", "varchar", 50)
	game_store:AddLogField("item_log", "self_name", "nvarchar", 50)
	game_store:AddLogField("item_log", "self_uid", "varchar", 32)
	game_store:AddLogField("item_log", "event_id", "int", 4)
	game_store:AddLogField("item_log", "event_type", "int", 4)
	game_store:AddLogField("item_log", "event_serial", "varchar", 32)
	game_store:AddLogField("item_log", "item_id", "varchar", 128)
	game_store:AddLogField("item_log", "item_uid", "varchar", 32)
	game_store:AddLogField("item_log", "item_lifetime", "int", 4)
	game_store:AddLogField("item_log", "`before`", "int", 4)
	game_store:AddLogField("item_log", "num", "int", 4)
	game_store:AddLogField("item_log", "`after`", "int", 4)
	game_store:AddLogField("item_log", "n_color_level", "int", 4)
	game_store:AddLogField("item_log", "sender_name", "nvarchar", 50)
	game_store:AddLogField("item_log", "sender_uid", "varchar", 32)
	game_store:AddLogField("item_log", "scene_id", "int", 11)
	game_store:AddLogField("item_log", "guild_id", "varchar", 50)
    game_store:AddLogField("item_log", "guild_name", "nvarchar", 50)
	game_store:AddLogField("item_log", "n_state", "int", 4)
	game_store:AddLogField("item_log", "comment", "nvarchar", 255)


	--���������־��
	game_store:AddLogTable("role_upgrade_log")
	game_store:AddLogField("role_upgrade_log", "c_account", "varchar", 50)
	game_store:AddLogField("role_upgrade_log", "s_role_name", "nvarchar", 50)
	game_store:AddLogField("role_upgrade_log", "d_create", "datetime", 8)
	game_store:AddLogField("role_upgrade_log", "n_upgrade_type", "int", 11)
	game_store:AddLogField("role_upgrade_log", "n_level_before", "int", 11)
	game_store:AddLogField("role_upgrade_log", "n_level_after", "int", 11)
	game_store:AddLogField("role_upgrade_log", "c_skill_id", "varchar", 50)
	game_store:AddLogField("role_upgrade_log", "s_comment", "nvarchar", 255)


	--�ͷ���־��
	game_store:AddLogTable("punish_log")
	game_store:AddLogField("punish_log", "c_gm_account", "varchar", 50)
	game_store:AddLogField("punish_log", "c_account", "varchar", 50)
	game_store:AddLogField("punish_log", "s_role_name", "nvarchar", 50)
	game_store:AddLogField("punish_log", "d_create", "datetime", 8)
	game_store:AddLogField("punish_log", "n_type", "int", 11)
	game_store:AddLogField("punish_log", "n_minutes", "int", 11)
	game_store:AddLogField("punish_log", "s_comment", "nvarchar", 255)


	--װ����־��
	game_store:AddLogTable("equip_log")
	game_store:AddLogField("equip_log", "c_account", "varchar", 50)
	game_store:AddLogField("equip_log", "s_role_name", "nvarchar", 50)
	game_store:AddLogField("equip_log", "d_create", "datetime", 8)
	game_store:AddLogField("equip_log", "n_operate_type", "int", 11)
	game_store:AddLogField("equip_log", "n_equip_pos", "int", 4)
	game_store:AddLogField("equip_log", "n_equip_slot", "int", 4)
	game_store:AddLogField("equip_log", "n_before_grade", "int", 4)
	game_store:AddLogField("equip_log", "n_after_grade", "int", 4)
	game_store:AddLogField("equip_log", "c_source_item_id", "varchar", 50)
	game_store:AddLogField("equip_log", "c_target_item_id", "varchar", 50)
	game_store:AddLogField("equip_log", "c_deduct_item_ids", "varchar", 200)
	game_store:AddLogField("equip_log", "c_deduct_item_nums", "varchar", 50)
	game_store:AddLogField("equip_log", "n_capital_type", "int", 4)
	game_store:AddLogField("equip_log", "n_capital_num", "int", 11)
	game_store:AddLogField("equip_log", "s_comment", "nvarchar", 255)


	--������־��
	game_store:AddLogTable("friend_log")
	game_store:AddLogField("friend_log", "c_account", "varchar", 50)
	game_store:AddLogField("friend_log", "s_role_name", "nvarchar", 50)
	game_store:AddLogField("friend_log", "d_create", "datetime", 8)
	game_store:AddLogField("friend_log", "s_friend_name", "nvarchar", 50)
	game_store:AddLogField("friend_log", "n_type", "int", 11)
	game_store:AddLogField("friend_log", "s_comment", "nvarchar", 255)


	--�����־��
	game_store:AddLogTable("guild_log")
	game_store:AddLogField("guild_log", "c_account", "varchar", 50)
	game_store:AddLogField("guild_log", "s_role_name", "nvarchar", 50)
	game_store:AddLogField("guild_log", "d_create", "datetime", 8)
	game_store:AddLogField("guild_log", "s_guild_name", "nvarchar", 50)
	game_store:AddLogField("guild_log", "n_grade", "int", 4)
	game_store:AddLogField("guild_log", "s_by_role_name", "nvarchar", 50)
	game_store:AddLogField("guild_log", "n_operate_type", "int", 11)
	game_store:AddLogField("guild_log", "n_capital_type", "int", 11)
	game_store:AddLogField("guild_log", "n_capital_num", "int", 11)
	game_store:AddLogField("guild_log", "n_donate_num", "int", 11)
	game_store:AddLogField("guild_log", "s_comment", "nvarchar", 255)


	--������־��
	game_store:AddLogTable("scene_log")
	game_store:AddLogField("scene_log", "c_account", "varchar", 50)
	game_store:AddLogField("scene_log", "s_role_name", "nvarchar", 50)
	game_store:AddLogField("scene_log", "n_scene_id", "int", 11)
	game_store:AddLogField("scene_log", "d_login_time", "datetime", 8)
	game_store:AddLogField("scene_log", "d_logout_time", "datetime", 8)
	game_store:AddLogField("scene_log", "n_total_time", "int", 11)
	game_store:AddLogField("scene_log", "s_comment", "nvarchar", 255)


	--BOSS��־��
	game_store:AddLogTable("boss_log")
	game_store:AddLogField("boss_log", "c_account", "varchar", 50)
	game_store:AddLogField("boss_log", "s_role_name", "nvarchar", 50)
	game_store:AddLogField("boss_log", "d_create", "datetime", 8)
	game_store:AddLogField("boss_log", "n_scene_id", "int", 11)
	game_store:AddLogField("boss_log", "c_boss_id", "varchar", 50)
	game_store:AddLogField("boss_log", "s_comment", "nvarchar", 255)


	--����������־��
	game_store:AddLogTable("pet_ride_log")
	game_store:AddLogField("pet_ride_log", "c_account", "varchar", 50)
	game_store:AddLogField("pet_ride_log", "s_role_name", "nvarchar", 50)
	game_store:AddLogField("pet_ride_log", "d_create", "datetime", 8)
	game_store:AddLogField("pet_ride_log", "c_pet_id", "varchar", 50)
	game_store:AddLogField("pet_ride_log", "n_operate_type", "int", 11)
	game_store:AddLogField("pet_ride_log", "n_level_before", "int", 11)
	game_store:AddLogField("pet_ride_log", "n_level_after", "int", 11)
	game_store:AddLogField("pet_ride_log", "c_skill_id", "varchar", 50)
	game_store:AddLogField("pet_ride_log", "s_comment", "nvarchar", 255)

	--PVP��־��
	game_store:AddLogTable("pvp_log")
	game_store:AddLogField("pvp_log", "c_account", "varchar", 50)
	game_store:AddLogField("pvp_log", "s_role_name", "nvarchar", 50)
	game_store:AddLogField("pvp_log", "d_create", "datetime", 8)
	game_store:AddLogField("pvp_log", "d_end", "datetime", 8)
	game_store:AddLogField("pvp_log", "s_be_challenge", "nvarchar", 50)
	game_store:AddLogField("pvp_log", "n_rank_before", "int", 11)
	game_store:AddLogField("pvp_log", "n_rank_after", "int", 11)
	game_store:AddLogField("pvp_log", "n_result", "int", 4)
	game_store:AddLogField("pvp_log", "s_comment", "nvarchar", 255)


	--����ϵͳ��־��
	game_store:AddLogTable("nation_log")
	game_store:AddLogField("nation_log", "c_account", "varchar", 50)
	game_store:AddLogField("nation_log", "s_role_name", "nvarchar", 50)
	game_store:AddLogField("nation_log", "d_create", "datetime", 8)
	game_store:AddLogField("nation_log", "n_operate_type", "int", 11)
	game_store:AddLogField("nation_log", "c_params", "varchar", 50)
	game_store:AddLogField("nation_log", "s_comment", "nvarchar", 255)


	--�ܾ���־��
	game_store:AddLogTable("sceret_log")
	game_store:AddLogField("sceret_log", "c_account", "varchar", 50)
	game_store:AddLogField("sceret_log", "s_role_name", "nvarchar", 50)
	game_store:AddLogField("sceret_log", "d_create", "datetime", 8)
	game_store:AddLogField("sceret_log", "d_end", "datetime", 8)
	game_store:AddLogField("sceret_log", "n_sceret_id", "int", 11)
	game_store:AddLogField("sceret_log", "n_dead", "int", 11)
	game_store:AddLogField("sceret_log", "n_result", "int", 4)
	game_store:AddLogField("sceret_log", "s_comment", "nvarchar", 255)


	--�ƺ���־��
	game_store:AddLogTable("title_log")
	game_store:AddLogField("title_log", "c_account", "varchar", 50)
	game_store:AddLogField("title_log", "s_role_name", "nvarchar", 50)
	game_store:AddLogField("title_log", "d_create", "datetime", 8)
	game_store:AddLogField("title_log", "n_title_id", "int", 11)
	game_store:AddLogField("title_log", "n_operate_type", "int", 11)
	game_store:AddLogField("title_log", "s_comment", "nvarchar", 255)


	--�齱��־��
	game_store:AddLogTable("extract_award_log")
	game_store:AddLogField("extract_award_log", "c_account", "varchar", 50)
	game_store:AddLogField("extract_award_log", "s_role_name", "nvarchar", 50)
	game_store:AddLogField("extract_award_log", "d_create", "datetime", 8)
	game_store:AddLogField("extract_award_log", "n_battle_ability", "int", 11)
	game_store:AddLogField("extract_award_log", "n_extract_type", "int", 11)
	game_store:AddLogField("extract_award_log", "c_params", "varchar", 50)
	game_store:AddLogField("extract_award_log", "n_award_level", "int", 11)
	game_store:AddLogField("extract_award_log", "s_comment", "nvarchar", 255)


	--��ֵ��־��
	game_store:AddLogTable("traceorder_log")
	game_store:AddLogField("traceorder_log", "c_account", "varchar", 50)
	game_store:AddLogField("traceorder_log", "s_role_name", "nvarchar", 50)
	game_store:AddLogField("traceorder_log", "d_create", "datetime", 8)
	game_store:AddLogField("traceorder_log", "n_custom_type", "int", 11)
	game_store:AddLogField("traceorder_log", "c_buy_index", "varchar", 50)
	game_store:AddLogField("traceorder_log", "c_trace_order", "varchar", 128)
	game_store:AddLogField("traceorder_log", "c_money", "varchar", 50)
	game_store:AddLogField("traceorder_log", "s_comment", "nvarchar", 255)

    --�ʼ���־��
	game_store:AddLogTable("mail_log")
	game_store:AddLogField("mail_log", "c_account", "varchar", 50)
	game_store:AddLogField("mail_log", "s_role_name", "nvarchar", 50)
	game_store:AddLogField("mail_log", "d_create", "datetime", 8)
	game_store:AddLogField("mail_log", "c_template_type", "varchar", 50)
	game_store:AddLogField("mail_log", "c_capital_params", "varchar", 255)
	game_store:AddLogField("mail_log", "c_item_params", "varchar", 255)
	game_store:AddLogField("mail_log", "s_comment", "nvarchar", 255)


	--������־��
	game_store:AddLogTable("meridian_log")
	game_store:AddLogField("meridian_log", "c_account", "varchar", 50)
	game_store:AddLogField("meridian_log", "s_role_name", "nvarchar", 50)
	game_store:AddLogField("meridian_log", "d_create", "datetime", 8)
	game_store:AddLogField("meridian_log", "n_operate_type", "int", 11)
	game_store:AddLogField("meridian_log", "n_stone_id", "varchar", 50)
	game_store:AddLogField("meridian_log", "n_before_grade", "int", 11)
	game_store:AddLogField("meridian_log", "n_after_grade", "int", 11)
	game_store:AddLogField("meridian_log", "n_before_exp", "int", 11)
	game_store:AddLogField("meridian_log", "n_after_exp", "int", 11)
	game_store:AddLogField("meridian_log", "c_deduct_item_ids", "varchar", 200)
	game_store:AddLogField("meridian_log", "c_deduct_item_nums", "varchar", 50)
	game_store:AddLogField("meridian_log", "n_capital_type", "int", 4)
	game_store:AddLogField("meridian_log", "n_capital_num", "int", 11)
	game_store:AddLogField("meridian_log", "n_pak_list", "varchar", 50)

	--��������־��
	game_store:AddLogTable("revenge_arena_log")
	game_store:AddLogField("revenge_arena_log", "c_account", "varchar", 50)
	game_store:AddLogField("revenge_arena_log", "s_role_name", "nvarchar", 50)
	game_store:AddLogField("revenge_arena_log", "d_start_time", "datetime", 8)
	game_store:AddLogField("revenge_arena_log", "d_end_time", "datetime", 8)
	game_store:AddLogField("revenge_arena_log", "s_enemy_name", "nvarchar", 50)
	game_store:AddLogField("revenge_arena_log", "n_result", "int", 11)
	
	--ʱװ��־��
	game_store:AddLogTable("fashion_log")
	game_store:AddLogField("fashion_log", "c_account", "varchar", 50)
	game_store:AddLogField("fashion_log", "s_role_name", "nvarchar", 50)
	game_store:AddLogField("fashion_log", "d_create", "datetime", 8)
	game_store:AddLogField("fashion_log", "n_operate_type", "int", 11)
	game_store:AddLogField("fashion_log", "c_fashion_id", "varchar", 50)
	game_store:AddLogField("fashion_log", "c_unlock_items", "varchar", 255)
	game_store:AddLogField("fashion_log", "c_unlock_capitals", "varchar", 255)
	game_store:AddLogField("fashion_log", "s_comment", "nvarchar", 255)
	
	-- ���׼�¼
	game_store:AddLogTable("transction_log")
	game_store:AddLogField("transction_log", "c_account", "varchar", 50)
	game_store:AddLogField("transction_log", "s_role_name", "nvarchar", 50)
	game_store:AddLogField("transction_log", "s_target_name", "nvarchar", 50)
	game_store:AddLogField("transction_log", "c_item_id", "varchar", 255)
	game_store:AddLogField("transction_log", "n_item_num", "int", 11)
	game_store:AddLogField("transction_log", "n_item_price", "int", 11)
	game_store:AddLogField("transction_log", "n_operate_type", "int", 11)
	game_store:AddLogField("transction_log", "d_timestamp", "datetime", 8)

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
	local member = nx_value("member")
	local public = nx_value("public")
	local entry = nx_value("entry")
	local store = nx_value("store")
	local logger = nx_value("logger")
	local lister = nx_value("lister")

	local sns_manager
    local sns_store
    local sns
	if world.UseSns then
        sns_manager = nx_value("sns_manager")
        sns_store = nx_value("sns_store")
        sns = nx_value("sns")
	end

	--�ر��б����
	world:WriteLine("lister server shutdown...")
	lister:Stop()

	--�رմ洢����
	world:WriteLine("store server shutdown...")
	store:Stop()

	--�رչ������ݷ���
	world:WriteLine("public server shutdown...")
	public:Stop()

	--�رս������
	world:WriteLine("entry server shutdown...")
	entry:Stop()

	--�رճ�������
	world:WriteLine("member server shutdown...")
	member:Stop()

	if world.UseSns then
        --�ر�sns����
        sns:WriteLine("sns server shutdown...")
        sns:Stop()

        --�ر�sns�洢����
        sns_store:WriteLine("sns_store server shutdown...")
        sns_store:Stop()

        --�ر�sns�������
        sns_manager:WriteLine("sns_manager server shutdown...")
        sns_manager:Stop()
	end

  --�ر���Ϸ����
  world:WriteLine("world server shutdown...")
  world:Stop()

	--�ر���־
	logger:Stop()

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

	nx_callback(self, "on_input_listentity", "on_input_listentity")
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
		--���ز���IP������
		local entry = nx_value("entry")
		if load_white_ip(entry) then
			--�������״̬��ֻ����ָ���Ĳ���IP��¼��Ϸ
			entry.BlockOrAllow = 1
			entry:WriteLine("server entry test status...")
			self:SetAppState(WAPS_TESTING);
		else
			entry:WriteLine("not found white ip file...")
			return 1
		end
    elseif command == "black" then
		--����IP������
		local entry = nx_value("entry")
		if load_black_ip(entry) then
			--�������״̬��ֻ����ָ���Ĳ���IP��¼��Ϸ
			entry.BlockOrAllow = 0
			entry:WriteLine("server entry normal status...")
		else
			entry:WriteLine("not found black ip file...")
			return 1
		end
	elseif command == "open" then
		local entry = nx_value("entry")
		--�뿪����״̬�������κ�IP��¼��Ϸ
		entry.BlockOrAllow = -1
		self:SetAppState(WAPS_OPENED);
		echo_response(entry, "server leave test status...")
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
	elseif command == "reset" then
	    self:ResetPlayerState("rob1000", nx_widestr("rob1000"));
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

--ö�ٳ���ǰ��������ʵ��
function on_input_listentity(self)
    local main_entity = nx_main();

    if main_entity == nil then
        echo_response(self, "get main entity failed!")
        return 0
    end

    local return_msg = "listentity"

    local entitys = {"SoloWorld", "SoloEntry", "SoloLister", "SoloPublic",
    "SoloMember", "SoloStore", "SoloSnsManager", "SoloSnsSotre", "SoloPlatform", "SoloRoom", "SoloLoginCache", "SoloAuction"}

    for i = 1, table.getn( entitys) do
        local obj = get_entity(nx_string(entitys[i]))
	    if obj ~= nil then
	        return_msg = return_msg .. " "
	        return_msg = return_msg .. nx_string(entitys[i])
	    end
	end

	echo_response(self, nx_string(return_msg))
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

--�����Զ������ָ��ģ��
function init_split_log(store, log_auto_serial)

local bAutoInc = log_auto_serial
local auto_inc_key = "`auto_int_serial`  int(8) NOT NULL AUTO_INCREMENT,"
local auto_prim_key = "PRIMARY KEY (`auto_int_serial`)"

if not bAutoInc then
    auto_inc_key = ""
    auto_prim_key = "PRIMARY KEY (`serial`)"
end

--��Ϸ�淨��־��
store:AddLogCreator("game_player_action_log", "CREATE TABLE `game_player_action_log%s` (" ..
	"`serial` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL," ..
	"`c_account` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`n_nation` int(11) DEFAULT NULL," ..
	"`s_role_name` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`d_create` datetime DEFAULT NULL," ..
	"`n_action_type` int(11) DEFAULT NULL," ..
	"`n_scene_id` int(11) DEFAULT NULL," ..
	"`n_action_state` int(4) DEFAULT NULL," ..
	"`s_comment` varchar(255) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"PRIMARY KEY (`serial`)," ..
	"KEY `idx_game_player_action_log_c_account` (`c_account`) USING BTREE," ..
    "KEY `idx_game_player_action_log_s_role_name` (`s_role_name`) USING BTREE,"..
    "KEY `idx_game_player_action_log_d_create` (`d_create`) USING BTREE," ..
    "KEY `idx_game_player_action_log_n_action_type` (`n_action_type`) USING BTREE," ..
    "KEY `idx_game_player_action_log_n_action_state` (`n_action_state`) USING BTREE" ..
	") ENGINE=MyISAM DEFAULT CHARSET=utf8;")



--��ҵ�¼��־��
store:AddLogCreator("role_log", "CREATE TABLE `role_log%s` (" ..
	"`serial` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL," ..
	"`c_account` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`s_role_name` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`c_ip` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`d_login_time` datetime DEFAULT NULL," ..
	"`d_logout_time` datetime DEFAULT NULL," ..
	"`c_mac` varchar(200) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`c_os_type` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`s_comment` varchar(255) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"PRIMARY KEY (`serial`)," ..
	"KEY `idx_role_log_d_login_time` (`d_login_time`) USING BTREE," ..
	"KEY `idx_role_log_s_role_name` (`s_role_name`) USING BTREE," ..
	"KEY `idx_role_log_c_account` (`c_account`) USING BTREE" ..
	") ENGINE=MyISAM DEFAULT CHARSET=utf8;")


--������־��
store:AddLogCreator("task_log", "CREATE TABLE `task_log%s` (" ..
	"`serial` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL," ..
	"`c_account` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`s_role_name` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`d_create` datetime DEFAULT NULL," ..
	"`c_task_id` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`n_type` int(4) DEFAULT NULL," ..
	"`n_state` int(4) DEFAULT NULL," ..
	"`s_comment` varchar(255) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"PRIMARY KEY (`serial`)," ..
	"KEY `idx_task_log_c_account` (`c_account`) USING BTREE," ..
    "KEY `idx_task_log_s_role_name` (`s_role_name`) USING BTREE," ..
    "KEY `idx_task_log_d_create` (`d_create`) USING BTREE," ..
    "KEY `idx_task_log_n_type` (`n_type`) USING BTREE," ..
	"KEY `idx_task_log_n_state` (`n_state`) USING BTREE," ..
    "KEY `idx_task_log_c_task_id` (`c_task_id`) USING BTREE" ..
	") ENGINE=MyISAM DEFAULT CHARSET=utf8;")



--��Ǯ��־��
store:AddLogCreator("money_log", "CREATE TABLE `money_log%s` (" ..
    auto_inc_key ..
	"`serial` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL," ..
	"`time` datetime DEFAULT NULL," ..
	"`store_id` int(4) DEFAULT NULL," ..
	"`recover_id` int(4) DEFAULT NULL," ..
	"`self_account` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`self_name` varchar(64) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`self_uid` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`event_id` int(4) DEFAULT NULL," ..
	"`event_type` int(11) DEFAULT NULL," ..
	"`event_serial` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`money_type` int(4) DEFAULT NULL," ..
	"`before` int(11) DEFAULT NULL," ..
	"`num` int(11) DEFAULT NULL," ..
	"`after` int(11) DEFAULT NULL," ..
	"`sender_name` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`sender_uid` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`scene_id` int(11) DEFAULT NULL," ..
	"`guild_id` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
    "`guild_name` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`n_state` int(4) DEFAULT NULL," ..
	"`comment` varchar(255) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	auto_prim_key .. "," ..
	"KEY `idx_money_log_self_name` (`self_name`) USING BTREE," ..
    "KEY `idx_money_log_self_account` (`self_account`) USING BTREE," ..
    "KEY `idx_money_log_time` (`time`) USING BTREE," ..
    "KEY `idx_money_log_event_id` (`event_id`) USING BTREE," ..
    "KEY `idx_money_log_self_uid` (`self_uid`) USING BTREE," ..
    "KEY `idx_money_log_scene_id` (`scene_id`) USING BTREE," ..
    "KEY `idx_money_log_money_type` (`money_type`) USING BTREE" ..
	") ENGINE=MyISAM DEFAULT CHARSET=utf8;")



--��Ʒ��־��
store:AddLogCreator("item_log", "CREATE TABLE `item_log%s` (" ..
    auto_inc_key ..
	"`serial` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL," ..
	"`time` datetime DEFAULT NULL," ..
	"`store_id` int(4) DEFAULT NULL," ..
	"`recover_id` int(4) DEFAULT NULL," ..
	"`self_account` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`self_name` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`self_uid` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`event_id` int(4) DEFAULT NULL," ..
	"`event_type` int(4) DEFAULT NULL," ..
	"`event_serial` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`item_id` varchar(128) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`item_uid` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`item_lifetime` int(4) DEFAULT NULL," ..
	"`before` int(4) DEFAULT NULL," ..
	"`num` int(4) DEFAULT NULL," ..
	"`after` int(4) DEFAULT NULL," ..
	"`n_color_level` int(4) DEFAULT NULL," ..
	"`sender_name` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`sender_uid` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`scene_id` int(11) DEFAULT NULL," ..
	"`guild_id` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
    "`guild_name` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`n_state` int(4) DEFAULT NULL," ..
	"`comment` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	auto_prim_key .. "," ..
	"KEY `idx_item_log_self_name` (`self_name`) USING BTREE," ..
    "KEY `idx_item_log_item_id` (`item_id`) USING BTREE," ..
    "KEY `idx_item_log_self_account` (`self_account`) USING BTREE," ..
    "KEY `idx_item_log_time` (`time`) USING BTREE," ..
    "KEY `idx_item_log_event_id` (`event_id`) USING BTREE," ..
    "KEY `idx_item_log_self_uid` (`self_uid`) USING BTREE," ..
    "KEY `idx_item_log_scene_id` (`scene_id`) USING BTREE" ..
	") ENGINE=MyISAM DEFAULT CHARSET=utf8;")



--���������־��
store:AddLogCreator("role_upgrade_log", "CREATE TABLE `role_upgrade_log%s` (" ..
	"`serial` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL," ..
    "`c_account` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`s_role_name` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`d_create` datetime DEFAULT NULL," ..
	"`n_upgrade_type` int(11) DEFAULT NULL," ..
	"`n_level_before` int(11) DEFAULT NULL," ..
	"`n_level_after` int(11) DEFAULT NULL," ..
	"`c_skill_id` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`s_comment` varchar(255) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"PRIMARY KEY (`serial`)," ..
	"KEY `idx_role_upgrade_log_d_create` (`d_create`) USING BTREE," ..
    "KEY `idx_role_upgrade_log_s_role_name` (`s_role_name`) USING BTREE," ..
    "KEY `idx_role_upgrade_log_c_account` (`c_account`) USING BTREE," ..
    "KEY `idx_role_upgrade_log_n_upgrade_type` (`n_upgrade_type`) USING BTREE," ..
    "KEY `idx_role_upgrade_log_c_skill_id` (`c_skill_id`) USING BTREE" ..
	") ENGINE=MyISAM DEFAULT CHARSET=utf8;")


--�ͷ���־��
store:AddLogCreator("punish_log", "CREATE TABLE `punish_log%s` (" ..
	"`serial` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL," ..
	"`c_gm_account` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`c_account` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`s_role_name` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`d_create` datetime DEFAULT NULL," ..
	"`n_type` int(11) DEFAULT NULL," ..
	"`n_minutes` int(11) DEFAULT NULL," ..
	"`s_comment` varchar(255) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"PRIMARY KEY (`serial`)," ..
	"KEY `idx_punish_log_d_create` (`d_create`) USING BTREE," ..
	"KEY `idx_punish_log_s_role_name` (`s_role_name`) USING BTREE" ..
	") ENGINE=MyISAM DEFAULT CHARSET=utf8;")

--װ����־��
store:AddLogCreator("equip_log", "CREATE TABLE `equip_log%s` (" ..
	"`serial` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL," ..
	"`c_account` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`s_role_name` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`d_create` datetime DEFAULT NULL," ..
	"`n_operate_type` int(11) DEFAULT NULL," ..
	"`n_equip_pos` int(11) DEFAULT NULL," ..
	"`n_equip_slot` int(11) DEFAULT NULL," ..
	"`n_before_grade` int(11) DEFAULT NULL," ..
	"`n_after_grade` int(11) DEFAULT NULL," ..
	"`c_source_item_id` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`c_target_item_id` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`c_deduct_item_ids` varchar(200) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`c_deduct_item_nums` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`n_capital_type` int(11) DEFAULT NULL," ..
	"`n_capital_num` int(11) DEFAULT NULL," ..
	"`s_comment` varchar(255) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"PRIMARY KEY (`serial`)," ..
	"KEY `idx_equip_log_d_create` (`d_create`) USING BTREE," ..
    "KEY `idx_equip_log_s_role_name` (`s_role_name`) USING BTREE," ..
    "KEY `idx_equip_log_c_account` (`c_account`) USING BTREE," ..
    "KEY `idx_equip_log_n_operate_type` (`n_operate_type`) USING BTREE," ..
    "KEY `idx_equip_log_c_source_item_id` (`c_source_item_id`) USING BTREE," ..
    "KEY `idx_equip_log_c_target_item_id` (`c_target_item_id`) USING BTREE," ..
    "KEY `idx_equip_log_c_deduct_item_ids` (`c_deduct_item_ids`) USING BTREE," ..
    "KEY `idx_equip_log_n_capital_type` (`n_capital_type`) USING BTREE" ..
	") ENGINE=MyISAM DEFAULT CHARSET=utf8;")


--������־��
store:AddLogCreator("friend_log", "CREATE TABLE `friend_log%s` (" ..
	"`serial` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL," ..
	"`c_account` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`s_role_name` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`d_create` datetime DEFAULT NULL," ..
	"`s_friend_name` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`n_type` int(11) DEFAULT NULL," ..
	"`s_comment` varchar(255) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"PRIMARY KEY (`serial`)," ..
	"KEY `idx_friend_log_d_create` (`d_create`) USING BTREE," ..
    "KEY `idx_friend_log_s_role_name` (`s_role_name`) USING BTREE," ..
    "KEY `idx_friend_log_c_account` (`c_account`) USING BTREE," ..
    "KEY `idx_friend_log_n_type` (`n_type`) USING BTREE" ..
	") ENGINE=MyISAM DEFAULT CHARSET=utf8;")


--������־��
store:AddLogCreator("guild_log", "CREATE TABLE `guild_log%s` (" ..
	"`serial` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL," ..
	"`c_account` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`s_role_name` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`d_create` datetime DEFAULT NULL," ..
	"`s_guild_name` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`n_grade` int(4) DEFAULT NULL," ..
    "`s_by_role_name` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`n_operate_type` int(11) DEFAULT NULL," ..
	"`n_capital_type` int(11) DEFAULT NULL," ..
	"`n_capital_num` int(11) DEFAULT NULL," ..
	"`n_donate_num` int(11) DEFAULT NULL," ..
	"`s_comment` varchar(255) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"PRIMARY KEY (`serial`)," ..
	"KEY `idx_guild_log_d_create` (`d_create`) USING BTREE," ..
    "KEY `idx_guild_log_s_role_name` (`s_role_name`) USING BTREE," ..
    "KEY `idx_guild_log_c_account` (`c_account`) USING BTREE," ..
    "KEY `idx_guild_log_s_guild_name` (`s_guild_name`) USING BTREE," ..
    "KEY `idx_guild_log_n_operate_type` (`n_operate_type`) USING BTREE," ..
    "KEY `idx_guild_log_n_capital_type` (`n_capital_type`) USING BTREE" ..
	") ENGINE=MyISAM DEFAULT CHARSET=utf8;")


--Boss��־��
store:AddLogCreator("boss_log", "CREATE TABLE `boss_log%s` (" ..
	"`serial` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL," ..
	"`c_account` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`s_role_name` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`d_create` datetime DEFAULT NULL," ..
	"`n_scene_id` int(11) DEFAULT NULL," ..
    "`c_boss_id` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`s_comment` varchar(255) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"PRIMARY KEY (`serial`)," ..
	"KEY `idx_boss_log_d_create` (`d_create`) USING BTREE," ..
    "KEY `idx_boss_log_s_role_name` (`s_role_name`) USING BTREE," ..
    "KEY `idx_boss_log_c_account` (`c_account`) USING BTREE," ..
    "KEY `idx_boss_log_n_scene_id` (`n_scene_id`) USING BTREE," ..
    "KEY `idx_boss_log_c_boss_id` (`c_boss_id`) USING BTREE" ..
	") ENGINE=MyISAM DEFAULT CHARSET=utf8;")

--������־��
store:AddLogCreator("scene_log", "CREATE TABLE `scene_log%s` (" ..
	"`serial` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL," ..
	"`c_account` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`s_role_name` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`n_scene_id` int(11) DEFAULT NULL," ..
	"`d_login_time` datetime DEFAULT NULL," ..
	"`d_logout_time` datetime DEFAULT NULL," ..
	"`n_total_time` int(11) DEFAULT NULL," ..
	"`s_comment` varchar(255) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"PRIMARY KEY (`serial`)," ..
	"KEY `idx_scene_log_d_login_time` (`d_login_time`) USING BTREE," ..
    "KEY `idx_scene_log_s_role_name` (`s_role_name`) USING BTREE," ..
    "KEY `idx_scene_log_c_account` (`c_account`) USING BTREE," ..
    "KEY `idx_scene_log_n_scene_id` (`n_scene_id`) USING BTREE" ..
	") ENGINE=MyISAM DEFAULT CHARSET=utf8;")

--������־
store:AddLogCreator("nx_chatlog", "CREATE TABLE `nx_chatlog%s` (" ..
	"`serial_no` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL," ..
	"`chat_time` datetime DEFAULT NULL," ..
	"`chat_from` varchar(128) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`chat_to` varchar(128) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`chat_type` int(11) DEFAULT NULL," ..
	"`chat_content` varchar(256) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`chat_comment` varchar(256) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"PRIMARY KEY (`serial_no`)," ..
	"KEY `IX_nx_chatlog_1` (`chat_time`) USING BTREE," ..
	"KEY `IX_nx_chatlog_2` (`chat_from`) USING BTREE," ..
	"KEY `IX_nx_chatlog_3` (`chat_to`) USING BTREE" ..
	") ENGINE=MyISAM DEFAULT CHARSET=utf8;")

--GM��־��
store:AddLogCreator("nx_gmlog", "CREATE TABLE `nx_gmlog%s` (" ..
	"`serial_no` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL," ..
	"`log_time` datetime DEFAULT NULL," ..
	"`log_name` varchar(128) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`log_source` int(11) DEFAULT NULL," ..
	"`log_type` int(11) DEFAULT NULL," ..
	"`log_content` varchar(256) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`log_comment` varchar(2048) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"PRIMARY KEY (`serial_no`)," ..
	"KEY `IX_nx_gmlog_1` (`log_time`) USING BTREE," ..
	"KEY `IX_nx_gmlog_2` (`log_name`) USING BTREE" ..
	") ENGINE=MyISAM DEFAULT CHARSET=utf8;")

--log����
store:AddLogCreator("nx_logdata", "CREATE TABLE `nx_logdata%s` (" ..
	"`serial_no` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL," ..
	"`log_time` datetime DEFAULT NULL," ..
	"`log_name` varchar(128) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`log_source` int(11) DEFAULT NULL," ..
	"`log_type` int(11) DEFAULT NULL," ..
	"`log_content` varchar(256) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`log_comment` varchar(256) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"PRIMARY KEY (`serial_no`)," ..
	"KEY `idx_nx_logdata_log_time` (`log_time`) USING BTREE," ..
    "KEY `idx_nx_logdata_log_name` (`log_name`) USING BTREE," ..
    "KEY `idx_nx_logdata_log_type` (`log_type`) USING BTREE" ..
	") ENGINE=MyISAM DEFAULT CHARSET=utf8;")


--����������־��
store:AddLogCreator("pet_ride_log", "CREATE TABLE `pet_ride_log%s` (" ..
	"`serial` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL," ..
    "`c_account` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`s_role_name` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`d_create` datetime DEFAULT NULL," ..
	"`c_pet_id` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`n_operate_type` int(11) DEFAULT NULL," ..
	"`n_level_before` int(11) DEFAULT NULL," ..
	"`n_level_after` int(11) DEFAULT NULL," ..
	"`c_skill_id` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`s_comment` varchar(255) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"PRIMARY KEY (`serial`)," ..
	"KEY `idx_pet_ride_log_d_create` (`d_create`) USING BTREE," ..
    "KEY `idx_pet_ride_log_s_role_name` (`s_role_name`) USING BTREE," ..
    "KEY `idx_pet_ride_log_c_account` (`c_account`) USING BTREE," ..
    "KEY `idx_pet_ride_log_n_operate_type` (`n_operate_type`) USING BTREE," ..
    "KEY `idx_pet_ride_log_c_pet_id` (`c_pet_id`) USING BTREE," ..
    "KEY `idx_pet_ride_log_c_skill_id` (`c_skill_id`) USING BTREE" ..
	") ENGINE=MyISAM DEFAULT CHARSET=utf8;")


--PVP��־��
store:AddLogCreator("pvp_log", "CREATE TABLE `pvp_log%s` (" ..
	"`serial` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL," ..
    "`c_account` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`s_role_name` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`d_create` datetime DEFAULT NULL," ..
	"`d_end` datetime DEFAULT NULL," ..
	"`s_be_challenge` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`n_rank_before` int(11) DEFAULT NULL," ..
	"`n_rank_after` int(11) DEFAULT NULL," ..
	"`n_result` int(4) DEFAULT NULL," ..
	"`s_comment` varchar(255) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"PRIMARY KEY (`serial`)," ..
	"KEY `idx_pvp_log_d_create` (`d_create`) USING BTREE," ..
	"KEY `idx_pvp_log_s_role_name` (`s_role_name`) USING BTREE," ..
	"KEY `idx_pvp_log_c_account` (`c_account`) USING BTREE" ..
	") ENGINE=MyISAM DEFAULT CHARSET=utf8;")


--������־��
store:AddLogCreator("nation_log", "CREATE TABLE `nation_log%s` (" ..
	"`serial` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL," ..
    "`c_account` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`s_role_name` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`d_create` datetime DEFAULT NULL," ..
	"`n_operate_type` int(11) DEFAULT NULL," ..
    "`c_params` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`s_comment` varchar(255) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"PRIMARY KEY (`serial`)," ..
	"KEY `idx_nation_log_d_create` (`d_create`) USING BTREE," ..
    "KEY `idx_nation_log_s_role_name` (`s_role_name`) USING BTREE," ..
    "KEY `idx_nation_log_c_account` (`c_account`) USING BTREE," ..
    "KEY `idx_nation_log_n_operate_type` (`n_operate_type`) USING BTREE" ..
	") ENGINE=MyISAM DEFAULT CHARSET=utf8;")


--�ܾ���־��
store:AddLogCreator("sceret_log", "CREATE TABLE `sceret_log%s` (" ..
	"`serial` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL," ..
    "`c_account` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`s_role_name` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`d_create` datetime DEFAULT NULL," ..
	"`d_end` datetime DEFAULT NULL," ..
	"`n_sceret_id` int(11) DEFAULT NULL," ..
	"`n_dead` int(11) DEFAULT NULL," ..
    "`n_result` int(4) DEFAULT NULL," ..
	"`s_comment` varchar(255) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"PRIMARY KEY (`serial`)," ..
	"KEY `idx_sceret_log_d_create` (`d_create`) USING BTREE," ..
    "KEY `idx_sceret_log_s_role_name` (`s_role_name`) USING BTREE," ..
    "KEY `idx_sceret_log_c_account` (`c_account`) USING BTREE," ..
    "KEY `idx_sceret_log_n_sceret_id` (`n_sceret_id`) USING BTREE" ..
	") ENGINE=MyISAM DEFAULT CHARSET=utf8;")


--�ƺ���־��
store:AddLogCreator("title_log", "CREATE TABLE `title_log%s` (" ..
	"`serial` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL," ..
    "`c_account` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`s_role_name` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`d_create` datetime DEFAULT NULL," ..
	"`n_title_id` int(11) DEFAULT NULL," ..
	"`n_operate_type` int(11) DEFAULT NULL," ..
	"`s_comment` varchar(255) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"PRIMARY KEY (`serial`)," ..
	"KEY `idx_title_log_d_create` (`d_create`) USING BTREE," ..
    "KEY `idx_title_log_s_role_name` (`s_role_name`) USING BTREE," ..
    "KEY `idx_title_log_c_account` (`c_account`) USING BTREE," ..
    "KEY `idx_title_log_n_operate_type` (`n_operate_type`) USING BTREE" ..
	") ENGINE=MyISAM DEFAULT CHARSET=utf8;")

--�齱��־��
store:AddLogCreator("extract_award_log", "CREATE TABLE `extract_award_log%s` (" ..
	"`serial` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL," ..
    "`c_account` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`s_role_name` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`d_create` datetime DEFAULT NULL," ..
	"`n_battle_ability` int(11) DEFAULT NULL," ..
	"`n_extract_type` int(11) DEFAULT NULL," ..
	"`c_params` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`n_award_level` int(11) DEFAULT NULL," ..
	"`s_comment` varchar(255) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"PRIMARY KEY (`serial`)," ..
	"KEY `idx_extract_award_log_d_create` (`d_create`) USING BTREE," ..
    "KEY `idx_extract_award_log_s_role_name` (`s_role_name`) USING BTREE," ..
    "KEY `idx_extract_award_log_c_account` (`c_account`) USING BTREE," ..
    "KEY `idx_extract_award_log_n_extract_type` (`n_extract_type`) USING BTREE" ..
	") ENGINE=MyISAM DEFAULT CHARSET=utf8;")


--������־��
store:AddLogCreator("traceorder_log", "CREATE TABLE `traceorder_log%s` (" ..
	"`serial` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL," ..
    "`c_account` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`s_role_name` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`d_create` datetime DEFAULT NULL," ..
	"`n_custom_type` int(11) DEFAULT NULL," ..
	"`c_buy_index` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`c_trace_order` varchar(128) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`c_money` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`s_comment` varchar(255) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"PRIMARY KEY (`serial`)," ..
	"KEY `idx_traceorder_log_d_create` (`d_create`) USING BTREE," ..
    "KEY `idx_traceorder_log_s_role_name` (`s_role_name`) USING BTREE," ..
    "KEY `idx_traceorder_log_c_account` (`c_account`) USING BTREE," ..
    "KEY `idx_traceorder_log_n_custom_type` (`n_custom_type`) USING BTREE" ..
	") ENGINE=MyISAM DEFAULT CHARSET=utf8;")


--�ʼ���־��
store:AddLogCreator("mail_log", "CREATE TABLE `mail_log%s` (" ..
	"`serial` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL," ..
    "`c_account` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`s_role_name` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`d_create` datetime DEFAULT NULL," ..
	"`c_template_type` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`c_capital_params` varchar(255) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`c_item_params` varchar(255) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`s_comment` varchar(255) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"PRIMARY KEY (`serial`)," ..
	"INDEX `idx_mail_log_d_create` (`d_create`) USING BTREE," ..
	"INDEX `idx_mail_log_s_role_name` (`s_role_name`) USING BTREE" ..
	") ENGINE=MyISAM DEFAULT CHARSET=utf8;")

	
--������־��
store:AddLogCreator("meridian_log", "CREATE TABLE `meridian_log%s` (" ..
	"`serial` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL," ..
	"`c_account` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`s_role_name` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`d_create` datetime DEFAULT NULL," ..
	"`n_operate_type` int(11) DEFAULT NULL," ..
	"`n_stone_id` varchar(50) DEFAULT NULL," ..
	"`n_before_grade` int(11) DEFAULT NULL," ..
	"`n_after_grade` int(11) DEFAULT NULL," ..
	"`n_before_exp` int(11) DEFAULT NULL," ..
	"`n_after_exp` int(11) DEFAULT NULL," ..
	"`c_deduct_item_ids` varchar(200) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`c_deduct_item_nums` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`n_capital_type` int(4) DEFAULT NULL," ..
	"`n_capital_num` int(11) DEFAULT NULL," ..
	"`n_pak_list` varchar(50) DEFAULT NULL," ..
	"PRIMARY KEY (`serial`)," ..
    "KEY `idx_wing_log_s_role_name` (`s_role_name`) USING BTREE," ..
    "KEY `idx_wing_log_c_account` (`c_account`) USING BTREE" ..
	") ENGINE=MyISAM DEFAULT CHARSET=utf8;")

--��������־��
store:AddLogCreator("revenge_arena_log", "CREATE TABLE `revenge_arena_log%s` (" ..
	"`serial` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL," ..
	"`c_account` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`s_role_name` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`d_start_time` datetime DEFAULT NULL," ..
	"`d_end_time` datetime DEFAULT NULL," ..
	"`s_enemy_name` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`n_result` int(11) DEFAULT NULL," ..
	"PRIMARY KEY (`serial`)," ..
    "KEY `idx_revenge_arena_log_s_role_name` (`s_role_name`) USING BTREE," ..
    "KEY `idx_revenge_arena_log_c_account` (`c_account`) USING BTREE" ..
	") ENGINE=MyISAM DEFAULT CHARSET=utf8;")
	
--ʱװ��־��
store:AddLogCreator("fashion_log", "CREATE TABLE `fashion_log%s` (" ..
	"`serial` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL," ..
	"`c_account` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`s_role_name` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`d_create` datetime DEFAULT NULL," ..
	"`n_operate_type` int(11) DEFAULT NULL," ..
	"`c_fashion_id` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`c_unlock_items` varchar(255) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`c_unlock_capitals` varchar(255) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`s_comment` varchar(255) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"PRIMARY KEY (`serial`)," ..
    "KEY `idx_fashion_log_s_role_name` (`s_role_name`) USING BTREE," ..
    "KEY `idx_fashion_log_c_account` (`c_account`) USING BTREE" ..
	") ENGINE=MyISAM DEFAULT CHARSET=utf8;")
	
--���ױ�
store:AddLogCreator("transction_log", "CREATE TABLE `transction_log%s` (" ..
	"`serial` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL," ..
	"`c_account` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`s_role_name` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`s_target_name` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`c_item_id` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL," ..
	"`n_item_num` int(11) DEFAULT NULL," ..
	"`n_item_price` int(11) DEFAULT NULL," ..
	"`n_operate_type` int(11) DEFAULT NULL," ..
	"`d_timestamp` datetime DEFAULT NULL," ..
	"PRIMARY KEY (`serial`)," ..
    "KEY `idx_transcation_log_s_role_name` (`s_role_name`) USING BTREE," ..
    "KEY `idx_transcation_log_c_account` (`c_account`) USING BTREE" ..
	") ENGINE=MyISAM DEFAULT CHARSET=utf8;")
	
   return true


end
