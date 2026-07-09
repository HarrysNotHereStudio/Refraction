#pragma once

#if defined(_WIN32)
#include <EditorPlatform/Windows/Dialogs.h>
#elif defined(__linux__)
#include <EditorPlatform/Linux/Dialogs.h>
#else
#error "Unsupported platform for Dialogs"
#endif