#pragma once
#include "GameNode.h"

// �÷��̾�� ��ü�� �����ϴ� Ŭ����
class MainGame final : public GameNode
{
public:
    MainGame();
    ~MainGame();

    // GameNode��(��) ���� ��ӵ�
    virtual void Init() override;
    virtual void Update() override;
    virtual void Render() override;

private:
    int nScore;             // ������ �����ϴ� ����
    int nLevel;             // ������ �����ϴ� ����

    class Player* pPlayer;  // �÷��̾ �����ϴ� ����
    class Object* pObject;  // ��ü�� �迭�� �����ϴ� Ŭ������ ������
};