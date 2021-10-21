#include <framework.h>

int keyParams[2];
bool keyFlags[2] = { 0, 0 };
void ReadConfig()
{
	WCHAR inputBuffer[50];
	WCHAR configName[] = L".\\plugins\\fovtweak.ini";

	GetPrivateProfileString(L"General", L"IncreaseKey", L"0x0066", inputBuffer, 50, configName);
	keyParams[0] = std::stoi(inputBuffer, nullptr, 16);

	GetPrivateProfileString(L"General", L"DecreaseKey", L"0x0064", inputBuffer, 50, configName);
	keyParams[1] = std::stoi(inputBuffer, nullptr, 16);
}

bool isAttached = true;
bool wasKeyPressed = false;
void ThreadMain(void *arg)
{
	ReadConfig();
	while(isAttached)
	{
		Sleep(50);
		if(GetKeyState(keyParams[0]) & 0x8000)
		{
			if(!keyFlags[0])
			{
				keyFlags[0] = true;
				GameHook::IncreaseFOV();
			}
		}
		else keyFlags[0] = false;

		if(GetKeyState(keyParams[1]) & 0x8000)
		{
			if(!keyFlags[1])
			{
				keyFlags[1] = true;
				GameHook::DecreaseFOV();
			}
		}
		else keyFlags[1] = false;
	}
}

DLLHOOK OnLoad()
{
	MH_Initialize();
	GameHook::CreateGameHooks();
	_beginthread(ThreadMain, 0, nullptr);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD callReason, LPVOID lpReserved)
{
	if(callReason == DLL_PROCESS_ATTACH)
		DisableThreadLibraryCalls(hModule);
	else if(callReason == DLL_PROCESS_DETACH)
		MH_Uninitialize(), isAttached = false;

    return TRUE;
}