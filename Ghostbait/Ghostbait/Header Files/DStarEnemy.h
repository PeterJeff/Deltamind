#pragma once
#include "HexGrid.h"
#include "EnemyBase.h"
#include "HexTileVector.h"
class RigidBody;

namespace DirectX { struct XMFLOAT2; }
class HexGrid;
class DStarEnemy: public EnemyBase  {
	HexGrid* grid;
	RigidBody* rb;

	std::size_t dstarId = 0;
	HexTile* goal = nullptr;
	HexTile* next = nullptr;
	HexTile* curTile = nullptr;

	unsigned eventAdd;
	unsigned eventRemove;

public:
	void SetGrid(HexGrid* _grid) override;

	void SetGoal(HexTile* _goal);
	void SetGoal(DirectX::XMFLOAT2 _goal) override;

	void Repath() override {};
	void Enable();
	void Disable();

	void Awake();
	void Update();
};
