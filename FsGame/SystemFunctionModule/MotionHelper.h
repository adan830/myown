//--------------------------------------------------------------------
// �ļ���:		MotionHelper.h
// ��  ��:		motion helper
// ˵  ��:		
// ��������:	2017��06��06��
// ������:		kevin
// ��Ȩ����:	WhalesGame Technology co.Ltd
//--------------------------------------------------------------------
#ifndef __CACHE_MOTION_HELPER_H__
#define	__CACHE_MOTION_HELPER_H__
#include "Fsgame/Define/header.h"
#include "public/VarList.h"
#include <vector>
#include <map>
#include "utils/cache/CacheString.hpp"

//��kernel����һ�£��ݲ�ʹ��namespace
//ע�⣺ Ikernel,IRoomKernel,�����֣����ڵײ�û�г��󣬼̳С����Զ���ʵ�֣�helper����Ҫע�⡣

// Ѱ·��ʽ
enum MotionMethod
{
	MOTION_METHOD_NORMAL = 1,	// 001��ͨѰ· 
	MOTION_METHOD_STATIC = 2,	// 010��̬Ѱ· 
	MOTION_METHOD_DYNAMIC = 4,	// 100��̬Ѱ· 
	MOTION_METHOD_NORMAL_STATIC = 3,	// 011��ͨѰ· + ��̬Ѱ·
	MOTION_METHOD_NORMAL_DYNAMIC = 5,	// 101��ͨѰ· + ��̬Ѱ· 
	MOTION_METHOD_ALL = 7,	// 111��ͨѰ· + ��̬Ѱ· + ��̬Ѱ· 
};

/// \brief �ƶ�����
struct MontionPos
{
public:
	int  grid_;		//����
public:	
	float pos_x_;	//����x
	float pos_z_;	//����z
	float pos_y_;	//����y
public:
	MontionPos() :grid_(0), pos_x_(0.0f), pos_z_(0.0f), pos_y_(0.0f){}
public:
	bool IsValid() const
	{
		return true; //�߽�δ֪����ֱ�ӷ���true
	}
	void Clear()
	{
		grid_ = 0;
		pos_x_ = 0.0f;
		pos_z_ = 0.0f;
		pos_y_ = 0.0f;
	}
public:
	///// \brief ��ֵ
	void Set(const MontionPos& ref)
	{
		grid_  = ref.grid_;
		pos_x_ = ref.pos_x_;
		pos_z_ = ref.pos_z_;
		pos_y_ = ref.pos_y_;
	}
public:
	///// \brief �����Ƿ���ͬ
	bool IsEqualGrid(const MontionPos& ref) const
	{
		if (grid_ == ref.grid_)
		{
			return true;
		}
		return false;
	}
};
//·����
struct MotionPathPos 
{
public:
	int		   find_result_;  // struct eFindResult
	MontionPos pos_;
	CVarList   path_;
public:
	MotionPathPos() :find_result_(0){}
	~MotionPathPos(){ Clear(); }
public:
	void Clear()
	{
		find_result_ = 0;
		pos_.Clear();
		if (path_.GetCount()>0)
		{
			path_.Clear();
		}
	}
public:
	void SetPath(int find_result, const MontionPos& dst_pos, const IVarList& dst_path)
	{
		Clear();
		find_result_ = find_result;
		pos_.Set(dst_pos);
		if (find_result>0)
		{
			if (dst_path.GetCount() <= 0)
			{
				find_result = 0;
			}
			else
			{
				path_.Concat(dst_path);
			}
		}
	}
};
//����ĳ������·��
typedef std::vector<MotionPathPos>  MotionPathGrid_t;

//·����
typedef std::map<int,MotionPathGrid_t> MotionPathMap_t;

//��̬��̬Ѱ·��
template< typename scene_type >
struct MotionPathMap_Case
{
private:
	typedef std::map<scene_type, MotionPathMap_t> MotionPathMap_Case_t;
private:
	MotionPathMap_Case_t map_;
public:
	MotionPathMap_Case(){}
	~MotionPathMap_Case(){Clear();}
public:
	void Clear()
	{
		if (map_.size() > 0)
		{
			map_.clear();
		}
	}
public:
	bool FindPathMap(const scene_type& key_value,MotionPathMap_t*& p_path_list)
	{
		MotionPathMap_Case_t::iterator it = map_.find(key_value);
		if (it != map_.end())
		{
			p_path_list = &it->second;
		}
		else
		{
			MotionPathMap_t t_path;
			map_.insert(std::make_pair(key_value, t_path));
			MotionPathMap_Case_t::iterator it_tt = map_.find(key_value);
			if (it_tt == map_.end())
			{
				Assert(0);
				return false;
			}
			p_path_list = &it_tt->second;
		}
		return true;
	}
};

//Cache����·����
struct MotionPathCache
{
public:
	MotionPathMap_Case<std::string> path_static_;	//��̬·��
	MotionPathMap_Case<int> path_dynamic_;  //��̬·��
public:
	MotionPathCache(){}
	~MotionPathCache()
	{
		path_static_.Clear();
		path_static_.Clear();
	}
};


/// \brief Ѱ·����
class MotionHelper
{
public:
	MotionHelper();
	~MotionHelper();
public:
	/// \brief Ѱ·,��̬Ѱ·
	/// \brief pKernel         kernel
	/// \brief self		       self
	/// \brief pos_sef         Դλ��
	/// \brief pos_target	   Ŀ��λ��
	/// \brief dst_point_count ����Ѱ·�ĵ�����ʧ�ܷ���0���� eFindResult
	/// \brief dst_path_list   ·�����б�
	static bool FindPathToTargets_Static(IKernel* pKernel, const PERSISTID& self, const MontionPos& pos_sef, const MontionPos& pos_target, int& dst_point_count, IVarList& dst_path_list);
	/// \brief Ѱ·,��̬Ѱ·
	/// \brief pKernel         kernel
	/// \brief self		       self
	/// \brief pos_sef         Դλ��
	/// \brief pos_target	   Ŀ��λ��
	/// \brief dst_path_list   ·�����б�
	static bool FindPathToTargets_Dynamic(IKernel* pKernel, const PERSISTID& self, const MontionPos& pos_sef, const MontionPos& pos_target, IVarList& dst_path_list);
public:
	/// \brief ��ǰ�����Ƿ���Ŀ�����Χ
	/// \param cur_grid ��ǰ����
	/// \param dst_grid Ŀ�����
	/// \param gridRows ��������
	/// \param targetGrid Ŀ����ռ������
	/// \param memo     ����ת��ʹ��������꣬��kernel�ṩ�ķ�ʽ��ͬ�����ǽ����ͬ��Ч�ʸ��ߡ�
	static bool IsGridAround(int cur_grid, int dst_grid, int gridRows,int targetGrid = 1);
public:
	/// \brief �����ƶ�����
	/// \brief pKernel kernel
	/// \brief dst_pos  Ŀ�ĸ���
	/// \brief dst_grid/(dst_x,dst_z)  Ŀ�ĸ�������
	static void ParseMotionPos(IKernel* pKernel, MontionPos& dst_pos, int dst_grid);
	static void ParseMotionPosEx(IKernel* pKernel, MontionPos& dst_pos, float dst_x, float dst_z);
	static void ParseMotionPosAll(IKernel* pKernel, MontionPos& dst_pos, float dst_x, float dst_z,float dst_y);
private:
	/// \brief ��ѯ���洢����·��
	static bool FindCachePath(MotionPathMap_t& p_path_list, int target_grid,const MontionPos& pos_sef, MotionPathPos*& find_path, int iTotalRow);
	static bool SaveCachePath(MotionPathMap_t& p_path_list, int target_grid, const MotionPathPos& find_path);
private:
	/// \brief ��ѯĿ��·�������б�
	static bool FindPathGrid(MotionPathMap_t& p_path_list, int target_grid, MotionPathGrid_t*& dst_path_grid);
private:
	//˽�б���: Ѱ·����
	static MotionPathCache& GetMotionPathCache();
};

#endif