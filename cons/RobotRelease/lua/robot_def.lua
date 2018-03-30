-- �ļ���: robot_def.lua
-- ˵��: common const define
-- ����:

-- ȫ�ֺ���
-- ����
-- ex_msg_box( string title, string content)
-- ת������ֵ����
-- ex_number( var )
-- ת�����ַ���
-- ex_string( var )

-- ���� CBytes ����
-- bytes()
-- CBytesд�ֽ�(��������д����)
-- tb_bytes.write_char(tb_bytes,char_val)
-- CBytes��ȡ�ֽ�(�������Ͷ�ȡ����)
-- tb_bytes.read_char(tb_bytes)

----------------------------------------------------------------
-- robot �������
----------------------------------------------------------------
-- CHARGE_URL = "http://123.207.7.182:8080/v1/tickets"
-- REGISTER_URL = "http://123.207.7.182:7080/acc/regist"
-- TRANS_URL = "http://123.207.7.182:7080/restful/acc/third/conver"

-- Charge,Register,Trans Ո��url
TRANS_URL = "http://192.168.9.8:7080/restful/acc/third/conver"
CHARGE_URL = "http://192.168.9.8:11000/api/v1/login"
REGISTER_URL = "http://192.168.9.8:11000/api/v1/login"

-- charge ���ã�group,server_id, ������Ĭ��Ϊ"1","1".����ȡֵ����ѯ����ά��Ա��лл
CHARGE_GROUP = "1"
CHARGE_SERVER_ID="1"

--�����Ʒ�
-- TRANS_URL = "http://123.207.74.46:10000/restful/acc/third/conver"
-- CHARGE_URL = "http://123.207.74.46:10000/v1/login"
-- REGISTER_URL = "http://123.207.74.46:10000/v1/acc/"

----------------------------------------------------------------
-- ROBOT���Ʋ��� ��ʵʱ����
----------------------------------------------------------------
ROBOT_PREFIX_ACCOUNT = "srobot"       --�ʺ�ǰ׺
ROBOT_PREFIX_NAME    = "brobot"         --����ǰ׺

ROBOT_ENTRY_IP   = "127.0.0.1"        -- Entry������ip��ַ
ROBOT_ENTRY_PORT = 2001               -- Entry�������˿�,Ĭ�J2001
ROBOT_LOGIN_TYPE = 1           		  -- 1,ʹ�ò����˺ſ��¼; 2,ʹ��xy�Ʒѵ�½


ROBOT_MODE_TYPE_SYNC = true              -- ������ִ�з�ʽ�����������
ROBOT_MODE_TIME_RANDOM=false             -- ʱ�����
ROBOT_MODE_TIME_INTERVAL=1000            -- ʱ��������λms��Ĭ��1000
ROBOT_MODE_TIME_INTERVAL_ADDIT=0         -- ʱ�丽�ӣ���λms��Ĭ��1000

----------------------------------------------------------------
-- ROBOT���Ʋ��� �����ļ�
----------------------------------------------------------------
ROBOT_TASK_IS_WORKING = false            -- �C�����΄չ�����
ROBOT_MOVE_IS_WORKING = false            -- �C�������߹�����

ROBOT_MOVE_VERIFY_TARGET_RANGE=true      -- �C�������ߣ��Ƿ�У����ߵ�Ŀ�˹�����
ROBOT_MOVE_VALID_TARGET_RANGE=20.0       -- �C�������ߵ��_Ŀ�˹����ȵ��L��

----------------------------------------------------------------
-- ROBOT���a���Å���
----------------------------------------------------------------
ROBOT_PROCESS_CUSTOM_COUNT=20            -- Robot�Զ��x���̣��ˆΔ���
----------------------------------------------------------------
-- ROBOT��������
----------------------------------------------------------------
CHECK_AUTO_LOGIN_DISCONNECT_TIME = 5000	--����Զ���¼�����ߵ�Ƶ��
EXCEED_LOGIN_TIME = 5000				--��¼��ĳһ���ĳ�ʱʱ��
IS_RANDOM_TYPE = false					-- ʹ���������

----------------------------------------------------------------
-- ��������Ϣid����
----------------------------------------------------------------
STOC_LOGIN_SUCCEED = 0
STOC_PROPERTY_TABLE = STOC_LOGIN_SUCCEED+1
STOC_RECORD_TABLE = STOC_PROPERTY_TABLE+1
STOC_ENTRY_SCENE = STOC_RECORD_TABLE+1
STOC_EXIT_SCENE = STOC_ENTRY_SCENE+1
STOC_ADD_OBJECT = STOC_EXIT_SCENE+1
STOC_REMOVE_OBJECT = STOC_ADD_OBJECT+1
STOC_SCENE_PROPERTY = STOC_REMOVE_OBJECT+1
STOC_OBJECT_PROPERTY = STOC_SCENE_PROPERTY+1
STOC_CREATE_VIEW = STOC_OBJECT_PROPERTY+1
STOC_DELETE_VIEW = STOC_CREATE_VIEW+1
STOC_VIEW_PROPERTY = STOC_DELETE_VIEW+1
STOC_VIEW_ADD = STOC_VIEW_PROPERTY+1
STOC_VIEW_REMOVE = STOC_VIEW_ADD+1
STOC_VIEW_CHANGE = STOC_VIEW_REMOVE+1
STOC_RECORD_ADD_ROW = STOC_VIEW_CHANGE+1
STOC_RRECORD_DEL_ROW = STOC_RECORD_ADD_ROW+1
STOC_RECORD_GRID = STOC_RRECORD_DEL_ROW+1
STOC_RECORD_CLEAR = STOC_RECORD_GRID+1
STOC_SPEECH = STOC_RECORD_CLEAR+1
STOC_SYSTEM_INFO = STOC_SPEECH+1
STOC_MENU = STOC_SYSTEM_INFO+1
STOC_CLEAR_MENU = STOC_MENU+1
STOC_CUSTOM = STOC_CLEAR_MENU+1
STOC_LOCATION = STOC_CUSTOM+1
STOC_MOVING = STOC_LOCATION+1
STOC_ALL_DEST = STOC_MOVING+1
STOC_WARNING = STOC_ALL_DEST+1
STOC_FROM_GMCC = STOC_WARNING+1
STOC_ALL_PROP = STOC_FROM_GMCC+1
STOC_ADD_MORE_OBJECT = STOC_ALL_PROP+1
STOC_REMOVE_MORE_OBJECT = STOC_ADD_MORE_OBJECT+1

STOC_SERVER_INFO = 39
STOC_SET_VERIFY = STOC_SERVER_INFO+1
STOC_SET_ENCODE = STOC_SET_VERIFY+1
STOC_ERROR_CODE = STOC_SET_ENCODE+1
STOC_WORLD_INFO = STOC_ERROR_CODE+1
STOC_IDLE = STOC_WORLD_INFO+1
STOC_QUEUE = STOC_IDLE+1
STOC_TERMINATE = STOC_QUEUE+1
STOC_LINK_TO = STOC_TERMINATE+1
STOC_UNLINK = STOC_LINK_TO+1
STOC_LINK_MOVE = STOC_UNLINK+1

----------------------------------------------------------------
-- �ͻ�����Ϣid����
----------------------------------------------------------------
CTOS_LOGIN = 0
CTOS_CREATE_ROLE = CTOS_LOGIN+1
CTOS_DELETE_ROLE = CTOS_CREATE_ROLE+1
CTOS_CHOOSE_ROLE = CTOS_DELETE_ROLE+1
CTOS_REVIVE_ROLE = CTOS_CHOOSE_ROLE+1
CTOS_WORLD_INFO = CTOS_REVIVE_ROLE+1
CTOS_READY = CTOS_WORLD_INFO+1
CTOS_CUSTOM = CTOS_READY+1
CTOS_REQUEST_MOVE = CTOS_CUSTOM+1
CTOS_SELECT = CTOS_REQUEST_MOVE+1
CTOS_SPEECH = CTOS_SELECT+1
CTOS_GET_VERIFY = CTOS_SPEECH+1
CTOS_RET_ENCODE = CTOS_GET_VERIFY+1

-- ��֤���ĳ���
VERIFY_MAX_LEN = 16;
SRV_VERSION=123456
VERITY_STRING = "0000000000000000"

--int���ֽ���
INT_SIZE=4

----------------------------------------------------------------
--�ͻ��˷��͵�����������Ϣ
----------------------------------------------------------------
--����GM�����ʽ��int msgid, wstring info
CLIENT_CUSTOMMSG_GM = 19


--����BOSS������Ϣ
CLIENT_CUSTOMMSG_WORLD_BOSS_ACTIVE = 238

--С��
CLIENT_CUSTOMMSG_QUIT_GAME = 500


----------------------------------------------------------------
--ROBOT����
----------------------------------------------------------------

----------------------------------------------------------------
--�������������ͻ��˵���Ϣ
----------------------------------------------------------------
--���Ϳ���ս����б�
-- int rank			����
-- string name		�����
-- int fight_power	ս����
-- int role_key		RoleKey
SERVER_CUSTOMMSG_ARENA_FIGHTER_LIST = 40

--ԭ�����ؿͻ�������
SERVER_CUSTOMMSG_ROUND_TRIP = 210

----------------------------------------------------------------
-- ��ͼ��ض���
----------------------------------------------------------------
VIEW_ID_EQUIP = 1                          -- װ����
VIEW_ID_TOOL = 2                           -- ������
VIEW_ID_DEPOT = 3                          -- �ֿ���
VIEW_ID_SKILL_CONTAINER = 4                -- ��������
VIEW_ID_ATTACK_CONTAINER = 5               -- ��ͨ������������
VIEW_ID_BUFFER = 6                         -- BUFFER����
VIEW_ID_GODS_BOX = 7                       -- ����װ������
VIEW_ID_GODS_BAG = 8                       -- ���鱳������
VIEW_ID_EQUIP_BAG = 9                      -- װ����������
VIEW_ID_JEWEL_BAG = 10                     -- ��ʯ��������
VIEW_ID_GODS_SOUL_BAG = 11                 -- �����������
VIEW_ID_SURPLUS_BAG = 12                   -- ��ʱ����
VIEW_ID_MAX = 12                           -- ��ͼid���ֵ