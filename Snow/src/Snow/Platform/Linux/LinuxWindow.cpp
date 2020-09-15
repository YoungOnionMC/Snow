#include <spch.h>
#include "Snow/Core/Window.h"

//#define SNOW_WINDOW_XLIB

#if defined(SNOW_WINDOW_GLFW)
    #include <GLFW/glfw3.h>
    #include <GLFW/glfw3native.h>
#elif defined(SNOW_WINDOW_XLIB)
    #include <X11/Xlib.h>
#elif defined(SNOW_WINDOW_XCB)
    #include <xcb/xcb.h>
#endif

#include "Snow/Core/Application.h"

namespace Snow {
    namespace Core {

#if defined(SNOW_WINDOW_GLFW)
        GLFWwindow* GLFWWindowHandle;
        int GLFWResult;

        void WindowCloseCallback(GLFWwindow* window) {

            auto appWindow = Application::Get().GetWindow();

            Event::WindowCloseEvent event;
            Event::EventSystem::AddEvent(event);
        }

        void WindowMinimizeCallback(GLFWwindow* window, int restored) {
            SNOW_CORE_TRACE("Minimized Window");

            auto appWindow = Application::Get().GetWindow();
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
#elif defined(SNOW_WINDOW_XLIB)

    Display* MainDisplay;
    ::Window RootWindow;
    ::Window MainWindow;
    int MainScreen;
    XSetWindowAttributes MainWindowAttribs;
    XWindowAttributes WindowAttribs;
    Visual* MainVisual;
    Colormap ColorMap;
    XEvent Events;
#elif defined(SNOW_WINDOW_XCB)
    xcb_connection_t* XCBConnection;
    int screenNumber;
    xcb_screen_t* Screen;
    xcb_window_t XCBWindow;

#endif

        bool Window::PlatformInit() {
            SNOW_CORE_TRACE("Creating Linux window");
#if defined(SNOW_WINDOW_GLFW)
            GLFWResult = glfwInit();
            if(!GLFWResult)
                SNOW_CORE_ERROR("GLFW initilization failed");
            else {
                SNOW_CORE_INFO("GLFW initialized");        
            }
                

            GLFWWindowHandle = glfwCreateWindow(720, 720, "Test Window", nullptr, nullptr);
            SNOW_CORE_INFO("Using GLFW window platform");

            glfwSetWindowCloseCallback(GLFWWindowHandle, WindowCloseCallback);
            glfwSetWindowIconifyCallback(GLFWWindowHandle, WindowMinimizeCallback);
            glfwSetWindowSizeCallback(GLFWWindowHandle, WindowResizeCallback);
            glfwSetWindowMaximizeCallback(GLFWWindowHandle, WindowMaximizedCallback);
            glfwSetWindowPosCallback(GLFWWindowHandle, WindowMovedCallback);
            glfwSetWindowFocusCallback(GLFWWindowHandle, WindowFocusCallback);
           // int yes = glfwVulkanSupported();
           // SNOW_CORE_TRACE("Vulkan Supported {0}", yes);
#elif defined(SNOW_WINDOW_XLIB)

            MainDisplay = XOpenDisplay(NULL);
            if(MainDisplay == NULL)
                SNOW_CORE_TRACE("Failed to connect to X server");
            SNOW_CORE_TRACE("Connected to the X server");
            MainScreen = DefaultScreen(MainDisplay);
            RootWindow = DefaultRootWindow(MainDisplay);

            MainVisual = DefaultVisual(MainDisplay, MainScreen);

            MainWindowAttribs.colormap = XCreateColormap(MainDisplay, RootWindow, MainVisual, AllocNone);
            MainWindowAttribs.event_mask = StructureNotifyMask | KeyPressMask | KeyReleaseMask |
                    PointerMotionMask | ButtonPressMask | ButtonReleaseMask |
                    ExposureMask | FocusChangeMask | VisibilityChangeMask |
                    EnterWindowMask | LeaveWindowMask | PropertyChangeMask;
            SNOW_CORE_TRACE("Creating XWindow...");
            MainWindow = XCreateWindow(MainDisplay, RootWindow, 0, 0, 1080, 720, 0, CopyFromParent, InputOutput, MainVisual, CWBorderPixel | CWColormap | CWEventMask, &MainWindowAttribs);
            XSelectInput(MainDisplay, MainWindow, ExposureMask | KeyPressMask);
            XMapWindow(MainDisplay, MainWindow);

#elif defined(SNOW_WINDOW_XCB)
            XCBConnection = xcb_connect(NULL, &screenNumber);
            const xcb_setup_t* setup = xcb_get_setup(XCBConnection);
            xcb_screen_iterator_t iterator = xcb_setup_roots_iterator(setup);

            for(uint32_t i=0; i< screenNumber; i++) {
                xcb_screen_next(&iterator);
            }

            Screen = iterator.data;

            SNOW_CORE_TRACE("Information of screen: {0}", Screen->root);
            SNOW_CORE_TRACE("width {0}", Screen->width_in_pixels);
            SNOW_CORE_TRACE("height {0}", Screen->height_in_pixels);

            XCBWindow = xcb_generate_id(XCBConnection);
            xcb_create_window(XCBConnection, XCB_COPY_FROM_PARENT, XCBWindow, Screen->root, 0, 0, 1080, 720, 3, XCB_WINDOW_CLASS_INPUT_OUTPUT, Screen->root_visual, 0, NULL);

            xcb_map_window(XCBConnection, XCBWindow);
            xcb_flush(XCBConnection);
#endif


            return true;
        }

        bool Window::PlatformShutdown() {
#if defined(SNOW_WINDOW_GLFW)
            SNOW_CORE_INFO("Destroying GLFW window");
            if(GLFWWindowHandle)
                glfwDestroyWindow(GLFWWindowHandle);
#elif defined(SNOW_WINDOW_XLIB)

#endif
            return true;
        }

        void Window::PlatformUpdate() {
#if defined(SNOW_WINDOW_GLFW)
            glfwPollEvents();
#elif defined(SNOW_WINDOW_XLIB)
            XNextEvent(MainDisplay, &Events);
#endif
        }

        void* Window::GetWindowHandle() {
#if defined(SNOW_WINDOW_GLFW)
            return GLFWWindowHandle;
#elif defined(SNOW_WINDOW_XLIB)
            return &MainWindow;
#endif
        }
    }
}