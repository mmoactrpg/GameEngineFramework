//==========================================
// 功能描述：	 箭头节点
// 创建者：JB
// 创建日期：[5/1/2012]
// 修改日期：[5/1/2012]
//-----------------------------------------------------------------------------
#pragma once

#include "../../stdafx.h"
#include "Scene.h"
#include "TransformNode.h"


namespace Engine
{
	namespace SceneManagement
	{

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

	}//end SceneManagement
}//end Engine