#include "GameObject.h"

#include <windows.h>
#include <gl/gl.h>

GameObject::GameObject(void)
{
}

/* 0-������, 1-������ ����� ���������, 2-������, 3-���������� ������ */
GameObject::GameObject(Sphere & pObj, UINT pIndex, double pRadius, Point3D pCenter)
{
	* obj = pObj;
	Index = pIndex;
	Radius = pRadius;
	Center = pCenter;
	//�������� �� �����
	Speed = Vector3D();
	//��������
	doRedraw();
}

GameObject::~GameObject(void)
{
}

/* ���������� �������� ������� �� ������ ����������� � �������� 
*/
void GameObject::doMove()
{
	//���� ������� � ������ ���������
	glPushMatrix();
	
	Center += Speed;

	glTranslated(Center.x, Center.y, Center.z);

	glCallList(Index);
	
	glPopMatrix();
}

/* ���������� ������
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