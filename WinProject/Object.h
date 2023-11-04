#pragma once
#include "GameNode.h"

// 모든 물체를 관리하는 클래스
class Object final : public GameNode
{
private:
	// 물체의 정보를 담는 구조체
	struct tagBox
	{
		RECT rt;
		float speed;
	};

public:
	Object();
	~Object();

	// GameNode을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;

	// 레벨을 저장하는 함수
	void SetLevel(int n) { nLevel = n; }

	// 물체의 배열 자체를 반환하는 함수
	vector<tagBox>& GetBox() { return vecBox; }
private:
	vector<tagBox> vecBox;	// 모든 물체를 저장하는 변수
	int nDelay;				// 생성 대기시간 저장함수
	int nLevel;				// 플레이어 레벨 저장하는 함수
};