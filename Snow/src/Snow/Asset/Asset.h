#pragma once

#include "Snow/Core/UUID.h"
#include "Snow/Asset/AssetTypes.h"

namespace Snow {
	using AssetHandle = UUID;

	class Asset : public RefCounted {
	public:
		AssetHandle Handle;
		uint16_t Flags = (uint16_t)AssetFlag::None;

		virtual ~Asset() {}

		virtual AssetType GetAssetType() const { return AssetType::None; }

		bool IsValid() const { return ((Flags & (uint16_t)AssetFlag::Missing) | (Flags & (uint16_t)AssetFlag::Invalid)) == 0; }

		virtual bool operator==(const Asset& other) const {
			return Handle == other.Handle;
		}

		virtual bool operator!=(const Asset& other) const {
			return !(*this == other);
		}

		bool IsFlagSet(AssetFlag flag) const { return (uint16_t)flag & Flags; }
		void SetFlag(AssetFlag flag, bool value = true) {
			if (value)
				Flags |= (uint16_t)flag;
			else
				Flags &= ~(uint16_t)flag;
		}

	private:
		friend class ContentBrowserPanel;
	};
}