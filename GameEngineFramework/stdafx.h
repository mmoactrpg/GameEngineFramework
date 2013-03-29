// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

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
// ����������	 ���ú�
// �����ߣ�JB
// �������ڣ�[4/8/2012]
// �޸����ڣ�[4/8/2012]
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


