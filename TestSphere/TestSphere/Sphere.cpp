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

/*конструктор сферы 
(pCountRings+1)*pCountSects+2 не должно быть больше 4294967295
*/
Sphere::Sphere(double pRadius, UINT pCountRings, UINT pCountSects, GLuint pColor, UINT pListIndex)
{
	//общее кол-вo треугольников 
	UINT CountTrian = (pCountRings + 1) * pCountSects * 2;
	//общее кол-во вершин
	UINT CountVertex = (pCountRings + 1) * pCountSects + 2;
	//da — шаг изменения сферического угла а 
	//(угол между вершинами одного треуголника в плоскости проходящей по оси y)
	double da = M_PI / (pCountRings + 2.0);//=180/(pCountRings+2). +2 это вершины
	//db — шаг изменения сферического угла b 
	//(угол между вершинами одного треуголника в плоскости xz)
	double db = 2.0 * M_PI / pCountSects;//=360/pCountSects
	//af и bf — конечные значения углов
	double af = M_PI - da / 2.0;//=180-угол между нормалью требуемой плоскости и осью у
	double bf = 2.0 * M_PI - db / 2.0;//=360-угол к нормали требуемой плоскости вокруг оси у
	
	//массив вершин (координат вершин)
	v = (VERTEX *) malloc(sizeof(VERTEX) * CountVertex);

	//северный полюс
	v[0].v = Point3D (0.0, pRadius, 0.0);
	v[0].n = Point3D (0.0, 1.0, 0.0);
	v[0].c = pColor;
	//южный полюс
	v[CountVertex - 1].v = Point3D (0.0, -pRadius, 0.0);
	v[CountVertex - 1].n = Point3D (0.0, -1.0, 0.0);
	v[CountVertex - 1].c = pColor;


	//индекс вершины, следующей за северным полюсом
	UINT n = 1;

	//цикл по широтам
	//здесь a - угол между лучом к вершине и осью у
	for ( double a = da; a < af; a += da)
	{
		//координата у постоянна для всего кольца
		double у = pRadius * cos(a);//расстояние от центра до координаты у
		//вспомогательная точка
		double xz = pRadius * sin(a);//диаметр кольца

		//цикл по секциям (долгота)(набивает точки на кольце)
		//здесь b - угол между лучом к вершине и осью x
		for ( double b = 0.0; b < bf; n++, b += db)
		{
			//координаты проекции в экваториальной плоскости (xz)
			double x = xz * sin(b);
			double z = xz * cos(b);
			//вершина, нормаль и цвет
			v[n].v = Point3D (x, у, z);
			v[n].n = Point3D (x / pRadius, у / pRadius, z / pRadius);
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
	
	//массив треугольников (индексов вершин треугольников в массиве вершин)
	t = (TRIANGLE *) malloc(sizeof(TRIANGLE) * CountTrian);

	//треугольники вблизи полюсов
	for (n = 0; n < pCountSects; n++)
	{
		//северный полюс
		t[n].i1 = 0;
		//следующая против часовой стрелки вершина
		t[n].i2 = n + 1;
		//замыкающая вершина
		t[n].i3 = (n == pCountSects - 1) ? 1 : n + 2;

		//южный полюс
		t [CountTrian - pCountSects + n].i1 = CountVertex - 1;
		//следующая против часовой стрелки вершина
		t [CountTrian - pCountSects + n].i2 = CountVertex - 2 - n;
		//замыкающая вершина
		t [CountTrian - pCountSects + n].i3 = (n == pCountSects - 1) ? CountVertex - 2 : CountVertex - 3 - n; 
	}

	
	//====== Треугольники разбиения колец

	//первое кольцо
	int k = 1;

	//====== gnSects - номер следующего треугольника

	//индекс массива вершин
	n = pCountSects;//первое кольцо (у полюса) уже заполнено, начинаем со второго

	//треугольники разбиения колец
	for (UINT i = 0; i < pCountRings; i++, k += pCountSects) {
		//i - номер кольца, k - номер сектора (вообще на всей сфере)
		for (UINT j = 0; j < pCountSects; j++, n += 2) {
			//j - номер трапеции на кольце
			
			//индекс общей вершины
			t[n].i1 = k + j;
			//индекс следующей против часовой стрелки вершины
			t[n].i2 = k + pCountSects + j;
			//замыкающая вершина
			t[n].i3 = k + pCountSects + ((j + 1) % pCountSects);

			//тo же для второго треугольника
			t[n + 1].i1 = t[n].i1;
			t[n + 1].i2 = t[n].i3;
			t[n + 1].i3 = k + ((j + 1) % pCountSects);
			

		}
	}
	
	//теперь отрисуем из этих массивов сферу в List

	//--------------------------------------------------------------------------
	glNewList(pListIndex, GL_COMPILE);
	{
		glEnableClientState(GL_VERTEX_ARRAY);//чтобы задавать массивом вершины
		glEnableClientState(GL_NORMAL_ARRAY);//чтобы задавать массивом нормали
		glEnableClientState(GL_COLOR_ARRAY);////чтобы задавать массивом цвет

		//зададим массив вершин
		glVertexPointer(3, GL_DOUBLE, sizeof(VERTEX), &v->v);
		glNormalPointer(GL_DOUBLE, sizeof(VERTEX), &v->n);
		glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(VERTEX), &v->c);
	
		//отрисуем по индексам из t
		glDrawElements(GL_TRIANGLES, CountTrian * 3, GL_UNSIGNED_INT, t);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
	}
	glEndList();
	//--------------------------------------------------------------------------
}
