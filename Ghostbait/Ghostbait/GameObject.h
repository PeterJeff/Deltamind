#pragma once
#include "Object.h"
#include "Delegate.h"
#include "EngineStructure.h"
#include "Controlable.h"

class GameObject: public Object {
public:
	//CloneFunction(GameObject)

	GameObject() {
		EngineStructure::Awake.add([=]() {this->Awake(); });
		EngineStructure::Update.add([=]() {this->Update(); });
	}


	virtual void Awake() {}
	virtual void Update() {}
};




class SomeCoolObject: public GameObject, public Controlable {


	void Awake() {
		WriteLine("I am a cool object being awakened!");
	}

	void Update() {
		if(KeyIsDown(teleport)) {
			Write("teleport down: ");
			WriteLine(Amount(teleport));
		}

	}
};
