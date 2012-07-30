#ifndef GLMANAGER_H
#define GLMANAGER_H

//Platform includes
#include <Windows.h>

//Graphics library includes
#include <GL/gl.h>

class GLRenderer
{
  public:
	GLRenderer(void);
	~GLRenderer(void);

	int Initialise(HDC hdc, unsigned int width, unsigned int height);
	void Resize(unsigned int width, unsigned int height);
	void Render(void);
  private:
    HDC m_hDC;
};

#endif