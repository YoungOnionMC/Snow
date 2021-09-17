#include <spch.h>
#include "Snow/Render/Renderer2D.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Snow/Math/Mat4.h"

#include <ft2build.h>
#include FT_FREETYPE_H

namespace Snow {
    namespace Render {
        Renderer2D::Renderer2D() {
            Init();
        }

        Renderer2D::~Renderer2D() {
            Shutdown();
        }

        void Renderer2D::Init() {

            m_CommandBuffer = RenderCommandBuffer::Create(0, "Renderer2D");

            FramebufferSpecification fbSpec;
            fbSpec.AttachmentList = { ImageFormat::RGBA32F, ImageFormat::DepthStencil };
            fbSpec.Samples = 1;
            //fbSpec.ClearOnLoad = false;
            fbSpec.ClearColor = { 0.1f, 0.1f, 0.1f, 1.0f };
            fbSpec.DebugName = "Renderer2D Framebuffer";

            Ref<Framebuffer> framebuffer = Framebuffer::Create(fbSpec);

            RenderPassSpecification rpSpec;
            rpSpec.TargetFramebuffer = framebuffer;
            rpSpec.DebugName = "Renderer2D RenderPass";
            Ref<RenderPass> renderPass = RenderPass::Create(rpSpec);

            // Quad Pipeline
            {
                PipelineSpecification pipelineSpec;
                pipelineSpec.DebugName = "Renderer2D Quad";
                pipelineSpec.Shader = Renderer::GetShaderLibrary()->Get("Renderer2D-Quad");
                pipelineSpec.BindedRenderPass = renderPass;
                pipelineSpec.BackfaceCulling = false;
                pipelineSpec.Layout = {
                    {AttribType::Float3, "a_Position"},
                    {AttribType::Float2, "a_TexCoord"},
                    {AttribType::Float, "a_TexID"},
                    {AttribType::Float4, "a_Color"}
                };
                m_QuadPipeline = Pipeline::Create(pipelineSpec);

                m_QuadVertexBuffer = VertexBuffer::Create(MaxVertices * sizeof(QuadVertex));
                m_QuadVertexBufferBase = new QuadVertex[MaxVertices];

                uint32_t* quadIndices = new uint32_t[MaxIndices];

                uint32_t offset = 0;
                for (uint32_t i = 0; i < MaxIndices; i += 6) {
                    quadIndices[i + 0] = offset + 0;
                    quadIndices[i + 1] = offset + 1;
                    quadIndices[i + 2] = offset + 2;

                    quadIndices[i + 3] = offset + 2;
                    quadIndices[i + 4] = offset + 3;
                    quadIndices[i + 5] = offset + 0;

                    offset += 4;
                }

                m_QuadIndexBuffer = IndexBuffer::Create(quadIndices, MaxIndices * sizeof(uint32_t));
                delete[] quadIndices;
            }

            m_QuadMaterial = Material::Create(m_QuadPipeline->GetSpecification().Shader, "QuadMaterial");
            

            // Lines
            {
                PipelineSpecification pipelineSpec;
                pipelineSpec.DebugName = "Renderer2D Line";
                pipelineSpec.Shader = Renderer::GetShaderLibrary()->Get("Renderer2D-Quad");
                pipelineSpec.BindedRenderPass = renderPass;
                pipelineSpec.Topology = PrimitiveTopology::Line;
                pipelineSpec.LineWidth = 2.0f;
                pipelineSpec.Layout = {
                    {AttribType::Float3, "a_Position"},
                    {AttribType::Float4, "a_Color"}
                };
                m_LinePipeline = Pipeline::Create(pipelineSpec);

                m_LineVertexBuffer = VertexBuffer::Create(MaxLineVertices * sizeof(LineVertex));
                m_LineVertexBufferBase = new LineVertex[MaxVertices];

                uint32_t* lineIndices = new uint32_t[MaxLineIndices];

                for (uint32_t i = 0; i < MaxLineIndices; i++) 
                    lineIndices[i] = i;

                m_LineIndexBuffer = IndexBuffer::Create(lineIndices, MaxLineIndices * sizeof(uint32_t));
                delete[] lineIndices;
            }

            m_LineMaterial = Material::Create(m_LinePipeline->GetSpecification().Shader, "LineMaterial");

            // Circles
            {
                PipelineSpecification pipelineSpec;
                pipelineSpec.DebugName = "Renderer2D Circle";
                pipelineSpec.Shader = Renderer::GetShaderLibrary()->Get("Renderer2D-Circle");
                pipelineSpec.BindedRenderPass = renderPass;
                pipelineSpec.Layout = {
                    {AttribType::Float3, "a_WorldPosition"},
                    {AttribType::Float, "a_Thickness"},
                    {AttribType::Float2, "a_LocalPosition"},
                    {AttribType::Float4, "a_Color"},
                };
                m_CirclePipeline = Pipeline::Create(pipelineSpec);

                m_CircleVertexBuffer = VertexBuffer::Create(MaxVertices * sizeof(CircleVertex));
                m_CircleVertexBufferBase = new CircleVertex[MaxVertices];
            }

            uint32_t framesInFlight = Renderer::GetConfig().FramesInFlight;
            m_UniformBufferSet = UniformBufferSet::Create(framesInFlight);
            m_UniformBufferSet->Create(sizeof(UBCamera), 0);
            
            m_QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
            m_QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
            m_QuadVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
            m_QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

            m_WhiteTexture = Renderer::GetWhiteTexture();

            m_TextureSlots[0] = m_WhiteTexture;
        }

        void Renderer2D::Shutdown() {

        }

        void Renderer2D::BeginScene(const glm::mat4& viewProj, const glm::mat4& view, bool depthTest) {
            m_CameraViewProj = viewProj;
            m_CameraView = view;
            m_DepthTest = depthTest;

            Renderer::Submit([uniformBufferSet = m_UniformBufferSet, viewProj]() mutable {
                uint32_t bufferIndex = Renderer::GetCurrentFrameIndex();
                uniformBufferSet->Get(0, 0, bufferIndex)->RTSetData(&viewProj, sizeof(UBCamera));
            });

            ResetStats();

            m_QuadIndexCount = 0;
            m_QuadVertexBufferPtr = m_QuadVertexBufferBase;

            m_LineIndexCount = 0;
            m_LineVertexBufferPtr = m_LineVertexBufferBase;

            m_CircleIndexCount = 0;
            m_CircleVertexBufferPtr = m_CircleVertexBufferBase;

            m_TextureSlotIndex = 1;

            for (uint32_t i = 1; i < m_TextureSlots.size(); i++) 
                m_TextureSlots[i] = nullptr;
            

        }

        void Renderer2D::EndScene() {
            m_CommandBuffer->Begin();
            Renderer::BeginRenderPass(m_CommandBuffer, m_QuadPipeline->GetSpecification().BindedRenderPass);

            uint32_t dataSize = (uint32_t)((uint8_t*)m_QuadVertexBufferPtr - (uint8_t*)m_QuadVertexBufferBase);
            if (dataSize) {
                m_QuadVertexBuffer->SetData(m_QuadVertexBufferBase, dataSize);

                for (uint32_t i = 0; i < m_TextureSlots.size(); i++) {
                    if (m_TextureSlots[i])
                        m_QuadMaterial->Set("u_Textures", m_TextureSlots[i], i);
                    else
                        m_QuadMaterial->Set("u_Textures", m_WhiteTexture, i);
                }

                Renderer::RenderGeometry(m_CommandBuffer, m_QuadPipeline, m_UniformBufferSet, nullptr, m_QuadMaterial, m_QuadVertexBuffer, m_QuadIndexBuffer, glm::mat4(1.0f), m_QuadIndexCount);
                
                m_Stats.DrawCalls++;
            }


            dataSize = (uint32_t)((uint8_t*)m_LineVertexBufferPtr - (uint8_t*)m_LineVertexBufferBase);
            if (dataSize) {
                m_LineVertexBuffer->SetData(m_LineVertexBufferBase, dataSize);

                //Renderer::SetLineWidth(m_CommandBuffer, m_LineWidth);

                Renderer::RenderGeometry(m_CommandBuffer, m_LinePipeline, m_UniformBufferSet, nullptr, m_LineMaterial, m_LineVertexBuffer, m_LineIndexBuffer, glm::mat4(1.0f), m_LineIndexCount);

                m_Stats.DrawCalls++;
            }

            Renderer::EndRenderPass(m_CommandBuffer);
            m_CommandBuffer->End();
            m_CommandBuffer->Submit();

            
        }

        Ref<RenderPass> Renderer2D::GetTargetRenderPass() {
            return m_QuadPipeline->GetSpecification().BindedRenderPass;
        }

        void Renderer2D::SetTargetRenderPass(Ref<RenderPass> renderPass) {
            if (renderPass != m_QuadPipeline->GetSpecification().BindedRenderPass) {
                {
                    PipelineSpecification pipelineSpec = m_QuadPipeline->GetSpecification();
                    pipelineSpec.BindedRenderPass = renderPass;
                    m_QuadPipeline = Pipeline::Create(pipelineSpec);
                }
                {
                    PipelineSpecification pipelineSpec = m_LinePipeline->GetSpecification();
                    pipelineSpec.BindedRenderPass = renderPass;
                    m_LinePipeline = Pipeline::Create(pipelineSpec);
                }
                {
                    PipelineSpecification pipelineSpec = m_CirclePipeline->GetSpecification();
                    pipelineSpec.BindedRenderPass = renderPass;
                    m_CirclePipeline = Pipeline::Create(pipelineSpec);
                }
            }
        }

        void Renderer2D::FlushAndReset() {
            m_QuadIndexCount = 0;
            m_QuadVertexBufferPtr = m_QuadVertexBufferBase;

            m_LineIndexCount = 0;
            m_LineVertexBufferPtr = m_LineVertexBufferBase;

            m_CircleIndexCount = 0;
            m_CircleVertexBufferPtr = m_CircleVertexBufferBase;

            m_TextureSlotIndex = 1;

            for (uint32_t i = 1; i < m_TextureSlots.size(); i++)
                m_TextureSlots[i] = nullptr;
        }

        void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {
            DrawQuad({ position.x, position.y, 0.0f }, size, color);
        }

        void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {
            glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 0.0f });

            DrawQuad(transform, color);
        }

        void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& tint) {
            DrawQuad({ position.x, position.y, 0.0f }, size, texture, tint);
        }

        void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& tint) {
            glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 0.0f });

            DrawQuad(transform, texture, tint);
        }

        void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color) {
            if (m_QuadIndexCount >= MaxIndices)
                FlushAndReset();

            constexpr size_t quadVertexCount = 4;
            const float textureIndex = 0.0f;
            constexpr glm::vec2 textureCoords[4] = { {0.0f, 0.0f}, {1.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f} };
            const float tilingFactor = 1.0f;

            for (size_t i = 0; i < quadVertexCount; i++) {
                m_QuadVertexBufferPtr->Position = transform * m_QuadVertexPositions[i];
                m_QuadVertexBufferPtr->TexCoord = textureCoords[i];
                m_QuadVertexBufferPtr->TexID = textureIndex;
                m_QuadVertexBufferPtr->Color = color;
                m_QuadVertexBufferPtr++;
            }

            m_QuadIndexCount += 6;

            m_Stats.QuadCount++;
        }

        void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, const glm::vec4& tint) {
            if (m_QuadIndexCount >= MaxIndices)
                FlushAndReset();
            
            constexpr size_t quadVertexCount = 4;
            const glm::vec2 textureCoords[4] = { {0.0f, 0.0f}, {1.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f} };
            const float tilingFactor = 1.0f;

            float textureIndex = 0.0f;
            for (uint32_t i = 1; i < m_TextureSlotIndex; i++) {
                if (*m_TextureSlots[i].Raw() == *texture.Raw()) {
                    textureIndex = (float)i;
                    break;
                }
            }

            if (textureIndex == 0.0f) {
                if (m_TextureSlotIndex >= MaxTextureSlots)
                    FlushAndReset();

                textureIndex = (float)m_TextureSlotIndex;
                m_TextureSlots[m_TextureSlotIndex] = texture;
                m_TextureSlotIndex++;
            }

            for (size_t i = 0; i < quadVertexCount; i++) {
                m_QuadVertexBufferPtr->Position = transform * m_QuadVertexPositions[i];
                m_QuadVertexBufferPtr->TexCoord = textureCoords[i];
                m_QuadVertexBufferPtr->TexID = textureIndex;
                m_QuadVertexBufferPtr->Color = tint;
                m_QuadVertexBufferPtr++;
            }

            m_QuadIndexCount += 6;

            m_Stats.QuadCount++;
        }

        void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color) {
            DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
        }

        void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color) {
            if (m_QuadIndexCount >= MaxIndices)
                FlushAndReset();

            constexpr size_t quadVertexCount = 4;
            const float textureIndex = 0.0f;
            constexpr glm::vec2 textureCoords[4] = { {0.0f, 0.0f}, {1.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f} };
            const float tilingFactor = 1.0f;

            glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
                * glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
                * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

            

            for (size_t i = 0; i < quadVertexCount; i++) {
                m_QuadVertexBufferPtr->Position = transform * m_QuadVertexPositions[i];
                m_QuadVertexBufferPtr->TexCoord = textureCoords[i];
                m_QuadVertexBufferPtr->TexID = textureIndex;
                m_QuadVertexBufferPtr->Color = color;
                m_QuadVertexBufferPtr++;
            }

            m_QuadIndexCount += 6;

            m_Stats.QuadCount++;
        }

        void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, const glm::vec4& color) {
            DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, color);
        }

        void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, const glm::vec4& color) {
            if (m_QuadIndexCount >= MaxIndices)
                FlushAndReset();

            constexpr size_t quadVertexCount = 4;
            constexpr glm::vec2 textureCoords[4] = { {0.0f, 0.0f}, {1.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f} };
            const float tilingFactor = 1.0f;

            glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
                * glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
                * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

            float textureIndex = 0.0f;
            for (uint32_t i = 1; i < m_TextureSlotIndex; i++) {
                if (*m_TextureSlots[i].Raw() == *texture.Raw()) {
                    textureIndex = (float)i;
                    break;
                }
            }

            if (textureIndex == 0.0f) {
                if (m_TextureSlotIndex >= MaxTextureSlots)
                    FlushAndReset();

                textureIndex = (float)m_TextureSlotIndex;
                m_TextureSlots[m_TextureSlotIndex] = texture;
                m_TextureSlotIndex++;
            }

            

            for (size_t i = 0; i < quadVertexCount; i++) {
                m_QuadVertexBufferPtr->Position = transform * m_QuadVertexPositions[i];
                m_QuadVertexBufferPtr->TexCoord = textureCoords[i];
                m_QuadVertexBufferPtr->TexID = textureIndex;
                m_QuadVertexBufferPtr->Color = color;
                m_QuadVertexBufferPtr++;
            }

            m_QuadIndexCount += 6;

            m_Stats.QuadCount++;
        }

        void Renderer2D::FillCircle(const glm::vec2& position, float radius, const glm::vec4& color, float thickness) {
            FillCircle({ position.x, position.y, 0.0f }, radius, color, thickness);
        }

        void Renderer2D::FillCircle(const glm::vec3& position, float radius, const glm::vec4& color, float thickness) {
            if (m_CircleIndexCount > MaxIndices)
                FlushAndReset();

            glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
                * glm::scale(glm::mat4(1.0f), { radius * 2.0f, radius * 2.0f, 1.0f });

            for (int i = 0; i < 4; i++) {
                m_CircleVertexBufferPtr->WorldPosition = transform * m_QuadVertexPositions[i];
                m_CircleVertexBufferPtr->Thickness = thickness;
                m_CircleVertexBufferPtr->LocalPosition = m_QuadVertexPositions[i] * 2.0f;
                m_CircleVertexBufferPtr->Color = color;
                m_CircleVertexBufferPtr++;
            }

            m_CircleIndexCount += 6;
            m_Stats.QuadCount++;
        }


        void Renderer2D::DrawLine(const glm::vec2& startPosition, const glm::vec2& endPosition, const glm::vec4& color) {
            DrawLine({startPosition.x, startPosition.y, 0.0f}, {endPosition.x, endPosition.y, 0.0f}, color);
        }

        void Renderer2D::DrawLine(const glm::vec3& startPosition, const glm::vec3& endPosition, const glm::vec4& color) {
            if (m_LineIndexCount >= MaxLineIndices)
                FlushAndReset();
            
            m_LineVertexBufferPtr->Position = startPosition;
            m_LineVertexBufferPtr->Color = color;
            m_LineVertexBufferPtr++;
            
            m_LineVertexBufferPtr->Position = endPosition;
            m_LineVertexBufferPtr->Color = color;
            m_LineVertexBufferPtr++;
            
            m_LineIndexCount += 2;

            m_Stats.LineCount++;
        }

        void Renderer2D::DrawCircle(const glm::vec3& position, const glm::vec3& rotation, float radius, const glm::vec4& color) {
            if (m_LineIndexCount >= MaxLineIndices)
                FlushAndReset();
            
            glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
                * glm::rotate(glm::mat4(1.0f), rotation.x, { 1.0f, 0.0f, 0.0f })
                * glm::rotate(glm::mat4(1.0f), rotation.y, { 0.0f, 1.0f, 0.0f })
                * glm::rotate(glm::mat4(1.0f), rotation.z, { 0.0f, 0.0f, 1.0f })
                * glm::scale(glm::mat4(1.0f), glm::vec3(radius));

            int segments = 32;
            for (int i = 0; i < segments; i++) {
                float angle = 2.0f * glm::pi<float>() * (float)i / segments;
                glm::vec4 startPosition = { glm::cos(angle), glm::sin(angle), 0.0f, 1.0f };
                angle = 2.0f * glm::pi<float>() * (float)((i + 1) % segments) / segments;
                glm::vec4 endPosition = { glm::cos(angle), glm::sin(angle), 0.0f, 1.0f };

                glm::vec3 p0 = transform * startPosition;
                glm::vec3 p1 = transform * endPosition;
                DrawLine(p0, p1, color);
            }
        }

        void Renderer2D::DrawAABB(const Core::AABB& aabb, const glm::mat4& transform, const glm::vec4& color) {
            if (m_LineIndexCount >= MaxLineIndices)
                FlushAndReset();
            
            glm::vec4 min = { aabb.Min.x, aabb.Min.y, aabb.Min.z, 1.0f };
            glm::vec4 max = { aabb.Max.x, aabb.Max.y, aabb.Max.z, 1.0f };

            glm::vec4 corners[8] = {
                transform * glm::vec4 { aabb.Min.x, aabb.Min.y, aabb.Max.z, 1.0f },
                transform * glm::vec4 { aabb.Min.x, aabb.Max.y, aabb.Max.z, 1.0f },
                transform * glm::vec4 { aabb.Max.x, aabb.Max.y, aabb.Max.z, 1.0f },
                transform * glm::vec4 { aabb.Max.x, aabb.Min.y, aabb.Max.z, 1.0f },

                transform * glm::vec4 { aabb.Min.x, aabb.Min.y, aabb.Min.z, 1.0f },
                transform * glm::vec4 { aabb.Min.x, aabb.Max.y, aabb.Min.z, 1.0f },
                transform * glm::vec4 { aabb.Max.x, aabb.Max.y, aabb.Min.z, 1.0f },
                transform * glm::vec4 { aabb.Max.x, aabb.Min.y, aabb.Min.z, 1.0f }
            };

            for (uint32_t i = 0; i < 4; i++)
                DrawLine(glm::vec3( corners[i]), glm::vec3(corners[(i + 1) % 4]), color);

            for (uint32_t i = 0; i < 4; i++)
                DrawLine(glm::vec3(corners[i + 4]), glm::vec3(corners[((i + 1) % 4) + 4]), color);

            for (uint32_t i = 0; i < 4; i++)
                DrawLine(glm::vec3(corners[i]), glm::vec3(corners[i + 4]), color);

        }

        void Renderer2D::DrawAABB(Ref<Mesh> mesh, const glm::mat4& transform, const glm::vec4& color) {
            const auto& meshAssetSubmeshes = mesh->GetMeshAsset()->GetSubmeshes();
            auto& submeshes = mesh->GetSubmeshes();
            for (uint32_t submeshIndex : submeshes) {
                const Submesh& submesh = meshAssetSubmeshes[submeshIndex];
                auto& aabb = submesh.BoundingBox;
                auto aabbTransform = transform * submesh.Transform;
                DrawAABB(aabb, aabbTransform, color);
            }
        }

        void Renderer2D::DrawString(const std::string& text, const std::string fontName, const glm::vec2& position, const glm::vec3& color) {
            /*
            Ref<Font> font = Renderer::GetFontManager()->Get(fontName);

            float x = position.x;

            auto scale = font->GetScale();

            

            for (uint32_t i = 0; i < text.length(); i++) {
                auto glyph = font->GetGlyph(text[i]);
                Ref<Texture2D> glyphTexture = font->GetCharTexture(text[i]);

                float textureIndex = 0.0f;
                for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) {
                    if (s_Data.TextureSlots[i].Raw() == glyphTexture.Raw()) {
                        textureIndex = (float)i;
                        break;
                    }
                }

                if (textureIndex == 0.0f) {
                    if (s_Data.TextureSlotIndex >= Renderer2DStaticData::MaxTextureSlots)
                        NextBatch();

                    textureIndex = (float)s_Data.TextureSlotIndex;
                    s_Data.TextureSlots[s_Data.TextureSlotIndex] = glyphTexture;
                    s_Data.TextureSlotIndex++;
                }

                if (glyphTexture) {
                    if (i > 0) {
                        
                    }
                    glm::vec2 pos = position + glm::vec2(x, 0.0f);
                    glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(pos, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3( glyph.BitmapWidth / scale.x,glyph.BitmapHeight / scale.y, 0.0f) );

                    float x0 = x + glyph.BitmapLeft / scale.x;
                    float y0 = position.y;
                    float x1 = x0 + glyph.BitmapWidth / scale.x;
                    float y1 = y0 + glyph.BitmapHeight / scale.y;

                    s_Data.StringVertexData->Position = transform * s_Data.QuadVertexPositions[0];
                    s_Data.StringVertexData->TexCoord = glm::vec2(0.0f, 1.0f);
                    s_Data.StringVertexData->TexID = textureIndex;
                    s_Data.StringVertexData->Color = color;
                    s_Data.StringVertexData++;

                    s_Data.StringVertexData->Position = transform * s_Data.QuadVertexPositions[1];
                    s_Data.StringVertexData->TexCoord = glm::vec2(1.0f, 1.0f);
                    s_Data.StringVertexData->TexID = textureIndex;
                    s_Data.StringVertexData->Color = color;
                    s_Data.StringVertexData++;

                    s_Data.StringVertexData->Position = transform * s_Data.QuadVertexPositions[2];
                    s_Data.StringVertexData->TexCoord = glm::vec2(1.0f, 0.0f);
                    s_Data.StringVertexData->TexID = textureIndex;
                    s_Data.StringVertexData->Color = color;
                    s_Data.StringVertexData++;

                    s_Data.StringVertexData->Position = transform * s_Data.QuadVertexPositions[3];
                    s_Data.StringVertexData->TexCoord = glm::vec2(0.0f, 0.0f);
                    s_Data.StringVertexData->TexID = textureIndex;
                    s_Data.StringVertexData->Color = color;
                    s_Data.StringVertexData++;

                    s_Data.StringIndexCount += 6;

                    x += (glyph.Advance.x>>6) / scale.x;
                }
            }
            */
        }

        void Renderer2D::SetLineWidth(float lineWidth) {
            m_LineWidth = lineWidth;
        }

        void Renderer2D::ResetStats() {
            memset(&m_Stats, 0, sizeof(Statistics));
        }

        Renderer2D::Statistics Renderer2D::GetStats() {
            return m_Stats;
        }
    }
}