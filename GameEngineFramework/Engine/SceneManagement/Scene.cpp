#pragma once

#include "Scene.h"

namespace Engine
{
	namespace SceneManagement
	{
		Scene::Scene(IDirect3DDevice9 * device, boost::shared_ptr<SceneNode> root) 
			: m_root(root)
			//,m_pMovementController(NULL)
		{
			m_pDevice = device;
			m_pDevice->AddRef();

			D3DXCreateMatrixStack(0,  &m_matrixStack);
		}

		Scene::~Scene(void)
		{
			d3d::Release<IDirect3DDevice9*>( m_pDevice);
			d3d::Release<ID3DXMatrixStack*>(m_matrixStack);
		}
		
		HRESULT Scene::Render()
		{
			if ( !m_root )
				return S_OK;

			//场景根可以使任何东西，但常常是一个包含单位矩阵的 TransformNode
			
			if( m_root->VPreRender(this) == S_OK )
			{
				m_root->VRender(this);
				m_root->VRenderChildren(this);
				m_root->VPostRender(this);
			}

			return S_OK;
		}

		HRESULT Scene::Restore()
		{
			if ( !m_root )
				return S_OK;

			return m_root->VRestore(this);
		}

		HRESULT Scene::Update()
		{
			if ( !m_root )
				return S_OK;

			static DWORD lastTime = 0;
			DWORD elapsedTime = 0;
			DWORD now = timeGetTime();
			
			if ( 0 == lastTime )
			{
				 lastTime = now;
			}

			elapsedTime = now - lastTime;
			lastTime = now;

			//这是一个控制器，但它不是很有用，应该重新做。。
			if ( m_pMovementController )
				m_pMovementController->Update(elapsedTime);

			return m_root->VUpdate(this, elapsedTime);

		}

	}//end namepsace scene
}//end namespace engine