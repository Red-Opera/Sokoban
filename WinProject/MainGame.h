#pragma once
#include "GameNode.h"

// 플레이어와 물체를 관리하는 클래스
class MainGame final : public GameNode
{
public:
    MainGame();
    ~MainGame();

    // GameNode을(를) 통해 상속됨
    virtual void Init() override;
    virtual void Update() override;
    virtual void Render() override;

private:
    int nScore;             // 점수를 저장하는 변수
    int nLevel;             // 레벨을 저장하는 변수

    class Player* pPlayer;  // 플레이어를 저장하는 변수
    class Object* pObject;  // 물체의 배열을 저장하는 클래스의 포인터
};