#include "stdafx.h"

#include "GameWorld.h"
#include <tge/graphics/GraphicsEngine.h>
#include <tge/drawers/SpriteDrawer.h>
#include <tge/texture/TextureManager.h>
#include <tge/drawers/DebugDrawer.h>
#include "SpriteRenderer2D.h"
#include "Transform.h"
#include "PlayerInput.h"
#include "CreatureMovementObserver.h"
#include "PlayerMovementObserver.h"
#include "Camera.h"
#include "Paralax.h"
#include "ClickableSprite.h"
#include "EditorEntity.h"
#include "EditorDragFloat.h"
#include "EditorDragFloat2.h"
#include "EditorDragFloat3.h"
#include "EditorColorPicker.h"
#include "EditorList.h"
#include "EditorManager.h"
#include "Physicbody.h"
#include "Collider2D.h"
#include "CircleCollider.h"
#include "RectangleCollider.h"
#include <tge/graphics/GraphicsStateStack.h>
#include <tge/graphics/DX11.h>
#include "FreeCamera.h"
#include "EditorColorPickerButton.h"
#include "CollisionGrid.h"
#include "Tag.h"
#include "EditorInputFloat4.h"
#include "EditorInputFilePath.h"
#include "Tilemap.h"
#include "Wind.h"
#include "EditorEntityManager.h"
#include "EditorInputFloat2.h"
#include "EditorInputFloat3.h"
#include "TilemapColliderGenerator.h"
#include "Animator.h"
#include "EditorEnum.h"
#include "EditorOptional.h"
#include "UIManager.h"
#include "UIElement.h"
#include "SceneLoaderButton.h"
#include "ScenePickerButton.h"
#include "EditorRectBounds.h"
#include "EditorCircleBounds.h"

#define RegisterComponent(Component) myComponentRegistry.Register<Component>(#Component, "Components/Defaults/" + std::string(#Component) + ".json", "Components/Editors/" + std::string(#Component) + ".json");

static GameWorld* locInstance = nullptr;

GameWorld::GameWorld(CommonUtilities::InputHandler& anInputHandler) : myInputHandler(anInputHandler), myRandomEngine(mySeed())
{
#ifndef _RETAIL
	myGameWindowCaptureMouse = false;
	myGameWindowCapture = false;
#endif

	locInstance = this;

	auto& engine = *Tga::Engine::GetInstance();
	myGameState = GameState::Game;

	//These sizes may be superfluous
	Tga::Vector2ui intResolution = engine.GetRenderSize();
	myScreenResolution = { static_cast<float>(intResolution.x), static_cast<float>(intResolution.y), 0 };
	//Tga::Vector2ui intResolution = engine.GetWindowSize();
	myScreenSize = { static_cast<float>(intResolution.x), static_cast<float>(intResolution.y), 1.0f, 0 };
	//End of comment
}

GameWorld::~GameWorld()
{
}

GameWorld* GameWorld::GetInstance()
{
	return locInstance;
}

cu::InputHandler& GameWorld::GetInputHandler()
{
	return myInputHandler;
}

EntityManager& GameWorld::GetEntityManager()
{
	return myEntityManager;
}

ComponentRegistry& GameWorld::GetComponentRegistry()
{
	return myComponentRegistry;
}

EditorRegistry& GameWorld::GetEditorRegistry()
{
	return myEditorRegistry;
}

RenderBuffer& GameWorld::GetRenderBuffer()
{
	return myRenderBuffer;
}

RenderBuffer& GameWorld::GetUIRenderBuffer()
{
	return myUIRenderBuffer;
}

AudioManager& GameWorld::GetAudioManager()
{
	return myAudioManager;
}

cu::Vector2<float> GameWorld::GetViewportSize() const
{
	return myViewportSize;
}

Tga::Vector2ui GameWorld::GetResolution() const
{
	return myResolution;
}

#ifndef _RETAIL
cu::Vector2<float> GameWorld::GetViewportPosition()
{
	return myWindowPosition;
}

bool GameWorld::GetGameWindowCaptureMouse() const
{
	return myGameWindowCaptureMouse;
}

bool GameWorld::GetGameWindowCapture() const
{
	return myGameWindowCapture;
}
#endif // !_RETAIL

void GameWorld::Init()
{
	nlohmann::json settings;
	{
		std::ifstream file(Tga::Settings::ResolveGameAssetPath("Settings.json"));
		file >> settings;
	}

	myResolution = Tga::Vector2ui(settings["RenderSize"][0], settings["RenderSize"][1]);
	myGameRenderTarget = Tga::RenderTarget::Create(myResolution);
#ifndef _RETAIL
	myGameViewRenderTarget = Tga::RenderTarget::Create(Tga::DX11::GetResolution());
#endif // !_RETAIL

	RegisterComponent(Transform);
	RegisterComponent(SpriteRenderer2D);
	RegisterComponent(CreatureInput);
	RegisterComponent(PlayerInput);
	RegisterComponent(CreatureMovementObserver);
	RegisterComponent(PlayerMovementObserver);
	RegisterComponent(MeleeEnemyObserver);
	RegisterComponent(RangedEnemyObserver);
	RegisterComponent(MeleeCrab);
	RegisterComponent(RangedCrab);
	RegisterComponent(Health);
	RegisterComponent(ExpirationTimer);
	RegisterComponent(Projectile);
	RegisterComponent(Camera);
	RegisterComponent(Physicbody);
	RegisterComponent(CircleCollider);
	RegisterComponent(RectangleCollider);
	RegisterComponent(Paralax);
	RegisterComponent(FreeCamera);
	RegisterComponent(Tag);
	RegisterComponent(Tilemap);
	RegisterComponent(TilemapColliderGenerator);
	RegisterComponent(ClickableSprite);
	RegisterComponent(Wind);
	RegisterComponent(Animator);

	// UI
	RegisterComponent(UIManager);
	RegisterComponent(UIElement);
	RegisterComponent(SceneLoaderButton);
	RegisterComponent(ScenePickerButton);

#ifndef _RETAIL

	RegisterComponent(EditorEntity);
	RegisterComponent(EditorManager);
	RegisterComponent(EditorEntityManager);
	RegisterComponent(EditorUnityManager);

	myEditorRegistry.Register<EditorDragFloat>("DragFloat");
	myEditorRegistry.Register<EditorDragFloat2>("DragFloat2");
	myEditorRegistry.Register<EditorDragFloat3>("DragFloat3");
	myEditorRegistry.Register<EditorColorPicker>("ColorPicker");
	myEditorRegistry.Register<EditorInputFloat2>("InputFloat2");
	myEditorRegistry.Register<EditorInputFloat4>("InputFloat4");
	myEditorRegistry.Register<EditorColorPickerButton>("ColorPickerButton");
	myEditorRegistry.Register<EditorInputFilePath>("InputFilePath");
	myEditorRegistry.Register<EditorInputFloat3>("InputFloat3");
	myEditorRegistry.Register<EditorList>("List");
	myEditorRegistry.Register<EditorEnum>("Enum");
	myEditorRegistry.Register<EditorOptional>("Optional");
	myEditorRegistry.Register<EditorRectBounds>("RectBounds");
	myEditorRegistry.Register<EditorCircleBounds>("CircleBounds");
#endif // !_RETAIL

	myAudioManager.Init();
	//TODO add proper level bounds
	//cu::Vector2<float> minBound = { -5000.f, -5000.f };
	//cu::Vector2<float> maxBound = { 5000.f, 5000.f };
	//InitCollisionGrid(minBound, maxBound);

	//EntityTemplate tPlayer;
	//tPlayer.AddComponent("Transform", { { "Translation", { 0, 10, 0} } });
	//tPlayer.AddComponent("SpriteRenderer2D", { { "Texture", "Sprites/t_playerPlaceholder_2.png" } });
	//tPlayer.AddComponent("PlayerInput");
	//tPlayer.AddComponent("PlayerMovementObserver");
	//tPlayer.AddComponent("Camera");
	//tPlayer.AddComponent("Physicbody");
	//tPlayer.AddComponent("RectangleCollider");
	//tPlayer.AddComponent("Health");
	//tPlayer.AddComponent("Tag", { {"Tag", "Player"} });
	//tPlayer.AddComponent("Tag", { {"Tag", "Friendly"} });
	//std::shared_ptr<Entity> player = myEntityManager.Add(tPlayer, myComponentRegistry);

	//EntityTemplate tBasicCrab;
	//tBasicCrab.AddComponent("Transform", { { "Translation", { 200, 10, 0} } });
	//tBasicCrab.AddComponent("SpriteRenderer2D", { { "Texture", "Sprites/tga_w.dds" } });
	//tBasicCrab.AddComponent("MeleeEnemyObserver");
	//tBasicCrab.AddComponent("MeleeCrab");
	//tBasicCrab.AddComponent("Physicbody");
	//tBasicCrab.AddComponent("RectangleCollider");
	//tBasicCrab.AddComponent("Health");
	//tBasicCrab.AddComponent("Tag", { {"Tag", "Enemy"} });
	//tBasicCrab.AddComponent("Tag", { {"Tag", "Hostile"} });
	//tBasicCrab.AddComponent("Tag", { {"Tag", "BasicCrab"} });
	//std::shared_ptr<Entity> basicCrab = myEntityManager.Add(tBasicCrab, myComponentRegistry);

	/*EntityTemplate tObstacle;
	tObstacle.AddComponent("Transform");
	tObstacle.AddComponent("SpriteRenderer2D", { { "Texture", "Sprites/White.dds"}, { "Size", { 100, 100 } } });
	std::shared_ptr<Entity> obstacle = myEntityManager.Add(tObstacle, myComponentRegistry);

	EntityTemplate tObstacleParalax;
	tObstacleParalax.AddComponent("Transform");
	tObstacleParalax.AddComponent("Paralax");
	tObstacleParalax.AddComponent("SpriteRenderer2D", { { "Texture", "Sprites/White.dds"}, { "Size", { 300, 50 } } });
	std::shared_ptr<Entity> obstacle2 = myEntityManager.Add(tObstacleParalax, myComponentRegistry);*/

#ifndef _RETAIL
	//EntityTemplate tCamera;
	//tCamera.AddComponent("Transform");
	//tCamera.AddComponent("Camera");
	//tCamera.AddComponent("FreeCamera");
	//myEntityManager.Add(tCamera, myComponentRegistry);

	EntityTemplate tEditorManager;
	tEditorManager.AddComponent("Transform");
	tEditorManager.AddComponent("EditorManager");
	tEditorManager.AddComponent("EditorEntityManager");
	tEditorManager.AddComponent("EditorUnityManager");
	tEditorManager.AddComponent("Camera");
	tEditorManager.AddComponent("FreeCamera");
	std::shared_ptr<Entity> editorManager = myEntityManager.Add(tEditorManager, myComponentRegistry);
#else
	nlohmann::json scene;
	std::ifstream sceneFile(Tga::Settings::ResolveGameAssetPath(settings["StartingScene"]));
	sceneFile >> scene;
	myEntityManager.LoadBatch(scene["UnityBatch"], myComponentRegistry);
	myEntityManager.LoadBatch(scene["Batch"], myComponentRegistry);
#endif // !_RETAIL
}

void GameWorld::Update(float aTimeDelta)
{
	myEntityManager.AddPending();
	myEntityManager.Update(aTimeDelta);
	Collider::RunCollisions();

	myEntityManager.RemoveDestroyed();
}

void GameWorld::Render()
{
	auto engine = Tga::Engine::GetInstance();
	auto& stateStack = engine->GetGraphicsEngine().GetGraphicsStateStack();

	stateStack.Push();
	stateStack.SetSamplerState(Tga::SamplerFilter::Point, Tga::SamplerAddressMode::Wrap);
	Tga::Camera camera;
	cu::Vector2<float> renderTargetSize = { static_cast<float>(GetResolution().x), static_cast<float>(GetResolution().y) };
	camera.SetOrtographicProjection(0, renderTargetSize.x, 0, renderTargetSize.y, -1, 1);
	stateStack.SetCamera(camera);

	myGameRenderTarget.SetAsActiveTarget();
	myGameRenderTarget.Clear({ 0.4f * 0.25f, 0.58f * 0.3f, 0.9f * 0.75f, 1.0f });

	// Game update
	Camera* activeCamera = Camera::GetActiveCamera();
	if (activeCamera)
	{
		cu::Matrix4x4<float> matrix = activeCamera->GetMatrix();
		myEntityManager.Render(engine->GetGraphicsEngine());
		myRenderBuffer.Render(matrix);
		myRenderBuffer.Clear();
	}
	else
	{
		std::cout << "[GameWorld]: Skipping render, No Active Camera." << std::endl;
	}

	// UI Rendering
	stateStack.Push();

	Tga::Camera uiCamera;
	uiCamera.SetOrtographicProjection(0, renderTargetSize.x, 0, renderTargetSize.y, -1, 1);
	stateStack.SetCamera(uiCamera);

	myUIRenderBuffer.Render(cu::Matrix4x4<float>());
	myUIRenderBuffer.Clear();

	stateStack.Pop();

#ifndef _RETAIL
	Tga::RenderTarget* renderTarget = &myGameViewRenderTarget;
	Tga::Vector2ui backbufferSize = Tga::DX11::GetResolution();
#else
	Tga::RenderTarget* renderTarget = Tga::DX11::BackBuffer;
	Tga::Vector2ui backbufferSize = Tga::DX11::GetResolution();
#endif

	stateStack.Pop();
	stateStack.SetSamplerState(Tga::SamplerFilter::Point, Tga::SamplerAddressMode::Wrap);
	renderTarget->SetAsActiveTarget();

	myViewportSize = { static_cast<float>(engine->GetWindowSize().x), static_cast<float>(engine->GetWindowSize().y) };

	Tga::SpriteSharedData shared;
	shared.myTexture = &myGameRenderTarget;

	Tga::Sprite2DInstanceData instance;
	instance.mySize = backbufferSize;
	instance.myPivot = { 0.f, 1.f };

	engine->GetGraphicsEngine().GetSpriteDrawer().Draw(shared, instance);

#ifndef _RETAIL
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_MenuBar;
	ImGui::Begin("Game View", nullptr, window_flags);

	myGameWindowCaptureMouse = ImGui::IsWindowHovered();
	myGameWindowCapture = ImGui::IsWindowFocused();

	ImVec2 windowPos = ImGui::GetWindowPos();
	ImVec2 vMin = ImGui::GetWindowContentRegionMin();
	ImVec2 vMax = ImGui::GetWindowContentRegionMax();
	ImVec2 contentAreaSize = { vMax.x - vMin.x, vMax.y - vMin.y };

	vMin.x += windowPos.x;
	vMin.y += windowPos.y;
	vMax.x += windowPos.x;
	vMax.y += windowPos.y;

	ImGui::Image(renderTarget->GetShaderResourceView(), contentAreaSize);

	myViewportSize = { vMax.x - vMin.x, vMax.y - vMin.y };
	myWindowPosition = { vMin.x, vMin.y };

	ImGui::End();

	Tga::DX11::BackBuffer->SetAsActiveTarget();
#endif
}

void GameWorld::LoadScene(const std::string& aSceneName)
{
	LoadSceneAbsolute(Tga::Settings::ResolveGameAssetPath(aSceneName));
}

void GameWorld::LoadSceneAbsolute(const std::string& aScenePath)
{
	myEntityManager.DestroyAll();
	nlohmann::json scene;
	std::ifstream sceneFile(aScenePath);
	sceneFile >> scene;
	myEntityManager.LoadBatch(scene["UnityBatch"], myComponentRegistry);
	myEntityManager.LoadBatch(scene["Batch"], myComponentRegistry);
}

void GameWorld::ReadFromFile(/*const Tga::Engine& anEngine*/) //Note: Engine should be included, but warning level 4 will prevent it from remaining unused
{
	using namespace nlohmann;
	std::ifstream variables;
	variables.open("Variables.json");
	assert(variables);
	myFile = nlohmann::json::parse(variables);
	variables.close();

	//HOW TO USE:
	// 
	//Example variable:
	//myGameObjectMovementSpeed.x = myFile["GameObjectHorizontalSpeed"];
	//Example vector:
	//myMainMenuImages = CreateImageVectorFromFile(anEngine, "mainMenuImagesAndPosition");
	//Example text vector:
	//myMainMenuText = CreateTextVectorFromFile("mainMenuTextAndPosition");
	//Example image:
	//myBackgroundImage = CreateSingleImageFromFile(anEngine, "backgroundImageAndPosition", 0);
}

std::vector<Image> GameWorld::CreateImageVectorFromFile(const Tga::Engine& anEngine, const std::string& aPath)
{
	std::vector<Image> vector;
	for (int index = 0; index < myFile[aPath].size(); index++)
	{
		vector.push_back(CreateSingleImageFromFile(anEngine, aPath, index));
	}
	return vector;
}

Image GameWorld::CreateSingleImageFromFile(const Tga::Engine& anEngine, const std::string& aPath, const int aVectorIndex)
{
	Image image;
	const std::wstring filePath = Tga::ToWstring(myFile[aPath][aVectorIndex]["path"].get<std::string>());
	image.mySharedData.myTexture = anEngine.GetTextureManager().GetTexture(filePath.c_str());
	image.mySpriteInstance.myPivot = { 0.50f, 0.50f };
	image.mySpriteInstance.mySize = image.mySharedData.myTexture->CalculateTextureSize();
	image.mySpriteInstance.myColor = Tga::Color(1, 1, 1, 1);
	float sizeX = static_cast<float>(image.mySharedData.myTexture->CalculateTextureSize().x);
	float sizeY = static_cast<float>(image.mySharedData.myTexture->CalculateTextureSize().y);
	image.mySpriteInstance.myPosition.x = static_cast<float>(myFile[aPath][aVectorIndex]["xMultiplier"]) * myScreenResolution.x - sizeX / 2;
	image.mySpriteInstance.myPosition.y = static_cast<float>(myFile[aPath][aVectorIndex]["yMultiplier"]) * myScreenResolution.y - sizeY / 2;
	return image;
}

std::vector < Tga::Text > GameWorld::CreateTextVectorFromFile(const std::string& aPath)
{
	Tga::Text text;
	std::vector < Tga::Text > vector;
	for (int index = 0; index < myFile[aPath].size(); index++)
	{
		text.SetText(myFile[aPath][index]["text"]);
		text.SetPosition(Tga::Vector2f{ static_cast<float>(myFile[aPath][index]["xMultiplier"])* myScreenResolution.x - text.GetWidth() / 2, static_cast<float>(myFile[aPath][index]["yMultiplier"])* myScreenResolution.y - text.GetHeight() / 2 });

		vector.push_back(text);
	}
	return vector;
}
