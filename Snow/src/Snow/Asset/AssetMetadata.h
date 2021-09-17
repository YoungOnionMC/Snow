#pragma once

#include "Snow/Asset/Asset.h"

namespace Snow {
	struct AssetMetadata {
		AssetHandle Handle = 0;
		AssetType Type;

		std::filesystem::path FilePath;
		bool IsDataLoaded = false;

		bool IsValid() const { return Handle != 0; }
	};
}