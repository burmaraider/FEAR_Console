#include "Drawing.h"
#include "Hook.h"

BOOL Drawing::bInit = FALSE; // Status of the initialization of ImGui.
bool Drawing::bDisplay = false; // Status of the menu display.
bool Drawing::bSetPos = false; // Status to update ImGui window size / position.
ImVec2 Drawing::vWindowPos = { 0, 0 }; // Last ImGui window position.
ImVec2 Drawing::vWindowSize = { 0, 0 }; // Last ImGui window size.

ImGuiTextBuffer consoleBuffer;
static bool scrollToBottom = false;

char __cdecl CPrintToConsole(int color, int position, char* outputstring)
{
	// check if string contains ANIMATION ERROR and skip printing it to the console, it will spam the console with errors.
	if (strstr(outputstring, "ANIMATION ERROR") != nullptr)
	{
		// If the string contains "ANIMATION ERROR", do not print it to the console.
		return Hook::original_CPrintToConsole(color, position, outputstring);
	}

	char buffer[1024];
	strncpy_s(buffer, sizeof(buffer), outputstring, _TRUNCATE);
	buffer[sizeof(buffer) - 1] = '\0'; // Ensure null termination
	consoleBuffer.appendf("%s\n", buffer);
	scrollToBottom = true; // Set flag to scroll to bottom after new message

	return Hook::original_CPrintToConsole(color, position, buffer);
}

HRESULT Drawing::EndSceneHook(const LPDIRECT3DDEVICE9 D3D9Device)
{
	if (!Hook::pDevice)
		Hook::pDevice = D3D9Device;

	if (!bInit)
		InitImGui(D3D9Device);

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (bDisplay)
	{
		ImGui::Begin("F.E.A.R. Console", &bDisplay);
		{
			ImGui::SetWindowSize({ 500, 300 }, ImGuiCond_Once);

			if (vWindowPos.x != 0.0f && vWindowPos.y != 0.0f && vWindowSize.x != 0.0f && vWindowSize.y != 0.0f && bSetPos)
			{
				ImGui::SetWindowPos(vWindowPos);
				ImGui::SetWindowSize(vWindowSize);
				bSetPos = false;
			}

			if (bSetPos == false)
			{
				vWindowPos = { ImGui::GetWindowPos().x, ImGui::GetWindowPos().y };
				vWindowSize = { ImGui::GetWindowSize().x, ImGui::GetWindowSize().y };
			}

			RenderConsole();
		}
		ImGui::End();
	}

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	return Hook::original_IDirect3DDevicee9_EndScene(D3D9Device);
}

HRESULT Drawing::SetRenderState(LPDIRECT3DDEVICE9 D3D9Device, D3DRENDERSTATETYPE State, DWORD Value)
{
	if (State == D3DRS_FILLMODE)
	{
		if (Hook::bWireframe)
		{
			//disable texture
			Hook::original_IDirect3DDevice9_SetTexture(D3D9Device, 0, nullptr);

			Value = D3DFILL_WIREFRAME;
			//set shademode to flat
			Hook::original_IDirect3DDevice9_SetRenderState(D3D9Device, D3DRS_SHADEMODE, D3DSHADE_FLAT);
		}
		else
		{
			Value = D3DFILL_SOLID;
			//set shademode to gouraud
			Hook::original_IDirect3DDevice9_SetRenderState(D3D9Device, D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
		}
	}

	return Hook::original_IDirect3DDevice9_SetRenderState(D3D9Device, State, Value);
}

HRESULT Drawing::SetTexture(LPDIRECT3DDEVICE9 D3D9Device, DWORD Sampler, LPDIRECT3DBASETEXTURE9 Texture)
{
	if (Hook::bWireframe)
	{
		Hook::original_IDirect3DDevice9_SetTexture(D3D9Device, 0, nullptr);
	}

	return Hook::original_IDirect3DDevice9_SetTexture(D3D9Device, Sampler, Texture);
}

void Drawing::InitImGui(const LPDIRECT3DDEVICE9 pDevice)
{
	D3DDEVICE_CREATION_PARAMETERS CP;
	pDevice->GetCreationParameters(&CP);
	Hook::window = CP.hFocusWindow;
	Hook::HookWindow();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.IniFilename = nullptr;
	io.Fonts->AddFontDefault();

	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(Hook::window);
	ImGui_ImplDX9_Init(pDevice);

	bInit = TRUE;
}

void Drawing::RenderConsole()
{

	// Begin the console child window
	ImGui::BeginChild("Console", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), true, ImGuiWindowFlags_HorizontalScrollbar);
	ImGui::TextUnformatted(consoleBuffer.begin());
	if (scrollToBottom)
		ImGui::SetScrollHereY(1.0f);
	scrollToBottom = false;
	ImGui::EndChild();

	// Input text and button layout
	static char inputBuffer[256] = "";
	ImGui::PushItemWidth(-ImGui::GetFrameHeightWithSpacing() - ImGui::CalcTextSize("Enter").x - ImGui::GetStyle().ItemSpacing.x);
	if (ImGui::InputText("##Input", inputBuffer, IM_ARRAYSIZE(inputBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
	{
		consoleBuffer.appendf("> %s\n", inputBuffer);
		Hook::original_RunConsoleCommand((int*)inputBuffer);
		inputBuffer[0] = '\0';
		scrollToBottom = true;
		ImGui::SetKeyboardFocusHere(-1); // Set focus back to the input field
	}
	ImGui::PopItemWidth();
	ImGui::SameLine();
	if (ImGui::Button("Enter"))
	{
		Hook::original_RunConsoleCommand((int*)inputBuffer);
		consoleBuffer.appendf("> %s\n", inputBuffer);
		//g_pLithtechStuff->RunConsoleString(inputBuffer);
		inputBuffer[0] = '\0';
		scrollToBottom = true;
		ImGui::SetKeyboardFocusHere(-1); // Set focus back to the input field
	}
}