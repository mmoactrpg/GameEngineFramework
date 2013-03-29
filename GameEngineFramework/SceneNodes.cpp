//==========================================
// 功能描述：	 
// 创建者：JB
// 创建日期：[5/1/2012]
// 修改日期：[5/1/2012]
//-----------------------------------------------------------------------------

#pragma once


#include "SceneNodes.h"
//#include "tchar.h"z

//using namespace d3d;

////////////////////////////////////////////////////
//
// Some useful vectors
// 
////////////////////////////////////////////////////

D3DXVECTOR4 g_forward4 = D3DXVECTOR4(0,0,1,1);
D3DXVECTOR4 g_right4 = D3DXVECTOR4(1,0,0,1);
D3DXVECTOR4 g_up4 = D3DXVECTOR4(0,1,0,1);

D3DXVECTOR3 g_forward3 = D3DXVECTOR3(0,0,1);
D3DXVECTOR3 g_right3 = D3DXVECTOR3(1,0,0);
D3DXVECTOR3 g_up3 = D3DXVECTOR3(0,1,0);


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




////////////////////////////////////////////////////
// SceneNode Implementation
////////////////////////////////////////////////////

SceneNode::~SceneNode()
{
	// Get rid of all those pesky kids...
	while (!m_children.empty())
	{
		m_children.pop_front();
	}
}


HRESULT SceneNode::VRestore(Scene *pScene)
{
	
	// This is meant to be called from any class
	// that inherits from SceneNode and overloads
	// VRestore()

	SceneNodeList::iterator i = m_children.begin();
	while (i != m_children.end())
	{
		(*i)->VRestore(pScene);
		i++;
	}

	return S_OK;
}

HRESULT SceneNode::VUpdate(Scene *pScene, DWORD const elapsedMs)
{
	// This is meant to be called from any class
	// that inherits from SceneNode and overloads
	// VUpdate()

	SceneNodeList::iterator i = m_children.begin();
	while (i != m_children.end())
	{
		(*i)->VUpdate(pScene, elapsedMs);
		i++;
	}
	return S_OK;
}


HRESULT SceneNode::VRenderChildren(Scene *pScene)
{
	// Iterate through the children....
	SceneNodeList::iterator i = m_children.begin();
	while (i != m_children.end())
	{
		if ((*i)->VPreRender(pScene)==S_OK)
		{
			// You could short-circuit rendering
			// if an object returns E_FAIL from
			// VPreRender()

			(*i)->VRender(pScene);
			(*i)->VRenderChildren(pScene);
		}
		(*i)->VPostRender(pScene);
		i++;
	}

	return S_OK;
}


////////////////////////////////////////////////////
// Scene Implementation
////////////////////////////////////////////////////

Scene::Scene(IDirect3DDevice9 *device, shared_ptr<SceneNode> root)
: m_root(root)
{
	m_pDevice = device;
	m_pDevice->AddRef();

	D3DXCreateMatrixStack(0, &m_matrixStack);
}

Scene::~Scene()
{
	SAFE_RELEASE(m_pDevice);
	SAFE_RELEASE(m_matrixStack);
}


HRESULT Scene::Render()
{
	if (!m_root)
		return S_OK;

	// The scene root could be anything, but it
	// is usually a TransformNode with the identity
	// matrix

	if (m_root->VPreRender(this)==S_OK)
	{
		m_root->VRender(this);
		m_root->VRenderChildren(this);
		m_root->VPostRender(this);
	}

	return S_OK;
}	



HRESULT Scene::Restore()
{
	if (!m_root)
		return S_OK;

	return m_root->VRestore(this);
}


HRESULT Scene::Update()
{
	static DWORD lastTime = 0;
	DWORD elapsedTime = 0;
	DWORD now = timeGetTime();

	if (!m_root)
		return S_OK;

	if (lastTime == 0)
	{
		lastTime = now;
	}

	elapsedTime = now - lastTime;
	lastTime = now;

	// There's one one controller. But that isn't 
	// very usefull, is it??? You should make a list 
	// of them.

	if (m_pMovementController)
		m_pMovementController->Update(elapsedTime);

	return m_root->VUpdate(this, elapsedTime);
}

////////////////////////////////////////////////////
// TransformNode Implementation
////////////////////////////////////////////////////

HRESULT TransformNode::VPreRender(Scene *pScene) 
{
	// Note this code carefully!!!!! It is COMPLETELY different
	// from some DirectX 9 documentation out there....

	pScene->m_matrixStack->Push();
	pScene->m_matrixStack->MultMatrixLocal(&m_toWorld);
	pScene->m_pDevice->SetTransform(D3DTS_WORLD, pScene->m_matrixStack->GetTop());
	return S_OK;
}

HRESULT TransformNode::VPostRender(Scene *pScene) 
{
	pScene->m_matrixStack->Pop();
	pScene->m_pDevice->SetTransform(D3DTS_WORLD, pScene->m_matrixStack->GetTop());
	return S_OK;
}

void TransformNode::VSetTransform(const D3DXMATRIX *toWorld, const D3DXMATRIX *fromWorld)
{
	m_toWorld = *toWorld;
	if (!fromWorld)
	{
		// Good god this is expensive...why bother???? Look in Chapter 10.
		D3DXMatrixInverse(&m_fromWorld, NULL, &m_toWorld);
	}
	else
	{
		m_fromWorld = *fromWorld;
	}
}

void TransformNode::VGetTransform(D3DXMATRIX *toWorld, D3DXMATRIX *fromWorld)
{
	if (toWorld)
		*toWorld = m_toWorld;
	
	if (fromWorld)
		*fromWorld = m_fromWorld;
}



////////////////////////////////////////////////////
// CameraNode Implementation
////////////////////////////////////////////////////

HRESULT CameraNode::VUpdate(Scene *pScene, DWORD const )
{
	pScene->m_pDevice->SetTransform( D3DTS_VIEW, &m_fromWorld );
	return S_OK;
}

////////////////////////////////////////////////////
// MeshNode Implementation
////////////////////////////////////////////////////

HRESULT MeshNode::VRender(Scene *pScene)
{
	pScene->m_pDevice->SetMaterial( &m_material );
	return m_mesh->DrawSubset(0); 
} 


////////////////////////////////////////////////////
// ArrowNode Implementation
////////////////////////////////////////////////////

ArrowNode::ArrowNode(const float length, const D3DXMATRIX *t, const D3DMATERIAL9 &material, IDirect3DDevice9 *pDevice) 
	: TransformNode(t) 
{ 
    D3DXCreateCylinder( pDevice, 0.1f * length, 0.0f, 0.3f * length, 16, 1, &m_cone, NULL);
	D3DXCreateCylinder( pDevice, 0.05f * length, 0.05f * length, 0.7f * length, 16, 2, &m_shaft, NULL);

	D3DXMatrixTranslation(&m_coneTrans, 0, 0, (0.7f + 0.15f) * length);
	D3DXMatrixTranslation(&m_shaftTrans, 0, 0, (0.35f * length));

	m_material = material; 
}

HRESULT ArrowNode::VRender(Scene *pScene)
{
	pScene->m_pDevice->SetMaterial( &m_material );

	pScene->m_matrixStack->Push();
	pScene->m_matrixStack->MultMatrixLocal(&m_shaftTrans);
	pScene->m_pDevice->SetTransform(D3DTS_WORLD, pScene->m_matrixStack->GetTop());

	m_shaft->DrawSubset(0); 
	pScene->m_matrixStack->Pop();

	pScene->m_matrixStack->Push();
	pScene->m_matrixStack->MultMatrixLocal(&m_coneTrans);
	pScene->m_pDevice->SetTransform(D3DTS_WORLD, pScene->m_matrixStack->GetTop());
	m_cone->DrawSubset(0);

	pScene->m_matrixStack->Pop();
	return S_OK;
}




////////////////////////////////////////////////////
// SkyNode Implementation
//
// This code wasn't finished at the time my deadline for
// Chapter 10 drew near, so I bailed. A sky box is a nice 
// special effect that makes the clouds look like they are 
// very away, and essentailly look like they are following you.
//
// To achieve the effect, build a largeish cube 
// without a bottom, put a nice sky texture on it, and move it
// exactly in tandem with the movements of the camera.
// 
// It was broken, sadly, and I never finished it. Perhaps 
// you can make it work!
////////////////////////////////////////////////////

SkyNode::SkyNode(const TCHAR *pTextureBaseName, shared_ptr<CameraNode> camera)
: TransformNode(D3DXMatrixIdentity(&m_toWorld))
, m_camera(camera)
{
	m_textureBaseName = pTextureBaseName;
	for (int i=0; i<5; ++i)
	{
		m_pTexture[i] = NULL;
	}
}


SkyNode::~SkyNode()
{
	for (int i=0; i<5; ++i)
	{
		SAFE_RELEASE(m_pTexture[i]);
	}
	SAFE_RELEASE(m_pVerts);
}

HRESULT SkyNode::VRestore(Scene *pScene)
{
	// Call the base class's restore
	SceneNode::VRestore(pScene);

	LPDIRECT3DDEVICE9 pDevice = pScene->m_pDevice;

	const TCHAR *suffix[] = { L"_n.jpg", L"_e.jpg",  L"_s.jpg",  L"_w.jpg",  L"_u.jpg" };
	for (int i=0; i<5; ++i)
	{
		TCHAR name[256];
		_tcscpy(name, _T("Textures\\"));
		_tcscat(name, m_textureBaseName);
		_tcscat(name, suffix[i]);
		//HRESULT rs = D3DUtil_CreateTexture( pDevice, name, &m_pTexture[i] );
		
		if( FAILED( D3DXCreateTextureFromFile(
			pDevice,
			name,
			&m_pTexture[i])  ) )
			return E_FAIL;
	}

	m_numVerts = 20;

    if( FAILED( pDevice->CreateVertexBuffer( 
		m_numVerts*sizeof(COLORED_TEXTURED_VERTEX),
        D3DUSAGE_WRITEONLY, D3DFVF_COLORED_TEXTURED_VERTEX,
        D3DPOOL_MANAGED, &m_pVerts, NULL ) ) )
    {
        return E_FAIL;
    }

    // Fill the vertex buffer. We are setting the tu and tv texture
    // coordinates, which range from 0.0 to 1.0
    COLORED_TEXTURED_VERTEX* pVertices;
    if( FAILED( m_pVerts->Lock( 0, 0, (void**)&pVertices, 0 ) ) )
        return E_FAIL;

	// Loop through the grid squares and calc the values
	// of each index. Each grid square has two triangles:
	//
	//		A - B
	//		| / |
	//		C - D

	COLORED_TEXTURED_VERTEX skyVerts[4];
	D3DCOLOR skyVertColor = 0xffffffff;
	float dim = 50.0f;

	skyVerts[0].position = D3DXVECTOR3( dim, dim, dim ); skyVerts[0].color=skyVertColor; skyVerts[0].tu=1; skyVerts[0].tv=0; 
	skyVerts[1].position = D3DXVECTOR3(-dim, dim, dim ); skyVerts[1].color=skyVertColor; skyVerts[1].tu=0; skyVerts[1].tv=0; 
	skyVerts[2].position = D3DXVECTOR3( dim,-dim, dim ); skyVerts[2].color=skyVertColor; skyVerts[2].tu=1; skyVerts[2].tv=1; 
	skyVerts[3].position = D3DXVECTOR3(-dim,-dim, dim ); skyVerts[3].color=skyVertColor; skyVerts[3].tu=0; skyVerts[3].tv=1; 

	D3DXMATRIX rotY;
	D3DXMatrixRotationY(&rotY, D3DX_PI/2.0f);
	D3DXMATRIX rotX;
	D3DXMatrixRotationX(&rotX,  - D3DX_PI/2.0f);
	
	m_sides = 5;

	for (DWORD side = 0; side < m_sides; side++)
	{
		for (DWORD v = 0; v < 4; v++)
		{
			D3DXVECTOR4 temp;
			if (side < m_sides-1)
			{
				D3DXVec3Transform(&temp, &skyVerts[v].position, &rotY);
			}
			else
			{
				skyVerts[0].tu=1; skyVerts[0].tv=1; 
				skyVerts[1].tu=1; skyVerts[1].tv=0; 
				skyVerts[2].tu=0; skyVerts[2].tv=1; 
				skyVerts[3].tu=0; skyVerts[3].tv=0; 

				D3DXVec3Transform(&temp, &skyVerts[v].position, &rotX);
			}
			skyVerts[v].position = D3DXVECTOR3(temp.x, temp.y, temp.z);
		}
		memcpy(&pVertices[side*4], skyVerts, sizeof(skyVerts));
	}

	m_pVerts->Unlock();
	return S_OK;
}

HRESULT SkyNode::VPreRender(Scene *pScene)
{
	D3DXMATRIX cameraTrans, skyTrans;
	m_camera->VGetTransform(&cameraTrans, NULL);

	D3DXVECTOR4 pos = D3DXVECTOR4(cameraTrans(3,0), cameraTrans(3,1), cameraTrans(3,2), cameraTrans(3,3));
	D3DXMatrixTranslation(&skyTrans,pos.x,pos.y,pos.z);

	VSetTransform(&skyTrans);

	return TransformNode::VPreRender(pScene);
}

HRESULT SkyNode::VRender(Scene *pScene)
{
	LPDIRECT3DDEVICE9 pDevice = pScene->m_pDevice;

	DWORD oldLightMode;
	pDevice->GetRenderState( D3DRS_LIGHTING, &oldLightMode );
	pDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

	DWORD oldCullMode;
	pDevice->GetRenderState( D3DRS_CULLMODE, &oldCullMode );
	pDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );


	//防止z-buffer写入，主要是使天空盒看起来有无限远的结果
	 //pDevice->SetRenderState(D3DRS_ZWRITEENABLE,false);
	 //pDevice->SetRenderState(D3DRS_ZENABLE, false);
	//利用夹取纹理寻址的方式来实现天空盒的无缝连接
	 pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	 pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);


	// Setup our texture. Using textures introduces the texture stage states,
    // which govern how textures get blended together (in the case of multiple
    // textures) and lighting information. In this case, we are modulating
    // (blending) our texture with the diffuse color of the vertices.

    pDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
    pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    pDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );

	pDevice->SetStreamSource( 0, m_pVerts, 0, sizeof(COLORED_TEXTURED_VERTEX) );
	pDevice->SetFVF( D3DFVF_COLORED_TEXTURED_VERTEX);

	

	for (DWORD side = 0; side < m_sides; side++)
	{
		//pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);
		//pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR);

		pDevice->SetTexture( 0, m_pTexture[side] );
		pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP , 4 * side, 2);
	}

	// Notice that the render states are returned to 
	// their original settings.....
	// Could there be a better way???

	pDevice->SetTexture (0, NULL);
	pDevice->SetRenderState( D3DRS_LIGHTING, oldLightMode );
	pDevice->SetRenderState( D3DRS_CULLMODE, oldCullMode );

	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
	//pDevice->SetRenderState(D3DRS_ZENABLE, true);
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	return S_OK;
}

////////////////////////////////////////////////////
// Grid Implementation
////////////////////////////////////////////////////

Grid::Grid(const DWORD gridSize, 
		   const DWORD color, 
		   const TCHAR *textureFile, 
		   const D3DXMATRIX *t)
	: TransformNode(t)
{
	m_gridSize = gridSize;
	m_color = color;
	m_textureFile = textureFile;

	m_pTexture = NULL;
	m_pVerts = NULL;
	m_pIndices = NULL;
	m_numVerts = m_numPolys = 0;
}


Grid::~Grid()
{
	SAFE_RELEASE(m_pTexture);
	SAFE_RELEASE(m_pVerts);
	SAFE_RELEASE(m_pIndices);
}


HRESULT Grid::VRestore(Scene *pScene)
{
	// Call the base class's restore
	SceneNode::VRestore(pScene);

	LPDIRECT3DDEVICE9 pDevice = pScene->m_pDevice;
	if( FAILED( D3DXCreateTextureFromFile(
		pDevice,
		const_cast<TCHAR *>(m_textureFile),
		&m_pTexture )
		) )
		return E_FAIL;

    // Create the vertex buffer - we'll need enough verts
	// to populate the grid. If we want a 2x2 grid, we'll
	// need 3x3 set of verts.
	m_numVerts = (m_gridSize+1)*(m_gridSize+1);

    if( FAILED( pDevice->CreateVertexBuffer( 
		m_numVerts*sizeof(COLORED_TEXTURED_VERTEX),
        D3DUSAGE_WRITEONLY, D3DFVF_COLORED_TEXTURED_VERTEX,
        D3DPOOL_MANAGED, &m_pVerts, NULL ) ) )
    {
        return E_FAIL;
    }

    // Fill the vertex buffer. We are setting the tu and tv texture
    // coordinates, which range from 0.0 to 1.0
    COLORED_TEXTURED_VERTEX* pVertices;
    if( FAILED( m_pVerts->Lock( 0, 0, (void**)&pVertices, 0 ) ) )
        return E_FAIL;

	for( DWORD j=0; j<(m_gridSize+1); j++ )
    {
		for (DWORD i=0; i<(m_gridSize+1); i++)
		{
			// Which vertex are we setting?
			int index = i + (j * (m_gridSize+1) );
			COLORED_TEXTURED_VERTEX *vert = &pVertices[index];

			// Default position of the grid is at the origin, flat on
			// the XZ plane.
			float x = (float)i;
			float y = (float)j;
			vert->position = ( x * D3DXVECTOR3(1,0,0) ) + ( y * D3DXVECTOR3(0,0,1) );
			vert->color    = m_color;

			// The texture coordinates are set to x,y to make the
			// texture tile along with units - 1.0, 2.0, 3.0, etc.
			vert->tu       = x;
			vert->tv       = y;
		}
	}
    m_pVerts->Unlock();

	// The number of indicies equals the number of polygons times 3
	// since there are 3 indicies per polygon. Each grid square contains
	// two polygons. The indicies are 16 bit, since our grids won't
	// be that big!
	m_numPolys = m_gridSize*m_gridSize*2;
	if( FAILED( pDevice->CreateIndexBuffer(sizeof(WORD) * m_numPolys * 3,
				D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIndices, NULL ) ) )
	{
		return E_FAIL;
	}

    WORD *pIndices;
    if( FAILED( m_pIndices->Lock( 0, 0, (void**)&pIndices, 0 ) ) )
        return E_FAIL;
	
	// Loop through the grid squares and calc the values
	// of each index. Each grid square has two triangles:
	//
	//		A - B
	//		| / |
	//		C - D

	for( DWORD j=0; j<m_gridSize; j++ )
    {
		for (DWORD i=0; i<m_gridSize; i++)
		{
			// Triangle #1  ACB
			*(pIndices) = WORD(i + (j*(m_gridSize+1)));
			*(pIndices+1) = WORD(i + ((j+1)*(m_gridSize+1)));
			*(pIndices+2) = WORD((i+1) + (j*(m_gridSize+1)));

			// Triangle #2  BCD
			*(pIndices+3) = WORD((i+1) + (j*(m_gridSize+1)));
			*(pIndices+4) = WORD(i + ((j+1)*(m_gridSize+1)));
			*(pIndices+5) = WORD((i+1) + ((j+1)*(m_gridSize+1)));
			pIndices+=6;
		}
	}
	
    m_pIndices->Unlock();
	return S_OK;
}


HRESULT Grid::VRender(Scene *pScene)
{
	// This is slightly different from the Chapter 7 implementation...

	LPDIRECT3DDEVICE9 pDevice = pScene->m_pDevice;

	DWORD oldLightMode;
	pDevice->GetRenderState( D3DRS_LIGHTING, &oldLightMode );
	pDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

	DWORD oldCullMode;
	pDevice->GetRenderState( D3DRS_CULLMODE, &oldCullMode );
	pDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

	// Setup our texture. Using textures introduces the texture stage states,
    // which govern how textures get blended together (in the case of multiple
    // textures) and lighting information. In this case, we are modulating
    // (blending) our texture with the diffuse color of the vertices.

	pDevice->SetTexture( 0, m_pTexture );
    pDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
    pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    pDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );

	pDevice->SetStreamSource( 0, m_pVerts, 0, sizeof(COLORED_TEXTURED_VERTEX) );
	pDevice->SetIndices(m_pIndices);
	pDevice->SetFVF( D3DFVF_COLORED_TEXTURED_VERTEX );
	pDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST , 0, 0, m_numVerts, 0, m_numPolys );

	// Notice that the render states are returned to 
	// their original settings.....
	// Could there be a better way???

	pDevice->SetTexture (0, NULL);
	pDevice->SetRenderState( D3DRS_LIGHTING, oldLightMode );
	pDevice->SetRenderState( D3DRS_CULLMODE, oldCullMode );

	return S_OK;
}

////////////////////////////////////////////////////
// MovementController Implementation
////////////////////////////////////////////////////

#define MAX(a, b) ((a) >= (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

#define RADIANS_TO_DEGREES(x) ((x) * 180.0f / D3DX_PI)
#define DEGREES_TO_RADIANS(x) ((x) * D3DX_PI / 180.0f)

MovementController::MovementController(shared_ptr<TransformNode> object, float initialYaw, float initialPitch)
: m_object(object)
{
	m_object->VGetTransform(&m_matToWorld, &m_matFromWorld);

	m_fTargetYaw = m_fYaw = RADIANS_TO_DEGREES(-initialYaw);
	m_fTargetPitch = m_fPitch = RADIANS_TO_DEGREES(initialPitch);

	D3DXVECTOR4 pos = D3DXVECTOR4(m_matToWorld(3,0), m_matToWorld(3,1), m_matToWorld(3,2), m_matToWorld(3,3));
	D3DXMatrixTranslation(&m_matPosition,pos.x,pos.y,pos.z);

	m_bLeftMouseDown = false;

    memset(m_bKey, 0x00, sizeof(m_bKey));
}


void MovementController::OnMouseMove(const POINTS &mousePos)
{
	if (m_bLeftMouseDown)
	{
		// Everytime the mouse moves, figure out how far and in 
		// which direction. The X axis is for yaw, the Y axis is 
		// for pitch. 

		m_fTargetYaw = m_fYawOnDown + (m_mousePosOnDown.x - mousePos.x);
		m_fTargetPitch = m_fPitchOnDown + (mousePos.y - m_mousePosOnDown.y);
	}
}

void MovementController::OnLButtonDown(const POINTS &mousePos)
{
	// The mouse is down - record where it happened.
	m_bLeftMouseDown = true;
	m_mousePosOnDown = mousePos; 
	m_fYawOnDown = m_fTargetYaw;
	m_fPitchOnDown = m_fTargetPitch;
}

void MovementController::OnLButtonUp(const POINTS &)
{
	m_bLeftMouseDown = false;
}


void MovementController::Update(DWORD const)
{
	if (m_bKey['Q'])
	{
		D3DXMATRIX camTranslate;
		D3DXMatrixTranslation(&m_matPosition, 8.847f, 7.055f, 11.618f);

		// The secret formula!!! Don't give it away!
		m_fTargetYaw = m_fYaw += -64.35f;
		m_fTargetPitch = m_fPitch = 28.57f;

		// Calculate the new rotation matrix from the camera
		// yaw and pitch.
		D3DXMATRIX matRot;
		D3DXMatrixRotationYawPitchRoll(&matRot, -m_fYaw * D3DX_PI / 180, m_fPitch * D3DX_PI / 180, 0);

		// Create the new object-to-world matrix, and the
		// new world-to-object matrix. 
		D3DXMatrixMultiply(&m_matToWorld, &matRot, &m_matPosition);
		D3DXMatrixInverse(&m_matFromWorld, NULL, &m_matToWorld);

		m_object->VSetTransform(&m_matToWorld, &m_matFromWorld);
	}

	if (m_bKey['W'] || m_bKey['S'])
	{
		// In D3D, the "look at" default is always
		// the positive Z axis.
		D3DXVECTOR4 at = D3DXVECTOR4(0.0f,0.0f,1.0f,0.0);
		if (m_bKey['S'])
			at *= -1;

		D3DXVECTOR4 atWorld(0,0,0,0);

		// This will give us the "look at" vector 
		// in world space - we'll use that to move
		// the camera.
		D3DXVec4Transform(&atWorld, &at, &m_matToWorld);

		// But not an entire meter at a time!
		atWorld *= 0.5f;

		D3DXMATRIX camTranslate;
		D3DXMatrixTranslation(&camTranslate, atWorld.x, atWorld.y, atWorld.z);

		// The newly created delta position matrix, camTranslate, 
		// is concatenated with the member position matrix.
		D3DXMatrixMultiply(&m_matPosition, &m_matPosition, &camTranslate);
	}

	/*if (m_bLeftMouseDown)
	{*/
		// The secret formula!!! Don't give it away!
		m_fYaw += (m_fTargetYaw - m_fYaw) * ( .35f );
		m_fTargetPitch = MAX(-90, MIN(90, m_fTargetPitch));
		m_fPitch += (m_fTargetPitch - m_fPitch) * ( .35f );

		// Calculate the new rotation matrix from the camera
		// yaw and pitch.
		D3DXMATRIX matRot;
		D3DXMatrixRotationYawPitchRoll(&matRot, -m_fYaw * D3DX_PI / 180, m_fPitch * D3DX_PI / 180, 0);

		// Create the new object-to-world matrix, and the
		// new world-to-object matrix. 
		D3DXMatrixMultiply(&m_matToWorld, &matRot, &m_matPosition);
		D3DXMatrixInverse(&m_matFromWorld, NULL, &m_matToWorld);

		m_object->VSetTransform(&m_matToWorld, &m_matFromWorld);
	//}
}

////////////////////////////////////////////////////
// Free Function to Build the Scene
////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// Name: D3DUtil_InitMaterial()
// Desc: Initializes a D3DMATERIAL9 structure, setting the diffuse and ambient
//       colors. It does not set emissive or specular colors.
//-----------------------------------------------------------------------------
VOID D3DUtil_InitMaterial( D3DMATERIAL9& mtrl, FLOAT r, FLOAT g, FLOAT b,
	FLOAT a )
{
	ZeroMemory( &mtrl, sizeof(D3DMATERIAL9) );
	mtrl.Diffuse.r = mtrl.Ambient.r = r;
	mtrl.Diffuse.g = mtrl.Ambient.g = g;
	mtrl.Diffuse.b = mtrl.Ambient.b = b;
	mtrl.Diffuse.a = mtrl.Ambient.a = a;
}


Scene *BuildScene(IDirect3DDevice9 *d3dDevice)
{
	// Setup some materials - we'll use these for 
	// making the same mesh appear in multiple
	// colors

	D3DMATERIAL9 colors[8];
	D3DUtil_InitMaterial( colors[0], 1.0f, 1.0f, 1.0f, 1.0f );	// white
	D3DUtil_InitMaterial( colors[1], 0.0f, 1.0f, 1.0f, 1.0f );	// cyan
	D3DUtil_InitMaterial( colors[2], 1.0f, 0.0f, 0.0f, 1.0f );	// red
	D3DUtil_InitMaterial( colors[3], 0.0f, 1.0f, 0.0f, 1.0f );	// green
	D3DUtil_InitMaterial( colors[4], 0.0f, 0.0f, 1.0f, 1.0f );	// blue
	D3DUtil_InitMaterial( colors[5], 0.4f, 0.4f, 0.4f, 0.4f );	// 40% grey
	D3DUtil_InitMaterial( colors[6], 0.25f, 0.25f, 0.25f, 0.25f );	// 25% grey
	D3DUtil_InitMaterial( colors[7], 0.65f, 0.65f, 0.65f, 0.65f );	// 65% grey

	// The identity matrix is always useful
	D3DXMATRIX ident;
	D3DXMatrixIdentity(&ident);

	// We'll use these rotations for some teapots and grid objects
	D3DXMATRIX rotateX, rotateY, rotateZ;

	// Create the root, and the camera.
	// Remeber how to use smart pointers?? I hope so!

	shared_ptr<TransformNode> root(new TransformNode(&ident));

	shared_ptr<CameraNode> camera(new CameraNode(&ident));
	root->m_children.push_back(camera);

	// We'll put the camera in the scene at (20,20,20) looking back at the Origin

	D3DXMATRIX rotOnly, result, inverse;
	float cameraYaw = - (3.0f * D3DX_PI) / 4.0f;
	float cameraPitch = D3DX_PI / 4.0f;
	D3DXQUATERNION q;
	D3DXQuaternionIdentity(&q);
	D3DXQuaternionRotationYawPitchRoll(&q, cameraYaw, cameraPitch, 0.0);
	D3DXMatrixRotationQuaternion(&rotOnly, &q);

	D3DXMATRIX trans;
	D3DXMatrixTranslation(&trans, 15.0f, 15.0f, 15.0f);

	D3DXMatrixMultiply(&result, &rotOnly, &trans);

	D3DXMatrixInverse(&inverse, NULL, &result);
	camera->VSetTransform(&result, &inverse);

	D3DXMatrixRotationZ(&rotateZ, D3DX_PI / 2.0f);
	D3DXMatrixRotationX(&rotateX, -D3DX_PI / 2.0f);
	D3DXVECTOR3 target(30, 2, 15);


	ID3DXMesh *teapot;
    if( SUCCEEDED( D3DXCreateTeapot( d3dDevice, &teapot, NULL ) ) )
	{
		// Teapot #1 - a white one at (x=6,y=2,z=4)
		D3DXMatrixTranslation(&trans,6,2,4);

		shared_ptr<SceneNode> mesh1(new MeshNode(teapot, &trans, colors[2]));
		root->m_children.push_back(mesh1);

		// Teapot #2 - a cyan one at (x=3,y=2,z=1)
		//   with a 
		D3DXMatrixTranslation(&trans, 3,2,1);
		D3DXMATRIX result;
		D3DXMatrixMultiply(&result, &rotateZ, &trans);

		shared_ptr<SceneNode> mesh2(new MeshNode(teapot, &result, colors[1]));
		root->m_children.push_back(mesh2);

		// Teapot #3 - another white one at (x=30, y=2, z=15)
		D3DXMATRIX rotateY90;
		D3DXMatrixRotationY(&rotateY90, D3DX_PI / 2.0f);
		D3DXMatrixTranslation(&trans, target.x, target.y, target.z);
		D3DXMatrixMultiply(&result, &rotateY90, &trans);
		shared_ptr<SceneNode> mesh3(new MeshNode(teapot, &result, colors[0]));
		root->m_children.push_back(mesh3);

		// We can release the teapot now, mesh1 and mesh2 AddRef'd it.
		SAFE_RELEASE(teapot);
	}

	ID3DXMesh *sphere;
	if ( SUCCEEDED( 
		D3DXCreateSphere( 
			d3dDevice, .25, 16, 16, &sphere, NULL) ) )
	{
		// We're going to create a spiral of spheres...
		// starting at (x=3, y=0, z=3), and spiraling
		// upward about a local Y axis.

		D3DXMatrixTranslation(&trans, 3,0,3);

		shared_ptr<SceneNode> sphere1(new MeshNode(sphere, &trans, colors[4]) );
		root->m_children.push_back(sphere1);

		// Here's the local rotation and translation.
		// We'll rotate about Y, and then translate
		// up (along Y) and forward (along Z).
		D3DXMatrixRotationY(&rotateY, D3DX_PI / 8.0f);
		D3DXMATRIX trans2;
		D3DXMatrixTranslation(&trans2, 0, 0.5, 0.5);
		D3DXMatrixMultiply(&result, &trans2, &rotateY);

		for (int i=0; i<25; i++)
		{
			// If you didn't think smart pointers were cool - 
			// watch this! No leaked memory....

			// Notice this is a heirarchy....
			shared_ptr<SceneNode> sphere2(new MeshNode(sphere, &result, colors[i%5]) );
			sphere1->m_children.push_back(sphere2);
			sphere1 = sphere2;
		}

		// We can release the sphere now, all the cylinders AddRef'd it.
		SAFE_RELEASE(sphere);
	}

	// Here are the grids...they make it easy for us to 
	// see where the coordinates are in 3D space.
	shared_ptr<SceneNode> grid1(new Grid(40, 0x00404040, L"Textures\\grid.dds", &ident));
	root->m_children.push_back(grid1);
	shared_ptr<SceneNode> grid2(new Grid(40, 0x00004000, L"Textures\\grid.dds", &rotateX));
	root->m_children.push_back(grid2);
	shared_ptr<SceneNode> grid3(new Grid(40, 0x00000040, L"Textures\\grid.dds", &rotateZ));
	root->m_children.push_back(grid3);

//  Here's the sky node that never worked!!!!
	shared_ptr<SkyNode> sky(new SkyNode(_T("Sky2"), camera));
	root->m_children.push_back(sky);

	D3DXMatrixTranslation(&trans,15,2,15);
	D3DXMatrixRotationY(&rotateY, D3DX_PI / 4.0f);
	D3DXMatrixMultiply(&result, &rotateY, &trans);

	shared_ptr<SceneNode> arrow1(new ArrowNode(2, &result, colors[0], d3dDevice));
	root->m_children.push_back(arrow1);

	D3DXMatrixRotationY(&rotateY, D3DX_PI / 2.0f);
	D3DXMatrixMultiply(&result, &rotateY, &trans);
	shared_ptr<SceneNode> arrow2(new ArrowNode(2, &result, colors[5], d3dDevice));
	root->m_children.push_back(arrow2);

	D3DXMatrixMultiply(&result, &rotateX, &trans);
	shared_ptr<SceneNode> arrow3(new ArrowNode(2, &result, colors[0], d3dDevice));
	root->m_children.push_back(arrow3);


	// Everything has been attached to the root. Now
	// we attach the root to the scene.

	Scene *scene = new Scene(d3dDevice, root);
	scene->Restore();

	// A movement controller is going to control the camera, 
	// but it could be constructed with any of the objects you see in this
	// function. You can have your very own remote controlled sphere. What fun...
	shared_ptr<MovementController> m_pMovementController(new MovementController(camera, cameraYaw, cameraPitch));
	scene->m_pMovementController = m_pMovementController;
	return scene;
}



