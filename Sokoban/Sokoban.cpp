// 과제 : 소코반 게임 만들기

// Sokoban.cpp : 애플리케이션에 대한 진입점을 정의합니다.

#include "stdafx.h"
#include "Sokoban.h"

#define MAX_LOADSTRING 100

// 해상도
#define RESOLUTION_X GetSystemMetrics(SM_CXSCREEN)
#define RESOLUTION_Y GetSystemMetrics(SM_CYSCREEN)

// 윈도우 크기
#define WIN_SIZE_X 750
#define WIN_SIZE_Y 550

// 윈도우 위치
#define WIN_POS_X (RESOLUTION_X - WIN_SIZE_X) / 2
#define WIN_POS_Y (RESOLUTION_Y - WIN_SIZE_Y) / 2

// 크기 설정
#define BOXSIZE 50
#define PLAYERSIZE (BOXSIZE * 0.8)

#define PLAYER_SPEED 4

// 정보 출력
#define PRINT_WALL(wallPos) HBRUSH myBrush = (HBRUSH)CreateSolidBrush(RGB(194,124,122)); HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, myBrush); \
                            for (const auto& wall : wallPos) Rectangle(hdc, wall.X * BOXSIZE, wall.Y * BOXSIZE, (wall.X + 1) * BOXSIZE, (wall.Y + 1) * BOXSIZE)

#define PRINT_TARGET(tar)  myBrush = (HBRUSH)CreateSolidBrush(RGB(0xff,0,0)); oldBrush = (HBRUSH)SelectObject(hdc, myBrush); \
                            for (const auto& pos : tar) { Rectangle(hdc, pos.x - BOXSIZE / 2, pos.y - BOXSIZE / 2, pos.x + BOXSIZE / 2, pos.y + BOXSIZE / 2); }

#define PRINT_PLAYER(pos) myBrush = (HBRUSH)CreateSolidBrush(RGB(0x54,0xF7,0xFF)); oldBrush = (HBRUSH)SelectObject(hdc, myBrush); \
                          Ellipse(hdc, pos.x - PLAYERSIZE / 2, pos.y - PLAYERSIZE / 2, pos.x + PLAYERSIZE / 2, pos.y + PLAYERSIZE / 2);

#define PRINT_TARGET_LOCATION(target) for (const auto& tar : target) \
                                      { SetTextColor(hdc,RGB(0,255,0)); SetBkColor(hdc,RGB(0,0,0)); TextOut(hdc, (tar.X + 0.5) * BOXSIZE, (tar.Y + 0.5) * BOXSIZE, L"*", 1); }
#define CHECK_COLLISION()   for (const auto& tarArea : targetArea) \
                            if (IntersectRect(&common, &tarArea, &area) && !((area.right == tarArea.right) && (area.top == tarArea.top))) return; \
                            for (const auto& wall : wallPos) { RECT wallArea = { wall.X * BOXSIZE, wall.Y * BOXSIZE, (wall.X + 1) * BOXSIZE, (wall.Y + 1) * BOXSIZE }; \
                            if (IntersectRect(&common, &wallArea, &area)) return; }

// 전역 변수:
enum class EPlayer_Dir { Up, Down, Left, Right };               // 방향키

HINSTANCE hInst;                                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING] = L"Sokoban (창고지기 게임)";       // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];                            // 기본 창 클래스 이름입니다.

POINT playerPos = { 5.5 * (LONG)BOXSIZE, 8.5 * (LONG)BOXSIZE }; // 플레이어 위치(픽셀 단위)
RECT playerArea;                                                // 플레이어 영역
EPlayer_Dir playerState;                                        // 플레이어 이동 상태
bool collisionTarget = false;                                   // 다른 물체와 충돌 여부

map<char, EPlayer_Dir> keyDown;                                 // 입력된 키에 따라 출력을 해주는 변수
steady_clock::time_point startGame;                             // 게임 시작 시각
steady_clock::time_point endGame;                               // 게임 종료 시각
int win = 0;                                                    // 성공 여부

vector<RECT> targetArea;                                        // 대상 영역(픽셀 단위)

// 벽 위치(좌표 기준)
const vector<COORD> wallPos =
{
    { 4,0 }, { 5,0 }, { 6,0 }, { 7,0 }, { 8,0 },
    { 4,1 }, { 8,1 },
    { 4,2 }, { 8,2 },
    { 2,3 }, { 3,3 }, { 4,3 }, { 8,3 }, { 9,3 },
    { 2,4 }, { 9,4 },
    { 0,5 }, { 1,5 }, { 2,5 }, { 4,5 }, { 6,5 }, { 7,5 }, { 9,5 } ,{ 10,5 } ,{ 11,5 } ,{ 12,5 } ,{ 13,5 }, { 14,5 },
    { 0,6 }, { 4,6 }, { 6,6 }, { 7,6 }, { 9,6 }, { 14,6 },
    { 0,7 }, { 14,7 },
    { 0,8 }, { 1,8 }, { 2,8 }, { 3,8 }, { 4,8 }, { 6,8 },{ 7,8 },{ 8,8 },{ 10,8 },{ 14,8 },
    { 4,9 }, { 10,9 }, { 11,9 }, { 12,9 }, { 13,9 }, { 14,9 },
    { 4,10 }, { 5,10 }, { 6,10 }, { 7,10 }, { 8,10 }, { 9,10 }, { 10,10 }
};

// 목적지 위치(좌표 기준)
const vector<COORD> targetLocation =
{
    { 11,6 }, { 12,6 }, { 13,6 },
    { 12,7 }, { 13,7 }, { 13,8 },
};

// 대상 위치(픽셀 기준)
vector<POINT> target =
{
    { (LONG)(5.5 * BOXSIZE),(LONG)(2.5 * BOXSIZE) }, { (LONG)(7.5 * BOXSIZE),(LONG)(3.5 * BOXSIZE) }, { (LONG)(5.5 * BOXSIZE),(LONG)(4.5 * BOXSIZE) },
    { (LONG)(7.5 * BOXSIZE),(LONG)(4.5 * BOXSIZE) }, { (LONG)(2.5 * BOXSIZE),(LONG)(7.5 * BOXSIZE) }, { (LONG)(7.5 * BOXSIZE),(LONG)(7.5 * BOXSIZE) }
};

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDC_SOKOBAN, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SOKOBAN));

    MSG msg;

    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

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

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SOKOBAN));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW + 3);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석: 이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고 주 프로그램 창을 만든 다음 표시합니다.
//

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   // 화면은 가운데에 위치하고 화면 사이즈는 WINSIZE
   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, WIN_POS_X, WIN_POS_Y, WIN_SIZE_X, WIN_SIZE_Y, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   // 화면 초기화
   {
       RECT workArea = { WIN_POS_X, WIN_POS_Y, WIN_POS_X + WIN_SIZE_X, WIN_POS_Y + WIN_SIZE_Y };    // window창 영역 정의

       AdjustWindowRect(&workArea, WS_OVERLAPPEDWINDOW, false);
       MoveWindow(hWnd, workArea.left, workArea.top, workArea.right - workArea.left, workArea.bottom - workArea.top, false);
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

// 플레이어 이동조작 함수
void PlayerMove(const EPlayer_Dir& move)
{
    bool isContinue = false;

    if (move == EPlayer_Dir::Up)        playerArea.top -= PLAYER_SPEED;
    else if (move == EPlayer_Dir::Left) playerArea.left -= PLAYER_SPEED;
    else if (move == EPlayer_Dir::Down) playerArea.bottom += PLAYER_SPEED;
    else if (move == EPlayer_Dir::Right)playerArea.right += PLAYER_SPEED;
    else return;

    for (const auto& wall : wallPos)
    {
        RECT wallArea = { wall.X * BOXSIZE, wall.Y * BOXSIZE, (wall.X + 1) * BOXSIZE, (wall.Y + 1) * BOXSIZE }, common;
        if (IntersectRect(&common, &wallArea, &playerArea))
        {
            isContinue = true;

            if (move == EPlayer_Dir::Up)            playerPos.y = wallArea.bottom + PLAYERSIZE / 2;
            else if (move == EPlayer_Dir::Left)     playerPos.x = wallArea.right + PLAYERSIZE / 2;
            else if (move == EPlayer_Dir::Down)     playerPos.y = wallArea.top - PLAYERSIZE / 2;
            else if (move == EPlayer_Dir::Right)    playerPos.x = wallArea.left - PLAYERSIZE / 2;

            break;
        }
    }

    if (!isContinue && !collisionTarget)
    {
        if (move == EPlayer_Dir::Up)            playerPos.y -= PLAYER_SPEED;
        else if (move == EPlayer_Dir::Left)     playerPos.x -= PLAYER_SPEED;
        else if (move == EPlayer_Dir::Down)     playerPos.y += PLAYER_SPEED;
        else if (move == EPlayer_Dir::Right)    playerPos.x += PLAYER_SPEED;
        isContinue = false;
    }

    playerState = move;
}

// 목표 이동조작 함수
void TargetMove(RECT& area, const int& i)
{
    // 플레이어 위치 미리이동해보기
    if (playerState == EPlayer_Dir::Up)             area.top -= PLAYER_SPEED;
    else if (playerState == EPlayer_Dir::Left)      area.right += PLAYER_SPEED;
    else if (playerState == EPlayer_Dir::Down)      area.bottom += PLAYER_SPEED;
    else if (playerState == EPlayer_Dir::Right)     area.left -= PLAYER_SPEED;
    else return;

    // 미리 이동했을 때 충돌이 일어나는지 확인아닌 경우 함수 탈출
    RECT common;
    CHECK_COLLISION();

    // 만약 이동이 가능하면 목표물 위치이동
    if (playerState == EPlayer_Dir::Up)             target.at(i).y -= PLAYER_SPEED;
    else if (playerState == EPlayer_Dir::Left)      target.at(i).x -= PLAYER_SPEED;
    else if (playerState == EPlayer_Dir::Down)      target.at(i).y += PLAYER_SPEED;
    else if (playerState == EPlayer_Dir::Right)     target.at(i).x += PLAYER_SPEED;
}

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
        }
        break;

    case WM_KEYDOWN:
    {
        // 플레이어 영역 정의
        playerArea = { (LONG)(playerPos.x - PLAYERSIZE / 2), (LONG)(playerPos.y - PLAYERSIZE / 2)
                     , (LONG)(playerPos.x + PLAYERSIZE / 2), (LONG)(playerPos.y + PLAYERSIZE / 2) };

        // 입력된 값을 Key값으로 만듬
        unsigned char select = wParam;

        // 해당 키에 따라 플레이어 조종
        PlayerMove(keyDown[select]);

        break;
    }

    case WM_CREATE:
    {
        // 입력된 값에 해당하는 값 정의
        {
            keyDown.insert(pair<char, EPlayer_Dir>('W', EPlayer_Dir::Up));      keyDown.insert(pair<char, EPlayer_Dir>(VK_UP, EPlayer_Dir::Up));
            keyDown.insert(pair<char, EPlayer_Dir>('A', EPlayer_Dir::Left));    keyDown.insert(pair<char, EPlayer_Dir>(VK_LEFT, EPlayer_Dir::Left));
            keyDown.insert(pair<char, EPlayer_Dir>('S', EPlayer_Dir::Down));    keyDown.insert(pair<char, EPlayer_Dir>(VK_DOWN, EPlayer_Dir::Down));
            keyDown.insert(pair<char, EPlayer_Dir>('D', EPlayer_Dir::Right));   keyDown.insert(pair<char, EPlayer_Dir>(VK_RIGHT, EPlayer_Dir::Right));
        }

        startGame = steady_clock::now();    // 게임 시작
        SetTimer(hWnd, 1, 50, NULL);        // 20프레임으로 반복
        
        // 최종점 영역 설정
        for (auto& pos : target)
            targetArea.push_back({ pos.x - BOXSIZE / 2, pos.y - BOXSIZE / 2, pos.x + BOXSIZE / 2, pos.y + BOXSIZE / 2 });

        break;
    }

    case WM_TIMER:
    {
        endGame = steady_clock::now();

        InvalidateRect(hWnd, NULL, true);

        bool isContinue = false;
        int i = 0;
        RECT common;
        for (auto& area : targetArea)
        {
            if (IntersectRect(&common, &playerArea, &area))
            {
                isContinue = true;          // 플레이어와 충돌한 오브젝트를 발견했기 때문에 for문을 탈출
                collisionTarget = false;    // 플레이어와 목표물의 충돌 해제 -> 목표물을 이동하여 충돌을 없애기 때문

                TargetMove(area, i);        // 목표 이동

                // 영역도 반영
                area = { target.at(i).x - (BOXSIZE - 4) / 2, target.at(i).y - (BOXSIZE - 4) / 2, target.at(i).x + (BOXSIZE - 4) / 2, target.at(i).y + (BOXSIZE - 4) / 2 };
            }

            if (isContinue)
            {
                isContinue = false;

                break;
            }

            i++;
        }

        // 이동을 했는데 아직도 충돌한 상태인 경우
        for (auto& area : targetArea)
        {
            if (IntersectRect(&common, &playerArea, &area))
            {
                collisionTarget = true;
                break;
            }
        }

        vector<POINT> victoryDecision =
        {
            { targetLocation.at(0).X * BOXSIZE + BOXSIZE / 4, targetLocation.at(0).Y * BOXSIZE + BOXSIZE / 4},
            { targetLocation.at(1).X * BOXSIZE + BOXSIZE / 4, targetLocation.at(1).Y * BOXSIZE + BOXSIZE / 2},
            { targetLocation.at(2).X * BOXSIZE + (BOXSIZE * 3)/ 4, targetLocation.at(2).Y * BOXSIZE + BOXSIZE / 4},
            { targetLocation.at(3).X * BOXSIZE + (BOXSIZE * 3) / 4, targetLocation.at(3).Y * BOXSIZE + (BOXSIZE * 3) / 4},
            { targetLocation.at(4).X * BOXSIZE + (BOXSIZE * 3) / 4, targetLocation.at(4).Y * BOXSIZE + BOXSIZE / 2},
            { targetLocation.at(5).X * BOXSIZE + (BOXSIZE * 3) / 4, targetLocation.at(5).Y * BOXSIZE + (BOXSIZE * 3) / 4}
        };

        vector<RECT> victoryArea;

        for (auto& area : victoryDecision)
            victoryArea.push_back({ area.x, area.y, area.x + 1, area.y + 1 });

        // 목표물이 목표지점에 도달했는지 확인
        win = 0;
        for (const auto& area : victoryArea)
        {
            for (const auto& tArea : targetArea)
            {
                if (IntersectRect(&common, &area, &tArea))
                {
                    win++;
                    break;
                }
            }
        }

        if(win == 6) DestroyWindow(hWnd);

        break; 
    }

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

            PRINT_WALL(wallPos);                    // 벽 출력
            PRINT_TARGET_LOCATION(targetLocation);  // 목적지 출력
            PRINT_TARGET(target);                   // 목표 출력
            PRINT_PLAYER(playerPos);                // 플레이어 출력

            SetTextColor(hdc, RGB(255, 255, 255));

            // 어떤 키를 눌렸는지 알려주는 기능
            if(playerState == EPlayer_Dir::Up)          TextOut(hdc, WIN_SIZE_X - 25, WIN_SIZE_Y - 25, L"↑", 1);
            else if (playerState == EPlayer_Dir::Down)  TextOut(hdc, WIN_SIZE_X - 25, WIN_SIZE_Y - 25, L"↓", 1);
            else if (playerState == EPlayer_Dir::Left)  TextOut(hdc, WIN_SIZE_X - 25, WIN_SIZE_Y - 25, L"←", 1);
            else if (playerState == EPlayer_Dir::Right) TextOut(hdc, WIN_SIZE_X - 25, WIN_SIZE_Y - 25, L"→", 1);

            // 현재 시간 측정
            duration<double> dur = endGame - startGame;

            WCHAR outTime[30];
            WCHAR timePrint[20] = L"진행  시간  :  %.2f초";
            _stprintf_s(outTime, timePrint, dur.count());
            TextOut(hdc, 25, WIN_SIZE_Y - 45, outTime, lstrlen(outTime));

            WCHAR out[16];
            WCHAR winPrint[15] = L"성공한  칸수 : %d";
            wsprintf(out, winPrint, win);
            TextOut(hdc, 25, WIN_SIZE_Y - 25, out, lstrlen(out));

            EndPaint(hWnd, &ps);
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
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
