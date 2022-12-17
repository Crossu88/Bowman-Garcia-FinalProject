#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <vector>
#include "graphics_headers.h"

class Transform
{
    public:
        Transform();
        ~Transform();

        glm::mat4 GetMatrix();
        glm::vec3 GetLocalPosition();
        glm::vec3 GetLocalAngle();
        glm::vec3 GetLocalScale();

        void SetMatrix(glm::mat4 newMatrix);
        void SetLocalPosition(glm::vec3 newPosition);
        void SetLocalAngle(glm::vec3 newAngle);
        void SetLocalScale(glm::vec3 newScale);

    private:
        glm::mat4 m_transformMatrix;
        glm::vec3 m_localPosition;
        glm::vec3 m_localAngle;
        glm::vec3 m_localScale;
};

#endif /* TRANSFORM_H */