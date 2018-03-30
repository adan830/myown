//----------------------------------------------------------
// �ļ���:      geometry_func.h
// ��  ��:      �������ͨ�ú���
// ˵  ��:
// ������:      shike
// ��������:    2011��06��
//    :        
//----------------------------------------------------------

#ifndef FSGAME_UTILITY_GEOMETRY_FUNC_H_
#define FSGAME_UTILITY_GEOMETRY_FUNC_H_

#include "util_func.h"
#include "extend_func.h"

// �������캯���͸�ֵ����������
#define DISALLOW_COPY(TypeName) \
    TypeName(const TypeName &);
#define DISALLOW_ASSIGN(TypeName) \
    void operator=(const TypeName &);
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
    DISALLOW_COPY(TypeName); \
    DISALLOW_ASSIGN(TypeName);

// ��
class Point
{
public:
    explicit Point(const float & init_x, const float & init_y, const float & init_z) :
        x(init_x),
        y(init_y),
        z(init_z)
    {
    }

    Point(const Point & point) : x(point.x), y(point.y), z(point.z)
    {
    }

    ~Point()
    {
    }

public:
    Point & operator=(const Point & point)
    {
        x = point.x;
        y = point.y;
        z = point.z;

        return *this;
    }

public:
    // 3������
    float x;
    float y;
    float z;
};

// ����
class Sphere
{
public:
    explicit Sphere(const Point & init_center, const float & init_radius) :
        center(init_center),
        radius(init_radius)
    {
    }

    ~Sphere()
    {
    }

public:
    // ����
    Point center;

    // �뾶
    float radius;

private:
    DISALLOW_COPY_AND_ASSIGN(Sphere);
};

// ����(��������x�����z���򹹳ɵ�ƽ����)
class Sector
{
public:
    explicit Sector(const Point & init_center, const float & init_radius, const float & init_direction, const float & init_half_angle) :
        center(init_center),
        radius(init_radius)
    {
        direction = util_normalize_angle(init_direction);
        half_angle = util_normalize_angle(init_half_angle);
        if (half_angle > PI)
        {
            half_angle = PI;
        }
    }

    explicit Sector(const Sector & sector) :
        center(sector.center),
        radius(sector.radius),
        direction(sector.direction),
        half_angle(sector.half_angle)
    {
    }

    ~Sector()
    {
    }

public:
    // Բ��
    Point center;

    // �뾶
    float radius;

    // ���ĳ���(0~2*pi)
    float direction;

    // ƫ�����ĳ������ҵĽǶ�(0~pi)
    float half_angle;

private:
    DISALLOW_ASSIGN(Sector);
};

// ����
class SectorCylinder
{
public:
    explicit SectorCylinder(const Sector & init_sector, const float & init_height) :
        sector(init_sector),
        height(init_height)
    {
    }

    ~SectorCylinder()
    {
    }

public:
    // ��������
    Sector sector;

    // Բ���߶�
    float height;

private:
    DISALLOW_COPY_AND_ASSIGN(SectorCylinder);
};

// Բ��(��������x�����z���򹹳ɵ�ƽ����)
class Circle
{
public:
    explicit Circle(const Point & init_center, const float & init_radius) :
        center(init_center),
        radius(init_radius)
    {
    }

    explicit Circle(const Sector & sector) :
        center(sector.center),
        radius(sector.radius)
    {
    }

    ~Circle()
    {
    }

public:
    // Բ��
    Point center;

    // �뾶
    float radius;

private:
    DISALLOW_ASSIGN(Circle);
};

// Բ��
class Cylinder
{
public:
    explicit Cylinder(const Circle & init_circle, const float & init_height) :
        circle(init_circle),
        height(init_height)
    {
    }

    explicit Cylinder(const SectorCylinder & sc) :
        circle(sc.sector.center, sc.sector.radius),
        height(sc.height)
    {
    }

    ~Cylinder()
    {
    }

public:
    // Բ������
    Circle circle;

    // Բ���߶�
    float height;

private:
    DISALLOW_COPY_AND_ASSIGN(Cylinder);
};

// �жϵ��Ƿ���������
bool util_is_point_in_sphere(const Point & point, const Sphere & sphere);

// �жϵ��Ƿ���Բ����
bool util_is_point_in_cylinder(const Point & point, const Cylinder & cylinder);

// �жϵ��Ƿ���������
bool util_is_point_in_sector_cylinder(const Point & point, const SectorCylinder & sc);

// ��Բ�������������
void util_random_point_in_circle(IKernel * pKernel, const Point & center, const float radius, Point & point);

// ���Ǻ�����
#define PRECISION (0.001f) // ��ɢ����
class TrigonometricWrap
{
private: 
    enum
    {
        SIZE = static_cast<int>(PI2 / PRECISION), // �����С
    };

private: 
    TrigonometricWrap(); 

public: 
    ~TrigonometricWrap()
    {
    }

    static const TrigonometricWrap & instance();

    float sinf(const float dir) const; 
    
    float cosf(const float dir) const; 

private: 
    float m_sinValue[SIZE + 1]; 
    float m_cosValue[SIZE + 1]; 
};

// ��ȡdir����ֵ��Ӧ����ɢsinֵ
float dis_sinf(const float dir); 

// ��ȡdir����ֵ��Ӧ����ɢcosֵ
float dis_cosf(const float dir); 

#endif

// end of file geometry_func.h

