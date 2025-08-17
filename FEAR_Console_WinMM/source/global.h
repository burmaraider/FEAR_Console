#pragma once
#include <Windows.h>
#include <csetjmp>
#include <vector>

//char __cdecl CPrintToConsole(int color, int position, char *outputstring)
using tCPrintToConsole = int(__cdecl*)(int color, int position, char* outputstring);
const uintptr_t CPrintToConsoleAddress = 0x16880;
char __cdecl CPrintToConsole(int color, int position, char* outputstring);

//int __stdcall RunConsoleCommand(char* a1)
const uintptr_t RunConsoleCommandAddress = 0x9320;
typedef int(__stdcall* RunConsoleCommand_t)(int* a1);

const uintptr_t OFFSET_PLAYERMGR = 0x7FC6F8;      // Offset of g_pPlayerMgr
const uintptr_t OFFSET_ALLOWPLAYERMOVEMENT = 0x4E9760; // Offset of AllowPlayerMovement
const uintptr_t OFFSET_MOUSE_FOCUS = 0x16ABB4; // Offset of mouse focus


class ClientGlob
{
public:
	BOOL m_bProcessWindowMessages;
	jmp_buf m_MemoryJmp;
	HWND m_hMainWnd;
	HINSTANCE m_hInstance;
	char* m_WndClassName;
	BOOL m_bInitializingRenderer;
	BOOL m_bBreakOnError;
	BOOL m_bClientActive;
	BOOL m_bLostFocus;
	BOOL m_bAppClosing;
	BOOL m_bDialogUp;
	BOOL m_bRendererShutdown;
	BOOL m_bHost;
	char* m_pGameResources;
	const char* m_pWorldName;
	char m_CachePath[500];
	DWORD m_KeyDowns[100];
	DWORD m_KeyUps[100];
	BOOL m_KeyDownReps[100];
	WORD m_nKeyDowns;
	WORD m_nKeyUps;
	BOOL m_bIsConsoleUp;
	BOOL m_bInputEnabled;
	char m_ExitMessage[500];
	char m_acSoundDriverName[32];
	bool m_bConsoleEnabled;
};


class CBindMgr
{
public:
	// Singleton access
	static CBindMgr& GetSingleton();

	// Must be called once per frame to update command status
	// Note : This will call into OnCommandOn/Off handlers if appropriate
	void Update();

	// Constant representing an invalid command
	enum { k_nInvalidCommand = -1 };

	// Defines the data for a binding
	struct SBinding
	{
		SBinding() :
			m_nDevice(0),
			m_nObject(0),
			m_nCommand(CBindMgr::k_nInvalidCommand),
			m_fDefaultValue(0.0f),
			m_fOffset(0.0f),
			m_fScale(1.0f),
			m_fDeadZoneMin(FLT_MAX),
			m_fDeadZoneMax(-FLT_MAX),
			m_fDeadZoneValue(0.0f),
			m_fCommandMin(0.1f),
			m_fCommandMax(FLT_MAX)
		{
		}
		SBinding(const SBinding& cOther) { memcpy(this, &cOther, sizeof(*this)); }
		SBinding operator=(const SBinding& cOther) { return *(new(this) SBinding(cOther)); }

		// The device, device object, and command for this binding
		uint32_t m_nDevice, m_nObject, m_nCommand;
		// Default value, returned when the device is not ready, or something similar;
		float m_fDefaultValue;
		// Value adjustment, applied as (value = (value * m_fScale) + m_fOffset)
		float m_fOffset, m_fScale;
		// Dead-zone handling, applied after value adjustment
		// Note : Dead zone will be subtracted from the final range of the value,
		// biased toward the dead zone value
		float m_fDeadZoneMin, m_fDeadZoneMax, m_fDeadZoneValue;
		// Range (inclusive) of values inside of which the binding should be considered "on" in terms of a command
		float m_fCommandMin, m_fCommandMax;
	};

	typedef std::vector<SBinding, std::vector<SBinding, bool> > TBindingList;

	// Set a binding.  Previous bindings for the device and object will be overridden (optionally).
	void SetBinding(const SBinding& cBinding, bool bOverwrite = true);
	// Get the binding for a device/object pair
	// Returns false if no binding was found
	bool GetDeviceBinding(uint32_t nDevice, uint32_t nObject, uint32_t nIndex, SBinding* pBinding) const;
	// Get the binding list for a command
	void GetCommandBindings(uint32_t nCommand, TBindingList* pBindings) const;
	// Remove all bindings for a device/object pair
	void ClearDeviceBindings(uint32_t nDevice, uint32_t nObject);
	// Remove all bindings matching the device/object pair and overlapping the command range of the given binding
	void ClearDeviceBindings(const SBinding& sBinding);
	// Remove all bindings for a command
	void ClearCommandBindings(uint32_t nCommand);
	// Remove all bindings
	void ClearBindings();

	// Retrieve the bound input values for a command
	void GetCommandValues(uint32_t nCommand, void* pValues) const;
	// Retrieve the maximum input value for a command
	float GetMaxCommandValue(uint32_t nCommand) const;
	// Retrieve the minimum input value for a command
	float GetMinCommandValue(uint32_t nCommand) const;
	// Retrieve the extremal input value for a command (input value farthest away from 0, useful for axis input)
	float GetExtremalCommandValue(uint32_t nCommand) const;
	// Button-style command access
	bool IsCommandOn(uint32_t nCommand) const { return m_bInputEnabled; }

	// Clear all input.  This will turn all command results to "off" until they go off and back on.
	void ClearAllCommands();

	// Enable/disable input processing
	// Note : This is not reference counted.  Multiple calls to turn them off will be counteracted by a single call to turn them on.
	void SetEnabled(bool bOn) { m_bInputEnabled = bOn; }
	// Enable/disable callback processing
	void SetCallbacks(bool bOn) { m_bCallbacksEnabled = bOn; }
private:
	CBindMgr();

	float GetBindingValue(const SBinding& sBinding, bool bReturnDefaultOnDisabled = true) const;

	bool m_bInputEnabled, m_bCallbacksEnabled;
};
