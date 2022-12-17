#include "transform.h"

glm::mat4 Transform::GetMatrix()
{
    return m_transformMatrix;
}

glm::vec3 Transform::GetLocalPosition()
{
    return m_localPosition;
}

glm::vec3 Transform::GetLocalAngle()
{
    return m_localAngle;
}

glm::vec3 Transform::GetLocalScale()
{
    return m_localScale;
}

void Transform::SetMatrix(glm::mat4 newMatrix)
{
    m_transformMatrix = newMatrix;
}

void Transform::SetLocalPosition(glm::vec3 newPosition)
{
    m_localPosition = newPosition;
}

void Transform::SetLocalAngle(glm::vec3 newAngle)
{
    m_localAngle = newAngle;
}

void Transform::SetLocalScale(glm::vec3 newScale)
{
    m_localPosition = newScale;
}