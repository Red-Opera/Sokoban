#pragma once
#include "GameNode.h"

class Player final : public GameNode
{
public:
	Player();
	~Player();

	// GameNode을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;

	// 플레이어의 객체를 반환하는 함수
	RECT& GetRect() { return rtBox1; }

private:
	POINT ptPos1;		// 조작되는 사각형의 위치
	RECT rtBox1;		// 플레이어 위치변수
	float fMoveSpeed;	// 플레이어 속력 변수
};