//==========================================
// ����������	 �����ʵ�����ڵ����
// �����ߣ�JB
// �������ڣ�[5/1/2012]
// �޸����ڣ�[5/1/2012]
//-----------------------------------------------------------------------------
#pragma once

#include "../../stdafx.h"
#include "TransformNode.h"
#include <vector>

namespace Engine
{
	namespace SceneManagement
	{
		typedef struct _ScaleMtrl
		{
			FLOAT x;
			FLOAT y;
			FLOAT z;
		}ScaleMtrl, *LPScaleMtrl;

		class XMeshNode : public TransformNode
		{			
		protected:
			IDirect3DDevice9 *m_d3dDevice;
			

			//
			ID3DXBuffer* adjBuffer;
			ID3DXBuffer* mtrlBuffer;
			DWORD        numMtrls;

			LPScaleMtrl m_scaleMtrl;

			D3DXMATRIX  ouTmatrix;

			HRESULT hr;
			
		public:

			//����
			ID3DXMesh * m_mesh;
			//����
			//D3DMATERIAL9 m_material;
			//��������

			std::vector<D3DMATERIAL9> Mtrls;
			//��ͼ����
			std::vector<IDirect3DTexture9 *> Textures;

			XMeshNode(LPCWSTR  filePath, IDirect3DDevice9 *d3dDevice, const D3DXMATRIX * matrix, LPScaleMtrl scaleMtrl);
			//{
				
				
				//LoadBaseSrc(filePath);
				
				/*m_mesh = mesh;
				m_mesh->AddRef();
				LoadBaseSrc()
				m_material = material;*/
			//}

			//void LoadBaseSrc(std::wstring * filePath, IDirect3DDevice9 *d3dDevice);

			/*void setMatrix(const D3DXMATRIX * matrix);*/

			XMeshNode(ID3DXMesh * mesh, std::vector<D3DMATERIAL9> mtrls,  std::vector<IDirect3DTexture9 *> textures, const D3DXMATRIX * matrix, int other);
			//	: TransformNode( matrix )
			//{
			//	m_mesh = mesh;
			//	m_mesh->AddRef();
			//	//m_material = material;
			//}

			


			SceneNode *Clone();
			boost::shared_ptr<SceneNode> CloneShared();

			virtual ~XMeshNode(void)
			{
				d3d::Release<ID3DXMesh*>(m_mesh);
				for(int i = 0; i < Textures.size(); i++)
					d3d::Release<IDirect3DTexture9*>( Textures[i] );
			}

			virtual HRESULT VRender(Scene *);

		};
	}
}//end namespace engine
