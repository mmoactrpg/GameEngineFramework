//==========================================
// 功能描述：	 带材质的网格节点对象
// 创建者：JB
// 创建日期：[5/1/2012]
// 修改日期：[5/1/2012]
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
			//网格
			ID3DXMesh * m_mesh;
			//材质
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
