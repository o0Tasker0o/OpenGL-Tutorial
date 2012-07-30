#include "GLRenderer.h"

GLRenderer::GLRenderer(void) : m_hDC(NULL)
{
}

GLRenderer::~GLRenderer(void)
{
}

int GLRenderer::Initialise(HDC hdc, unsigned int width, unsigned int height)
{
	//Store the device context in a member variable
	m_hDC = hdc;

	//Use the defined window dimensions
	Resize(width, height);

	//Set the default cleared buffer colour
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	//Everything went fine
	return 0;
}

void GLRenderer::Resize(unsigned int width, unsigned int height)
{  
	//Make sure the windows has some width!
	if(0 == width)
	{
		width = 1;
	}

	//Make sure the windows has some height!
	if(0 == height)
	{
		height = 1;
	}

	//Create an OpenGL viewport to match the given dimensions
	glViewport(0, 0, (GLsizei) width, (GLsizei) height); 
}

void GLRenderer::Render()
{
	//Clear the colour buffer
	glClear(GL_COLOR_BUFFER_BIT);
    
	//Display the backbuffer
	SwapBuffers(m_hDC);
}