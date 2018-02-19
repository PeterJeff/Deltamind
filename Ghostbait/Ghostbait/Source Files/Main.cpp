#include "Renderer.h"
#include "vld.h"
//#include "GameObject.h"
#include "InputManager.h"
#include "Messagebox.h"
#include "Game.h"
#include "ThreadPool.h"
#include "PhysicsManager.h"
#include "AnimatorManager.h"
#include "GhostTime.h"
#include "Projectile.h"
#include "EnemyBase.h"
#include "ViveController.h"
#include "Spawner.h"
#include "ObjectManager.h"
#include "MessageEvents.h"
#include "VRManager.h"
#include "MaterialManager.h"
#include "MeshManager.h"
#include "ControllerObject.h"
#include "PhysicsTestObj.h"
#include "ProgressBar.h"
#include "AudioManager.h"
#include "BuildTool.h"
#include "EngineStructure.h"
#include "Menu.h"

Renderer* rendInter;
Game* game;
InputManager* inputMan;
PhysicsManager* phyMan;
MemoryManager MemMan;
ObjectManager* objMan;
EngineStructure engine;
AnimatorManager* animMan;
AudioManager* audioMan;

void ExecuteAsync() {
	Console::WriteLine << "I am executed asyncly!";
	throw std::invalid_argument("ERROR: This is a test showing we can know if a thread throws an exception on its work.\n");
}

void Setup(HINSTANCE hInstance, int nCmdShow) {
	Console::Allocate();

	Window wnd(900, 900);

	if(!wnd.Initialize(hInstance, nCmdShow)) { Messagebox::ShowError("Error!!", "Main window is not initialized!"); }
	wnd.UpdateTitle(L"Ghostbait");

	_Pool_Base::RegisterMemory(&MemMan);
	Console::WriteLine << "App has been initalized!";

	//Minimize();

#pragma region testing
	//EngineStructure engine;
	//SomeCoolObject* t = new SomeCoolObject();
	//engine.ExecuteAwake();
	//engine.ExecuteUpdate();
	//delete t;
	//MessageEvents::SendMessage(EVENT_Input, InputMessage(teleport, 0.567f));
	//system("pause");
	//CleanUp();
	//Free();
	//exit(0);
#pragma endregion

	//Memory Test
	//=============================
	//WriteLine((int)sizeof(Pool<Object>(15)));
	//WriteLine((int)sizeof(Pool<SomeLeakyObject>(15)));

	//=============================
	//Multithreading Test
	//=============================

	ThreadPool::Start();
	auto temp = ThreadPool::MakeJob(ExecuteAsync);

	// check future for errors and / or completion
	// This is a proof of concept, thread decoupling with .get is still uncertain.
	try {
		temp.get();
	} catch(const std::exception& e) {
		//std::rethrow_exception(e);
		// handle it

		Console::Write << e.what();
	}
	//=============================

	rendInter = new Renderer();
	audioMan = new AudioManager();
	bool isVR = VRManager::GetInstance().Init();	
	if(isVR) {
		rendInter->Initialize(wnd);
		Console::WriteLine << "Renderer initialized......";
		inputMan = new InputManager(VR);
		if(inputMan) Console::WriteLine << "Input Manager initialized......";
		audioMan->setCamera(&VRManager::GetInstance().GetPlayerPosition());
	} else {
		Console::WriteLine << "VR not initialized! Defaulting to 2D";
		rendInter->Initialize(wnd);
		Console::WriteLine << "Renderer initialized......";
		inputMan = new InputManager(KEYBOARD);
		if(inputMan) Console::WriteLine << "Input Manager initialized......";
		audioMan->setCamera(&(rendInter->getCamera())->position);
	}
	Console::WriteLine << "Nothing's wrong here......";

	animMan = new AnimatorManager(rendInter->getAnimationManager());
	if(animMan) Console::WriteLine << "Animation Manager initialized......";
	phyMan = new PhysicsManager();
	if(phyMan) Console::WriteLine << "Physics Manager initialized......";
	objMan = new ObjectManager(&MemMan);
	if(objMan) Console::WriteLine << "Object Manager initialized......";
	objMan->Initialize(80);
	ObjectFactory::Initialize(objMan, "NOT USED STRING");

	game = new Game();
	game->Start(&engine);

	Console::WriteLine << "Object Factory Initialized......";
	ObjectFactory::RegisterPrefabBase<ControllerObject>(3);
	ObjectFactory::RegisterPrefabBase<Gun>(20);
	ObjectFactory::RegisterPrefabBase<ProgressBar>(20);
	ObjectFactory::RegisterPrefabBase<ViveController>(20);
	ObjectFactory::RegisterPrefabBase<MenuControllerItem>(20);
	ObjectFactory::RegisterPrefabBase<GameObject>(512);
	ObjectFactory::RegisterPrefabBase<Projectile>(512);
	ObjectFactory::RegisterPrefabBase<Spawner>(24);
	ObjectFactory::RegisterPrefabBase<EnemyBase>(32);
	ObjectFactory::RegisterPrefabBase<MenuCube>(5);
	ObjectFactory::RegisterPrefabBase<CoreCube>(5);
	ObjectFactory::RegisterPrefabBase<BuildTool>(20);
	ObjectFactory::RegisterPrefabBase<PhysicsTestObj>(32);
	ObjectFactory::RegisterPrefabBase<ResumeButton>(32);
	Console::WriteLine << "Prefab base registered......";

	ObjectFactory::RegisterManager<Mesh, MeshManager>(rendInter->getMeshManager());
	ObjectFactory::RegisterManager<PhysicsComponent, PhysicsManager>(phyMan);
	ObjectFactory::RegisterManager<Material, MaterialManager>(rendInter->getMaterialManager());
	ObjectFactory::RegisterManager<Animator, AnimatorManager>(animMan);
	Console::WriteLine << "Managers registered......";


	//------
	// ToDo: Find an appropriate place for these?
	// You might be able to put the above stuff with it
	//=========================================================
	TypeMap::RegisterObjectAlias<ControllerObject>("ControllerObject");
	TypeMap::RegisterObjectAlias<ViveController>("ViveController");
	TypeMap::RegisterObjectAlias<MenuControllerItem>("MenuControllerItem");
	TypeMap::RegisterObjectAlias<Gun>("Gun");
	TypeMap::RegisterObjectAlias<ProgressBar>("ProgressBar");
	TypeMap::RegisterObjectAlias<Projectile>("Projectile");
	TypeMap::RegisterObjectAlias<Spawner>("Spawner");
	TypeMap::RegisterObjectAlias<EnemyBase>("EnemyBase");
	TypeMap::RegisterObjectAlias<MenuCube>("MenuCube");
	TypeMap::RegisterObjectAlias<CoreCube>("CoreCube");
	TypeMap::RegisterObjectAlias<GameObject>("GameObject");
	TypeMap::RegisterObjectAlias<PhysicsTestObj>("PhysicsTestObj");
	TypeMap::RegisterObjectAlias<BuildTool>("BuildTool");
	TypeMap::RegisterObjectAlias<ResumeButton>("ResumeButton");
	Console::WriteLine << "Object Alias registered......";

	//------
	// Scenemanager would make this
	//
	//	
	//
	//   PLEASE STOP INSTANTIATING USING A PREFAB ID AND WEIRD MANUAL COUNTING UNLESS ASSIGNED PROGRAMATICALLY
	//   PLEASE STOP INSTANTIATING USING A PREFAB ID AND WEIRD MANUAL COUNTING UNLESS ASSIGNED PROGRAMATICALLY
	//   PLEASE STOP INSTANTIATING USING A PREFAB ID AND WEIRD MANUAL COUNTING UNLESS ASSIGNED PROGRAMATICALLY
	//
	//	 Use this:
	//																						v String Identifier
	//			ObjectFactory::CreatePrefab(&std::string("Assets/ViveController2.ghost"), "VController", true);
	//																										^ Base Class Type Default Override 
	//																						(only set this if you want the class default for EVENT_InstantiateRequestByType, stomps out previous class asignment)
	//   Then use this:
	//			
	//			MessageEvents::SendMessage(EVENT_InstantiateRequestByName_DEBUG_ONLY, InstantiateNameMessage("TheObjectNAme", { 0,0,0 }, (GameObject**)&leftController.obj));
	//	Or This:
	//			MessageEvents::SendMessage(EVENT_InstantiateRequestByType, InstantiateTypeMessage(TypeMap::GetObjectTypeID<ControllerObject>(), {1,0,1}, (GameObject**) &rightController.obj));
	//
	//
	//=========================================================
	
	
	
	
	
	
	ObjectFactory::CreatePrefab(&std::string("Assets/EmptyContainer2.ghost"), "SomeEmptyContainer"); //0 // stop using magic number prefab ID
	ObjectFactory::CreatePrefab(&std::string("Assets/ViveController2.ghost"), "ViveController", true);
	ObjectFactory::CreatePrefab(&std::string("Assets/basicSphere.ghost"), "BasicSphere");
	ObjectFactory::CreatePrefab(&std::string("Assets/ScifiRoom.ghost"), "MainRoom");
	ObjectFactory::CreatePrefab(&std::string("Assets/ProjectileSphere.ghost"), "Projectile");
	ObjectFactory::CreatePrefab(&std::string("Assets/Spawner.ghost"), "Spawner"); //5 // stop using magic number prefab ID
	ObjectFactory::CreatePrefab(&std::string("Assets/EnemyRobot.ghost"), "TestEnemy");
	ObjectFactory::CreatePrefab(&std::string("Assets/StartCube.ghost"), "startCube");
	ObjectFactory::CreatePrefab(&std::string("Assets/CoreCube.ghost"), "CoreCube");
	ObjectFactory::CreatePrefab(&std::string("Assets/WinCube.ghost"), "WinCube");
	ObjectFactory::CreatePrefab(&std::string("Assets/LoseCube.ghost"), "LoseCube"); //10 // stop using magic number prefab ID
	ObjectFactory::CreatePrefab(&std::string("Assets/EarthMage.ghost"), "EarthMage");
	ObjectFactory::CreatePrefab(&std::string("Assets/PhysicsTest1.ghost"), "PhyTest1");
	ObjectFactory::CreatePrefab(&std::string("Assets/PhysicsTest2.ghost"), "PhyTest2");
	ObjectFactory::CreatePrefab(&std::string("Assets/PlaneMap1.ghost"), "Plane");
	ObjectFactory::CreatePrefab(&std::string("Assets/OverheatBar.ghost"), "GUIOverheatTest"); //15 // stop using magic number prefab ID
	ObjectFactory::CreatePrefab(&std::string("Assets/BuildTool.ghost"), "BuildTool");
	ObjectFactory::CreatePrefab(&std::string("Assets/PhysicsTest3.ghost"), "PhyTest3");
	ObjectFactory::CreatePrefab(&std::string("Assets/MenuControllerItem.ghost"), "MenuController");
	ObjectFactory::CreatePrefab(&std::string("Assets/Gun.ghost"), "GunTest", true);
	//ObjectFactory::CreatePrefab(&std::string("Assets/Teddy.ghost"));
	ObjectFactory::CreatePrefab(&std::string("Assets/ResumeButton.ghost")); //20

	//ObjectFactory::CreatePrefab(&std::string("Assets/TeleportSphere.ghost"));
	//ObjectFactory::CreatePrefab(&std::string("Object.ghost"));
	//ObjectFactory::CreatePrefab(&std::string("Object"));
	//ObjectFactory::CreatePrefab(&std::string("SomeCoolObject"));
	//ObjectFactory::CreatePrefab(&std::string("LeftControllerObject"));
	//ObjectFactory::CreatePrefab(&std::string("RightControllerObject"));

	Console::WriteLine << "Prefabs created......";
	//=============================
	
	if(VRManager::GetInstance().IsEnabled()) VRManager::GetInstance().CreateControllers();
	//DirectX::XMFLOAT4X4 roomMatrix;
	//DirectX::XMStoreFloat4x4(&roomMatrix, DirectX::XMMatrixScaling(0.15f, 0.15f, 0.15f) * DirectX::XMMatrixTranslation(0, 3, 0));
	//MessageEvents::SendMessage(EVENT_InstantiateRequestByType, InstantiateTypeMessage(14, { 0, 0, 0 }/*roomMatrix*/));
	//MessageEvents::SendMessage(EVENT_InstantiateRequestByName_DEBUG_ONLY, InstantiateNameMessage("startCube", {4, 1.5f, 0.0f}, (GameObject**)&startCube));
	//MessageEvents::SendMessage(EVENT_InstantiateRequestByType, InstantiateTypeMessage(11, { 0, 0, 0 }, nullptr));
	//GameObject* teddy;
	//MessageEvents::SendMessage(EVENT_InstantiateRequestByType, InstantiateTypeMessage(11, {0, 0, 0}, &teddy));
	//teddy->GetComponent<Animator>()->setState("Walk");


	////********* TEMPORARY Start Cube ************
	////TODO: Should move this to games start eventually when it is supported
	//MenuCube* startCube;
	//MessageEvents::SendMessage(EVENT_InstantiateRequestByName_DEBUG_ONLY, InstantiateNameMessage<MenuCube>("startCube", {0, 1.5f, 0.0f}, &startCube));
	//DirectX::XMStoreFloat4x4(&startCube->position, DirectX::XMLoadFloat4x4(&startCube->position) * DirectX::XMMatrixScaling(0.5f, 0.5f, 0.5f));
	//startCube->Enable();
	////*******************************************


	//Spawner *spawner1, *spawner2;
	//MessageEvents::SendMessage(EVENT_InstantiateRequestByName_DEBUG_ONLY, InstantiateNameMessage<Spawner>( "Spawner", { 5.0f, 5.0f, 5.0f }, &spawner1));
	//MessageEvents::SendMessage(EVENT_InstantiateRequestByName_DEBUG_ONLY, InstantiateNameMessage<Spawner>( "Spawner", { -5.0f, 5.0f, -5.0f }, &spawner2));
	//spawner2->Enable();

	//TestArc
	MessageEvents::SendMessage(EVENT_InstantiateRequestByName_DEBUG_ONLY, InstantiateNameMessage<GameObject>("Plane", { 3.0f, -1.0f, 0.0f }, nullptr));

	
	//********************* PHYSICS TEST CODE **********************************
	//PhysicsTestObj *test1, *test2;
	//MessageEvents::SendMessage(EVENT_InstantiateRequestByName_DEBUG_ONLY, InstantiateNameMessage<PhysicsTestObj>("PhyTest1", { 0.0f, 2.0f, -1.0f }, &test1));
	////DirectX::XMStoreFloat4x4(&test1->position, DirectX::XMLoadFloat4x4(&test1->position) * DirectX::XMMatrixRotationRollPitchYaw(0.5f, 0.5f, 0.5f));
	//MessageEvents::SendMessage(EVENT_InstantiateRequestByName_DEBUG_ONLY, InstantiateNameMessage<PhysicsTestObj>("PhyTest3", { 0.0f, 1.0f, 0.0f }, &test2));
	//DirectX::XMStoreFloat4x4(&test2->position, DirectX::XMLoadFloat4x4(&test2->position) * DirectX::XMMatrixRotationRollPitchYaw(0.5f, 0.5f, 0.5f));
	//MessageEvents::SendMessage(EVENT_InstantiateRequestByName_DEBUG_ONLY, InstantiateNameMessage<PhysicsTestObj>("PhyTest2", { 2.0f, 2.0f, 0.0f }, &test2));
	//DirectX::XMStoreFloat4x4(&test2->position, DirectX::XMLoadFloat4x4(&test2->position) * DirectX::XMMatrixRotationRollPitchYaw(0.5f, 0.5f, 0.5f));
	//MessageEvents::SendMessage(EVENT_InstantiateRequestByName_DEBUG_ONLY, InstantiateNameMessage<PhysicsTestObj>("PhyTest1", { -2.0f, 2.0f, 0.0f }, nullptr));

	//dynamic_cast<PhysicsTestObj*>(test1)->isControllable = true;
	//dynamic_cast<PhysicsTestObj*>(test1)->isRayCasting = true;

	//test1->Enable();

	//***************************************************************************
	
	
	//------
	// Test Gun
	//=========================================================
	//ControllerObject *debugController;
	//MessageEvents::SendMessage(EVENT_InstantiateRequestByType, InstantiateTypeMessage<ControllerObject>({ 0,0,0 }, &debugController));
	//debugController->Init(ControllerObject::ControllerHand::LEFT);
	//debugController->AddItem(0, 1);
	//debugController->AddItem(1, 19, Gun::FireType::SEMI, 60, 50);
	//debugController->AddItem(2, 19, Gun::FireType::AUTO, 8, 25);
	//debugController->AddItem(3, 16, { 1, 2, 5 });

	GhostTime::Initalize();
	MessageEvents::Initilize();

	Console::WriteLine << "Starting Game Loop......";
}

void Loop() {
	GhostTime::Tick();
	if (!game->IsPaused()) {
		phyMan->Update();
		audioMan->Update();

	}
	else {
		VRManager::GetInstance().leftController.obj->PausedUpdate();
		VRManager::GetInstance().rightController.obj->PausedUpdate();
	}
	game->Update();
	inputMan->HandleInput();
	rendInter->Render();
}

void CleanUp() {
	if(rendInter) {
		rendInter->Destroy();
		delete rendInter;
	}
	ObjectFactory::Shutdown();
	ThreadPool::Shutdown();
	objMan->Shutdown();
	delete objMan;
	delete phyMan;
	delete inputMan;
	delete animMan;
	delete audioMan;
	if(game) {
		game->Clean();
		delete game;
	}
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
	UNREFERENCED_PARAMETER(hPrevInstance); UNREFERENCED_PARAMETER(lpCmdLine);

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GHOSTBAIT));

	Setup(hInstance, nCmdShow);

	MSG msg;
	while(true) {
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if(msg.message == WM_QUIT) { break; }
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} else {
			Loop();
		}
	}

	CleanUp();

	Console::Free();

	return (int) msg.wParam;
}
