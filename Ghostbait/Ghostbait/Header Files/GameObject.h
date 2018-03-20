#pragma once
#include "Object.h"
#include "Controlable.h"
#include "Console.h"


enum GAMEOBJECT_FLAGS {
	NONE = 0,
	PERSIST_ON_RESET = 1,
	DESTROYED = 2,
	OTHER = 4,
	OTHERI = 8,
	OTHERII = 16,
	AHHHH = 32,
};

class GameObject: public Object {
	friend class ObjectFactory;
	virtual void GameObject::SetData(char* data) {};
protected:
	std::string tag = "none";
	unsigned typeID = 0;
	unsigned updateID = 0;  //Update Delegate ID
	unsigned eventDeleteAllGameObjects = 0;
	unsigned flags = 0;
	bool destroyOnReset = true;

public:
	GameObject();
	void PersistentSubscribe();
	void PersistentUnSubscribe();
	void PersistOnReset();
	void DestroyOnReset();
	void DisableNow();

	virtual void Subscribe();
	virtual void UnSubscribe();
	virtual void Enable();
	virtual void Disable();
	virtual void Update();
	virtual void Destroy();


	virtual void OnCollision(GameObject* obj);
	virtual void OnTrigger(GameObject* obj);


	virtual void Awake(Object* obj);

	inline const std::string GetTag() const { return tag; };
	inline void SetTag(std::string _tag) { tag = _tag; };
};












