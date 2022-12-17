#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>
#include <stack>
using namespace std;

#include "graphics_headers.h"
#include "camera.h"
#include "shader.h"
#include "object.h"
#include "sphere.h"
#include "mesh.h"

#define numVBOs 2;
#define numIBs 2;


class Graphics
{
  public:
    Graphics();
    ~Graphics();
    bool Initialize(int width, int height);
    void Update(double dt);
    void Render();

    void CreateScene();
    void AnimateScene(double time);

    Camera* getCamera() { return m_camera; }

    glm::vec3 CalculateOrbitPos(double time, double speed, glm::vec3 offset);

  private:
    std::string ErrorString(GLenum error);

    bool collectShPrLocs();

    stack<glm::mat4> modelStack;

    Camera *m_camera;
    Shader *m_shader;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;
    GLint m_positionAttrib;
    GLint m_colorAttrib;
    GLint m_tcAttrib;
    GLint m_hasTexture;


    // Celestial Models
    Sphere* m_sun;
    Sphere* m_planet;
    Sphere* m_moon;
    Mesh* m_ship;

    glm::vec3 m_systemOrigin = glm::vec3(0.0f);

    stack<glm::mat4> m_modelStack;
};

#endif /* GRAPHICS_H */