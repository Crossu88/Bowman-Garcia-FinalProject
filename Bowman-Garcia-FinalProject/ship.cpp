#include "ship.h"

Ship::Ship()
{

}

Ship::Ship(Mesh* mesh, Camera* camera)
{
    m_mesh = mesh;
    m_camera = camera;

    m_shipTransform = new Transform(glm::vec3(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f));
    m_meshTransform = new Transform(m_shipTransform, glm::vec3(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.1f));
    // m_cameraTransform = new Transform(m_shipTransform, glm::vec3(-5.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f));
    m_cameraTransform = new Transform(glm::vec3(0.0f, 12.0f, 20.0f), glm::vec3(-35.0f, -90.0f, 90.0f), glm::vec3(1.0f));
}

Ship::~Ship()
{

}

void Ship::Update(double deltaTime)
{
    ProcessInput();
    UpdatePhysics(deltaTime);
    UpdateMesh();
    UpdateCamera();
}

void Ship::UpdateMesh()
{
    // m_mesh->SetModel(m_shipTransform->GetMatrix());
    m_mesh->SetModel(m_meshTransform->GetMatrix());
}

void Ship::UpdateCamera()
{
    auto position = *m_cameraTransform->GetPosition();
    auto rotation = *m_cameraTransform->GetRotation();

    // printf("Update Position -- X: %f, Y: %f, Z: %f\n", position.x, position.y, position.z);
    // printf("Update Rotation -- X: %f, Y: %f, Z: %f\n", rotation.x, rotation.y, rotation.z);

    // m_camera->SetViewPosition(position);
    // m_camera->SetViewRotation(rotation);
}

void Ship::UpdatePhysics(double deltaTime)
{
    if (m_direction.z != 0.0f)
        m_velocity += m_direction.z * m_shipTransform->Forward() * m_acceleration * static_cast<float>(deltaTime);

    if (m_direction.z == 0.0f && glm::length2(m_velocity) > 0.0f)
        m_velocity += glm::normalize(m_velocity) * m_acceleration * 0.5f * static_cast<float>(deltaTime);

    auto position = *m_shipTransform->GetLocalPosition();
    m_shipTransform->Translate(m_velocity * 2.0f * static_cast<float>(deltaTime));

    // if (velLength <= 0) return;
    // printf("Position -- X: %f, Y: %f, Z: %f\n", position.x, position.y, position.z);
    // printf("Velocity -- X: %f, Y: %f, Z: %f\n", m_velocity.x, m_velocity.y, m_velocity.z);
}

void Ship::ProcessInput()
{
    auto input = Input::GetInstance();
    ProcessKeyboardInput(input);
    ProcessMouseInput(input);
}

void Ship::ProcessKeyboardInput(Input* input)
{
    glm::vec3 speed = glm::vec3(0.0f);

    if (input->GetKeyDown(GLFW_KEY_W) || input->GetKeyDown(GLFW_KEY_UP))
        speed -= glm::vec3(0.0f, 0.0f, 1.0f);
    if (input->GetKeyDown(GLFW_KEY_S) || input->GetKeyDown(GLFW_KEY_DOWN))
        speed += glm::vec3(0.0f, 0.0f, 1.0f);
    // if (input->GetKeyDown(GLFW_KEY_D) || input->GetKeyDown(GLFW_KEY_RIGHT))
    //     speed -= glm::vec3(1.0f, 0.0f, 0.0f);
    // if (input->GetKeyDown(GLFW_KEY_A) || input->GetKeyDown(GLFW_KEY_LEFT))
    //     speed += glm::vec3(1.0f, 0.0f, 0.0f);

    m_direction = speed;
}

void Ship::ProcessMouseInput(Input* input)
{
	auto cursorPos = input->GetCursorPos();
	
	double xpos = cursorPos.x;
	double ypos = cursorPos.y;

	if (mouseNotSet)
	{
		lastX = xpos;
		lastY = ypos;
		mouseNotSet = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

    // m_shipTransform->Rotate(glm::vec3(0.0f, xoffset, 0.0f));
    m_shipTransform->Rotate(glm::vec3(yoffset, xoffset, 0.0f));
    // m_shipTransform->SetLocalRotation(glm::vec3(yoffset, xoffset, 0.0f));

    auto viewAng = *m_shipTransform->GetLocalRotation();

	if (viewAng.x > 89.0f)
		m_shipTransform->SetLocalRotation(glm::vec3(89.0f, viewAng.y, viewAng.z));

	if (viewAng.x < -89.0f)
		m_shipTransform->SetLocalRotation(glm::vec3(-89.0f, viewAng.y, viewAng.z));
}