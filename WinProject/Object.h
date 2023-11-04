#pragma once
#include "GameNode.h"

// ��� ��ü�� �����ϴ� Ŭ����
class Object final : public GameNode
{
private:
	// ��ü�� ������ ��� ����ü
	struct tagBox
	{
		RECT rt;
		float speed;
	};

public:
	Object();
	~Object();

	// GameNode��(��) ���� ��ӵ�
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;

	// ������ �����ϴ� �Լ�
	void SetLevel(int n) { nLevel = n; }

	// ��ü�� �迭 ��ü�� ��ȯ�ϴ� �Լ�
	vector<tagBox>& GetBox() { return vecBox; }
private:
	vector<tagBox> vecBox;	// ��� ��ü�� �����ϴ� ����
	int nDelay;				// ���� ���ð� �����Լ�
	int nLevel;				// �÷��̾� ���� �����ϴ� �Լ�
};