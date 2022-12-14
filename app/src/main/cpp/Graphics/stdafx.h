/* Precompiled header file for graphics library */
#pragma once

// OpenGL headers
#include "GL.hpp"

#ifdef _WIN32
// Windows Header File
#include <windows.h>
#endif

// SDL header files
#include <SDL.h>

// TODO: reference additional headers your program requires here
#include <Shared/Shared.hpp>

// Asset loading macro
#define CheckedLoad(__stmt) if(!(__stmt)){Logf("Failed to load asset [%s]", Logger::Severity::Error, #__stmt); return false; }
