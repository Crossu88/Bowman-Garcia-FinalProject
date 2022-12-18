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
    Sphere* m_mercury;
    Sphere* m_venus;
    Sphere* m_planet;
    Sphere* m_mars;
    Sphere* m_jupiter;
    Sphere* m_saturn;
    Sphere* m_uranus;
    Sphere* m_neptune;

    Sphere* m_moon;
    Sphere* m_deimos;
    Sphere* m_phobos;
    Sphere* m_jmoon1;
    Sphere* m_jmoon2;
    Sphere* m_jmoon3;
    Sphere* m_jmoon4;
    Sphere* m_jmoon5;
    Sphere* m_smoon1;
    Sphere* m_smoon2;
    Sphere* m_smoon3;
    Sphere* m_smoon4;
    Sphere* m_smoon5;
    Sphere* m_umoon1;
    Sphere* m_umoon2;
    Sphere* m_umoon3;
    Sphere* m_umoon4;
    Sphere* m_umoon5;
    Sphere* m_nmoon1;
    Sphere* m_nmoon2;
    Sphere* m_nmoon3;
    Sphere* m_nmoon4;
    Sphere* m_nmoon5;

    Sphere* m_halley;

    Mesh* m_ship;

    glm::vec3 m_systemOrigin = glm::vec3(0.0f);

    stack<glm::mat4> m_modelStack;
};

#endif /* GRAPHICS_H */
