#pragma execution_character_set("utf-8")

#include "UI.h"

#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_sdlrenderer2.h"

#include "Cube.h"

void UI::Init(SDL_Window* window, SDL_Renderer* renderer)
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	static ImVector<ImWchar> ranges;
	static ImFontGlyphRangesBuilder builder;
	builder.AddText(u8"¥¹ÆæÊê£³ÑñÓóŒœŸ¯¿");
	builder.AddRanges(io.Fonts->GetGlyphRangesDefault());
	builder.BuildRanges(&ranges);

	io.Fonts->AddFontFromFileTTF("res/RobotoMono-Regular.ttf", 17.0f, nullptr, ranges.Data);

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.f, 6.f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10.f, 8.f));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(6.f, 4.f));

	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
	ImGui_ImplSDLRenderer2_Init(renderer);
}

void UI::ProcessEvent(SDL_Event event)
{
	ImGui_ImplSDL2_ProcessEvent(&event);  // Forward event to imgui backend
}

void UI::StartFrame()
{
	// (After event loop)
	// Start the Dear ImGui frame
	ImGui_ImplSDLRenderer2_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
	//ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());
	//ImGui::ShowDemoWindow(); // Show demo window! :)
}

void UI::DrawMenu(UI::Options* outOptions, std::function<void()> callback)
{
	ImGui::Begin("Opcje", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::SeparatorText("Opcje:");

	static float angle = outOptions->angle;
	static float vel = outOptions->vel;
	static float mass = outOptions->mass;
	static float friction = outOptions->friction;

	static float a_min = 10.f;
	static float a_max = 45.f;

	static float v_min = 0.f;
	static float v_max = 100.f;

	static float m_min = 0.5f;
	static float m_max = 20.f;

	static float f_min = 0.f;
	static float f_max = 1.f;

	ImGui::PushItemWidth(12.f * ImGui::GetFontSize());

	int dragFlags = ImGuiSliderFlags_AlwaysClamp;

	ImGui::Text("K¹t nachylenia:");
	ImGui::SliderFloat("##1", &angle, a_min, a_max, "%.f", dragFlags);

	ImGui::Text("Prêdkoœæ klocka:");
	ImGui::SliderFloat("##2", &vel, v_min, v_max, "%.f", dragFlags);

	ImGui::Text("Masa klocka:");
	ImGui::SliderFloat("##3", &mass, m_min, m_max, "%.1f", dragFlags);

	ImGui::Text("Wspó³czynnik tarcia:");
	ImGui::SliderFloat("##4", &friction, f_min, f_max, "%.3f", dragFlags);


	ImGui::SeparatorText("");
	if (ImGui::Button("Zastosuj", { -FLT_MIN, 30.0f }))
	{
		callback();
	}
	


	outOptions->angle = angle;
	outOptions->vel = vel;
	outOptions->mass = mass;
	outOptions->friction = friction;

	ImGui::End();

}


void UI::DrawStats(const Cube* cube)
{
	ImGui::Begin("Positions", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDecoration);
	ImGui::SeparatorText("Positions:");

	ImGui::PushItemWidth(13.f * ImGui::GetFontSize());

	ImGui::Text("position:");
	ImGui::Text("x: %f, y: %f", cube->pos.x, cube->pos.y);

	ImGui::Text("start position:");
	ImGui::Text("x: %f, y: %f", cube->start_pos.x, cube->start_pos.y);

	ImGui::Text("center:");
	ImGui::Text("x: %f, y: %f", cube->center.x, cube->center.y);

	ImGui::Text("forward:");
	ImGui::Text("x: %f, y: %f", cube->forward.x, cube->forward.y);

	ImGui::Text("up:");
	ImGui::Text("x: %f, y: %f", cube->up.x, cube->up.y);

	ImGui::Text("v1:");
	ImGui::Text("x: %f, y: %f", cube->vertices[0].x, cube->vertices[0].y);

	ImGui::SeparatorText("");
	ImGui::End();


	ImGui::Begin("Stats", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDecoration);
	ImGui::SeparatorText("Stats:");

	ImGui::Text("velocity:");
	ImGui::Text("x: %f", cube->vel);

	ImGui::Text("acceleration:");
	ImGui::Text("x: %f", cube->acc);

	ImGui::Text("gravity: %f", cube->gravity);
	ImGui::Text("friction: %f", cube->frictionCoeff);

	ImGui::SeparatorText("");
	ImGui::End();
}


void UI::EndFrame(SDL_Renderer* renderer)
{
	// Rendering
	// (Your code clears your framebuffer, renders your other stuff etc.)
	ImGui::Render();
	ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
}

void UI::Shutdown()
{
	ImGui_ImplSDLRenderer2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}

