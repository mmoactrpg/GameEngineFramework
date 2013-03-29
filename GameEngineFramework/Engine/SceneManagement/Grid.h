//==========================================
// 功能描述：	 一个网格节点
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

		//==========================================
		// 功能描述：	 Grid 定义
		// 创建者：JB
		// 创建日期：[4/8/2012]
		// 修改日期：[4/8/2012]
		//-----------------------------------------------------------------------------
		class Grid :
			public TransformNode
		{
		protected:
			LPDIRECT3DTEXTURE9				m_pTexture;		//gird 纹理
			LPDIRECT3DVERTEXBUFFER9		m_pVerts;			//grid 顶点	
			LPDIRECT3DINDEXBUFFER9		m_pIndices;		//gird 索引

			DWORD	m_numVerts;
			DWORD m_numPolys;
			DWORD m_gridSize;
			DWORD m_color;
			const TCHAR * m_textureFile;

		public:
			Grid(const DWORD gridSize, const DWORD color,
				const TCHAR * textureFile, const D3DXMATRIX * matrix);
			~Grid(void);

			HRESULT VRestore(Scene * pScene);
			HRESULT VRender(Scene * pScene);

			//struct COLORED_TEXTURED_VERTEX
			//{
			//	D3DXVECTOR3 position;           // The position
			//	D3DXVECTOR3 normal;     // The normal
			//	D3DCOLOR        color;                  // The color
			//	FLOAT           tu, tv;                         // The texture coordinates
			//};
		};

	}//end namepsace scene
}//end namespace engine
