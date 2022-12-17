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
	viewPos += speed * glm::vec3(dt);
	view = glm::lookAt(viewPos, viewPos + viewFront, viewUp);
}

void Camera::SetSpeed(glm::vec3 speedVec)
{
	auto newSpeed = glm::vec3(0.0f);
	newSpeed += speedVec.x * glm::normalize(glm::cross(viewFront, viewUp));
	newSpeed += speedVec.y * viewUp;
	newSpeed += speedVec.z * viewFront;
	speed = newSpeed;
}

void Camera::UpdateCamera(double xpos, double ypos)
{
	if (!mouseSet)
	{
		lastX = xpos;
		lastY = ypos;
		mouseSet = true;
	}

	float xdiff = xpos - lastX;
	float ydiff = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f;
	xdiff *= sensitivity;
	ydiff *= sensitivity;

	yaw += xdiff;
	pitch += ydiff;

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

glm::mat4 Camera::GetProjection()
{
	return projection;
}

glm::mat4 Camera::GetView()
{
	return view;
}


