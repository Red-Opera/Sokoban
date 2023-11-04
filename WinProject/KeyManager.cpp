#include "stdafx.h"
#include "KeyManager.h"

void KeyManager::Init()
{
	ZeroMemory(key_state, sizeof(key_state));	// 키보드 입력을 저장하는 배열을 모두 0으로 초기화
}

void KeyManager::Update()
{
	for (int key = 0; key < 256; key++)		// 키보드 버퍼 배열을 모두 돌아서
	{
		if (GetAsyncKeyState(key) & 0x8000)		// 해당 버퍼에서 순간 해당 키를 누르고 있을 때 (해당 키에서 어떤 키로 누르고 있을 때)
		{
			switch (key_state[key])
			{
			case Up:
			case Release:
				key_state[key] = Down;				// 키를 놓을 경우 -> 키를 누르는 경우, 키를 누르지 않는 경우 -> 키를 누르는 경우
				break;

			case Down:
				key_state[key] = State;				// 키를 누른 경우 -> 키를 누른 체로 유지한 경우
				break;

			default:
				break;
			}
		}

		else									// 해당 버퍼에서 순간 해당 키를 누르지 않은 경우
		{
			switch (key_state[key])
			{
			case Down:
			case State:
				key_state[key] = Up;				// 키를 누를 경우 -> 키를 놓은 경우, 키를 누른 체로 유지한 경우 -> 키를 놓은 경우
				break;

			case Up:
				key_state[key] = Release;			// 키를 놓은 경우 -> 키를 유지할 경우
				break;

			default:
				break;
			}
		}
	}
}
