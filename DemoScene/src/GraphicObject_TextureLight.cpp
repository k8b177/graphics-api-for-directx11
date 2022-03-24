#include "GraphicObject_TextureLight.h"
#include "Model.h"
#include "ShaderTexture.h"
#include "Texture.h"
#include "d3dUtil.h"
#include <assert.h>

GraphicObject_TextureLight::GraphicObject_TextureLight(ShaderTextureLight* shader, Model* mod)
{
	SetModel(mod);
	pShader = shader;

	World = Matrix(IDENTITY);

	// Textures
	int n = mod->GetMeshCount();
	DefaultTexture = new Texture(mod->GetMDevice(), L"../Assets/Textures/blackPixel.tga");

	MeshTextures = new Texture * [n];
	for (int i = 0; i < n; i++)
		MeshTextures[i] = DefaultTexture;

	// Materials
	DefaultMaterial.Amb = Colors::White;
	DefaultMaterial.Dif = Colors::White;
	DefaultMaterial.Sp = Vect(1, 1, 1, 100);

	MeshMaterials = new Material[n];
	for (int i = 0; i < n; i++)
		MeshMaterials[i] = DefaultMaterial;
}

GraphicObject_TextureLight::~GraphicObject_TextureLight()
{
	delete DefaultTexture;
	delete[] MeshTextures;

	delete[] MeshMaterials;
}

void GraphicObject_TextureLight::SetTexture(Texture* tex)
{
	for (int i = 0; i < this->pModel->GetMeshCount(); i++)
		MeshTextures[i] = tex;
}

void GraphicObject_TextureLight::SetTexture(Texture* tex, int meshnum)
{
	assert(pModel->ValidMeshNum(meshnum));
	MeshTextures[meshnum] = tex;
}

void GraphicObject_TextureLight::SetMaterial(const Vect& amb, const Vect& dif, const Vect& sp)
{
	for (int i = 0; i < this->pModel->GetMeshCount(); i++)
	{
		MeshMaterials[i].Amb = amb;
		MeshMaterials[i].Dif = dif;
		MeshMaterials[i].Sp = sp;
	}
}

void GraphicObject_TextureLight::SetMaterial(const Vect& amb, const Vect& dif, const Vect& sp, int meshnum)
{
	assert(pModel->ValidMeshNum(meshnum));

	MeshMaterials[meshnum].Amb = amb;
	MeshMaterials[meshnum].Dif = dif;
	MeshMaterials[meshnum].Sp = sp;
}

void GraphicObject_TextureLight::SetWorld(const Matrix& m)
{
	World = m;
}

void GraphicObject_TextureLight::Render()
{
	pModel->SetToContext(pShader->GetContext());

	for (int i = 0; i < pModel->GetMeshCount(); i++)
	{
		pShader->SetTextureResourceAndSampler(MeshTextures[i]);
		pShader->SendWorldAndMaterial(World, MeshMaterials[i].Amb, MeshMaterials[i].Dif,
			MeshMaterials[i].Sp);
		pModel->RenderMesh(pShader->GetContext(), i);
	}
}