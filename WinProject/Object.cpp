#include "stdafx.h"
#include "Object.h"

Object::Object()
{
}

Object::~Object()
{
}

void Object::Init()
{
	nDelay = 50;
}

void Object::Update()
{
    if (nDelay >= 50)                       // 물체 생성 대기시간이 지난 경우
    {
        tagBox box;

        // 길이가 30인 상자를 화면 밖의 랜덤한 위치에 생성
        box.rt.left = rand() % WINSIZE_X;
        box.rt.right = box.rt.left + 30;
        box.rt.top = -30;
        box.rt.bottom = 0;

        box.speed = rand() % 10 + 5;    // 떨어지는 속도 조절

        vecBox.push_back(box);

        nDelay = rand() % 40;
    }

    else nDelay += nLevel;                  // 대기시간이 지나지 않은 경우 레벨에 따라 시간 상승

    // 상자 하강
    for (auto iter = vecBox.begin(); iter != vecBox.end(); iter++)
    {
        iter->rt.top += iter->speed + nLevel;
        iter->rt.bottom += iter->speed + nLevel;
    }
}

// 모든 물체 출력함수
void Object::Render()
{
    for (const auto& rect : vecBox)
        RECT_DRAW(rect.rt);
}