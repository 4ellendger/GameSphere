#include "GameLogic.h"

#include <iostream>
#include <sstream>
using std::stringstream;
#include "Sphere.h"
#include "Ray.h"
#include "PreSet.h"
#include <math.h>


//���������� ������
GameObject * go;

GameLogic::GameLogic(void)
{
}


GameLogic::~GameLogic(void)
{
}


/* ���������� ����� (���������� ��� ���)
*/
void GameLogic::DrawScene()
{
	//�������� ������������� �����
	world = Sphere(WORLD_RADIUS, WORLD_RINGS_AND_HALF_SECTORS, WORLD_RINGS_AND_HALF_SECTORS * 2, WORLD_COLOR, 1);

	//����� ����� 4 �������
	CountGO = GAME_OBJECT_COUNT;
	//������ ������ ��� ������� �������
	go = new GameObject[CountGO];

	//������ ������ ����
	//if (glIsList(Lists[0])) glDeleteLists (Lists[0], 1); 

	//�������� ��� �������
	for (UINT i = 0; i < CountGO; i++)
	{
		doGreateObject(i);
		//���������� �� ������� �� ���� x � y
		if (i % 2 == 0)//������
		{
			go[i].Center.x = 0;
			if (i % 4 == 0) go[i].Center.y = i * -GAME_OBJECT_RADIUS;
			else go[i].Center.y = i * GAME_OBJECT_RADIUS;
		}
		else//��������
		{
			go[i].Center.y = 0;
			if ((i-1) % 4 == 0) go[i].Center.x = (i + 1) * -GAME_OBJECT_RADIUS;
			else go[i].Center.x = i * GAME_OBJECT_RADIUS;
		}
	}

}

/* �������������� ��� ������� ��������� �� ��������, ������������ � ����
*/
void GameLogic::onUpdate()
{
	//��������� �������� � ���������� ���� �������� � ����� ����� � ������ ���� ������������
	CalculatePhysics();
	//static bool end = true; if (end){CalculatePhysics();end = false;}

	//�������� ��� �������
	for (UINT i = 0; i < CountGO; i++)//��� ������� ����
	{
		go[i].doRedraw();
	}

	//�������� ������������� �����
	glCallList(1);//����� ���� - (0, 0, 0)
}

/* ������� ������� ������
	pIndex - ������ ������� � ������ ��������
*/
void GameLogic::doGreateObject(UINT pIndex)
{	
	double sx = 1.0f, sy = 1.0f, sz = 1.0f;
	double x = 0, y = 0, z = 0;
	GLuint color;
	Sphere sph;//����� ��������� �������� �� �������� �������

	char buf[100];//��� ����

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

	//�������� ����� ������� (pIndex + 2 ����� ��� ������ ����� ������ ���� > 0 � ������ ���� ������ ������������� �����)
	sph = Sphere( GAME_OBJECT_RADIUS, GAME_OBJECT_RINGS, GAME_OBJECT_SECTORS, color, pIndex + 2 );
	//�������� ������ � ��������� ��� �����
	go[pIndex] = GameObject( sph, pIndex + 2, GAME_OBJECT_RADIUS, Point3D(x, y, z) );
	sph.~Sphere();
	//�������� �������� �������
	go[pIndex].Speed = Vector3D(sx, sy, sz);

	//������� � ��� ����� � ���������� �������
	sprintf(buf, "go[%d]: sx=%f sy=%f sz=%f x=%f y=%f z=%f\n", pIndex, 
		go[pIndex].Speed.x, go[pIndex].Speed.y, go[pIndex].Speed.z, go[pIndex].Center.x, go[pIndex].Center.y, go[pIndex].Center.z);
	OutputDebugString(buf);
}


/*������� ������ �������� ����� �������� �������� 
���������� (� ��������� �� TIMESTEP / COLLISION_TIMESTEP_DIVIDER):
pPoint - ����� ������������
pTimeCol - ����� ������������
pIndex1 - ������ ������� ����
pIndex2 - ������ ������� ����
*/
//UINT GameLogic::FindSphereCollusion (Point3D& pPoint, double &pTimeCol, UINT &pIndex1, UINT &pIndex2)
inline void GameLogic::CalculatePhysics()
{
	Point3D NewCoor;//����� ����������
	double CurrentTime;//������� ����� (�� 0 �� 1)
	const double DeltaTime = 1.0 / COLLISION_TIMESTEP_DIVIDER;//������� ���
	Point3D NewCoor2;//����� ���������� ������� ����

	Ray rays;
	Vector3D Axis;

	//�������� ���� ���������� � ��������� ������������
	for (CurrentTime = 0.0; CurrentTime < 1.0; CurrentTime += DeltaTime)//������ ���:
	{
		for (UINT i = 0; i < CountGO; i++)//��� ���� �����:
		{
			//���������� ����� ���������� ������� � ���� ����
			NewCoor = go[i].Center + go[i].Speed * DeltaTime;
			//�������� - �� ������� �� �� �� ������� ���� 
			if ( NewCoor.Magnitude(NewCoor) >= WORLD_RADIUS - GAME_OBJECT_RADIUS )
			{
				//�������� �� �������. ���������� ����� �������� � ��������� ����
				//������� ����� ����� = CurrentTime - DeltaTime
				//����� ����� ������� ��� ��� ������� �� ��� �� �����
				//(���� COLLISION_TIMESTEP_DIVIDER ���������� ������� �� ������������ �� ������� ������������ ����)

				//������� - ��� ������ ��������� ����� � ������������ ������������� ��������� ����� ����� (� ������ ������)
				Vector3D normal = Vector3D(go[i].Center).unit();
				//���������� �������� ����� ������������
				doCalculateSpeed(i, normal);//�������� go[i].Speed

				continue;//������ ������������ ���������� �� ��������� ����
			}

			
			//�������� ������������ ����� ���� � ������
			for (UINT j = i + 1; j < CountGO; j++)//�� ����� ����������� ������:
			{
				//����� ���������� ������������ ����
				NewCoor2 = go[j].Center + go[j].Speed * DeltaTime;
				//������ ����� �������� �����
				Axis = NewCoor - NewCoor2;

				//���� ���������� ����� �������� ������ ��� 2 �������
				if ( Axis.Magnitude(Axis) <= 2 * GAME_OBJECT_RADIUS )
				{
					//��� �����������
					doCalculateSpeed(i, Axis.unit());
					doCalculateSpeed(j, -Axis);

					Axis = NewCoor2 + go[j].Speed * DeltaTime - (NewCoor + go[i].Speed * DeltaTime);

					if( Axis.Magnitude(Axis) <= 2 * GAME_OBJECT_RADIUS)
					{
						//����� ������� � ����� ���������� ����� ������ ��� ����� ����� ��������
						doCalculateSpeed(i, Axis.unit());
						doCalculateSpeed(j, -Axis);
					}

//					continue;
				}
			}

			//���� �� � ���� �� �������� �� ���� ����
			go[i].Center = NewCoor;
		}
	}
}

/* ���������� �������� ����� ������������
���������
pIndex - ������ �������, �������� �������� ��������
pNormal - ������� � ����� ����������� �� ������� ��������� ������ (� ������� ���������!)
*/
void GameLogic::doCalculateSpeed(UINT &pIndex, Vector3D pNormal)
{
	//������� ����� ������� ��������
	double speed = go[pIndex].Speed.Magnitude(go[pIndex].Speed);
	//����������� ������ ��������
	go[pIndex].Speed.unit();
	//������������ ����� �����������
	go[pIndex].Speed = pNormal * (pNormal * 2 * (-go[pIndex].Speed)) + go[pIndex].Speed;
	go[pIndex].Speed.unit();
	//������� ������ ����������� �� �����
	go[pIndex].Speed *= speed;
}