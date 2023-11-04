#include "stdafx.h"
#include "MainGame.h"
#include "Player.h"
#include "Object.h"

// MainGame Ŭ���� ���� �� �÷��̾�� ��ü�� ��ü�� ����
MainGame::MainGame()
{
    pPlayer = new Player;
    pObject = new Object;
}

// MainGame Ŭ���� ����� ����
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

// MainGame �ʱⰪ ����
void MainGame::Init()
{
    nScore = 0;
    nLevel = 0;

    // �÷��̾�� ��ü�� �����Լ� ����
    if (pPlayer != nullptr) pPlayer->Init();
    if (pObject != nullptr) pObject->Init();
}

void MainGame::Update()
{
    InvalidateRect(g_hWnd, NULL, false);   // NULL�� ��쿡�� ��� ������ �������ش�. 

    nLevel = nScore / 100 + 1;

    // �÷��̾�� ������Ʈ�� �ݺ��Լ� ����
    if (pPlayer != nullptr) pPlayer->Update();
    if (pObject != nullptr)
    {
        pObject->SetLevel(nLevel);
        pObject->Update();
    }

    RECT rt;

    // ���� �ϰ�
    for (auto iter = pObject->GetBox().begin(); iter != pObject->GetBox().end();)   // ��ü Ŭ���� �迭�� ��ȸ
    {
        RECT rtIter = iter->rt;     // �ش� ��ü�� ������ ����

        if (iter->rt.bottom > WINSIZE_Y)                            // ���� ���� ���
        {
            nScore += 5;
    
            iter = pObject->GetBox().erase(iter);
        }
    
        else if (IntersectRect(&rt, &rtIter, &pPlayer->GetRect()))  // �÷��̾�� ��ü�� ���� ���
        {
            nScore -= 20;
            iter = pObject->GetBox().erase(iter);
        }
    
        else if (PtInRect(&rtIter, g_ptMouse))                      // ���콺 �����Ϳ� ���� ���
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

    // ���� �ʱ�ȭ ���ִ� �Լ�(hdc, ������, �ٲ� ����)
    // ����۸� ������� �ʱ�ȭ
    PatBlt(g_hDC, 0, 0, WINSIZE_X, WINSIZE_Y, WHITENESS);
    
    // ����ۿ� �׸��� �׷��ش�.
    // �÷��̾�� ������Ʈ�� ����Լ� ����
    if (pPlayer != nullptr) pPlayer->Render();
    if (pObject != nullptr) pObject->Render();

    // ������ ���� ��� �κ�
    wstring wScore(to_wstring(nScore).c_str());
    TextOut(g_hDC, 10, 10, wScore.c_str(), wScore.length());

    wstring wLevel(to_wstring(nLevel).c_str());
    TextOut(g_hDC, 10, 30, wLevel.c_str(), wLevel.length());

    // ����ۿ��� �׸� �̹����� ������ۿ� ����
    BitBlt(hdc, 0, 0, WINSIZE_X, WINSIZE_Y, g_hDC, 0, 0, SRCCOPY);

    EndPaint(g_hWnd, &ps);
}