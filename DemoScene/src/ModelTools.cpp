#include "ModelTools.h"
//#include "Enum.h"
//#include <math.h>
#include "Matrix.h"
#include <assert.h>
#include "Model.h"
#include "d3dUtil.h"


/// Creates the unit box centered at the origin
void ModelTools::CreateUnitBox(StandardVertex*& pVerts, int& nverts, TriangleByIndex*& pTriList, int& ntri)
{
	nverts = 24;
	pVerts = new StandardVertex[nverts];
	ntri = 12;
	pTriList = new TriangleByIndex[ntri];


	// Setting up faces
	// Forward
	int vind = 0;
	int tind = 0;
	Vect n = Vect(0, 0, 1);
	pVerts[vind].set(0.5f, 0.5f, 0.5f, n, Colors::White);
	pVerts[vind + 1].set(-0.5f, 0.5f, 0.5f, n, Colors::Cyan);
	pVerts[vind + 2].set(-0.5f, -0.5f, 0.5f, n, Colors::Blue);
	pVerts[vind + 3].set(0.5f, -0.5f, 0.5f, n, Colors::Magenta);
	pTriList[tind].set(vind, vind + 1, vind + 2);
	pTriList[tind + 1].set(vind, vind + 2, vind + 3);

	// Back
	vind += 4;
	tind += 2;
	n = Vect(0, 0, -1, 0);
	pVerts[vind].set(0.5f, 0.5f, -0.5f, n, Colors::Yellow);
	pVerts[vind + 1].set(-0.5f, 0.5f, -0.5f, n, Colors::Lime);
	pVerts[vind + 2].set(-0.5f, -0.5f, -0.5f, n, Colors::Black);
	pVerts[vind + 3].set(0.5f, -0.5f, -0.5f, n, Colors::Red);
	pTriList[tind].set(vind + 2, vind + 1, vind);
	pTriList[tind + 1].set(vind + 3, vind + 2, vind);

	// Left
	vind += 4;
	tind += 2;
	n = Vect(1, 0, 0, 0);
	pVerts[vind].set(0.5f, 0.5f, -0.5f, n, Colors::Yellow);
	pVerts[vind + 1].set(0.5f, 0.5f, 0.5f, n, Colors::White);
	pVerts[vind + 2].set(0.5f, -0.5f, 0.5f, n, Colors::Magenta);
	pVerts[vind + 3].set(0.5f, -0.5f, -0.5f, n, Colors::Red);
	pTriList[tind].set(vind, vind + 1, vind + 2);
	pTriList[tind + 1].set(vind, vind + 2, vind + 3);

	// Right
	vind += 4;
	tind += 2;
	n = Vect(-1, 0, 0, 0);
	pVerts[vind].set(-0.5f, 0.5f, 0.5f, n, Colors::Cyan);
	pVerts[vind + 1].set(-0.5f, 0.5f, -0.5f, n, Colors::Lime);
	pVerts[vind + 2].set(-0.5f, -0.5f, -0.5f, n, Colors::Black);
	pVerts[vind + 3].set(-0.5f, -0.5f, 0.5f, n, Colors::Blue);
	pTriList[tind].set(vind, vind + 1, vind + 2);
	pTriList[tind + 1].set(vind, vind + 2, vind + 3);

	// Top
	vind += 4;
	tind += 2;
	n = Vect(0, 1, 0, 0);
	pVerts[vind].set(0.5f, 0.5f, -0.5f, n, Colors::Yellow);
	pVerts[vind + 1].set(-0.5f, 0.5f, -0.5f, n, Colors::Lime);
	pVerts[vind + 2].set(-0.5f, 0.5f, 0.5f, n, Colors::Cyan);
	pVerts[vind + 3].set(0.5f, 0.5f, 0.5f, n, Colors::White);
	pTriList[tind].set(vind, vind + 1, vind + 2);
	pTriList[tind + 1].set(vind, vind + 2, vind + 3);

	// Bottom
	vind += 4;
	tind += 2;
	n = Vect(0, -1, 0, 0);
	pVerts[vind].set(0.5f, -0.5f, 0.5f, n, Colors::Magenta);
	pVerts[vind + 1].set(-0.5f, -0.5f, 0.5f, n, Colors::Blue);
	pVerts[vind + 2].set(-0.5f, -0.5f, -0.5f, n, Colors::Black);
	pVerts[vind + 3].set(0.5f, -0.5f, -0.5f, n, Colors::Red);
	pTriList[tind].set(vind, vind + 1, vind + 2);
	pTriList[tind + 1].set(vind, vind + 2, vind + 3);
}

void ModelTools::CreateUnitBoxRepTexture(StandardVertex*& pVerts, int& nverts, TriangleByIndex*& pTriList, int& ntri)
{
	nverts = 24;
	pVerts = new StandardVertex[nverts];
	ntri = 12;
	pTriList = new TriangleByIndex[ntri];


	// Setting up faces
	// Forward
	int vind = 0;
	int tind = 0;
	pVerts[vind].set(0.5f, 0.5f, 0.5f, 1, 0, 0, 0, 1);
	pVerts[vind + 1].set(-0.5f, 0.5f, 0.5f, 0, 0, 0, 0, 1);
	pVerts[vind + 2].set(-0.5f, -0.5f, 0.5f, 0, 1, 0, 0, 1);
	pVerts[vind + 3].set(0.5f, -0.5f, 0.5f, 1, 1, 0, 0, 1);
	pTriList[tind].set(vind, vind + 1, vind + 2);
	pTriList[tind + 1].set(vind, vind + 2, vind + 3);

	// Back
	vind += 4;
	tind += 2;
	pVerts[vind].set(0.5f, 0.5f, -0.5f, 0, 0, 0, 0, -1);
	pVerts[vind + 1].set(-0.5f, 0.5f, -0.5f, 1, 0, 0, 0, -1);
	pVerts[vind + 2].set(-0.5f, -0.5f, -0.5f, 1, 1, 0, 0, -1);
	pVerts[vind + 3].set(0.5f, -0.5f, -0.5f, 0, 1, 0, 0, -1);
	pTriList[tind].set(vind + 2, vind + 1, vind);
	pTriList[tind + 1].set(vind + 3, vind + 2, vind);

	// Left
	vind += 4;
	tind += 2;
	pVerts[vind].set(0.5f, 0.5f, -0.5f, 1, 0, 1, 0, 0);
	pVerts[vind + 1].set(0.5f, 0.5f, 0.5f, 0, 0, 1, 0, 0);
	pVerts[vind + 2].set(0.5f, -0.5f, 0.5f, 0, 1, 1, 0, 0);
	pVerts[vind + 3].set(0.5f, -0.5f, -0.5f, 1, 1, 1, 0, 0);
	pTriList[tind].set(vind, vind + 1, vind + 2);
	pTriList[tind + 1].set(vind, vind + 2, vind + 3);

	// Right
	vind += 4;
	tind += 2;
	pVerts[vind].set(-0.5f, 0.5f, 0.5f, 1, 0, -1, 0, 0);
	pVerts[vind + 1].set(-0.5f, 0.5f, -0.5f, 0, 0, -1, 0, 0);
	pVerts[vind + 2].set(-0.5f, -0.5f, -0.5f, 0, 1, -1, 0, 0);
	pVerts[vind + 3].set(-0.5f, -0.5f, 0.5f, 1, 1, -1, 0, 0);
	pTriList[tind].set(vind, vind + 1, vind + 2);
	pTriList[tind + 1].set(vind, vind + 2, vind + 3);

	// Top
	vind += 4;
	tind += 2;
	pVerts[vind].set(0.5f, 0.5f, -0.5f, 1, 0, 0, 1, 0);
	pVerts[vind + 1].set(-0.5f, 0.5f, -0.5f, 0, 0, 0, 1, 0);
	pVerts[vind + 2].set(-0.5f, 0.5f, 0.5f, 0, 1, 0, 1, 0);
	pVerts[vind + 3].set(0.5f, 0.5f, 0.5f, 1, 1, 0, 1, 0);
	pTriList[tind].set(vind, vind + 1, vind + 2);
	pTriList[tind + 1].set(vind, vind + 2, vind + 3);

	// Bottom
	vind += 4;
	tind += 2;
	pVerts[vind].set(0.5f, -0.5f, 0.5f, 1, 0, 0, -1, 0);
	pVerts[vind + 1].set(-0.5f, -0.5f, 0.5f, 0, 0, 0, -1, 0);
	pVerts[vind + 2].set(-0.5f, -0.5f, -0.5f, 0, 1, 0, -1, 0);
	pVerts[vind + 3].set(0.5f, -0.5f, -0.5f, 1, 1, 0, -1, 0);
	pTriList[tind].set(vind, vind + 1, vind + 2);
	pTriList[tind + 1].set(vind, vind + 2, vind + 3);
}

void ModelTools::CreateUnitBoxSixFaceTexture(StandardVertex*& pVerts, int& nverts, TriangleByIndex*& pTriList, int& ntri)
{
	nverts = 24;
	pVerts = new StandardVertex[nverts];
	ntri = 12;
	pTriList = new TriangleByIndex[ntri];



	// Setting up faces
	// Forward
	int vind = 0;
	int tind = 0;
	pVerts[vind].set(0.5f, 0.5f, 0.5f, 0.25, 0.625, 0, 0, 1);
	pVerts[vind + 1].set(-0.5f, 0.5f, 0.5f, 0.25, 0.375, 0, 0, 1);
	pVerts[vind + 2].set(-0.5f, -0.5f, 0.5f, 0, 0.375, 0, 0, 1);
	pVerts[vind + 3].set(0.5f, -0.5f, 0.5f, 0, 0.625, 0, 0, 1);
	pTriList[tind].set(vind, vind + 1, vind + 2);
	pTriList[tind + 1].set(vind, vind + 2, vind + 3);


	// Back
	vind += 4;
	tind += 2;
	pVerts[vind].set(0.5f, 0.5f, -0.5f, 0.5, 0.625, 0, 0, -1);
	pVerts[vind + 1].set(-0.5f, 0.5f, -0.5f, 0.5, 0.375, 0, 0, -1);
	pVerts[vind + 2].set(-0.5f, -0.5f, -0.5f, 0.75, 0.3750, 0, 0, -1);
	pVerts[vind + 3].set(0.5f, -0.5f, -0.5f, 0.75, 0.625, 0, 0, -1);
	pTriList[tind].set(vind + 2, vind + 1, vind);
	pTriList[tind + 1].set(vind + 3, vind + 2, vind);


	// Left
	vind += 4;
	tind += 2;
	pVerts[vind].set(0.5f, 0.5f, -0.5f, 0.5, 0.625, 1, 0, 0);
	pVerts[vind + 1].set(0.5f, 0.5f, 0.5f, 0.25, 0.625, 1, 0, 0);
	pVerts[vind + 2].set(0.5f, -0.5f, 0.5f, 0.25, 0.875, 1, 0, 0);
	pVerts[vind + 3].set(0.5f, -0.5f, -0.5f, 0.5, 0.875, 1, 0, 0);
	pTriList[tind].set(vind, vind + 1, vind + 2);
	pTriList[tind + 1].set(vind, vind + 2, vind + 3);

	// Right
	vind += 4;
	tind += 2;
	pVerts[vind].set(-0.5f, 0.5f, 0.5f, 0.25, 0.375, -1, 0, 0);
	pVerts[vind + 1].set(-0.5f, 0.5f, -0.5f, 0.5, 0.375, -1, 0, 0);
	pVerts[vind + 2].set(-0.5f, -0.5f, -0.5f, 0.5, 0.125, -1, 0, 0);
	pVerts[vind + 3].set(-0.5f, -0.5f, 0.5f, 0.25, 0.125, -1, 0, 0);
	pTriList[tind].set(vind, vind + 1, vind + 2);
	pTriList[tind + 1].set(vind, vind + 2, vind + 3);

	// Top
	vind += 4;
	tind += 2;
	pVerts[vind].set(0.5f, 0.5f, -0.5f, 0.5, 0.625, 0, 1, 0);
	pVerts[vind + 1].set(-0.5f, 0.5f, -0.5f, 0.5, 0.375, 0, 1, 0);
	pVerts[vind + 2].set(-0.5f, 0.5f, 0.5f, 0.25, 0.375, 0, 1, 0);
	pVerts[vind + 3].set(0.5f, 0.5f, 0.5f, 0.25, 0.625, 0, 1, 0);
	pTriList[tind].set(vind, vind + 1, vind + 2);
	pTriList[tind + 1].set(vind, vind + 2, vind + 3);

	// Bottom
	vind += 4;
	tind += 2;
	pVerts[vind].set(0.5f, -0.5f, 0.5f, 1, 0.625, 0, -1, 0);
	pVerts[vind + 1].set(-0.5f, -0.5f, 0.5f, 1, 0.375, 0, -1, 0);
	pVerts[vind + 2].set(-0.5f, -0.5f, -0.5f, 0.75, 0.375, 0, -1, 0);
	pVerts[vind + 3].set(0.5f, -0.5f, -0.5f, 0.75, 0.625, 0, -1, 0);
	pTriList[tind].set(vind, vind + 1, vind + 2);
	pTriList[tind + 1].set(vind, vind + 2, vind + 3);
}

void ModelTools::CreateUnitPyramid(StandardVertex*& pVerts, int& nverts, TriangleByIndex*& pTriList, int& ntri)
{
	nverts = 16;
	pVerts = new StandardVertex[nverts];
	ntri = 6;
	pTriList = new TriangleByIndex[ntri];


	Vect v1;
	Vect v2;
	Vect n;

	// Front Face
	pVerts[0].set(0.0f, 0.5f, 0.0f, Colors::Lime);
	pVerts[1].set(+0.5f, -0.5f, -0.5f, Colors::Red);
	pVerts[2].set(-0.5f, -0.5f, -0.5f, Colors::Black);
	pTriList[0].set(0, 1, 2);

	v1 = pVerts[2].Pos - pVerts[0].Pos;
	v2 = pVerts[1].Pos - pVerts[0].Pos;
	n = v2.cross(v1);
	n.norm();

	pVerts[0].normal = Vect(n.X(), n.Y(), n.Z(), 0.0f);
	pVerts[1].normal = Vect(n.X(), n.Y(), n.Z(), 0.0f);
	pVerts[2].normal = Vect(n.X(), n.Y(), n.Z(), 0.0f);


	// Right Face
	pVerts[3].set(0.0f, 0.5f, 0.0f, Colors::Lime);
	pVerts[4].set(-0.5f, -0.5f, -0.5f, Colors::Black);
	pVerts[5].set(-0.5f, -0.5f, +0.5f, Colors::Blue);
	pTriList[1].set(3, 4, 5);

	v1 = pVerts[5].Pos - pVerts[3].Pos;
	v2 = pVerts[4].Pos - pVerts[3].Pos;
	n = v2.cross(v1);
	n.norm();

	pVerts[3].normal = Vect(n.X(), n.Y(), n.Z(), 0.0f);
	pVerts[4].normal = Vect(n.X(), n.Y(), n.Z(), 0.0f);
	pVerts[5].normal = Vect(n.X(), n.Y(), n.Z(), 0.0f);


	// Back Face
	pVerts[6].set(0.0f, 0.5f, 0.0f, Colors::Lime);
	pVerts[7].set(-0.5f, -0.5f, +0.5f, Colors::Blue);
	pVerts[8].set(+0.5f, -0.5f, +0.5f, Colors::Magenta);
	pTriList[2].set(6, 7, 8);

	v1 = pVerts[8].Pos - pVerts[6].Pos;
	v2 = pVerts[7].Pos - pVerts[6].Pos;
	n = v2.cross(v1);
	n.norm();

	pVerts[6].normal = Vect(n.X(), n.Y(), n.Z(), 0.0f);
	pVerts[7].normal = Vect(n.X(), n.Y(), n.Z(), 0.0f);
	pVerts[8].normal = Vect(n.X(), n.Y(), n.Z(), 0.0f);


	// Left Face
	pVerts[9].set(0.0f, 0.5f, 0.0f, Colors::Lime);
	pVerts[10].set(+0.5f, -0.5f, +0.5f, Colors::Magenta);
	pVerts[11].set(+0.5f, -0.5f, -0.5f, Colors::Red);
	pTriList[3].set(9, 10, 11);

	v1 = pVerts[11].Pos - pVerts[9].Pos;
	v2 = pVerts[10].Pos - pVerts[9].Pos;
	n = v2.cross(v1);
	n.norm();

	pVerts[9].normal = Vect(n.X(), n.Y(), n.Z(), 0.0f);
	pVerts[10].normal = Vect(n.X(), n.Y(), n.Z(), 0.0f);
	pVerts[11].normal = Vect(n.X(), n.Y(), n.Z(), 0.0f);


	// Bottom Faces
	n = Vect(0, -1, 0);
	pVerts[12].set(-0.5f, -0.5f, -0.5f, Colors::Black);
	pVerts[12].normal = Vect(n.X(), n.Y(), n.Z(), 0.0f);
	pVerts[13].set(+0.5f, -0.5f, -0.5f, Colors::Red);
	pVerts[13].normal = Vect(n.X(), n.Y(), n.Z(), 0.0f);
	pVerts[14].set(-0.5f, -0.5f, +0.5f, Colors::Blue);
	pVerts[14].normal = Vect(n.X(), n.Y(), n.Z(), 0.0f);
	pTriList[4].set(12, 13, 14);

	pVerts[15].set(+0.5f, -0.5f, +0.5f, Colors::Magenta);
	pVerts[15].normal = Vect(n.X(), n.Y(), n.Z(), 0.0f);
	pTriList[5].set(13, 15, 14);
}

void ModelTools::CreateUnitPyramidRepTexture(StandardVertex*& pVerts, int& nverts, TriangleByIndex*& pTriList, int& ntri)
{
	nverts = 16;
	pVerts = new StandardVertex[nverts];
	ntri = 6;
	pTriList = new TriangleByIndex[ntri];


	Vect v1;
	Vect v2;
	Vect n;

	// Front Face
	pVerts[0].set(0.0f, 0.5f, 0.0f, 0.5, 0);
	pVerts[1].set(+0.5f, -0.5f, -0.5f, 0, 1);
	pVerts[2].set(-0.5f, -0.5f, -0.5f, 1, 1);
	pTriList[0].set(0, 1, 2);

	v1 = pVerts[2].Pos - pVerts[0].Pos;
	v2 = pVerts[1].Pos - pVerts[0].Pos;
	n = v2.cross(v1);
	n.norm();

	pVerts[0].normal = Vect(n.X(), n.Y(), n.Z(), 0.0f);
	pVerts[1].normal = Vect(n.X(), n.Y(), n.Z(), 0.0f);
	pVerts[2].normal = Vect(n.X(), n.Y(), n.Z(), 0.0f);


	// Right Face
	pVerts[3].set(0.0f, 0.5f, 0.0f, 0.5, 0);
	pVerts[4].set(-0.5f, -0.5f, -0.5f, 0, 1);
	pVerts[5].set(-0.5f, -0.5f, +0.5f, 1, 1);
	pTriList[1].set(3, 4, 5);

	v1 = pVerts[5].Pos - pVerts[3].Pos;
	v2 = pVerts[4].Pos - pVerts[3].Pos;
	n = v2.cross(v1);
	n.norm();

	pVerts[3].normal = Vect(n.X(), n.Y(), n.Z(), 0.0f);
	pVerts[4].normal = Vect(n.X(), n.Y(), n.Z(), 0.0f);
	pVerts[5].normal = Vect(n.X(), n.Y(), n.Z(), 0.0f);


	// Back Face
	pVerts[6].set(0.0f, 0.5f, 0.0f, 0.5, 0);
	pVerts[7].set(-0.5f, -0.5f, +0.5f, 0, 1);
	pVerts[8].set(+0.5f, -0.5f, +0.5f, 1, 1);
	pTriList[2].set(6, 7, 8);

	v1 = pVerts[8].Pos - pVerts[6].Pos;
	v2 = pVerts[7].Pos - pVerts[6].Pos;
	n = v2.cross(v1);
	n.norm();

	pVerts[6].normal = Vect(n.X(), n.Y(), n.Z(), 0.0f);
	pVerts[7].normal = Vect(n.X(), n.Y(), n.Z(), 0.0f);
	pVerts[8].normal = Vect(n.X(), n.Y(), n.Z(), 0.0f);


	// Left Face
	pVerts[9].set(0.0f, 0.5f, 0.0f, 0.5, 0);
	pVerts[10].set(+0.5f, -0.5f, +0.5f, 0, 1);
	pVerts[11].set(+0.5f, -0.5f, -0.5f, 1, 1);
	pTriList[3].set(9, 10, 11);

	v1 = pVerts[11].Pos - pVerts[9].Pos;
	v2 = pVerts[10].Pos - pVerts[9].Pos;
	n = v2.cross(v1);
	n.norm();

	pVerts[9].normal = Vect(n.X(), n.Y(), n.Z(), 0.0f);
	pVerts[10].normal = Vect(n.X(), n.Y(), n.Z(), 0.0f);
	pVerts[11].normal = Vect(n.X(), n.Y(), n.Z(), 0.0f);


	// Bottom Faces
	pVerts[12].set(-0.5f, -0.5f, -0.5f, 0, 0, 0, -1, 0);
	pVerts[13].set(+0.5f, -0.5f, -0.5f, 0, 1, 0, -1, 0);
	pVerts[14].set(-0.5f, -0.5f, +0.5f, 1, 0, 0, -1, 0);
	pTriList[4].set(12, 13, 14);

	pVerts[15].set(+0.5f, -0.5f, +0.5f, 1, 1, 0, -1, 0);
	pTriList[5].set(13, 15, 14);


}

void ModelTools::CreateUnitSphere(int vslice, int hslice, StandardVertex*& pVerts, int& nverts, TriangleByIndex*& pTriList, int& ntri)
{
	nverts = vslice * (hslice + 1) + 2;
	ntri = vslice * hslice * 2;
	pVerts = new StandardVertex[nverts];
	pTriList = new TriangleByIndex[ntri];

	Vect* texcoords = new Vect[nverts];

	// North pole.
	pVerts[0].set(0.0f, 1.0f, 0.0f, 0.0f, 1.0f);

	// South pole.
	pVerts[nverts - 1].set(0.0f, -1.0f, 0.0f, 0.0f, 0.0f);

	// +1.0f because there's a gap between the poles and the first parallel.
	float latitude_step = 1.0f / (hslice + 1.0f);
	float longitude_step = 1.0f / (vslice);

	// start writing new vertices at position 1
	uint32_t vertex = 1;
	for (uint32_t latitude = 0; latitude < hslice; latitude++) {
		for (uint32_t longitude = 0; longitude <= vslice; longitude++) {
			// Scale coordinates into the 0...1 texture coordinate range,
			// with north at the top (y = 1).
			texcoords[vertex] = Vect(longitude * longitude_step, 1.0f - (latitude + 1) * latitude_step, 0.0f, 0.0f);


			// Convert to spherical coordinates:
			// theta is a longitude angle (around the equator) in radians.
			// phi is a latitude angle (north or south of the equator).
			float theta = texcoords[vertex].X() * 2.0f * 3.14159f;
			float phi = (texcoords[vertex].Y() - 0.5f) * 3.14159f;

			// This determines the radius of the ring of this line of latitude.
			// It's widest at the equator, and narrows as phi increases/decreases.
			float c = cosf(phi);

			// Usual formula for a vector in spherical coordinates.
			// You can exchange x & z to wind the opposite way around the sphere.
			float x = c * cosf(theta);
			float y = sinf(phi);
			float z = c * sinf(theta);
			Vect v = Vect(x, y, z);
			v.norm();
			pVerts[vertex].set(
				x, y, z,
				texcoords[vertex].X(),
				texcoords[vertex].Y(),
				v.X(), v.Y(), v.Z()
			);

			// Proceed to the next vertex.
			vertex++;
		}
	}

	uint32_t triangle = 0;

	for (uint32_t face = 0; face < vslice; face++) {
		pTriList[triangle++].set(0, face + 2, face + 1);
	}

	// Each row has one more unique vertex than there are lines of longitude,
	// since we double a vertex at the texture seam.
	uint32_t row_length = vslice + 1;

	for (uint32_t latitude = 0; latitude < hslice - 1; latitude++) {
		// Plus one for the pole.
		int row_start = latitude * row_length + 1;
		for (uint32_t longitude = 0; longitude < vslice; longitude++) {
			uint32_t first_corner = row_start + longitude;

			// First triangle of quad: Top-Left, Bottom-Left, Bottom-Right
			pTriList[triangle++].set(first_corner,
				first_corner + row_length + 1,
				first_corner + row_length);

			// Second triangle of quad: Top-Left, Bottom-Right, Top-Right
			pTriList[triangle++].set(first_corner,
				first_corner + 1,
				first_corner + row_length + 1);
		}
	}

	uint32_t south_pole = nverts - 1;
	uint32_t bottom_row = (hslice - 1) * row_length + 1;


	for (uint32_t face = 0; face < vslice; face++) {
		pTriList[triangle++].set(south_pole, bottom_row + face,
			bottom_row + face + 1);
	}

	delete[] texcoords;
}


