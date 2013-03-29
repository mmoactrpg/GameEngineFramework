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
	//设置一些颜色材质，以便显示不同的颜色网格

	D3DMATERIAL9 colors[5];
	colors[0] = InitMtrl(WHITE, WHITE, WHITE, BLACK, 2.0f);
	colors[1] = InitMtrl(RED, RED, RED, BLACK, 2.0f);
	colors[2] = InitMtrl(GREEN, GREEN, GREEN, BLACK, 2.0f);
	colors[3] = InitMtrl(BLUE, BLUE, BLUE, BLACK, 2.0f);
	colors[4] = InitMtrl(YELLOW, YELLOW, YELLOW, BLACK, 2.0f);

	//单位矩阵总是有用的
	D3DXMATRIX ident;
	D3DXMatrixIdentity(&ident);

	//我们将对一些茶壶和网格对象使用这些旋转
	D3DXMATRIX rotateX, rotateZ;
	D3DXMatrixRotationZ ( &rotateZ, D3DX_PI / 2.0f );
	D3DXMatrixRotationX( &rotateX, -D3DX_PI / 2.0f );

	//创建根节点和摄像机
	SmartPtr<SceneNode> root (new TransformNode(&ident));
	SmartPtr<SceneNode> camera ( new CameraNode(&ident));
	root->m_children.push_back(camera);

	//茶壶
	ID3DXMesh * teapot;
	if ( SUCCEEDED( D3DXCreateTeapot( d3dDevice, &teapot, NULL) ) )
	{
		//茶壶#1 白色（x=6, y=2, z=4）
		D3DXMATRIX trans;
		D3DXMatrixTranslation(&trans, 6, 2, 4);
		
		SmartPtr<SceneNode> mesh1 ( new MeshNode(teapot, &trans, colors[0] ));
		root->m_children.push_back(mesh1);

		//茶壶#2 红色 (x=3,y=2,z=1)
		D3DXMatrixTranslation( &trans, 3, 2, 1 );
		D3DXMATRIX result;
		D3DXMatrixMultiply( &result, &rotateZ, &trans );

		SmartPtr<SceneNode> mesh2 (new MeshNode(teapot, &result, colors[1] ));
		root->m_children.push_back(mesh2);

		//释放茶壶，mesh1 和 mesh2 已经保存引用
		Release<ID3DXMesh * >(teapot);
	}//end teapot

	//
	ID3DXMesh * sphere;
	if( SUCCEEDED( D3DXCreateSphere( d3dDevice, .25, 16, 16, &sphere, NULL ) ) )
	{
		//我们将创建一个螺旋线的球体，从（x=3, y=0, z=3）开始，沿 x 轴上升
		D3DXMATRIX trans;
		D3DXMatrixTranslation( &trans, 3, 0, 3);

		SmartPtr<SceneNode> sphere1 ( new MeshNode( sphere, &trans, colors[4] ) );
		root->m_children.push_back(sphere1);

		//下面是局部旋转和缩放
		//我们先围绕 y 轴旋转 然后沿y 轴向上平移，沿 z 轴向前平移
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

	//一切因绑定到根节点，现在把节点附加到场景中

	Scene * scene = new Scene(d3dDevice, root);
	scene->Restore();

	//一个运动控制摄像机
	/*SmartPtr<MovementController> m_pMovementController ( new MovementController(camera));
	scene->m_pMovementController = m_pMovementController;*/

	
	return scene;

}//

/**/