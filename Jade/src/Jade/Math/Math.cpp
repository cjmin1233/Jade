#include "jdpch.h"

#include "Jade/Math/Math.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

namespace Jade::Math
{
    bool DecomposeTransform(const glm::mat4& transform, glm::vec3& outTranslation, glm::vec3& outRotation, glm::vec3& outScale)
    {
        glm::mat4 localMatrix(transform);

        // Normalize the matrix.
        if (glm::epsilonEqual(localMatrix[3][3], static_cast<float>(0), glm::epsilon<float>()))
            return false;

        // First, isolate perspective.  This is the messiest.
        if (
            glm::epsilonNotEqual(localMatrix[0][3], static_cast<float>(0), glm::epsilon<float>()) ||
            glm::epsilonNotEqual(localMatrix[1][3], static_cast<float>(0), glm::epsilon<float>()) ||
            glm::epsilonNotEqual(localMatrix[2][3], static_cast<float>(0), glm::epsilon<float>())
            )
        {
            // Clear the perspective partition
            localMatrix[0][3] = localMatrix[1][3] = localMatrix[2][3] = static_cast<float>(0);
            localMatrix[3][3] = static_cast<float>(1);
        }

        // Next take care of translation (easy).
        outTranslation = glm::vec3(localMatrix[3]);
        localMatrix[3] = glm::vec4(0, 0, 0, localMatrix[3].w);  // Clear translation

        glm::vec3 row[3], pdum3;

        // Now get scale and shear.
        for (glm::length_t i = 0; i < 3; ++i)
        {
            for (glm::length_t j = 0; j < 3; ++j)
            {
                row[i][j] = localMatrix[i][j];
            }
        }

        outScale.x = glm::length(row[0]);
        row[0] = glm::detail::scale(row[0], static_cast<float>(1));
        outScale.y = glm::length(row[1]);
        row[1] = glm::detail::scale(row[1], static_cast<float>(1));
        outScale.z = glm::length(row[2]);
        row[2] = glm::detail::scale(row[2], static_cast<float>(1));

#if 0
        // At this point, the matrix (in rows[]) is orthonormal.
        // Check for a coordinate system flip.  If the determinant
        // is -1, then negate the matrix and the scaling factors.

        pdum3 = glm::cross(row[1], row[2]); // v3Cross(row1, row2, pdum3);
        if (glm::dot(row[0], pdum3) < 0)
        {
            for (glm::length_t i = 0; i < 3; i++)
            {
                outScale[i] *= static_cast<float>(-1);
                row[i] *= static_cast<float>(-1);
            }
        }
#endif

        outRotation.y = asin(-row[0][2]);

        // remaining angles depend on cos(y)
        if (cos(outRotation.y) != 0)
        {
            outRotation.x = atan2(row[1][2], row[2][2]);
            outRotation.z = atan2(row[0][1], row[0][0]);
        }
        else
        {
            outRotation.x = atan2(-row[2][0], row[1][1]);
            outRotation.z = 0;
        }

        return true;
    }
}
