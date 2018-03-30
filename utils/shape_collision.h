
//--------------------------------------------------------------------
// �ļ���:      code\mobile\tools\shape_collision.h
// ��  ��:      ͼ����ײ���
// ˵  ��:      ����ѧ���㣬ҵ������߼���Ҫ�ӵ����Ŀǰֻ֧��2d���
// ��������:    2014��4��3��
// ������:      longch
//    :       
//--------------------------------------------------------------------


#ifndef __SHAPE_COLLISION_H__
#define __SHAPE_COLLISION_H__

#include "utils/math/math.h"

#include "utils/util_func.h"

#define TARGET_FILTER_HIGHT_LIMIT (3.0f)

// ��ײ��ⷵ�ؽ��
enum ECollideResult
{
    ECOLLIDE_RESULT_INTERSECT,              // �ཻ
    ECOLLIDE_RESULT_ERR_TOONEAR,            // ����̫��
    ECOLLIDE_RESULT_ERR_TOOFAR,             // ����̫Զ
    ECOLLIDE_RESULT_ERR_HORI,               // ����ˮƽ�Ƕȷ�Χ
    ECOLLIDE_RESULT_ERR_VERT,               // ���ڴ�ֱ�Ƕȷ�Χ
    ECOLLIDE_RESULT_ERR_COLLIDE,            // ֱ�߾���֮������ײ
    ECOLLIDE_RESULT_ERR_RECTANGLE,          // ���ھ��η�Χ��
    ECOLLIDE_RESULT_ERR_TOOHIGH,            // Ŀ��λ��̫��
    ECOLLIDE_RESULT_ERR_TOOLOW,             // Ŀ��λ��̫��
    ECOLLIDE_RESULT_ERR_UNKNOWN,            // ��������
};

// 2dԲ
struct Cricle2D
{
    FmVec2 pos;
    float radius;

    Cricle2D():pos(0.0f,0.0f), radius(0.0f){}
    Cricle2D(const FmVec2& o, float r) : pos(o), radius(r){}
    Cricle2D(const Cricle2D& c) : pos(c.pos), radius(c.radius)
    {
    }
    Cricle2D operator=(const Cricle2D& c)
    {
        if (this == &c)
        {
            return *this;
        }
        pos = c.pos;
        radius = c.radius;
        return *this;
    }
    ~Cricle2D(){}

};
//2d����
struct Sector2D
{
    FmVec2 pos;
    FmVec2 orient;// ���ĳ���(0~2*pi)
    float radius; // �뾶
    float angle;// �Ƕȴ�С(0~2*pi)

    Sector2D():pos(0.0f,0.0f), orient(0.0f,1.0f), radius(0.0f), angle(0.0f){}
    Sector2D(const FmVec2& o, float fOrientAngle, float r, float a)
        : pos(o), orient(FMSIN(fOrientAngle), FMCOS(fOrientAngle)), radius(r), angle(a)
    {
        //orient.normalize();
        FmVec2Normalize(&orient, &orient);
        angle = ::util_normalize_angle(angle);
    }
    Sector2D(const FmVec2& o, const FmVec2& dir, float r, float a) 
        : pos(o), orient(dir), radius(r), angle(a)
    {
        //orient.normalize();
        FmVec2Normalize(&orient, &orient);
        angle = ::util_normalize_angle(angle);
    }
    Sector2D(const Sector2D& c) 
        : pos(c.pos), orient(c.orient), radius(c.radius), angle(c.angle)
    {
        //orient.normalize();
        FmVec2Normalize(&orient, &orient);
        angle = ::util_normalize_angle(angle);
    }
    Sector2D operator=(const Sector2D& c)
    {
        if (this == &c)
        {
            return *this;
        }
        pos = c.pos;
        orient = c.orient;
        radius = c.radius;
        angle = c.angle;

        //orient.normalize();
        FmVec2Normalize(&orient, &orient);
        angle = ::util_normalize_angle(angle);
        return *this;
    }
    ~Sector2D(){}
};
//2d����
struct Rectangle2D
{
    FmVec2 pos;// �������ĵ�λ��
    FmVec2 orient;// ���γ��򣬳�����width��ֱ����heightƽ��
    float width;
    float height;

    Rectangle2D():pos(0.0f,0.0f), orient(0.0f,1.0f), width(0.0f), height(0.0f){}
    Rectangle2D(const FmVec2& o, float fOrientAngle, float w, float h)
        : pos(o), orient(FMSIN(fOrientAngle), FMCOS(fOrientAngle)), width(w), height(h)
    {
        //orient.normalize();
        FmVec2Normalize(&orient, &orient);
    }
    Rectangle2D(const FmVec2& o, const FmVec2& dir, float w, float h) 
        : pos(o), orient(dir), width(w), height(h)
    {
        //orient.normalize();
        FmVec2Normalize(&orient, &orient);
    }
    Rectangle2D(const Rectangle2D& c) 
        : pos(c.pos), orient(c.orient), width(c.width), height(c.height)
    {
        //orient.normalize();
        FmVec2Normalize(&orient, &orient);
    }
    Rectangle2D operator=(const Rectangle2D& c)
    {
        if (this == &c)
        {
            return *this;
        }
        pos = c.pos;
        orient = c.orient;
        width = c.width;
        height = c.height;

        //orient.normalize();
        FmVec2Normalize(&orient, &orient);
        return *this;
    }
    ~Rectangle2D(){}
};

struct CricleRing2D
{
    FmVec2 pos;
    float innerRadius;
    float outerRadius;

    CricleRing2D():pos(0.0f,0.0f), innerRadius(0.0f), outerRadius(0.0f){}
    CricleRing2D(const FmVec2& o, float r, float R) : pos(o), innerRadius(r), outerRadius(R){}
    CricleRing2D(const CricleRing2D& c) : pos(c.pos), innerRadius(c.innerRadius), outerRadius(c.outerRadius)
    {
    }
    CricleRing2D operator=(const CricleRing2D& c)
    {
        if (this == &c)
        {
            return *this;
        }
        pos = c.pos;
        innerRadius = c.innerRadius;
        outerRadius = c.outerRadius;
        return *this;
    }
    ~CricleRing2D(){}
};

struct SectorRing2D
{
    FmVec2 pos;
    FmVec2 orient;// ���ĳ���(0~2*pi)
    float innerRadius; // �ڰ뾶
    float outerRadius; // ��뾶
    float angle;// �Ƕȴ�С(0~2*pi)

    SectorRing2D():pos(0.0f,0.0f), orient(0.0f,1.0f), innerRadius(0.0f), outerRadius(0.0f), angle(0.0f){}
    SectorRing2D(const FmVec2& o, float fOrient, float r, float R, float a)
        : pos(o), orient(FMSIN(fOrient), FMCOS(fOrient)), innerRadius(r), outerRadius(R), angle(a)
    {
        //orient.normalize();
        FmVec2Normalize(&orient, &orient);
        angle = ::util_normalize_angle(angle);
    }
    SectorRing2D(const FmVec2& o, const FmVec2& dir, float r, float R, float a)
        : pos(o), orient(dir), innerRadius(r), outerRadius(R), angle(a)
    {
        //orient.normalize();
        FmVec2Normalize(&orient, &orient);
        angle = ::util_normalize_angle(angle);
    }
    SectorRing2D(const SectorRing2D& c)
        : pos(c.pos), orient(c.orient), innerRadius(c.innerRadius), outerRadius(c.outerRadius), angle(c.angle)
    {
        //orient.normalize();
        FmVec2Normalize(&orient, &orient);
        angle = ::util_normalize_angle(angle);
    }
    SectorRing2D operator=(const SectorRing2D& c)
    {
        if (this == &c)
        {
            return *this;
        }
        pos = c.pos;
        orient = c.orient;
        innerRadius = c.innerRadius;
        outerRadius = c.outerRadius;
        angle = c.angle;

        //orient.normalize();
        FmVec2Normalize(&orient, &orient);
        angle = ::util_normalize_angle(angle);
        return *this;
    }
    ~SectorRing2D(){}
};

/// ---
namespace ShapeCollision
{
    // ����ͼ�ε���ײ���;�ж�Ŀ����״�Ƿ��뷶Χ��״�ཻ�����ڷ�Χ��״��
    extern ECollideResult Intersects(const Cricle2D& targetShape, const Cricle2D& rangeShape);
    extern ECollideResult Intersects(const Cricle2D& targetShape, const Sector2D& rangeShape);
    extern ECollideResult Intersects(const Cricle2D& targetShape, const Rectangle2D& rangeShape);
    extern ECollideResult Intersects(const Cricle2D& targetShape, const CricleRing2D& rangeShape);
    extern ECollideResult Intersects(const Cricle2D& targetShape, const SectorRing2D& rangeShape);
} // end of namespace ShapeCollision

#endif // __SHAPE_COLLISION_H__
