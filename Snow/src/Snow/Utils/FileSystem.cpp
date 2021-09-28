#include <spch.h>
#include "Snow/Utils/FileSystem.h"


namespace Snow {
	bool FileSystem::CreateDirectory(const std::filesystem::path& directory) {
		return std::filesystem::create_directories(directory);
	}

	bool FileSystem::CreateDirectory(const std::string& directory) {
		return FileSystem::CreateDirectory(std::filesystem::path(directory));
	}

	bool FileSystem::Move(const std::filesystem::path& oldPath, const std::filesystem::path& newPath) {
		if (FileSystem::Exists(newPath))
			return false;

		std::filesystem::rename(oldPath, newPath);
		return true;
	}

	bool FileSystem::MoveFile(const std::filesystem::path& filepath, const std::filesystem::path& dstPath) {
		return FileSystem::Move(filepath, dstPath / filepath.filename());
	}

	bool FileSystem::Rename(const std::filesystem::path& oldFilepath, const std::filesystem::path& newFilepath) {
		return FileSystem::Move(oldFilepath, newFilepath);
	}

	bool FileSystem::RenameFilename(const std::filesystem::path& oldFilepath, const std::string& newFilepath) {
		std::filesystem::path newPath = fmt::format("{0}\\{1}{2}", oldFilepath.parent_path().string(), newFilepath, oldFilepath.extension());
		return Rename(oldFilepath, newPath);
	}

	bool FileSystem::Exists(const std::filesystem::path& filepath) {
		return std::filesystem::exists(filepath);
	}

	bool FileSystem::Exists(const std::string& filepath) {
		return FileSystem::Exists(std::filesystem::path(filepath));
	}

	bool FileSystem::DeleteFile(const std::filesystem::path& filepath) {
		if (!FileSystem::Exists(filepath))
			return false;

		if (std::filesystem::is_directory(filepath))
			return std::filesystem::remove_all(filepath) > 0;
		return std::filesystem::remove(filepath);
	}

	bool FileSystem::IsDirectory(const std::filesystem::path& filepath) {
		return std::filesystem::is_directory(filepath);
	}
}