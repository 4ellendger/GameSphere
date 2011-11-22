//���������� � �����
#include <windows.h>
//OpenGL
#pragma comment(lib, "OpenGL32.lib")
#include <gl/gl.h>
//��� ������� (timeGetTime)
#pragma comment(lib, "winmm.lib")
#include <mmsystem.h>
//��� ������ ���������� ����������
#include <iostream>
#include <sstream>
using std::stringstream;

//�������� ������������� �������
#pragma comment(lib, "WSock32.lib") // ���������� ��� ����������
#include <WinSock.h>

//���� ������
#include "GLContext.h"
#include "Sphere.h"
#include "GameObject.h"
#include "GameLogic.h"

typedef struct _MOUSEMSG
{
  SHORT  x; // ���������� X
  SHORT  y; // ���������� Y
  USHORT codeHitTest; // ���������� ������������
  USHORT fsInp;       // �����
} MSEMSG;
typedef MSEMSG *PMSEMSG;

#define MOUSEMSG(pmsg) \
   ((PMSEMSG)((PBYTE)pmsg + sizeof(WPARAM)))

//������� main.cpp-----------------------------------------------------------

//������ opengl
GLContext opengl;
//������� �����
Point3D SceneAngle;
//����������� �����
Point3D SceneMove;
//���������� ����� (��� ���������� �� ����� ��������)
Point3D SceneSave;
//���������� ����
float MouseX;
float MouseY;
//������ ������������ ������ OpenGL
UINT * Lists;
//������ ������ ������
GameLogic LogicMain;

//������ �� ������� ������ ������
UINT Player = UINT_MAX;
//���� � ���, ��� ������ ���
BOOL isLeftButtonDown;
//���� � ���, ��� ������ ���
BOOL isRightButtonDown;

//GDI ��������
HDC hDC = NULL;
//�������� ����������
HGLRC hRC = NULL;
//�������
HINSTANCE Instance = NULL;
//���������� �����
HWND hWnd = NULL;
//���������� ���������� ���������� ������
DEVMODE DMsaved;

//������� �����
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
//������� ���� OpenGL
BOOL GreateOpenGLWindow(char* title, int width, int height, BOOL fullscreenflag);
//�������� ������� ����
void doResize(int pWidth, int pHeight);

bool doConnect(char * str);

//��������� ������� ����
void doHandleLeftClick(LPARAM lParam);
void doHandleRightClick(LPARAM lParam);
void doHandleMouseMove(float px, float py);

//��������� ������� ����������
void CALLBACK doUp();
void CALLBACK doDown();
void CALLBACK doLeft();
void CALLBACK doRight();
void CALLBACK doLookUp();
void CALLBACK doLookDown();
void CALLBACK doLookLeft();
void CALLBACK doLookRight();
