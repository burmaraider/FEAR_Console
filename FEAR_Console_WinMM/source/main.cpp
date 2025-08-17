#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "Hook.h"

struct msacm32_dll {
	HMODULE dll;
	FARPROC OrignalXRegThunkEntry;
	FARPROC OrignalacmDriverAddA;
	FARPROC OrignalacmDriverAddW;
	FARPROC OrignalacmDriverClose;
	FARPROC OrignalacmDriverDetailsA;
	FARPROC OrignalacmDriverDetailsW;
	FARPROC OrignalacmDriverEnum;
	FARPROC OrignalacmDriverID;
	FARPROC OrignalacmDriverMessage;
	FARPROC OrignalacmDriverOpen;
	FARPROC OrignalacmDriverPriority;
	FARPROC OrignalacmDriverRemove;
	FARPROC OrignalacmFilterChooseA;
	FARPROC OrignalacmFilterChooseW;
	FARPROC OrignalacmFilterDetailsA;
	FARPROC OrignalacmFilterDetailsW;
	FARPROC OrignalacmFilterEnumA;
	FARPROC OrignalacmFilterEnumW;
	FARPROC OrignalacmFilterTagDetailsA;
	FARPROC OrignalacmFilterTagDetailsW;
	FARPROC OrignalacmFilterTagEnumA;
	FARPROC OrignalacmFilterTagEnumW;
	FARPROC OrignalacmFormatChooseA;
	FARPROC OrignalacmFormatChooseW;
	FARPROC OrignalacmFormatDetailsA;
	FARPROC OrignalacmFormatDetailsW;
	FARPROC OrignalacmFormatEnumA;
	FARPROC OrignalacmFormatEnumW;
	FARPROC OrignalacmFormatSuggest;
	FARPROC OrignalacmFormatTagDetailsA;
	FARPROC OrignalacmFormatTagDetailsW;
	FARPROC OrignalacmFormatTagEnumA;
	FARPROC OrignalacmFormatTagEnumW;
	FARPROC OrignalacmGetVersion;
	FARPROC OrignalacmMessage32;
	FARPROC OrignalacmMetrics;
	FARPROC OrignalacmStreamClose;
	FARPROC OrignalacmStreamConvert;
	FARPROC OrignalacmStreamMessage;
	FARPROC OrignalacmStreamOpen;
	FARPROC OrignalacmStreamPrepareHeader;
	FARPROC OrignalacmStreamReset;
	FARPROC OrignalacmStreamSize;
	FARPROC OrignalacmStreamUnprepareHeader;
} msacm32;

__declspec(naked) void FakeXRegThunkEntry() { _asm { jmp[msacm32.OrignalXRegThunkEntry] } }
__declspec(naked) void FakeacmDriverAddA() { _asm { jmp[msacm32.OrignalacmDriverAddA] } }
__declspec(naked) void FakeacmDriverAddW() { _asm { jmp[msacm32.OrignalacmDriverAddW] } }
__declspec(naked) void FakeacmDriverClose() { _asm { jmp[msacm32.OrignalacmDriverClose] } }
__declspec(naked) void FakeacmDriverDetailsA() { _asm { jmp[msacm32.OrignalacmDriverDetailsA] } }
__declspec(naked) void FakeacmDriverDetailsW() { _asm { jmp[msacm32.OrignalacmDriverDetailsW] } }
__declspec(naked) void FakeacmDriverEnum() { _asm { jmp[msacm32.OrignalacmDriverEnum] } }
__declspec(naked) void FakeacmDriverID() { _asm { jmp[msacm32.OrignalacmDriverID] } }
__declspec(naked) void FakeacmDriverMessage() { _asm { jmp[msacm32.OrignalacmDriverMessage] } }
__declspec(naked) void FakeacmDriverOpen() { _asm { jmp[msacm32.OrignalacmDriverOpen] } }
__declspec(naked) void FakeacmDriverPriority() { _asm { jmp[msacm32.OrignalacmDriverPriority] } }
__declspec(naked) void FakeacmDriverRemove() { _asm { jmp[msacm32.OrignalacmDriverRemove] } }
__declspec(naked) void FakeacmFilterChooseA() { _asm { jmp[msacm32.OrignalacmFilterChooseA] } }
__declspec(naked) void FakeacmFilterChooseW() { _asm { jmp[msacm32.OrignalacmFilterChooseW] } }
__declspec(naked) void FakeacmFilterDetailsA() { _asm { jmp[msacm32.OrignalacmFilterDetailsA] } }
__declspec(naked) void FakeacmFilterDetailsW() { _asm { jmp[msacm32.OrignalacmFilterDetailsW] } }
__declspec(naked) void FakeacmFilterEnumA() { _asm { jmp[msacm32.OrignalacmFilterEnumA] } }
__declspec(naked) void FakeacmFilterEnumW() { _asm { jmp[msacm32.OrignalacmFilterEnumW] } }
__declspec(naked) void FakeacmFilterTagDetailsA() { _asm { jmp[msacm32.OrignalacmFilterTagDetailsA] } }
__declspec(naked) void FakeacmFilterTagDetailsW() { _asm { jmp[msacm32.OrignalacmFilterTagDetailsW] } }
__declspec(naked) void FakeacmFilterTagEnumA() { _asm { jmp[msacm32.OrignalacmFilterTagEnumA] } }
__declspec(naked) void FakeacmFilterTagEnumW() { _asm { jmp[msacm32.OrignalacmFilterTagEnumW] } }
__declspec(naked) void FakeacmFormatChooseA() { _asm { jmp[msacm32.OrignalacmFormatChooseA] } }
__declspec(naked) void FakeacmFormatChooseW() { _asm { jmp[msacm32.OrignalacmFormatChooseW] } }
__declspec(naked) void FakeacmFormatDetailsA() { _asm { jmp[msacm32.OrignalacmFormatDetailsA] } }
__declspec(naked) void FakeacmFormatDetailsW() { _asm { jmp[msacm32.OrignalacmFormatDetailsW] } }
__declspec(naked) void FakeacmFormatEnumA() { _asm { jmp[msacm32.OrignalacmFormatEnumA] } }
__declspec(naked) void FakeacmFormatEnumW() { _asm { jmp[msacm32.OrignalacmFormatEnumW] } }
__declspec(naked) void FakeacmFormatSuggest() { _asm { jmp[msacm32.OrignalacmFormatSuggest] } }
__declspec(naked) void FakeacmFormatTagDetailsA() { _asm { jmp[msacm32.OrignalacmFormatTagDetailsA] } }
__declspec(naked) void FakeacmFormatTagDetailsW() { _asm { jmp[msacm32.OrignalacmFormatTagDetailsW] } }
__declspec(naked) void FakeacmFormatTagEnumA() { _asm { jmp[msacm32.OrignalacmFormatTagEnumA] } }
__declspec(naked) void FakeacmFormatTagEnumW() { _asm { jmp[msacm32.OrignalacmFormatTagEnumW] } }
__declspec(naked) void FakeacmGetVersion() { _asm { jmp[msacm32.OrignalacmGetVersion] } }
__declspec(naked) void FakeacmMessage32() { _asm { jmp[msacm32.OrignalacmMessage32] } }
__declspec(naked) void FakeacmMetrics() { _asm { jmp[msacm32.OrignalacmMetrics] } }
__declspec(naked) void FakeacmStreamClose() { _asm { jmp[msacm32.OrignalacmStreamClose] } }
__declspec(naked) void FakeacmStreamConvert() { _asm { jmp[msacm32.OrignalacmStreamConvert] } }
__declspec(naked) void FakeacmStreamMessage() { _asm { jmp[msacm32.OrignalacmStreamMessage] } }
__declspec(naked) void FakeacmStreamOpen() { _asm { jmp[msacm32.OrignalacmStreamOpen] } }
__declspec(naked) void FakeacmStreamPrepareHeader() { _asm { jmp[msacm32.OrignalacmStreamPrepareHeader] } }
__declspec(naked) void FakeacmStreamReset() { _asm { jmp[msacm32.OrignalacmStreamReset] } }
__declspec(naked) void FakeacmStreamSize() { _asm { jmp[msacm32.OrignalacmStreamSize] } }
__declspec(naked) void FakeacmStreamUnprepareHeader() { _asm { jmp[msacm32.OrignalacmStreamUnprepareHeader] } }

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
	char path[MAX_PATH];
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		CopyMemory(path + GetSystemDirectory(path, MAX_PATH - 13), "\\msacm32.dll", 14);
		msacm32.dll = LoadLibrary(path);
		if (!msacm32.dll)
		{
			MessageBox(0, "Cannot load original msacm32.dll library", "Proxy", MB_ICONERROR);
			ExitProcess(0);
		}
		msacm32.OrignalXRegThunkEntry = GetProcAddress(msacm32.dll, "XRegThunkEntry");
		msacm32.OrignalacmDriverAddA = GetProcAddress(msacm32.dll, "acmDriverAddA");
		msacm32.OrignalacmDriverAddW = GetProcAddress(msacm32.dll, "acmDriverAddW");
		msacm32.OrignalacmDriverClose = GetProcAddress(msacm32.dll, "acmDriverClose");
		msacm32.OrignalacmDriverDetailsA = GetProcAddress(msacm32.dll, "acmDriverDetailsA");
		msacm32.OrignalacmDriverDetailsW = GetProcAddress(msacm32.dll, "acmDriverDetailsW");
		msacm32.OrignalacmDriverEnum = GetProcAddress(msacm32.dll, "acmDriverEnum");
		msacm32.OrignalacmDriverID = GetProcAddress(msacm32.dll, "acmDriverID");
		msacm32.OrignalacmDriverMessage = GetProcAddress(msacm32.dll, "acmDriverMessage");
		msacm32.OrignalacmDriverOpen = GetProcAddress(msacm32.dll, "acmDriverOpen");
		msacm32.OrignalacmDriverPriority = GetProcAddress(msacm32.dll, "acmDriverPriority");
		msacm32.OrignalacmDriverRemove = GetProcAddress(msacm32.dll, "acmDriverRemove");
		msacm32.OrignalacmFilterChooseA = GetProcAddress(msacm32.dll, "acmFilterChooseA");
		msacm32.OrignalacmFilterChooseW = GetProcAddress(msacm32.dll, "acmFilterChooseW");
		msacm32.OrignalacmFilterDetailsA = GetProcAddress(msacm32.dll, "acmFilterDetailsA");
		msacm32.OrignalacmFilterDetailsW = GetProcAddress(msacm32.dll, "acmFilterDetailsW");
		msacm32.OrignalacmFilterEnumA = GetProcAddress(msacm32.dll, "acmFilterEnumA");
		msacm32.OrignalacmFilterEnumW = GetProcAddress(msacm32.dll, "acmFilterEnumW");
		msacm32.OrignalacmFilterTagDetailsA = GetProcAddress(msacm32.dll, "acmFilterTagDetailsA");
		msacm32.OrignalacmFilterTagDetailsW = GetProcAddress(msacm32.dll, "acmFilterTagDetailsW");
		msacm32.OrignalacmFilterTagEnumA = GetProcAddress(msacm32.dll, "acmFilterTagEnumA");
		msacm32.OrignalacmFilterTagEnumW = GetProcAddress(msacm32.dll, "acmFilterTagEnumW");
		msacm32.OrignalacmFormatChooseA = GetProcAddress(msacm32.dll, "acmFormatChooseA");
		msacm32.OrignalacmFormatChooseW = GetProcAddress(msacm32.dll, "acmFormatChooseW");
		msacm32.OrignalacmFormatDetailsA = GetProcAddress(msacm32.dll, "acmFormatDetailsA");
		msacm32.OrignalacmFormatDetailsW = GetProcAddress(msacm32.dll, "acmFormatDetailsW");
		msacm32.OrignalacmFormatEnumA = GetProcAddress(msacm32.dll, "acmFormatEnumA");
		msacm32.OrignalacmFormatEnumW = GetProcAddress(msacm32.dll, "acmFormatEnumW");
		msacm32.OrignalacmFormatSuggest = GetProcAddress(msacm32.dll, "acmFormatSuggest");
		msacm32.OrignalacmFormatTagDetailsA = GetProcAddress(msacm32.dll, "acmFormatTagDetailsA");
		msacm32.OrignalacmFormatTagDetailsW = GetProcAddress(msacm32.dll, "acmFormatTagDetailsW");
		msacm32.OrignalacmFormatTagEnumA = GetProcAddress(msacm32.dll, "acmFormatTagEnumA");
		msacm32.OrignalacmFormatTagEnumW = GetProcAddress(msacm32.dll, "acmFormatTagEnumW");
		msacm32.OrignalacmGetVersion = GetProcAddress(msacm32.dll, "acmGetVersion");
		msacm32.OrignalacmMessage32 = GetProcAddress(msacm32.dll, "acmMessage32");
		msacm32.OrignalacmMetrics = GetProcAddress(msacm32.dll, "acmMetrics");
		msacm32.OrignalacmStreamClose = GetProcAddress(msacm32.dll, "acmStreamClose");
		msacm32.OrignalacmStreamConvert = GetProcAddress(msacm32.dll, "acmStreamConvert");
		msacm32.OrignalacmStreamMessage = GetProcAddress(msacm32.dll, "acmStreamMessage");
		msacm32.OrignalacmStreamOpen = GetProcAddress(msacm32.dll, "acmStreamOpen");
		msacm32.OrignalacmStreamPrepareHeader = GetProcAddress(msacm32.dll, "acmStreamPrepareHeader");
		msacm32.OrignalacmStreamReset = GetProcAddress(msacm32.dll, "acmStreamReset");
		msacm32.OrignalacmStreamSize = GetProcAddress(msacm32.dll, "acmStreamSize");
		msacm32.OrignalacmStreamUnprepareHeader = GetProcAddress(msacm32.dll, "acmStreamUnprepareHeader");


		//DisableThreadLibraryCalls(hModule);
		Hook::hDDLModule = hModule;
		CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)Hook::HookDirectX, nullptr, 0, nullptr);

		break;
	}
	case DLL_PROCESS_DETACH:
	{
		Hook::UnHookDirectX();
		FreeLibrary(msacm32.dll);
	}
	break;
	}
	return TRUE;
}
