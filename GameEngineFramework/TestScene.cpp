#pragma once

#include "TestScene.h"

using namespace d3d;

TestScene::TestScene(void)
{
}


TestScene::~TestScene(void)
{
}
/**/
Scene * TestScene::BuildScene(IDirect3DDevice9 * d3dDevice)
{
	//����һЩ��ɫ���ʣ��Ա���ʾ��ͬ����ɫ����

	D3DMATERIAL9 colors[5];
	colors[0] = InitMtrl(WHITE, WHITE, WHITE, BLACK, 2.0f);
	colors[1] = InitMtrl(RED, RED, RED, BLACK, 2.0f);
	colors[2] = InitMtrl(GREEN, GREEN, GREEN, BLACK, 2.0f);
	colors[3] = InitMtrl(BLUE, BLUE, BLUE, BLACK, 2.0f);
	colors[4] = InitMtrl(YELLOW, YELLOW, YELLOW, BLACK, 2.0f);

	//��λ�����������õ�
	D3DXMATRIX ident;
	D3DXMatrixIdentity(&ident);

	//���ǽ���һЩ������������ʹ����Щ��ת
	D3DXMATRIX rotateX, rotateZ;
	D3DXMatrixRotationZ ( &rotateZ, D3DX_PI / 2.0f );
	D3DXMatrixRotationX( &rotateX, -D3DX_PI / 2.0f );

	//�������ڵ�������
	SmartPtr<SceneNode> root (new TransformNode(&ident));
	SmartPtr<SceneNode> camera ( new CameraNode(&ident));
	root->m_children.push_back(camera);

	//���
	ID3DXMesh * teapot;
	if ( SUCCEEDED( D3DXCreateTeapot( d3dDevice, &teapot, NULL) ) )
	{
		//���#1 ��ɫ��x=6, y=2, z=4��
		D3DXMATRIX trans;
		D3DXMatrixTranslation(&trans, 6, 2, 4);
		
		SmartPtr<SceneNode> mesh1 ( new MeshNode(teapot, &trans, colors[0] ));
		root->m_children.push_back(mesh1);

		//���#2 ��ɫ (x=3,y=2,z=1)
		D3DXMatrixTranslation( &trans, 3, 2, 1 );
		D3DXMATRIX result;
		D3DXMatrixMultiply( &result, &rotateZ, &trans );

		SmartPtr<SceneNode> mesh2 (new MeshNode(teapot, &result, colors[1] ));
		root->m_children.push_back(mesh2);

		//�ͷŲ����mesh1 �� mesh2 �Ѿ���������
		Release<ID3DXMesh * >(teapot);
	}//end teapot

	//
	ID3DXMesh * sphere;
	if( SUCCEEDED( D3DXCreateSphere( d3dDevice, .25, 16, 16, &sphere, NULL ) ) )
	{
		//���ǽ�����һ�������ߵ����壬�ӣ�x=3, y=0, z=3����ʼ���� x ������
		D3DXMATRIX trans;
		D3DXMatrixTranslation( &trans, 3, 0, 3);

		SmartPtr<SceneNode> sphere1 ( new MeshNode( sphere, &trans, colors[4] ) );
		root->m_children.push_back(sphere1);

		//�����Ǿֲ���ת������
		//������Χ�� y ����ת Ȼ����y ������ƽ�ƣ��� z ����ǰƽ��
		D3DXMATRIX rotateY;
		D3DXMatrixRotationY(&rotateY, D3DX_PI / 8.0f);
		D3DXMATRIX trans2;
		D3DXMatrixTranslation(&trans2, 0, 0.5, 0.5);
		D3DXMATRIX result;
		D3DXMatrixMultiply( &result, &trans2, &rotateY);

		for ( int i =0; i < 25; i++)
		{
			SmartPtr<SceneNode> sphere2 ( new MeshNode(sphere, &result, colors[i%5] ) );
			sphere1->m_children.push_back(sphere2);
			sphere1 = sphere2;
		}

		Release<ID3DXMesh*>(sphere);

	}//end sphere

	/*TCHAR * filename = (TCHAR*)"grid.dss";

	SmartPtr<SceneNode> grid1 ( 
		new Grid(40, 0x00400000, filename, &ident) );
	root->m_children.push_back(grid1);

	SmartPtr<SceneNode> grid2 ( 
		new Grid(40, 0x00004000, filename, &rotateX) );
	root->m_children.push_back(grid2);

	SmartPtr<SceneNode> grid3 ( 
		new Grid(40, 0x00000040, filename, &rotateZ) );
	root->m_children.push_back(grid3);*/

	//һ����󶨵����ڵ㣬���ڰѽڵ㸽�ӵ�������

	Scene * scene = new Scene(d3dDevice, root);
	scene->Restore();

	//һ���˶����������
	/*SmartPtr<MovementController> m_pMovementController ( new MovementController(camera));
	scene->m_pMovementController = m_pMovementController;*/

	
	return scene;

}//

/**/