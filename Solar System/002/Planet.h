#include <string>
#include <glew.h>

using namespace std;

class Planet {
public:
	string Name;
	float orbitalSpeed, orbitalRadius, orbitalPeriod;
	float InclinationZ, InclinationY;
	float Scale;
	float CurrentLocationOwnAxis, CurrentLocationInSystem;
	float PositionX, PositionY, PositionZ;
	char * texturePath;
	GLuint Texture;
	Planet * SateliteOf;

	/*The perihelion is the point in the orbit of a celestial body where it is nearest to its orbital focus
	aphelion is the point in the orbit where the celestial body is farthest from its focus.
	orbital period is the time taken for a given object to make one complete orbit around another object*/
	Planet(string name, float radius, float period, char * path, float scale, float inclination, Planet * sateliteOf = NULL);

	void Orbit(float degree, float scaleOwnAxisRotation);

	void Draw();
};