//Интеграция в винду
#include <windows.h>
//OpenGL
#pragma comment(lib, "OpenGL32.lib")
#include <gl/gl.h>
//для таймера (timeGetTime)
#pragma comment(lib, "winmm.lib")
#include <mmsystem.h>
//для вывода отладочной информации
#include <iostream>
#include <sstream>
using std::stringstream;

//включаем использование сокетов
#pragma comment(lib, "WSock32.lib") // Используем эту библиотеку
#include <WinSock.h>

//Свои классы
#include "GLContext.h"
#include "Sphere.h"
#include "GameObject.h"
#include "GameLogic.h"

typedef struct _MOUSEMSG
{
  SHORT  x; // коодрината X
  SHORT  y; // координата Y
  USHORT codeHitTest; // результаты тестирования
  USHORT fsInp;       // флаги
} MSEMSG;
typedef MSEMSG *PMSEMSG;

#define MOUSEMSG(pmsg) \
   ((PMSEMSG)((PBYTE)pmsg + sizeof(WPARAM)))

//объекты main.cpp-----------------------------------------------------------

//объект opengl
GLContext opengl;
//поворот сцены
Point3D SceneAngle;
//перемещение сцены
Point3D SceneMove;
//координаты сцены (для сохранения во время вращения)
Point3D SceneSave;
//координаты мыши
float MouseX;
float MouseY;
//список существующих листов OpenGL
UINT * Lists;
//Объект класса логики
GameLogic LogicMain;

//объект за которым следит камера
UINT Player = UINT_MAX;
//флаг о том, что нажата ЛКМ
BOOL isLeftButtonDown;
//флаг о том, что нажата ПКМ
BOOL isRightButtonDown;

//GDI контекст
HDC hDC = NULL;
//контекст рендеринга
HGLRC hRC = NULL;
//инстанс
HINSTANCE Instance = NULL;
//обработчик винды
HWND hWnd = NULL;
//сохранение предыдущих параметров экрана
DEVMODE DMsaved;

//События винды
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
//Создать окно OpenGL
BOOL GreateOpenGLWindow(char* title, int width, int height, BOOL fullscreenflag);
//Изменить размеры окна
void doResize(int pWidth, int pHeight);

bool doConnect(char * str);

//Обработка событий мыши
void doHandleLeftClick(LPARAM lParam);
void doHandleRightClick(LPARAM lParam);
void doHandleMouseMove(float px, float py);

//Обработка событий клавиатуры
void CALLBACK doUp();
void CALLBACK doDown();
void CALLBACK doLeft();
void CALLBACK doRight();
void CALLBACK doLookUp();
void CALLBACK doLookDown();
void CALLBACK doLookLeft();
void CALLBACK doLookRight();
