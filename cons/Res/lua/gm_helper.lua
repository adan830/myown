local TIPSTYPE_GMINFO = 0;


--�����������complete_task taskid
function complete_task(gm, ...)
	if table.getn(arg) == 0 then
		nx_sysinfo(gm, TIPSTYPE_GMINFO, "gm_info", nx_widestr("Format: complete_task <taskid>"))
		return
	end

	local taskid = nx_int(arg[1])
	local target = get_prop_target(gm)
	if not nx_exists(target) then
		return;
	end
	if nx_complete_task(target, taskid) then
		nx_sysinfo(gm, TIPSTYPE_GMINFO, "gm_info", nx_widestr("Complete task " .. nx_string(taskid) .. "succeed."))
	else
		nx_sysinfo(gm, TIPSTYPE_GMINFO, "gm_info", nx_widestr("Complete task " .. nx_string(taskid) .. "failed."))
	end
end

--��������accept_task taskid
function accept_task(gm, ...)
	if table.getn(arg) == 0 then
		nx_sysinfo(gm, TIPSTYPE_GMINFO, "gm_info", nx_widestr("Format: accept_task <taskid>"))
		return
	end

	local taskid = nx_int(arg[1])
	local target = get_prop_target(gm)
	if not nx_exists(target) then
		return;
	end

	if nx_accept_task(target, taskid) then
		nx_sysinfo(gm, TIPSTYPE_GMINFO, "gm_info", nx_widestr("Accept task " .. nx_string(taskid) .. "succeed."))
	else
		nx_sysinfo(gm, TIPSTYPE_GMINFO, "gm_info", nx_widestr("Accept task " .. nx_string(taskid) .. "failed."))
	end
end

--��������giveup_task taskid
function giveup_task(gm, ...)
	if table.getn(arg) == 0 then
		nx_sysinfo(gm, TIPSTYPE_GMINFO, "gm_info", nx_widestr("Format: giveup_task <taskid>"))
		return
	end

	local taskid = nx_int(arg[1])
	local target = get_prop_target(gm)
	if not nx_exists(target) then
		return;
	end

	if nx_giveup_task(target, taskid) then
		nx_sysinfo(gm, TIPSTYPE_GMINFO, "gm_info", nx_widestr("Giveup task " .. nx_string(taskid) .. "succeed."))
	else
		nx_sysinfo(gm, TIPSTYPE_GMINFO, "gm_info", nx_widestr("Giveup task " .. nx_string(taskid) .. "failed."))
	end
end

-- ����ʼ�
function clear_all_mail(gm, ...)

	nx_clear_all_mail(gm);
	nx_sysinfo(gm, TIPSTYPE_GMINFO, "gm_info", nx_widestr("clear_all_mail "  .. " ok."));

end

-- ��ֹ��½ ����� ������
function BlockPlayerMin(gm, ...)
	if table.getn(arg) < 2 then
		nx_sysinfo(gm, TIPSTYPE_GMINFO, "0", nx_widestr("blockplayer failed"));
		return;
	end

	local name = nx_widestr(arg[1]);
	local min = nx_int(arg[2]);
	if (nx_block_player_minite(gm, name, min)) then
		nx_sysinfo(gm, TIPSTYPE_GMINFO, "0", nx_widestr("blockplayer succeed"));
	else
		nx_sysinfo(gm, TIPSTYPE_GMINFO, "0", nx_widestr("blockplayer failed"));
	end
end

-- ��ֹ��½ ����� �� �� �� ʱ �� ��
function BlockPlayerDate(gm, ...)
	if table.getn(arg) < 7 then
		nx_sysinfo(gm, TIPSTYPE_GMINFO, "0", nx_widestr("blockplayer failed"));
		return;
	end

	local name = nx_widestr(arg[1]);
	local y = nx_int(arg[2]);
	local m = nx_int(arg[3]);
	local d = nx_int(arg[4]);
	local h = nx_int(arg[5]);
	local min = nx_int(arg[6]);
	local s = nx_int(arg[7]);
	if (nx_block_player(gm, name, y, m, d, h, min, s)) then
		nx_sysinfo(gm, TIPSTYPE_GMINFO, "0", nx_widestr("blockplayer succeed"));
	else
		nx_sysinfo(gm, TIPSTYPE_GMINFO, "0", nx_widestr("blockplayer failed"));
	end
end

-- ���ü�����״̬�Ƿ��쳣
function CheckPlayerState(gm, ...)

	local funcName = nx_string("nx_check_player_state");
	local tips = nx_widestr("CheckPlayerState failed");

	if table.getn(arg) == 1 then
		nx_dispatch_to_member(gm, funcName, nx_int(arg[1]));
		tips = nx_widestr("CheckPlayerState succeed");
	elseif table.getn(arg) == 4 then
		nx_dispatch_to_member(gm, funcName, nx_int(arg[1]), nx_int(arg[2]), nx_double(arg[3]), nx_double(arg[4]));
		tips = nx_widestr("CheckPlayerState succeed");
	end

	nx_sysinfo(gm, TIPSTYPE_GMINFO, "0", tips);

end

--ǿ���������
function breakbyaccount(gm, ... )
	if table.getn(arg) < 1 then
		nx_sysinfo(gm, TIPSTYPE_GMINFO, "0", nx_widestr("breakbyaccount account"));
		return;
	end

	local account = nx_string(arg[1]);
	if (nx_break_account(account)) then
		nx_sysinfo(gm, TIPSTYPE_GMINFO, "0", nx_widestr("breakbyaccount succeed"));
	else
		nx_sysinfo(gm, TIPSTYPE_GMINFO, "0", nx_widestr("breakbyaccount failed"));
	end
end

--���û�����ʶ
function reset_action_mutex(gm, ...)

	local target = get_prop_target(gm);
	if not nx_exists(target) then
		return;
	end

	reset_action_mutex(gm);
	nx_sysinfo(gm, TIPSTYPE_GMINFO, "gm_info", nx_widestr("reset_action_mutex "  .. " succeed"));
end

--�����ظ�����������
function clear_repeat_arena(gm, ...)
	nx_clear_arena_repeatinfo();
end
