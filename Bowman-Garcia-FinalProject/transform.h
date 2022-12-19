#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <vector>
#include <algorithm>
#include "graphics_headers.h"

class Transform
{
    public:
        // Constructor
        Transform(Transform* parent, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
        Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
        Transform();

        // Destructor
        ~Transform();

        // Data accessors
        Transform* GetParent();
        glm::mat4 GetMatrix();
        glm::vec3* GetLocalPosition();
        glm::vec3* GetLocalRotation();
        glm::vec3* GetLocalScale();
        glm::vec3* GetPosition();
        glm::vec3* GetRotation();
        glm::vec3* GetScale();
        glm::vec3 Forward();
        glm::vec3 Right();
        glm::vec3 Up();

        // Data modifiers
        void SetParent(Transform* parent);
        void SetLocalPosition(glm::vec3 newPosition);
        void SetLocalRotation(glm::vec3 newAngle);
        void SetLocalScale(glm::vec3 newScale);

        // Operations
        void Translate(glm::vec3 translation);
        void LocalTranslate(glm::vec3 translation);
        void Rotate(glm::vec3 rotation);

    private:
        // Transform hierarchy
        Transform* m_parent;
        std::vector<Transform*> m_children;

        // Transformation matrix
        glm::mat4 m_transformMatrix;

        // Local transformation data
        glm::vec3 m_localPosition;
        glm::vec3 m_localRotation;
        glm::vec3 m_localScale;

        // Absolute transformation data
        glm::vec3 m_position;
        glm::vec3 m_rotation;
        glm::vec3 m_scale;

        // Relative directions
        glm::vec3 m_forward;
        glm::vec3 m_right;
        glm::vec3 m_up;

        // Internal functions
        void UpdateTransform();
        glm::mat4 ConstructLocalMatrix();
        glm::mat4 CalculateTransformMatrix();
        void AddChild(Transform* child);
        void RemoveChild(Transform* child);
};

#endif /* TRANSFORM_H */