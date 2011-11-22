#include "GLContext.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include "PreSet.h"

    HWND mhWnd;
    HDC mhDC;
    HGLRC mhRC;

	//public

    GLContext::GLContext()
    {
        GLContext::reset();
    }

    GLContext::~GLContext()
    {
        GLContext::purge();
    }

    void GLContext::init(HWND hWnd)
    {
		//массив для настроек
		GLfloat v[4]; 

        //обработчик винды
        mhWnd = hWnd;

        //контекст устройства
        mhDC = GetDC( mhWnd );
		
		//определим формат пикселя
		static PIXELFORMATDESCRIPTOR pfd;
		{
			pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
			pfd.nVersion = 1;
			pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
			pfd.iPixelType = PFD_TYPE_RGBA;
			pfd.cColorBits = 32;
			pfd.cRedBits = 0; pfd.cRedShift = 0; 
			pfd.cGreenBits = 0; pfd.cGreenShift = 0;
			pfd.cBlueBits = 0; pfd.cBlueShift = 0;
			pfd.cAlphaBits = 0; pfd.cAlphaShift = 0;

			pfd.cAccumBits = 0;
			pfd.cAccumRedBits = 0;
			pfd.cAccumGreenBits = 0;
			pfd.cAccumBlueBits = 0;
			pfd.cAccumAlphaBits = 0;

			pfd.cDepthBits = 16;
			pfd.cStencilBits = 0;
			pfd.cAuxBuffers = 0;

			pfd.iLayerType = PFD_MAIN_PLANE;
			pfd.bReserved = 0;

			pfd.dwLayerMask = 0;
			pfd.dwVisibleMask = 0;
			pfd.dwDamageMask = 0;
		}
        int format = ChoosePixelFormat( mhDC, &pfd );
        SetPixelFormat( mhDC, format, &pfd );

        //создадим контекст рендеринга
        mhRC = wglCreateContext( mhDC );
        //сделаем наш контекст текущим
        wglMakeCurrent( mhDC, mhRC );
		
		//со сглаживанием цветов
		glShadeModel ( GL_SMOOTH );
		//черный фон
		glClearColor(0.0f, 0.0f, 0.0f, 0.5f);

		//разрешить тест глубины
		glEnable( GL_DEPTH_TEST );
		//тип теста глубины
		glDepthFunc( GL_LEQUAL );
		//разрешить очистку буфера глубины
		glClearDepth( 1.0f );

		//разрешить смешивание
		glEnable( GL_BLEND );
		//определим способ смешивания
		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

		//улучшение в вычислении перспективы
		glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
		//тест прозрачности    
		glEnable( GL_ALPHA_TEST );
		//закрашивать полигоны полностью
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

		//рассеяный свет
		v[0] = 0.5; v[1] = 0.5; v[2] = 0.5; v[3] = 1.0;
		glLightModelfv( GL_LIGHT_MODEL_AMBIENT, v ); 
		//задаем величину диффузной освещенности
		v[0] = 0.9f; v[1] = 0.9f; v[2] = 0.9f;
		glLightfv( GL_LIGHT0, GL_DIFFUSE, v ) ;
		//задаем позицию лампочки (пусть будет за миром)
		v[0] = 2*WORLD_RADIUS; v[1] = 2*WORLD_RADIUS; v[2] = 2*WORLD_RADIUS; v[3] = 1.0f;
		glLightfv( GL_LIGHT0, GL_POSITION, v );
		//задаем направление света
		v[0] = -1; v[1] = -1; v[2] = -1;
		glLightfv( GL_LIGHT0, GL_SPOT_DIRECTION, v );

		//задаем отражающие свойства материала
		v[0] = 0.9f; v[1] = 0.9f; v[2] = 0.9f;
		glMaterialfv( GL_FRONT, GL_SPECULAR, v );
		//задаем степень блесткости материала
		glMateriali( GL_FRONT, GL_SHININESS, 128 );
		//материалы будут цветными
		glEnable( GL_COLOR_MATERIAL );
		//не учитываем обратные поверхности полигонов
		glCullFace( GL_BACK );

		//включим свет
		glEnable( GL_LIGHTING );
		//включим лампу 0
		glEnable( GL_LIGHT0 );
		
    }
	

	//private
	
    void GLContext::reset()
    {
        mhWnd = NULL;
        mhDC = NULL;
        mhRC = NULL;
    }

    void GLContext::purge()
    {
        if ( mhRC )
        {
            wglMakeCurrent( NULL, NULL );
            wglDeleteContext( mhRC );
        }
        if ( mhWnd && mhDC )
        {
            ReleaseDC( mhWnd, mhDC );
        }
        reset();
    }
	
	/* Утановить перспективу */
	void GLContext::gluPerspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar)
	{
	   GLdouble xmin, xmax, ymin, ymax;

	   ymax = zNear * tan(fovy * M_PI / 360.0);
	   ymin = -ymax;

	   xmin = ymin * aspect;
	   xmax = ymax * aspect;

	   glFrustum(xmin, xmax, ymin, ymax, zNear, zFar);
	}