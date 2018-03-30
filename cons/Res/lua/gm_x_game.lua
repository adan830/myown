require("define")
local gm_helper = require("gm_x_helper")

------------------------------------------------------------------------
-- ��������
-- ���¼���ĳ�������ļ�
function x_reload_config(gm, ...)
	if table.getn(arg) < 1 then
		return gm_helper.error("format:x_reload_config <configkey>");
	end

	local configkey = nx_string(arg[1]);
	if nx_reload_config(configkey) then
		return gm_helper.succeed();
	else
		return gm_helper.error("reload config file failed");
	end
end


-- ��������ǰʱ��
function x_servertime(gm)
    local time = os.time();
    local timeTab = os.date("*t",time)

    local sInfo =  timeTab.year .. "/" .. timeTab.month .. "/" .. timeTab.day .. " ";
    sInfo = sInfo .. timeTab.hour .. ":" .. timeTab.min .. ":" .. timeTab.sec;
    return gm_helper.content(sInfo);
end


--�������뵱ǰ���Բ�������������ļ�
--[[
function x_reload(gm)
	local result = {};
	local target = get_prop_target(gm)
	if not nx_exists(target) then
		result["error"] = "not found target";
		return tojson(result);
	end

	if not nx_load_config(target) then
		result["error"] = "Reload config file failed!";
		return tojson(result);
	else
		result["success"] = "Reload config file succeed!";
		return tojson(result);
	end
end
---]]

------------------------------------------------------------------------
-- λ������
--˲�Ƶ�ĳ����(ȡ��0��߶�): x_goto [x] [z]
function x_goto(gm, ...)
    if table.getn(arg) < 2 then
        return gm_helper.error("format: x_goto <x> <z>");
    end

    return x_goto_ex( gm, arg[1], arg[2], 0);
end

--˲�Ƶ�ĳ����(���ݴ����ȡ�߶�): goto [x] [z] [floor]
function x_gotoex(gm, ...)
    if table.getn(arg) < 3 then
        return gm_helper.error("format: x_goto <x> <z> <floor>");
    end
	
	local target = get_prop_target(gm);
	if not nx_exists(target) then
		target = gm;
	end

    local x, y, z, orient = nx_get_position(target);
    x = nx_float(arg[1]);
    z = nx_float(arg[2]);
    floor = nx_int(arg[3]);
    
    y = nx_get_height(x, z, floor);
    if nx_moveto(gm, x, y, z, orient) then
       return gm_helper.succeed();
    else
		return gm_helper.error("move failed.");
    end
end

--��ѯGM��ǰλ����Ϣ
function x_pos(gm)
	local target = get_prop_target(gm);
	if not nx_exists(target) then
		target = gm;
	end
	
    local x, y, z, orient, col, row = nx_get_position(target);
    local height = nx_get_height(x, z, 0);

	local pos_ = {};
	pos_.x = x;
	pos_.y = y;
	pos_.z = z;
	pos_.r = row;
	pos_.c = col;
	pos_.o = orient;
	pos_.h = height;
    
	return gm_helper.content(pos_);
end


------------------------------------------------------------------------
-- ��������
--�����������complete_task taskid
function x_complete_task(gm, ...)
	if table.getn(arg) == 0 then
		return gm_helper.error("format:x_complete_task <taskid>");
	end

	local taskid = nx_int(arg[1])
	local target = get_prop_target(gm)
	if not nx_exists(target) then
		return gm_helper.error( "not found target!" );
	end
	if nx_complete_task(target, taskid) then
		return gm_helper.succeed();
	else
		return gm_helper.error("complete_task failed!");
	end
end

--��������accept_task taskid
function x_accept_task(gm, ...)
	if table.getn(arg) == 0 then
		return gm_helper.error("format: x_accept_task <taskid>");
	end

	local taskid = nx_int(arg[1])
	local target = get_prop_target(gm)
	if not nx_exists(target) then
		return gm_helper.error("not found target!");
	end

	if nx_accept_task(target, taskid) then
		return gm_helper.succeed();
	else
		return gm_helper.error("accept_task failed!");
	end
end

--��������giveup_task taskid
function x_giveup_task(gm, ...)
	if table.getn(arg) == 0 then
		return gm_helper.error("format: giveup_task <taskid>");
	end

	local taskid = nx_int(arg[1])
	local target = get_prop_target(gm)
	if not nx_exists(target) then
		return gm_helper.error("not found target!");
	end

	if nx_giveup_task(target, taskid) then
		return gm_helper.succeed();
	else
		return gm_helper.error("giveup_task failed!");
	end
end


-----------------------------------------------------------------------------------------
-- �ʼ�����
-- ����ʼ�
function x_clear_all_mail(gm, ...)
	local target = get_prop_target(gm)
	if not nx_exists(target) then
		return gm_helper.error("not found target!");
	end
	
	if nx_clear_all_mail(gm) then
		return gm_helper.succeed();
	else
		return gm_helper.error("clear_all_mail failed!");
	end

end

-------------------------------------------------------------------------------------------
-- ��ɫ����
-- ��ֹ��½ ����� ������
function x_block_player_by_minute(gm, ...)
	if table.getn(arg) < 2 then
		return gm_helper.error("format: x_block_player_by_minute <name> <minute>");
	end

	local name = nx_widestr(arg[1]);
	local min = nx_int(arg[2]);
	if nx_block_player_minite(gm, name, min) then
		return gm_helper.succeed();
	else
		return gm_helper.error("block player failed!");
	end
end

-- ��ֹ��½ ����� �� �� �� ʱ �� ��
function x_block_player_util_date(gm, ...)
	if table.getn(arg) < 7 then
		return gm_helper.error("format: x_block_player_util_date <name> <year> <month> <day> <hour> <minute> <second>");
	end

	local name = nx_widestr(arg[1]);
	local y = nx_int(arg[2]);
	local m = nx_int(arg[3]);
	local d = nx_int(arg[4]);
	local h = nx_int(arg[5]);
	local min = nx_int(arg[6]);
	local s = nx_int(arg[7]);
	if nx_block_player(gm, name, y, m, d, h, min, s) then
		return gm_helper.succeed();
	else
		return gm_helper.error("execute failed.");
	end
end

-- ��ȡ������ߵ�½
function x_offline_player_login(gm, ...)
	if table.getn(arg) < 3 then
		return gm_helper.error("format: offline_player_login <name> <account>");
	end

	local name = nx_widestr(arg[1]);
	local account = nx_string(arg[2]);
	local time = nx_int(arg[3]);
	if (nx_offline_player_login(name, account, time)) then
		return gm_helper.succeed();
	else
		return gm_helper.error("offline_player_login failed!");
	end
end

--ǿ���������
function x_break_by_account(gm, ... )
	if table.getn(arg) < 1 then
		return gm_helper.error("format: x_break_by_account <account>");
	end

	local account = nx_string(arg[1]);
	if (nx_break_account(account)) then
		return gm_helper.succeed();
	else
		return gm_helper.error("breakbyaccount failed!");
	end
end


--------------------------------------------------------------
-- �����淨
--��ʱ�����Ϣ
function limit_gift_info( gm, ... )
    local ret = nx_limit_gift_info(gm);
	return gm_helper.succeed(ret);
end

--�������ȫ�ֱ�־λ
function x_reset_global(gm, ...)
	if table.getn(arg) < 1 then
        return gm_helper.error("format:reset_global <pos>");
    end

    local target = get_prop_target(gm)
    if not nx_exists(target) then
        return gm_helper.error("not found target");
    end

	local pos = nx_int(arg[1]);
    local value = nx_query_property(target, "PlayerGlobalFlag");
	local old_ = nx_int64(value);
	--local new_ = old_ & (~(1 << pos));
	-- todo: ��֧��λ����
	return gm_helper.content(old_);
end

--���û�����ʶ
function x_reset_action_mutex(gm, ...)
	local target = get_prop_target(gm);
	if not nx_exists(target) then
		return gm_helper.error("not found target!");
	end

	if reset_action_mutex(gm) then
		return gm_helper.succeed();
	else
		return gm_helper.error("reset_action_mutex failed!");
	end
end