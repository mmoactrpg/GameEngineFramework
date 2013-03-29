#pragma once

#include "CameraNode.h"

namespace Engine
{
	namespace SceneManagement
	{
		/*CameraNode::CameraNode(const D3DXMATRIX * matrix)
		{
		}*/

		CameraNode::~CameraNode(void)
		{
		}

		HRESULT CameraNode::VUpdate(Scene * pScene, DWORD const elapsedMs)
		{
			pScene->m_pDevice->SetTransform( D3DTS_VIEW, &m_fromWorld );
			return S_OK;
		}

	}//end namepsace scene
}//end namespace engine