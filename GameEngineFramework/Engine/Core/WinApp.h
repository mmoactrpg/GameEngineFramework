//WinApp.h

#pragma once

#include "stdafx.h"


//==========================================
// 功能描述：	 引擎   
// 创建者：JB
// 创建日期：[3/25/2012]
// 修改日期：[3/25/2012]
//-----------------------------------------------------------------------------
namespace Engine
{
	//==========================================
	// 功能描述：	 引擎核心 
	// 创建者：JB
	// 创建日期：[3/25/2012]
	// 修改日期：[3/25/2012]
	//-----------------------------------------------------------------------------
	namespace Core
	{
		class WinApp;
		//窗口 引用
		static WinApp* _theWinApp = 0;
		
		//==========================================
		// 功能描述：	 声明-》默认消息处理
		// 创建者：JB
		// 创建日期：[3/25/2012]
		// 修改日期：[3/25/2012]
		//-----------------------------------------------------------------------------
		static LRESULT CALLBACK  WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

		

		//==========================================
		// 功能描述：Windows窗口程序基类
		// 创建者：JB
		// 创建日期：[3/25/2012]
		// 修改日期：[3/25/2012]
		//-----------------------------------------------------------------------------
		class WinApp
		{
		public:
			
			//窗口 实例
			HINSTANCE hInstance;		
			//命令行参数
			LPTSTR cmdLine;
			//显示命令行
			int cmdShow;

			HWND hWnd;

			//窗口 全屏
			bool fullScreen;

			//键盘按下
			bool keyPressed[256];	// holds true for keys that are pressed
			//消息循环
			bool done;				// flag saying when our app is complete

			/*bool (*PtrDisplay)(float timeDelta);*/

		public:
			Engine::Core::WinApp()
			{
				if(_theWinApp)
				{
					//只能有一个App实例,否则抛出异常.
	#ifdef UNICODE
					::MessageBox(0,L"App类在一个应用程序中只能有一个实例,应用程序自动退出",L"警告!",MB_ICONERROR);
	#else
					::MessageBox(0,"App类在一个应用程序中只能有一个实例,应用程序自动退出","警告!",MB_ICONERROR);
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

			

			//初始化窗口
			virtual int Load() = 0;

			////渲染
			//bool Render(float timeDelta)
			//{

			//}


			//处理Windows消息
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
		// 功能描述：	  窗口类
		// 创建者：JB
		// 创建日期：[3/25/2012]
		// 修改日期：[3/25/2012]
		//-----------------------------------------------------------------------------
		class WinFrame
		{
		public:
			//窗口 句柄
			HWND hWnd;
			
			//窗口 实例
			HINSTANCE hInstance;	

			//窗口类
			WNDCLASSEX windowClass;		

			//窗口 消息
			MSG msg;

			//窗口 位置和尺寸
			RECT windowRect;

			//窗口 扩展样式
			DWORD dwExStyle;	
			//窗口 样式
			DWORD dwStyle;

			//窗口 宽度
			int width;
			//窗口 高度
			int height;

			//窗口位数
			int bits;

			//窗口 全屏
			bool fullScreen;

			////键盘按下
			//bool keyPressed[256];	// holds true for keys that are pressed
			////消息循环
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



			

			//注册Windows窗口类
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

			//默认创建窗口方法
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
		// 功能描述：	 实现消息处理 
		// 创建者：JB
		// 创建日期：[3/25/2012]
		// 修改日期：[3/25/2012]
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
// 功能描述：Windows 程序入口   
// 创建者：JB
// 创建日期：[3/25/2012]
// 修改日期：[3/25/2012]
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





