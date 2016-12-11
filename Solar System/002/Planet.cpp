#include <string>
#include <fstream>
#include <iostream>
#include "Planet.h"

Planet::Planet(string name, float radius, float period, char * path, float scale, float inclination, Planet * sateliteOf) {
    CurrentLocationInSystem = 0;
	Name = name;
	orbitalRadius = radius;
	orbitalPeriod = period;
	texturePath = path;
	Scale = scale;
	SateliteOf = sateliteOf;
	InclinationZ = inclination;
	InclinationY = rand() % 180;
}

void Planet::Orbit(float degree, float scaleOwnAxisRotation) {
	CurrentLocationOwnAxis += degree * scaleOwnAxisRotation;
	CurrentLocationInSystem += degree / (orbitalPeriod == 0 ? 1 : orbitalPeriod);

	CurrentLocationInSystem = CurrentLocationInSystem > 360 ? CurrentLocationInSystem - 360 : CurrentLocationInSystem;
	CurrentLocationOwnAxis = CurrentLocationOwnAxis > 360 ? CurrentLocationOwnAxis - 360 : CurrentLocationOwnAxis;
}

void Planet::Draw() {
	GLfloat position[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat light_ambient[] = { 0.3, 0.3, 0.3, 1.0 };
	const float pi = 3.1415f;
	const float degreeToRadian = 57.29577951f;
	float translationInSystem = 0.05f * (orbitalRadius * 2);

	PositionX = -cos((CurrentLocationInSystem / degreeToRadian + pi)) * translationInSystem;
	PositionY = 0;
	PositionZ = sin((CurrentLocationInSystem / degreeToRadian + pi)) * translationInSystem;

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

		GLUquadric * qobj = gluNewQuadric();
		gluQuadricTexture(qobj, GL_TRUE);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, Texture);
		glRotatef(270.0f, 1.0f, 0.0f, 0.0f);
		glPushMatrix();
			glRotated(CurrentLocationOwnAxis, 0, 0, 1);
			gluSphere(qobj, Scale/3, 30, 30);
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