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
		//������ ��� ��������
		GLfloat v[4]; 

        //���������� �����
        mhWnd = hWnd;

        //�������� ����������
        mhDC = GetDC( mhWnd );
		
		//��������� ������ �������
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

        //�������� �������� ����������
        mhRC = wglCreateContext( mhDC );
        //������� ��� �������� �������
        wglMakeCurrent( mhDC, mhRC );
		
		//�� ������������ ������
		glShadeModel ( GL_SMOOTH );
		//������ ���
		glClearColor(0.0f, 0.0f, 0.0f, 0.5f);

		//��������� ���� �������
		glEnable( GL_DEPTH_TEST );
		//��� ����� �������
		glDepthFunc( GL_LEQUAL );
		//��������� ������� ������ �������
		glClearDepth( 1.0f );

		//��������� ����������
		glEnable( GL_BLEND );
		//��������� ������ ����������
		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

		//��������� � ���������� �����������
		glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
		//���� ������������    
		glEnable( GL_ALPHA_TEST );
		//����������� �������� ���������
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

		//��������� ����
		v[0] = 0.5; v[1] = 0.5; v[2] = 0.5; v[3] = 1.0;
		glLightModelfv( GL_LIGHT_MODEL_AMBIENT, v ); 
		//������ �������� ��������� ������������
		v[0] = 0.9f; v[1] = 0.9f; v[2] = 0.9f;
		glLightfv( GL_LIGHT0, GL_DIFFUSE, v ) ;
		//������ ������� �������� (����� ����� �� �����)
		v[0] = 2*WORLD_RADIUS; v[1] = 2*WORLD_RADIUS; v[2] = 2*WORLD_RADIUS; v[3] = 1.0f;
		glLightfv( GL_LIGHT0, GL_POSITION, v );
		//������ ����������� �����
		v[0] = -1; v[1] = -1; v[2] = -1;
		glLightfv( GL_LIGHT0, GL_SPOT_DIRECTION, v );

		//������ ���������� �������� ���������
		v[0] = 0.9f; v[1] = 0.9f; v[2] = 0.9f;
		glMaterialfv( GL_FRONT, GL_SPECULAR, v );
		//������ ������� ���������� ���������
		glMateriali( GL_FRONT, GL_SHININESS, 128 );
		//��������� ����� ��������
		glEnable( GL_COLOR_MATERIAL );
		//�� ��������� �������� ����������� ���������
		glCullFace( GL_BACK );

		//������� ����
		glEnable( GL_LIGHTING );
		//������� ����� 0
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
	
	/* ��������� ����������� */
	void GLContext::gluPerspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar)
	{
	   GLdouble xmin, xmax, ymin, ymax;

	   ymax = zNear * tan(fovy * M_PI / 360.0);
	   ymin = -ymax;

	   xmin = ymin * aspect;
	   xmax = ymax * aspect;

	   glFrustum(xmin, xmax, ymin, ymax, zNear, zFar);
	}