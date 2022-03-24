#ifndef _TerrainModel
#define _TerrainModel

#include "Matrix.h"
#include <tuple>
#include <d3d11.h>
#include "DirectXTex.h"

class Model;
struct ID3D11DeviceContext;
struct ID3D11Device;

class TerrainModel : public Align16
{

private:
	Model* pModTerrain;

	const size_t pixel_width = 4;			// 4 bytes RGBA per pixel

	int TexelIndex(int side, int j, int i) const;
	int GetVertexAt(int side, int i, int j);
	uint8_t BlurredVal(const DirectX::Image* HM, int side, int i, int j);

public:
	TerrainModel(const TerrainModel&) = delete;				// Copy constructor
	TerrainModel(Model&&) = delete;						// Move constructor
	TerrainModel& operator=(const TerrainModel&) & = delete;  // Copy assignment operator
	TerrainModel& operator=(TerrainModel&&) & = delete;       // Move assignment operator
	~TerrainModel();

	TerrainModel(ID3D11Device* dev, LPCWSTR heightmapFile, float len, float maxheight, float ytrans, int RepeatU, int RepeatV);

	void Render(ID3D11DeviceContext* context);

};



#endif _TerrainModel