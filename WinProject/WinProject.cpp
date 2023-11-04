// WinProject.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "WinProject.h"

#define MAX_LOADSTRING 100

// 전역 변수
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING] = PROGRAM_TITLE;  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

HWND g_hWnd;
POINT g_ptMouse;
HDC g_hDC;

MainGame* pMainGame = nullptr;

//enum MOVE_DIR { MOVE_LEFT, MOVE_RIGHT, MOVE_UP, MOVE_DOWN };

// POINT ptPos2 = { WINSIZE_X / 2 ,WINSIZE_Y / 2 };
// RECT rtBox2;

// bool isPicked = false;
// 
// Window API의 main함수
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,             // 프로그램 핸들 인스턴스 -> 현재 프로세서
                     _In_opt_ HINSTANCE hPrevInstance,      // 이전에 실행된 핸들 인스턴스
                     _In_ LPWSTR    lpCmdLine,              // cmd로 입력된 프로그램 인수 - 실행할 때 프로그램의 초기설정 변경 가능
                     _In_ int       nCmdShow)               // 프로그램이 시작 될 때 형태(최소화 보통 등의 상태값)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    // IDS_APP_TITLE값을 szTitle로 복사 중(Title값을 이미 정했으므로 주석 처리)
    // LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINPROJECT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    // 단축키(액셀레이터) 테이블을 읽는다.
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINPROJECT));

    MSG msg;

    pMainGame = new MainGame;
    pMainGame->Init();
    KeyManager::Get().Init();

    HDC hdc = GetDC(g_hWnd);
    // 메인 DC를 사용하여 추가로 DC를 생성
    g_hDC = CreateCompatibleDC(hdc);
    // 백버퍼로 사용할 비트맵 생성
    // 비트맵 : 픽셀 이미지를 저장하는 이진파일 또는 저장 방식
    HBITMAP hBitMap = (HBITMAP)CreateCompatibleBitmap(hdc, WINSIZE_X, WINSIZE_Y); // hdc, 도화지 크기
    // 백버퍼 DC에 비트맵 연결
    SelectObject(g_hDC, hBitMap);

    // 기본 메시지 루프입니다:
    // while (GetMessage(&msg, nullptr, 0, 0)) // GetMessage : 메시지 큐에서 메시지를 읽는다.
    // {
    //     if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
    //     {
    //         TranslateMessage(&msg);
    //         DispatchMessage(&msg);
    //     }
    // }

    // 해당 문에서 컴퓨터의 성능에 따라서 게임속도가 달라진다는 문제가 생긴다.
    while (true)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) 
        {
            if (msg.message == WM_QUIT) break; // while문 탈출시키는 조건문
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        else
        {
            if (pMainGame != nullptr)
            {
                pMainGame->Update();
                pMainGame->Render();

            }
        }
    }

    delete pMainGame;

    return (int) msg.wParam;
}

//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX); // 구조체의 크기정보를 담아준다.

    // CS_HREDRAW | CS_VREDRAW -> 가로 세로 크기 변경시 창을 다시 그린다.
    wcex.style          = CS_HREDRAW | CS_VREDRAW;                              // 윈도우 스타일
    wcex.lpfnWndProc    = WndProc;                                              // 메세지 처리 함수 지정
    wcex.cbClsExtra     = 0;                                                    // 클래스 여분 메모리
    wcex.cbWndExtra     = 0;                                                    // 윈도우 여분 메모리
    wcex.hInstance      = hInstance;                                            // 인스턴스
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINPROJECT)); // 창의 아이콘
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);                       // 마우스 커서
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);                             // 배경색
    wcex.lpszMenuName   = NULL;                                                 // 메뉴 이름(NULL일시 메뉴 제거)
    wcex.lpszClassName  = szWindowClass;                                        // 클래스 이름
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL)); // 작은 아이콘

    // 윈도우 클래스 등록
    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석: 이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//

// 창을 초기화하고 생성해주는 부분
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   // 화면 해상도 받기
   int nResolutionX = GetSystemMetrics(SM_CXSCREEN);
   int nResolutionY = GetSystemMetrics(SM_CYSCREEN);

   // 화면를 정가운데로 이동
   int nWinPosX = nResolutionX / 2 - WINSIZE_X / 2;
   int nWinPosY = nResolutionY / 2 - WINSIZE_Y / 2;

   // 창을 생성하고 저장한다. (hWnd : 윈도우 핸들)
   HWND hWnd = CreateWindowW(
       szWindowClass,           // 윈도우 클래스 이름
       szTitle,                 // 타이틀 바 이름
       WS_OVERLAPPEDWINDOW,     // 윈도우 스타일
       nWinPosX,                // 윈도우 화면 좌표 X : 실행할 때 어느위치에서 띄울 것인지
       nWinPosY,                // 윈도우 화면 좌표 Y
       WINSIZE_X,               // 윈도우 가로 사이즈
       WINSIZE_Y,               // 윈도우 세로 사이즈
       nullptr,                 // 부모 윈도우
       nullptr,                 // 메뉴 핸들
       hInstance,               // 인스턴스 저장
       nullptr                  // 자식 윈도우를 생성하면 지정 없다면 NULL
   );

   if (!hWnd)
   {
      return FALSE;
   }

   g_hWnd = hWnd;

   // 윈도우 사이즈 조정(타이틀 바 및 메뉴 사이즈를 실 사이즈에서 제외)
   {
       // 원하는 작업 영역 (Window창이 시작하는 부분부터 끝까지의 영역)
       RECT rt = { nWinPosX, nWinPosY, nWinPosX + WINSIZE_X, nWinPosY + WINSIZE_Y };

       // 원하는 작업영역 크기, 스타일 값, 메뉴의 여부를 넘기면 원하는 작업영역에 맞는 윈도우 크기를 계산해 ft에 넣어준다.
       AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);

       // 윈도우의 위치, 크기 변경 함수
       MoveWindow(
           hWnd,
           nWinPosX,            // 윈도우 위치 X
           nWinPosY,            // 윈도우 위치 Y
           rt.right - rt.left,  // 너비 (size : WINSIZE_X)
           rt.bottom - rt.top,  // 높이 (size : WINSIZE_Y)
           TRUE                 // 창 새로고침 여부

       );
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // 메뉴 선택을 구문 분석합니다:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }

        break;
    }

    case WM_CREATE:
    {
        // 자동적으로 WM_TIMER를 호출하기 때문에 NULL를 넣어 호출할 함수가 없도록 함
        // SetTimer(hWnd, 1, 10, NULL);
        
        srand(time(NULL));

        break;
    }

    //case WM_TIMER:
    //{
    //    //rtBox2 = RECT_MAKE(ptPos2.x, ptPos2.y, 100);
    //
    //    //RECT rt;
    //    // IntersectRect(&rt, &rtBox1, &rtBox2);
    //
    //    //if (IntersectRect(&rt, &rtBox1, &rtBox2))
    //    //{
    //    //    switch (eMoveDir)
    //    //    {
    //    //    case MOVE_LEFT:
    //    //        ptPos2.x -= fMoveSpeed;
    //    //        break;
    //    //    case MOVE_RIGHT:
    //    //        ptPos2.x += fMoveSpeed;
    //    //        break;
    //    //    case MOVE_UP:
    //    //        ptPos2.y -= fMoveSpeed;
    //    //        break;
    //    //    case MOVE_DOWN:
    //    //        ptPos2.y += fMoveSpeed;
    //    //        break;
    //    //    default:
    //    //        break;
    //    //    }
    //    //}
    //    //
    //    //if (isPicked)
    //    //{
    //    //    ptPos2 = ptMouse;
    //    //}
    //    KeyManager::Get().Update();
    //
    //    if (pMainGame != nullptr)
    //        pMainGame->Update();
    //
    //    break;
    //}

    case WM_MOUSEMOVE:
    {
        g_ptMouse.x = LOWORD(lParam); // 낮은 비트에 x좌표값이 존재
        g_ptMouse.y = HIWORD(lParam); // 높은 비트에 y좌표값이 존재

        break;
    }

    //case WM_LBUTTONDOWN: // 마우스 왼쪽 클릭시 발생하는 메시지
    //{
    //    //if (PtInRect(&rtBox2, ptMouse))
    //    //{
    //    //    isPicked = true;
    //    //}
    //
    //    break;
    //}
    //
    //case WM_LBUTTONUP: // 마우스 왼쪽 클릭을 놓았을 때 발생하는 메시지
    //{
    //    isPicked = false;
    //    break; 
    //}

    //case WM_PAINT:
    //{
    //    //    //int ptX = ptPos1.x * roomSize;
    //    //    //int ptY = ptPos1.y * roomSize;
    //    //    //
    //    //    //// (10,10)만큼 오른쪽 아래으로 들어가고 (10, 10)만큼 왼쪽 위만큼 들어간다.
    //    //    //Rectangle(hdc, ptX + 10, ptY + 10, ptX + roomSize - /10, ptY + roomSize - 10);
    //

    //    //    RECT_DRAW(rtBox1);
    //    //    //RECT_DRAW(rtBox2);
    //    //
    //    //    for (const auto& rect : vecBox)
    //    //        RECT_DRAW(rect.rt);
    //    //
    //    //    wstring wScore(to_wstring(nScore).c_str());
    //    //    TextOut(hdc, 10, 10, wScore.c_str(), wScore.length());
    //    //
    //    //    wstring wLevel(to_wstring(nLevel).c_str());
    //    //    TextOut(hdc, 10, 30, wLevel.c_str(), wLevel.length());
    //    //
    //    //    EndPaint(hWnd, &ps);
    //    //}
    //
    //    if (pMainGame != nullptr)
    //        pMainGame->Render();
    //
    //    break;
    //}

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
        break;
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}