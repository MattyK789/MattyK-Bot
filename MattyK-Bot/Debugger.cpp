#ifndef DEBUGGER_CPP
#define DEBUGGER_CPP

#include "Offsets.h"
#include "Debugger.h"

void SetConsoleColour(int Colour)
{
	// Green = 10 | Red = 12 | White  = 15
	HANDLE  hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, Colour);
}


// Constructor for the debugger.
Debugger::Debugger()
{
	SetConsoleTitle("MattyK Bot");

	Process_BaseAddress = NULL;
	Client_BaseAddress = NULL;
	Engine_BaseAddress = NULL;
	Local_PlayerAddress = NULL;

	SetConsoleColour(10);

	// Getting window handle.
	if (SetWindow_Handle())
	{
		cout << "Acquired window - " << hex << Window_Handle << endl;
		Sleep(300);
	}
	else
	{
		SetConsoleColour(12);
		cout << "Could not get window handle!" << endl;
		Sleep(3000);
		exit(0);
	}

	// Getting process ID (base address).
	if (SetProcess_BaseAddress())
	{
		cout << "Acquired process address - " << hex << Process_BaseAddress << endl;
		Sleep(300);
	}
	else
	{
		SetConsoleColour(12);
		cout << "Could not get process address!" << endl;
		Sleep(3000);
		exit(0);
	}

	// Getting process handle.
	if (SetProcess_Handle())
	{
		cout << "Acquired process handle - " << hex << Process_Handle << endl;
		Sleep(300);
	}
	else
	{
		SetConsoleColour(12);
		cout << "Could not get process handle!" << endl;
		Sleep(3000);
		exit(0);
	}

	// Getting Client.dll and Engine.dll base addresses.
	if (SetEngine_BaseAddress())
	{
		cout << "Acquired \"engine.dll\" base address - " << hex << Engine_BaseAddress << endl;
		Sleep(300);
	}
	else
	{
		SetConsoleColour(12);
		cout << "Could not get \"engine.dll\" base address!" << endl;
		Sleep(3000);
		exit(0);
	}

	if (SetClient_BaseAddress())
	{
		cout << "Acquired \"client_panorama.dll\" base address - " << hex << Client_BaseAddress << endl;
		Sleep(300);
	}
	else
	{
		SetConsoleColour(12);
		cout << "Could not get \"client_panorama.dll\"  base address!" << endl;
		Sleep(3000);
		exit(0);
	}

	// Getting local player address.
	if (SetLocal_PlayerAddress())
	{
		cout << "Acquired local player address - " << hex << Local_PlayerAddress << endl;
		Sleep(300);
	}
	else
	{
		SetConsoleColour(12);
		cout << "Could not get local player address!" << endl;
		Sleep(3000);
		exit(0);
	}

	cout << endl;
	cout << "Starting program..." << endl;
	Sleep(3000);
	Clear();
}

// Destructor for the debugger.
Debugger::~Debugger()
{

}

// Function that returns a handle to a window by using the window's name.
HWND Debugger::GetHandleByWindowName(LPCSTR WindowName)
{
	HWND Handle = NULL;															// Where our handle will be stored.

	Handle = FindWindowA(NULL, WindowName);										// Function to get handle by window name.

	if (Handle == NULL)															// Tests to see if window was found.
		return NULL;															// Returns NULL because the window was not found.
	else
		return Handle;															// Returns the handle variable as the window was found.
}

// Function that returns a handle to the opened process.
HANDLE Debugger::OpenProcessByID(DWORD ID)
{
	HANDLE Process_Handle = OpenProcess(PROCESS_ALL_ACCESS, false, ID);			// Alternative access "PROCESS_ALL_ACCESS".

	if (Process_Handle == NULL)													// Tests to see if process was found.
		return NULL;															// Returns NULL because the process was not found.
	else
		return Process_Handle;													// Returns the handle variable as the process was found.
}

// Function that returns the base address of a given module.
DWORD Debugger::GetModuleBaseAddress(DWORD dwProcessIdentifier, TCHAR *lpszModuleName)
{
	// Not a fucking clue how this works.
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwProcessIdentifier);
	DWORD dwModuleBaseAddress = 0;
	if (hSnapshot != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 ModuleEntry32 = { 0 };
		ModuleEntry32.dwSize = sizeof(MODULEENTRY32);
		if (Module32First(hSnapshot, &ModuleEntry32))
		{
			do
			{
				if (strcmp(ModuleEntry32.szModule, lpszModuleName) == 0)
				{
					dwModuleBaseAddress = (DWORD)ModuleEntry32.modBaseAddr;
					break;
				}
			} while (Module32Next(hSnapshot, &ModuleEntry32));
		}
		CloseHandle(hSnapshot);
	}
	return dwModuleBaseAddress;
}

// Getter and setter methods.
bool Debugger::SetWindow_Handle()
{
	Window_Handle = GetHandleByWindowName("Counter-Strike: Global Offensive");

	if (Window_Handle == NULL)
		return 0;
	else
		return 1;
}

bool Debugger::SetProcess_BaseAddress()
{
	GetWindowThreadProcessId(Window_Handle, &Process_BaseAddress);

	if (!Process_BaseAddress)
		return 0;
	else
		return 1;
}

bool Debugger::SetProcess_Handle()
{
	Process_Handle = OpenProcessByID(Process_BaseAddress);

	if (Process_Handle == NULL)
		return 0;
	else
		return 1;
}

bool Debugger::SetClient_BaseAddress()
{

	Client_BaseAddress = GetModuleBaseAddress(Process_BaseAddress, "client_panorama.dll");

	if (!Client_BaseAddress)
		return 0;
	else
		return 1;
}

bool Debugger::SetEngine_BaseAddress()
{
	Engine_BaseAddress = GetModuleBaseAddress(Process_BaseAddress, "engine.dll");

	if (!Engine_BaseAddress)
		return 0;
	else
		return 1;
}

bool Debugger::SetLocal_PlayerAddress()
{
	Local_PlayerAddress = Read<DWORD>(Client_BaseAddress + dwLocalPlayer);

	if (Local_PlayerAddress == NULL)
		return 0;
	else
		return 1;
}

HWND Debugger::GetWindow_Handle() { return Window_Handle; }
HANDLE Debugger::GetProcess_Handle() { return Process_Handle; }
DWORD Debugger::GetProcess_BaseAddress() { return Process_BaseAddress; }
DWORD Debugger::GetClient_BaseAddress() { return Client_BaseAddress; }
DWORD Debugger::GetEngine_BaseAddress() { return Engine_BaseAddress; }
DWORD Debugger::GetLocal_PlayerAddress() { return Local_PlayerAddress; }

// Other arbitrary functions.
void Debugger::Clear() { system("cls"); }

// Cheat functions.
void Debugger::Bhop(bool enabled)
{
	if (!enabled)
		return;

	if (GetAsyncKeyState(VK_SPACE))
	{
		if (Read<int>(Local_PlayerAddress + m_fFlags) == 257)
			Write<int>(Client_BaseAddress + dwForceJump, 5);

		else if (Read<int>(Local_PlayerAddress + m_fFlags) == 256)
		{
			Write<int>(Client_BaseAddress + dwForceJump, 4);
			Write<int>(Client_BaseAddress + dwForceJump, 5);
			Write<int>(Client_BaseAddress + dwForceJump, 4);
		}

		else
			Write<int>(Client_BaseAddress + dwForceJump, 4);
	}
}

void Debugger::ESP(bool enabled)
{
	if (!enabled)
		return;

	CGlow Glow;

	int GlowArray = Read<int>(Client_BaseAddress + dwGlowObjectManager);																				// Get glow array.
	int GlowCount = Read<int>(Client_BaseAddress + dwGlowObjectManager + 0x4);																			// Get number of glowable objects.
	int LocalTeam = Read<int>(Local_PlayerAddress + m_iTeamNum);																						// Get local team.

	for (int i = 0; i < GlowCount; i++)																													// Loop through entity list.
	{

		int gEntity = Read<int>(GlowArray + 0x38 * i);																									// Get entity address.

		if (gEntity == 0)																																// Skips if entity doesn't exist.
			continue;

		bool dormant = Read<int>(gEntity + m_bDormant);																									// Get dormant value.

		int enTeam = Read<int>(gEntity + m_iTeamNum);																									// Gets entity team.

		if (dormant || !enTeam)																															// If entity is dormant or has no team, skip.
			continue;

		Glow = Read<CGlow>(GlowArray + (i * 0x38));																										// Reads other values for glow struct.

		if (enTeam == LocalTeam)																														// Checks if on friendly team, rewrites glow struct.
		{
			Glow.r = 0.f;
			Glow.g = 0.f;
			Glow.b = 1.f;
			Glow.a = 1.f;
			Glow.m_bRenderWhenOccluded = true;
			Glow.m_bRenderWhenUnoccluded = false;
		}
		else																																			// Checks if on enemy team, rewrites glow struct.
		{
			Glow.r = 1.f;
			Glow.g = 0.f;
			Glow.b = 0.f;
			Glow.a = 1.f;
			Glow.m_bRenderWhenOccluded = true;
			Glow.m_bRenderWhenUnoccluded = false;
		}

		Write<CGlow>(GlowArray + (i * 0x38), Glow);																										// Writes glow struct.
	}
}

void Debugger::Trigger(bool enabled)
{
	if (!enabled)
		return;

	if (GetAsyncKeyState(VK_MENU))
	{
		//Retrieve player informations
		int LocalPlayer_inCross = Read<int>(Local_PlayerAddress + m_iCrosshairId);
		int LocalPlayer_Team = Read<int>(Local_PlayerAddress + m_iTeamNum);

		DWORD Trigger_EntityBase = Read<DWORD>(Client_BaseAddress + dwEntityList + ((LocalPlayer_inCross - 1) * 0x10));
		int Trigger_EntityTeam = Read<int>(Trigger_EntityBase + m_iTeamNum);
		bool Trigger_EntityDormant = Read<bool>(Trigger_EntityBase + m_bDormant);

		if ((LocalPlayer_inCross > 0 && LocalPlayer_inCross <= 64) && (Trigger_EntityBase != NULL) && (Trigger_EntityTeam != LocalPlayer_Team) && (!Trigger_EntityDormant))
		{
			//External way, you can customize the delays
			Sleep(10); //Delay before shooting
			mouse_event(MOUSEEVENTF_LEFTDOWN, NULL, NULL, NULL, NULL);
			Sleep(10); //Delay between shots
			mouse_event(MOUSEEVENTF_LEFTUP, NULL, NULL, NULL, NULL);
			Sleep(10); //Delay after shooting
		}
	}
}

void Debugger::NoFlash(bool enabled)
{
	if (!enabled)
		return;

	float FlashAlpha = Read<float>(Local_PlayerAddress + m_flFlashMaxAlpha);																			// Get's the flash alpha value.
	float FlashDuration = Read<float>(Local_PlayerAddress + m_flFlashDuration);																			// Get the remaing flash duration.

	if (!FlashAlpha || !FlashDuration)																													// Check if the alpha value is zero or the flash is finished.
		Write<float>(Local_PlayerAddress + m_flFlashMaxAlpha, 255.f);																					// Reset values.
	else
		Write<float>(Local_PlayerAddress + m_flFlashMaxAlpha, 0.f);																						// Write new values.
}

/*
CGlow Glow;

int glowArray = Read<int>(Client_BaseAddress + dwGlowObjectManager);
int glowCount = Read<int>(Client_BaseAddress + dwGlowObjectManager + 0x4);

for (int i = 0; i < glowCount; i++)
{

int gEntity = Read<int>(glowArray + 0x38 * i);

if (gEntity == 0)
continue;

bool dormant = Read<int>(gEntity + m_bDormant);

if (dormant)
continue;

int enTeam = Read<int>(gEntity + m_iTeamNum);

Glow = Read<CGlow>(glowArray + (i * 0x38));

Glow.r = 1.f;
Glow.g = 0.f;
Glow.b = 0.f;
Glow.a = 1.f;
Glow.m_bRenderWhenOccluded = true;
Glow.m_bRenderWhenUnoccluded = false;

Write<CGlow>(glowArray + (i * 0x38), Glow);
}
*/

#endif

