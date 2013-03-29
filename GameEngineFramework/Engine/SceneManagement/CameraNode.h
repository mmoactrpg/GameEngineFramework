//==========================================
// 功能描述：相机节点，控制可视的摄像机
// 创建者：JB
// 创建日期：[5/1/2012]
// 修改日期：[5/1/2012]
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
			//更新节点
			// elapsedMs : 更新消耗时间
			virtual HRESULT VUpdate(Scene *, DWORD const elapsedMs);

			~CameraNode(void);
		};
		
	}//end namepsace scene
}//end namespace engine
