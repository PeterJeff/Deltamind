#pragma once
#include "GameObject.h"
#include "Health.h"
#include "Core.h"

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
	float maxSpeed = 100.0f;
	float speed = 100.0f;
	DirectX::XMFLOAT3 target = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);

	Core* core = nullptr;

	bool reachedCore = false;
	float attackSpeed = 1;
	float attackDamage = 10;
	float timeSinceLastAttack = -1;

	unsigned eventLose = 0;
	bool hurt = false;
	double hurtTimer = 0;
	double hurtDuration = 1;
	bool sentDeathMessage = false;

public:
	EnemyBase() { tag = std::string("Enemy");  };
	void Enable() override;
	void Disable() override;
	void Destroy() override;
	void Awake(Object* obj) override;
	void Update() override;
	void Subscribe() override;
	void UnSubscribe() override;
	void OnCollision(GameObject* _other) override;


	virtual void SetGrid(HexGrid* _grid) {};
	virtual void SetCore(Core* _core) { core = _core; };
	virtual void Repath() {};
	virtual void RandomizeStats();
};
