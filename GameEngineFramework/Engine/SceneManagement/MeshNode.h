//==========================================
// ����������	 �����ʵ�����ڵ����
// �����ߣ�JB
// �������ڣ�[5/1/2012]
// �޸����ڣ�[5/1/2012]
//-----------------------------------------------------------------------------
#pragma once

#include "../../stdafx.h"
#include "TransformNode.h"

namespace Engine
{
	namespace SceneManagement
	{
		class MeshNode : public TransformNode
		{
		protected:
			//����
			ID3DXMesh * m_mesh;
			//����
			D3DMATERIAL9 m_material;

		public:
			MeshNode(ID3DXMesh * mesh, const D3DXMATRIX * matrix, 
								const D3DMATERIAL9 & material)
								: TransformNode( matrix )
			{
				m_mesh = mesh;
				m_mesh->AddRef();
				m_material = material;
			}

			virtual ~MeshNode(void)
			{
				d3d::Release<ID3DXMesh*>(m_mesh);
			}

			virtual HRESULT VRender(Scene *);

		};

	}//end namepsace scene
}//end namespace engine
