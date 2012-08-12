//Platform includes
#include <Windows.h>

//Local includes
#include "resource.h"
#include "GLRenderer.h"

//Define the string to appear in the top left corner of the window
#define WINDOW_TITLE "OpenGL Tutorial"

//The name of the window class is the window title plus " Class"
#define WINDOW_CLASS WINDOW_TITLE " Class"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

//Global variable
GLRenderer *g_pGLRenderer(NULL);

//The function that will handle any messages sent to the window
LRESULT CALLBACK WndProc(HWND hwnd, UINT m, WPARAM wp, LPARAM lp)
{    
	//Check the message code
	switch(m)
	{    
		//If the window is resized
		case WM_SIZE:
		{
			RECT windowRect;

			//Get the dimensions of the inside of the window
			GetClientRect(hwnd, &windowRect);

			//If the glrenderer has been created
			if(NULL != g_pGLRenderer)
			{
				//Set the OpenGL viewport size to fit the window
				g_pGLRenderer->Resize(windowRect.right, windowRect.bottom);
			}
		}
		  break;
		//If the window is sent the close/destroy message
		case WM_CLOSE:
		case WM_DESTROY:
			PostQuitMessage(0);
		  break;
		//If a key was pushed down
		case WM_KEYDOWN:
			switch(wp)
			{ 
				//If the escape key was pushed down
				case VK_ESCAPE:
					PostQuitMessage(0);
				  break;
				default:
				  break;
			}
		  break;
		default:
		  break;
	}

	//Pass remaining messages to default handler.
	return (DefWindowProc(hwnd, m, wp, lp));
}

void SetupPixelFormat(HDC hDC)
{
	//Create a description of the pixel format that you want
	PIXELFORMATDESCRIPTOR pfd;

	//Store the size of the structure
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);

	//This version value should always be set to 1
	pfd.nVersion = 1;

	//Drawing to a window, using double buffered opengl
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;

	//Use RGBA colours rather than indexed
	pfd.iPixelType = PFD_TYPE_RGBA;

	//24 bits for the RGB of the pixels (don't include the size of the alpha channel)
	pfd.cColorBits = 24;

	//Number of bits for the depth buffer
	pfd.cDepthBits = 16;
  
	//Find the nearest approximation for your pixel format
	int pixelFormat(ChoosePixelFormat(hDC, &pfd));

	//Set the current pixel format as that nearest approximation
	SetPixelFormat(hDC, pixelFormat, &pfd);
}

//This program's entry point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prev, LPSTR cmd, int show)
{	   
	//Struct to describe the main window.
	WNDCLASSEX windowClass;

	//Empty the windowClass struct
	memset(&windowClass, 0, sizeof(WNDCLASSEX));

	windowClass.cbSize = sizeof(WNDCLASSEX);		//Store the size of the struct
	windowClass.style = CS_HREDRAW | CS_VREDRAW;	//Redraw the window on horizontal or vertical size changes
	windowClass.lpfnWndProc = WndProc;				//The message handling function to be used by the window
	windowClass.hInstance = hInstance;				//Handle to the window's instance
	windowClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));	//Use the custom icon that we added to the project
	windowClass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));	//Use the custom icon that we added to the project
	windowClass.hCursor = LoadCursor(NULL, IDC_CROSS);		//Use a cross cursor in this window instead of the standard arrow
	windowClass.hbrBackground = (HBRUSH) COLOR_ACTIVEBORDER;//Use the "active border" colour for the window background
	windowClass.lpszClassName = WINDOW_CLASS;				//Assign a name to this window class

	//Attempt to register this window class with Windows
	if(!RegisterClassEx(&windowClass))
	{
		//Exit the program
		return -1;
	}

	//Create an, initially invisible, window
	HWND hwnd(CreateWindowEx(NULL,						//Do not use an extended window style
							 WINDOW_CLASS,				//Give the window the class name we have just registered
							 WINDOW_TITLE,				//Set the text in the top left of the window
							 WS_OVERLAPPEDWINDOW,		//Give the window a style with a titlebar and border
							 100, 100,					//Open the window on the desktop at point (100, 100)
							 WINDOW_WIDTH, WINDOW_HEIGHT,//Start the window with the defined dimensions
							 0,							//This window has no parent
							 0,							//This window has no menu
							 hInstance,					//Give the window a handle to the instance of this module
							 NULL));					//No additional data is needed to create this window
   
	//If the window creation failed
	if(!hwnd)
	{
		//Exit the program
		return -1;
	}
  
	//Get the window's device context
	HDC hDC(GetDC(hwnd));

	//Create an appropriate pixel format from the device context
	SetupPixelFormat(hDC);

	//Create a windows opengl context from the device context
	HGLRC hRC(wglCreateContext(hDC));

	//If creating a windows GL context failed
	if(NULL == hRC)
	{
		//Exit the program
		return -1;
	}

	//Make the newly created GL context the current one to use
	if(!wglMakeCurrent(hDC, hRC))
	{
		//Exit the program
		return -1;
	}
	
	//Construct a new GLRenderer object
	g_pGLRenderer = new GLRenderer();

	//Stop the window from being invisible
	ShowWindow(hwnd, SW_SHOW);
  
	//Create a structure that will contain each message sent to the window
	MSG msg;
  
	//Get the internal dimensions of the window
	RECT windowRect;
	GetClientRect(hwnd, &windowRect);

	//If initialising the GLRenderer succeeds
	if(g_pGLRenderer->Initialise(hDC, windowRect.right, windowRect.bottom) == 0)
	{
		//This is the message loop.
		while(true)
		{
			//Take a message from the message queue
			if(PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
			{
				//If a quit message was posted to the queue, then exit the rendering loop
				if(WM_QUIT == msg.message) 
				{
					break;
				}

				//Take a virtual-key message and change it to a MSG struct
				TranslateMessage(&msg);

				//Send this translated message to the message handling function
				DispatchMessage(&msg);
			}
			else
			{
				//Run the OpenGL draw commands
				g_pGLRenderer->Render();
				g_pGLRenderer->Update();
			}
		}		
	}

	//Deconstruct the GLRenderer
	delete g_pGLRenderer; 

	//Make sure there is now no windows GL context in use
	wglMakeCurrent(hDC, NULL);

	//Delete the windows GL context that you created
	wglDeleteContext(hRC);

	//Now that you're done with the window, unregister it
	UnregisterClass(WINDOW_CLASS, windowClass.hInstance);

	//Exit the program with the appropriate return command
	return (int) msg.wParam;
}