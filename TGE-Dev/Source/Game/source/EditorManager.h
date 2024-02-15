#pragma once
#include "Component.h"
#include <CommonUtilities/InputHandler.h>
#include "EditorEntity.h"
#include "EditorEntityManager.h"
#include "EditorUnityManager.h"
#include "Camera.h"
#include "EntityManager.h"

namespace cu = CommonUtilities;

class EditorManager : public Component
{
public:
	struct Scene
	{
		std::string DefaultSaveFile;
		nlohmann::json UnityBatch;
		nlohmann::json EditorBatch;
	};

	EditorManager();
	~EditorManager() = default;

	//void SaveBatch();
	bool OpenScene();
	bool OpenUnity();

	void LoadScene();

	void Save();
	void SaveAs();

	//nlohmann::json GetBatch() const;

	void Play();
	void StopPlay();
	void ClearEditor();

protected:
	virtual void Read(const nlohmann::json& someData) override;
	virtual void Start() override;
	virtual void Update(float aTimeDelta) override;
	virtual void OnDestroy() override;
private:
	void ShowBatchExplorer(bool* aOpen);

	void onAddEntity(std::shared_ptr<Entity> aEntity);
	EventKey myOnAddEntityKey;

	std::vector<std::shared_ptr<Entity>> myCaughtEntities;

	bool myIsPlaying;
	Camera* myEditorCamera;

	std::weak_ptr<EditorEntityManager> myEditorEntityManager;
	std::weak_ptr<EditorUnityManager> myEditorUnityManager;

	cu::InputHandler* myInputHandler;

	Scene myScene;
};