#include <spch.h>
#include "Snow/Core/Window.h"

#include <windows.h>
#include <windowsx.h>

EXTERN_C IMAGE_DOS_HEADER __ImageBase;

namespace Snow {
    namespace Core {

        HWND WindowHandle;
        HINSTANCE HInstance;

        LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

        static PIXELFORMATDESCRIPTOR GetPixelFormat() {
            PIXELFORMATDESCRIPTOR result = {};
            result.nSize = sizeof(PIXELFORMATDESCRIPTOR);
            result.nVersion = 1;
            result.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
            result.iPixelType = PFD_TYPE_RGBA;
            result.cColorBits = 32;
            result.cDepthBits = 24;
            result.cStencilBits = 8;
            result.cAuxBuffers = 0;
            result.iLayerType = PFD_MAIN_PLANE;
            return result;
        }

        bool Window::PlatformInit() {

            HInstance = (HINSTANCE)&__ImageBase;

            WNDCLASSEXA winclass = {};
            winclass.hInstance = HInstance;
            winclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
            winclass.lpfnWndProc = WindowProc;
            winclass.lpszClassName = "Snow";
            winclass.cbClsExtra = 0;
            winclass.cbWndExtra = 0;
            winclass.hCursor = LoadCursor(NULL, IDC_ARROW);
            winclass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
            winclass.hIconSm = winclass.hIcon;
            winclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
            winclass.lpszMenuName = NULL;
            winclass.cbSize = sizeof(WNDCLASSEX);
            RegisterClassExA(&winclass);

            WindowHandle = CreateWindowExA(WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
                winclass.lpszClassName, "Test Application",
                WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_MAXIMIZEBOX | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_THICKFRAME | WS_POPUP,
                0, 0, 1080, 720, NULL, NULL, HInstance, NULL);

            if (WindowHandle == NULL)
                SNOW_CORE_ERROR("Failed to create window instance");

            auto HDC = GetDC(WindowHandle);

            auto pfd = GetPixelFormat();
            int pixelformat = ChoosePixelFormat(HDC, &pfd);
            if (pixelformat) {
                if (!SetPixelFormat(HDC, pixelformat, &pfd))
                    SNOW_CORE_ERROR("Could not set pixel format");
            }
            else
                SNOW_CORE_ERROR("You Goofed up man");

            ShowWindow(WindowHandle, SW_SHOW);
            SetFocus(WindowHandle);
#define SNOW_USE_WIN32
            return true;
        }

        bool Window::PlatformShutdown() {
            return true;
        }

        void Window::PlatformUpdate() {
            MSG message;
            while (PeekMessage(&message, NULL, NULL, NULL, PM_REMOVE) > 0) {
                if (message.message == WM_QUIT) {
                    Core::Event::WindowCloseEvent event;
                    Core::Event::EventSystem::AddEvent(event);
                    return;
                }
                TranslateMessage(&message);
                DispatchMessage(&message);
            }
        }

        void* Window::GetWindowHandle() {
            return WindowHandle;
        }

        LRESULT CALLBACK WindowProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam) {
            return DefWindowProc(windowHandle, message, wParam, lParam);
        }

    }
}