#pragma once


#include "XMeshNode.h"

namespace Engine
{
	namespace SceneManagement
	{

		XMeshNode::XMeshNode(LPCWSTR  filePath, IDirect3DDevice9 *d3dDevice, const D3DXMATRIX * matrix, LPScaleMtrl scaleMtrl)
			: TransformNode( matrix )
		{
			//
			//m_scaleMtrl = 0;
			//
			//m_mesh->AddRef();

			hr = 0;
			//
			m_d3dDevice = d3dDevice;

			hr = D3DXLoadMeshFromX(
				filePath,
				D3DXMESH_MANAGED,
				d3dDevice,
				&adjBuffer,
				&mtrlBuffer,
				0,
				&numMtrls,
				&m_mesh);

			if(FAILED(hr))
			{
				::MessageBox(0, L"D3DXLoadMeshFromX() - FAILED", 0, 0);
				return;
			}

			if( mtrlBuffer != 0 && numMtrls != 0 )
			{
				D3DXMATERIAL* mtrls = (D3DXMATERIAL*)mtrlBuffer->GetBufferPointer();

				for(int i = 0; i < numMtrls; i++)
				{
					// the MatD3D property doesn't have an ambient value set
					// when its loaded, so set it now:
					mtrls[i].MatD3D.Ambient = mtrls[i].MatD3D.Diffuse;

					// save the ith material
					Mtrls.push_back( mtrls[i].MatD3D );

					// check if the ith material has an associative texture
					if( mtrls[i].pTextureFilename != 0 )
					{
						// yes, load the texture for the ith subset
						IDirect3DTexture9* tex = 0;

						//LPCSTR  t = (LPCSTR);
						D3DXCreateTextureFromFileA(
							m_d3dDevice,
							mtrls[i].pTextureFilename, //mtrls[i].pTextureFilename,
							&tex);

						// save the loaded texture
						Textures.push_back( tex );
					}
					else
					{
						// no texture for the ith subset
						Textures.push_back( 0 );
					}
				}
			}
			d3d::Release<ID3DXBuffer*>(mtrlBuffer); // done w/ buffer



			//
			// Optimize the mesh.
			//

			hr = m_mesh->OptimizeInplace(		
				D3DXMESHOPT_ATTRSORT |
				D3DXMESHOPT_COMPACT  |
				D3DXMESHOPT_VERTEXCACHE,
				(DWORD*)adjBuffer->GetBufferPointer(),
				0, 0, 0);

			d3d::Release<ID3DXBuffer*>(adjBuffer); // done w/ buffer

			if(FAILED(hr))
			{
				::MessageBox(0, L"OptimizeInplace() - FAILED", 0, 0);
				return;
			}
			
			//XMeshNode(m_mesh, matrix);


		}
		
		XMeshNode::XMeshNode(ID3DXMesh * mesh, std::vector<D3DMATERIAL9> mtrls,  std::vector<IDirect3DTexture9 *> textures, const D3DXMATRIX * matrix, int other)
			: TransformNode( matrix )
		{
			hr = 0;

			m_mesh = mesh;
			m_mesh->AddRef();

			Mtrls = mtrls;
			Textures = textures;

			//m_material = material;

			//
			// Extract the materials, and load textures.
			//
			


			//D3DXMatrixScaling(&ouTmatrix, scaleMtrl->x, scaleMtrl->y, scaleMtrl->z);

			//D3DXMatrixMultiply(&ouTmatrix, matrix, &ouTmatrix);



			//D3DXMatrixScaling()

			//
			// Set texture filters.
			//

			//d3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
			//d3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
			//d3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

			// 
			// Set Lights.
			//

			//D3DXVECTOR3 dir(1.0f, -1.0f, 1.0f);
			//D3DXCOLOR col(1.0f, 1.0f, 1.0f, 0.0f);
			//D3DLIGHT9 light = d3d::InitDirectionalLight(&dir, &col);


			//Device->SetLight(0, &light);
			//d3dDevice->LightEnable(0, true);
			//d3dDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
			//d3dDevice->SetRenderState(D3DRS_SPECULARENABLE, true);
		}

		/*void XMeshNode::setMatrix(const D3DXMATRIX * matrix)
		{
			
		}*/
		
		SceneNode *XMeshNode::Clone()
		{
			return new XMeshNode(*this);
		}

		boost::shared_ptr<SceneNode> XMeshNode::CloneShared()
		{
			return boost::shared_ptr<SceneNode>(new XMeshNode(*this));
			
			//return (boost::static_pointer_cast<Y, X>(CloneImpl())); // no need for dynamic_pointer_cast
		}



		HRESULT XMeshNode::VRender(Scene * pScene)
		{
			
			//pScene->m_pDevice->SetMaterial( &m_material);
			for(int i = 0; i < Mtrls.size(); i++)
			{
				pScene->m_pDevice->SetMaterial( &Mtrls[i] );
				pScene->m_pDevice->SetTexture(0, Textures[i]);
				m_mesh->DrawSubset(i);
			}	
			pScene->m_pDevice->SetTransform(D3DTS_WORLD, &ouTmatrix);
			return S_OK;
		}

	}//end namepsace scene
}//end namespace engine
