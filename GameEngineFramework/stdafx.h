// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#include <assert.h>
#include <windows.h>
#include <tchar.h>

#include <list>

#include <d3d9.h>
#include <d3dx9.h>
#include <d3dx9math.h>

//#include <boost\config.hpp>

//#include "SmartPtr.h"

#include "d3dUtility.h"


#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")



//==========================================
// 功能描述：	 常用宏
// 创建者：JB
// 创建日期：[4/8/2012]
// 修改日期：[4/8/2012]
//-----------------------------------------------------------------------------

#define MAX(a, b) ( (a) >= (b) ? (a) : (b) )
#define MIN(a, b) ( (a) < (b) ? (a) : (b) )

#define RADIANS_TO_DEGREES(x) ((x) * 180.0f / D3DX_PI)
#define DEGREES_TO_RADIANS(x) ((x) * D3DX_PI / 180.0f)

//-----------------------------------------------------------------------------
// Miscellaneous helper functions
//-----------------------------------------------------------------------------
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }




//


