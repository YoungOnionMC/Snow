#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#include "Snow/Render/Texture.h"
#include "Snow/Core/Ref.h"

#include <glm/glm.hpp>

namespace Snow {
	namespace Render {

		struct GlyphData {
			FT_Vector Advance;

			unsigned char* Buffer;
			int BitmapLeft;
			int BitmapTop;
			uint32_t BitmapWidth;
			uint32_t BitmapHeight;
			uint32_t Size;
			char Character;
		};

		class Font : public RefCounted {
		public:
			Font(const std::string& filePath);

			const std::string GetName() const { return m_Name; }

			const glm::vec2 GetScale() const { return m_Scale; }

			GlyphData& GetGlyph(char character) { return m_Glyphs.at(character); }

			Ref<Texture2D> GetCharTexture(char character) { return m_CharTextures[character]; }
			FT_Face GetFontFace() { return m_FontFace; }
		private:

			void PackTexture();


			FT_Face m_FontFace;
			FT_GlyphSlot m_GlyphSlot;
			std::pair<void*, uint32_t> m_PackedBitmap;
			Ref<Texture2D> m_FontTexture;
			std::vector<Ref<Texture2D>> m_CharTextures;

			glm::vec2 m_Scale;

			uint32_t m_Width, m_Height;

			std::string m_Name;
			std::unordered_map<char, GlyphData> m_Glyphs;
		};
	}
}