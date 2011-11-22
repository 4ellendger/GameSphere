#pragma once

#include <windows.h>
#include <gl/gl.h>
#include "GameObject.h"

class GameLogic
{
public:
	//������� ������� (�����)
	GameObject * go;
	//���-�� ������� ��������
	UINT CountGO;
	//������������� �����
	Sphere world;

	GameLogic(void);
	~GameLogic(void);
	
	//���������� �����
	void GameLogic::DrawScene();
	//������������ ��� ������� (��� �������� �����)
	void GameLogic::onUpdate();

private:
	//������� ������ � �������� pIndex
	void doGreateObject(UINT pIndex);
	/*������� ������ �������� ����� �������� ��������
	����������:
	pPoint - ����� ������������
	pTimeCol - ����� ������������
	pIndex1 - ������ ������� ����
	pIndex2 - ������ ������� ����
	*/
	UINT GameLogic::FindSphereCollusion (Point3D& pPoint, double &pTimeCol, UINT &pIndex1, UINT &pIndex2);
	/* ���������� �������� ����� ������������
	���������:
	pIndex - ������ �������, �������� �������� ��������
	pNormal - ������� � ����� ����������� �� ������� ��������� ������
	*/
	void GameLogic::doCalculateSpeed(UINT &pIndex, Vector3D pNormal);

	inline void GameLogic::CalculatePhysics();
};

