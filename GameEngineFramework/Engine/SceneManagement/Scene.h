//==========================================
// 功能描述：Scene 的定义 在SceneNode 中定义,一个含有层级的场景节点容器
// 创建者：JB
// 创建日期：[5/1/2012]
// 修改日期：[5/1/2012]
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
			//d3d 设备
			IDirect3DDevice9 * m_pDevice;
			//矩阵堆栈
			ID3DXMatrixStack * m_matrixStack;
			//场景子节点
			boost::shared_ptr<SceneNode> m_root;
			//运动控制器
			boost::shared_ptr<MovementController> m_pMovementController;

			//
			Scene(IDirect3DDevice9 * device, boost::shared_ptr<SceneNode> root);
			~Scene(void);

			//渲染
			HRESULT Render();
			//恢复
			HRESULT Restore();
			//更新
			HRESULT Update();

		};

	}//end namepsace scene
}//end namespace engine


