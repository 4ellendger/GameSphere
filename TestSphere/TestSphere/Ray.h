#pragma once

#include "Point3D.h"
#include "Vector3D.h"
#include "PreSet.h"

class Ray
{
private:
	Point3D _P;//T����
	Vector3D _V;//�����������

public:
	//�����������
	Ray() {}
	
	//����� ����� ����� �������
	Ray(const Point3D &pPoint1, const Point3D &pPoint2);
	//����� � �����������
	Ray(const Point3D &pPoint, const Vector3D &pDir);

	//��������� ����� �� ������
	void adjacentPoints(const Ray &ray, Vector3D &point1, Vector3D &point2) const;
	
	//������� ���������
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
	
	//���������
	Vector3D P() const { return _P; }
	Vector3D V() const { return _V; }

	//����������
	double dist(const Ray & ray) const;
	double dist(const Vector3D & point) const;
};

