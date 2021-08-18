#include <Windows.h>

#include <fstream>

#include "patches.h"

#include <meow_hook/detour.h>
#include <meow_hook/memory.h>
#include <meow_hook/util.h>

#define VK_Z 0x5A

namespace f76
{
uintptr_t GetAddress(uint32_t address)
{
  static auto base = GetModuleHandle(nullptr);
  return (uintptr_t)((char*)base + address);
}

#pragma pack(push, 1)
struct Input {
public:
  char _pad[0x8];
  struct {
    char _pad[0x80];
    bool m_old_state[256];
    bool m_state[256];
  } * m_keyboard;

public:
  static Input& instance()
  {
    return **(Input**)GetAddress(0x55D9E80);
  }
};
#pragma pack(pop)

LRESULT WndProc(HWND, UINT, WPARAM, LPARAM);
int64_t UpdateKeys(uintptr_t a1, float a2);

static decltype(WndProc)*    pfn_WndProc    = nullptr;
static decltype(UpdateKeys)* pfn_UpdateKeys = nullptr;

static bool g_do_keypress = false;

LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
  switch (msg) {
    case WM_KEYDOWN: {
      const auto vsc = MapVirtualKeyEx(static_cast<int32_t>(wparam), MAPVK_VK_TO_VSC, GetKeyboardLayout(0));
      if ((vsc == 62 || wparam == VK_F4) && (lparam >> 30) == 0) {
        g_do_keypress = !g_do_keypress;
        return true;
      }
    }

    case WM_MOUSEMOVE: {
      if (g_do_keypress) {
        return true;
      }

      break;
    }
  }

  return pfn_WndProc(hwnd, msg, wparam, lparam);
}

void press_key(uint32_t vk, bool state)
{
  Input::instance().m_keyboard->m_state[vk] = state;
}

int64_t UpdateKeys(uintptr_t a1, float dt)
{
  static float current_time = 2.5f;
  current_time += dt;

  if (g_do_keypress) {
    // press the keys every 2.5 seconds!
    if (current_time >= 2.5f) {
      press_key(VK_SPACE, true);
      press_key(VK_Z, true);

      // after holding the keys for 1 second, release!
      if (current_time >= 3.5f) {
        press_key(VK_SPACE, false);
        press_key(VK_Z, false);
        current_time = 0.0f;
      }
    }
  } else {
    current_time = 2.5f;
  }

  return pfn_UpdateKeys(a1, dt);
}

bool init()
{
  if (strcmp((const char*)GetAddress(0x4160F78), "Fallout 76") != 0) {
    return false;
  }

  pfn_WndProc    = MH_STATIC_DETOUR(GetAddress(0x1762410), WndProc);
  pfn_UpdateKeys = MH_STATIC_DETOUR(GetAddress(0x2B82E10), UpdateKeys);
  return true;
}
}; // namespace f76