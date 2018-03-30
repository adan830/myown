/************************************************************************/
/*                            ������ӿ�                                */
/************************************************************************/

#ifndef IFindPath_H_
#define IFindPath_H_

#include "ISceneMapProcess.h"
#include "Point.h"

#include <stdio.h>
#include <string>
#include <vector>
#include <list>

typedef void * (POpenFile)(const char *, const char *);
typedef bool (PCloseFile)(void *);
typedef size_t (PReadFile)(void *, void *, size_t);
typedef size_t (PGetFileSize)(const char *);

// �糡��Ѱ·�ķ��ؽ���Ľṹ
typedef struct _SCENEPATHITEM
{
    std::string sceneid;
    float xBegin;
    float yBegin;
    float zBegin;
    float xEnd;
    float yEnd;
    float zEnd;
} SCENEPATHITEM;

typedef struct _PATHLINK
{
    int index;
    float height; // Ȩ�أ����룩
} PATHLINK;

typedef struct _PATHPOINT
{
    _PATHPOINT()
        : x(0.0f), y(0.0f), z(0.0f),
          r(0.0f),
          bRemove(false)
    {
        vecLink.clear();
    }

    float x, y, z;                  // ����
    float r;                        // �õ�İ뾶
    bool bRemove;                   // ��Ҫɾ���ĵ�
    std::vector<PATHLINK> vecLink;  // ����ͨ��ϵ�ĵ�
} PATHPOINT;

enum eFindResult
{
    SCENE_FIND_PATH_ERROR       = -6,   // �糡��Ѱ·ʧ��
    FIND_PATH_TIMEOUT           = -5,   // Ѱ·��ʱ
    FIND_PATH_ERROR             = -4,   // Ѱ·A*��������ʧ��
    NO_PATH_POINT_NEAR_END      = -3,   // �յ㸽��û��Ѱ·��
    NO_PATH_POINT_REACH_BEGIN   = -2,   // ��㸽��û����ֱ�ӵ����·��
    BEGIN_POINT_CANT_WALK       = -1,   // ��ǰ��վ�ĵ㣬���ǿ����ߵ�
    UNKNOW_ERROR                = 0,    // δ֪����!
    FIND_PATH_SUCCESS           = 1,    // 1������,Ѱ·�ɹ�!
};

#ifdef FIND_PATH_DLL
#ifdef FINDPATHNEW_EXPORTS
#define FIND_PATH_API __declspec(dllexport)
#else
#define FIND_PATH_API __declspec(dllimport)
#endif
#else
#define FIND_PATH_API 
#endif

class FIND_PATH_API IFindPath
{
public:
    /// \brief  ����ʵ��
    static IFindPath* Create();
    /// \brief  ɾ��ʵ��
    static void Destroy(IFindPath * pInstance);

    /// \brief  ���ö�ȡ�ļ��ĺ���ָ�롣
    ///         ������Ĭ��Ϊfopen,fclose...
    /// \param  FunName      ��������
    /// \param  FunPointer   ����ָ��
    /// \return ���óɹ�����true�����ɹ�����false
    virtual bool SetFileFunc(const char * FunName, void * FunPointer) = 0;

public:
    /// \brief  ��ʼ��Ѱ·ģ�顣
    /// \param  path    Ѱ·�ļ�����·��
    virtual void InitFindPath(const char * path) = 0;

    /// \brief  ���ָ��������·������
    /// \param  scene_res   ����Resource
    /// \return ·������
    virtual int GetPathPointCount(const char * scene_res) = 0;

    /// \brief  ���ָ��������ָ����Χ��·�������б�
    /// \param  scene_res   ����CONFIGID
    /// \param  radius      �뾶
    /// \param  point_lst   ·���������浽��
    /// \param  max_count   point_lst������������
    /// \return �ɹ�����true��ʧ�ܷ���false
    virtual int GetPathPointInRadius(const char * scene_res,
        float x, float z, float radius,
        int point_lst[], int max_count) = 0;

    /// \brief  ���ָ��������ָ�������·�������б�
    /// \param  scene_res   ����CONFIGID
    /// \param  point_lst   ·���������浽��
    /// \param  max_count   point_lst������������
    /// \return �ɹ�����true��ʧ�ܷ���false
    virtual int GetPathPointInZone(const char * scene_res,
        float left, float top, float right, float bottom,
        int point_lst[], int max_count) = 0;

    /// \brief  ��ѯ·��
    /// \param  scene_res   ����CONFIGID
    /// \param  x, y, z     ·������
    /// \return �ɹ�����·��������ţ�ʧ�ܷ���-1
    virtual int PathPointExist(const char * scene_res,
        float x, float y, float z) = 0;

    /// \brief  ���ָ��������ָ��·�����Ϣ
    /// \param  scene_res   ����CONFIGID
    /// \param  index       ·��������
    /// \return �ɹ�����·��ṹ��ָ�룬ʧ�ܷ���NULL
    virtual PATHPOINT * GetPathPointInfo(const char * scene_res, int index) = 0;

    /// \brief  ���·��
    /// \param  scene_res   ����CONFIGID
    /// \param  x, y, z     ·������
    /// \param  r           ·��뾶
    /// \return �ɹ�����·��������ţ�ʧ�ܷ���-1
    virtual int AddPathPoint(const char * scene_res,
        float x, float y, float z, float r) = 0;

    /// \brief  ɾ��·��
    /// \param  scene_res   ����CONFIGID
    /// \param  index       ·������
    virtual void RemovePathPoint(const char * scene_res, int index) = 0;
    virtual void RemovePathPoint(const char * scene_res, int index,
        int begin, int end) = 0;
    /// \brief  ɾ�����һ��·��
    /// \param  scene_res   ����CONFIGID
    virtual void RemoveEndPathPoint(const char * scene_res) = 0;

    /// \brief  �޸�·��λ��
    /// \param  scene_res   ����CONFIGID
    /// \param  index       ·������
    /// \param  x, y, z     ·������
    /// \return �ɹ�����true
    virtual bool SetPathPointPos(const char * scene_res,
        int index, float x, float y, float z) = 0;

    /// \brief  �����ͨ��ϵ
    /// \param  scene_res   ����CONFIGID
    /// \param  index       ·������
    /// \param  link_index  ��ͨ��·������
    /// \return �ɹ�����true, ʧ�ܷ���false
    virtual bool AddLink(const char * scene_res, int index, int link_index) = 0;

    /// \brief  ɾ����ͨ��ϵ
    /// \param  scene_res   ����CONFIGID
    /// \param  index       ·������
    /// \param  link_index  ��ͨ��·������
    virtual void RemoveLink(const char * scene_res, int index, int link_index) = 0;

    /// \brief  ������Դ�Ƿ��Ѽ��ء�
    /// \param  scene_res   Ҫ���صĳ���CONFIG
    /// \return ���سɹ�����true�����򷵻�false
    virtual bool IsLoaded(const char * scene_res) = 0;

    /// \brief  �½�Ѱ·��Դ��
    /// \param  scene_res   Ҫ�½��ĳ���CONFIG
    /// \return �½��ɹ�����true�����򷵻�false
    virtual bool NewPath(const char * scene_res, ISceneMapProcess * pSceneMapProcess) = 0;

    /// \brief  ����Ѱ·��Դ��
    /// \param  scene_res   Ҫ���صĳ���CONFIG
    /// \return ���سɹ�����true�����򷵻�false
    virtual bool LoadPath(const char * scene_res, ISceneMapProcess * pSceneMapProcess) = 0;

    /// \brief  ����Ѱ·��Դ��
    /// \param  scene_res   ����CONFIGID
    /// \return �ɹ�����true�����򷵻�false
    virtual bool SavePath(const char * scene_res) = 0;

    /// \brief  �ͷ�Ѱ·��Դ��
    /// \param  scene_res   Ҫ�ͷŵĳ���CONFIG
    virtual void ReleasePath(const char * scene_res) = 0;

    /// \brief  �ͷ�����Ѱ·��Դ��
    virtual void ReleaseAllPath() = 0;

    /// \brief  ��Ѱ·�ӿڡ���ȡ��㵽�յ��·��
    /// \param  xBegin, yBegin, zBegin  �������
    /// \param  xEnd, yEnd, zEnd        �յ�����
    /// \return ����Ѱ·�ĵ�����ʧ�ܷ���0���� eFindResult
    virtual int PointFindPath(const char * scene_res,
        float xBegin, float yBegin, float zBegin,
        float xEnd, float yEnd, float zEnd,
        float x[], float y[], float z[], int maxcount,
        bool bNormal = false, bool bServer = true) = 0;

    /// \brief  ��Ѱ·�ӿڡ���ȡ��㵽�յ��·��
    ///         ��Ϊ���ɵ�Ѱ·�ĵ�һ��
    ///         ��ʱֻ�ڿͻ����õ�
    /// \param  xBegin, yBegin, zBegin  �������
    /// \param  xEnd, yEnd, zEnd        �յ�����
    /// \return ����Ѱ·�ĵ�����ʧ�ܷ���0
    virtual int PointFindMainPath(const char * scene_res,
        float xBegin, float yBegin, float zBegin,
        float xEnd, float yEnd, float zEnd,
        float x[], float y[], float z[], int maxcount) { return 0; };

    /// \brief  ��Ѱ·�ӿڡ��糡��Ѱ·
    /// \param  scene_resBegin          ���ĳ�����
    /// \param  xBegin, yBegin, zBegin  �������
    /// \param  scene_resEnd            �յ�ĳ�����
    /// \param  xEnd, yEnd, zEnd        �յ�����
    /// \return ����Ѱ·�ĵ�����ʧ�ܷ���0
    virtual int SceneFindPath(const char * begin_scene_res,
        float xBegin, float yBegin, float zBegin,
        const char * end_scene_res,
        float xEnd, float yEnd, float zEnd,
        SCENEPATHITEM scenepath[], int maxcount) = 0;

    /// \brief  ����ָ�����Ƿ�Ϸ�
    /// \param  x, y, z ����
    virtual bool TryPoint(const char * scene_res, float x, float y, float z) = 0;

    /// \brief  ���������Ƿ����ֱ��
    /// \param  xBegin, yBegin, zBegin  ��ʼ������
    /// \param  xEnd, yEnd, zEnd        �յ�����
    virtual bool TryLine(const char * scene_res,
        float xBegin, float yBegin, float zBegin,
        float xEnd, float yEnd, float zEnd) = 0;

    /// \brief  ����·������Ż�
    /// \param  scene_res   ������ʶ
    /// \param   pointList  Ҫ�Ż��ĵ��б�
    virtual int Optimize(const char * scene_res, std::list<SPointf>& pointList) = 0;

    /// \brief  ����Ѱ·��ʱʱ��
    /// \param  time ʱ�䣨���룩
    virtual void SetTimeOut(size_t time) = 0;
};

#endif  // IFindPath_H_