#pragma once

namespace Snow {
	namespace Utils::String {
		std::string GetFilename(const std::string& filepath);
		std::string GetExtension(const std::string& filepath);
		std::string RemoveExtension(const std::string& filepath);
		bool StartsWith(const std::string& string, const std::string& start);
		std::vector<std::string> SplitString(const std::string& string, const std::string& delimiters);
		std::vector<std::string> SplitString(const std::string& string, const char delimiter);
		std::string ToLower(const std::string& string);
		std::string BytesToString(uint64_t bytes);

		template<class...Durations, class DurationIn>
		std::tuple<Durations...> BreakDownDuration(DurationIn d) {
			std::tuple<Durations...> retval;
			using discard = int[];
			(void)discard {
				0, (void((
					(std::get<Durations>(retval) = std::chrono::duration_cast<Durations>(d)),
					(d -= std::chrono::duration_cast<DurationIn>(std::get<Durations>(retval)))
					)), 0)...
			};
			return retval;
		}

		std::string DurationToString(std::chrono::duration<double> duration);
	}
}