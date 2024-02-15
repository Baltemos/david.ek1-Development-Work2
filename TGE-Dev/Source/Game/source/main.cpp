#include "stdafx.h"
#include "GameWorld.h"
#include <CommonUtilities/InputHandler.h>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
//#include <thread>

CommonUtilities::InputHandler globalInputHandler;

#ifndef _RETAIL
static cu::Vector2<float> locClientWindowOffset;
static std::atomic_bool locGameViewCaptureMouse;
static std::atomic_bool locGameViewCapture;
#endif

void Go(void);

int main(const int /*argc*/, const char* /*argc*/[])
{
	Go();
	return 0;
}

#ifndef _RETAIL
void ImGuiWinProcIntercept(HWND hWnd, UINT& message, WPARAM& wParam, LPARAM& lParam)
{
	hWnd;
	message;
	wParam;
	lParam;

	switch (message)
	{
	case WM_MOUSEMOVE:
		if (locGameViewCaptureMouse)
		{
			LPARAM rest = ~(0xffff << 16 | 0xffff);

			LPARAM x = GET_X_LPARAM(lParam);
			LPARAM y = GET_Y_LPARAM(lParam);

			POINT mousePos;
			GetCursorPos(&mousePos);

			x = static_cast<LPARAM>(mousePos.x - locClientWindowOffset.x);
			y = static_cast<LPARAM>(mousePos.y - locClientWindowOffset.y);

			lParam = rest | (x | (y << 16));
		}
		else
		{
			message = WM_NULL;
		}
		break;
	case WM_MOUSEWHEEL:
		if (!locGameViewCapture || !locGameViewCaptureMouse)
		{
			message = WM_NULL;
		}
		break;

	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		if (!locGameViewCapture)
		{
			message = WM_NULL;
		}
		break;

	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
		if (!locGameViewCaptureMouse)
		{
			message = WM_NULL;
		}
		break;

	default:
		break;
	}
}

#endif

LRESULT WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	lParam;
	wParam;
	hWnd;
	switch (message)
	{
		// this message is read when the window is closed
	case WM_DESTROY:
	{
		// close the application entirely
		PostQuitMessage(0);
		return 0;
	}
	default:
#ifndef _RETAIL
		ImGuiWinProcIntercept(hWnd, message, wParam, lParam);
		//if (ImGui::GetCurrentContext() != nullptr && !ImGui::IsAnyItemActive())
		{
			if (globalInputHandler.UpdateEvents(message, wParam, lParam, hWnd))
			{
				return 0;
			}
		}
#else
		if (globalInputHandler.UpdateEvents(message, wParam, lParam, hWnd))
		{
			return 0;
		}
#endif
		}
	return 0;
	}


void Go()
{
	Tga::LoadSettings(TGE_PROJECT_SETTINGS_FILE);

	Tga::EngineConfiguration winconf;

	winconf.myApplicationName = L"TGE - Amazing Game";
	winconf.myWinProcCallback = [](HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) { return WinProc(hWnd, message, wParam, lParam); };
#ifdef _DEBUG
	winconf.myActivateDebugSystems = Tga::DebugFeature::Fps | Tga::DebugFeature::Mem | Tga::DebugFeature::Filewatcher | Tga::DebugFeature::Cpu | Tga::DebugFeature::Drawcalls | Tga::DebugFeature::OptimizeWarnings;
#elif _RELEASE
	winconf.myActivateDebugSystems = Tga::DebugFeature::None;
#else
	winconf.myActivateDebugSystems = Tga::DebugFeature::Filewatcher;
#endif

	if (!Tga::Engine::Start(winconf))
	{
		ERROR_PRINT("Fatal error! Engine could not start!");
		system("pause");
		return;
	}

	Tga::Engine& engine = *Tga::Engine::GetInstance();

#ifndef _RETAIL
	engine.BeginFrame();

	const char* dockspace_name = "dockspace";
	{
		ImGui::Begin(dockspace_name, nullptr);

		ImGuiID dockspace_id = ImGui::GetID(dockspace_name);
		ImGui::DockBuilderRemoveNode(dockspace_id);
		ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_DockSpace);
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);

		auto dock_id_left = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.1f, nullptr, &dockspace_id);
		auto dock_id_right = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Right, 0.3f, nullptr, &dockspace_id);
		auto dock_id_down = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Down, 0.4f, nullptr, &dockspace_id);

		ImGui::DockBuilderDockWindow("Game View", dockspace_id);
		ImGui::DockBuilderDockWindow("BatchExplorer", dock_id_down);
		ImGui::DockBuilderDockWindow("Sidebar", dock_id_left);
		ImGui::DockBuilderDockWindow("Entity Editor", dock_id_right);
		ImGui::DockBuilderFinish(dockspace_id);

		ImGui::End();
	}

	engine.EndFrame();
#endif // !_RETAIL

	{
		GameWorld gameWorld(globalInputHandler);
		gameWorld.Init();

		while (engine.BeginFrame())
		{
#ifndef _RETAIL
			locGameViewCaptureMouse = gameWorld.GetGameWindowCaptureMouse();
			locGameViewCapture = gameWorld.GetGameWindowCapture();
			locClientWindowOffset = gameWorld.GetViewportPosition();

			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);

			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(1.0f, 0.0f));

			ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking |
				ImGuiWindowFlags_NoDecoration |
				ImGuiWindowFlags_NoMove |
				ImGuiWindowFlags_NoBringToFrontOnFocus |
				ImGuiWindowFlags_NoNavFocus |
				ImGuiWindowFlags_NoInputs |
				ImGuiWindowFlags_NoBackground;

			ImGui::Begin(dockspace_name, nullptr, window_flags);
			ImGui::PopStyleVar(3);

			ImGuiID dockspace_id = ImGui::GetID(dockspace_name);
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
#endif // !_RETAIL

			gameWorld.Update(engine.GetDeltaTime());

			gameWorld.Render();

#ifndef _RETAIL
			ImGui::End();
#endif // !_RETAIL

			engine.EndFrame();
			globalInputHandler.UpdatePreviousInput();
		}
	}

	Tga::Engine::GetInstance()->Shutdown();
}

