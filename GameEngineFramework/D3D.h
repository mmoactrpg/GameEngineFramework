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
	//Ĭ�Ϲ���D3D����
	D3D(void);
	//����D3D����
	~D3D(void);


	//���ƵĿ��
	int Width;
	//���Ƶĸ߶�
	int Height;

	//D3D9 �豸
	IDirect3DDevice9* Device; 

	
	//ͶӰ����
	D3DXMATRIX ProjMatrix;



	//���̰���
	UINT KeyChar;
	
	//��ʾ�߳̾��
	HANDLE g_hDisplayThread;
	//��ʾ�߳�ID
	DWORD g_dwDisplayThreadID;
	//�����Ƿ�������
	bool g_bIsDisplayRunning;
	//�����Ƿ��ʼ��
	bool g_bIsDisplayInitialise;

	//
	
	class Scene * m_pScene;
	

	/**
	 *	��ʼ��		 
	hwnd	��ʼ���ľ��
	Height ��ʼ���߶�
	Width	��ʼ�����
	windowed	�Ƿ񴰿�
	**/
	void Init(HWND hWnd, int Height, int Width, bool windowed);
	
	/**
	 *	��ʼ��D3D
	 **/
	bool Setup();
	
	/**
	 *	���D3Dռ��
	 **/
	void Cleanup();
	
	/**
	 *	��ʾ��Ⱦһ֡
	 **/
	bool Display(float timeDelta);

	VOID  D3DUtil_InitMaterial( D3DMATERIAL9& mtrl, FLOAT r, FLOAT g, FLOAT b, FLOAT a );

	Scene * BuildScene(IDirect3DDevice9 * d3dDevice);

	
	
	 LRESULT Process(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


	/**
	 *	��Ⱦ���߳�
	 **/
	static DWORD WINAPI  ListenDisplayThread(LPVOID lpParameter)
	{

		D3D* pD3D = (D3D *)lpParameter;
		
		//����������Ϊ����״̬
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

		//�ȴ��߳̽���
		WaitForSingleObject(pD3D->g_hDisplayThread, INFINITE);
		//ɾ�����߳���Դ
		CloseHandle(pD3D->g_hDisplayThread);
		return 0; 
	}


};

