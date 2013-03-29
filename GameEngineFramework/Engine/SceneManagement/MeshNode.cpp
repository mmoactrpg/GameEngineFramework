#pragma once


#include "MeshNode.h"

namespace Engine
{
	namespace SceneManagement
	{

		/*MeshNode::MeshNode(ID3DXMesh * mesh, const D3DMATRIX * matrix, const D3DMATERIAL9 & material)
		{
		}

		MeshNode::~MeshNode(void)
		{
		}
		*/

		HRESULT MeshNode::VRender(Scene * pScene)
		{
			pScene->m_pDevice->SetMaterial( &m_material);
			return m_mesh->DrawSubset(0);
		}

	}//end namepsace scene
}//end namespace engine
