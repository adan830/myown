#ifndef _ApperanceDefine_h_
#define _ApperanceDefine_h_

#include <vector>
#define APPERANCE_PRI_FILE "ini/Item/Apperance/ApperancePri.xml"
#define APPERANCE_PKG_FILE "ini/Item/Apperance/ApperancePkg.xml"

// �����Դ
enum ApperanceSource
{
	APPERANCE_SOURCE_NONE = 0,

	APPERANCE_SOURCE_EQUIP,			// װ��
	APPERANCE_SOURCE_STRENGTHEN,	// ǿ��
	APPERANCE_SOURCE_FASHION,		// ʱװ

	APPERANCE_SOURCE_MAX,
};

// �����ʾ���ȼ�
struct ApperancePri 
{
	int apperance_src;	// ��Դ
	int pri;			// ���ȼ�

	ApperancePri(int src) :
		apperance_src(src),
		pri(0)
	{}

	bool operator == (const ApperancePri& cfg) const
	{
		return cfg.apperance_src == apperance_src;
	}
};
typedef std::vector<ApperancePri> ApperancePriVec;

// ������԰�
struct ApperancePkg
{
	int apperance_id;
	int package;

	ApperancePkg(int id) :
		apperance_id(id),
		package(0)
	{}

	bool operator == (const ApperancePkg& cfg) const
	{
		return cfg.apperance_id == apperance_id;
	}
};
typedef std::vector<ApperancePkg> ApperancePkgVec;

#endif