#include <spch.h>

#include "Snow/Utils/FileDialogs.h"


#if defined(SNOW_WINDOW_GLFW)
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#endif

#include <commdlg.h>
#include <Windows.h>
#include <ShlObj.h>

#include "Snow/Core/Application.h"

namespace Snow {

	
	namespace Utils {
		int MBCSToWS(WCHAR*& wStr, const char* str, int len = -1, bool addTrailingZero = false) {
			SNOW_CORE_ASSERT(str);
			SNOW_CORE_ASSERT(len >= 0 || len == -1);
			int wideLen = MultiByteToWideChar(CP_ACP, 0, str, len, NULL, 0);

			if (len == -1)
				--wideLen;
			if (wideLen == 0)
				return 0;
			if (addTrailingZero) {
				wStr = new WCHAR[(wideLen + 1) * sizeof(WCHAR)];
				memset(wStr, 0, (wideLen + 1) * sizeof(WCHAR));
			}
			else {
				wStr = new WCHAR[wideLen * sizeof(WCHAR)];
				memset(wStr, 0, wideLen * sizeof(WCHAR));
			}
			MultiByteToWideChar(CP_ACP, 0, str, len, wStr, wideLen);
			return wideLen;
		}

		int WSToMBCS(char*& str, const WCHAR* wStr, int len = -1, bool addTrailingZero = false) {
			SNOW_CORE_ASSERT(wStr);
			SNOW_CORE_ASSERT(len >= 0 || len == -1);
			int charLen = WideCharToMultiByte(CP_ACP, 0, wStr, len, NULL, 0, NULL, NULL);

			if (len == -1)
				--charLen;
			if (charLen == 0)
				return 0;
			if (addTrailingZero) {
				str = new char[(charLen + 1)];
				memset(str, 0, (charLen + 1));
			}
			else {
				str = new char[charLen];
				memset(str, 0, charLen);
			}
			WideCharToMultiByte(CP_ACP, 0, wStr, len, str, charLen, NULL, NULL);
			return charLen;
		}
	}

	std::optional<std::string> Utils::FileDialogs::OpenFile(const char* filter) {
		OPENFILENAME ofn;

#ifdef UNICODE
		WCHAR szFile[260] = { 0 };
		WCHAR currentDir[256] = { 0 };
#else
		CHAR szFile[260] = { 0 };
		CHAR currentDir[256] = { 0 };
#endif

		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
#if defined(SNOW_WINDOW_GLFW)
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Core::Application::Get().GetWindow()->GetWindowHandle());
#elif defined(SNOW_WINDOW_WIN32)
		ofn.hwndOwner = (HWND)Core::Application::Get().GetWindow()->GetWindowHandle();
#endif
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		if (GetCurrentDirectory(256, currentDir))
			ofn.lpstrInitialDir = currentDir;

		//Utils::MBCSToWS((wchar_t*)ofn.lpstrFilter, filter);
			//Utils::WSToMBCS((void*)ofn.lpstrInitialDir,currentDir);
#ifdef UNICODE
		mbstowcs((wchar_t*)ofn.lpstrFilter, filter, std::strlen(filter));
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		//std::string result;
		if (GetOpenFileName(&ofn) == TRUE) {
			//SNOW_CORE_TRACE("Name of Project File: {0}", ofn.lpstrFile);
			SNOW_CORE_TRACE("Size: {0}", wcslen(ofn.lpstrFile));
			char* result;
			WSToMBCS(result, ofn.lpstrFile, wcslen(ofn.lpstrFile), true);
			return result;
		}
#else
		memcpy((char*)ofn.lpstrFilter, filter, std::strlen(filter));
		//mbstowcs((wchar_t*)ofn.lpstrFilter, filter, std::strlen(filter));
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		//std::string result;
		if (GetOpenFileName(&ofn) == TRUE) {
			//SNOW_CORE_TRACE("Name of Project File: {0}", ofn.lpstrFile);
			SNOW_CORE_TRACE("Size: {0}", strlen(ofn.lpstrFile));
			//char* result;
			//WSToMBCS(result, ofn.lpstrFile, wcslen(ofn.lpstrFile), true);
			return ofn.lpstrFile;
		}
#endif
		return std::nullopt;
	}

	std::optional<std::string> Utils::FileDialogs::OpenFolder(const char* initialFolder) {
		std::string result;
		IFileOpenDialog* dialog;
		if (SUCCEEDED(CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, (void**)&dialog))) {
			DWORD options;
			dialog->GetOptions(&options);
			dialog->SetOptions(options | FOS_PICKFOLDERS);
			if (SUCCEEDED(dialog->Show(NULL))) {
				IShellItem* selectedItem;
				if (SUCCEEDED(dialog->GetResult(&selectedItem))) {
					PWSTR pszFilePath;
					if (SUCCEEDED(selectedItem->GetDisplayName(SIGDN_DESKTOPABSOLUTEPARSING, &pszFilePath))) {
						std::filesystem::path p = pszFilePath;
						result = p.string();
						CoTaskMemFree(pszFilePath);
					}
					selectedItem->Release();
				}
			}
			dialog->Release();
		}

		return result;
	}

	std::optional<std::string> Utils::FileDialogs::SaveFile(const char* filter) {
		OPENFILENAME ofn;
#ifdef UNICODE
		WCHAR szFile[260] = { 0 };
		WCHAR currentDir[256] = { 0 };
#else
		CHAR szFile[260] = { 0 };
		CHAR currentDir[256] = { 0 };
#endif
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
#if defined(SNOW_WINDOW_GLFW)
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Core::Application::Get().GetWindow()->GetWindowHandle());
#elif defined(SNOW_WINDOW_WIN32)
		ofn.hwndOwner = (HWND)Core::Application::Get().GetWindow()->GetWindowHandle();
#endif
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		if (GetCurrentDirectory(256, currentDir))
			ofn.lpstrInitialDir = currentDir;
		//Utils::MBCSToWS(ofn.lpstrFile, filter);
		//ofn.lpstrFilter = filter;
#ifdef UNICODE
		mbstowcs((wchar_t*)ofn.lpstrFilter, filter, std::strlen(filter));
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

		// Sets the default extension by extracting it from the filter
		//ofn.lpstrDefExt = strchr(filter, '\0') + 1;
		//std::string extension = strchr(filter, '\0') + 1;
		//size_t size = extension.size();
		//ofn.lpstrDefExt =
		//MBCSToWS((WCHAR*)ofn.lpstrDefExt, extension.c_str(), size);//mbstowcs(&size, *(wchar_t*)ofn.lpstrDefExt, extension.c_str(), extension.size());
		//Utils::MBCSToWS(ofn.lpstrDefExt, extension.c_str());

		std::string result;
		if (GetSaveFileName(&ofn) == TRUE) {
			char* result;
			WSToMBCS(result, ofn.lpstrFile, wcslen(ofn.lpstrFile), true);
			return result;

			//wcstombs((char*)result.c_str(), ofn.lpstrFile, std::strlen(filter));
			//return result;
		}
#else
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

		if (GetSaveFileName(&ofn) == TRUE) {
			return ofn.lpstrFile;
		}
#endif
		return std::nullopt;
	}
	

	/*
	namespace Utils {
		std::optional<std::string> FileDialogs::OpenFile(const char* filter) {
			OPENFILENAME ofn;
			CHAR szFile[260] = { 0 };
			CHAR currentDir[256] = { 0 };
			ZeroMemory(&ofn, sizeof(OPENFILENAME));
			ofn.lStructSize = sizeof(OPENFILENAME);
#if defined(SNOW_WINDOW_GLFW)
			ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Core::Application::Get().GetWindow()->GetWindowHandle());
#elif defined(SNOW_WINDOW_WIN32)
			ofn.hwndOwner = (HWND)Core::Application::Get().GetWindow()->GetWindowHandle();
#endif
			ofn.lpstrFile = szFile;
			ofn.nMaxFile = sizeof(szFile);
			if (GetCurrentDirectoryA(256, currentDir))
				ofn.lpstrInitialDir = currentDir;
			ofn.lpstrFilter = filter;
			ofn.nFilterIndex = 1;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

			if (GetOpenFileNameA(&ofn) == TRUE)
				return ofn.lpstrFile;
			return std::nullopt;
		}

		std::optional<std::string> FileDialogs::SaveFile(const char* filter) {
			OPENFILENAME ofn;
			CHAR szFile[260] = { 0 };
			CHAR currentDir[256] = { 0 };
			ZeroMemory(&ofn, sizeof(OPENFILENAME));
			ofn.lStructSize = sizeof(OPENFILENAME);
#if defined(SNOW_WINDOW_GLFW)
			ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Core::Application::Get().GetWindow()->GetWindowHandle());
#elif defined(SNOW_WINDOW_WIN32)
			ofn.hwndOwner = (HWND)Core::Application::Get().GetWindow()->GetWindowHandle();
#endif
			ofn.lpstrFile = szFile;
			ofn.nMaxFile = sizeof(szFile);
			if (GetCurrentDirectory(256, currentDir))
				ofn.lpstrInitialDir = currentDir;
			ofn.lpstrFilter = filter;
			ofn.nFilterIndex = 1;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

			// Sets the default extension by extracting it from the filter
			ofn.lpstrDefExt = strchr(filter, '\0') + 1;

			if (GetSaveFileName(&ofn) == TRUE)
				return ofn.lpstrFile;
			return std::nullopt;
		}
	}
	*/
}