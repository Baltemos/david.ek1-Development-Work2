#pragma once
#include "Component.h"
#include "SpriteRenderer2D.h"
#include <tge/sprite/sprite.h>

class Animation
{
public:
	Animation();

	bool Update(float aDeltaTime);
	void Reset();

	const Tga::SpriteSharedData& GetShared() const;

	void SetFrameTime(float aFrameTime);
	float GetFrameTime() const;

	void SetFrameIndex(size_t aFrameIndex);
	size_t GetFrameIndex() const;

	size_t GetFrameCount() const;

	void SetSpriteSheet(const Tga::SpriteSharedData& aShared, float aFrameSizeX, float aFrameSizeY, size_t aColumns, size_t aTotalCount);
	Tga::Sprite2DInstanceData GetCurrentFrame() const;
private:
	float myFrameTime;
	float myTimer;
	size_t myFrameIndex;
	Tga::SpriteSharedData myShared;
	std::vector<Tga::Sprite2DInstanceData> myFrames;
};

class Animator : public Component
{
	public:
		class State
		{
		public:
			State();

			bool Update(float aDeltaTime);
			void Read(const nlohmann::json& someData);
			void Start();
			void Reset();

			void SetName(const std::string& aName);
			const std::string& GetName() const;

			bool IsLooping();
			void SetLooping(bool aIsLooping);

			bool Finished() const;

			void AddChangeCondition(const std::string& aCondition, Animator::State* aState);
			const std::map<std::string, State*>& GetChangeConditions() const;

			const Tga::SpriteSharedData& GetShared() const;
			Tga::Sprite2DInstanceData GetFrame() const;
		private:
			std::string myName;
			Animation myAnimation;
			bool myIsLooping;
			bool myIsRunning;
			bool myAnimationFinished;
			std::map<std::string, State*> myChangeConditions;
		};

		Animator();
		~Animator();

		bool Begin(const std::string& aStateName, bool aImmediate);

		void ChangeState(const std::string& aStateName);
		Animator::State* GetState(const std::string& aStateName);

		void Update(float aDeltaTime) override;
		void Start() override;
		void Read(const nlohmann::json& someData) override;

		Event<Animator&, State*, State*> OnStateChange;
		Event<Animator&> OnAnimationFinished;
	protected:
		void SetState(State* aState, bool aStartImmediate);
	private:
		State* myState;
		std::vector<State*> myStates;

		std::weak_ptr<SpriteRenderer2D> mySpriteRenderer;
};

