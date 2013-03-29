//==========================================
// 功能描述：	 使用D3D堆栈矩阵进行物体【局部】到【世界】的矩阵变换
// 创建者：JB
// 创建日期：[5/1/2012]
// 修改日期：[5/1/2012]
//-----------------------------------------------------------------------------
#pragma once

#include "../../stdafx.h"

#include "SceneNode.h"
#include "Scene.h"

namespace Engine
{
	namespace SceneManagement
	{
		class TransformNode : public SceneNode 
		{
		public:
			//世界坐标
			D3DXMATRIX m_toWorld;
			//局部坐标
			D3DXMATRIX m_fromWorld;

			TransformNode(
				const D3DXMATRIX * toWorld,
				const D3DXMATRIX * fromWorld = NULL)
			{ VSetTransform(toWorld, fromWorld); }

			~TransformNode(void);

			//渲染前
			virtual HRESULT VPreRender(Scene *);
			//渲染后
			virtual HRESULT VPostRender(Scene *);

			//进行局部到世界坐标的矩阵变换
			virtual void VSetTransform(
				const D3DXMATRIX * toWorld,
				const D3DXMATRIX * fromWorld = NULL);

			//获取世界坐标和变换坐标的矩阵
			virtual void VGetTransform(
				D3DXMATRIX * toWorld,
				D3DXMATRIX * fromWorld);

		};

	}
}
