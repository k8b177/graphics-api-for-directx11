#include "ShaderColorLight.h"
#include "d3dUtil.h"
#include <d3d11.h>
#include <assert.h>

ShaderColorLight::ShaderColorLight(ID3D11Device* dev)
	: ShaderBase(dev, L"../Assets/Shaders/ColorLight.hlsl")
{
	// Define the input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT numElements = ARRAYSIZE(layout);
	this->CreateInputLayout(layout, numElements);

	HRESULT hr = S_OK;

	// View Projection buffer
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(CamMatrices);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	hr = this->GetDevice()->CreateBuffer(&bd, nullptr, &mpBufferCamMatrices);
	assert(SUCCEEDED(hr));

	// light param
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Data_LightParams);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	hr = this->GetDevice()->CreateBuffer(&bd, nullptr, &mpBufferLightParams);
	assert(SUCCEEDED(hr));

	// Color buffer
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Data_WorldAndMaterial);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	hr = this->GetDevice()->CreateBuffer(&bd, nullptr, &mpBuffWordAndMaterial);
	assert(SUCCEEDED(hr));

	// Zeroing the light data
	ZeroMemory(&DirLightData, sizeof(DirLightData));
	ZeroMemory(&PointLightData, sizeof(PointLightData));
	ZeroMemory(&SpotLightData, sizeof(SpotLightData));

	PntlightCnt = POINTLIGHT_MAX;
	SpotlightCnt = SPOTLIGHT_MAX;
}

ShaderColorLight::~ShaderColorLight()
{
	ReleaseAndDeleteCOMobject(mpBuffWordAndMaterial);
	ReleaseAndDeleteCOMobject(mpBufferLightParams);
	ReleaseAndDeleteCOMobject(mpBufferCamMatrices);
}

void ShaderColorLight::SendCamMatrices(const Matrix& view, const Matrix& proj)
{
	CamMatrices	mCamMatrices;
	mCamMatrices.View = view;
	mCamMatrices.Projection = proj;

	this->GetContext()->UpdateSubresource(mpBufferCamMatrices, 0, nullptr, &mCamMatrices, 0, 0);
}

void ShaderColorLight::SetDirectionalLightParameters(const Vect& dir, const Vect& amb, const Vect& dif, const Vect& sp)
{
	DirLightData.Light.Ambient = amb;
	DirLightData.Light.Diffuse = dif;
	DirLightData.Light.Specular = sp;
	DirLightData.Direction = dir;
}

void ShaderColorLight::SetPointLightParameters(int lightID, const Vect& pos, float r, const Vect& att, const Vect& amb, const Vect& dif, const Vect& sp)
{
	assert(lightID >= 0 && lightID < SPOTLIGHT_MAX);

	PointLightData[lightID].Light.Ambient = amb;
	PointLightData[lightID].Light.Diffuse = dif;
	PointLightData[lightID].Light.Specular = sp;
	PointLightData[lightID].Position = pos;
	PointLightData[lightID].Attenuation = att;
	PointLightData[lightID].Range = r;
}

void ShaderColorLight::SetSpotLightParameters(int lightID, const Vect& pos, float r, const Vect& att, const Vect& dir, float spotExp, const Vect& amb, const Vect& dif, const Vect& sp)
{
	assert(lightID >= 0 && lightID < SPOTLIGHT_MAX);

	SpotLightData[lightID].Light.Ambient = amb;
	SpotLightData[lightID].Light.Diffuse = dif;
	SpotLightData[lightID].Light.Specular = sp;
	SpotLightData[lightID].Position = pos;
	SpotLightData[lightID].Direction = dir;
	SpotLightData[lightID].Attenuation = att;
	SpotLightData[lightID].Range = r;
	SpotLightData[lightID].SpotExp = spotExp;
}

void ShaderColorLight::SendLightParameters(const Vect& eyepos)
{
	Data_LightParams dl;
	dl.DirLight = DirLightData;
	dl.pointlightCount = PntlightCnt;
	dl.spotlightCount = SpotlightCnt;
	// copy over the point light data to the struct
	for (int i = 0; i < PntlightCnt; i++)
	{
		dl.PntLight[i] = PointLightData[i];
	}
	// copy over the spotlight data to the struct
	for (int i = 0; i < SpotlightCnt; i++)
	{
		dl.SptLight[i] = SpotLightData[i];
	}
	dl.EyePosWorld = eyepos;
	this->GetContext()->UpdateSubresource(mpBufferLightParams, 0, nullptr, &dl, 0, 0);
}

void ShaderColorLight::SendWorldAndMaterial(const Matrix& world, const Vect& amb, const Vect& dif, const Vect& sp)
{
	Data_WorldAndMaterial wm;
	wm.World = world;
	wm.WorlInv = world.getInv();
	wm.Mat.Ambient = amb;
	wm.Mat.Diffuse = dif;
	wm.Mat.Specular = sp;

	this->GetContext()->UpdateSubresource(mpBuffWordAndMaterial, 0, nullptr, &wm, 0, 0);
}

void ShaderColorLight::SetToContext(ID3D11DeviceContext* devcon)
{
	ShaderBase::SetContext(devcon);
	ShaderBase::SetToContext_VS_PS_InputLayout();

	devcon->VSSetConstantBuffers(0, 1, &mpBufferCamMatrices);
	devcon->VSSetConstantBuffers(1, 1, &mpBufferLightParams);
	devcon->VSSetConstantBuffers(2, 1, &mpBuffWordAndMaterial);

	devcon->PSSetConstantBuffers(0, 1, &mpBufferCamMatrices);
	devcon->PSSetConstantBuffers(1, 1, &mpBufferLightParams);
	devcon->PSSetConstantBuffers(2, 1, &mpBuffWordAndMaterial);
}