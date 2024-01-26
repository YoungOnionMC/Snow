#include "RayRenderer.h"


static glm::vec4 ConvertUIntToVec4(uint32_t num) {
    uint8_t a = num >> 24 & 0xff;
    uint8_t b = num >> 16 & 0xff;
    uint8_t g = num >> 8 & 0xff;
    uint8_t r = num & 0xff;
    return glm::vec4(r, g, b, a);
}

static uint32_t ConvertVec4ToUInt(glm::vec4 num) {
    uint8_t r = (uint8_t)(num.r * 255.0f);
    uint8_t g = (uint8_t)(num.g * 255.0f);
    uint8_t b = (uint8_t)(num.b * 255.0f);
    uint8_t a = (uint8_t)(num.a * 255.0f);
    return (a << 24) | (b << 16) | (g << 8) | (r);
}

void RayRenderer::OnResize(uint32_t width, uint32_t height) {
    m_ViewportSize = { width, height };
    if (m_OutputImage) {
        m_OutputImage->Resize(width, height);
        //m_OutputImage->Invalidate();
    }
    else {
        Render::ImageSpecification imageSpec;
        imageSpec.Format = ImageFormat::RGBA;
        imageSpec.Width = width, imageSpec.Height = height;
        imageSpec.Usage = ImageUsage::Texture;
        imageSpec.DebugName = "RayTraceAttachment4";
        m_OutputImage = Render::Image2D::Create(imageSpec);
        m_OutputImage->Invalidate();
    }

    m_ImageData.resize(width * height);
    //SNOW_CORE_TRACE(m_ImageData.size());
}

void RayRenderer::OnRender(const Scene& scene, const RayCamera& camera) {
    //OnResize(m_ViewportSize.x, m_ViewportSize.y);
   // camera.OnResize(m_ViewportSize.x, m_ViewportSize.y);

    m_ActiveScene = &scene;
    m_ActiveCamera = &camera;
    if (m_OutputImage == nullptr)
        return;
    
    Buffer imageData = Buffer(m_ViewportSize.x * m_ViewportSize.y * 4);

    for (int y = 0; y < m_OutputImage->GetHeight(); y++) {
        for (int x = 0; x < m_OutputImage->GetWidth(); x++) {
#if false
            {
                glm::vec4 color = RayGen(x, y);
                color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));
                imageData.As<uint32_t>()[x + y * m_OutputImage->GetWidth()] = ConvertVec4ToUInt(color);
            }
#else
            {
                glm::vec4 color = glm::vec4(1.0f, float(x) / (float)m_OutputImage->GetWidth(), 1.0f, 1.0f);
                imageData.As<uint32_t>()[x + y * m_OutputImage->GetWidth()] = ConvertVec4ToUInt(color);
            }
#endif
        }
    }

    m_OutputImage->SetData(imageData);
}

glm::vec4 RayRenderer::RayGen(uint32_t x, uint32_t y) {
    Math::Ray ray;
    ray.Origin = m_ActiveCamera->GetPosition();
    ray.Direction = m_ActiveCamera->getRayDirections()[x + y * m_OutputImage->GetWidth()];

    glm::vec3 color(0.0f);
    float multiplier = 1.0f;
    for (int i = 0; i < m_Bounces; i++) {
        RayRenderer::HitInfo hitInfo = TraceRay(ray);
        if (hitInfo.HitDistance < 0.0f) {
            glm::vec3 skyColor = glm::vec3(0.0f, 0.0f, 0.0f);
            color += skyColor * multiplier;
            break;
        }

        glm::vec3 lightDir = glm::normalize(glm::vec3(1, -1, -1));

        float dot = glm::max(glm::dot(hitInfo.WorldNormal, -lightDir), 0.0f);

        const Sphere& sphere = m_ActiveScene->Spheres[hitInfo.ObjectIndex];

        glm::vec3 sphereColor = sphere.Color;
        sphereColor *= dot;
        color += sphereColor * multiplier;

        multiplier *= 0.7f;

        ray.Origin = hitInfo.WorldPos + hitInfo.WorldNormal * 0.00001f;
        ray.Direction = glm::reflect(ray.Direction, hitInfo.WorldNormal);
    }
    return glm::vec4(color, 1.0f);
}

RayRenderer::HitInfo RayRenderer::TraceRay(const Math::Ray& ray) {
    uint32_t closestSphere = std::numeric_limits<uint32_t>::max();
    float hitDistance = std::numeric_limits<float>::max();

    for (size_t i = 0; i < m_ActiveScene->Spheres.size(); i++) {
        const Sphere& sphere = m_ActiveScene->Spheres[i];
        glm::vec3 origin = ray.Origin - sphere.Pos;

        float a = glm::dot(ray.Direction, ray.Direction);
        float b = 2.0f * glm::dot(origin, ray.Direction);
        float c = glm::dot(origin, origin) - sphere.radius * sphere.radius;

        float discriminant = b * b - 4.0f * a * c;

        if (discriminant < 0.0f)
            continue;

        //float t0 = (-b + glm::sqrt(discriminant)) / (2.0f * a);
        float closestT = (-b - glm::sqrt(discriminant)) / (2.0f * a);
        if (closestT > 0.0f && closestT < hitDistance) {
            hitDistance = closestT;
            closestSphere = i;
        }
    }

    if (closestSphere == std::numeric_limits<uint32_t>::max())
        return Miss(ray);

    return ClosestHit(ray, hitDistance, closestSphere);
}

RayRenderer::HitInfo RayRenderer::ClosestHit(const Math::Ray& ray, float hitDistance, uint32_t objectIndex) {
    RayRenderer::HitInfo hitInfo;
    hitInfo.HitDistance = hitDistance;
    hitInfo.ObjectIndex = objectIndex;

    const Sphere& closestSphere = m_ActiveScene->Spheres[objectIndex];
    //glm::vec3 hit0 = ray.Origin + ray.Direction * t0;
    glm::vec3 origin = ray.Origin - closestSphere.Pos;
    hitInfo.WorldPos = origin + ray.Direction * hitDistance;

    hitInfo.WorldNormal = glm::normalize(hitInfo.WorldPos);

    hitInfo.WorldPos += closestSphere.Pos;

    return hitInfo;
}

RayRenderer::HitInfo RayRenderer::Miss(const Math::Ray& ray) {
    RayRenderer::HitInfo hitInfo;
    hitInfo.HitDistance = -1.0f;
    return hitInfo;
}