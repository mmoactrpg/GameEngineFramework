#pragma once

#include "SceneNode.h"

namespace Engine
{
	namespace SceneManagement
	{

		//==========================================
		// ����������	 SceneNode ʵ��
		// �����ߣ�JB
		// �������ڣ�[4/8/2012]
		// �޸����ڣ�[4/8/2012]
		//-----------------------------------------------------------------------------

		SceneNode::~SceneNode()
		{
			//������Щ����ĺ���
			while ( !m_children.empty() )
			{
				m_children.pop_front();
			}
		}

		//�κδ� SceneNode ������������ VRestore() �������඼Ӧ�õ����������
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

		//�κδ� SceneNode ������������ VUpdate() �������඼Ӧ�õ����������
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

		//�����ӽڵ�...
		HRESULT SceneNode::VRenderChildren(Scene * pScene)
		{
			SceneNodeList::iterator i = m_children.begin();

			while ( i != m_children.end() )
			{
				if ( (*i)->VPreRender(pScene) == S_OK )
				{
					//���һ����������� VPreRender ���� E_FAIL, ��������Ⱦ
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
