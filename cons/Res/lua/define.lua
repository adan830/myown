TYPE_SCENE	= 1;	-- ����
TYPE_PLAYER	= 2;	-- ���
TYPE_NPC	= 4;	-- NPC
TYPE_ITEM	= 8;	-- ��Ʒ
TYPE_HELPER	= 16;	-- ��������

--������������
VTYPE_UNKNOWN = 0;	-- δ֪
VTYPE_BOOL = 1;		-- ����
VTYPE_INT = 2;		-- 32λ����
VTYPE_INT64 = 3;	-- 64λ����
VTYPE_FLOAT = 4;	-- �����ȸ�����
VTYPE_DOUBLE = 5;	-- ˫���ȸ�����
VTYPE_STRING = 6;	-- �ַ���
VTYPE_WIDESTR = 7;	-- ���ַ���
VTYPE_OBJECT = 8;	-- �����
VTYPE_POINTER = 9;	-- ָ��
VTYPE_USERDATA = 10;-- �û�����
VTYPE_TABLE = 11;	-- ��
VTYPE_MAX = 12;

--��ʾ��Ϣ����
TIPSTYPE_SYSTEMINFO = 0;	--ϵͳ��Ϣ(����ģ�鷢�͵���ʾ��Ϣ�ã��߼�ģ�鲻Ҫʹ��)
TIPSTYPE_SYSTEMNO = 1;		--ϵͳ��ʾ(�ͻ�����ʾ��[��ʾ]ǰ׺)
TIPSTYPE_TIPSINFO = 2;		--���������Ϲ�������ʾ��Ϣ
TIPSTYPE_GMINFO = 99;		--GM������Ϣ

-- ת����ʽ
function nx_to_type(vtype, value)

	if vtype == VTYPE_INT then
		return nx_int(value);
	elseif vtype == VTYPE_INT64 then
		return nx_int64(value);
	elseif vtype == VTYPE_FLOAT then
		return nx_float(value);
	elseif vtype == VTYPE_DOUBLE then
		return nx_double(value);
	elseif vtype == VTYPE_STRING then
		return nx_string(value);
	elseif vtype == VTYPE_WIDESTR then
		return nx_widestr(value);
	elseif vtype == VTYPE_OBJECT then
		return nx_objet(value);
	end

end
