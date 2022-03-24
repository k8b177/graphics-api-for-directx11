#include "GraphicObject_Texture.h"
#include "Model.h"
#include "ShaderTexture.h"
#include "Texture.h"
#include <assert.h>

GraphicObject_Texture::GraphicObject_Texture(ShaderTexture* shader, Model* mod)
{
	SetModel(mod);
	pShader = shader;

	World = Matrix(IDENTITY);

	int n = mod->GetMeshCount();
	DefaultTexture = new Texture(mod->GetMDevice(), L"../Assets/Textures/blackPixel.tga");

	MeshTextures = new Texture*[n];
	for (int i = 0; i < n; i++)
		MeshTextures[i] = DefaultTexture;
}

GraphicObject_Texture::~GraphicObject_Texture()
{
	delete DefaultTexture;
	delete MeshTextures;
}

void GraphicObject_Texture::SetTexture(Texture* tex)
{
	for (int i = 0; i < this->pModel->GetMeshCount(); i++)
		MeshTextures[i] = tex;
}

void GraphicObject_Texture::SetTexture(Texture* tex, int meshnum)
{
	assert(pModel->ValidMeshNum(meshnum));
	MeshTextures[meshnum] = tex;
}

void GraphicObject_Texture::SetWorld(const Matrix& m)
{
	World = m;
}

void GraphicObject_Texture::Render()
{
	pModel->SetToContext(pShader->GetContext());
	pShader->SendWorld(World);

	for (int i = 0; i < pModel->GetMeshCount(); i++)
	{
		pShader->SetTextureResourceAndSampler(MeshTextures[i]);
		pModel->RenderMesh(pShader->GetContext(), i);
	}
}