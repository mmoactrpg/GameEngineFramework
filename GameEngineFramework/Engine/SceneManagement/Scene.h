//==========================================
// ����������Scene �Ķ��� ��SceneNode �ж���,һ�����в㼶�ĳ����ڵ�����
// �����ߣ�JB
// �������ڣ�[5/1/2012]
// �޸����ڣ�[5/1/2012]
//-----------------------------------------------------------------------------

#pragma once

#include "../../stdafx.h"
#include "SceneNode.h"
#include "MovementController.h"

namespace Engine
{
	namespace SceneManagement
	{
		

		class Scene
		{
		public:
			//d3d �豸
			IDirect3DDevice9 * m_pDevice;
			//�����ջ
			ID3DXMatrixStack * m_matrixStack;
			//�����ӽڵ�
			boost::shared_ptr<SceneNode> m_root;
			//�˶�������
			boost::shared_ptr<MovementController> m_pMovementController;

			//
			Scene(IDirect3DDevice9 * device, boost::shared_ptr<SceneNode> root);
			~Scene(void);

			//��Ⱦ
			HRESULT Render();
			//�ָ�
			HRESULT Restore();
			//����
			HRESULT Update();

		};

	}//end namepsace scene
}//end namespace engine


