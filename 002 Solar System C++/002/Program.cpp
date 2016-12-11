#define _CRT_SECURE_NO_DEPRECATE
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <ctime>
#include "Planet.h"
#include "List.h"
#include "windows.h"

using namespace std;

Planet * planetHover;
List<Planet*> * planets;

float stopWatch = 0.0f, zoom = 20.0f;
float dragX = -1, dragY = -1;
float mouseX, mouseY = 90;
const float halfpi = 3.1415f / 180;

void Orbit() {
	long newStopWatch = clock();

	for (int i = 0; i < planets->Length(); i++) {
		planets->Get(i)->Orbit(0.1f * ((newStopWatch - stopWatch) / 20), 30);
	}

	stopWatch = newStopWatch;
	glutPostRedisplay();
}

void Draw(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	float x, y, z;
	x = sin((-mouseX + dragX) * halfpi) * cos((mouseY - dragY) * halfpi) * zoom;
	y = sin(( mouseY - dragY) * halfpi) * zoom;
	z = cos((-mouseX + dragX) * halfpi) * cos((mouseY - dragY) * halfpi) * zoom;

	gluLookAt(planetHover->PositionX + x, planetHover->PositionY + y, planetHover->PositionZ + z,
		planetHover->PositionX, planetHover->PositionY, planetHover->PositionZ, 0.0, 1.0, 0.0);

	glPushMatrix();
	for (int i = 0; i < planets->Length(); i++) {
		planets->Get(i)->Draw();
	}
	glPopMatrix();

	glutSwapBuffers();
}

void ReshapeWindow(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0, w / h, 1.0, 1500.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void JumpToPlanet(int index) {
	Planet * planet;
	planet = planets->Get(index);
	planetHover = planet;
}

void KeyboardPressed(unsigned char key, int A, int B) {
	switch (key) {
		case 'w': zoom--; break;
		case 's': zoom++; break;
		case '0': JumpToPlanet(0); break;
		case '1': JumpToPlanet(1); break;
		case '2': JumpToPlanet(2); break;
		case '3': JumpToPlanet(3); break;
		case '4': JumpToPlanet(4); break;
		case 27: exit(0); break;
		default:break;
	}
}

unsigned char * LoadBmp(char *fn, int *wi, int *hi) {
	BITMAPFILEHEADER bmfh;
	BITMAPINFOHEADER bmih;
	WORD bits;
	FILE * t24;
	unsigned char * lpBitmapBits;
	long imagesize, nc;

	// read the bitmap
	t24 = fopen((char *)fn, "rb");
	if (t24 == NULL) { printf("Could not open input file\n"); exit(0); }
	fread((char *)&bmfh, sizeof(BITMAPFILEHEADER), 1, t24);
	fread((char *)&bmih, sizeof(BITMAPINFOHEADER), 1, t24);
	if (bmih.biClrUsed != 0) {
		nc = bmih.biClrUsed;
	} else {
		bits = bmih.biBitCount;
		switch (bits) {
		case 1:    nc = 2;   break;
		case 4:    nc = 16;  break;
		case 8:    nc = 256; break;
		default:   nc = 0;   break;
		}
	}

	if (nc > 0) {
		printf("Cannot handle paletted image\n"); exit(0); 
	}
	imagesize = bmfh.bfSize - bmfh.bfOffBits;
	if ((lpBitmapBits = (unsigned char *)malloc(imagesize)) == NULL) { 
		fclose(t24); exit(0); 
	}
	fread((char *)lpBitmapBits, imagesize, 1, t24);
	fclose(t24);
	*wi = bmih.biWidth; *hi = bmih.biHeight;
	return lpBitmapBits;
}

void AddTexture() {
	unsigned char * pix;
	int w, h;

	for (int i = 0; i < planets->Length(); i++) {
		glEnable(GL_TEXTURE_2D);
		pix = LoadBmp(planets->Get(i)->TexturePath, &w, &h);
		glGenTextures(1, &planets->Get(i)->Texture);
		glBindTexture(GL_TEXTURE_2D, planets->Get(i)->Texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pix);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		free(pix);
		glDisable(GL_TEXTURE_2D);
	}
}


void mouse(int button, int state, int x, int y) {
	switch (button) {
	case 0:
		if (state == 0) {
			if (dragX == -1) {
				dragX = x;
				dragY = y;
			}
			else {
				dragX += x - mouseX;
				dragY += y - mouseY;
			}
			mouseX = x;
			mouseY = y;
		}
		break;
	case 1:
		break;
	default:
		break;
	}
}

void mouseMove(int x, int y) {
	mouseX = x;
	mouseY = y;
}

void PrintMenu(unsigned char key) {
	system("CLS");
	for (int i = 0; i < 4; i++)
		cout << "Press " << i << " to see " << planets->Get(i)->Name << endl;
	cout << "Press W to zoom in" << endl;
	cout << "Press S to zoom in" << endl;
	cout << "Hold left click and drag to move camera around" << endl;
	cout << "--------------------------------------------" << endl;
}

void InitialiseGlut() {
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Solar System");

	glutReshapeFunc(ReshapeWindow);
	glutDisplayFunc(Draw);
	glutKeyboardFunc(KeyboardPressed);
	glutMotionFunc(mouseMove);
	glutMouseFunc(mouse);

	glEnable(GL_DEPTH_TEST);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glutIdleFunc(Orbit);
	AddTexture();
	glutMainLoop();
}

void InitialisePlanets() {
	planets = new List<Planet*>();
	planets->Add(new Planet("Sun",         0,    0,   0    ,"Sun.bmp"    , 109/30,  1.0));
	planets->Add(new Planet("Mercury",    50,   50,   0.240,"Mercury.bmp", 0.38,    1.0));
	planets->Add(new Planet("Venus",     100,  100,   0.615,"Venus.bmp"  , 0.94,    1.0));
	planets->Add(new Planet("Earth",     160,  160,   1    ,"Earth.bmp"  , 1,       1.0));
	planets->Add(new Planet("Mars",      220,  220,   1.880,"Mars.bmp"   , 0.53,    1.0));
	planets->Add(new Planet("TheMoon",  10,   10,   0.07 ,"Pluto.bmp"  , 0.18,   15.0, planets->Get(3)));
	planets->Add(new Planet("Phobos", 10, 10, 0.09 - (float)(rand() % 60) / 1000, "Pluto.bmp", 0.1, rand() % 180, planets->Get(4)));
	planets->Add(new Planet("Deimos", 10, 10, 0.09 - (float)(rand() % 60) / 1000, "Pluto.bmp", 0.1, rand() % 180, planets->Get(4)));

	planetHover = planets->Get(0);
}

int main(int argc, char **argv) {
	InitialisePlanets();
	PrintMenu('0');
	glutInit(&argc, argv);
	InitialiseGlut();
	return 0;
}