#include <spch.h>
#include "Snow/Utils/FileSystem.h"


#define UNICODE


#if defined(SNOW_WINDOW_GLFW)
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#elif defined(SNOW_WINDOW_WIN32)
#include <Windows.h>
#endif

#include <commdlg.h>
#include <Windows.h>
#include <shellapi.h>

#include "Snow/Core/Application.h"

#include "Snow/Project/Project.h"


namespace Snow {
	

	FileSystem::FileSystemChangedCallbackFn FileSystem::s_Callback;

	static bool s_Watching = false;
	static bool s_IgnoreNextChange = false;

	static HANDLE s_WatcherThread;

	void FileSystem::SetChangeCallback(const FileSystemChangedCallbackFn& callback) {
		s_Callback = callback;
	}

	void FileSystem::StartWatching() {
		s_Watching = true;
		DWORD threadID;
		s_WatcherThread = CreateThread(NULL, 0, Watch, 0, 0, &threadID);
		SNOW_CORE_ASSERT(s_WatcherThread != NULL);
	}

	void FileSystem::StopWatching() {
		if (!s_Watching) {
			SNOW_CORE_TRACE("Was not watching the filesystem");
			return;
		}

		s_Watching = false;
		DWORD result = WaitForSingleObject(s_WatcherThread, 5000);
		if (result == WAIT_TIMEOUT)
			TerminateThread(s_WatcherThread, 0);
		CloseHandle(s_WatcherThread);
		//SNOW_CORE_ASSERT(s_WatcherThread != NULL);
	}

	void FileSystem::SkipNextFileSystemChange() {
		s_IgnoreNextChange = true;
	}

	unsigned long FileSystem::Watch(void* param) {
		auto assetDirectory = Project::GetActive()->GetAssetDirectory();
#ifdef UNICODE
		std::wstring dirStr = assetDirectory.wstring();
#else
		std::string dirStr = assetDirectory.string();
#endif

		char buf[2048];
		DWORD bytesReturned;
		std::filesystem::path filepath;
		BOOL result = TRUE;

		HANDLE directoryHandle = CreateFile(dirStr.c_str(), GENERIC_READ | FILE_LIST_DIRECTORY,
			FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
			NULL,
			OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED, NULL);

		if (directoryHandle == INVALID_HANDLE_VALUE) {
			SNOW_CORE_VERIFY(false, "Failed to open directory");
			return 0;
		}

		OVERLAPPED pollingOverlap;
		pollingOverlap.OffsetHigh = 0;
		pollingOverlap.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

		//std::vector<Core::Event::FileSystemChangedEvent> eventBatch;
		//eventBatch.reserve(10);

		while (s_Watching && result) {
			result = ReadDirectoryChangesW(directoryHandle, &buf, sizeof(buf), TRUE,
				FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME | FILE_NOTIFY_CHANGE_SIZE, 
				&bytesReturned, &pollingOverlap, NULL);

			WaitForSingleObject(pollingOverlap.hEvent, INFINITE);

			if (s_IgnoreNextChange) {
				s_IgnoreNextChange = false;
				continue;
			}

			FILE_NOTIFY_INFORMATION* notify;
			int offset = 0;
			std::wstring oldName;

			do {
				notify = (FILE_NOTIFY_INFORMATION*)((char*)buf + offset);
				size_t filenameLen = notify->FileNameLength / sizeof(wchar_t);

				//Core::Event::FileSystemChangedEvent e;
				std::string name;
				wcstombs((char*)name.c_str(), notify->FileName, filenameLen);
				auto path = std::filesystem::path(std::wstring(notify->FileName, filenameLen));

				switch (notify->Action) {
				case FILE_ACTION_ADDED: {
					Core::Event::FileAddedEvent e(name, path, IsDirectory(path));
					s_Callback(e);
					break;
				}
				case FILE_ACTION_REMOVED: {
					Core::Event::FileDeletedEvent e(name, path, IsDirectory(path));
					s_Callback(e);
					break;
				}
				case FILE_ACTION_MODIFIED: {
					Core::Event::FileModifiedEvent e(name, path, IsDirectory(path));
					s_Callback(e);
					break;
				}
				case FILE_ACTION_RENAMED_OLD_NAME: {
					oldName = path.filename();
					break;
				}
				case FILE_ACTION_RENAMED_NEW_NAME: {
					std::string oName = std::string(oldName.begin(), oldName.end());
					Core::Event::FileRenamedEvent e(name, path, oName, IsDirectory(path));
					s_Callback(e);
					break;
				}
				}

				offset += notify->NextEntryOffset;
				
			} while (notify->NextEntryOffset);
		}

		CloseHandle(directoryHandle);
		return 0;
	}

	bool FileSystem::WriteBytes(const std::filesystem::path& path, const Buffer& buffer) {
		std::ofstream stream(path, std::ios::binary | std::ios::trunc);

		if (!stream) {
			stream.close();
			return false;
		}
		stream.write((char*)buffer.Data, buffer.Size);
		stream.close();
		return true;
	}

	Buffer FileSystem::ReadBytes(const std::filesystem::path& path) {
		Buffer buffer;

		std::ifstream stream(path, std::ios::binary | std::ios::ate);
		SNOW_CORE_ASSERT(stream);

		std::streampos end = stream.tellg();
		stream.seekg(0, std::ios::beg);
		uint32_t size = end - stream.tellg();
		SNOW_CORE_ASSERT(size != 0);

		buffer.Allocate(size);
		stream.read((char*)buffer.Data, buffer.Size);

		return buffer;
	}

	bool FileSystem::ShowFileInExplorer(const std::filesystem::path& filepath) {
		auto absolutePath = std::filesystem::canonical(filepath);
		if (!FileSystem::Exists(absolutePath))
			return false;

		std::string cmd = fmt::format("explorer.exe /select,\"{0}\"", absolutePath.string());
		system(cmd.c_str());
		return true;
	}

	bool FileSystem::OpenDirectoryInExplorer(const std::filesystem::path& filepath) {
		auto absolutePath = std::filesystem::canonical(filepath);
		if (!FileSystem::Exists(absolutePath))
			return false;

		ShellExecuteW(NULL, (LPCWSTR)L"explore", (LPCWSTR)absolutePath.c_str(), NULL, NULL, SW_SHOWNORMAL);
		return true;
	}

	bool FileSystem::OpenExternally(const std::filesystem::path& path) {
		auto absolutePath = std::filesystem::canonical(path);
		if (!FileSystem::Exists(absolutePath))
			return false;
#ifdef UNICODE
		ShellExecute(NULL, (LPCWSTR)L"open", (LPCWSTR)absolutePath.c_str(), NULL, NULL, SW_SHOWNORMAL);
#else
		ShellExecute(NULL, (LPCSTR)L"open", (LPCSTR)absolutePath.c_str(), NULL, NULL, SW_SHOWNORMAL);
#endif
		return true;
	}
}