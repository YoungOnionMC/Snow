#pragma once

#include "Snow/Render/Texture.h"
#include "Snow/Render/Font.h"

namespace Snow {
	namespace Render {
		class TextRenderer {
		public:
			static void Init();
			static void Shutdown();

			static FT_Library GetFreeTypeLibrary();

			static Ref<Font> CreateFontFromFontFile(const std::string& filepath);

		private:
		

		};

		class FontManager : public RefCounted {
		public:
			void Add(Ref<Font>& fontTexture);

			Ref<Font>& Get(const std::string& name);
			void Load(const std::string& filePath);
		private:
			static std::unordered_map<std::string, Ref<Font>> s_FontTextures;
		};
	}
}