#include "camera.h"

Camera::Camera()
{

}

Camera::~Camera()
{

}

bool Camera::Initialize(int w, int h)
{
	projection = glm::perspective(glm::radians(45.f), //the FoV typically 90 degrees is good which is what this is set to
		float(w) / float(h), //Aspect Ratio, so Circles stay Circular
		0.01f, //Distance to the near plane, normally a small value like this
		100.0f); //Distance to the far plane, 

	return true;
}

void Camera::Update(double dt)
{
	ProcessInput();

	viewPos += m_speed * glm::vec3(dt);
	view = glm::lookAt(viewPos, viewPos + viewFront, viewUp);
}

void Camera::SetSpeed(glm::vec3 speedVec)
{
	auto newSpeed = glm::vec3(0.0f);
	newSpeed += speedVec.x * glm::normalize(glm::cross(viewFront, viewUp));
	newSpeed += speedVec.y * viewUp;
	newSpeed += speedVec.z * viewFront;
	m_speed = newSpeed;
}

void Camera::ProcessInput()
{
	auto input = Input::GetInstance();

	ProcessMouseInput(input);
	ProcessKeyboardInput(input);
}

void Camera::ProcessMouseInput(Input* input)
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

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;

	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 lookDirection;
    lookDirection.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    lookDirection.y = sin(glm::radians(pitch));
    lookDirection.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    viewFront = glm::normalize(lookDirection);
}

void Camera::ProcessKeyboardInput(Input* input)
{
    glm::vec3 speed = glm::vec3(0.0f);

    if (input->GetKeyDown(GLFW_KEY_W) || input->GetKeyDown(GLFW_KEY_UP))
        speed -= glm::vec3(0.0f, 0.0f, 10.0f);
    if (input->GetKeyDown(GLFW_KEY_S) || input->GetKeyDown(GLFW_KEY_DOWN))
        speed += glm::vec3(0.0f, 0.0f, 10.0f);
    if (input->GetKeyDown(GLFW_KEY_A) || input->GetKeyDown(GLFW_KEY_LEFT))
        speed += glm::vec3(10.0f, 0.0f, 0.0f);
    if (input->GetKeyDown(GLFW_KEY_D) || input->GetKeyDown(GLFW_KEY_RIGHT))
        speed -= glm::vec3(10.0f, 0.0f, 0.0f);

	SetSpeed(speed);
}

// Getters

glm::mat4 Camera::GetProjection()
{
	return projection;
}

glm::mat4 Camera::GetView()
{
	return view;
}


