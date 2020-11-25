#include <spch.h>

#include "Snow/Utils/FileDialogs.h"

#include <commdlg.h>
#include <Windows.h>

#if defined(SNOW_WINDOW_GLFW)
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#elif defined(SNOW_WINDOW_WIN32)
#include <Windows.h>
#endif

#include "Snow/Core/Application.h"

namespace Snow {
	namespace Utils {
		std::optional<std::string> FileDialogs::OpenFile(const char* filter) {
			OPENFILENAMEA ofn;
			CHAR szFile[260] = { 0 };
			ZeroMemory(&ofn, sizeof(OPENFILENAME));
			ofn.lStructSize = sizeof(OPENFILENAME);
#if defined(SNOW_WINDOW_GLFW)
			ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Core::Application::Get().GetWindow()->GetWindowHandle());
#elif defined(SNOW_WINDOW_WIN32)
			ofn.hwndOwner = (HWND)Core::Application::Get().GetWindow()->GetWindowHandle();
#endif
			ofn.lpstrFile = szFile;
			ofn.nMaxFile = sizeof(szFile);
			ofn.lpstrFilter = filter;
			ofn.nFilterIndex = 1;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

			if (GetOpenFileNameA(&ofn) == TRUE)
				return ofn.lpstrFile;
			return std::nullopt;
		}

		std::optional<std::string> FileDialogs::SaveFile(const char* filter) {
			OPENFILENAMEA ofn;
			CHAR szFile[260] = { 0 };
			ZeroMemory(&ofn, sizeof(OPENFILENAME));
			ofn.lStructSize = sizeof(OPENFILENAME);
#if defined(SNOW_WINDOW_GLFW)
			ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Core::Application::Get().GetWindow()->GetWindowHandle());
#elif defined(SNOW_WINDOW_WIN32)
			ofn.hwndOwner = (HWND)Core::Application::Get().GetWindow()->GetWindowHandle();
#endif
			ofn.lpstrFile = szFile;
			ofn.nMaxFile = sizeof(szFile);
			ofn.lpstrFilter = filter;
			ofn.nFilterIndex = 1;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

			// Sets the default extension by extracting it from the filter
			ofn.lpstrDefExt = strchr(filter, '\0') + 1;

			if (GetSaveFileNameA(&ofn) == TRUE)
				return ofn.lpstrFile;
			return std::nullopt;
		}
	}
}