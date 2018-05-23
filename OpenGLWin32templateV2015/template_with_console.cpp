
//includes areas for keyboard control, mouse control, resizing the window
//and draws a spinning rectangle
#include "Image_Loading/nvImage.h"
#include <windows.h>		// Header file for Windows
#include <iostream>
#include <cstdlib>
#include <gl\gl.h>			// Header file for the OpenGL32 Library
#include <gl\glu.h>			// Header file for the GLu32 Library
#include <math.h>
using namespace std;
int	mouse_x = 0, mouse_y = 0;
bool LeftPressed = false;
int screenWidth = 480, screenHeight = 480;
bool keys[256];
bool start = false;
float spin = 0;
float racer_spin = -13.7;
float racer_speed = .1;
float speed = 0;
float player_speed = .1;
float player_x = 205;
float player_y = 560;
int frame_count = 0;
int frame_count_racer = 0;
float racer_x = 110;
float racer_y = 560;




float player_tl = 0;
float player_tr = 0;
float player_br = 0;
float player_bl = 0;

float racer_x1 = 0;
float racer_x2 = 0;
float racer_y1 = 0;
float racer_y2 = 0;


float mPlayer[16];
float mRacer[16];
double mushXCoords[4] = { 300,500,730,1120 };
double mushYCoords[4] = { 790,210,1000,620 };

double checkPointX[11] = { 200,695,788,1020,1083,1740,1766,1347,1257,245,110 };
double checkPointY[11] = { 800,855,1042,1160,1334,1290,835,695,287,254,560 };
int currentPoint = 0;
int lapNumber = 0;
GLubyte pixel[4];

//OPENGL FUNCTION PROTOTYPES
void display();				//called in winmain to draw everything to the screen
void startScreen();
void reshape(int width, int height);				//called when the window is resized
void drawBall(double x, double y);
void drawThing(double x, double y);
void drawSeeds();
void moveRacer(); 
void init();				//called in winmain when the program starts.
void processKeys();         //called in winmain to process keyboard input
void update();				//called in winmain to update variables
bool touchedSeed(double x, double y);
bool speedup();
bool isOnTrack(double x, double y);
bool collision();
bool lost();
bool crossedFinish();
void swapTexturePlayer();
void swapTextureRacer();
void drawMiniMap();

float translationX = 0;
GLuint myTexture = 0;
GLuint myTexture2 = 0;
GLuint myTexture3 = 0;
GLuint myTexture4 = 0;
GLuint myTexture5 = 0;
GLuint myTexture6 = 0;
GLuint myTexture7 = 0;
GLuint start_space = 0;
GLuint starting_image = 0;
GLuint instructions = 0;
GLuint title = 0;
GLuint current_player = 0;
GLuint current_racer = 0; 
GLuint contracted_racer = 0;
GLuint contracted_player = 0;
GLuint extended_racer = 0;
GLuint extended_player = 0;
GLuint finishImage = 0;
GLuint lostImage = 0;
GLuint endImage = 0;
GLuint minimap = 0;

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

	extended_player = loadPNG("extendedrun.png");
	contracted_racer = loadPNG("contractedrun_op.png");
	extended_racer = loadPNG("extendedrun_op.png");
	contracted_player = loadPNG("contractedrun.png");
	myTexture4 = loadPNG("racetrack.png");
	myTexture5 = loadPNG("speedup.png");
	myTexture6 = loadPNG("finishline.png");
	myTexture7 = loadPNG("rainbow.png");
	start_space = loadPNG("spaceinstruction.png");
	instructions = loadPNG("instructions.png");
	starting_image = loadPNG("starting.png");
	title = loadPNG("name.png");
	finishImage = loadPNG("finish.png");
	lostImage = loadPNG("lost.png");
	current_player = extended_player;
	current_racer = contracted_racer;
	minimap = loadPNG("smalltrack.png");


	/**********************************************************************************************/
}


/*************    START OF OPENGL FUNCTIONS   ****************/


void startScreen()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);

	glClearColor(0, 0, 0, 0.0);

	glMatrixMode(GL_TEXTURE);
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	glColor3f(0, 0, 0);
	glBindTexture(GL_TEXTURE_2D, starting_image);
	glBegin(GL_POLYGON);//background
	glTexCoord2f(1.0, 1.0); glVertex2f(screenWidth, screenHeight);
	glTexCoord2f(0.0, 1.0); glVertex2f(0, screenHeight);
	glTexCoord2f(0.0, 0.0); glVertex2f(0, 0);
	glTexCoord2f(1.0, 0.0); glVertex2f(screenWidth, 0);
	glEnd();

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, start_space);
	glBegin(GL_POLYGON);
	glTexCoord2f(1.0, 1.0); glVertex2f(470, 70);
	glTexCoord2f(0.0, 1.0); glVertex2f(20, 70);
	glTexCoord2f(0.0, 0.0); glVertex2f(20, 20);
	glTexCoord2f(1.0, 0.0); glVertex2f(470, 20);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, instructions);
	glBegin(GL_POLYGON);
	glTexCoord2f(1.0, 1.0); glVertex2f(470, 220);
	glTexCoord2f(0.0, 1.0); glVertex2f(20, 220);
	glTexCoord2f(0.0, 0.0); glVertex2f(20, 80);
	glTexCoord2f(1.0, 0.0); glVertex2f(470, 80);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, title);
	glBegin(GL_POLYGON);
	glTexCoord2f(1.0, 1.0); glVertex2f(470, 450);
	glTexCoord2f(0.0, 1.0); glVertex2f(20, 450);
	glTexCoord2f(0.0, 0.0); glVertex2f(20, 300);
	glTexCoord2f(1.0, 0.0); glVertex2f(470, 300);
	glEnd();


	glDisable(GL_TEXTURE_2D);

	glDisable(GL_BLEND);
	glFlush();

}
void display()
{


	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);



	glMatrixMode(GL_TEXTURE);
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();


	glTranslatef(-player_x + 250, -player_y + 250, 0);
	glColor3f(0, 0, 0);

	glBindTexture(GL_TEXTURE_2D, myTexture4);
	glBegin(GL_POLYGON);//background
	glTexCoord2f(1.0, 1.0); glVertex2f(2000, 1500);
	glTexCoord2f(0.0, 1.0); glVertex2f(0, 1500);
	glTexCoord2f(0.0, 0.0); glVertex2f(0, 0);
	glTexCoord2f(1.0, 0.0); glVertex2f(2000, 0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, myTexture6);
	glBegin(GL_POLYGON);//finish line
	glTexCoord2f(1.0, 1.0); glVertex2f(230, 520);
	glTexCoord2f(0.0, 1.0); glVertex2f(80, 520);
	glTexCoord2f(0.0, 0.0); glVertex2f(80, 490);
	glTexCoord2f(1.0, 0.0); glVertex2f(230, 490);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, myTexture7);
	glBegin(GL_POLYGON);//raindow road
	glTexCoord2f(1.0, 1.0); glVertex2f(530, 800);
	glTexCoord2f(0.0, 1.0); glVertex2f(360, 800);
	glTexCoord2f(0.0, 0.0); glVertex2f(360, 750);
	glTexCoord2f(1.0, 0.0); glVertex2f(530, 750);
	glEnd();

	glBegin(GL_POLYGON);//raindow road 2
	glTexCoord2f(0.0, 0.0); glVertex2f(1870, 1140);
	glTexCoord2f(0.0, 1.0); glVertex2f(1820, 1140);
	glTexCoord2f(1.0, 1.0); glVertex2f(1820, 950);
	glTexCoord2f(1.0, 0.0); glVertex2f(1870, 950);
	glEnd();

	glBegin(GL_POLYGON);//raindow road 3
	glTexCoord2f(1.0, 1.0); glVertex2f(900, 240);
	glTexCoord2f(0.0, 1.0); glVertex2f(600, 240);
	glTexCoord2f(0.0, 0.0); glVertex2f(600, 190);
	glTexCoord2f(1.0, 0.0); glVertex2f(900, 190);
	glEnd();

	glBegin(GL_POLYGON);//raindow road 4
	glTexCoord2f(1.0, 1.0); glVertex2f(1460, 1400);
	glTexCoord2f(0.0, 1.0); glVertex2f(1236, 1400);
	glTexCoord2f(0.0, 0.0); glVertex2f(1236, 1350);
	glTexCoord2f(1.0, 0.0); glVertex2f(1460, 1350);
	glEnd();

	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, myTexture5);





	//draw sunflower seeds
	drawSeeds();



	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, current_player);
	//draw player car
	glPushMatrix();
	glColor3f(1, 0, 0);//player
	glTranslatef(player_x, player_y, 0);//translate back
	glRotatef(spin, 0, 0, 1);//rotate
	glTranslatef(-player_x, -player_y, 0);//translate from center to origin
	drawBall(player_x, player_y);
	
	glPopMatrix();
	drawMiniMap();
	glBindTexture(GL_TEXTURE_2D, current_racer);
	glPushMatrix();
	glColor3f(0, 0, 1);//racer
	if (!crossedFinish() || lost() || currentPoint >= 11)
	{
		moveRacer();
	}
	glTranslatef(racer_x, racer_y, 0);//translate back
	glRotatef(racer_spin, 0, 0, 1);//rotate
	glTranslatef(-racer_x, -racer_y, 0);//translate from center to origin
	drawBall(racer_x, racer_y);
	glGetFloatv(GL_MODELVIEW_MATRIX, mRacer);
	glPopMatrix();




	if (isOnTrack(racer_x, racer_y))
	{
		racer_speed = 0.06;
	}
	else
	{
		racer_speed = 0.03;
	}


	if (isOnTrack(player_x, player_y))
	{
		player_speed = .07;
		if (speedup())
		{
			player_speed =0.3;
		}
		else if (touchedSeed(player_x, player_y))
		{
			player_speed = 0.5;
		}
	}
	else if (player_x < 40 || player_x>1960 || player_y < 40 || player_y>1460)
	{
		player_speed = 0;
		if (player_x < 40)
		{
			player_x = 42;
		}
		if (player_x > 1960)
		{
			player_x = 1958;
		}
		if (player_y < 40)
		{
			player_y = 42;
		}
		if (player_y > 1460)
		{
			player_y = 1458;
		}
	}
	else {


		player_speed = .01;

	}


	if (lapNumber>0||lost()||crossedFinish())
	{

			glBindTexture(GL_TEXTURE_2D, endImage);

		glBegin(GL_POLYGON);//finish line
		glTexCoord2f(1.0, 1.0); glVertex2f(player_x+250, player_y+250);
		glTexCoord2f(0.0, 1.0); glVertex2f(player_x - 250, player_y + 250);
		glTexCoord2f(0.0, 0.0); glVertex2f(player_x - 250, player_y - 250);
		glTexCoord2f(1.0, 0.0); glVertex2f(player_x + 250, player_y - 250);
		glEnd();
	}
	
	glColor3f(1, 0, 0);



	glDisable(GL_TEXTURE_2D);

	glDisable(GL_BLEND);
	glFlush();
}

bool isOnTrack(double car_x, double car_y)
{
	bool insideRect1 = car_x < 230 && car_x>80 && car_y > 420 && car_y < 660;

	bool insideCorner1 = car_x < 260 && car_x>90 && car_y > 660 && car_y < 812;
	bool insideCorner2 = car_x < 350 && car_x>155 && car_y > 740 && car_y < 875;
	bool insideCorner25 = car_x > 155 && car_x < 350 && car_y>690 && car_y < 807;

	bool insideRect2 = car_x < 600 && car_x>350 && car_y > 725 && car_y < 880;

	bool insideCorner3 = car_x < 776 && car_x>600 && car_y > 745 && car_y < 890;
	bool insideCorner35 = car_x > 770 && car_x < 830 && car_y>830 && car_y < 885;
	bool insideCorner4 = car_x < 850 && car_x>680 && car_y > 880 && car_y < 1040;
	bool insideCorner5 = car_x < 1070 && car_x>725 && car_y > 990 && car_y < 1184;
	bool insideCorner6 = car_x < 1138 && car_x>965 && car_y > 1166 && car_y < 1420;

	bool insideRect3 = car_x < 1640 && car_x>1110 && car_y > 1265 && car_y < 1410;

	bool insideCorner7 = car_x < 1800 && car_x>1640 && car_y > 1270 && car_y < 1400;
	bool insideCorner8 = car_x < 1880 && car_x>1725 && car_y > 1190 && car_y < 1335;
	bool insideCorner85 = car_x >1637  && car_x<1750 && car_y > 1196 && car_y < 1275;

	bool insideRect4 = car_x < 1880 && car_x>1725 && car_y > 930 && car_y < 1190;

	bool insideCorner9 = car_x < 1880 && car_x>1686 && car_y > 760 && car_y < 930;
	bool insideCorner10 = car_x < 1686 && car_x>1530 && car_y > 680 && car_y < 1010;

	bool insideCorner16 = car_x > 1420 && car_x<1535 && car_y > 650 && car_y < 810;
	bool insideCorner17 = car_x > 1277 && car_x<1436 && car_y > 550 && car_y < 694;
	bool insideCorner18 = car_x > 1335 && car_x<1425 && car_y > 668 && car_y < 765;

	bool insideRect5 = car_x < 1530 && car_x>1290 && car_y > 860 && car_y < 1010;

	bool insideCorner11 = car_x < 1290 && car_x>1175 && car_y > 840 && car_y < 1010;
	bool insideCorner12 = car_x < 1270 && car_x>1100 && car_y > 800 && car_y < 925;

	bool insideRect6 = car_x < 1250 && car_x>1100 && car_y > 580 && car_y < 800;

	bool insideCorner13 = car_x < 1430&& car_x>1100 && car_y > 465 && car_y < 575;
	bool insideCorner14 = car_x > 1184 && car_x < 1346 && car_y>300 && car_y < 475;
	bool insideCorner15 = car_x > 1080 && car_x < 1310 && car_y>170 && car_y < 345;

	bool insideRect7= car_x < 1100 && car_x>310 && car_y > 175 && car_y < 330;

	bool insideCorner20 = car_x < 310 && car_x>120 && car_y > 180&& car_y < 330;
	bool insideCorner21 = car_x < 230 && car_x>80 && car_y > 200 && car_y < 420;

	return insideRect1 || insideRect2 ||insideRect3||insideRect4||insideRect5
		||insideRect6||insideRect7||insideCorner1||insideCorner2 || insideCorner3
		|| insideCorner25|| insideCorner35|| insideCorner4 || insideCorner5
		|| insideCorner6 || insideCorner7 || insideCorner8|| insideCorner85
		|| insideCorner9||insideCorner10||insideCorner11||insideCorner12 
		|| insideCorner13 || insideCorner14|| insideCorner15|| insideCorner16
		|| insideCorner17|| insideCorner18||insideCorner20 || insideCorner21;

}

bool collision()
{
	return(sqrt((player_x - racer_x)*(player_x - racer_x) + (player_y - racer_y)*(player_y - racer_y)) <= 50);
}

bool crossedFinish()
{
	bool crossed = false;
	if (player_x < 230 && player_x>80 && player_y > 490 && player_y < 520)
	{
		endImage = finishImage;
		crossed = true;
		lapNumber++;
	}
	return crossed;
}

bool lost()
{
	bool lost = false;
	if (racer_x < 230 && racer_x>80 && racer_y > 490 && racer_y < 520)
	{
		endImage = lostImage;
		lost = true;
		lapNumber++;
	}
	return lost;
}
void swapTexturePlayer()
{
	if (current_player == extended_player)
	{
		current_player = contracted_player;
	}
	else
	{
		current_player = extended_player;
	}
}
void swapTextureRacer()
{
	if (current_racer == contracted_racer)
	{
		current_racer = extended_racer;
	}
	else
	{
		current_racer = contracted_racer;
	}
}
void drawMiniMap()
{
	glBindTexture(GL_TEXTURE_2D, minimap);
	glBegin(GL_POLYGON);
	glTexCoord2f(1.0, 1.0); glVertex2f(player_x-140, player_y+230);
	glTexCoord2f(0.0, 1.0); glVertex2f(player_x-240, player_y+230);
	glTexCoord2f(0.0, 0.0); glVertex2f(player_x-240, player_y+155);
	glTexCoord2f(1.0, 0.0); glVertex2f(player_x-140, player_y+155);
	glEnd();


	glBindTexture(GL_TEXTURE_2D, extended_racer);
	glBegin(GL_POLYGON);//draw opponent icon
	glTexCoord2f(1.0, 1.0); glVertex2f(racer_x / 20 + (player_x - 240) + 5, racer_y / 20 + (player_y + 155) + 5);
	glTexCoord2f(0.0, 1.0); glVertex2f(racer_x / 20 + (player_x - 240) - 5, racer_y / 20 + (player_y + 155) + 5);
	glTexCoord2f(0.0, 0.0); glVertex2f(racer_x / 20 + (player_x - 240) - 5, racer_y / 20 + (player_y + 155) - 5);
	glTexCoord2f(1.0, 0.0); glVertex2f(racer_x / 20 + (player_x - 240) + 5, racer_y / 20 + (player_y + 155) - 5);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, contracted_player);
	glBegin(GL_POLYGON);//player icon
	glTexCoord2f(1.0, 1.0); glVertex2f(player_x /20+(player_x-240)+5, player_y /20+(player_y+155)+5);
	glTexCoord2f(0.0, 1.0); glVertex2f(player_x/20 + (player_x - 240)-5, player_y/20 + (player_y + 155)+5);
	glTexCoord2f(0.0, 0.0); glVertex2f(player_x/20 + (player_x - 240)-5, player_y /20 + (player_y + 155)-5);
	glTexCoord2f(1.0, 0.0); glVertex2f(player_x/20 + (player_x - 240)+5, player_y/20 + (player_y + 155)-5);
	glEnd();
	//cout << player_x / 20 + (player_x - 240) << endl;
}
bool touchedSeed(double car_x, double car_y)
{
	int i = 0;
	bool touched = false;
	while (!touched && i<4)
	{
		touched = (car_x + 20 > mushXCoords[i] && car_x - 20 <mushXCoords[i]) && (car_y + 20 > mushYCoords[i] && car_y - 20 < mushYCoords[i]);
		if (touched)
		{
			mushXCoords[i] = 0;
			mushYCoords[i] = 0;
		}
		i++;
	}
	
	return touched;
}
void drawSeeds()
{
	for (int i = 0; i < 4; i++)
	{
		drawThing(mushXCoords[i], mushYCoords[i]);
	}
}
bool speedup()
{
	bool speedup1 = player_x < 530 && player_x>360 && player_y < 800 && player_y>750;
	bool speedup3= player_x < 900 && player_x>600 && player_y < 240 && player_y>190;
	bool speedup2 = player_x > 1820 && player_x < 1870 && player_y>950 && player_y < 1140;
	bool speedup4 = player_x > 1236 && player_x < 1460 && player_y>1350 && player_y < 1400;
	return speedup1||speedup2||speedup3||speedup4;
}
void moveRacer()
{
	
	if (currentPoint < 11)
	{
		if(sqrt( (pow((checkPointX[currentPoint] - racer_x),2)+pow((checkPointY[currentPoint] - racer_y),2))) >45)
		{
			//racer_spin = -atan2(checkPointX[currentPoint] - racer_x, checkPointY[currentPoint] - racer_y) * 180 * 3.1415;

				racer_spin = (int)(-atan2(checkPointX[currentPoint] - racer_x, checkPointY[currentPoint] - racer_y) * 180 * 3.1415) % 360 + 360;
			
			//cout << -atan2(checkPointX[currentPoint] - racer_x, checkPointY[currentPoint] - racer_y) * 180 * 3.1415 << endl;
			


			
			
			if (currentPoint == 0)
			{
				racer_spin = -13.7;
			}
			/*
			else if (currentPoint == 1)
			{
				racer_spin = -83.7;
			}
			
			else if (currentPoint == 2)
			{
				racer_spin = -26.4;
			}
			else if (currentPoint == 3)
			{
				racer_spin = -63;
			}
			else if (currentPoint == 4)
			{
				racer_spin = -19.9;
			}
			else if (currentPoint == 5)
			{
				racer_spin = -93.83;
			}
			else if (currentPoint == 6)
			{
				racer_spin = 179.7;
			}
			else if (currentPoint == 7)
			{
				racer_spin = 108.5;
			}
			else if (currentPoint == 8)
			{
				racer_spin = 167.6;
			}
			else if (currentPoint == 9)
			{
				racer_spin = 91.87;
			}
			else if (currentPoint == 9)
			{
				racer_spin = 37.5;
			}
			
			*/


			racer_x -= racer_speed * sinf(racer_spin / 180 * 3.1415);
			racer_y += racer_speed * cosf(racer_spin / 180 * 3.1415);

		}
		else
		{
			

			if (currentPoint < 11)
			{
				currentPoint++;
				
				
				
			}
			else if (currentPoint == 11)
			{
				currentPoint = 0;
			}
			cout << "Racer on point: " << currentPoint << endl;

			
			cout << racer_spin << endl;
		}
	}
	
		else
			racer_spin=0;
	
	frame_count_racer++;
	if (frame_count_racer % 25 == 0)
	{
		swapTextureRacer();
	}

	if (collision())
	{
		racer_spin += .7;
	}

	
	

}
void drawBall(double x, double y)//50 by 50 with center (x,y)
{
	glBegin(GL_POLYGON);
	glTexCoord2f(1.0, 1.0); glVertex2f(25 + x, 25 + y);
	glTexCoord2f(0.0, 1.0); glVertex2f(-25 + x, 25 + y);
	glTexCoord2f(0.0, 0.0); glVertex2f(-25 + x, -25 + y);
	glTexCoord2f(1.0, 0.0); glVertex2f(25 + x, -25 + y);
	glEnd();
}

void drawThing(double x, double y)
{
	glBegin(GL_POLYGON);
	glTexCoord2f(1.0, 1.0); glVertex2f(15 + x, 15 + y);
	glTexCoord2f(0.0, 1.0); glVertex2f(-15 + x, 15 + y);
	glTexCoord2f(0.0, 0.0); glVertex2f(-15 + x, -15 + y);
	glTexCoord2f(1.0, 0.0); glVertex2f(15 + x, -15 + y);
	glEnd();
}

void reshape(int width, int height)		// Resize the OpenGL window
{
	screenWidth = width; screenHeight = height;           // to ensure the mouse coordinates match 
														  // we will use these values to set the coordinate system

	glViewport(0, 0, width, height);						// Reset the current viewport

	glMatrixMode(GL_PROJECTION);						// select the projection matrix stack
	glLoadIdentity();									// reset the top of the projection matrix to an identity matrix

	gluOrtho2D(0, screenWidth, 0, screenHeight);           // set the coordinate system for the window

	glMatrixMode(GL_MODELVIEW);							// Select the modelview matrix stack
	glLoadIdentity();									// Reset the top of the modelview matrix to an identity matrix
}


void processKeys()
{

	if (keys[VK_UP])
	{

		//std::cout << "player y: " << player_y << std::endl << "player x: " << player_x << endl<<"Player speed: "<<player_speed<<endl;

		frame_count++;
		if (frame_count % 100 == 0)
		{
			swapTexturePlayer();
		}
		player_x -= player_speed * sinf(spin / 180 * 3.1415);
		player_y += player_speed * cosf(spin / 180 * 3.1415);

		if (collision())
		{
			spin += .7;
		}



	}
	if (keys[VK_DOWN])
	{

		//std::cout << "player y: " << player_y << std::endl << "player x" << player_x << endl;;

		player_x += player_speed * sinf(spin / 180 * 3.1415);
		player_y -= player_speed * cosf(spin / 180 * 3.1415);

		if (collision())
		{
			spin += .2;
		}

	}
	if (keys[VK_LEFT])
	{
		//std::cout << "player_x: " << player_x << std::endl;

		spin += .1;


	}
	if (keys[VK_RIGHT])
	{
		//std::cout << "player_x: " << player_x << std::endl;

		spin -= .1;

	}

	if (keys[VK_SPACE])
	{
		start = true;
	}
}
void update()
{

	if (spin > 360)
		spin = 0;
}
/**************** END OPENGL FUNCTIONS *************************/

//WIN32 functions
LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc
void KillGLWindow();									// releases and destroys the window
bool CreateGLWindow(char* title, int width, int height); //creates the window
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);  // Win32 main function

													   //win32 global variabless
HDC			hDC = NULL;		// Private GDI Device Context
HGLRC		hRC = NULL;		// Permanent Rendering Context
HWND		hWnd = NULL;		// Holds Our Window Handle
HINSTANCE	hInstance;		// Holds The Instance Of The Application


							/******************* WIN32 FUNCTIONS ***************************/
int WINAPI WinMain(HINSTANCE	hInstance,			// Instance
	HINSTANCE	hPrevInstance,		// Previous Instance
	LPSTR		lpCmdLine,			// Command Line Parameters
	int			nCmdShow)			// Window Show State
{
	MSG		msg;									// Windows Message Structure
	bool	done = false;								// Bool Variable To Exit Loop


	AllocConsole();
	FILE *stream;
	freopen_s(&stream, "CONOUT$", "w", stdout);

	// Create Our OpenGL Window
	if (!CreateGLWindow("racing game", screenWidth, screenHeight))
	{
		return 0;									// Quit If Window Was Not Created
	}

	while (!done)									// Loop That Runs While done=FALSE
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))	// Is There A Message Waiting?
		{
			if (msg.message == WM_QUIT)				// Have We Received A Quit Message?
			{
				done = true;							// If So done=TRUE
			}
			else									// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		else										// If There Are No Messages
		{
			if (keys[VK_ESCAPE])
				done = true;

			processKeys();			//process keyboard
			if(!start)
				{
					startScreen();
					update();
					// update variables
					SwapBuffers(hDC);
				}
			if (start)
			{
				display();					// Draw The Scene
				update();
				// update variables
				SwapBuffers(hDC);
			}				// Swap Buffers (Double Buffering)
		}
	}

	// Shutdown
	KillGLWindow();									// Kill The Window
	return (int)(msg.wParam);						// Exit The Program
}

//WIN32 Processes function - useful for responding to user inputs or other events.
LRESULT CALLBACK WndProc(HWND	hWnd,			// Handle For This Window
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
		reshape(LOWORD(lParam), HIWORD(lParam));  // LoWord=Width, HiWord=Height
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
		mouse_y = screenHeight - HIWORD(lParam);
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
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void KillGLWindow()								// Properly Kill The Window
{
	if (hRC)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL, NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL, "Release Of DC And RC Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL, "Release Rendering Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		}
		hRC = NULL;										// Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd, hDC))					// Are We Able To Release The DC
	{
		MessageBox(NULL, "Release Device Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hDC = NULL;										// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL, "Could Not Release hWnd.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hWnd = NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass("OpenGL", hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(NULL, "Could Not Unregister Class.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hInstance = NULL;									// Set hInstance To NULL
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
	WindowRect.left = (long)0;			// Set Left Value To 0
	WindowRect.right = (long)width;		// Set Right Value To Requested Width
	WindowRect.top = (long)0;				// Set Top Value To 0
	WindowRect.bottom = (long)height;		// Set Bottom Value To Requested Height

	hInstance = GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc = (WNDPROC)WndProc;					// WndProc Handles Messages
	wc.cbClsExtra = 0;									// No Extra Window Data
	wc.cbWndExtra = 0;									// No Extra Window Data
	wc.hInstance = hInstance;							// Set The Instance
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground = NULL;									// No Background Required For GL
	wc.lpszMenuName = NULL;									// We Don't Want A Menu
	wc.lpszClassName = "OpenGL";								// Set The Class Name

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL, "Failed To Register The Window Class.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;											// Return FALSE
	}

	dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
	dwStyle = WS_OVERLAPPEDWINDOW;							// Windows Style

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

																	// Create The Window
	if (!(hWnd = CreateWindowEx(dwExStyle,							// Extended Style For The Window
		"OpenGL",							// Class Name
		title,								// Window Title
		dwStyle |							// Defined Window Style
		WS_CLIPSIBLINGS |					// Required Window Style
		WS_CLIPCHILDREN,					// Required Window Style
		0, 0,								// Window Position
		WindowRect.right - WindowRect.left,	// Calculate Window Width
		WindowRect.bottom - WindowRect.top,	// Calculate Window Height
		NULL,								// No Parent Window
		NULL,								// No Menu
		hInstance,							// Instance
		NULL)))								// Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Window Creation Error.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd =				// pfd Tells Windows How We Want Things To Be
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

	if (!(hDC = GetDC(hWnd)))							// Did We Get A Device Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Create A GL Device Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Find A Suitable PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if (!SetPixelFormat(hDC, PixelFormat, &pfd))		// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Set The PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if (!(hRC = wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Create A GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	if (!wglMakeCurrent(hDC, hRC))					// Try To Activate The Rendering Context
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Activate The GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}

	ShowWindow(hWnd, SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window
	reshape(width, height);					// Set Up Our Perspective GL Screen

	init();

	return true;									// Success
}



