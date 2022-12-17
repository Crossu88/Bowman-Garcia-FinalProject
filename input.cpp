#include "input.h"

// Static instance
Input* Input::m_instance = nullptr;;

// Public Static

Input* Input::GetInstance()
{
    if (m_instance == nullptr)
        m_instance = new Input();

    return m_instance;
}

void Input::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (m_instance == nullptr) return;

    m_instance->cursorX = xpos;
    m_instance->cursorY = ypos;
}

// Public

void Input::SetWindow(Window* window)
{
    // Set window to get input from
    m_window = window;

    // Initialize mouse input
    glfwSetCursorPosCallback(m_window->getWindow(), mouse_callback);
    glfwGetCursorPos(m_window->getWindow(), &cursorX, &cursorY);
}


// Accessors

int Input::GetKey(int key) 
{
    return glfwGetKey(m_window->getWindow(), key);
}

bool Input::GetKeyDown(int key)
{
    return GetKey(key) == GLFW_PRESS;
}

bool Input::GetKeyReleased(int key)
{
    return GetKey(key) == GLFW_RELEASE;
}

glm::vec2 Input::GetCursorPos()
{
    return glm::vec2(cursorX, cursorY);
}

// Private

Input::Input() {}