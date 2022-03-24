
#ifndef _ShaderTexture
#define _ShaderTexture

#include "ShaderBase.h"
#include "Matrix.h"

struct ID3D11Buffer;
struct ID3D11Device;

class Texture;

class ShaderTexture : public ShaderBase
{
public:
	ShaderTexture(const ShaderTexture&) = delete;				 // Copy constructor
	ShaderTexture(ShaderTexture&&) = default;                    // Move constructor
	ShaderTexture& operator=(const ShaderTexture&) & = default;  // Copy assignment operator
	ShaderTexture& operator=(ShaderTexture&&) & = default;       // Move assignment operator
	~ShaderTexture();		  							         // Destructor

	ShaderTexture(ID3D11Device* device);

	virtual void SetToContext(ID3D11DeviceContext* devcon) override;

	void SetTextureResourceAndSampler(Texture* tex);
	void SendCamMatrices(const Matrix& view, const Matrix& proj);
	void ShaderTexture::SendWorld(const Matrix& world);

	void SetFogParameters(float fogStart, float fogRange, const Vect& fogColor);
	void SendFogParameters();
	void SendEyePos(Vect eyePos);

private:
	ID3D11Buffer* mpcbViewProj;

	struct ViewProjData
	{
		Matrix mView;
		Matrix mProjection;
	};

	ID3D11Buffer* mpcbWorld;

	struct WorldData
	{
		Matrix World;
		Matrix WorldInv;
		// The texture might change too, but that's done through mpTextureRV
	};

	struct FogSettings
	{
		float FogStart = 0.0f;
		float FogRange = 10000.0f;
		Vect FogColor = Vect(1, 1, 1, 1);
	};

	FogSettings FogSettings;


	struct Data_FogParams
	{
		float FogStart;
		float FogRange;
		Vect FogColor;
	};

	ID3D11Buffer* mpBufferFogParams;

	struct Data_EyePos
	{
		Vect EyePosWorld;
	};
	ID3D11Buffer* mpBufferEyePos;
};

#endif _ShaderTexture

