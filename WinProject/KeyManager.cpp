#include "stdafx.h"
#include "KeyManager.h"

void KeyManager::Init()
{
	ZeroMemory(key_state, sizeof(key_state));	// Ű���� �Է��� �����ϴ� �迭�� ��� 0���� �ʱ�ȭ
}

void KeyManager::Update()
{
	for (int key = 0; key < 256; key++)		// Ű���� ���� �迭�� ��� ���Ƽ�
	{
		if (GetAsyncKeyState(key) & 0x8000)		// �ش� ���ۿ��� ���� �ش� Ű�� ������ ���� �� (�ش� Ű���� � Ű�� ������ ���� ��)
		{
			switch (key_state[key])
			{
			case Up:
			case Release:
				key_state[key] = Down;				// Ű�� ���� ��� -> Ű�� ������ ���, Ű�� ������ �ʴ� ��� -> Ű�� ������ ���
				break;

			case Down:
				key_state[key] = State;				// Ű�� ���� ��� -> Ű�� ���� ü�� ������ ���
				break;

			default:
				break;
			}
		}

		else									// �ش� ���ۿ��� ���� �ش� Ű�� ������ ���� ���
		{
			switch (key_state[key])
			{
			case Down:
			case State:
				key_state[key] = Up;				// Ű�� ���� ��� -> Ű�� ���� ���, Ű�� ���� ü�� ������ ��� -> Ű�� ���� ���
				break;

			case Up:
				key_state[key] = Release;			// Ű�� ���� ��� -> Ű�� ������ ���
				break;

			default:
				break;
			}
		}
	}
}
