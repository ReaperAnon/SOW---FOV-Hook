#ifndef SCANNER
    #define SCANNER

class AOBScanner
{
    // Data type to store individual module names and sizes that are part of the current process.
    struct Module
    {
        LPVOID moduleBase;
        uintptr_t moduleSize;
    };

    // Scans a given memory part for a given pattern.
    BYTE* ScanPattern(const SHORT* pattern, uintptr_t patternLen, BYTE* begin, uintptr_t size);

    // Determines which region of the process module to scan and passes it onto the pattern scanner.
    BYTE* ScanModuleInternal(const SHORT* pattern, uintptr_t patternLen, BYTE* begin, uintptr_t size);

    // Populates the module data for the given module name.
    Module GetModuleData(const WCHAR* moduleName);


public:
    // Returns a static address with a given offset from the module entry.
    BYTE* GetStaticAddress(uintptr_t offset, const WCHAR *moduleName);

    // Performs a pattern scan on a given process module. Returns nullptr if not found.
    BYTE* PerformModuleScan(const SHORT* pattern, uintptr_t patternLen, uintptr_t patternOffset, const WCHAR* moduleName);

    // Performs a region scan between two given addresses for a given module. Returns nullptr if not found.
    BYTE* PerformRegionScan(const SHORT* pattern, uintptr_t patternLen, uintptr_t patternOffset, uintptr_t offset, uintptr_t size, const WCHAR* moduleName);
};
#endif