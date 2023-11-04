// header.h: 표준 시스템 포함 파일
// 또는 프로젝트 특정 포함 파일이 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>
// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#define WINSIZE_X 640
#define WINSIZE_Y 720

#define PROGRAM_TITLE L"윈도우API게임"

#define RECT_MAKE(x, y, s) { x - s / 2, y - s / 2, x + s / 2, y + s / 2}
#define RECT_DRAW(rt) Rectangle(g_hDC, rt.left, rt.top, rt.right, rt.bottom)


// " " : 일반 문자열
// L" " : 한글과 같은 멀티바이트 문자를 표시할 때 사용하는 문자열

#include <string>
#include <time.h>
#include <vector>
using namespace std;

extern HWND g_hWnd;
extern POINT g_ptMouse;
extern HDC g_hDC;

#include "MainGame.h"
#include "KeyManager.h"