#pragma once

#include "MovementController.h"


//==========================================
// 功能描述：	 MovementController 实现
// 创建者：JB
// 创建日期：[4/8/2012]
// 修改日期：[4/8/2012]
//-----------------------------------------------------------------------------

namespace Engine
{
	namespace SceneManagement
	{

		MovementController::MovementController(boost::shared_ptr<TransformNode> object, float initialYaw, float initialPitch)
			: m_object(object)
		{
			m_object->VGetTransform(&m_matToWorld, &m_matFromWorld);

			m_fTargetYaw = m_fYaw = RADIANS_TO_DEGREES(-initialYaw);
			m_fTargetPitch = m_fPitch = RADIANS_TO_DEGREES(initialPitch);

			D3DXVECTOR4 pos = D3DXVECTOR4(m_matToWorld(3,0), m_matToWorld(3,1), m_matToWorld(3,2), m_matToWorld(3,3));
			D3DXMatrixTranslation(&m_matPosition,pos.x,pos.y,pos.z);

			m_bLeftMouseDown = false;

			memset(m_bKey, 0x00, sizeof(m_bKey));


		}

		void MovementController::OnMouseMove(const POINTS &mousePos)
		{
			if( m_bLeftMouseDown )
			{
				//每次鼠标移动，找出各个方向运动了多元。x 轴使之偏转， y 轴使之倾斜
				m_fTargetYaw = m_fYawOnDwon + ( m_mousePosOnDown.x - mousePos.x  );
				m_fTargetPitch = m_fPitchOnDown + ( mousePos.y - m_mousePosOnDown.y );
			}
		}

		//鼠标被按下——记录下再那里发生的
		void MovementController::OnLButtonDown(const POINTS &mousePos)
		{
			m_bLeftMouseDown = true;
			m_mousePosOnDown = mousePos;
			m_fYawOnDwon = m_fTargetYaw;
			m_fPitchOnDown = m_fTargetPitch;
		}

		void MovementController::OnLButtonUp(const POINTS &mousePos)
		{
			m_bLeftMouseDown = false;
		}


		//鼠标被按下——记录下再那里发生的
		void MovementController::OnRButtonDown(const POINTS &mousePos)
		{
			m_bLeftMouseDown = true;
			m_mousePosOnDown = mousePos;
			m_fYawOnDwon = m_fTargetYaw;
			m_fPitchOnDown = m_fTargetPitch;
		}

		void MovementController::OnRButtonUp(const POINTS &mousePos)
		{
			m_bLeftMouseDown = false;
		}

		void MovementController::Update(DWORD const elapsedMs)
		{

			if (m_bKey['Q'])
			{
				D3DXMATRIX camTranslate;
				D3DXMatrixTranslation(&m_matPosition, 8.847f, 7.055f, 11.618f);

				// The secret formula!!! Don't give it away!
				m_fTargetYaw = m_fYaw += -64.35f;
				m_fTargetPitch = m_fPitch = 28.57f;

				// Calculate the new rotation matrix from the camera
				// yaw and pitch.
				D3DXMATRIX matRot;
				D3DXMatrixRotationYawPitchRoll(&matRot, -m_fYaw * D3DX_PI / 180, m_fPitch * D3DX_PI / 180, 0);

				// Create the new object-to-world matrix, and the
				// new world-to-object matrix. 
				D3DXMatrixMultiply(&m_matToWorld, &matRot, &m_matPosition);
				D3DXMatrixInverse(&m_matFromWorld, NULL, &m_matToWorld);

				m_object->VSetTransform(&m_matToWorld, &m_matFromWorld);
			}

			if ( m_bKey['W'] || m_bKey['S'] )
			{
				// D3D 中，默认的“观察发现” 总是 z 轴正方向
				D3DXVECTOR4 at = D3DXVECTOR4( 0.0f, 0.0f, 1.0f, 0.0);

				if( m_bKey['S'] )
					at *= -1;
				D3DXVECTOR4 atWorld( 0, 0, 0, 0 );

				//计算出世界空间中“观察方向”向量，我们将用它移动摄像机
				D3DXVec4Transform( &atWorld, &at, &m_matToWorld );

				//但一次 不用移动整整 1米
				atWorld *= 0.5f;

				D3DXMATRIX camTranslate;
				D3DXMatrixTranslation( &camTranslate, atWorld.x, atWorld.y, atWorld.z);

				//新创建的的位置变动矩阵——camTransform，和位置矩阵相连接
				D3DXMatrixMultiply( &m_matPosition, &m_matPosition, &camTranslate);

			}//end w s 



			if ( m_bKey['A'] || m_bKey['D'] )
			{
				// D3D 中，默认的“观察发现” 总是 z 轴正方向
				D3DXVECTOR4 at = D3DXVECTOR4( 1.0f, 0.0f, 0.0f, 0.0);

				if( m_bKey['A'] )
					at *= -1;
				D3DXVECTOR4 atWorld( 0, 0, 0, 0 );

				//计算出世界空间中“观察方向”向量，我们将用它移动摄像机
				D3DXVec4Transform( &atWorld, &at, &m_matToWorld );

				//但一次 不用移动整整 1米
				atWorld *= 0.5f;

				D3DXMATRIX camTranslate;
				D3DXMatrixTranslation( &camTranslate, atWorld.x, atWorld.y, atWorld.z);

				//新创建的的位置变动矩阵——camTransform，和位置矩阵相连接
				D3DXMatrixMultiply( &m_matPosition, &m_matPosition, &camTranslate);

			}//end A D



		/*	if ( m_bLeftMouseDown )
			{*/
				//神秘公式 别泄密！
				m_fYaw += (  m_fTargetYaw - m_fYaw ) * ( 0.35f );
				m_fTargetPitch = MAX( -90, MIN( 90, m_fTargetPitch ));
				m_fPitch += ( m_fTargetPitch - m_fPitch ) * ( 0.35f );

				//从摄像机偏转和倾斜算出新的旋转矩阵
				D3DXMATRIX matRot;
				D3DXMatrixRotationYawPitchRoll( 
					&matRot,
					-m_fYaw * D3DX_PI  / 180,			//倾斜
					m_fPitch * D3DX_PI / 180,			//偏转
					0);													//翻滚

				//创建新的物体——世界矩阵，和新的世界——物体矩阵
				D3DXMatrixMultiply( &m_matToWorld, &matRot, &m_matPosition);
				D3DXMatrixInverse( &m_matFromWorld, NULL, &m_matToWorld);

				m_object->VSetTransform( &m_matToWorld, &m_matFromWorld);
			/*}*/

		}//end update

	}//end namepsace scene
}//end namespace engine