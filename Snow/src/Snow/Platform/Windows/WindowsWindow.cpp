#include <spch.h>
#include "Snow/Core/Window.h"

#if defined(SNOW_WINDOW_WIN32)

    #include <windows.h>
    #include <windowsx.h>
    
    EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#elif defined(SNOW_WINDOW_GLFW)
    #include <GLFW/glfw3.h>
#endif

namespace Snow {
    namespace Core {

#if defined(SNOW_WINDOW_WIN32)
        HWND Win32WindowHandle;
        HINSTANCE HInstance;

        LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

        extern void KeyCallback(KeyCode key, int flags, UINT message);
        extern void MouseButtonCallback(MouseCode button, bool pressed);
        extern void MouseMoveCallback(int xPos, int yPos);
        extern void MouseScrollCallback(double xOffset, double yOffset);

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

        void WindowCloseCallback() {
            Event::WindowCloseEvent event;
            Event::EventSystem::AddEvent(event);
        }

        void WindowMinimizedCallback() {
            Event::WindowMinimizedEvent event;
            Event::EventSystem::AddEvent(event);
        }

        void WindowMaximizedCallback() {
            SNOW_CORE_TRACE("Window maximized");
        }

        void WindowMovedCallback(int xPos, int yPos) {
            Event::WindowMovedEvent event(xPos, yPos);
            Event::EventSystem::AddEvent(event);
        }

        void WindowResizeCallback(int width, int height) {
            Event::WindowResizeEvent event(width, height);
            Event::EventSystem::AddEvent(event);
        }

        void WindowFocusCallback() {

        }
#elif defined(SNOW_WINDOW_GLFW)
        GLFWwindow* GLFWWindowHandle;
        int GLFWResult;

        void WindowCloseCallback(GLFWwindow* window) {
            Event::WindowCloseEvent event;
            Event::EventSystem::AddEvent(event);
        }

        void WindowMinimizeCallback(GLFWwindow* window, int restored) {
            Event::WindowMinimizedEvent event;
            Event::EventSystem::AddEvent(event);

        }

        void WindowMaximizedCallback(GLFWwindow* window, int restored) {
            SNOW_CORE_TRACE("Window Maximized");
        }

        void WindowMovedCallback(GLFWwindow* window, int xPos, int yPos) {
            Event::WindowMovedEvent event(xPos, yPos);
            Event::EventSystem::AddEvent(event);
        }

        void WindowResizeCallback(GLFWwindow* window, int width, int height) {
            Event::WindowResizeEvent event(width, height);
            Event::EventSystem::AddEvent(event);
        }

        void WindowFocusCallback(GLFWwindow* window, int focus) {
            //SNOW_CORE_TRACE("Lost Focus? {0}", focus == GLFW_FALSE);
        }
#endif

        

        bool Window::PlatformInit() {
            SNOW_CORE_TRACE("Creating Windows window");
#if defined(SNOW_WINDOW_WIN32)
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

            Win32WindowHandle = CreateWindowExA(WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
                winclass.lpszClassName, "Test Application",
                WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_MAXIMIZEBOX | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_THICKFRAME | WS_POPUP,
                0, 0, 1080, 720, NULL, NULL, HInstance, NULL);

            if (Win32WindowHandle == NULL)
                SNOW_CORE_ERROR("Failed to create window instance");

            auto HDC = GetDC(Win32WindowHandle);

            auto pfd = GetPixelFormat();
            int pixelformat = ChoosePixelFormat(HDC, &pfd);
            if (pixelformat) {
                if (!SetPixelFormat(HDC, pixelformat, &pfd))
                    SNOW_CORE_ERROR("Could not set pixel format");
            }
            else
                SNOW_CORE_ERROR("You Goofed up man");

            ShowWindow(Win32WindowHandle, SW_SHOW);
            SetFocus(Win32WindowHandle);
#elif defined(SNOW_WINDOW_GLFW)
            GLFWResult = glfwInit();
            if (!GLFWResult)
                SNOW_CORE_ERROR("GLFW initilization failed");
            else {
                SNOW_CORE_INFO("GLFW initialized");
            }

            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
            GLFWWindowHandle = glfwCreateWindow(1080, 720, "Test Window", nullptr, nullptr);
            SNOW_CORE_INFO("Using GLFW window");

            glfwSetWindowCloseCallback(GLFWWindowHandle, WindowCloseCallback);
            glfwSetWindowIconifyCallback(GLFWWindowHandle, WindowMinimizeCallback);
            glfwSetWindowSizeCallback(GLFWWindowHandle, WindowResizeCallback);
            glfwSetWindowMaximizeCallback(GLFWWindowHandle, WindowMaximizedCallback);
            glfwSetWindowPosCallback(GLFWWindowHandle, WindowMovedCallback);
            glfwSetWindowFocusCallback(GLFWWindowHandle, WindowFocusCallback);
#endif
            return true;
        }

        bool Window::PlatformShutdown() {
#if defined(SNOW_WINDOW_WIN32)

#elif defined(SNOW_WINDOW_GLFW)
            if (GLFWWindowHandle)
                glfwDestroyWindow(GLFWWindowHandle);
#endif

            return true;
        }

        void Window::PlatformUpdate() {
#if defined(SNOW_WINDOW_WIN32)
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
#elif defined(SNOW_WINDOW_GLFW)
            glfwPollEvents();
#endif
        }

        void* Window::GetWindowHandle() {
#if defined(SNOW_WINDOW_WIN32)
            return Win32WindowHandle;
#elif defined(SNOW_WINDOW_GLFW)
            return GLFWWindowHandle;
#endif
        }

#if defined(SNOW_WINDOW_WIN32)
        LRESULT CALLBACK WindowProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam) {
            LRESULT result = NULL;
            
            switch (message) {
            case WM_ACTIVATE: {
                if (!HIWORD(wParam)) {}
                    //active
                else {}
                    //inactive
            }
            return 0;
            case WM_SYSCOMMAND: {
                switch (wParam) {
                case SC_SCREENSAVE:
                case SC_MONITORPOWER:
                    return 0;
                }
                result = DefWindowProc(windowHandle, message, wParam, lParam);
            } break;
            case WM_SETFOCUS:
                WindowFocusCallback();
                break;
            case WM_KILLFOCUS:
                WindowFocusCallback();
                break;
            case WM_CLOSE:
            case WM_DESTROY:
                WindowCloseCallback();
                DestroyWindow(windowHandle);
                PostQuitMessage(0);
                break;
            case WM_KEYDOWN:
            case WM_KEYUP:
            case WM_SYSKEYDOWN:
            case WM_SYSKEYUP:
                KeyCallback((KeyCode)wParam, lParam, message);
                break;
            case WM_LBUTTONDOWN:
            case WM_RBUTTONDOWN:
            case WM_MBUTTONDOWN:
                MouseButtonCallback((MouseCode)message, true);
                break;
            case WM_LBUTTONUP:
            case WM_RBUTTONUP:
            case WM_MBUTTONUP:
                MouseButtonCallback((MouseCode)message, false);
                break;
            case WM_MOUSEMOVE:
                MouseMoveCallback(LOWORD(lParam), HIWORD(lParam));
                break;
            case WM_MOUSEWHEEL:
                MouseScrollCallback(0, GET_WHEEL_DELTA_WPARAM(wParam)); 
                break;
            case WM_MOUSEHWHEEL:
                MouseScrollCallback(GET_WHEEL_DELTA_WPARAM(wParam), 0);
                break;
            case WM_SIZE:
                WindowResizeCallback(LOWORD(lParam), HIWORD(lParam));
                break;
            case WM_MOVE:
                WindowMovedCallback(LOWORD(lParam), HIWORD(lParam));
                break;
            default:
                result = DefWindowProc(windowHandle, message, wParam, lParam);
            }
            
            return result;
        }
#endif
    }
}