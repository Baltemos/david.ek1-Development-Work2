#pragma once
#include "EntityManager.h"
#include <CommonUtilities/Timer.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <tge/text/text.h>
#include <tge/graphics/GraphicsEngine.h>
#include <tge/drawers/SpriteDrawer.h>
#include <tge/texture/TextureManager.h>
#include <tge/drawers/DebugDrawer.h>
#include <random>
#include <CommonUtilities/Vector.hpp>
#include <CommonUtilities/Matrix.hpp>
#include <memory>
#include <CommonUtilities/InputHandler.h>
#include "ComponentRegistry.h"
#include "EditorRegistry.h"
#include <tge/sprite/sprite.h>
#include "SpriteManager.h"
#include "RenderBuffer.h"
#include "MeleeEnemyObserver.h"
#include "RangedEnemyObserver.h"
#include "MeleeCrab.h"
#include "RangedCrab.h"
#include "Health.h"
#include "ExpirationTimer.h"

namespace cu = CommonUtilities;

struct Image
{
	Tga::Sprite2DInstanceData mySpriteInstance;
	Tga::SpriteSharedData mySharedData;
};

enum class GameState
{
	Game
};

class GameWorld
{
public:
	GameWorld(CommonUtilities::InputHandler& anInputHandler);
	~GameWorld();

	static GameWorld* GetInstance();

	cu::InputHandler& GetInputHandler();
	EntityManager& GetEntityManager();
	ComponentRegistry& GetComponentRegistry();
	EditorRegistry& GetEditorRegistry();
	SpriteManager& GetSpriteManager();
	RenderBuffer& GetRenderBuffer();

	cu::Vector2<float> GetViewportSize() const;
	Tga::Vector2ui GetResolution() const;
#ifndef _RETAIL
	cu::Vector2<float> GetViewportPosition();
	bool GetGameWindowCaptureMouse() const;
	bool GetGameWindowCapture() const;
#endif

	void Init();
	void Update(float aTimeDelta);
	void Render();

	void ReadFromFile(/*const Tga::Engine& anEngine*/); //Note: Engine should be included, but warning level 4 will prevent it from remaining unused
	std::vector<Image> CreateImageVectorFromFile(const Tga::Engine& anEngine, const std::string& aPath);
	Image CreateSingleImageFromFile(const Tga::Engine& anEngine, const std::string& aPath, const int aVectorIndex);
	std::vector <Tga::Text> CreateTextVectorFromFile(const std::string& aPath);
private:
	EntityManager myEntityManager;
	ComponentRegistry myComponentRegistry;
	EditorRegistry myEditorRegistry;
	SpriteManager mySpriteManager;
	RenderBuffer myRenderBuffer;

	Tga::RenderTarget myGameRenderTarget;
	Tga::Vector2ui myResolution;

#ifndef _RETAIL
	Tga::RenderTarget myGameViewRenderTarget;
	bool myGameWindowCaptureMouse;
	bool myGameWindowCapture;
	cu::Vector2<float> myWindowPosition;
#endif

	cu::Vector2<float> myViewportSize;

	cu::InputHandler& myInputHandler;
	cu::Vector3<float> myScreenResolution;
	cu::Vector4<float> myScreenSize;
	cu::Timer  myTimer;
	nlohmann::json myFile;
	std::vector<Tga::Text> myGameText;
	GameState myGameState;
	std::random_device mySeed;
	std::mt19937 myRandomEngine;
};