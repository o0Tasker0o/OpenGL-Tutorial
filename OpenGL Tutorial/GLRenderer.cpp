#include "GLRenderer.h"

GLRenderer::GLRenderer(void) : m_hDC(NULL),
							   m_width(1),
							   m_height(1),
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

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	//Set the default cleared buffer colour
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	
	//Create 128 empty display lists
	m_textList = glGenLists(128);

	//Create a font with the given parameters
	m_font = CreateFont(32,						//Height of the font
						18,						//Use the default width
						0,						//Angle of each character
						0,						//Orientation
						FW_NORMAL,				//Weight of the font
						FALSE,					//Do not use italics
						FALSE,					//Do not underline
						FALSE,					//Do not strikeout
						ANSI_CHARSET,			//Use ansi characters
						OUT_TT_PRECIS,			//Try to use truetype fonts
						CLIP_DEFAULT_PRECIS,	//Default clipping precision
						ANTIALIASED_QUALITY,	//Antialias the font
						FF_DONTCARE | DEFAULT_PITCH,// Family And Pitch
						"Impact");				//Use the impact font
  
	//Set the device context to use the newly created font
	SelectObject(m_hDC, m_font);

	//Create 128 bitmap display lists in the textLists
	wglUseFontBitmaps(m_hDC, 0, 128, m_textList);

	//Everything went fine
	return 0;
}

void GLRenderer::Resize(unsigned int width, unsigned int height)
{  
	//Record the new width and height info
	m_width = width;
	m_height = height;
  
	//Make sure the windows has some width!
	if(0 == m_width)
	{
		m_width = 1;
	}

	//Make sure the windows has some height!
	if(0 == m_height)
	{
		m_height = 1;
	}

	//Create an OpenGL viewport to match the given dimensions
	glViewport(0, 0, (GLsizei) m_width, (GLsizei) m_height); 
}

void GLRenderer::DrawCube()
{
	//Prepare to use the projection matrix
	glMatrixMode(GL_PROJECTION);

	//Set the projection matrix to an identity matrix
	glLoadIdentity();

	//Create a projection matrix
	gluPerspective(45.0,
				   (double) m_width / (double) m_height,
				   0.1,
				   20.0);

	//Prepare to use the Model-View matrix
	glMatrixMode(GL_MODELVIEW);//Use the identity modelview matrix

	//Use the identity modelview matrix
	glLoadIdentity();

	//Create a lookat matrix (i.e. where the camera is)
	//from 4 units away in the z axis looking towards 
	//the origin with the y axis as up
	gluLookAt(0.0f, 0.0f, 4.0f,	//Eye coordinate
			  0.0f, 0.0f, 0.0f,	//Focus coordinates
			  0.0f, 1.0f, 0.0f);//Up vector

	//Rotate the modelview matrix by m_rotation degrees in the y axis
	glRotatef(m_rotation, 0.0f, 1.0f, 0.0f);
	
	glEnable(GL_LIGHTING);

	//Start drawing the trianglestrip at the back of the cube
	glBegin(GL_TRIANGLE_STRIP);
  
		glNormal3f(0.0f, 0.0f, -1.0f);
		glVertex3f(-1.0f, -1.0f, -1.0f);

		glNormal3f(0.0f, 0.0f, -1.0f);
		glVertex3f(-1.0f, 1.0f, -1.0f);

		glNormal3f(0.0f, 0.0f, -1.0f);
		glVertex3f(1.0f, -1.0f, -1.0f);

		glNormal3f(0.0f, 0.0f, -1.0f);
		glVertex3f(1.0f, 1.0f, -1.0f);

	//Stop drawing the trianglestrip
	glEnd();
  
	//Start drawing the trianglestrip at the front of the cube
	glBegin(GL_TRIANGLE_STRIP);
  
		glNormal3f(0.0f, 0.0f, 1.0f);
		glVertex3f(1.0f, -1.0f, 1.0f);

		glNormal3f(0.0f, 0.0f, 1.0f);
		glVertex3f(1.0f, 1.0f, 1.0f);

		glNormal3f(0.0f, 0.0f, 1.0f);
		glVertex3f(-1.0f, -1.0f, 1.0f);

		glNormal3f(0.0f, 0.0f, 1.0f);
		glVertex3f(-1.0f, 1.0f, 1.0f);

	//Stop drawing the trianglestrip
	glEnd();
  
	//Start drawing the trianglestrip at the left of the cube
	glBegin(GL_TRIANGLE_STRIP);
  
		glNormal3f(-1.0f, 0.0f, 0.0f);
		glVertex3f(-1.0f, -1.0f, 1.0f);
	
		glNormal3f(-1.0f, 0.0f, 0.0f);
		glVertex3f(-1.0f, 1.0f, 1.0f);
	
		glNormal3f(-1.0f, 0.0f, 0.0f);
		glVertex3f(-1.0f, -1.0f, -1.0f);
	
		glNormal3f(-1.0f, 0.0f, 0.0f);
		glVertex3f(-1.0f, 1.0f, -1.0f);

	//Stop drawing the trianglestrip
	glEnd();
  
	//Start drawing the trianglestrip at the right of the cube
	glBegin(GL_TRIANGLE_STRIP);
  
		glNormal3f(1.0f, 0.0f, 0.0f);
		glVertex3f(1.0f, -1.0f, -1.0f);
	
		glNormal3f(1.0f, 0.0f, 0.0f);
		glVertex3f(1.0f, 1.0f, -1.0f);
	
		glNormal3f(1.0f, 0.0f, 0.0f);
		glVertex3f(1.0f, -1.0f, 1.0f);
	
		glNormal3f(1.0f, 0.0f, 0.0f);
		glVertex3f(1.0f, 1.0f, 1.0f);

	//Stop drawing the trianglestrip
	glEnd();

	glDisable(GL_LIGHTING);	
}

void GLRenderer::DrawText()
{
	//Prepare to use the projection matrix
	glMatrixMode(GL_PROJECTION);

	//Set the projection matrix to an identity matrix
	glLoadIdentity();

	//Create an orthographic projection matrix (because we're going to be drawing the text in 2D)
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

	//Prepare to use the Model-View matrix
	glMatrixMode(GL_MODELVIEW);

	//Use the identity modelview matrix
	glLoadIdentity();

	//Prepare to draw in the top left corner of the screen
	glRasterPos2f(-0.9f, 0.8f);

	//Create and initialise a 128 character string
	char pTextBuffer[128];
	memset(pTextBuffer, '\0', sizeof(char) * 128);

	//Copy some text and a random number into the string
	sprintf_s(pTextBuffer, 128, "Text plus a number : %d", rand());

	//Prepare to draw the display lists contained in m_textList onwards
	glListBase(m_textList);

	//Draw the text display lists
	glCallLists(strlen(pTextBuffer), GL_UNSIGNED_BYTE, pTextBuffer);
}

void GLRenderer::Render()
{
	//Clear the colour buffer and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
	DrawCube();
	DrawText();
    
	//Display the backbuffer
	SwapBuffers(m_hDC);
}

void GLRenderer::Update()
{
	//Increase the amount of degrees by which to rotate the scene
	m_rotation += 0.05f;
}