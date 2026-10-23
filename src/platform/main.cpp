#include <iostream>
#include <raylib.h>
#include <imgui.h>
#include <rlImGui.h>
#include <gameMain.h>
#include <asserts.h>
#include <randomStuff.h>



int main(void)
{

	extern int global_blockType;
	global_blockType = 0;

#if PRODUCTION_BUILD == 1
	SetTraceLogLevel(LOG_NONE); //no log output to console by raylib
#endif

	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(800, 450, "Test Window");

	SetExitKey(KEY_NULL); //Disable Esc from closing window
	//large fps leads to small deltaTime so imprecisions in calculations
	SetTargetFPS(240);

#pragma region imgui
	rlImGuiSetup(true);

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.FontGlobalScale = 2;

#pragma endregion

	if (!initGame()) {
		return 0; 
	}


	while (!WindowShouldClose()) {

		BeginDrawing();
		ClearBackground(BLACK);

#pragma region imgui
		rlImGuiBegin();

		//docking stuff
		ImGui::PushStyleColor(ImGuiCol_WindowBg, {});
		ImGui::PushStyleColor(ImGuiCol_DockingEmptyBg, {});
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
		ImGui::PopStyleColor(2);

#pragma endregion

		

#pragma region imgui windows

		ImGui::Begin("Select Block Type");

		ImGui::InputInt("Select Block", &global_blockType);

		ImGui::End();
		

#pragma endregion

		if (!updateGame()) {
			CloseWindow();
		}

#pragma region imgui
		rlImGuiEnd();

#pragma endregion

		EndDrawing();
	}

	CloseWindow();

	closeGame();

#pragma region imgui
	rlImGuiShutdown();
#pragma endregion

	return 0;
}