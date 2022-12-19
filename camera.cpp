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
	CalcLookDirection();

	viewPos += velocity * glm::vec3(dt);
	view = glm::lookAt(viewPos, viewPos + viewFront, viewUp);
}

void Camera::SetViewRotation(glm::vec3 rotation)
{
	// printf("Setting Rotation -- X: %f, Y: %f, Z: %f\n", rotation.x, rotation.y, rotation.z);
	viewAng = rotation;
    // printf("Internal Rotation -- X: %f, Y: %f, Z: %f\n", viewAng.x, viewAng.y, viewAng.z);
}

void Camera::SetViewPosition(glm::vec3 position)
{
	// printf("Setting Position -- X: %f, Y: %f, Z: %f\n", position.x, position.y, position.z);
	viewPos = position;
    // printf("Internal Position -- X: %f, Y: %f, Z: %f\n", viewPos.x, viewPos.y, viewPos.z);
}

void Camera::SetSpeed(glm::vec3 speedVec)
{
	auto newSpeed = glm::vec3(0.0f);
	newSpeed += speedVec.x * glm::normalize(glm::cross(viewFront, viewUp));
	newSpeed += speedVec.y * viewUp;
	newSpeed += speedVec.z * viewFront;
	velocity = newSpeed;
}

void Camera::CalcLookDirection()
{
	glm::vec3 lookDirection;

    lookDirection.x = cos(glm::radians(viewAng.y)) * cos(glm::radians(viewAng.x));
    lookDirection.y = sin(glm::radians(viewAng.x));
    lookDirection.z = sin(glm::radians(viewAng.y)) * cos(glm::radians(viewAng.x));

    viewFront = glm::normalize(lookDirection);

	view = glm::lookAt(viewPos, viewPos + viewFront, viewUp);
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

	viewAng += glm::vec3(yoffset, xoffset, 0.0f);

	if (viewAng.x > 89.0f)
		viewAng.x = 89.0f;

	if (viewAng.x < -89.0f)
		viewAng.x = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(viewAng.y)) * cos(glm::radians(viewAng.x));
    direction.y = sin(glm::radians(viewAng.x));
    direction.z = sin(glm::radians(viewAng.y)) * cos(glm::radians(viewAng.x));
    viewFront = glm::normalize(direction);	glm::vec3 viewDir;
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


