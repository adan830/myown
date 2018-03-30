//-------------------------------------------------------------------- 
// �ļ���:		InternalFields.h 
// ��  ��:		��ֿ�ڲ��ֶ������� 
// ˵  ��:		�ֶ����ƶ��� 
// ������:		lihl 
// ��  ��:      2016/03/31		 
//-------------------------------------------------------------------- 
#ifndef _INTERNAL_FIELDS_DEFINE_H_ 
#define _INTERNAL_FIELDS_DEFINE_H_ 
 
// �������ڲ����Զ��壺
#ifndef FIELD_PROP_NAME
#define	FIELD_PROP_NAME				"Name"				// VTYPE_WIDESTR		// ����
#endif 

#ifndef FIELD_PROP_MAXPLAYERS
#define	FIELD_PROP_MAXPLAYERS		"MaxPlayers"		// VTYPE_INT		// �����������ο�ֵ
#endif 

#ifndef FIELD_PROP_CURPLAYERS
#define	FIELD_PROP_CURPLAYERS		"CurPlayers"		// VTYPE_INT		// ��ǰ�������
#endif 

#ifndef FIELD_PROP_MAXVISUALS
#define	FIELD_PROP_MAXVISUALS		"MaxVisuals"		// VTYPE_INT		// �����Ұ���ƶ���������
#endif 

#ifndef FIELD_PROP_MAXLOOKERS
#define	FIELD_PROP_MAXLOOKERS		"MaxLookers"		// VTYPE_INT		// 0�ŷ����п��Ӷ�������۲�������
#endif 

#ifndef FIELD_PROP_MAXLOOKERS
#define	FIELD_PROP_COLLIDEPATH		"CollidePath"		// VTYPE_STRING		// ������ײ��Ϣ�ļ�·��
#endif

#ifndef FIELD_PROP_MAPEVENT
#define	FIELD_PROP_MAPEVENT			"MapEvent"			// VTYPE_INT		// �Ƿ������ͼ����仯�¼�OnMapChanged
#endif

#ifndef FIELD_PROP_BORN
#define	FIELD_PROP_BORN				"Born"				// VTYPE_STRING		// ��������
#endif

#ifndef FIELD_PROP_GROUPING
#define	FIELD_PROP_GROUPING			"Grouping"			// VTYPE_INT		// �Ƿ���鳡��
#endif


#ifndef FIELD_PROP_WORLDINFO
#define	FIELD_PROP_WORLDINFO			"WorldInfo"			// VTYPE_WIDESTR		// ��Ϸ������Ϣ
#endif

#ifndef FIELD_PROP_NOTSWITCHSTORE
#define	FIELD_PROP_NOTSWITCHSTORE	"NotSwitchStore"	// VTYPE_INT		// �����г���ǰ�����������
#endif

#ifndef FIELD_PROP_NOTTIMINGSTORE
#define	FIELD_PROP_NOTTIMINGSTORE	"NotTimingStore"	// VTYPE_INT		// ����Ҫ��ʱ�����������
#endif

#ifndef FIELD_PROP_ISNIGHT
#define	FIELD_PROP_ISNIGHT			"IsNight"		   	// VTYPE_INT		// 
#endif

//��ҽ�ɫ���ڲ����Զ��壺
#ifndef FIELD_PROP_NAME
#define	FIELD_PROP_NAME				"Name"				// VTYPE_WIDESTR		// �������
#endif

#ifndef FIELD_PROP_UID
#define	FIELD_PROP_UID				"Uid"				// VTYPE_STRING		// ��ɫΨһ��ʶ
#endif

#ifndef FIELD_PROP_ROLEINFO
#define	FIELD_PROP_ROLEINFO			"RoleInfo"			// VTYPE_WIDESTR		// ��ɫ��¼��Ϣ����ɫ���ݱ����save_para1�ֶΣ�
#endif

#ifndef FIELD_PROP_ACCOUNT
#define	FIELD_PROP_ACCOUNT			"Account"			// VTYPE_STRING		// �ʺ�
#endif

#ifndef FIELD_PROP_ACCOUNTID
#define	FIELD_PROP_ACCOUNTID			"AccountID"			// VTYPE_INT		// �ʺű��
#endif

#ifndef FIELD_PROP_ISSUERID
#define	FIELD_PROP_ISSUERID			"IssuerID"			// VTYPE_INT		// ��Ӫ�̱��
#endif

#ifndef FIELD_PROP_LOGONID
#define	FIELD_PROP_LOGONID			"LogonID"			// VTYPE_STRING		// ���ε�¼��ʶ��
#endif

#ifndef FIELD_PROP_CONNID
#define	FIELD_PROP_CONNID			"ConnId"			// VTYPE_INT		// �������ӱ��
#endif

#ifndef FIELD_PROP_TOTALSEC
#define	FIELD_PROP_TOTALSEC			"TotalSec"			// VTYPE_INT		// ��ɫ������ʱ�䣨�룩
#endif

#ifndef FIELD_PROP_ILLEGALS
#define	FIELD_PROP_ILLEGALS			"Illegals"			// VTYPE_INT		// �ͻ������״���
#endif

#ifndef FIELD_PROP_ADDRESS
#define	FIELD_PROP_ADDRESS			"Address"			// VTYPE_STRING		// ��������IP��ַ
#endif

#ifndef FIELD_PROP_GAMEMASTER
#define	FIELD_PROP_GAMEMASTER		"GameMaster"		// VTYPE_INT		// GMȨ�ޣ�0Ϊ��ͨ��ң�
#endif

#ifndef FIELD_PROP_VISUALRANGE
#define	FIELD_PROP_VISUALRANGE		"VisualRange"		// VTYPE_FLOAT		// ��Ұ��Χ	
#endif

#ifndef FIELD_PROP_SPRINGRANGE
#define	FIELD_PROP_SPRINGRANGE		"SpringRange"		// VTYPE_FLOAT		// ������Χ
#endif

#ifndef FIELD_PROP_INVISIBLE
#define	FIELD_PROP_INVISIBLE			"Invisible"			// VTYPE_INT		// �Ƿ񲻿ɼ�
#endif

#ifndef FIELD_PROP_NOCOLLIDE
#define	FIELD_PROP_NOCOLLIDE			"NoCollide"			// VTYPE_INT		// �Ƿ�����ײ
#endif

#ifndef FIELD_PROP_THROUGH
#define	FIELD_PROP_THROUGH			"Through"			// VTYPE_INT		// �Ƿ���Դ�͸��������
#endif

#ifndef FIELD_PROP_MAPFREE
#define	FIELD_PROP_MAPFREE			"MapFree"			// VTYPE_INT		// �Ƿ��ܿ����߷�Χ��Լ��
#endif

#ifndef FIELD_PROP_WALKCAP
#define	FIELD_PROP_WALKCAP			"WalkCap"			// VTYPE_INT		// ������������
#endif

#ifndef FIELD_PROP_WALKSTEP
#define	FIELD_PROP_WALKSTEP			"WalkStep"			// VTYPE_FLOAT		// ���߸߶Ȳ�
#endif

#ifndef FIELD_PROP_DUMMY
#define	FIELD_PROP_DUMMY				"Dummy"				// VTYPE_INT		// �Ƿ񻹲��ڳ���������
#endif

#ifndef FIELD_PROP_COLLIDESHAPE
#define	FIELD_PROP_COLLIDESHAPE		"CollideShape"		// VTYPE_INT		// ��ײ����
#endif

#ifndef FIELD_PROP_COLLIDEHEIGHT
#define	FIELD_PROP_COLLIDEHEIGHT		"CollideHeight"		// VTYPE_FLOAT		// ��ײ�߶�
#endif

#ifndef FIELD_PROP_COLLIDERADIUS
#define	FIELD_PROP_COLLIDERADIUS		"CollideRadius"		// VTYPE_FLOAT		// ��ײ�뾶
#endif

#ifndef FIELD_PROP_COLLIDEVERTEX
#define	FIELD_PROP_COLLIDEVERTEX		"CollideVertex"		// VTYPE_STRING		// ��ײ����ζ���
#endif

#ifndef FIELD_PROP_GROUPID
#define	FIELD_PROP_GROUPID			"GroupID"			// VTYPE_INT		// ������
#endif

#ifndef FIELD_PROP_NOSPEECH
#define	FIELD_PROP_NOSPEECH			"NoSpeech"			// VTYPE_INT		// �Ƿ��ֹ��ͨ����
#endif

#ifndef FIELD_PROP_ONLINE
#define	FIELD_PROP_ONLINE			"Online"			// VTYPE_INT		// �Ƿ�����
#endif

#ifndef FIELD_PROP_ACCOUNTINFO
#define	FIELD_PROP_ACCOUNTINFO		"AccountInfo"		// VTYPE_STRING		// �ʺ���Ϣ
#endif

#ifndef FIELD_PROP_CHARGEINFO
#define	FIELD_PROP_CHARGEINFO		"ChargeInfo"		// VTYPE_STRING		// �Ʒ���Ϣ
#endif

#ifndef FIELD_PROP_LETTERMAX
#define	FIELD_PROP_LETTERMAX			"LetterMax"			// VTYPE_INT		// ��������
#endif

#ifndef FIELD_PROP_CREATETIME
#define	FIELD_PROP_CREATETIME		"CreateTime"		// VTYPE_DOUBLE		// ��ɫ����ʱ��
#endif

#ifndef FIELD_PROP_LIMITTIME
#define	FIELD_PROP_LIMITTIME			"LimitTime"			// VTYPE_DOUBLE		// �¿���ֹʱ��
#endif

#ifndef FIELD_PROP_VISUALPLAYERS
#define	FIELD_PROP_VISUALPLAYERS		"VisualPlayers"		// VTYPE_INT		// ��Ұ�ڿɼ����������������
#endif

#ifndef FIELD_PROP_VISUALEVENT
#define	FIELD_PROP_VISUALEVENT		"VisualEvent"		// VTYPE_INT		// ���������Ұʱ�Ƿ񴥷��ص�
#endif

#ifndef FIELD_PROP_LANDSCENE
#define	FIELD_PROP_LANDSCENE			"LandScene"			// VTYPE_INT		// �´ε�¼ʱ���ߵĳ������
#endif

#ifndef FIELD_PROP_LANDPOSIX
#define	FIELD_PROP_LANDPOSIX			"LandPosiX"			// VTYPE_DOUBLE		// �´ε�¼ʱ���ߵ�λ��
#endif

#ifndef FIELD_PROP_LANDPOSIY
#define	FIELD_PROP_LANDPOSIY			"LandPosiY"			// VTYPE_DOUBLE		//
#endif

#ifndef FIELD_PROP_LANDPOSIZ
#define	FIELD_PROP_LANDPOSIZ			"LandPosiZ"			// VTYPE_DOUBLE		//
#endif

#ifndef FIELD_PROP_LANDORIENT
#define	FIELD_PROP_LANDORIENT		"LandOrient"		// VTYPE_DOUBLE		//
#endif

#ifndef FIELD_PROP_CHECKSYNCRANGE
#define	FIELD_PROP_CHECKSYNCRANGE	"CheckSyncRange"	// VTYPE_FLOAT		// λ��У��������С���뷶Χ
#endif

#ifndef FIELD_PROP_CHECKSYNCBIAS
#define	FIELD_PROP_CHECKSYNCBIAS		"CheckSyncBias"		// VTYPE_FLOAT		// λ��У������ƫ�ưٷֱ�
#endif

#ifndef FIELD_PROP_SERVERILLEGAL
#define	FIELD_PROP_SERVERILLEGAL		"ServerIllegal"		// VTYPE_INT		// �Ƿ������ڷǷ����������û�
#endif

#ifndef FIELD_PROP_CHECKDISTTOTAL
#define	FIELD_PROP_CHECKDISTTOTAL	"CheckDistTotal"	// VTYPE_DOUBLE		// ͳ���ܹ�������ƫ��
#endif

#ifndef FIELD_PROP_SERVERMONITOR
#define	FIELD_PROP_SERVERMONITOR		"ServerMonitor"		// VTYPE_INT64		// �����ڲ����ֵ
#endif

#ifndef FIELD_PROP_TYPE
#define	FIELD_PROP_TYPE				"Type"				// VTYPE_INT		//�������
#endif

#ifndef FIELD_PROP_CAPACITY
#define	FIELD_PROP_CAPACITY			"Capacity"			// VTYPE_INT		//�������
#endif

#ifndef FIELD_PROP_CONTAINERPOS
#define	FIELD_PROP_CONTAINERPOS		"ContainerPos"		// VTYPE_INT		//���λ��
#endif

#ifndef FIELD_PROP_SCRIPT
#define	FIELD_PROP_SCRIPT			"Script"			// VTYPE_STRING		//��ҽű�
#endif

#ifndef FIELD_PROP_CONFIG
#define	FIELD_PROP_CONFIG			"Config"			// VTYPE_STRING		//��ҵ�����
#endif

#ifndef FIELD_PROP_RANDSEED
#define	FIELD_PROP_RANDSEED			"RandSeed"			// VTYPE_INT		// �Զ��� ������֤ʹ��
#endif

#ifndef FIELD_PROP_PATHGRID
#define	FIELD_PROP_PATHGRID			"PathGrid"			// VTYPE_INT		// ռ��·����������
#endif

#ifndef FIELD_PROP_PATHTHROUGH
#define	FIELD_PROP_PATHTHROUGH		"PathThrough"		// VTYPE_INT		// �ɴ�Խ�����϶���
#endif

//NPC���ڲ����Զ��壺
#ifndef FIELD_PROP_NAME
#define	FIELD_PROP_NAME				"Name"					// VTYPE_WIDESTR		// ����
#endif

#ifndef FIELD_PROP_VISUALRANGE
#define	FIELD_PROP_VISUALRANGE		"VisualRange"			// VTYPE_FLOAT		// ��Ұ��Χ
#endif

#ifndef FIELD_PROP_SPRINGRANGE
#define	FIELD_PROP_SPRINGRANGE		"SpringRange"			// VTYPE_FLOAT		// ������Χ
#endif

#ifndef FIELD_PROP_INVISIBLE
#define	FIELD_PROP_INVISIBLE			"Invisible"				// VTYPE_INT		// �Ƿ񲻿ɼ�
#endif

#ifndef FIELD_PROP_NOCOLLIDE
#define	FIELD_PROP_NOCOLLIDE			"NoCollide"				// VTYPE_INT		// �Ƿ�����ײ
#endif

#ifndef FIELD_PROP_NOSPRING
#define	FIELD_PROP_NOSPRING			"NoSpring"				// VTYPE_INT		// �Ƿ��޴���
#endif

#ifndef FIELD_PROP_SPRINGER
#define	FIELD_PROP_SPRINGER			"Springer"				// VTYPE_INT		// �Ƿ���Դ�����������
#endif

#ifndef FIELD_PROP_THROUGH
#define	FIELD_PROP_THROUGH			"Through"				// VTYPE_INT		// �Ƿ���Դ�͸��������
#endif

#ifndef FIELD_PROP_MAPFREE
#define	FIELD_PROP_MAPFREE			"MapFree"				// VTYPE_INT		// �Ƿ��ܵ�ͼ�����߷�Χ��Լ��
#endif

#ifndef FIELD_PROP_WALKCAP
#define	FIELD_PROP_WALKCAP			"WalkCap"				// VTYPE_INT		// ������������
#endif

#ifndef FIELD_PROP_WALKSTEP
#define	FIELD_PROP_WALKSTEP			"WalkStep"				// VTYPE_FLOAT		// ���߸߶Ȳ�
#endif

#ifndef FIELD_PROP_LIFETIME
#define	FIELD_PROP_LIFETIME			"LifeTime"				// VTYPE_INT		// �ڳ����������ʱ�䣨Ϊ0��ʶ�����ƣ�
#endif

#ifndef FIELD_PROP_COLLIDESHAPE
#define	FIELD_PROP_COLLIDESHAPE		"CollideShape"			// VTYPE_INT		// ��ײ����
#endif

#ifndef FIELD_PROP_COLLIDEHEIGHT
#define	FIELD_PROP_COLLIDEHEIGHT		"CollideHeight"			// VTYPE_FLOAT		// ��ײ�߶�
#endif

#ifndef FIELD_PROP_COLLIDERADIUS
#define	FIELD_PROP_COLLIDERADIUS		"CollideRadius"			// VTYPE_FLOAT		// ��ײ�뾶
#endif

#ifndef FIELD_PROP_COLLIDEVERTEX
#define	FIELD_PROP_COLLIDEVERTEX		"CollideVertex"			// VTYPE_STRING		// ��ײ����ζ���
#endif

#ifndef FIELD_PROP_FIXED
#define	FIELD_PROP_FIXED				"Fixed"					// VTYPE_INT		// �Ƿ�̶����壨���Դ����ڵ�ͼ�Ĳ�����������
#endif

#ifndef FIELD_PROP_GROUPID
#define	FIELD_PROP_GROUPID			"GroupID"				// VTYPE_INT		// ������ 
#endif

#ifndef FIELD_PROP_TYPE
#define	FIELD_PROP_TYPE				"Type"			// VTYPE_INT		//npc����
#endif

#ifndef FIELD_PROP_CAPACITY
#define	FIELD_PROP_CAPACITY			"Capacity"		// VTYPE_INT		//npc����
#endif

#ifndef FIELD_PROP_CONTAINERPOS
#define	FIELD_PROP_CONTAINERPOS		"ContainerPos	// VTYPE_INT		//npcλ��
#endif

#ifndef FIELD_PROP_SCRIPT
#define	FIELD_PROP_SCRIPT			"Script"		// VTYPE_STRING		//npc�ű�
#endif

#ifndef FIELD_PROP_CONFIG
#define	FIELD_PROP_CONFIG			"Config"		// VTYPE_STRING		//npc������
#endif

#ifndef FIELD_PROP_PATHGRID
#define	FIELD_PROP_PATHGRID			"PathGrid"		// VTYPE_INT		// ռ��·����������
#endif

#ifndef FIELD_PROP_PATHTHROUGH
#define	FIELD_PROP_PATHTHROUGH		"PathThrough"	// VTYPE_INT		// �ɴ�Խ�����϶���
#endif

// ��Ʒ���ڲ����Զ��壺
#ifndef FIELD_PROP_NAME
#define	FIELD_PROP_NAME				"Name"				//VTYPE_WIDESTR		// ����
#endif

#ifndef FIELD_PROP_VISUALRANGE
#define	FIELD_PROP_VISUALRANGE		"VisualRange"		//VTYPE_FLOAT		// ��Ұ��Χ
#endif

#ifndef FIELD_PROP_SPRINGRANGE
#define	FIELD_PROP_SPRINGRANGE		"SpringRange"		//VTYPE_FLOAT		// ������Χ
#endif

#ifndef FIELD_PROP_INVISIBLE
#define	FIELD_PROP_INVISIBLE			"Invisible"			//VTYPE_INT		// �Ƿ񲻿ɼ�
#endif


#ifndef FIELD_PROP_NOCOLLIDE
#define	FIELD_PROP_NOCOLLIDE			"NoCollide"			//VTYPE_INT		// �Ƿ�����ײ
#endif

#ifndef FIELD_PROP_NOSPRING
#define	FIELD_PROP_NOSPRING			"NoSpring"			//VTYPE_INT		// �Ƿ��޴���
#endif

#ifndef FIELD_PROP_LIFETIME
#define	FIELD_PROP_LIFETIME			"LifeTime"			//VTYPE_INT		// �ڳ����������ʱ�䣨Ϊ0��ʶ�����ƣ�
#endif

#ifndef FIELD_PROP_COLLIDEHEIGHT
#define	FIELD_PROP_COLLIDEHEIGHT		"CollideHeight"		//VTYPE_FLOAT		// ��ײ�߶�
#endif

#ifndef FIELD_PROP_COLLIDEVERTEX
#define	FIELD_PROP_COLLIDEVERTEX		"CollideVertex"		//VTYPE_STRING		// ��ײ����ζ���
#endif

#ifndef FIELD_PROP_FIXED
#define	FIELD_PROP_FIXED				"Fixed"				//VTYPE_INT		// �Ƿ�̶����壨���Դ����ڵ�ͼ�Ĳ�����������
#endif

#ifndef FIELD_PROP_GROUPID
#define	FIELD_PROP_GROUPID			"GroupID"			//VTYPE_INT		// ������
#endif

#ifndef FIELD_PROP_TYPE
#define	FIELD_PROP_TYPE				"Type"				// VTYPE_INT		//��Ʒ����
#endif

#ifndef FIELD_PROP_CONTAINERPOS
#define	FIELD_PROP_CONTAINERPOS		"ContainerPos"		// VTYPE_INT		//��Ʒλ��
#endif

#ifndef FIELD_PROP_SCRIPT
#define	FIELD_PROP_SCRIPT			"Script"			// VTYPE_STRING		//��Ʒ�ű�
#endif

#ifndef FIELD_PROP_CONFIG
#define	FIELD_PROP_CONFIG			"Config"			// VTYPE_STRING		//��Ʒ������
#endif

#ifndef FIELD_PROP_PATHGRID
#define	FIELD_PROP_PATHGRID			"PathGrid"			// VTYPE_INT		// ռ��·����������
#endif

#ifndef FIELD_PROP_PATHTHROUGH
#define	FIELD_PROP_PATHTHROUGH		"PathThrough"		// VTYPE_INT		// �ɴ�Խ�����϶���
#endif


//��Ϸ����������ڲ����Զ��壺
#ifndef FIELD_PROP_NAME
#define	FIELD_PROP_NAME				"Name"		// VTYPE_WIDESTR	        // ����
#endif

#ifndef FIELD_PROP_GROUPID
#define	FIELD_PROP_GROUPID			"GroupID"	// VTYPE_INT		// ������
#endif

//�������������ڲ����Զ��壺
#ifndef FIELD_PROP_NAME
#define	FIELD_PROP_NAME				"Name"		// VTYPE_WIDESTR		// ����
#endif

#ifndef FIELD_PROP_GROUPID
#define	FIELD_PROP_GROUPID			"GroupID"	// VTYPE_INT		// ������
#endif

#ifndef FIELD_PROP_TYPE
#define	FIELD_PROP_TYPE				"Type"				// VTYPE_INT		//npc����
#endif

#ifndef FIELD_PROP_CAPACITY
#define	FIELD_PROP_CAPACITY			"Capacity"			// VTYPE_INT		//npc����
#endif

#ifndef FIELD_PROP_CONTAINERPOS
#define	FIELD_PROP_CONTAINERPOS		"ContainerPos"		// VTYPE_INT		//npcλ��
#endif

#ifndef FIELD_PROP_SCRIPT
#define	FIELD_PROP_SCRIPT			"Script"			// VTYPE_STRING		//npc�ű�
#endif

#ifndef FIELD_PROP_CONFIG
#define	FIELD_PROP_CONFIG			"Config"			// VTYPE_STRING		//npc������
#endif

 
#endif // _FIELDS_DEFINE_H_