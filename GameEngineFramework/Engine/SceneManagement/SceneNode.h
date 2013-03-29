//==========================================
// 功能描述： 一个基本的场景图定义
// File : SceneNode.h
// 创建者：JB
// 创建日期：[5/1/2012]
// 修改日期：[5/1/2012]
//-----------------------------------------------------------------------------
#pragma once
#include "../../stdafx.h"
#include <boost/shared_ptr.hpp>

namespace
{
	
}

namespace Engine
{
	namespace SceneManagement
	{
		//定义存的类
		class SceneNode;
		class Scene;
		class TransformNode;
		class MovementController;

		class CameraNode;
		class Grid;
		class MeshNode;

		class SkyNode;
		class ArrowNode;

		typedef std::list<boost::shared_ptr<SceneNode>> SceneNodeList;


		

		//==========================================
		// 功能描述：	 SceneNode 的定义
		// 创建者：JB
		// 创建日期：[4/8/2012]
		// 修改日期：[4/8/2012]
		//-----------------------------------------------------------------------------
		class SceneNode
		{
		public:
			//子节点
			SceneNodeList	 m_children;

			virtual ~SceneNode();

			//渲染前
			virtual HRESULT VPreRender(Scene *) { return S_OK; }
			//渲染
			virtual HRESULT VRender(Scene *) { return S_OK; }
			//渲染后
			virtual HRESULT VPostRender(Scene *) { return S_OK; }

			//渲染子节点
			virtual HRESULT VRenderChildren(Scene *);
			
			//恢复设置
			virtual HRESULT VRestore(Scene *);

			//更新
			virtual HRESULT VUpdate(Scene *, DWORD const elapsedMs);

			// 设置矩阵变换
			virtual void VSetTransform(const D3DXMATRIX * /*toWorld*/, const D3DXMATRIX * /*fromWorld*/=NULL) { }
		
			//拷贝自己
			virtual SceneNode* Clone() const 
			{ return new SceneNode(*this); } 

			virtual boost::shared_ptr<SceneNode> CloneShared() const
			{
				return boost::shared_ptr<SceneNode>(new SceneNode(*this));
			}

		};


	}//end namespace scene
}//end namepsace engine