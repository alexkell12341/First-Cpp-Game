#include <iostream>
#include <raylib.h>
#include <imgui.h>
#include <rlImGui.h>
#include <gameMain.h>
#include <asserts.h>

int main(void)
{

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

		ImGui::Begin("Test");

		ImGui::Text("Hello");
		if (ImGui::Button("Button")) {
			std::cout << "Text\n";
		}
		ImGui::SameLine();

		ImGui::PushID(2);
		if (ImGui::Button("Button")) {
			std::cout << "Second button\n";
		}
		ImGui::PopID();

		ImGui::End();

		ImGui::Begin("Second Window");

		ImGui::Text("Hello");
		ImGui::Separator();
		ImGui::NewLine();

		//if this wasnt static it would be redefined every frame (i think), which we obviously dont want
		static float a = 0;
		ImGui::SliderFloat("slider", &a, 0, 1);




		ImGui::End();

		ImGui::Begin("practice");

		if (ImGui::Button("Light Theme")) {
			ImGui::StyleColorsLight();
		}
		ImGui::SameLine();
		if (ImGui::Button("Dark Theme")) {
			ImGui::StyleColorsDark();
		}
		ImGui::SameLine();
		if (ImGui::Button("Classic Theme")) {
			ImGui::StyleColorsClassic();
		}
		
		static bool aBool = false;
		ImGui::Checkbox("Checkbox", &aBool);


		static int radioInt = 0;
		static int idk = 5;

		ImGui::RadioButton("RadioButton", &radioInt, idk);

		ImGui::SliderInt("radioint", &radioInt, 0, 5);
		ImGui::SliderInt("idk", &idk, 0, 5);


		static int c = 0;
		ImGui::InputInt("Input Int", &c);

		static float DF3initialValues[3] = { 1.0, 4.5, 6.7 };
		static float DF3resolution = 0.2;
		static float DF3min = 0.0;
		static float DF3max = 10.0;

		ImGui::DragFloat3("Drag float 3", DF3initialValues, DF3resolution, DF3min, DF3max);

		static float VertSliderInt = 0;
		ImGui::VSliderFloat("VertSlider", { 60,120 }, &VertSliderInt, 0, 5, "%.2f");

		constexpr float pi = 3.14159;
		static float angleSlideFloat = pi;
		static float angleMin = 0;
		static float angleMax = 360;

		ImGui::SliderAngle("AngleSlider", &angleSlideFloat, angleMin, angleMax);

		static float editColour = 0;
		ImGui::ColorEdit3("Colour edit", &editColour);


		static float pickColour[4] = {0,0,0};
		ImGui::ColorPicker4("Colour Picker", pickColour);


		static float progressFraction = 0.2;

		ImGui::ProgressBar(progressFraction);

		ImGui::CollapsingHeader("Collapsing header");

		ImGui::TreeNode("TreeNode");

		ImGui::BeginChild("BeginChild", { 50,50 });
		ImGui::EndChild();

		ImGui::End();

		ImGui::Begin("example");
		ImGui::Text("Speed");
		ImGui::SameLine();
		ImGui::TextDisabled("(?)");

		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::Text("Controls how fast the player moves.");
			ImGui::EndTooltip();
		}

		static char buffer[128] = "";

		ImGuiInputTextFlags flags =
			ImGuiInputTextFlags_CharsNoBlank |        // no spaces
			ImGuiInputTextFlags_EnterReturnsTrue |    // return true on Enter
			ImGuiInputTextFlags_AutoSelectAll;        // select all text when focused

		if (ImGui::InputText("Player Name", buffer, sizeof(buffer), flags))
		{
			std::cout << "Confirmed name: " << buffer << "\n";
		}
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