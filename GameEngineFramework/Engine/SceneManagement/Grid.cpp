#pragma once

#include "Grid.h"

#include "def.h"

namespace Engine
{
	//==========================================
	// 功能描述：	 Gird 实现
	// 创建者：JB
	// 创建日期：[4/8/2012]
	// 修改日期：[4/8/2012]
	//-----------------------------------------------------------------------------
	namespace SceneManagement
	{

		Grid::Grid(const DWORD gridSize,
			const DWORD color,
			const TCHAR * textureFile,
			const D3DXMATRIX * matrix): TransformNode( matrix )
		{
			m_gridSize = gridSize;
			m_color = color;
			m_textureFile = textureFile;

			m_pTexture = NULL;
			m_pVerts = NULL;
			m_pIndices = NULL;
			m_numVerts = m_numPolys = 0;
		}


		Grid::~Grid(void)
		{
			d3d::Release<LPDIRECT3DTEXTURE9>(m_pTexture);
			d3d::Release<LPDIRECT3DVERTEXBUFFER9>(m_pVerts);
			d3d::Release<LPDIRECT3DINDEXBUFFER9>(m_pIndices);
		}

		HRESULT Grid::VRestore(Scene * pScene)
		{
			// Call the base class's restore
			SceneNode::VRestore(pScene);

			LPDIRECT3DDEVICE9 pDevice = pScene->m_pDevice;
			if( FAILED( D3DXCreateTextureFromFile(
				pDevice,
				const_cast<TCHAR *>(m_textureFile),
				&m_pTexture )
				) )
				return E_FAIL;

			// Create the vertex buffer - we'll need enough verts
			// to populate the grid. If we want a 2x2 grid, we'll
			// need 3x3 set of verts.
			m_numVerts = (m_gridSize+1)*(m_gridSize+1);

			if( FAILED( pDevice->CreateVertexBuffer( 
				m_numVerts*sizeof(COLORED_TEXTURED_VERTEX),
				D3DUSAGE_WRITEONLY, D3DFVF_COLORED_TEXTURED_VERTEX,
				D3DPOOL_MANAGED, &m_pVerts, NULL ) ) )
			{
				return E_FAIL;
			}

			// Fill the vertex buffer. We are setting the tu and tv texture
			// coordinates, which range from 0.0 to 1.0
			COLORED_TEXTURED_VERTEX* pVertices;
			if( FAILED( m_pVerts->Lock( 0, 0, (void**)&pVertices, 0 ) ) )
				return E_FAIL;

			for( DWORD j=0; j<(m_gridSize+1); j++ )
			{
				for (DWORD i=0; i<(m_gridSize+1); i++)
				{
					// Which vertex are we setting?
					int index = i + (j * (m_gridSize+1) );
					COLORED_TEXTURED_VERTEX *vert = &pVertices[index];

					// Default position of the grid is at the origin, flat on
					// the XZ plane.
					float x = (float)i;
					float y = (float)j;
					vert->position = ( x * D3DXVECTOR3(1,0,0) ) + ( y * D3DXVECTOR3(0,0,1) );
					vert->color    = m_color;

					// The texture coordinates are set to x,y to make the
					// texture tile along with units - 1.0, 2.0, 3.0, etc.
					vert->tu       = x;
					vert->tv       = y;
				}
			}
			m_pVerts->Unlock();

			// The number of indicies equals the number of polygons times 3
			// since there are 3 indicies per polygon. Each grid square contains
			// two polygons. The indicies are 16 bit, since our grids won't
			// be that big!
			m_numPolys = m_gridSize*m_gridSize*2;
			if( FAILED( pDevice->CreateIndexBuffer(sizeof(WORD) * m_numPolys * 3,
				D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIndices, NULL ) ) )
			{
				return E_FAIL;
			}

			WORD *pIndices;
			if( FAILED( m_pIndices->Lock( 0, 0, (void**)&pIndices, 0 ) ) )
				return E_FAIL;

			// Loop through the grid squares and calc the values
			// of each index. Each grid square has two triangles:
			//
			//		A - B
			//		| / |
			//		C - D

			for( DWORD j=0; j<m_gridSize; j++ )
			{
				for (DWORD i=0; i<m_gridSize; i++)
				{
					// Triangle #1  ACB
					*(pIndices) = WORD(i + (j*(m_gridSize+1)));
					*(pIndices+1) = WORD(i + ((j+1)*(m_gridSize+1)));
					*(pIndices+2) = WORD((i+1) + (j*(m_gridSize+1)));

					// Triangle #2  BCD
					*(pIndices+3) = WORD((i+1) + (j*(m_gridSize+1)));
					*(pIndices+4) = WORD(i + ((j+1)*(m_gridSize+1)));
					*(pIndices+5) = WORD((i+1) + ((j+1)*(m_gridSize+1)));
					pIndices+=6;
				}
			}

			m_pIndices->Unlock();
			return S_OK;
		}

		HRESULT Grid::VRender(Scene * pScene)
		{
			// This is slightly different from the Chapter 7 implementation...

			LPDIRECT3DDEVICE9 pDevice = pScene->m_pDevice;

			DWORD oldLightMode;
			pDevice->GetRenderState( D3DRS_LIGHTING, &oldLightMode );
			pDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

			DWORD oldCullMode;
			pDevice->GetRenderState( D3DRS_CULLMODE, &oldCullMode );
			pDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

			// Setup our texture. Using textures introduces the texture stage states,
			// which govern how textures get blended together (in the case of multiple
			// textures) and lighting information. In this case, we are modulating
			// (blending) our texture with the diffuse color of the vertices.

			pDevice->SetTexture( 0, m_pTexture );
			pDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
			pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
			pDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );

			pDevice->SetStreamSource( 0, m_pVerts, 0, sizeof(COLORED_TEXTURED_VERTEX) );
			pDevice->SetIndices(m_pIndices);
			pDevice->SetFVF( D3DFVF_COLORED_TEXTURED_VERTEX );
			pDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST , 0, 0, m_numVerts, 0, m_numPolys );

			// Notice that the render states are returned to 
			// their original settings.....
			// Could there be a better way???

			pDevice->SetTexture (0, NULL);
			pDevice->SetRenderState( D3DRS_LIGHTING, oldLightMode );
			pDevice->SetRenderState( D3DRS_CULLMODE, oldCullMode );

			return S_OK;
		}

	}//end namepsace scene
}//end namespace engine
