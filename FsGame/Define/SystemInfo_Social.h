//--------------------------------------------------------------------
// �ļ���:	SystemInfo_Sociale.h
// �� ��:	�罻ϵͳ��Ϣ����
// ˵ ��:		
// ��������:
// ������:	tzt    
//--------------------------------------------------------------------
#ifndef _SystemInfo_Social_H_
#define _SystemInfo_Social_H_


// SocialSystemModule �罻ϵͳ(17001-19000)����ϵͳ��ϸ���֡�
enum SystemInfo_Social
{	
#pragma region // ChatModule���죨17001-17100��
	SYSTEM_INFO_ID_17001 = 17001,	// �㱻�����ˣ��뷴˼һ���
	SYSTEM_INFO_ID_17002,			// ��Ϣ����ʧ��
	SYSTEM_INFO_ID_17003,			// ��ǰ��������������
	SYSTEM_INFO_ID_17004,			// ����Ƶ��������Ҫ����ȼ�15��
	SYSTEM_INFO_ID_17005,			// �㻹��Ҫ{0}������ڸ�Ƶ������
	SYSTEM_INFO_ID_17006,			// ���[{0}]������
	SYSTEM_INFO_ID_17007,			// ���[{0}]������
	SYSTEM_INFO_ID_17008,			// �������Լ�����
	SYSTEM_INFO_ID_17009,			// ��û�д�������У�����ʹ�ø�Ƶ����

#pragma endregion	

#pragma region // FriendModule���ѣ�17101-17300��
	SYSTEM_INFO_ID_17101 = 17101,	// ���������Ѿ�����
	SYSTEM_INFO_ID_17102,			// {0}�Ѿ����������ѹ�ϵ
	SYSTEM_INFO_ID_17103,			// {0}�ܾ�����ĺ�������
	SYSTEM_INFO_ID_17104,			// �Լ����ѱ���
	SYSTEM_INFO_ID_17105,			// {0}��������ĺ�������
	SYSTEM_INFO_ID_17106,			// �������ͳɹ�
	SYSTEM_INFO_ID_17107,			// �յ�����{0}������
	SYSTEM_INFO_ID_17108,           // ����{0}���Ǻ���
	SYSTEM_INFO_ID_17109,			// ���{0}����ɹ�
	SYSTEM_INFO_ID_17110,			// ���{0}��Ե�ɹ�
	SYSTEM_INFO_ID_17111,			// {0}�ܾ�����Ľ�������
	SYSTEM_INFO_ID_17112,			// {0}�ܾ�����Ľ�Ե����
	SYSTEM_INFO_ID_17113,			// ����ʧ��
	SYSTEM_INFO_ID_17114,			// ��Եʧ��
	SYSTEM_INFO_ID_17115,			// ���{0}����˽����ϵ
	SYSTEM_INFO_ID_17116,			// ���{0}����˽�Ե��ϵ
	SYSTEM_INFO_ID_17117,			// �����{0}���ܶ�����
	SYSTEM_INFO_ID_17118,			// ����б�����,�޷���ӳ���
	SYSTEM_INFO_ID_17119,			// �������ﲻ����
	SYSTEM_INFO_ID_17120,			// �Է��ں�������
	SYSTEM_INFO_ID_17121,			// �Լ�����������
	SYSTEM_INFO_ID_17122,			// Ѱ��Ԯ���ɹ�
	SYSTEM_INFO_ID_17123,			// ����{0}���Ǻ���
	SYSTEM_INFO_ID_17124,			// ����{0}�����ܶ��Ѵ����ޣ���ݺ�ɼ�������
	SYSTEM_INFO_ID_17125,			// ����{0}�����ܶ��Ѵ����ޣ��޷�����
	SYSTEM_INFO_ID_17126,			// {0}�Ѿ��Ǻ���
	SYSTEM_INFO_ID_17127,			// {0}�ں�������

#pragma endregion	
	
#pragma region // RequestModule������ģ�飨17301-17400��
	SYSTEM_INFO_ID_17301 = 17301,	// ���{0}�����ڻ�������, ����ʧ��
	SYSTEM_INFO_ID_17302,			// {0}�ܾ������{1}����
	SYSTEM_INFO_ID_17303,			// {0}�ܾ����ܴ�����
	SYSTEM_INFO_ID_17304,			// ���{0}��{1}�����ѳ�ʱ...
	SYSTEM_INFO_ID_17305,			// ������ʧЧ
	SYSTEM_INFO_ID_17306,			// ����[{0}]������������Ϣ���ȴ���Ӧ��������


#pragma endregion	

#pragma region // TeamModule���ģ�飨17401-17600��
	SYSTEM_INFO_ID_17401 = 17401,	// {0}�Ķ����������������ʧ��
	SYSTEM_INFO_ID_17402,			// ��ǰ������ֹ���
	SYSTEM_INFO_ID_17403,		    // PVPս���в����Բ�������
	SYSTEM_INFO_ID_17404,			// {0}�����ڻ����ߣ�����ʧ��
	SYSTEM_INFO_ID_17405,			// {0}�Ѿ��Ƕ����Ա�����跢���������
	SYSTEM_INFO_ID_17406,			// ���Ѿ����Ŷ���,���ܷ����������
	SYSTEM_INFO_ID_17407,			// ������ȼ�Ҫ��
	SYSTEM_INFO_ID_17408,			// ��ֹ���
	SYSTEM_INFO_ID_17409,			// �������������ܷ����������
	SYSTEM_INFO_ID_17410,			// {0}�����ߣ����ʧ��
	SYSTEM_INFO_ID_17411,			// ��������ⳡ��
	SYSTEM_INFO_ID_17412,			// {0}�Ѿ��ڶ����У����ܷ����������
	SYSTEM_INFO_ID_17413,			// {0}�Ѿ������������У����ܷ����������
	SYSTEM_INFO_ID_17414,			// {0}������ȼ�����
	SYSTEM_INFO_ID_17415,			// {0}�Ѿ����Ƕӳ�������������
	SYSTEM_INFO_ID_17416,			// �Է��������������ܷ����������
	SYSTEM_INFO_ID_17417,			// �Է�û�ж��飬����ʧ��
	SYSTEM_INFO_ID_17418,			// {0}�Ѿ��ж��飬���ʧ��
	SYSTEM_INFO_ID_17419,			// {0}�����������У��������ʧ��
	SYSTEM_INFO_ID_17420,			// ��û�����Ȩ��
	SYSTEM_INFO_ID_17421,			// {0}���Ƕ��ѣ������ƽ�
	SYSTEM_INFO_ID_17422,			// {0}�����ߣ��ƽ�ʧ�ܣ�
	SYSTEM_INFO_ID_17423,			// �齨����ɹ�
	SYSTEM_INFO_ID_17424,			// �������ɹ�
	SYSTEM_INFO_ID_17425,			// {0}�������
	SYSTEM_INFO_ID_17426,			// {0}�뿪����
	SYSTEM_INFO_ID_17427,			// �ӳ���{0}�Ӷ������߳�
	SYSTEM_INFO_ID_17428,			// �ӳ�����Ӷ������߳�
	SYSTEM_INFO_ID_17429,			// �����ɢ
	SYSTEM_INFO_ID_17430,			// ���˳��˶���
	SYSTEM_INFO_ID_17431,			// ���Ϊ�ӳ�
	SYSTEM_INFO_ID_17432,			// {0}��Ϊ�ӳ�
	SYSTEM_INFO_ID_17433,			// ����{0}����
	SYSTEM_INFO_ID_17434,			// �����Ѿ����Զ�ƥ���в��ܸ���Ŀ��
	SYSTEM_INFO_ID_17435,			// �ӳ�������
	SYSTEM_INFO_ID_17436,			// ����Ŀ���Ѹ��ģ��Զ�ȡ��ƥ��
	SYSTEM_INFO_ID_17437,			// �δ����

#pragma endregion
#pragma region // GuildModule ����ģ�飨17601-17800��
	SYSTEM_INFO_ID_17601 = 17601,	// �ð�������Ѵ�����
	SYSTEM_INFO_ID_17602,			// ��ְλ�����Ѵ����ޣ�ְλ�䶯ʧ��
	SYSTEM_INFO_ID_17603,			// ������ư���������Ż�Ƿ��ַ�
	SYSTEM_INFO_ID_17604,			// �˹���{0}������
	SYSTEM_INFO_ID_17605,			// ����Ѽ�����
	SYSTEM_INFO_ID_17606,			// ��������ַ�����
	SYSTEM_INFO_ID_17607,			// Ԫ������
	SYSTEM_INFO_ID_17608,			// ��������Ѵ�����
	SYSTEM_INFO_ID_17609,			// ��������ظ����޷�����	 
	SYSTEM_INFO_ID_17610,			// ��Ȩ����
	SYSTEM_INFO_ID_17611,			// ��Ȩ����
	SYSTEM_INFO_ID_17612,			// ���˹���,��������
	SYSTEM_INFO_ID_17613,			// �޸Ĺ���ɹ�
	SYSTEM_INFO_ID_17614,			// �޸Ĺ���ʧ��
	SYSTEM_INFO_ID_17615,			// �޸����Գɹ�
	SYSTEM_INFO_ID_17616,			// �޸�����ʧ��
	SYSTEM_INFO_ID_17617,			// {0}��֯�ܾ�������빫�������
	SYSTEM_INFO_ID_17618,			//  ��֯�����Ѵ���
	SYSTEM_INFO_ID_17619,			//  ��������������Ѵ�����
	SYSTEM_INFO_ID_17620,			//  �Ѿ�������ð��
	SYSTEM_INFO_ID_17621,			//  �ð�������б�����
	SYSTEM_INFO_ID_17622,			//  �ð�������Ѵ�����
	SYSTEM_INFO_ID_17623,			//  �����˻���ȴʱ�䣬�޷�������ߴ������
	SYSTEM_INFO_ID_17624,			//  ����ʧ�ܣ����������ѹ���
	SYSTEM_INFO_ID_17625,			//  ����ְλ��Ȩ��������
	SYSTEM_INFO_ID_17626,			//  ��������Ƿ��ַ�
	SYSTEM_INFO_ID_17627,			//  ��֯�ʽ���
	SYSTEM_INFO_ID_17628,			//  ���԰����Ƿ��ַ�
	SYSTEM_INFO_ID_17629,			//  ��Ȩ����
	SYSTEM_INFO_ID_17630,			//  �̵���Ʒ�������㹺��
	SYSTEM_INFO_ID_17631,			//  ��Ȩ��������
	SYSTEM_INFO_ID_17632,			//  �����������蹫��ȼ�����
	SYSTEM_INFO_ID_17633,			//  ���ﹱ����
	SYSTEM_INFO_ID_17634,			//  �����ѽ���
	SYSTEM_INFO_ID_17635,			//  ��ǰ��֯û�иü����޷�����
	SYSTEM_INFO_ID_17636,			//  ��֯�����ѵ�����ҵȼ��޷�����
	SYSTEM_INFO_ID_17637,			//  ��û�����Ȩ��
	SYSTEM_INFO_ID_17638,			//  �Ѿ��м������о�
	SYSTEM_INFO_ID_17639,			//  �ð�������Ѵ�����
	SYSTEM_INFO_ID_17640,			//  ��Ҳ����߲����������֯
#pragma endregion	


};


#endif