//==========================================
// 功能描述：天空盒节点
// 创建者：JB
// 创建日期：[5/1/2012]
// 修改日期：[5/1/2012]
//-----------------------------------------------------------------------------
#pragma once
#include "../../stdafx.h"
#include "Scene.h"
#include "TransformNode.h"
#include "CameraNode.h"

namespace Engine
{
	namespace SceneManagement
	{

		class SkyNode : public TransformNode
		{
		protected:
			LPDIRECT3DTEXTURE9		m_pTexture[5];		// the sky textures
			LPDIRECT3DVERTEXBUFFER9 m_pVerts;			// the sky verts
			DWORD					m_numVerts;
			DWORD					m_sides;
			const TCHAR *			m_textureBaseName;
			boost::shared_ptr<CameraNode>	m_camera;

		public:
			SkyNode(const TCHAR *textureFile, boost::shared_ptr<CameraNode> camera);
			~SkyNode();
			HRESULT VRestore(Scene *pScene);
			HRESULT VRender(Scene *pScene);
			HRESULT VPreRender(Scene *pScene);
		};

	}//end SceneManagement
}//end Engine