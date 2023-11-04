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
    if (nDelay >= 50)                       // ��ü ���� ���ð��� ���� ���
    {
        tagBox box;

        // ���̰� 30�� ���ڸ� ȭ�� ���� ������ ��ġ�� ����
        box.rt.left = rand() % WINSIZE_X;
        box.rt.right = box.rt.left + 30;
        box.rt.top = -30;
        box.rt.bottom = 0;

        box.speed = rand() % 10 + 5;    // �������� �ӵ� ����

        vecBox.push_back(box);

        nDelay = rand() % 40;
    }

    else nDelay += nLevel;                  // ���ð��� ������ ���� ��� ������ ���� �ð� ���

    // ���� �ϰ�
    for (auto iter = vecBox.begin(); iter != vecBox.end(); iter++)
    {
        iter->rt.top += iter->speed + nLevel;
        iter->rt.bottom += iter->speed + nLevel;
    }
}

// ��� ��ü ����Լ�
void Object::Render()
{
    for (const auto& rect : vecBox)
        RECT_DRAW(rect.rt);
}