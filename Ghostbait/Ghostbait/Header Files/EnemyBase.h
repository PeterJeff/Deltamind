#pragma once
#include "GameObject.h"
#include "Health.h"

class HexGrid;

class EnemyBase: public GameObject, public Health {
protected:
	enum State {
		IDLE,
		PATROL,
		ATTACK,
		DEATH,
	};
	
	State currState = IDLE;
	float maxSpeed = 2.0f;
	DirectX::XMFLOAT3 target = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);

	unsigned eventLose;

	bool hurt = false;
	double hurtTimer = 0;
	double hurtDuration = 1;

	bool sentDeathMessage = false;

public:
	EnemyBase() { tag = std::string("Enemy");  };
	void Awake() override;
	void Enable(bool _destroyOnReset = true) override;
	void Disable() override;
	void Destroy() override;
	void Update() override;
	void Subscribe() override;
	void UnSubscribe() override;
	void OnCollision(GameObject* _other) override;

	void CloneData(Object* obj);

	virtual void SetGrid(HexGrid* _grid) {};
	virtual void SetGoal(DirectX::XMFLOAT2 _goal) {};
	virtual void Repath() {};
};
