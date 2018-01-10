#include "InputManager.h"

//Input Constructors
InputManager::VRInput::VRInput() {
	MapKey(none, 0);
	MapKey(forward, 1);
	MapKey(backward, 2);
	MapKey(left, 3);
	MapKey(right, 4);
	MapKey(teleport, 5);
	MapKey(attack, 6);
	MapKey(menu, 7);
}
InputManager::KeyboardInput::KeyboardInput() {

}
InputManager::ControllerInput::ControllerInput() {

}

//Input readers
InputPackage InputManager::VRInput::CheckForInput() {
	InputPackage message;
	message.control = Control::none;
	message.amount = 1;
	return message;
}
InputPackage InputManager::KeyboardInput::CheckForInput() {
	InputPackage message;
	message.control = Control::none;
	message.amount = 1;
	return message;
}
InputPackage InputManager::ControllerInput::CheckForInput() {
	InputPackage message;
	message.control = Control::none;
	message.amount = 1;
	return message;
}

//Input remapping
bool InputManager::VRInput::MapKey(Control control, int key) {
	if (keyBind.find(control) != keyBind.end()) {
		keyBind[control] = key;
		return true;
	}
	else return false;
}
bool InputManager::KeyboardInput::MapKey(Control control, int key) {
	if (keyBind.find(control) != keyBind.end()) {
		keyBind[control] = key;
		return true;
	}
	else return false;
}
bool InputManager::ControllerInput::MapKey(Control control, int key) {
	if (keyBind.find(control) != keyBind.end()) {
		keyBind[control] = key;
		return true;
	}
	else return false;
}

//Main interface
InputPackage InputManager::HandleInput() {
	InputPackage input = bridge.CheckForInput();
	
	//SendMessage with input
	InputMessage msg;
	msg.amount = input.amount;
	msg.c = input.control;

	InputMessage test;
	test.amount = 5.0f;
	test.c = forward;

	MessageEvents::SendMessage(EVENT_Input, test);

	return input;
}

void InputManager::SetInputType(InputType type) {
	inputType = type;
	switch (type) {
	case InputType::VR:
		bridge = VRInput();
		break;
	case InputType::KEYBOARD:
		bridge = KeyboardInput();
		break;
	case InputType::CONTROLLER:
		bridge = ControllerInput();
		break;
	default:
		CRASH;
	}
}
