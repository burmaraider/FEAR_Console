#pragma once

#include <stdint.h>
#include <Windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <TlHelp32.h>
#include <stdexcept>

class MemoryUtils {
public:
	template<typename T>
	static T ReadMemory(uintptr_t baseAddress, std::vector<uintptr_t> offsets)
	{
		uintptr_t address = baseAddress;
		for (uintptr_t offset : offsets)
		{
			uintptr_t temp = *reinterpret_cast<uintptr_t*>(address);
			if (!IsValidPointer((void*)temp))
			{
				//throw std::runtime_error("Invalid memory address encountered.");
				return T();
			}
			address = *reinterpret_cast<uintptr_t*>(address);
			address += offset;
		}
		if (!IsValidPointer(reinterpret_cast<void*>(address)))
		{
			throw std::runtime_error("Invalid memory address encountered.");
		}

		//TODO: Fix this hack for loading into apartments. This is a temporary fix.
		//Sometimes the string is stored another 4 bytes ahead of the address, so we need to check if the address is null.
		if (!*(uintptr_t*)address)
		{
			address += sizeof(uintptr_t);

			//check again
			if (!*(uintptr_t*)address)
			{
				return T();
			}

			char buffer[64];
			strcpy_s(buffer, 64, (char*)address);

			return T(buffer);

		}

		return *reinterpret_cast<T*>(address);
	}
	template<typename T>
	static T ReadMemory2(uintptr_t baseAddress, std::vector<uintptr_t> offsets)
	{
		uintptr_t address = baseAddress;
		for (uintptr_t offset : offsets)
		{
			if (!IsValidPointer(reinterpret_cast<void*>(address)))
			{
				return T();
			}

			address = *reinterpret_cast<uintptr_t*>(address);
			address += offset;
		}

		if (!IsValidPointer(reinterpret_cast<void*>(address)))
		{
			return T();
		}

		return *reinterpret_cast<T*>(address);
	}


	template<typename T>
	static T ReadMemory(uintptr_t baseAddress)
	{
		return *reinterpret_cast<T*>(baseAddress);
	}

	template<typename T>
	static void WriteMemory(uintptr_t baseAddress, std::vector<uintptr_t> offsets, T value)
	{
		uintptr_t address = baseAddress;
		for (uintptr_t offset : offsets)
		{
			address = *reinterpret_cast<uintptr_t*>(address);
			address += offset;
		}
		DWORD oldProtect;
		VirtualProtect(reinterpret_cast<void*>(address), sizeof(T), PAGE_EXECUTE_READWRITE, &oldProtect);
		*reinterpret_cast<T*>(address) = value;
		VirtualProtect(reinterpret_cast<void*>(address), sizeof(T), oldProtect, &oldProtect);
	}
	template<typename T>
	static void WriteMemory(uintptr_t baseAddress, T value)
	{
		DWORD oldProtect;
		VirtualProtect(reinterpret_cast<void*>(baseAddress), sizeof(T), PAGE_EXECUTE_READWRITE, &oldProtect);
		*reinterpret_cast<T*>(baseAddress) = value;
		VirtualProtect(reinterpret_cast<void*>(baseAddress), sizeof(T), oldProtect, &oldProtect);
	}

	static void WriteMemoryBytes(uintptr_t baseAddress, const char* values, size_t size)
	{
		DWORD oldProtect;
		VirtualProtect(reinterpret_cast<void*>(baseAddress), size, PAGE_EXECUTE_READWRITE, &oldProtect);
		memcpy(reinterpret_cast<void*>(baseAddress), values, size);
		VirtualProtect(reinterpret_cast<void*>(baseAddress), size, oldProtect, &oldProtect);
	}

	static uintptr_t GetModuleBaseAddress(const wchar_t* moduleName)
	{
		DWORD procId = GetCurrentProcessId();
		uintptr_t baseAddress = 0;
		HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
		if (snapshot != INVALID_HANDLE_VALUE) {
			MODULEENTRY32 moduleEntry;
			moduleEntry.dwSize = sizeof(MODULEENTRY32);
			if (Module32First(snapshot, &moduleEntry))
			{
				do
				{
					if (!_wcsicmp(reinterpret_cast<const wchar_t*>(moduleEntry.szModule), moduleName))
					{
						baseAddress = (uintptr_t)moduleEntry.modBaseAddr;
						break;
					}
				} while (Module32Next(snapshot, &moduleEntry));
			}
			CloseHandle(snapshot);
		}
		return baseAddress;
	}
	static bool IsValidPointer(void* ptr)
	{
		MEMORY_BASIC_INFORMATION mbi;
		if (VirtualQuery(ptr, &mbi, sizeof(mbi)))
		{
			DWORD mask = (PAGE_READWRITE | PAGE_READONLY | PAGE_WRITECOPY | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY);
			bool isValid = (mbi.Protect & mask) && !(mbi.Protect & (PAGE_GUARD | PAGE_NOACCESS));
			return isValid;
		}
		return false;
	}
private:


};

class TrampolineHook {
public:
	static bool InstallHook(void* targetFunction, void* hookFunction, unsigned char* originalBytes, size_t length) {
		if (length < 5) {
			std::cerr << "Length of the overwritten bytes must be at least 5." << std::endl;
			return false;
		}

		DWORD oldProtect;
		if (!VirtualProtect(targetFunction, length, PAGE_EXECUTE_READWRITE, &oldProtect)) {
			std::cerr << "Failed to change memory protection." << std::endl;
			return false;
		}

		// Save original bytes
		memcpy(originalBytes, targetFunction, length);

		// Calculate the relative address for the jump
		uintptr_t relativeAddress = (uintptr_t)hookFunction - (uintptr_t)targetFunction - 5;

		// Write the jump instruction
		unsigned char* p = (unsigned char*)targetFunction;
		*p = 0xE9; // JMP instruction
		*(uintptr_t*)(p + 1) = relativeAddress;

		// Fill the rest with NOPs
		for (size_t i = 5; i < length; ++i) {
			p[i] = 0x90; // NOP instruction
		}

		if (!VirtualProtect(targetFunction, length, oldProtect, &oldProtect)) {
			std::cerr << "Failed to restore memory protection." << std::endl;
			return false;
		}

		return true;
	}

	static void UninstallHook(void* targetFunction, unsigned char* originalBytes, size_t length) {
		DWORD oldProtect;
		if (!VirtualProtect(targetFunction, length, PAGE_EXECUTE_READWRITE, &oldProtect)) {
			std::cerr << "Failed to change memory protection." << std::endl;
			return;
		}

		// Restore original bytes
		memcpy(targetFunction, originalBytes, length);

		if (!VirtualProtect(targetFunction, length, oldProtect, &oldProtect)) {
			std::cerr << "Failed to restore memory protection." << std::endl;
		}
	}
};

class MidFunctionHook {
public:
	static bool InstallHook(void* targetFunction, void* hookFunction, unsigned char* originalBytes, size_t length, size_t offset) {
		if (length < 5) {
			std::cerr << "Length of the overwritten bytes must be at least 5." << std::endl;
			return false;
		}

		DWORD oldProtect;
		unsigned char* targetAddress = (unsigned char*)targetFunction + offset;

		if (!VirtualProtect(targetAddress, length, PAGE_EXECUTE_READWRITE, &oldProtect)) {
			std::cerr << "Failed to change memory protection." << std::endl;
			return false;
		}

		// Save original bytes
		memcpy(originalBytes, targetAddress, length);

		// Calculate the relative address for the jump
		uintptr_t relativeAddress = (uintptr_t)hookFunction - (uintptr_t)targetAddress - 5;

		// Write the jump instruction
		unsigned char* p = targetAddress;
		*p = 0xE9; // JMP instruction
		*(uintptr_t*)(p + 1) = relativeAddress;

		// Fill the rest with NOPs
		for (size_t i = 5; i < length; ++i) {
			p[i] = 0x90; // NOP instruction
		}

		if (!VirtualProtect(targetAddress, length, oldProtect, &oldProtect)) {
			std::cerr << "Failed to restore memory protection." << std::endl;
			return false;
		}

		return true;
	}

	static void UninstallHook(void* targetFunction, unsigned char* originalBytes, size_t length, size_t offset) {
		DWORD oldProtect;
		unsigned char* targetAddress = (unsigned char*)targetFunction + offset;

		if (!VirtualProtect(targetAddress, length, PAGE_EXECUTE_READWRITE, &oldProtect)) {
			std::cerr << "Failed to change memory protection." << std::endl;
			return;
		}

		// Restore original bytes
		memcpy(targetAddress, originalBytes, length);

		if (!VirtualProtect(targetAddress, length, oldProtect, &oldProtect)) {
			std::cerr << "Failed to restore memory protection." << std::endl;
		}
	}
};

