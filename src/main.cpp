#include <iostream>
#include <raylib.h>

#include <imgui.h>
#include <rlImGui.h>

int main()
{
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(800, 450, "Test Window");

	rlImGuiSetup(true);

	while (!WindowShouldClose()) {

		BeginDrawing();
		ClearBackground(RAYWHITE);

		rlImGuiBegin();

		DrawRectangle(50, 50, 100, 100, {255,0,0,127});
		DrawRectangle(75, 75, 100, 100, {0,255,0,127});
		

		ImGui::Begin("Test");

		ImGui::Text("Hello");
		ImGui::Button("Button");

		ImGui::End();

		ImGui::ShowDemoWindow();

		
		rlImGuiEnd();

		EndDrawing();
	}

	rlImGuiShutdown();

	CloseWindow();

	return 0;
}