require("gm_x_util")
local gm_helper = require("gm_x_helper")

--��ѯ��ǰ���Բ����Ķ���������Ӷ���
function x_child(gm, ...)
    local target = get_prop_target(gm)
    if not nx_exists(target) then
        return gm_helper.error( "not found target" );
    end

	local script = "";
	if table.getn(arg) > 1 then
		script = nx_string(arg[1]);
	end

	local result = {};
	local childlist = nx_get_child_list(target, 0);
	for i = 1, table.getn(childlist) do
		if string.len(script) == 0 or nx_get_script(childlist[i]) == script then
			local data = {};
			data["config"] = nx_get_config(childlist[i]);
			data["name"] = nx_query_property(childlist[i], "Name");
			data["uid"] = nx_query_property(childlist[i], "Uid");
			data["script"] = nx_get_script(childlist[i]);
			table.insert( result, data );
		end
	end

	return gm_helper.content(result);
end

--��ѯ��ǰ���Բ����Ķ���Ĵ����ű��������ļ�����
function x_see(gm)

    local target = get_prop_target(gm)
    if not nx_exists(target) then
        return gm_helper.error("not found target");
    end

	local data = {};
	data["script"] = nx_get_script(target);
	data["config"] = nx_get_config(target);
	data["persistid"] = nx_string(target);
	
	return gm_helper.content(data);
end