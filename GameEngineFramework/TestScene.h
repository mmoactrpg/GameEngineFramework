#pragma once

//#include "stdafx.h"

#include "Engine/SceneManagement/SceneNode.h"
#include "Engine/SceneManagement/Scene.h"
#include "Engine/SceneManagement/TransformNode.h"
#include "Engine/SceneManagement/CameraNode.h"
#include "Engine/SceneManagement/MeshNode.h"
#include "Engine/SceneManagement/Grid.h"
#include "Engine/SceneManagement/MovementController.h"

using namespace Engine::SceneManagement;

class TestScene
{
public:
	TestScene(void);
	~TestScene(void);

	Scene * BuildScene( IDirect3DDevice9 * d3dDevice);

};

