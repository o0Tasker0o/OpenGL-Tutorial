#include "GLRenderer.h"

GLRenderer::GLRenderer(void) : m_hDC(NULL),
							   m_rotation(0.0f),
							   m_textureID(0)
{
}

GLRenderer::~GLRenderer(void)
{
	//Clean up the textures that you created
	glDeleteTextures(1, &m_textureID);
}

int GLRenderer::Initialise(HDC hdc, unsigned int width, unsigned int height)
{
	//Store the device context in a member variable
	m_hDC = hdc;

	//Use the defined window dimensions
	Resize(width, height);
	
	//Create the ID for one texture
	glGenTextures(1, &m_textureID);
  
	//Bind the newly created texture ID to the 2D texture target
	glBindTexture(GL_TEXTURE_2D, m_textureID);
  
	//Create a checkerboard style texture (dark, light, dark, light...)
	unsigned char texturePixels[] = { 128, 255, 128, 255,
									  255, 128, 255, 128,
									  128, 255, 128, 255,
									  255, 128, 255, 128 };
  
	//Multiply (modulate) the texture's pixels with the polygon's pixels
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  
	//If the polygon is small, filter the texture using nearest neighbour interpolation
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	//If the polygon is big, filter the texture using nearest neighbour interpolation
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);	
  
	//Texture coordinates are from 0 -> 1. Anything outside that
	//will repeat in the S and T axes
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  
	//Create a texture from the byte array
	glTexImage2D(GL_TEXTURE_2D,	//Create a 2D texture
				 0,				//Use the main image (rather than mipmaps)
				 GL_RGB,		//Store the texture as rgb
				 4,				//The texture is 4 pixels wide
				 4,				//The texture is 4 pixels high
				 0,				//There is no border to the texture
				 GL_LUMINANCE,	//Interpret the texture data as luminance data
				 GL_UNSIGNED_BYTE,//Read the data as an array of unsigned bytes
				 texturePixels);//Use the texturePixels array as data

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
	
	//Turn on texturing
	glEnable(GL_TEXTURE_2D);

	//Start drawing quadrangles
	glBegin(GL_QUADS);
	
		glTexCoord2f(0.0f, 0.0f);		//Use the bottom left of the texture
		glColor3f(1.0f, 0.0f, 0.0f);	//Set the color to red
		glVertex3f(-1.0f, -1.0f, 0.0f);	//Create the bottom left vertex

		glTexCoord2f(0.0f, 1.0f);		//Use the top left of the texture
		glColor3f(1.0f, 0.0f, 0.0f);	//Set the color to red
		glVertex3f(-0.8f, 1.0f, 0.0f);	//Create the top left vertex

		glTexCoord2f(1.0f, 1.0f);		//Use the top right of the texture
		glColor3f(0.0f, 1.0f, 0.0f);	//Set the color to green
		glVertex3f(1.0f, 0.5f, -1.0f);	//Create the top right vertex

		glTexCoord2f(1.0f, 0.0f);		//Use the bottom right of the texture
		glColor3f(0.0f, 1.0f, 0.0f);	//Set the color to green
		glVertex3f(0.6f, -0.9f, -1.0f);	//Create the bottom right vertex

	glEnd();
	
	//Turn off texturing
	glDisable(GL_TEXTURE_2D);

	//Enable transparency
	glEnable(GL_BLEND);
	
	//Setup the function to use to blend fragments
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Start drawing triangles
	glBegin(GL_TRIANGLES);
 
		glColor4f(1.0f, 0.0f, 0.0f, 0.5f);//Set the color to red
		glVertex3f(-1.0f, -1.0f, 0.0f);	//Create the bottom left vertex
	
		glColor4f(0.0f, 1.0f, 0.0f, 0.5f);//Set the color to green
		glVertex3f(0.0f, 1.0f, 0.0f);	//Create the top middle
	
		glColor4f(0.0f, 0.0f, 1.0f, 0.5f);//Set the color to blue
		glVertex3f(1.0f, -1.0f, 0.0f);	//Create the bottom right

	//Stop drawing triangles
	glEnd();

	//Turn off transparency
	glDisable(GL_BLEND);

	//Display the backbuffer
	SwapBuffers(m_hDC);
}

void GLRenderer::Update()
{
	//Increase the amount of degrees by which to rotate the scene
	m_rotation += 0.05f;
}