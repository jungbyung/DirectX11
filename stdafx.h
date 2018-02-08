// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
//#define _XM_NO_INTRINSICS_
#define SafeDelete(p) {if(p) {delete(p); (p) = NULL;}}
#define ReleaseCOM(p) {if(p) {(p)->Release(); (p) = NULL;}}
#define _XM_NO_INTRINSICS_
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// TODO: reference additional headers your program requires here

#include "Resource.h"
#include "d3dx11effect.h"
#include "DirectXTex.h"
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <D3Dcompiler.h>
#include <iostream>
#include <stdio.h>
#include <assert.h>
#include <map>
#include <vector>
#include <fbxsdk.h>

#pragma comment(lib, "libfbxsdk-md.lib")

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "effects11d.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "DirectXTex.lib")

using namespace std;
using namespace DirectX;

#include "Effect.h"
#include "Util.h"

using namespace JB;
