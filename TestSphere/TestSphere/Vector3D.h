#pragma once

#include <math.h>
#include "PreSet.h"

class Vector3D
{
public:
    double   x;
    double   y;
    double   z;
        
    Vector3D() 
	{
        x = 0;
        y = 0;
        z = 0;
	}
        
    Vector3D(double r, double s, double t)
    {
        x = r;
        y = s;
        z = t;
    }
        
    Vector3D& Set(double r, double s, double t)
    {
        x = r;
        y = s;
        z = t;
        return (*this);
    }

    double& operator [](long k)
    {
        return ((&x)[k]);
    }
        
    const double& operator [](long k) const
    {
        return ((&x)[k]);
    }
                
    Vector3D& operator +=(const Vector3D& v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
        return (*this);
    }
                
    Vector3D& operator -=(const Vector3D& v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return (*this);
    }
                
    Vector3D& operator *=(double t)
    {
        x *= t;
        y *= t;
        z *= t;
        return (*this);
    }
        
    Vector3D& operator /=(double t)
    {
        double f = 1.0F / t;
        x *= f;
        y *= f;
        z *= f;
        return (*this);
    }
        
    Vector3D& operator %=(const Vector3D& v)
    {
        double       r, s;
            
        r = y * v.z - z * v.y;
        s = z * v.x - x * v.z;
        z = x * v.y - y * v.x;
        x = r;
        y = s;
            
        return (*this);
    }
        
    Vector3D& operator &=(const Vector3D& v)
    {
        x *= v.x;
        y *= v.y;
        z *= v.z;
        return (*this);
    }
        
    Vector3D operator -(void) const
    {
        return (Vector3D(-x, -y, -z));
    }
    
    Vector3D operator +(const Vector3D& v) const
    {
        return (Vector3D(x + v.x, y + v.y, z + v.z));
    }
                
    Vector3D operator -(const Vector3D& v) const
    {
        return (Vector3D(x - v.x, y - v.y, z - v.z));
    }
        
    Vector3D operator *(double t) const
    {
        return (Vector3D(x * t, y * t, z * t));
    }
        
    Vector3D operator /(double t) const
    {
        double f = 1.0F / t;
        return (Vector3D(x * f, y * f, z * f));
    }
        
    double operator *(const Vector3D& v) const
    {
        return (x * v.x + y * v.y + z * v.z);
    }
        
    Vector3D operator %(const Vector3D& v) const
    {
        return (Vector3D(y * v.z - z * v.y, z * v.x - x * v.z,
                x * v.y - y * v.x));
    }
        
    Vector3D operator &(const Vector3D& v) const
    {
        return (Vector3D(x * v.x, y * v.y, z * v.z));
    }
        
    bool operator ==(const Vector3D& v) const
    {
        return ((x == v.x) && (y == v.y) && (z == v.z));
    }
        
    bool operator !=(const Vector3D& v) const
    {
        return ((x != v.x) || (y != v.y) || (z != v.z));
    }
    

    Vector3D& Normalize(void)
    {
        return (*this /= sqrt(x * x + y * y + z * z));
    }
        
    Vector3D& RotateAboutX(double angle);
    Vector3D& RotateAboutY(double angle);
    Vector3D& RotateAboutZ(double angle);
    Vector3D& RotateAboutAxis(double angle, const Vector3D& axis);

	//скалярное произведение
	inline double Dot(const Vector3D& v1, const Vector3D& v2)
	{
		return (v1 * v2);
	}

	//пересечение векторов
	inline Vector3D Cross(const Vector3D& v1, const Vector3D& v2)
	{
		return (v1 % v2);
	}

	//длина вектора в единицах длины
	inline double Magnitude(const Vector3D& v)
	{
		return (sqrt(v.x * v.x + v.y * v.y + v.z * v.z));
	}

	// 1 / длину вектора 
	inline double InverseMag(const Vector3D& v)
	{
		return (1.0 / sqrt(v.x * v.x + v.y * v.y + v.z * v.z));
	}

	//квадрат длины
	inline double SquaredMag(const Vector3D& v)
	{
		return (v.x * v.x + v.y * v.y + v.z * v.z);
	}

	//нормализация вектора (коллинеарный с единичной длиной)
	Vector3D &Vector3D::unit() 
	{
		double rep = Magnitude(*this);
		if (rep < EPSILON_NONZERO_VECTOR) 
		{
			x = 0.0;
			y = 0.0;
			z = 0.0;
		} 
		else 
		{
			double temp = 1.0 / rep;
			x *= temp;
			y *= temp;
			z *= temp;
		}
		return *this;
	}

};


