#ifndef DEBUGGER_H
#define DEBUGGER_H

#include "Includes.h"

using namespace std;

void SetConsoleColour(int Colour);

class Debugger
{
private:
	// Private member variables.
	HWND Window_Handle;
	HANDLE Process_Handle;
	DWORD Process_BaseAddress;
	DWORD Client_BaseAddress;
	DWORD Engine_BaseAddress;
	DWORD Local_PlayerAddress;

public:
	// Constructors and destructors.
	Debugger();
	~Debugger();

	// Getter and setter methods (boolean values returned as true when operation is a success).
	bool SetWindow_Handle();
	bool SetProcess_Handle();
	bool SetProcess_BaseAddress();
	bool SetClient_BaseAddress();
	bool SetEngine_BaseAddress();
	bool SetLocal_PlayerAddress();

	HWND GetWindow_Handle();
	HANDLE GetProcess_Handle();
	DWORD GetProcess_BaseAddress();
	DWORD GetClient_BaseAddress();
	DWORD GetEngine_BaseAddress();
	DWORD GetLocal_PlayerAddress();

	// Helper functions.
	HWND GetHandleByWindowName(LPCSTR);
	DWORD GetModuleBaseAddress(DWORD, TCHAR*);
	HANDLE OpenProcessByID(DWORD);
	void Clear();

	// Cheat functions.
	void Bhop();
	void ESP();
	void Shoot();
	void Trigger();
	void NoFlash();

	// Read/Write functions.
	template<class T>
	T Read(DWORD dwAddress);
	template<class T>
	BOOL Write(DWORD dwAddress, T ValueToWrite);
};

// Function Template used to read data of any type from a given memory address.
template<class T>
T Debugger::Read(DWORD dwAddress)
{
	T Value;
	ReadProcessMemory(Process_Handle, (LPVOID)dwAddress, &Value, sizeof(T), NULL);
	return Value;
}

// Function Template used to write data of any type to a given memory address.
template<class T>
BOOL Debugger::Write(DWORD dwAddress, T ValueToWrite)
{
	return WriteProcessMemory(Process_Handle, (LPVOID)dwAddress, &ValueToWrite, sizeof(T), NULL);
}

#endif