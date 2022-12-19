#ifndef ENGINE_H
#define ENGINE_H


#include <assert.h>
#include "window.h"
#include "graphics.h"



class Engine
{
  public:
    Engine(const char*  name, int width, int height);
    
    ~Engine();
    bool Initialize();
    void Run();
    void ProcessInput();
    // int GetKey(int key);
    // bool GetKeyDown(int key);
    // bool GetKeyReleased(int key);
    double getDT();
    long long GetCurrentTimeMillis();
    void Display(GLFWwindow*, double);
  
  private:
    // Window related variables
    Window *m_window;    
    const char* m_WINDOW_NAME;
    int m_WINDOW_WIDTH;
    int m_WINDOW_HEIGHT;
    bool m_FULLSCREEN;

    // Mouse Position Variables
    double mouseX;
    double mouseY;

    // Time variables
    double lastFrame = 0.0;

    Graphics *m_graphics;
    bool m_running;
};

#endif // ENGINE_H
