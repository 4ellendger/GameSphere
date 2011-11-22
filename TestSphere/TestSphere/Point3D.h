#pragma once

#include <math.h>
#include "Vector3D.h"

class Point3D : public Vector3D
{
public:
        
    Point3D() {}
        
    Point3D(double r, double s, double t) : Vector3D(r, s, t) {}
        
    Point3D& operator =(const Vector3D& v)
    {
        x = v.x;
        y = v.y;
        z = v.z;
        return (*this);
    }
        
    Point3D& operator *=(double t)
    {
        x *= t;
        y *= t;
        z *= t;
        return (*this);
    }
        
    Point3D& operator /=(double t)
    {
        double f = 1.0F / t;
        x *= f;
        y *= f;
        z *= f;
        return (*this);
    }
        
    Point3D& operator &=(const Vector3D& v)
    {
        x *= v.x;
        y *= v.y;
        z *= v.z;
        return (*this);
    }
        
    Point3D operator -(void) const
    {
        return (Point3D(-x, -y, -z));
    }
        
    Point3D operator +(const Vector3D& v) const
    {
        return (Point3D(x + v.x, y + v.y, z + v.z));
    }
        
    Point3D operator -(const Vector3D& v) const
    {
        return (Point3D(x - v.x, y - v.y, z - v.z));
    }
        
    Vector3D operator -(const Point3D& p) const
    {
        return (Vector3D(x - p.x, y - p.y, z - p.z));
    }
        
    Point3D operator *(double t) const
    {
        return (Point3D(x * t, y * t, z * t));
    }
        
    Point3D operator /(double t) const
    {
        double f = 1.0F / t;
        return (Point3D(x * f, y * f, z * f));
    }
        
    Point3D operator &(const Vector3D& v) const
    {
        return (Point3D(x * v.x, y * v.y, z * v.z));
    }
};
