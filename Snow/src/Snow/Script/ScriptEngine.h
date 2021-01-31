#pragma once

#include "Snow/Scene/Components.h"
#include "Snow/Scene/Entity.h"

#include <unordered_map>

extern "C" {
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoClassField MonoClassField;
}

namespace Snow {
	namespace Script {

		enum class FieldType {
			None = 0,
			Bool,
			Char,

			Int8, 
			Int16, 
			Int32,
			Int64,

			UInt8,
			UInt16, 
			UInt32,
			UInt64,

			Float, 
			Double,

			String,

			Vec2, 
			Vec3, 
			Vec4,

			SByte = Int8,
			Byte = UInt8,

			Short = Int16,
			UShort = UInt16,
			
			Int = Int32,
			UInt = UInt32,
			
			Long = Int64,
			ULong = UInt64,
		};

		const char* FieldTypeToString(FieldType type);

		struct EntityScriptClass;
		class ScriptEngine;
		struct EntityInstance {
			EntityScriptClass* ScriptClass = nullptr;

			uint32_t Handle = 0;
			Scene* SceneInstance = nullptr;

			MonoObject* GetInstance();
		};

		struct PublicField {
			std::string Name;
			FieldType Type;

			PublicField(const std::string& name, FieldType type);
			PublicField(const PublicField&) = delete;
			PublicField& operator=(const PublicField&) = delete;
			PublicField(PublicField&& other);
			~PublicField();

			void CopyStoredValueToRuntime();
			bool IsRuntimeAvailable() const;

			template<typename T>
			T GetStoredValue() const {
				T value;
				GetStoredValue_Internal(&value);
				return value;
			}

			template<typename T>
			void SetStoredValue(T value) const {
				SetStoredValue_Internal(&value);
			}

			template<typename T>
			T GetRuntimeValue() const {
				T value;
				GetRuntimeValue_Internal(&value);
				return value;
			}

			template<typename T>
			void SetRuntimeValue(T value) const {
				SetRuntimeValue_Internal(&value);
			}

			void SetStoredValueRaw(void* src);
		private:
			EntityInstance* m_EntityInstance;
			MonoClassField* m_MonoClassField;
			uint8_t* m_StoredValueBuffer = nullptr;

			uint8_t* AllocateBuffer(FieldType type);
			void SetStoredValue_Internal(void* value) const;
			void GetStoredValue_Internal(void* outvalue) const;
			void SetRuntimeValue_Internal(void* value) const;
			void GetRuntimeValue_Internal(void* outvalue) const;

			friend class ScriptEngine;
		};

		using ScriptModuleField = std::unordered_map<std::string, PublicField>;
		using ScriptModuleFieldMap = std::unordered_map<std::string, ScriptModuleField>;

		struct EntityInstanceData {
			EntityInstance Instance;
			ScriptModuleFieldMap ModuleFieldMap;
		};

		using EntityInstanceMap = std::unordered_map<UUID, std::unordered_map<UUID, EntityInstanceData>>;

		class ScriptEngine {
		public:
			static void Init(const std::string& assemblyPath);
			static void Shutdown();

			static void OnSceneDestruct(UUID sceneID);

			static void LoadSnowRuntimeAssembly(const std::string& path);
			static void ReloadAssembly(const std::string& path);

			static void SetSceneContext(const Ref<Scene>& scene);
			static const Ref<Scene>& GetSceneContext();

			static void CopyEntityScriptData(UUID dst, UUID src);

			static void OnCreateEntity(Entity entity);
			static void OnCreateEntity(UUID sceneID, UUID entityID);
			static void OnUpdateEntity(UUID sceneID, UUID entityID, Timestep ts);

			static void OnScriptComponentDestroyed(UUID sceneID, UUID entityID);


			static bool ModuleExists(const std::string& moduleName);
			static void InitScriptEntity(Entity entity);
			static void ShutdownScriptEntity(Entity entity, const std::string& moduleName);
			static void InstantiateEntityClass(Entity entity);

			static EntityInstanceMap& GetEntityInstanceMap();
			static EntityInstanceData& GetEntityInstanceData(UUID sceneID, UUID entityID);

			static void OnImGuiRender();

		private:


		};
	}
}