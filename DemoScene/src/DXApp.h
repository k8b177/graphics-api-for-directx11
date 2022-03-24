// DXApp
// Andre Berthiaume, June 2016
// Note: DX 11 SDK https://www.microsoft.com/en-us/download/details.aspx?id=6812
// Note on weird stuff with swap chain (1s and 2s) https://msdn.microsoft.com/en-us/library/windows/desktop/jj863687(v=vs.85).aspx

#ifndef _DXApp
#define _DXApp

#include <d3d11.h>
#include "d3dUtil.h"
#include "Align16.h"
#include "GameTimer.h"
#include <string>

// New includes for demo
#include "Vect.h"
#include "Matrix.h"
#include "Camera.h"

class Model;
class Texture;
class FlatPlane;
class Terrain;

class ShaderColor;
class ShaderColorLight;
class ShaderTexture;
class ShaderTextureLight;

class GraphicObject_Color;
class GraphicObject_ColorLight;
class GraphicObject_Texture;
class GraphicObject_TextureLight;

class SkyBox;


class DXApp : public Align16
{
private:	
	// Main window handle
	HWND      mhMainWnd;

	Vect BackgroundColor;

	// DX application elements
	ID3D11Device* md3dDevice;					// Connects to the graphics card
	ID3D11DeviceContext* md3dImmediateContext;	// Settings for the GPU to use
	IDXGISwapChain* mSwapChain;					// image buffers used for rendering
	ID3D11RenderTargetView* mRenderTargetView;	// Where to send rendring operations (typically: points to one of the swap buffers)
	ID3D11DepthStencilView* mpDepthStencilView; // Needed to force depth-buffer operations

	GameTimer mTimer;
	std::string mMainWndCaption;
	int mClientWidth;
	int mClientHeight;

	void InitDirect3D();
	void InitDemo();
	void UpdateScene();
	void DrawScene();
	void CalculateFrameStats();

	// Shader loading utility. SHould be moved elsewhere later...
	HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

	// Debug utility
	void ReportLiveDXObjects();

	// Demo specific additions
	Camera mCam;

	// shaders
	ShaderColor*			pShaderCol;
	ShaderColorLight*		pShaderColLight;
	ShaderTexture*			pShaderTex;
	ShaderTextureLight*		pShaderTexLight;

	// gas station
	GraphicObject_TextureLight* GraphObj_GasStation;
	Texture* pTex_GasStation;
	Model* pModel_GasStation;

	// ship
	GraphicObject_TextureLight* GraphObj_Ship;
	Texture* pTex_Ship;
	Model* pModel_Ship;
	Matrix mWorld_Ship;
	// ship spotlight
	Matrix mWorld_ShipSpotlight;
	Vect v_ShipSpotlightPos;

	// small ship
	GraphicObject_TextureLight* GraphObj_SmallShip;
	Texture* pTex_SmallShip;
	Model* pModel_SmallShip;
	Matrix mWorld_SmallShip;
	float smallShipRotSpeed = 0.008f;
	Vect v_SmallShipStartingPos;
	float SmallShipPauseTimer = 0.0f;
	bool SmallShipRotating = false;
	// small ship - spotlight
	Matrix mWorld_SmallShipSpotlight;
	Vect v_SmallShipSpotlightPos;

	// gas can
	GraphicObject_TextureLight* GraphObj_GasCan;
	Texture* pTex_GasCan;
	Model* pModel_GasCan;


	// skybox
	SkyBox* pSkyBox;
	Texture* pTex_SkyBox;

	// terrain
	Texture* pTex_Terrain;
	Terrain* pTerrain;


public:
	DXApp(HWND hwnd);
	virtual ~DXApp();
	
	void FrameTick();

	// overrides for handling mouse input.
	void OnMouseDown(WPARAM btnState, int xval, int yval);
	void OnMouseUp(WPARAM btnState, int xval, int yval);
	void OnMouseMove(WPARAM btnState, int xval, int yval);
	void OnMouseWheel(short delta);


};

#endif _DXApp