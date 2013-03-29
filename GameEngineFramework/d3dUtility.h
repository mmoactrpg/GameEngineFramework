//////////////////////////////////////////////////////////////////////////////////////////////////
// 
// File: d3dUtility.h
// 
// Author: Frank Luna (C) All Rights Reserved
//
// System: AMD Athlon 1800+ XP, 512 DDR, Geforce 3, Windows XP, MSVC++ 7.0 
//
// Desc: Provides utility functions for simplifying common tasks.
//          
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __d3dUtilityH__
#define __d3dUtilityH__


#include <string>

#include <limits>
#pragma comment(lib, "Winmm.lib")

#include "d3d9.h"
//#pragma comment(lib, "D:\\Program Files\\Microsoft DirectX SDK (February 2010)\\Lib\\x86\\d3d9.lib")
#pragma comment(lib, "d3d9.lib")

#include "d3dx9.h"
//#pragma comment(lib, "D:\\Program Files\\Microsoft DirectX SDK (February 2010)\\Lib\\x86\\d3dx9.lib")
#pragma comment(lib, "d3dx9.lib")

namespace d3d
{
	/*static HANDLE g_hThread;
	static DWORD g_dwThreadID;
	static bool g_bIsRunning;
	static bool g_bIsInitialise;*/

	

	//typedef bool (*ptr_display)(float timeDelta);
	//ptr_display pd;

	///*int RenderThread();*/
	//DWORD WINAPI ListenThread(LPVOID lpParameter);
	//
	// Init
	//
	 bool InitWindow(
		HINSTANCE hInstance,		// [in] Application instance.);
		int width, int height,		// [in] Backbuffer dimensions.
		HWND *hwnd);				// [out] 创建窗口成功句柄

	bool InitD3D(
		HWND hwnd,					// [in]	窗口句柄
		int width, int height,		// [in] 显示大小.
		bool windowed,				// [in] 是否全屏显示
		D3DDEVTYPE deviceType,		// [in] HAL or REF
		IDirect3DDevice9** device);	// [out]The created device.

	int EnterMsgLoop( 
		bool (*ptr_display)(float timeDelta));

	LRESULT CALLBACK WndProc(
		HWND hwnd,
		UINT msg, 
		WPARAM wParam,
		LPARAM lParam);

	//
	// Cleanup
	//
	template<class T> void Release(T t)
	{
		if( t )
		{
			t->Release();
			t = 0;
		}
	}
		
	template<class T> void Delete(T t)
	{
		if( t )
		{
			delete t;
			t = 0;
		}
	}

	//
	// Colors
	//
	const D3DXCOLOR      WHITE( D3DCOLOR_XRGB(255, 255, 255) );
	const D3DXCOLOR      BLACK( D3DCOLOR_XRGB(  0,   0,   0) );
	const D3DXCOLOR        RED( D3DCOLOR_XRGB(255,   0,   0) );
	const D3DXCOLOR      GREEN( D3DCOLOR_XRGB(  0, 255,   0) );
	const D3DXCOLOR       BLUE( D3DCOLOR_XRGB(  0,   0, 255) );
	const D3DXCOLOR     YELLOW( D3DCOLOR_XRGB(255, 255,   0) );
	const D3DXCOLOR       CYAN( D3DCOLOR_XRGB(  0, 255, 255) );
	const D3DXCOLOR    MAGENTA( D3DCOLOR_XRGB(255,   0, 255) );

	//
	// Lights
	//

	D3DLIGHT9 InitDirectionalLight(D3DXVECTOR3* direction, D3DXCOLOR* color);
	D3DLIGHT9 InitPointLight(D3DXVECTOR3* position, D3DXCOLOR* color);
	D3DLIGHT9 InitSpotLight(D3DXVECTOR3* position, D3DXVECTOR3* direction, D3DXCOLOR* color);

	//
	// Materials
	//

	D3DMATERIAL9 InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p);

	const D3DMATERIAL9 WHITE_MTRL  = InitMtrl(WHITE, WHITE, WHITE, BLACK, 2.0f);
	const D3DMATERIAL9 RED_MTRL    = InitMtrl(RED, RED, RED, BLACK, 2.0f);
	const D3DMATERIAL9 GREEN_MTRL  = InitMtrl(GREEN, GREEN, GREEN, BLACK, 2.0f);
	const D3DMATERIAL9 BLUE_MTRL   = InitMtrl(BLUE, BLUE, BLUE, BLACK, 2.0f);
	const D3DMATERIAL9 YELLOW_MTRL = InitMtrl(YELLOW, YELLOW, YELLOW, BLACK, 2.0f);

	


//#ifndef UNDER_CE
//	//-----------------------------------------------------------------------------
//	// Name: DXUtil_GetDXSDKMediaPathCch()
//	// Desc: Returns the DirectX SDK media path
//	//       cchDest is the size in TCHARs of strDest.  Be careful not to 
//	//       pass in sizeof(strDest) on UNICODE builds.
//	//-----------------------------------------------------------------------------
//	HRESULT DXUtil_GetDXSDKMediaPathCch( TCHAR* strDest, int cchDest )
//	{
//		if( strDest == NULL || cchDest < 1 )
//			return E_INVALIDARG;
//
//		lstrcpy( strDest, TEXT("") );
//
//		// Open the appropriate registry key
//		HKEY  hKey;
//		LONG lResult = RegOpenKeyEx( HKEY_LOCAL_MACHINE,
//			_T("Software\\Microsoft\\DirectX SDK"),
//			0, KEY_READ, &hKey );
//		if( ERROR_SUCCESS != lResult )
//			return E_FAIL;
//
//		DWORD dwType;
//		DWORD dwSize = cchDest * sizeof(TCHAR);
//		lResult = RegQueryValueEx( hKey, _T("DX9SDK Samples Path"), NULL,
//			&dwType, (BYTE*)strDest, &dwSize );
//		strDest[cchDest-1] = 0; // RegQueryValueEx doesn't NULL term if buffer too small
//		RegCloseKey( hKey );
//
//		if( ERROR_SUCCESS != lResult )
//			return E_FAIL;
//
//		const TCHAR* strMedia = _T("\\Media\\");
//		if( lstrlen(strDest) + lstrlen(strMedia) < cchDest )
//			_tcscat( strDest, strMedia );
//		else
//			return E_INVALIDARG;
//
//		return S_OK;
//	}
//#endif // !UNDER_CE
//
//
//
//#ifndef UNDER_CE
//	//-----------------------------------------------------------------------------
//	// Name: DXUtil_FindMediaFileCch()
//	// Desc: Returns a valid path to a DXSDK media file
//	//       cchDest is the size in TCHARs of strDestPath.  Be careful not to 
//	//       pass in sizeof(strDest) on UNICODE builds.
//	//-----------------------------------------------------------------------------
//	HRESULT DXUtil_FindMediaFileCch( TCHAR* strDestPath, int cchDest, TCHAR* strFilename )
//	{
//		HRESULT hr;
//		HANDLE file;
//		TCHAR* strShortNameTmp = NULL;
//		TCHAR strShortName[MAX_PATH];
//		int cchPath;
//
//		if( NULL==strFilename || NULL==strDestPath || cchDest < 1 )
//			return E_INVALIDARG;
//
//		lstrcpy( strDestPath, TEXT("") );
//		lstrcpy( strShortName, TEXT("") );
//
//		// Build full path name from strFileName (strShortName will be just the leaf filename)
//		cchPath = GetFullPathName(strFilename, cchDest, strDestPath, &strShortNameTmp);
//		if ((cchPath == 0) || (cchDest <= cchPath))
//			return E_FAIL;
//		if( strShortNameTmp )
//			lstrcpyn( strShortName, strShortNameTmp, MAX_PATH );
//
//		// first try to find the filename given a full path
//		file = CreateFile( strDestPath, GENERIC_READ, FILE_SHARE_READ, NULL, 
//			OPEN_EXISTING, 0, NULL );
//		if( INVALID_HANDLE_VALUE != file )
//		{
//			CloseHandle( file );
//			return S_OK;
//		}
//
//		// next try to find the filename in the current working directory (path stripped)
//		file = CreateFile( strShortName, GENERIC_READ, FILE_SHARE_READ, NULL, 
//			OPEN_EXISTING, 0, NULL );
//		if( INVALID_HANDLE_VALUE != file )
//		{
//			_tcsncpy( strDestPath, strShortName, cchDest );
//			strDestPath[cchDest-1] = 0; // _tcsncpy doesn't NULL term if it runs out of space
//			CloseHandle( file );
//			return S_OK;
//		}
//
//		// last, check if the file exists in the media directory
//		if( FAILED( hr = DXUtil_GetDXSDKMediaPathCch( strDestPath, cchDest ) ) )
//			return hr;
//
//		if( lstrlen(strDestPath) + lstrlen(strShortName) < cchDest )
//			lstrcat( strDestPath, strShortName );
//		else
//			return E_INVALIDARG;
//
//		file = CreateFile( strDestPath, GENERIC_READ, FILE_SHARE_READ, NULL, 
//			OPEN_EXISTING, 0, NULL );
//		if( INVALID_HANDLE_VALUE != file )
//		{
//			CloseHandle( file );
//			return S_OK;
//		}
//
//		// On failure, just return the file as the path
//		_tcsncpy( strDestPath, strFilename, cchDest );
//		strDestPath[cchDest-1] = 0; // _tcsncpy doesn't NULL term if it runs out of space
//		return HRESULT_FROM_WIN32( ERROR_FILE_NOT_FOUND );
//	}
//#endif // !UNDER_CE
//
//
//
//#ifndef UNDER_CE
//	HRESULT DXUtil_GetDXSDKMediaPathCb( TCHAR* szDest, int cbDest )
//	{
//		return DXUtil_GetDXSDKMediaPathCch( szDest, cbDest / sizeof(TCHAR) );
//	}
//
//	HRESULT DXUtil_FindMediaFileCb( TCHAR* szDestPath, int cbDest, TCHAR* strFilename )
//	{
//		return DXUtil_FindMediaFileCch( szDestPath, cbDest / sizeof(TCHAR), strFilename );
//	}
//#endif // !UNDER_CE
//
//	//-----------------------------------------------------------------------------
//	// Name: D3DUtil_CreateTexture()
//	// Desc: Helper function to create a texture. It checks the root path first,
//	//       then tries the DXSDK media path (as specified in the system registry).
//	//-----------------------------------------------------------------------------
//	HRESULT D3DUtil_CreateTexture( LPDIRECT3DDEVICE9 pd3dDevice, TCHAR* strTexture,
//		LPDIRECT3DTEXTURE9* ppTexture, D3DFORMAT d3dFormat )
//	{
//		HRESULT hr;
//		TCHAR strPath[MAX_PATH];
//
//		// Get the path to the texture
//		if( FAILED( hr = DXUtil_FindMediaFileCb( strPath, sizeof(strPath), strTexture ) ) )
//			return hr;
//
//		// Create the texture using D3DX
//		return D3DXCreateTextureFromFileEx( pd3dDevice, strPath, 
//			D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, d3dFormat, 
//			D3DPOOL_MANAGED, D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR, 
//			D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR, 0, NULL, NULL, ppTexture );
//	}
//

	//
	// Bounding Objects
	//

	struct BoundingBox
	{
		BoundingBox();

		bool isPointInside(D3DXVECTOR3& p);

		D3DXVECTOR3 _min;
		D3DXVECTOR3 _max;
	};

	struct BoundingSphere
	{
		BoundingSphere();

		D3DXVECTOR3 _center;
		float       _radius;
	};

	//
	// Constants
	//

	const float INFINITY = FLT_MAX;
	const float EPSILON  = 0.001f;

	//
	// Drawing
	//

	// Function references "desert.bmp" internally.  This file must
	// be in the working directory.
	bool DrawBasicScene(
		IDirect3DDevice9* device,// Pass in 0 for cleanup.
		float scale);            // uniform scale 

	//
	// Vertex Structures
	//

	struct Vertex
	{
		Vertex(){}
		Vertex(float x, float y, float z, 
			float nx, float ny, float nz,
			float u, float v)
		{
			_x  = x;  _y  = y;  _z  = z;
			_nx = nx; _ny = ny; _nz = nz;
			_u  = u;  _v  = v;
		}
		float _x, _y, _z;
		float _nx, _ny, _nz;
		float _u, _v;

		static const DWORD FVF;
	};

	//
	// Randomness
	//

	// Desc: Return random float in [lowBound, highBound] interval.
	float GetRandomFloat(float lowBound, float highBound);
	

	// Desc: Returns a random vector in the bounds specified by min and max.
	void GetRandomVector(
		D3DXVECTOR3* out,
		D3DXVECTOR3* min,
		D3DXVECTOR3* max);

	//
	// Conversion
	//
	DWORD FtoDw(float f);
}

#endif // __d3dUtilityH__