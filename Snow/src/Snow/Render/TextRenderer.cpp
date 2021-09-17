#include <spch.h>

#include "Snow/Render/TextRenderer.h"

#include <ft2build.h>
#include FT_FREETYPE_H

namespace Snow {
	namespace Render {
		static FT_Library s_FreeTypeLibrary;

		void TextRenderer::Init() {
			FT_Library lib;
			auto error = FT_Init_FreeType(&s_FreeTypeLibrary);
			if (error != 0)
				SNOW_CORE_ERROR("FreeType error: {0}", FT_Error_String(error));

		}

		void TextRenderer::Shutdown() {
			FT_Done_FreeType(s_FreeTypeLibrary);
		}

		Ref<Font> TextRenderer::CreateFontFromFontFile(const std::string& filePath) {
			

			return nullptr;
		}

		FT_Library TextRenderer::GetFreeTypeLibrary() {
			return s_FreeTypeLibrary;
		}

		std::unordered_map<std::string, Ref<Font>> FontManager::s_FontTextures;

		void FontManager::Add(Ref<Font>& font) {
			auto& name = font->GetName();
			SNOW_ASSERT(s_FontTextures.find(name) == s_FontTextures.end());
			s_FontTextures[name] = font;
		}

		void FontManager::Load(const std::string& filePath) {			
			auto font = Ref<Font>::Create(filePath);
			auto& name = font->GetName();
			SNOW_ASSERT(s_FontTextures.find(name) == s_FontTextures.end());
			s_FontTextures[name] = font;
		}

		Ref<Font>& FontManager::Get(const std::string& name) {
			SNOW_ASSERT(s_FontTextures.find(name) != s_FontTextures.end());
			return s_FontTextures.at(name);
		}
	}
}