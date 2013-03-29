//WinApp.h

#pragma once

#include "stdafx.h"


//==========================================
// ����������	 ����   
// �����ߣ�JB
// �������ڣ�[3/25/2012]
// �޸����ڣ�[3/25/2012]
//-----------------------------------------------------------------------------
namespace Engine
{
	//==========================================
	// ����������	 ������� 
	// �����ߣ�JB
	// �������ڣ�[3/25/2012]
	// �޸����ڣ�[3/25/2012]
	//-----------------------------------------------------------------------------
	namespace Core
	{
		class WinApp;
		//���� ����
		static WinApp* _theWinApp = 0;
		
		//==========================================
		// ����������	 ����-��Ĭ����Ϣ����
		// �����ߣ�JB
		// �������ڣ�[3/25/2012]
		// �޸����ڣ�[3/25/2012]
		//-----------------------------------------------------------------------------
		static LRESULT CALLBACK  WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

		

		//==========================================
		// ����������Windows���ڳ������
		// �����ߣ�JB
		// �������ڣ�[3/25/2012]
		// �޸����ڣ�[3/25/2012]
		//-----------------------------------------------------------------------------
		class WinApp
		{
		public:
			
			//���� ʵ��
			HINSTANCE hInstance;		
			//�����в���
			LPTSTR cmdLine;
			//��ʾ������
			int cmdShow;

			HWND hWnd;

			//���� ȫ��
			bool fullScreen;

			//���̰���
			bool keyPressed[256];	// holds true for keys that are pressed
			//��Ϣѭ��
			bool done;				// flag saying when our app is complete

			/*bool (*PtrDisplay)(float timeDelta);*/

		public:
			Engine::Core::WinApp()
			{
				if(_theWinApp)
				{
					//ֻ����һ��Appʵ��,�����׳��쳣.
	#ifdef UNICODE
					::MessageBox(0,L"App����һ��Ӧ�ó�����ֻ����һ��ʵ��,Ӧ�ó����Զ��˳�",L"����!",MB_ICONERROR);
	#else
					::MessageBox(0,"App����һ��Ӧ�ó�����ֻ����һ��ʵ��,Ӧ�ó����Զ��˳�","����!",MB_ICONERROR);
	#endif
					::ExitProcess(0);
				}
				else 
				{
					_theWinApp = this;
				}
			}

			int InitWinApp(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPTSTR lpCmdLine,int nCmdShow)
			{
				this->hInstance = hInstance;
				this->cmdLine = lpCmdLine;
				this->cmdShow = nCmdShow;
				//EnterMsgLoop(NULL);
				return 0;
			}

			

			//��ʼ������
			virtual int Load() = 0;

			////��Ⱦ
			//bool Render(float timeDelta)
			//{

			//}


			//����Windows��Ϣ
			virtual LRESULT Process(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) = 0;

			bool Destroy()
			{
				return done = true;
			}

			virtual bool Render() = 0;

			int EnterMsgLoop()
			{
				MSG msg;
				::ZeroMemory(&msg, sizeof(MSG));
				//static float lastTime = (float)timeGetTime();

				//bool done = false;

				while ( !done )
				{
					if( ::PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
					{
						::TranslateMessage(&msg);		// translate and dispatch to event queue
						::DispatchMessage(&msg);
					}

					if (msg.message == WM_QUIT)// do we receive a WM_QUIT message?
					{
						done = true;			// if so, time to quit the application
					}

					Sleep(10);

					//else
					//{
					//	//float currTime = (float)timeGetTime();
					//	//float timeDelta = ( currTime - lastTime )* 0.001f;
					//	Render();
					//	//lastTime = currTime;
					//}
				}

				if (fullScreen)
				{
					ChangeDisplaySettings(NULL,0);		// If So Switch Back To The Desktop
					ShowCursor(TRUE);					// Show Mouse Pointer
				}

				return msg.wParam;
			}//end loop
			
		};


		//==========================================
		// ����������	  ������
		// �����ߣ�JB
		// �������ڣ�[3/25/2012]
		// �޸����ڣ�[3/25/2012]
		//-----------------------------------------------------------------------------
		class WinFrame
		{
		public:
			//���� ���
			HWND hWnd;
			
			//���� ʵ��
			HINSTANCE hInstance;	

			//������
			WNDCLASSEX windowClass;		

			//���� ��Ϣ
			MSG msg;

			//���� λ�úͳߴ�
			RECT windowRect;

			//���� ��չ��ʽ
			DWORD dwExStyle;	
			//���� ��ʽ
			DWORD dwStyle;

			//���� ���
			int width;
			//���� �߶�
			int height;

			//����λ��
			int bits;

			//���� ȫ��
			bool fullScreen;

			////���̰���
			//bool keyPressed[256];	// holds true for keys that are pressed
			////��Ϣѭ��
			//bool done;				// flag saying when our app is complete

			WinFrame()
			{

			}

			WinFrame(int width, int height, bool fullScreen)
			{
				// temp var's
				this->width = width;
				this->height = height;

				this->fullScreen = fullScreen;
				this->bits = 32;
			}//

			bool Create(int width, int height, bool fullScreen)
			{
				// temp var's
				this->width = width;
				this->height = height;

				this->fullScreen = fullScreen;
				this->bits = 32;
				//
				if(!RegisterWindowClass())
					return false;
				CheckFullScreen();
				//
				if(!CreateMainWindow())
					return false;
				
				
			}

			bool Show(int nComdShow)
			{
				ShowWindow(hWnd, nComdShow);		// display the window //SW_SHOW
				UpdateWindow(hWnd);			// update the window

				return true;
			}



			

			//ע��Windows������
			bool RegisterWindowClass()
			{
				// fill out the window class structure
				windowClass.cbSize			= sizeof(WNDCLASSEX);
				windowClass.style			= CS_HREDRAW | CS_VREDRAW;
				windowClass.lpfnWndProc	= (WNDPROC)WndProc;
				windowClass.cbClsExtra		= 0;
				windowClass.cbWndExtra		= 0;
				windowClass.hInstance			= hInstance;
				windowClass.hIcon				= LoadIcon(NULL, IDI_APPLICATION);	// default icon
				windowClass.hCursor			= LoadCursor(NULL, IDC_ARROW);		// default arrow
				windowClass.hbrBackground		= NULL;							// don't need background
				windowClass.lpszMenuName		= NULL;							// no menu
				windowClass.lpszClassName		= L"MyClass";
				windowClass.hIconSm			= LoadIcon(NULL, IDI_WINLOGO);		// windows logo small icon

				// register the windows class
				if (!RegisterClassEx(&windowClass))
					return false;
				return true;
			}

			//Ĭ�ϴ������ڷ���
			bool CreateMainWindow()
			{
				windowRect.left=(long)0;				// Set Left Value To 0
				windowRect.right=(long)this->width;		// Set Right Value To Requested Width
				windowRect.top=(long)0;				// Set Top Value To 0
				windowRect.bottom=(long)this->height;	// Set Bottom Value To Requested Height

				hWnd = CreateWindowEx(NULL,	// extended style
					L"MyClass",				// class name
					L"DXFrameWork",			// app name
					dwStyle | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
					0, 0,					// x,y coordinate
					windowRect.right - windowRect.left,
					windowRect.bottom - windowRect.top,	// width, height
					NULL,							// handle to parent
					NULL,							// handle to menu
					hInstance,						// application instance
					NULL);							// no extra params

				// check if window creation failed (hwnd would equal NULL)
				if (!hWnd)
					return false;
				return true;
			}

			void CheckFullScreen( void )
			{
				if (fullScreen)								// fullscreen?
				{
					DEVMODE dmScreenSettings;					// device mode
					memset(&dmScreenSettings,0,sizeof(dmScreenSettings));
					dmScreenSettings.dmSize = sizeof(dmScreenSettings);	
					dmScreenSettings.dmPelsWidth = width;		// screen width
					dmScreenSettings.dmPelsHeight = height;		// screen height
					dmScreenSettings.dmBitsPerPel = bits;		// bits per pixel
					dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

					// 
					if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
					{
						// setting display mode failed, switch to windowed
						MessageBox(NULL, L"Display mode failed", NULL, MB_OK);
						fullScreen=FALSE;	
					}
				}

				if (fullScreen)								// Are We Still In Fullscreen Mode?
				{
					dwExStyle =		WS_EX_APPWINDOW;			// Window Extended Style
					dwStyle	  =		WS_POPUP;				// Windows Style
					ShowCursor(FALSE);						// Hide Mouse Pointer
				}
				else
				{
					dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;	// Window Extended Style
					dwStyle=WS_OVERLAPPEDWINDOW;					// Windows Style
				}
			}

			

		};

		//==========================================
		// ����������	 ʵ����Ϣ���� 
		// �����ߣ�JB
		// �������ڣ�[3/25/2012]
		// �޸����ڣ�[3/25/2012]
		//-----------------------------------------------------------------------------
		static LRESULT CALLBACK  WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			switch ( msg )
			{
			case WM_CLOSE:
				Engine::Core::_theWinApp->done = true;
				return 0;
				break;
			case WM_DESTROY:
				Engine::Core::_theWinApp->done = true;
				return 0;
				break;
			default:

				break;
			}
			Engine::Core::_theWinApp->Process(hwnd,	msg, wParam, lParam);

			return ::DefWindowProc(hwnd, msg, wParam, lParam);
		}//

	}//end Core

}//end Engine


//==========================================
// ����������Windows �������   
// �����ߣ�JB
// �������ڣ�[3/25/2012]
// �޸����ڣ�[3/25/2012]
//-----------------------------------------------------------------------------
int APIENTRY _tWinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPTSTR lpCmdLine,int nCmdShow)
{
	/*Engine::Core::Instance = hInstance;
	Engine::Core::CmdLine = lpCmdLine;
	Engine::Core::CmdShow = nCmdShow;*/
	Engine::Core::_theWinApp->InitWinApp(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	Engine::Core::_theWinApp->Load();
	Engine::Core::_theWinApp->EnterMsgLoop();
	return 0;
}





