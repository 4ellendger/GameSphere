#pragma once

#include "Point3D.h"
#include "Vector3D.h"
#include "PreSet.h"

class Ray
{
private:
	Point3D _P;//Tочка
	Vector3D _V;//Ќаправление

public:
	//конструктор
	Ray() {}
	
	//лини€ между двум€ точками
	Ray(const Point3D &pPoint1, const Point3D &pPoint2);
	//точка и направление
	Ray(const Point3D &pPoint, const Vector3D &pDir);

	//ближайшие точки на лини€х
	void adjacentPoints(const Ray &ray, Vector3D &point1, Vector3D &point2) const;
	
	//унарные операторы
	static Ray &invert(const Ray &r, Ray &result)
	{ 
		result._P = r._P;
		//Vector3D::invert(r._V, result._V);
		result._V = r._V * -1;
		return result; 
	}
	Ray operator-() const 
	{ 
		return invert(*this, Ray());
	}
	
	//селекторы
	Vector3D P() const { return _P; }
	Vector3D V() const { return _V; }

	//рассто€ние
	double dist(const Ray & ray) const;
	double dist(const Vector3D & point) const;
};

