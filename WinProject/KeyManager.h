#pragma once

// 키 입력 상태를 저장하기 위한 값
enum KEY_STATE
{
	Release,
	Down,
	Up,
	State
};

// 키 버퍼를 관리하는 싱글톤 클래스
class KeyManager final
{
public:
	// KeyManager 클래스를 불러오는 함수
	static KeyManager& Get()
	{
		static KeyManager key;
		return key;
	}

	void Init();
	void Update();

	// 해당 버퍼 인덱스의 방향키를 저장 함수
	bool isRelease(int key) { return key_state[key] == Release; }
	bool isDown(int key) { return key_state[key] == Down; }
	bool isUp(int key) { return key_state[key] == Up; }
	bool isState(int key) { return key_state[key] == State; }

private:
	KeyManager() = default;

private:
	KEY_STATE key_state[256];	// 키보드 버퍼를 저장하는 변수
};

