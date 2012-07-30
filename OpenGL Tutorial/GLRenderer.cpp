#include "GLRenderer.h"

GLRenderer::GLRenderer(void) : m_hDC(NULL),
							   m_rotation(0.0f)
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
	
	//Turn on depth testing
	glEnable(GL_DEPTH_TEST);

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

	//Set the projection matrix as the current matrix
	glMatrixMode(GL_PROJECTION);

	//Reset the projection matrix to an identity matrix
	glLoadIdentity();

	//Multiply the identity projection matrix by a perspective
	//matrix with the following parameters
	gluPerspective(45.0,		//45 degree field of view in the y direction
				   (double) width / (double) height,	//Aspect ratio
				   0.1,			//The near clipping plane
				   20.0);		//The far clipping plane

	//Switch back to using the modelview matrix
	glMatrixMode(GL_MODELVIEW);
}

void GLRenderer::Render()
{
	//Clear the colour buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Use the identity modelview matrix
	glLoadIdentity();

	//Create a lookat matrix (i.e. where the camera is)
	//from 4 units away in the z axis looking towards 
	//the origin with the y axis as up
	gluLookAt(0.0f, 0.0f, 4.0f,	//Eye coordinate
			0.0f, 0.0f, 0.0f,	//Focus coordinates
			0.0f, 1.0f, 0.0f);	//Up vector

	//Rotate the modelview matrix by m_rotation degrees in the y axis
	glRotatef(m_rotation, 0.0f, 1.0f, 0.0f);

	//Start drawing triangles
	glBegin(GL_TRIANGLES);
 
		glColor3f(1.0f, 0.0f, 0.0f);	//Set the color to red
		glVertex3f(-1.0f, -1.0f, 0.0f);	//Create the bottom left vertex
	
		glColor3f(0.0f, 1.0f, 0.0f);	//Set the color to green
		glVertex3f(0.0f, 1.0f, 0.0f);	//Create the top middle
	
		glColor3f(0.0f, 0.0f, 1.0f);	//Set the color to blue
		glVertex3f(1.0f, -1.0f, 0.0f);	//Create the bottom right

	//Stop drawing triangles
	glEnd();

	//Start drawing quadrangles
	glBegin(GL_QUADS);

	  glColor3f(0.5f, 0.5f, 0.5f);		//Set the colour to dark grey
	  glVertex3f(-1.0f, -1.0f, 0.0f);	//Create the bottom left vertex

	  glColor3f(0.5f, 0.5f, 0.5f);		//Set the colour to dark grey
	  glVertex3f(-0.8f, 1.0f, 0.0f);	//Create the top left vertex

	  glColor3f(0.75f, 0.75f, 0.75f);	//Set the colour to light grey
	  glVertex3f(1.0f, 0.5f, -1.0f);	//Create the top right vertex

	  glColor3f(0.75f, 0.75f, 0.75f);	//Set the colour to light grey
	  glVertex3f(0.6f, -0.9f, -1.0f);	//Create the bottom right vertex

	glEnd();

	//Display the backbuffer
	SwapBuffers(m_hDC);
}

void GLRenderer::Update()
{
	//Increase the amount of degrees by which to rotate the scene
	m_rotation += 0.05f;
}