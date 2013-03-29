#pragma once

#include "stdafx.h"

//#include "d3dUtility.h"
//#include "silhouetteEdges.h"


//#include "Engine/SceneManagement/SceneNode.h"
//#include "Engine/SceneManagement/Scene.h"

//#include "TestScene.h"

#include "Engine/SceneManagement/SceneNode.h"
#include "Engine/SceneManagement/Scene.h"
#include "Engine/SceneManagement/TransformNode.h"
#include "Engine/SceneManagement/CameraNode.h"
#include "Engine/SceneManagement/MeshNode.h"
#include "Engine/SceneManagement/XMeshNode.h"
#include "Engine/SceneManagement/Grid.h"
#include "Engine/SceneManagement/MovementController.h"
#include "Engine/SceneManagement/SkyNode.h"
#include "Engine/SceneManagement/ArrowNode.h"

using namespace boost;
using namespace Engine::SceneManagement;
using namespace d3d;


class D3D
{
public:
	//默认构造D3D对象
	D3D(void);
	//析构D3D对象
	~D3D(void);


	//绘制的宽度
	int Width;
	//绘制的高度
	int Height;

	//D3D9 设备
	IDirect3DDevice9* Device; 

	
	//投影矩阵
	D3DXMATRIX ProjMatrix;



	//键盘按键
	UINT KeyChar;
	
	//显示线程句柄
	HANDLE g_hDisplayThread;
	//显示线程ID
	DWORD g_dwDisplayThreadID;
	//绘制是否在运行
	bool g_bIsDisplayRunning;
	//绘制是否初始化
	bool g_bIsDisplayInitialise;

	//
	
	class Scene * m_pScene;
	

	/**
	 *	初始化		 
	hwnd	初始化的句柄
	Height 初始化高度
	Width	初始化宽度
	windowed	是否窗口
	**/
	void Init(HWND hWnd, int Height, int Width, bool windowed);
	
	/**
	 *	初始化D3D
	 **/
	bool Setup();
	
	/**
	 *	清除D3D占用
	 **/
	void Cleanup();
	
	/**
	 *	显示渲染一帧
	 **/
	bool Display(float timeDelta);

	VOID  D3DUtil_InitMaterial( D3DMATERIAL9& mtrl, FLOAT r, FLOAT g, FLOAT b, FLOAT a );

	Scene * BuildScene(IDirect3DDevice9 * d3dDevice);

	
	
	 LRESULT Process(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


	/**
	 *	渲染用线程
	 **/
	static DWORD WINAPI  ListenDisplayThread(LPVOID lpParameter)
	{

		D3D* pD3D = (D3D *)lpParameter;
		
		//启动，设置为运行状态
		pD3D->g_bIsDisplayRunning = true;

		static float lastTime = (float)timeGetTime(); 
		float currTime;
		float timeDelta;
		//static D3D d3d;

		while (pD3D->g_bIsDisplayRunning)
		{
			/*g_threadInput.Capture();
			if (g_threadInput.IsUp(DIK_ESCAPE))
				g_bIsRunning = false;
			{*/
				

				currTime  = (float)timeGetTime();
				timeDelta = (currTime - lastTime)*0.001f;

				//ptr_display(timeDelta);
				//d3d.Display(timeDelta);
				//d3d.Display(timeDelta);
				pD3D->Display(timeDelta);


				//d3d.Display(timeDelta);

				lastTime = currTime;
			//}
				Sleep(30);
		}

		//等待线程结束
		WaitForSingleObject(pD3D->g_hDisplayThread, INFINITE);
		//删除的线程资源
		CloseHandle(pD3D->g_hDisplayThread);
		return 0; 
	}


};

