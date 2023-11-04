#include "stdafx.h"
#include "Player.h"

Player::Player()
{
}

Player::~Player()
{
}

// 플레이어의 초기화
void Player::Init()
{
	ptPos1 = { WINSIZE_X / 2, WINSIZE_Y - 30 };  // 조작되는 사각형의 위치
	fMoveSpeed = 20;
}

// 플레이어의 반복함수
void Player::Update()
{
    // 왼쪽 키를 누른 순간이거나 계속 누르고 있을 경우
    if (KeyManager::Get().isDown(VK_LEFT) || KeyManager::Get().isState(VK_LEFT))
    {
        if (rtBox1.left - fMoveSpeed >= 0) ptPos1.x -= fMoveSpeed;  // 이동한 위치가 벽이 아닌 경우 플레이어를 해당 위치로 변경
    }
         
    if (KeyManager::Get().isDown(VK_RIGHT) || KeyManager::Get().isState(VK_RIGHT))
    {
        if (rtBox1.right + fMoveSpeed <= WINSIZE_X) ptPos1.x += fMoveSpeed;
    }

    // 플레이어와 적의 위치를 업데이트 해준다.
    rtBox1 = RECT_MAKE(ptPos1.x, ptPos1.y, 50);
}

// 플레이어 출력 함수
void Player::Render()
{
    RECT_DRAW(rtBox1);
}
