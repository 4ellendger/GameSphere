#pragma once

#include <windows.h>
#include <gl/gl.h>
#include "Point3D.h"

class GLContext
{

public:
    HDC mhDC;
    HWND mhWnd;
    HGLRC mhRC;
	GLContext(void);
	~GLContext(void);
	void init(HWND hWnd);
	void GLContext::gluPerspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar);
	void GLContext::setCamera(Point3D pCP, float pDX, float pDY, float pDZ);
	
private:

	void reset();
	void purge();
};

