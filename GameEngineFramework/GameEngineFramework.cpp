#include "Engine/Core/WinApp.h"
#include "D3D.h"

using namespace Engine::Core;

//bool Render(float timeDelta)
//{
//	return false;
//}

class GameEngineFramework:public WinApp
{
protected:
	WinFrame frame;
	D3D d3d;

	virtual int Load()
	{
		
		/*_theWinApp->width = 800;
		_theWinApp->height = 600;
		_theWinApp->fullScreen = false;*/

		//创建窗口
		frame.Create(800, 600, false);
		frame.Show(SW_SHOW);
		//开个线程渲染
		d3d.Init(frame.hWnd, 800, 600, true);

		//::MessageBox(0, L"应用程序运行正常",L"OK",MB_ICONINFORMATION);

		
		return 0;
	}


	virtual LRESULT Process(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
	
		switch(wParam)
		{
		case VK_ESCAPE:
			_theWinApp->done = true;
			break;
		}
		
		d3d.Process(hwnd, msg, wParam, lParam);

		return S_OK;
	}

	
	virtual bool Render()
	{
		return S_OK;
	}
};

//默认执行
GameEngineFramework theApp;