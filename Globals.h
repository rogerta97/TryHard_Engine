#pragma once

// Warning disabled ---
#pragma warning( disable : 4577 ) // Warning that exceptions are disabled
#pragma warning( disable : 4530 )

#include <windows.h>
#include <stdio.h>

#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);

#define CONSOLE_LOG(format, ...) console_log(__FILE__, __LINE__, format, __VA_ARGS__);
#define CONSOLE_DEBUG(format, ...) console_debug(__FILE__, __LINE__, format, __VA_ARGS__);
#define CONSOLE_ERROR(format, ...) console_error(__FILE__, __LINE__, format, __VA_ARGS__);

void log(const char file[], int line, const char* format, ...);

void console_log(const char file[], int line, const char* format, ...);
void console_debug(const char file[], int line, const char* format, ...);
void console_error(const char file[], int line, const char* format, ...);

#define CAP(n) ((n <= 0.0f) ? n=0.0f : (n >= 1.0f) ? n=1.0f : n=n)
#define MIN(a,b) ((a)<(b)) ? (a) : (b)
#define MAX(a,b) ((a)>(b)) ? (a) : (b)

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#define PI 3.14159265358979323846f
#define TWO_PI 6.28318530717958647692f
#define HALF_PI 1.57079632679489661923f
#define DEFAULT_GEOMETRY_COLOR 0.7f, 0.7f, 1.0f
#define DEFAULT_WIREFRAME_COLOR 0.0f, 0.0f, 1.0f

typedef unsigned int uint;

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

// ImGui Defines --------
#define SEPARATE_WITH_SPACE ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();

// Configuration -----------
#define WIN_FULLSCREEN false
#define WIN_RESIZABLE true
#define WIN_BORDERLESS false
#define WIN_FULLSCREEN_DESKTOP false
#define VSYNC true
#define TITLE "Tryhard Engine"