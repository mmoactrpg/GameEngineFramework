#include "ArrowNode.h"

namespace Engine
{
	namespace SceneManagement
	{

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

	}//end SceneManagement
}//end Engine