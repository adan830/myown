-- gm��������壬�ɹ�����"ok" �������ڲ���֧���������ݣ����ַ���doubleֵ

-- ����Ҫ���֧�ֵ�����
local rule_no_player={
	x_reload_config=true,		-- ��������
	x_get_config_prop=true,		-- ��ȡ��Ʒ����
	x_servertime=true,			-- ������ʱ��
	x_pub_rec_data=true,		-- ȡ�������ݱ���������
	x_pub_rec_findrow=true,		-- ���ҹ������ݱ������к�
	x_pub_rec_clear=true,		-- ����������ݱ�
	x_pub_rec_addrow=true,		-- ��ӹ���������
	x_pub_rec_removerow=true,	-- �Ƴ��������ݱ���
	x_pub_rec_set_value=true,	-- ���ù������ݱ��ֶ�ֵ
	x_pub_rec_get_value=true,	-- ��ȡ�������ݱ��ֶ�ֵ
	x_pub_prop_query=true,		-- ��ѯ������������ֵ
	x_pub_prop_set=true,		-- ���ù�����������ֵ
	x_reload_lua=true,			-- ����lua�ű�[����Ҫ��չ��]
	reload_lua=true,			-- ����lua�ű�[�ļ���Ҫ��.lua]
	x_import_robot_data=true,
	x_repair_pub_rec_data=true,
	x_repair_arena_rank_list14=true,
};
function run_no_player(cmd)
	if rule_no_player[cmd] == true then 
		return "ok";
	end
	
	return "no";
end

-- ��������ִ�е�����
local rule_offline = {
	x_offline_player_login = true
};
function run_offline(cmd)
	if rule_offline[cmd] == true then 
		return "ok";
	end
	
	return "no";
end

print(run_no_player("gm_x_reload_lua"));