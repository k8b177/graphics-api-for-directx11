#include "SkyBox.h"
#include "Model.h"
#include "d3dUtil.h"

#include "ShaderTexture.h"

SkyBox::SkyBox(ID3D11Device* dev, ShaderTexture* sh, Texture* tex, float size)
	:pShader(sh), pTex(tex)
{
	// Creating Box model
	int nverts = 24;
	StandardVertex* pVerts = new StandardVertex[nverts];
	int ntri = 12;
	TriangleByIndex* pTriList = new TriangleByIndex[ntri];

	// Forward (facing inside)
	int vind = 0;
	int tind = 0;
	pVerts[vind].set(0.5f * size, -0.5f * size, 0.5f * size, 0.25, 0.6666666666666666);
	pVerts[vind + 1].set(-0.5f * size, 0.5f * size, 0.5f * size, 0.5, 0.3333333333333333);
	pVerts[vind + 2].set(0.5f * size, 0.5f * size, 0.5f * size, 0.25, 0.3333333333333333);
	pVerts[vind + 3].set(-0.5f * size, -0.5f * size, 0.5f * size, 0.5, 0.6666666666666666);
	pTriList[tind].set(vind, vind + 1, vind + 2);
	pTriList[tind + 1].set(vind, vind + 3, vind + 1);

	// Back (facing inside)
	vind += 4;
	tind += 2;
	pVerts[vind].set(0.5f * size, 0.5f * size, -0.5f * size, 1, 0.3333333333333333);
	pVerts[vind + 1].set(-0.5f * size, 0.5f * size, -0.5f * size, 0.75, 0.3333333333333333);
	pVerts[vind + 2].set(-0.5f * size, -0.5f * size, -0.5f * size, 0.75, 0.6666666666666666);
	pVerts[vind + 3].set(0.5f * size, -0.5f * size, -0.5f * size, 1, 0.6666666666666666);
	pTriList[tind].set(vind + 2, vind, vind + 1);
	pTriList[tind + 1].set(vind + 2, vind + 3, vind);

	// Left (facing inside)
	vind += 4;
	tind += 2;
	pVerts[vind].set(0.5f * size, 0.5f * size, -0.5f * size, 0, 0.3333333333333333);
	pVerts[vind + 1].set(0.5f * size, 0.5f * size, 0.5f * size, 0.25, 0.3333333333333333);
	pVerts[vind + 2].set(0.5f * size, -0.5f * size, 0.5f * size, 0.25, 0.6666666666666666);
	pVerts[vind + 3].set(0.5f * size, -0.5f * size, -0.5f * size, 0, 0.6666666666666666);
	pTriList[tind].set(vind + 2, vind + 1, vind);
	pTriList[tind + 1].set(vind + 2, vind, vind + 3);
	

	// Right
	vind += 4;
	tind += 2;
	pVerts[vind].set(-0.5f * size, 0.5f * size, 0.5f * size, 0.5, 0.3333333333333333);
	pVerts[vind + 1].set(-0.5f * size, 0.5f * size, -0.5f * size, 0.75, 0.3333333333333333);
	pVerts[vind + 2].set(-0.5f * size, -0.5f * size, -0.5f * size, 0.75, 0.6666666666666666);
	pVerts[vind + 3].set(-0.5f * size, -0.5f * size, 0.5f * size, 0.5, 0.6666666666666666);
	pTriList[tind].set(vind + 2, vind + 1, vind);
	pTriList[tind + 1].set(vind + 2, vind, vind + 3);

	// Top
	vind += 4;
	tind += 2;
	pVerts[vind].set(0.5f * size, 0.5f * size, -0.5f * size, 0.25, 0);
	pVerts[vind + 1].set(-0.5f * size, 0.5f * size, -0.5f * size, 0.5, 0);
	pVerts[vind + 2].set(-0.5f * size, 0.5f * size, 0.5f * size, 0.5, 0.3333333333333333);
	pVerts[vind + 3].set(0.5f * size, 0.5f * size, 0.5f * size, 0.25, 0.3333333333333333);
	pTriList[tind].set(vind + 2, vind + 1, vind);
	pTriList[tind + 1].set(vind + 2, vind, vind + 3);

	// Bottom
	vind += 4;
	tind += 2;
	pVerts[vind].set(0.5f * size, -0.5f * size, 0.5f * size, 0.25, 0.6666666666666666);
	pVerts[vind + 1].set(-0.5f * size, -0.5f * size, 0.5f * size, 0.5, 0.6666666666666666);
	pVerts[vind + 2].set(-0.5f * size, -0.5f * size, -0.5f * size, 0.5, 1);
	pVerts[vind + 3].set(0.5f * size, -0.5f * size, -0.5f * size, 0.25, 1);
	pTriList[tind].set(vind + 2, vind + 1, vind);
	pTriList[tind + 1].set(vind + 2, vind, vind + 3);


	pBox = new Model(dev, pVerts, nverts, pTriList, ntri);

	delete[] pVerts;
	delete[] pTriList;
}

SkyBox::~SkyBox()
{
	delete pBox;
}

void SkyBox::Render()
{
	// set model to context
	pBox->SetToContext(pShader->GetContext());
	// send world matrix
	pShader->SendWorld(Matrix(IDENTITY));
	// set texture to context
	pShader->SetTextureResourceAndSampler(pTex);
	// render
	pBox->Render(pShader->GetContext());
}