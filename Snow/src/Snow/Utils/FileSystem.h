#pragma once

#include <functional>
#include <filesystem>
#include <optional>

#include "Snow/Core/Event/Event.h"

#include "Snow/Core/Buffer.h"

namespace Snow {
	namespace Core::Event {
		class FileSystemChangedEvent : public Event {
		public:
			FileSystemChangedEvent(const std::string& filename, const std::filesystem::path& path, bool isDirectory) :
				m_FileName(filename), m_FilePath(path), m_IsDirectory(isDirectory) {}

			std::string& GetFileName() { return m_FileName; }
			const std::string& GetFileName() const { return m_FileName; }
			std::filesystem::path& GetFilePath() { return m_FilePath; }
			const std::filesystem::path& GetFilePath() const { return m_FilePath; }

			bool IsDirectory() const { return m_IsDirectory; }

			EVENT_CLASS_CATEGORY(EventCategoryFileChanged);
		protected:
			std::string m_FileName;
			std::filesystem::path m_FilePath;
			bool m_IsDirectory;
		};

		class FileAddedEvent : public FileSystemChangedEvent {
		public:
			FileAddedEvent(const std::string& filename, const std::filesystem::path& path, bool isDirectory) :
				FileSystemChangedEvent(filename, path, isDirectory) {}

			EVENT_CLASS_TYPE(FileAdded);
		};

		class FileModifiedEvent : public FileSystemChangedEvent {
		public:
			FileModifiedEvent(const std::string& filename, const std::filesystem::path& path, bool isDirectory) :
				FileSystemChangedEvent(filename, path, isDirectory) {}

			EVENT_CLASS_TYPE(FileModified);
		private:
			std::filesystem::path m_OldPath;
		};

		class FileDeletedEvent : public FileSystemChangedEvent {
		public:
			FileDeletedEvent(const std::string& filename, const std::filesystem::path& path, bool isDirectory) :
				FileSystemChangedEvent(filename, path, isDirectory) {}

			EVENT_CLASS_TYPE(FileDeleted);
		};

		class FileRenamedEvent : public FileSystemChangedEvent {
		public:
			FileRenamedEvent(const std::string& filename, const std::filesystem::path& path, const std::string& oldName, bool isDirectory) :
				FileSystemChangedEvent(filename, path, isDirectory), m_OldName(oldName) {}

			const std::string& GetOldName() const { return m_OldName; }

			EVENT_CLASS_TYPE(FileRenamed);

		private:
			std::string m_OldName;
		};
	}

	class FileSystem {
	public:
		static bool CreateDirectory(const std::filesystem::path& directory);
		static bool CreateDirectory(const std::string& directory);
		static bool Exists(const std::filesystem::path& filepath);
		static bool Exists(const std::string& filepath);
		static bool DeleteFile(const std::filesystem::path& filepath);
		static bool MoveFile(const std::filesystem::path& filepath, const std::filesystem::path& dstPath);
		static bool IsDirectory(const std::filesystem::path& filepath);

		static bool Move(const std::filesystem::path& filepath, const std::filesystem::path& dstPath);
		static bool Rename(const std::filesystem::path& oldFilepath, const std::filesystem::path& newFilePath);
		static bool RenameFilename(const std::filesystem::path& oldFilepath, const std::string& newName);

		static bool ShowFileInExplorer(const std::filesystem::path& filepath);
		static bool OpenDirectoryInExplorer(const std::filesystem::path& filepath);
		static bool OpenExternally(const std::filesystem::path& filepath);

		static bool WriteBytes(const std::filesystem::path& path, const Buffer& buffer);
		static Buffer ReadBytes(const std::filesystem::path& path);

		using FileSystemChangedCallbackFn = std::function<void(const Core::Event::FileSystemChangedEvent&)>;

		static void SetChangeCallback(const FileSystemChangedCallbackFn& callback);
		static void StartWatching();
		static void StopWatching();

		static void SkipNextFileSystemChange();
	private:
		static unsigned long Watch(void* param);

		static FileSystemChangedCallbackFn s_Callback;
	};
}