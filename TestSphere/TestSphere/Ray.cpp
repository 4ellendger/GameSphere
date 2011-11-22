#include "Ray.h"
#include <math.h>

//линия между двумя точками
Ray::Ray(const Point3D &pPoint1, const Point3D &pPoint2)
{
	_P = pPoint1;
	_V = pPoint2 - pPoint1;
}

//точка и направление
Ray::Ray(const Point3D &pPoint1, const Vector3D &pDir)
{
	_P = pPoint1;
	_V = pDir;
}

//ближайшие точки на линиях
void Ray::adjacentPoints(const Ray &ray, Vector3D &point1, Vector3D &point2) const 
{
	double temp = point1.Dot(_V, ray._V);//Vector3D::Dot(_V, ray._V);
	double temp2 = 1.0 - temp * temp;
	Vector3D tv;//временный вектор для промужуточных вычислений

	//проверка на параллельность
	if (fabs(temp2) < EPSILON_PARALLEL_RAYS) {
		double mu = point1.Dot(_V, _P-ray._P) / temp;//Vector3D::Dot(_V, _P-ray._P) / temp;
		point1 = _P;
		//Vector3D::add(ray._P, Vector3D::multiply(ray._V, mu, tv), point2);
		point2 = ray._P + ray._V * mu;
	} else {
		//double a = Vector3D::Dot(_V, Vector3D::subtract(ray._P, _P, tv));
		double a = point1.Dot(_V, ray._P - _P);
		//double b = Vector3D::dot(ray._V, Vector3D::subtract(_P, ray._P, tv));
		double b = point1.Dot(ray._V, _P - ray._P);
		double mu = (b + temp * a) / temp2;
		double lambda = (a + temp*b)/temp2;
		//Vector3D::add(_P, Vector3D::multiply(_V, lambda, tv), point1);
		point1 = _P + _V * lambda;
		//Vector3D::add(ray._P, Vector3D::multiply(ray._V, mu, tv), point2);
		point2 = ray._P + ray._V * mu;
	}
}

//расстояние между двумя лучами
double Ray::dist(const Ray &ray) const 
{
	Vector3D point1, point2;
	adjacentPoints(ray, point1, point2);
	//return point1.dist(point2);
	return point2.Magnitude(point1 - point2);
}

//расстояние между лучом и точкой
double Ray::dist(const Vector3D &point) const 
{
	Vector3D tv, point2;
	//double lambda = Vector3D::Dot(_V, point-_P);
	double lambda = point2.Dot(_V, point - _P);
	//Vector3D::add(_P, Vector3D::multiply(_V, lambda, tv), point2);
	point2 = _P + _V * lambda;
	//return point.dist(point2);
	return point2.Magnitude(point - point2);
}