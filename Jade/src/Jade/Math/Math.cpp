#include "jdpch.h"

#include "Jade/Math/Math.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

namespace Jade::Math
{
    // Decomposes a transformation matrix into its translation, rotation (in radians), and scale components.
    bool DecomposeTransform(const glm::mat4& transform, glm::vec3& outTranslation, glm::vec3& outRotation, glm::vec3& outScale)
    {
        // Note: glm::mat4 is column-major by default.
        // localMatrix[column][row] -> access element at (row, column)
        glm::mat4 localMatrix(transform);

        // If the localMatrix[3][3] is close to zero, the matrix is not invertible.
        // We cannot decompose such a matrix, thus we return false.
        if (glm::epsilonEqual(localMatrix[3][3], static_cast<float>(0), glm::epsilon<float>()))
            return false;

        // Normalize the matrix.
        localMatrix /= localMatrix[3][3];

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
        // Extract translation
        outTranslation = glm::vec3(localMatrix[3]);
        // Clear translation
        localMatrix[3] = glm::vec4(0, 0, 0, localMatrix[3].w);

        glm::vec3 basis[3];

        // Copy the upper 3x3 matrix.
        for (glm::length_t i = 0; i < 3; ++i)
        {
            for (glm::length_t j = 0; j < 3; ++j)
            {
                basis[i][j] = localMatrix[i][j];
            }
        }

        // Extract the scales.
        outScale.x = glm::length(basis[0]);
        outScale.y = glm::length(basis[1]);
        outScale.z = glm::length(basis[2]);
        // Normalize the basis vectors.
        basis[0] = glm::normalize(basis[0]);
        basis[1] = glm::normalize(basis[1]);
        basis[2] = glm::normalize(basis[2]);

#if 1
        // At this point, the matrix (in basis) is orthonormal.
        // Check for a coordinate system flip.  If the determinant
        // is -1, then negate the matrix and the scaling factors.
        glm::vec3 pdum3 = glm::cross(basis[1], basis[2]); // v3Cross(row1, row2, pdum3);

        // If the determinant is -1, we need to invert one scale
        if (glm::dot(basis[0], pdum3) < 0)
        {
            for (glm::length_t i = 0; i < 3; i++)
            {
                outScale[i] *= static_cast<float>(-1);
                basis[i] *= static_cast<float>(-1);
            }
        }
#endif

        // Now, extract the rotations.
        outRotation.y = asin(-basis[0][2]); // Pitch

        // remaining angles depend on cos(y)
        // Avoid gimbal lock
        if (cos(outRotation.y) != 0)
        {
            outRotation.x = atan2(basis[1][2], basis[2][2]);    // Roll
            outRotation.z = atan2(basis[0][1], basis[0][0]);    // Yaw
        }
        else
        {
            outRotation.x = atan2(-basis[2][0], basis[1][1]);   // Roll
            outRotation.z = 0;                                  // Yaw
        }

        return true;
    }
}
