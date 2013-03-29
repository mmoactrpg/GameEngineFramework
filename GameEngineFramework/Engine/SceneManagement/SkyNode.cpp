#include "SkyNode.h"
#include "def.h"

////////////////////////////////////////////////////
// SkyNode Implementation
//
// This code wasn't finished at the time my deadline for
// Chapter 10 drew near, so I bailed. A sky box is a nice 
// special effect that makes the clouds look like they are 
// very away, and essentailly look like they are following you.
//
// To achieve the effect, build a largeish cube 
// without a bottom, put a nice sky texture on it, and move it
// exactly in tandem with the movements of the camera.
// 
// It was broken, sadly, and I never finished it. Perhaps 
// you can make it work!
////////////////////////////////////////////////////

namespace Engine
{
	namespace SceneManagement
	{

		SkyNode::SkyNode(const TCHAR *pTextureBaseName, boost::shared_ptr<CameraNode> camera)
			: TransformNode(D3DXMatrixIdentity(&m_toWorld))
			, m_camera(camera)
		{
			m_textureBaseName = pTextureBaseName;
			for (int i=0; i<5; ++i)
			{
				m_pTexture[i] = NULL;
			}
		}


		SkyNode::~SkyNode()
		{
			for (int i=0; i<5; ++i)
			{
				SAFE_RELEASE(m_pTexture[i]);
			}
			SAFE_RELEASE(m_pVerts);
		}

		HRESULT SkyNode::VRestore(Scene *pScene)
		{
			// Call the base class's restore
			SceneNode::VRestore(pScene);

			LPDIRECT3DDEVICE9 pDevice = pScene->m_pDevice;

			const TCHAR *suffix[] = { L"_n.jpg", L"_e.jpg",  L"_s.jpg",  L"_w.jpg",  L"_u.jpg" };
			for (int i=0; i<5; ++i)
			{
				TCHAR name[256];
				_tcscpy(name, _T("Textures\\"));
				_tcscat(name, m_textureBaseName);
				_tcscat(name, suffix[i]);
				//HRESULT rs = D3DUtil_CreateTexture( pDevice, name, &m_pTexture[i] );

				if( FAILED( D3DXCreateTextureFromFile(
					pDevice,
					name,
					&m_pTexture[i])  ) )
					return E_FAIL;
			}

			m_numVerts = 20;

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

			// Loop through the grid squares and calc the values
			// of each index. Each grid square has two triangles:
			//
			//		A - B
			//		| / |
			//		C - D

			COLORED_TEXTURED_VERTEX skyVerts[4];
			D3DCOLOR skyVertColor = 0xffffffff;
			float dim = 50.0f;

			skyVerts[0].position = D3DXVECTOR3( dim, dim, dim ); skyVerts[0].color=skyVertColor; skyVerts[0].tu=1; skyVerts[0].tv=0; 
			skyVerts[1].position = D3DXVECTOR3(-dim, dim, dim ); skyVerts[1].color=skyVertColor; skyVerts[1].tu=0; skyVerts[1].tv=0; 
			skyVerts[2].position = D3DXVECTOR3( dim,-dim, dim ); skyVerts[2].color=skyVertColor; skyVerts[2].tu=1; skyVerts[2].tv=1; 
			skyVerts[3].position = D3DXVECTOR3(-dim,-dim, dim ); skyVerts[3].color=skyVertColor; skyVerts[3].tu=0; skyVerts[3].tv=1; 

			D3DXMATRIX rotY;
			D3DXMatrixRotationY(&rotY, D3DX_PI/2.0f);
			D3DXMATRIX rotX;
			D3DXMatrixRotationX(&rotX,  - D3DX_PI/2.0f);

			m_sides = 5;

			for (DWORD side = 0; side < m_sides; side++)
			{
				for (DWORD v = 0; v < 4; v++)
				{
					D3DXVECTOR4 temp;
					if (side < m_sides-1)
					{
						D3DXVec3Transform(&temp, &skyVerts[v].position, &rotY);
					}
					else
					{
						skyVerts[0].tu=1; skyVerts[0].tv=1; 
						skyVerts[1].tu=1; skyVerts[1].tv=0; 
						skyVerts[2].tu=0; skyVerts[2].tv=1; 
						skyVerts[3].tu=0; skyVerts[3].tv=0; 

						D3DXVec3Transform(&temp, &skyVerts[v].position, &rotX);
					}
					skyVerts[v].position = D3DXVECTOR3(temp.x, temp.y, temp.z);
				}
				memcpy(&pVertices[side*4], skyVerts, sizeof(skyVerts));
			}

			m_pVerts->Unlock();
			return S_OK;
		}

		HRESULT SkyNode::VPreRender(Scene *pScene)
		{
			D3DXMATRIX cameraTrans, skyTrans;
			m_camera->VGetTransform(&cameraTrans, NULL);

			D3DXVECTOR4 pos = D3DXVECTOR4(cameraTrans(3,0), cameraTrans(3,1), cameraTrans(3,2), cameraTrans(3,3));
			D3DXMatrixTranslation(&skyTrans,pos.x,pos.y,pos.z);

			VSetTransform(&skyTrans);

			return TransformNode::VPreRender(pScene);
		}

		HRESULT SkyNode::VRender(Scene *pScene)
		{
			LPDIRECT3DDEVICE9 pDevice = pScene->m_pDevice;

			DWORD oldLightMode;
			pDevice->GetRenderState( D3DRS_LIGHTING, &oldLightMode );
			pDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

			DWORD oldCullMode;
			pDevice->GetRenderState( D3DRS_CULLMODE, &oldCullMode );
			pDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );


			//防止z-buffer写入，主要是使天空盒看起来有无限远的结果
			//pDevice->SetRenderState(D3DRS_ZWRITEENABLE,false);
			//pDevice->SetRenderState(D3DRS_ZENABLE, false);
			//利用夹取纹理寻址的方式来实现天空盒的无缝连接
			pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
			pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);


			// Setup our texture. Using textures introduces the texture stage states,
			// which govern how textures get blended together (in the case of multiple
			// textures) and lighting information. In this case, we are modulating
			// (blending) our texture with the diffuse color of the vertices.

			pDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
			pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
			pDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );

			pDevice->SetStreamSource( 0, m_pVerts, 0, sizeof(COLORED_TEXTURED_VERTEX) );
			pDevice->SetFVF( D3DFVF_COLORED_TEXTURED_VERTEX);



			for (DWORD side = 0; side < m_sides; side++)
			{
				//pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);
				//pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR);

				pDevice->SetTexture( 0, m_pTexture[side] );
				pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP , 4 * side, 2);
			}

			// Notice that the render states are returned to 
			// their original settings.....
			// Could there be a better way???

			pDevice->SetTexture (0, NULL);
			pDevice->SetRenderState( D3DRS_LIGHTING, oldLightMode );
			pDevice->SetRenderState( D3DRS_CULLMODE, oldCullMode );

			//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
			//pDevice->SetRenderState(D3DRS_ZENABLE, true);
			pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
			pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

			return S_OK;
		}


	}//end SceneManagement
}//end Engine