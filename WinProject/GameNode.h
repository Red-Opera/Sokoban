#pragma once

// Ŭ������ ������ �� �Լ�
class GameNode
{
public:
	GameNode();
	virtual ~GameNode();

	virtual void Init() = 0;		// �ʱ�ȭ �Լ� 
	virtual void Update() = 0;		// �ݺ� �Լ�
	virtual void Render() = 0;		// ��� �Լ�
};