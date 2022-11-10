#pragma once

#include "Snow/Render/Texture.h"

namespace Snow {
    using namespace Render;
    class Environment : public Asset {
    public:
        Ref<Render::TextureCube> RadianceMap;
        Ref<Render::TextureCube> IrradianceMap;

        Environment() = default;
        Environment(const Ref<TextureCube>& radianceMap, const Ref<TextureCube>& irradianceMap) :
            RadianceMap(radianceMap), IrradianceMap(irradianceMap) {}

        static AssetType GetStaticType() { return AssetType::EnvMap; }
        virtual AssetType GetAssetType() const override { return GetStaticType(); }
    };
}