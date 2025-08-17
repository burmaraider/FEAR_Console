#pragma once

#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "../ImGui/imgui.h"

class Drawing
{
public:
	static bool bDisplay;
	static BOOL bInit;
	static bool bSetPos;
	static HRESULT SetRenderState(LPDIRECT3DDEVICE9 D3D9Device, D3DRENDERSTATETYPE State, DWORD Value);
	static HRESULT SetTexture(LPDIRECT3DDEVICE9 D3D9Device, DWORD Sampler, LPDIRECT3DBASETEXTURE9 Texture);
	static HRESULT APIENTRY EndSceneHook(LPDIRECT3DDEVICE9 D3D9Device);

private:
	static ImVec2 vWindowPos;
	static ImVec2 vWindowSize;

	static void RenderConsole();
	static void InitImGui(LPDIRECT3DDEVICE9 pDevice);
};