#include <Windows.h>

#include "patches.h"
#include "xinput1_3.h"

static HMODULE g_original_module = nullptr;

BOOL WINAPI DllMain(HINSTANCE hModule, DWORD fdwReason, LPVOID lpvReserved)
{
  switch (fdwReason) {
    case DLL_PROCESS_ATTACH: {
      DisableThreadLibraryCalls(hModule);

      // exit if not fallout76
      if (!GetModuleHandle("Fallout76.exe")) {
        return FALSE;
      }

      char buffer[MAX_PATH] = {0};
      GetSystemDirectoryA(buffer, MAX_PATH);
      strcat_s(buffer, MAX_PATH, "\\xinput1_3.dll");

      g_original_module = LoadLibraryA(buffer);
      if (g_original_module) {
        XInputGetState_        = (XInputGetState_t)GetProcAddress(g_original_module, "XInputGetState");
        XInputSetState_        = (XInputSetState_t)GetProcAddress(g_original_module, "XInputSetState");
        XInputGetCapabilities_ = (XInputGetCapabilities_t)GetProcAddress(g_original_module, "XInputGetCapabilities");
      }

      f76::init();
      break;
    }

    case DLL_PROCESS_DETACH: {
      if (g_original_module) {
        FreeLibrary(g_original_module);
      }

      break;
    }
  }

  return TRUE;
}