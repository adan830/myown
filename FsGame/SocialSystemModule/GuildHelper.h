//--------------------------------------------------------------------
// �ļ���:		GuildHelper.h
// ��  ��:		guild helper
// ˵  ��:		
// ��������:	2017��08��03��
// ������:		kevin
// ��Ȩ����:	WhalesGame Technology co.Ltd
//--------------------------------------------------------------------
#ifndef __HELPER_GUILD_H__
#define	__HELPER_GUILD_H__
#include "Fsgame/Define/header.h"
#include "server/IPubKernel.h"
#include "FsGame/Define/GuildDefine.h"
#include "utils/cache/ICacheString.h"
class GuildHelper
{
public:
	GuildHelper();
	~GuildHelper();
public:
	//��ʼ����������
	static bool InitPubData(IKernel* pKernel);
	static bool InitPubDataEx(IPubKernel* pPubKernel);
public:
	// (inc)for server id
	static const std::wstring& GetServerId(const wchar_t* server_id = NULL);
	// (inc)for space
	static IPubSpace* GetSpaceDomain(IPubSpace* pubSpace = NULL);
	static IPubSpace* GetSpaceGuild(IPubSpace* pubSpace = NULL);
public:
	// ���������ռ�
	static const wchar_t* Parse_SpaceNameById(const wchar_t* space_name, int server_id, bool has_split_string = true);
	static const wchar_t* Parse_SpaceName(const wchar_t* space_name, bool has_server_id = true,bool has_split_string=true);
public:
	//(inc)���ṫ������
	static IPubData* SpaceGuild_PubData(IPubData* pubData = NULL);
	//(inc)����guild����
	static bool SpaceGuild_Data(const wchar_t* data_name, IPubData*& guild_data,bool has_server_id=false);
public:
	//(inc)����domain����
	static bool SpaceDomain_Data(const wchar_t* data_name, IPubData*& domain_data, bool has_server_id = false);
public:
	//for ���������ҵ���߼������
	//���᣺���ع���ͼ�꣬���
	static bool GetGuildIdent(const wchar_t* guildName, int& target_ident, const wchar_t*& target_alias);
	//���᣺ ���ҹ���᳤
	static bool GetGuildCaptain(IPubKernel* pPubKernel, const wchar_t* guild_name, std::wstring& guild_captain);
	//���᣺���ع������ڣ������б�,�к�
	static bool GetGuildListRec(const wchar_t* guildName, IRecord*& p_record, int& p_row);
};

//�ͻ������� �����ʻ�����
//cs �ο��� public static string TranslateNation(string value)
//format(�ͻ�������): ��Ӫ
bool util_text_format_nation(int i_nation, ICacheString& dst_string);
//format(�ͻ�������): ��֯�ǳ���
bool util_text_format_town_name(int i_town, ICacheString& dst_string);
//format(�ͻ�������): ��֯�ǳ���Ӫ
bool util_text_format_town_nation(int i_town, ICacheString& dst_string);
//format(�ͻ�������): ��֯�ǳر��⣺�ܶ棬�ֶ�
bool util_text_format_town_title(int i_town, ICacheString& dst_string);
//format(�ͻ�������): ��֯�ǳع�ְ
bool util_text_format_town_position(int i_town, ICacheString& dst_string);

#endif