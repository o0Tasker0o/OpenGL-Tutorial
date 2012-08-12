#ifndef GLMANAGER_H
#define GLMANAGER_H

//Platform includes
#include <Windows.h>

//Graphics library includes
#include <GL/gl.h>
#include <GL/glu.h>		//GL Utility functions

//Utility includes
#include <stdio.h>

class GLRenderer
{
  public:
	GLRenderer(void);
	~GLRenderer(void);

	int Initialise(HDC hdc, unsigned int width, unsigned int height);
	void Resize(unsigned int width, unsigned int height);
	void Render(void);
	void Update();
	void DrawCube();
	void DrawText();
  private:
    HDC m_hDC;
	int m_width, m_height;
	float m_rotation;
	HFONT m_font;
	GLuint m_textList;
};

#endif