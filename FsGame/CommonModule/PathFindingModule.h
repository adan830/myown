//--------------------------------------------------------------------
// �ļ���:		PathFindingModule.h
// ��  ��:		Ѱ·�ӿ�
// ˵  ��:		
// ��������:	2014��06��10��
// ��������:	2014��06��10��
// ������:		  ( )
// �޸���:     
//    :	    
//--------------------------------------------------------------------

#ifndef __PathFindingModule_H__
#define __PathFindingModule_H__

#include "public/FastStr.h"
#include "Fsgame/Define/header.h"
#include "IFindPath.h"
#include <map>
#include <math.h>

class Point;

class SceneMapProcess : public ISceneMapProcess
{
public:
    SceneMapProcess( IKernel * pKernel ) : m_pKernel( pKernel )
    {
    }

    ~SceneMapProcess()
    {
    }

public:
	/// \brief  ����ָ�����Ƿ�Ϸ�
    virtual bool TryPoint( float x, float y, float z );
	/// \brief  ���������Ƿ����ֱ�ߡ�
    virtual bool TryLine( float xb, float yb, float zb, float xe, float ye, float ze );

	/// \brief  ��õ�ͼ��Ϣ
    virtual bool GetMapSize( float & left, float & top, float & width, float & height );
    virtual int GetLastTryError()
    {
        return 0;
    }
	virtual bool IsExistWater( float xb, float yb, float zb )
	{
		return 0;
	}

private:
    IKernel * m_pKernel;
};

// Ѱ·ϵͳ
class PathFindingModule : public ILogicModule
{
private:
	typedef std::map< std::string, SceneMapProcess * > SceneMapProcessType;
    typedef std::map< std::string, SceneMapProcess * >::const_iterator
    SceneMapProcessTypeConstItr;
    typedef std::map< std::string, SceneMapProcess * >::iterator
    SceneMapProcessTypeItr;

#define TRACEMSG( s, ... ) \
    {\
        char msg[ 128 ];\
        ::sprintf_s( msg, 128, s, __VA_ARGS__ );\
        pKernel->Trace( msg );\
    }

public:
    virtual bool Init( IKernel * pKernel );
    virtual bool Shut( IKernel * pKernel );

private:
    // ���������ص�
    static int OnSceneCreate( IKernel * pKernel,
                             const PERSISTID & scene,
                             const PERSISTID & sender,
                             const IVarList & args );

public:
    // ��scene_res��Ӧ�����е���������Ѱ·
    static int PointFindPath( const char * scene_res,
                             const Point & p1,
                             const Point & p2,
                             IVarList & point_lst );

private:
    static IFindPath * m_pFindPath;

    static SceneMapProcessType m_sceneMapProcessMap;
};

#endif //__PathFindingModule_H__

