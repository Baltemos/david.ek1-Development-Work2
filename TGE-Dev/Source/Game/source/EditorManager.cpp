#include "EditorManager.h"
#include "GameWorld.h"
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/misc/cpp/imgui_stdlib.h>
#include "Camera.h"
#include "FileDialog.h"
#include "tge/util/StringCast.h"
#include <sstream>

EditorManager::EditorManager()
{
	myIsPlaying = false;
	myScene.EditorBatch = nlohmann::json(nlohmann::detail::value_t::array);
	myScene.UnityBatch = nlohmann::json(nlohmann::detail::value_t::array);
}

bool EditorManager::OpenScene()
{
	FileDialogResult result = OpenFileDialog(std::filesystem::current_path(), L".json", L"Json Files (*.json)\0*.json\0All Files (*.*)\0*.*\0");
	if (result.Succeeded)
	{
		myScene.DefaultSaveFile = string_cast<std::string>(result.Path);
		std::ifstream file(myScene.DefaultSaveFile);
		nlohmann::json data;
		file >> data;
		myScene.UnityBatch = data["UnityBatch"];
		myScene.EditorBatch = data["Batch"];
	}
	return result.Succeeded;
}

bool EditorManager::OpenUnity()
{
	FileDialogResult result = OpenFileDialog(std::filesystem::current_path(), L".json", L"Json Files (*.json)\0*.json\0All Files (*.*)\0*.*\0");
	if (result.Succeeded)
	{
		std::ifstream file(result.Path);
		nlohmann::json data;
		file >> data;
		myScene.UnityBatch = data["scene_objects"];
		auto& prefabs = data["entity_prefabs"];
		myScene.EditorBatch.insert(myScene.EditorBatch.end(), prefabs.begin(), prefabs.end());
	}
	return result.Succeeded;
}

void EditorManager::LoadScene()
{
	ClearScene();
	myEditorEntityManager.lock()->LoadBatch(myScene.EditorBatch);
	myEditorUnityManager.lock()->LoadBatch(myScene.UnityBatch);
}

void EditorManager::Save()
{
	if (myScene.DefaultSaveFile.empty())
	{
		SaveAs();
	}
	else
	{
		nlohmann::json data;
		data["Batch"] = myEditorEntityManager.lock()->GetBatch();
		data["UnityBatch"] = myScene.UnityBatch;
		std::ofstream file(myScene.DefaultSaveFile);
		file << data;
	}
}

void EditorManager::SaveAs()
{
	FileDialogResult result = SaveFileDialog(std::filesystem::current_path(), L"json", L"Json Files (*.json)\0*.json\0All Files (*.*)\0*.*\0");

	if (result.Succeeded)
	{
		myScene.DefaultSaveFile = string_cast<std::string>(result.Path);
		nlohmann::json data;
		data["Batch"] = myEditorEntityManager.lock()->GetBatch();
		data["UnityBatch"] = myScene.UnityBatch;
		std::ofstream file(myScene.DefaultSaveFile);
		file << data;
	}
}

void EditorManager::Play()
{
	myIsPlaying = true;
	myEditorEntityManager.lock()->SetEnabled(false);

	myScene.EditorBatch = myEditorEntityManager.lock()->GetBatch();

	myEditorCamera = Camera::GetActiveCamera();
	if (myEditorCamera)
	{
		myEditorCamera->SetActive(false);
	}

	ClearScene();

	EntityManager& manager = GameWorld::GetInstance()->GetEntityManager();

	manager.LoadBatch(myScene.UnityBatch, GameWorld::GetInstance()->GetComponentRegistry());
	manager.LoadBatch(myScene.EditorBatch, GameWorld::GetInstance()->GetComponentRegistry());
}

void EditorManager::StopPlay()
{
	LoadScene();

	const std::multiset<Camera*>& cameras = Camera::GetCameras();
	auto it = cameras.begin();
	if (it != cameras.end())
	{
		(**it).SetActive();
	}

	myIsPlaying = false;
	myEditorEntityManager.lock()->SetEnabled(true);
}

void EditorManager::ClearScene()
{
	for (std::shared_ptr<Entity> entity : myCaughtEntities)
	{
		entity->Destroy();
	}

	myEditorEntityManager.lock()->ClearEditorEntities();
	myEditorUnityManager.lock()->Clear();
}

void EditorManager::ClearEditor()
{
	ClearScene();

	myScene.DefaultSaveFile = std::string();
	myScene.EditorBatch = nlohmann::json(nlohmann::detail::value_t::array);
	myScene.UnityBatch = nlohmann::json(nlohmann::detail::value_t::array);
}

void EditorManager::Read(const nlohmann::json&)
{
	if (IsDestroyed() == false)
	{
		GetEntity()->SetIndestructible(true);
	}

	myEditorEntityManager = GetEntity()->GetComponent<EditorEntityManager>();
	myEditorUnityManager = GetEntity()->GetComponent<EditorUnityManager>();

	myInputHandler = &GameWorld::GetInstance()->GetInputHandler();
}

void EditorManager::Start()
{
	myOnAddEntityKey = GameWorld::GetInstance()->GetEntityManager().GetOnAddEntity().Subscribe(this, &EditorManager::onAddEntity);
}

void EditorManager::Update(float)
{
	//Draws world coordinates of cursor when pressing Control.
	Camera* activeCamera = Camera::GetActiveCamera();
	if (activeCamera && myInputHandler->IsKeyPressed(static_cast<int>(cu::Keys::CONTROL)))
	{
		POINT mousePosition = myInputHandler->GetMouseClientPosition();
		cu::Vector2<float> worldPos = activeCamera->ScreenToWorld({ static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y), 0 }).ConvertToVector2XY();
		ImVec2 editorPos = ImGui::GetMousePos();

		std::ostringstream worldStr;
		worldStr << "World: { " << worldPos.x << ", " << worldPos.y << "}";
		ImGui::GetForegroundDrawList()->AddText(ImVec2(editorPos.x + 24, editorPos.y), IM_COL32(255, 255, 255, 255), worldStr.str().c_str());

		std::ostringstream clientStr;
		clientStr << "Client: { " << mousePosition.x << ", " << mousePosition.y << "}";
		ImGui::GetForegroundDrawList()->AddText(ImVec2(editorPos.x + 24, editorPos.y + 24), IM_COL32(255, 255, 255, 255), clientStr.str().c_str());
	}
	////

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_MenuBar;
	ImGui::Begin("Game View", nullptr, window_flags);
	if (ImGui::BeginMenuBar())
	{
		if (!myIsPlaying && ImGui::MenuItem("Play"))
		{
			Play();
		}
		if (myIsPlaying && ImGui::MenuItem("Stop"))
		{
			StopPlay();
		}

		ImGui::EndMenuBar();
	}
	ImGui::End();

	if (!myIsPlaying)
	{
		ShowBatchExplorer(nullptr);
	}
}

void EditorManager::OnDestroy()
{
	ClearScene();
	GameWorld::GetInstance()->GetEntityManager().GetOnAddEntity().Unsubscribe(myOnAddEntityKey);
}

void EditorManager::ShowBatchExplorer(bool* aOpen)
{
	ImGui::Begin("BatchExplorer", aOpen);
	if (ImGui::Button("Load"))
	{
		OpenScene();
		LoadScene();
	}
	if (ImGui::Button("Load Unity"))
	{
		OpenUnity();
		LoadScene();
	}
	if (ImGui::Button("Save"))
	{
		Save();
	}
	if (ImGui::Button("Clear"))
	{
		ClearEditor();
	}
	ImGui::End();
}

void EditorManager::onAddEntity(std::shared_ptr<Entity> aEntity)
{
	myCaughtEntities.push_back(aEntity);
}
