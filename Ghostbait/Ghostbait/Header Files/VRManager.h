#pragma once
#include "openvr.h" //todo get rid.  (convert to ptrs and include in cpp) then uncomment namespace below
#include <DirectXMath.h> // same as above

//namespace vr {
//	class IVRSystem;
//	class IVRRenderModels;
//	class IVRCompositor;
//	enum EVREye;
//	struct HmdMatrix34_t;
////	class TrackedDevicePose_t;
//}
class ControllerObject;

#define FLOAT4X4Identity DirectX::XMFLOAT4X4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);

enum class VRControllerType {
	Left,
	Right
};

class VRManager {
private:
	DirectX::XMFLOAT4X4 leftProj;
	DirectX::XMFLOAT4X4 rightProj;
	DirectX::XMFLOAT4X4 leftEyeToHead;
	DirectX::XMFLOAT4X4 rightEyeToHead;

	DirectX::XMFLOAT4X4 VRProjectionToDirectXMatrix(vr::EVREye eye, float nearPlane, float farPlane);
	DirectX::XMFLOAT4X4 VRMatrix34ToDirectXMatrix44(vr::HmdMatrix34_t m);
	DirectX::XMFLOAT4X4 VRMatrix44ToDirectXMatrix44(vr::HmdMatrix44_t m);

	void WriteMatrix(DirectX::XMFLOAT4X4 m, int frame);

	void UpdateVRPoses();
	vr::TrackedDevicePose_t trackedDevicePose[vr::k_unMaxTrackedDeviceCount];
	void Shutdown();

public:
	struct VRController {
		int index;
		DirectX::XMFLOAT4X4 pose = FLOAT4X4Identity;
		ControllerObject* obj;
	};

	static DirectX::XMFLOAT4X4 world;
	DirectX::XMFLOAT4X4 hmdPose = FLOAT4X4Identity;
	static VRController leftController, rightController;

	vr::IVRSystem *pVRHMD;
	vr::IVRRenderModels* pVRRenderModel;
	vr::IVRCompositor* pVRCompositor;
	uint32_t RecommendedRenderHeight, RecommendedRenderWidth;

	VRManager();
	~VRManager();

	bool Init();

	void CreateControllers();

	void Vibrate(VRControllerType ctrl, unsigned short durationMs);

	void GetVRMatrices(DirectX::XMFLOAT4X4* leftProj, DirectX::XMFLOAT4X4* rightProj, DirectX::XMFLOAT4X4* leftView, DirectX::XMFLOAT4X4* rightView);
	void SendToHMD(void* leftTexture, void* rightTexture);
};
