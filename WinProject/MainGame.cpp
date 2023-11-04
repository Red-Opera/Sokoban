#include "stdafx.h"
#include "MainGame.h"
#include "Player.h"
#include "Object.h"

// MainGame 클래스 생성 시 플레이어와 물체의 객체를 생성
MainGame::MainGame()
{
    pPlayer = new Player;
    pObject = new Object;
}

// MainGame 클래스 종료시 제거
MainGame::~MainGame()
{
    if (pObject)
    {
        delete pPlayer;
        pPlayer = nullptr;
    }

    if (pPlayer)
    {
        delete pPlayer;
        pPlayer = nullptr;
    }

}

// MainGame 초기값 설정
void MainGame::Init()
{
    nScore = 0;
    nLevel = 0;

    // 플레이어와 물체의 시작함수 실행
    if (pPlayer != nullptr) pPlayer->Init();
    if (pObject != nullptr) pObject->Init();
}

void MainGame::Update()
{
    InvalidateRect(g_hWnd, NULL, false);   // NULL일 경우에는 모든 영역을 갱신해준다. 

    nLevel = nScore / 100 + 1;

    // 플레이어와 오브젝트의 반복함수 실행
    if (pPlayer != nullptr) pPlayer->Update();
    if (pObject != nullptr)
    {
        pObject->SetLevel(nLevel);
        pObject->Update();
    }

    RECT rt;

    // 상자 하강
    for (auto iter = pObject->GetBox().begin(); iter != pObject->GetBox().end();)   // 물체 클래스 배열을 순회
    {
        RECT rtIter = iter->rt;     // 해당 물체의 영역을 저장

        if (iter->rt.bottom > WINSIZE_Y)                            // 땅에 닿은 경우
        {
            nScore += 5;
    
            iter = pObject->GetBox().erase(iter);
        }
    
        else if (IntersectRect(&rt, &rtIter, &pPlayer->GetRect()))  // 플레이어와 물체가 닿은 경우
        {
            nScore -= 20;
            iter = pObject->GetBox().erase(iter);
        }
    
        else if (PtInRect(&rtIter, g_ptMouse))                      // 마우스 포인터에 닿은 경우
        {
            nScore += 3;
            iter = pObject->GetBox().erase(iter);
        }
    
        else  iter++;
    }
}

void MainGame::Render()
{
    PAINTSTRUCT ps;

    HDC hdc = BeginPaint(g_hWnd, &ps);

    // 버퍼 초기화 해주는 함수(hdc, 사이즈, 바꿀 색상)
    // 백버퍼를 흰색으로 초기화
    PatBlt(g_hDC, 0, 0, WINSIZE_X, WINSIZE_Y, WHITENESS);
    
    // 백버퍼에 그림을 그러준다.
    // 플레이어와 오브젝트의 출력함수 실행
    if (pPlayer != nullptr) pPlayer->Render();
    if (pObject != nullptr) pObject->Render();

    // 점수와 레벨 출력 부분
    wstring wScore(to_wstring(nScore).c_str());
    TextOut(g_hDC, 10, 10, wScore.c_str(), wScore.length());

    wstring wLevel(to_wstring(nLevel).c_str());
    TextOut(g_hDC, 10, 30, wLevel.c_str(), wLevel.length());

    // 백버퍼에서 그린 이미지를 전면버퍼에 복사
    BitBlt(hdc, 0, 0, WINSIZE_X, WINSIZE_Y, g_hDC, 0, 0, SRCCOPY);

    EndPaint(g_hWnd, &ps);
}