//--------------------------------------------------------------------
// �ļ���:	    FriendMsgDefine.h
// ��  ��:		����ϵͳ-��Ϣ����
// ˵  ��:		
// ��������:	2014��10��29��
// ��������:	2014��10��29��
// ������:		  
// �޸���:     
//    :	    
//--------------------------------------------------------------------

#ifndef __FriendMsgDefine_H__
#define __FriendMsgDefine_H__

enum CSFriendMsg
{
	//�������
	//�ͻ���������Ӻ��� ��ʽ��string msgid, wstring name
	CS_FRIEND_REQUEST_ADD_FRIEND = 1,
	//�ͻ���ͬ��������� ��ʽ��string msgid, wstring name
	CS_FRIEND_CONFIRM_ADD_FRIEND = 2,
	//�ͻ�������ɾ������ ��ʽ��string msgid, wstring name
	CS_FRIEND_REQUEST_DELETE_FRIEND = 3,
	//�ͻ���������Ӻ����� ��ʽ��string msgid, wstring name
	CS_FRIEND_ADD_BLACKLIST = 5,
	//�ͻ�������ɾ�������� ��ʽ��string msgid, wstring name
	CS_FRIEND_DELETE_BLACKLIST = 6,
	//�ͻ�������ɾ�������ԣ��������� ��ʽ��string msgid, wstring name
	CS_FRIEND_DELETE_APPLY = 7,
	// �ͻ�������ˢ���Ƽ��������� ��ʽ��string msgid, int type, wstring name(ģ������,�ղ�����,ͬ�ȼ�һ������)[Ҳ��ģ������](typeĿǰΪ0)
	CS_FRIEND_REFRESH_FRIEND_RMD = 8,
	//������Ϣ����,��ʽ��string msgid
	CS_FRIEND_INFO_UPDATE = 9,
	//�ͻ�������ɾ�����к����� ��ʽ��string msgid
	CS_FRIEND_DELETE_ALL_BLACKLIST = 10,
	// �ͻ�������ɾ�����ˣ���Ϣ��ʽ��unsigned msgid, wstring name(��������)
	CS_FRIEND_DELETE_ENEMY = 12,
	// �ͻ�������ɾ�����г��ˣ���Ϣ��ʽ��unsigned msgid
	CS_FRIEND_DELETE_ALL_ENEMY = 13,
	// �ͻ������󸽽�����
	CS_FRIEND_ALL_NEARBY = 14,
	// ����������
	CS_FRIEND_SEND_GIFT = 17,		//����   Args: uid, ID��FriendGift��id
	// һ��ȷ��/�ܾ���������
	CS_FRIEND_CONFIRM_ALL_APPLY = 18,		// ����: yes/no (1:yes 0: no)
	// ��������
	CS_FRIEND_APPLAY_SWORN	= 19,	// str ���uid
	// ����������
	CS_FRIEND_APPLAY_SWORN_REPLY = 20,   //wstr ����� int ���0�ܾ� 1ͬ��
	// Ѱ��Ԯ��
	CS_FRIEND_SEEK_AID = 21,			// str �����
	// �޸ĺ��ѳ�ν
	CS_FRIEND_CHANGE_FRIEND_TITILE  =22, //wstr ����� wstring����
	// ɾ������
	CS_FRIEND_DEL_SWORN = 23,           //wstr �����
	// Ԯ��
	CS_FRIEND_AID = 24,					//wstr �����
	//���ܶȽ���
	CS_FRIEND_INTIMACY_AWARD = 25,		//
	//һ����Ӻ���
	CS_FRIEND_ADD_FRIEND_BY_ONE_KEY = 26, // һ����Ӻ��� wsring����� 
};
	

enum SSFrienMsg
{
	//�������������֪ͨ
	SS_FRIEND_ONLINE_STATUS_UPDATE = 1,
	//֪ͨ�Է�ɾ������
	SS_FRIEND_REMOVE_PLAYER = 2,
	//���Ӻ��������б�
	SS_FRIEND_APPLY = 4,
	//���ѵȼ�����
	SS_FRIEND_LEVEL_UPDATE = 5,
	//����ս��������
	SS_FRIEND_BATTLEABILITY_UPDATE = 6,
	//���Ѱ�����
	SS_FRIEND_GUILDNAME_UPDATE = 7,
	//��Ӻ��ѷ���
	SS_FRIEND_ADD_RESULT = 11,
	// �����ͻ�
	SS_FRIEND_SEND_GIFT = 13,		// ����: sender uid, sender Name, flower number
	// ���ѻ���������
	SS_FRIEND_FLOWER_UPDATE = 14,	// ����: pMyUid << nMySumFlower << nMyWeekFlower
	//���ӱ˴����ܶ�
	SS_FRIEND_ADD_MUTUAL_INTIMACY = 15,
	//�������ܶ�
	SS_FRIEND_ADD_INTIMACY = 16,
	//���º���teamid
	SS_FRIEND_UPDATE_TEAM_ID = 17,
	//�������
	SS_FRIEND_APPLAY_SWORN = 18,
	//���������
	SS_FRIEND_APPLAY_SWORN_RSP = 19,
	//����ɹ�
	SS_FRIEND_SWORN_SUCCESS = 20,
	//ɾ������
	SS_FRIEND_DEL_SWORN = 21,
	//Ԯ������������
	SS_FRIEND_AID_TARGET_POS_REQ = 22,
	//Ԯ������
	SS_FRIEND_AID_TARGET_POS_RSP = 23,
	//���ӵ���
	SS_FRIEND_ADD_ONECE_ENEMY = 24,
	//���ú���vip
	SS_FRIEND_UPDATE_VIP = 26,
	//��ӻ�������ܶ�
	SS_FRIEND_TEAM_ADD_INTIMACY = 27,
};

enum FriendS2C
{
	S2C_SEEK_AID , //Ѱ��Ԯ�� ����� sceneid
	S2C_APPLAY_SWORN,// ������� wstr �����
	S2C_FRIEND_SYSTEM_RECOMMEND ,// ϵͳ�����Ƽ�
	S2C_FRIEND_INTIMACY_AWARD,//���ܶ��������

};

#endif