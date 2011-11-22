#include "GameObject.h"

#include <windows.h>
#include <gl/gl.h>

GameObject::GameObject(void)
{
}

/* 0-объект, 1-индекс листа отрисовки, 2-радиус, 3-координаты центра */
GameObject::GameObject(Sphere & pObj, UINT pIndex, double pRadius, Point3D pCenter)
{
	* obj = pObj;
	Index = pIndex;
	Radius = pRadius;
	Center = pCenter;
	//скорость по нулям
	Speed = Vector3D();
	//нарисуем
	doRedraw();
}

GameObject::~GameObject(void)
{
}

/* Осуществим движение объекта по старым координатам и скорости 
*/
void GameObject::doMove()
{
	//если матрица в начале координат
	glPushMatrix();
	
	Center += Speed;

	glTranslated(Center.x, Center.y, Center.z);

	glCallList(Index);
	
	glPopMatrix();
}

/* Нарисовать объект
*/
void GameObject::doRedraw()
{
	glPushMatrix();
	{
		glTranslated(Center.x, Center.y, Center.z);
		glCallList(Index);
	}
	glPopMatrix();
}