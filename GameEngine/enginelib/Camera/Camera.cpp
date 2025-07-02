#include <Camera/Camera.h>
#include <ftspch.h>

namespace fts
{

    glm::quat Camera::GetOrientation()
    {

        // return glm::quat(glm::radians(glm::vec3(-rotation.x, rotation.y, 0.0f)));
        return glm::quat(glm::vec3(-m_Rotation.x /*pitch*/, -m_Rotation.y /*yaw*/, 0.0f));
    }

    const glm::vec3& Camera::GetUp()
    {
        m_CameraUp = (glm::vec3)glm::rotate(GetOrientation(), m_WorldUp);
        return m_CameraUp;
    }

    const glm::vec3& Camera::GetRight()
    {
        m_CameraRight = (glm::vec3)glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
        return m_CameraRight;
    }

    const glm::vec3& Camera::GetForward()
    {
        m_CameraFront = (glm::vec3)glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
        return m_CameraFront;
    }

} // namespace fts
