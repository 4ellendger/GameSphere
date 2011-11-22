#include "main.h"

/* Main вызываемый виндой */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
				   LPSTR lpCmdLine, int iCmdShow)
{
	MSG msg;
	BOOL quit = false;

	DWORD StartTime;
	DWORD Time = 0;
	UINT FrameCounter = 0;//счетчик кадров всей жизни проги

	Instance = hInstance;

	//создадим окно OpenGL
	if (!GreateOpenGLWindow("GameSphere", 700, 700, false))
	{
		return 0;
	}
		
	LogicMain = GameLogic();
	LogicMain.DrawScene();//создадим сцену
	
	//настроим таймер задающий частоту кадров
	timeBeginPeriod(1);//зададим четкость в 1 мс
	StartTime = timeGetTime();

	//фоновый цикл
	while ( !quit )
	{
		
		//проверка сообщений (винды)
		if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE )  )
		{
			
			//если нажат "ESC"
			if ( msg.message == WM_QUIT ) 
			{
				quit = TRUE;
			} 
			else 
			{
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
			
		} 
		else //иначе рисуем
		{
			Time = timeGetTime();
			if ( (Time - StartTime) / TIMESTEP >= FrameCounter){

				//рисуем

				//----------------------------------------------------------------------
				//почистим буферы цвета и глубины
				glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
			
				//сохраним
				glPushMatrix();
				{
					//повернем и сдвинем координаты для правильного взгляда камеры
					if (Player != UINT_MAX){
						//если камера привязана к какому-то объекту
						glTranslated(LogicMain.go[Player].Center.x, LogicMain.go[Player].Center.y, LogicMain.go[Player].Center.z);
					} else {
						glTranslated(SceneMove.x, SceneMove.y, SceneMove.z);
					}
					glRotated( SceneAngle.x, 1.0, 0.0, 0.0 );
					glRotated( SceneAngle.y, 0.0, 1.0, 0.0 );
					glRotated( SceneAngle.z, 0.0, 0.0, 1.0 );
					
					//перерисуем все объекты
					LogicMain.onUpdate();

				}
				//восстановим
				glPopMatrix();

				//переключим буферы
				SwapBuffers( hDC );
				//----------------------------------------------------------------------

				//Увеличим счетчик кадров
				if (FrameCounter == UINT_MAX){
					//если переполнился то сбрасываем в ноль, Time при этом обновляется
					//это случается каждые 2,7 года (при TIMESTEP = 20)
					Time = timeGetTime();
					FrameCounter = 0;
				} else {
					FrameCounter++;
				}

			}//if ( (Time - StartTime) / TIMESTEP >= FrameCounter){

			//здесь можно сделать что-нить не синронизированное с частотой кадров

		}//если не проверка сообщения
		
	}

	timeEndPeriod(1);//отключим четкость
	
	//отключаем OpenGL
	//DisableOpenGL( hWnd, hDC, hRC );
	opengl.~GLContext();

	//убиваем окно
	DestroyWindow( hWnd );
	
	return msg.wParam;
	
}

/* все события винды */
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	char buf[100];
	
	switch (message)
	{
		
	case WM_CREATE://создание
		return 0;
		
	case WM_CLOSE://закрытие
		PostQuitMessage( 0 );
		return 0;
		
	case WM_DESTROY://закрытие
		return 0;

	case WM_SIZE://изменение размера
		doResize(LOWORD(lParam),HIWORD(lParam));
		return 0;

	case WM_LBUTTONDOWN://ЛКМ нажали
		MouseX = HIWORD(lParam);
		MouseY = LOWORD(lParam);
		SceneSave = SceneMove;
		isLeftButtonDown = true;
		return 0;
		
	case WM_LBUTTONUP://ЛКМ отпустили
		isLeftButtonDown = false;
		sprintf(buf, "SceneAngle.y=%f SceneAngle.x=%f\n", SceneAngle.y, SceneAngle.x);
		OutputDebugString(buf);

		return 0;
		
	case WM_RBUTTONDOWN://ПКМ нажали
		MouseX = HIWORD(lParam);
		MouseY = LOWORD(lParam);
		SceneSave = SceneMove;
		isRightButtonDown = true;
		return 0;
		
	case WM_RBUTTONUP://ПКМ отпустили
		isRightButtonDown = false;
		sprintf(buf, "SceneMove.z=%f\n", SceneMove.z);
		OutputDebugString(buf);

		return 0;

	case WM_MOUSEMOVE://движение мыши
		doHandleMouseMove(HIWORD(lParam), LOWORD(lParam));
		return 0;

	case WM_KEYDOWN://нажата кнопа на клаве
		switch ( wParam )
		{
			
		case VK_ESCAPE:
			//выход из приложения
			PostQuitMessage(0);
			break;

		case VK_INSERT:
			//пробуем сконнектится с сервером
			char serv[1024];
			char buf[100];
			if (doConnect(serv)) {
				sprintf(buf, "Connect to server is successful!\n");
				OutputDebugString(buf);
			} else {
				sprintf(buf, "Connect to server is faild!/n");
				OutputDebugString(buf);
			}
			OutputDebugString(serv);
			sprintf(buf, "\n");
			OutputDebugString(buf);
			break;

		/*
		case VK_F1:
			//попробуем сделать fullscreen
			opengl.~GLContext();
			if (!GreateOpenGLWindow("GameSphere", 700, 700, true))
			{
				return 0;
			}
			break;
		*/
			
		case VK_SPACE:
			//переключим камеру следить за следующим объектом
			if (Player < LogicMain.CountGO - 1) 
				Player++;
			else 
				if (Player != LogicMain.CountGO - 1)
					Player = 0;
				else {
					SceneMove = SceneSave;
					Player = UINT_MAX;
				}
			break;

		//курсорные
		case VK_UP:
			doUp();
			break;
		case VK_DOWN:
			doDown();
			break;
		case VK_LEFT:
			doLeft();
			break;
		case VK_RIGHT:
			doRight();
			break;

		//цифровой клавиатуры
		case VK_NUMPAD8:
			doLookUp();
			break;
		case VK_NUMPAD5:
			doLookDown();
			break;
		case VK_NUMPAD4:
			doLookLeft();
			break;
		case VK_NUMPAD6:
			doLookRight();
			break;
		
		//дополнительные
		case VK_HOME:
			SceneMove.z += 0.5f;
			break;
		case VK_END:
			SceneMove.z -= 0.5f;
			break;

		}
		return 0;
	
	default://иначе послать обратно
		return DefWindowProc( hWnd, message, wParam, lParam );
			
	}
	
}

BOOL GreateOpenGLWindow(char* title, int width, int height, BOOL fullscreenflag)
{
	WNDCLASS wc;
	DWORD dwExStyle;
	DWORD dwStyle;
	//размеры окна
	RECT WindowRect;
	WindowRect.left = 200;
	WindowRect.top = 200;
	WindowRect.right= width;
	WindowRect.bottom = height;

	
	//опишем окно
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = Instance;
	wc.hIcon = LoadIcon( NULL, IDI_APPLICATION );
	wc.hCursor = LoadCursor( NULL, IDC_ARROW );
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "GameSphere";
	
	
	EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &DMsaved); // save the current display state (NEW)

	if (fullscreenflag)
	{
		DEVMODE dmScreenSettings;
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = width;
		dmScreenSettings.dmPelsHeight = height;
		dmScreenSettings.dmFields = DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		//пробуем установить полноэкранный режим
		if ( ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL )
		{
			//не получилось
			fullscreenflag = false;
		}
	}
	
	//зарегистрируем класс нашего окна
	RegisterClass( &wc );
	
	if (fullscreenflag)
	{
		WindowRect.left = 0;
		WindowRect.top = 0;
		dwExStyle = WS_EX_APPWINDOW;
		dwStyle = WS_POPUP;
		ShowCursor(false);
	}
	else
	{
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		dwStyle = WS_OVERLAPPEDWINDOW;
	}
	
	//придадим окну требуемый размер
	AdjustWindowRectEx(&WindowRect, dwStyle, false, dwExStyle);
	
	//создадим окно
	if (!(hWnd = CreateWindow( 
		//dwExStyle,
		"GameSphere", 
		title,
		dwStyle | WS_CAPTION | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		200, 200, 
		width, height,
		NULL,
		NULL,
		Instance,
		NULL )))
	{
		opengl.~GLContext();
		return false;
	}

	//инициализируем OpenGL
	opengl = GLContext();
	opengl.mhWnd = hWnd;
	opengl.init(hWnd);

	
	//получим контексты
	hDC = opengl.mhDC;
	hRC = opengl.mhRC;
	
	//покажем
	ShowWindow(hWnd, SW_SHOW);
	//высший приоритет
	SetForegroundWindow(hWnd);
	//фокус клавиатуры
	SetFocus(hWnd);
	//установим перспективу
	doResize(width, height);
	
	//установим камеру
	SceneAngle = Point3D(0.0, 0.0, 0.0);
	SceneMove = Point3D(0.0, 0.0, 0.0);


	//сместим сцену от камеры
	SceneSave = Point3D(0, 0, -CAMERA_Z);
	SceneMove = SceneSave;

	return true;
}

/* при изменении размеров окна */
void doResize(int pWidth, int pHeight)
{
	glViewport(0, 0, pWidth, pHeight);
	//перспектива
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//отношение сторон окна
	GLfloat aspectRatio = (GLfloat)pWidth / pHeight;
	//установить матрицу перспективы (градус, отношение, ближняя и дальняя границы)
	opengl.gluPerspective(CAMERA_PERSPECTIVE_ANGLE, aspectRatio, CAMERA_PERSPECTIVE_NEAR, CAMERA_PERSPECTIVE_FAR);
}

//присоединиться к серверу
bool doConnect(char * str)
{
	WSADATA wsa_data; // Информация о сокетах
	SOCKET s = NULL; // Серверный сокет

	// 1. Инициализируем сокеты (требуем версию 1.1 как минимум)
	if (WSAStartup(0x101,&wsa_data) != 0) {
		sprintf(str, "WSAStartup is faild");
		return false;
	}

	// 2. Открываем серверный сокет
	s = socket(AF_INET, SOCK_STREAM, 0);
	if (s == INVALID_SOCKET) {
		sprintf(str, "INVALID_SOCKET");
		return false;
	}

	// 3. Привязываем сокет к адресу
	sockaddr_in addr; // Для хранения адреса
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(SERVER_PORT); // Наш порт, который будем открывать для коннектов
	if (bind(s, (LPSOCKADDR)&addr, sizeof(addr)) != 0) {
		sprintf(str, "bind is faild");
		return false;
	}

	// 4. Соединяемся
	if (connect(s, (LPSOCKADDR)&addr, sizeof(addr)) == SOCKET_ERROR) {
		sprintf(str, "connect is faild");
		return false;
	}

	// 5. Обмениваемся данными
	// (получаем приветствие)
	char buf[1024];
	int len = recv(s, buf, 1024, 0);
	if (len == SOCKET_ERROR) {
		sprintf(str, "SOCKET_ERROR");
		return false;
	}
	str = buf;

	//buf[len] = 0;

	// Отсылаем ответ серверу
	char *reply = "Fuck you too, buddy!";
	send(s, reply, (int)strlen(reply), 0);

	// 6. Закрываем сокет
	closesocket(s);

	// 7. Завершаем работу с сокетами
	WSACleanup();
	return true;
}

//------------------ Обработка нажатий кнопок мыши -------------------------------
//Движение при зажатой ЛКМ
void doHandleMouseMove(float px, float py)
{
	if (isLeftButtonDown)
	{
		SceneAngle.y = py - MouseY;
		SceneAngle.x = px - MouseX;
	} 
	else if (isRightButtonDown)
	{
		SceneMove.z = SceneSave.z + (px - MouseX) * 0.1f;
		if (SceneMove.z > 0) SceneMove.z = 0;
	}
}

//------------------ Обработка нажатий клавиш -------------------------------
void CALLBACK doUp()
{
	SceneMove.y += 0.1f;
}

void CALLBACK doDown()
{
	SceneMove.y -= 0.1f;
}

void CALLBACK doLeft()
{
	SceneMove.x -= 0.1f;
}

void CALLBACK doRight()
{
	SceneMove.x += 0.1f;
}

void CALLBACK doLookUp()
{
	SceneAngle.x -= 1.0f;
}

void CALLBACK doLookDown()
{
	SceneAngle.x += 1.0f;
}

void CALLBACK doLookLeft()
{
	SceneAngle.y -= 1.0f;
}

void CALLBACK doLookRight()
{
	SceneAngle.y += 1.0f;
}
//---------------------------------------------------------------------------