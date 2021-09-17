#include <spch.h>

#include "Snow/Utils/StringUtils.h"

#include <iomanip>
#include <sstream>
#include <chrono>

namespace Snow {
	namespace Utils {
		std::string GetFilename(const std::string& filepath) {
			std::vector<std::string> parts = SplitString(filepath, "/\\");

			if (parts.size() > 0)
				return parts[parts.size() - 1];

			return "";
		}

		std::string GetExtension(const std::string& filepath) {
			std::vector<std::string> parts = SplitString(filepath, '.');

			if (parts.size() > 1)
				return parts[parts.size() - 1];

			return "";
		}

		std::string RemoveExtension(const std::string& filename) {
			return filename.substr(0, filename.find_last_of('.'));
		}

		bool StartsWith(const std::string& string, const std::string& start) {
			return string.find(start) == 0;
		}

		std::vector<std::string> SplitString(const std::string& string, const std::string& delimiters) {
			size_t start = 0;
			size_t end = string.find_first_of(delimiters);

			std::vector<std::string> result;

			while (end <= std::string::npos) {
				std::string token = string.substr(start, end - start);
				if (!token.empty())
					result.push_back(token);

				if (end == std::string::npos)
					break;

				start = end + 1;
				end = string.find_first_of(delimiters, start);
			}

			return result;
		}

		std::vector<std::string> SplitString(const std::string& string, const char delimiter) {
			return SplitString(string, std::string(1, delimiter));
		}

		std::string ToLower(const std::string& string) {
			std::string result;
			for (const auto& character : string) {
				result += std::tolower(character);
			}
			return result;
		}

		std::string BytesToString(uint64_t bytes) {
			static const double GB = 1024 * 1024 * 1024;
			static const double MB = 1024 * 1024;
			static const double KB = 1024;

			char buffer[16];

			if ((double)bytes > GB)
				sprintf_s(buffer, "%.2f GB", (double)bytes / GB);
			else if ((double)bytes > MB)
				sprintf_s(buffer, "%.2f MB", (double)bytes / MB);
			else if ((double)bytes > KB)
				sprintf_s(buffer, "%.2f KB", (double)bytes / KB);
			else
				sprintf_s(buffer, "%.2f bytes", (double)bytes);

			return std::string(buffer);
		}

		std::string DurationToString(std::chrono::duration<double> duration) {
			auto durations = BreakDownDuration<std::chrono::minutes, std::chrono::seconds, std::chrono::milliseconds>(duration);

			std::stringstream durSs;
			durSs << std::setfill('0') << std::setw(1) << std::get<0>(durations).count() << ":"
				<< std::setfill('0') << std::setw(2) << std::get<1>(durations).count() << ":"
				<< std::setfill('0') << std::setw(3) << std::get<2>(durations).count();
			return durSs.str();
		}
	}
}