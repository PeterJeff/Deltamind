#include "InputManager.h"
#include <unordered_map>
#include <assert.h>
#include "Messagebox.h"
#include "PhysicsComponent.h"

#define RAD_PI 3.14159265359

#define RAD_PI_4 0.78539816339
#define RAD_3PI_4 2.35619449019
#define RAD_5PI_4 3.92699081699
#define RAD_7PI_4 5.49778714378

//VR
InputManager::VRInput::VRInput(VRManager* vrManager) {
	vrMan = vrManager;
	MapKey(none, 0);
	MapKey(forward, 1);
	MapKey(backward, 2);
	MapKey(left, 3);
	MapKey(right, 4);
	MapKey(teleport, 5);
	MapKey(leftAttack, 6);
	MapKey(rightAttack, 7);
	MapKey(menu, 8);
}
bool InputManager::VRInput::MapKey(Control control, int key) {
	if(keyBind.find(control) != keyBind.end()) {
		keyBind[control] = key;
		return true;
	} else { return false; }
}
InputPackage InputManager::VRInput::CheckForInput() {
	Control input = none;
	float amount = 0;

	vr::VREvent_t event;
	while(vrMan->pVRHMD->PollNextEvent(&event, sizeof(event))) {
		switch(event.eventType) {
		case vr::VREvent_ButtonTouch:
		{
			if(event.data.controller.button == vr::k_EButton_SteamVR_Touchpad) {
				vr::VRControllerState_t state;
				if (event.trackedDeviceIndex == vrMan->leftController.index) {
					vrMan->pVRHMD->GetControllerState(vrMan->leftController.index, &state, sizeof(state));
					leftTPX = state.rAxis[0].x;
					leftTPY = state.rAxis[0].y;
				}
				else {
					vrMan->pVRHMD->GetControllerState(vrMan->rightController.index, &state, sizeof(state));
					rightTPX = state.rAxis[0].x;
					rightTPY = state.rAxis[0].y;
				}
			}
			break;
		}
		case vr::VREvent_ButtonUntouch:
		{
			if(event.data.controller.button == vr::k_EButton_SteamVR_Touchpad) {
			}
			break;
		}
		case vr::VREvent_ButtonPress:
		{
			switch(event.data.controller.button) {
			case vr::k_EButton_ApplicationMenu:
				if(event.trackedDeviceIndex == vrMan->leftController.index) {
				
				} 
				else {
					input = teleport;
					amount = 1;
					DirectX::XMStoreFloat4x4(&vrMan->world, DirectX::XMLoadFloat4x4(&vrMan->world) * DirectX::XMMatrixTranslation(vrMan->hmdPose._31, vrMan->hmdPose._32, vrMan->hmdPose._33));
				}
				break;
			case vr::k_EButton_Grip:
				break;
			case vr::k_EButton_SteamVR_Touchpad:
				float x, y, rads;
				if (event.trackedDeviceIndex == vrMan->leftController.index) {
					x = leftTPX; 
					y = leftTPY;
				}
				else {
					x = rightTPX; 
					y = rightTPY;
				}
				if (x == 0 && y == 0) break;

				rads = atan2(x, y);
				if (rads < 0) rads += (float)(2 * RAD_PI);
				if (rads >= RAD_PI_4 && rads < RAD_3PI_4) {
					input = event.trackedDeviceIndex == vrMan->leftController.index ? leftItem3 : rightItem3;
					amount = 1;
				}
				else if (rads >= RAD_3PI_4 && rads < RAD_5PI_4) {
					input = event.trackedDeviceIndex == vrMan->leftController.index ? leftItem4 : rightItem4;
					amount = 1;
				}
				else if (rads >= RAD_5PI_4 && rads < RAD_7PI_4) {
					input = event.trackedDeviceIndex == vrMan->leftController.index ? leftItem2 : rightItem2;
					amount = 1;
				}
				else {
					input = event.trackedDeviceIndex == vrMan->leftController.index ? leftItem1 : rightItem1;
					amount = 1;
				}
				break;
			case vr::k_EButton_SteamVR_Trigger:
				if(event.trackedDeviceIndex == VRManager::leftController.index) {
					input = leftAttack;
					amount = 1.0f;
				} 
				else {
					input = rightAttack;
					amount = 1.0f;
				}
				break;
			}
			break;
		}
		case vr::VREvent_ButtonUnpress:
		{
			switch(event.data.controller.button) {
			case vr::k_EButton_ApplicationMenu:
				break;
			case vr::k_EButton_Grip:
				break;
			case vr::k_EButton_SteamVR_Touchpad:
				break;
			case vr::k_EButton_SteamVR_Trigger:
				if(event.trackedDeviceIndex == vrMan->leftController.index) {
					input = leftAttack;
					amount = 0.0f;
				} else if(event.trackedDeviceIndex == vrMan->rightController.index) {
					input = rightAttack;
					amount = 0.0f;
				}
				break;
			}
			break;
		}
		}
	}
	InputPackage message(input, amount);

	return message;
}

//Keyboard
InputManager::KeyboardInput::KeyboardInput() {
	MapKey(forward, 'W');
	MapKey(backward, 'S');
	MapKey(left, 'A');
	MapKey(right, 'D');
	MapKey(teleport, 'T');
	MapKey(leftAttack, 'B');
	MapKey(rightAttack, 'N');
}
InputPackage InputManager::KeyboardInput::CheckForInput() {
	Control input = none;
	float amount = 0;

	while(inputQueue.size() > 0) {
		int j = 0;
		for(auto value : keyBind) {
			if(value.second == inputQueue.front()) {
				input = value.first;
				amount = 1;
			}
		}
		inputQueue.pop();
	}

	InputPackage message(input, amount);
	return message;
}
bool InputManager::KeyboardInput::MapKey(Control control, int key) {
	keyBind[control] = key;
	return true;
}

//Controller
InputManager::ControllerInput::ControllerInput() {
}
InputPackage InputManager::ControllerInput::CheckForInput() {
	InputPackage message(none, 1);
	return message;
}
bool InputManager::ControllerInput::MapKey(Control control, int key) {
	if(keyBind.find(control) != keyBind.end()) {
		keyBind[control] = key;
		return true;
	} else { return false; };
}

//Input Manager
std::queue<uint64_t> InputManager::inputQueue;

InputPackage InputManager::HandleInput() {
	InputPackage input = bridge->CheckForInput();

	if(input.control != Control::none)
		MessageEvents::SendMessage(EVENT_Input, InputMessage(input.control, input.amount));

	return input;
}
void InputManager::SetInputType(InputType type) {
	if(bridge)
		delete bridge;
	inputType = type;
	switch(type) {
	case InputType::VR:
		this->bridge = new VRInput(vrMan);
		break;
	case InputType::KEYBOARD:
		bridge = new KeyboardInput();
		break;
	case InputType::CONTROLLER:
		bridge = new ControllerInput();
		break;
	default:
		Messagebox::ShowError("Input Type Error", "No InputType is defined!");
	}
}
