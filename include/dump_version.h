//-------------------------------------------------------------------
// �� �� ����dump_version.h
// ��    �ݣ�
// ˵    ����
// �������ڣ�2014��2��24��
// �� �� �ˣ�Liangxk
// ��Ȩ���У�������ţ�������޹�˾
//-------------------------------------------------------------------


#if !defined __DUMP_VERSION_H__
#define __DUMP_VERSION_H__

/*
AAAA.BBBB
AA: ���汾��		ת���������䶯��仯
BB: ��Ҫ�汾��	����ģ��仯��仯
*/

//#define CURRENT_TOOLSET_VERSION "1000.1000"

/// 2014.4.16
/// ���ӽ������ݿ�->ͳ���ض���������Ʒ�����Ĺ���
/// dumptool_backuprole.cpp
/// 1000.1001

/// 2014.4.16
///	����MemFile�����д����ַ���(��ֱ��д���ļ�, �����Ƿ��뻺��), ��ᵼ��memfile���ڴ沿�����ݺʹ��ַ���˳��ߵ���bug 
/// MemFile.cpp
/// bool MemFile::Write
/// 1001.1001

/// 2014.4.20
/// ���������Ķ���game_object�����Ӷ������Ʋ��Ҷ���ʱ�߼�����(���ȸ������ֲ���, ��û������������Ӷ����script����)
/// game_object.cpp
/// IOBJECT* object_t::GetChildByName
/// 1002.1001


/// 2014.5.4
/// ������������������ʱ, ���û�����Ƶ���������ܵ����쳣��bug
/// ReadBin.cpp
/// void ReadBin::OnReadRole
/// 1003.1001


/// 2014.5.5
/// ������������������, ��������ʱ����ʧ��, ����ʱ�����쳣���� ����崻���bug
/// game_object.cpp 
/// game_object::DeleteObj

/// 2014.7.10
/// ����64λ�汾���޸�ת������ʱdump�ļ��Ĵ���
/// 1005.1000

/// 2015.7.29
/// ����DumpLoader����ʱ����ָ��ִ��manage_command.ini�е�����
/// 1006.1000


#define CURRENT_TOOLSET_VERSION "1006.1001"




#endif // __DUMP_VERSION_H__