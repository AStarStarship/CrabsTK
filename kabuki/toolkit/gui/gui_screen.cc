/*
    src/screen.cpp -- Top-level widget and interface between NanoGUI and GLFW

    A significant redesign of this code was contributed by Christian Schueller.

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#include "t_screen.h"

#if defined(_WIN32)
#  define NOMINMAX
#  undef APIENTRY

#  define WIN32_LEAN_AND_MEAN
#  include <windows.h"

#  define GLFW_EXPOSE_NATIVE_WGL
#  define GLFW_EXPOSE_NATIVE_WIN32
#  include <GLFW/glfw3native.h"
#endif

/* Allow enforcing the GL2 implementation of NanoVG */
#define NANOVG_GL3_IMPLEMENTATION
#include <nanovg_gl.h"

namespace _ {

std::map<GLFWwindow *, Screen *> __nanogui_screens;

#if defined(NANOGUI_GLAD)
static bool gladInitialized = false;
#endif

/* Calculate pixel ratio for hi-dpi devices. */
static float get_pixel_ratio(GLFWwindow *window) {
#if defined(_WIN32)
    HWND hWnd = glfwGetWin32Window(window);
    HMONITOR monitor = nullptr;
    #if defined(MONITOR_DEFAULTTONEAREST)
        monitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
    #else
        static HMONITOR (WINAPI *MonitorFromWindow_)(HWND, DWORD) = nullptr;
        static bool MonitorFromWindow_tried = false;
        if (!MonitorFromWindow_tried) {
            auto user32 = LoadLibrary(TEXT("user32"));
            if (user32)
                MonitorFromWindow_ = (decltype(MonitorFromWindow_)) GetProcAddress(user32, "MonitorFromWindow");
            MonitorFromWindow_tried = true;
        }
        if (MonitorFromWindow_)
            monitor = MonitorFromWindow_(hWnd, 2);
    #endif  // defined(MONITOR_DEFAULTTONEAREST)
    /* The following function only exists on Windows 8.1+, but we don't want to make that a dependency */
    static HRESULT (WINAPI *GetDpiForMonitor_)(HMONITOR, UINT, UINT*, UINT*) = nullptr;
    static bool GetDpiForMonitor_tried = false;

    if (!GetDpiForMonitor_tried) {
        auto shcore = LoadLibrary(TEXT("shcore"));
        if (shcore)
            GetDpiForMonitor_ = (decltype(GetDpiForMonitor_)) GetProcAddress(shcore, "GetDpiForMonitor");
        GetDpiForMonitor_tried = true;
    }

    if (GetDpiForMonitor_ && monitor) {
        uint32_t dpiX, dpiY;
        if (GetDpiForMonitor_(monitor, 0 /* effective DPI */, &dpiX, &dpiY) == S_OK)
            return dpiX / 96.0;
    }
    return 1.f;
#elif defined(__linux__)
    (void) window;

    float ratio = 1.0f;
    FILE *fp;
    /* Try to read the pixel ratio from KDEs config */
    auto currentDesktop = std::getenv("XDG_CURRENT_DESKTOP");
    if (currentDesktop && currentDesktop == std::string("KDE")) {
        fp = popen("kreadconfig5 --group KScreen --key ScaleFactor", "r");
        if (!fp)
            return 1;

        if (fscanf(fp, "%f", &ratio) != 1)
            return 1;
    } else {
        /* Try to read the pixel ratio from GTK */
        fp = popen("gsettings get org.gnome.desktop.interface scaling-factor", "r");
        if (!fp)
            return 1;

        int ratioInt = 1;
        if (fscanf(fp, "uint32 %i", &ratioInt) != 1)
            return 1;
        ratio = ratioInt;
    }
    if (pclose(fp) != 0)
        return 1;
    return ratio >= 1 ? ratio : 1;

#else
    Vector2i fbSize, size;
    glfwGetFramebufferSize(window, &fbSize[0], &fbSize[1]);
    glfwGetWindowSize(window, &size[0], &size[1]);
    return (float)fbSize[0] / (float)size[0];
#endif
}
}  // namespace _
