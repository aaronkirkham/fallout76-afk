#pragma once

#include <cstdint>

namespace
{
using XInputGetState_t        = uint32_t (*)(uint32_t, uintptr_t);
using XInputSetState_t        = uint32_t (*)(uint32_t, uintptr_t);
using XInputGetCapabilities_t = uint32_t (*)(uint32_t, uint32_t, uintptr_t);

static XInputGetState_t        XInputGetState_;
static XInputSetState_t        XInputSetState_;
static XInputGetCapabilities_t XInputGetCapabilities_;

__declspec(dllexport) uint32_t XInputGetState(uint32_t dwUserIndex, uintptr_t pVibration)
{
  return XInputGetState_(dwUserIndex, pVibration);
}

__declspec(dllexport) uint32_t XInputSetState(uint32_t dwUserIndex, uintptr_t pVibration)
{
  return XInputSetState_(dwUserIndex, pVibration);
}

__declspec(dllexport) uint32_t XInputGetCapabilities(uint32_t dwUserIndex, uint32_t dwFlags, uintptr_t pCapabilities)
{
  return XInputGetCapabilities_(dwUserIndex, dwFlags, pCapabilities);
}
}; // namespace
