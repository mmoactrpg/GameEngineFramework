

#include "D3D.h"






D3D::D3D(void)
{
	Device = 0; 

	Width  = 640;
	Height = 480;
	//ProjMatrix;

	//m_pScene = NULL;


	KeyChar = 0;
}


D3D::~D3D(void)
{
}


void D3D::Init(HWND hWnd, int Height, int Width, bool windowed)
{
	
	if(!d3d::InitD3D(hWnd,
		Width, Height, windowed, D3DDEVTYPE_HAL, &Device))
	{
		::MessageBox(0, L"InitD3D() - FAILED", 0, 0);
		return;
	}

	if(!Setup())
	{
		::MessageBox(0, L"Setup() - FAILED", 0, 0);
		return;
	}
	
	//设置初始化开始
	g_bIsDisplayInitialise = true;
	//启动渲染线程
	g_hDisplayThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ListenDisplayThread, (void *)this, 0, &g_dwDisplayThreadID);
}

bool D3D::Setup()
{
	m_pScene = BuildScene(Device);
	//m_pScene->Update();

	// Set up the textures
	Device->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
	Device->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	Device->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
	Device->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
	Device->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	Device->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	Device->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
	Device->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );

	// Set miscellaneous render states
	Device->SetRenderState( D3DRS_DITHERENABLE,   FALSE );
	Device->SetRenderState( D3DRS_SPECULARENABLE, FALSE );
	Device->SetRenderState( D3DRS_ZENABLE,        TRUE );
	Device->SetRenderState( D3DRS_AMBIENT,        0x000F0F0F );

	// Set up our view matrix. A view matrix can be defined given an eye point,
	// a point to lookat, and a direction for which way is up. Here, we set the
	// eye five units back along the z-axis and up three units, look at the
	// origin, and define "up" to be in the y-direction.

	// Set the projection matrix
	D3DXMATRIX matProj;
	FLOAT fAspect = (FLOAT)Width / (FLOAT)Height;
	D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI/4, fAspect, 1.0f, 10000.0f );
	Device->SetTransform( D3DTS_PROJECTION, &matProj );

	// Set up lighting stat`es
	D3DXVECTOR3 dir(1.0f, -0.0f, 0.25f);
	D3DXCOLOR   c = d3d::WHITE;
	D3DLIGHT9 light = d3d::InitDirectionalLight(&dir, &c);

	Device->SetLight( 0, &light );
	Device->LightEnable( 0, TRUE );
	Device->SetRenderState( D3DRS_LIGHTING, TRUE );

	return true;
}

void D3D::Cleanup()
{
	g_bIsDisplayRunning = false;
	
}

bool D3D::Display(float timeDelta)
{
	if( Device )
	{
		// 
		// Update the scene: Allow user to rotate around scene.
		//

		static float angle  = (3.0f * D3DX_PI) / 2.0f;
		static float height = 5.0f;


		//angle -= 0.1f;
			
		switch(KeyChar)
		{
			case VK_LEFT:
				//angle -= 0.5f;// * timeDelta;
				angle -= 0.5f * timeDelta;
				break;
			case VK_RIGHT:
				//angle += 0.5f;// * timeDelta;
				angle += 0.5f * timeDelta;
				break;
			case VK_UP:
				//height += 5.0f;// * timeDelta;
				height += 5.0f * timeDelta;
				break;
			case VK_DOWN:
				//height -= 5.0f;// * timeDelta;
				height -= 5.0f * timeDelta;
				break;
		}

		

		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);
		if( SUCCEEDED( Device->BeginScene() ) )
		{
			if(m_pScene)
			{
				m_pScene->Render();
				m_pScene->Update();
			}

			Device->EndScene();
		}

		
		Device->Present(0, 0, 0, 0);
	}
	return true;
}

 LRESULT D3D::Process(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
 {
	 switch( msg )
	 {
	 case WM_PAINT:
		 {
			 // Draw on the window tell the user that the app is loading
			 // TODO: change as needed
			 HDC hDC = GetDC( hwnd );
			 TCHAR strMsg[MAX_PATH];
			 wsprintf( strMsg, TEXT("Loading... Please wait") );
			 RECT rct;
			 GetClientRect( hwnd, &rct );
			 DrawText( hDC, strMsg, -1, &rct, DT_CENTER|DT_VCENTER|DT_SINGLELINE );
			 ReleaseDC( hwnd, hDC );

			 break;
		 }

	 case WM_KEYDOWN:
		 if (m_pScene && m_pScene->m_pMovementController)
			 m_pScene->m_pMovementController->OnKeyDown(static_cast<const BYTE>(wParam));
		 break;

	 case WM_KEYUP:
		 if (m_pScene && m_pScene->m_pMovementController)
			 m_pScene->m_pMovementController->OnKeyUp(static_cast<const BYTE>(wParam));
		 break;

	 case WM_MOUSEMOVE:
		 if (m_pScene && m_pScene->m_pMovementController)
			 m_pScene->m_pMovementController->OnMouseMove(MAKEPOINTS(lParam));
		 break;

	 case WM_LBUTTONDOWN:
		 SetCapture(hwnd);
		 if (m_pScene && m_pScene->m_pMovementController)
			 m_pScene->m_pMovementController->OnLButtonDown(MAKEPOINTS(lParam));
		 break;

	 case WM_LBUTTONUP:
		 SetCapture(NULL);
		 if (m_pScene && m_pScene->m_pMovementController)
			 m_pScene->m_pMovementController->OnLButtonUp(MAKEPOINTS(lParam));
		 break;

	 case WM_RBUTTONDOWN:
		 SetCapture(hwnd);
		 if (m_pScene && m_pScene->m_pMovementController)
			 m_pScene->m_pMovementController->OnRButtonDown(MAKEPOINTS(lParam));
		 break;

	 case WM_RBUTTONUP:
		 SetCapture(NULL);
		 if (m_pScene && m_pScene->m_pMovementController)
			 m_pScene->m_pMovementController->OnRButtonUp(MAKEPOINTS(lParam));
		 break;

	 }

	 return S_OK;//CD3DApplication::MsgProc( hWnd, msg, wParam, lParam );
 }




 ////////////////////////////////////////////////////
 // Free Function to Build the Scene
 ////////////////////////////////////////////////////

 //-----------------------------------------------------------------------------
 // Name: D3DUtil_InitMaterial()
 // Desc: Initializes a D3DMATERIAL9 structure, setting the diffuse and ambient
 //       colors. It does not set emissive or specular colors.
 //-----------------------------------------------------------------------------
 VOID D3D::D3DUtil_InitMaterial( D3DMATERIAL9& mtrl, FLOAT r, FLOAT g, FLOAT b,
	 FLOAT a )
 {
	 ZeroMemory( &mtrl, sizeof(D3DMATERIAL9) );
	 mtrl.Diffuse.r = mtrl.Ambient.r = r;
	 mtrl.Diffuse.g = mtrl.Ambient.g = g;
	 mtrl.Diffuse.b = mtrl.Ambient.b = b;
	 mtrl.Diffuse.a = mtrl.Ambient.a = a;
 }


Scene * D3D::BuildScene(IDirect3DDevice9 *d3dDevice)
 {
	 // Setup some materials - we'll use these for 
	 // making the same mesh appear in multiple
	 // colors

	 D3DMATERIAL9 colors[8];
	 D3DUtil_InitMaterial( colors[0], 1.0f, 1.0f, 1.0f, 1.0f );	// white
	 D3DUtil_InitMaterial( colors[1], 0.0f, 1.0f, 1.0f, 1.0f );	// cyan
	 D3DUtil_InitMaterial( colors[2], 1.0f, 0.0f, 0.0f, 1.0f );	// red
	 D3DUtil_InitMaterial( colors[3], 0.0f, 1.0f, 0.0f, 1.0f );	// green
	 D3DUtil_InitMaterial( colors[4], 0.0f, 0.0f, 1.0f, 1.0f );	// blue
	 D3DUtil_InitMaterial( colors[5], 0.4f, 0.4f, 0.4f, 0.4f );	// 40% grey
	 D3DUtil_InitMaterial( colors[6], 0.25f, 0.25f, 0.25f, 0.25f );	// 25% grey
	 D3DUtil_InitMaterial( colors[7], 0.65f, 0.65f, 0.65f, 0.65f );	// 65% grey

	 // The identity matrix is always useful
	 D3DXMATRIX ident;
	 D3DXMatrixIdentity(&ident);

	 // We'll use these rotations for some teapots and grid objects
	 D3DXMATRIX rotateX, rotateY, rotateZ;

	 // Create the root, and the camera.
	 // Remeber how to use smart pointers?? I hope so!

	 boost::shared_ptr<TransformNode> root(new TransformNode(&ident));

	 boost::shared_ptr<CameraNode> camera(new CameraNode(&ident));
	 root->m_children.push_back(camera);



	 // We'll put the camera in the scene at (20,20,20) looking back at the Origin

	 D3DXMATRIX rotOnly, result, inverse;
	 float cameraYaw = - (3.0f * D3DX_PI) / 4.0f;
	 float cameraPitch = D3DX_PI / 4.0f;
	 D3DXQUATERNION q;
	 D3DXQuaternionIdentity(&q);
	 D3DXQuaternionRotationYawPitchRoll(&q, cameraYaw, cameraPitch, 0.0);
	 D3DXMatrixRotationQuaternion(&rotOnly, &q);

	 D3DXMATRIX trans;
	 D3DXMatrixTranslation(&trans, 15.0f, 15.0f, 15.0f);

	 D3DXMatrixMultiply(&result, &rotOnly, &trans);

	 D3DXMatrixInverse(&inverse, NULL, &result);
	 camera->VSetTransform(&result, &inverse);

	 D3DXMatrixRotationZ(&rotateZ, D3DX_PI / 2.0f);
	 D3DXMatrixRotationX(&rotateX, -D3DX_PI / 2.0f);
	 D3DXVECTOR3 target(30, 2, 15);

	 //

	

	 ID3DXMesh *teapot;
	 if( SUCCEEDED( D3DXCreateTeapot( d3dDevice, &teapot, NULL ) ) )
	 {
		 // Teapot #1 - a white one at (x=6,y=2,z=4)
		 D3DXMatrixTranslation(&trans,6,2,4);

		 boost::shared_ptr<SceneNode> mesh1(new MeshNode(teapot, &trans, colors[2]));
		 root->m_children.push_back(mesh1);

		 // Teapot #2 - a cyan one at (x=3,y=2,z=1)
		 //   with a 
		 D3DXMatrixTranslation(&trans, 3,2,1);
		 D3DXMATRIX result;
		 D3DXMatrixMultiply(&result, &rotateZ, &trans);

		 boost::shared_ptr<SceneNode> mesh2(new MeshNode(teapot, &result, colors[1]));
		 root->m_children.push_back(mesh2);

		 // Teapot #3 - another white one at (x=30, y=2, z=15)
		 D3DXMATRIX rotateY90;
		 D3DXMatrixRotationY(&rotateY90, D3DX_PI / 2.0f);
		 D3DXMatrixTranslation(&trans, target.x, target.y, target.z);
		 D3DXMatrixMultiply(&result, &rotateY90, &trans);
		 boost::shared_ptr<SceneNode> mesh3(new MeshNode(teapot, &result, colors[0]));
		 root->m_children.push_back(mesh3);

		 // We can release the teapot now, mesh1 and mesh2 AddRef'd it.
		 SAFE_RELEASE(teapot);
	 }

	 ID3DXMesh *sphere;
	 if ( SUCCEEDED( 
		 D3DXCreateSphere( 
		 d3dDevice, .25, 16, 16, &sphere, NULL) ) )
	 {
		 // We're going to create a spiral of spheres...
		 // starting at (x=3, y=0, z=3), and spiraling
		 // upward about a local Y axis.

		 D3DXMatrixTranslation(&trans, 3,0,3);

		 boost::shared_ptr<SceneNode> sphere1(new MeshNode(sphere, &trans, colors[4]) );
		 root->m_children.push_back(sphere1);

		 // Here's the local rotation and translation.
		 // We'll rotate about Y, and then translate
		 // up (along Y) and forward (along Z).
		 D3DXMatrixRotationY(&rotateY, D3DX_PI / 8.0f);
		 D3DXMATRIX trans2;
		 D3DXMatrixTranslation(&trans2, 0, 0.5, 0.5);
		 D3DXMatrixMultiply(&result, &trans2, &rotateY);

		 for (int i=0; i<25; i++)
		 {
			 // If you didn't think smart pointers were cool - 
			 // watch this! No leaked memory....

			 // Notice this is a heirarchy....
			 boost::shared_ptr<SceneNode> sphere2(new MeshNode(sphere, &result, colors[i%5]) );
			 sphere1->m_children.push_back(sphere2);
			 sphere1 = sphere2;
		 }

		 // We can release the sphere now, all the cylinders AddRef'd it.
		 SAFE_RELEASE(sphere);
	 }
	 

	 //
	 D3DXMatrixTranslation(&trans,-25,20,20);
	 //D3DXMatrixScaling(&trans, -10, -10, -10);
	 ScaleMtrl scale;
	 scale.x = -50.0f;
	 scale.y = -50.0f;
	 scale.z = -50.0f;
	 boost::shared_ptr<SceneNode> xmesh1(new XMeshNode(L"gf3.x", d3dDevice, &trans, &scale));
	 root->m_children.push_back(xmesh1);

	 
	  root->m_children.push_back(xmesh1);

	 /*D3DXMatrixTranslation(&trans,-45,20,20);
	 boost::shared_ptr<SceneNode> xmesh11(new XMeshNode(L"gf3.x", d3dDevice, &trans, &scale));
	 root->m_children.push_back(xmesh11);*/


	  XMeshNode *mm = new XMeshNode(L"gow_m1.x", d3dDevice, &trans, &scale);

	 D3DXMatrixTranslation(&trans,10,10,10);
	 //D3DXMatrixScaling(&trans, -10, -10, -10);
	 //ScaleMtrl scale;
	 scale.x = 100.0f;
	 scale.y = 100.0f;
	 scale.z = 100.0f;
	 boost::shared_ptr<SceneNode> xmesh2( new XMeshNode(L"gow_m1.x", d3dDevice, &trans, &scale));
	 root->m_children.push_back(xmesh2);

	 
	 
	 /*D3DXMatrixTranslation(&trans,20,20,20);
	 boost::shared_ptr<SceneNode> xmesh3(new XMeshNode(mm->m_mesh, mm->Mtrls, mm->Textures, &trans, 0));
	 root->m_children.push_back(xmesh3);*/

	 int col = 10;
	 int row= 10;
	 int zoom = 10;
	 const int COUNT = 13;
	 for(int i = 0; i < COUNT; i++)
	 {
		 for (int j = 0; j< COUNT; j++)
		 {
			 for(int z = 0; z< COUNT; z++)
			 {
				 D3DXMatrixTranslation(&trans, col + i, row + j , zoom + z);
				 boost::shared_ptr<SceneNode> xmeshNew(new XMeshNode(mm->m_mesh, mm->Mtrls, mm->Textures, &trans, 0));
				 root->m_children.push_back(xmeshNew);
			 }
		 }
	 }


	 //D3DXMatrixScaling(&trans, 10, 10, 10);

	 // Here are the grids...they make it easy for us to 
	 // see where the coordinates are in 3D space.
	 boost::shared_ptr<SceneNode> grid1(new Grid(40, 0x00404040, L"Textures\\grid.dds", &ident));
	 root->m_children.push_back(grid1);
	 boost::shared_ptr<SceneNode> grid2(new Grid(40, 0x00004000, L"Textures\\grid.dds", &rotateX));
	 root->m_children.push_back(grid2);
	 boost::shared_ptr<SceneNode> grid3(new Grid(40, 0x00000040, L"Textures\\grid.dds", &rotateZ));
	 root->m_children.push_back(grid3);

	 //  Here's the sky node that never worked!!!!
	 boost::shared_ptr<SkyNode> sky(new SkyNode(_T("Sky2"), camera));
	 root->m_children.push_back(sky);

	 D3DXMatrixTranslation(&trans,15,2,15);
	 D3DXMatrixRotationY(&rotateY, D3DX_PI / 4.0f);
	 D3DXMatrixMultiply(&result, &rotateY, &trans);

	 boost::shared_ptr<SceneNode> arrow1(new ArrowNode(2, &result, colors[0], d3dDevice));
	 root->m_children.push_back(arrow1);

	 D3DXMatrixRotationY(&rotateY, D3DX_PI / 2.0f);
	 D3DXMatrixMultiply(&result, &rotateY, &trans);
	 boost::shared_ptr<SceneNode> arrow2(new ArrowNode(2, &result, colors[5], d3dDevice));
	 root->m_children.push_back(arrow2);

	 D3DXMatrixMultiply(&result, &rotateX, &trans);
	 boost::shared_ptr<SceneNode> arrow3(new ArrowNode(2, &result, colors[0], d3dDevice));
	 root->m_children.push_back(arrow3);


	 // Everything has been attached to the root. Now
	 // we attach the root to the scene.

	 Scene *scene = new Scene(d3dDevice, root);
	 scene->Restore();

	 // A movement controller is going to control the camera, 
	 // but it could be constructed with any of the objects you see in this
	 // function. You can have your very own remote controlled sphere. What fun...
	 boost::shared_ptr<MovementController> m_pMovementController(new MovementController(camera, cameraYaw, cameraPitch));

	 scene->m_pMovementController = m_pMovementController;
	 return scene;
 }





//Scene * D3D::BuildScene(IDirect3DDevice9 * d3dDevice)
//{
//	//设置一些颜色材质，以便显示不同的颜色网格
//
//	D3DMATERIAL9 colors[5]; 
//	colors[0] = InitMtrl(WHITE, WHITE, WHITE, BLACK, 2.0f);
//	colors[1] = InitMtrl(RED, RED, RED, BLACK, 2.0f);
//	colors[2] = InitMtrl(GREEN, GREEN, GREEN, BLACK, 2.0f);
//	colors[3] = InitMtrl(BLUE, BLUE, BLUE, BLACK, 2.0f);
//	colors[4] = InitMtrl(YELLOW, YELLOW, YELLOW, BLACK, 2.0f);
//
//	//单位矩阵总是有用的
//	D3DXMATRIX ident;
//	D3DXMatrixIdentity(&ident);
//
//	//我们将对一些茶壶和网格对象使用这些旋转
//	D3DXMATRIX rotateX, rotateZ;
//	D3DXMatrixRotationZ ( &rotateZ, D3DX_PI / 2.0f );
//	D3DXMatrixRotationX( &rotateX, -D3DX_PI / 2.0f );
//
//	//创建根节点和摄像机
//	SmartPtr<SceneNode> root (new TransformNode(&ident));
//	SmartPtr<SceneNode> camera ( new CameraNode(&ident));
//	root->m_children.push_back(camera);
//
//	
//	//茶壶
//	ID3DXMesh * teapot;
//	if ( SUCCEEDED( D3DXCreateTeapot( d3dDevice, &teapot, NULL) ) )
//	{
//		//茶壶#1 白色（x=6, y=2, z=4）
//		D3DXMATRIX trans;
//		D3DXMatrixTranslation(&trans, 6, 2, 4);
//		
//		SmartPtr<SceneNode> mesh1 ( new MeshNode(teapot, &trans, colors[0] ));
//		root->m_children.push_back(mesh1);
//
//		//茶壶#2 红色 (x=3,y=2,z=1)
//		D3DXMatrixTranslation( &trans, 3, 2, 1 );
//		D3DXMATRIX result;
//		D3DXMatrixMultiply( &result, &rotateZ, &trans );
//
//		SmartPtr<SceneNode> mesh2 (new MeshNode(teapot, &result, colors[1] ));
//		root->m_children.push_back(mesh2);
//
//		//释放茶壶，mesh1 和 mesh2 已经保存引用
//		Release<ID3DXMesh * >(teapot);
//	}//end teapot
//
//	/**/
//	//
//	ID3DXMesh * sphere;
//	if( SUCCEEDED( D3DXCreateSphere( d3dDevice, .25, 16, 16, &sphere, NULL ) ) )
//	{
//		//我们将创建一个螺旋线的球体，从（x=3, y=0, z=3）开始，沿 x 轴上升
//		D3DXMATRIX trans;
//		D3DXMatrixTranslation( &trans, 3, 0, 3);
//
//		SmartPtr<SceneNode> sphere1 ( new MeshNode( sphere, &trans, colors[4] ) );
//		root->m_children.push_back(sphere1);
//
//		//下面是局部旋转和缩放
//		//我们先围绕 y 轴旋转 然后沿y 轴向上平移，沿 z 轴向前平移
//		D3DXMATRIX rotateY;
//		D3DXMatrixRotationY(&rotateY, D3DX_PI / 8.0f);
//		D3DXMATRIX trans2;
//		D3DXMatrixTranslation(&trans2, 0, 0.5, 0.5);
//		D3DXMATRIX result;
//		D3DXMatrixMultiply( &result, &trans2, &rotateY);
//
//		for ( int i =0; i < 25; i++)
//		{
//			SmartPtr<SceneNode> sphere2 ( new MeshNode(sphere, &result, colors[i%5] ) );
//			sphere1->m_children.push_back(sphere2);
//			sphere1 = sphere2;
//		}
//
//		Release<ID3DXMesh*>(sphere);
//
//	}//end sphere
//	
//	/**/
//
//	//TCHAR * filename = (TCHAR*)"grid.dss";
//
//	/*
//	SmartPtr<SceneNode> grid1 ( 
//		new Grid(40, 0x00400000, L"grid.dss", &ident) );
//	root->m_children.push_back(grid1);
//
//	SmartPtr<SceneNode> grid2 ( 
//		new Grid(40, 0x00004000, L"grid.dss", &rotateX) );
//	root->m_children.push_back(grid2);
//
//	SmartPtr<SceneNode> grid3 ( 
//		new Grid(40, 0x00000040, L"grid.dss", &rotateZ) );
//	root->m_children.push_back(grid3);
//	*/
//
//	//一切因绑定到根节点，现在把节点附加到场景中
//
//	//设置贴图采样器过滤方式
//	/*Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
//	Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
//	Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);*/
//
//	
//
//	Scene * scene = new Scene(d3dDevice, root);
//	scene->Restore();
//
//	//一个运动控制摄像机
//	/*SmartPtr<MovementController> m_pMovementController ( new MovementController(camera));
//	scene->m_pMovementController = m_pMovementController;*/
//
//
//	//D3DXVECTOR3 position(0.0f, 0.0f, -3.0f);
//	//D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
//	//D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
//	//D3DXMATRIX V;
//	//D3DXMatrixLookAtLH(&V, &position, &target, &up);
//	//d3dDevice->SetTransform(D3DTS_VIEW, &V);
//
//	////
//	//// Calculate projection matrix.
//	////
//
//	//D3DXMatrixPerspectiveFovLH(
//	//	&ProjMatrix, D3DX_PI * 0.5f, 
//	//	(float)Width / (float)Height, 1.0f, 1000.0f);
//	//d3dDevice->SetTransform(D3DTS_PROJECTION, &ProjMatrix);
//	
//	
//
//	d3dDevice->SetFVF(D3DFVF_XYZ);
//
//	D3DXMATRIXA16 matWorld;
//	D3DXMatrixIdentity( &matWorld );
//	D3DXMatrixRotationX( &matWorld, timeGetTime()/1000.0f );
//
//	d3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
//	D3DXVECTOR3 vEyePt( 0.0f, 3.0f,-5.0f );
//	D3DXVECTOR3 vLookatPt( 0.0f, 0.0f, 0.0f );
//	D3DXVECTOR3 vUpVec( 0.0f, 1.0f, 0.0f );
//	D3DXMATRIXA16 matView;
//	D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookatPt, &vUpVec );
//	d3dDevice->SetTransform( D3DTS_VIEW, &matView );
//	D3DXMATRIXA16 matProj;
//	D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI/2, 1.0f, 1.0f, 500.0f );
//	d3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );
//
//
//
//	//// Set up the textures
//	//d3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
//	//d3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
//	//d3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
//	//d3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
//	//d3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
//	//d3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
//	//d3dDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
//	//d3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
//
//	//// Set miscellaneous render states
//	//d3dDevice->SetRenderState( D3DRS_DITHERENABLE,   FALSE );
//	//d3dDevice->SetRenderState( D3DRS_SPECULARENABLE, FALSE );
//	//d3dDevice->SetRenderState( D3DRS_ZENABLE,        TRUE );
//	//d3dDevice->SetRenderState( D3DRS_AMBIENT,        0x000F0F0F );
//
//	// Set up our view matrix. A view matrix can be defined given an eye point,
//	// a point to lookat, and a direction for which way is up. Here, we set the
//	// eye five units back along the z-axis and up three units, look at the
//	// origin, and define "up" to be in the y-direction.
//
//	// Set the projection matrix
//	/*D3DXMATRIX matProj;
//	FLOAT fAspect = (FLOAT)Width / (FLOAT)Height;
//	D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI/0.5f, fAspect, 1.0f, 100.0f );
//	d3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );*/
//
//	// Set up lighting states
//	D3DLIGHT9 light;
//	D3DXVECTOR3 vecLightDirUnnormalized(1.0f, -6.0f, 1.0f);
//	D3DXCOLOR  lc = BLUE;
//	//D3DUtil_InitLight( light, D3DLIGHT_DIRECTIONAL, 1.0f, -6.0f, 1.0f );
//	InitDirectionalLight(&vecLightDirUnnormalized, &lc);
//	d3dDevice->SetLight( 0, &light );
//	d3dDevice->LightEnable( 0, TRUE );
//	d3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
//
//	
//	
//	//
//	d3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
//	//
//	
//
//	return scene;
//
//}//


//-----------------------------------------------------------------------------
// Name: D3DUtil_InitLight()
// Desc: Initializes a D3DLIGHT structure, setting the light position. The
//       diffuse color is set to white; specular and ambient are left as black.
//-----------------------------------------------------------------------------
//VOID  D3D::D3DUtil_InitLight( D3DLIGHT9& light, D3DLIGHTTYPE ltType,
//	FLOAT x, FLOAT y, FLOAT z )
//{
//	D3DXVECTOR3 vecLightDirUnnormalized(x, y, z);
//	ZeroMemory( &light, sizeof(D3DLIGHT9) );
//	light.Type        = ltType;
//	light.Diffuse.r   = 1.0f;
//	light.Diffuse.g   = 1.0f;
//	light.Diffuse.b   = 1.0f;
//	D3DXVec3Normalize( (D3DXVECTOR3*)&light.Direction, &vecLightDirUnnormalized );
//	light.Position.x   = x;
//	light.Position.y   = y;
//	light.Position.z   = z;
//	light.Range        = 1000.0f;
//}