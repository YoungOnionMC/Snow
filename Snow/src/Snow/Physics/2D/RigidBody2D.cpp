#include <spch.h>
#include "Snow/Physics/2D/RigidBody2D.h"

#include "Snow/Math/Mat4.h"

#include <glm/gtx/vec_swizzle.hpp>

namespace Snow {

	RigidBody2D::RigidBody2D(b2World* world, const glm::mat4& transform) :
		m_World(world) {

        glm::vec3 translation, rotation, scale;
        Math::DecomposeTransform(transform, translation, rotation, scale);


        m_BodyDef = new b2BodyDef();
        m_BodyDef->type = b2_dynamicBody;
        m_BodyDef->position.Set(translation.x, translation.y);
        m_BodyDef->angle = rotation.z;

        m_Body = world->CreateBody(m_BodyDef);

        m_Shape = new b2PolygonShape();
        m_Size = scale.xy();
        m_Shape->SetAsBox(scale.x / 2.0f, scale.y / 2.0f);
        m_FixtureDef = new b2FixtureDef();
        m_FixtureDef->shape = m_Shape;
        m_FixtureDef->density = 1.0f;
        m_FixtureDef->friction = 0.0f;
        m_Fixture = m_Body->CreateFixture(m_FixtureDef);
	}

	void RigidBody2D::SetTransform(const glm::mat4& transform) {
		glm::vec3 Transform, Rotation, Scale;
		Math::DecomposeTransform(transform, Transform, Rotation, Scale);

		m_Body->SetTransform({ Transform.x, Transform.y }, Rotation.z);

        if (Scale.xy() != m_Size)
            SetSizeAsBox(Scale.xy);
	}

}
