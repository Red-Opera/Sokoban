#pragma once

// Ű �Է� ���¸� �����ϱ� ���� ��
enum KEY_STATE
{
	Release,
	Down,
	Up,
	State
};

// Ű ���۸� �����ϴ� �̱��� Ŭ����
class KeyManager final
{
public:
	// KeyManager Ŭ������ �ҷ����� �Լ�
	static KeyManager& Get()
	{
		static KeyManager key;
		return key;
	}

	void Init();
	void Update();

	// �ش� ���� �ε����� ����Ű�� ���� �Լ�
	bool isRelease(int key) { return key_state[key] == Release; }
	bool isDown(int key) { return key_state[key] == Down; }
	bool isUp(int key) { return key_state[key] == Up; }
	bool isState(int key) { return key_state[key] == State; }

private:
	KeyManager() = default;

private:
	KEY_STATE key_state[256];	// Ű���� ���۸� �����ϴ� ����
};

