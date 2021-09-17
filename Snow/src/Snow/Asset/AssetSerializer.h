#pragma once

#include "Snow/Asset/AssetMetadata.h"

namespace Snow {
	class AssetSerializer {
	public:
		virtual bool Serialize(const AssetMetadata& metadata, const Ref<Asset>& asset) const = 0;
		virtual bool TryLoadData(const AssetMetadata& metadata, Ref<Asset>& asset) const = 0;
	};
}