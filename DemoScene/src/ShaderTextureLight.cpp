#include "ShaderTextureLight.h"
#include "d3dUtil.h"
#include <d3d11.h>
#include <assert.h>

#include "Texture.h"

ShaderTextureLight::ShaderTextureLight(ID3D11Device* dev)
	: ShaderBase(dev, L"../Assets/Shaders/TextureLight.hlsl")
{
	// Define the input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0 },
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

	// Light param buffer
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Data_LightParams);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	hr = this->GetDevice()->CreateBuffer(&bd, nullptr, &mpBufferLightParams);
	assert(SUCCEEDED(hr));

	// World and Material buffer
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


	PointlightCnt = POINTLIGHT_MAX;
	SpotlightCnt = SPOTLIGHT_MAX;


	// Fog data buffer
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Data_FogParams);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	hr = this->GetDevice()->CreateBuffer(&bd, nullptr, &mpBufferFogParams);
	assert(SUCCEEDED(hr));
	// initialize fog data buffer to default values (no fog effect)
}

ShaderTextureLight::~ShaderTextureLight()
{
	ReleaseAndDeleteCOMobject(mpBuffWordAndMaterial);
	ReleaseAndDeleteCOMobject(mpBufferLightParams);
	ReleaseAndDeleteCOMobject(mpBufferCamMatrices);
	ReleaseAndDeleteCOMobject(mpBufferFogParams);
}

void ShaderTextureLight::SendCamMatrices(const Matrix& view, const Matrix& proj)
{
	CamMatrices	mCamMatrices;
	mCamMatrices.View = view;
	mCamMatrices.Projection = proj;

	this->GetContext()->UpdateSubresource(mpBufferCamMatrices, 0, nullptr, &mCamMatrices, 0, 0);
}

void ShaderTextureLight::SetDirectionalLightParameters(const Vect& dir, const Vect& amb, const Vect& dif, const Vect& sp)
{
	DirLightData.Light.Ambient = amb;
	DirLightData.Light.Diffuse = dif;
	DirLightData.Light.Specular = sp;
	DirLightData.Direction = dir;
}

void ShaderTextureLight::SetPointLightParameters(int lightID, const Vect& pos, float r, const Vect& att, const Vect& amb, const Vect& dif, const Vect& sp)
{
	assert(lightID >= 0 && lightID < SPOTLIGHT_MAX);

	PointLightData[lightID].Light.Ambient = amb;
	PointLightData[lightID].Light.Diffuse = dif;
	PointLightData[lightID].Light.Specular = sp;
	PointLightData[lightID].Position = pos;
	PointLightData[lightID].Attenuation = att;
	PointLightData[lightID].Range = r;
}

void ShaderTextureLight::SetSpotLightParameters(int lightID, const Vect& pos, float r, const Vect& att, const Vect& dir, float spotExp, const Vect& amb, const Vect& dif, const Vect& sp)
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

void ShaderTextureLight::SendLightParameters(const Vect& eyepos)
{
	Data_LightParams dl;
	dl.pointlightCount = PointlightCnt;
	dl.spotlightCount = SpotlightCnt;
	dl.DirLight = DirLightData;
	// copy over the point light data to the struct
	for (int i = 0; i < PointlightCnt; i++)
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

void ShaderTextureLight::SendWorldAndMaterial(const Matrix& world, const Vect& amb, const Vect& dif, const Vect& sp)
{
	Data_WorldAndMaterial wm;
	wm.World = world;
	wm.WorlInv = world.getInv();
	wm.Mat.Ambient = amb;
	wm.Mat.Diffuse = dif;
	wm.Mat.Specular = sp;

	this->GetContext()->UpdateSubresource(mpBuffWordAndMaterial, 0, nullptr, &wm, 0, 0);
}

void ShaderTextureLight::SetFogParameters(float fogStart, float fogRange, const Vect& fogColor)
{
	FogSettings.FogStart = fogStart;
	FogSettings.FogRange = fogRange;
	FogSettings.FogColor = fogColor;
}

void ShaderTextureLight::SendFogParameters()
{
	Data_FogParams df;
	df.FogColor = FogSettings.FogColor;
	df.FogStart = FogSettings.FogStart;
	df.FogRange = FogSettings.FogRange;

	this->GetContext()->UpdateSubresource(mpBufferFogParams, 0, nullptr, &df, 0, 0);
}

void ShaderTextureLight::SetToContext(ID3D11DeviceContext* devcon)
{
	ShaderBase::SetContext(devcon);
	ShaderBase::SetToContext_VS_PS_InputLayout();

	SendFogParameters();

	devcon->VSSetConstantBuffers(0, 1, &mpBufferCamMatrices);
	devcon->VSSetConstantBuffers(1, 1, &mpBufferLightParams);
	devcon->VSSetConstantBuffers(2, 1, &mpBuffWordAndMaterial);
	devcon->VSSetConstantBuffers(3, 1, &mpBufferFogParams);

	devcon->PSSetConstantBuffers(0, 1, &mpBufferCamMatrices);
	devcon->PSSetConstantBuffers(1, 1, &mpBufferLightParams);
	devcon->PSSetConstantBuffers(2, 1, &mpBuffWordAndMaterial);
	devcon->PSSetConstantBuffers(3, 1, &mpBufferFogParams);
}

void ShaderTextureLight::SetTextureResourceAndSampler(Texture* tex)
{
	tex->SetToContext(this->GetContext());
}