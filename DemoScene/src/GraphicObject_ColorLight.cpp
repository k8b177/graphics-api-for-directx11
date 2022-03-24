#include "GraphicObject_ColorLight.h"
#include "Model.h"
#include "ShaderColor.h"
#include "d3dUtil.h"

#include <assert.h>

GraphicObject_ColorLight::GraphicObject_ColorLight(ShaderColorLight* shader, Model* mod)
{
	SetModel(mod);
	pShader = shader;

	World = Matrix(IDENTITY);

	DefaultMaterial.Amb = Colors::SlateGray;
	DefaultMaterial.Dif = Colors::SlateGray;
	DefaultMaterial.Sp = Vect(1, 1, 1, 100);

	int n = mod->GetMeshCount();
	MeshMaterials = new Material[n];
	for (int i = 0; i < n; i++)
		MeshMaterials[i] = DefaultMaterial;
}

GraphicObject_ColorLight::~GraphicObject_ColorLight()
{
	delete[] MeshMaterials;
}

void GraphicObject_ColorLight::SetMaterial(const Vect& amb, const Vect& dif, const Vect& sp)
{
	for (int i = 0; i < this->pModel->GetMeshCount(); i++)
	{
		MeshMaterials[i].Amb = amb;
		MeshMaterials[i].Dif = dif;
		MeshMaterials[i].Sp = sp;
	}
}

void GraphicObject_ColorLight::SetMaterial(const Vect& amb, const Vect& dif, const Vect& sp, int meshnum)
{
	assert(pModel->ValidMeshNum(meshnum));

	MeshMaterials[meshnum].Amb = amb;
	MeshMaterials[meshnum].Dif = dif;
	MeshMaterials[meshnum].Sp = sp;
}

void GraphicObject_ColorLight::SetWorld(const Matrix& m)
{
	World = m;
}

void GraphicObject_ColorLight::Render()
{
	pModel->SetToContext(pShader->GetContext());
	for (int i = 0; i < pModel->GetMeshCount(); i++)
	{
		pShader->SendWorldAndMaterial(World, MeshMaterials[i].Amb, MeshMaterials[i].Dif,
			MeshMaterials[i].Sp);
		pModel->RenderMesh(pShader->GetContext(), i);
	}
}