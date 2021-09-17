#pragma once

#include "Snow/Render/StorageBufferSet.h"

#include <map>
#include "Snow/Core/Assert.h"

namespace Snow {
	class VulkanStorageBufferSet : public Render::StorageBufferSet {
	public:
		VulkanStorageBufferSet(uint32_t frames) :
			m_Frames(frames) {}

		~VulkanStorageBufferSet() override = default;

		virtual void Create(uint32_t size, uint32_t binding) override {
			for (uint32_t frame = 0; frame < m_Frames; frame++) {
				const Ref<Render::StorageBuffer> storageBuffer = Render::StorageBuffer::Create(size, binding);
				Set(storageBuffer, 0, frame);
			}
		}

		virtual void Resize(const uint32_t binding, const uint32_t set, const uint32_t size) override {
			for (uint32_t frame = 0; frame < m_Frames; frame++) {
				m_StorageBuffers.at(frame).at(set).at(binding)->Resize(size);
			}
		}

		virtual Ref<Render::StorageBuffer> Get(uint32_t binding, uint32_t set = 0, uint32_t frame = 0) override {
			SNOW_CORE_ASSERT(m_StorageBuffers.find(frame) != m_StorageBuffers.end());
			SNOW_CORE_ASSERT(m_StorageBuffers.at(frame).find(set) != m_StorageBuffers.at(frame).end());
			SNOW_CORE_ASSERT(m_StorageBuffers.at(frame).at(set).find(binding) != m_StorageBuffers.at(frame).at(set).end());

			return m_StorageBuffers.at(frame).at(set).at(binding);
		}

		virtual void Set(Ref<Render::StorageBuffer> storageBuffer, uint32_t set = 0, uint32_t frame = 0) override {
			m_StorageBuffers[frame][set][storageBuffer->GetBinding()] = storageBuffer;
		}
	private:
		uint32_t m_Frames;

		std::map<uint32_t, std::map<std::uint32_t, std::map<uint32_t, Ref<Render::StorageBuffer>>>> m_StorageBuffers; //Frame -> Set -> Binding -> Buffer
	};
}