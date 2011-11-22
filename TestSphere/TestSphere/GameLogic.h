#pragma once

#include <windows.h>
#include <gl/gl.h>
#include "GameObject.h"

class GameLogic
{
public:
	//игровые объекты (сферы)
	GameObject * go;
	//кол-во игровых объектов
	UINT CountGO;
	//всеобъемлющая сфера
	Sphere world;

	GameLogic(void);
	~GameLogic(void);
	
	//Нарисовать сцену
	void GameLogic::DrawScene();
	//Перерисовать все объекты (все движение здесь)
	void GameLogic::onUpdate();

private:
	//создать объект с индексом pIndex
	void doGreateObject(UINT pIndex);
	/*находит первую коллизию между игровыми обектами
	возвращает:
	pPoint - точку столкновения
	pTimeCol - время столкновения
	pIndex1 - индекс первого шара
	pIndex2 - индекс второго шара
	*/
	UINT GameLogic::FindSphereCollusion (Point3D& pPoint, double &pTimeCol, UINT &pIndex1, UINT &pIndex2);
	/* Вычисление скорости после столкновения
	принимает:
	pIndex - индекс объекта, скорость которого изменяем
	pNormal - нормаль к точке поверхности от которой произошел отскок
	*/
	void GameLogic::doCalculateSpeed(UINT &pIndex, Vector3D pNormal);

	inline void GameLogic::CalculatePhysics();
};

