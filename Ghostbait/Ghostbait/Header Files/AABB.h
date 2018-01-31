#pragma once
#include <DirectXMath.h>
struct AABB {
	DirectX::XMFLOAT3 min, max;

	bool operator==(const AABB& other) {
		if (min.x != other.min.x) return false;
		if (min.y != other.min.y) return false;
		if (min.z != other.min.z) return false;
		if (max.x != other.max.x) return false;
		if (max.y != other.max.y) return false;
		if (max.z != other.max.z) return false;
		return true;
	}
	bool operator!=(const AABB& other) {
		if (*this == other) return false;
		return true;
	}
};