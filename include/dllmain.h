#ifndef DLLMAIN
	#define DLLMAIN

	UINT APIENTRY MainThread(void* pHandle);

	// Event function which runs when the current process makes a call to the dll.
	BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason_for_call, LPVOID lpReserved);
#endif