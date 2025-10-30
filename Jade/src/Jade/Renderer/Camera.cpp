#include "jdpch.h"

#include "Jade/Renderer/Camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Jade
{
    Camera::Camera() 
        : m_ViewMatrix(1.0f)
        , m_ProjectionMatrix(1.0f)
        , m_ViewProjectionMatrix(1.0f)
        , m_Position()
        , m_Rotation()
    {
    }

    void Camera::UpdateViewMatrix()
    {
        //// 쿼터니언을 3x3 회전 행렬로 변환
        //const float w = m_Rotation.w;
        //const float x = m_Rotation.x;
        //const float y = m_Rotation.y;
        //const float z = m_Rotation.z;

        //glm::mat3 rot3x3 = glm::mat3(
        //    1 - 2 * (y * y + z * z),     2 * (x * y - z * w),         2 * (x * z + y * w),
        //    2 * (x * y + z * w),         1 - 2 * (x * x + z * z),     2 * (y * z - x * w),
        //    2 * (x * z - y * w),         2 * (y * z + x * w),         1 - 2 * (x * x + y * y)
        //);

        //// 4x4 변환 행렬 생성
        //glm::mat4 rotation = glm::mat4(1.0f);
        //rotation[0][0] = rot3x3[0][0]; rotation[0][1] = rot3x3[0][1]; rotation[0][2] = rot3x3[0][2];
        //rotation[1][0] = rot3x3[1][0]; rotation[1][1] = rot3x3[1][1]; rotation[1][2] = rot3x3[1][2];
        //rotation[2][0] = rot3x3[2][0]; rotation[2][1] = rot3x3[2][1]; rotation[2][2] = rot3x3[2][2];

        glm::mat4 translation = glm::translate(glm::mat4(1.0f), m_Position);
        glm::mat4 transform = translation * glm::mat4_cast(m_Rotation);
        m_ViewMatrix = glm::inverse(transform);
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }
}