#pragma once

typedef unsigned int UINT;
typedef unsigned long DWORD;
typedef float GLfloat;
typedef unsigned int GLuint;

#include "Point3D.h"

struct VERTEX
{
	Point3D v; // ���������� �������
	Point3D n; // ���������� �������
	GLuint c; // ���� �������
}; 

struct TRIANGLE
{
//������� ���� ������ ������������,
//���������� �� ������� ������ ���� VERTEX
//������� ������ � ������ ������� �������
	UINT i1;
	UINT i2;
	UINT i3;
}; 

class Sphere
{
public:
	Sphere(void);
	Sphere::Sphere(double pRadius, UINT pCountRings, UINT pCountSects, GLuint pColor, UINT pListIndex);
	~Sphere(void);
};
