#include "Animator.h"
#include "Entity.h"
#include <tge/engine.h>
#include <tge/texture/TextureManager.h>
#include <tge/settings/settings.h>
#include <tge/util/StringCast.h>

Animation::Animation()
{
	myFrameTime = 0.0f;
	Reset();
}

bool Animation::Update(float aDeltaTime)
{
	myTimer += aDeltaTime;

	bool finished = false;

	if (myFrames.empty() == false && myFrameTime != 0)
	{
		size_t frameDiff = static_cast<size_t>(myTimer / myFrameTime);
		myTimer -= myFrameTime * static_cast<float>(frameDiff);
		myFrameIndex += frameDiff;
		if (myFrameIndex >= myFrames.size())
		{
			finished = true;
		}
		myFrameIndex %= myFrames.size();
	}

	return finished;
}

void Animation::Reset()
{
	myTimer = 0.0f;
	myFrameIndex = 0;
}

const Tga::SpriteSharedData& Animation::GetShared() const
{
	return myShared;
}

Tga::Sprite2DInstanceData Animation::GetCurrentFrame() const
{
	return myFrames[myFrameIndex];
}

void Animation::SetSpriteSheet(const Tga::SpriteSharedData& aShared, float aFrameSizeX, float aFrameSizeY, size_t aColumns, size_t aTotalCount)
{
	myFrames.clear();
	myShared = aShared;

	Tga::Vector2ui textureSize = myShared.myTexture->CalculateTextureSize();

	float spriteWidthPercentage = aFrameSizeX / static_cast<float>(textureSize.x);
	float spriteHeightPercentage = aFrameSizeY / static_cast<float>(textureSize.y);

	for (size_t i = 0; i < aTotalCount; i++)
	{
		size_t indX = i % aColumns;
		size_t indY = i / aColumns;

		float pixelsFromLeft = static_cast<float>(indX) * aFrameSizeX;
		float pixelsFromTop = static_cast<float>(indY) * aFrameSizeY;

		Tga::TextureRext spriteRect;
		spriteRect.myStartX = pixelsFromLeft / static_cast<float>(textureSize.x);
		spriteRect.myEndX = spriteRect.myStartX + spriteWidthPercentage;
		spriteRect.myEndY = 1.f - pixelsFromTop / static_cast<float>(textureSize.y);
		spriteRect.myStartY = spriteRect.myEndY + spriteHeightPercentage;

		Tga::Sprite2DInstanceData frame;
		frame.myTextureRect = spriteRect;
		frame.mySize = { aFrameSizeX, aFrameSizeY };

		myFrames.push_back(frame);
	}

}

void Animation::SetFrameTime(float aFrameTime)
{
	myFrameTime = aFrameTime;
}

float Animation::GetFrameTime() const
{
	return myFrameTime;
}

void Animation::SetFrameIndex(size_t aFrameIndex)
{
	myFrameIndex = aFrameIndex;
	myFrameIndex %= myFrames.size();
}

size_t Animation::GetFrameIndex() const
{
	return myFrameIndex;
}

size_t Animation::GetFrameCount() const
{
	return myFrames.size();
}


///////////
/// Animator
///////////

Animator::Animator()
{
	myState = nullptr;
}

Animator::~Animator()
{
	for (State* ptr : myStates)
	{
		delete ptr;
	}
	myState = nullptr;
}

bool Animator::Begin(const std::string& aStateName, bool aImmediate)
{
	if (myState)
	{
		if (myState->GetName() == aStateName)
		{
			return true;
		}
		if (aImmediate || myState->Finished())
		{
			ChangeState(aStateName);
			return true;
		}
	}
	return false;
}

void Animator::ChangeState(const std::string& aStateName)
{
	if (myState->GetName() == aStateName)
	{
		return;
	}
	for (State* state : myStates)
	{
		if (state->GetName() == aStateName)
		{
			SetState(state, true);
		}
	}
}

Animator::State* Animator::GetState(const std::string& aStateName)
{
	for (State* state : myStates)
	{
		if (state->GetName() == aStateName)
		{
			return state;
		}
	}
	return nullptr;
}

void Animator::Update(float aDeltaTime)
{
	if (myState)
	{
		if (myState->Update(aDeltaTime))
		{
			OnAnimationFinished(*this);
		}

		std::shared_ptr<SpriteRenderer2D> renderer = mySpriteRenderer.lock();

		Tga::Sprite2DInstanceData renderInstance = renderer->GetInstance();
		const Tga::Sprite2DInstanceData frameInstance = myState->GetFrame();

		renderInstance.myTextureRect = frameInstance.myTextureRect;
		renderInstance.mySize = frameInstance.mySize;

		renderer->SetInstance(renderInstance);

		renderer->SetShared(myState->GetShared());
	}
}

void Animator::Start()
{

}

void Animator::Read(const nlohmann::json& someData)
{
	mySpriteRenderer = GetEntity()->GetComponent<SpriteRenderer2D>();

	auto& states = someData["States"];

	for (auto& stateData : states)
	{
		State* state = new State();
		state->Read(stateData);
		myStates.push_back(state);
	}

	if (myStates.empty() == false)
	{
		SetState(*myStates.begin(), someData["RunOnStart"]);
	}
}

void Animator::SetState(State* aState, bool aStartImmediate)
{
	State* prevState = myState;
	if (myState)
	{
		myState->Reset();
	}
	myState = aState;

	if (aStartImmediate)
	{
		myState->Start();
	}

	OnStateChange(*this, prevState, myState);
}

///////////
/// Animator::State
///////////

Animator::State::State()
{
	myIsLooping = true;
	myIsRunning = false;
	myAnimationFinished = true;
}

bool Animator::State::Update(float aDeltaTime)
{
	bool finished = false;
	if (myIsRunning)
	{
		if (myAnimation.Update(aDeltaTime))
		{
			myAnimationFinished = true;
			finished = true;
			if (!myIsLooping)
			{
				Reset();
			}
		}
		else
		{
			myAnimationFinished = false;
		}
	}
	return finished;
}

void Animator::State::Read(const nlohmann::json& someData)
{
	myName = someData["Name"];
	myIsLooping = someData["Looping"];

	{
		auto& animation = someData["Animation"];
		Tga::SpriteSharedData shared;
		std::wstring spriteSheetPath = string_cast<std::wstring>(animation["SpriteSheet"].get<std::string>());
		shared.myTexture = Tga::Engine::GetInstance()->GetTextureManager().GetTexture(Tga::Settings::ResolveGameAssetPathW(spriteSheetPath).c_str());

		auto& frameSize = animation["FrameSize"];
		float frameSizeX = frameSize[0];
		float frameSizeY = frameSize[1];

		size_t columns = animation["Columns"];
		size_t frameCount = animation["FrameCount"];

		myAnimation.SetSpriteSheet(shared, frameSizeX, frameSizeY, columns, frameCount);

		float totalTime = animation["Time"];
		myAnimation.SetFrameTime(totalTime / static_cast<float>(frameCount));
	}
}

void Animator::State::Start()
{
	myAnimation.Reset();
	myIsRunning = true;
}

void Animator::State::Reset()
{
	myAnimation.Reset();
	myIsRunning = false;
	myAnimationFinished = true;
}

void Animator::State::SetName(const std::string& aName)
{
	myName = aName;
}

const std::string& Animator::State::GetName() const
{
	return myName;
}

bool Animator::State::IsLooping()
{
	return myIsLooping;
}

void Animator::State::SetLooping(bool aIsLooping)
{
	myIsLooping = aIsLooping;
}

bool Animator::State::Finished() const
{
	return myAnimationFinished;
}

void Animator::State::AddChangeCondition(const std::string& aCondition, Animator::State* aState)
{
	myChangeConditions.emplace(aCondition, aState);
}

const std::map<std::string, Animator::State*>& Animator::State::GetChangeConditions() const
{
	return myChangeConditions;
}

const Tga::SpriteSharedData& Animator::State::GetShared() const
{
	return myAnimation.GetShared();
}

Tga::Sprite2DInstanceData Animator::State::GetFrame() const
{
	return myAnimation.GetCurrentFrame();
}
