//==========================================
// ����������	 ʹ��D3D��ջ����������塾�ֲ����������硿�ľ���任
// �����ߣ�JB
// �������ڣ�[5/1/2012]
// �޸����ڣ�[5/1/2012]
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
			//��������
			D3DXMATRIX m_toWorld;
			//�ֲ�����
			D3DXMATRIX m_fromWorld;

			TransformNode(
				const D3DXMATRIX * toWorld,
				const D3DXMATRIX * fromWorld = NULL)
			{ VSetTransform(toWorld, fromWorld); }

			~TransformNode(void);

			//��Ⱦǰ
			virtual HRESULT VPreRender(Scene *);
			//��Ⱦ��
			virtual HRESULT VPostRender(Scene *);

			//���оֲ�����������ľ���任
			virtual void VSetTransform(
				const D3DXMATRIX * toWorld,
				const D3DXMATRIX * fromWorld = NULL);

			//��ȡ��������ͱ任����ľ���
			virtual void VGetTransform(
				D3DXMATRIX * toWorld,
				D3DXMATRIX * fromWorld);

		};

	}
}
