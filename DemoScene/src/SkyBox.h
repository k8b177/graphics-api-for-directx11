#ifndef SkyBox_
#define SkyBox_

#include "Matrix.h"

class Model;
class Texture;
class ShaderTexture;

struct ID3D11DeviceContext;
struct ID3D11Device;

class SkyBox : public Align16
{
	friend class ShaderTexture;
private:
	Model* pBox;
	Texture* pTex;
	ShaderTexture* pShader;

public:
	SkyBox(const SkyBox&) = delete;				// Copy constructor
	SkyBox(Model&&) = delete;						// Move constructor
	SkyBox& operator=(const SkyBox&) & = delete;  // Copy assignment operator
	SkyBox& operator=(SkyBox&&) & = delete;       // Move assignment operator
	~SkyBox();

	SkyBox(ID3D11Device* dev, ShaderTexture* sh, Texture* tex, float size);

	void Render();

};

#endif SkyBox_
