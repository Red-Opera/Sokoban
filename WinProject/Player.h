#pragma once
#include "GameNode.h"

class Player final : public GameNode
{
public:
	Player();
	~Player();

	// GameNode��(��) ���� ��ӵ�
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;

	// �÷��̾��� ��ü�� ��ȯ�ϴ� �Լ�
	RECT& GetRect() { return rtBox1; }

private:
	POINT ptPos1;		// ���۵Ǵ� �簢���� ��ġ
	RECT rtBox1;		// �÷��̾� ��ġ����
	float fMoveSpeed;	// �÷��̾� �ӷ� ����
};