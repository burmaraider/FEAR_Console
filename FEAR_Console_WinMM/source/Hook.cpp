#include "global.h"
#include "Hook.h"
#include "Drawing.h"
#include "mem.h"

LPDIRECT3DDEVICE9 Hook::pDevice = nullptr; // Direct3D9 Device Object
tEndScene Hook::original_IDirect3DDevicee9_EndScene = nullptr; // Pointer of the original EndScene function
tSetRenderState Hook::original_IDirect3DDevice9_SetRenderState = nullptr; // Pointer of the original SetRenderState function
tSetTexture Hook::original_IDirect3DDevice9_SetTexture = nullptr; // Pointer of the original SetTexture function

tReset Hook::original_IDirect3DDevice9_Reset = nullptr; // Pointer of the original Reset function
HWND Hook::window = nullptr; // Window of the current process
HMODULE Hook::hDDLModule = nullptr; // HMODULE of the DLL

int Hook::windowHeight = 0; // Height of the window
int Hook::windowWidth = 0; // Width of the window
void* Hook::d3d9Device[119]; // Array of pointer of the DirectX functions.
WNDPROC Hook::OWndProc = nullptr; // Pointer of the original window message handler.
bool Hook::bWireframe = false; // Status of the wireframe mode.

uintptr_t Hook::baseAddress = 0x0; // Base address of the module
tCPrintToConsole Hook::original_CPrintToConsole = nullptr;
ClientGlob* Hook::pClientGlob = nullptr; // Pointer to the ClientGlob structure
RunConsoleCommand_t Hook::original_RunConsoleCommand = nullptr; // Pointer of the original RunConsoleCommand function
uintptr_t Hook::gameClientShellAddress = 0x0; // Base Address of the GameClient.dll module

void Hook::HookDirectX()
{
	baseAddress = (uintptr_t)GetModuleHandle(TEXT("FearDevSP.exe"));

	while (1)
	{
		if (GetD3D9Device(d3d9Device, sizeof(d3d9Device)))
		{
			original_CPrintToConsole = reinterpret_cast<tCPrintToConsole>(baseAddress + CPrintToConsoleAddress);
			original_RunConsoleCommand = reinterpret_cast<RunConsoleCommand_t>(baseAddress + RunConsoleCommandAddress);

			MemoryUtils::WriteMemoryBytes(baseAddress + 0x7DA65, "\xE9\xB4\x0\x0\x0", 5);

			pClientGlob = (ClientGlob*)(uintptr_t)(baseAddress + 0x16C288);
			pClientGlob->m_bBreakOnError = false;

			original_IDirect3DDevicee9_EndScene = (tEndScene)d3d9Device[42];
			original_IDirect3DDevice9_SetRenderState = reinterpret_cast<tSetRenderState>(d3d9Device[57]);
			original_IDirect3DDevice9_SetTexture = reinterpret_cast<tSetTexture>(d3d9Device[65]);
			original_IDirect3DDevice9_Reset = (tReset)d3d9Device[16];
			DetourTransactionBegin();
			DetourUpdateThread(GetCurrentThread());
			DetourAttach(&(PVOID&)original_IDirect3DDevicee9_EndScene, Drawing::EndSceneHook);
			//TODO: Fix the wireframe mode
			//DetourAttach(&(PVOID&)original_IDirect3DDevice9_SetRenderState, Drawing::SetRenderState);
			//DetourAttach(&(PVOID&)original_IDirect3DDevice9_SetTexture, Drawing::SetTexture);
			DetourAttach(&(PVOID&)original_CPrintToConsole, CPrintToConsole);
			DetourTransactionCommit();
			break;
		}
		Sleep(100);
	}

	//TODO: Fix the wireframe mode
	//bool bWireframeHook = false;

	while (1) //main loop
	{
		if (!Hook::gameClientShellAddress)
		{
			Hook::gameClientShellAddress = (uintptr_t)GetModuleHandle("GameClient.dll");
			continue;
		}

		//Detect the tilde key (usually the key above Tab) to toggle the console
		if (GetAsyncKeyState(VK_OEM_3) & 1)
		{
			if (Drawing::bDisplay)
			{
				Drawing::bDisplay = false;
				Drawing::bSetPos = true;
				if (Hook::gameClientShellAddress)
				{
					SetPlayerMovement(true);
					MemoryUtils::WriteMemory<int>(Hook::baseAddress + OFFSET_MOUSE_FOCUS, 1); //Locks the mouse to the game window
				}
			}
			else
			{
				Drawing::bDisplay = true;
				Drawing::bSetPos = true;
				if (Hook::gameClientShellAddress)
				{
					SetPlayerMovement(false);
					MemoryUtils::WriteMemory<int>(Hook::baseAddress + OFFSET_MOUSE_FOCUS, 0); //Unlocks the mouse from the game window
				}
			}
		}

		if (GetAsyncKeyState(VK_F4) & 1)
		{
			//TODO: Fix the wireframe mode
			/*if (!bWireframeHook)
			{
				DetourTransactionBegin();
				DetourUpdateThread(GetCurrentThread());
				DetourAttach(&(PVOID&)original_IDirect3DDevice9_SetRenderState, Drawing::SetRenderState);
				DetourAttach(&(PVOID&)original_IDirect3DDevice9_SetTexture, Drawing::SetTexture);
				DetourTransactionCommit();
			}*/
			Hook::bWireframe = !Hook::bWireframe;
		}

		Sleep(1);
	}
}

void Hook::UnHookDirectX()
{
	if (Drawing::bInit)
	{
		UnHookWindow();
		ImGui_ImplDX9_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}

	Drawing::bInit = FALSE;

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourDetach(&(PVOID&)original_IDirect3DDevicee9_EndScene, Drawing::EndSceneHook);
	DetourDetach(&(PVOID&)original_IDirect3DDevice9_Reset, hkReset);
	DetourTransactionCommit();
}

BOOL CALLBACK Hook::enumWind(const HWND handle, LPARAM lp)
{
	DWORD procID;
	GetWindowThreadProcessId(handle, &procID);
	if (GetCurrentProcessId() != procID)
		return TRUE;

	window = handle;
	return FALSE;
}

HWND Hook::GetProcessWindow()
{
	window = nullptr;

	EnumWindows(enumWind, NULL);

	RECT size;
	if (window == nullptr)
		return nullptr;

	GetWindowRect(window, &size);

	windowWidth = size.right - size.left;
	windowHeight = size.bottom - size.top;

	windowHeight -= 29;
	windowWidth -= 5;

	return window;
}

BOOL Hook::GetD3D9Device(void** pTable, const size_t size)
{
	if (!pTable)
		return FALSE;

	IDirect3D9* pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (!pD3D)
		return FALSE;

	IDirect3DDevice9* pDummyDevice = nullptr;

	D3DPRESENT_PARAMETERS d3dpp = {};
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = GetProcessWindow();
	d3dpp.Windowed = (GetWindowLongPtr(d3dpp.hDeviceWindow, GWL_STYLE) & WS_POPUP) != 0 ? FALSE : TRUE;

	if (HRESULT dummyDevCreated = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDummyDevice); dummyDevCreated != S_OK)
	{
		d3dpp.Windowed = !d3dpp.Windowed;
		dummyDevCreated = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDummyDevice);

		if (dummyDevCreated != S_OK)
		{
			pD3D->Release();
			return FALSE;
		}
	}

	memcpy(pTable, *(void***)(pDummyDevice), size);
	pDummyDevice->Release();
	pD3D->Release();
	return TRUE;
}

void Hook::HookWindow()
{
	OWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
}


void Hook::UnHookWindow()
{
	SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)OWndProc);
}

LRESULT WINAPI Hook::WndProc(const HWND hWnd, const UINT msg, const WPARAM wParam, const LPARAM lParam)
{
	if (Hook::pClientGlob)
	{
		pClientGlob->m_bLostFocus = false;
	}

	if (Drawing::bDisplay && ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
	{
		ImGui::GetIO().MouseDrawCursor = ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow);
		return true;
	}

	if (Drawing::bInit)
		ImGui::GetIO().MouseDrawCursor = ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow);

	// Block input to main WndProc if ImGui wants to capture mouse and mouse is inside any ImGui window
	if (ImGui::GetIO().WantCaptureMouse && ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow))
	{
		return true;
	}

	if (msg == WM_CLOSE)
	{
		UnHookDirectX();
		UnHookWindow();
		TerminateProcess(GetCurrentProcess(), 0);
	}

	if (ImGui::GetIO().WantCaptureMouse)
	{
		if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow))
			return true;
		return false;
	}

	return CallWindowProc(OWndProc, hWnd, msg, wParam, lParam);
}

HRESULT Hook::hkReset(D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	Drawing::bSetPos = true;
	UnHookWindow();
	Drawing::bInit = FALSE;
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	pDevice = nullptr;

	return original_IDirect3DDevice9_Reset(pPresentationParameters);
}

void Hook::SetPlayerMovement(bool allow)
{
	void* pPlayerMgr = (void*)(gameClientShellAddress + OFFSET_PLAYERMGR);
	if (!pPlayerMgr) return;

	typedef void(__thiscall* AllowPlayerMovementFn)(void*, bool);
	AllowPlayerMovementFn AllowPlayerMovement = (AllowPlayerMovementFn)(gameClientShellAddress + OFFSET_ALLOWPLAYERMOVEMENT);

	AllowPlayerMovement(pPlayerMgr, allow);
}