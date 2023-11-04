#include "stdafx.h"
#include "Player.h"

Player::Player()
{
}

Player::~Player()
{
}

// �÷��̾��� �ʱ�ȭ
void Player::Init()
{
	ptPos1 = { WINSIZE_X / 2, WINSIZE_Y - 30 };  // ���۵Ǵ� �簢���� ��ġ
	fMoveSpeed = 20;
}

// �÷��̾��� �ݺ��Լ�
void Player::Update()
{
    // ���� Ű�� ���� �����̰ų� ��� ������ ���� ���
    if (KeyManager::Get().isDown(VK_LEFT) || KeyManager::Get().isState(VK_LEFT))
    {
        if (rtBox1.left - fMoveSpeed >= 0) ptPos1.x -= fMoveSpeed;  // �̵��� ��ġ�� ���� �ƴ� ��� �÷��̾ �ش� ��ġ�� ����
    }
         
    if (KeyManager::Get().isDown(VK_RIGHT) || KeyManager::Get().isState(VK_RIGHT))
    {
        if (rtBox1.right + fMoveSpeed <= WINSIZE_X) ptPos1.x += fMoveSpeed;
    }

    // �÷��̾�� ���� ��ġ�� ������Ʈ ���ش�.
    rtBox1 = RECT_MAKE(ptPos1.x, ptPos1.y, 50);
}

// �÷��̾� ��� �Լ�
void Player::Render()
{
    RECT_DRAW(rtBox1);
}
