#pragma once

#include "MovementController.h"


//==========================================
// ����������	 MovementController ʵ��
// �����ߣ�JB
// �������ڣ�[4/8/2012]
// �޸����ڣ�[4/8/2012]
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
				//ÿ������ƶ����ҳ����������˶��˶�Ԫ��x ��ʹ֮ƫת�� y ��ʹ֮��б
				m_fTargetYaw = m_fYawOnDwon + ( m_mousePosOnDown.x - mousePos.x  );
				m_fTargetPitch = m_fPitchOnDown + ( mousePos.y - m_mousePosOnDown.y );
			}
		}

		//��걻���¡�����¼�������﷢����
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


		//��걻���¡�����¼�������﷢����
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
				// D3D �У�Ĭ�ϵġ��۲췢�֡� ���� z ��������
				D3DXVECTOR4 at = D3DXVECTOR4( 0.0f, 0.0f, 1.0f, 0.0);

				if( m_bKey['S'] )
					at *= -1;
				D3DXVECTOR4 atWorld( 0, 0, 0, 0 );

				//���������ռ��С��۲췽�����������ǽ������ƶ������
				D3DXVec4Transform( &atWorld, &at, &m_matToWorld );

				//��һ�� �����ƶ����� 1��
				atWorld *= 0.5f;

				D3DXMATRIX camTranslate;
				D3DXMatrixTranslation( &camTranslate, atWorld.x, atWorld.y, atWorld.z);

				//�´����ĵ�λ�ñ䶯���󡪡�camTransform����λ�þ���������
				D3DXMatrixMultiply( &m_matPosition, &m_matPosition, &camTranslate);

			}//end w s 



			if ( m_bKey['A'] || m_bKey['D'] )
			{
				// D3D �У�Ĭ�ϵġ��۲췢�֡� ���� z ��������
				D3DXVECTOR4 at = D3DXVECTOR4( 1.0f, 0.0f, 0.0f, 0.0);

				if( m_bKey['A'] )
					at *= -1;
				D3DXVECTOR4 atWorld( 0, 0, 0, 0 );

				//���������ռ��С��۲췽�����������ǽ������ƶ������
				D3DXVec4Transform( &atWorld, &at, &m_matToWorld );

				//��һ�� �����ƶ����� 1��
				atWorld *= 0.5f;

				D3DXMATRIX camTranslate;
				D3DXMatrixTranslation( &camTranslate, atWorld.x, atWorld.y, atWorld.z);

				//�´����ĵ�λ�ñ䶯���󡪡�camTransform����λ�þ���������
				D3DXMatrixMultiply( &m_matPosition, &m_matPosition, &camTranslate);

			}//end A D



		/*	if ( m_bLeftMouseDown )
			{*/
				//���ع�ʽ ��й�ܣ�
				m_fYaw += (  m_fTargetYaw - m_fYaw ) * ( 0.35f );
				m_fTargetPitch = MAX( -90, MIN( 90, m_fTargetPitch ));
				m_fPitch += ( m_fTargetPitch - m_fPitch ) * ( 0.35f );

				//�������ƫת����б����µ���ת����
				D3DXMATRIX matRot;
				D3DXMatrixRotationYawPitchRoll( 
					&matRot,
					-m_fYaw * D3DX_PI  / 180,			//��б
					m_fPitch * D3DX_PI / 180,			//ƫת
					0);													//����

				//�����µ����塪��������󣬺��µ����硪���������
				D3DXMatrixMultiply( &m_matToWorld, &matRot, &m_matPosition);
				D3DXMatrixInverse( &m_matFromWorld, NULL, &m_matToWorld);

				m_object->VSetTransform( &m_matToWorld, &m_matFromWorld);
			/*}*/

		}//end update

	}//end namepsace scene
}//end namespace engine