#ifndef CAMERA_H
#define CAMERA_H

#include "graphics_headers.h"
#include "Input.h"

class Camera
{
  public:
    Camera();
    ~Camera();

    bool Initialize(int w, int h);

    void Update(double dt);

    // Modifiers
    void SetSpeed(glm::vec3 newSpeed);
    void SetViewRotation(glm::vec3 newRot);
    void SetViewPosition(glm::vec3 newPos);

    glm::mat4 GetProjection();
    glm::mat4 GetView();
  
  private:
    glm::mat4 projection;
    glm::mat4 view;
    glm::vec3 velocity = glm::vec3(0.0f);

    // Movement related variables
    glm::vec3 viewFront = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 viewUp = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 viewPos = glm::vec3(0.0f, 12.0f, 20.0f);
    // glm::vec3 viewPos = glm::vec3(0.0f);
    glm::vec3 viewAng = glm::vec3(-35.0f, -90.0f, 0.0f);
    // glm::vec3 viewAng = glm::vec3(0.0f, );

    // Mouse related variables
    double lastX;
    double lastY;
    bool mouseNotSet = true;

    // Input processing
    // void ProcessInput();
    // void ProcessMouseInput(Input* input);
    // void ProcessKeyboardInput(Input* input);

    // Internal Functions
    void CalcLookDirection();
};

#endif /* CAMERA_H */
