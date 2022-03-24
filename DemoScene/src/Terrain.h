#ifndef Terrain_
#define Terrain_

#include "Matrix.h"

class Model;
class Texture;
class ShaderTextureLight;
class TerrainModel;

struct ID3D11DeviceContext;
struct ID3D11Device;

class Terrain : public Align16
{
	friend class ShaderBase;
private:
	TerrainModel* pModel;
	Texture* pTex;
	ShaderTextureLight* pShader;

public:
	Terrain(const Terrain&) = delete;				// Copy constructor
	Terrain& operator=(const Terrain&) & = delete;  // Copy assignment operator
	Terrain& operator=(Terrain&&) & = delete;       // Move assignment operator
	~Terrain();

	Terrain(ID3D11Device* dev, ShaderTextureLight* sh, Texture* tex, LPCWSTR heightmapFile, float len, float maxheight, float ytrans, int RepeatU, int RepeatV);

	void Render();

};

#endif Terrain_
