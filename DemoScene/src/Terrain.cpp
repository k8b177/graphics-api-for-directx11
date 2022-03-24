#include "Terrain.h"

#include "TerrainModel.h"
#include "ShaderTextureLight.h"


Terrain::Terrain(ID3D11Device* dev, ShaderTextureLight* sh, Texture* tex, LPCWSTR heightmapFile, float len, float maxheight, float ytrans, int RepeatU, int RepeatV)
	:pShader(sh), pTex(tex)
{
	pModel = new TerrainModel(dev, heightmapFile, len, maxheight, ytrans, RepeatU, RepeatV);
}

Terrain::~Terrain()
{
	delete pModel;
}

void Terrain::Render()
{
	// set texture to context
	pShader->SetTextureResourceAndSampler(pTex);
	// render
	pModel->Render(pShader->GetContext());
}