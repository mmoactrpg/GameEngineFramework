#pragma  once

#include "../../stdafx.h"
////////////////////////////////////////////////////
//
// Vertex Type Definitions
//
////////////////////////////////////////////////////

struct TRANSFORMED_VERTEX
{
	D3DXVECTOR3 position;	// The screen x, y, z
	float rhw;				// always 1.0, the reciprocal of homogeneous w)
};
#define D3DFVF_TRANSFORMED_VERTEX (D3DFVF_XYZRHW)		

struct UNTRANSFORMED_VERTEX
{
	D3DXVECTOR3 position; // The position in 3D space
};
#define D3DFVF_UNTRANSFORMED_VERTEX (D3DFVF_XYZ) 

struct UNTRANSFORMED_LIT_VERTEX
{
	D3DXVECTOR3 position;	// The position in 3D space
	D3DCOLOR    diffuse;    // The diffuse color
	D3DCOLOR    specular;   // The specular color
};
#define D3DFVF_UNTRANSFORMED_LIT_VERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_SPECULAR)		

struct UNTRANSFORMED_UNLIT_VERTEX
{
	D3DXVECTOR3 position;	// The position in 3D space
	D3DXVECTOR3 normal;		// The normal vector (must be 1.0 units in length)
	D3DCOLOR    diffuse;    // The diffuse color
	D3DCOLOR    specular;   // The specular color
};
#define D3DFVF_UNTRANSFORMED_UNLIT_VERTEX \
	(D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_SPECULAR)		


// A structure for our custom vertex type. We added texture coordinates
struct COLORED_TEXTURED_VERTEX
{
	D3DXVECTOR3 position; // The position
	D3DCOLOR    color;    // The color
	FLOAT       tu, tv;   // The texture coordinates
};

// Our custom FVF, which describes our custom vertex structure
#define D3DFVF_COLORED_TEXTURED_VERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)

