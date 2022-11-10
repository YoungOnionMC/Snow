#pragma once

#include "Snow/Render/Shader.h"
#include "Snow/Render/ShaderUniform.h"

#include "Snow/Core/Ref.h"


#include "Snow/Render/Texture.h"

#include <glm/glm.hpp>
#include <unordered_set>
#include <string>

namespace Snow {
	namespace Render {

		enum class MaterialFlag {
			None = BIT(0),
			DepthTest = BIT(1),
			Blend = BIT(2),
			TwoSided = BIT(3),
		};

		struct MaterialVariable {
			UniformType Type;
			Buffer Data;
		};

		using VariableMap = std::unordered_map<std::string, MaterialVariable>;

		class Material : public RefCounted {
			friend class Material;
		public:
			static Ref<Material> Create(const Ref<Shader>& shader, const std::string& name = "");
			static Ref<Material> Copy(const Ref<Material>& other, const std::string& name = "");
			virtual ~Material() {}

			virtual void Invalidate() = 0;

			virtual void Set(const std::string& name, float value) = 0;
			virtual void Set(const std::string& name, double value) = 0;
			virtual void Set(const std::string& name, int value) = 0;
			virtual void Set(const std::string& name, uint32_t value) = 0;
			virtual void Set(const std::string& name, bool value) = 0;
			virtual void Set(const std::string& name, const glm::vec2& value) = 0;
			virtual void Set(const std::string& name, const glm::vec3& value) = 0;
			virtual void Set(const std::string& name, const glm::vec4& value) = 0;
			virtual void Set(const std::string& name, const glm::mat3& value) = 0;
			virtual void Set(const std::string& name, const glm::mat4& value) = 0;

			virtual void Set(const std::string& name, const Ref<Texture2D>& texture) = 0;
			virtual void Set(const std::string& name, const Ref<Texture2D>& texture, uint32_t arrayIndex) = 0;
			virtual void Set(const std::string& name, const Ref<TextureCube>& texture) = 0;
			virtual void Set(const std::string& name, const Ref<Image2D>& texture) = 0;

			

			virtual float& GetFloat(const std::string& name) = 0;
			virtual int32_t& GetInt(const std::string& name) = 0;
			virtual uint32_t& GetUInt(const std::string& name) = 0;
			virtual bool& GetBool(const std::string& name) = 0;
			virtual glm::vec2& GetVector2(const std::string& name) = 0;
			virtual glm::vec3& GetVector3(const std::string& name) = 0;
			virtual glm::vec4& GetVector4(const std::string& name) = 0;
			virtual glm::mat3& GetMatrix3(const std::string& name) = 0;
			virtual glm::mat4& GetMatrix4(const std::string& name) = 0;

			virtual Ref<Texture2D> GetTexture2D(const std::string& name) = 0;
			virtual Ref<TextureCube> GetTextureCube(const std::string& name) = 0;

			virtual Ref<Texture2D> TryGetTexture2D(const std::string& name) = 0;
			virtual Ref<TextureCube> TryGetTextureCube(const std::string& name) = 0;

			virtual uint32_t GetFlags() const = 0;
			virtual bool GetFlag(MaterialFlag flag) const = 0;
			virtual void SetFlag(MaterialFlag flag, bool value = true) = 0;

			virtual Ref<Shader> GetShader() = 0;
			virtual const std::string& GetName() const = 0;

			virtual VariableMap GetVariables() const = 0;
			virtual std::map<std::string, std::pair<Render::ResourceType, Ref<Render::Texture>>> GetTextures() const = 0;
		};
	}
}
