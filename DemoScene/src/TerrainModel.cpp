#include "TerrainModel.h"
#include "Model.h"
#include "d3dUtil.h"
#include "DirectXTex.h"
#include <assert.h>


TerrainModel::TerrainModel(ID3D11Device* dev, LPCWSTR heightmapFile, float len, float maxheight, float ytrans, int RepeatU, int RepeatV)
{
	DirectX::ScratchImage scrtTex;
	HRESULT hr = LoadFromTGAFile(heightmapFile, nullptr, scrtTex);
	assert(SUCCEEDED(hr));

	const DirectX::Image* hgtmap = scrtTex.GetImage(0, 0, 0);
	assert(hgtmap->height == hgtmap->width);

	size_t side = hgtmap->height;	// the image should be square

	// Creating Vertex Array
	int nverts = side * side;
	StandardVertex* pVerts = new StandardVertex[nverts];

	// Creating Triangle Array
	int ntri = (side - 1) * (side - 1) * 2;
	TriangleByIndex* pTriList = new TriangleByIndex[ntri];

	float step = 1.0f / (side - 1);
	float uStep = step * RepeatU;
	float vStep = step * RepeatV;
	float x = 0.5f;
	float z = 0.5f;
	int index = 0;
	uint8_t h_val;
	float u = 0;
	float v = 0;

	// building the vertices
	for (int i = 0; i < side; i++)
	{
		for (int j = 0; j < side; j++)
		{
			// getting the h_val value from the hgtmap
			//if ( (i == 0) || (i == side - 1) || (j == 0) || (j == side - 1)) // bad form but oh well
				h_val = hgtmap->pixels[TexelIndex(side, j, i)]; // h_val (out of 255) 
			//else 
				//h_val = BlurredVal(hgtmap, side, i, j);

			pVerts[index].set(x * len, (h_val / 255.0f * maxheight) + ytrans, z * len, u, v, 0.0f, 0.0f, 0.0f); // default normal is (0, 0, 0)
			x -= step;
			u += uStep;
			index++;
		}
		x = 0.5f;
		u = 0;
		z -= step;
		v += vStep;
	}

	// setting the field normals
	Vect v1, v2, v3, v4, v5, v6;
	Vect n1, n2, n3, n4, n5, n6, n;
	Vect pos;
	for (int i = 1; i < side - 1; i++)
	{
		for (int j = 1; j < side - 1; j++)
		{
			// saving the position of the main (center) vertex
			pos = pVerts[GetVertexAt(side, i, j)].Pos; 

			// Getting normal for Face 1
			v1 = pVerts[GetVertexAt(side, i - 1, j)].Pos - pos; // getting v1
			v2 = pVerts[GetVertexAt(side, i, j - 1)].Pos - pos; // getting v2
			n1 = v1.cross(v2); // getting n1, normal for face 1

			// Getting normal for Face 2
			v3 = pVerts[GetVertexAt(side, i + 1, j - 1)].Pos - pos; // getting v3
			n2 = v2.cross(v3); // getting n2, normal for face 2

			// Getting normal for Face 3
			v4 = pVerts[GetVertexAt(side, i + 1, j)].Pos - pos; // getting v4
			n3 = v3.cross(v4); // getting n3, normal for face 3

			// Getting normal for Face 4
			v5 = pVerts[GetVertexAt(side, i, j + 1)].Pos - pos; // getting v5
			n4 = v4.cross(v5); // getting n4, normal for face 4

			// Getting normal for Face 5
			v6 = pVerts[GetVertexAt(side, i - 1, j + 1)].Pos - pos; // getting v6
			n5 = v5.cross(v6); // getting n5, normal for face 5

			// Getting normal for Face 6
			n6 = v6.cross(v1);

			// n is the average of all 6 of the Face Normals
			n = (n1 + n2 + n3 + n4 + n5 + n6);
			n *= (1.0f / 6.0f);
			// normalize n
			n.norm();

			// setting the normal value for vertex(i, j)
			pVerts[GetVertexAt(side, i, j)].normal = Vect(n.X(), n.Y(), n.Z(), 0.0f);
		}
	}
	



	index = 0;

	// building the triangles
	int t = 0;
	for (int i = 0; i < (side - 1); i++)
	{
		for (int j = 0; j < (side - 1); j++)
		{
			pTriList[index].set(t, t + side, t + 1);
			index++;
			pTriList[index].set(t + 1, t + side, t + side + 1);
			index++;
			t++;
		}
		t++;

	}
	


	pModTerrain = new Model(dev, pVerts, nverts, pTriList, ntri);

	delete[] pVerts;
	delete[] pTriList;

}

uint8_t TerrainModel::BlurredVal(const DirectX::Image* HM, int side, int i, int j)
{
	Matrix blurMatrix = Matrix(Vect(0.0625f, 0.125f, 0.0625f), Vect(0.125, 0.25, 0.125), Vect(0.0625, 0.125, 0.0625), Vect(1, 1, 1));
	uint8_t val = 0;
	val += HM->pixels[TexelIndex(side, j - 1, i-1)] * blurMatrix.M0();
	val += HM->pixels[TexelIndex(side, j, i - 1)] * blurMatrix.M1();
	val += HM->pixels[TexelIndex(side, j + 1, i - 1)] * blurMatrix.M2();
	val += HM->pixels[TexelIndex(side, j -1, i)] * blurMatrix.M4();
	val += HM->pixels[TexelIndex(side, j, i)] * blurMatrix.M5();
	val += HM->pixels[TexelIndex(side, j + 1, i)] * blurMatrix.M6();
	val += HM->pixels[TexelIndex(side, j - 1, i + 1)] * blurMatrix.M8();
	val += HM->pixels[TexelIndex(side, j, i + 1)] * blurMatrix.M9();
	val += HM->pixels[TexelIndex(side, j + 1, i + 1)] * blurMatrix.M10();

	return val;
}

int TerrainModel::TexelIndex(int side, int j, int i) const
{
	return pixel_width * (i * side + j);
}

int TerrainModel::GetVertexAt(int side, int i, int j)
{
	return i * side + j;
}

void TerrainModel::Render(ID3D11DeviceContext* context)
{
	pModTerrain->SetToContext(context);
	pModTerrain->Render(context);
}

TerrainModel::~TerrainModel()
{
	delete pModTerrain;
}