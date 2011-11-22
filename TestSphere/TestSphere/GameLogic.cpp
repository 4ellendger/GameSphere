#include "GameLogic.h"

#include <iostream>
#include <sstream>
using std::stringstream;
#include "Sphere.h"
#include "Ray.h"
#include "PreSet.h"
#include <math.h>


//переменные класса
GameObject * go;

GameLogic::GameLogic(void)
{
}


GameLogic::~GameLogic(void)
{
}


/* Нарисовать сцену (вызывается одн раз)
*/
void GameLogic::DrawScene()
{
	//создадим всеобъемлющую сферу
	world = Sphere(WORLD_RADIUS, WORLD_RINGS_AND_HALF_SECTORS, WORLD_RINGS_AND_HALF_SECTORS * 2, WORLD_COLOR, 1);

	//пусть будет 4 объекта
	CountGO = GAME_OBJECT_COUNT;
	//забъем память под игровые объекты
	go = new GameObject[CountGO];

	//удалим старый лист
	//if (glIsList(Lists[0])) glDeleteLists (Lists[0], 1); 

	//создадим все объекты
	for (UINT i = 0; i < CountGO; i++)
	{
		doGreateObject(i);
		//расположим их крестом на осях x и y
		if (i % 2 == 0)//четный
		{
			go[i].Center.x = 0;
			if (i % 4 == 0) go[i].Center.y = i * -GAME_OBJECT_RADIUS;
			else go[i].Center.y = i * GAME_OBJECT_RADIUS;
		}
		else//нечетный
		{
			go[i].Center.y = 0;
			if ((i-1) % 4 == 0) go[i].Center.x = (i + 1) * -GAME_OBJECT_RADIUS;
			else go[i].Center.x = i * GAME_OBJECT_RADIUS;
		}
	}

}

/* Перерисовывает все объекты обдумывая их движения, столкновения и проч
*/
void GameLogic::onUpdate()
{
	//вычислить скорости и координаты всех объектов в конце кадра с учетом всех столкновений
	CalculatePhysics();
	//static bool end = true; if (end){CalculatePhysics();end = false;}

	//нарисуем все объекты
	for (UINT i = 0; i < CountGO; i++)//для каждого шара
	{
		go[i].doRedraw();
	}

	//нарисуем всеобъемлющую сферу
	glCallList(1);//центр мира - (0, 0, 0)
}

/* Создает игровой объект
	pIndex - индекс объекта в списке объектов
*/
void GameLogic::doGreateObject(UINT pIndex)
{	
	double sx = 1.0f, sy = 1.0f, sz = 1.0f;
	double x = 0, y = 0, z = 0;
	GLuint color;
	Sphere sph;//сфера создается отдельно от создания объекта

	char buf[100];//для лога

	//
	sx = 0.2 * (float)rand() / (float)RAND_MAX;
	sy = 0.2 * (float)rand() / (float)RAND_MAX;
	sz = 0.2 * (float)rand() / (float)RAND_MAX;
	/*
	x = 10.0 * ((float)rand() / (float)RAND_MAX);
	y = 10.0 * ((float)rand() / (float)RAND_MAX);
	z = 10.0 * ((float)rand() / (float)RAND_MAX);
	*/
	color = 0xFF000000 | (rand() / 0xFF)<<16 |(rand() / 0xFF)<<8 | (rand() / 0xFF);

	//создадим сферу объекта (pIndex + 2 потом что индекс листа должен быть > 0 и первый лист всегда всеобъемлющая сфера)
	sph = Sphere( GAME_OBJECT_RADIUS, GAME_OBJECT_RINGS, GAME_OBJECT_SECTORS, color, pIndex + 2 );
	//создадим объект и передадим ему сферу
	go[pIndex] = GameObject( sph, pIndex + 2, GAME_OBJECT_RADIUS, Point3D(x, y, z) );
	sph.~Sphere();
	//настроим скорость объекта
	go[pIndex].Speed = Vector3D(sx, sy, sz);

	//выведем в лог номер и координаты объекта
	sprintf(buf, "go[%d]: sx=%f sy=%f sz=%f x=%f y=%f z=%f\n", pIndex, 
		go[pIndex].Speed.x, go[pIndex].Speed.y, go[pIndex].Speed.z, go[pIndex].Center.x, go[pIndex].Center.y, go[pIndex].Center.z);
	OutputDebugString(buf);
}


/*находит первую коллизию между игровыми обектами 
возвращает (с точностью до TIMESTEP / COLLISION_TIMESTEP_DIVIDER):
pPoint - точку столкновения
pTimeCol - время столкновения
pIndex1 - индекс первого шара
pIndex2 - индекс второго шара
*/
//UINT GameLogic::FindSphereCollusion (Point3D& pPoint, double &pTimeCol, UINT &pIndex1, UINT &pIndex2)
inline void GameLogic::CalculatePhysics()
{
	Point3D NewCoor;//новые координаты
	double CurrentTime;//текущее время (от 0 до 1)
	const double DeltaTime = 1.0 / COLLISION_TIMESTEP_DIVIDER;//текущий шаг
	Point3D NewCoor2;//новые координаты другого шара

	Ray rays;
	Vector3D Axis;

	//запустим цикл нахождения и обработок столкновений
	for (CurrentTime = 0.0; CurrentTime < 1.0; CurrentTime += DeltaTime)//каждый шаг:
	{
		for (UINT i = 0; i < CountGO; i++)//для всех шаров:
		{
			//рассчитаем новые координаты объекта в этом шаге
			NewCoor = go[i].Center + go[i].Speed * DeltaTime;
			//проверим - не вылетит ли он за пределы мира 
			if ( NewCoor.Magnitude(NewCoor) >= WORLD_RADIUS - GAME_OBJECT_RADIUS )
			{
				//вылетает за пределы. Рассчитаем новую скорость и положение шара
				//возъмем время удара = CurrentTime - DeltaTime
				//тогда будем считать что шар остался на том же месте
				//(если COLLISION_TIMESTEP_DIVIDER достаточно большой то пользователь не заметит пропущенного шага)

				//нормаль - это вектор единичной длины с направлением эквивалентным величинам точки удара (в данном случае)
				Vector3D normal = Vector3D(go[i].Center).unit();
				//рассчитаем скорость после столкновения
				doCalculateSpeed(i, normal);//изменяет go[i].Speed

				continue;//другие столкновения обработаем на следующем шаге
			}

			
			//проверим столкновения шаров друг с другом
			for (UINT j = i + 1; j < CountGO; j++)//со всеми оставшимися шарами:
			{
				//новые координаты проверяемого шара
				NewCoor2 = go[j].Center + go[j].Speed * DeltaTime;
				//вектор между центрами шаров
				Axis = NewCoor - NewCoor2;

				//если расстояние между центрами меньше чем 2 радиуса
				if ( Axis.Magnitude(Axis) <= 2 * GAME_OBJECT_RADIUS )
				{
					//они столкнулись
					doCalculateSpeed(i, Axis.unit());
					doCalculateSpeed(j, -Axis);

					Axis = NewCoor2 + go[j].Speed * DeltaTime - (NewCoor + go[i].Speed * DeltaTime);

					if( Axis.Magnitude(Axis) <= 2 * GAME_OBJECT_RADIUS)
					{
						//после разлета с этими скоростями между шарами все равно будет коллизия
						doCalculateSpeed(i, Axis.unit());
						doCalculateSpeed(j, -Axis);
					}

//					continue;
				}
			}

			//если ни в кого не врезался на этом шаге
			go[i].Center = NewCoor;
		}
	}
}

/* Вычисление скорости после столкновения
принимает
pIndex - индекс объекта, скорость которого изменяем
pNormal - нормаль к точке поверхности от которой произошел отскок (с текущей скоростью!)
*/
void GameLogic::doCalculateSpeed(UINT &pIndex, Vector3D pNormal)
{
	//находим длину вектора скорости
	double speed = go[pIndex].Speed.Magnitude(go[pIndex].Speed);
	//нормализуем вектор скорости
	go[pIndex].Speed.unit();
	//рассчитываем новое направление
	go[pIndex].Speed = pNormal * (pNormal * 2 * (-go[pIndex].Speed)) + go[pIndex].Speed;
	go[pIndex].Speed.unit();
	//умножим вектор направления на длину
	go[pIndex].Speed *= speed;
}