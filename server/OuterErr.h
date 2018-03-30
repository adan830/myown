//--------------------------------------------------------------------
// �ļ���:		OuterErr.h
// ��  ��:		ͨ�÷��������棬�ͻ��˺ͷ�����ͨѶ�Ĵ������
// ˵  ��:		
// ��������:	2008��9��16��
// ������:		½����
// ��Ȩ����:	������ţ�������޹�˾
//--------------------------------------------------------------------

#ifndef _SERVER_OUTERERR_H
#define _SERVER_OUTERERR_H

// ��������Ϣ��ʽ [5λ������]
// ��1λ����ϵͳ����
// ��2λ����ģ�����
// ��3λ������(0-�洢���̡�1-�������2-����ͨ��)
// ����λΪ˳��ű���
// 1000Ϊ����Ԥ֪�Ĵ���

#define OUTER_RESULT_SUCCEED		1		// ����ɹ�
#define OUTER_RESULT_FAILED			0		// ���ʧ��

// 1000Ϊ����Ԥ֪�Ĵ���
#define OUTER_ERR_UNEXPECT			1000	// ��Ԥ�ڴ���

// ��Ϸ��������ش���������[2xxxx]

// ����ӿ���ش���[20xxx]

#define MGS_ERR_VERSION				            20101	// ����ͨѶЭ��汾����
#define MGS_ERR_REPEAT_LOGIN		          20102	// ͬһ���ʺŲ����ظ���½
#define MGS_ERR_LAND_SCENE			          20103	// Ҫ����ĳ���δ���Ż��ѹر�
#define MGS_ERR_NO_CONNECT			          20104	// ��������
#define MGS_ERR_BLOCK_MOMENT		          20105	// ��һ��ʱ���ڽ�ֹ��½
#define MGS_ERR_TIME_OUT			            20106	// ��¼��ʱ
#define MGS_ERR_QUEUE_FULL			          20107	// ��¼������
#define MGS_ERR_STORAGE_FAILED		        20108	// ���������ɫ���ݿ�����ӹ���
#define MGS_ERR_CHAREGE_FAILED		        20109	// ��������Ʒѷ����������ӹ���
#define MGS_ERR_SWITCH_SCENE		          20110	// Ҫ�л��ĳ���������
#define MGS_ERR_ACCOUNT_LOGGED		        20111	// ���ʺ��Ѿ���½����Ϸ������
#define MGS_ERR_ACCOUNT_INVALID		        20112	// �ʺ������а����Ƿ��ַ�
#define MGS_ERR_CHARGE_BREAK		          20113	// ����ò����Ѿ���ϵͳ�߳�
#define MGS_ERR_PLAYER_FULL			          20114	// ���������Ѿ��ﵽ����������
#define MGS_ERR_NO_ANY_SCENE		          20115	// û�п�ʹ�õĳ���
#define MGS_ERR_KICK_OUT			            20116	// ��ϵͳ������,���������������
#define MGS_ERR_IN_PROCESSING		          20117	// ���ڽ��д�����
#define MGS_ERR_CANT_PROCESS		          20118	// �޷���������
#define MGS_ERR_MESSAGE_ILLEGAL		        20119	// ����ʹ�����׳�������������ͷǷ���Ϣ
#define MGS_ERR_ACCOUNT_IN_GAME		        20120	// ���ʺ��Ѿ�������Ϸ
#define MGS_ERR_CANT_REPLACE		          20121	// ʹ���ܱ�����¼�Ĳ��ܱ�������ͬIP����
#define MGS_ERR_SUPERVISOR_KICK_OUT	      20122	// ������Ա������
#define MGS_ERR_SUPERVISOR_LOGGED	        20123	// ��¼����Ϊ����Ա�����ܱ�������
#define MGS_ERR_DENY_ENTRY_SCENE	        20124	// ��ǰ��������������볡��
#define MGS_ERR_CAN_NOT_RECONNECT         20125   // ���˺��޷�ʹ�üƷ���֤���Զ�����
#define MSG_ERR_ROOM				              20126	// �����쳣
#define MSG_ERR_TRY_LOGIN			            20127	// ���Ե����µ�¼���������ȴ�ս�������



// �洢�ӿ���ش���[21xxx]

#define MGS_ERR_ROLE_DUPLICATE		        21101	// ��������
#define MGS_ERR_ROLE_PROTECT		          21102	// �����ڱ������ڣ�����ɾ��
#define MGS_ERR_ROLE_SAVE_FAIL		        21103	// �������ʧ��
#define MGS_ERR_ROLE_DATA			            21104	// �������ݴ���
#define MGS_ERR_ROLE_TOO_MANY		          21105	// �����ܵ�������������
#define MGS_ERR_ROLE_INDEX			          21106	// ����λ�ô���
#define MGS_ERR_ROLE_NOT_FOUND		        21107	// ����û�ҵ�
#define MGS_ERR_NAME_REPEAT			          21108	// �����ظ�
#define MGS_ERR_NAME_INVALID		          21109	// ���ְ����Ƿ��ַ�
#define MGS_ERR_NAME_EXISTS			          21110	// �������Ѵ���
#define MGS_ERR_STORAGE_BUSY		          21111	// ���ݿ�æ���޷���������
#define MGS_ERR_CHARGE_BUSY			          21112	// �Ʒ�ϵͳæ���޷���������
#define MGS_ERR_LETTER_TOO_MANY		        21113	// �ż�������������
#define MGS_ERR_NO_ROLE_UID			          21114	// ��������ʱ�����Դ����
#define MGS_ERR_SWITCH_NO_SERVER	        21115	// ����ת��ʱ������������
#define MGS_ERR_SWITCH_CAN_NOT		        21116	// �˷���������ִ������ת��
#define MGS_ERR_ROLE_TOO_BIG		          21117	// ����������ݳ������������
#define MGS_ERR_STATUS_CHANGED		        21118	// ת��ʱĿ���������״̬�����ı�
#define MGS_ERR_SET_LAND_SCENE		        21119	// �½�����ʱδ���õ�½����
#define MGS_ERR_NEW_ROLE_FAILED		        21120	// �½�����ʱ��������ʧ��
#define MGS_ERR_ROLE_BLOCKED		          21121	// ����������һ��ʱ���ڱ����
#define MGS_ERR_LOGON_FAILED		          21122	// �Ʒѵ�½ʧ��
#define MGS_ERR_LOAD_ROLE_FAILED	        21123	// ������������ʧ��
#define MGS_ERR_GET_ROLES_FAILED	        21124	// ��ȡ��ɫ�б�ʧ��
#define MGS_ERR_CREATE_ROLE_FAILED	      21125	// ������������ʧ��
#define MGS_ERR_DELETE_ROLE_FAILED	      21126	// ɾ����������ʧ��
#define MGS_ERR_RECEIVER_NOT_FOUND	      21127	// �����˲�����
#define MGS_ERR_ROLE_IN_USE			          21128	// ��ɫ����ʹ����
#define MGS_ERR_VALIDATE_FAILED		        21129	// ��֤�ʺ�ʧ��
#define MGS_ERR_CONTINUE_FAILED		        21130	// �����¼ʧ��
#define MGS_ERR_ROLE_LOCKED			          21131	// ��ɫ����ʱ����
#define MGS_ERR_DENY_CREATE_ROLE	        21132	// ��ֹ����������
#define MGS_ERR_ACC_NOT_IS_OWNER	        21133	// ��ɫ�����ڸ��˺�
#define MGS_ERR_CROSS_STATUS_ERROR	      21134	// ��ɫ����״̬����
#define MGS_ERR_CROSSING			            21135	// ��ɫ���ݿ���в��ܵ�¼
#define MGS_ERR_CROSS_NO_SERVER		        21136	// ��ɫ���ʱĿ�������������
#define MGS_ERR_ROLE_DATA_ERROR		21137	// ��ɫ�����쳣
#define MSG_ERR_CHOOSE_OTHER_OS_ROLE			 21138	// �ý�ɫ�󶨵�ƽ̨�����޷���½��
#define MGS_ERR_CONTINUE_OTHER_OS_ROLE		        21139	// ����ƽ̨��ɫ���ɶ���

// ս��������ش���[22xxx]

#define MGS_ERR_ROOM_INNER					      22000	// �������ڲ�����
#define MGS_ERR_ROOM_CLOSE					      22001	// ����������Ѿ��ر�
#define MSG_ERR_PLAYER_NOT_FOUNND			    22002	// ��Ҳ�����
#define MSG_ERR_PLATFORM_CLOSE				    22003	// ƽ̨�������Ѿ��ر�
#define MSG_ERR_BATTLE_OVER					      22004	// ս���Ѿ�����
#define MSG_ERR_WORLD_REGISTED				    22005	// world id �Ѿ���ע��

// �Ʒ�Ӧ����ش���������

#define CAS_ERR_CONNECT_TIMEOUT						1000	 //���ӳ�ʱ,���Ժ�����
#define CAS_ERR_OPERATE_ERR								10002	 //�����쳣
#define CAS_ERR_PARAM_NOT_ENOUGH					1001	 //ȱ�ٲ������������ȷ
#define CAS_ERR_TABLE_NOT_EXIST						1002	 //�����ͼ������
#define CAS_ERR_JOINT_OPERATION_LOGIN_URL_FAILURE						1003	 //������Ӫ����¼ URL��ʱ�䳬��ʧЧ
#define CAS_ERR_JOINT_OPERATION_LOGIN_STRING_FAILURE        1004	 //������Ӫ����¼ У�鴮����ȷ
#define CAS_ERR_SQL_PROCEDURE_ERR					1005	 //���ݿ���̵���ʧ��

// ������ע�ᡢע�� 50xxx
#define CAS_ERR_VALIDATE_FAILED           50000	//�ʺ���֤ʧ�ܣ����Ժ����� (��Ҫ��½����������ע��ӿ����ע�����)
#define CAS_ERR_SERVER_ID			            50001	// ��Ϸ������ID����ʶ��
#define CAS_ERR_SERVER_NOT_EXIST          50002	// ��Ϸ�����������ڻ��ѽ���
#define CAS_ERR_SERVER_FORBID		          50003	// ��Ϸ������״̬Ϊ�ѽ���
#define CAS_ERR_SERVER_IP			            50004	// ��Ϸ������ע��IP����ȷ
//#define CAS_ERR_GAME_ID				            50004	// ��Ϸ����ID����ȷ

// ����ʺŵ�¼���۵㡢�˳� 51xxx
#define CAS_ERR_NO_ACCOUNT			          51001	// ��ţͨ��֤������
#define CAS_ERR_ACCOUNT_PSWD_NOT_MATCH    51002 //�����ʺ���˷�������Ӫ�̲�ƥ�䣬�뵽��Ա���Ĳ�ѯ����ѯ�ͷ�
#define CAS_ERR_ACCOUNT_BLOCK		          51003	// ���ʺ��ѱ�����
#define CAS_ERR_ACCOUNT_ONLY		          51004	// �ʺ�Ϊ����״̬������ϵ��Ա���Ľ���������
#define	CAS_ERR_ILLEGAL_GM			          51005	// �Ƿ�GM�ʺŵ�¼
#define CAS_ERR_ACCOUNT_PSWD		          51006	// ��¼�ʺŻ����벻��ȷ

#define CAS_ERR_DYNAMIC_PSWD_PROTECT      51007	// ���ʺ��ܶ�̬������񱣻�����ѡ��̬�����¼��ʽ
#define	CAS_ERR_ACCOUNT_NOT_DYNAMIC_PSWD	51008	// �����ʺŲ�δ�󶨶�̬�������δ��ͨ������ѡ����ͨ��ʽ��¼
#define CAS_ERR_GAME_DISTRICT_NOT_EXIST   51009	// ����Ϸ����������
#define CAS_ERR_GAME_NOT_EXIST            51010	// ����Ϸ�����ڻ��ѽ���

//#define CAS_ERR_GAME_DISTRICT_NOT_ACTiVATE	51011	// ����Ϸ����δ������
#define CAS_ERR_ACCOUNT_LOGGED		51011	// ���ʺ����ڷ�����������������¼

#define CAS_ERR_GAME_NOT_ACTIVATE		      51012	// ����Ϸδ������
#define CAS_ERR_ACCOUNT_DISTRICT_NOT_ACTiVATE   51013	// ����Ϸ�����ʻ���δ����(���û����������û�г��ֵ)��),�뵽��Ա���Ĳ鿴�Լ��ܵ�¼��Щ��Ϸ������.
#define CAS_ERR_ACCOUNT_DISTRICT_LOGGED   51014 //���ʺ�������Ϸ������������������¼
#define CAS_ERR_DYNAMIC_PSWD_ERROR				51015	//��̬������󣬽�ֹ��¼
#define CAS_ERR_ACCOUNT_DYNAMIC_PSWD_OVERDUE	51016	//�����ʺŰ󶨵Ķ�̬����ѹ��ڣ������󶨻򻻿����ٵ�¼
#define CAS_ERR_GAME_DISTRICT_NO_POINTS 	51017	//����Ϸ�����ʻ�����
#define CAS_ERR_ACCOUNT_UPDATE_ERROR			51018	//���·����˻���Ϣʧ��
#define CAS_ERR_ADD_SUBSITE_LOGIN_STAT_ERR  		51019	//������Ϸ��վ�ʺŵ�¼ͳ����Ϣʧ��
#define CAS_ERR_SUBSITE_LOGIN_STAT_ERR 		51020	//��¼��Ϸ��վ�ʺŵ�¼ͳ����Ϣʧ��
#define CAS_ERR_CREATE_LOGIN_LOG_ERR  	  51021	//������¼��־ʧ��
#define CAS_ERR_QUERY_ITEM_LOG_ERR			  51022	//��ѯ��Ϸ������Ϣʧ��[������߱��벻����]
#define CAS_ERR_ITEM_PRICE_NOT_MATCH			51023	//��Ϸ���߼۸�ƥ��[��Ʒ�������۷����仯,��ˢ��]
#define CAS_ERR_BUY_NUM_ERROR        			51024	//���������򵥼�����
#define CAS_ERR_ITEM_POINT_RATE_NOT_MATCH	51025	//��Ϸ���ߴ���ȯʹ�ñ��ʲ�ƥ��
#define CAS_ERR_ACCOUNT_NOT_POINT		      51026	//�˻�����[�Ż��ʻ�����]
#define CAS_ERR_ACCOUNT_GIVE_NOT_EXIST		51027	//�����͵��ʺŲ�����[δ֪�ʺ�����]
#define CAS_ERR_BUY_ITEM_LOG_ERROR				51028	//�̳��ڹ��������־��¼ʧ��
#define CAS_ERR_DEDUCT_POINT_ERROR				51029	//�۳��˻����ʧ��[�㿨����֮ǰ���۳����ѳ���]
#define CAS_ERR_CARD_LOG_INSERT_ERROR			51030	//���۵㿨��¼����ʧ��
#define CAS_ERR_DISTRICT_ACCOUNT_RECEIPTS_ERR		51031	//�����˻�����ʧ��
#define CAS_ERR_BUY_CARD_ERR		          51032	//������۵㿨ʧ��
#define CAS_ERR_BUY_CARD_LOG_INSERT_ERR		51033	//������Ϸ�е㿨���۹��������־��¼ʧ��
#define CAS_ERR_QUERY_LAST_LOGIN_ERR		  51034	//��ѯ�û�����¼��Ϣʧ��
#define CAS_ERR_QUERY_ACCOUNT_SECURITY_ERR	    51035	//��ѯ�ʺŰ�ȫ��Ϣʧ��
#define CAS_ERR_QUERY_SERVICE_SUBMIT_ERR  51036	//��ѯ�ͷ��ύ����ʧ��
#define CAS_ERR_QUERY_ACCOUNT_BASE_INFO_ERR	    51037	//��ѯͨ��֤������Ϣʧ��
#define CAS_ERR_QUERY_ELEC_CARD_INFO_ERR	51038	//��ѯ�����ȡ���ӿ����Ϣʧ��
#define CAS_ERR_WEEL_GETED_ELEC_CARD			51039	//һ������ȡ�����ӿ��
#define CAS_ERR_NO_AVAILABLE_ELEC_CARD		51040	//û�п��õĵ��ӿ��
#define CAS_ERR_GET_ELEC_CARD_ERR     		51041	//��ȡ���ӿ��ʧ��
#define CAS_ERR_QUERY_VIP_LEVEL_ERR				51042	//��ѯVIP����ʧ��,�޼�¼
#define CAS_ERR_RECORD_SUBSITE_LOGIN_LOG_ERR	  51043	//��¼��Ϸ��վ�ʺŵ�¼��־ʧ��
#define CAS_ERR_UPDATE_SUBSITE_LOGIN_LOG_ERR 		51044	//������Ϸ��վ�ʺŵ�¼��־ʧ��
#define CAS_ERR_CARD_NOT_FILL							51045	//�����δ��д
#define CAS_ERR_QUERY_ACCOUNT_DISTRICT_ACTIVATE_ERR 51046	//��ѯ���û�����������Ϣʧ��
#define CAS_ERR_QUERY_ACCOUNT_ACTIVATE_ERR			51047	//��ѯ���û���Ϸ������Ϣʧ��
#define CAS_ERR_QUERY_ACCOUNT_DISTRICT_ERR			51048	//�����˻���ѯʧ��
#define CAS_ERR_QUERY_UNREAD_PEPLY_ERR	  51049	//��ѯ�û�δ���ظ���Ϣʧ��
#define CAS_ERR_BUY_ITEM_GET_EXECUTE_ERR	51050	//�û���Ϸ�ڹ�����Ʒ��ȡ��ִִ��ʧ��
#define CAS_ERR_QUERY_BUY_ITEM_INFO_ERR		51051	//��ѯ�û������̳ǹ�����Ʒ����Ʒ��Ϣʧ��
#define CAS_ERR_GET_NUM_LESSTHAN_ZERO		  51052	//�û���ȡ����С��0
#define CAS_ERR_GET_BUY_ITEM_INFO_ERR			51053	//��ȡ��������̳ǹ�����Ʒ����Ʒ��Ϣʧ��
#define CAS_ERR_RECV_ITEM_UPDATE_ERR			51054	//�û��յ���Ʒ�Ļ�ִ����ʧ��
#define CAS_ERR_SUPER_PWSD_LOGIN_LON_ERR	51055	//��¼���������¼��־ʧ��
#define CAS_ERR_GAME_SERVER_HISTTORY_ONLINE_NUM_ERR	51056	//��¼��Ϸ��������ʷ��������ʧ��
#define CAS_ERR_CONSIGNMENT_CARD_UNDO    	51057	//���۵㿨�ѳ���
#define CAS_ERR_CONSIGNMENT_CARD_SOLD			51058	//���۵㿨���۳�
#define CAS_ERR_CONSIGNMENT_CARD_INFO_ERR	51059	//���۵㿨��Ϣ����
#define CAS_ERR_CONSIGNMENT_CARD_UNDO_2 	51060	//�㿨������Сʱ�ڲ��ɳ���
#define CAS_ERR_NOT_BUY_SELF_CARD		      51061	//�����˲��ܹ����Լ����۵ĵ㿨
#define CAS_ERR_ONLINE_NUM_STAT_ERR	      51062	//��Ϸ��������������ͳ��ʧ��
#define CAS_ERR_CONSIGNMENT_CARD_POINT_NOT_100 			51063	//���۵�����������100��
#define CAS_ERR_CONSIGNMENT_ACCOUNT_NOT_EXIT				51064	//�������ʺŲ�����
#define CAS_ERR_CONSIGNMENT_ITEM_NOT_EXIT	51065	//���۵ĵ��߲�����
#define CAS_ERR_CONSIGNMENT_ITEM_SOLD   	51066	//���۵ĵ������۳�
#define CAS_ERR_UPDATE_LOGIN_LOG_MAC_ERR	51067	//������Ϸ��¼��־MAC��ַʧ��
#define CAS_ERR_CURR_ONLINE_NOT_EXIST   	51068	//��ǰ���߼�¼������
#define CAS_ERR_ANTI_ADDICTION_UNABLE_GAME          51069	//�𾴵��û��������ڷ������ʺţ������ǲ�������Ϸʱ�䣬������Ϣ��5Сʱ�󷽿ɵ�½��Ϸ��
#define CAS_ERR_EXCHANGE_RATE_ERROR				51070	//�һ���������ȷ(1��ֻ�ܶһ�4�Ľ��ӻ���Ʊ)
#define CAS_ERR_EXCHANGE_POINT_TYPE_ERROR	51071	//�һ���Ϸ�������Ͳ���ȷ
#define CAS_ERR_ACOUNT_IN_SIM_PROTECT			51072	//���ʺ���SIM���ܱ����񱣻�����ѡ��SIM����ʽ��¼
#define CAS_ERR_ACOUNT_AND_SIM_NOT_BINDE	51073	//�����ʺŲ�δ��SIM������δ��ͨ������ѡ����ͨ��ʽ��¼
#define CAS_ERR_SIM_DYNAMIC_PSWD_ERROR  	51074	 //SIM����̬�����ȷ
#define CAS_ERR_ITEM_SOLD_OUT             51075	//����������¼�
#define CAS_ERR_ACTIVITY_NOT_STARTED  		51076	//�δ��ʼ
#define CAS_ERR_ACTIVITY_END							51077	//��ѽ���
#define CAS_ERR_TOTAL_NUM_UPPER_LIMIT			51078	//��������������
#define CAS_ERR_SINGLE_ACCOUNT_UPPER_LIMIT	  	51079	//���ʺ���������
#define CAS_ERR_CREATE_ACCOUNT_ERROR			51080	//����ʧ��, ��Ϸ��ɫ��/�������ѱ�ʹ��
#define CAS_ERR_EDITE_ACCOUNT_ERROR			  51081	//�޸�ʧ��, ��Ϸ��ɫ��/������������
#define CAS_ERR_DELETE_ACCOUNT_ERROR			51082	//ɾ��ʧ��, ��Ϸ��ɫ��/������������
#define CAS_ERR_ORDER_NUMBER_DISPOSED   	51083	//�˶������Ѵ����(������:�������ɵ�Ψһ��ʶ)
#define CAS_ERR_INTERFACE_STOP_USED 			51084	//�˽ӿ���ֹͣʹ�ã�����µ��°汾
#define CAS_ERR_EXCHANGE_POINT_ERROR			51085	//�һ���Ϸ����ʧ��
#define CAS_ERR_DISTRICT_TEST_NOT_BELONG	51086	//�Բ���,�������ڱ��εĲ�������,�޷�������Ϸ��½
#define CAS_ERR_LOGIN_LIMITED 						51087	//��¼����,����ϵ�ͷ�����
#define CAS_ERR_ACCOUNT_TOKEN_PROTECT_OVERDUE			51088	//�����ʺŰ󶨵������ܱ��ѹ��ڣ������󶨻�������ٵ�¼
#define CAS_ERR_ACCOUNT_TOKEN_NEED_VERIFY	51089	//�����ʺŰ󶨵������ܱ���ҪЧ�飬�뵽��Ա���Ĳ���
#define CAS_ERR_TOKEN_PWSD_ERROR					51090	//���������ܱ����󣬽�ֹ��¼
#define CAS_ERR_ACCOUNT_TOKEN_PROTECT			51091	//���ʺ��������ܱ����񱣻�����ѡ�������ܱ���ʽ��¼
#define CAS_ERR_ACCOUNT_TRANSFER_LOCK			51092	//�ʺ�Ϊת������״̬�����Ժ�����
#define CAS_ERR_GAME_PERMISSION_NOT_ACTIVATE			51093	//���ڴ˷���������ϷȨ����δ����,����jhmҳ����м���
#define CAS_ERR_ACCOUNT_PHONE_TOKEN_PROTECT			  51094	//���ʺ����ֻ������ܱ����񱣻�����ѡ���ֻ������ܱ���ʽ��¼
#define CAS_ERR_ACCOUNT_PHONE_TOKEN_PROTECT_OVERDUE			51095	//�����ʺŰ󶨵��ֻ������ܱ��ѹ��ڣ������󶨻�������ٵ�¼
#define CAS_ERR_ACCOUNT_PHONE_TOKEN_NEED_VERIFY   51096	//�����ʺŰ󶨵��ֻ������ܱ���ҪЧ�飬�뵽��Ա���Ĳ���
#define CAS_ERR_PHONE_TOKEN_PWSD_ERROR		51097	//�����ֻ������ܱ����󣬽�ֹ��¼
#define CAS_ERR_MONTH_CARD_OPERATE_ERROR  51098	//����ʧ�ܣ���������ʱ����ڽ�������ʱ��
#define CAS_ERR_SSO_VERIFY_ERROR        	51101	//SSO ��֤ʧ�ܣ������µ�¼(��ʧЧ������ˢ��)
#define CAS_ERR_ACCOUNT_NOT_SECURITY			51990	//�����˻�����ȫ�� ����ȥXXXX�޸�����
#define CAS_ERR_ACCOUNT_PARENTS_MONITOR		51999	//�����ʺ�������ҳ���أ��˺ű���ͣ

#define APP_CLIENT_VERSION_TOOLOWER			90001	// �ͻ��˰汾����
#endif // _SERVER_OUTERERR_H

