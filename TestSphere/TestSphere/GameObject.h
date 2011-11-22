#pragma once

#include "Sphere.h"
#include "Point3D.h"
#include "Vector3D.h"

class GameObject
{
public:
	//объект (сфера)
	Sphere * obj;
	UINT Index;
	//координаты центра объекта
	Point3D Center;
	//радиус объекта
	double Radius;
	//скорость движения объекта
	Vector3D Speed;

	GameObject(void);
	GameObject(Sphere & pObj, UINT pIndex, double pRadius, Point3D pCenter);
	~GameObject(void);

	void GameObject::doMove();
	void GameObject::doRedraw();
};

