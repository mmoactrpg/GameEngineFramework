//==========================================
// ��������������ڵ㣬���ƿ��ӵ������
// �����ߣ�JB
// �������ڣ�[5/1/2012]
// �޸����ڣ�[5/1/2012]
//-----------------------------------------------------------------------------
#pragma once

#include "../../stdafx.h"
#include "TransformNode.h"

namespace Engine
{
	namespace SceneManagement
	{

		class CameraNode : public TransformNode
		{
		public:
			CameraNode(const D3DXMATRIX * matrix ) : TransformNode(matrix) { }
			//���½ڵ�
			// elapsedMs : ��������ʱ��
			virtual HRESULT VUpdate(Scene *, DWORD const elapsedMs);

			~CameraNode(void);
		};
		
	}//end namepsace scene
}//end namespace engine
