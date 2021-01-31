#pragma once

#include <box2d/box2d.h>

#include <glm/glm.hpp>

namespace Snow {

    enum class RigidBodyType {
        Static = 0,
        Kinematic,
        Dynamic
    };

	class RigidBody2D {
	public:
        RigidBody2D() = default;
        RigidBody2D(b2World* world, const glm::mat4& transform);


        glm::vec2 GetPosition() const { return glm::vec2(m_Body->GetPosition().x, m_Body->GetPosition().y); }
        float GetRotation() const { return m_Body->GetAngle(); }


        void SetTransform(const glm::mat4& transform);
        glm::vec2 GetSize() const { return m_Size; }
        void SetSizeAsBox(const glm::vec2& size) { m_Shape->SetAsBox(size.x / 2.0f, size.y / 2.0f); }


        float GetFriction() const { return m_Fixture->GetFriction(); }
        void SetFriction(float friction) { m_Fixture->SetFriction(friction); }
        
        float GetDensity() const { return m_Fixture->GetDensity(); }
        void SetDensity(float density) { m_Fixture->SetDensity(density); }
        
        uint32_t GetType() const { return (uint32_t)m_Body->GetType(); }
        void SetType(RigidBodyType type) { m_Body->SetType((b2BodyType)type); }

        void SetPolygonShape(const glm::vec2* points, uint32_t numPoints) { m_Shape->Set((b2Vec2*)points, numPoints); }

        b2Body* GetBody() const { return m_Body; }

        b2Body* operator->() { return GetBody(); }
	private:
        b2Body* m_Body;
        b2BodyDef* m_BodyDef;
        b2World* m_World;
        b2PolygonShape* m_Shape;
        b2FixtureDef* m_FixtureDef;
        b2Fixture* m_Fixture;
        glm::vec2 m_Size;
	};

}