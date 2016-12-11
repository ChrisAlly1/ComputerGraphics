#include <string>
#include <fstream>
#include <iostream>
#include "Planet.h"

using namespace std;

Planet::Planet() {
	CurrentLocationInSystem = 0;
}

Planet::Planet(string name, double aphelion, double perihelion, double orbitalPeriod, char* texturePath, double size, double inclination, Planet * sateliteOf) {
	CurrentLocationInSystem = 0;
	Name = name;
	Aphelion = aphelion;
	Perihelion = perihelion;
	OrbitalPeriod = orbitalPeriod;
	TexturePath = texturePath;
	Size = size;
	SateliteOf = sateliteOf;
	InclinationZ = inclination;
	InclinationY = rand() % 180;
}

void Planet::Orbit(double degree, double scaleOwnAxisRotation) {
	CurrentLocationOwnAxis += degree * scaleOwnAxisRotation;
	CurrentLocationInSystem += degree / (OrbitalPeriod == 0 ? 1 : OrbitalPeriod);

	CurrentLocationInSystem = CurrentLocationInSystem > 360 ? CurrentLocationInSystem - 360 : CurrentLocationInSystem;
	CurrentLocationOwnAxis = CurrentLocationOwnAxis > 360 ? CurrentLocationOwnAxis - 360 : CurrentLocationOwnAxis;
}

void Planet::Draw() {
	GLfloat position[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat light_ambient[] = { 0.3, 0.3, 0.3, 1.0 };
	const double pi = 3.1415;
	const double degreeToRadian = 57.29577951;
	double translationInSystemX = 0.05 * (Aphelion + Perihelion);
	double translationInSystemZ = 0.05 * (Aphelion + Perihelion) / 2;
	PositionX = -cos((double)(CurrentLocationInSystem / degreeToRadian + pi)) *translationInSystemX;
	PositionY = 0;

	if (SateliteOf == NULL) {
		PositionZ = sin((double)(CurrentLocationInSystem / degreeToRadian + pi)) * translationInSystemX;
	} else {
		PositionZ = sin((double)(CurrentLocationInSystem / degreeToRadian + pi)) * translationInSystemX;
	}

	if (Name == "Sun") {
		glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
		glLightfv(GL_LIGHT0, GL_POSITION, position);
	}

	if (SateliteOf != NULL) {
		glPushMatrix();
		glTranslated(SateliteOf->PositionX, SateliteOf->PositionY, SateliteOf->PositionZ);
	}

	glPushMatrix();
		if (SateliteOf != NULL) {
			glRotated(InclinationY, 0, 1, 0);
			glRotated(InclinationZ, 1, 0, 0);
		}
		glTranslated(PositionX, PositionY, PositionZ);

		GLUquadric *qobj = gluNewQuadric();
		gluQuadricTexture(qobj, GL_TRUE);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, Texture);
		glRotatef(270.0f, 1.0f, 0.0f, 0.0f);
		glPushMatrix();
			glRotated(CurrentLocationOwnAxis, 0, 0, 1);
			gluSphere(qobj, Size/3, 30, 30);
		glPopMatrix();
		glDisable(GL_TEXTURE_2D);
		gluDeleteQuadric(qobj);

		if (Name == "Sun") {
			glEnable(GL_LIGHTING);
		}
	glPopMatrix();

	if (SateliteOf != NULL) {
		glPopMatrix();
	}
}