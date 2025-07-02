#ifndef CAMERA_H
#define CAMERA_H

#include <Utils/GLM_Header.h>

namespace fts
{

    enum class CameraType
    {
        Orthographic,
        Perspective,
        FPS,
        ThirdPersCamera,
        Editor
    };

    class Camera
    {
    public:
        Camera()          = default;
        virtual ~Camera() = default;

        glm::mat4 GetProjectionMatrix() const
        {
            return m_ProjectionMatrix;
        }
        glm::mat4 GetViewMatrix() const
        {
            return m_ViewMatrix;
        }
        glm::mat4 GetViewProjectionMatrix() const
        {
            return m_ViewProjectionMatrix;
        }

        const glm::vec3& GetPosition() const
        {
            return m_Position;
        }
        void SetPosition(const glm::vec3 pos)
        {
            m_Position = pos;
            RecalculateViewProjection();
        }
        glm::vec3 GetRotation() const
        {
            return m_Rotation;
        }
        void SetRotation(glm::vec3 rotation)
        {
            m_Rotation = rotation;
            RecalculateViewProjection();
        }

        glm::quat GetOrientation();
        const glm::vec3& GetUp();
        const glm::vec3& GetRight();
        const glm::vec3& GetForward();

    protected:
        glm::mat4 m_ProjectionMatrix     = glm::mat4(1.0f);
        glm::mat4 m_ViewMatrix           = glm::mat4(1.0f);
        glm::mat4 m_ViewProjectionMatrix = glm::mat4(1.0f);

        glm::vec3 m_Rotation    = {0.0f, 0.0f, 0.0f};
        glm::vec3 m_Position    = {0.0f, 0.0f, -5.0f};
        glm::quat m_Orientation = glm::identity<glm::quat>();

        glm::vec3 m_CameraFront = glm::vec3(0.0f, 0.0f, -1.0f); // forward
        glm::vec3 m_CameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 m_CameraRight = glm::vec3(1.0f, 0.0f, 0.0f);

        const glm::vec3 m_WorldUp{0.0f, 1.0f, 0.0f};

    protected:
        virtual void RecalculateViewProjection() = 0;
    };

} // namespace fts

#endif // CAMERA_H
