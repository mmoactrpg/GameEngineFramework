//==========================================
// ���������� һ�������ĳ���ͼ����
// File : SceneNode.h
// �����ߣ�JB
// �������ڣ�[5/1/2012]
// �޸����ڣ�[5/1/2012]
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
		//��������
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
		// ����������	 SceneNode �Ķ���
		// �����ߣ�JB
		// �������ڣ�[4/8/2012]
		// �޸����ڣ�[4/8/2012]
		//-----------------------------------------------------------------------------
		class SceneNode
		{
		public:
			//�ӽڵ�
			SceneNodeList	 m_children;

			virtual ~SceneNode();

			//��Ⱦǰ
			virtual HRESULT VPreRender(Scene *) { return S_OK; }
			//��Ⱦ
			virtual HRESULT VRender(Scene *) { return S_OK; }
			//��Ⱦ��
			virtual HRESULT VPostRender(Scene *) { return S_OK; }

			//��Ⱦ�ӽڵ�
			virtual HRESULT VRenderChildren(Scene *);
			
			//�ָ�����
			virtual HRESULT VRestore(Scene *);

			//����
			virtual HRESULT VUpdate(Scene *, DWORD const elapsedMs);

			// ���þ���任
			virtual void VSetTransform(const D3DXMATRIX * /*toWorld*/, const D3DXMATRIX * /*fromWorld*/=NULL) { }
		
			//�����Լ�
			virtual SceneNode* Clone() const 
			{ return new SceneNode(*this); } 

			virtual boost::shared_ptr<SceneNode> CloneShared() const
			{
				return boost::shared_ptr<SceneNode>(new SceneNode(*this));
			}

		};


	}//end namespace scene
}//end namepsace engine