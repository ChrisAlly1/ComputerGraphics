#include <string>
#include <glut.h>

using namespace std;

class Planet {
public:
	double _Aphelion;
	float ratio = 100;
	string Name;
	//Texture Texture;
	//VBO<Vector3> Vertexes;
	//VBO<Vector2> VertexesUV;
	//VBO<Vector3> VertexesNormal;
	//VBO<int> VertexesElements;
	double OrbitalSpeed;
	double Aphelion;// { get{ return _Aphelion; } set{ Translate = new Vector3((float)value / ratio, 0, 0); _Aphelion = value; } }
	double Perihelion;
	double OrbitalPeriod;
	double InclinationZ;
	double InclinationY;
	double Size;
	//Matrix4 Model;
	double DeltaTime;
	double CurrentLocationOwnAxis;
	double CurrentLocationInSystem;
	//Vector3 Translate;
	double PositionX;
	double PositionY;
	double PositionZ;
	char* TexturePath;
	GLuint Texture;
	Planet *SateliteOf;

	Planet();

	/*The perihelion is the point in the orbit of a celestial body where it is nearest to its orbital focus
	aphelion is the point in the orbit where the celestial body is farthest from its focus.
	orbital period is the time taken for a given object to make one complete orbit around another object*/
	Planet(string name, double aphelion, double perihelion, double orbitalPeriod, char* texturePath, double size, double inclination, Planet *sateliteOf = NULL);

	void Orbit(double degree, double scaleOwnAxisRotation);

	void Draw();
};