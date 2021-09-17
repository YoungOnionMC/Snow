#include <spch.h>

#include "Snow/Render/Font.h"

#include "Snow/Render/Renderer.h"
#include "Snow/Render/TextRenderer.h"

#define FT_DEBUG_LEVEL_ERROR
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_ERRORS_H


namespace Snow {
	namespace Render {
		static void FreeTypeError(FT_Error error) {
			if (error) {
				auto code = FT_Error_String(error);
				if(code != nullptr)
					SNOW_CORE_ERROR("FreeType error: {0}", code);
			}
		}

		Font::Font(const std::string& filePath) {
			FreeTypeError(FT_New_Face(TextRenderer::GetFreeTypeLibrary(), filePath.c_str(), 0, &m_FontFace));

			m_GlyphSlot = m_FontFace->glyph;

			size_t found = filePath.find_last_of("/\\");
			m_Name = found != std::string::npos ? filePath.substr(found + 1) : filePath;
			found = m_Name.find_first_of(".");
			m_Name = found != std::string::npos ? m_Name.substr(0, found) : m_Name;
			FreeTypeError(FT_Set_Pixel_Sizes(m_FontFace, 0, 32));

			m_Scale = { m_FontFace->size->metrics.x_ppem, m_FontFace->size->metrics.y_ppem };
			//RenderCommand::SetImagePacking(1);
			for (uint32_t i = 0; i < 128; i++) {
				GlyphData gd = { {0,0}, nullptr, 0, 0, 0, 0, 0, 0 };
				char c = (char)i;
				FT_UInt glyphIndex = FT_Get_Char_Index(m_FontFace, c);

				FreeTypeError(FT_Load_Char(m_FontFace, c, FT_LOAD_RENDER));
				FreeTypeError(FT_Load_Glyph(m_FontFace, glyphIndex, FT_LOAD_RENDER));
				FreeTypeError(FT_Render_Glyph(m_GlyphSlot, FT_RENDER_MODE_NORMAL));

				gd.Buffer = new unsigned char[m_GlyphSlot->bitmap.width * m_GlyphSlot->bitmap.rows];
				memcpy(gd.Buffer, m_GlyphSlot->bitmap.buffer, m_GlyphSlot->bitmap.width * m_GlyphSlot->bitmap.rows);

				gd.BitmapWidth = m_GlyphSlot->bitmap.width;
				gd.BitmapHeight = m_GlyphSlot->bitmap.rows;
				gd.BitmapLeft = m_GlyphSlot->bitmap_left;
				gd.BitmapTop = m_GlyphSlot->bitmap_top;
				gd.Size = m_GlyphSlot->bitmap.width * m_GlyphSlot->bitmap.rows;
				gd.Advance = m_GlyphSlot->advance;
				gd.Character = c;
				m_Glyphs.insert(std::unordered_map<char, GlyphData>::value_type(c, gd));
				
				//Ref<Texture2D> charTex = Texture2D::Create(ImageFormat::Red, gd.BitmapWidth, gd.BitmapHeight);
				//charTex->SetData((void*)gd.Buffer, gd.Size);
				//m_CharTextures.push_back(charTex);
				//RenderCommand::SetImagePacking(4);
			}

			PackTexture();
			//m_FontTexture = API::Texture2D::Create(API::TextureFormat::R, m_Width, m_Height);
			//m_FontTexture->SetData((void*)m_PackedBitmap.first, m_PackedBitmap.second);
		}

		void Font::PackTexture() {

		}
	}
}