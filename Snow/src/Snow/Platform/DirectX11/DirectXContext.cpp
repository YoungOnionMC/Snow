#include <spch.h>

#include "Snow/Platform/DirectX11/DirectXContext.h"

#if defined(SNOW_WINDOW_GLFW)
#define GLFW_EXPOSE_NATIVE_WIN32
#include <glfw/glfw3.h>
#include <glfw/glfw3native.h>
#endif

namespace Snow {
	DirectX11RenderContext::DirectX11RenderContext(const Render::ContextSpecification& spec) :
		m_Specification(spec) {

		m_Device = Ref<DirectX11Device>::Create();

		m_SwapChain.Init(m_Device);

#if defined(SNOW_WINDOW_GLFW)
		m_SwapChain.Create(1280, 720, glfwGetWin32Window((GLFWwindow*)spec.WindowHandle));
#elif defined(SNOW_WINDOW_WIN32)
		m_SwapChain.Create(1280, 720, (HWND)spec.WindowHandle);
#endif
	}
}