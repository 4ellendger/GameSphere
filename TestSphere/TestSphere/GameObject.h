#pragma once

#include "Sphere.h"
#include "Point3D.h"
#include "Vector3D.h"

class GameObject
{
public:
	//������ (�����)
	Sphere * obj;
	UINT Index;
	//���������� ������ �������
	Point3D Center;
	//������ �������
	double Radius;
	//�������� �������� �������
	Vector3D Speed;

	GameObject(void);
	GameObject(Sphere & pObj, UINT pIndex, double pRadius, Point3D pCenter);
	~GameObject(void);

	void GameObject::doMove();
	void GameObject::doRedraw();
};

