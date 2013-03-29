#pragma once

#include "../../stdafx.h"
#include "Scene.h"
#include "TransformNode.h"

namespace Engine
{
	namespace SceneManagement
	{
		//==========================================
		// 功能描述：	 IKeyboardSensitive & IMouseSensitive
		// 创建者：JB
		// 创建日期：[4/8/2012]
		// 修改日期：[4/8/2012]
		//-----------------------------------------------------------------------------

		class IKeyboardSensitive
		{
			virtual void OnKeyDown(const BYTE keycode) = 0;
			virtual void OnKeyUp( const BYTE keycode) = 0;
		};


		class IMouseSensitive
		{
		public:
			virtual void OnMouseMove(const POINTS &mousePos) = 0;
			virtual void OnLButtonDown(const POINTS &mousePos) = 0;
			virtual void OnLButtonUp(const POINTS &mousePos) = 0;
			virtual void OnRButtonDown(const POINTS &mousePos) = 0;
			virtual void OnRButtonUp(const POINTS &mousePos) = 0;
		};


		//==========================================
		// 功能描述：	 MovementController 定义
		// 创建者：JB
		// 创建日期：[4/8/2012]
		// 修改日期：[4/8/2012]
		//-----------------------------------------------------------------------------
		class MovementController 
			: public IMouseSensitive,
			  public IKeyboardSensitive
		{
		protected:
			D3DXMATRIX m_matFromWorld;
			D3DXMATRIX m_matToWorld;
			D3DXMATRIX m_matPosition;

			BOOL		m_bLeftMouseDown;
			POINTS		m_mousePos;					//当期啊鼠标位置
			POINTS		m_mousePosOnDown;	//收到按下时间鼠标的位置
			BYTE		m_bKey[256];					//切换按键起落状态

			//方向控制
			FLOAT m_fTargetYaw;
			FLOAT m_fTargetPitch;
			FLOAT m_fYaw;
			FLOAT m_fPitch;
			FLOAT m_fPitchOnDown;
			FLOAT m_fYawOnDwon;

			boost::shared_ptr<TransformNode> m_object;

		public:
			MovementController(boost::shared_ptr<TransformNode> object, float initialYaw, float initialPitch);
			void SetObject( boost::shared_ptr<TransformNode> newObject);
			void Update(DWORD const elapsedMs);

			void OnMouseMove(const POINTS &mousePos);
			void OnLButtonDown(const POINTS &mousePos);
			void OnLButtonUp(const POINTS &mousePos);
			void OnRButtonDown(const POINTS &mousePos);
			void OnRButtonUp(const POINTS &mousePos);

			void OnKeyDown(const BYTE keycode) { m_bKey[keycode] = true; }
			void OnKeyUp( const BYTE keycode) {  m_bKey[keycode] = false; }

			const D3DXMATRIX * GetToWorld() { return &m_matToWorld; }
			const D3DXMATRIX * GetFromWorld() { return &m_matFromWorld; }
		};

	}//end namepsace scene
}//end namespace engine