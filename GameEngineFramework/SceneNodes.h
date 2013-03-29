#pragma once
//========================================================================
// File: SceneNodes.h
//
// Part of the GameCode Application
//
// GameCode is the sample application that encapsulates much of the source code
// discussed in "Game Coding Complete" by Mike McShaffry, published by
// Paraglyph Press. ISBN: 1-932111-75-1
//
// If this source code has found it's way to you, and you think it has helped you
// in any way, do the author a favor and buy a new copy of the book - there are 
// detailed explanations in it that compliment this code well. Buy a copy at Amazon.com
// by clicking here: http://www.amazon.com/exec/obidos/ASIN/1932111751/gamecodecompl-20/
//
// There's also a companion web site at http://www.mcshaffry.com/GameCode/portal.php
//
// (c) Copyright 2003 Michael L. McShaffry
//
// This work is licensed under the Creative Commons Attribution-ShareAlike License. 
// To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/1.0/ 
// or send a letter to:
//      Creative Commons
//      559 Nathan Abbott Way
//      Stanford, California 94305, USA.
//
//========================================================================

////////////////////////////////////////////////////
//
// Game Coding Complete reference - Chapter 9 & 10
// 
////////////////////////////////////////////////////

#pragma warning ( disable : 4702 ) // unreachable code in <list> implementation *sigh*

#include "stdafx.h"



using namespace boost;
using namespace d3d;


// Forward declarations
class SceneNode;
class Scene;
class MovementController;


typedef std::list<shared_ptr<SceneNode> > SceneNodeList;




////////////////////////////////////////////////////
//
// SceneNode Definition
//
////////////////////////////////////////////////////

class SceneNode
{
public:
	SceneNodeList		m_children;

	virtual ~SceneNode();

	virtual HRESULT VPreRender(Scene *) { return S_OK; }
	virtual HRESULT VRender(Scene *) { return S_OK; }
	virtual HRESULT VPostRender(Scene *) { return S_OK; }

	virtual HRESULT VRenderChildren(Scene *);
	virtual HRESULT VRestore(Scene *);

	virtual HRESULT VUpdate(Scene *, DWORD const elapsedMs);

	// added comments around parameter names to avoid VC++ compiler warning about
	// unreferenced formal parameters -taj
	virtual void VSetTransform(const D3DXMATRIX * /*toWorld*/, const D3DXMATRIX * /*fromWorld*/=NULL) { }
};


////////////////////////////////////////////////////
//
// Scene Definition
//
// A heirarchical container of scene nodes.
////////////////////////////////////////////////////

class Scene
{
public:
	IDirect3DDevice9 *m_pDevice;
	ID3DXMatrixStack *m_matrixStack;
	shared_ptr<SceneNode>  m_root;
	shared_ptr<MovementController> m_pMovementController;

	Scene(IDirect3DDevice9 *device, shared_ptr<SceneNode> root);
	~Scene();

	HRESULT Render();
	HRESULT Restore();
	HRESULT Update();
};


////////////////////////////////////////////////////
//
// TransformNode Definition
//
//    This node changes the local object to world
//    transform by using D3D's Matrix Stack
//
////////////////////////////////////////////////////

class TransformNode : public SceneNode
{
public:
	D3DXMATRIX m_toWorld, m_fromWorld;

	TransformNode(const D3DXMATRIX *to, const D3DXMATRIX *from=NULL) { VSetTransform(to, from); }
	virtual HRESULT VPreRender(Scene *);
	virtual HRESULT VPostRender(Scene *);

	virtual void VSetTransform(const D3DXMATRIX *toWorld, const D3DXMATRIX *fromWorld=NULL);
	virtual void VGetTransform(D3DXMATRIX *toWorld, D3DXMATRIX *fromWorld);
};


////////////////////////////////////////////////////
//
// CameraNode Definition
//
//    A camera node controls the D3D view transform  
//
////////////////////////////////////////////////////

class CameraNode : public TransformNode
{
public:
	CameraNode(const D3DXMATRIX *t) : TransformNode(t) { }
	virtual HRESULT VUpdate(Scene *, DWORD const elapsedMs);
};

////////////////////////////////////////////////////
//
// MeshNode Definition
//
//    Attaches a D3D mesh object to the scene graph
//    with an accompanying material
//
////////////////////////////////////////////////////

class MeshNode : public TransformNode
{
protected:
	ID3DXMesh *m_mesh;
	D3DMATERIAL9 m_material;
	
public:
	MeshNode(ID3DXMesh *mesh, const D3DXMATRIX *t, const D3DMATERIAL9 &material) 
		: TransformNode(t) { m_mesh = mesh;  m_material = material; m_mesh->AddRef(); }

	virtual ~MeshNode() { SAFE_RELEASE(m_mesh); }
	virtual HRESULT VRender(Scene *);
};


////////////////////////////////////////////////////
//
// Grid Definition
//
//    A slightly modified version of Grid from 
//       the beginning 3D chapter
//
////////////////////////////////////////////////////

class Grid : public TransformNode
{
protected:
	LPDIRECT3DTEXTURE9		m_pTexture;			// the grid texture for the axes planes
	LPDIRECT3DVERTEXBUFFER9 m_pVerts;			// the grid verts
	LPDIRECT3DINDEXBUFFER9	m_pIndices;			// the grid index
	DWORD					m_numVerts;
	DWORD					m_numPolys;
	DWORD					m_gridSize;
	DWORD					m_color;
	const TCHAR *			m_textureFile;

public:
	Grid(const DWORD gridSize, const DWORD color, const TCHAR *textureFile, const D3DXMATRIX *t);
	virtual ~Grid();
	HRESULT VRestore(Scene *pScene);
	HRESULT VRender(Scene *pScene);
};

////////////////////////////////////////////////////
//
// Grid Definition
//
//    A slightly modified version of Grid from 
//       the beginning 3D chapter
//
////////////////////////////////////////////////////

class ArrowNode : public TransformNode
{
protected:
	ID3DXMesh *m_shaft;
	ID3DXMesh *m_cone;
	D3DMATERIAL9 m_material;
	D3DXMATRIX m_coneTrans;
	D3DXMATRIX m_shaftTrans;
	
public:
	ArrowNode(const float length, const D3DXMATRIX *t, const D3DMATERIAL9 &material, IDirect3DDevice9 *pDevice);

	virtual ~ArrowNode() { SAFE_RELEASE(m_shaft); SAFE_RELEASE(m_cone); }
	virtual HRESULT VRender(Scene *);
};


////////////////////////////////////////////////////
//
// IKeyboardSensitive & IMouseSensitive 
// Interface Definition
//
////////////////////////////////////////////////////

class IKeyboardSensitive
{
	virtual void OnKeyDown(const BYTE c)=0;
	virtual void OnKeyUp(const BYTE c)=0;
};

class IMouseSensitive
{
public:
	virtual void OnMouseMove(const POINTS &mousePos) = 0;
	virtual void OnLButtonDown(const POINTS &mousePos) = 0;
	virtual void OnLButtonUp(const POINTS &mousePos) = 0;
	virtual void OnRButtonDown(const POINTS &mousePos) = 0;
	virtual void OnRButtonUp(const POINTS &mousePos) = 0;
};


////////////////////////////////////////////////////
//
// MovementController Definition
//
//    Implements a quake-style movement controller
//
////////////////////////////////////////////////////

class MovementController : public IMouseSensitive, public IKeyboardSensitive
{
protected:
	D3DXMATRIX  m_matFromWorld;
	D3DXMATRIX	m_matToWorld;
    D3DXMATRIX  m_matPosition;

	BOOL					m_bLeftMouseDown;
	POINTS					m_mousePos;				// The current mouse postion
	POINTS					m_mousePosOnDown;		// The position of the mouse on
													//   a down event

	BYTE					m_bKey[256];			// Which keys are up and down

	// Orientation Controls
	FLOAT		m_fTargetYaw;
	FLOAT		m_fTargetPitch;
	FLOAT		m_fYaw;
	FLOAT		m_fPitch;
	FLOAT		m_fPitchOnDown;
	FLOAT		m_fYawOnDown;

	shared_ptr<TransformNode> m_object;

public:
	MovementController(shared_ptr<TransformNode> object, float initialYaw, float initialPitch);
	void SetObject(shared_ptr<TransformNode> newObject);

	void Update(DWORD const elapsedMs);

public:
	void OnMouseMove(const POINTS &mousePos);
	void OnLButtonDown(const POINTS &mousePos);
	void OnLButtonUp(const POINTS &mousePos);
	void OnRButtonDown(const POINTS &) { }
	void OnRButtonUp(const POINTS &) { }

	void OnKeyDown(const BYTE c) { m_bKey[c] = true; }
	void OnKeyUp(const BYTE c) { m_bKey[c] = false; }

	const D3DXMATRIX *GetToWorld() { return &m_matToWorld; }
	const D3DXMATRIX *GetFromWorld() { return &m_matFromWorld; }	
};

////////////////////////////////////////////////////
//
// SkyNode Definition
//
//    Implements a believable sky that follows
//	  the camera around - see more comments in the CPP
//
////////////////////////////////////////////////////

class SkyNode : public TransformNode
{
protected:
	LPDIRECT3DTEXTURE9		m_pTexture[5];		// the sky textures
	LPDIRECT3DVERTEXBUFFER9 m_pVerts;			// the sky verts
	DWORD					m_numVerts;
	DWORD					m_sides;
	const TCHAR *			m_textureBaseName;
	shared_ptr<CameraNode>	m_camera;

public:
	SkyNode(const TCHAR *textureFile, shared_ptr<CameraNode> camera);
	~SkyNode();
	HRESULT VRestore(Scene *pScene);
	HRESULT VRender(Scene *pScene);
	HRESULT VPreRender(Scene *pScene);
};


extern Scene *BuildScene(IDirect3DDevice9 *d3dDevice);
