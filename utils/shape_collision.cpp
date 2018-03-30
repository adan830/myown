
//--------------------------------------------------------------------
// �ļ���:      code\mobile\tools\shape_collision.cpp
// ��  ��:      ͼ����ײ���
// ˵  ��:      ����ѧ���㣬ҵ������߼���Ҫ�ӵ�����
// ��������:    2014��4��3��
// ������:      longch
//    :       
//--------------------------------------------------------------------


#include "utils/shape_collision.h"
#include "utils/util_func.h"

namespace ShapeCollision
{
    const float EPSILON = 0.00001f;
    const float MY_PI = 3.1415927f;

    ECollideResult CheckCricleInCricle(const Cricle2D& targetShape, const Cricle2D& rangeShape);
    ECollideResult CheckCricleInSector(const Cricle2D& targetShape, const Sector2D& rangeShape);
    ECollideResult CheckCricleInRectangle(const Cricle2D& targetShape, const Rectangle2D& rangeShape);

    ECollideResult CheckCricleInCricleRing(const Cricle2D& targetShape, const CricleRing2D& rangeShape);
    ECollideResult CheckCricleInSectorRing(const Cricle2D& targetShape, const SectorRing2D& rangeShape);


    // ���߶Ȳ�
    ECollideResult CheckHeightDiff(float y, float dy, float fHeightDiff);

    // ��������
    struct Rect;
    struct RectStruct;
    struct FmVec3Wrap;

    // ����߶��Ƿ�������ཻ������һ�������жϣ���������rect��һ������ƽ�е�
    bool CheckRectLine(const FmVec2& start, const FmVec2& end, const Rect& rect);
    // CheckRectLineH��ˮƽ�����ϵļ�⣬���Ǽ����ε��ϱ������±��ߣ�CheckRectLineV�����ε�����ߺ��ұ��ߡ�
    bool CheckRectLineH(const FmVec2& start, const FmVec2& end, float y0, float x1, float x2);
    bool CheckRectLineV(const FmVec2& start, const FmVec2& end, float x0, float y1, float y2);

    // �����������Ƿ��ڷ�Χ��
    bool CheckPointToPointLessDist(const FmVec2& pt1, const FmVec2& pt2, float distance);

    // �жϵ��Ƿ��ھ��η�Χ��
    bool CheckPointInRectangle(const FmVec2& pt, const Rectangle2D& rangeShape);

    // ���ؾ����߶�����ĵ�ľ���
    // ptA,ptB���߶������˵����꣬pt��Ŀ��㣬dist������������
    // ����ֵ��-2����������A����룻-1�������ǵ㵽B��ľ��룻0�������߶��ϣ�1�����ؾ����ǵ㵽�߶εĴ�ֱ����
    int GetNearestDistance(const FmVec2& ptA, const FmVec2& ptB, const FmVec2& pt, float& dist);

    //////////////////////////////////////////////////////////////////////////
    /// ����������
    ECollideResult TestCircleInSectorRing(const Cricle2D& targetShape, const SectorRing2D& rangeShape);

    /// ���Ŀ����Ƿ�������������
    ECollideResult TestRectangleInSectorRing(const Rectangle2D& targetShape, const SectorRing2D& rangeShape);

    /// \brief ������Ŀ���Ƿ���һ��ָ���ľ��η�Χ��
    ECollideResult TestRectangleInRectangle(const Rectangle2D& targetShape, const Rectangle2D& rangeShape);

    //private:
    /// \brief  ���3ά�ռ��е������������Ƿ���ײ
    /// \param [in] baseRect ������ṹ
    /// \param [in] otherRect ��һ��������ṹ
    /// \return ���ؼ���� ps����������㷨��ĳ��������
    bool CheckRectToRect(RectStruct const& a, RectStruct const& b);

    // �����ж����νǶ��Ƿ����Ҫ��
    bool CheckFanToPoint(FmVec2 const& basePos, FmVec2 const& targetPos, float orien, float anglet);
}; // end of namespace ShapeCollision



// 3ά��
struct ShapeCollision::FmVec3Wrap : public FmVec3
{
public:
    FmVec3Wrap(void) 
    {
    }

    FmVec3Wrap(const VERTTYPE& init_x, const VERTTYPE& init_y, const VERTTYPE& init_z) 
        : FmVec3(init_x, init_y, init_z)
    {
    }

    FmVec3Wrap(const FmVec3Wrap& point)
        : FmVec3(point.x, point.y, point.z)
    {
    }

    operator VERTTYPE* ()
    {
        return (VERTTYPE*) &x;
    }

    operator const VERTTYPE* () const
    {
        return (const VERTTYPE*) &x;
    }

    FmVec3Wrap operator - (const FmVec3Wrap& v) const
    {
        return FmVec3Wrap(x - v.x, y - v.y, z - v.z);
    }

    ~FmVec3Wrap()
    {
    }
};


// ���������ṹ��
struct ShapeCollision::RectStruct
{
    //float x;    // x��zΪһ�������꣬�����Ǿ��ε���㣬Ҳ�����Ǿ��εĶ�����һ�����������е�
    //float z;
    //float orient;   // �Ƕȣ�width�Ǵ�ֱ��orient�ķ���height��ƽ����orient�ķ���
    //float width;    // ��
    //float height;

    FmVec3Wrap c; // OBB center point
    FmVec3Wrap u[3]; // Local x-, y-, and z-axes
    FmVec3Wrap e; // Positive halfwidth extents of OBB along each axis
};

// Region R = { x | x = c+r*u[0]+s*u[1]+t*u[2] }, |r|<=e[0], |s|<=e[1], |t|<=e[2]
//struct OBB {
//    D3DXVECTOR3 c; // OBB center point
//    D3DXVECTOR3 u[3]; // Local x-, y-, and z-axes
//    D3DXVECTOR3 e; // Positive halfwidth extents of OBB along each axis
//};


struct ShapeCollision::Rect
{
    FmVec2 leftTop;
    FmVec2 rightBottom;

public:
    Rect(float x1, float y1, float x2, float y2)
    {
        if (x1 < x2)
        {
            leftTop.x = x1;
            rightBottom.x = x2;
        }
        else
        {
            leftTop.x = x2;
            rightBottom.x = x1;
        }

        if (y1 < y2)
        {
            leftTop.y = y1;
            rightBottom.y = y2;
        }
        else
        {
            leftTop.y = y2;
            rightBottom.y = y1;
        }
    }
    bool Contains(const FmVec2& pt) const
    {
        bool result = true;
        result = result && leftTop.x <= pt.x;
        result = result && leftTop.y <= pt.y;
        result = result && pt.x <= rightBottom.x;
        result = result && pt.y <= rightBottom.y;
        return result;
    }
};



ECollideResult ShapeCollision::CheckCricleInCricle(const Cricle2D& targetShape, const Cricle2D& rangeShape)
{
    if (!CheckPointToPointLessDist(targetShape.pos, rangeShape.pos, targetShape.radius + rangeShape.radius))
    {
        return ECOLLIDE_RESULT_ERR_TOOFAR;
    }
    return ECOLLIDE_RESULT_INTERSECT;
}

ECollideResult ShapeCollision::CheckCricleInSector(const Cricle2D& targetShape, const Sector2D& rangeShape)
{
    const FmVec2 dist = targetShape.pos - rangeShape.pos;
    float fDistance = dist.length();

    float fHalfAngle = rangeShape.angle * 0.5f;

    //�����жϽǶ�
    FmVec2 vec2LineOrient = dist;
    //vec2LineOrient.normalize(); // ͼ�����ĵ����߷���ָ��Ŀ����״
    FmVec2Normalize(&vec2LineOrient, &vec2LineOrient);// ͼ�����ĵ����߷���ָ��Ŀ����״
    FmVec2 rangeShapeOrient;// = rangeShape.orient.normalized();
    FmVec2Normalize(&rangeShapeOrient, &(rangeShape.orient));
    float fDeltaOrient = FMACOS(FmVec2Dot(&vec2LineOrient, &rangeShapeOrient));
    if (fDeltaOrient > fHalfAngle)
    { // �Ƕ���������
        if (targetShape.radius < 0.001f)
        { // ����ˮƽ�Ƕȷ�Χ��
            return  ECOLLIDE_RESULT_ERR_HORI;
        }

        //////////////////////////////////////////////////////////////////////////
        float fBeltaAngle = fDeltaOrient - fHalfAngle; // Ŀ��㷽�����αߵĲ��
        float d = fDistance * sin(fBeltaAngle);  // Ŀ��㵽���αߵľ���
        if (d > targetShape.radius)
        {
            return ECOLLIDE_RESULT_ERR_HORI;
        }
    }

    // �жϾ���
    if (fDistance > rangeShape.radius + targetShape.radius)
    {
        return ECOLLIDE_RESULT_ERR_TOOFAR;
    }
    return ECOLLIDE_RESULT_INTERSECT;
}

ECollideResult ShapeCollision::CheckCricleInRectangle(const Cricle2D& targetShape, const Rectangle2D& rangeShape)
{
    // ���ε����Բ�뾶
    float fOuterRadius = (rangeShape.height + rangeShape.width) * 0.5f;
    // ������������Բ����ײ��ֱ�ӷ���
    if (ECOLLIDE_RESULT_INTERSECT != CheckCricleInCricle(targetShape, Cricle2D(rangeShape.pos, fOuterRadius)))
    {
        return ECOLLIDE_RESULT_ERR_TOOFAR;
    }

    // Ŀ�������ھ��η�Χ�ڣ����ཻ
    if (CheckPointInRectangle(targetShape.pos, rangeShape))
    {
        return ECOLLIDE_RESULT_INTERSECT;
    }

    float fHalfWidth = rangeShape.width * 0.5f;
    float fHalfHeight = rangeShape.height * 0.5f;
    FmVec2 vorient = rangeShape.orient.rotated90();
    // ƫ������
    FmVec2 hVec2 = rangeShape.orient * fHalfHeight;
    FmVec2 wVec2 = vorient * fHalfWidth;

    // �����ĸ�����
    FmVec2 A = rangeShape.pos - hVec2 - wVec2;
    FmVec2 B = rangeShape.pos + hVec2 - wVec2;
    FmVec2 C = rangeShape.pos + hVec2 + wVec2;
    FmVec2 D = rangeShape.pos - hVec2 + wVec2;
    //extend_warning(LOG_TRACE, "[ShapeCollision::CheckCricleInRectangle] A:(%f,%f) B:(%f,%f) C:(%f,%f) D:(%f,%f)", A.x, A.y, B.x, B.y, C.x, C.y, D.x, D.y);
    //extend_warning(LOG_TRACE, "[ShapeCollision::CheckCricleInRectangle] ���εİ볤��w=%f len:%f", fHalfWidth, fHalfHeight);

    // �ж�Ŀ�����ĵ������ߵľ���
    float dist = 0.0f;
    int ret = GetNearestDistance(A, B, targetShape.pos, dist);
    //extend_warning(LOG_TRACE, "[ShapeCollision::CheckCricleInRectangle] ���߶�AB����������dist:%f", dist);
    if (dist < targetShape.radius)
    {
        return ECOLLIDE_RESULT_INTERSECT;
    }
    ret = GetNearestDistance(B, C, targetShape.pos, dist);
    //extend_warning(LOG_TRACE, "[ShapeCollision::CheckCricleInRectangle] ���߶�BC����������dist:%f", dist);
    if (dist < targetShape.radius)
    {
        return ECOLLIDE_RESULT_INTERSECT;
    }
    ret = GetNearestDistance(C, D, targetShape.pos, dist);
    //extend_warning(LOG_TRACE, "[ShapeCollision::CheckCricleInRectangle] ���߶�CD����������dist:%f", dist);
    if (dist < targetShape.radius)
    {
        return ECOLLIDE_RESULT_INTERSECT;
    }
    ret = GetNearestDistance(D, A, targetShape.pos, dist);
    //extend_warning(LOG_TRACE, "[ShapeCollision::CheckCricleInRectangle] ���߶�DA����������dist:%f", dist);
    if (dist < targetShape.radius)
    {
        return ECOLLIDE_RESULT_INTERSECT;
    }
    return ECOLLIDE_RESULT_ERR_TOOFAR;
}

ECollideResult ShapeCollision::CheckCricleInCricleRing(const Cricle2D& targetShape, const CricleRing2D& rangeShape)
{
    FmVec2 dist = targetShape.pos - rangeShape.pos;
    float distSq = dist.lenSqr();
    float d = rangeShape.outerRadius + targetShape.radius;
    if (distSq > d*d)
    {// ���ڴ�Բ�ڣ���Ŀ��̫Զ
        return ECOLLIDE_RESULT_ERR_TOOFAR;
    }

    d = rangeShape.innerRadius - targetShape.radius;
    if (distSq < d*d)
    { // Ŀ����ȫ����Բ�У�̫��
        return ECOLLIDE_RESULT_ERR_TOONEAR;
    }
    return ECOLLIDE_RESULT_INTERSECT;
}

ECollideResult ShapeCollision::CheckCricleInSectorRing(const Cricle2D& targetShape, const SectorRing2D& rangeShape)
{
    const FmVec2 dist = targetShape.pos - rangeShape.pos;
    float fDistance = dist.length();

    float fHalfAngle = rangeShape.angle * 0.5f;

    //�����жϽǶ�
    FmVec2 vec2LineOrient = dist;
    //vec2LineOrient.normalize(); // ͼ�����ĵ����߷���ָ��Ŀ����״
    FmVec2Normalize(&vec2LineOrient, &vec2LineOrient);
    FmVec2 rangeShapeOrient;// = rangeShape.orient.normalized();
    FmVec2Normalize(&rangeShapeOrient, &(rangeShape.orient));
    float fDeltaOrient = FMACOS(FmVec2Dot(&vec2LineOrient, &rangeShapeOrient));
    if (fDeltaOrient > fHalfAngle)
    { // �Ƕ���������
        if (targetShape.radius < 0.001f)
        { // ����ˮƽ�Ƕȷ�Χ��
            return  ECOLLIDE_RESULT_ERR_HORI;
        }

        //////////////////////////////////////////////////////////////////////////
        float fBeltaAngle = fDeltaOrient - fHalfAngle; // Ŀ��㷽�����αߵĲ��
        float d = fDistance * sin(fBeltaAngle);  // Ŀ��㵽���αߵľ���
        if (d > targetShape.radius)
        {
            return ECOLLIDE_RESULT_ERR_HORI;
        }
    }

    // �жϾ���
    if (fDistance > rangeShape.outerRadius + targetShape.radius)
    {
        return ECOLLIDE_RESULT_ERR_TOOFAR;
    }
    if (fDistance < rangeShape.innerRadius - targetShape.radius)
    {
        return ECOLLIDE_RESULT_ERR_TOONEAR;
    }

    return ECOLLIDE_RESULT_INTERSECT;
}



ECollideResult ShapeCollision::CheckHeightDiff(float y, float dy, float fHeightDiff)
{
    // ���û�д��ݸ߶Ȳ����ݣ�ʹ��Ĭ�ϸ߲�����
    if (fHeightDiff < 0.0001f)
    {
        fHeightDiff = TARGET_FILTER_HIGHT_LIMIT;
    }

    //����߶Ȳ��Ƿ�������Χ��
    if (fHeightDiff < (y - dy))
    {
        return ECOLLIDE_RESULT_ERR_TOOLOW;
    }
    else if (fHeightDiff < (dy - y))
    {
        return ECOLLIDE_RESULT_ERR_TOOHIGH;
    }

    return ECOLLIDE_RESULT_INTERSECT;
}


bool ShapeCollision::CheckRectLine(const FmVec2& start, const FmVec2& end, const Rect& rect)
{
    bool result = false;
    if (rect.Contains(start) || rect.Contains(end))
        result = true;
    else
    {
        result = result || CheckRectLineH(start, end, rect.leftTop.y, rect.leftTop.x, rect.rightBottom.x);
        result = result || CheckRectLineH(start, end, rect.rightBottom.y, rect.leftTop.x, rect.rightBottom.x);
        result = result || CheckRectLineV(start, end, rect.leftTop.x, rect.leftTop.y, rect.rightBottom.y);
        result = result || CheckRectLineV(start, end, rect.rightBottom.x, rect.leftTop.y, rect.rightBottom.y);
    }

    return result;
}

// �߶�������Ƿ��ཻ�ķ����ͱ�����߶�����ε�4�����Ƿ��ཻ���м�⡣
// ����÷���CheckRectLineH��ˮƽ�����ϵļ�⣬���Ǽ����ε��ϱ������±��ߣ�
// ��CheckRectLineV���Ǽ����ε�����ߺ��ұ��ߡ�
// ���������ʵ���Ǽ��X1��Y0�Ľ�����X2��Y0�Ľ����Ƿ����ɫ�߶��Ƿ��ཻ��
// ����ཻ�����ж��佻���Ƿ���x1��x2֮��ķ�Χ��
bool ShapeCollision::CheckRectLineH(const FmVec2& start, const FmVec2& end, float y0, float x1, float x2)
{
    //ֱ���ڵ���Ϸ�
    if ((y0 < start.y) && (y0 < end.y))  return false;
    //ֱ���ڵ���·�
    if ((y0 > start.y) && (y0 > end.y))  return false;
    //ˮƽֱ��
    if (start.y == end.y)
    {
        //ˮƽֱ����㴦��ͬһˮƽ��
        if (y0 == start.y)
        {
            //ֱ���ڵ�����
            if ((start.x < x1) && (end.x < x1))  return false;
            //ֱ����x2��ֱ���ұ�
            if ((start.x > x2) && (end.x > x2))  return false;
            //ֱ�ߵĲ��ֻ���ȫ�����ڵ���x2��ֱ��֮��
            return true;
        }
        
        //ˮƽֱ����㲻����ͬһˮƽ��
        return false;
    }
    //б��
    float x = (end.x - start.x) * (y0 - start.y) / (end.y - start.y) + start.x;
    return ((x >= x1) && (x <= x2));
}


bool ShapeCollision::CheckRectLineV(const FmVec2& start, const FmVec2& end, float x0, float y1, float y2)
{
    if ((x0 < start.x) && (x0 < end.x))  return false;
    if ((x0 > start.x) && (x0 > end.x))  return false;
    if (start.x == end.x)
    {
        if (x0 == start.x)
        {
            if ((start.y < y1) && (end.y < y1)) 
            {
                return false;
            }
            if ((start.y > y2) && (end.y > y2))
            {
                return false;
            }
            return true;
        }
        
        return false;
    }
    float y = (end.y - start.y) * (x0 - start.x) / (end.x - start.x) + start.y;
    return ((y >= y1) && (y <= y2));
}


bool ShapeCollision::CheckPointToPointLessDist(const FmVec2& pt1, const FmVec2& pt2, float distance)
{
    FmVec2 dist = pt1 - pt2;
    return distance * distance >= dist.x * dist.x + dist.y * dist.y;
}

// �жϵ��Ƿ��ھ��η�Χ��
bool ShapeCollision::CheckPointInRectangle(const FmVec2& pt, const Rectangle2D& rangeShape)
{
    // �Ծ�������Ϊ����ԭ�㣬����ԭ�㵽P������OP
    FmVec2 OP = pt - rangeShape.pos;

    // P������ھ�������ϵ������(h,w)
    float h = FmVec2Dot(&OP, &(rangeShape.orient));    
    if (FMABS(h) > rangeShape.height * 0.5f)
    {
        return false;
    }

    FmVec2 vorient = rangeShape.orient.rotated90();
    float w = FmVec2Dot(&OP, &vorient);
    if (FMABS(w) > rangeShape.width*0.5f)
    {
        return false;
    }

    return true;
}

// ���ؾ����߶�����ĵ�ľ���
// ptA,ptB���߶������˵����꣬pt��Ŀ��㣬dist������������
// ����ֵ��-2����������A����룻-1�������ǵ㵽B��ľ��룻0�������߶��ϣ�1�����ؾ����ǵ㵽�߶εĴ�ֱ����
int ShapeCollision::GetNearestDistance(const FmVec2& ptA, const FmVec2& ptB, const FmVec2& pt, float& dist)
{
    FmVec2 PA = ptA - pt;
    FmVec2 PB = ptB - pt;
    FmVec2 AB = ptB - ptA;
    
    float a = PB.length();
    if (a <= EPSILON)
    {// P��B���غ�
        dist = 0.0f;
        return 0;
    }

    float b = PA.length();
    if (b <= EPSILON)
    {// P��A���غ�
        dist = 0.0f;
        return 0;
    }

    float c = AB.length();
    if (c <= EPSILON)
    {// A����B���غ�
        dist = a;
        return 1;
    }

    if (a*a >= b*b + c*c)
    {// ��BA�ӳ��߷�����A�����
        dist = b;
        return -2;
    }
//     else if(FMABS(a*a - (b*b + c*c)) < EPSILON)
//     {
//         dist = b;
//         return 1;
//     }

    if (b*b >= a*a + c*c)
    { // ��AB�ӳ��߷�����B�����
        dist = a;
        return -1;
    }
//     else if (FMABS(b*b - (a*a + c*c)) < EPSILON)
//     {
//         dist = a;
//         return 1;
//     }

    float len = (a+b+c)*0.5f; // �ܳ���һ��
    float s = sqrt(len * (len-a) * (len-b) * (len-c)); // ���׹�ʽ�����
    dist = 2*s/c;
    return 1;
}


//////////////////////////////////////////////////////////////////////////

ECollideResult ShapeCollision::TestCircleInSectorRing(const Cricle2D& targetShape, const SectorRing2D& rangeShape)
{
    FmVec2 vec2BasePos = rangeShape.pos;
    FmVec2 vec2TargetPos = targetShape.pos;
    float fHalfAngle = rangeShape.angle * 0.5f;
    const FmVec2 vec2ShapeOrient = rangeShape.orient;

    FmMat4 mat;
    FmVec2 rightLine, leftLine, rightNormal, leftNormal;

    //right line
    mat.RotationY(-fHalfAngle);
    FmVec2TransformNormal(&rightLine, &vec2ShapeOrient, &mat);
    //rightLine.normalize();
    FmVec2Normalize(&rightLine, &rightLine);
    rightNormal = rightLine.rotated90();//FmVec2(-rightLine.y, rightLine.x);

    //left line
    mat.RotationY(fHalfAngle);
    FmVec2TransformNormal(&leftLine, &vec2ShapeOrient, &mat);
    //leftLine.normalize();
    FmVec2Normalize(&leftLine, &leftLine);
    leftNormal = -leftNormal.rotated90();//FmVec2(leftNormal.y, -leftNormal.x);

    float cr = 0 - FmVec2Dot(&rightNormal, &vec2BasePos);
    float cl = 0 - FmVec2Dot(&leftNormal, &vec2BasePos);

    //a*x + b*y +c = 0
    float distToRight = FmVec2Dot(&vec2TargetPos, &rightNormal) + cr;
    float distToLeft = FmVec2Dot(&vec2TargetPos, &leftNormal) + cl;

    // �ų����ߵ�λ��
    if (distToRight< 0.0f-targetShape.radius)
        return ECOLLIDE_RESULT_ERR_HORI;

    if (distToLeft< 0.0f-targetShape.radius)
        return ECOLLIDE_RESULT_ERR_HORI;

    // �ų�Բ�����λ��
    FmVec2 dirWithLength = vec2TargetPos - vec2BasePos;
    float fDistance = dirWithLength.length();
    if (fDistance > rangeShape.outerRadius + targetShape.radius)
    {
        return ECOLLIDE_RESULT_ERR_TOOFAR;
    }
    if (fDistance < rangeShape.innerRadius - targetShape.radius)
    {
        return ECOLLIDE_RESULT_ERR_TOONEAR;
    }

    return ECOLLIDE_RESULT_INTERSECT;
}


// ���κ;�����ײ
ECollideResult ShapeCollision::TestRectangleInSectorRing(const Rectangle2D& targetShape, const SectorRing2D& rangeShape)
{
    // ���ε����Բ�뾶
    float fOuterRadius = (targetShape.height + targetShape.width) * 0.5f;
    // ������������Բ����ײ��ֱ�ӷ���
    if (!CheckPointToPointLessDist(targetShape.pos, rangeShape.pos, rangeShape.outerRadius + fOuterRadius))
    {
        return ECOLLIDE_RESULT_ERR_TOOFAR;
    }

    // ����������ĵ��������󣬷�������ε��ĸ����λ��
    FmMat4 rectWorldMat, rectRotaMat;
    FmMat4Translation(&rectWorldMat, targetShape.pos.x, 0, targetShape.pos.y);
    float fOrient = util_normalize_angle(targetShape.height);
    FmMat4RotationY(&rectRotaMat, fOrient);
    FmMat4Multiply(&rectWorldMat, &rectRotaMat, &rectWorldMat);

    float x0 = -targetShape.width * 0.5f;
    float x1 = targetShape.width * 0.5f;
    float z0 = -targetShape.height * 0.5f;
    float z1 = targetShape.height * 0.5f;
    // ���ε��ĸ�����
    FmVec3 posTemp[4];
    posTemp[0] = FmVec3(x1, 0.0f, z1);
    posTemp[1] = FmVec3(x0, 0.0f, z1);
    posTemp[2] = FmVec3(x1, 0.0f, z0);
    posTemp[3] = FmVec3(x0, 0.0f, z0);

    FmVec3 pos[4];
    FmVec3TransformCoordStream(pos, sizeof(FmVec3), posTemp, sizeof(FmVec3), 4, &rectWorldMat);

    // ����Բ�ĵ������ĸ���������ĵ�Ĵ�Ȩ����
    FmVec2 vert[5];    // ���һ��vecΪ�е㵽����Բ�ĵ�����
    vert[0] = FmVec2(pos[0].x, pos[0].z) - rangeShape.pos;
    vert[1] = FmVec2(pos[1].x, pos[1].z) - rangeShape.pos;
    vert[2] = FmVec2(pos[2].x, pos[2].z) - rangeShape.pos;
    vert[3] = FmVec2(pos[3].x, pos[3].z) - rangeShape.pos;
    vert[4] = targetShape.pos - rangeShape.pos;

    // ֻҪ��һ�����ڽǶȺ;���������Ҫ�󣬾Ϳ�����
    // �����ε������ߵĽǶ�
    float fHalfAngle = rangeShape.angle * 0.5f;
    float leng;
    float fMaxDistSquare, fMinDistSquare;
    fMaxDistSquare = rangeShape.outerRadius * rangeShape.outerRadius;
    fMinDistSquare = rangeShape.innerRadius * rangeShape.innerRadius;
    float fShapeOrient = FMACOS(rangeShape.orient.y);
    for (int i = 0; i < 4; ++i)
    {
        if (CheckFanToPoint(rangeShape.pos, FmVec2(pos[i].x, pos[i].z), fShapeOrient, fHalfAngle))
        {
            // �Ƕȷ��ϣ��жϾ���
            leng = vert[i].x * vert[i].x + vert[i].y * vert[i].y; //���ȵ�ƽ��
            if (leng <= fMaxDistSquare && leng >= fMinDistSquare)
            {
                return ECOLLIDE_RESULT_INTERSECT;
            }
        }
    }

    if (CheckFanToPoint(rangeShape.pos, targetShape.pos, fShapeOrient, fHalfAngle))
    {
        // �Ƕȷ��ϣ��жϾ���
        leng = vert[4].x * vert[4].x + vert[4].y * vert[4].y; //���ȵ�ƽ��
        if (leng <= fMaxDistSquare && leng >= fMinDistSquare)
        {
            return ECOLLIDE_RESULT_INTERSECT;
        }
    }

    // ���濼���������
    // 1.�����ھ����ڲ�����Բ�ı��ھ�����
    //Ŀ������������ʼ��ķ���
    float fRelateionOrient = util_dot_angle(rangeShape.pos.x, rangeShape.pos.y, targetShape.pos.x, targetShape.pos.y) - fOrient;
    //Ŀ������������ʼ��ľ���
    float fDistance = util_dot_distance(rangeShape.pos.x, rangeShape.pos.y, targetShape.pos.x, targetShape.pos.y);

    //��Ŀ��������ת������Ծ�����ʼ����������
    float fTargetX = fDistance * ::sin(fRelateionOrient);
    float fTargetZ = fDistance * ::cos(fRelateionOrient);

    if (fTargetX >= x0 && fTargetX <= x1 && fTargetZ >= z0 && fTargetZ <= z1)
    {
        return ECOLLIDE_RESULT_INTERSECT;
    }

    // ���ε���������ֱ���
    float fx1 = fTargetX + rangeShape.outerRadius*::sin(fRelateionOrient + fHalfAngle);
    float fz1 = fTargetZ + rangeShape.outerRadius*::cos(fRelateionOrient + fHalfAngle);
    float fx2 = fTargetX + rangeShape.outerRadius*::sin(fRelateionOrient - fHalfAngle);
    float fz2 = fTargetZ + rangeShape.outerRadius*::cos(fRelateionOrient - fHalfAngle);

    // ������ֱ�ߵĽ����Ƿ��ھ�����
    if (CheckRectLine(FmVec2(fTargetX, fTargetZ), FmVec2(fx1, fz1), Rect(x0, z0, x1,z1)))
    {
        return ECOLLIDE_RESULT_INTERSECT;
    }

    if (CheckRectLine(FmVec2(fTargetX, fTargetZ), FmVec2(fx2, fz2), Rect(x0, z0, x1,z1)))
    {
        return ECOLLIDE_RESULT_INTERSECT;
    }

    if (CheckRectLine(FmVec2(fx2, fz2), FmVec2(fx1, fz1), Rect(x0, z0, x1,z1)))
    {
        return ECOLLIDE_RESULT_INTERSECT;
    }

    return ECOLLIDE_RESULT_ERR_TOOFAR;;
}

// ���κ;�����ײ
ECollideResult ShapeCollision::TestRectangleInRectangle(const Rectangle2D& targetShape, const Rectangle2D& rangeShape)
{
    //rangeShape�ǹ�����Χ��targetShape�Ǳ���������״���������ײ��ָ�������߷ŵļ����˺������Ǿ��Σ����������ߵ���������Ҳ�Ǿ���

    //���Բ�뾶
    float rangeOuterRadius = (rangeShape.height + rangeShape.width) * 0.5f;
    // ��һ�����Բ�뾶
    float targetOuterRadius = (targetShape.height + targetShape.width) * 0.5f;
    // ������Բû���ཻ����ֱ�ӷ���
    if (!CheckPointToPointLessDist(rangeShape.pos, targetShape.pos, rangeOuterRadius + targetOuterRadius))
    {
        return ECOLLIDE_RESULT_ERR_TOOFAR;
    }

    RectStruct baseRect, otherRect;
    baseRect.c = FmVec3Wrap(rangeShape.pos.x, 0.0f, rangeShape.pos.y);
    baseRect.u[0] = FmVec3Wrap(rangeShape.pos.x, 0.0f, rangeShape.orient.y);
    baseRect.u[1] = FmVec3Wrap(0.0f, 1.0f, 0.0f);
    baseRect.u[2] = FmVec3Wrap(rangeShape.orient.y, 0.0f, -rangeShape.orient.x);
    baseRect.e = FmVec3Wrap(rangeShape.height * 0.5f, 3.0f, rangeShape.width * 0.5f);

    otherRect.c = FmVec3Wrap(targetShape.pos.x, 0.0f, targetShape.pos.y);
    otherRect.u[0] = FmVec3Wrap(targetShape.pos.x, 0.0f, targetShape.pos.y);
    otherRect.u[1] = FmVec3Wrap(0.0f, 1.0f, 0.0f);
    otherRect.u[2] = FmVec3Wrap(targetShape.pos.y, 0.0f, -targetShape.pos.x);
    otherRect.e = FmVec3Wrap(targetShape.height * 0.5f, 3.0f, targetShape.width * 0.5f);

    if (CheckRectToRect(baseRect, otherRect))
    {
        return ECOLLIDE_RESULT_INTERSECT;
    }

    return ECOLLIDE_RESULT_ERR_RECTANGLE;
}


// ps:��������Ǵ�ĳ���������Ͽ������ģ�������������ײ��
bool ShapeCollision::CheckRectToRect(RectStruct const& a, RectStruct const& b)
{
    float ra, rb;
    float R[3][3], AbsR[3][3];
    // Compute rotation matrix exprescosg b in a’s coordinate frame
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            R[i][j] = FmVec3Dot(&(a.u[i]), &(b.u[j]));
        }
    }

    // Compute translation vector t
    FmVec3Wrap t = b.c - a.c;
    // Bring translation into a’s coordinate frame
    t = FmVec3Wrap(FmVec3Dot(&t, &(a.u[0])), FmVec3Dot(&t, &(a.u[2])), FmVec3Dot(&t, &(a.u[2])));
    // Compute common subexpressions. Add in an epsilon term to
    // counteract arithmetic errors when two edges are parallel and
    // their cross product is (near) null (see text for details)
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            AbsR[i][j] = fabs(R[i][j]) + EPSILON;
        }
    }
    // Test axes L = A0, L = A1, L = A2
    for (int i = 0; i < 3; i++)
    {
        ra = a.e[i];
        rb = b.e[0] * AbsR[i][0] + b.e[1] * AbsR[i][1] + b.e[2] * AbsR[i][2];
        if (fabs(t[i]) > ra + rb)
        {
            return false;
        }
    }
    // Test axes L = B0, L = B1, L = B2
    for (int i = 0; i < 3; i++)
    {
        ra = a.e[0] * AbsR[0][i] + a.e[1] * AbsR[1][i] + a.e[2] * AbsR[2][i];
        rb = b.e[i];
        if (fabs(t[0] * R[0][i] + t[1] * R[1][i] + t[2] * R[2][i]) > ra + rb)
        {
            return false;
        }
    }
    // Test axis L = A0 x B0
    ra = a.e[1] * AbsR[2][0] + a.e[2] * AbsR[1][0];
    rb = b.e[1] * AbsR[0][2] + b.e[2] * AbsR[0][1];
    if (fabs(t[2] * R[1][0] - t[1] * R[2][0]) > ra + rb)
    {
        return false;
    }
    // Test axis L = A0 x B1
    ra = a.e[1] * AbsR[2][1] + a.e[2] * AbsR[1][1];
    rb = b.e[0] * AbsR[0][2] + b.e[2] * AbsR[0][0];
    if (fabs(t[2] * R[1][1] - t[1] * R[2][1]) > ra + rb)
    {
        return false;
    }
    // Test axis L = A0 x B2
    ra = a.e[1] * AbsR[2][2] + a.e[2] * AbsR[1][2];
    rb = b.e[0] * AbsR[0][1] + b.e[1] * AbsR[0][0];
    if (fabs(t[2] * R[1][2] - t[1] * R[2][2]) > ra + rb)
    {
        return false;
    }
    // Test axis L = A1 x B0
    ra = a.e[0] * AbsR[2][0] + a.e[2] * AbsR[0][0];
    rb = b.e[1] * AbsR[1][2] + b.e[2] * AbsR[1][1];

    if (fabs(t[0] * R[2][0] - t[2] * R[0][0]) > ra + rb)
    {
        return false;
    }
    // Test axis L = A1 x B1
    ra = a.e[0] * AbsR[2][1] + a.e[2] * AbsR[0][1];
    rb = b.e[0] * AbsR[1][2] + b.e[2] * AbsR[1][0];
    if (fabs(t[0] * R[2][1] - t[2] * R[0][1]) > ra + rb)
    {
        return false;
    }
    // Test axis L = A1 x B2
    ra = a.e[0] * AbsR[2][2] + a.e[2] * AbsR[0][2];
    rb = b.e[0] * AbsR[1][1] + b.e[1] * AbsR[1][0];
    if (fabs(t[0] * R[2][2] - t[2] * R[0][2]) > ra + rb)
    {
        return false;
    }
    // Test axis L = A2 x B0
    ra = a.e[0] * AbsR[1][0] + a.e[1] * AbsR[0][0];
    rb = b.e[1] * AbsR[2][2] + b.e[2] * AbsR[2][1];
    if (fabs(t[1] * R[0][0] - t[0] * R[1][0]) > ra + rb)
    {
        return false;
    }
    // Test axis L = A2 x B1
    ra = a.e[0] * AbsR[1][1] + a.e[1] * AbsR[0][1];
    rb = b.e[0] * AbsR[2][2] + b.e[2] * AbsR[2][0];
    if (fabs(t[1] * R[0][1] - t[0] * R[1][1]) > ra + rb)
    {
        return false;
    }
    // Test axis L = A2 x B2
    ra = a.e[0] * AbsR[1][2] + a.e[1] * AbsR[0][2];
    rb = b.e[0] * AbsR[2][1] + b.e[1] * AbsR[2][0];
    if (fabs(t[1] * R[0][2] - t[0] * R[1][2]) > ra + rb)
    {
        return false;
    }
    // cosce no separating axis is found, the OBBs must be intersecting
    return true;
}


bool ShapeCollision::CheckFanToPoint(FmVec2 const& basePos, FmVec2 const& targetPos, float orien, float anglet)
{
    float fOrient = ::util_normalize_angle(orien);
    float fLineOrient = util_dot_angle(basePos.x, basePos.y, targetPos.x, targetPos.y);
    fLineOrient = util_normalize_angle(fLineOrient);

    float fDeltaOrient = util_normalize_angle(fLineOrient - fOrient);
    if (fDeltaOrient > MY_PI)
    {
        fDeltaOrient = util_normalize_angle(fOrient - fLineOrient);
    }

    if (fDeltaOrient > anglet)
    {
        return false;
    }
    return true;
}


namespace ShapeCollision
{
    // ����ͼ�ε���ײ���;�ж�Ŀ����״�Ƿ��뷶Χ��״�ཻ�����ڷ�Χ��״��
    ECollideResult Intersects(const Cricle2D& targetShape, const Cricle2D& rangeShape)
    {
        ECollideResult result = CheckCricleInCricle(targetShape, rangeShape);

//         extend_warning(LOG_TRACE,
//             "[ShapeCollision::Intersects] target Cricle2D pos:(%f,%f) r:%f  range Cricle2D pos:(%f,%f) r:%f  result: %d",
//             targetShape.pos.x, targetShape.pos.y, targetShape.radius,
//             rangeShape.pos.x, rangeShape.pos.y, rangeShape.radius,
//             result);

        return result;
    }

    ECollideResult Intersects(const Cricle2D& targetShape, const Sector2D& rangeShape)
    {
        ECollideResult result = CheckCricleInSector(targetShape, rangeShape);

//         extend_warning(LOG_TRACE,
//             "[ShapeCollision::Intersects] target Cricle2D pos:(%f,%f) r:%f  range Sector2D pos:(%f,%f) r:%f angle:%f  result: %d",
//             targetShape.pos.x, targetShape.pos.y, targetShape.radius,
//             rangeShape.pos.x, rangeShape.pos.y, rangeShape.radius, rangeShape.angle,
//             result);

        return result;
    }

    ECollideResult Intersects(const Cricle2D& targetShape, const Rectangle2D& rangeShape)
    {
        ECollideResult result = CheckCricleInRectangle(targetShape, rangeShape);

//         extend_warning(LOG_TRACE,
//             "[ShapeCollision::Intersects] target Cricle2D pos:(%f,%f) r:%f  range Rectangle2D pos:(%f,%f) orient(%f,%f) w:%f h:%f  result: %d",
//             targetShape.pos.x, targetShape.pos.y, targetShape.radius,
//             rangeShape.pos.x, rangeShape.pos.y, rangeShape.orient.x, rangeShape.orient.y, 
//             rangeShape.width, rangeShape.height,
//             result);

        return result;
    }

    ECollideResult Intersects(const Cricle2D& targetShape, const CricleRing2D& rangeShape)
    {
        ECollideResult result = CheckCricleInCricleRing(targetShape, rangeShape);

//         extend_warning(LOG_TRACE,
//             "[ShapeCollision::Intersects] target Cricle2D pos:(%f,%f) r:%f  range CricleRing2D pos:(%f,%f) R:%f r:%f  result: %d",
//             targetShape.pos.x, targetShape.pos.y, targetShape.radius,
//             rangeShape.pos.x, rangeShape.pos.y, rangeShape.outerRadius, rangeShape.innerRadius,
//             result);

        return result;
    }

    ECollideResult Intersects(const Cricle2D& targetShape, const SectorRing2D& rangeShape)
    {
        ECollideResult result = CheckCricleInSectorRing(targetShape, rangeShape);

//         extend_warning(LOG_TRACE,
//             "[ShapeCollision::Intersects] target Cricle2D pos:(%f,%f) r:%f  range SectorRing2D pos:(%f,%f) orient(%f,%f) R:%f r:%f angle:%f  result: %d",
//             targetShape.pos.x, targetShape.pos.y, targetShape.radius,
//             rangeShape.pos.x, rangeShape.pos.y, rangeShape.orient.x, rangeShape.orient.y,
//             rangeShape.outerRadius, rangeShape.innerRadius, rangeShape.angle,
//             result);

        return result;
    }
}

