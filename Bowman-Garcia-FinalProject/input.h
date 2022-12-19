#ifndef INPUT_H
#define INPUT_H

// #include "graphics_headers.h"

#include <GL/glew.h>
#include <glm/glm.hpp>
// #include <GLFW/glfw3.h>
// #include <GL/glu.h>

#include "window.h"

class Input
{
    public:
        // Accessor
        static Input* GetInstance();

        // Modifiers
        void SetWindow(Window* window);

        // Input accessors
        int GetKey(int key);
        bool GetKeyDown(int key);
        bool GetKeyReleased(int key);
        glm::vec2 GetCursorPos();

    private:
        // Constructor
        Input();

        // Static functions
        static void mouse_callback(GLFWwindow* window, double xpos, double ypos);

        // Instance
        static Input* m_instance;

        // References
        Window* m_window = nullptr;

        // Cursor variables
        double cursorX;
        double cursorY;
};

#endif /* INPUT_H */