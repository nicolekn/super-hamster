
//includes areas for keyboard control, mouse control, resizing the window
//and draws a spinning rectangle
#include "Image_Loading/nvImage.h"
#include <windows.h>		// Header file for Windows
#include <iostream>
#include <cstdlib>
#include <gl\gl.h>			// Header file for the OpenGL32 Library
#include <gl\glu.h>			// Header file for the GLu32 Library
using namespace std;
int	mouse_x=0, mouse_y=0;
bool LeftPressed = false;
int screenWidth=480, screenHeight=480;
bool keys[256];
float spin=0;
float speed=0;
float player_speed = .1;
float player_x = 205;
float player_y = 350;

float racer_x = 145;
float racer_y = 350;
//unsigned char pixel[4];
GLubyte pixel[4];

//OPENGL FUNCTION PROTOTYPES
void display();				//called in winmain to draw everything to the screen
void reshape(int width, int height);				//called when the window is resized
void drawCar(double x, double y);
void drawThing(double x, double y);
void init();				//called in winmain when the program starts.
void processKeys();         //called in winmain to process keyboard input
void update();				//called in winmain to update variables


float translationX = 0;
GLuint myTexture = 0;
GLuint myTexture2 = 0;
GLuint myTexture3 = 0;
GLuint myTexture4 = 0;
GLuint myTextureBackground = 0;

GLuint loadPNG(char* name)
{
	// Texture loading object
	nv::Image img;

	GLuint myTextureID;

	// Return true on success
	if (img.loadImageFromFile(name))
	{
		glGenTextures(1, &myTextureID);
		glBindTexture(GL_TEXTURE_2D, myTextureID);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
		glTexImage2D(GL_TEXTURE_2D, 0, img.getInternalFormat(), img.getWidth(), img.getHeight(), 0, img.getFormat(), img.getType(), img.getLevel(0));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);
	}

	else
		MessageBox(NULL, "Failed to load texture", "End of the world", MB_OK | MB_ICONINFORMATION);

	return myTextureID;
}


void init()
{
	glClearColor(0.3725, 0.81176, 0.3725, 0.0);

	/**********************************************************************************************/

	myTexture = loadPNG("player.png");
	myTexture3 = loadPNG("racer.png");
	myTexture2 = loadPNG("grass2.png");
	myTexture4 = loadPNG("racetrack.png");
	myTextureBackground = loadPNG("nebula.png");

	/**********************************************************************************************/
}


/*************    START OF OPENGL FUNCTIONS   ****************/
void display()									
{
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, myTextureBackground);

	glMatrixMode(GL_TEXTURE);
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();


	glTranslatef(10,10,0);
	glTranslatef(-player_x+200,-player_y+200,0);
	glColor3f(0, 0, 0);

	glBindTexture(GL_TEXTURE_2D, myTexture4);
	glBegin(GL_POLYGON);//background
	glTexCoord2f(1.0, 1.0); glVertex2f(2000, 1500);
	glTexCoord2f(0.0, 1.0); glVertex2f(0 , 1500 );
	glTexCoord2f(0.0, 0.0); glVertex2f(0 , 0);
	glTexCoord2f(1.0, 0.0); glVertex2f(2000 , 0 );
	glEnd();

	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	
	drawThing(730, 1000);
	drawThing(300, 790);
	drawThing(500, 500);
/*
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, myTexture2);
	glColor3f(0, 1, 0.2);//inner grass
	glBegin(GL_POLYGON);
	glTexCoord2f(1.0, 1.0); glVertex2f(1800 , 1300);
	glTexCoord2f(0.0, 1.0); glVertex2f(230, 1300 );
	glTexCoord2f(0.0, 0.0); glVertex2f(230 , 230 );
	glTexCoord2f(1.0, 0.0); glVertex2f(1800 , 230 );
	glEnd();
	glPopMatrix();
	*/

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, myTexture3);
	//draw player car
	glPushMatrix();
	glColor3f(1, 0, 0);//player
	glTranslatef(player_x, player_y, 0);//translate back
	glRotatef(spin, 0, 0, 1);//rotate
	glTranslatef(-player_x, -player_y, 0);//translate from center to origin
	drawCar(player_x, player_y);
	glPopMatrix();
	

	glPushMatrix();
	glColor3f(0, 0, 1);//racer
	drawCar(racer_x, racer_y);
	glPopMatrix();

	glReadPixels(player_x, player_y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
	cout << "R: " << (int)pixel[0] << endl;
	cout << "G: " << (int)pixel[1] << endl;
	cout << "B: " << (int)pixel[2] << endl;
	


	if (pixel[0] != 77)
	{
		player_speed = .09;
	}
	else {
		player_speed = .16;
	}


	/*if(player_x+25)*/
	glDisable(GL_TEXTURE_2D);

	glDisable(GL_BLEND);
	glFlush();
}
void drawCar(double x, double y)//cars are 50 by 80 with center at x,y
{
	glBegin(GL_POLYGON);
	glTexCoord2f(1.0, 1.0); glVertex2f(25 + x, 40 + y);
	glTexCoord2f(0.0, 1.0); glVertex2f(-25 + x, 40 + y);
	glTexCoord2f(0.0, 0.0); glVertex2f(-25 + x, -40 + y);
	glTexCoord2f(1.0, 0.0); glVertex2f(25 + x, -40 + y);
	glEnd();
}

void drawThing(double x, double y)
{
	glBegin(GL_POLYGON);
	glTexCoord2f(1.0, 1.0); glVertex2f(5 + x, 5 + y);
	glTexCoord2f(0.0, 1.0); glVertex2f(-5 + x, 5 + y);
	glTexCoord2f(0.0, 0.0); glVertex2f(-5 + x, -5 + y);
	glTexCoord2f(1.0, 0.0); glVertex2f(5 + x, -5 + y);
	glEnd();
}

void reshape(int width, int height)		// Resize the OpenGL window
{
	screenWidth=width; screenHeight = height;           // to ensure the mouse coordinates match 
														// we will use these values to set the coordinate system

	glViewport(0,0,width,height);						// Reset the current viewport

	glMatrixMode(GL_PROJECTION);						// select the projection matrix stack
	glLoadIdentity();									// reset the top of the projection matrix to an identity matrix

	gluOrtho2D(0,screenWidth,0,screenHeight);           // set the coordinate system for the window

	glMatrixMode(GL_MODELVIEW);							// Select the modelview matrix stack
	glLoadIdentity();									// Reset the top of the modelview matrix to an identity matrix
}

void processKeys()
{

	if(keys[VK_UP])
	{
		
		std::cout << "player y: " << player_y << std::endl<<"player x: "<<player_x<<endl; 


			player_x -= player_speed*sinf(spin/180*3.1415);
			player_y += player_speed * cosf(spin / 180 * 3.1415);


			//glReadPixels(player_x, player_y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
			//cout << "R: " << (int)pixel[0] << endl;
			//cout << "G: " << (int)pixel[1] << endl;
			//cout << "B: " << (int)pixel[2] << endl;



	}
	if(keys[VK_DOWN])
	{
		
		std::cout << "player y: " << player_y << std::endl << "player x" << player_x << endl;;

		player_x += player_speed * sinf(spin / 180 * 3.1415);
		player_y -= player_speed * cosf(spin / 180 * 3.1415);
	
		glReadPixels(player_x, player_y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
		cout << "R: " << (int)pixel[0] << endl;
		cout << "G: " << (int)pixel[1] << endl;
		cout << "B: " << (int)pixel[2] << endl;


	}
	if (keys[VK_LEFT])
	{
		std::cout << "camera_x: " << player_x << std::endl; 

			spin+=.1;

		
	}
	if (keys[VK_RIGHT])
	{
		std::cout << "camera_x: " << player_x << std::endl;

			spin-=.1;

	}
}
void update()
{
	spin += speed;
	if(spin > 360)
		spin = 0;
}
/**************** END OPENGL FUNCTIONS *************************/

//WIN32 functions
LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc
void KillGLWindow();									// releases and destroys the window
bool CreateGLWindow(char* title, int width, int height); //creates the window
int WINAPI WinMain(	HINSTANCE, HINSTANCE, LPSTR, int);  // Win32 main function

//win32 global variabless
HDC			hDC=NULL;		// Private GDI Device Context
HGLRC		hRC=NULL;		// Permanent Rendering Context
HWND		hWnd=NULL;		// Holds Our Window Handle
HINSTANCE	hInstance;		// Holds The Instance Of The Application


/******************* WIN32 FUNCTIONS ***************************/
int WINAPI WinMain(	HINSTANCE	hInstance,			// Instance
					HINSTANCE	hPrevInstance,		// Previous Instance
					LPSTR		lpCmdLine,			// Command Line Parameters
					int			nCmdShow)			// Window Show State
{
	MSG		msg;									// Windows Message Structure
	bool	done=false;								// Bool Variable To Exit Loop


	AllocConsole();
	FILE *stream;
	freopen_s(&stream, "CONOUT$", "w", stdout);

	// Create Our OpenGL Window
	if (!CreateGLWindow("racecar?",screenWidth,screenHeight))
	{
		return 0;									// Quit If Window Was Not Created
	}

	while(!done)									// Loop That Runs While done=FALSE
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// Is There A Message Waiting?
		{
			if (msg.message==WM_QUIT)				// Have We Received A Quit Message?
			{
				done=true;							// If So done=TRUE
			}
			else									// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		else										// If There Are No Messages
		{
			if(keys[VK_ESCAPE])
				done = true;

			processKeys();			//process keyboard
			
			display();					// Draw The Scene
			update();					// update variables
			SwapBuffers(hDC);				// Swap Buffers (Double Buffering)
		}
	}

	// Shutdown
	KillGLWindow();									// Kill The Window
	return (int)(msg.wParam);						// Exit The Program
}

//WIN32 Processes function - useful for responding to user inputs or other events.
LRESULT CALLBACK WndProc(	HWND	hWnd,			// Handle For This Window
							UINT	uMsg,			// Message For This Window
							WPARAM	wParam,			// Additional Message Information
							LPARAM	lParam)			// Additional Message Information
{
	switch (uMsg)									// Check For Windows Messages
	{
		case WM_CLOSE:								// Did We Receive A Close Message?
		{
			PostQuitMessage(0);						// Send A Quit Message
			return 0;								// Jump Back
		}
		break;

		case WM_SIZE:								// Resize The OpenGL Window
		{
			reshape(LOWORD(lParam),HIWORD(lParam));  // LoWord=Width, HiWord=Height
			return 0;								// Jump Back
		}
		break;

		case WM_LBUTTONDOWN:
			{
	            mouse_x = LOWORD(lParam);          
				mouse_y = screenHeight - HIWORD(lParam);
				LeftPressed = true;
			}
		break;

		case WM_LBUTTONUP:
			{
	            LeftPressed = false;
			}
		break;

		case WM_MOUSEMOVE:
			{
	            mouse_x = LOWORD(lParam);          
				mouse_y = screenHeight  - HIWORD(lParam);
			}
		break;
		case WM_KEYDOWN:							// Is A Key Being Held Down?
		{
			keys[wParam] = true;					// If So, Mark It As TRUE
			return 0;								// Jump Back
		}
		break;
		case WM_KEYUP:								// Has A Key Been Released?
		{
			keys[wParam] = false;					// If So, Mark It As FALSE
			return 0;								// Jump Back
		}
		break;
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

void KillGLWindow()								// Properly Kill The Window
{
	if (hRC)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL,NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										// Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd,hDC))					// Are We Able To Release The DC
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass("OpenGL",hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									// Set hInstance To NULL
	}
}

/*	This Code Creates Our OpenGL Window.  Parameters Are:					*
 *	title			- Title To Appear At The Top Of The Window				*
 *	width			- Width Of The GL Window Or Fullscreen Mode				*
 *	height			- Height Of The GL Window Or Fullscreen Mode			*/
 
bool CreateGLWindow(char* title, int width, int height)
{
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left=(long)0;			// Set Left Value To 0
	WindowRect.right=(long)width;		// Set Right Value To Requested Width
	WindowRect.top=(long)0;				// Set Top Value To 0
	WindowRect.bottom=(long)height;		// Set Bottom Value To Requested Height

	hInstance			= GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= hInstance;							// Set The Instance
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= NULL;									// No Background Required For GL
	wc.lpszMenuName		= NULL;									// We Don't Want A Menu
	wc.lpszClassName	= "OpenGL";								// Set The Class Name

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;											// Return FALSE
	}
	
	dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
	dwStyle=WS_OVERLAPPEDWINDOW;							// Windows Style
	
	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	// Create The Window
	if (!(hWnd=CreateWindowEx(	dwExStyle,							// Extended Style For The Window
								"OpenGL",							// Class Name
								title,								// Window Title
								dwStyle |							// Defined Window Style
								WS_CLIPSIBLINGS |					// Required Window Style
								WS_CLIPCHILDREN,					// Required Window Style
								0, 0,								// Window Position
								WindowRect.right-WindowRect.left,	// Calculate Window Width
								WindowRect.bottom-WindowRect.top,	// Calculate Window Height
								NULL,								// No Parent Window
								NULL,								// No Menu
								hInstance,							// Instance
								NULL)))								// Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		24,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		24,											// 24Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};
	
	if (!(hDC=GetDC(hWnd)))							// Did We Get A Device Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if (!(hRC=wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if(!wglMakeCurrent(hDC,hRC))					// Try To Activate The Rendering Context
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	ShowWindow(hWnd,SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window
	reshape(width, height);					// Set Up Our Perspective GL Screen

	init();
	
	return true;									// Success
}



