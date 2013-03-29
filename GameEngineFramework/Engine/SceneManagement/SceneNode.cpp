#pragma once

#include "SceneNode.h"

namespace Engine
{
	namespace SceneManagement
	{

		//==========================================
		// 功能描述：	 SceneNode 实现
		// 创建者：JB
		// 创建日期：[4/8/2012]
		// 修改日期：[4/8/2012]
		//-----------------------------------------------------------------------------

		SceneNode::~SceneNode()
		{
			//摆脱这些讨厌的孩子
			while ( !m_children.empty() )
			{
				m_children.pop_front();
			}
		}

		//任何从 SceneNode 派生并且重载 VRestore() 函数的类都应该调用这个函数
		HRESULT SceneNode::VRestore(Scene * pScene)
		{	
			SceneNodeList::iterator i = m_children.begin();

			while ( i != m_children.end() )
			{
				(*i)->VRestore(pScene);
				i++;
			}

			return S_OK;
		}

		//任何从 SceneNode 派生并且重载 VUpdate() 函数的类都应该调用这个函数
		HRESULT SceneNode::VUpdate(Scene * pScene, DWORD const elapsedMs)
		{
			SceneNodeList::iterator i = m_children.begin();

			while ( i != m_children.end() )
			{
				(*i)->VUpdate(pScene, elapsedMs);
				i++;
			}

			return S_OK;
		}

		//遍历子节点...
		HRESULT SceneNode::VRenderChildren(Scene * pScene)
		{
			SceneNodeList::iterator i = m_children.begin();

			while ( i != m_children.end() )
			{
				if ( (*i)->VPreRender(pScene) == S_OK )
				{
					//如果一个对象调用了 VPreRender 返回 E_FAIL, 则跳过渲染
					(*i)->VRender(pScene);
					(*i)->VRenderChildren(pScene);
				}

				(*i)->VPostRender(pScene);
				i++;
			}

			return S_OK;
		}

	}
}
