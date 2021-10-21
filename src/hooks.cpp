#include <framework.h>

namespace GameHook
{
    // Multi Skill Equip
    const SHORT bytePattern[] = { 0x0F, 0x28, 0xD8, 0x8B, 0x08, 0x89, 0x8B };

    const SHORT patternLength = 7;

    const SHORT patternOffset = 0;

    // Name of the module in which to scan for byte patterns.
    const WCHAR moduleName[] = L"ShadowOfWar.exe";

    AOBScanner pScanner;

    // Injections and scanned addresses.
    extern "C"
    {
        EXPORT BYTE* fovAddress = nullptr;
        UINT_PTR fovJmpAddress = 0;
        float oldFOV = 0.69813f;
        float minFOV, minFOVRaw;
        float maxFOV, maxFOVRaw;

        INSTRUCTIONSET FOVCode()
        {
            __asm__
            (
                // original code
                "movaps %xmm0, %xmm3\n\t"
                "mov (%rax), %ecx\n\t"

                "movss 4(%rax), %xmm0\n\t" // actual FOV value
                "movss minFOV, %xmm2\n\t" // minimum FOV value
                "movss maxFOV, %xmm1\n\t" // maximum FOV value

                "ucomiss %xmm1, %xmm0\n\t"
                "ja 2f\n\t" // don't set FOV if new value would be above max
                "ucomiss %xmm0, %xmm2\n\t"
                "ja 2f\n\t" // don't set FOV is new value would be below min

                "movss oldFOV, %xmm1\n\t"
                "ucomiss %xmm1, %xmm0\n\t"
                "ja 1f\n\t" // if FOV is higher than the old max but lower than new, set it to new max

                "subss %xmm2, %xmm0\n\t"
                "subss %xmm2, %xmm1\n\t"

                "divss %xmm1, %xmm0\n\t"

                "movss maxFOV, %xmm1\n\t"
                "subss %xmm2, %xmm1\n\t"

                "mulss %xmm1, %xmm0\n\t"
                "addss %xmm2, %xmm0\n\t"
                "jmp 2f\n\t"

                "1:\n\t"
                "movss maxFOV, %xmm1\n\t"
                "movss %xmm1, 4(%rax)\n\t"

                "2:\n\t"
                "jmp *fovJmpAddress\n\t"
            );
        }
    }

    void ReadConfig()
    {
        WCHAR inputBuffer[50];
        WCHAR configName[] = L".\\plugins\\fovtweak.ini";

        GetPrivateProfileString(L"General", L"FOVMax", L"55", inputBuffer, 50, configName);
        maxFOVRaw = std::stof(inputBuffer, nullptr); maxFOV = maxFOVRaw * 0.01745329252;

        GetPrivateProfileString(L"General", L"FOVMin", L"40", inputBuffer, 50, configName);
        minFOVRaw = std::stof(inputBuffer, nullptr); minFOV = minFOVRaw * 0.01745329252;
    }

    void CreateFOVHook()
    {
        fovAddress = pScanner.PerformModuleScan(bytePattern, patternLength, patternOffset, moduleName);
        fovJmpAddress = (UINT_PTR)fovAddress + 5;
        MH_CreateHook(fovAddress, (LPVOID)FOVCode, nullptr);
    }


    // Hook startup.
    void CreateGameHooks()
    {
        ReadConfig();
        CreateFOVHook();
        MH_EnableHook(MH_ALL_HOOKS);
    }

    void IncreaseFOV()
    {
        maxFOVRaw += 1;
        maxFOV = maxFOVRaw * 0.01745329252;
        WritePrivateProfileString(L"General", L"FOVMax", std::to_wstring(maxFOVRaw).c_str(), L".\\plugins\\fovtweak.ini");
    }

    void DecreaseFOV()
    {
        maxFOVRaw -= 1;
        maxFOV = maxFOVRaw * 0.01745329252;
        WritePrivateProfileString(L"General", L"FOVMax", std::to_wstring(maxFOVRaw).c_str(), L".\\plugins\\fovtweak.ini");
    }
}