#pragma once
#include "MessageEvents.h"
#include "VRManager.h"
#include <queue>

enum InputType {
	VR,
	KEYBOARD,
	CONTROLLER
};

struct InputPackage {
	Control control;
	float amount;
	InputPackage() {};
	InputPackage(const Control _control, const float _amount) : control(_control), amount(_amount) {}
};

class InputManager {
private:

	struct InputBridge {
		std::unordered_map<Control, int> keyBind;
		virtual bool MapKey(Control control, int key) = 0;
		virtual InputPackage CheckForInput() = 0;
	};
	struct VRInput: public InputBridge {
		VRManager* vrMan;
		float rightTPX = 0, rightTPY = 0;
		float leftTPX = 0, leftTPY = 0;
		VRInput() {};
		VRInput(VRManager* vrManager);
		bool MapKey(Control control, int key) override;
		InputPackage CheckForInput() override;
	};
	struct KeyboardInput: public InputBridge {
		KeyboardInput();
		bool MapKey(Control control, int key);
		InputPackage CheckForInput();
	};
	struct ControllerInput: public InputBridge {
		ControllerInput();
		bool MapKey(Control control, int key);
		InputPackage CheckForInput();
	};

	static std::queue<uint64_t> inputQueue;

	InputType inputType = KEYBOARD;
	InputBridge* bridge = nullptr;

	VRManager* vrMan;

public:
	InputManager() {};
	InputManager(InputType type, VRManager* vrManager = nullptr) {
		vrMan = vrManager;
		SetInputType(type);
	};
	~InputManager() { delete bridge; };

	static void AddToQueue(uint64_t key) { inputQueue.push(key); };

	/// <summary>
	/// Called to check input devices for new user input.
	/// Should be called every frame.
	/// Results in a message sent with respective input.
	/// </summary>
	/// <returns>The control currently selected.</returns>
	InputPackage HandleInput();

	/// <summary>
	/// Retreive the current input method (Ex. VR, Keyboard, Controller, etc.)
	/// </summary>
	/// <returns>The current input method.</returns>
	inline InputType GetInputType() { return inputType; };

	/// <summary>
	/// Sets the current input method (Ex. VR, Keyboard, Controller, etc.)
	/// </summary>
	/// <param name="type">The new input method to replace the current one.</param>
	void SetInputType(InputType type);
};
