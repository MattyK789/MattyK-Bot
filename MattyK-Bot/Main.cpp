#include "Debugger.h"
#include "Offsets.h"

#define underline_start "\033[4m"
#define underline_stop "\033[0m"

using namespace std;

Debugger Debug = Debugger();

bool ESP_Enabled = false;
bool Bhop_Enabled = false;
bool Trigger_Enabled = false;
bool NoFlash_Enabled = false;

void UpdateScreen()
{
	Debug.Clear();

	SetConsoleColour(10);
	cout << R"(
   __  ______ ______________  ____ __  ___  ____  ______
  /  |/  / _ /_  __/_  __/\ \/ / //_/ / _ )/ __ \/_  __/
 / /|_/ / __ |/ /   / /    \  / ,<   / _  / /_/ / / /   
/_/  /_/_/ |_/_/   /_/     /_/_/|_| /____/\____/ /_/    
)" << endl << endl;

	cout << "Coded By: MattyK" << endl;
	cout << "UC Profile: https://www.unknowncheats.me/forum/members/1444635.html" << endl << endl;

	cout << underline_start << "Cheat Functions:" << underline_stop << endl << endl;

	SetConsoleColour((ESP_Enabled) ? 10 : 12);
	cout << ((ESP_Enabled) ? "[F1] ESP: \t\tEnabled" : "[F1] ESP: \t\tDisabled") << endl;

	SetConsoleColour((Bhop_Enabled) ? 10 : 12);
	cout << ((Bhop_Enabled) ? "[F2] Bhop: \t\tEnabled" : "[F2] Bhop: \t\tDisabled") << endl;

	SetConsoleColour((Trigger_Enabled) ? 10 : 12);
	cout << ((Trigger_Enabled) ? "[F3] Trigger: \t\tEnabled" : "[F3] Trigger: \t\tDisabled") << endl;

	SetConsoleColour((NoFlash_Enabled) ? 10 : 12);
	cout << ((Trigger_Enabled) ? "[F4] No flash: \t\tEnabled" : "[F4] No flash: \t\tDisabled") << endl;
}

int main()
{
	UpdateScreen();

	while (true)
	{
		if (GetAsyncKeyState(VK_F1) & 1)
		{
			ESP_Enabled = !ESP_Enabled;
			UpdateScreen();
		}

		if (GetAsyncKeyState(VK_F2) & 1)
		{
			Bhop_Enabled = !Bhop_Enabled;
			UpdateScreen();
		}

		if (GetAsyncKeyState(VK_F3) & 1)
		{
			Trigger_Enabled = !Trigger_Enabled;
			UpdateScreen();
		}

		if (GetAsyncKeyState(VK_F4) & 1)
		{
			NoFlash_Enabled = !NoFlash_Enabled;
			UpdateScreen();
		}

		if (ESP_Enabled)
			Debug.ESP();

		if (Bhop_Enabled)
			Debug.Bhop();

		if (Trigger_Enabled)
			Debug.Trigger();

		if (NoFlash_Enabled)
			Debug.NoFlash();

		Sleep(1);
	}
}
