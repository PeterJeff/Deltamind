#pragma once

#include <DirectXMath.h>

#define MAX_LIGHTS 83

struct genericLight {
	DirectX::XMFLOAT4 color = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	DirectX::XMFLOAT3 dir;
	float radius = 0.0f;
	DirectX::XMFLOAT3 pos;
	float outerRadius = 0.0f;
};

struct lightBufferStruct {
	genericLight cpu_side_lights[MAX_LIGHTS];
	DirectX::XMFLOAT3 ambientColor = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
	float ambientIntensity = 0.5f;
	DirectX::XMFLOAT3 cameraPos;
	float padding;
};

class LightManager
{
	int numLights = 0;
	int nextID = 0;
	lightBufferStruct cpu_light_info;
	int IDList[MAX_LIGHTS];
public:
	int addLight(genericLight toAdd);
	void setAmbient(DirectX::XMFLOAT3 color, float factor);
	lightBufferStruct* getLightBuffer() { return &cpu_light_info; };
	genericLight* getLight(int ID);
	void removeLight(int ID);

	LightManager();
	~LightManager();
};