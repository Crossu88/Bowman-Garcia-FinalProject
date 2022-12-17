
#include "engine.h"
#include "glm/ext.hpp"

Engine::Engine(const char* name, int width, int height)
{
  m_WINDOW_NAME = name;
  m_WINDOW_WIDTH = width;
  m_WINDOW_HEIGHT = height;
}


Engine::~Engine()
{
  delete m_window;
  delete m_graphics;
  m_window = NULL;
  m_graphics = NULL;
}


bool Engine::Initialize()
{
  // Start a window
  m_window = new Window(m_WINDOW_NAME, &m_WINDOW_WIDTH, &m_WINDOW_HEIGHT);
  if(!m_window->Initialize())
  {
    printf("The window failed to initialize.\n");
    return false;
  }

  // Locks the mouse in the window
  glfwSetInputMode(m_window->getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // Start the graphics
  m_graphics = new Graphics();
  if(!m_graphics->Initialize(m_WINDOW_WIDTH, m_WINDOW_HEIGHT))
  {
    printf("The graphics failed to initialize.\n");
    return false;
  }
 
  // No errors
  return true;
}


void Engine::Run()
{
  m_running = true;

  while (!glfwWindowShouldClose(m_window->getWindow()))
  {
      ProcessInput();
      Display(m_window->getWindow(), glfwGetTime());
      glfwPollEvents();
  }
  m_running = false;

}

void Engine::ProcessInput()
{
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_window->getWindow(), true);

    double lastMouseY = mouseY;
    double lastMouseX = mouseX;
    double sensitivity = 0.01;
    glfwGetCursorPos(m_window->getWindow(), &mouseX, &mouseY);

    glm::vec3 speed = glm::vec3(0.0f);
    double xDiff = (mouseX - lastMouseX) * sensitivity;
    double yDiff = (mouseY - lastMouseY) * sensitivity;

    if (GetKeyDown(GLFW_KEY_W) || GetKeyDown(GLFW_KEY_UP))
        speed -= glm::vec3(0.0f, 0.0f, 10.0f);
    if (GetKeyDown(GLFW_KEY_S) || GetKeyDown(GLFW_KEY_DOWN))
        speed += glm::vec3(0.0f, 0.0f, 10.0f);
    if (GetKeyDown(GLFW_KEY_A) || GetKeyDown(GLFW_KEY_LEFT))
        speed += glm::vec3(10.0f, 0.0f, 0.0f);
    if (GetKeyDown(GLFW_KEY_D) || GetKeyDown(GLFW_KEY_RIGHT))
        speed -= glm::vec3(10.0f, 0.0f, 0.0f);

    m_graphics->getCamera()->SetSpeed(speed);
    m_graphics->getCamera()->UpdateCamera(mouseX, mouseY);
}

int Engine::GetKey(int key) 
{
    return glfwGetKey(m_window->getWindow(), key);
}

bool Engine::GetKeyDown(int key)
{
    return GetKey(key) == GLFW_PRESS;
}

bool Engine::GetKeyReleased(int key)
{
    return GetKey(key) == GLFW_RELEASE;
}

double Engine::getDT()
{
  return lastFrame - glfwGetTime();
}

long long Engine::GetCurrentTimeMillis()
{
    return (long long) glfwGetTime();
}

void Engine::Display(GLFWwindow* window, double time) {

    m_graphics->Render();

    m_window->Swap();

    // m_graphics->Update(time);
    m_graphics->Update(getDT());

    lastFrame = glfwGetTime();
}
