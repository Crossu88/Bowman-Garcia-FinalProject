#include "transform.h"

Transform::Transform(Transform* parent, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
{
    this->m_localPosition = position;
    this->m_localRotation = rotation;
    this->m_localScale = scale;

    if (parent != nullptr)
        SetParent(parent);

    UpdateTransform();
}
Transform::Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) :Transform(nullptr, position, rotation, scale) { }
Transform::Transform() : Transform(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f)) {};

Transform::~Transform() { }

// Public

// Accessors
Transform* Transform::GetParent() { return m_parent; }
glm::mat4 Transform::GetMatrix() { return CalculateTransformMatrix(); }
glm::vec3* Transform::GetLocalPosition() { return &m_localPosition; }
glm::vec3* Transform::GetLocalRotation() { return &m_localRotation; }
glm::vec3* Transform::GetLocalScale() { return &m_localScale; }
glm::vec3* Transform::GetPosition() { return &m_position; }
glm::vec3* Transform::GetRotation() { return &m_rotation; }
glm::vec3* Transform::GetScale() { return &m_scale; }
glm::vec3 Transform::Forward() { return m_forward; }
glm::vec3 Transform::Right() { return m_right; }
glm::vec3 Transform::Up() { return m_forward; }

// Modifiers
void Transform::SetParent(Transform* parent)
{
    if (parent == nullptr) return;

    if (m_parent != nullptr)
        m_parent->RemoveChild(this);

    m_parent = parent;

    m_parent->AddChild(this);
}
void Transform::SetLocalPosition(glm::vec3 newPosition) 
{ 
    m_localPosition = newPosition; 
    UpdateTransform();
}
void Transform::SetLocalRotation(glm::vec3 newAngle) 
{ 
    m_localRotation = newAngle; 
    UpdateTransform();
}
void Transform::SetLocalScale(glm::vec3 newScale) 
{ 
    m_localPosition = newScale; 
    UpdateTransform();
}

// Operations
void Transform::Translate(glm::vec3 translation) 
{ 
    m_localPosition += translation;
    UpdateTransform();
}

void Transform::LocalTranslate(glm::vec3 translation)
{
    UpdateTransform();
}

void Transform::Rotate(glm::vec3 rotation) 
{ 
    m_localRotation += rotation;
    UpdateTransform();
}


// Private

void Transform::UpdateTransform()
{
    m_transformMatrix = CalculateTransformMatrix();
    
    glm::vec3 scale;
    glm::quat rotation;
    glm::vec3 translation;
    glm::vec3 skew;
    glm::vec4 perspective;

    glm::decompose(m_transformMatrix, scale, rotation, translation, skew, perspective);

    auto rotRad = glm::eulerAngles(rotation);
    auto rotDeg = glm::degrees(rotRad);

    m_position = translation;
    m_rotation = rotDeg;
    m_scale = scale;

    m_forward.x = cos(rotRad.y) * cos(rotRad.x);
    m_forward.y = sin(rotRad.x);
    m_forward.z = sin(rotRad.y) * cos(rotRad.x);

    // m_forward.x = cos(m_rotation.x) * sin(m_rotation.y);
    // m_forward.y = -sin(m_rotation.y);
    // m_forward.z = cos(m_rotation.x) * cos(m_rotation.y);

    // m_up.x = sin(m_rotation.x) * cos(m_rotation.y);
    // m_up.y = cos(m_rotation.x);
    // m_up.z = cos(m_rotation.x) * cos(m_rotation.y);

    // m_right.x = cos(m_rotation.y);
    // m_right.y = 0;
    // m_right.z = -sin(m_rotation.y);

    // m_up = glm::cross(m_forward, m_right);

    for (auto child : m_children)
        child->UpdateTransform();
}

glm::mat4 Transform::ConstructLocalMatrix()
{
    glm::mat4 position = glm::translate(m_localPosition);

    glm::mat4 rotX = glm::rotate(glm::radians(m_localRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 rotY = glm::rotate(glm::radians(m_localRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 rotZ = glm::rotate(glm::radians(m_localRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 rotation = rotX * rotY * rotZ;

    glm::mat4 scale = glm::scale(m_localScale);

    return position * rotation * scale;
}

glm::mat4 Transform::CalculateTransformMatrix()
{
    if (m_parent == nullptr)
        return ConstructLocalMatrix();

    return m_parent->CalculateTransformMatrix() * ConstructLocalMatrix();
}

void Transform::AddChild(Transform* child)
{
    m_children.push_back(child);
}

void Transform::RemoveChild(Transform* child)
{
    m_children.erase(std::remove(m_children.begin(), m_children.end(), child), m_children.end());
}