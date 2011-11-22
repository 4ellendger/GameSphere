#include "Sphere.h"

#include <windows.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <gl/gl.h>
#include <stdio.h>

#include <iostream>
#include <sstream>
using std::stringstream;

VERTEX * v;
TRIANGLE * t;

Sphere::Sphere(void)
{
}


Sphere::~Sphere(void)
{
}

/*����������� ����� 
(pCountRings+1)*pCountSects+2 �� ������ ���� ������ 4294967295
*/
Sphere::Sphere(double pRadius, UINT pCountRings, UINT pCountSects, GLuint pColor, UINT pListIndex)
{
	//����� ���-�o ������������� 
	UINT CountTrian = (pCountRings + 1) * pCountSects * 2;
	//����� ���-�� ������
	UINT CountVertex = (pCountRings + 1) * pCountSects + 2;
	//da � ��� ��������� ������������ ���� � 
	//(���� ����� ��������� ������ ����������� � ��������� ���������� �� ��� y)
	double da = M_PI / (pCountRings + 2.0);//=180/(pCountRings+2). +2 ��� �������
	//db � ��� ��������� ������������ ���� b 
	//(���� ����� ��������� ������ ����������� � ��������� xz)
	double db = 2.0 * M_PI / pCountSects;//=360/pCountSects
	//af � bf � �������� �������� �����
	double af = M_PI - da / 2.0;//=180-���� ����� �������� ��������� ��������� � ���� �
	double bf = 2.0 * M_PI - db / 2.0;//=360-���� � ������� ��������� ��������� ������ ��� �
	
	//������ ������ (��������� ������)
	v = (VERTEX *) malloc(sizeof(VERTEX) * CountVertex);

	//�������� �����
	v[0].v = Point3D (0.0, pRadius, 0.0);
	v[0].n = Point3D (0.0, 1.0, 0.0);
	v[0].c = pColor;
	//����� �����
	v[CountVertex - 1].v = Point3D (0.0, -pRadius, 0.0);
	v[CountVertex - 1].n = Point3D (0.0, -1.0, 0.0);
	v[CountVertex - 1].c = pColor;


	//������ �������, ��������� �� �������� �������
	UINT n = 1;

	//���� �� �������
	//����� a - ���� ����� ����� � ������� � ���� �
	for ( double a = da; a < af; a += da)
	{
		//���������� � ��������� ��� ����� ������
		double � = pRadius * cos(a);//���������� �� ������ �� ���������� �
		//��������������� �����
		double xz = pRadius * sin(a);//������� ������

		//���� �� ������� (�������)(�������� ����� �� ������)
		//����� b - ���� ����� ����� � ������� � ���� x
		for ( double b = 0.0; b < bf; n++, b += db)
		{
			//���������� �������� � �������������� ��������� (xz)
			double x = xz * sin(b);
			double z = xz * cos(b);
			//�������, ������� � ����
			v[n].v = Point3D (x, �, z);
			v[n].n = Point3D (x / pRadius, � / pRadius, z / pRadius);
			v[n].c = pColor;

		}
	}

	/*
	char buf[1024];
	for(UINT i = 0; i < CountVertex; i++) {
		sprintf(buf, "T%d: %f %f %f\n", i, v[i].v.x, v[i].v.y, v[i].v.z);
		OutputDebugString(buf);
	}
	*/
	
	//������ ������������� (�������� ������ ������������� � ������� ������)
	t = (TRIANGLE *) malloc(sizeof(TRIANGLE) * CountTrian);

	//������������ ������ �������
	for (n = 0; n < pCountSects; n++)
	{
		//�������� �����
		t[n].i1 = 0;
		//��������� ������ ������� ������� �������
		t[n].i2 = n + 1;
		//���������� �������
		t[n].i3 = (n == pCountSects - 1) ? 1 : n + 2;

		//����� �����
		t [CountTrian - pCountSects + n].i1 = CountVertex - 1;
		//��������� ������ ������� ������� �������
		t [CountTrian - pCountSects + n].i2 = CountVertex - 2 - n;
		//���������� �������
		t [CountTrian - pCountSects + n].i3 = (n == pCountSects - 1) ? CountVertex - 2 : CountVertex - 3 - n; 
	}

	
	//====== ������������ ��������� �����

	//������ ������
	int k = 1;

	//====== gnSects - ����� ���������� ������������

	//������ ������� ������
	n = pCountSects;//������ ������ (� ������) ��� ���������, �������� �� �������

	//������������ ��������� �����
	for (UINT i = 0; i < pCountRings; i++, k += pCountSects) {
		//i - ����� ������, k - ����� ������� (������ �� ���� �����)
		for (UINT j = 0; j < pCountSects; j++, n += 2) {
			//j - ����� �������� �� ������
			
			//������ ����� �������
			t[n].i1 = k + j;
			//������ ��������� ������ ������� ������� �������
			t[n].i2 = k + pCountSects + j;
			//���������� �������
			t[n].i3 = k + pCountSects + ((j + 1) % pCountSects);

			//�o �� ��� ������� ������������
			t[n + 1].i1 = t[n].i1;
			t[n + 1].i2 = t[n].i3;
			t[n + 1].i3 = k + ((j + 1) % pCountSects);
			

		}
	}
	
	//������ �������� �� ���� �������� ����� � List

	//--------------------------------------------------------------------------
	glNewList(pListIndex, GL_COMPILE);
	{
		glEnableClientState(GL_VERTEX_ARRAY);//����� �������� �������� �������
		glEnableClientState(GL_NORMAL_ARRAY);//����� �������� �������� �������
		glEnableClientState(GL_COLOR_ARRAY);////����� �������� �������� ����

		//������� ������ ������
		glVertexPointer(3, GL_DOUBLE, sizeof(VERTEX), &v->v);
		glNormalPointer(GL_DOUBLE, sizeof(VERTEX), &v->n);
		glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(VERTEX), &v->c);
	
		//�������� �� �������� �� t
		glDrawElements(GL_TRIANGLES, CountTrian * 3, GL_UNSIGNED_INT, t);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
	}
	glEndList();
	//--------------------------------------------------------------------------
}
