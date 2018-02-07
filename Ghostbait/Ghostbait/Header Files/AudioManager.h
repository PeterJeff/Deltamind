#pragma once
#include "Camera.h"
#include "GameObject.h"
#include <vector>

#ifdef NDEBUG
#define AK_OPTIMIZED
#endif // !NDEBUG

class AudioManager
{
	const Camera* cam;
	std::vector<GameObject*> objects;
public:
	AudioManager();
	~AudioManager();

	void setCamera(const Camera* _camera);
	
	void Update();
};

