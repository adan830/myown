//----------------------------------------------------------
// �ļ���:      geometry_func.cpp
// ��  ��:      �������ͨ�ú���
// ˵  ��:
// ������:      shike
// ��������:    2011��06��
//    :        
//----------------------------------------------------------

#include "geometry_func.h"
#include <cmath>

bool util_is_point_in_sphere(const Point & point, const Sphere & sphere)
{
    // ����
    const Point & center = sphere.center;

    // ��������ж�
    const float dist2 = util_dot_distance2(point.x, point.y, point.z, center.x, center.y, center.z);
    if (dist2 > sphere.radius * sphere.radius)
    {
        return false;
    }

    return true;
}

bool util_is_point_in_cylinder(const Point & point, const Cylinder & cylinder)
{
    // Բ������Բ��
    const Point & center = cylinder.circle.center;

    // �жϸ߶ȷ�Χ
    const float min_y = center.y;
    const float max_y = min_y + cylinder.height;
    if (point.y < min_y || point.y > max_y)
    {
        return false;
    }

    // �жϵ��淶Χ
    const float dist2 = util_dot_distance2(point.x, point.z, center.x, center.z);
    if (dist2 > cylinder.circle.radius * cylinder.circle.radius)
    {
        return false;
    }

    return true;
}

bool util_is_point_in_sector_cylinder(const Point & point, const SectorCylinder & sc)
{
    // ����=>Բ��
    const Cylinder cylinder(sc);

    // �ж��Ƿ������������ɵ�Բ����
    if (!util_is_point_in_cylinder(point, cylinder))
    {
        return false;
    }

    // �ж��Ƿ������εĽǶȷ�Χ��
    const Point & center = sc.sector.center;
    const float point_dir = util_dot_angle(center.x, center.z, point.x, point.z);
    const float angle_diff = util_get_angle_diff(point_dir, sc.sector.direction);
    if (angle_diff > sc.sector.half_angle)
    {
        return false;
    }

    return true;
}

void util_random_point_in_circle(IKernel * pKernel,
                                 const Point & center,
                                 const float radius,
                                 Point & point)
{
    const float r = util_random_float(radius);
    const float angle = util_random_float(PI2);

    point.x = center.x + r * cosf(angle);
    point.z = center.z + r * sinf(angle);
    point.y = GetCurFloorHeight(pKernel, point.x, center.y, point.z);
}

TrigonometricWrap::TrigonometricWrap()
{
    float x = 0.0f; 
    for (int i = 0; i < SIZE; ++i)
    {
        m_sinValue[i] = sin(x);
        m_cosValue[i] = cos(x);

        x += PRECISION; 
    }
}

const TrigonometricWrap & TrigonometricWrap::instance() 
{
    static const TrigonometricWrap trigonometric_wrap;
    return trigonometric_wrap;
}

float TrigonometricWrap::sinf(const float dir) const
{
    const float nor_dir = util_normalize_angle(dir); 
    const int index = static_cast<int>(nor_dir / PRECISION);

    if (index < 0 || index > SIZE)
    {
        return 0.0f; 
    }

    return m_sinValue[index];
}

float TrigonometricWrap::cosf(const float dir) const
{
    const float nor_dir = util_normalize_angle(dir); 
    const int index = static_cast<int>(nor_dir / PRECISION);

    if (index < 0 || index > SIZE)
    {
        return 0.0f; 
    }

    return m_cosValue[index];
}

float dis_sinf(const float dir)
{
    return TrigonometricWrap::instance().sinf(dir); 
}

float dis_cosf(const float dir)
{
    return TrigonometricWrap::instance().cosf(dir); 
}

// end of file geometry_func.cpp

