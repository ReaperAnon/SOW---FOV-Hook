#ifndef FRAMEWORK
	#define FRAMEWORK

	#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
	// Windows Header Files
	#include <windows.h>
	#include <mmsystem.h>

	#include <stdio.h>
	#include <stdlib.h>
	#include <psapi.h>
	#include <unknwn.h>
	#include <TlHelp32.h>
	#include <vector>
	#include <string>
	#include <fstream>
	#include <process.h>
	
	#include <MinHook.h>

	#define EXPORT __declspec(dllexport)
	#define DLLHOOK extern "C" void __declspec(dllexport)
	#define FUNCWRAPPER void __declspec(dllexport) __declspec(naked)
	#define INSTRUCTIONSET void __declspec(naked)

	#include <aobscanner.h>
	#include <hooks.h>
	#include <dllmain.h>
#endif