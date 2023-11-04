#pragma once

// 클래스가 가져야 할 함수
class GameNode
{
public:
	GameNode();
	virtual ~GameNode();

	virtual void Init() = 0;		// 초기화 함수 
	virtual void Update() = 0;		// 반복 함수
	virtual void Render() = 0;		// 출력 함수
};