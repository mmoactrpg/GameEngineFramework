#pragma once

#include "TransformNode.h"


namespace Engine
{
	namespace SceneManagement
	{
		
		//==========================================
		// 功能描述：	 Transform 实现
		// 创建者：JB
		// 创建日期：[4/8/2012]
		// 修改日期：[4/8/2012]
		//-----------------------------------------------------------------------------
		TransformNode::~TransformNode(void)
		{
		}

		HRESULT TransformNode::VPreRender(Scene * pScene)
		{
			pScene->m_matrixStack->Push();
			pScene->m_matrixStack->MultMatrixLocal(&m_toWorld);
			pScene->m_pDevice->SetTransform(D3DTS_WORLD,
																				pScene->m_matrixStack->GetTop());

			return S_OK;
		}

		HRESULT TransformNode::VPostRender(Scene * pScene)
		{
			pScene->m_matrixStack->Pop();
			pScene->m_pDevice->SetTransform(D3DTS_WORLD,
																				pScene->m_matrixStack->GetTop());

			return S_OK;
		}


		void TransformNode::VSetTransform(const D3DXMATRIX * toWorld, const D3DXMATRIX * fromWorld)
		{
			m_toWorld = *toWorld;

			if ( !fromWorld )
			{
				D3DXMatrixInverse(&m_fromWorld, NULL, &m_toWorld);
			}
			else
			{
				m_fromWorld = *fromWorld;
			}
		}

		void TransformNode::VGetTransform(D3DXMATRIX * toWorld, D3DXMATRIX * fromWorld)
		{
			if (toWorld)
				*toWorld = m_toWorld;

			if (fromWorld)
				*fromWorld = m_fromWorld;
		}

	}//
}//