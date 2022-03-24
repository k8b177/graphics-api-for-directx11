#include "ShaderTexture.h"
#include "d3dUtil.h"
#include <d3d11.h>
#include <assert.h>

#include "Texture.h"

ShaderTexture::ShaderTexture(ID3D11Device* device)
	: ShaderBase(device, L"../Assets/Shaders/Texture.hlsl")
{
	// Define the input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE(layout);
	this->CreateInputLayout(layout, numElements);

	HRESULT hr;

	// View Projection buffer
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ViewProjData);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	hr = this->GetDevice()->CreateBuffer(&bd, nullptr, &mpcbViewProj);
	assert(SUCCEEDED(hr));

	// Color buffer
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WorldData);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	hr = this->GetDevice()->CreateBuffer(&bd, nullptr, &mpcbWorld);
	assert(SUCCEEDED(hr));

	// Fog data buffer
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Data_FogParams);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	hr = this->GetDevice()->CreateBuffer(&bd, nullptr, &mpBufferFogParams);
	assert(SUCCEEDED(hr));

	
	// EyePosWorld data buffer
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Data_EyePos);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	hr = this->GetDevice()->CreateBuffer(&bd, nullptr, &mpBufferEyePos);
	assert(SUCCEEDED(hr));

}

ShaderTexture::~ShaderTexture()
{
	ReleaseAndDeleteCOMobject(mpcbWorld);
	ReleaseAndDeleteCOMobject(mpcbViewProj);
	ReleaseAndDeleteCOMobject(mpBufferFogParams);
	ReleaseAndDeleteCOMobject(mpBufferEyePos);
}

void ShaderTexture::SetTextureResourceAndSampler(Texture* tex)
{
	tex->SetToContext(this->GetContext());
}

void ShaderTexture::SendCamMatrices(const Matrix& view, const Matrix& proj)
{
	ViewProjData	mCamMatrices;
	mCamMatrices.mView = view;
	mCamMatrices.mProjection = proj;

	this->GetContext()->UpdateSubresource(mpcbViewProj, 0, nullptr, &mCamMatrices, 0, 0);
}

void ShaderTexture::SendWorld(const Matrix& world)
{
	WorldData w;
	w.World = world;
	w.WorldInv = world.getInv();

	this->GetContext()->UpdateSubresource(mpcbWorld, 0, nullptr, &w, 0, 0);
}

void ShaderTexture::SetFogParameters(float fogStart, float fogRange, const Vect& fogColor)
{
	FogSettings.FogStart = fogStart;
	FogSettings.FogRange = fogRange;
	FogSettings.FogColor = fogColor;
}

void ShaderTexture::SendFogParameters()
{
	Data_FogParams df;
	df.FogColor = FogSettings.FogColor;
	df.FogStart = FogSettings.FogStart;
	df.FogRange = FogSettings.FogRange;

	this->GetContext()->UpdateSubresource(mpBufferFogParams, 0, nullptr, &df, 0, 0);
}

void ShaderTexture::SendEyePos(Vect eyePos)
{
	Data_EyePos de;
	de.EyePosWorld = eyePos;

	this->GetContext()->UpdateSubresource(mpBufferEyePos, 0, nullptr, &de, 0, 0);
}

void ShaderTexture::SetToContext(ID3D11DeviceContext* devcon)
{
	ShaderBase::SetContext(devcon);
	ShaderBase::SetToContext_VS_PS_InputLayout();

	SendFogParameters();

	devcon->VSSetConstantBuffers(0, 1, &mpcbViewProj);
	devcon->VSSetConstantBuffers(1, 1, &mpcbWorld);
	devcon->VSSetConstantBuffers(2, 1, &mpBufferFogParams);
	devcon->VSSetConstantBuffers(3, 1, &mpBufferEyePos);

	devcon->PSSetConstantBuffers(0, 1, &mpcbViewProj);
	devcon->PSSetConstantBuffers(1, 1, &mpcbWorld);
	devcon->PSSetConstantBuffers(2, 1, &mpBufferFogParams);
	devcon->PSSetConstantBuffers(3, 1, &mpBufferEyePos);
}